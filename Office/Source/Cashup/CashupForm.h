//---------------------------------------------------------------------------
#ifndef CashupFormH
#define CashupFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <ComCtrls.hpp>
#include "DateRangePicker.h"
#include "ZedCollection.h"
#include "VirtualTrees.hpp"
#include "NumericEdit.h"
#include "touchbtn.h"
#include "touchcontrols.h"
//---------------------------------------------------------------------------
class TfrmCashup : public TForm
{
__published:	// IDE-managed Components
	TTouchBtn *btnExit;
	TPanel *Panel1;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TPageControl *PageControl1;
	TTabSheet *tsZReports;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label6;
	TLabel *Label7;
	TListView *ListView1;
	TComboBox *cbEndHours;
	TComboBox *cbEndMinutes;
	TComboBox *cbStartHours;
	TComboBox *cbStartMinutes;
	TTabSheet *tsBalance;
	TLabel *Label5;
	TDateTimePicker *dtpStartDate;
	TLabel *Label1;
	TDateTimePicker *dtpEndDate;
	TTouchBtn *btnSelectToday;
	TTouchBtn *btnSelectYesterday;
	TVirtualStringTree *vtvStocktake;
	TNumericEdit *neStockQty;
	TTouchBtn *btnNext;
	TTouchBtn *btnViewReport;
	TTabSheet *tsZedReport;
	TMemo *Memo1;
	TTouchBtn *btnBackOutOfReport;
	TBevel *Bevel1;
	TImage *Image1;
	TTouchBtn *btnBackOutOfEntry;
	TTouchBtn *btnSave;
	TTouchBtn *btnReports;
	void __fastcall btnExitMouseClick(TObject *Sender);
	void __fastcall btnSelectTodayMouseClick(TObject *Sender);
	void __fastcall btnSelectYesterdayMouseClick(TObject *Sender);
	void __fastcall vtvStocktakeGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
	void __fastcall vtvStocktakeAfterPaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
	void __fastcall vtvStocktakeBeforePaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
	void __fastcall vtvStocktakeCreateEditor(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink);
	void __fastcall vtvStocktakeEdited(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
	void __fastcall vtvStocktakeEditing(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, bool &Allowed);
	void __fastcall vtvStocktakeFocusChanged(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
	void __fastcall vtvStocktakeFocusChanging(TBaseVirtualTree *Sender,
          PVirtualNode OldNode, PVirtualNode NewNode,
          TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed);
	void __fastcall ListView1Change(TObject *Sender, TListItem *Item,
          TItemChange Change);
	void __fastcall btnNextMouseClick(TObject *Sender);
	void __fastcall btnViewReportMouseClick(TObject *Sender);
	void __fastcall btnBackOutOfReportMouseClick(TObject *Sender);
	void __fastcall btnBackOutOfEntryMouseClick(TObject *Sender);
	void __fastcall btnSaveMouseClick(TObject *Sender);
	void __fastcall neStockQtyKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall neStockQtyKeyPress(TObject *Sender, char &Key);
	void __fastcall neStockQtyKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall btnReportsMouseClick(TObject *Sender);
	void __fastcall PageControl1Change(TObject *Sender);
private:	// User declarations
	DateRangePicker dateRangePicker;
	Cashup::ZedCollection zedCollection;
	void __fastcall DateTimeChanging(
		TDateTime OldStart, TDateTime& NewStart,
		TDateTime OldEnd, TDateTime& NewEnd);
	void __fastcall DateTimeChanged(TDateTime NewStart, TDateTime NewEnd);

	void LoadZeds();
	void SelectZed(int zedId);
	int _zedId;
public:		// User declarations
	__fastcall TfrmCashup();
};
//---------------------------------------------------------------------------
#endif
