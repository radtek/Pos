//---------------------------------------------------------------------------

#ifndef CreateOrdersH
#define CreateOrdersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "NumericEdit.h"
#include "VirtualTrees.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <Dialogs.hpp>
#include <memory>
#include "PurchaseOrder.h"
#include "ReceiveStockItem.h"
#include "SelectSupplierStockRequest.h"
//---------------------------------------------------------------------------


class TOrderSupplierItemNodeData
{
public:
	int		PurchaseStockKey;
	AnsiString	Code;
	WideString	Text;
	AnsiString	StockCategory;
	AnsiString	StockGroup;
	AnsiString	GLCode;
	AnsiString	Location;
	double		GSTPercent;
	AnsiString	StocktakeUnit;
	AnsiString	Supplier;
	AnsiString     SupplierWeborderTargetUri;
	int		SupplierKey;
	AnsiString	SupplierCode;
	AnsiString	SupplierUnit;
	double		SupplierUnitCost;
	double		SupplierUnitSize;						// Stocktake Units Per Supplier Unit
	double		SupplierUnitsOrdered;				// Total units initially ordered
	double		SupplierUnitsToReceive;				// Total units left to receive
	double		SupplierUnitQty;						// Units received this invoice (user enters this figure)
	double		SupplierUnitsBackOrder;				// Units left on order after finishing. Difference is written off.
	double		SupplierUnitsReceived;				// Number of units received via previous invoices.
};
//---------------------------------------------------------------------------
class TfrmCreateOrders : public TForm
{
	__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel4;
	TPanel *Panel5;
	TLabel *lbeTitle;
	TBitBtn *btnAuto;
	TVirtualStringTree *vtvStockQty;
	TNumericEdit *neStockQty;
	TBitBtn *btnCancel;
	TBitBtn *btnCommit;
	TIBTransaction *Transaction;
	TBitBtn *btnAdd;
	TBitBtn *btnRemove;
	TFindDialog *FindDialog;
	TIBQuery *qrStock;
	TIBQuery *qrGetStockInfo;
	TIBQuery *qrSupplierStock;
	TIBQuery *qrSupplier;
	TButton *Button2;
	TIBQuery *qrUpdateRequestStatus;
	TIBQuery *qrSuppliedStock;
	TIBQuery *qrSupplierStockAuto;
	TIBQuery *qrOrderedStock;
	TIBQuery *qrStockSupplier;
	TIBQuery *qrStockByLocation;
	TIBQuery *qrStockSupplierAuto;
	TIBQuery *qrStockPrefferedSupplier;
	TIBQuery *qrStockSupplierSec;
	TIBQuery *qrStockByLocationCat;
	TIBQuery *qrLocationSetting;
	TIBTransaction *TransactionMM;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
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
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall btnRemoveClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall neStockQtyKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall btnCommitClick(TObject *Sender);
	void __fastcall btnAutoClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
	const std::auto_ptr<TfrmPurchaseOrder>frmPurchaseOrder;
	const std::auto_ptr<TfrmReceiveStockItem>frmReceiveStockItem;
	void GetStockItemFromRequest();
	void AddItemToOrder(TSupplierStockRequestNodeData *stockRequest);
	void CompressStockRequest(TSupplierStockRequestNodeData *stockRequest);
	void UpdateRequestStatus(TOrderSupplierItemNodeData *NodeData);
	void UpdateRemovedRequestStatus(TOrderSupplierItemNodeData *NodeData);
	double GetOrderQuantity(double OnHandQty,double MinQtyOrdered, double MinLevel, double MaxLevel);
	double GetActualOrderQuantity(double QtyRequired,double MinQtyOrdered);
	void PopulateNodeData(	TOrderSupplierItemNodeData *NodeData);
	void PopulateStockWithCategory(AnsiString Location);
	void PopulateStockWithOutCategory(AnsiString Location);

public:		// User declarations
	__fastcall TfrmCreateOrders(TComponent* Owner);
	TList *SelectedRequestList;
	TDateTime DateRequired;
	AnsiString StockLocation;
	bool AutoEnable;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmCreateOrders *frmCreateOrders;
//---------------------------------------------------------------------------
#endif
