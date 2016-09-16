//---------------------------------------------------------------------------

#ifndef UpdateSupplierCostsH
#define UpdateSupplierCostsH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <vcl.h>
#include "VirtualTrees.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include "NumericEdit.h"
#include <Dialogs.hpp>
#include "NumericEdit.h"
#include <IBSQL.hpp>
#include <memory>
#include "StockData.h"
#include "NumericEdit.h"
#include "CSV.h"

//---------------------------------------------------------------------------
#define OptionTrue "Yes"
#define OptionFalse "No"

class TSupplierItemNodeData
{
public:
   int SupplierKey;
   int StockKey;
   int SupplierStockKey;
   AnsiString   StockCode;
   AnsiString	SupplierCode;
   AnsiString	Description;
   AnsiString	StockUnit ;
   AnsiString	OrderUnit ;
   double StockQuantity;
   double SupplierUnitCost;
   bool PreferredSupplier;
   bool IsNewlyAdded;
};




class TfrmUpdateSupplierCosts : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *dlgOpen;
  	TPanel *Panel1;
	TPanel *Panel4;
	TPanel *Panel5;
	TLabel *lbeTitle;
        TVirtualStringTree *vtvStockItem;
	TNumericEdit *neStockNumericEdit;
        TEdit *neTextEdit;
        TComboBox *dbcbPreferredSupplier;
        TComboBox *dbcbUnit;
	TBitBtn *btnCancel;
	TBitBtn *btnCommit;
        TIBTransaction *Transaction;
        TIBQuery *qrGetStockUnit;
        TIBQuery *qrGetSupplierStock;
        TIBSQL *Query;
        TIBSQL *PreferredSupplierQuery;
        TIBSQL *AddSupplierStockQuery;
        TBitBtn *btnImport;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall vtvStockItemAfterPaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
	void __fastcall vtvStockItemBeforePaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
	void __fastcall vtvStockItemCreateEditor(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink);
	void __fastcall vtvStockItemEdited(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
	void __fastcall vtvStockItemEditing(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, bool &Allowed);
	void __fastcall vtvStockItemFocusChanged(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
	void __fastcall vtvStockItemFocusChanging(TBaseVirtualTree *Sender,
          PVirtualNode OldNode, PVirtualNode NewNode,
          TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed);
	void __fastcall vtvStockItemGetImageIndex(TBaseVirtualTree *Sender,
          PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column,
          bool &Ghosted, int &ImageIndex);
	void __fastcall vtvStockItemGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
	void __fastcall vtvStockItemKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
   void __fastcall KeyDownHandler(TObject *Sender, WORD &Key,
          TShiftState Shift);
   void __fastcall btnCancelClick(TObject *Sender);
   void __fastcall btnCommitClick(TObject *Sender);
        void __fastcall btnImportClick(TObject *Sender);
    void __fastcall neStockNumericEditKeyPress(TObject *Sender, char &Key);
private:	// User declarations
  void AddDataToList();
  void LoadUnits(int StockKey);
  TStringList* StockUnits;
  TList* AddedSupplierStocks;
  void UpdateStockData(TSupplierItemNodeData* StockInfo);
  void AddSupplierStock(TSupplierItemNodeData* NodeData);
  bool GetStockDetail(TSupplierItemNodeData* NodeData);
  bool CheckNegativeCostOrQty();  
public:		// User declarations
   __fastcall TfrmUpdateSupplierCosts(TComponent* Owner);
   	int ContactKey;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmUpdateSupplierCosts *frmUpdateSupplierCosts;
//---------------------------------------------------------------------------
#endif
