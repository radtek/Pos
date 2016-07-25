//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectDateAndTime.h"
//--------------------------
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchTime"
#pragma resource "*.dfm"
#include "MMTouchNumpad.h"
#include "MMMessageBox.h"
#include <DateUtils.hpp>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
__fastcall TfrmSelectDateTime::TfrmSelectDateTime(TComponent* Owner)
        : TForm(Owner)
{

}

//---------------------------------------------------------------------------
void __fastcall TfrmSelectDateTime::FormShow(TObject *Sender)
{

    DeliveryDate = Now();
    DeliveryDate = IncMinute(DeliveryDate,MinutesToAdd);
    BtnDate->Caption = DayOf(DeliveryDate);
    BtnMonth->Caption = MonthOf(DeliveryDate);
    BtnYear->Caption = YearOf(DeliveryDate);
    BtnHour->Caption = HourOf(DeliveryDate);
    BtnMinute->Caption = MinuteOf(DeliveryDate);
    BtnAMPM->Caption ="AM" ;
    int Hour = HourOf(DeliveryDate);
    if(Hour >= 12)
    {
       BtnAMPM->Caption ="PM" ;
       if(Hour != 12)
       {
         BtnHour->Caption = IntToStr(Hour - 12);
       }
     }
     if(Hour == 0)
     {
       BtnHour->Caption = IntToStr(12);
     }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectDateTime::btnOkMouseClick(TObject *Sender)
{
    int Day = StrToInt(BtnDate->Caption);
    int Month = StrToInt(BtnMonth->Caption);
    int Year = StrToInt(BtnYear->Caption);
    int Hour = 0;
    if(StrToInt(BtnHour->Caption) != 12)
    {
      Hour = BtnAMPM->Caption == "PM" ?  StrToInt(BtnHour->Caption) + 12 : StrToInt(BtnHour->Caption);
    }
    else
    {
      if(BtnAMPM->Caption == "AM")
      {
        Hour = 0;
      }
      else
      {
        Hour = StrToInt(BtnHour->Caption);
      }
    }
    if(Hour > 23)
    {
      Hour = 0;
    }
    int Minute = StrToInt(BtnMinute->Caption);
    TDateTime encodedDate =  EncodeDateTime(Year,Month,Day,Hour,Minute,59,999);
    TValueRelationship result  = CompareDateTime(encodedDate,Now());
    if(result == LessThanValue)
    {
        if(Year<(YearOf(Now())) || Month<(MonthOf(Now())) ||Day<(DayOf(Now())))
        {
           MessageBox("Date entered is prior to current date, please enter a valid date.", "Invalid Date", MB_OK + MB_ICONERROR);
        }
        else
        {
        MessageBox("Time entered is prior to current time, please enter a valid time.", "Invalid Time", MB_OK + MB_ICONERROR);
        }

   //   MessageBox("Invalid Date, Please Fix and Try again", "Invalid Date", MB_OK + MB_ICONERROR);
    }
    else
	{
   	   TFormatSettings Settings;
       GetLocaleFormatSettings(10258, Settings);
       encodedDate = EncodeDateTime(Year,Month,Day,StrToInt(BtnHour->Caption),Minute,59,999);

       if(Settings.ShortDateFormat == "d/MM/yyyy" || Settings.ShortDateFormat == "dd-MM-yyyy")
       {
         SelectedTime = encodedDate.FormatString("DD/MM/YYYY hh:mm '" +(BtnAMPM->Caption)+ "'");
       }
       else if(Settings.ShortDateFormat == "d/MM/yy" || Settings.ShortDateFormat == "dd-MM-YY")
       {
         SelectedTime = encodedDate.FormatString("DD/MM/YY hh:mm '" +(BtnAMPM->Caption)+ "'");
      }
      else if(Settings.ShortDateFormat == "DD/MM/YY" || Settings.ShortDateFormat == "D-M-YY")
      {
        SelectedTime = encodedDate.FormatString("DD/MM/YY hh:mm '" +(BtnAMPM->Caption)+ "'");
      }
      else if(Settings.ShortDateFormat == "yyyy-MM-dd")
      {

        SelectedTime = encodedDate.FormatString("YYYY/MM/DD hh:mm '" +(BtnAMPM->Caption)+ "'");
      }
      else
      {
        SelectedTime = encodedDate.FormatString("DD/MM/YY hh:mm '" +(BtnAMPM->Caption)+ "'");
      }
      ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectDateTime::tbtnCancelMouseClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectDateTime::BtnDateMouseClick(TObject *Sender)
{
   	BtnDate->Caption = PopUpDatePicker("Enter the Date", UnicodeString(BtnDate->Caption));
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectDateTime::BtnMonthMouseClick(TObject *Sender)
{
	BtnMonth->Caption = PopUpDatePicker("Enter the Month", UnicodeString(BtnMonth->Caption));
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectDateTime::BtnYearMouseClick(TObject *Sender)
{
	BtnYear->Caption = PopUpDatePicker("Enter the Year", UnicodeString (BtnYear->Caption));
}
//---------------------------------------------------------------------------

UnicodeString TfrmSelectDateTime::PopUpDatePicker(AnsiString Title, UnicodeString &value)
{
    UnicodeString value1 = "";
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
    frmTouchNumpad->Caption = Title;
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->PreSelect = true;
	frmTouchNumpad->Mode = pmNumber;
    if(value == "Day" || value == "Month" || value == "Year")
    {
        value1 = value;
    }

    if(value1 == "")
    {
	    frmTouchNumpad->INTInitial = StrToInt(value);
    }

	if (frmTouchNumpad->ShowModal() == mrOk)
	{
      if(ValidateData(Title, frmTouchNumpad->INTResult))
      {
         value = IntToStr(frmTouchNumpad->INTResult);
      }
	}
   return value;
}


void __fastcall TfrmSelectDateTime::BtnHourMouseClick(TObject *Sender)
{
     BtnHour->Caption = PopUpDatePicker("Enter the Hour", UnicodeString (BtnHour->Caption));
}
//

void __fastcall TfrmSelectDateTime::BtnMinuteMouseClick(TObject *Sender)
{
	BtnMinute->Caption = PopUpDatePicker("Enter the Minute", UnicodeString (BtnMinute->Caption));
}
//

void __fastcall TfrmSelectDateTime::BtnAMPMMouseClick(TObject *Sender)
{
    if(BtnAMPM->Caption == "PM")
    {
      BtnAMPM->Caption = "AM";
    }
    else
    {
      BtnAMPM->Caption = "PM";
    }

}

//---------------------------------------------------------------------------


bool TfrmSelectDateTime::ValidateData(AnsiString &Title, int &value)
{
  bool retval = false;

  if(Title == "Enter the Year")
  {
     if(value >= YearOf(Now()) && value <=9999)
     {
       retval = true;
     }
     else
     {
        MessageBox("Invalid Year, Please Enter Valid Year", "Invalid Year", MB_OK + MB_ICONERROR);
     }
  }
  else if(Title == "Enter the Month")
  {
     if(value != 0 && value < 13)
     {
        retval = true;
     }
     else
     {
       MessageBox("Invalid Month, Please Enter Valid Month", "Invalid Month", MB_OK + MB_ICONERROR);
     }
  }
  else if(Title == "Enter the Date")
  {
     if(value != 0 && value < 32)
     {
        retval = true;
     }
     else
     {
       MessageBox("Invalid Date, Please Enter Valid Date", "Invalid Date", MB_OK + MB_ICONERROR);
     }
  }
  else if(Title == "Enter the Hour")
  {
     if(value != 0 && value < 13)
     {
        retval = true;
     }
     else
     {
       MessageBox("Invalid Hour, Please Enter 12 hour Format", "Invalid Hour", MB_OK + MB_ICONERROR);
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
  return retval;
}


