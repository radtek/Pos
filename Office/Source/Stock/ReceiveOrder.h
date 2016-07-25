//---------------------------------------------------------------------------

#ifndef ReceiveOrderH
#define ReceiveOrderH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <StdCtrls.hpp>
#include "ReceiveInvoice.h"
//---------------------------------------------------------------------------
class TfrmReceiveOrder : public TForm
{
__published:	// IDE-managed Components
	TIBQuery *qrOrder;
	TIBQuery *qrOrderStock;
	TDataSource *dsOrderStock;
	TIBTransaction *Transaction;
	TIBQuery *qrSetUndelivered;
	TIBQuery *qrSetOnOrder;
	TIBQuery *qrGetOnOrder;
	TPanel *Panel1;
	TPanel *Panel4;
	TPanel *Panel5;
	TTreeView *tvOrders;
	TPanel *Panel6;
	TBitBtn *btnNewInvoice;
	TBitBtn *btnFind;
	TLabel *lbeTitle;
	TLabel *lbeReference;
	TDBGrid *dbgPurchaseStock;
	TButton *btnUndelivered;
	TBitBtn *btnClose;
	TIBQuery *qrOrders;
	TIBQuery *qrInvoiceExists;
	TFindDialog *FindDialog;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall qrOrderStockAfterOpen(TDataSet *DataSet);
	void __fastcall tvOrdersChange(TObject *Sender, TTreeNode *Node);
	void __fastcall btnNewInvoiceClick(TObject *Sender);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall FindDialogFind(TObject *Sender);
	void __fastcall btnUndeliveredClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
	void ResizeGrids();
	void LoadTree();
	bool InvoiceExists(AnsiString InvoiceReference);
	const std::auto_ptr<TfrmReceiveInvoice>frmReceiveInvoice;
public:		// User declarations
	__fastcall TfrmReceiveOrder(TComponent* Owner);
	AnsiString OrderNumber;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmReceiveOrder *frmReceiveOrder;
//---------------------------------------------------------------------------
#endif
