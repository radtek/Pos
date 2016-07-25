//---------------------------------------------------------------------------

#ifndef SelectSupplierStockRequestH
#define SelectSupplierStockRequestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "VirtualTrees.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <Dialogs.hpp>
#include "NumericEdit.h"
#include <IBSQL.hpp>
#include <memory>
#include "StockData.h"
//---------------------------------------------------------------------------

class TSupplierStockRequestData
{
public:
        int RequestNumber;
        AnsiString DateGenerated;
        AnsiString DateRequired;
        AnsiString Location;
        bool IsSelected;
};

class TSupplierStockRequestNodeData
{
public:
        int StockRequestKey;
        int StockKey;
        int SupplierKey;
        AnsiString StockCode;
        AnsiString SupplierName;
        AnsiString DateGenerated;
        AnsiString DateRequired;
        AnsiString Location;
        AnsiString OrderUnit;
        Float Quantity;
        Float UnitCost;
        bool IsProcessed;
        bool IsRemoved;
};



class TfrmSelectSupplierStockRequest : public TForm
{
__published:	// IDE-managed Components
  	TPanel *Panel1;
	TPanel *Panel4;
	TPanel *Panel5;
	TLabel *lbeTitle;
        TVirtualStringTree *vtvStockRequest;
        TBitBtn *btnCancel;
        TBitBtn *btnNext;
        TIBTransaction *Transaction;
        TIBQuery *qrGetStockRequest;
        TIBQuery *qrGetStockRequestItems;
    TIBQuery *qrStockRequestSelect;
    TIBQuery *qrStockReqStock;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnCancelClick(TObject *Sender);
        void __fastcall btnNextClick(TObject *Sender);
        void __fastcall vtvStockRequestAfterItemPaint(
          TBaseVirtualTree *Sender, TCanvas *TargetCanvas,
          PVirtualNode Node, TRect &ItemRect);
        void __fastcall vtvStockRequestAfterPaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
        void __fastcall vtvStockRequestGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
        void __fastcall vtvStockRequestChecked(TBaseVirtualTree *Sender,
          PVirtualNode Node);
private:	// User declarations
void LoadSupplierStockRequest();
void GetStockRequestItems(TSupplierStockRequestData *stockRequest);
void GetStockRequestDate();
public:		// User declarations
        __fastcall TfrmSelectSupplierStockRequest(TComponent* Owner);
        TList *SelectedRequestList;
        TDateTime DateRequired;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmSelectSupplierStockRequest *frmSelectSupplierStockRequest;
//---------------------------------------------------------------------------
#endif
