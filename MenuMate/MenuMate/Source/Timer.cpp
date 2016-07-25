//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Timer.h"
#include "MMTouchNumpad.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchTime"
#pragma link "TouchBtn"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmClockTime::TfrmClockTime(TComponent* Owner, TDateTime &inDate)
	: TForm(Owner), Date(inDate)
{
	unsigned short dd, dm, dy;
	unsigned short th, tm, ts, tms;
	Date.DecodeDate(&dy, &dm, &dd);
	Date.DecodeTime(&th, &tm, &ts, &tms);

	Day = dd;
	Month = dm;
	Year = dy;
	Hour = th;
	Minute = tm;
	TimeDisplay->Time = Date;
}
//---------------------------------------------------------------------------
void __fastcall TfrmClockTime::OkMouseclick(TObject *Sender)
{
	if (!TryEncodeDate(Year, Month, Day, Date) || !TryEncodeTime(Hour, Minute, 0, 0, Date))
	{

	  MessageBox("Invalid Date, Please Fix and Try again", "Invalid Date", MB_OK + MB_ICONERROR);
	}
	else
	{
		Date = TDateTime(Year, Month, Day, Hour, Minute, 59, 999);
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmClockTime::CancelMouseClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------


void __fastcall TfrmClockTime::BtnDateMouseClick(TObject *Sender)
{
	PopUpDatePicker("Enter the Date", Day);
	BtnDate->Caption = Day;
}
//---------------------------------------------------------------------------

void TfrmClockTime::PopUpDatePicker(AnsiString Title, int &date)
{
   std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
	frmTouchNumpad->Caption = Title;
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->PreSelect = true;
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = date;

	if (frmTouchNumpad->ShowModal() == mrOk)
	{
	  date = frmTouchNumpad->INTResult;
	}

}


void __fastcall TfrmClockTime::BtnMonthMouseClick(TObject *Sender)
{
	PopUpDatePicker("Enter the Month", Month);
	BtnMonth->Caption = Month;
}
//---------------------------------------------------------------------------

void __fastcall TfrmClockTime::BtnYearMouseClick(TObject *Sender)
{
	PopUpDatePicker("Enter the Year", Year);
	BtnYear->Caption = Year;
}
//---------------------------------------------------------------------------

void __fastcall TfrmClockTime::FormShow(TObject *Sender)
{
	BtnDate->Caption = Day;
	BtnMonth->Caption = Month;
	BtnYear->Caption = Year;
	TimeDisplay->Time = Date;
}
//---------------------------------------------------------------------------

void __fastcall TfrmClockTime::TimeDisplayChange(TObject *Sender)
{
	TDateTime temp = TimeDisplay->Time;
	unsigned short th, tm, ts, tms;
	temp.DecodeTime(&th, &tm, &ts, &tms);
	Hour = th;
	Minute = tm;
}
//---------------------------------------------------------------------------

