//---------------------------------------------------------------------------
#pragma hdrstop

#include "EftposCadmus.h"
#include "DropDown.h"
#include "DropDownVar.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#include "DeviceRealTerminal.h"
#include "MMMessageBox.h"
#include "ManagerVariable.h"
#include "MM_DBCore.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//                             TEftPosANZ
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TEftPosCadmus::TEftPosCadmus()
{
	EFTPOSType = eTEftPosCadmus;
// Create a Comm Port here.
	Port = new TComPort(NULL);
	Port->BaudRate = br9600;
	Port->DataBits = dbEight;
	Port->Parity->Bits = prNone;
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
	MerchantNumber = 1;
	ChequeAccountMaxLength = 10;
	ChequeBranchMaxLength = 8;
	ChequeSerialMaxLength = 6;
}


TEftPosCadmus::~TEftPosCadmus()
{
	ClosePort();
	delete Port;
}

void TEftPosCadmus::Initialise()
{
	TEftPos::Initialise();

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	MerchantNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposMerchantNumber,1);
	PortNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposSerialPort,-1);
	DBTransaction.Commit();
	try
	{
		if(TGlobalSettings::Instance().EnableEftPosCadmus)
		{
			if(OpenPort(PortNumber))
			{
				Enabled = true;
			}
			else
			{
				MessageBox("Unable To Open the Cadmus Eftpos Serial Port.\r Check this port is set correctly and is not\r in use by another program or printer.", "Error",
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
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Initialise TEftPosCadmus" + E.Message);
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Initialise TEftPosCadmus"  + E.Message);
		Enabled = false;
	}
}

void TEftPosCadmus::DoControlPannel()
{
	if(!Enabled) return;
	try
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		MerchantNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposMerchantNumber,1);
		PortNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposSerialPort,-1);
		DBTransaction.Commit();

		std::auto_ptr<TfrmDropDownFunc>(frmDropDown)(TfrmDropDownFunc::Create<TfrmDropDownFunc>(Screen->ActiveForm));
		frmDropDown->AddButton("EFTPOS Logon",&DoLogon);
		frmDropDown->AddButton("Settlement  Enquiry",&DoSettlementEnquiry);
		frmDropDown->AddButton("Settlement  CutOver",&DoSettlementCutover);
		frmDropDown->AddButton("Reprint Receipt",&ReprintReceipt);
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

void TEftPosCadmus::ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase,Currency AmtCash,
									 UnicodeString TxnRef,ePANSource PANSource,
									 UnicodeString CardSwipe,int ExpiryMonth, int ExpiryYear)
{
	if(Enabled)
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		MerchantNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposMerchantNumber,1);
		PortNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposSerialPort,-1);
		DBTransaction.Commit();

		long PrimaryAmount = 0;
		long SecondaryAmount = 0;
		AnsiString Packet;
		switch (TxnType)
		{
			case TransactionType_PURCHASE_PLUS_CASH :
			case TransactionType_PURCHASE :
			switch(PANSource)
			{
				case PANSource_None:
				case PANSource_CardReader:
					Packet = BuildPurchasePacket(AmtPurchase,AmtCash,CardSwipe);
				break;
				case PANSource_Manual:
					Packet = BuildManualPurchasePacket(AmtPurchase,CardSwipe,ExpiryMonth,ExpiryYear);
				break;
			}
			break;
			case TransactionType_CASH_ADVANCE :
				Packet = BuildCashOutPacket(AmtCash,CardSwipe);
			break;
			case TransactionType_REFUND :
				Packet = BuildRefundPacket(AmtPurchase,CardSwipe);
			break;
			case TransactionType_INQUIRY :
				Packet = BuildSettlementEnquiryPacket(Now());
			case TransactionType_NONE :
			default :
			break;
		}
		SendPacket(Packet);
		TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(IntToStr(0));
		if(EftTrans)
		{
			EftTrans->Result = eManualQuery;
			EftTrans->ResultText = "Confirm EFTPOS Transaction.";
			EftTrans->EventCompleted = true;
		}
	}
}

void __fastcall TEftPosCadmus::TransactionEvent(TObject *Sender,long lUniqueID, long lResult)
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

