//---------------------------------------------------------------------------

#ifndef MallExportResendReportH
#define MallExportResendReportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include <ExtCtrls.hpp>
#include "ZForm.h"
//---------------------------------------------------------------------------
class TfrmMallExportResendReport : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
    TPanel *Panel1;
    TTouchBtn *btnOk;
    TTouchBtn *btnCancel;
    TPanel *Panel2;
    TTouchGrid *tgReportList;
    TTouchBtn *btnResendReport;
    TTouchBtn *btnSelectAll;
    void __fastcall btnOkMouseClick(TObject *Sender);
    void __fastcall btnCancelMouseClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall tgReportListMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton);
    void __fastcall btnResendReportMouseClick(TObject *Sender);
    void __fastcall btnSelectAllMouseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmMallExportResendReport(TComponent* Owner);
    TStringList* allReports;
    TStringList* ReportList;
    UnicodeString YearFolder;
    bool status;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMallExportResendReport *frmMallExportResendReport;
//---------------------------------------------------------------------------
#endif
