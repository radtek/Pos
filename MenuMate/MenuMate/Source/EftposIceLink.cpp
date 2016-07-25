//---------------------------------------------------------------------------
#pragma hdrstop

#include "EftposIceLink.h"
#include "DropDown.h"
#include "DropDownVar.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#include "DeviceRealTerminal.h"
#include "MMMessageBox.h"
#include "ManagerVariable.h"
#include "MM_DBCore.h"
#include "MMLogging.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//                             TEftPosANZ
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TEftPosIceLink::TEftPosIceLink()
{
	EFTPOSType = eTEftPosICELink;
// Create a Comm Port here.
	Port = new TComPort(NULL);
	Port->BaudRate = br9600;
	Port->DataBits = dbSeven;
	Port->Parity->Bits = prEven;
	Port->StopBits = sbOneStopBit;
	Port->Parity->Check = false;
	Port->Parity->Replace = false;
	Port->Buffer->InputSize = 16384;
	Port->Buffer->OutputSize = 16384;
	Port->DiscardNull = false;
	Port->OnRxChar = NULL;
	Port->Timeouts->ReadInterval = 200;
	Port->Timeouts->ReadTotalMultiplier = 200;
	Port->Timeouts->ReadTotalConstant = 300;
	CloseingPort = false;
	OpeningPort = false;
	Busy = false;
	PortNumber = -1;
	ChequeAccountMaxLength = 19;
	ChequeBranchMaxLength = 0;
	ChequeSerialMaxLength = 8;

	EnquiryEnabled = false;
}


TEftPosIceLink::~TEftPosIceLink()
{
	ClosePort();
	delete Port;
}

void TEftPosIceLink::Initialise()
{
	TEftPos::Initialise();

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	PortNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposSerialPort,-1);
	DBTransaction.Commit();
	try
	{
		if(TGlobalSettings::Instance().EnableEftPosIceLink)
		{
			if(OpenPort(PortNumber))
			{
				Enabled = true;
			}
			else
			{
				MessageBox("Unable To Open the Eftpos Serial Port.\r Check this port is set correctly and is not\r in use by another program or printer.", "Error",
				MB_OK + MB_ICONWARNING);
			}
		}
		else
		{
			Enabled = false;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Initialise TEftPosIceLink" + E.Message);
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Initialise TEftPosIceLink"  + E.Message);
		Enabled = false;
	}
}

void TEftPosIceLink::DoControlPannel()
{
	if(!Enabled) return;
	try
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		PortNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposSerialPort,-1);
		DBTransaction.Commit();

		std::auto_ptr<TfrmDropDownFunc>(frmDropDown)(TfrmDropDownFunc::Create<TfrmDropDownFunc>(Screen->ActiveForm));

		frmDropDown->AddButton("EFTPOS Logon",&DoLogon);
		frmDropDown->AddButton("Settlement ",&DoSettlementCutover);
/*		frmDropDown->AddButton("Settlement  Enquiry",&DoSettlementEnquiry);
		frmDropDown->AddButton("Reprint Receipt",&ReprintReceipt);*/
		if(frmDropDown->ShowModal() == mrOk)
		{
			frmDropDown->FunctionToCall();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"DoControlPannel Failed" + E.Message);
	}
}

