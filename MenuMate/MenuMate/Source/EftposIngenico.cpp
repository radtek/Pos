//---------------------------------------------------------------------------
#pragma hdrstop

#include "EftposIngenico.h"
//---------------------------------------------------------------------------


#include "Enum.h"
#include "DeviceRealTerminal.h"
#include "Comms.h"
#include "main.h"
#include "MMMessageBox.h"
#include "Processing.h"
#include "Printout.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//                             TEftPosANZ
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TEftPosIngenico::TEftPosIngenico()
{
	EftPosControl	= NULL;
	EFTPOSType = eTEftPosIngenico;
	ChequeAccountMaxLength = 14;
	ChequeBranchMaxLength = 6;
	ChequeSerialMaxLength = 12;
}


TEftPosIngenico::~TEftPosIngenico()
{
   if(EftPosControl != NULL)
   {
		delete EftPosControl;
   }
}

void TEftPosIngenico::Initialise()
{
	TEftPos::Initialise();
	try
   {
		if(TGlobalSettings::Instance().EnableEftPosIngenico)
      {
         Enabled = true;
         EftPosControl = new TCsdEft(frmMain);
			EftPosControl->OnPrintReceiptEvent = PrintReceiptEvent;
			EftPosControl->OnReprintReceiptEvent = ReprintReceiptEvent;
         EftPosControl->OnTransactionEvent = TransactionEvent;
         EftPosControl->OnChequeVerifyEvent = ChequeVerifyEvent;
         EftPosControl->OnGetLastTransactionEvent = GetLastTransactionEvent;
			EftPosControl->OnGetLastReceiptEvent = GetLastReceiptEvent;
//         EftPosControl->OnGetMerchantsEvent = GetMerchantsEvent;
//			EftPosControl->DoGetMerchants();
         if(EftPosControl->VersionMajor != 3)
         {
            Enabled = false;
				TManagerLogs::Instance().Add("NA",EFTPOSLOG,"EFTPOS Version Mismatch Version 3 OCX Control Required");
            TManagerLogs::Instance().Add("NA",ERRORLOG,"EFTPOS Version Mismatch Version 3 OCX Control Required");
         }
      }
      else
      {
         Enabled = false;
      }
   }
   catch(Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to create TEftPosIngenico" + E.Message);
      TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to create TEftPosIngenico"  + E.Message);
      EftPosControl = NULL;
   	Enabled = false;
   }
}

void TEftPosIngenico::DoControlPannel()
{
	if(Enabled && EftPosControl->Ready)
   {
		EftPosControl->DialogX = 0;
		EftPosControl->DialogY = 0;
      EftPosControl->DialogType = "TouchScreen";
		EftPosControl->DialogPosition = "Center";
		EftPosControl->DoControlPanel();
   }
}

void TEftPosIngenico::ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase,
											Currency AmtCash, UnicodeString TxnRef,
                                 ePANSource PANSource, UnicodeString CardSwipe,
                                 int ExpiryMonth, int ExpiryYear)
{
	if(Enabled)
   {
	  EftPosControl->DialogPosition = "center";
	  EftPosControl->DialogTitle = "MenuMate EftPos";
	  EftPosControl->DialogType = "Standard";
	  EftPosControl->DialogX = 0;
	  EftPosControl->DialogY = 0;
	  EftPosControl->EnableTopmost = true;

		switch (TxnType)
      {
			case TransactionType_PURCHASE :
      	case TransactionType_CASH_ADVANCE :
      	case TransactionType_PURCHASE_PLUS_CASH :
	         EftPosControl->TxnType = "P";
         break;
      	case TransactionType_REFUND :
				EftPosControl->TxnType = "R";
			break;
			case TransactionType_INQUIRY :
			case TransactionType_NONE :
         default :
				EftPosControl->TxnType = "";
         break;
      }
      switch(PANSource)
      {
      	case PANSource_None:
		      EftPosControl->PanSource = " ";
         break;
      	case PANSource_Manual:
		      EftPosControl->PanSource = "K";
	         EftPosControl->Pan = CardSwipe;
         break;
      	case PANSource_CardReader:
		      EftPosControl->PanSource = "S";
	         EftPosControl->Track2 = CardSwipe;
         break;
		}

		if(TxnType == TransactionType_INQUIRY) // EFTPOS Recovery.
		{
			EftPosControl->DoGetLastTransaction();
		}
		else
		{
			EftPosControl->EnableTip = false;
			EftPosControl->CutReceipt = false;
			EftPosControl->ReceiptAutoPrint = false;
			EftPosControl->EnableTip = false;
			EftPosControl->AccountType = "";
			EftPosControl->TxnRef = TxnRef;
			EftPosControl->AmtPurchase = AmtPurchase;
			EftPosControl->AmtCash = AmtCash;
			EftPosControl->DateExpiry = IntToStr(ExpiryMonth) + "//" + IntToStr(ExpiryYear);
			EftPosControl->DoTransaction();
		}
	}
}

