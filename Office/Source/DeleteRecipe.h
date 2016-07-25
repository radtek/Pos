//---------------------------------------------------------------------------

#ifndef DeleteRecipeH
#define DeleteRecipeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
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
//---------------------------------------------------------------------------
class TDeleteRecipeNodeData
{
public:
	AnsiString Recipe;
    AnsiString Final_Stock;
    int Recipe_ID;

};

class TfrmDeleteRecipe : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel4;
    TLabel *Label1;
    TBitBtn *btnOk;
    TBitBtn *btnCancel;
    TIBTransaction *Transaction;
    TIBDataSet *dtRecipes;
    TDataSource *dsRecipes;
    TVirtualStringTree *vtvStock;
    TIBSQL *Query;
    TIBQuery *qrRecipe;
    void __fastcall btnCancelOnClick(TObject *Sender);
    void __fastcall vtvStockGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
    void __fastcall DeleteRecipeOnClick(TObject *Sender);
private:	// User declarations
    void DisplayRecipes(void);
    void DeleteRecipe(int Recipe_ID);
public:		// User declarations
    __fastcall TfrmDeleteRecipe(TComponent* Owner);
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
