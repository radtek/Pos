//---------------------------------------------------------------------------

#ifndef ReceiveInvoiceH
#define ReceiveInvoiceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include "NumericEdit.h"
#include "VirtualTrees.hpp"
#include <Dialogs.hpp>
#include <Graphics.hpp>
#include <IBSQL.hpp>
#include <ComCtrls.hpp>
#include <memory>
#include "ReceiveStockItem.h"

#include "XeroIntegration.h"

//---------------------------------------------------------------------------
class TInvoiceItemNodeData
{
public:
	int			PurchaseStockKey;
	AnsiString	Code;
	int			StockKey;
	WideString	Text;
	AnsiString	StockCategory;
	AnsiString	StockGroup;
	AnsiString	GLCode;
	AnsiString	Location;
	double		GSTPercent;
	AnsiString	SupplierCode;
	AnsiString  SupplierUnit;
   AnsiString  StocktakeUnit;
   AnsiString  DisplayedUnit;
	double		SupplierUnitCost;
	double		SupplierTotalCost;
	double		SupplierUnitSize;
	double		SupplierUnitsToReceive;				// Total units left to receive
	double		OrderQty;//SupplierUnitQty;		// Supplier Units received this invoice (user enters this figure)
	double		BackOrder;//SupplierUnitsBackOrder;	// Units left on order after finishing. Difference is written off
	bool        Initialised;
	TDateTime	InitialisedTime;
   int TransactionNumber;
   bool IsUnitChanged;
   bool IsUnitEditable;
   double LatestCost;
   double OnHandQty;
   int SupplierKey;
   int  StockRequestStockKey;
   TDateTime Createdtime;


};
//---------------------------------------------------------------------------
struct TInvoiceItemSummary
{
	TInvoiceItemSummary() : BackOrderWarning(false), CostExcl(0), CostInc(0), ItemGST(0) {}
	bool			BackOrderWarning;
	Currency		CostExcl;
	Currency		CostInc;
	Currency		ItemGST;
};
//---------------------------------------------------------------------------
struct TInvoiceSummary
{
	TInvoiceSummary() : BackOrderWarning(false), TotalExcl(0), TotalInc(0), TotalGST(0) {}
	bool			BackOrderWarning;
	Currency		TotalExcl;
	Currency		TotalInc;
	Currency		TotalGST;
};
//---------------------------------------------------------------------------
class TfrmReceiveInvoice : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel4;
    TPanel *Panel5;
    TLabel *lbeTitle;
    TBitBtn *btnFind;
    TIBTransaction *Transaction;
    TIBQuery *qrPurchaseStock;
    TVirtualStringTree *vtvStockQty;
    TNumericEdit *neStockQty;
    TGroupBox *GroupBox1;
    TRadioButton *rbExcludingGST;
    TRadioButton *rbIncludingGST;
    TNumericEdit *neCost;
    TNumericEdit *neTotalCost;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *lbeTotalExc;
    TLabel *lbeTotalInc;
    TBitBtn *btnOk;
    TBitBtn *btnCancel;
    TBitBtn *btnAdd;
    TFindDialog *FindDialog;
    TBitBtn *btnRemove;
    TImage *imgWarning;
    TLabel *lbeWarning;
    TIBQuery *qrPurchaseOrder;
    TIBQuery *qrSupplier;
    TDateTimePicker *dtpInvoiceDate;
    TLabel *Label4;
    TNumericEdit *neGST;
    TBitBtn *BitBtn1;
    TCheckBox *chbGSTOverride;
    TComboBox *dbcbUnit;
    TComboBox *dbcbLocation;
    TBitBtn *btnCommitPackingSlip;
    TIBQuery *qrStockTransaction;
    TButton *btnPrintInvoice;
    TIBSQL *qrUpdateOrder;
    TIBQuery *qrSelectLocationUpdate;
    TIBQuery *qrSupplierOrderQty;
        TNumericEdit *neBackOrder;
    TIBQuery *qrLocationUpdate;
    TIBQuery *qrPurchaseStockAuto;
    TIBQuery *qrUnitChanged;
    TIBQuery *qrUnitqty;
    TLabel *lblStockReceiptNumber;
    TLabel *lblReceiptnumber;
    TIBQuery *qrBatchKey;
    TBitBtn *btnPrintCommitInvoice;
    TIBSQL *qrDeleteStock;
    TBitBtn *btnSave;
    TIBQuery *qrCheckCommitted;
    TIBQuery *qrSupplierKey;
    TIBQuery *qrCheckSupplierUnit;
    TIBQuery *qrPurchaseStockTakeAuto;
    TIBQuery *qrGetStockUnitSize;
    TIBQuery *qrUpdateDateInStockTrans;
    TIBQuery *qrUpdateDateInTransactionBatch;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall vtvStockQtyAfterPaint(TBaseVirtualTree *Sender, TCanvas *TargetCanvas);
    void __fastcall vtvStockQtyBeforePaint(TBaseVirtualTree *Sender, TCanvas *TargetCanvas);
    void __fastcall vtvStockQtyCreateEditor(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink);
    void __fastcall vtvStockQtyEdited(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column);
    void __fastcall vtvStockQtyEditing(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column, bool &Allowed);
    void __fastcall vtvStockQtyFocusChanged(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column);
    void __fastcall vtvStockQtyFocusChanging(TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode, TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed);
    void __fastcall vtvStockQtyGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType, WideString &CellText);
    void __fastcall vtvStockQtyKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall rbGSTClick(TObject *Sender);
    void __fastcall neStockQtyKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall FindDialogFind(TObject *Sender);
    void __fastcall vtvStockQtyGetImageIndex(TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column, bool &Ghosted, int &ImageIndex);
    void __fastcall btnAddClick(TObject *Sender);
    void __fastcall btnRemoveClick(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall btnFindClick(TObject *Sender);
    void __fastcall neStockQtyKeyPress(TObject *Sender, char &Key);
    void __fastcall neCostExit(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall neGSTKeyPress(TObject *Sender, char &Key);
    void __fastcall neGSTKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall neGSTExit(TObject *Sender);
    void __fastcall chbGSTOverrideClick(TObject *Sender);
    void __fastcall neStockQtyExit(TObject *Sender);
    void __fastcall neTotalCostExit(TObject *Sender);
    void __fastcall btnPrintInvoiceClick(TObject *Sender);
        void __fastcall dbcbUnitChange(TObject *Sender);
    void __fastcall dbcbLocationChange(TObject *Sender);
    void __fastcall btnPrintCommitInvoiceClick(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall btnCommitPackingSlipClick(TObject *Sender);
    //void __fastcall neCostKeyPress(TObject *Sender, char &Key);

private:	// User declarations

    void ShowTotals();
    bool CheckInvoice();
    bool CheckPackingSlip();
    void AddStockItem(AnsiString StockCode);
    const std::auto_ptr<TfrmReceiveStockItem>frmReceiveStockItem;
    void CalculateInvoiceTotals(TInvoiceSummary &InvoiceSummary);

    void       InitXeroIntegration();
    AnsiString LocalHost();
    bool       XeroIntegrationIsInstalled();
    bool       SendInvoiceToXero( TXeroInvoice *inXeroInvoice );
    AnsiString ReadXeroFolderPath();
    AnsiString ReadCompanyName();
    void LoadForm();
    void UpdatePackingSlip();
    void ProcessPackingSlip();
    void LoadUnits(int stockKey);
    void LoadLocations(int stockKey);
    void UpdatePurchaseOrder();
    void CommitInvoice();
    TStringList* Locations;
    TStringList* StockUnits;
    void CommitPackingSlip(bool isCommitted);
    float GetItemQty(int supplier_key, int stock_key);
    double GetSupplierUnitCost(AnsiString supplier_unit, int stock_key, int batch_key, TInvoiceItemNodeData *NodeData);
    bool CheckSupplierUnit(int stock_key, int supplier_key, AnsiString _supplierunit);
    double GetSupplierUnitSize(int stock_key, int supplier_key, AnsiString _supplierunit);
    void UpdateSupplierUnit(TInvoiceItemNodeData *NodeData);
    void UpdateNodeQty(TInvoiceItemNodeData *NodeData, double QtyToReceive, double newSupplierUnitSize);
    void UpdateSupplierUnitCost(int order_key, TInvoiceItemNodeData *NodeData);
    void UpdateUnitqty(TInvoiceItemNodeData *NodeData);
    double GetStockTakeUnitSize(int stock_key, int supplier_key);
    bool CheckInvoiceQtyAndPrice();
    void PrintReceiveInvoice();    

public:		// User declarations
	__fastcall TfrmReceiveInvoice(TComponent* Owner);

    int OrderKey;
    int BatchKey;
    int SupplierKey;
    int ReceiptNumber;
    bool IsPackingSlipUpdateMode;
    bool IsPurchaseOrderMode;
    AnsiString InvoiceReference;
    AnsiString SupplierName;
    AnsiString BaseUnit;
    int Decimalpalaces ;
    bool IsPrintReport;
    bool IsSavedPackingSlip;

};
//---------------------------------------------------------------------------

#endif
