//---------------------------------------------------------------------------
#pragma hdrstop

#include "EftposSyncro.h"
#include "DropDown.h"
#include "DropDownVar.h"
//---------------------------------------------------------------------------
#include "Enum.h"
#include "DeviceRealTerminal.h"
#include "Comms.h"
#include "main.h"
#include "MMMessageBox.h"
#include "printout.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//                             TEftPosANZ
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TEftPosSyncro::TEftPosSyncro()
{
	SyncEftPos	= NULL;
	EFTPOSType = eTEftPosSyncro;
	ChequeAccountMaxLength = 8;
	ChequeBranchMaxLength = 6;
	ChequeSerialMaxLength = 8;
   OverLimitAmount = 99999.99;		
}


TEftPosSyncro::~TEftPosSyncro()
{
   if(SyncEftPos != NULL)
   {
		delete SyncEftPos;
   }
}

void TEftPosSyncro::Initialise()
{
	TEftPos::Initialise();
	try
	{
		if(TGlobalSettings::Instance().EnableEftPosSyncro)
		{
			Enabled = true;
			SyncEftPos = new TPOSInterface(frmMain);
			SyncEftPos->POSHardware = UnicodeString("WIN32").w_str();
			SyncEftPos->POSSoftwareName = UnicodeString("MnuMte").w_str();
			SyncEftPos->POSSoftwareNumber = UnicodeString("3200").w_str();
			SyncEftPos->OnTransactionComplete = TransactionEvent;
			SyncEftPos->OnStartPrintBanner = StartPrintBanner;
			SyncEftPos->OnEndPrintBanner = EndPrintBanner;
		}
		else
		{
			Enabled = false;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to create TEftPosSyncro" + E.Message);
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to create TEftPosSyncro"  + E.Message);
		SyncEftPos = NULL;
		Enabled = false;
	}
	catch(...)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to create TEftPosSyncro");
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to create TEftPosSyncro");
		SyncEftPos = NULL;
		Enabled = false;
	}
}

