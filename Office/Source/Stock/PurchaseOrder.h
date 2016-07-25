//---------------------------------------------------------------------------
#ifndef PurchaseOrderH
#define PurchaseOrderH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
#include <Buttons.hpp>
#include <Db.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <DB.hpp>
#include "VirtualTrees.hpp"
#include "NumericEdit.h"
#include <IBSQL.hpp>
#include <memory>
#include "ReceiveStockItem.h"
#include <vector>
//---------------------------------------------------------------------------
class TOrderItemNodeData
{
public:
	//	int			OrderItemKey;
	AnsiString	Code;
	WideString	Text;
	AnsiString	GLCode;
	AnsiString	Location;
	double		GSTPercent;
	AnsiString	StocktakeUnit;
	int SupplierKey;
	AnsiString	SupplierCode;
	AnsiString	SupplierUnit;
	double		SupplierUnitCost;
	double		SupplierUnitSize;						// Stocktake Units Per Supplier Unit
	double		SupplierUnitsOrdered;				// Total units initially ordered
	double		SupplierUnitQty;						// Units ordered (user enters this figure)
};
//---------------------------------------------------------------------------
//enum TPurchaseMode { pmAdd, pmEdit };
//---------------------------------------------------------------------------
class TfrmPurchaseOrder : public TForm
{
	__published:	// IDE-managed Components
	TIBTransaction *Transaction;
	TIBQuery *qrSupplier;
	TIBDataSet *dtUser;
	TIBQuery *qrDeliveryAddress;
	TIBQuery *qrSupplierStock;
	TIBQuery *qrOrderedStock;
	TIBQuery *qrUpdateStock;
	TIBQuery *qrStock;
	TIBQuery *qrSuppliedStock;
	TPanel *Panel1;
	TPanel *Panel4;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *lbeTotalExc;
	TLabel *lbeGST;
	TLabel *lbeTotalInc;
	TPanel *Panel5;
	TLabel *lbeTitle;
	TBitBtn *btnAdd;
	TBitBtn *btnRemove;
	TBitBtn *btnSave;
	TBitBtn *btnCancel;
	TLabel *Label1;
	TLabel *Label14;
	TBitBtn *btnSelectDeliveryAdd;
	TLabel *Label9;
	TBitBtn *btnNextOrderNumber;
	TBitBtn *btnRequiredDate;
	TLabel *Label11;
	TMemo *memInstructions;
	TLabel *Label16;
	TBitBtn *btnAuto;
	TBitBtn *btnCommit;
	TVirtualStringTree *vtvStockQty;
	TNumericEdit *neStockQty;
	TNumericEdit *neCost;
	TNumericEdit *neTotalCost;
	TMemo *memSupplierAddress;
	TMemo *memDeliveryAddress;
	TEdit *edOrderNumber;
	TEdit *edDateRequired;
	TBitBtn *btnCommitOld;
	TBitBtn *btnOk;
	TIBQuery *qrPurchaseOrder;
	TIBQuery *qrPurchaseStock;
	TIBQuery *qrClearOrderStock;
	TIBQuery *qrDeleteOrder;
	TIBQuery *qrAddPurchaseOrder;
	TIBQuery *qrAddOrderStock;
	TIBSQL *qrOrderKey;
	TIBQuery *qrUpdateOrder;
	TIBSQL *qrOrderStockKey;
	TIBQuery *qrPostOrder;
	TLabel *Label2;
	TEdit *edFAX;
	TIBQuery *qrPostOrderItems;
	TBitBtn *BitBtn1;
	TIBSQL *sqlOrderNumber;
	TBitBtn *btnCommitEmail;
	TBitBtn *btnCommitAndSubmitWeborder;
	TIBQuery *qrSupplierStockAuto;
	TIBQuery *qrStockLocationchk;
	TIBQuery *qrStockchk;
	TIBQuery *qrStockCat;
    TIBQuery *qrStockAutoPopulate;
  
	TIBTransaction *TransactionMM;
	TIBQuery *qrLocationSetting;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnRequiredDateClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btnAutoClick(TObject *Sender);
	void __fastcall btnSelectDeliveryAddClick(TObject *Sender);
	void __fastcall btnNextOrderNumberClick(TObject *Sender);
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall vtvStockQtyGetText(TBaseVirtualTree *Sender,
	PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
	WideString &CellText);
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
	void __fastcall vtvStockQtyKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall neStockQtyKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall btnRemoveClick(TObject *Sender);
	void __fastcall edDateRequiredKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall btnCommitClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall neStockQtyKeyPress(TObject *Sender, char &Key);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall btnCommitAndSubmitWeborderClick(TObject *Sender);
private:	// User declarations
	void Initialise();
	void LoadOrder();
	void RecieveStockItem(AnsiString StockCode);
	double GetOrderQuantity(double OnHandQty,double MinQtyOrdered, double MinLevel, double MaxLevel);
	double GetQuantity(double OnHandQty,double OnOrdered, double MinLevel, double MaxLevel,double QtyUnit);
	void PopulateStockWithCategory(AnsiString Location);
	void PopulateStockWithOutCategory(AnsiString Location);
	
	double GetActualOrderQuantity(double QtyRequired,double MinQtyOrdered);
	void ShowTotals();
	void DoCommit(bool emailp);
	bool SaveOrder();
	bool CommitOrder();
	bool VerifyQuantities();
	AnsiString EmailAddress;
	virtual int __fastcall ShowModal();
	bool validate_order();
	bool is_okay_to_commit();
	std::vector<std::string> split_crlf_delimited_string(
	const AnsiString &source);
	AnsiString weborder_uri;
	const std::auto_ptr<TfrmReceiveStockItem>frmReceiveStockItem;
	bool OrderNumberExists(AnsiString OrderNumber);
	//	bool GetMaxOrderNumber(AnsiString Prefix,AnsiString Suffix, AnsiString &MaxOrderNumber,int &NextOrderNumber);
    int CheckImage();
    void CallPurchaseOrderRerport(AnsiString ReportName, bool emailp, AnsiString efname);
public:
	TDateTime DateRequired;
	// User declarations
	__fastcall TfrmPurchaseOrder(TComponent* Owner);
	//	TPurchaseMode PurchaseMode;
	int ContactKey, OrderKey;
	AnsiString ContactName;
	void InitialisePreDef(int SupplierKey, AnsiString);	// Use only for pre-defining the order.
	TModalResult ExecutePreDef();	// Use only for pre-defining the order.
	TModalResult Execute();			// Call this - not Show()
	bool IsStockRequestMode;
	int MinSupplierkey;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmPurchaseOrder *frmPurchaseOrder;
//---------------------------------------------------------------------------
#endif