void TEftPosIceLink::ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase,Currency AmtCash,
									 UnicodeString TxnRef,ePANSource PANSource,
									 UnicodeString CardSwipe,int ExpiryMonth, int ExpiryYear)
{
	if(Enabled)
   {
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		PortNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposSerialPort,-1);
		DBTransaction.Commit();

		long PrimaryAmount = 0;
		long SecondaryAmount = 0;
		AnsiString Packet;
		switch (TxnType)
		{
			case TransactionType_PURCHASE_PLUS_CASH :
			switch(PANSource)
			{
				case PANSource_None:
				case PANSource_CardReader:
					Packet = BuildPurchasePlusCashPacket(AmtPurchase,AmtCash,CardSwipe,TxnRef);
				break;
				case PANSource_Manual:
					Packet = BuildPurchasePlusCashPacket(AmtPurchase,AmtCash,"",TxnRef,CardSwipe,ExpiryMonth,ExpiryYear);
				break;
			}
			break;
			case TransactionType_PURCHASE :
			switch(PANSource)
			{
				case PANSource_None:
				case PANSource_CardReader:
					Packet = BuildPurchasePacket(AmtPurchase,CardSwipe,TxnRef);
				break;
				case PANSource_Manual:
					Packet = BuildPurchasePacket(AmtPurchase,"",TxnRef,CardSwipe,ExpiryMonth,ExpiryYear);
				break;
			}
			break;
			case TransactionType_CASH_ADVANCE :
				Packet = BuildCashOutPacket(AmtCash,CardSwipe,TxnRef);
			break;
			case TransactionType_REFUND :
			switch(PANSource)
			{
				case PANSource_None:
				case PANSource_CardReader:
					Packet = BuildRefundPacket(AmtPurchase,CardSwipe,TxnRef);
				break;
				case PANSource_Manual:
					Packet = BuildRefundPacket(AmtPurchase,"",TxnRef,CardSwipe,ExpiryMonth,ExpiryYear);
				break;
			}
			break;
			case TransactionType_INQUIRY :
				Packet = BuildSettlementEnquiryPacket(Now());
			case TransactionType_NONE :
			default :
			break;
		}
		SendPacket(Packet);
		TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnRef);
		if(EftTrans)
		{
			EftTrans->Result = eManualQuery;
			EftTrans->ResultText = "Confirm EFTPOS Transaction.";
			EftTrans->EventCompleted = true;
		}
	}
}

void __fastcall TEftPosIceLink::TransactionEvent(TObject *Sender,long lUniqueID, long lResult)
{
/*	TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(IntToStr(lUniqueID));
	if(EftTrans != NULL)
	{
		switch(lResult)
		{
			case ACCEPTED :
				EftTrans->Result = eAccepted;
				EftTrans->ResultText = "ACCEPTED";
			break;
			case DECLINED :
			case FAILED :
			case EFTPOS_UNAVAILABLE :
			case SPECIAL :
				EftTrans->Result = eDeclined;
				EftTrans->ResultText = "DECLINED";
			break;
		}
		EftTrans->EventCompleted = true;
	}*/
}