void TEftPosSyncro::DoControlPannel()
{
	if(!Enabled) return;
	try
	{
		std::auto_ptr<TfrmDropDownFunc>(frmDropDown)(TfrmDropDownFunc::Create<TfrmDropDownFunc>(Screen->ActiveForm));
		frmDropDown->AddButton("EFTPOS Logon",&DoLogon);
		frmDropDown->AddButton("Settlement  Enquiry",&DoSettlementEnquiry);
		frmDropDown->AddButton("Print Terminal Totals",&DoPrintLocalTotals);
		frmDropDown->AddButton("Utility Functions",&DoUtilityFunctions);
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

void TEftPosSyncro::ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase,Currency AmtCash,
									 UnicodeString TxnRef,ePANSource PANSource,
									 UnicodeString CardSwipe,int ExpiryMonth, int ExpiryYear)
{
	if(Enabled)
	{
      long PrimaryAmount = 0;
      long SecondaryAmount = 0;
		FinancialTypeEnum eFinancialType = FinancialTypeEnum::FINANCIAL_NONE;
		switch (TxnType)
		{
			case TransactionType_PURCHASE :
				PrimaryAmount = double(AmtPurchase) * 100.0;
				eFinancialType = FinancialTypeEnum::FINANCIAL_PURCHASE;
			break;
			case TransactionType_CASH_ADVANCE :
				PrimaryAmount = double(AmtCash) * 100.0;
				eFinancialType = FinancialTypeEnum::FINANCIAL_CASH_ADVANCE;
         break;
      	case TransactionType_PURCHASE_PLUS_CASH :
	         PrimaryAmount = double(AmtPurchase) * 100.0;
				SecondaryAmount = double(AmtCash) * 100.0;
				eFinancialType = FinancialTypeEnum::FINANCIAL_PURCHASE_PLUS_CASH;
			break;
			case TransactionType_REFUND :
				PrimaryAmount = double(AmtPurchase) * 100.0;
				eFinancialType = FinancialTypeEnum::FINANCIAL_REFUND;
			break;
			case TransactionType_INQUIRY :
				PrimaryAmount = double(AmtPurchase) * 100.0;
				SecondaryAmount = double(AmtCash) * 100.0;
				eFinancialType = FinancialTypeEnum::FINANCIAL_INQUIRY;
			break;
		}

      switch(PANSource)
      {
      	case PANSource_None:
         break;
      	case PANSource_Manual:
				SyncEftPos->InputCustomerManualPAN = CardSwipe.w_str();

				SyncEftPos->InputMotoType = MotoTransTypeEnum::MOTO_SINGLE_MAIL_ORDER;

				if(ExpiryMonth < 10)
				{
					SyncEftPos->InputCustomerExpiryDateMonth = UnicodeString("0" + IntToStr(ExpiryMonth)).w_str();
				}
				else
				{
					SyncEftPos->InputCustomerExpiryDateMonth = UnicodeString(IntToStr(ExpiryMonth)).w_str();
				}

				if(ExpiryYear < 10)
				{
					SyncEftPos->InputCustomerExpiryDateYear = UnicodeString("0" + IntToStr(ExpiryYear)).w_str();
				}
				else
				{
					SyncEftPos->InputCustomerExpiryDateYear = UnicodeString(IntToStr(ExpiryYear)).w_str();
				}
         break;
      	case PANSource_CardReader:
				SyncEftPos->InputCustomerTrack2Data = UnicodeString(CardSwipe).w_str();
         break;
      }

		SyncEftPos->StartFinancialTransaction(eFinancialType,StrToInt(TxnRef),true,PrimaryAmount,SecondaryAmount);
   }
}

void __fastcall TEftPosSyncro::TransactionEvent(TObject *Sender,long lUniqueID, long lResult)
{
	TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(IntToStr((int)lUniqueID));
	if(EftTrans != NULL)
	{
		switch(lResult)
		{
			case ResultCodeEnum::ACCEPTED :
				EftTrans->Result = eAccepted;
				EftTrans->ResultText = SyncEftPos->ResultText;				
			break;
			case ResultCodeEnum::DECLINED :
			case ResultCodeEnum::FAILED :
			case ResultCodeEnum::EFTPOS_UNAVAILABLE :
			case ResultCodeEnum::SPECIAL :
				EftTrans->Result = eDeclined;
				EftTrans->ResultText = SyncEftPos->ResultText;
			break;
		}
		EftTrans->EventCompleted = true;
	}
}

void __fastcall TEftPosSyncro::DoSettlementCutover()
{
	if(!Enabled) return;
	long lUniqueID = 0;
	SyncEftPos->GetUniqueID(&lUniqueID);
	AnsiString UniqueID = IntToStr((int)lUniqueID);
	try
	{
		SyncEftPos->StartAdminFunction(AdminTypeEnum::ADMIN_SETTLEMENT_CUTOVER,lUniqueID,false);
/*		SetTransactionEvent(UniqueID,TransactionType_INQUIRY);
		SyncEftPos->StartAdminFunction(ADMIN_SETTLEMENT_CUTOVER,lUniqueID,false);
		if(WaitOnEftPosEvent(UniqueID))
		{
			TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(UniqueID);
			if(EftTrans != NULL)
			{
				if(EftTrans->Result != eAccepted)
				{
					MessageBox(EftTrans->ResultText, "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
				}
			}
			else
			{
				MessageBox("EFTPOS FAILED", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
			}
		}
		else
		{
			MessageBox("EFTPOS Failed", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
		}*/
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void __fastcall TEftPosSyncro::DoSettlementEnquiry()
{
	if(!Enabled) return;
	long lUniqueID = 0;
	SyncEftPos->GetUniqueID(&lUniqueID);
	AnsiString UniqueID = IntToStr((int)lUniqueID);
	try
	{
		SyncEftPos->StartAdminFunction(AdminTypeEnum::ADMIN_SETTLEMENT_INQUIRY,lUniqueID,false);
/*		SetTransactionEvent(UniqueID,TransactionType_INQUIRY);
		SyncEftPos->StartAdminFunction(ADMIN_SETTLEMENT_INQUIRY,lUniqueID,false);
		if(WaitOnEftPosEvent(UniqueID))
		{
			TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(UniqueID);
			if(EftTrans != NULL)
			{
				if(EftTrans->Result != eAccepted)
				{
					MessageBox(EftTrans->ResultText, "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
				}
			}
			else
			{
				MessageBox("EFTPOS FAILED", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
			}
		}
		else
		{
			MessageBox("EFTPOS Failed", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
		}*/
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void __fastcall TEftPosSyncro::DoLogon()
{
	if(!Enabled) return;
	long lUniqueID = 0;
	SyncEftPos->GetUniqueID(&lUniqueID);
	AnsiString UniqueID = IntToStr((int)lUniqueID);
	try
	{
		SyncEftPos->StartAdminFunction(AdminTypeEnum::ADMIN_LOGON,lUniqueID,false);
/*		SetTransactionEvent(UniqueID,TransactionType_INQUIRY);
		SyncEftPos->StartAdminFunction(ADMIN_LOGON,lUniqueID,false);
		if(WaitOnEftPosEvent(UniqueID))
		{
			TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(UniqueID);
			if(EftTrans != NULL)
			{
				if(EftTrans->Result != eAccepted)
				{
					MessageBox(EftTrans->ResultText, "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
				}
			}
			else
			{
				MessageBox("EFTPOS FAILED", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
			}
		}
		else
		{
			MessageBox("EFTPOS Failed", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
		} */
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void __fastcall TEftPosSyncro::DoUtilityFunctions()
{
	if(!Enabled) return;
	try
	{
		std::auto_ptr<TfrmDropDownVar> frmDropDownVar(TfrmDropDownVar::Create<TfrmDropDownVar>(Screen->ActiveForm));

		frmDropDownVar->AddButton(TddvBtn("Archive Journal",UtilityTypeEnum::UTILITY_ARCHIVE_JOURNAL));
		frmDropDownVar->AddButton(TddvBtn("Change Configuration",UtilityTypeEnum::UTILITY_CHANGE_CONFIGURATION));
		frmDropDownVar->AddButton(TddvBtn("Diagnostic Inquiry",UtilityTypeEnum::UTILITY_DIAGNOSTIC_INQUIRY));
		frmDropDownVar->AddButton(TddvBtn("Display KVC",UtilityTypeEnum::UTILITY_DISPLAY_KVC));
		frmDropDownVar->AddButton(TddvBtn("Display PIN Pad Serial No.",UtilityTypeEnum::UTILITY_DISPLAY_PINPAD_SERIAL_NUMBER));
		frmDropDownVar->AddButton(TddvBtn("Display Version",UtilityTypeEnum::UTILITY_DISPLAY_TERMINAL_SOFTWARE_VERSION));
		frmDropDownVar->AddButton(TddvBtn("Download Card Table",UtilityTypeEnum::UTILITY_DOWNLOAD_CARD_TABLE));
		frmDropDownVar->AddButton(TddvBtn("Download Terminal Settings",UtilityTypeEnum::UTILITY_DOWNLOAD_TERMINAL_SETTINGS));
		frmDropDownVar->AddButton(TddvBtn("Enter Registration Key",UtilityTypeEnum::UTILITY_ENTER_REGISTRATION_KEY));
		frmDropDownVar->AddButton(TddvBtn("Menu",UtilityTypeEnum::UTILITY_SELECT_FROM_MENU));
		frmDropDownVar->AddButton(TddvBtn("Print Pending Receipts",UtilityTypeEnum::UTILITY_PRINT_PENDING_RECEIPTS));
		frmDropDownVar->AddButton(TddvBtn("Print Registration Info",UtilityTypeEnum::UTILITY_PRINT_REGISTRATION_INFO));
		frmDropDownVar->AddButton(TddvBtn("Print Stored Trans Totals",UtilityTypeEnum::UTILITY_PRINT_STORED_TRANSACTION_TOTALS));
		frmDropDownVar->AddButton(TddvBtn("Reprint Eftpos Receipt",UtilityTypeEnum::UTILITY_REPRINT_RECEIPT));
		frmDropDownVar->AddButton(TddvBtn("Reset Terminal",UtilityTypeEnum::UTILITY_RESET_TERMINAL));
		frmDropDownVar->AddButton(TddvBtn("Transmit EOV",UtilityTypeEnum::UTILITY_TRANSMIT_EOV));
		frmDropDownVar->AddButton(TddvBtn("View Journal",UtilityTypeEnum::UTILITY_VIEW_JOURNAL));
		frmDropDownVar->AddButton(TddvBtn("View Report",UtilityTypeEnum::UTILITY_VIEW_REPORT));

		if(frmDropDownVar->ShowModal() == mrOk)
		{
			long lUniqueID = 0;
			SyncEftPos->GetUniqueID(&lUniqueID);
			AnsiString UniqueID = IntToStr((int)lUniqueID);
			SyncEftPos->StartUtility(frmDropDownVar->Result,lUniqueID,false);
/*				long lUniqueID = 0;
			SyncEftPos->GetUniqueID(&lUniqueID);
			AnsiString UniqueID = IntToStr(lUniqueID);
			SetTransactionEvent(UniqueID,TransactionType_NONE);
			SyncEftPos->StartUtility(frmDropDownVar->Result,lUniqueID,false);
			if(WaitOnEftPosEvent(UniqueID))
			{
				TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(UniqueID);
				if(EftTrans != NULL)
				{
					if(EftTrans->Result != eAccepted)
					{
						MessageBox(EftTrans->ResultText, "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
					}
				}
				else
				{
					MessageBox("EFTPOS FAILED", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
				}
			}
			else
			{
				MessageBox("EFTPOS Failed", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
			} */
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"DoControlPannel Failed" + E.Message);
	}
}

void __fastcall TEftPosSyncro::DoPrintLocalTotals()
{
	if(!Enabled) return;
	long lUniqueID = 0;
	SyncEftPos->GetUniqueID(&lUniqueID);
	AnsiString UniqueID = IntToStr((int)lUniqueID);
	try
	{
		SyncEftPos->StartAdminFunction(AdminTypeEnum::ADMIN_PRINT_LOCAL_TERMINAL_TOTALS,lUniqueID,false);
/*		SetTransactionEvent(UniqueID,TransactionType_INQUIRY);
		SyncEftPos->StartAdminFunction(ADMIN_PRINT_LOCAL_TERMINAL_TOTALS,lUniqueID,false);
		if(WaitOnEftPosEvent(UniqueID))
		{
			TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(UniqueID);
			if(EftTrans != NULL)
			{
				if(EftTrans->Result != eAccepted)
				{
					MessageBox(EftTrans->ResultText, "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
				}
			}
			else
			{
				MessageBox("EFTPOS FAILED", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
			}
		}
		else
		{
			MessageBox("EFTPOS Failed", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
		}   */
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
	}
}

void TEftPosSyncro::ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
							 AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef)
{
	if(Enabled)
   {
		long PrimaryAmount = double(AmtPurchase) * 100.0;
		SyncEftPos->StartChequeAuthorisation(UnicodeString(ChequeSerial).w_str(),UnicodeString(ChequeBranch).w_str()
														 ,UnicodeString(ChequeAccount).w_str(),true,PrimaryAmount);
	}
}

//kbybd_event

void __fastcall TEftPosSyncro::ReprintReceipt()
{
	if(!Enabled) return;
	try
	{
		long lUniqueID = 0;
		SyncEftPos->GetUniqueID(&lUniqueID);
		AnsiString UniqueID = IntToStr((int)lUniqueID);
		SyncEftPos->StartUtility(UtilityTypeEnum::UTILITY_REPRINT_RECEIPT,lUniqueID,false);

/*		long lUniqueID = 0;
		SyncEftPos->GetUniqueID(&lUniqueID);
		AnsiString UniqueID = IntToStr(lUniqueID);
		SetTransactionEvent(UniqueID,TransactionType_NONE);
		SyncEftPos->StartUtility(UTILITY_REPRINT_RECEIPT,lUniqueID,false);
		if(WaitOnEftPosEvent(UniqueID))
		{
			TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(UniqueID);
			if(EftTrans != NULL)
			{
				if(EftTrans->Result != eAccepted)
				{
					MessageBox(EftTrans->ResultText, "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
				}
			}
			else
			{
				MessageBox("EFTPOS FAILED", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
			}
		}
		else
		{
			MessageBox("EFTPOS Failed", "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
		} */
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Reprint EFTPOS Receipt Failed" + E.Message);
	}
}

AnsiString TEftPosSyncro::GetRefNumber()
{
	long UniqueID = 0;
	if(Enabled)
	{
		SyncEftPos->GetUniqueID(&UniqueID);
	}
	return IntToStr((int)UniqueID);
}

bool TEftPosSyncro::CheckChequeBranch(AnsiString Branch)
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
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to CheckChequeBranch TEftPosSyncro" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to CheckChequeBranch TEftPosSyncro"  + E.Message);
         ResultText = "Cheque Bank Branch Verification Failed";
      }
	}
   return RetVal;
}

bool TEftPosSyncro::CheckChequeSerial(AnsiString ChequeSerial)
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
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Serial TEftPosSyncro" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Serial TEftPosSyncro"  + E.Message);
         ResultText = "Cheque Serial Number Verification Failed";
      }
	}
   return RetVal;
}

