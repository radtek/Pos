//---------------------------------------------------------------------------

#ifndef RegenerateMallReportH
#define RegenerateMallReportH
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
#include <ComCtrls.hpp>
#include "EstanciaMall.h"
#include <IBQuery.hpp>
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
class TfrmRegenerateMallReport : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TTouchBtn *btnOk;
    TTouchBtn *btnCancel;
    TMonthCalendar *mcStartDate;
    TMonthCalendar *mcEndDate;
    TTouchBtn *btnGenerate;
    TComboBox *cbStartHour;
    TComboBox *cbStartMin;
    TComboBox *cbEndHour;
    TComboBox *cbEndMin;
    TLabel *lbStartDate;
    TLabel *lbEndDate;
    TLabel *lbStartHour;
    TLabel *lbStartMin;
    TLabel *lbEndHour;
    TLabel *lbEndMin;
    TEdit *edLocationPath;
    TLabel *lbGenPath;
    TTouchBtn *btnLoadPath;
    TGroupBox *gbRegenReport;
    void __fastcall btnOkMouseClick(TObject *Sender);
    void __fastcall btnCancelMouseClick(TObject *Sender);
    void __fastcall mcStartDateClick(TObject *Sender);
    void __fastcall mcEndDateClick(TObject *Sender);
    void __fastcall btnGenerateMouseClick(TObject *Sender);
    void __fastcall cbStartHourChange(TObject *Sender);
    void __fastcall cbStartMinChange(TObject *Sender);
    void __fastcall cbEndHourChange(TObject *Sender);
    void __fastcall cbEndMinChange(TObject *Sender);
    void __fastcall edLocationPathMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnLoadPathMouseClick(TObject *Sender);
private:	// User declarations

    Database::TDBTransaction *dbTransaction;
    int day, month, year;
    AnsiString terminal_Name;
    TEstanciaMall estanciaMall;

    TMallExportPrepareData PrepareDataForExport(Database::TDBTransaction &dBTransaction, int zKey);

public:		// User declarations
    __fastcall TfrmRegenerateMallReport(TComponent* Owner);

    TDateTime StartDate;
    TDateTime EndDate;
    TDateTime SDate;
    TDateTime EDate;
    UnicodeString ReportPath;
    UnicodeString StartHour;
    UnicodeString EndHour;
    UnicodeString StartMin;
    UnicodeString EndMin;

    void InitializeTimeOptions();
    void InitializeTimeSet(TDateTime &SDate, TDateTime &EDate);
    UnicodeString FixTime(UnicodeString Time);
    void SetSpecificMallTimes(int &StartH, int &EndH, int &StartM, int &EndM);

    void RegenerateEstanciaMallExport();
    TDateTime TrasactionDate;
    int StartH;
    int EndH;
    int StartM;
    int EndM;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRegenerateMallReport *frmRegenerateMallReport;
//---------------------------------------------------------------------------
#endif