void __fastcall TEftPosCadmus::DoSettlementCutover()
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

void __fastcall TEftPosCadmus::DoSettlementEnquiry()
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

void __fastcall TEftPosCadmus::DoLogon()
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

void __fastcall TEftPosCadmus::DoUtilityFunctions()
{
}

void __fastcall TEftPosCadmus::DoPrintLocalTotals()
{
}

void TEftPosCadmus::ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
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

void __fastcall TEftPosCadmus::ReprintReceipt()
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

AnsiString TEftPosCadmus::GetRefNumber()
{
	__int64 UniqueID = 0;
	if(Enabled)
	{
	}
	return IntToStr(UniqueID);
}

bool TEftPosCadmus::CheckChequeBranch(AnsiString Branch)
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
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to CheckChequeBranch TEftPosCadmus" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to CheckChequeBranch TEftPosCadmus"  + E.Message);
         ResultText = "Cheque Bank Branch Verification Failed";
      }
	}
   return RetVal;
}

void TEftPosCadmus::CancelTransaction(AnsiString inID)
{
	if(!Enabled) return;
	try
	{
		AnsiString Packet = BuildCancelTransactionPacket(inID);
		SendPacket(Packet);
		TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(inID);
		if(EftTrans)
		{
			EftTrans->Result = eDeclined;
			EftTrans->ResultText = "Transaction Cancelled.";
			EftTrans->EventCompleted = true;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

bool TEftPosCadmus::CheckChequeSerial(AnsiString ChequeSerial)
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
			TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Serial TEftPosCadmus" + E.Message);
			TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Serial TEftPosCadmus"  + E.Message);
			ResultText = "Cheque Serial Number Verification Failed";
		}
	}
	return RetVal;
}

bool TEftPosCadmus::CheckChequeAccount(AnsiString ChequeAccount)
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
         else if(!IsNumeric(ChequeAccount))
         {
				ResultText = "Cheque Account number must only contain numbers";
         }
         else
         {
         	RetVal = true;
         }
      }
      catch(Exception &E)
      {
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Account TEftPosCadmus" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Account TEftPosCadmus"  + E.Message);
			ResultText = "Cheque Serial Account Verification Failed";
		}
	}
	return RetVal;
}

//---------------------------------------------------------------------------

bool TEftPosCadmus::OpenPort(int PortNum)
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

void TEftPosCadmus::ClosePort()
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


void TEftPosCadmus::SendPacket(AnsiString Data)
{
	const char STX = 0x02;
	const char ETX = 0x03;
	char LRC = 0;

	if(Busy) return;
	Busy = true;

	try
	{
		Data += ETX;

		for (int i = 1; i <= Data.Length() ; i++)
		{
			LRC ^= Data[i];
		}

		Data += LRC;
		Data = AnsiString(&STX,sizeof(STX)) + Data;

		if(OpenPort(PortNumber))
		{
			TMemoryStream *Stream = new TMemoryStream;
			// Eftpos Logging
			TStringList *List = new TStringList;
			try
			{
				Stream->Position = 0;
				Stream->Write(Data.c_str(),Data.Length());
				Stream->Position = 0;
				Port->Write((unsigned char *)(Stream->Memory),Stream->Size);

				Stream->Position = 0;
				if(FileExists(ExtractFilePath(Application->ExeName)+ "CadmusEftpos.log"))
				{
					List->LoadFromFile(ExtractFilePath(Application->ExeName)+ "CadmusEftpos.log");
				}
				AnsiString LogData = AnsiString((char *)(Stream->Memory),Stream->Size);
				List->Add(LogData);
				while(List->Count > 100)
				{
					List->Delete(0);
				}
				List->SaveToFile(ExtractFilePath(Application->ExeName)+ "CadmusEftpos.log");
			}
			__finally
			{
				delete Stream;
				delete List;				
			}
			ClosePort();
		}
	}
	__finally
	{
		Busy = false;
	}
}

AnsiString TEftPosCadmus::BuildPurchasePacket(Currency Purchase, Currency CashOut, AnsiString Card)
{
	AnsiString RetVal = "PUR,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += FormatFloat("000000.00",Purchase)+ ",";
	RetVal += FormatFloat("000000.00",CashOut);
	if(Card != "")
	{
		RetVal += "," + Card;
	}
   return RetVal;
}