bool TEftPosSyncro::CheckChequeAccount(AnsiString ChequeAccount)
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
         TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to Check Cheque Account TEftPosSyncro" + E.Message);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to Check Cheque Account TEftPosSyncro"  + E.Message);
			ResultText = "Cheque Serial Account Verification Failed";
      }
	}
   return RetVal;
}

//---------------------------------------------------------------------------

void __fastcall TEftPosSyncro::StartPrintBanner(TObject *Sender,
		ReceiptTypeEnum eReceiptType)
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
		if(eReceiptType == ReceiptTypeEnum::RECEIPT_FINANCIAL) Printout->PrintFormat->DocumentName = "RECEIPT";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_FINANCIAL_EXCEPTION) Printout->PrintFormat->DocumentName = "RECEIPT_FINANCIAL_EXCEPTION";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_LOGON) Printout->PrintFormat->DocumentName = "RECEIPT_LOGON";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_LOGON_EXCEPTION) Printout->PrintFormat->DocumentName = "RECEIPT_LOGON_EXCEPTION";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_PENDING_REVERSAL) Printout->PrintFormat->DocumentName = "RECEIPT_PENDING_REVERSAL";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_SETTLEMENT) Printout->PrintFormat->DocumentName = "RECEIPT_SETTLEMENT";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_SETTLEMENT_EXCEPTION) Printout->PrintFormat->DocumentName = "RECEIPT_SETTLEMENT_EXCEPTION";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_SIGNATURE_DECLINED) Printout->PrintFormat->DocumentName = "RECEIPT_SIGNATURE_DECLINED";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_OFFLINE_PROCESSED) Printout->PrintFormat->DocumentName = "RECEIPT_OFFLINE_PROCESSED";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_OFFLINE_SIG_DECLINED) Printout->PrintFormat->DocumentName = "RECEIPT_OFFLINE_SIG_DECLINED";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_OFFLINE_DECLINED) Printout->PrintFormat->DocumentName = "RECEIPT_OFFLINE_DECLINED";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_STORED_TRANSACTION_TOTALS) Printout->PrintFormat->DocumentName = "RECEIPT_STORED_TRANSACTION_TOTALS";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_STORED_TRANSACTION) Printout->PrintFormat->DocumentName = "RECEIPT_STORED_TRANSACTION";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_PLAIN_TEXT) Printout->PrintFormat->DocumentName = "RECEIPT_PLAIN_TEXT";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_POS_PRINT) Printout->PrintFormat->DocumentName = "RECEIPT_POS_PRINT";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_MISC) Printout->PrintFormat->DocumentName = "RECEIPT COPY";
      else Printout->PrintFormat->DocumentName = "";

