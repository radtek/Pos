//---------------------------------------------------------------------------

#ifndef PurchaseOrdersH
#define PurchaseOrdersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <DB.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <memory>
#include "PurchaseOrder.h"
//---------------------------------------------------------------------------
class TfrmPurchaseOrders : public TForm
{
__published:	// IDE-managed Components
	TIBQuery *qrOrder;
	TDataSource *dsOrder;
	TIBTransaction *Transaction;
	TIBQuery *qrPurchStock;
	TDataSource *dsPurchStock;
	TIBQuery *qrUpdate;
	TPanel *Panel2;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TDBGrid *dbgDetails;
	TTreeView *tvInvoices;
	TPanel *Panel3;
	TLabel *lbeReference;
	TBitBtn *btnClose;
	TBitBtn *btnFind;
	TButton *btnNew;
	TButton *btnUpdate;
	TFindDialog *FindDialog;
	TIBQuery *qrSupplier;
	TButton *btnDelete;
	TButton *btnPrint;
	TButton *Button2;
	TIBQuery *qrDeleteOrder;
	TButton *btnEmailOrder;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnNewClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnUpdateClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall qrOrderAfterScroll(TDataSet *DataSet);
	void __fastcall btnPrintClick(TObject *Sender);
	void __fastcall dbgPurchaseOrderKeyPress(TObject *Sender, char &Key);
	void __fastcall tvInvoicesChange(TObject *Sender, TTreeNode *Node);
	void __fastcall tvInvoicesDblClick(TObject *Sender);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall FindDialogFind(TObject *Sender);
	void __fastcall dbgDetailsDrawColumnCell(TObject *Sender,
          const TRect &Rect, int DataCol, TColumn *Column,
          TGridDrawState State);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall btnDeleteClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
	void ResizeGrids();
	void LoadTree();
	AnsiString CurrentOrderKey;
	AnsiString EmailAddress;
	const std::auto_ptr<TfrmPurchaseOrder>frmPurchaseOrder;
public:		// User declarations
	__fastcall TfrmPurchaseOrders(TComponent* Owner);
	int ContactKey;
	void Execute();
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmPurchaseOrders *frmPurchaseOrders;
//---------------------------------------------------------------------------
#endif
