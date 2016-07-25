//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "MM_ModData.h"

#include "main.h"
#include "help.h"
#include "MMMessageBox.h"
#include "Version.h"
#include <DateUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchBtn"
#pragma link "TouchNumpad"
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	FormResize(this);
	lbSource->Caption = ExtractFilePath(Application->ExeName);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Button4Click(TObject *Sender)
{
	frmHelp->Show();
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::btnQuitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSourceBrowseClick(TObject *Sender)
{
	Open->FileName = "";
	Open->InitialDir = ExtractFilePath(lbSource->Caption);
	Open->Title = gbSource->Caption;
	if(Open->Execute())
	{
		lbSource->Caption = Open->FileName;
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnDestBrowseClick(TObject *Sender)
{
	Open->FileName = "";
	Open->InitialDir = ExtractFilePath(lbDest->Caption);
	Open->Title = gbDest->Caption;
	if(Open->Execute())
	{
		lbDest->Caption = Open->FileName;
	}
}
//---------------------------------------------------------------------------

bool TfrmMain::OpenSource()
{
	modData->InterbaseIP = "localhost";
	modData->DatabasePath = lbSource->Caption;
	return modData->OpenPrimaryDB();
}

bool TfrmMain::OpenDest()
{
	modData->InterbaseIP1 = "localhost";
	modData->DatabasePath1 = lbDest->Caption;
	return modData->OpenSecondaryDB();
}

void __fastcall TfrmMain::Button3Click(TObject *Sender)
{
	if(LowerCase(lbSource->Caption) == LowerCase(lbDest->Caption))
	{
		MessageBox("Source and Destination are the Same.",
					  "Error", MB_OK & MB_ICONERROR);
	}
	else
	{
		if(OpenSource() && OpenDest())
		{
			if(SourceVersion() == DestVersion())
			{
				if(SourceVersion() == JoinMateVersion())
				{
					MergeData();
				}
				else
				{
					MessageBox("Program Version does not match database version.\r"
								  "Program Version " + JoinMateVersion() + " \r"
								  "Database Version " + SourceVersion(),
								  "Error", MB_OK & MB_ICONERROR);
				}
			}
			else
			{
				MessageBox("Database Versions do not match.\r"
							  "Source Version " + SourceVersion() + " \r"
							  "Destination Version " + DestVersion(),
							  "Error", MB_OK & MB_ICONERROR);
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::MergeData()
{
	//TODO: Add your source code here
	modData->SQLQueryA->SQL->Clear();
	modData->SQLQueryA->Params->Clear();
	modData->SQLQueryA->ParamCheck = true;
   if(edDateFilter->Text == "")
   {
      ProgBar->Max = GetTotalInvoices();
      ProgBar->Position = 0;
      lbProg->Caption = "Processing " + IntToStr(ProgBar->Max) + " Invoices";

      modData->SQLQueryA->SQL->Text =
         "SELECT *"
         " FROM "
         " ARCBILL";
   }
   else
   {
      ProgBar->Max = GetTotalInvoices(edDateFilter->Text);
      ProgBar->Position = 0;
      lbProg->Caption = "Processing " + IntToStr(ProgBar->Max) + " Invoices";

      modData->SQLQueryA->SQL->Text =
         "SELECT *"
         " FROM "
         " ARCBILL WHERE TIME_STAMP > '" + edDateFilter->Text + "'";
   }
   
	modData->SQLQueryA->Open();
	TDateTime BeginTime = Now();
	unsigned int Time;
	Time = GetTickCount() + 1000;
	int InvoiceMerged = 0;
	for (modData->SQLQueryA->First(); !modData->SQLQueryA->Eof ; modData->SQLQueryA->Next())
	{
		ProgBar->Position++;
		if(Time < GetTickCount() )
		{
			Application->ProcessMessages();
			Time = GetTickCount() + 1000;
		}

		if(!InvoiceExists(modData->SQLQueryA))
		{
			InvoiceMerged++;
			InvoiceCopy(modData->SQLQueryA);
		}
	}
   TDateTime EndTime = Now();
	MessageBox("Merging process completed.\r Total Invoices Merged " + IntToStr(InvoiceMerged),
				  "Data Merged", MB_OK & MB_ICONINFORMATION);
	MessageBox("Elasped Time Minutes: " + IntToStr(MinutesBetween(EndTime, BeginTime)),"Time Info", MB_OK & MB_ICONINFORMATION);
}

bool TfrmMain::InvoiceExists(TSQLQuery *Invoice)
{
	modData->SQLQuery1->SQL->Clear();
	modData->SQLQuery1->Params->Clear();
	modData->SQLQuery1->ParamCheck = true;
	modData->SQLQuery1->SQL->Text =
		"SELECT ARCBILL_KEY"
		" FROM "
		" ARCBILL"
		" WHERE INVOICE_NUMBER = :INVOICE_NUMBER AND"
		" TIME_STAMP = :TIME_STAMP";
	modData->SQLQuery1->ParamByName("INVOICE_NUMBER")->AsInteger =
		Invoice->FieldByName("INVOICE_NUMBER")->AsInteger;
	modData->SQLQuery1->ParamByName("TIME_STAMP")->AsSQLTimeStamp =
		Invoice->FieldByName("TIME_STAMP")->AsSQLTimeStamp;
	modData->SQLQuery1->Open();
	return !modData->SQLQuery1->IsEmpty();
}

int TfrmMain::InvoiceCopy(TSQLQuery *Invoice)
{
	int Retval;

	TTransactionDesc TD;
	TD.TransactionID = 1;
//	TD.IsolationLevel = xilDIRTYREAD;
	TD.IsolationLevel = xilREADCOMMITTED;
	modData->SQLConnection1->StartTransaction(TD);

	Retval = CopyArcBill(Invoice);
	CopyArcBillPay(Invoice,Retval);
	CopyArcRef(Invoice,Retval);
	CopyArcSurcharge(Invoice,Retval);
	CopyArchive(Invoice,Retval);

	modData->SQLConnection1->Commit(TD); // on success, commit the changes;
	return Retval;
}

int TfrmMain::CopyArcBill(TSQLQuery *Invoice)
{
	TMemoryStream *ReceiptToArchive = new TMemoryStream;
	int Retval = 0;
	try
	{
		modData->SQLQuery2->Close();
		modData->SQLQuery2->SQL->Text = "SELECT GEN_ID(GEN_ARCBILL, 1) NEWKEY FROM RDB$DATABASE";
		modData->SQLQuery2->Open();
		Retval = modData->SQLQuery2->FieldByName("NEWKEY")->AsInteger;
		modData->SQLQuery2->Close();

		modData->SQLQuery2->Close();
		modData->SQLQuery2->SQL->Text =
			"INSERT INTO ARCBILL ("
				"ARCBILL_KEY, "
				"TERMINAL_NAME, "
				"STAFF_NAME, "
				"TIME_STAMP, "
				"TOTAL, "
				"DISCOUNT, "
				"PATRON_COUNT, "
				"RECEIPT, "
				"SECURITY_REF, "
				"SALES_TYPE, "
				"BILLED_LOCATION, "
				"INVOICE_NUMBER, "
				"INVOICE_KEY) "
			"VALUES ("
				":ARCBILL_KEY, "
				":TERMINAL_NAME, "
				":STAFF_NAME, "
				":TIME_STAMP, "
				":TOTAL, "
				":DISCOUNT, "
				":PATRON_COUNT, "
				":RECEIPT, "
				":SECURITY_REF, "
				":SALES_TYPE, "
				":BILLED_LOCATION,"
				":INVOICE_NUMBER, "
				":INVOICE_KEY) ";
		modData->SQLQuery2->ParamByName("ARCBILL_KEY")->AsString	 		= Retval;
		modData->SQLQuery2->ParamByName("TERMINAL_NAME")->AsString	 	= Invoice->FieldByName("TERMINAL_NAME")->AsString;
		modData->SQLQuery2->ParamByName("STAFF_NAME")->AsString		 	= Invoice->FieldByName("STAFF_NAME")->AsString;
		modData->SQLQuery2->ParamByName("TIME_STAMP")->AsSQLTimeStamp	= Invoice->FieldByName("TIME_STAMP")->AsSQLTimeStamp;
		modData->SQLQuery2->ParamByName("TOTAL")->AsCurrency			 	= Invoice->FieldByName("TOTAL")->AsCurrency;
		modData->SQLQuery2->ParamByName("DISCOUNT")->AsCurrency		  	= Invoice->FieldByName("DISCOUNT")->AsCurrency;
		modData->SQLQuery2->ParamByName("PATRON_COUNT")->AsInteger	 	= Invoice->FieldByName("PATRON_COUNT")->AsInteger;
		modData->SQLQuery2->ParamByName("BILLED_LOCATION")->AsString 	= Invoice->FieldByName("BILLED_LOCATION")->AsString;
		modData->SQLQuery2->ParamByName("SECURITY_REF")->AsInteger  	= CopySecurityRef(Invoice->FieldByName("SECURITY_REF")->AsInteger);
		modData->SQLQuery2->ParamByName("INVOICE_NUMBER")->AsString 	= Invoice->FieldByName("INVOICE_NUMBER")->AsString;
		modData->SQLQuery2->ParamByName("SALES_TYPE")->AsInteger 	 	= Invoice->FieldByName("SALES_TYPE")->AsInteger;
		modData->SQLQuery2->ParamByName("INVOICE_KEY")->AsInteger 	 	= 0;
		ReceiptToArchive->Clear();
		TBlobField *ReceiptField = (TBlobField *)Invoice->FieldByName("RECEIPT");
		ReceiptField->SaveToStream(ReceiptToArchive);
		ReceiptToArchive->Position = 0;
		modData->SQLQuery2->ParamByName("RECEIPT")->LoadFromStream(ReceiptToArchive,ftBlob);
		modData->SQLQuery2->ExecSQL();
	}
	__finally
	{
		delete ReceiptToArchive;
	}
	return Retval;
}

int TfrmMain::CopyArcBillPay(TSQLQuery *Invoice, int ArcBillKey)
{
	int Retval = 0;
	modData->SQLQueryB->SQL->Clear();
	modData->SQLQueryB->Params->Clear();
	modData->SQLQueryB->ParamCheck = true;
	modData->SQLQueryB->SQL->Text =
		"SELECT *"
		" FROM "
		" ARCBILLPAY "
		" WHERE "
		" ARCBILL_KEY = :ARCBILL_KEY";
	modData->SQLQueryB->ParamByName("ARCBILL_KEY")->AsInteger = Invoice->FieldByName("ARCBILL_KEY")->AsInteger;
	modData->SQLQueryB->Open();
	for (modData->SQLQueryB->First(); !modData->SQLQueryB->Eof ; modData->SQLQueryB->Next())
	{
		modData->SQLQuery2->Close();
		modData->SQLQuery2->SQL->Text = "SELECT GEN_ID(GEN_ARCBILLPAY, 1) NEWKEY FROM RDB$DATABASE";
		modData->SQLQuery2->Open();
		Retval = modData->SQLQuery2->FieldByName("NEWKEY")->AsInteger;
		modData->SQLQuery2->Close();

		modData->SQLQuery2->Close();
		modData->SQLQuery2->SQL->Text =
			"INSERT INTO ARCBILLPAY ("
				"ARCBILLPAY_KEY, "
				"ARCBILL_KEY, "
				"PAY_TYPE, "
				"SUBTOTAL, "
				"VOUCHER_NUMBER, "
				"CASH_OUT, "
				"NOTE, "
				"TAX_FREE, "
				"GROUP_NUMBER, "
				"PROPERTIES, "
				"PAY_TYPE_DETAILS,"
            "ROUNDING) "
			"VALUES ("
				":ARCBILLPAY_KEY, "
				":ARCBILL_KEY, "
				":PAY_TYPE, "
				":SUBTOTAL, "
				":VOUCHER_NUMBER, "
				":CASH_OUT, "
				":NOTE, "
				":TAX_FREE, "
				":GROUP_NUMBER, "
				":PROPERTIES, "
				":PAY_TYPE_DETAILS,"
            ":ROUNDING) ";

		modData->SQLQuery2->ParamByName("ARCBILLPAY_KEY")->AsInteger	= Retval;
		modData->SQLQuery2->ParamByName("ARCBILL_KEY")->AsInteger		= ArcBillKey;
		modData->SQLQuery2->ParamByName("PAY_TYPE")->AsString				= modData->SQLQueryB->FieldByName("PAY_TYPE")->AsString;
		modData->SQLQuery2->ParamByName("SUBTOTAL")->AsCurrency		  	= modData->SQLQueryB->FieldByName("SUBTOTAL")->AsCurrency;
		modData->SQLQuery2->ParamByName("VOUCHER_NUMBER")->AsString   	= modData->SQLQueryB->FieldByName("VOUCHER_NUMBER")->AsString;
		modData->SQLQuery2->ParamByName("CASH_OUT")->AsString				= DBBool(modData->SQLQueryB->FieldByName("CASH_OUT")->AsString);
		modData->SQLQuery2->ParamByName("NOTE")->AsString				   = modData->SQLQueryB->FieldByName("NOTE")->AsString;
		modData->SQLQuery2->ParamByName("TAX_FREE")->AsString				= DBBool(modData->SQLQueryB->FieldByName("TAX_FREE")->AsString);
		modData->SQLQuery2->ParamByName("GROUP_NUMBER")->AsInteger	   = modData->SQLQueryB->FieldByName("GROUP_NUMBER")->AsInteger;
		modData->SQLQuery2->ParamByName("PROPERTIES")->AsInteger	      = modData->SQLQueryB->FieldByName("PROPERTIES")->AsInteger;
		modData->SQLQuery2->ParamByName("PAY_TYPE_DETAILS")->AsString	= modData->SQLQueryB->FieldByName("PAY_TYPE_DETAILS")->AsString;
		modData->SQLQuery2->ParamByName("ROUNDING")->AsCurrency		  	= modData->SQLQueryB->FieldByName("ROUNDING")->AsCurrency;
		modData->SQLQuery2->ExecSQL();
	}
	return Retval;
}

int TfrmMain::CopyArcRef(TSQLQuery *Invoice, int ArcBillKey)
{
	int Retval = 0;
	modData->SQLQueryB->SQL->Clear();
	modData->SQLQueryB->Params->Clear();
	modData->SQLQueryB->ParamCheck = true;
	modData->SQLQueryB->SQL->Text =
		"SELECT *"
		" FROM "
		" ARCREF "
		" WHERE "
		" ARCBILL_KEY = :ARCBILL_KEY";
	modData->SQLQueryB->ParamByName("ARCBILL_KEY")->AsInteger = Invoice->FieldByName("ARCBILL_KEY")->AsInteger;
	modData->SQLQueryB->Open();
	for (modData->SQLQueryB->First(); !modData->SQLQueryB->Eof ; modData->SQLQueryB->Next())
	{
		modData->SQLQuery2->Close();
		modData->SQLQuery2->SQL->Text = "SELECT GEN_ID(GEN_ARCREF, 1) NEWKEY FROM RDB$DATABASE";
		modData->SQLQuery2->Open();
		Retval = modData->SQLQuery2->FieldByName("NEWKEY")->AsInteger;
		modData->SQLQuery2->Close();

		modData->SQLQuery2->Close();
		modData->SQLQuery2->SQL->Text =
			"INSERT INTO ARCREF ("
				"ARCREF_KEY, "
				"ARCBILL_KEY, "
				"REFS_KEY, "
				"REFERENCE) "
			"VALUES ("
				":ARCREF_KEY, "
				":ARCBILL_KEY, "
				":REFS_KEY, "
				":REFERENCE) ";

		modData->SQLQuery2->ParamByName("ARCREF_KEY")->AsInteger	      = Retval;
		modData->SQLQuery2->ParamByName("ARCBILL_KEY")->AsInteger		= ArcBillKey;
		modData->SQLQuery2->ParamByName("REFS_KEY")->AsString				= modData->SQLQueryB->FieldByName("REFS_KEY")->AsInteger;
		modData->SQLQuery2->ParamByName("REFERENCE")->AsCurrency		  	= modData->SQLQueryB->FieldByName("REFERENCE")->AsString;
		modData->SQLQuery2->ExecSQL();
	}
	return Retval;
}

AnsiString TfrmMain::DBBool(AnsiString BoolVal)
{
	AnsiString RetVal = "F";
	if(BoolVal == "T" || BoolVal == "F" )
	{
		RetVal = BoolVal;
	}
	return RetVal;
}

int TfrmMain::CopyArcSurcharge(TSQLQuery *Invoice, int ArcBillKey)
{
	int Retval = 0;
	modData->SQLQueryB->SQL->Clear();
	modData->SQLQueryB->Params->Clear();
	modData->SQLQueryB->ParamCheck = true;
	modData->SQLQueryB->SQL->Text =
		"SELECT *"
		" FROM "
		" ARCSURCHARGE "
		" WHERE "
		" ARCBILL_KEY = :ARCBILL_KEY";
	modData->SQLQueryB->ParamByName("ARCBILL_KEY")->AsInteger = Invoice->FieldByName("ARCBILL_KEY")->AsInteger;
	modData->SQLQueryB->Open();
	for (modData->SQLQueryB->First(); !modData->SQLQueryB->Eof ; modData->SQLQueryB->Next())
	{
		modData->SQLQuery2->Close();
		modData->SQLQuery2->SQL->Text = "SELECT GEN_ID(GEN_ARCSURCHARGE, 1) NEWKEY FROM RDB$DATABASE";
		modData->SQLQuery2->Open();
		Retval = modData->SQLQuery2->FieldByName("NEWKEY")->AsInteger;
		modData->SQLQuery2->Close();

		modData->SQLQuery2->Close();
		modData->SQLQuery2->SQL->Text =
			"INSERT INTO ARCSURCHARGE ("
				"SURCHARGE_KEY, "
				"ARCBILL_KEY, "
				"PAY_TYPE, "
				"PAY_TYPE_DETAILS, "
				"SUBTOTAL, "
				"NOTE, "
				"TAX_FREE, "
				"PROPERTIES, "
				"GROUP_NUMBER, "
            "ROUNDING) "
			"VALUES ("
				":SURCHARGE_KEY, "
				":ARCBILL_KEY, "
				":PAY_TYPE, "
				":PAY_TYPE_DETAILS, "
				":SUBTOTAL, "
				":NOTE, "
				":TAX_FREE, "
				":PROPERTIES, "
				":GROUP_NUMBER, "
            ":ROUNDING) ";

		modData->SQLQuery2->ParamByName("SURCHARGE_KEY")->AsInteger		= Retval;
		modData->SQLQuery2->ParamByName("ARCBILL_KEY")->AsInteger		= ArcBillKey;
		modData->SQLQuery2->ParamByName("PAY_TYPE")->AsString				= modData->SQLQueryB->FieldByName("PAY_TYPE")->AsString;
		modData->SQLQuery2->ParamByName("PAY_TYPE_DETAILS")->AsString  = modData->SQLQueryB->FieldByName("PAY_TYPE_DETAILS")->AsString;
		modData->SQLQuery2->ParamByName("SUBTOTAL")->AsCurrency		  	= modData->SQLQueryB->FieldByName("SUBTOTAL")->AsCurrency;
		modData->SQLQuery2->ParamByName("NOTE")->AsString				   = modData->SQLQueryB->FieldByName("NOTE")->AsString;
		modData->SQLQuery2->ParamByName("TAX_FREE")->AsString				= DBBool(modData->SQLQueryB->FieldByName("TAX_FREE")->AsString);
		modData->SQLQuery2->ParamByName("PROPERTIES")->AsInteger	      = modData->SQLQueryB->FieldByName("PROPERTIES")->AsInteger;
		modData->SQLQuery2->ParamByName("GROUP_NUMBER")->AsInteger	   = modData->SQLQueryB->FieldByName("GROUP_NUMBER")->AsInteger;
		modData->SQLQuery2->ExecSQL();
	}
	return Retval;
}

void TfrmMain::CopyArchive(TSQLQuery *Invoice,int ArcBillKey)
{
	int Retval;
	modData->SQLQueryB->SQL->Clear();
	modData->SQLQueryB->Params->Clear();
	modData->SQLQueryB->ParamCheck = true;
	modData->SQLQueryB->SQL->Text =
		"SELECT *"
		" FROM "
		" ARCHIVE "
		" WHERE "
		" ARCBILL_KEY = :ARCBILL_KEY";
	modData->SQLQueryB->ParamByName("ARCBILL_KEY")->AsInteger = Invoice->FieldByName("ARCBILL_KEY")->AsInteger;
	modData->SQLQueryB->Open();
	for (modData->SQLQueryB->First(); !modData->SQLQueryB->Eof ; modData->SQLQueryB->Next())
	{
		modData->SQLQuery2->Close();
		modData->SQLQuery2->SQL->Text = "SELECT GEN_ID(GEN_ARCHIVE, 1) NEWKEY FROM RDB$DATABASE";
		modData->SQLQuery2->Open();
		Retval = modData->SQLQuery2->FieldByName("NEWKEY")->AsInteger;
		modData->SQLQuery2->Close();

		modData->SQLQuery2->Close();
		modData->SQLQuery2->Params->Clear();
		modData->SQLQuery2->ParamCheck = true;
		modData->SQLQuery2->SQL->Text =
			"INSERT INTO ARCHIVE ( "
			"ARCHIVE_KEY,"
			"ARCBILL_KEY,"
			"TERMINAL_NAME,"
			"MENU_NAME,"
			"COURSE_NAME,"
			"ITEM_NAME,"
			"ITEM_SHORT_NAME,"
			"ITEM_ID,"
			"ITEM_CATEGORY,"
			"SIZE_NAME,"
			"TABLE_NUMBER,"
			"TABLE_NAME,"
			"SEAT_NUMBER,"
			"SERVER_NAME,"
			"TAB_NAME,"
			"LOYALTY_NAME,"
			"ORDER_TYPE,"
			"TIME_STAMP,"
			"TIME_STAMP_BILLED,"
			"ORDER_LOCATION,"
			"PRICE,"
			"COST,"
			"HAPPY_HOUR,"
			"NOTE,"
			"SECURITY_REF,"
			"TIME_KEY,"
			"GST_PERCENT,"
			"COST_GST_PERCENT,"
			"QTY,"
			"DISCOUNT,"
			"REDEEMED,"
			"POINTS_PERCENT,"
			"POINTS_EARNED,"
			"LOYALTY_KEY,"
			"THIRDPARTYCODES_KEY,"
			"CATEGORY_KEY,"
         "DISCOUNT_REASON,"
         "PRICE_LEVEL0,"
         "PRICE_LEVEL1,"
         "SERVINGCOURSES_KEY) "
			"VALUES ("
			":ARCHIVE_KEY,"
			":ARCBILL_KEY,"
			":TERMINAL_NAME,"
			":MENU_NAME,"
			":COURSE_NAME,"
			":ITEM_NAME,"
			":ITEM_SHORT_NAME,"
			":ITEM_ID,"
			":ITEM_CATEGORY,"
			":SIZE_NAME,"
			":TABLE_NUMBER,"
			":TABLE_NAME,"
			":SEAT_NUMBER,"
			":SERVER_NAME,"
			":TAB_NAME,"
			":LOYALTY_NAME,"
			":ORDER_TYPE,"
			":TIME_STAMP,"
			":TIME_STAMP_BILLED,"
			":ORDER_LOCATION,"
			":PRICE,"
			":COST,"
			":HAPPY_HOUR,"
			":NOTE,"
			":SECURITY_REF,"
			":TIME_KEY,"
			":GST_PERCENT,"
			":COST_GST_PERCENT,"
			":QTY,"
			":DISCOUNT,"
			":REDEEMED,"
			":POINTS_PERCENT,"
			":POINTS_EARNED,"
			":LOYALTY_KEY,"
			":THIRDPARTYCODES_KEY,"
			":CATEGORY_KEY,"
         ":DISCOUNT_REASON,"
         ":PRICE_LEVEL0,"
         ":PRICE_LEVEL1,"
         ":SERVINGCOURSES_KEY) ";


		modData->SQLQuery2->ParamByName("ARCHIVE_KEY")->AsInteger			= Retval;
		modData->SQLQuery2->ParamByName("ARCBILL_KEY")->AsInteger			= ArcBillKey;
		modData->SQLQuery2->ParamByName("TERMINAL_NAME")->AsString			= modData->SQLQueryB->FieldByName("TERMINAL_NAME")->AsString;
		modData->SQLQuery2->ParamByName("MENU_NAME")->AsString				= modData->SQLQueryB->FieldByName("MENU_NAME")->AsString;
		modData->SQLQuery2->ParamByName("COURSE_NAME")->AsString				= modData->SQLQueryB->FieldByName("COURSE_NAME")->AsString;
		modData->SQLQuery2->ParamByName("ITEM_NAME")->AsString				= modData->SQLQueryB->FieldByName("ITEM_NAME")->AsString;
		modData->SQLQuery2->ParamByName("ITEM_SHORT_NAME")->AsString		= modData->SQLQueryB->FieldByName("ITEM_SHORT_NAME")->AsString;
		modData->SQLQuery2->ParamByName("ITEM_ID")->AsInteger					= modData->SQLQueryB->FieldByName("ITEM_ID")->AsInteger;
		modData->SQLQuery2->ParamByName("ITEM_CATEGORY")->AsString			= modData->SQLQueryB->FieldByName("ITEM_CATEGORY")->AsString;
		modData->SQLQuery2->ParamByName("SIZE_NAME")->AsString				= modData->SQLQueryB->FieldByName("SIZE_NAME")->AsString;
		modData->SQLQuery2->ParamByName("TABLE_NUMBER")->AsInteger			= modData->SQLQueryB->FieldByName("TABLE_NUMBER")->AsInteger;
		modData->SQLQuery2->ParamByName("TABLE_NAME")->AsString				= modData->SQLQueryB->FieldByName("TABLE_NAME")->AsString;
		modData->SQLQuery2->ParamByName("SEAT_NUMBER")->AsInteger			= modData->SQLQueryB->FieldByName("SEAT_NUMBER")->AsInteger;
		modData->SQLQuery2->ParamByName("SERVER_NAME")->AsString				= modData->SQLQueryB->FieldByName("SERVER_NAME")->AsString;
		modData->SQLQuery2->ParamByName("TAB_NAME")->AsString					= modData->SQLQueryB->FieldByName("TAB_NAME")->AsString;
		modData->SQLQuery2->ParamByName("LOYALTY_NAME")->AsString			= modData->SQLQueryB->FieldByName("LOYALTY_NAME")->AsString;
		modData->SQLQuery2->ParamByName("ORDER_TYPE")->AsInteger				= modData->SQLQueryB->FieldByName("ORDER_TYPE")->AsInteger;
		modData->SQLQuery2->ParamByName("TIME_STAMP")->AsSQLTimeStamp			= modData->SQLQueryB->FieldByName("TIME_STAMP")->AsSQLTimeStamp;
		modData->SQLQuery2->ParamByName("TIME_STAMP_BILLED")->AsSQLTimeStamp	= modData->SQLQueryB->FieldByName("TIME_STAMP_BILLED")->AsSQLTimeStamp;
		modData->SQLQuery2->ParamByName("ORDER_LOCATION")->AsString			= modData->SQLQueryB->FieldByName("ORDER_LOCATION")->AsString;
		modData->SQLQuery2->ParamByName("PRICE")->AsCurrency					= modData->SQLQueryB->FieldByName("PRICE")->AsCurrency;
		modData->SQLQuery2->ParamByName("COST")->AsCurrency					= modData->SQLQueryB->FieldByName("COST")->AsCurrency;
		modData->SQLQuery2->ParamByName("HAPPY_HOUR")->AsString				= DBBool(modData->SQLQueryB->FieldByName("HAPPY_HOUR")->AsString);
		modData->SQLQuery2->ParamByName("NOTE")->AsString				      = modData->SQLQueryB->FieldByName("NOTE")->AsString;
		modData->SQLQuery2->ParamByName("SECURITY_REF")->AsInteger 			= CopySecurityRef(modData->SQLQueryB->FieldByName("SECURITY_REF")->AsInteger);
		modData->SQLQuery2->ParamByName("TIME_KEY")->AsInteger		      = modData->SQLQueryB->FieldByName("TIME_KEY")->AsInteger;
		modData->SQLQuery2->ParamByName("GST_PERCENT")->AsFloat				= modData->SQLQueryB->FieldByName("GST_PERCENT")->AsFloat;
		modData->SQLQuery2->ParamByName("COST_GST_PERCENT")->AsFloat		= modData->SQLQueryB->FieldByName("COST_GST_PERCENT")->AsFloat;
		modData->SQLQuery2->ParamByName("QTY")->AsFloat							= modData->SQLQueryB->FieldByName("QTY")->AsFloat;
		modData->SQLQuery2->ParamByName("DISCOUNT")->AsCurrency           = modData->SQLQueryB->FieldByName("DISCOUNT")->AsCurrency;
		modData->SQLQuery2->ParamByName("REDEEMED")->AsCurrency				= modData->SQLQueryB->FieldByName("REDEEMED")->AsCurrency;
		modData->SQLQuery2->ParamByName("POINTS_PERCENT")->AsFloat			= modData->SQLQueryB->FieldByName("POINTS_PERCENT")->AsFloat;
		modData->SQLQuery2->ParamByName("POINTS_EARNED")->AsFloat 			= modData->SQLQueryB->FieldByName("POINTS_EARNED")->AsFloat;
		modData->SQLQuery2->ParamByName("LOYALTY_KEY")->AsInteger 			= modData->SQLQueryB->FieldByName("LOYALTY_KEY")->AsInteger;
		modData->SQLQuery2->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = CopyThirdPartyCodes(modData->SQLQueryB->FieldByName("THIRDPARTYCODES_KEY")->AsInteger);
		modData->SQLQuery2->ParamByName("CATEGORY_KEY")->AsInteger 			= CopyCategory(modData->SQLQueryB->FieldByName("CATEGORY_KEY")->AsInteger);
		modData->SQLQuery2->ParamByName("DISCOUNT_REASON")->AsString		= modData->SQLQueryB->FieldByName("DISCOUNT_REASON")->AsString;
		modData->SQLQuery2->ParamByName("PRICE_LEVEL0")->AsCurrency       = modData->SQLQueryB->FieldByName("PRICE_LEVEL0")->AsCurrency;
		modData->SQLQuery2->ParamByName("PRICE_LEVEL1")->AsCurrency       = modData->SQLQueryB->FieldByName("PRICE_LEVEL1")->AsCurrency;
		modData->SQLQuery2->ParamByName("SERVINGCOURSES_KEY")->AsInteger 	= CopyServingCourse(modData->SQLQueryB->FieldByName("SERVINGCOURSES_KEY")->AsInteger);
		modData->SQLQuery2->ExecSQL();

		CopyArchiveCats(Retval,modData->SQLQueryB->FieldByName("ARCHIVE_KEY")->AsInteger);

	}
}

int TfrmMain::CopyThirdPartyCodes(int ThridPartyCodeKey)
{
	int RetVal = 0;
	modData->SQLQueryC->SQL->Clear();
	modData->SQLQueryC->Params->Clear();
	modData->SQLQueryC->ParamCheck = true;
	modData->SQLQueryC->SQL->Text =
	" SELECT * FROM THIRDPARTYCODES "
	" WHERE THIRDPARTYCODES_KEY = :THIRDPARTYCODES_KEY";
	modData->SQLQueryC->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = ThridPartyCodeKey;
	modData->SQLQueryC->Open();
	if(!modData->SQLQueryC->IsEmpty())
	{
		modData->SQLQuery3->SQL->Clear();
		modData->SQLQuery3->Params->Clear();
		modData->SQLQuery3->ParamCheck = true;
		modData->SQLQuery3->SQL->Text =
		" SELECT THIRDPARTYCODES_KEY FROM THIRDPARTYCODES "
		" WHERE CODE = :CODE AND "
		" CODETYPE = :CODETYPE";
		modData->SQLQuery3->ParamByName("CODE")->AsString =
			modData->SQLQueryC->FieldByName("CODE")->AsString;
		modData->SQLQuery3->ParamByName("CODETYPE")->AsInteger =
			modData->SQLQueryC->FieldByName("CODETYPE")->AsInteger;
		modData->SQLQuery3->Open();
		if(modData->SQLQuery3->IsEmpty())
		{
			modData->SQLQuery3->Close();
			modData->SQLQuery3->SQL->Text = "SELECT GEN_ID(GEN_THIRDPARTYCODES, 1) NEXTKEY FROM RDB$DATABASE";
			modData->SQLQuery3->Open();
			RetVal = modData->SQLQuery3->FieldByName("NEXTKEY")->AsInteger;

			modData->SQLQuery3->Close();
			modData->SQLQuery3->SQL->Text =
				"INSERT INTO THIRDPARTYCODES ("
					"THIRDPARTYCODES_KEY,"
					"CODE,"
					"CODETYPE,"
					"VISIBLE,"
					"DESCRIPTION) "
				"VALUES ("
					":THIRDPARTYCODES_KEY,"
					":CODE,"
					":CODETYPE,"
					":VISIBLE,"
					":DESCRIPTION);";
			modData->SQLQuery3->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = RetVal;
			modData->SQLQuery3->ParamByName("CODE")->AsString = modData->SQLQueryC->FieldByName("CODE")->AsString;
			modData->SQLQuery3->ParamByName("CODETYPE")->AsInteger = modData->SQLQueryC->FieldByName("CODETYPE")->AsInteger;
			modData->SQLQuery3->ParamByName("VISIBLE")->AsString = modData->SQLQueryC->FieldByName("VISIBLE")->AsString;
			modData->SQLQuery3->ParamByName("DESCRIPTION")->AsString = modData->SQLQueryC->FieldByName("DESCRIPTION")->AsString;
			modData->SQLQuery3->ExecSQL();
		}
		else
		{
			RetVal = modData->SQLQuery3->FieldByName("THIRDPARTYCODES_KEY")->AsInteger;
		}
	}
	return RetVal;
}

int TfrmMain::CopyServingCourse(int ServingCourseKey)
{
	int RetVal = 0;
	modData->SQLQueryC->SQL->Clear();
	modData->SQLQueryC->Params->Clear();
	modData->SQLQueryC->ParamCheck = true;
	modData->SQLQueryC->SQL->Text =
	" SELECT * FROM SERVINGCOURSES "
	" WHERE SERVINGCOURSES_KEY = :SERVINGCOURSES_KEY";
	modData->SQLQueryC->ParamByName("SERVINGCOURSES_KEY")->AsInteger = ServingCourseKey;
	modData->SQLQueryC->Open();
	if(!modData->SQLQueryC->IsEmpty())
	{
		modData->SQLQuery3->SQL->Clear();
		modData->SQLQuery3->Params->Clear();
		modData->SQLQuery3->ParamCheck = true;
		modData->SQLQuery3->SQL->Text =
		" SELECT SERVINGCOURSES_KEY FROM SERVINGCOURSES "
		" WHERE SERVINGCOURSE_NAME = :SERVINGCOURSE_NAME";
		modData->SQLQuery3->ParamByName("SERVINGCOURSE_NAME")->AsString =
			modData->SQLQueryC->FieldByName("SERVINGCOURSE_NAME")->AsString;
		modData->SQLQuery3->Open();
		if(modData->SQLQuery3->IsEmpty())
		{
			modData->SQLQuery3->Close();
			modData->SQLQuery3->SQL->Text = "SELECT GEN_ID(GEN_SERVINGCOURSES, 1) NEXTKEY FROM RDB$DATABASE";
			modData->SQLQuery3->Open();
			RetVal = modData->SQLQuery3->FieldByName("NEXTKEY")->AsInteger;

			modData->SQLQuery3->Close();
			modData->SQLQuery3->SQL->Text =
				"INSERT INTO SERVINGCOURSES ("
					"SERVINGCOURSES_KEY,"
					"SERVINGCOURSE_NAME,"
					"SERVINGCOURSE_KITCHEN_NAME,"
					"DELETED,"
					"SELECTABLE,"
					"DISPLAY_ORDER,"
					"COLOUR) "
				"VALUES ("
					":SERVINGCOURSES_KEY,"
					":SERVINGCOURSE_NAME,"
					":SERVINGCOURSE_KITCHEN_NAME,"
					":DELETED,"
					":SELECTABLE,"
               ":DISPLAY_ORDER,"
               ":COLOUR);";
			modData->SQLQuery3->ParamByName("SERVINGCOURSES_KEY")->AsInteger = RetVal;
			modData->SQLQuery3->ParamByName("SERVINGCOURSE_NAME")->AsString = modData->SQLQueryC->FieldByName("SERVINGCOURSE_NAME")->AsString;
			modData->SQLQuery3->ParamByName("SERVINGCOURSE_KITCHEN_NAME")->AsString = modData->SQLQueryC->FieldByName("SERVINGCOURSE_KITCHEN_NAME")->AsString;
			modData->SQLQuery3->ParamByName("DELETED")->AsString = modData->SQLQueryC->FieldByName("DELETED")->AsString;
			modData->SQLQuery3->ParamByName("SELECTABLE")->AsString = modData->SQLQueryC->FieldByName("SELECTABLE")->AsString;
			modData->SQLQuery3->ParamByName("DISPLAY_ORDER")->AsInteger = modData->SQLQueryC->FieldByName("DISPLAY_ORDER")->AsInteger;
			modData->SQLQuery3->ParamByName("COLOUR")->AsInteger = modData->SQLQueryC->FieldByName("COLOUR")->AsInteger;
			modData->SQLQuery3->ExecSQL();
		}
		else
		{
			RetVal = modData->SQLQuery3->FieldByName("SERVINGCOURSES_KEY")->AsInteger;
		}
	}
	return RetVal;
}


int TfrmMain::CopySecurityRef(int SourceSecurityRef)
{
	int RetVal = 0;
   modData->SQLQueryC->SQL->Clear();
	modData->SQLQueryC->Params->Clear();
	modData->SQLQueryC->ParamCheck = true;
	modData->SQLQueryC->SQL->Text =
	" SELECT * FROM SECURITY "
	" WHERE SECURITY_REF = :SECURITY_REF";
	modData->SQLQueryC->ParamByName("SECURITY_REF")->AsInteger = SourceSecurityRef;
	modData->SQLQueryC->Open();   
	if(!modData->SQLQueryC->IsEmpty())
	{
      modData->SQLQuery3->Close();
      modData->SQLQuery3->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_REF, 1) NEXTKEY FROM RDB$DATABASE";
      modData->SQLQuery3->Open();
      RetVal = modData->SQLQuery3->FieldByName("NEXTKEY")->AsInteger;

      for (modData->SQLQueryC->First(); !modData->SQLQueryC->Eof ; modData->SQLQueryC->Next())
      {
         int SecurityKey;
         modData->SQLQuery3->Close();
         modData->SQLQuery3->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_KEY, 1) SECKEY FROM RDB$DATABASE";
         modData->SQLQuery3->Open();
         SecurityKey = modData->SQLQuery3->FieldByName("SECKEY")->AsInteger;

         modData->SQLQuery3->Close();
         modData->SQLQuery3->SQL->Text =
            "INSERT INTO SECURITY ("
               "SECURITY_KEY,"
               "SECURITY_REF,"
               "SECURITY_EVENT,"
               "FROM_VAL,"
               "TO_VAL,"
               "NOTE,"
               "TERMINAL_NAME,"
               "USER_KEY,"
               "TIME_STAMP) "
            "VALUES ("
               ":SECURITY_KEY,"
               ":SECURITY_REF,"
               ":SECURITY_EVENT,"
               ":FROM_VAL,"
               ":TO_VAL,"
               ":NOTE,"
               ":TERMINAL_NAME,"
               ":USER_KEY,"
               ":TIME_STAMP);";
         modData->SQLQuery3->ParamByName("SECURITY_KEY")->AsInteger     = SecurityKey;
         modData->SQLQuery3->ParamByName("SECURITY_REF")->AsInteger     = RetVal;
         modData->SQLQuery3->ParamByName("SECURITY_EVENT")->AsString    = modData->SQLQueryC->FieldByName("SECURITY_EVENT")->AsString;
         modData->SQLQuery3->ParamByName("FROM_VAL")->AsString          = modData->SQLQueryC->FieldByName("FROM_VAL")->AsString;
         modData->SQLQuery3->ParamByName("TO_VAL")->AsString            = modData->SQLQueryC->FieldByName("TO_VAL")->AsString;
         modData->SQLQuery3->ParamByName("NOTE")->AsString              = modData->SQLQueryC->FieldByName("NOTE")->AsString;
         modData->SQLQuery3->ParamByName("TERMINAL_NAME")->AsString     = modData->SQLQueryC->FieldByName("TERMINAL_NAME")->AsString;
         modData->SQLQuery3->ParamByName("USER_KEY")->AsInteger         = StrToIntDef(edContactKey->Text,1);
         modData->SQLQuery3->ParamByName("TIME_STAMP")->AsSQLTimeStamp  = modData->SQLQueryC->FieldByName("TIME_STAMP")->AsSQLTimeStamp;
         modData->SQLQuery3->ExecSQL();
      }
   }
   return RetVal;
}

int TfrmMain::CopyCategory(int CatKey)
{
	/* Get the CATEGORY out of the Source Via its Key.*/
	modData->SQLQueryC->SQL->Clear();
	modData->SQLQueryC->Params->Clear();
	modData->SQLQueryC->ParamCheck = true;
	modData->SQLQueryC->SQL->Text =
	" SELECT CATEGORY_KEY,CATEGORY FROM ARCCATEGORIES "
	" WHERE CATEGORY_KEY = :CATEGORY_KEY";
	modData->SQLQueryC->ParamByName("CATEGORY_KEY")->AsInteger = CatKey;
	modData->SQLQueryC->Open();   
	/* Ensure it exists in the Destination.*/
	return GetOrCreateCat(modData->SQLQueryC->FieldByName("CATEGORY")->AsString);
}



void TfrmMain::CopyArchiveCats(int NewArchiveKey,int OldArchiveKey)
{
	modData->SQLQueryC->SQL->Clear();
	modData->SQLQueryC->Params->Clear();
	modData->SQLQueryC->ParamCheck = true;
	modData->SQLQueryC->SQL->Text =
	" SELECT ARCHIVE_KEY,CATEGORY_KEY,ARCCATEGORIES.CATEGORY FROM ARCCATEGORY "
	" LEFT JOIN ARCCATEGORIES ON ARCCATEGORIES.CATEGORY_KEY = ARCCATEGORY.CATEGORY_KEY "
	" WHERE ARCHIVE_KEY = :ARCHIVE_KEY";
	modData->SQLQueryC->ParamByName("ARCHIVE_KEY")->AsInteger = OldArchiveKey;
	modData->SQLQueryC->Open();
	for (modData->SQLQueryC->First(); !modData->SQLQueryC->Eof ; modData->SQLQueryC->Next())
	{
		int NewCatKey = GetOrCreateCat(modData->SQLQueryC->FieldByName("CATEGORY")->AsString);

		modData->SQLQuery2->Close();
		modData->SQLQuery2->SQL->Text =
			"INSERT INTO ARCCATEGORY ("
				"ARCHIVE_KEY, "
				"CATEGORY_KEY) "
			"VALUES ("
				":ARCHIVE_KEY, "
				":CATEGORY_KEY) ";

		modData->SQLQuery2->ParamByName("ARCHIVE_KEY")->AsInteger		= NewArchiveKey;
		modData->SQLQuery2->ParamByName("CATEGORY_KEY")->AsInteger		= NewCatKey;
		modData->SQLQuery2->ExecSQL();
	}
}

int TfrmMain::GetOrCreateCat(AnsiString Category)
{
	int RetVal;
	modData->SQLQuery3->SQL->Clear();
	modData->SQLQuery3->Params->Clear();
	modData->SQLQuery3->ParamCheck = true;
	modData->SQLQuery3->SQL->Text =
	" SELECT CATEGORY_KEY,CATEGORY FROM ARCCATEGORIES "
	" WHERE CATEGORY = :CATEGORY";
	modData->SQLQuery3->ParamByName("CATEGORY")->AsString = Category;
	modData->SQLQuery3->Open();

	if(modData->SQLQuery3->IsEmpty())
	{
		modData->SQLQuery3->Close();
		modData->SQLQuery3->SQL->Text = "SELECT GEN_ID(GEN_ARCCATEGORIES, 1) NEWKEY FROM RDB$DATABASE";
		modData->SQLQuery3->Open();
		RetVal = modData->SQLQuery3->FieldByName("NEWKEY")->AsInteger;
		modData->SQLQuery3->Close();

		modData->SQLQuery3->Close();
		modData->SQLQuery3->SQL->Text =
			"INSERT INTO ARCCATEGORIES ("
				"CATEGORY_KEY, "
				"CATEGORY,"
				"CATEGORYGROUPS_KEY,"
				"COO)"
			"VALUES ("
				":CATEGORY_KEY, "
				":CATEGORY, "
				":CATEGORYGROUPS_KEY, "
				":COO) ";

		modData->SQLQuery3->ParamByName("CATEGORY_KEY")->AsInteger = RetVal;
		modData->SQLQuery3->ParamByName("CATEGORY")->AsString  = Category;
		modData->SQLQuery3->ParamByName("CATEGORYGROUPS_KEY")->AsInteger = GetOrCreateCatGroup(Category);
		modData->SQLQuery3->ParamByName("COO")->AsInteger = 0;
		modData->SQLQuery3->ExecSQL();
	}
	else
	{
		RetVal = modData->SQLQuery3->FieldByName("CATEGORY_KEY")->AsInteger;
	}
	return RetVal;
}

int TfrmMain::GetOrCreateCatGroup(AnsiString Category)
{
	/* If we cannot find a Category group in the Source DB for this Category
	we will add it to All. however look it up and see first.*/
	AnsiString CategoryGroup = GetSourceCatGroupForCategory(Category);
	if(CategoryGroup == "" )
	{
   	CategoryGroup = "ALL";
	}

	int RetVal;
	modData->SQLQuery4->SQL->Clear();
	modData->SQLQuery4->Params->Clear();
	modData->SQLQuery4->ParamCheck = true;
	modData->SQLQuery4->SQL->Text =
	" SELECT CATEGORYGROUPS_KEY,NAME FROM CATEGORYGROUPS "
	" WHERE NAME = :NAME";
	modData->SQLQuery4->ParamByName("NAME")->AsString = CategoryGroup;
	modData->SQLQuery4->Open();

	if(modData->SQLQuery4->IsEmpty())
	{
		modData->SQLQuery4->Close();
		modData->SQLQuery4->SQL->Text = "SELECT GEN_ID(GEN_CATEGORYGROUPS, 1) NEWKEY FROM RDB$DATABASE";
		modData->SQLQuery4->Open();
		RetVal = modData->SQLQuery4->FieldByName("NEWKEY")->AsInteger;
		modData->SQLQuery4->Close();

		modData->SQLQuery4->Close();
		modData->SQLQuery4->SQL->Text =
			"INSERT INTO CATEGORYGROUPS ("
				"CATEGORYGROUPS_KEY, "
				"NAME,"
				"VISIBLE)"
			"VALUES ("
				":CATEGORYGROUPS_KEY, "
				":NAME, "
				":VISIBLE) ";

		modData->SQLQuery4->ParamByName("CATEGORYGROUPS_KEY")->AsInteger = RetVal;
		modData->SQLQuery4->ParamByName("NAME")->AsString  = CategoryGroup;
		modData->SQLQuery4->ParamByName("VISIBLE")->AsString = "T";
		modData->SQLQuery4->ExecSQL();
	}
	else
	{
		RetVal = modData->SQLQuery4->FieldByName("CATEGORYGROUPS_KEY")->AsInteger;
	}
	return RetVal;
}


AnsiString TfrmMain::GetSourceCatGroupForCategory(AnsiString Category)
{
	AnsiString RetVal = "";
	modData->SQLQueryC->SQL->Clear();
	modData->SQLQueryC->Params->Clear();
	modData->SQLQueryC->ParamCheck = true;
	modData->SQLQueryC->SQL->Text =
	" SELECT CATEGORYGROUPS.NAME FROM ARCCATEGORIES "
	" LEFT JOIN CATEGORYGROUPS ON ARCCATEGORIES.CATEGORYGROUPS_KEY = CATEGORYGROUPS.CATEGORYGROUPS_KEY "
	" WHERE ARCCATEGORIES.CATEGORY = :CATEGORY";
	modData->SQLQueryC->ParamByName("CATEGORY")->AsString = Category;
	modData->SQLQueryC->Open();
	if(!modData->SQLQueryC->IsEmpty())
	{
		RetVal = modData->SQLQueryC->FieldByName("NAME")->AsString;
	}
	return RetVal;
}


AnsiString TfrmMain::SourceVersion()
{
	int DBMajorVersion = -1;
	int DBMinorVersion = -1;
	GetSourceDataBaseVersion(DBMajorVersion,DBMinorVersion);
	return IntToStr(DBMajorVersion) + "." + IntToStr(DBMinorVersion);
}

AnsiString TfrmMain::DestVersion()
{
	int DBMajorVersion = -1;
	int DBMinorVersion = -1;
	GetDestDataBaseVersion(DBMajorVersion,DBMinorVersion);
	return IntToStr(DBMajorVersion) + "." + IntToStr(DBMinorVersion);
}

void TfrmMain::GetSourceDataBaseVersion(int &DBMajorVersion,int &DBMinorVersion)
{
	modData->SQLQueryC->Close();
	modData->SQLQueryC->SQL->Text =
	"SELECT TIME_STAMP,VERSION_NUMBER FROM VERSIONHISTORY ORDER BY TIME_STAMP DESC";
	modData->SQLQueryC->Open();
	if(!modData->SQLQueryC->IsEmpty())
	{
		AnsiString Ver = modData->SQLQueryC->FieldByName("VERSION_NUMBER")->AsString;
		int FirstSectionEnd = Ver.Pos(".")-1;
		DBMajorVersion = StrToIntDef(Ver.SubString(1,FirstSectionEnd),-1);
		AnsiString RestOfStr = Ver.SubString(Ver.Pos(".")+1,Ver.Length());
		int SecondSectionEnd = RestOfStr.Pos(".")-1;
		if(SecondSectionEnd <= 0) SecondSectionEnd = RestOfStr.Length();
		DBMinorVersion = StrToIntDef(RestOfStr.SubString(1,SecondSectionEnd),-1);
	}
	else
	{
		DBMajorVersion = -1;
		DBMinorVersion = -1;
	}
}

void TfrmMain::GetDestDataBaseVersion(int &DBMajorVersion,int &DBMinorVersion)
{
	modData->SQLQuery3->Close();
	modData->SQLQuery3->SQL->Text =
	"SELECT TIME_STAMP,VERSION_NUMBER FROM VERSIONHISTORY ORDER BY TIME_STAMP DESC";
	modData->SQLQuery3->Open();
	if(!modData->SQLQuery3->IsEmpty())
	{
		AnsiString Ver = modData->SQLQuery3->FieldByName("VERSION_NUMBER")->AsString;
		int FirstSectionEnd = Ver.Pos(".")-1;
		DBMajorVersion = StrToIntDef(Ver.SubString(1,FirstSectionEnd),-1);
		AnsiString RestOfStr = Ver.SubString(Ver.Pos(".")+1,Ver.Length());
		int SecondSectionEnd = RestOfStr.Pos(".")-1;
		if(SecondSectionEnd <= 0) SecondSectionEnd = RestOfStr.Length();
		DBMinorVersion = StrToIntDef(RestOfStr.SubString(1,SecondSectionEnd),-1);
	}
	else
	{
		DBMajorVersion = -1;
		DBMinorVersion = -1;
	}
}

AnsiString TfrmMain::JoinMateVersion()
{
	int MajorVersion = GetFileVersionMajor();
	int MinorVersion = GetFileVersionMinor();
	return IntToStr(MajorVersion) + "." + IntToStr(MinorVersion);
}

void __fastcall TfrmMain::Button1Click(TObject *Sender)
{
	Database::TDBControl DBControl;
   DBControl.Disconnect();
   DBControl.Init(Database::TDBSettings("localhost",lbSource->Caption,false));
   DBControl.Connect();

	Database::TDBTransaction DBTransaction(DBControl);
	try
	{
   	DBTransaction.StartTransaction();
      
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuery2 = DBTransaction.Query(DBTransaction.AddQuery());

		TIBSQL *IBDayArcBill = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBDayArcBillPay = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBDayArchive = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBDayArcSurcharge = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBDayArcRef = DBTransaction.Query(DBTransaction.AddQuery());

		TIBSQL *IBArchive = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArcBill = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArcBillPay = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArcSurcharge = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArcRef = DBTransaction.Query(DBTransaction.AddQuery());

		try
		{
			IBDayArcBill->Close();
			IBDayArcBill->SQL->Text = "select * from DAYARCBILL";

			IBDayArcBillPay->Close();
			IBDayArcBillPay->SQL->Text = "select * from DAYARCBILLPAY where ARCBILL_KEY = :ARCBILL_KEY";

			IBDayArcRef->Close();
			IBDayArcRef->SQL->Text = "select * from DAYARCREF where ARCBILL_KEY = :ARCBILL_KEY";

			IBDayArchive->Close();
			IBDayArchive->SQL->Text = "select * from DAYARCHIVE where ARCBILL_KEY = :ARCBILL_KEY";

			IBDayArcSurcharge->Close();
			IBDayArcSurcharge->SQL->Text = "select * from DAYARCSURCHARGE where ARCBILL_KEY = :ARCBILL_KEY";

			IBArchive->Close();
			IBArchive->SQL->Text =
				"insert into ARCHIVE "
			  "(ARCHIVE.ARCHIVE_KEY, ARCHIVE.ARCBILL_KEY, ARCHIVE.TERMINAL_NAME, "
				"ARCHIVE.MENU_NAME, ARCHIVE.COURSE_NAME, ARCHIVE.ITEM_NAME, "
				"ARCHIVE.ITEM_SHORT_NAME, ARCHIVE.ITEM_ID, ARCHIVE.ITEM_CATEGORY, "
				"ARCHIVE.SIZE_NAME, ARCHIVE.TABLE_NUMBER, ARCHIVE.TABLE_NAME, "
				"ARCHIVE.SEAT_NUMBER, ARCHIVE.SERVER_NAME, ARCHIVE.TAB_NAME, "
				"ARCHIVE.LOYALTY_NAME, ARCHIVE.ORDER_TYPE, ARCHIVE.TIME_STAMP, "
				"ARCHIVE.TIME_STAMP_BILLED, ARCHIVE.ORDER_LOCATION, ARCHIVE.PRICE, "
				"ARCHIVE.COST, ARCHIVE.HAPPY_HOUR, "
				"ARCHIVE.NOTE, ARCHIVE.SECURITY_REF, ARCHIVE.TIME_KEY, ARCHIVE.GST_PERCENT, "
				"ARCHIVE.COST_GST_PERCENT, ARCHIVE.QTY, ARCHIVE.DISCOUNT, "
				"ARCHIVE.REDEEMED, ARCHIVE.POINTS_PERCENT, ARCHIVE.POINTS_EARNED, "
				"ARCHIVE.LOYALTY_KEY,ARCHIVE.THIRDPARTYCODES_KEY,ARCHIVE.CATEGORY_KEY,ARCHIVE.DISCOUNT_REASON, "
				"ARCHIVE.PRICE_LEVEL0, ARCHIVE.PRICE_LEVEL1,ARCHIVE.SERVINGCOURSES_KEY) "
			"values "
			  "(:ARCHIVE_KEY, :ARCBILL_KEY, :TERMINAL_NAME, :MENU_NAME, :COURSE_NAME, "
				":ITEM_NAME, :ITEM_SHORT_NAME, :ITEM_ID, :ITEM_CATEGORY, :SIZE_NAME, "
				":TABLE_NUMBER, :TABLE_NAME, :SEAT_NUMBER, :SERVER_NAME, :TAB_NAME, "
				":LOYALTY_NAME, :ORDER_TYPE, :TIME_STAMP, :TIME_STAMP_BILLED, "
				":ORDER_LOCATION, :PRICE, :COST, :HAPPY_HOUR, "
				":NOTE, :SECURITY_REF, :TIME_KEY, :GST_PERCENT, :COST_GST_PERCENT, "
				":QTY, :DISCOUNT, :REDEEMED, :POINTS_PERCENT, :POINTS_EARNED, "
				":LOYALTY_KEY, :THIRDPARTYCODES_KEY, :CATEGORY_KEY, :DISCOUNT_REASON,"
				":PRICE_LEVEL0, :PRICE_LEVEL1, :SERVINGCOURSES_KEY); ";

			IBArcBill->Close();
			IBArcBill->SQL->Text =
			  "insert into \"ARCBILL\" "
				  "(\"ARCBILL\".\"ARCBILL_KEY\", \"ARCBILL\".\"TERMINAL_NAME\", \"ARCBILL\".\"STAFF_NAME\", "
				  "\"ARCBILL\".\"TIME_STAMP\", \"ARCBILL\".\"TOTAL\", \"ARCBILL\".\"DISCOUNT\", \"ARCBILL\".\"PATRON_COUNT\", "
				  "\"ARCBILL\".\"RECEIPT\", \"ARCBILL\".\"SECURITY_REF\", \"ARCBILL\".\"SALES_TYPE\", "
				  "\"ARCBILL\".\"BILLED_LOCATION\", \"ARCBILL\".\"INVOICE_NUMBER\", \"ARCBILL\".\"INVOICE_KEY\") "
			  "values "
				  "(:\"ARCBILL_KEY\", :\"TERMINAL_NAME\", :\"STAFF_NAME\", :\"TIME_STAMP\", :\"TOTAL\", "
				  ":\"DISCOUNT\", :\"PATRON_COUNT\", :\"RECEIPT\", :\"SECURITY_REF\", :\"SALES_TYPE\", "
				  ":\"BILLED_LOCATION\", :\"INVOICE_NUMBER\", :\"INVOICE_KEY\")";

			IBArcBillPay->Close();
			IBArcBillPay->SQL->Text =
			"insert into \"ARCBILLPAY\" "
			  "(\"ARCBILLPAY\".\"ARCBILLPAY_KEY\", \"ARCBILLPAY\".\"ARCBILL_KEY\", \"ARCBILLPAY\".\"PAY_TYPE\", "
				"\"ARCBILLPAY\".\"SUBTOTAL\", \"ARCBILLPAY\".\"VOUCHER_NUMBER\", \"ARCBILLPAY\".\"CASH_OUT\", "
				"\"ARCBILLPAY\".\"NOTE\", \"ARCBILLPAY\".\"TAX_FREE\", \"ARCBILLPAY\".\"GROUP_NUMBER\", "
				"\"ARCBILLPAY\".\"PROPERTIES\", \"ARCBILLPAY\".\"PAY_TYPE_DETAILS\",\"ARCBILLPAY\".\"ROUNDING\") "
			"values "
			  "(:\"ARCBILLPAY_KEY\", :\"ARCBILL_KEY\", :\"PAY_TYPE\", :\"SUBTOTAL\", :\"VOUCHER_NUMBER\", "
				":\"CASH_OUT\", :\"NOTE\", :\"TAX_FREE\", :\"GROUP_NUMBER\", :\"PROPERTIES\", :\"PAY_TYPE_DETAILS\", :\"ROUNDING\") ";


			IBArcSurcharge->Close();
			IBArcSurcharge->SQL->Text =
			"insert into \"ARCSURCHARGE\" "
			  "(\"ARCSURCHARGE\".\"SURCHARGE_KEY\", \"ARCSURCHARGE\".\"ARCBILL_KEY\", \"ARCSURCHARGE\".\"PAY_TYPE\", "
				"\"ARCSURCHARGE\".\"PAY_TYPE_DETAILS\", \"ARCSURCHARGE\".\"SUBTOTAL\", \"ARCSURCHARGE\".\"NOTE\", "
				"\"ARCSURCHARGE\".\"TAX_FREE\", \"ARCSURCHARGE\".\"PROPERTIES\", \"ARCSURCHARGE\".\"GROUP_NUMBER\",\"ARCSURCHARGE\".\"ROUNDING\") "
			"values "
			  "(:\"SURCHARGE_KEY\", :\"ARCBILL_KEY\", :\"PAY_TYPE\", :\"PAY_TYPE_DETAILS\", "
				":\"SUBTOTAL\", :\"NOTE\", :\"TAX_FREE\", :\"PROPERTIES\", :\"GROUP_NUMBER\", :\"ROUNDING\") ";

			IBArcRef->Close();
			IBArcRef->SQL->Text =
			"insert into \"ARCREF \" "
			  "(\"ARCREF_KEY\", \"ARCBILL_KEY\", \"REFS_KEY\",\"REFERENCE\") "
			"values "
			  "(:\"ARCREF_KEY\", :\"ARCBILL_KEY\", :\"REFS_KEY\", :\"REFERENCE\") ";


			IBDayArcBill->ExecQuery();
			for (;!IBDayArcBill->Eof ; IBDayArcBill->Next())
			{
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCBILL, 1) FROM RDB$DATABASE";
				IBInternalQuery->ExecQuery();
				int ArcBillKey = IBInternalQuery->Fields[0]->AsInteger;

				// Copy all the daily ArcBill Fields to the main archive
				IBArcBill->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
				for (int i=1; i< IBDayArcBill->FieldCount; i++)
				{
					AnsiString FieldName = IBDayArcBill->Fields[i]->Name;
					if(FieldName.UpperCase() == "RECEIPT")
					{
						std::auto_ptr<TMemoryStream> ReceiptToArchive(new TMemoryStream);
						IBDayArcBill->FieldByName("RECEIPT")->SaveToStream(ReceiptToArchive.get());
						ReceiptToArchive->Position = 0;
						IBArcBill->ParamByName("RECEIPT")->LoadFromStream(ReceiptToArchive.get());
					}
					else
					{
						IBArcBill->ParamByName(FieldName)->AsVariant =
						IBDayArcBill->FieldByName(FieldName)->AsVariant;
					}
				}
				IBArcBill->ExecQuery();

				IBDayArcBillPay->Close();
				IBDayArcBillPay->ParamByName("ARCBILL_KEY")->AsInteger = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
				IBDayArcBillPay->ExecQuery();
				for (;!IBDayArcBillPay->Eof ; IBDayArcBillPay->Next())
				{
					// Copy all the daily ArcBillPay Fields to the main archive. Don't copy field 1 as it is the
					// link to dtArcBill
					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCBILLPAY, 1) FROM RDB$DATABASE";
					IBInternalQuery->ExecQuery();
					int ArcBillPayKey = IBInternalQuery->Fields[0]->AsInteger;

					for (int i=2; i<IBDayArcBillPay->FieldCount; i++)
					{
						AnsiString FieldName = IBDayArcBillPay->Fields[i]->Name;
						IBArcBillPay->ParamByName(FieldName)->AsVariant =
						IBDayArcBillPay->FieldByName(FieldName)->AsVariant;
					}
					IBArcBillPay->ParamByName("ARCBILLPAY_KEY")->AsInteger = ArcBillPayKey;
					IBArcBillPay->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
					IBArcBillPay->ExecQuery();
				}

				IBDayArcRef->Close();
				IBDayArcRef->ParamByName("ARCBILL_KEY")->AsInteger = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
				IBDayArcRef->ExecQuery();
				for (;!IBDayArcRef->Eof ; IBDayArcRef->Next())
				{
					// Copy all the daily ArcBillPay Fields to the main archive. Don't copy field 1 as it is the
					// link to dtArcBill
					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCREF, 1) FROM RDB$DATABASE";
					IBInternalQuery->ExecQuery();
					int ArcRefKey = IBInternalQuery->Fields[0]->AsInteger;

					for (int i=2; i<IBDayArcRef->FieldCount; i++)
					{
						AnsiString FieldName = IBDayArcRef->Fields[i]->Name;
						IBArcRef->ParamByName(FieldName)->AsVariant =
						IBDayArcRef->FieldByName(FieldName)->AsVariant;
					}
					IBArcRef->ParamByName("ARCREF_KEY")->AsInteger = ArcRefKey;
					IBArcRef->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
					IBArcRef->ExecQuery();
				}

				IBDayArcSurcharge->Close();
				IBDayArcSurcharge->ParamByName("ARCBILL_KEY")->AsInteger = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
				IBDayArcSurcharge->ExecQuery();
				for (;!IBDayArcSurcharge->Eof ; IBDayArcSurcharge->Next())
				{
					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCSURCHARGE, 1) FROM RDB$DATABASE";
					IBInternalQuery->ExecQuery();
					int ArcSurchargeKey = IBInternalQuery->Fields[0]->AsInteger;

					// Copy all the daily ArcBillPay Fields to the main archive. Don't copy field 1 as it is the
					// link to dtArcBill
					for (int i=2; i< IBDayArcSurcharge->FieldCount; i++)
					{
						AnsiString FieldName = IBDayArcSurcharge->Fields[i]->Name;
						IBArcSurcharge->ParamByName(FieldName)->AsVariant =
						IBDayArcSurcharge->FieldByName(FieldName)->AsVariant;
					}
					IBArcSurcharge->ParamByName("SURCHARGE_KEY")->AsInteger = ArcSurchargeKey;
					IBArcSurcharge->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
					IBArcSurcharge->ExecQuery();
				}

				// Find all the archive entries connected to this arcbill.
				IBDayArchive->Close();
				IBDayArchive->ParamByName("ARCBILL_KEY")->AsInteger = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
				IBDayArchive->ExecQuery();
				for (;!IBDayArchive->Eof ; IBDayArchive->Next())
				{
					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCHIVE, 1) FROM RDB$DATABASE";
					IBInternalQuery->ExecQuery();
					int ArchiveKey = IBInternalQuery->Fields[0]->AsInteger;

					for (int i=2; i<IBDayArchive->FieldCount; i++)
					{
						AnsiString FieldName = IBDayArchive->Fields[i]->Name;
						IBArchive->ParamByName(FieldName)->AsVariant =
						IBDayArchive->FieldByName(FieldName)->AsVariant;
					}
					IBArchive->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
					IBArchive->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
					IBArchive->ExecQuery();

               IBInternalQuery->Close();
					IBInternalQuery->SQL->Text = "SELECT CATEGORY_KEY FROM DAYARCCATEGORY WHERE ARCHIVE_KEY = :ARCHIVE_KEY";
					IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = IBDayArchive->FieldByName("ARCHIVE_KEY")->AsInteger;
					IBInternalQuery->ExecQuery();
					for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
					{
						IBInternalQuery2->Close();
						IBInternalQuery2->SQL->Text =
                     "INSERT INTO ARCCATEGORY ("
                        "ARCHIVE_KEY,"
								"CATEGORY_KEY) "
                     "VALUES ("
                        ":ARCHIVE_KEY,"
								":CATEGORY_KEY);";
						IBInternalQuery2->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
						IBInternalQuery2->ParamByName("CATEGORY_KEY")->AsInteger =
							IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger;
						IBInternalQuery2->ExecQuery();
               }
				}
			}

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "DELETE FROM DAYARCBILL";
			IBInternalQuery->ExecQuery();
		}
		__finally
		{
		}
   	DBTransaction.Commit();
	}
	catch(Exception &E)
	{
		throw;
	}
   ShowMessage("Close Complete");
}
//---------------------------------------------------------------------------