//         Printout->PrintFormat->PrintGraphic();

      Printout->PrintFormat->Line->FontInfo.Reset();
      Printout->PrintFormat->Line->ColCount					= 1;
      Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
      Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
      Printout->PrintFormat->Line->Columns[0]->Text			= Printout->PrintFormat->DocumentName;
      Printout->PrintFormat->AddLine();


		for (int i=0; i<Receipt->Header->Count; i++)
		{
			Printout->PrintFormat->Line->Columns[0]->Text	= Receipt->Header->Strings[i];
			Printout->PrintFormat->AddLine();
		}

		Printout->Print();
		delete Printout;
		SyncEftPos->ContinuePrinting();
   }
   catch (Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
		delete Printout;
   }
}
//---------------------------------------------------------------------------

void __fastcall TEftPosSyncro::EndPrintBanner(TObject *Sender,
      ReceiptTypeEnum eReceiptType)
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
		if(eReceiptType == ReceiptTypeEnum::RECEIPT_FINANCIAL) Printout->PrintFormat->DocumentName = "RECEIPT_FINANCIAL";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_FINANCIAL_EXCEPTION) Printout->PrintFormat->DocumentName = "RECEIPT_FINANCIAL_EXCEPTION";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_LOGON) Printout->PrintFormat->DocumentName = "RECEIPT_LOGON";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_LOGON_EXCEPTION) Printout->PrintFormat->DocumentName = "RECEIPT_LOGON_EXCEPTION";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_PENDING_REVERSAL) Printout->PrintFormat->DocumentName = "RECEIPT_PENDING_REVERSAL";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_SETTLEMENT) Printout->PrintFormat->DocumentName = "RECEIPT_SETTLEMENT";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_SETTLEMENT_EXCEPTION) Printout->PrintFormat->DocumentName = "RECEIPT_SETTLEMENT_EXCEPTION";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_SIGNATURE_DECLINED) Printout->PrintFormat->DocumentName = "RECEIPT_SIGNATURE_DECLINED";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_OFFLINE_PROCESSED) Printout->PrintFormat->DocumentName = "RECEIPT_OFFLINE_PROCESSED";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_OFFLINE_SIG_DECLINED) Printout->PrintFormat->DocumentName = "RECEIPT_OFFLINE_SIG_DECLINED";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_OFFLINE_DECLINED) Printout->PrintFormat->DocumentName = "RECEIPT_OFFLINE_DECLINED";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_STORED_TRANSACTION_TOTALS) Printout->PrintFormat->DocumentName = "RECEIPT_STORED_TRANSACTION_TOTALS";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_STORED_TRANSACTION) Printout->PrintFormat->DocumentName = "RECEIPT_STORED_TRANSACTION";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_PLAIN_TEXT) Printout->PrintFormat->DocumentName = "RECEIPT_PLAIN_TEXT";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_POS_PRINT) Printout->PrintFormat->DocumentName = "RECEIPT_POS_PRINT";
		else if(eReceiptType == ReceiptTypeEnum::RECEIPT_MISC) Printout->PrintFormat->DocumentName = "RECEIPT_MISC";
		else Printout->PrintFormat->DocumentName = "";

