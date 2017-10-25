//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ServingTime.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchTime"
#pragma resource "*.dfm"
TfrmServingTime *frmServingTime;
//---------------------------------------------------------------------------
__fastcall TfrmServingTime::TfrmServingTime(TComponent* Owner)
    : TZForm(Owner)
{
    TouchTime1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TfrmServingTime::btnCloseMouseClick(TObject *Sender)
{
    if(TimeType == startTime)
    {
        Time1 = TouchTime1->Time;
        ModalResult = mrOk;
    }
    else
    {
        Time2 = TouchTime1->Time;
        if(Time1 < Time2)
        {
          ModalResult = mrOk;
        }
        else
          MessageBox("Meal end time should be more than start time","Warning",MB_OK);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmServingTime::btnCancelMouseClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmServingTime::FormShow(TObject *Sender)
{
     FormResize(Sender);
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmServingTime::FormResize(TObject *Sender)
{
//	PanelDetails->Left = (Screen->Width - PanelDetails->Width) / 2;
//	PanelDetails->Top  = (Screen->Height - PanelDetails->Height) / 2;
}
//---------------------------------------------------------------------------
