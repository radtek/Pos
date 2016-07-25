//---------------------------------------------------------------------------
#ifndef PreviewH
#define PreviewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "TouchButton.h"
//---------------------------------------------------------------------------
class TfrmPreview : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *lbeTotal;
	TStaticText *lbeTotalCost;
	TStaticText *stOrderMode;
	TPanel *pnlChitNumber;
	TStaticText *stChitNumber;
	TStaticText *StaticText19;
	TStaticText *stTime;
	TTouchButton *stDrinksDishes;
	TPanel *Panel3;
	TTouchButton *stDrawer;
	TTouchButton *stClose;
	TPanel *sbDishes;
	TTouchButton *stLoyalty;
	TTouchButton *stCash;
	TTouchButton *stTender;
	TTouchButton *stSave;
	TPanel *Panel2;
	TPanel *Panel4;
	TLabel *Label4;
	TLabel *Label1;
	TLabel *lbeItemCost;
	TTouchButton *stRemove;
	TTouchButton *stOptions;
	TTouchButton *btnAddSide;
	TListBox *ListBox1;
	TTouchButton *btnUp;
	TTouchButton *btnDown;
	TPanel *pnlCourses;
	TScrollBox *sbCourses;
	TTouchButton *TouchButton14;
	TTouchButton *TouchButton13;
	TTouchButton *TouchButton12;
	TTouchButton *TouchButton11;
	TTouchButton *TouchButton10;
	TTouchButton *TouchButton8;
	TTouchButton *TouchButton9;
	TTouchButton *TouchButton7;
	TTouchButton *TouchButton6;
	TTouchButton *TouchButton5;
	TTouchButton *TouchButton4;
	TTouchButton *TouchButton3;
	TTouchButton *TouchButton2;
	TTouchButton *TouchButton1;
	TTouchButton *btnCourseUp;
	TTouchButton *btnCourseDown;
	TTouchButton *stSelectTable;
	TTouchButton *TouchButton15;
	TTouchButton *TouchButton16;
	TTouchButton *TouchButton17;
	TTouchButton *TouchButton18;
	TTouchButton *TouchButton19;
	TTouchButton *TouchButton20;
	TTouchButton *TouchButton21;
	TTouchButton *TouchButton22;
	TTouchButton *TouchButton23;
	TTouchButton *TouchButton24;
	TTouchButton *TouchButton25;
	TTouchButton *TouchButton26;
	TTouchButton *TouchButton27;
	TTouchButton *TouchButton28;
	TTouchButton *TouchButton29;
	TTouchButton *TouchButton30;
	TTouchButton *TouchButton31;
	TTouchButton *TouchButton32;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall stCloseClick(TObject *Sender);
	void __fastcall StaticText1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btnCourseDownClick(TObject *Sender);
	void __fastcall btnCourseUpClick(TObject *Sender);
private:	// User declarations

protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)

public:		// User declarations
	__fastcall TfrmPreview(TComponent* Owner);
   TTouchButton *CurrentCourseBtn;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmPreview *frmPreview;
//---------------------------------------------------------------------------
#endif
