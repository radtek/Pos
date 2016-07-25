//---------------------------------------------------------------------------
#ifndef ShowPrintoutH
#define ShowPrintoutH
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
#include "TouchBtn.h"
#include "TouchControls.h"
#include <Memory>
#include "ZForm.h"
//---------------------------------------------------------------------------
class TfrmShowPrintout : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TTouchBtn *btnCancel;
	TTouchBtn *btnClose;
	TTouchBtn *btnBillUp;
	TTouchBtn *btnBillDown;
	TMemo *memReceipt;
	TTouchBtn *btnClosePrint;
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnClosePrintClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall btnBillDownAutoRepeat(TObject *Sender);
	void __fastcall btnBillUpAutoRepeat(TObject *Sender);
private:
	void ShowPrintout();
	TDateTime FSelectedDate;
protected:
	__fastcall TfrmShowPrintout(TForm *Owner);
public:

	void Execute();
	bool PrintoutExist();
	virtual __fastcall ~TfrmShowPrintout();
	const std::auto_ptr<TMemoryStream> CurrentPrintout;
	int ExitCode;	
};
//---------------------------------------------------------------------------
#endif
