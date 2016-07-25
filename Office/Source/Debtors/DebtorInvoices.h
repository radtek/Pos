//---------------------------------------------------------------------------

#ifndef DebtorInvoicesH
#define DebtorInvoicesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <DB.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TfrmDebtorInvoices : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TTreeView *tvInvoices;
	TPanel *Panel1;
	TLabel *lbeReference;
	TBitBtn *btnPrintInvoice;
	TBitBtn *btnClose;
	TBitBtn *btnFind;
	TIBQuery *qrDebtorInvoices;
	TIBTransaction *Transaction;
	TIBQuery *qrInvoiceDetail;
	TDataSource *dsInvoiceDetail;
	TIBQuery *qrInvoiceTotal;
	TDataSource *dsInvoiceTotal;
	TFindDialog *FindDialog;
	TIBQuery *qrPayments;
	TDataSource *dsPayments;
	TPageControl *PageControl;
	TTabSheet *tsInvoice;
	TTabSheet *tsPayments;
	TDBGrid *dbgPayments;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *lbePayments;
	TLabel *lbeInvoiceTotal;
	TBitBtn *BitBtn1;
	TDBGrid *dbgDetails;
    TIBQuery  *qrtotalPriceTotal;
    TDataSource *dsTotalPriceTotal;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall tvInvoicesChange(TObject *Sender, TTreeNode *Node);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnPrintInvoiceClick(TObject *Sender);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall FindDialogFind(TObject *Sender);
	void __fastcall tvInvoicesDeletion(TObject *Sender, TTreeNode *Node);
private:	// User declarations
	void LoadTree();
	//void RenderInvoices(AnsiString rootText, const InvoiceMonthList& invoiceMonthList);
	void ResizeGrids();
public:		// User declarations
	__fastcall TfrmDebtorInvoices(TComponent* Owner);
	int DebtorKey;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDebtorInvoices *frmDebtorInvoices;
//---------------------------------------------------------------------------
#endif
