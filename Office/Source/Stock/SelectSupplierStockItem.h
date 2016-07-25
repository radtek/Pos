//---------------------------------------------------------------------------

#ifndef SelectSupplierStockItemH
#define SelectSupplierStockItemH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <Buttons.hpp>
#include <IBDatabase.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TfrmSelectSupplierStockItem : public TForm
{
__published:	// IDE-managed Components
   TIBTransaction *Transaction;
	TPanel *Panel2;
	TPanel *Panel1;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *Label1;
	TTreeView *tvStock;
	TBitBtn *btnSelect;
	TBitBtn *btnClose;
	TBitBtn *btnFind;
	TPanel *Panel3;
	TBitBtn *btnOther;
	TFindDialog *FindDialog;
	TIBQuery *qrSupplierStock;
    TIBQuery *qrLocationSupplierStock;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall btnCloseClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall btnSelectClick(TObject *Sender);
   void __fastcall btnOtherClick(TObject *Sender);
   void __fastcall DBGrid1DblClick(TObject *Sender);
	void __fastcall tvStockChange(TObject *Sender, TTreeNode *Node);
	void __fastcall tvStockDblClick(TObject *Sender);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall FindDialogFind(TObject *Sender);
private:	// User declarations
	bool Registered;
	void LoadTree();
//	int GetStockItemCount();
	static int LastStockKey;
public:		// User declarations
   __fastcall TfrmSelectSupplierStockItem(TComponent* Owner);
	int			CompanyKey;
	AnsiString	StockCode;
   AnsiString Location;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmSelectSupplierStockItem *frmSelectSupplierStockItem;
//---------------------------------------------------------------------------
#endif
