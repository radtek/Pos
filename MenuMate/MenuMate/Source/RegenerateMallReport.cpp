//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include <fstream>
#include <sstream>
#include <string>
#include "RegenerateMallReport.h"
#include "MMTouchKeyboard.h"
#include "DeviceRealTerminal.h"
#include "MallExportTextFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmRegenerateMallReport *frmRegenerateMallReport;
//---------------------------------------------------------------------------
__fastcall TfrmRegenerateMallReport::TfrmRegenerateMallReport(TComponent* Owner)
    : TZForm(Owner)
{
    StartDate = Date();
    EndDate = Date() + 1.0;
    mcStartDate->Date = StartDate;
    mcEndDate->Date = EndDate;
    SetYear = Now().FormatString("yyyy");

    InitializeTimeOptions();
    cbStartHour->ItemIndex = 5;
    cbEndHour->ItemIndex = 5;
    cbStartMin->ItemIndex = 0;
    cbEndMin->ItemIndex = 0;
    ReportPath = "";

    StartHour = cbStartHour->ItemIndex;
    StartHour = "0" + StartHour;
    EndHour = cbEndHour->ItemIndex;
    EndHour = "0" + EndHour;
    StartMin = cbStartMin->ItemIndex;
    StartMin = "0" + StartMin;
    EndMin = cbEndMin->ItemIndex;
    EndMin = "0" + EndMin;
    InitializeTimeSet(SDate, EDate);

    edLocationPath->Text = "";

    edLocationPath->Enabled = true;
    edLocationPath->Color = clWindow;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRegenerateMallReport::btnOkMouseClick(TObject *Sender)
{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegenerateMallReport::btnCancelMouseClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegenerateMallReport::mcStartDateClick(TObject *Sender)
{
   StartDate = mcStartDate->Date;
   SetYear = StartDate.FormatString("yyyy");
   InitializeTimeSet(SDate, EDate);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegenerateMallReport::mcEndDateClick(TObject *Sender)
{
   if(int(mcStartDate->Date) > int(mcEndDate->Date))
   {
        mcEndDate->Date = mcStartDate->Date;
   }
   EndDate = mcEndDate->Date;
   InitializeTimeSet(SDate, EDate);
}

//---------------------------------------------------------------------------

void __fastcall TfrmRegenerateMallReport::btnGenerateMouseClick(TObject *Sender)

{
    switch(TGlobalSettings::Instance().mallInfo.MallId)
    {
        case 1:
        RegenerateEstanciaMallExport();
        break;

        default:
        break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegenerateMallReport::cbStartHourChange(TObject *Sender)

{
    StartHour = cbStartHour->ItemIndex;
    StartHour = FixTime(StartHour);
    InitializeTimeSet(SDate, EDate);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegenerateMallReport::cbStartMinChange(TObject *Sender)
{

    StartMin = cbStartMin->ItemIndex;
    InitializeTimeSet(SDate, EDate);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegenerateMallReport::cbEndHourChange(TObject *Sender)
{
    EndHour = cbEndHour->ItemIndex;
    InitializeTimeSet(SDate, EDate);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegenerateMallReport::cbEndMinChange(TObject *Sender)
{
    EndMin = cbEndMin->ItemIndex;
    InitializeTimeSet(SDate, EDate);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegenerateMallReport::edLocationPathMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 300;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edLocationPath->Text;
	frmTouchKeyboard->Caption = "Enter File Location";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
        edLocationPath->Text = frmTouchKeyboard->KeyboardText;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegenerateMallReport::btnLoadPathMouseClick(TObject *Sender)
{
    std::list<TMallExportSettings>::iterator it;
    //Get File Path
    for(it = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); it != TGlobalSettings::Instance().mallInfo.MallSettings.end(); it++)
    {
        if(it->ControlName == "edNewMallPath")
             edLocationPath->Text = it->Value;
    }
}
//---------------------------------------------------------------------------
void TfrmRegenerateMallReport::InitializeTimeOptions()
{
    // Initialize StartTime and End Time
    for(int i = 0; i <= 23; i++)
    {
        UnicodeString HourValue = "0";
        if(i < 10)
        {
            HourValue += i;
        }
        else
        {
            HourValue = i;
        }

        cbStartHour->AddItem(HourValue.t_str(),NULL);
        cbEndHour->AddItem(HourValue.t_str(),NULL);
    }

    // Initialize Start Minutes and End Minutes
    for(int i = 0; i <= 59; i++)
    {
        UnicodeString MinutesValue = "0";
        if(i < 10)
        {
            MinutesValue += i;
        }
        else
        {
            MinutesValue = i;
        }

        cbStartMin->AddItem(MinutesValue.t_str(),NULL);
        cbEndMin->AddItem(MinutesValue.t_str(),NULL);
    }
}
//---------------------------------------------------------------------------
void TfrmRegenerateMallReport::InitializeTimeSet(TDateTime &SDate, TDateTime &EDate)
{
    UnicodeString StartHM = StartHour + ":" + StartMin + ":00";
    UnicodeString EndHM = EndHour + ":" + EndMin + ":00";

    SDate = StartDate + StrToTime(StartHM);
    EDate = EndDate + StrToTime(EndHM);
}
//-------------------------------------------------------------------------------------------------------------
void TfrmRegenerateMallReport::RegenerateEstanciaMallExport()
{
    TMallExportPrepareData preparedData;

    //Prepare Data For Exporting into File
    preparedData = PrepareDataForExport();

    //Create Export Medium
    TMallExportTextFile* exporter =  new TMallExportTextFile();
    exporter->WriteToFile(preparedData);

   /* Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();

    TIBSQL* TimeQuery = DBTransaction.Query(DBTransaction.AddQuery());
    TimeQuery->Close();
    TimeQuery->SQL->Text =
                            "SELECT "
                            "    Extract (Day From a.TIME_STAMP) Bill_Day,     "
                            "    Extract (month From a.TIME_STAMP) Bill_Month, "
                            "    Extract (year From a.TIME_STAMP) Bill_Year    "
                            "FROM ARCBILL a "
                            "WHERE "
                            "       a.TIME_STAMP >= :START_TIME AND "
                            "       a.TIME_STAMP < :END_TIME "
                            "GROUP BY 3,2,1 "
                            "ORDER BY 3,2,1 ";
    TimeQuery->ParamByName("START_TIME")->AsDateTime = SDate;
    TimeQuery->ParamByName("END_TIME")->AsDateTime = EDate;
    TimeQuery->ExecQuery();

    for (; !TimeQuery->Eof; TimeQuery->Next())
    {
        day =  TimeQuery->FieldByName("Bill_Day")->AsInteger;
        month =  TimeQuery->FieldByName("Bill_Month")->AsInteger;
        year =  TimeQuery->FieldByName("Bill_Year")->AsInteger;
        TrasactionDate = EncodeDateTime(year, month, day, 0, 0, 0, 0);
        TIBSQL* IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        // for opened section code 1
        WriteInToFileForFirstCode(&DBTransaction, day, month, year, TrasactionDate);

        // Get all bills for that time period
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                                    "SELECT "
                                        "a.ARCBILL_KEY, "
                                        "a.TIME_STAMP "
                                    "FROM ARCBILL a "
                                    "WHERE "
                                        "Extract (Day From a.TIME_STAMP) = :Day and "
                                        "Extract (Month From a.TIME_STAMP) = :Month and "
                                        "Extract (Year From a.TIME_STAMP) = :Year "
                                    "GROUP BY "
                                        "a.ARCBILL_KEY,a.TIME_STAMP "
                                    "Order By "
                                        "a.TIME_STAMP asc ";
        IBInternalQuery->ParamByName("Day")->AsInteger = day;
        IBInternalQuery->ParamByName("Month")->AsInteger = month;
        IBInternalQuery->ParamByName("Year")->AsInteger = year;
        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            int ArcBillKey = IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger;

            //section 101 starts here
            WriteInToFileForSecondCode(&DBTransaction, ArcBillKey, day, month, year, TrasactionDate);
            //section 101 ends here

            //section 111 starts here
            WriteInToFileForThirdCode(&DBTransaction, ArcBillKey, day, month, year, TrasactionDate);

            //121 section starts from here
            WriteInToFileForFourthCode(&DBTransaction, ArcBillKey, day, month, year, TrasactionDate);

            //section 131 starts from here
            WriteInToFileForFifthCode(&DBTransaction, ArcBillKey, day, month, year, TrasactionDate);
    }
     //for code 1 closed section starts from here
    WriteInToFileForSixthCode(&DBTransaction, day, month, year);

    //load file to ftp
    LoadFileToFTP();

    //change file name to null
//    TGlobalSettings::Instance().DLFMallFileName = "";
//    std::auto_ptr<TfrmAnalysis>(frmAnalysis)(TfrmAnalysis::Create<TfrmAnalysis>(this));
//    frmAnalysis->SaveCompValueinDBStrUnique(vmDLFMallFileName, TGlobalSettings::Instance().DLFMallFileName);

    //increase file_number
    TGlobalSettings::Instance().BatchNo = TGlobalSettings::Instance().BatchNo+1;;
    TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmBatchNo, TGlobalSettings::Instance().BatchNo);

    }

    DBTransaction.Commit(); */
}
//---------------------------------------------------------------------------
void TfrmRegenerateMallReport::WriteInToFileForFirstCode(Database::TDBTransaction *dbTransaction, int day, int month, int year, TDateTime cdate)
{
    TIBSQL* IBInternalQuery = dbTransaction->Query(dbTransaction->AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = " SELECT "
                                        "FIRST 1 a.ARCBILL_KEY, "
                                        "a.INVOICE_NUMBER, "
                                        "a.TIME_STAMP, "
                                        "a.STAFF_NAME, "
                                        "a.TERMINAL_NAME "
                                "FROM ARCBILL a "
                                 "WHERE "
                                        "Extract (Day From a.TIME_STAMP) = :Day and "
                                        "Extract (Month From a.TIME_STAMP) = :Month and "
                                        "Extract (Year From a.TIME_STAMP) = :Year "
                                 "ORDER BY "
                                        "a.TIME_STAMP asc ";
      IBInternalQuery->ParamByName("Day")->AsInteger = day;
      IBInternalQuery->ParamByName("Month")->AsInteger = month;
      IBInternalQuery->ParamByName("Year")->AsInteger = year;

    IBInternalQuery->ExecQuery();

    AnsiString invoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
    AnsiString staffName = IBInternalQuery->FieldByName("STAFF_NAME")->AsString;
    terminal_Name = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
    TDateTime date_Time = IBInternalQuery->FieldByName("TIME_STAMP")->AsString;

    AnsiString cmd_code = "1";
    AnsiString file_stat = "OPENED";
    AnsiString tenant_No = TGlobalSettings::Instance().TenantNo;
    AnsiString pos_No = terminal_Name;
    AnsiString Receipt_No = invoiceNumber-1;
    int Tran_File_No =TGlobalSettings::Instance().BatchNo;
    AnsiString date = date_Time.FormatString("yyyymmdd");
    AnsiString time = date_Time.FormatString("hh:mm:ss");
    AnsiString user_ID = staffName;
    AnsiString Sale_date = date_Time.FormatString("yyyymmdd");

    AnsiString  finalValue = cmd_code+"|" +file_stat+"|"+tenant_No+"|"+pos_No+"|"+Receipt_No+"|"+ Tran_File_No+ "|" +
    date  +"|" +time+"|" +user_ID+"|" +Sale_date;

    //creating instance of TListPaymentSystem
    TListPaymentSystem *PaymentSystem;
    if(TGlobalSettings::Instance().DLFMallFileName==""||TGlobalSettings::Instance().DLFMallFileName== "[null]" )
    {
        AnsiString fileName = PaymentSystem->CreateFilename(cdate);
        AnsiString filePath = PaymentSystem->CreateTextFile(fileName,finalValue);  //CREATE TEXT FILE METHOD NAME WILL BE CHANGED
    }
}
//---------------------------------------------------------------------------
void TfrmRegenerateMallReport::WriteInToFileForSecondCode(Database::TDBTransaction *dbTransaction, int arcBillKey, int day , int month , int year, TDateTime date)
{
    TIBSQL* IBInternalQuery0 = dbTransaction->Query(dbTransaction->AddQuery());
    IBInternalQuery0->Close();
    IBInternalQuery0->SQL->Text =
                                "SELECT "
                                    "a.INVOICE_NUMBER, "
                                    "cast((CASE WHEN coalesce(a.TOTAL,0) < 0  THEN  a.INVOICE_NUMBER  "
                                                             "END) as numeric(17, 4)) AS Refund_Receipt, "
                                    "a.TIME_STAMP, "
                                    "a.STAFF_NAME, "
                                    "a.TERMINAL_NAME, "
                                    "a.PATRON_COUNT "
                                "FROM ARCBILL a "
                                "WHERE "
                                        "Extract (Day From a.TIME_STAMP) = :Day and "
                                        "Extract (Month From a.TIME_STAMP) = :Month and "
                                        "Extract (Year From a.TIME_STAMP) = :Year "
                                        "AND a.ARCBILL_KEY = :ARCBILL_KEY "
                                "GROUP BY "
                                    "a.INVOICE_NUMBER, "
                                    "a.TIME_STAMP,  "
                                    " a.STAFF_NAME, "
                                    "a.TERMINAL_NAME, "
                                    "a.PATRON_COUNT, "
                                    "a.TOTAL ";
    IBInternalQuery0->ParamByName("Day")->AsInteger = day;
    IBInternalQuery0->ParamByName("Month")->AsInteger = month;
    IBInternalQuery0->ParamByName("Year")->AsInteger = year;
    IBInternalQuery0->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
    IBInternalQuery0->ExecQuery();

    AnsiString cmd_code= "101";
    AnsiString receipt_No   = IBInternalQuery0->FieldByName("INVOICE_NUMBER")->AsString;
    AnsiString shift_No = "1";
    TDateTime invoice_Date_Time =  IBInternalQuery0->FieldByName("TIME_STAMP")->AsDateTime;
    AnsiString Date = invoice_Date_Time.FormatString("yyyymmdd");
    AnsiString Time = invoice_Date_Time.FormatString("hh:mm:ss");
    AnsiString user_id = IBInternalQuery0->FieldByName("STAFF_NAME")->AsString;
    AnsiString manual_Receipt = "";
    UnicodeString refund_Receipt = IBInternalQuery0->FieldByName("Refund_Receipt")->AsString;
    AnsiString reason_Code = "";
    AnsiString salesMan_Code = IBInternalQuery0->FieldByName("STAFF_NAME")->AsString;
    AnsiString table_No = "";//IBInternalQuery0->FieldByName("TERMINAL_NAME")->AsString;
    int cust_Count = IBInternalQuery0->FieldByName("PATRON_COUNT")->AsInteger;
    AnsiString training = "N";
    AnsiString tran_status = "SALE";

    AnsiString  finalValue= cmd_code+"|" +receipt_No+"|"+shift_No+"|"+Date+"|"+Time+"|"+ user_id+ "|" +
    manual_Receipt  +"|" +refund_Receipt+"|" +reason_Code+"|" +salesMan_Code
    +"|" +table_No+"|" +cust_Count+"|" +training+"|" +tran_status;

    TListPaymentSystem *PaymentSystem;
    AnsiString fileName = PaymentSystem->CreateFilename(date);
    AnsiString filePath = PaymentSystem->CreateTextFile(fileName,finalValue);
}
//------------------------------------------------------------------------------------------------------
void TfrmRegenerateMallReport::WriteInToFileForThirdCode(Database::TDBTransaction *dbTransaction, int arcBillKey, int day , int month , int year, TDateTime date)
{
    TIBSQL* IBInternalQuery1 = dbTransaction->Query(dbTransaction->AddQuery());
    IBInternalQuery1->Close();
    IBInternalQuery1->SQL->Text =

                                "  SELECT "
                                            "    ARC.ARCBILL_KEY,  "
                                            "    ARC.ITEM_ID,      "
                                            "    SUM(ARC.QTY) QTY, "
                                            "    ARC.PRICE_LEVEL0, "
                                            "    cast((CASE WHEN coalesce(ARC.PRICE_ADJUST,0) <> 0  THEN  coalesce(ARC.PRICE_ADJUST,0)                                                             "
                                            "                               WHEN ARC.HAPPY_HOUR = 'F' AND coalesce(ARC.PRICE_ADJUST,0) = 0 AND ARC.PRICE <> 0 THEN  coalesce(ARC.PRICE_LEVEL0,0)   "
                                            "                               WHEN ARC.HAPPY_HOUR = 'T' AND coalesce(ARC.PRICE_ADJUST,0) = 0 AND ARC.PRICE <> 0 THEN  coalesce(ARC.PRICE_LEVEL1,0)   "
                                            "                               WHEN coalesce(ARC.PRICE_ADJUST,0) = 0 AND ARC.PRICE = 0 THEN  coalesce(ARC.PRICE,0)                                    "
                                            "                         END) as numeric(17, 4)) AS New_Price,                                                                                        "
                                            "    ARC.DISCOUNT,          "
                                            "    ARC.MENU_NAME,         "
                                            "    ARC.CATEGORY_KEY,      "
                                            "    CAST(SUM(ARC.QTY*ARC.PRICE)+ ARC.DISCOUNT - Sum(  COALESCE(AOT.VAT,0) ) - Sum( COALESCE( AOT.ServiceCharge,0)) - Sum( COALESCE( AOT.OtherServiceCharge,0))AS NUMERIC(17,4))ITEM_NSALES, "
                                            "    CAST(SUM(ARC.DISCOUNT) AS NUMERIC(17,4)) TOTAL_DISCOUNT, "
                                            "    CAST(Sum(COALESCE(AOT.VAT,0) ) + Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0)) AS NUMERIC(17,4)) TOTAL_TAX, "
                                            "    ARC.PLU "
                                "FROM ARCHIVE ARC "
                                    "LEFT JOIN (  "
                                            "	SELECT    "
                                            "	ARCORDERTAXES.ARCHIVE_KEY, "
                                            "	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,                "
                                            "	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,      "
                                            "	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge  "
                                            "	FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,            "
                                            "	Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                                            "	FROM ARCORDERTAXES a                               "
                                            "	group by  a.TAX_TYPE  , a.ARCHIVE_KEY              "
                                            "	order by 1 )  ARCORDERTAXES                        "
                                            "	GROUP BY ARCORDERTAXES.ARCHIVE_KEY )               "
                                    "	AOT ON ARC.ARCHIVE_KEY = AOT.ARCHIVE_KEY           "
                                "WHERE "
                                        "Extract (Day From ARC.TIME_STAMP_BILLED) = :Day and "
                                        "Extract (Month From ARC.TIME_STAMP_BILLED) = :Month and "
                                        "Extract (Year From ARC.TIME_STAMP_BILLED) = :Year and ARC.ARCBILL_KEY = :ARCBILL_KEY  "
                               "GROUP BY "
                                        " ARC.ITEM_ID,       "
                                        "    ARC.QTY,                "
                                        "    ARC.PRICE_LEVEL0,       "
                                        "    ARC.PRICE_LEVEL1,       "
                                        "    ARC.PRICE_ADJUST,       "
                                        "    ARC.DISCOUNT,           "
                                        "    ARC.MENU_NAME,          "
                                        "    ARC.CATEGORY_KEY,       "
                                        "    AOT.VAT,                "
                                        "    AOT.ServiceCharge,      "
                                        "    AOT.OtherServiceCharge, "
                                        "    ARC.PLU,                "
                                        "    ARC.ARCBILL_KEY,        "
                                        "    ARC.HAPPY_HOUR,         "
                                        "    ARC.PRICE               "
                                "ORDER BY "
                                        "ARC.ARCBILL_KEY    ";
    IBInternalQuery1->ParamByName("Day")->AsInteger = day;
    IBInternalQuery1->ParamByName("Month")->AsInteger = month;
    IBInternalQuery1->ParamByName("Year")->AsInteger = year;
    IBInternalQuery1->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
    IBInternalQuery1->ExecQuery();

   for (; !IBInternalQuery1->Eof; IBInternalQuery1->Next())
    {
      	int qty = IBInternalQuery1->FieldByName("QTY")->AsCurrency;
		AnsiString cmd_code= "111";
		AnsiString  item_Code = IBInternalQuery1->FieldByName("ITEM_ID")->AsInteger;
		AnsiString  item_Qty =  IBInternalQuery1->FieldByName("QTY")->AsCurrency;
		AnsiString  org_Price = IBInternalQuery1->FieldByName("PRICE_LEVEL0")->AsCurrency;
		AnsiString  new_Price = IBInternalQuery1->FieldByName("New_Price")->AsCurrency;
		AnsiString  item_Flag              ="";
		AnsiString  tax_Code               ="Tax";
		AnsiString  discount_Code          ="";
		AnsiString  discount_Amt   = IBInternalQuery1->FieldByName("DISCOUNT")->AsCurrency;
		AnsiString  item_Dept              = IBInternalQuery1->FieldByName("MENU_NAME")->AsString;//ItemComplete->MenuName;              	//length 8
		AnsiString  item_Catg              = IBInternalQuery1->FieldByName("CATEGORY_KEY")->AsInteger;//catkey ;          			  	//length 8
		AnsiString  label_Keys             = "";              				//length 8
		AnsiString  item_Comm              = "0";             				 //length 1
		AnsiString  item_Nsales            = IBInternalQuery1->FieldByName("ITEM_NSALES")->AsCurrency;

		AnsiString  discount_By            = IBInternalQuery1->FieldByName("TOTAL_DISCOUNT")->AsCurrency;
		AnsiString  discount_Sign          ="$";              				//length 1
		AnsiString  item_Stax              = IBInternalQuery1->FieldByName("TOTAL_TAX")->AsCurrency;//CurrToStr(ItemComplete->BillCalcResult.TotalTax);              //length 10
		AnsiString  plu_Code               = IBInternalQuery1->FieldByName("PLU")->AsInteger;


		AnsiString  finalValue= cmd_code+"|" +item_Code+"|"+item_Qty+"|"+org_Price+"|"+new_Price+"|"+ item_Flag+ "|" +
		tax_Code  +"|" +discount_Code+"|" +discount_Amt+"|" +item_Dept
		+"|" +item_Catg+"|" +label_Keys+"|" +item_Comm+"|" +item_Nsales+"|" +discount_By+"|" +discount_Sign+"|" +item_Stax+"|" +plu_Code;

        TListPaymentSystem *PaymentSystem;
		AnsiString fileName = PaymentSystem->CreateFilename(date);
		AnsiString filePath = PaymentSystem->CreateTextFile(fileName,finalValue);
    }
}
//------------------------------------------------------------------------------------------------------------------
void TfrmRegenerateMallReport::WriteInToFileForFourthCode(Database::TDBTransaction *dbTransaction, int arcBillKey, int day , int month , int year, TDateTime date)
{
    TIBSQL* IBInternalQuery2 = dbTransaction->Query(dbTransaction->AddQuery());
    IBInternalQuery2->Close();
    IBInternalQuery2->SQL->Text =
                                    "SELECT AB.ARCBILL_KEY,                                                   "
                                        "    CAST (AB.TOTAL-AB.DISCOUNT AS NUMERIC (17,4)) SALES,                 "
                                        "    CAST (AB.TOTAL AS NUMERIC (17,4)) TOTAL_SALES,                       "
                                        "    AB.DISCOUNT,                                                         "
                                        "    CAST(Sum( COALESCE( AOT.ServiceCharge,0))AS NUMERIC(17,4)) CHARGES,  "
                                        "    CAST(Sum(COALESCE(AOT.VAT,0) ) + Sum( COALESCE( AOT.ServiceCharge,0)) + Sum( COALESCE( AOT.OtherServiceCharge,0)) AS NUMERIC(17,4)) TAX, "
                                        "    AB.ROUNDING_ADJUSTMENT ROUNDING_AMT  "
                                    "FROM ARCBILL AB                          "
                                    "LEFT JOIN (                              "
                                    "                SELECT                   "
                                    "                    AOT.ARCBILL_KEY,     "
                                    "                    CAST(Sum(COALESCE(AOT.VAT,0) ) AS NUMERIC(17,4)) VAT, "
                                    "                    CAST(Sum( COALESCE( AOT.ServiceCharge,0)) AS NUMERIC(17,4)) ServiceCharge,           "
                                    "                    CAST(Sum( COALESCE( AOT.OtherServiceCharge,0)) AS NUMERIC(17,4)) OtherServiceCharge  "
                                    "                FROM                                                                                     "
                                    "                (                                                                                        "
                                    "                    SELECT                                                                               "
                                    "                            ARCORDERTAXES.ARCHIVE_KEY,                                                   "
                                    "                            MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT,                "
                                    "                            MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,      "
                                    "                            MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge ,"
                                    "                            ARCHIVE.ARCBILL_KEY                                                                               "
                                    "                            FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,                                                           "
                                    "                            Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE                                                "
                                    "                            FROM ARCORDERTAXES a                                                                              "
                                    "                            group by  a.TAX_TYPE  , a.ARCHIVE_KEY                                                             "
                                    "                            order by 1 )  ARCORDERTAXES                                           "
                                    "                            LEFT JOIN ARCHIVE ON ARCHIVE.ARCHIVE_KEY = ARCORDERTAXES.ARCHIVE_KEY  "
                                    "                            GROUP BY ARCORDERTAXES.ARCHIVE_KEY, ARCHIVE.ARCBILL_KEY)AOT           "
                                    "                GROUP BY         "
                                    "                AOT.ARCBILL_KEY  "
                                    "            )                    "
                                    "        AOT ON  AB.ARCBILL_KEY = AOT.ARCBILL_KEY  "
                                    "WHERE "
                                        "Extract (Day From AB.TIME_STAMP) = :Day and "
                                        "Extract (Month From AB.TIME_STAMP) = :Month and "
                                        "Extract (Year From AB.TIME_STAMP) = :Year and AB.ARCBILL_KEY = :ARCBILL_KEY "
                                    "GROUP BY                    "
                                    "    AB.ARCBILL_KEY,         "
                                    "    AB.TOTAL,               "
                                    "    AB.DISCOUNT,            "
                                    "    AOT.VAT,                "
                                    "    AOT.ServiceCharge,      "
                                    "    AOT.OtherServiceCharge, "
                                    "    AB.ROUNDING_ADJUSTMENT  "
                                    "ORDER BY                    "
                                    "    AB.ARCBILL_KEY          " ;
    IBInternalQuery2->ParamByName("Day")->AsInteger = day;
    IBInternalQuery2->ParamByName("Month")->AsInteger = month;
    IBInternalQuery2->ParamByName("Year")->AsInteger = year;
    IBInternalQuery2->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
    IBInternalQuery2->ExecQuery();

	AnsiString cmd_code= "121";
	AnsiString  sales         = IBInternalQuery2->FieldByName("SALES")->AsCurrency;
	AnsiString  discount      = IBInternalQuery2->FieldByName("DISCOUNT")->AsCurrency;
	AnsiString  cess          ="";              //length 11
	AnsiString  charges       = IBInternalQuery2->FieldByName("CHARGES")->AsCurrency;
	AnsiString  tax           = IBInternalQuery2->FieldByName("TAX")->AsCurrency;          //length 11
	AnsiString  tax_Type      = "";
    AnsiString grandTotal =  IBInternalQuery2->FieldByName("TOTAL_SALES")->AsCurrency;
	tax_Type="E";
	AnsiString  exempt_Gst    ="Y";
	AnsiString discount_Code = "";
	AnsiString  other_chg     ="";
	AnsiString  discount_Per  = IBInternalQuery2->FieldByName("DISCOUNT")->AsCurrency;
	AnsiString  rounding_Amt  = IBInternalQuery2->FieldByName("ROUNDING_AMT")->AsCurrency;

	AnsiString finalValue= cmd_code+"|" +sales+"|"+discount+"|"+cess+"|"+charges+"|"+ tax+ "|" +
	tax_Type  +"|" +exempt_Gst+"|" +discount_Code+"|" +other_chg
	+"|" +discount_Per+"|" +rounding_Amt;

    TListPaymentSystem *PaymentSystem;
	AnsiString fileName = PaymentSystem->CreateFilename(date);
	AnsiString filePath =     PaymentSystem->CreateTextFile(fileName,finalValue);
}
//----------------------------------------------------------------------------------------------------------------------------------
void TfrmRegenerateMallReport::WriteInToFileForFifthCode(Database::TDBTransaction *dbTransaction, int arcBillKey,int day , int month , int year, TDateTime date)
{
    TIBSQL* IBInternalQuery3 = dbTransaction->Query(dbTransaction->AddQuery());
    IBInternalQuery3->Close();
    IBInternalQuery3->SQL->Text =
                                    "SELECT   "
                                        "a.ARCBILL_KEY, "
                                        "a.PAY_TYPE,   "
                                        "a.SUBTOTAL,   "
                                        "a.ROUNDING,   "
                                        "AB.TOTAL,      "
                                        "a.NOTE "
                                    "FROM ARCBILLPAY a "
                                    "INNER JOIN ARCBILL AB on a.ARCBILL_KEY = AB.ARCBILL_KEY "
                                    "WHERE "
                                        "a.SUBTOTAL <> 0 AND  "
                                        " a.ARCBILL_KEY = :ARCBILL_KEY AND  "
                                        "Extract (Day From AB.TIME_STAMP) = :Day and "
                                        "Extract (Month From AB.TIME_STAMP) = :Month and "
                                        "Extract (Year From AB.TIME_STAMP) = :Year "
                                    "ORDER BY a.ARCBILL_KEY ";
    IBInternalQuery3->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
    IBInternalQuery3->ParamByName("Day")->AsInteger = day;
    IBInternalQuery3->ParamByName("Month")->AsInteger = month;
    IBInternalQuery3->ParamByName("Year")->AsInteger = year;
    IBInternalQuery3->ExecQuery();


    for (; !IBInternalQuery3->Eof; IBInternalQuery3->Next())
    {
        AnsiString cmd_code= "131";
        AnsiString  type;
        if(IBInternalQuery3->FieldByName("NOTE")->AsString != "")
        {
            type          = "C";
        }
        else
        {
            type          = "T";
        }
        AnsiString  payment_Name  = IBInternalQuery3->FieldByName("PAY_TYPE")->AsString;//SubPayment->Name;              //length 20
        AnsiString  curr_Code     = "$";
        AnsiString  buy_Rate      = "1";
        AnsiString  amount        =  IBInternalQuery3->FieldByName("SUBTOTAL")->AsCurrency;
        AnsiString  remarks1      = "";
        AnsiString  remarks2      = "";
        AnsiString  base_Amount   = IBInternalQuery3->FieldByName("TOTAL")->AsCurrency;//PaymentTransaction.Money.GrandTotal;              //length 10

        AnsiString  finalValue= cmd_code+"|" +type+"|"+payment_Name+"|"+curr_Code+"|"+buy_Rate+"|"+ amount+ "|" +
        remarks1  +"|" +remarks2+"|" +base_Amount;

         TListPaymentSystem *PaymentSystem;
        AnsiString fileName = PaymentSystem->CreateFilename(date);
        AnsiString filePath =  PaymentSystem->CreateTextFile(fileName,finalValue);
   }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TfrmRegenerateMallReport::WriteInToFileForSixthCode(Database::TDBTransaction *dbTransaction,int day , int month , int year)
{
    TIBSQL* IBInternalQuery4 = dbTransaction->Query(dbTransaction->AddQuery());
    IBInternalQuery4->Close();
    IBInternalQuery4->SQL->Text = " SELECT "
                                        "FIRST 1 a.ARCBILL_KEY, "
                                        "a.INVOICE_NUMBER, "
                                        "a.TIME_STAMP, "
                                        "a.STAFF_NAME, "
                                        "a.TERMINAL_NAME "
                                "FROM ARCBILL a "
                                 "WHERE "
                                        "Extract (Day From a.TIME_STAMP) = :Day and "
                                        "Extract (Month From a.TIME_STAMP) = :Month and "
                                        "Extract (Year From a.TIME_STAMP) = :Year "
                                 "ORDER BY "
                                        "a.ARCBILL_KEY DESC ";
    IBInternalQuery4->ParamByName("Day")->AsInteger = day;
    IBInternalQuery4->ParamByName("Month")->AsInteger = month;
    IBInternalQuery4->ParamByName("Year")->AsInteger = year;

    IBInternalQuery4->ExecQuery();

    TDateTime End_Date_Time = IBInternalQuery4->FieldByName("TIME_STAMP")->AsString;

    AnsiString cmd_code= "1";
    AnsiString file_stat ="Closed";
    AnsiString tenant_No = TGlobalSettings::Instance().TenantNo;
    AnsiString pos_No = terminal_Name;//IBInternalQuery4->FieldByName("TERMINAL_NAME")->AsString;
    AnsiString Receipt_No = IBInternalQuery4->FieldByName("INVOICE_NUMBER")->AsString;
    AnsiString Tran_File_No = TGlobalSettings::Instance().BatchNo;
    AnsiString date = End_Date_Time.FormatString("yyyymmdd");
    AnsiString time = End_Date_Time.FormatString("hh:mm:ss");
    AnsiString user_ID = IBInternalQuery4->FieldByName("STAFF_NAME")->AsString;
    AnsiString Sale_date = End_Date_Time.FormatString("yyyymmdd");

    AnsiString finalValue= cmd_code+"|" +file_stat+"|"+tenant_No+"|"+pos_No+"|"+Receipt_No+"|"+ Tran_File_No+ "|" +
    date  +"|" +time+"|" +user_ID+"|" +Sale_date;

    if(TGlobalSettings::Instance().DLFMallFileName!=""&&TGlobalSettings::Instance().DLFMallFileName!= "[null]" )
    {
        AnsiString mallPath = TGlobalSettings::Instance().MallPath;
        AnsiString LocalPath = mallPath + "\\DLFMall\\" +  TGlobalSettings::Instance().DLFMallFileName;

        TStrings * List = new TStringList();
        if (FileExists(LocalPath ))
        {
           List->LoadFromFile(LocalPath );
        }
        List->Add(finalValue);
        List->SaveToFile(LocalPath );
        delete List;
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void TfrmRegenerateMallReport::LoadFileToFTP()
{
    UnicodeString HostName = TGlobalSettings::Instance().FTPServer;
    UnicodeString UserPath = TGlobalSettings::Instance().FTPPath;
    UnicodeString UserName = TGlobalSettings::Instance().FTPUserName;
    UnicodeString UserPassword = TGlobalSettings::Instance().FTPPassword;

    UnicodeString LocalPathFileName= TGlobalSettings::Instance().MallPath+ "\\DLFMall\\"+TGlobalSettings::Instance().DLFMallFileName; //"C:/trip.txt";//
    UnicodeString LocalFileName = TGlobalSettings::Instance().DLFMallFileName ;
    int FCount=0;
    const char * hostName = HostName.t_str();
    const char * userPath = UserPath.t_str();
    const char * userName = UserName.t_str();
    const char * userPassword = UserPassword.t_str();

   // std::auto_ptr<TfrmAnalysis>(frmAnalysis)(TfrmAnalysis::Create<TfrmAnalysis>(this));
  //  frmAnalysis->FileSubmit(hostName, userName, userPassword, userPath, LocalPathFileName, LocalFileName, FCount);
}
//-------------------------------------------------------------------------------------------------------------------------------------

// Initialize Start and End Time for each mall
void TfrmRegenerateMallReport::SetSpecificMallTimes(int &StartH, int &EndH, int &StartM, int &EndM)
{
    if(TGlobalSettings::Instance().MallIndex == AYALAMALL)
    {
        StartH = 6;
        EndH = 6;
        StartM = 0;
        EndM = 0;
    }
    else
    {
        StartH = 5;
        EndH = 5;
        StartM = 0;
        EndM = 0;
    }
}
//---------------------------------------------------------------------------
TMallExportPrepareData TfrmRegenerateMallReport::PrepareDataForExport()
{
    //Create TMallExportPrepareData  for returning prepared data
    TMallExportPrepareData preparedData;

    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        //Set for inserting index. these indexes will be used for fetching data
        std::set<int> keyToCheck;

        //Indexes for which data will not selected
        int dailySalekeys[8] = {1, 2, 3, 33, 35, 66, 67, 68};

        //insert these indexes into set.
        keyToCheck = estanciaMall.InsertInToSet(dailySalekeys, 8);

        //Prepare Data For Daily Sales File
        PrepareDataForDailySalesFile(dbTransaction, keyToCheck, preparedData, 1);

       //indexes for selecting total Net sale, patron count, etc
        int  hourIndexkeys[3] = {65, 64, 32};

        //Clear the map because same map is used for many time insertion
        keyToCheck.clear();

        //insert these indexes into set.
        keyToCheck = estanciaMall.InsertInToSet(hourIndexkeys, 3);

        //Prepare Data For Hourly File
        PrepareDataForHourlySalesFile(dbTransaction, keyToCheck, preparedData, 2);

        //indexes for selecting total Net sale, invoice number , status
        int invoiceIndex[3] = {65, 67, 68};

         //Clear the map because same map is used for many time insertion
        keyToCheck.clear();

        //insert these indexes into set.
        keyToCheck = estanciaMall.InsertInToSet(invoiceIndex, 3);

        //Prepare Data For Invoice File
        PrepareDataForInvoiceSalesFile(dbTransaction, keyToCheck, preparedData, 3);

       //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return preparedData;
}
//-----------------------------------------------------------------------------------------------
void TfrmRegenerateMallReport::PrepareDataForInvoiceSalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys,
                                                                                        TMallExportPrepareData &prepareDataForInvoice, int index)
{
    //Create List Of SalesData for invoice file
    std::list<TMallExportSalesData> salesDataForISF;
    try
    {
        ///Store First Letter of file name
        UnicodeString fileName = "I";

        //Seperate key with commas in the form of string.
        UnicodeString indexKeysList = estanciaMall.GetFieldIndexList(indexKeys);

        ///Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Declare Set For storing index
        std::set<int>keysToSelect;

        //Create array for storing index by which file name will be prepared
        int  fileNameKeys[4] = {1, 2, 3, 33};

        //Store keys into set
        keysToSelect = estanciaMall.InsertInToSet(fileNameKeys, 4);

        //Get file name according to field index.
        fileName = fileName + "" + estanciaMall.GetFileName(dBTransaction, keysToSelect);

        //insert filename into map according to index and file type
        prepareDataForInvoice.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

        //insert indexes into array for fetching tenant code, date , terminal number, sales type
        int invoiceIndexKeys[4] = {1, 2, 3, 35};

        //clear the map.
        keysToSelect.clear();

        //Store keys into set
        keysToSelect = estanciaMall.InsertInToSet(invoiceIndexKeys, 4);

         ///Load MallSetting For writing into file
        estanciaMall.LoadMallSettingsForInvoiceFile(dBTransaction, prepareDataForInvoice, keysToSelect, index);

        //Query for selecting data for invoice file
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT a.ARCBILL_KEY, "
                                               "a.CREATED_BY, "
                                               "a.DATE_CREATED, "
                                               "a.FIELD, "
                                               "LPAD((CASE WHEN (a.FIELD_INDEX = 65) THEN 6 "
                                                          "WHEN (a.FIELD_INDEX = 67) THEN 7 "
                                                          "WHEN (a.FIELD_INDEX = 68) THEN 5 "
                                                          "ELSE (a.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                                "(CASE WHEN (a.FIELD_INDEX = 65) THEN (TOTALNETSALE.FIELD_VALUE*100) "
                                                      "WHEN (a.FIELD_INDEX = 68) THEN LPAD(a.FIELD_VALUE,5,0) "
                                                      "WHEN (a.FIELD_INDEX = 67) THEN LPAD(a.FIELD_VALUE,2,0) "
                                                      "ELSE a.FIELD_VALUE END ) FIELD_VALUE, "
                                                "a.VALUE_TYPE, "
                                                "meh.MM_NAME "
                                        "FROM MALLEXPORT_SALES a "
                                        "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                        "LEFT JOIN(SELECT a.ARCBILL_KEY, CAST(a.FIELD_VALUE AS numeric(17,2))FIELD_VALUE "
                                                        "FROM MALLEXPORT_SALES a WHERE a.FIELD_INDEX = 65)TOTALNETSALE ON a.ARCBILL_KEY = TOTALNETSALE.ARCBILL_KEY "
                                "WHERE a.FIELD_INDEX IN(" + indexKeysList + " ) AND meh.IS_ACTIVE = :IS_ACTIVE "
                                "AND a.DATE_CREATED >= :START_TIME and a.DATE_CREATED < :END_TIME "
                                "ORDER BY 1,5 ASC; ";
        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ParamByName("START_TIME")->AsDateTime = SDate;
        IBInternalQuery->ParamByName("END_TIME")->AsDateTime = EDate;
        IBInternalQuery->ExecQuery();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
       {
          ///prepare sales data
          TMallExportSalesData salesData;
          salesData.ArcBillKey =  IBInternalQuery->Fields[0]->AsInteger;
          salesData.CreatedBy  =IBInternalQuery->Fields[1]->AsString;
          salesData.DateCreated =IBInternalQuery->Fields[2]->AsDateTime;
          salesData.Field = IBInternalQuery->Fields[3]->AsString;
          salesData.FieldIndex = IBInternalQuery->Fields[4]->AsInteger;
          salesData.DataValue = IBInternalQuery->Fields[4]->AsString + "" + IBInternalQuery->Fields[5]->AsString;
          salesData.DataValueType = IBInternalQuery->Fields[6]->AsString;

          //insert sales data object into   list
          salesDataForISF.push_back(salesData);
       }

       //insert list into TMallExportPrepareData's map
       prepareDataForInvoice.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, salesDataForISF ));
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------
void TfrmRegenerateMallReport::PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys,
                                                                                TMallExportPrepareData &prepareDataForHSF, int index)
{
    //Create List Of SalesData for hourly file
    std::list<TMallExportSalesData> prepareListForHSF;
    try
    {
        ///Store First Letter of file name
        UnicodeString fileName = "H";

        //Seperate key with commas in the form of string.
        UnicodeString indexKeysList = estanciaMall.GetFieldIndexList(indexKeys);

        ///Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Declare Set For storing index
        std::set<int>keysToSelect;

        //Create array for storing index by which file name will be prepared
        int  fileNameKeys[4] = {1, 2, 3, 33};

        //Store keys into set
        keysToSelect = estanciaMall.InsertInToSet(fileNameKeys, 4);

        //Get file name according to field index.
        fileName = fileName + "" + estanciaMall.GetFileName(dBTransaction, keysToSelect);

        //insert filename into map according to index and file type
        prepareDataForHSF.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

         //insert indexes into array for fetching tenant code, date , terminal number
        int hourIndexKeys[3] = {1, 2, 3};

        //clear the map
        keysToSelect.clear();

        //Store keys into set
        keysToSelect = estanciaMall.InsertInToSet(hourIndexKeys, 3);

        ///Load MallSetting For writing into file
        estanciaMall.LoadMallSettingsForFile(dBTransaction, prepareDataForHSF, keysToSelect, index);

        //Query for selecting data for hourly file
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                             "SELECT LPAD((CASE WHEN (HOURLYDATA.FIELD_INDEX = 32) THEN 7  "
                                            "WHEN (HOURLYDATA.FIELD_INDEX = 34) THEN 6 "
                                            "WHEN (HOURLYDATA.FIELD_INDEX = 66) THEN 4 "
                                            "ELSE (HOURLYDATA.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                    "HOURLYDATA.FIELD, "
                                    "SUM(HOURLYDATA.FIELD_VALUE) FIELD_VALUE , "
                                    "HOURLYDATA.VALUE_TYPE , "
                                    "HOURLYDATA.Hour_code  "
                            "FROM "
                                "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS int) FIELD_VALUE, a.VALUE_TYPE, "
                                                        "meh.MM_NAME,Extract (Hour From a.DATE_CREATED) Hour_code "
                                 "FROM MALLEXPORT_SALES a "
                                 "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                 "WHERE a.FIELD_INDEX IN(32,34,66) AND meh.IS_ACTIVE = :IS_ACTIVE  "
                                 "AND a.DATE_CREATED >= :START_TIME and a.DATE_CREATED < :END_TIME "
                                 "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA  "
                            "GROUP BY 1,2,4,5 "

                            "UNION ALL "

                            "SELECT LPAD((CASE WHEN (HOURLYDATA.FIELD_INDEX = 65) THEN 5 ELSE (HOURLYDATA.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                    "HOURLYDATA.FIELD, CAST(SUM(HOURLYDATA.FIELD_VALUE)*100 AS INT ) FIELD_VALUE , HOURLYDATA.VALUE_TYPE, HOURLYDATA.Hour_code "
                            "FROM "
                                    "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE, meh.MM_NAME, "
                                            "Extract (Hour From a.DATE_CREATED) Hour_code "
                                     "FROM MALLEXPORT_SALES a "
                                     "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID"
                                    " WHERE a.FIELD_INDEX IN(65) AND meh.IS_ACTIVE = :IS_ACTIVE "
                                    "AND a.DATE_CREATED >= :START_TIME and a.DATE_CREATED < :END_TIME "
                                     "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA "
                            "GROUP BY 1,2,4 ,5 "

                            "ORDER BY 5 ASC, 1 ASC ";

        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ParamByName("START_TIME")->AsDateTime = SDate;
        IBInternalQuery->ParamByName("END_TIME")->AsDateTime = EDate;
        IBInternalQuery->ExecQuery();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;
          salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsCurrency;
          salesData.DataValueType = IBInternalQuery->Fields[3]->AsString;
          salesData.MallExportSalesId = IBInternalQuery->Fields[4]->AsInteger;
          prepareListForHSF.push_back(salesData);
        }

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                                "SELECT LPAD((CASE WHEN (HOURLYDATA.FIELD_INDEX = 34) THEN 9  "
                                                  "WHEN (HOURLYDATA.FIELD_INDEX = 65) THEN 8 "
                                                  "ELSE (HOURLYDATA.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                        "HOURLYDATA.FIELD, "
                                        "CAST(SUM(CASE WHEN HOURLYDATA.FIELD_INDEX = 65 THEN (HOURLYDATA.FIELD_VALUE*100) ELSE (HOURLYDATA.FIELD_VALUE) END )  AS INT ) FIELD_VALUE, "
                                         "HOURLYDATA.VALUE_TYPE "
                                "FROM "
                                "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE "
                                "FROM MALLEXPORT_SALES a  "
                                "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                "WHERE a.FIELD_INDEX IN(65, 34) AND meh.IS_ACTIVE = :IS_ACTIVE "
                                "AND a.DATE_CREATED >= :START_TIME and a.DATE_CREATED < :END_TIME "
                                "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA "
                                "GROUP BY 1,2,4 "
                                "ORDER BY 1 ASC ";

        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ParamByName("START_TIME")->AsDateTime = SDate;
        IBInternalQuery->ParamByName("END_TIME")->AsDateTime = EDate;
        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           ///prepare sales data
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;
          salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsCurrency;
          salesData.DataValueType = IBInternalQuery->Fields[3]->AsString;

          //insert prepared data into list
          prepareListForHSF.push_back(salesData);
        }

        //insert list into TMallExportPrepareData's map
        prepareDataForHSF.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, prepareListForHSF ));
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------------------
void TfrmRegenerateMallReport::PrepareDataForDailySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys,
                                                                                TMallExportPrepareData &prepareDataForDSF, int index)
{
    //Create List Of SalesData for hourly file
    std::list<TMallExportSalesData> prepareListForDSF;
    try
    {
        ///Store First Letter of file name ie; file type
        UnicodeString fileName = "D";

        //Seperate key with commas in the form of string.
        UnicodeString indexKeysList = estanciaMall.GetFieldIndexList(indexKeys);

        //Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Declare Set For storing index
        std::set<int>keysToSelect;

        //Create array for storing index by which file name will be prepared
        int  fileNameKeys[4] = {1, 2, 3, 33};

         //Store keys into set
        keysToSelect = estanciaMall.InsertInToSet(fileNameKeys, 4);

        //Get file name according to field index.
        fileName = fileName + "" + estanciaMall.GetFileName(dBTransaction, keysToSelect);

        //insert filename into map according to index and file type
        prepareDataForDSF.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

         //insert indexes into array for fetching tenant code, date , terminal number
        int dailyIndexKeys[3] = {1, 2, 3};

        //clear the map
        keysToSelect.clear();

        //Store keys into set
        keysToSelect = estanciaMall.InsertInToSet(dailyIndexKeys, 3);

        ///Load MallSetting For writing into file
        estanciaMall.LoadMallSettingsForFile(dBTransaction, prepareDataForDSF, keysToSelect, index);

        //Query for fetching data for writing into daily sales file.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD, CAST(SUM(DAILYDATA.FIELD_VALUE)*100 AS INT) FIELD_VALUE , DAILYDATA.VALUE_TYPE, DAILYDATA.Z_KEY, DAILYDATA.MM_NAME "
                                      "FROM "
                                            "(SELECT a.ARCBILL_KEY, a.FIELD, LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE, meh.MM_NAME, MAX(A.Z_KEY) Z_KEY "
                                             "FROM MALLEXPORT_SALES a "
                                             "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                             "WHERE a.FIELD_INDEX NOT IN(" + indexKeysList + ") AND meh.IS_ACTIVE = :IS_ACTIVE  "
                                             "AND a.DATE_CREATED >= :START_TIME and a.DATE_CREATED < :END_TIME "
                                             "GROUP BY a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, meh.MM_NAME, a.FIELD_VALUE  "
                                             "ORDER BY A.ARCBILL_KEY ASC )DAILYDATA "
                                    "GROUP BY 1,2,4,5,6 "
                                    "UNION ALL "
                                     "SELECT LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, a.FIELD, cast(a.FIELD_VALUE as int ) FIELD_VALUE , a.VALUE_TYPE, a.Z_KEY, meh.MM_NAME  "
                                     "FROM "
                                        "MALLEXPORT_SALES a inner join MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                        "where a.FIELD_INDEX IN( 33, 35 ) AND meh.IS_ACTIVE = :IS_ACTIVE "
                                        "AND a.DATE_CREATED >= :START_TIME and a.DATE_CREATED < :END_TIME "
                                    "GROUP BY 1,2,3,4,5,6 "
                                    "ORDER BY 1 ASC  ";   //TODO AFTER DISCUSSION
        IBInternalQuery->ParamByName("START_TIME")->AsDateTime = SDate;
        IBInternalQuery->ParamByName("END_TIME")->AsDateTime = EDate;
        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ExecQuery();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
       {
           ///prepare sales data
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;
          salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsCurrency;
          salesData.DataValueType = IBInternalQuery->Fields[3]->AsString;
          salesData.ZKey = IBInternalQuery->Fields[4]->AsInteger;

          //insert prepared data into list
          prepareListForDSF.push_back(salesData);
       }

       //insert list into TMallExportPrepareData's map
       prepareDataForDSF.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, prepareListForDSF ));
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------------------
UnicodeString TfrmRegenerateMallReport::FixTime(UnicodeString Time)
{
    UnicodeString result = "";

    if(Time.Length() == 1)
    {
        result = "0" + Time;
    }
    else
    {
        result = Time;
    }
    return result;
}