void __fastcall TEftPosIngenico::TransactionEvent(TObject *Sender)
{
	//Loop though Payment Types and set as Appropreate.
	TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(EftPosControl->TxnRef);
	if(EftTrans != NULL)
	{
		if(EftPosControl->ResponseCode == WideString("00") || EftPosControl->ResponseCode == WideString("08"))
		{
			EftTrans->Result = eAccepted;
			EftTrans->ResultText = EftPosControl->ResponseText;
			if(EftPosControl->ResponseCode == WideString("08"))
			{
				EftTrans->SuppressReceipt = true;
			}
		}
		else
		{
			EftTrans->Result = eDeclined;
			EftTrans->ResultText = EftPosControl->ResponseText;
		}
		EftTrans->EventCompleted = true;
	}
}

void __fastcall TEftPosIngenico::GetLastTransactionEvent(TObject *Sender)
{
	if(EftPosControl->Success)
	{
		TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(EftPosControl->TxnRef);
		if(EftTrans != NULL)
		{
			if(EftPosControl->ResponseCode == WideString("00") || EftPosControl->ResponseCode == WideString("08"))
			{
				EftTrans->Result = eAccepted;
				if(EftPosControl->ResponseCode == WideString("08"))
				{
					EftTrans->SuppressReceipt = true;
				}
			}
			EftTrans->ResultText = EftPosControl->ResponseText;
			EftTrans->EventCompleted = true;
		}
	}
	else
	{
		TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(EftPosControl->TxnRef);
		if(EftTrans != NULL)
		{
			EftTrans->Result = eFailed;
			EftTrans->EventCompleted = true;
		}
	}
}

void __fastcall TEftPosIngenico::GetLastReceiptEvent(TObject *Sender)
{
//	TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(EftPosControl->TxnRef);
//	if(EftTrans != NULL)
//	{
		WCHAR ReceiptType = 'C';
		PrintReceiptEvent(NULL, &ReceiptType);
//		EftTrans->EventCompleted = true;
//	}
}

void __fastcall TEftPosIngenico::ChequeVerifyEvent(TObject *Sender)
{
	TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(EftPosControl->ChqSerialNumber +
																				  EftPosControl->ChqBranch +
																				  EftPosControl->ChqAccount);
	if(EftTrans != NULL)
	{
		if(EftPosControl->Success)
		{
			EftTrans->Result = eAccepted;
			EftTrans->ResultText = EftPosControl->ResponseText;
		}
		else
		{
			EftTrans->Result = eDeclined;
			EftTrans->ResultText = EftPosControl->ResponseText;
		}
		EftTrans->EventCompleted = true;
	}
}

void __fastcall TEftPosIngenico::GetMerchantsEvent(TObject *Sender)
{
}

void __fastcall TEftPosIngenico::PrintReceiptEvent(TObject *Sender,
		BSTR ReceiptType)
{
	if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		TManagerLogs::Instance().Add("NA",EFTPOSLOG,"No Recipt Printer Configured for EFTPOS output.");
		TManagerLogs::Instance().Add("NA",ERRORLOG,"No Recipt Printer Configured for EFTPOS output.");
		return;
	}

	if((*ReceiptType == 'R' || *ReceiptType == 'C') && (EftPosControl->Receipt.Length() > 0))
	{
		TPrintout *Printout = new TPrintout;
		Printout->Printer = TComms::Instance().ReceiptPrinter;
		Printout->PrintFormat->DocumentName = "";
		try
		{
			Printout->PrintFormat->Line->FontInfo.Reset();
			Printout->PrintFormat->Line->ColCount					= 1;
			Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
			Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
			Printout->PrintFormat->Line->Columns[0]->Text		= Printout->PrintFormat->DocumentName;
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();

			LastEftPosReceipt->Clear();
			LastEftPosReceipt->Text = EftPosControl->Receipt;
			for (int i=0; i< LastEftPosReceipt->Count; i++)
			{
				Printout->PrintFormat->Line->Columns[0]->Text	= LastEftPosReceipt->Strings[i];
				Printout->PrintFormat->AddLine();
			}

			Printout->PrintFormat->PartialCut();

			Printout->Print();
			delete Printout;
		}
		catch (Exception &E)
		{
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
			TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
			delete Printout;
		}
	}
	TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(IntToStr(ePrintReceiptEvent));
	if(EftTrans != NULL)
	{
		EftTrans->EventCompleted = true;
	}
}