int TfrmMain::GetTotalInvoices(AnsiString Filter)
{
   int Total = 0;
   if(!modData->SQLConnection->Connected)
   return Total;

   modData->SQLQueryA->SQL->Clear();
	modData->SQLQueryA->Params->Clear();
	modData->SQLQueryA->ParamCheck = true;
   modData->SQLQueryA->SQL->Text =
      "SELECT "
		" COUNT (ARCBILL_KEY)"
      " FROM"
		" ARCBILL WHERE TIME_STAMP > '" + Filter + "'";
	modData->SQLQueryA->Open();
   Total = modData->SQLQueryA->FieldByName("COUNT")->AsInteger;
	return Total;
}

int TfrmMain::GetTotalInvoices()
{
   int Total = 0;
   if(!modData->SQLConnection->Connected)
   return Total;

   modData->SQLQueryA->SQL->Clear();
	modData->SQLQueryA->Params->Clear();
	modData->SQLQueryA->ParamCheck = true;
   modData->SQLQueryA->SQL->Text =
      "SELECT "
		" COUNT (ARCBILL_KEY)"
      " FROM"
		" ARCBILL";
	modData->SQLQueryA->Open();
   Total = modData->SQLQueryA->FieldByName("COUNT")->AsInteger;
	return Total;
}
