//---------------------------------------------------------------------------
#pragma hdrstop

#include "EftposANZ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ENZEFTLib_OCX"

#include "Enum.h"
#include "DeviceRealTerminal.h"
#include "Comms.h"
#include "main.h"
#include "MMMessageBox.h"
#include "Processing.h"
#include <memory>
#include "printout.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//                             TEftPosANZ
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TEftPosANZ::TEftPosANZ()
{
	OcxEftPos	= NULL;
	EFTPOSType = eTEftPosANZ;
	ChequeAccountMaxLength = 10;
	ChequeBranchMaxLength = 8;
	ChequeSerialMaxLength = 6;
}


TEftPosANZ::~TEftPosANZ()
{
   if(OcxEftPos != NULL)
	{
		delete OcxEftPos;
   }
}

void TEftPosANZ::Initialise()
{
	TEftPos::Initialise();
	try
   {
      if(TGlobalSettings::Instance().EnableEftPosANZ)
      {
         Enabled = true;
         OcxEftPos = new TENZEFT(frmMain);
         OcxEftPos->OnPrintReceiptEvent = PrintReceiptEvent;
         OcxEftPos->OnTransactionEvent = TransactionEvent;
         OcxEftPos->OnChequeVerifyEvent = ChequeVerifyEvent;
         OcxEftPos->OnGetLastTransactionEvent = GetLastTransactionEvent;
         OcxEftPos->OnGetLastReceiptEvent = GetLastReceiptEvent;
         OcxEftPos->OnGetMerchantsEvent = GetMerchantsEvent;
         OcxEftPos->DoGetMerchants();
		 if(OcxEftPos->VersionMajor < 5)
		 {
            Enabled = false;
			TManagerLogs::Instance().Add("NA",EFTPOSLOG,"EFTPOS Version Mismatch Version 5 OCX Control or Higher Required");
            TManagerLogs::Instance().Add("NA",ERRORLOG,"EFTPOS Version Mismatch Version 5 OCX Control or Higher Required");
         }
      }
      else
      {
         Enabled = false;
      }
   }
   catch(Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to create TEftPosANZ" + E.Message);
      TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to create TEftPosANZ"  + E.Message);
      OcxEftPos = NULL;
   	Enabled = false;
   }
}

void TEftPosANZ::DoControlPannel()
{
	if(Enabled && OcxEftPos->Ready)
   {
		OcxEftPos->DialogX = 0;
		OcxEftPos->DialogY = 0;
      OcxEftPos->DialogType = "TouchScreen";
		OcxEftPos->DialogPosition = "Center";
		OcxEftPos->DoControlPanel();
   }
}

void TEftPosANZ::ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase,
											Currency AmtCash, UnicodeString TxnRef,
                                 ePANSource PANSource, UnicodeString CardSwipe,
                                 int ExpiryMonth, int ExpiryYear)
{
	if(Enabled)
   {
   	switch (TxnType)
      {
      	case TransactionType_PURCHASE :
      	case TransactionType_CASH_ADVANCE :
      	case TransactionType_PURCHASE_PLUS_CASH :
				OcxEftPos->TxnType = "P";
         break;
      	case TransactionType_REFUND :
	         OcxEftPos->TxnType = "R";
			break;
			case TransactionType_INQUIRY :
			case TransactionType_NONE :
         default :
	         OcxEftPos->TxnType = "";
         break;
      }
      switch(PANSource)
      {
      	case PANSource_None:
		      OcxEftPos->PanSource = " ";
         break;
      	case PANSource_Manual:
		      OcxEftPos->PanSource = "K";
	         OcxEftPos->Pan = CardSwipe;
         break;
      	case PANSource_CardReader:
		      OcxEftPos->PanSource = "S";
				OcxEftPos->Track2 = CardSwipe;
         break;
		}

		if(TxnType == TransactionType_INQUIRY) // EFTPOS Recovery.
		{
			OcxEftPos->DoGetLastTransaction();
		}
		else
		{
			OcxEftPos->EnableTip = false;
			OcxEftPos->CutReceipt = false;
			OcxEftPos->TxnRef = TxnRef;
			OcxEftPos->AmtPurchase = AmtPurchase;
			OcxEftPos->AmtCash = AmtCash;
			OcxEftPos->DateExpiry = IntToStr(ExpiryMonth) + "//" + IntToStr(ExpiryYear);
			OcxEftPos->DoTransaction();
		}
	}
}

void __fastcall TEftPosANZ::TransactionEvent(TObject *Sender)
{
	//Loop though Payment Types and set as Appropreate.
	TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(OcxEftPos->TxnRef);
	if(EftTrans != NULL)
	{
		if(OcxEftPos->ResponseCode == WideString("00") || OcxEftPos->ResponseCode == WideString("08"))
		{
			EftTrans->Result = eAccepted;
			EftTrans->ResultText = OcxEftPos->ResponseText;
			if(OcxEftPos->ResponseCode == WideString("08"))
			{
				EftTrans->SuppressReceipt = true;
			}
		}
		else
		{
			EftTrans->Result = eDeclined;
			EftTrans->ResultText = OcxEftPos->ResponseText;
		}
		EftTrans->EventCompleted = true;
	}
	else
	{
		TEftPosTransaction *FailWaitingEftTrans = EftPos->GetTransactionEventInWaiting();
		if(FailWaitingEftTrans != NULL)
		{
			FailWaitingEftTrans->Result = eFailed;
			FailWaitingEftTrans->ResultText = "Invalid Transaction Reference received in Transaction Event.";
			FailWaitingEftTrans->EventCompleted = true;
		}
	}
}

