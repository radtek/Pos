//---------------------------------------------------------------------------

#ifndef MallExportRegenerateReportH
#define MallExportRegenerateReportH
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

#include "MallExportFederalLandMall.h"
#include "MallExportShangrilaMall.h"
#include "MallExportMegaworldMall.h"
#include "MallExportAlphalandMall.h"
#include "MallExportAyalaMall.h"
#include "MallExportCapitalandMall.h"
#include "MallExportPowerPlantMall.h"
#include "MallExportRobinsonMall.h"
#include "MallExportSMMall.h"
#include "GlobalSettings.h"
#include "MallExportIInput.h"
#include "MallExportIOutput.h"
#include "MallExportDataManager.h"
#include <IBQuery.hpp>
#include "MM_DBCore.h"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmMallExportRegenerateReport : public TZForm
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
    TSpeedButton *sbAllTerminals;
    TSpeedButton *sbThisTerminal;
    TLabel *lbFrom;
    TLabel *lbTo;
    void __fastcall btnOkMouseClick(TObject *Sender);
    void __fastcall btnCancelMouseClick(TObject *Sender);
    void __fastcall mcStartDateClick(TObject *Sender);
    void __fastcall mcEndDateClick(TObject *Sender);
    void __fastcall btnGenerateMouseClick(TObject *Sender);
    void __fastcall cbStartHourChange(TObject *Sender);
    void __fastcall cbStartMinChange(TObject *Sender);
    void __fastcall cbEndHourChange(TObject *Sender);
    void __fastcall cbEndMinChange(TObject *Sender);
    void __fastcall edLocationPathMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall btnLoadPathMouseClick(TObject *Sender);
    void __fastcall sbThisTerminalClick(TObject *Sender);
    void __fastcall sbAllClick(TObject *Sender);
private:	// User declarations

    Database::TDBTransaction *dbTransaction;
    std::map<UnicodeString, UnicodeString> DataRead;
    std::vector<UnicodeString> DataToWrite;
    TMallExportIInput* InputManager;
    TMallExportIOutput* OutputManager;
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

