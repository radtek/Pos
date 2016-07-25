//---------------------------------------------------------------------------

#ifndef DebtorsMainH
#define DebtorsMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmDebtorsMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel13;
	TPanel *Panel5;
	TPanel *Panel3;
	TPanel *Panel4;
	TLabel *Label2;
	TPanel *Panel6;
	TPanel *Panel10;
	TPanel *Panel11;
	TLabel *Label3;
	TPanel *Panel12;
	TBitBtn *btnEditStock;
	TBitBtn *btnPrintInvoice;
	TBitBtn *btnStatement;
	TBitBtn *btnClose;
	TBitBtn *btnDiscounts;
	TBitBtn *btnBalance;
	TBitBtn *btnAllStatements;
	TBitBtn *BitBtn1;
	void __fastcall btnEditStockClick(TObject *Sender);
	void __fastcall btnPrintInvoiceClick(TObject *Sender);
	void __fastcall btnStatementClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall btnAllStatementsClick(TObject *Sender);
	void __fastcall btnBalanceClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmDebtorsMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDebtorsMain *frmDebtorsMain;
//---------------------------------------------------------------------------
#endif