void __fastcall TEftPosANZ::GetLastTransactionEvent(TObject *Sender)
{
	if(OcxEftPos->LastTxnSuccess)
	{
		TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(OcxEftPos->TxnRef);
		if(EftTrans != NULL)
		{
			if(OcxEftPos->ResponseCode == WideString("00") || OcxEftPos->ResponseCode == WideString("08"))
			{
				EftTrans->Result = eAccepted;
				if(OcxEftPos->ResponseCode == WideString("08"))
				{
					EftTrans->SuppressReceipt = true;
				}
			}
			EftTrans->EventCompleted = true;
		}
		else
		{  // The OCX Conrtol returned a Diff Trans Ref than the one we were
			// waiting for. The EFTPOS probably never got a chance to process this
			// transaction after is was saved and therefore returned the Prevous
			// Transaction in its Ref number feild.
			TEftPosTransaction *FailWaitingEftTrans = EftPos->GetTransactionEventInWaiting();
			if(FailWaitingEftTrans != NULL)
			{
				FailWaitingEftTrans->Result = eDeclined;
				FailWaitingEftTrans->ResultText = "Transaction was not processed by PC - Eftpos.\r Transaction Ref numbers do not match.";
				FailWaitingEftTrans->EventCompleted = true;
			}
		}
	}
	else
	{
		TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(TransactionType_INQUIRY);
		if(EftTrans != NULL)
		{
			EftTrans->Result = eDeclined;
			EftTrans->ResultText = "Unable to Retrieve Last Transaction Result from PIN Pad.";
			EftTrans->EventCompleted = true;			
		}
	}
}

void __fastcall TEftPosANZ::GetLastReceiptEvent(TObject *Sender)
{
	TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(OcxEftPos->TxnRef);
	if(EftTrans != NULL)
	{
		WCHAR ReceiptType = 'C';
		PrintReceiptEvent(NULL, &ReceiptType);
		EftTrans->EventCompleted = true;
	}
}

void __fastcall TEftPosANZ::ChequeVerifyEvent(TObject *Sender)
{
	TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(OcxEftPos->ChqSerialNumber +
																				  OcxEftPos->ChqBranch +
																				  OcxEftPos->ChqAccount);
	if(EftTrans != NULL)
	{
		if(OcxEftPos->ResponseCode != WideString("Y8"))
		{
			EftTrans->Result = eAccepted;
			EftTrans->ResultText = OcxEftPos->ResponseText;
		}
		else
		{
			EftTrans->Result = eDeclined;
			EftTrans->ResultText = OcxEftPos->ResponseText;
		}
		EftTrans->EventCompleted = true;
	}
}

void __fastcall TEftPosANZ::GetMerchantsEvent(TObject *Sender)
{
}

void __fastcall TEftPosANZ::PrintReceiptEvent(TObject *Sender,
		BSTR ReceiptType)
{
	if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		TManagerLogs::Instance().Add("NA",EFTPOSLOG,"No Recipt Printer Configured for EFTPOS output.");
		TManagerLogs::Instance().Add("NA",ERRORLOG,"No Recipt Printer Configured for EFTPOS output.");
		return;
	}

	std::auto_ptr<TPrintout> Printout(new TPrintout);
	Printout->Printer	= TComms::Instance().ReceiptPrinter;
	try
	{
		if(*ReceiptType == 'C') Printout->PrintFormat->DocumentName = "Customer Receipt";
		else if(*ReceiptType == 'M') Printout->PrintFormat->DocumentName = "Merchant Receipt";
		else if(*ReceiptType == 'S') Printout->PrintFormat->DocumentName = "Settlement Receipt";
		else if(*ReceiptType == 'L') Printout->PrintFormat->DocumentName = "Logon Receipt";
		else if(*ReceiptType == 'R') Printout->PrintFormat->DocumentName = "";
		else Printout->PrintFormat->DocumentName = "";

//         Printout->PrintFormat->PrintGraphic();

		Printout->PrintFormat->Line->FontInfo.Reset();
		Printout->PrintFormat->Line->ColCount					= 1;
		Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
		Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
		Printout->PrintFormat->Line->Columns[0]->Text			= Printout->PrintFormat->DocumentName;
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->NewLine();

		LastEftPosReceipt->Clear();
		LastEftPosReceipt->Text = OcxEftPos->Receipt;
		for (int i=0; i< LastEftPosReceipt->Count; i++)
		{
			Printout->PrintFormat->Line->Columns[0]->Text	= LastEftPosReceipt->Strings[i];
			Printout->PrintFormat->AddLine();
		}

		Printout->PrintFormat->PartialCut();
		Printout->Print();
	}
	catch (Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
	TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(IntToStr(ePrintReceiptEvent));
	if(EftTrans != NULL)
	{
		EftTrans->EventCompleted = true;
	}
}


