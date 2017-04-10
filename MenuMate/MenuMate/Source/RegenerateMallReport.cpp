//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include <fstream>
#include <sstream>
#include <string>
#include "RegenerateMallReport.h"
#include "MMTouchKeyboard.h"
#include "DeviceRealTerminal.h"
#include "EstanciaMall.h"
#include "MallExport.h"
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
    if(EDate >= SDate)
    {
        if(TGlobalSettings::Instance().mallInfo.MallId)
        {
            //Instantiation is happenning in a factory based on the active mall in database
            TMallExport* mall = TMallFactory::GetMallType();
            mall->RegenerateMallReport(SDate, EDate);
            delete mall;
        }
    }
    else
    {
        MessageBox( "End date is set prior to start date", "Invalid Date For File Generation", MB_OK );
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
//  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
//	frmTouchKeyboard->MaxLength = 300;
//	frmTouchKeyboard->AllowCarriageReturn = false;
//	frmTouchKeyboard->StartWithShiftDown = false;
//	frmTouchKeyboard->KeyboardText = edLocationPath->Text;
//	frmTouchKeyboard->Caption = "Enter File Location";
//	if (frmTouchKeyboard->ShowModal() == mrOk)
//	{
//        edLocationPath->Text = frmTouchKeyboard->KeyboardText;
//	}
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
void TfrmRegenerateMallReport::SetSpecificMallTimes(int &StartH, int &EndH, int &StartM, int &EndM)
{
        StartH = 5;
        EndH = 5;
        StartM = 0;
        EndM = 0;
}
//---------------------------------------------------------------------------
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
