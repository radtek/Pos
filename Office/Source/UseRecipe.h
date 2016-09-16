//---------------------------------------------------------------------------

#ifndef UseRecipeH
#define UseRecipeH
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
#include <IBSQL.hpp>
#include <vector>
#include <map>
//---------------------------------------------------------------------------
class TfrmUseRecipe : public TForm
{
__published:
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TVirtualStringTree *vtvStock;
    TDBGrid *DBGrid1;
    TEdit *Edit1;
    TMemo *Memo1;
    TNumericEdit *NumQty;
    TBitBtn *BtnOk;
    TBitBtn *BitBtn2;
    TIBDataSet *dtRecipes;
    TIBTransaction *Transaction;
    TDataSource *dsRecipes;
    TIBQuery *qrRecipe;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TNumericEdit *NumericEdit1;
    TComboBox *ComboBox1;
    TIBSQL *Query;
    TComboBox *ComboBox2;
    TLabel *Label5;
    TIBQuery *qrRecipeComplete;
    TPanel *Panel5;
    TLabel *Label1;
    TNumericEdit *NumTotal;
    TLabel *Label6;
    TLabel *Label7;
    TIBTransaction *IBTransaction1;
    TIBSQL *MMQuery;
   TEdit *SearchReceipe  ;
    TBitBtn *BitBtnSearch;
    void __fastcall DBGridOnClick(TColumn *Column);
    void __fastcall vtvStockGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
    void __fastcall QuantityOnKeyPress(TObject *Sender, char &Key);
    void __fastcall vtvStockCreateEditor(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink);
    void __fastcall vtvStockEdited(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
    void __fastcall vtvStockOnEditing(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, bool &Allowed);
    void __fastcall vtvStockAfterPaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
    void __fastcall vtvStockBeforePaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
    void __fastcall vtvStockFocusChanged(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
    void __fastcall vtvStockFocusChanging(TBaseVirtualTree *Sender,
          PVirtualNode OldNode, PVirtualNode NewNode,
          TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed);
    void __fastcall BtnOkOnClick(TObject *Sender);
    void __fastcall BtnCancelOnClick(TObject *Sender);
    void __fastcall NumQtyOnChange(TObject *Sender);
    
    void __fastcall SearchReceipeOnClick(TObject *Sender );
    void __fastcall SearchBtnClick(TObject *Sender  );
    void __fastcall NumTotalKeyPress(TObject *Sender, char &Key);
    void __fastcall NumericEdit1KeyPress(TObject *Sender, char &Key);

private:
    void DisplayStock(void);
    void GetLocations(AnsiString Item, TComboBox *ComboBox);
    void UpdateDB(void);
    void UpdateDescription(int Recipe_ID);
    void UpdateLocationsDB(AnsiString Item, AnsiString Location, double Qty);
    void UpdateTotal(void);
    void PopulatePriceArray(AnsiString Location, AnsiString Item);
    void UpdatePrices(AnsiString Item, AnsiString Location);
    bool IsSecretRecipe(void);
    AnsiString RecipeName;
    bool Filter;
    bool HideCosts;
    int RecipeID;
    std::vector<AnsiString> Locations;
    std::map<AnsiString, double> ItemPrices;
    bool ReceipeSearch;
    void Searchreceipe(AnsiString ReceipeName);
    bool IsSearchBoxClicked;
    void loadreceipe();
public:
    __fastcall TfrmUseRecipe(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif


