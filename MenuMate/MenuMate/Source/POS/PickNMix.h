//---------------------------------------------------------------------------
#ifndef PickNMixH
#define PickNMixH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <set>
#include <vector>

//---------------------------------------------------------------------------
class TfrmPickNMix : public TForm
{
__published:	// IDE-managed Components
	TTimer *Timer1;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TMemo *Memo1;
	TPanel *Panel5;
	TLabel *Label2;
	TLabel *Label8;
	TLabel *Label7;
	TStringGrid *StringGrid1;
	TPanel *Panel6;
	TLabel *Label1;
	TPanel *stCancel;
	TPanel *stBill;
	TPanel *stSplit;
	TPanel *stRemove;
	TPanel *btnBillUp;
	TPanel *btnBillDown;
	TPanel *btnDishUp;
	TPanel *btnDishDown;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall stCancelClick(TObject *Sender);
	void __fastcall stBillClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall btnDishUpMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnDishUpMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnDishDownMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnDishDownMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnBillUpMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnBillUpMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnBillDownMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnBillDownMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall stSplitClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall StringGrid1Click(TObject *Sender);
	void __fastcall StringGrid1DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall stRemoveMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations

	TList *TabOrderList;		// All the orders linked to their tabs in TPnMTab and TPnMOrder classes.
   std::vector<long> UndoList;

	void InitDishes();
	void ClearDishes();
	void ShowDishes();
   void ShowReceipt();
   TWinControl *ScrollingControl;
   TControl *ButtonClicked;
   bool ScrollUp;
	bool PickMode;
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)

public:		// User declarations
	__fastcall TfrmPickNMix(TComponent* Owner);
	std::set<long> TabKeys;    					// The tabs with the orders to pick
	eTransactionType SalesType;
	int BookingID;
	int RoomNumber;
};
//---------------------------------------------------------------------------
class TPnMTab
{
public:
	TPnMTab() 		{ 	Orders = new TList;	}
	~TPnMTab() 		{ 	delete Orders; 		}
	long 	Key;		// LK field in dtTab
	AnsiString 		Name;		// To be displayed in grid
   TList *			Orders;	// List of TPnMOrders (PnM = PickNMix)
//   Currency			Credit;
//   bool				CreditTransfered;
};
//---------------------------------------------------------------------------
   									// but on the bill.
//---------------------------------------------------------------------------
extern PACKAGE TfrmPickNMix *frmPickNMix;
//---------------------------------------------------------------------------
#endif
