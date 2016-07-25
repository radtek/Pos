//---------------------------------------------------------------------------
#ifndef AddStockH
#define AddStockH

#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBSQL.hpp>
#include <Mask.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include "NumericEdit.h"
#include <memory>
#include "NumericEdit.h"
#include "AddSupplierUnit.h"
#include "AddSupplierBarcode.h"

//---------------------------------------------------------------------------
enum TAdjustStockMode { amAddStock, amEditStock, amAddSupplierUnit, amAddLocation };
//---------------------------------------------------------------------------
class TfrmAddStock : public TForm
{
__published:	// IDE-managed Components
	TIBTransaction *Transaction;

	TDataSource *dsStockEdit;
	TIBDataSet *dtStockEdit;
	TIBQuery *qrAddStockGroup;
	TIBSQL *sqlAddLK;
	TIBQuery *IBQuery;
	TIBQuery *qrAddSupplier;
	TIBQuery *qrSuppliers;
	TDataSource *dsSuppliers;
	TIBSQL *sqlFindSupplier;
	TIBDataSet *dtStockLocationEdit;
	TDataSource *dsStockLocationEdit;
	TIBQuery *qrNewItem;
	TTabSheet *tsItemDetails;
	TTabSheet *tsSuppliers;
	TTabSheet *tsLocations;
	TPageControl *PageControl1;
	TIBQuery *qrRemoveSupplier;
	TIBDataSet *dtSuppliersStock;
	TDataSource *dsSuppliersStock;
	TGroupBox *gbLocations;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TDBText *dbtOnOrder;
	TDBText *DBText1;
	TDBText *DBText5;
	TDBText *dbtOnHand;
	TLabel *lbeLatestCost;
	TDBText *DBText6;
	TLabel *Label30;
	TLabel *lbeAveCost;
	TDBText *DBText7;
	TLabel *Label24;
	TDBEdit *dbeMinLevel;
	TDBEdit *dbeMaxLevel;
	TButton *btnAddLocation;
	TDBGrid *dbgLocations;
	TButton *btnDeleteLocation;
	TDBEdit *dbeLatestCost;
	TDBEdit *dbeAveCost;
	TGroupBox *gbSuppliers;
	TLabel *Label7;
	TLabel *Label12;
	TLabel *Label23;
	TDBText *dbtStocktakeUnit;
	TLabel *Label32;
	TLabel *Label21;
	TLabel *Label19;
	TButton *btnRemoveSupplier;
	TButton *btnAddSupplier;
	TDBGrid *dbgSuppliers;
	TDBEdit *dbcOrderQty;
	TButton *btnAddOrderUnit;
	TDBEdit *dbeCost;
	TButton *btnDeleteOrderUnit;
	TDBGrid *dbgOrderUnit;
	TDBEdit *dbeSupplierCode;
	TCheckBox *chbDefaultOrderUnit;
	TGroupBox *gbItemClass;
	TLabel *Label29;
	TRadioButton *rbUnitClass;
	TRadioButton *rbBottleClass;
	TRadioButton *rbBulkClass;
	TComboBox *cbBottleSize;
	TGroupBox *gbSalesUnit;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label17;
	TLabel *Label26;
	TLabel *Label27;
	TLabel *Label28;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TIBSQL *qrClearPreferredSupplier;
	TCheckBox *chbPreferredSupplier;
	TBitBtn *btnNext2;
	TGroupBox *gbItemDetail;
	TLabel *Label1;
	TLabel *Label3;
	TLabel *Label2;
	TDBText *dbtStockCode;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TDBEdit *dbeStockCode;
	TButton *btnAddStockGroup;
	TDBCheckBox *dbcEnabled;
	TDBEdit *dbeDescription;
	TButton *btnNextCode;
	TComboBox *cbStockGroup;
	TDBComboBox *dbcbPurchaseUnit;
	TButton *btnAddPurchaseUnit;
	TDBComboBox *dbcSaleUnit;
	TButton *btnAddSalesUnit;
	TDBEdit *dbeItemSize;
	TBitBtn *btnNext1;
	TBitBtn *btnPrev1;
	TLabel *Label16;
	TDBEdit *dbeGST;
	TLabel *Label18;
	TLabel *Label25;
	TLabel *Label33;
	TDBEdit *dbeGLCode;
	TCheckBox *chbDefaultLocation;
	TLabel *Label35;
	TLabel *Label22;
	TLabel *Label34;
	TLabel *Label36;
	TLabel *Label37;
	TLabel *Label39;
	TLabel *Label38;
	TIBSQL *qrSupplierName;
	TIBDataSet *IBDataSet1;
	TLabel *Label40;
	TDBEdit *dbeMinOrderQty;
	TLabel *Label41;
	TDBEdit *dbeAssessedValue;
	TIBQuery *qrLocations;
	TIBTransaction *trMenuMate;
	TTabSheet *tsBarcodes;
	TGroupBox *gbDetailsBarcodes;
	TGroupBox *gbSuppliersBarcodes;
	TButton *btnAddItemBarcode;
	TButton *btnDeleteItemBarcode;
	TButton *btnAddSupplierBarcode;
	TButton *btnDeleteSupplierBarcode;
	TIBDataSet *dtItemBarcode;
	TIBSQL *IBSQL1;
	TDataSource *dsItemBarcode;
	TBitBtn *btnScanItemBarcode;
	TBitBtn *btnScanSupplierBarcode;
	TDBGrid *dbgItemBarcode;
	TDataSource *dsSupplierBarcode;
	TIBDataSet *dtSupplierBarcode;
	TDBGrid *dbgSupplierBarcode;
	TIBQuery *qrSupplierUnits;
	TIBSQL *sqlBarcode;
    TLabel *Label20;
    TDBEdit *dbeSupplierCost;
   TLabel *lblReductionUnit;
   TLabel *lblConversion;
   TDBComboBox *dbcbReductionUnit;
   TDBEdit *dbeConversionFactor;
   TButton *btnAddReductionUnit;
  TGroupBox *gbDetailsSubstitutes1;
     TGroupBox *gbDetailsSubstitutes2;
       TGroupBox *gbDetailsSubstitutes3;
  //  TDBEdit *dbeSupplierCost;
    TLabel *Label31;
    TButton *btnAddStockSubGroup;
    TDBComboBox *cbStockSubGroup;
    TCheckBox *chbminLevel;
    TBitBtn *btnGo;
    TEdit *edSearch;
    TLabel *Label42;
    TLabel *Label43;
    TLabel *Label46;
    TIBQuery *qrStock;
    TEdit *edSearch1;
    TEdit *edSearch2;
    TComboBox *DBCmbsubsttem1;
    TComboBox *DBCmbsubststockitem1;
     TComboBox *DBCmbsubsttem2;
    TComboBox *DBCmbsubststockitem2;
     TComboBox *DBCmbsubsttem3;
    TComboBox *DBCmbsubststockitem3;
    TLabel *Label47;
    TNumericEdit *txtvariance1;
    TNumericEdit *txtvariance2;
    TNumericEdit *txtvariance3;
    TIBQuery *qrSubstitute;
   TIBDataSet *dtStockSubstituteCount;
   TIBQuery *qrSelectSubstitute;
    TIBQuery *STOCK_SUBSTITUTE_PERMISSIONS;
    TIBQuery *qrSubstituteId;
    TIBQuery *qrChkStockIdExist;
    TIBQuery *qrInsertSTOCK_SUBSTITUTES;
    TIBQuery *qrUpdateSTOCK_SUBSTITUTES;
   TIBQuery *qrpopulateSubstitute;
    TIBQuery *qrSetMinLevel;
    TIBQuery *qrStockSubstitute;
    TIBQuery *qrUpdateStockPermission;
    TCheckBox *cbReadyForSubstitute1;
    TCheckBox *cbReadyForSubstitute2;
    TCheckBox *cbReadyForSubstitute3;
    TIBQuery *qrSubstitutePersmissionID;
    TIBQuery *qrupdateSubstituteSetting;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall dbeKeyPress(TObject *Sender, char &Key);
	void __fastcall btnAddStockGroupClick(TObject *Sender);
	void __fastcall dbeMaxLevelKeyPress(TObject *Sender, char &Key);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btnAddPurchaseUnitClick(TObject *Sender);
//	void __fastcall btnAddLocationClick(TObject *Sender);
//	void __fastcall dbcbLocationKeyDown(TObject *Sender, WORD &Key,
//          TShiftState Shift);
//	void __fastcall dbcbLocationKeyPress(TObject *Sender, char &Key);
	void __fastcall dbcbPurchaseUnitKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall dbcbPurchaseUnitKeyPress(TObject *Sender, char &Key);
	void __fastcall dbeKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall cbStockGroupChange(TObject *Sender);
	void __fastcall cbStockGroupKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall cbStockGroupKeyPress(TObject *Sender, char &Key);
	void __fastcall btnAddSupplierClick(TObject *Sender);
	void __fastcall btnAddLocationClick(TObject *Sender);
	void __fastcall btnNextCodeClick(TObject *Sender);
	void __fastcall PageControl1Changing(TObject *Sender, bool &AllowChange);
	void __fastcall btnAddSalesUnitClick(TObject *Sender);
	void __fastcall btnRemoveSupplierClick(TObject *Sender);
	void __fastcall btnDeleteLocationClick(TObject *Sender);
	void __fastcall dtStockLocationEditAfterOpen(TDataSet *DataSet);
	void __fastcall btnAddOrderUnitClick(TObject *Sender);
	void __fastcall qrSuppliersAfterScroll(TDataSet *DataSet);
	void __fastcall btnDeleteOrderUnitClick(TObject *Sender);
	void __fastcall PageControl1Change(TObject *Sender);
	void __fastcall dtSuppliersStockAfterOpen(TDataSet *DataSet);
	void __fastcall dtStockLocationEditBeforeInsert(TDataSet *DataSet);
	void __fastcall chbDefaultOrderUnitClick(TObject *Sender);
	void __fastcall dtSuppliersStockAfterScroll(TDataSet *DataSet);
	void __fastcall chbPreferredSupplierClick(TObject *Sender);
	void __fastcall btnNext1Click(TObject *Sender);
	void __fastcall btnPrev1Click(TObject *Sender);
	//void __fastcall btnScanItemClick(TObject *Sender);
	void __fastcall chbDefaultLocationClick(TObject *Sender);
	void __fastcall dtStockLocationEditAfterScroll(TDataSet *DataSet);
	void __fastcall dtSuppliersStockBeforePost(TDataSet *DataSet);
	void __fastcall dbeStockCodeKeyPress(TObject *Sender, char &Key);
	//void __fastcall dbeBarcodeKeyPress(TObject *Sender, char &Key);
	void __fastcall chbDefaultOrderUnitKeyPress(TObject *Sender, char &Key);
	void __fastcall btnAddItemBarcodeClick(TObject *Sender);
	void __fastcall btnScanItemBarcodeClick(TObject *Sender);
	void __fastcall btnDeleteItemBarcodeClick(TObject *Sender);
	void __fastcall btnDeleteSupplierBarcodeClick(TObject *Sender);
    void __fastcall dbeGSTExit(TObject *Sender);
    void __fastcall dsSuppliersStockDataChange(TObject *Sender, TField *Field);
   void __fastcall btnAddReductionUnitClick(TObject *Sender);
    void __fastcall cbStockSubGroupKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall cbStockSubGroupKeyPress(TObject *Sender, char &Key);
    void __fastcall btnAddStockSubGroupClick(TObject *Sender);
    void __fastcall btnGoClick(TObject *Sender);
    void __fastcall btnGo1Click(TObject *Sender);
    void __fastcall btnGo3Click(TObject *Sender);
    void __fastcall chbminLevelClick(TObject *Sender);
 //  void __fastcall dbeLatestCostChange(TObject *Sender);

