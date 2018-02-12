//---------------------------------------------------------------------------

#ifndef StockRequestH
#define StockRequestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "NumericEdit.h"
#include "VirtualTrees.hpp"
#include <Buttons.hpp>
#include <DB.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <DBCtrls.hpp>
#include "PurchaseOrder.h"
#include "ReceiveStockItem.h"
#include "CreateOrders.h"
#include "VirtualTrees.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <IBSQL.hpp>
#include <memory>
#include <IBSQL.hpp>

//---------------------------------------------------------------------------
class TStockRequestItemNodeData
{
public:
	int			PurchaseStockKey;
	AnsiString	Code;
	WideString	Text;
	AnsiString	StockCategory;
	AnsiString	StockGroup;
	AnsiString	GLCode;
	AnsiString	Location;
	double		GSTPercent;
	AnsiString	StocktakeUnit;
    double onhand;
	AnsiString	Supplier;
	AnsiString  SupplierWeborderTargetUri;
	int StockKey;
	int			STOCKGROUPKEY;
	int			STOCKCATEGORYKEY;
	int			SupplierKey;
	AnsiString	SupplierCode;
	AnsiString	SupplierUnit;
	double		SupplierUnitCost;
	double		SupplierUnitSize;						// Stocktake Units Per Supplier Unit

	double		SupplierUnitsOrdered;				// Total units initially ordered
	double		SupplierUnitsToReceive;				// Total units left to receive
	double		SupplierUnitQty;						// Units received this invoice (user enters this figure)
	double		SupplierUnitsBackOrder;				// Units left on order after finishing. Difference is written off.

	double		SupplierUnitsReceived;				// Number of units received via previous invoices.
	AnsiString        RequestType   ;
	int StockReqStockRequestNumber;
	int RequestNumber;
   double LatestCost;
   int SUPPLIERSTOCKKEY;

};
class TfrmStockRequest : public TForm
{
	__published:	// IDE-managed Components

	TPanel *Panel1;
	TPanel *Panel4;
	TLabel *lbeTitle;
	TVirtualStringTree *vtvStockQty;
	TNumericEdit *neStockQty;
	TBitBtn *btnCancel;
	TBitBtn *btnCommit;
	TFindDialog *FindDialog;
	TIBQuery *qrStock;
	TButton *Button2;
	TLabel *Label1;
	TEdit *edDateRequired;
	TLabel *Label4;
	TIBQuery *qrAddStockRequest;
	TIBQuery *qrStockRequest;
	TLabel *Label2;
	TComboBox *cbRequestType;
	TIBSQL *qrStockReq;
	TIBTransaction *Transaction;
	TComboBox *cmbLocation;
	TIBDataSet *dtStockReqCount;
	TIBSQL *Query;
	TIBQuery *qrLocationList;
	TIBQuery *qrAddStockRequestStock;
	TIBQuery *qrStockRequestStock;
	TIBDataSet *dtStockReqStockCount;
	TIBQuery *qrLocation;
	TButton *btnSave;
	TIBQuery *qrStockRequestSelect;
	TIBQuery *qrUpdateStockRequest;
	TIBQuery *IBQuery2;
	TIBQuery *qrUpdateStockRequestStock;
    TIBQuery *qrStockByLocation;
    TIBQuery *qrStockSupplier;
    TBitBtn *btnAdd;
    TBitBtn *BitBtn1;
    TBitBtn *btnRemove;
    TIBQuery *qrStockONhand;
    TIBQuery *qrStockReqStock;
    TIBQuery *qrStockPrefferedSupplier;
    TIBQuery *qrCheckSupplier;
    TIBQuery *qrUpdateStockReqKey;
    TIBQuery *qrLocationSetting;
    TIBTransaction *TransactionMM;
    TIBQuery *qrStockLcationWithoutCat;
    TBitBtn *btnAuto;
	void __fastcall btnRequiredDateClick(TObject *Sender);

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall vtvStockQtyAfterPaint(TBaseVirtualTree *Sender,
	TCanvas *TargetCanvas);
	void __fastcall vtvStockQtyBeforePaint(TBaseVirtualTree *Sender,
	TCanvas *TargetCanvas);
	void __fastcall vtvStockQtyCreateEditor(TBaseVirtualTree *Sender,
	PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink);
	void __fastcall vtvStockQtyEdited(TBaseVirtualTree *Sender,
	PVirtualNode Node, TColumnIndex Column);
	void __fastcall vtvStockQtyEditing(TBaseVirtualTree *Sender,
	PVirtualNode Node, TColumnIndex Column, bool &Allowed);
	void __fastcall vtvStockQtyFocusChanged(TBaseVirtualTree *Sender,
	PVirtualNode Node, TColumnIndex Column);
	void __fastcall vtvStockQtyFocusChanging(TBaseVirtualTree *Sender,
	PVirtualNode OldNode, PVirtualNode NewNode,
	TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed);
	void __fastcall vtvStockQtyGetImageIndex(TBaseVirtualTree *Sender,
	PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column,
	bool &Ghosted, int &ImageIndex);
	void __fastcall vtvStockQtyGetText(TBaseVirtualTree *Sender,
	PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
	WideString &CellText);
	void __fastcall vtvStockQtyKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall neStockQtyKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall btnCommitClick(TObject *Sender);
	void __fastcall edDateRequiredClick(TObject *Sender);
	void __fastcall cbRequestTypeClick(TObject *Sender);

	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall btnSaveClick(TObject *Sender);
	void __fastcall cmbLocationChange(TObject *Sender);
    void __fastcall cbRequestTypeChange(TObject *Sender);
    void __fastcall edDateRequiredChange(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnAddClick(TObject *Sender);
    void __fastcall btnRemoveClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall btnAutoClick(TObject *Sender);
    void __fastcall neStockQtyKeyPress(TObject *Sender, char &Key);

private:	// User declarations
	const std::auto_ptr<TfrmPurchaseOrder>frmPurchaseOrder;
	const std::auto_ptr<TfrmReceiveStockItem>frmReceiveStockItem;
	TDateTime DateRequired;
	const std::auto_ptr<TfrmAddStock>frmAddStock;
	bool VerifyQuantities();
	bool PersistStockRequest(bool isCommitted);
	bool PersistStockRequestUpdate(bool isCommitted)  ;
	void LoadStockRequest(int RequestNumber,int supplierStockkey);
    void PopulateStock();
    void RemovedStockRequestUpdate();
    void RecieveStockItem(AnsiString StockCode);
    void PopulateStockWithCategory(AnsiString Location);
    void PopulateStockWithOutCategory(AnsiString Location);
public:		// User declarations
	__fastcall TfrmStockRequest(TComponent* Owner);
	AnsiString	StockCode;
	AnsiString	Location;
	AnsiString	LocationChk;
	AnsiString RequestType;
	TDateTime Datetime;
	int StockReqStockCounter;
	int StockReqCounter;
	int   OrderKey;
	int RequestNumber;
	int StockReqStockRequestNumber  ;
	bool IsSupplierQty;
	bool IsUpdateQuery;
     int	ContactKey;
   int StockRequestNumber;

    
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmStockRequest *frmStockRequest;
//---------------------------------------------------------------------------
#endif

