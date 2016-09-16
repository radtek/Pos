//---------------------------------------------------------------------------

#ifndef EditRecipeH
#define EditRecipeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "NumericEdit.h"
#include "VirtualTrees.hpp"
#include <Buttons.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>

#include "CreateRecipe.h"
#include <IBSQL.hpp>
#include "Utilities.h"

//---------------------------------------------------------------------------
class TfrmEditRecipe : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TLabel *Label1;
    TVirtualStringTree *vtvStock;
    TDBGrid *DBGrid1;
    TEdit *Edit1;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TBitBtn *BitBtn4;
    TBitBtn *BitBtn5;
    TNumericEdit *NumericEdit1;
    TNumericEdit *NumericEdit2;
    TComboBox *cbLocation;
    TLabel *Label2;
    TIBTransaction *Transaction;
    TIBDataSet *dtRecipes;
    TDataSource *dsRecipes;
    TIBQuery *qrRecipe;
    TIBQuery *qrDescription;
    TIBQuery *QueryRecipe;
    TIBSQL *Query;
    TLabel *Label3;
    TBitBtn *BitBtn6;
    TLabel *Label4;
    TLabel *Label5;
    void __fastcall DBGridOnClick(TColumn *Column);
    void __fastcall vtvStockGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
    void __fastcall FindItemOnClick(TObject *Sender);
    void __fastcall BoxExit(TObject *Sender);
    void __fastcall IncludeOnClick(TObject *Sender);
    void __fastcall LocationOnDropDown(TObject *Sender);
    void __fastcall QuantityOnClick(TObject *Sender);
    void __fastcall CancelOnClick(TObject *Sender);
    void __fastcall OkOnClick(TObject *Sender);
    void __fastcall vtvStockCreateEditor(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink);
    void __fastcall vtvStockOnEditing(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, bool &Allowed);
    void __fastcall vtvStockOnEdited(TBaseVirtualTree *Sender, PVirtualNode Node,
          TColumnIndex Column);
    void __fastcall vtvStockAfterPaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
    void __fastcall vtvStockBeforePaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
    void __fastcall vtvStockFocusChanged(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
    void __fastcall vtvStockFocusChanging(TBaseVirtualTree *Sender,
          PVirtualNode OldNode, PVirtualNode NewNode,
          TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed);
    void __fastcall RemoveItemOnClick(TObject *Sender);
    void __fastcall NumericEdit1Change(TObject *Sender);
    void __fastcall NumericEdit1KeyPress(TObject *Sender, char &Key);
    void __fastcall NumericEdit2KeyPress(TObject *Sender, char &Key);
private:	// User declarations
    AnsiString RecipeName;
    int RecipeID;
    AnsiString StockCode;
    AnsiString StockLocation;
    std::vector<AnsiString> TempLocations;
    std::vector<AnsiString> StockLocations;
//	const std::auto_ptr<TfrmReceiveStockItem>frmReceiveStockItem;
    void DisplayStock(AnsiString inRecipe);
    void ChangeFont(TObject *Object, ebox box);
    void CheckDefaultLocation(void);
    void GetStockLocations(AnsiString Code);
    void GetStockLocations(TComboBox *ComboBox, AnsiString Code);
    double GetItemCost(AnsiString Item, AnsiString Location);
    void PopulatePriceArray(AnsiString Location, AnsiString Item);
    bool ChangeInRecipe;
    void UpdateDB(void);

    std::map<AnsiString, double> ItemPrices;
public:		// User declarations
    __fastcall TfrmEditRecipe(TComponent* Owner);
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
 