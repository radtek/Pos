//---------------------------------------------------------------------------
#ifndef TimeClockH
#define TimeClockH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include "ZForm.h"
#include "Contact.h"

//---------------------------------------------------------------------------
class TfrmTimeClock : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel8;
	TPanel *Panel1;
	TPanel *Panel5;
	TPanel *Panel6;
	TPanel *Panel4;
	TPanel *pnlLable;
	TTouchBtn *tbtnClockOut;
	TTouchGrid *tbgLogin;
	TTouchBtn *tbtnOk;
	TTouchBtn *btnChangePIN;
	TTouchBtn *btnBreakTimes;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall btnClockOutClick(TObject *Sender);
   void __fastcall btnOKClick(TObject *Sender);
	void __fastcall btnChangePINClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ClockInClick(TObject *Sender);
	void __fastcall tbgLoginMouseClick(TObject *Sender, TMouseButton Button,
			 TShiftState Shift, TGridButton *GridButton);
	void __fastcall btnBreakTimesMouseClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall CardSwipe(Messages::TMessage& Message);
	BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	  MESSAGE_HANDLER(UWM_CARDSWIPE, Messages::TMessage, CardSwipe)
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
	__fastcall TfrmTimeClock(TComponent* Owner,Database::TDBControl &inIBDatabase);
	void DisableLoginButtons();
	void EnableLoginButtons();
	AnsiString SwipeString;
	bool Swiping;
	TMMContactInfo CurrentUser;
	Database::TDBControl &IBDatabase;
	void UpdateButtonGrid(bool GridEnabled);
public:		// User declarations
};
//---------------------------------------------------------------------------
#endif