void __fastcall TEftPosANZ::DoSettlementCutover()
{
	if(Enabled)
   {
		OcxEftPos->DialogX = 0;
		OcxEftPos->DialogY = 0;
      OcxEftPos->DialogType = "TouchScreen";
		OcxEftPos->DialogPosition = "Center";
		OcxEftPos->DoSettlementCutover();
   }
}

void __fastcall TEftPosANZ::DoSettlementEnquiry()
{
	if(Enabled)
   {
		OcxEftPos->DialogX = 0;
		OcxEftPos->DialogY = 0;
      OcxEftPos->DialogType = "TouchScreen";
		OcxEftPos->DialogPosition = "Center";
		OcxEftPos->DoSettlementEnquiry();
   }
}

void TEftPosANZ::ChequeVerify( Currency AmtPurchase,
									 AnsiString ChequeSerial,
                            AnsiString ChequeBranch,
                            AnsiString ChequeAccount,
                            ePANSource PANSource,
                            AnsiString TxnRef)
{
	if(Enabled)
   {
      switch(PANSource)
      {
      	case PANSource_None:
				OcxEftPos->PanSource = " ";
         break;
      	case PANSource_Manual:
				OcxEftPos->PanSource = "K";
         break;
      	case PANSource_CardReader:
		      OcxEftPos->PanSource = "S";
         break;
      }

      OcxEftPos->AmtPurchase = AmtPurchase;
		OcxEftPos->ChqSerialNumber = ChequeSerial;
		OcxEftPos->ChqBranch = ChequeBranch;
		OcxEftPos->ChqAccount = ChequeAccount;
		OcxEftPos->DoChequeVerify();
   }
}

void __fastcall TEftPosANZ::ReprintReceipt()
{
	if(Enabled)
   {
		OcxEftPos->DoReprintReceipt();
	}
}

AnsiString TEftPosANZ::GetRefNumber()
{
	return AnsiString("S_") + TDateTime::CurrentDateTime().FormatString("yyyymmddhhmmss");
}

bool TEftPosANZ::CheckChequeBranch(AnsiString Branch)
{
	bool RetVal = false;
   ResultText = "EFTPOS Disabled";
	if(Enabled)
   {
      try
      {
			if(Branch.Length() < 1)
         {
				ResultText = "Bank branch number to short. 1-"+IntToStr(ChequeBranchMaxLength)+" digits required.";
         }
			else if(Branch.Length() > ChequeBranchMaxLength)
			{
				ResultText = "Bank branch number to long. 1-"+IntToStr(ChequeBranchMaxLength)+" digits required.";
         }
         else
         {
         	RetVal = true;
         }
      }
      catch(Exception &E)
      {
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to CheckChequeBranch TEftPosANZ" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to CheckChequeBranch TEftPosANZ"  + E.Message);
         ResultText = "Cheque Bank Branch Verification Failed";
		}
	}
   return RetVal;
}

bool TEftPosANZ::CheckChequeSerial(AnsiString ChequeSerial)
{
	bool RetVal = false;
   ResultText = "EFTPOS Disabled";
	if(Enabled)
   {
      try
      {
			if(ChequeSerial.Length() < 1)
			{
				ResultText = "Cheque serial number to short. 1-"+IntToStr(ChequeSerialMaxLength)+" digits required.";
         }
			else if(ChequeSerial.Length() > ChequeSerialMaxLength)
         {
				ResultText = "Cheque serial number to long. 1-"+IntToStr(ChequeSerialMaxLength)+" digits required.";
         }
         else
			{
         	RetVal = true;
         }
      }
      catch(Exception &E)
      {
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Serial TEftPosANZ" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Serial TEftPosANZ"  + E.Message);
         ResultText = "Cheque Serial Number Verification Failed";
		}
	}
   return RetVal;
}

bool TEftPosANZ::CheckChequeAccount(AnsiString ChequeAccount)
{
	bool RetVal = false;
   ResultText = "EFTPOS Disabled";
	if(Enabled)
	{
      try
      {
			if(ChequeAccount.Length() < 1)
         {
				ResultText = "Cheque Account number to short. 1-"+IntToStr(ChequeAccountMaxLength)+" digits required.";
         }
			else if(ChequeAccount.Length() > ChequeAccountMaxLength)
         {
				ResultText = "Cheque Account number to long. 1-"+IntToStr(ChequeAccountMaxLength)+" digits required.";
         }
         else
			{
         	RetVal = true;
         }
      }
      catch(Exception &E)
      {
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Account TEftPosANZ" + E.Message);
			TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Account TEftPosANZ"  + E.Message);
         ResultText = "Cheque Serial Account Verification Failed";
      }
	}
   return RetVal;
}
//--------------------------------------------------------------------------
bool TEftPosANZ::IsCashOutSupported()
{
    return true;
}
//--------------------------------------------------------------------------
