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
#include <Dialogs.hpp>
#include "EJournalEngine.h"
#include "Printout.h"

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
    TLabel *LabelFromDate;
    TLabel *LabelToDate;
    TTouchBtn *btnGenerate;
    TDateTimePicker *FromDateTimePicker;
    TDateTimePicker *ToDateTimePicker;
    TSaveDialog *SaveDialog1;
    void __fastcall btnCancelMouseClick(TObject *Sender);
    void __fastcall btnGenerateMouseClick(TObject *Sender);
    void __fastcall btnSavePDFMouseClick(TObject *Sender);
    void __fastcall btnPrintMouseClick(TObject *Sender);
    void __fastcall btnReportDownAutoRepeat(TObject *Sender);
    void __fastcall btnReportUpAutoRepeat(TObject *Sender);
    void __fastcall FromDateTimePickerCloseUp(TObject *Sender);
    void __fastcall ToDateTimePickerCloseUp(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	int ExitCode;
    void CheckAndPopulateData();
    void PopulateReport(TMemoryStream *Receipt);
    void ExtractEJournalReport(EJournalType type);
    void ExtractZedAndXReport(AnsiString deviceName);
    void ExtractZedReport(AnsiString deviceName);
    void ExtractZedReceiptReport(AnsiString deviceName);
    void ExtractZedReceiptAndXReport(AnsiString deviceName);
    void ExtractConsolidatedZedReport(AnsiString deviceName);
    //TConsolidatedMonthType TypeOfMonth;
public:		// User declarations
    __fastcall TfrmEJournal(TComponent* Owner);
    const std::auto_ptr<TMemoryStream> CurrentPrintout;
	void Execute();
    bool IsConsolidatedZed;
    bool CheckDateRangeForConolidatedZed();
    bool CalculateDateRangeForConolidatedZed(int toMonth, int fromMonth, int fromDay, int toDay, int fromYear);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEJournal *frmEJournal;
//---------------------------------------------------------------------------
#endif
