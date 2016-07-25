//---------------------------------------------------------------------------
#ifndef LoyaltyH
#define LoyaltyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmLoyalty : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox4;
	TLabel *lbeCustomerName;
	TLabel *lbeContactPhone;
	TLabel *lbeCustomerTotal;
	TLabel *lbeExpiryDate;
	TLabel *lbeCardNumber;
	TGroupBox *gbCustomerName;
	TDBGrid *DBGrid1;
	TDBEdit *DBEdit1;
	TDBEdit *DBEdit3;
	TDBEdit *DBEdit4;
	TDBEdit *DBEdit5;
	TDBEdit *DBEdit6;
	TGroupBox *GroupBox1;
	TPanel *Panel1;
	TBitBtn *btn3;
	TBitBtn *btn2;
	TBitBtn *btn1;
	TBitBtn *btn7;
	TBitBtn *btn8;
	TBitBtn *btn9;
	TBitBtn *btn4;
	TBitBtn *btn0;
	TBitBtn *btn5;
	TBitBtn *btn6;
	TBitBtn *btnDot;
	TBitBtn *btnClear;
	TEdit *edDollars;
	TButton *Button1;
	TLabel *Label1;
	TPanel *Panel2;
	TPanel *Panel15;
	TImage *imgExit;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall imgExitClick(TObject *Sender);
	void __fastcall btn7Click(TObject *Sender);
	void __fastcall btnClearClick(TObject *Sender);
	void __fastcall DBEdit1Enter(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
private:	// User declarations
   Currency wrkPayAmount;
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
public:		// User declarations
	__fastcall TfrmLoyalty(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLoyalty *frmLoyalty;
//---------------------------------------------------------------------------
#endif
