#pragma hdrstop

#include "EftposDPS.h"
#include "DropDown.h"
#include "DropDownVar.h"

//---------------------------------------------------------------------------

#include "Enum.h"
#include "DeviceRealTerminal.h"
#include "Comms.h"
#include "main.h"
#include "MMMessageBox.h"
#include "Printout.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//                             TEftPosANZ
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TEftPosMMDPS::TEftPosMMDPS()
{
	EftPosDPS	= NULL;
	EFTPOSType = eTEftPosDPS;
	ChequeAccountMaxLength = 8;
	ChequeBranchMaxLength = 6;
	ChequeSerialMaxLength = 8;
}


TEftPosMMDPS::~TEftPosMMDPS()
{
   if(EftPosDPS != NULL)
   {
		delete EftPosDPS;
   }
}

void TEftPosMMDPS::Initialise()
{
	TEftPos::Initialise();
	try
	{
		if(TGlobalSettings::Instance().EnableEftPosDPS)
		{
			Enabled = true;
			EftPosDPS = new TDpsEftX(frmMain);
			EftPosDPS->OnAuthorizeEvent = TransactionEvent;
			EftPosDPS->OnOutputReceiptEvent = PrintBanner;
//         EftPosDPS->OnGetLastReceiptEvent = PrintBanner;
         EftPosDPS->OnGetLastTransactionEvent = TransactionEvent;
         EftPosDPS->EnablePrintReceipt = 0;
         EftPosDPS->EnableTestMode = 0;
		 EftPosDPS->ReceiptWidth = TComms::Instance().ReceiptPrinter.NormalCharPerLine;
			EftPosDPS->OnEditTenderEvent = EditTenderEvent;

			// populate tippable card types
			PopulateTippableCardTypes();
		}
		else
		{
			Enabled = false;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to create TEftPosDPS" + E.Message);
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to create TEftPosDPS"  + E.Message);
		EftPosDPS = NULL;
		Enabled = false;
	}
}

void TEftPosMMDPS::DoControlPannel()
{
	if(!Enabled) return;
	try
	{
		std::auto_ptr<TfrmDropDownFunc>(frmDropDown)(TfrmDropDownFunc::Create<TfrmDropDownFunc>(Screen->ActiveForm));
		frmDropDown->AddButton("Logon",&DoLogon);
		frmDropDown->AddButton("Maintenance Control Panel",&DoMaintenance);
		frmDropDown->AddButton("Configuration Control Panel",&DoConfiguration);
		frmDropDown->AddButton("Enquiry",&DoSettlementEnquiry);
		frmDropDown->AddButton("Cutover",&DoSettlementCutover);
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

void TEftPosMMDPS::ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase,Currency AmtCash,
									 UnicodeString TxnRef,ePANSource PANSource,
                            UnicodeString CardSwipe,int ExpiryMonth, int ExpiryYear)
{
	if(Enabled)
   {
      EftPosDPS->Track1 = "";
      EftPosDPS->Track2 = "";
	  EftPosDPS->CardNumber = "";
      
      switch(PANSource)
      {
      	case PANSource_None:
         break;
      	case PANSource_Manual:
         {
			  EftPosDPS->CardNumber = WideString(CardSwipe);

            WideString ExpMonth = "";
            WideString ExYear = "";
				if(ExpiryMonth < 10)
            {
		      	ExpMonth = WideString("0" + IntToStr(ExpiryMonth));
            }
				else
            {
		      	ExpMonth = WideString(IntToStr(ExpiryMonth));
            }

            if(ExpiryYear < 10)
            {
			      ExYear = WideString("0" + IntToStr(ExpiryYear));
            }
            else
            {
			      ExYear = WideString(IntToStr(ExpiryYear));
            }

            EftPosDPS->DateExpiry = ExpMonth + ExYear;
         }
         break;
      	case PANSource_CardReader:
         {
	         EftPosDPS->Track2 = WideString(CardSwipe);
         }
         break;
	  }

		switch (TxnType)
		{
			case TransactionType_CASH_ADVANCE :
			case TransactionType_PURCHASE_PLUS_CASH :
			case TransactionType_PURCHASE :
				EftPosDPS->TxnType = "Purchase";
				break;
			case TransactionType_REFUND :
				EftPosDPS->TxnType = "Refund";
				break;
			case TransactionType_INQUIRY :
				EftPosDPS->TxnType = "";
				break;
			case TransactionType_TIP :
				EftPosDPS->TxnType = "Tip";
				break;
		}

		//if the transaction type is tip, prevent displaying accounts for cheque and savings
		if(TxnType == TransactionType_TIP)
		{
			EftPosDPS->EnableCheckAccount = 0;
			EftPosDPS->EnableSavingAccount = 0;
		}
		else
		{
			EftPosDPS->EnableCheckAccount = 1;
			EftPosDPS->EnableSavingAccount = 1;
        }

      if(TxnType != TransactionType_INQUIRY)
      {
		 EftPosDPS->TxnRef = TxnRef;
		 EftPosDPS->Amount = FormatFloat("0.00",AmtPurchase);
		 EftPosDPS->AmountCashOut = FormatFloat("0.00",AmtCash);
		 EftPosDPS->DoAuthorize();
	  }
	  else
      { //Recovery.
         EftPosDPS->TxnRef = TxnRef;
         EftPosDPS->Amount = FormatFloat("0.00",AmtPurchase);
         EftPosDPS->AmountCashOut = FormatFloat("0.00",AmtCash);
         EftPosDPS->DoGetLastTransaction();
      }
   }
}

void __fastcall TEftPosMMDPS::TransactionEvent(TObject *Sender)
{
    TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(EftPosDPS->TxnRef);
    if(EftTrans != NULL)
    {
        if(EftPosDPS->Authorized)
        {
            if(EftTrans->ResultText == "TRANS CANCELLED")
            {
                EftTrans->Result = eDeclined;
                EftTrans->ResultText = "Transaction cancelled.";
            }
            else
            {
                EftTrans->EftposTransactionID = EftPosDPS->DpsTxnRef; // unique transaction id returned from the eftpos machine
                EftTrans->Result = eAccepted;
                EftTrans->ResultText = EftPosDPS->ResponseText;
                EftTrans->CardType = EftPosDPS->CardType;
                EftTrans->FinalAmount = EftPosDPS->AmountAuthOut;
            }
        }
        else
        {
            EftTrans->Result = eDeclined;
            EftTrans->ResultText = EftPosDPS->ResponseText;
            // check the eftpos response code and set appropriate message to user
            if(EftPosDPS->ReCo == "A1" && TGlobalSettings::Instance().EnableDPSTipping)
            {
                EftTrans->ResultText = "Eftpos configuration invalid for tipping";
            }
        }
        EftTrans->EventCompleted = true;
    }
}

void __fastcall TEftPosMMDPS::DoMaintenance()
{
	if(!Enabled) return;
	try
	{
		EftPosDPS->DoMaint();	
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void __fastcall TEftPosMMDPS::DoConfiguration()
{
	if(!Enabled) return;
	try
	{
		EftPosDPS->DoConfig();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void __fastcall TEftPosMMDPS::DoSettlementEnquiry()
{
	if(!Enabled) return;
	try
	{
      EftPosDPS->Parameter1 = "Enquiry";
      EftPosDPS->DateSettlement = Now().FormatString("yyyymmdd");
		EftPosDPS->DoSettlement();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void __fastcall TEftPosMMDPS::DoLogon()
{
	if(!Enabled) return;
	try
	{
      EftPosDPS->DoLogon();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void __fastcall TEftPosMMDPS::DoSettlementCutover()
{
	if(!Enabled) return;
	try
	{
		EftPosDPS->Parameter1 = "Cutover";
		EftPosDPS->DateSettlement = Now().FormatString("yyyymmdd");
		EftPosDPS->DoSettlement();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void TEftPosMMDPS::ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
							 AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef)
{
	if(Enabled)
   {
  		EftPosDPS->TxnType = "Cheque";
		EftPosDPS->TxnRef = TxnRef;
		EftPosDPS->Amount = FormatFloat("0.00",AmtPurchase);
      EftPosDPS->Track2 = ChequeBranch + " " + ChequeSerial + " " + ChequeAccount;
		EftPosDPS->DoAuthorize();
	}
}

//kbybd_event

void __fastcall TEftPosMMDPS::ReprintReceipt()
{
	if(!Enabled) return;
	try
	{
		EftPosDPS->DoGetLastReceipt();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Reprint EFTPOS Receipt Failed" + E.Message);
	}
}

AnsiString TEftPosMMDPS::GetRefNumber()
{
	return Now().FormatString("yyyymmddhhnnss");
}

bool TEftPosMMDPS::CheckChequeBranch(AnsiString Branch)
{
	bool RetVal = false;
   ResultText = "EFTPOS Disabled";
	if(Enabled)
   {
      try
      {
			if(Branch.Length() != ChequeBranchMaxLength)
         {
				ResultText = "Bank branch number must be "+IntToStr(ChequeBranchMaxLength)+" digits.";
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
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to CheckChequeBranch TEftPosDPS" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to CheckChequeBranch TEftPosDPS"  + E.Message);
         ResultText = "Cheque Bank Branch Verification Failed";
      }
	}
   return RetVal;
}

bool TEftPosMMDPS::CheckChequeSerial(AnsiString ChequeSerial)
{
	bool RetVal = false;
   ResultText = "EFTPOS Disabled";
	if(Enabled)
   {
      try
      {
			if(ChequeSerial.Length() < 6)
         {
				ResultText = "Cheque serial number to short. 6-"+IntToStr(ChequeSerialMaxLength)+" digits required.";
         }
			else if(ChequeSerial.Length() > ChequeSerialMaxLength)
			{
				ResultText = "Cheque serial number to long. 6-"+IntToStr(ChequeSerialMaxLength)+" digits required.";
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
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Serial TEftPosDPS" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Serial TEftPosDPS"  + E.Message);
         ResultText = "Cheque Serial Number Verification Failed";
      }
	}
   return RetVal;
}

bool TEftPosMMDPS::CheckChequeAccount(AnsiString ChequeAccount)
{
	bool RetVal = false;
   ResultText = "EFTPOS Disabled";
	if(Enabled)
   {
      try
      {
			if(ChequeAccount.Length() < 7)
         {
				ResultText = "Cheque Account number to short. 7- "+IntToStr(ChequeAccountMaxLength)+" digits required.";
         }
			else if(ChequeAccount.Length() > ChequeAccountMaxLength)
         {
				ResultText = "Cheque Account number to long. 7- "+IntToStr(ChequeAccountMaxLength)+" digits required.";
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
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Account TEftPosDPS" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Account TEftPosDPS"  + E.Message);
			ResultText = "Cheque Serial Account Verification Failed";
      }
	}
   return RetVal;
}

//---------------------------------------------------------------------------

void __fastcall TEftPosMMDPS::PrintBanner(TObject *Sender)
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
      Printout->PrintFormat->DocumentName = "RECEIPT COPY";

//         Printout->PrintFormat->PrintGraphic();

      Printout->PrintFormat->Line->FontInfo.Reset();
      Printout->PrintFormat->Line->ColCount					= 1;
      Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
      Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
      Printout->PrintFormat->Line->Columns[0]->Text		= Printout->PrintFormat->DocumentName;
      Printout->PrintFormat->AddLine();


		for (int i=0; i<Receipt->Header->Count; i++)
		{
			Printout->PrintFormat->Line->Columns[0]->Text	= Receipt->Header->Strings[i];
			Printout->PrintFormat->AddLine();
		}


//         Printout->PrintFormat->PrintGraphic();

		LastEftPosReceipt->Clear();
      EftPosDPS->ReceiptWidth =  Printout->PrintFormat->Width;
      AnsiString EftReceipt = EftPosDPS->Receipt;
      for( int i = 1 ; i < EftReceipt.Length();)
      {
         AnsiString Data = EftReceipt.SubString(i,EftPosDPS->ReceiptWidth).Trim();
			Printout->PrintFormat->Line->Columns[0]->Text = Data;
			Printout->PrintFormat->AddLine();
         LastEftPosReceipt->Add(Data);
         i += EftPosDPS->ReceiptWidth;
      }

		for (int i=0; i<Receipt->Footer->Count; i++)
		{
			Printout->PrintFormat->Line->Columns[0]->Text			= Receipt->Footer->Strings[i];
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

// --------------------------------------------------------------------------
bool TEftPosMMDPS::AllowsTipsOnTransactions()
{
	AllowsTipping = true && TGlobalSettings::Instance().EnableDPSTipping;
	return TEftPos::AllowsTipsOnTransactions();
}

//---------------------------------------------------------------------------
void TEftPosMMDPS::ProcessTip(WideString OriginalDpsTxnRef, Currency OriginalAmount, Currency TipAmount, UnicodeString MerchantRef)
{
	EftPosDPS->DpsTxnRef = OriginalDpsTxnRef;
	EftPosDPS->Amount = FormatFloat("0.00",OriginalAmount);
	EftPosDPS->Amount2 = FormatFloat("0.00",OriginalAmount + TipAmount);
	EftPosDPS->TxnType = "Tip";
	DpsEftposTipOwnerRefNumber = OriginalDpsTxnRef;
	EftPosDPS->DoEditTender();
}

//---------------------------------------------------------------------------

void __fastcall TEftPosMMDPS::EditTenderEvent(TObject *Sender)
{
	TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(DpsEftposTipOwnerRefNumber);

	if(EftTrans != NULL)
	{
	  if(EftPosDPS->Success && EftPosDPS->ReCo == "00")
	  {
			EftTrans->Result = eAccepted;
			EftTrans->ResultText = EftPosDPS->ResponseText;
			EftTrans->CardType = EftPosDPS->CardType;
			EftTrans->EftposTransactionID = DpsEftposTipOwnerRefNumber;
	  }
	  else
	  {
			EftTrans->Result = eDeclined;
			EftTrans->ResultText = EftPosDPS->ResponseText;
	  }
	  EftTrans->EventCompleted = true;
	  DpsEftposTipOwnerRefNumber = "";
	}
}

//---------------------------------------------------------------------------

void TEftPosMMDPS::PopulateTippableCardTypes()
{
	TippableCardTypes.push_back("Visa");
	TippableCardTypes.push_back("Diners");
	TippableCardTypes.push_back("Amex");
	TippableCardTypes.push_back("MasterCard");
}
