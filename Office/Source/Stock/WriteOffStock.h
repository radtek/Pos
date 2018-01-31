//---------------------------------------------------------------------------

#ifndef WriteOffStockH
#define WriteOffStockH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "NumericEdit.h"
#include <Buttons.hpp>
#include <DB.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <ComCtrls.hpp>
#include <IBSQL.hpp>
#include <vector>

//---------------------------------------------------------------------------
class TfrmWriteOffStock : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel1;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TStringGrid *sgWriteOffs;
	TGroupBox *GroupBox4;
	TLabel *Label7;
	TLabel *Label3;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label2;
	TBitBtn *btnAddSupplierUnit;
	TNumericEdit *neCost1;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TNumericEdit *neCost3;
	TNumericEdit *neQty;
	TLabel *Label1;
    TLabel *Label4;
    TEdit *Edit1;
    TLabel *Label6;
    TDateTimePicker *DateTimePicker1;
    TIBQuery *qrLocations;
    TDataSource *dsLocation;
    TIBTransaction *IBTransaction1;
    TComboBox *cbLocations;
    TIBQuery *qrItem;
    TIBQuery *qrPrice;
    TEdit *SuppliedUnit;
    TDataSource *dsSuppliedUnit;
    TLabel *lUnit;
    TBitBtn *btnRemove;
    TNumericEdit *neTotal;
    TLabel *Label5;
    TIBQuery *qrStockLocation;
    TIBQuery *IBQuery2;
    TIBSQL *Query;
    TEdit *EItem;
    TBitBtn *btnAddItem;
    void __fastcall LocationOnSelect(TObject *Sender);
    void __fastcall QuantityOnChange(TObject *Sender);
    void __fastcall btnAddOnClick(TObject *Sender);
    void __fastcall sgWriteOffsSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall btnRemoveOnClick(TObject *Sender);
    void __fastcall btnokOnClick(TObject *Sender);
    void __fastcall btncancelOnClick(TObject *Sender);
    void __fastcall btnAddItemOnClick(TObject *Sender);
    void __fastcall neQtyKeyPress(TObject *Sender, char &Key);
private:	// User declarations
    void CostDisplayChange(void);
    void UpdateStockLocation(int StockKey);
    void EnableAdd(void);
    void UpdateStockTrans(void);
    void StockLocation(void);
    AnsiString Unit;
    AnsiString Item;
    AnsiString Locations;
    double Quantity;
    bool CostDisplayChangeOk;
    int StockKey;
    double Price;
    int RowNumber;
    std::vector<double> ItemQty;
    std::vector<double> itemcosts;
    std::vector<TDateTime> DateArray;
    double totalcost;
    int DeleteRow;

public:		// User declarations
    __fastcall TfrmWriteOffStock(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 