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
    std::map<UnicodeString, UnicodeString> DataRead;
    std::vector<UnicodeString> DataToWrite;
    ////DLFMALL
    void WriteInToFileForFirstCode(Database::TDBTransaction *dbTransaction, int day , int month , int year, TDateTime date);
    void WriteInToFileForSecondCode(Database::TDBTransaction *dbTransaction, int arcBillKey, int day , int month , int year, TDateTime date);
    void WriteInToFileForThirdCode(Database::TDBTransaction *dbTransaction, int arcBillKey, int day , int month , int year, TDateTime date);
    void WriteInToFileForFourthCode(Database::TDBTransaction *dbTransaction, int arcBillKey, int day , int month , int year, TDateTime date);
    void WriteInToFileForFifthCode(Database::TDBTransaction *dbTransaction, int arcBillKey, int day , int month , int year, TDateTime date);
    void WriteInToFileForSixthCode(Database::TDBTransaction *dbTransaction, int day , int month , int year);
    void LoadFileToFTP();
    int day, month, year;
    AnsiString terminal_Name;
    TEstanciaMall estanciaMall;

    TMallExportPrepareData PrepareDataForExport();

    //Prepare data for Invoice Sales File
    void PrepareDataForInvoiceSalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, TMallExportPrepareData &prepareDataForInvoice, int index);

    //Fetch Data For Invoice Sales File writing
    void PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, TMallExportPrepareData &prepareDataForHSF, int index);

    //Fetch Data For Daily Sales File writing
    void PrepareDataForDailySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, TMallExportPrepareData &prepareDataForDSF, int index);

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
    UnicodeString SetYear;

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


