//---------------------------------------------------------------------------
#ifndef CardSwipeH
#define CardSwipeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ZForm.h"
#include "enum.h"
//---------------------------------------------------------------------------
class TfrmCardSwipe : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TTimer *tiSwipe;
   TPanel *Panel16;
   TPanel *Panel1;
   TTouchBtn *tbtnCancel;
   TTouchBtn *tbOtherOpt;
   TAnimate *Animate1;
   TLabel *Label1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall tbOtherOptClick(TObject *Sender);
protected:
	void __fastcall CardSwipe(Messages::TMessage& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(UWM_CARDSWIPE, Messages::TMessage, CardSwipe);
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
   bool Swiping;
   bool Done;
	__fastcall TfrmCardSwipe(TComponent* Owner);
public:		// User declarations

	UnicodeString SwipeString;
};
//---------------------------------------------------------------------------
#endif