public:		// User declarations
    __fastcall TfrmMallExportRegenerateReport(TComponent* Owner);

    TDateTime StartDate;
    TDateTime EndDate;
    TDateTime SDate;
    TDateTime EDate;
    UnicodeString ReportPath;
    UnicodeString StartHour;
    UnicodeString EndHour;
    UnicodeString StartMin;
    UnicodeString EndMin;
    UnicodeString DataQuery;
    UnicodeString SetYear;
    UnicodeString OutputValue;

    TMallExportDataManager* dataManager;
    TMallExportAlphalandMall* alphalandExport;
    TMallExportAyalaMall* ayalaExport;
    TMallExportCapitalandMall* capitalandExport;
    TMallExportPowerPlantMall* powerplantExport;
    TMallExportRobinsonMall* robinsonExport;
    TMallExportSMMall* smExport;
    TMallExportMegaworldMall* megaworldExport;
    TMallExportShangrilaMall* shangrilaExport;
    TMallExportFederalLandMall* federallandExport;

    void ResetMallExportValues();
    void InitializeTimeOptions();
    void RegenerateSpecificMall();
    void RegenerateSMExport();
    void RegenerateRobinsonExport();
    void RegenerateAyalaExport();
    void RegeneratePowerPlantExport();
    void RegenerateCapitalandExport();
    void RegenerateAlphalandExport();
    void RegenerateMegaworldExport();
    void RegenerateShangrilaExport();
    void RegenerateFederalLandExport();
    void GenerateNoExportDays(UnicodeString &TerminalName, UnicodeString &MallCode, TDateTime &TransactionDate, UnicodeString &BegInvoice,
                              UnicodeString &EndInvoice, Currency &OldGT, Currency &NewGT, UnicodeString &BegOR, UnicodeString &EndOR, UnicodeString Mall, bool &Pastdate);
    void InitializeTimeSet(TDateTime &SDate, TDateTime &EDate);
    UnicodeString FixTime(UnicodeString Time);
    UnicodeString GetHourlyFormat(UnicodeString TerminalName, UnicodeString TenantName,
                                  UnicodeString DateValue, UnicodeString TimeValue);
    // For Ayala
    UnicodeString GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                                UnicodeString &DateValue, UnicodeString &TimeValue,
                                int &TransactionTotal, UnicodeString &Amount);

    // For Megaworld
    int GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                                UnicodeString &DateValue, UnicodeString &TimeValue,
                                int &TransactionTotal, UnicodeString &Amount, int &PatronCount);

    // For Shangrila
    UnicodeString GetHourlyFormat(UnicodeString TenantName, UnicodeString DateValue, UnicodeString TimeValue,
                                                        UnicodeString AmountValue, UnicodeString TaxValue, UnicodeString AdjValue, UnicodeString SChargeValue,
                                                        int TransactionTotal, UnicodeString SalesDept);

    // For Federal Land
    UnicodeString GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                                UnicodeString &DateValue, UnicodeString &TimeValue,
                                int &TransactionTotal, UnicodeString &Amount, UnicodeString &MinuteValue,
                                int &SCDiscountCount, int &PatronCount);

    UnicodeString AddZeroesToValue(int ZeroCount, int Value);
    UnicodeString CheckValue(Currency Amount);
    void SetSpecificMallTimes(int &StartH, int &EndH, int &StartM, int &EndM);

    //DLF Mall Export
    void RegenerateDLFMallExport();

    void ShowDateTimes();

    UnicodeString MALLCODE;
    UnicodeString TENANT_NAME;
    UnicodeString CLASSCODE;
    UnicodeString TRADECODE;
    UnicodeString OUTLETNUMBER;
    Currency NEW_GRANDTOTAL;
    Currency OLD_GRANDTOTAL;
    UnicodeString SALESTYPE;
    Currency DEPARTMENTSUM;
    Currency REGDISCOUNT_AMOUNT;
    Currency EMPDISCOUNT_AMOUNT;
    Currency SCDISCOUNT_AMOUNT;
    Currency VIPDISCOUNT_AMOUNT;
    Currency PWDDISCOUNT_AMOUNT;
    Currency GPCDISCOUNT_AMOUNT;
    Currency DISCOUNTG1_AMOUNT;
    Currency DISCOUNTG2_AMOUNT;
    Currency DISCOUNTG3_AMOUNT;
    Currency DISCOUNTG4_AMOUNT;
    Currency DISCOUNTG5_AMOUNT;
    Currency DISCOUNTG6_AMOUNT;
    Currency VAT_SALES;
    Currency OTHERTAX;
    Currency ADJUSTMENTS;
    Currency POS_ADJ;
    Currency NEG_ADJ;
    Currency POS_ADJ_NONTAX;
    Currency NEG_ADJ_NONTAX;
    Currency DAILY_SALES;
    Currency VOID_AMOUNT;
    Currency REFUND_AMOUNT;
    Currency TAX_INCLUSIVE_SALES;
    Currency VATEXEMPT_SALES;
    Currency CHARGECRED_SALES;
    Currency CHARGECRED_SALES_TAX;
    Currency GIFT_CHEQUE_SALES;
    Currency DEBIT_CARD_SALES;
    Currency OTHER_TENDER_SALES;
    Currency MASTERCARD_SALES;
    Currency VISA_SALES;
    Currency AMEX_SALES;
    Currency DINERS_SALES;
    Currency JCB_SALES;
    Currency OTHER_CARD_SALES;
    Currency SCHARGE_AMOUNT;
    Currency OTHER_CHARGES;
    int FIRST_TRANSACTION;
    int LAST_TRANSACTION;
    int TRANSACTION_COUNT;
    UnicodeString BEGINNING_INVOICE;
    UnicodeString ENDING_INVOICE;
    int CASH_COUNT;
    int GC_COUNT;
    int DEBIT_COUNT;
    int OTHER_TENDER_COUNT;
    int MASTERCARD_COUNT;
    int VISA_COUNT;
    int AMEX_COUNT;
    int DINERS_COUNT;
    int JCB_COUNT;
    int OTHERCARD_COUNT;
    UnicodeString TERMINAL_NAME;
    UnicodeString SERIALNUMBER;
    TDateTime TRANSACTIONTIME;
    TDateTime TRANSACTIONDATE;
    Currency GROSS_SALES;
    int VOID_COUNT;
    int REGDISCOUNT_COUNT;
    int REFUND_COUNT;
    int SCDISCOUNT_COUNT;
    Currency EODOLD;
    Currency EODNEW;
    TDateTime DATE_VALUE;
    Currency NOVELTY_SALES;
    Currency MISC_SALES;
    Currency LOCALTAXES;
    Currency PHARMA_SALES;
    Currency NONPHARMA_SALES;
    Currency SALESNOTSUBTORENT;
    Currency REPTRANSACTION_AMOUNT;
    int REPTRANSACTION_COUNT;
    Currency CASH_SALES;
    UnicodeString HOUR_VALUE;
    Currency TOTALDISCOUNT;
    UnicodeString BEGINNING_OR;
    UnicodeString ENDING_OR;
    Currency LOCTAXEXEMPTDLY_SALES;
    int FINEDINECUST_COUNT;
    Currency TENDERSURCHARGES;
    Currency NONVAT_SALES;
    Currency CHECK_SALES;
    Currency EPAY_SALES;
    Currency NO_SALES;
    int PREVEODCOUNTER;
    int CURRENTEODCOUNTER;
    int DISCOUNT_COUNT;
    Currency CARD_SALES;
    Currency OTHER_SALES;
    TDateTime CURRENT_DATE_VALUE;
    UnicodeString INVOICE_NUMBER;
    Currency PRODISCOUNT_AMOUNT;
    Currency OTHERDISCOUNT_AMOUNT;
    Currency OTHER_SURCHARGE;
    Currency NET_SALES;
    Currency GC_SALES_AMOUNT;
    Currency DEPOSIT_AMOUNT;
    Currency APPLIED_DEPOSIT_AMOUNT;
    int ZCOUNT;
    int VAT_SALES_COUNT;
    int NONVAT_SALES_COUNT;
    int VATEXEMPT_SALES_COUNT;
    int EMPDISCOUNT_COUNT;
    int PRODISCOUNT_COUNT;
    int OTHERDISCOUNT_COUNT;
    int SCHARGE_COUNT;
    int OTHER_SURCHARGE_COUNT;
    int CARD_COUNT;
    int CHECK_COUNT;
    int EPAY_COUNT;
    int OTHER_COUNT;
    int TRANS_VOID_COUNT;
    int DEPOSIT_COUNT;
    int APPLIED_DEPOSIT_COUNT;
    int PATRON_COUNT;
    int NOSALE_COUNT;
    UnicodeString SALES_FLAG;
    Currency VAT_PERCENTAGE;
    Currency SALESTYPE_FOOD;
    Currency SALESTYPE_NONFOOD;
    Currency SALESTYPE_GROCERIES;
    Currency SALESTYPE_MEDICINES;
    Currency SALESTYPE_OTHERS;
    TDateTime TrasactionDate;
    int StartH;
    int EndH;
    int StartM;
    int EndM;
    bool isAllTerminalsSelected;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMallExportRegenerateReport *frmMallExportRegenerateReport;
//---------------------------------------------------------------------------
#endif