AnsiString TEftPosCadmus::BuildManualPurchasePacket(Currency Purchase,AnsiString Card, int Month, int Year)
{
	
	AnsiString RetVal = "MAN,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += FormatFloat("000000.00",Purchase)+ ",";
	RetVal += FormatFloat("00",Month) + FormatFloat("00",Year);
	if(Card != "")
	{
		RetVal += "," + Card;
	}
	return RetVal;
}


AnsiString TEftPosCadmus::BuildCashOutPacket(Currency CashOut, AnsiString Card)
{
	
	AnsiString RetVal = "CSH,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += FormatFloat("000000.00",CashOut);
	if(Card != "")
	{
		RetVal += "," + Card;
	}
	return RetVal;
}

AnsiString TEftPosCadmus::BuildCancelTransactionPacket(AnsiString inID)
{
	
	AnsiString RetVal = "CAN,";
	RetVal += IntToStr(MerchantNumber);
	return RetVal;
}

AnsiString TEftPosCadmus::BuildRefundPacket(Currency Refund, AnsiString Card)
{
	
	AnsiString RetVal = "REF,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += FormatFloat("000000.00",Refund);
	if(Card != "")
	{
		RetVal += "," + Card;
	}
	return RetVal;
}

AnsiString TEftPosCadmus::BuildChequeAuthPacket(Currency ChequeAmount, AnsiString BankAndBranch,
																AnsiString AccountNumber,AnsiString ChequeSerialNumber)
{
	const int MaxBankAndBranch = 8;
	const int MaxAccountNumber = 10;

	if(BankAndBranch.Length() < MaxBankAndBranch)
	{
		int LengthToPack = MaxBankAndBranch - BankAndBranch.Length();
		BankAndBranch = AnsiString::StringOfChar(' ',LengthToPack) + BankAndBranch;
	}

	if(AccountNumber.Length() < MaxAccountNumber)
	{
		int LengthToPack = MaxAccountNumber - BankAndBranch.Length();
		AccountNumber = AnsiString::StringOfChar(' ',LengthToPack) + AccountNumber;
	}

	
	AnsiString RetVal = "CHQ,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += FormatFloat("000000.00",ChequeAmount) + ",";
	RetVal += BankAndBranch + ",";
	RetVal += AccountNumber + ",";
	RetVal += ChequeSerialNumber;
	return RetVal;
}

AnsiString TEftPosCadmus::BuildLogonPacket()
{
	
	AnsiString RetVal = "LOG,";
	RetVal += IntToStr(MerchantNumber);
	return RetVal;
}

AnsiString TEftPosCadmus::BuildCancelPacket()
{
	
	AnsiString RetVal = "CAN,";
	RetVal += IntToStr(MerchantNumber);
	return RetVal;
}

AnsiString TEftPosCadmus::BuildSigOkPacket(bool SigOk)
{
	
	AnsiString RetVal = "SIG,";
	if(SigOk)
	{
		RetVal += "YES";
	}
	else
	{
		RetVal += "NO";
	}
	return RetVal;
}

AnsiString TEftPosCadmus::BuildReprintReceiptPacket()
{
	
	AnsiString RetVal = "REP,";
	RetVal += IntToStr(MerchantNumber);
	return RetVal;
}

AnsiString TEftPosCadmus::BuildSettlementCutoverPacket()
{
	
	AnsiString RetVal = "SET,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += IntToStr(1);
	return RetVal;
}

AnsiString TEftPosCadmus::BuildSettlementEnquiryPacket(TDateTime Date)
{
	
	AnsiString RetVal = "SET,";
	RetVal += IntToStr(MerchantNumber) + ",";
	RetVal += IntToStr(0) + ",";
	RetVal += FormatDateTime("ddmmyyyy",Date);
	return RetVal;
}
//--------------------------------------------------------------------------
bool TEftPosCadmus::IsCashOutSupported()
{
    return true;
}
//--------------------------------------------------------------------------
