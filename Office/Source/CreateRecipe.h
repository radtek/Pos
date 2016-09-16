//---------------------------------------------------------------------------

#ifndef CreateRecipeH
#define CreateRecipeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "VirtualTrees.hpp"
#include "NumericEdit.h"
#include <Buttons.hpp>
#include <Mask.hpp>
#include "PurchaseOrder.h"
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include <DBCtrls.hpp>
#include <vector>
#include <map>



const int GUIBoxes = 4;
enum ebox{eRecipeName, eDescription, eQuantity, eLocation, eStockBox};
enum {eAVG_COST, eLATEST_COST};

const char * MessagesArray[3] = {"Please select a stock item",
                            "Please enter a non zero stock quantity",
                            "Please select a location from the drop down menu"};

typedef std::map<AnsiString, AnsiString> InnerMap;
typedef std::map<InnerMap, double> NestedMap;
//---------------------------------------------------------------------------
class TRecipeNodeData
{
public:
	AnsiString	Code;
	WideString	Text;
	AnsiString	Location;
	AnsiString	Unit;
	double		Qty;
    double      RecipeQty;
    double      AverageCost;
    double      LatestCost;
};

//---------------------------------------------------------------------------
class TfrmCreateRecipe : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TVirtualStringTree *vtvStock;
    TEdit *ERecipeName;
    TEdit *Edit3;
    TBitBtn *BitBtn1;
    TEdit *Edit4;
    TBitBtn *btnInclude;
    TBitBtn *btnRemove;
    TBitBtn *btnOk;
    TBitBtn *BitBtn5;
    TBitBtn *BitBtn6;
    TComboBox *cbStockLocation;
    TMemo *MemDescription;
    TIBQuery *qrDescription;
    TIBTransaction *IBTransaction1;
    TComboBox *cbFinalLocation;
    TIBSQL *Query;
    TDataSource *dsDescription;
    TLabel *Label2;
    TIBQuery *QueryRecipe;
    TPanel *Panel5;
    TLabel *Label1;
    TNumericEdit *NumericEdit1;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TNumericEdit *NumQuantity;
    void __fastcall DescriptionOnClick(TObject *Sender);
    void __fastcall FindItemMouseClick(TObject *Sender);
    void __fastcall RecipeNameOnClick(TObject *Sender);
    void __fastcall QuantityOnClick(TObject *Sender);
    void __fastcall BoxExit(TObject *Sender);
    void __fastcall LocationDropDown(TObject *Sender);
    void __fastcall IncludeOnClick(TObject *Sender);
    void __fastcall vtvStockGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
    void __fastcall RemoveOnClick(TObject *Sender);
    void __fastcall CancelOnClick(TObject *Sender);
    void __fastcall OkOnClick(TObject *Sender);
    void __fastcall vtvStockAfterPaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
    void __fastcall vtvStockBeforePaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall NumQuantityChange(TObject *Sender);
    void __fastcall NumQuantityClick(TObject *Sender);
    void __fastcall NumQuantityKeyPress(TObject *Sender, char &Key);
private:	// User declarations
    void EnterText(AnsiString &Text);
    void ChangeFont(TObject *Object, ebox box);
    std::vector<bool> firstclick;
    void GetStockLocations(TComboBox *ComboBox, AnsiString Code);
    void GetStockLocations(AnsiString Code);
    AnsiString StockCode;
    AnsiString FinalStockCode;
    AnsiString Stock_Unit;
    AnsiString StockLocation;
    std::vector<AnsiString> StockLocations;
    std::vector<AnsiString> FinalStockLocations;
    std::vector<AnsiString> TempLocations;
    void CheckDefaultLocation(void);
    bool CheckQuantity(std::vector<double> &QtyVec, std::vector<std::pair<AnsiString, AnsiString> > VecOfKeys);
    bool CheckFinalStock_Recipe(void);
    void UpdateLocationDB(void);
    void UpdateRecipeDB(TRecipeNodeData *NodeData, double Costs[], int RecipeID);
    bool CheckRecipeExists(void);
    double GetItemCost(AnsiString Item, AnsiString Location);
    void UpdatePrices(AnsiString Item, AnsiString Location);

public:		// User declarations
    __fastcall TfrmCreateRecipe(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