	//void __fastcall btnScanSupplierBarcodeClick(TObject *Sender);
   
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)

private:	// User declarations
	bool PostData();
   void ResizeGrids();
	void ResizeBarcodeGrids();

	bool CheckDetails();
   bool CheckPurchaseUnit();
	bool CheckOrderUnits();
	bool CheckSalesUnit();
	bool CheckLocations();
   void LoadUnits();
   bool CheckReductionUnit();
	const std::auto_ptr<TfrmAddSupplierUnit>frmAddSupplierUnit;
    void ResetSubstitueControl();
public:		// User declarations
	__fastcall TfrmAddStock(TComponent* Owner);
	TAdjustStockMode Mode;
	AnsiString DefaultUnit;
//
    	AnsiString DefaultSubGroup;

	int StockKey;
	int StockGroupKey;
	AnsiString NewItemName;

	int NewSupplierKey;
	AnsiString SupplierUnit;
	AnsiString Location;
    //
	AnsiString	StockCode;
	AnsiString	Description;
	AnsiString	Unit;
int SubsituteStockKey;
AnsiString SubsituteDiscription; // end
	bool AutoStockCode;
   AnsiString DefaultLocation;
	void AddItemBarcode( AnsiString Barcode );
	void AddSupplierBarcode( AnsiString Barcode );
	bool CheckBarcodeBeforeInsert( AnsiString Barcode );
//
 void SubsituteData()  ;

int InsertSubstituteField(int StockKey, int SubstituteStockkey, int SubstituteUniqueId)  ;
void InsertSubstituteSettings(int substituteId, AnsiString SUBSTITUTE_COND1, AnsiString SUBSTITUTE_COND2, AnsiString SUBSTITUTE_COND3,bool isSelectable );
bool SubsitutePopulateData();
void ResetSubstiteControl();
void PopulateSubstituteStockControl()  ;
void PopulateSubstituteField();
AnsiString PopulateSubstituteDescription(int StockKey)  ;
   int SubstituteStockkey1;
   int SubstituteStockkey2;
   int SubstituteStockkey3;
    int SubstituteUniqueId1;
   int SubstituteUniqueId2;
   int SubstituteUniqueId3;


// end
//   int CopyStockItem(AnsiString CodeToCopy);
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmAddStock *frmAddStock;
//---------------------------------------------------------------------------
#endif
