//---------------------------------------------------------------------------

#ifndef StockSubstituteH
#define StockSubstituteH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "VirtualTrees.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBSQL.hpp>
#include <IBCustomDataSet.hpp>
#include <memory>
#include "MM_DBCore.h"
#include "MMData.h"
//---------------------------------------------------------------------------

class TStockSubstituteData
{
public:
        int SubstituteKey;
        int StockKey;
        int SubstituteStockKey;
        AnsiString StockName;
        AnsiString SubstituteStockName;
        AnsiString StockCode;
        AnsiString SubstituteStockCode;
        AnsiString StockLocation;
        AnsiString SubstituteStockLocation;
        Currency StockLatestCost;
        Currency SubstituteLatestCost;
        Currency StockOnHandQty;
        Currency SubstituteOnHandQty;
        Currency StockMinLevel;
        Currency SubstituteMinLevel;
        Currency Variance;
        int Condition1;
        int Condition2;
        bool RevertToOriginal;
        bool IsSelected;
        bool IsDisplay;
        int SubstituteNumber;
};



class TfrmStockSubstitute : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel4;
	TPanel *Panel5;
	TLabel *lbeTitle;
   TVirtualStringTree *vtvStockSubstitute;
   TBitBtn *btnCancel;
   TBitBtn *btnNext;
   TIBTransaction *Transaction;
   TIBQuery *qrGetSubstituteStock;
   TLabel *Label3;
   TIBSQL *Query;
   TComboBox *cbStockLocation;
   TComboBox *cbSubstituteNumber;
   TCheckBox *cbRevertToOriginal;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall vtvStockSubstituteAfterItemPaint(
          TBaseVirtualTree *Sender, TCanvas *TargetCanvas,
          PVirtualNode Node, TRect &ItemRect);
   void __fastcall vtvStockSubstituteAfterPaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
   void __fastcall vtvStockSubstituteGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
   void __fastcall vtvStockSubstituteChecked(TBaseVirtualTree *Sender,
          PVirtualNode Node);
   void __fastcall btnCancelClick(TObject *Sender);
   void __fastcall btnNextClick(TObject *Sender);
   void __fastcall cbStockLocationChange(TObject *Sender);
   void __fastcall vtvStockSubstituteCreateEditor(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink);
   void __fastcall vtvStockSubstituteEdited(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
   void __fastcall vtvStockSubstituteEditing(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, bool &Allowed);
   void __fastcall vtvStockSubstituteFocusChanged(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
   void __fastcall vtvStockSubstituteFocusChanging(
          TBaseVirtualTree *Sender, PVirtualNode OldNode,
          PVirtualNode NewNode, TColumnIndex OldColumn,
          TColumnIndex NewColumn, bool &Allowed);
   void __fastcall cbSubstituteNumberChange(TObject *Sender);
private:	// User declarations
   void GetStockSubstitute(AnsiString Location);
   void DisplayStockSubstitute();
   void UpdateSubstituteStocks();
   void LoadLocations();
   TList *Substitutes;
   TStringList *RecipeStocks;
   bool CheckSubstitute(TStockSubstituteData *SubstituteData);
   void DisplayStockSubstituteItem(TStockSubstituteData *SubstituteData);
   void CloneSubstituteData(TStockSubstituteData *Source,TStockSubstituteData *Destination);
   void LoadNumbers(TStockSubstituteData *SubstituteData);
   void GetRecipeStocks();
   void UpdateToOriginalStock();
   int Decimalpalaces;
public:		// User declarations
   __fastcall TfrmStockSubstitute(TComponent* Owner);
     AnsiString Location;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmStockSubstitute *frmStockSubstitute;
//---------------------------------------------------------------------------
#endif