void __fastcall TEftPosIceLink::DoSettlementCutover()
{
	if(!Enabled) return;
	try
	{
		AnsiString Packet = BuildSettlementCutoverPacket();
		SendPacket(Packet);

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void __fastcall TEftPosIceLink::DoSettlementEnquiry()
{
	if(!Enabled) return;
	try
	{
		AnsiString Packet = BuildSettlementEnquiryPacket(Now());
		SendPacket(Packet);
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void __fastcall TEftPosIceLink::DoLogon()
{
	if(!Enabled) return;
	try
	{
		AnsiString Packet = BuildLogonPacket();
		SendPacket(Packet);
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void __fastcall TEftPosIceLink::DoUtilityFunctions()
{
}

void __fastcall TEftPosIceLink::DoPrintLocalTotals()
{
}

void TEftPosIceLink::ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
							 AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef)
{
	if(!Enabled) return;
	try
	{
		AnsiString Packet = BuildChequeAuthPacket(AmtPurchase,ChequeBranch,ChequeAccount,ChequeSerial);
		SendPacket(Packet);
		TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TxnRef);
		if(EftTrans)
		{
			EftTrans->Result = eManualQuery;
			EftTrans->ResultText = "Confirm Cheque Authorisation.";
			EftTrans->EventCompleted = true;			
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

//kbybd_event

void __fastcall TEftPosIceLink::ReprintReceipt()
{
	if(!Enabled) return;
	try
	{
		AnsiString Packet = BuildReprintReceiptPacket();
		SendPacket(Packet);
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

AnsiString TEftPosIceLink::GetRefNumber()
{
	return TDateTime::CurrentDateTime().FormatString("mmddhhnnss");
}

bool TEftPosIceLink::CheckChequeBranch(AnsiString Branch)
{
	bool RetVal = false;
   ResultText = "EFTPOS Disabled";
	if(Enabled)
   {
      try
      {
			if(Branch.Length() > ChequeBranchMaxLength)
			{
				ResultText = "Bank branch number must be Max of "+IntToStr(ChequeBranchMaxLength)+" digits.";
         }
         else if(!IsNumeric(Branch))
         {
				ResultText = "Bank branch number must only contain numbers";
         }
         else
         {
         	RetVal = true;
         }
      }
      catch(Exception &E)
      {
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to CheckChequeBranch TEftPosIceLink" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to CheckChequeBranch TEftPosIceLink"  + E.Message);
         ResultText = "Cheque Bank Branch Verification Failed";
      }
	}
   return RetVal;
}

bool TEftPosIceLink::CheckChequeSerial(AnsiString ChequeSerial)
{
	bool RetVal = false;
   ResultText = "EFTPOS Disabled";
	if(Enabled)
   {
      try
      {
			if(ChequeSerial.Length() != ChequeSerialMaxLength)
			{
				ResultText = "Cheque serial number incorrect. "+IntToStr(ChequeSerialMaxLength)+" digits required.";
			}
         else if(!IsNumeric(ChequeSerial))
         {
				ResultText = "Cheque serial number must only contain numbers";
         }
         else
         {
         	RetVal = true;
         }
      }
      catch(Exception &E)
      {
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Serial TEftPosIceLink" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Serial TEftPosIceLink"  + E.Message);
         ResultText = "Cheque Serial Number Verification Failed";
      }
	}
   return RetVal;
}

bool TEftPosIceLink::CheckChequeAccount(AnsiString ChequeAccount)
{
	bool RetVal = false;
   ResultText = "EFTPOS Disabled";
	if(Enabled)
   {
      try
      {
			if(ChequeAccount.Length() > ChequeAccountMaxLength)
			{
				ResultText = "Cheque Account number to long. Max "+IntToStr(ChequeAccountMaxLength)+" digits required.";
         }
         else
         {
         	RetVal = true;
         }
      }
      catch(Exception &E)
      {
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Account TEftPosIceLink" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Account TEftPosIceLink"  + E.Message);
			ResultText = "Cheque Serial Account Verification Failed";
		}
	}
	return RetVal;
}

//---------------------------------------------------------------------------

bool TEftPosIceLink::OpenPort(int PortNum)
{
	bool Success = false;
	try
	{
		if (!OpeningPort)
		{
			OpeningPort = true;
			try
			{
				if(PortNum > 0)
				{
					if(Port->Connected)
					{
						Port->Close();
					}
					Port->Port = "COM" + IntToStr(PortNum);
					Port->Open();
					Success = true;
					PortNumber = PortNum;
				}
				else
				{
					if(Port->Connected)
					{
						Port->Close();
					}
				}
			}
			__finally
			{
				OpeningPort = false;
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG, E.Message + " Port Number : " + IntToStr(PortNum));
		Success = false;
	}
	return Success;
}

void TEftPosIceLink::ClosePort()
{
	if(CloseingPort) return;
	CloseingPort = true;
	try
	{
		Port->Close();
	}
	__finally
	{
		CloseingPort = false;
	}
}


void TEftPosIceLink::SendPacket(AnsiString Data)
{
	if(Busy) return;
	Busy = true;

	try
	{
		if(OpenPort(PortNumber))
		{
			std::auto_ptr<TMemoryStream> Stream (new TMemoryStream);
			std::auto_ptr<TStringList> List (new TStringList);
			// Eftpos Logging
			Stream->Position = 0;
			Stream->Write(Data.c_str(),Data.Length());
			Stream->Position = 0;
			Port->Write((unsigned char *)(Stream->Memory),Stream->Size);

			Stream->Position = 0;
			if(FileExists(ExtractFilePath(Application->ExeName)+ "ICELinkEftpos.log"))
			{
				List->LoadFromFile(ExtractFilePath(Application->ExeName)+ "ICELinkEftpos.log");
			}
			AnsiString LogData = AnsiString((char *)(Stream->Memory),Stream->Size);
			List->Add(LogData);
			while(List->Count > 100)
			{
				List->Delete(0);
			}
			List->SaveToFile(ExtractFilePath(Application->ExeName)+ "ICELinkEftpos.log");
			ClosePort();
		}
	}
	__finally
	{
		Busy = false;
	}
}

AnsiString TEftPosIceLink::BuildPurchasePacket(Currency Purchase, AnsiString Card,AnsiString TxnRef,
															  AnsiString ManPAN, int Month, int Year)
{
	
	AnsiString RetVal = "@PU,";
	RetVal += FormatFloat("0000000",Purchase*100.0)+ ",";
	if(Card != "")
	{
		RetVal += Card.SubString(1,37)+ ",";
	}
	else
	{
		RetVal += ",";
	}

	if(ManPAN != "")
	{
		RetVal += ManPAN.SubString(1,19)+ ",";
		RetVal += FormatFloat("00",Year) + FormatFloat("00",Month) + ",";
	}
	else
	{
		RetVal += ","; // No Man PAN
		RetVal += ","; // No Exp Date
	}

	RetVal += TxnRef + "\r"; // POS Ref.
	return RetVal;
}

AnsiString TEftPosIceLink::BuildPurchasePlusCashPacket(Currency Purchase,Currency Cash,AnsiString Card,AnsiString TxnRef,
																		 AnsiString ManPAN, int Month, int Year)
{
	
	AnsiString RetVal = "@PC,";
	RetVal += FormatFloat("0000000",Purchase*100.0)+ ",";
	RetVal += FormatFloat("0000000",Cash*100.0)+ ",";
	if(Card != "")
	{
		RetVal += Card.SubString(1,37)+ ",";
	}
	else
	{
		RetVal += ",";
	}

	if(ManPAN != "")
	{
		RetVal += ManPAN.SubString(1,19)+ ",";
		RetVal += FormatFloat("00",Year) + FormatFloat("00",Month) + ",";
	}
	else
	{
		RetVal += ","; // No Man PAN
		RetVal += ","; // No Exp Date
	}

	RetVal += TxnRef + "\r"; // POS Ref.
	return RetVal;
}

AnsiString TEftPosIceLink::BuildCashOutPacket(Currency CashOut, AnsiString Card,AnsiString TxnRef)
{
	
	AnsiString RetVal = "@CO,";
	RetVal += FormatFloat("0000000",CashOut*100.0)+ ",";
	if(Card != "")
	{
		RetVal += Card.SubString(1,37)+ ",";
	}
	else
	{
		RetVal += ",";
	}

	RetVal += ","; // No Man PAN
	RetVal += ","; // No Exp Date
	RetVal += TxnRef + "\r"; // POS Ref.
	return RetVal;
}

AnsiString TEftPosIceLink::BuildRefundPacket(Currency Refund, AnsiString Card,AnsiString TxnRef,AnsiString ManPAN, int Month, int Year)
{
	
	AnsiString RetVal = "@RF,";
	RetVal += FormatFloat("0000000",Refund*100.0) + ",";
	if(Card != "")
	{
		RetVal += Card.SubString(1,37)+ ",";
	}
	else
	{
		RetVal += ",";
	}

	if(ManPAN != "")
	{
		RetVal += ManPAN.SubString(1,19)+ ",";
		RetVal += FormatFloat("00",Year) + FormatFloat("00",Month) + ",";
	}
	else
	{
		RetVal += ","; // No Man PAN
		RetVal += ","; // No Exp Date
	}

	RetVal += ","; // No Merchant Card Number.
	RetVal += TxnRef + "\r"; // POS Ref.
	return RetVal;
}

AnsiString TEftPosIceLink::BuildChequeAuthPacket(Currency ChequeAmount, AnsiString BankAndBranch,
																AnsiString AccountNumber,AnsiString ChequeSerialNumber)
{
	
	AnsiString RetVal = "@CQ,";
	RetVal += FormatFloat("0000000",ChequeAmount * 100.0) + ",";
	RetVal += AccountNumber + ",";
	RetVal += ChequeSerialNumber;
	RetVal += "\r";
	return RetVal;
}

AnsiString TEftPosIceLink::BuildLogonPacket()
{
	
	AnsiString RetVal = "@LG\r";
	return RetVal;
}

AnsiString TEftPosIceLink::BuildCancelPacket()
{
	AnsiString RetVal = "";
	return RetVal;
}

AnsiString TEftPosIceLink::BuildReprintReceiptPacket()
{
	AnsiString RetVal = "";
	return RetVal;
}

AnsiString TEftPosIceLink::BuildSettlementCutoverPacket()
{
	AnsiString RetVal = "@ST\r";
	return RetVal;
}

AnsiString TEftPosIceLink::BuildSettlementEnquiryPacket(TDateTime Date)
{
	AnsiString RetVal = "";
	return RetVal;
}

