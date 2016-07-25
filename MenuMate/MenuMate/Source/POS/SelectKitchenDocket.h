//---------------------------------------------------------------------------
#ifndef SelectKitchenDocketH
#define SelectKitchenDocketH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <IBQuery.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"

//---------------------------------------------------------------------------
class TfrmSelectKitchenDocket : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
   TPanel *Panel1;
	TTouchBtn *btnClose;
	TTouchBtn *btnPrev;
	TTouchBtn *btnPrint;
	TTouchBtn *btnNext;
   TLabel *Label1;
   TMonthCalendar *calReceipt;
   TSpeedButton *sbAllTerminals;
   TSpeedButton *sbThisTerminal;
	TTouchBtn *btnBillUp;
	TTouchBtn *btnBillDown;
   TMemo *memReceipt;
	TTouchBtn *TouchButton1;
	TSpeedButton *SpeedButton1;
	TLabel *Label2;
   void __fastcall btnPrintClick(TObject *Sender);
   void __fastcall btnCloseClick(TObject *Sender);
   void __fastcall calReceiptClick(TObject *Sender);
   void __fastcall sbAllClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
	void __fastcall TouchButton1MouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall sbThisTerminalClick(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
   void __fastcall memReceiptMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnPrevMouseClick(TObject *Sender);
	void __fastcall btnNextMouseClick(TObject *Sender);
	void __fastcall btnBillDownMouseClick(TObject *Sender);
	void __fastcall btnBillUpMouseClick(TObject *Sender);
private:
	__fastcall TfrmSelectKitchenDocket(TComponent* Owner);
	
	void PrintReceipt(TStringList *Receipt);
	void ShowReceipt();
	TDateTime FSelectedDate;
	void SetCurrentDate(TDateTime SelectedDate);
public:

	void Execute();
	void ClearReceiptArchive();
	void PrintLastReceipt();
   void SortTables();
   __property TDateTime CurrentDate = { read = FSelectedDate ,write = SetCurrentDate};
   virtual __fastcall ~TfrmSelectKitchenDocket();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectKitchenDocket *frmSelectKitchenDocket;
//---------------------------------------------------------------------------
#endif
