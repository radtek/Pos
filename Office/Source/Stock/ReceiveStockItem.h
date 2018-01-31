//---------------------------------------------------------------------------

#ifndef ReceiveStockItemH
#define ReceiveStockItemH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <Mask.hpp>
#include <IBDatabase.hpp>
#include <IBSQLMonitor.hpp>
#include <IBUpdateSQL.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include "NumericEdit.h"
#include <memory>
#include "NumericEdit.h"
#include "AddStock.h"
//---------------------------------------------------------------------------
class TfrmReceiveStockItem : public TForm
{
__published:	// IDE-managed Components
	TIBQuery *qrStock;
	TDataSource *dsStock;
	TIBTransaction *Transaction;
	TIBQuery *qrSupplierStock;
	TDataSource *dsSupplierStock;
	TPanel *Panel1;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TLabel *Label17;
	TLabel *lbeStockCode;
	TStringGrid *sgLocations;
	TGroupBox *GroupBox4;
	TLabel *lbeCost1;
	TLabel *Label7;
	TDBText *dbtQtyOrderUnit;
	TLabel *Label3;
	TLabel *Label15;
	TDBText *dbtCostOrderUnit;
	TLabel *lbeCost2;
	TLabel *Label16;
	TLabel *Label2;
	TBitBtn *btnAddSupplierUnit;
	TDBLookupComboBox *dbluSupplierUnit;
	TNumericEdit *neCost1;
	TNumericEdit *neCost2;
	TDBLookupComboBox *dbluLocation;
	TBitBtn *btnAddLocation;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TNumericEdit *neCost3;
	TNumericEdit *neCost4;
	TNumericEdit *neQty;
	TLabel *Label1;
	TDBLookupComboBox *dbluSupplier;
	TBitBtn *btnAddSupplier;
	TIBQuery *qrSupplier;
	TDataSource *dsSupplier;
	TLabel *Label4;
	TLabel *lbeStocktakeUnit;
    TIBQuery *qrCheapestSupplier;
    TIBQuery *qrSupplierSelection;
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Panel1Enter(TObject *Sender);
	void __fastcall qrStockAfterOpen(TDataSet *DataSet);
	void __fastcall dtStockTransAfterOpen(TDataSet *DataSet);
	void __fastcall cbSupplierUnitChange(TObject *Sender);
	void __fastcall qrSupplierStockAfterScroll(TDataSet *DataSet);
	void __fastcall dbluSupplierUnitCloseUp(TObject *Sender);
	void __fastcall dbluLocationCloseUp(TObject *Sender);
	void __fastcall btnAddSupplierUnitClick(TObject *Sender);
	void __fastcall btnAddLocationClick(TObject *Sender);
	void __fastcall sgLocationsDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall neCostChange(TObject *Sender);
	void __fastcall neQtyChange(TObject *Sender);
	void __fastcall sgLocationsClick(TObject *Sender);
	void __fastcall sgLocationsKeyPress(TObject *Sender, char &Key);
	void __fastcall qrStockAfterScroll(TDataSet *DataSet);
	void __fastcall dbluSupplierCloseUp(TObject *Sender);
	void __fastcall qrSupplierAfterScroll(TDataSet *DataSet);
    void __fastcall neCost1KeyPress(TObject *Sender, char &Key);
    void __fastcall neCost2KeyPress(TObject *Sender, char &Key);
    void __fastcall neCost3KeyPress(TObject *Sender, char &Key);
    void __fastcall neCost4KeyPress(TObject *Sender, char &Key);
    void __fastcall neQtyKeyPress(TObject *Sender, char &Key);

protected:
private:	// User declarations
	void ResizeGrids();
	virtual int __fastcall ShowModal();
        bool isPrefferedSupplierSelected;
//	bool InvoiceContainsItem(AnsiString StockCode, AnsiString SupplierUnit, AnsiString InvoiceRef);
	const std::auto_ptr<TfrmAddStock>frmAddStock;
      int Decimalpalaces ;
public:		// User declarations
	__fastcall TfrmReceiveStockItem(TComponent* Owner);
	AnsiString	StockCode;
//	AnsiString	OrderNumber;
//	AnsiString	Reference;
	int		StockKey;
	AnsiString	Description;
	AnsiString	StockCategory;
	AnsiString	StockGroup;
	AnsiString	GLCode;
	AnsiString	Location;
	double		GSTPercent;
	AnsiString	StocktakeUnit;
	bool		   Initialised;
	TDateTime	InitialisedTime;
   AnsiString  SupplierWeborderTargetUri;
	int		   SupplierKey;
	AnsiString	Supplier;
	AnsiString	SupplierUnit;
	AnsiString	SupplierCode;
	double		SupplierUnitSize;
	double		SupplierUnitQty;
	double		StocktakeUnitQty;
	Currency	   SupplierUnitCost;
    double LatestCost;
   double OnHandQty;
   	int	StockRequestSupplierKey;
   	AnsiString	CheckLocation;
	TModalResult Execute();	// Call this - not Show()
    bool AllowNegativeQuantity;

};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmReceiveStockItem *frmReceiveStockItem;
//---------------------------------------------------------------------------
#endif
