//---------------------------------------------------------------------------

#ifndef TimerH
#define TimerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchControls.h"
#include "TouchTime.h"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "TouchBtn.h"
//---------------------------------------------------------------------------
class TfrmClockTime : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TTouchTime *TimeDisplay;
	TTouchBtn *btnOk;
	TTouchBtn *btnCancel;
	TTouchBtn *BtnDate;
	TTouchBtn *BtnMonth;
	TTouchBtn *BtnYear;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	void __fastcall OkMouseclick(TObject *Sender);
	void __fastcall CancelMouseClick(TObject *Sender);
	void __fastcall BtnDateMouseClick(TObject *Sender);
	void __fastcall BtnMonthMouseClick(TObject *Sender);
	void __fastcall BtnYearMouseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TimeDisplayChange(TObject *Sender);
private:	// User declarations
	void PopUpDatePicker(AnsiString Title, int &date);
	int Day;
	int Month;
	int Year;
	int Hour;
	int Minute;
	TDateTime &Date;
public:		// User declarations
	__fastcall TfrmClockTime(TComponent* Owner, TDateTime &inDate);

	int GetDay(void){return Day;}
	int GetMonth(void){return Month;}
	int GetYear(void){return Year;}
	TDateTime GetDate(void){return Date;}

	void SetDay(int inDay){Day = inDay;}
	void SetMonth(int inMonth){Month = inMonth;}
	void SetYear(int inYear){Year = inYear;}
	void SetDate(TDateTime inDate){Date = inDate;}
};
//---------------------------------------------------------------------------
//extern PACKAGE TTimer *Timer;
//---------------------------------------------------------------------------
#endif
