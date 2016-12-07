//---------------------------------------------------------------------------

#ifndef ShowEJournalH
#define ShowEJournalH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "Comms.h"
#include "PrintFormat.h"
#include "MMMessageBox.h"
#include "Printout.h"
#include "ReportUtilities.h"

#define RECEIPT_DELIMITER "#####"
//---------------------------------------------------------------------------
class TfrmEJournal : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
    TPanel *Panel1;
    TTouchBtn *btnCancel;
    TTouchBtn *btnSaveAsPDF;
    TTouchBtn *btnBillUp;
    TTouchBtn *btnBillDown;
    TMemo *memReceipt;
    TTouchBtn *btnClosePrint;
    TGroupBox *GroupBoxFilter;
    TDateTimePicker *ToDateTimePicker;
    TDateTimePicker *FromDateTimePicker;
    TLabel *LabelFromDate;
    TLabel *LabelToDate;
    TTouchBtn *btnGenerate;
    void __fastcall btnCancelMouseClick(TObject *Sender);
    void __fastcall FromDateOnChange(TObject *Sender);
    void __fastcall ToDateOnChange(TObject *Sender);
    void __fastcall btnGenerateMouseClick(TObject *Sender);
    void __fastcall btnSavePDFMouseClick(TObject *Sender);
    void __fastcall btnPrintMouseClick(TObject *Sender);
    void __fastcall btnReportDownAutoRepeat(TObject *Sender);
    void __fastcall btnReportUpAutoRepeat(TObject *Sender);
    void __fastcall FromDateTimePickerExit(TObject *Sender);
    void __fastcall FromDateTimePickerCloseUp(TObject *Sender);
private:	// User declarations
	int ExitCode;
public:		// User declarations
    __fastcall TfrmEJournal(TComponent* Owner);
    const std::auto_ptr<TMemoryStream> CurrentPrintout;
	void Execute();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEJournal *frmEJournal;
//---------------------------------------------------------------------------
#endif
