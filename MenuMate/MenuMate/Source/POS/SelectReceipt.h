//---------------------------------------------------------------------------
#ifndef SelectReceiptH
#define SelectReceiptH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <Grids.hpp>
#include <IBQuery.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ZForm.h"
#include <ComCtrls.hpp>


//---------------------------------------------------------------------------
class TfrmSelectReceipt : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
   TPanel *Panel1;
   TLabel *Label1;
   TMonthCalendar *calReceipt;
   TSpeedButton *sbAllTerminals;
   TSpeedButton *sbThisTerminal;
	TTouchBtn *btnBillUp;
	TTouchBtn *btnBillDown;
   TMemo *memReceipt;
	TTouchBtn *btnClose;
	TTouchBtn *TouchBtn1;
	TTouchBtn *btnSearch;
	TTouchBtn *btnNext;
	TTouchBtn *btnPrev;
    TTouchBtn *btnAddTip;
   void __fastcall btnPrintClick(TObject *Sender);
   void __fastcall btnCloseClick(TObject *Sender);
   void __fastcall calReceiptClick(TObject *Sender);
   void __fastcall sbAllClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
	void __fastcall sbThisTerminalClick(TObject *Sender);
	void __fastcall btnNextMouseClick(TObject *Sender);
	void __fastcall btnPrevMouseClick(TObject *Sender);
	void __fastcall btnSearchMouseClick(TObject *Sender);
	void __fastcall btnPrevAutoRepeat(TObject *Sender);
	void __fastcall btnNextAutoRepeat(TObject *Sender);
	void __fastcall btnBillDownMouseClick(TObject *Sender);
	void __fastcall btnBillUpMouseClick(TObject *Sender);
    void __fastcall btnAddTipMouseClick(TObject *Sender);
private:
	void PrintReceipt(TStringList *Receipt);
	void ShowReceipt();
   TDateTime FSelectedDate;
   void SetCurrentDate(TDateTime SelectedDate);
   void toggleAvailabilityOfTippingButton();
public:
	__fastcall TfrmSelectReceipt(TComponent* Owner);
	void Execute();
	void Execute(AnsiString ReceiptNumber);   
	void ClearReceiptArchive();
	void PrintLastReceipt();
   void SortTables();
   void SearchUsingTransactionNumber(UnicodeString inTransactionNumber);
   __property TDateTime CurrentDate = { read = FSelectedDate ,write = SetCurrentDate};
   virtual __fastcall ~TfrmSelectReceipt();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectReceipt *frmSelectReceipt;
//---------------------------------------------------------------------------
#endif
