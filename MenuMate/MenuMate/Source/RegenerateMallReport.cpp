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

    SDate = (StartDate + StrToTime(StartHM));
    EDate = EndDate + StrToTime(EndHM) ;
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
}
//---------------------------------------------------------------------------
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
                                    "ORDER BY 5, 1 ASC  ";   //TODO AFTER DISCUSSION
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
