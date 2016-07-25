//---------------------------------------------------------------------------
#ifndef MMTouchKeyboardH
#define MMTouchKeyboardH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchKeyboard.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------

class TfrmTouchKeyboard : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel16;
	TPanel *Panel21;
	TLabel *lbeCaption;
	TTouchBtn *TouchBtn1;
	TTouchBtn *TouchBtn2;
	TTouchBtn *TouchBtn3;
	TPanel *pnlKeyboard;
	TMMTouchKeyboard *TouchKeyboard;
	TMemo *memText;
	TPanel *Panel3;
	TPanel *Panel4;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnDoneClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall memTextKeyPress(TObject *Sender, char &Key);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall ClearClick(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall memTextKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall TouchKeyboardClick(TObject *Sender, wchar_t Char);
        void __fastcall TouchBtn4MouseClick(TObject *Sender);
        void __fastcall TouchBtn5MouseClick(TObject *Sender);



private:	// User declarations
	__fastcall TfrmTouchKeyboard(TComponent* Owner);
	
	bool ShiftDown;
   bool CapsDown;
   void AddStringToText(AnsiString stringToAdd);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)

public:		// User declarations



   // Write to this string to set initial text.
   // Read from it on a ModalResult of mrOk for the result.
   UnicodeString KeyboardText;
   bool AllowCarriageReturn;
   bool CloseOnDoubleCarriageReturn;
   bool MustHaveValue;   
   int MaxLength;
   bool StartWithShiftDown;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
