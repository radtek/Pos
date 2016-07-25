//---------------------------------------------------------------------------

#ifndef SelectStockItemH
#define SelectStockItemH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <IBDatabase.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TfrmSelectStockItem : public TForm
{
__published:	// IDE-managed Components
	TIBQuery *qrStock;
	TDataSource *dsStock;
	TDBGrid *dbgResult;
	TBitBtn *btnOk;
	TIBTransaction *Transaction;
	TPanel *Panel1;
	TLabel *Label1;
	TEdit *edSearch;
	TBitBtn *btnGo;
	TBitBtn *btnFindAdd;
	TBitBtn *btnCancel;
	TLabel *Label2;
	void __fastcall btnGoClick(TObject *Sender);
	void __fastcall edSearchKeyPress(TObject *Sender, char &Key);
	void __fastcall dbgResultDblClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall dbgResultKeyPress(TObject *Sender, char &Key);
	void __fastcall btnFindAddClick(TObject *Sender);
	void __fastcall dbgResultKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall edSearchChange(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall CMDialogChar(TWMKey &Msg);
   BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
		MESSAGE_HANDLER(CM_DIALOGCHAR, TWMKey, CMDialogChar)
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
	void ResizeGrids();
public:		// User declarations
	__fastcall TfrmSelectStockItem(TComponent* Owner);

	int			StockKey;
	AnsiString	StockCode;
	AnsiString	Description;
	AnsiString	Unit;
   Currency ConversionFactor;
   AnsiString	ReductionUnit;
	bool			Reset;
   AnsiString Location;
   void ClearLocation();
	AnsiString	NewItemName;
    		// Allows a name to be entered from Menu Editor into new stock item
	    								// when press Find/Add
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectStockItem *frmSelectStockItem;
//---------------------------------------------------------------------------
#endif