//         Printout->PrintFormat->PrintGraphic();

		Printout->PrintFormat->Line->FontInfo.Reset();
		Printout->PrintFormat->Line->ColCount					= 1;
		Printout->PrintFormat->Line->Columns[0]->Width		= Printout->PrintFormat->Width;
		Printout->PrintFormat->Line->Columns[0]->Alignment	= taCenter;
/*		Printout->PrintFormat->Line->Columns[0]->Text			= Printout->PrintFormat->DocumentName;
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->NewLine();*/

		LastEftPosReceipt->Clear();
		LastEftPosReceipt->Text = SyncEftPos->ReceiptData;
		for (int i=0; i< LastEftPosReceipt->Count; i++)
		{
			LastEftPosReceipt->Strings[i] = LastEftPosReceipt->Strings[i].Trim();
			Printout->PrintFormat->Line->Columns[0]->Text	= LastEftPosReceipt->Strings[i];
			Printout->PrintFormat->AddLine();
		}

		for (int i=0; i<Receipt->Footer->Count; i++)
		{
			Printout->PrintFormat->Line->Columns[0]->Text			= Receipt->Footer->Strings[i];
			Printout->PrintFormat->AddLine();
		}

		Printout->PrintFormat->PartialCut();

		Printout->Print();
		delete Printout;
		SyncEftPos->ContinuePrinting();
	}
	catch (Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
      TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,E.Message);
      delete Printout;
   }
}

