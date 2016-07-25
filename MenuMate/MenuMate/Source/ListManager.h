//---------------------------------------------------------------------------

#ifndef ListManagerH
#define ListManagerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ZForm.h"
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TListManagerEvents;

typedef void (__closure *TMMListManagerEvent)(int ItemIndex, int ColIndex = -1);


class TListManagerEvents
{
   private:
	std::vector<TMMListManagerEvent> Events;
   bool RecursionProtected;
   bool Enabled;
   bool Asleep;
   bool FiredWhileAsleep;
	public:
	void RegisterForEvent(TMMListManagerEvent Function);
	void DeregisterForEvent(TMMListManagerEvent Function);
	void Occured(int ItemIndex, int ColIndex);
	AnsiString ErrorMessage;
	AnsiString ErrorTitle;
	bool HaltProceedingEvents;
	bool EventsFailed;
	void Enable();
	void Disable();
   void Sleep();
   void Wakeup();
   int EventCount();
	TListManagerEvents();
};


class TfrmListManager : public TZForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *pnlMajor;
	TPanel *pnlLabel;
	TStringGrid *sgDisplay;
	TPanel *pnlMinor;
	TTouchBtn *btnClose;
	TTouchBtn *tbtnAdd;
	TTouchBtn *tbtnDelete;
	TTouchBtn *tbtnEdit;
	TTouchBtn *btnActive;
	void __fastcall FormResize(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall tbtnAddMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall tbtnEditMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall tbtnDeleteMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall btnCloseMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnActiveMouseUp(TObject *Sender, TMouseButton Button,
			 TShiftState Shift, int X, int Y);
	void __fastcall sgDisplaySelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall sgDisplayDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
			 TGridDrawState State);


private:	// User declarations
protected :
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	 BEGIN_MESSAGE_MAP
			MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	 END_MESSAGE_MAP(TForm)
public:		// User declarations
	__fastcall TfrmListManager(TComponent* Owner);
	TGridDrawState AState;

	TListManagerEvents Close;
	TListManagerEvents Add;
	TListManagerEvents Edit;
	TListManagerEvents Delete;
	TListManagerEvents Active;   
	TListManagerEvents Select;
	TListManagerEvents DrawCell;
   void SetCaption(AnsiString inCaption);
};

//---------------------------------------------------------------------------
#endif
