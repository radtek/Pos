//---------------------------------------------------------------------------
#ifndef StocktakeH
#define StocktakeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include <DB.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>
#include "NumericEdit.h"
#include "VirtualTrees.hpp"
#include <vector>        
#include <map>

#include "MM_StockTransactions.h"
//---------------------------------------------------------------------------
const WM_AFTERSHOW = WM_USER + 3;
//---------------------------------------------------------------------------
class TStocktakeGUI
{
public:
	void Execute();
private:
};
//---------------------------------------------------------------------------
class TfrmStocktake : public TForm
{
__published:	// IDE-managed Components
	TIBTransaction *Transaction;
	TIBQuery *qrCalcVariance;
	TNumericEdit *neStockQty;
	TIBSQL *qrStock;
	TFindDialog *FindDialog;
	TPanel *Panel1;
	TPanel *Panel2;
	TImage *Image2;
	TImage *Image5;
	TImage *Image1;
	TImage *Image3;
	TPanel *Panel9;
	TLabel *lbeLocation;
	TVirtualStringTree *vtvStocktake;
	TBitBtn *btnPrintStocktake;
	TBitBtn *btnInitialise;
	TStaticText *StaticText1;
	TBitBtn *btnPrintVariance;
	TBitBtn *btnCommit;
	TBitBtn *btnFind;
	TBitBtn *btnClose;
	TBitBtn *btnPrintValuation;
	TBitBtn *btnImportCount;
	TBitBtn *btnCancelStocktake;
	TIBSQL *sqlBarcodeLookup;
	TIBTransaction *MenuMateTransaction;
	TIBSQL *sqlCheckStockTrack;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnInitialiseClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btnCommitClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall btnPrintStocktakeClick(TObject *Sender);
	void __fastcall btnPrintVarianceClick(TObject *Sender);
	void __fastcall btnPrintValuationClick(TObject *Sender);
	void __fastcall vtvStocktakeColumnDblClick(TBaseVirtualTree *Sender,
          TColumnIndex Column, TShiftState Shift);
	void __fastcall vtvStocktakeCreateEditor(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink);
	void __fastcall vtvStocktakeEdited(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
	void __fastcall vtvStocktakeEditing(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, bool &Allowed);
	void __fastcall vtvStocktakeFocusChanged(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
	void __fastcall vtvStocktakeFocusChanging(TBaseVirtualTree *Sender,
          PVirtualNode OldNode, PVirtualNode NewNode,
          TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed);
	void __fastcall vtvStocktakeGetImageIndex(TBaseVirtualTree *Sender,
          PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column,
          bool &Ghosted, int &ImageIndex);
	void __fastcall vtvStocktakeGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
	void __fastcall vtvStocktakeKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall neStockQtyKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall vtvStocktakeAfterPaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
	void __fastcall vtvStocktakeBeforePaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall FindDialogFind(TObject *Sender);
	void __fastcall neStockQtyKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall neStockQtyKeyPress(TObject *Sender, char &Key);
	void __fastcall btnImportCountClick(TObject *Sender);
	void __fastcall btnCancelStocktakeClick(TObject *Sender);
protected:
	void __fastcall AfterShow(TMessage& Message);
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_AFTERSHOW, TMessage, AfterShow)
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
//	const std::auto_ptr<Stock::TStocktakeControl> StocktakeControl;
	Stock::TStocktakeControl &fStocktakeControl;
	Stock::TStocktakeDetails &fStocktake;

	void LoadTree();
	void GetCurrentNode();
	int FindStockKey(AnsiString Barcode,double &LookupQty, bool &LookSuccess);
   void ResetInitialisation(int RadioButtonValue);
	AnsiString CurrentStockCategory, CurrentStockGroup;
	int CurrentStockKey;
    int Decimalpalaces;
     
public:		// User declarations
	__fastcall TfrmStocktake(Stock::TStocktakeControl &StocktakeControl, Stock::TStocktakeDetails &Stocktake);
//	AnsiString Location;
    
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmStocktake *frmStocktake;
//---------------------------------------------------------------------------
#endif
