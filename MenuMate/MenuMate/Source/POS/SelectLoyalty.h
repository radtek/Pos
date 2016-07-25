//---------------------------------------------------------------------------
#ifndef SelectLoyaltyH
#define SelectLoyaltyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <DBGrids.hpp>
#include <vector>
#include "ZForm.h"
//---------------------------------------------------------------------------
class TfrmSelectLoyalty : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TBitBtn *btnExit;
	TBitBtn *btnLogin;
	TPanel *Panel2;
	TBitBtn *btn7;
	TBitBtn *btn8;
	TBitBtn *btn9;
	TBitBtn *btn6;
	TBitBtn *btn5;
	TBitBtn *btn4;
	TBitBtn *btn1;
	TBitBtn *btn2;
	TBitBtn *btn3;
	TBitBtn *btn0;
	TPanel *Panel3;
	TButton *Button1;
	TButton *Button2;
	TEdit *edSearch;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TBitBtn *BitBtn3;
	TBitBtn *BitBtn4;
	TBitBtn *BitBtn5;
	TBitBtn *BitBtn6;
	TBitBtn *BitBtn7;
	TBitBtn *BitBtn8;
	TBitBtn *BitBtn9;
	TBitBtn *BitBtn10;
	TBitBtn *BitBtn11;
	TBitBtn *BitBtn12;
	TBitBtn *BitBtn13;
	TBitBtn *BitBtn14;
	TBitBtn *BitBtn15;
	TBitBtn *BitBtn16;
	TBitBtn *BitBtn17;
	TTimer *Timer1;
	TStringGrid *StringGrid1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btn7Click(TObject *Sender);
	void __fastcall BitBtn17Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnLoginClick(TObject *Sender);
	void __fastcall btnExitClick(TObject *Sender);
	void __fastcall Button2MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Button2MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Button1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Button1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall StringGrid1Click(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
private:	// User declarations
	AnsiString SearchString;
   bool TableScrollUp;
   std::vector<int> CustomerLKs;
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
public:		// User declarations
	__fastcall TfrmSelectLoyalty(TComponent* Owner);
   long CustomerLK;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectLoyalty *frmSelectLoyalty;
//---------------------------------------------------------------------------
#endif
