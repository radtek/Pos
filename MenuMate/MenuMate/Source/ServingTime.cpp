//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ServingTime.h"
#include "MMMessageBox.h"
#include "MMTouchNumpad.h"
#include "MMTouchKeyboard.h"


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
  //  TouchTime1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TfrmServingTime::btnCloseMouseClick(TObject *Sender)
{
    if(TimeType == startTime)
    {

       // Time1
       Time1 = EncodeTime(StrToInt(BtnServingTimeHour->Caption),StrToInt(BtnServingTimeMinute->Caption),StrToInt(BtnServingTimeSecond->Caption),0);
       SetZeroToTime();
       ModalResult = mrOk;
    }
    else
    {
        Time2 = EncodeTime(StrToInt(BtnServingTimeHour->Caption),StrToInt(BtnServingTimeMinute->Caption),StrToInt(BtnServingTimeSecond->Caption),0);
        if(Time1 < Time2)
        {
           SetZeroToTime();
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
     SetZeroToTime();
     FormResize(Sender);
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmServingTime::FormResize(TObject *Sender)
{
//	PanelDetails->Left = (Screen->Width - PanelDetails->Width) / 2;
//	PanelDetails->Top  = (Screen->Height - PanelDetails->Height) / 2;
}
//---------------------------------------------------------------------------
void _fastcall TfrmServingTime::BtnServingTimeHourMouseClick(TObject *Sender)
{


      std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
      frmTouchNumpad->Mode = pmNumber;
      frmTouchNumpad->btnSurcharge->Caption = "Ok";
      frmTouchNumpad->btnDiscount->Visible = false;
      frmTouchNumpad->btnSurcharge->Visible = true;
      frmTouchNumpad->Caption = "Enter the Hour";
      UnicodeString Title = frmTouchNumpad->Caption;
      frmTouchNumpad->SetMaxLengthValue(2);
      int initialValue = 0;
      AnsiString Hour = "";
      frmTouchNumpad->INTInitial = initialValue;
      if (frmTouchNumpad->ShowModal() == mrOk)
       {
        if(ValidateData(Title,frmTouchNumpad->INTResult))
        {
           Hour = frmTouchNumpad->INTResult;
           BtnServingTimeHour->Caption = Hour;
        }

      }
}

//----------------------------------------------------------------------------------
void _fastcall TfrmServingTime::BtnServingTimeMinuteMouseClick(TObject *Sender)
{      std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
      frmTouchNumpad->Mode = pmNumber;
      frmTouchNumpad->btnSurcharge->Caption = "Ok";
      frmTouchNumpad->btnDiscount->Visible = false;
      frmTouchNumpad->btnSurcharge->Visible = true;
      frmTouchNumpad->Caption = "Enter the Minute";
      UnicodeString Title = frmTouchNumpad->Caption;
      frmTouchNumpad->SetMaxLengthValue(2);
      int initialValue = 0;
      AnsiString Minute = "";
      frmTouchNumpad->INTInitial = initialValue;
      if (frmTouchNumpad->ShowModal() == mrOk)
       {
           if(ValidateData(Title,frmTouchNumpad->INTResult))
            {
               Minute = frmTouchNumpad->INTResult;
                BtnServingTimeMinute->Caption = Minute;
            }
       }
}

//-------------------------------------------------------------------------------------------------------------------
void _fastcall TfrmServingTime::BtnServingTimeSecondMouseClick(TObject *Sender)
{      std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
      frmTouchNumpad->Mode = pmNumber;
      frmTouchNumpad->btnSurcharge->Caption = "Ok";
      frmTouchNumpad->btnDiscount->Visible = false;
      frmTouchNumpad->btnSurcharge->Visible = true;
      frmTouchNumpad->Caption = "Enter the Seconds";
      UnicodeString Title = frmTouchNumpad->Caption;
      frmTouchNumpad->SetMaxLengthValue(2);
      int initialValue = 0;
      AnsiString Seconds = "";
      frmTouchNumpad->INTInitial = initialValue;
      if (frmTouchNumpad->ShowModal() == mrOk)
       {
           if(ValidateData(Title,frmTouchNumpad->INTResult))
            {
                Seconds = frmTouchNumpad->INTResult;
                BtnServingTimeSecond->Caption = Seconds;
            }
       }

}
//------------------------------------------------------------------------------------------------
bool TfrmServingTime::ValidateData(UnicodeString &Title , int &value)
{
  bool retval = false;

  if(Title == "Enter the Hour")
  {
     if(value < 24)
     {
        retval = true;
     }
     else
     {
       MessageBox("Invalid Hour, Please Enter 24 hour Format", "Invalid Hour", MB_OK + MB_ICONERROR);
     }
  }
  else if(Title == "Enter the Minute")
  {
     if(value < 60)
     {
        retval = true;
     }
     else
     {
       MessageBox("Invalid Minute, Please Enter Valid Minute", "Invalid Minute", MB_OK + MB_ICONERROR);
     }
  }

  else if(Title == "Enter the Seconds")
  {
   if(value < 60)
   {
        retval = true;
   }
   else
   {
          MessageBox("Invalid Second, Please Enter Valid Second", "Invalid Second", MB_OK + MB_ICONERROR);

   }
  }
  return retval;
}
//----------------------------------------------------------------------------------------------------------------
void TfrmServingTime::SetZeroToTime()
{
  BtnServingTimeHour->Caption = "00";
  BtnServingTimeMinute->Caption = "00";
  BtnServingTimeSecond->Caption = "00";

}
