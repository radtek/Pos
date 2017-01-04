//---------------------------------------------------------------------------


#pragma hdrstop

#include "FloatSkimData.h"
#include "DeviceRealTerminal.h"
#include "MMLogging.h"
#include "Message.h"
#include "MMMessageBox.h"
#include "Comms.h"
#include "ShowPrintout.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TFloatSkimData::TFloatSkimData(TForm * inOwner, TMMContactInfo info, FloatSkimTransaction type, Currency inAmount, int inZed_Key):
Owner(inOwner), UserInfo(info), TransType(type), Amount(inAmount), Z_Key(inZed_Key)
{
}

void TFloatSkimData::InsertToDatabase(Database::TDBTransaction &DBTransaction)
{
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);
	  int Skim_Float_Key;

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_REFLOAT_SKIM, 1) FROM RDB$DATABASE";
		 IBInternalQuery->ExecQuery();
		 Skim_Float_Key = IBInternalQuery->Fields[0]->AsInteger;

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		" INSERT INTO REFLOAT_SKIM(REFLOAT_SKIM_KEY, TRANSACTION_TYPE, AMOUNT, STAFF, TERMINAL_NAME, TIME_STAMP, Z_KEY, REASONS, IS_FLOAT_WITHDRAWN_FROM_CASH) "
		" VALUES (:REFLOAT_SKIM_KEY, :TRANSACTION_TYPE, :AMOUNT, :STAFF, :TERMINAL_NAME, :TIME_STAMP, :Z_KEY, :REASONS, :IS_FLOAT_WITHDRAWN_FROM_CASH);";
	  IBInternalQuery->ParamByName("REFLOAT_SKIM_KEY")->AsInteger = Skim_Float_Key;
	  IBInternalQuery->ParamByName("TRANSACTION_TYPE")->AsString = Transaction_types[TransType];
	  IBInternalQuery->ParamByName("AMOUNT")->AsCurrency = Amount;
	  IBInternalQuery->ParamByName("STAFF")->AsString = UserInfo.Name;
	  IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
	  IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
	  IBInternalQuery->ParamByName("Z_KEY")->AsInteger = Z_Key;
	  IBInternalQuery->ParamByName("REASONS")->AsString = Reason;
      IBInternalQuery->ParamByName("IS_FLOAT_WITHDRAWN_FROM_CASH")->AsString = TGlobalSettings::Instance().FloatWithdrawFromCash == 1 ? "T" : "F";
	  IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }

   DBTransaction.Commit();

}

void TFloatSkimData::runReason(void)
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();


	std::auto_ptr <TfrmMessage> frmMessage(TfrmMessage::Create <TfrmMessage> (Owner, TDeviceRealTerminal::Instance().DBControl));
	frmMessage->MessageType = eSkimRefloat;
	frmMessage->ShowModal();
	if (TGlobalSettings::Instance().ForcedReasons && frmMessage->TextResult == "")
	{
		do
		{
			MessageBox("You must enter a Deposit/Withdrawal reason", "Please Comply", MB_OK + MB_ICONINFORMATION);
			frmMessage->ShowModal();
		}
		while (frmMessage->TextResult == "");
	}
	Reason = frmMessage->TextResult;
}

void TFloatSkimData::PrintDocket(void)
{
   try
   {

	  bool PrinterExists = true;

	  if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	  {
		 PrinterExists = false;
	  }

	  std::auto_ptr <TPrintout> Printout(new TPrintout);

	  if (!PrinterExists)
	  {
		 TPrinterPhysical DefaultScreenPrinter;
		 DefaultScreenPrinter.NormalCharPerLine = 40;
		 DefaultScreenPrinter.BoldCharPerLine = 40;
		 Printout->Printer = DefaultScreenPrinter;
	  }
	  else
	  {
		 Printout->Printer = TComms::Instance().ReceiptPrinter;
	  }


 	  Printout->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
	  Printout->PrintFormat->Line->ColCount = 1;
	  Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
	  Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
			Printout->PrintFormat->Line->Columns[0]->Text = "Transaction Docket";
	  Printout->PrintFormat->AddLine();

	  Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
	  Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;

	  Printout->PrintFormat->NewLine();
	  Printout->PrintFormat->Line->Columns[0]->Text = Now().FormatString("d mmm yyyy h:nn:ss am/pm");
	  Printout->PrintFormat->AddLine();
	  Printout->PrintFormat->Line->Columns[0]->Text = TDeviceRealTerminal::Instance().ID.Name;
	  Printout->PrintFormat->AddLine();
	  Printout->PrintFormat->NewLine();

	  Printout->PrintFormat->Line->Columns[0]->DoubleLine();
	  Printout->PrintFormat->AddLine();
	  Printout->PrintFormat->NewLine();

	  Printout->PrintFormat->Line->Columns[0]->Text = UserInfo.Name;
	  Printout->PrintFormat->AddLine();
	  Printout->PrintFormat->Line->Columns[0]->Text = Transaction_types[TransType] + " of " + FormatFloat("$0.00", Amount);
	  Printout->PrintFormat->AddLine();
	  Printout->PrintFormat->Line->Columns[0]->Text = Reason;
	  Printout->PrintFormat->AddLine();
	  Printout->PrintFormat->NewLine();
	  Printout->PrintFormat->Line->Columns[0]->Text = "Signature ______________________ ";
	  Printout->PrintFormat->AddLine();
	  Printout->PrintFormat->NewLine();
	  Printout->PrintFormat->Line->Columns[0]->DoubleLine();
	  Printout->PrintFormat->AddLine();


	  Printout->PrintFormat->PartialCut();

	  std::auto_ptr <TfrmShowPrintout> (frmShowPrintout)(TfrmShowPrintout::Create <TfrmShowPrintout> (Owner));
	  Printout->PrintToStream(frmShowPrintout->CurrentPrintout.get());

		 frmShowPrintout->btnCancel->Caption = "Close";
		 frmShowPrintout->btnClose->Visible = false;
		 frmShowPrintout->btnClosePrint->Caption = "Print";

	  frmShowPrintout->btnClosePrintClick(Owner);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}