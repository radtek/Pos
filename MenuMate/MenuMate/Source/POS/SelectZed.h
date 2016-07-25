//---------------------------------------------------------------------------
#ifndef SelectZedH
#define SelectZedH
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
#include "MM_DBCore.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"

//---------------------------------------------------------------------------
class TCurrentZed
{
private:
	int ZedKey;
	int Max();
	int Min();
	Database::TDBControl &DBControl;
public:
	TDateTime TimeFilter;
	bool TerminalFilter;

	TMemoryStream *CurrentZed;
	__fastcall TCurrentZed(Database::TDBControl &inDBControl);
	virtual __fastcall ~TCurrentZed();
	bool Get();
	bool Prior();
	bool Next();
	bool Last();
	bool First();
   int Count();   
};

//---------------------------------------------------------------------------
class TfrmSelectZed : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
   TPanel *Panel1;
	TTouchBtn *btnClose;
	TTouchBtn *btnPrev;
	TTouchBtn *btnPrint;
	TTouchBtn *btnNext;
   TLabel *Label1;
   TMonthCalendar *calZed;
   TSpeedButton *sbAllTerminals;
   TSpeedButton *sbThisTerminal;
	TTouchBtn *btnBillUp;
	TTouchBtn *btnBillDown;
   TMemo *memReceipt;
   void __fastcall btnPrintClick(TObject *Sender);
   void __fastcall btnNextClick(TObject *Sender);
   void __fastcall btnPrevClick(TObject *Sender);
   void __fastcall btnCloseClick(TObject *Sender);
   void __fastcall calZedClick(TObject *Sender);
   void __fastcall sbAllClick(TObject *Sender);
   void __fastcall btnBillDownMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall btnBillUpMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	TTouchBtn *btnSendEmail; //MM-4104
	void __fastcall btnSendEmailClick(TObject *Sender); //MM-4104
	
private:

	
	TCurrentZed *Zed;
	void PrintReceipt(TStringList *Receipt);
	void ShowReceipt();
	TDateTime FSelectedDate;
	void SetCurrentDate(TDateTime SelectedDate);
	Database::TDBControl &DBControl;
    bool EnableEmail;  //MM-4104
    void __fastcall EmailSettingCheck(); // MM-4104
     TMemoryStream* FormattedZed(TMemoryStream *ZedToArchive);

public:
    __fastcall TfrmSelectZed(TComponent* Owner,Database::TDBControl &inDBControl);
	void Execute();
	bool ReceiptsExist();
	void ClearReceiptArchive();
   __property TDateTime CurrentDate = { read = FSelectedDate ,write = SetCurrentDate};
   virtual __fastcall ~TfrmSelectZed();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