void __fastcall TEftPosIngenico::ReprintReceiptEvent(TObject *Sender)
{
	if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		TManagerLogs::Instance().Add("NA",EFTPOSLOG,"No Recipt Printer Configured for EFTPOS output.");
		TManagerLogs::Instance().Add("NA",ERRORLOG,"No Recipt Printer Configured for EFTPOS output.");
		return;
	}

	TPrintout *Printout = new TPrintout;
	Printout->Printer = TComms::Instance().ReceiptPrinter;

	try
	{
		Printout->PrintFormat->DocumentName = "Receipt Reprint";

//         Printout->PrintFormat->PrintGraphic();

      Printout->PrintFormat->Line->FontInfo.Reset();
		Printout->PrintFormat->Line->ColCount					= 1;
		Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
		Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
		Printout->PrintFormat->Line->Columns[0]->Text			= Printout->PrintFormat->DocumentName;
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->NewLine();

/*         for (int i=0; i<Receipt->Header->Count; i++)
		{
			Printout->PrintFormat->Line->Columns[0]->Text	= Receipt->Header->Strings[i];
			Printout->PrintFormat->AddLine();
		}
		Printout->PrintFormat->NewLine();*/

		TStringList *EFTPOSReceipt = new TStringList;
		try
		{
			EFTPOSReceipt->Text = EftPosControl->Receipt;
			for (int i=0; i< EFTPOSReceipt->Count; i++)
			{
				Printout->PrintFormat->Line->Columns[0]->Text	= EFTPOSReceipt->Strings[i];
				Printout->PrintFormat->AddLine();
			}
		}
		__finally
		{
			delete EFTPOSReceipt;
		}

/*         Printout->PrintFormat->NewLine();
		for (int i=0; i<Receipt->Footer->Count; i++)
		{
			Printout->PrintFormat->Line->Columns[0]->Text			= Receipt->Footer->Strings[i];
			Printout->PrintFormat->AddLine();
		}*/

		Printout->PrintFormat->PartialCut();

		Printout->Print();
		delete Printout;
	}
	catch (Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
		delete Printout;
	}
	TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(IntToStr(ePrintReceiptEvent));
	if(EftTrans != NULL)
	{
		EftTrans->EventCompleted = true;
	}
}


void __fastcall TEftPosIngenico::DoSettlementCutover()
{
	if(Enabled)
	{
		EftPosControl->DialogX = 0;
		EftPosControl->DialogY = 0;
      EftPosControl->DialogType = "TouchScreen";
		EftPosControl->DialogPosition = "Center";
		EftPosControl->DoDisplaySettlement();
   }
}

void __fastcall TEftPosIngenico::DoSettlementEnquiry()
{
	if(Enabled)
   {
		EftPosControl->DialogX = 0;
		EftPosControl->DialogY = 0;
      EftPosControl->DialogType = "TouchScreen";
		EftPosControl->DialogPosition = "Center";
		EftPosControl->DoDisplaySettlement();
   }
}

void TEftPosIngenico::ChequeVerify( Currency AmtPurchase,
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
				EftPosControl->PanSource = " ";
			break;
			case PANSource_Manual:
				EftPosControl->PanSource = "K";
			break;
			case PANSource_CardReader:
				EftPosControl->PanSource = "S";
			break;
		}

		EftPosControl->AccountType = "0";
		EftPosControl->AmtPurchase = AmtPurchase;
		EftPosControl->ChqSerialNumber = ChequeSerial;
		EftPosControl->ChqBranch = ChequeBranch;
		EftPosControl->ChqAccount = ChequeAccount;
		EftPosControl->DoChequeVerify();
	}
}

void __fastcall TEftPosIngenico::ReprintReceipt()
{
	if(Enabled)
   {
// This is faulty, The Reprint Event Fires but contains no data.
//		EftPosControl->DoReprintReceipt();
		EftPosControl->DoGetLastReceipt();
	}
}

AnsiString TEftPosIngenico::GetRefNumber()
{
	return TDateTime::CurrentDateTime().FormatString("yyyymmddhhmmss");
}

bool TEftPosIngenico::CheckChequeBranch(AnsiString Branch)
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
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to CheckChequeBranch TEftPosIngenico" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to CheckChequeBranch TEftPosIngenico"  + E.Message);
         ResultText = "Cheque Bank Branch Verification Failed";
      }
	}
   return RetVal;
}

bool TEftPosIngenico::CheckChequeSerial(AnsiString ChequeSerial)
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
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Serial TEftPosIngenico" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Serial TEftPosIngenico"  + E.Message);
         ResultText = "Cheque Serial Number Verification Failed";
      }
	}
   return RetVal;
}

bool TEftPosIngenico::CheckChequeAccount(AnsiString ChequeAccount)
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
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Account TEftPosIngenico" + E.Message);
			TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Account TEftPosIngenico"  + E.Message);
         ResultText = "Cheque Serial Account Verification Failed";
      }
	}
   return RetVal;
}
//--------------------------------------------------------------------------
bool TEftPosIngenico::IsCashOutSupported()
{
    return true;
}
//--------------------------------------------------------------------------
