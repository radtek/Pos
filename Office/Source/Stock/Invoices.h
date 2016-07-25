//---------------------------------------------------------------------------

#ifndef InvoicesH
#define InvoicesH
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
#include <IBQuery.hpp>
#include <Buttons.hpp>
#include <IBDatabase.hpp>
#include <DB.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <IBSQL.hpp>
#include <memory>
#include "ReceiveStockItem.h"
#include "ReceiveInvoice.h"
//---------------------------------------------------------------------------
class TfrmInvoices : public TForm
{
__published:	// IDE-managed Components
	TIBQuery *qrContact;
	TDataSource *dsContact;
	TIBTransaction *Transaction;
   TIBQuery *qrInvoice;
   TDataSource *dsInvoice;
   TIBQuery *qrInvoiceExists;
   TIBQuery *qrInvoiceDetail;
   TDataSource *dsInvoiceDetail;
   TIBQuery *qrInvoiceTotal;
   TDataSource *dsInvoiceTotal;
	TPanel *Panel2;
	TPanel *Panel8;
	TPanel *Panel9;
	TDBGrid *dbgDetails;
	TLabel *Label3;
	TLabel *lbeGSTCaption;
	TDBText *dbtInvoiceTotal;
	TBitBtn *btnUpdateInvoice;
	TBitBtn *btnNewInvoice;
	TBitBtn *btnClose;
	TLabel *lbeTitle;
	TTreeView *tvInvoices;
	TPanel *Panel1;
	TLabel *lbeReference;
	TBitBtn *btnFind;
	TFindDialog *FindDialog;
	TDBText *dbtGST;
	TLabel *Label1;
	TDBText *dbtInvoiceTotalIncluding;
    TBitBtn *btnPrintInvoice;
    TIBQuery *qrSupplier;
    TIBQuery *qrIsCommitted;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormResize(TObject *Sender);
   void __fastcall btnNewInvoiceClick(TObject *Sender);
   void __fastcall btnUpdateInvoiceClick(TObject *Sender);
   void __fastcall qrInvoiceDetailAfterScroll(TDataSet *DataSet);
   void __fastcall qrInvoiceTotalAfterScroll(TDataSet *DataSet);
	void __fastcall tvInvoicesChange(TObject *Sender, TTreeNode *Node);
	void __fastcall FindDialogFind(TObject *Sender);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall tvInvoicesDblClick(TObject *Sender);
	void __fastcall tvInvoicesKeyPress(TObject *Sender, char &Key);
    void __fastcall btnPrintInvoiceClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
    int TransactionNumber;      
    void ResizeGrids();
    bool InvoiceExists(AnsiString InvoiceRef);
    void LoadTree();

    const std::auto_ptr<TfrmReceiveStockItem>frmReceiveStockItem;
    const std::auto_ptr<TfrmReceiveInvoice>frmReceiveInvoice;
public:		// User declarations
	__fastcall TfrmInvoices(TComponent* Owner);
	int ContactKey;
   AnsiString SupplierCode;
   TDateTime LastDateCreated;
   AnsiString InvoiceRef ;
   AnsiString BatchKeyVal;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmInvoices *frmInvoices;
//---------------------------------------------------------------------------
#endif
