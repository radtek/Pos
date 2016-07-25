//---------------------------------------------------------------------------
#ifndef SelectDateH
#define SelectDateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmSelectDate : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TPanel *Panel10;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TMonthCalendar *Calendar;
	void __fastcall CalendarDblClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmSelectDate(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectDate *frmSelectDate;
//---------------------------------------------------------------------------
#endif
