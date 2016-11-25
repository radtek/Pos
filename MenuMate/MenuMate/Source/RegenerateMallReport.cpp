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
    int zKey;

    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    ///Register Query
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    dbTransaction.RegisterQuery(IBInternalQuery);

    //Query for selecting data for invoice file
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text =  "SELECT a.Z_KEY FROM MALLEXPORT_SALES a "
                                    "WHERE a.Z_KEY != :Z_KEY AND a.DATE_CREATED >= :START_TIME AND a.DATE_CREATED < :END_TIME "
                                    "GROUP BY a.Z_KEY "
                                    "ORDER BY 1 ASC ";

    IBInternalQuery->ParamByName("Z_KEY")->AsInteger = 0;
    IBInternalQuery->ParamByName("START_TIME")->AsDateTime = SDate;
    IBInternalQuery->ParamByName("END_TIME")->AsDateTime = EDate;
    IBInternalQuery->ExecQuery();

   for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
   {
        //Fetch z-key
        zKey = IBInternalQuery->Fields[0]->AsInteger;

        //Prepare Data For Exporting into File
        preparedData = PrepareDataForExport(dbTransaction, zKey);

        //Create Export Medium
        TMallExportTextFile* exporter =  new TMallExportTextFile();
        exporter->WriteToFile(preparedData);
    }

     //Commit the transaction as we have completed all the transactions
    dbTransaction.Commit();
}
//---------------------------------------------------------------------------
// Initialize Start and End Time for each mall
void TfrmRegenerateMallReport::SetSpecificMallTimes(int &StartH, int &EndH, int &StartM, int &EndM)
{
        StartH = 5;
        EndH = 5;
        StartM = 0;
        EndM = 0;
}
//---------------------------------------------------------------------------
TMallExportPrepareData TfrmRegenerateMallReport::PrepareDataForExport(Database::TDBTransaction &dbTransaction, int zKey)
{
    //Create TMallExportPrepareData  for returning prepared data
    TMallExportPrepareData preparedData;

    try
    {
        //Set for inserting index. these indexes will be used for fetching data
        std::set<int> keyToCheck;

        //Indexes for which data will not selected
        int dailySalekeys[8] = {1, 2, 3, 33, 35, 66, 67, 68};

        //insert these indexes into set.
        keyToCheck = estanciaMall.InsertInToSet(dailySalekeys, 8);

        //Prepare Data For Daily Sales File
        estanciaMall.PrepareDataForDailySalesFile(dbTransaction, keyToCheck, preparedData, 1, zKey);

       //indexes for selecting total Net sale, patron count, etc
        int  hourIndexkeys[3] = {65, 64, 32};

        //Clear the map because same map is used for many time insertion
        keyToCheck.clear();

        //insert these indexes into set.
        keyToCheck = estanciaMall.InsertInToSet(hourIndexkeys, 3);

        //Prepare Data For Hourly File
        estanciaMall.PrepareDataForHourlySalesFile(dbTransaction, keyToCheck, preparedData, 2, zKey);

        //indexes for selecting total Net sale, invoice number , status
        int invoiceIndex[3] = {65, 67, 68};

         //Clear the map because same map is used for many time insertion
        keyToCheck.clear();

        //insert these indexes into set.
        keyToCheck = estanciaMall.InsertInToSet(invoiceIndex, 3);

        //Prepare Data For Invoice File
        estanciaMall.PrepareDataForInvoiceSalesFile(dbTransaction, keyToCheck, preparedData, 3, zKey);
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
