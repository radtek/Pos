//---------------------------------------------------------------------------
#ifndef AnalysisH
#define AnalysisH
#include "Membership.h"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <Db.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <DB.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include <vector>
#include <map>
#include "StockInterface.h"
#include "MM_DBCore.h"
#include "ZForm.h"
#include "Printing.h"
#include "ManagerReports.h"
#include "BlindBalanceController.h"
#include "StaffHours.h"
#include "Commission.h"
#include "PrinterReadings.h"
#include "PaxCount.h"
#include "POS_XMLBase.h"
#include "GUIScale.h"
#include "GlobalSettings.h"
#include "ReportUtilities.h"
#include "ReportManager.h"
#include "enumPoints.h"
#include "XeroInvoiceBuilder.h"
#include "XeroIntegration.h"
#include "MYOBInvoiceBuilder.h"
#include "MYOBIntegration.h"
#include <DCBillProcessingAtZed.h>
//---------------------------------------------------------------------------
enum TReportSource {
	CashDrawer,
	PrintZed,
	SelectDish
};

class TfrmAnalysis : public TZForm
{
	friend TZForm;

	__published:	// IDE-managed Components
	TPanel *pnlReports;
	TTouchGrid *TouchGrid1;
	void __fastcall btnReportsClick(void);
	void __fastcall btnZReportClick(void);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnReprintZClick(void);
	void __fastcall tbSetFloatClick(void);
	void __fastcall btnAlterFloatClick(void);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall tbSettleUserClick(void);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnUpdateStockClick(void);
	void __fastcall GridMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);

private:	// User declarations
	std::auto_ptr<TPOS_XMLBase> GroupTotalsXML;
	std::auto_ptr<TPOS_XMLBase> CategoriesTotalsXML;
	std::auto_ptr<TPOS_XMLBase> PaymentTotalsXML;
	std::auto_ptr<TPOS_XMLBase> CalculatedTotalsXML;
	std::auto_ptr<TPOS_XMLBase> DiscountTotalsXML;
	std::auto_ptr<TPOS_XMLBase> ProductTotalsXML;
	std::auto_ptr<TPOS_XMLBase> StaffTotalsXML;
	std::auto_ptr<TPOS_XMLBase> HourlyTotalsXML;
	std::auto_ptr<TPOS_XMLBase> DiscountListXML;
	std::auto_ptr<TPOS_XMLBase> StaffListXML;
	std::auto_ptr<TPOS_XMLBase> PaymentTypeListXML;
	std::auto_ptr<TPOS_XMLBase> PatronCountXML;
	std::auto_ptr<TPOS_XMLBase> PatronListXML;
	std::auto_ptr<TPOS_XMLBase> GroupsListXML;
	std::auto_ptr<TPOS_XMLBase> CategoryListXML;
    UnicodeString StockMasterPath;
	bool IntergratedEFT;
	std::vector<UnicodeString>	PalmList;
	int	 PalmListOffset;
	int	 ChosenDeviceOffset; // The green button
	TMemoryStream * ZedToArchive;
    static TMMContactInfo lastAuthenticatedUser;
    bool are_any_tables_or_tabs_outstanding() const;
    TMemoryStream * ZedToMail; // MM-4130
    std::vector<int> zed_ids;
    TDateTime date_time;
    int ResetKey; //MM- 4579
    std::vector<int> CANCELITEMS_KEY_ids;
    bool CheckCANCELITEMS_KEY;
    int MallExportKey;
    int IntegerValue;
    UnicodeString FieldName;
    UnicodeString StringValue;
    Currency CurrencyValue;
    TDateTime TimeStampValue;
    std::map<UnicodeString, UnicodeString> DataRead;
    std::vector<int> GetContactKeys;

    __fastcall TfrmAnalysis(TComponent* Owner);
    void __fastcall ReportXReport();
	void __fastcall ReportConsumption();
	void __fastcall ReportFloatAdjustments();
	void __fastcall ReportWriteOff(void);
	void __fastcall ReportSiteSummary(void);
	void __fastcall TfrmAnalysis::VariantDBAssign(TIBXSQLVAR *InData,TIBXSQLVAR *OutData);
	bool __fastcall TfrmAnalysis::ParamExists(TIBSQL *IBSQL, AnsiString FieldName);
	void __fastcall TableTabSummaryReport();
	void __fastcall ExportIntaMateVersion();
	void __fastcall ExportIntaMateListPaymentTypes();
	void __fastcall ExportIntaMatePaymentTotals();
	void __fastcall ExportIntaMateGroupTotals();
	void __fastcall ExportIntaMateHourlyTotals();
	void __fastcall ExportIntaMateCategoriesTotals();
	void __fastcall ExportIntaMateListDiscounts();
	void __fastcall ExportIntaMateDiscountTotals();
	void __fastcall ExportIntaMateListCalculated(); // Fixed List.
	void __fastcall ExportIntaMateCalculatedTotals();
	void __fastcall ExportIntaMateProductTotals();
	void __fastcall ExportIntaMateListStaff();
	void __fastcall ExportIntaMateStaffTotals();
	void __fastcall ExportIntaMatePatronCounts();
	void __fastcall ExportIntaMateListGroups();
	void __fastcall ExportIntaMateListCategories();
    void __fastcall ResetPoint(Database::TDBTransaction &DBTransaction, int ContactKey,TResetPoints check); //MM- 4579
    void __fastcall CheckDate(Database::TDBTransaction &DBTransaction, int keys,int ContactKey, double points,TResetPoints check, int caseTicked, AnsiString boxTicked,AnsiString options); //MM- 4579
    void __fastcall FiscalPrinterSettlement();

    void CheckCancelItemsTable();
    void UpdateCancelItemsTable();
    void UpdateArcMallExport(Database::TDBTransaction &DBTransaction);
    void MallExportReadFromDB(UnicodeString DataQuery, std::map<UnicodeString, UnicodeString> &DataRead);
    UnicodeString MallExportCheckValue(UnicodeString StringValue, int IntegerValue, Currency CurrencyValue,TDateTime TimeStampValue, TIBSQL* query);
    void DeleteStaffHours(Database::TDBTransaction &DBTransaction);
    void CheckUserLoggedOut(Database::TDBTransaction &DBTransaction);
    void UpdateZedStaffHoursEnable(Database::TDBTransaction &DBTransaction, int key);
    TMemoryStream* FormattedZed(TMemoryStream *ZedToArchive);
    void CheckPocketVoucherPaymentType();
    void SyncCompanyDetails();
    void UpdateSalesForce();
    void EmailZedReport(int z_key);
    void UpdateMallExportDetails();
    void ClearParkedSale(Database::TDBTransaction &DBTransaction);
    void UpdateArchive(TIBSQL *IBInternalQuery, Database::TDBTransaction &DBTransaction, UnicodeString DeviceName, int zedKey);
    void UpdateStock(bool UpdateingStock);
    void ResetPoints();
    void OpenCashDrawer();
    void PostDataToXeroAndMyOB(std::vector<TXeroInvoiceDetail>  &XeroInvoiceDetails, std::vector<TMYOBInvoiceDetail>  &MYOBInvoiceDetails, bool CompleteZed);
    void UpdateDLFMall();
    void UpdateContactTimeZedStatus(Database::TDBTransaction &DBTransaction);
    UnicodeString CheckRegistered();
    void GetCategoryNameAndGLCode(Database::TDBTransaction &DBTransaction, std::map<AnsiString, AnsiString>&CollectCategoryName, TDateTime startTime, TDateTime endTime);
    TPrintout* SetupPrintOutInstance();
	void UpdateArchive(Database::TDBTransaction &DBTransaction,TMembership *Membership,UnicodeString DeviceName, int zedKey);
	void ProcessStock(Database::TDBTransaction &DBTransaction);
	bool UpdateStockAllowed(Database::TDBTransaction &DBTransaction);
	void UpdateStockComplete(Database::TDBTransaction &DBTransaction);
	void PrintConsumption(Database::TDBTransaction &DBTransaction);
	void AdjustStock(Database::TDBTransaction &DBTransaction, StockInterface::TStockInterface &StockInterface);
	void GetCosts(Database::TDBTransaction &DBTransaction,StockInterface::TStockInterface &StockInterface);
	inline void Log(bool Success, UnicodeString Message);
	void GetFloatDetails(UnicodeString DeviceName, int Z_Key);
	void AddSectionTitle(TPrintout *Printout,UnicodeString Title);
	void AddSubSectionTitle(TPrintout *Printout,UnicodeString Title);
	void PrintoutFormatForTxtValue(TPrintout *Printout);
	void WriteBlindBalance(Database::TDBTransaction &DBTransaction, TPrintout *Printout, AnsiString Title, TBlindBalances Balances, AnsiString DeviceName, bool t);
	void PrintBlindBalance(Database::TDBTransaction &DBTransaction, TBlindBalances Balances, AnsiString DeviceName);

	bool GetBlindBalences(Database::TDBTransaction &DBTransaction,TBlindBalances &Balance, AnsiString &DepositBagID, AnsiString DeviceName);
	void UpdateBlindBlances(Database::TDBTransaction &DBTransaction,int ZedKey,TBlindBalances &Balance, AnsiString &BagID);
	void UpdateCommissionDatabase(Database::TDBTransaction &DBTransaction,int ZedKey, TCommissionCache &Commission);
	void UpdatePrinterReadingsDatabase(Database::TDBTransaction &DBTransaction,int ZedKey, TPrinterReadingsInterface &PrinterReading);
	void UpdatePaxCountDatabase(Database::TDBTransaction &DBTransaction,int ZedKey, TPaxCount &inPaxCount);


	void GetSummaGrossSalesTotal(Database::TDBTransaction &DBTransaction,UnicodeString DeviceName, TCalculatedTotals &BaseSales);
	void GetSummaNetSalesTotal(Database::TDBTransaction &DBTransaction,UnicodeString DeviceName,TCalculatedTotals &BaseSales);
	void GetSummaGrossNet(Database::TDBTransaction &DBTransaction,int ArchiveKey, TCalculatedTotals &BaseSales);

	void DefaultItemQuantities(Database::TDBTransaction &);

	// Accumulated ZED
	Currency grand_total_eod;
	void SaveVariable(vmVariables vmVar, int CompName);
    int GetBeginningInvoiceNumber();
    int GetEndingInvoiceNumber();
    void GetReportData( int _key);
    void UpdateEmailstatus(int z_key);
    bool checkPointPurchase(Database::TDBTransaction &DBTransaction, int ContactKey); // MM 4579
    bool checkPointRedeem(Database::TDBTransaction &DBTransaction, int ContactKey); // MM 4579
    bool checkPointEarned(Database::TDBTransaction &DBTransaction, int ContactKey); // MM 4579
    void PostToAccountingSystem();
    std::vector<TXeroInvoiceDetail> CalculateAccountingSystemData(Database::TDBTransaction &DBTransaction);
    std::vector<TMYOBInvoiceDetail> CalculateMYOBData(Database::TDBTransaction &DBTransaction, TBlindBalances Balances);
    AnsiString GetJOBCode(AnsiString glCode);
    AnsiString GetCompanyName(Database::TDBTransaction &DBTransaction);
    void CreateXeroInvoiceAndSend(std::vector<TXeroInvoiceDetail>  &XeroInvoiceDetails);
    void CreateMYOBInvoiceAndSend(std::vector<TMYOBInvoiceDetail>  &MYOBInvoiceDetails);
    bool SendInvoiceToMYOB( TMYOBInvoice* inMYOBInvoice );
    bool SendInvoiceToXero( TXeroInvoice* inXeroInvoice );
    //-------------------------------------------------
    void AddInvoiceItem(TXeroInvoiceDetail &XeroInvoiceDetail,AnsiString Description,double unitAmount,AnsiString AccountCode,double taxAmount);
    void AddMYOBInvoiceItem(TMYOBInvoiceDetail &MYOBInvoiceDetail,AnsiString GLCode,AnsiString Description,double unitAmount,double taxAmount,AnsiString AccountCode,AnsiString taxStatus);
    AnsiString GetMYOBJobCode(Database::TDBTransaction &DBTransaction);
    void GetPaymentDetails(AnsiString &paymentDetails, AnsiString terminalNamePredicate);
    void CalculateNextday(TDateTime &nextDay);
    void GetQueriesForMYOB(AnsiString &Tax,AnsiString &zeroTax,AnsiString terminalNamePredicate);
    //--------------------------------------------------
    void GetPointsAndVoucherData(Database::TDBTransaction &DBTransaction,double &PurchasedPoints, double &PurchasedVoucher,TDateTime startTime,TDateTime endTime);
    bool CheckContactsAdjustmentType(Database::TDBTransaction &DBTransaction, int contacts, int type); //MM5610
    void AddInvoicePayment(TXeroInvoiceDetail &XeroInvoiceDetail,AnsiString Description,double unitAmount,AnsiString AccountCode,double taxAmount);
    void GetTabCreditReceivedRefunded(Database::TDBTransaction &DBTransaction,double &TabCreditReceived, double &TabRefundReceived,
                                 TDateTime startTime,TDateTime endTime);
    void GetFloatAmounts(Database::TDBTransaction &DBTransaction,double &floatAmount,AnsiString terminalNamePredicate);
    AnsiString GetCashGlCode(Database::TDBTransaction &DBTransaction);
    void CompleteDLFMallExport();
    TDateTime GetMinDayArchiveTime(Database::TDBTransaction &DBTransaction, TDateTime PrevZedTime);
    void UpdateZKeyForMallExportSales(bool isMasterTerminal, int fieldIndex);
    double GetCashWithdrawal(Database::TDBTransaction &DBTransaction);
    double GetCashBlindBalance(TBlindBalances Balances);
    TDateTime GetMaxDayArchiveTime(Database::TDBTransaction &DBTransaction);
    double GetTipAmount(Database::TDBTransaction &DBTransaction,TDateTime startTime,UnicodeString &tipGLCode);
    double GetOldAccumulatedSales(Database::TDBTransaction &DBTransaction, int fieldIndex);
    void UpdateAccumulatedSales(Database::TDBTransaction &DBTransaction);
public:		// User declarations
	bool ZedCancel;
	bool ZedCompleted;
    Currency TerminalEarnings;

	static TLoginSuccess AuthenticateReportsAccess(TReportSource);
	static const TMMContactInfo &GetLastAuthenticatedUser();
	TFinancialDetails GetFinancialDetails(Database::TDBTransaction &DBTransaction,TTransactionInfo &TransactionInfo,AnsiString DeviceName);
	void BuildXMLListCalculated(TPOS_XMLBase &Data);
	void SiteSummaryReport(Database::TDBTransaction &, TPrintout *, TDateTime);
	void PaxCountReport(Database::TDBTransaction &, TPrintout *, TPaxCount &PaxCount, Currency &Total, TStaffHoursInterface &inStaffHours, TCommissionCache &inCommission, TPrinterReadingsInterface &inPrinterReadings);
	void PrintFloatAdjustments(Database::TDBTransaction &DBTransaction, UnicodeString DeviceName);
	UnicodeString GetTerminalName();
	void PrintWriteOff(Database::TDBTransaction &DBTransaction, UnicodeString DeviceName);
	TDateTime GetPrevZedTime(Database::TDBTransaction &DBTransaction);
	void print_weborder_statistics(Database::TDBTransaction &, TPrintFormat &);
	void print_chit_statistics(TPrintout &);
    void FileSubmit(const char * hostName, const char * userName,
                    const char * userPassword, const char * userPath,
                    UnicodeString LocalPathFileName, UnicodeString LocalFileName,
                    int FCount);
     void SaveCompValueinDBStrUnique(vmVariables vmVar, UnicodeString CompName);
};

//---------------------------------------------------------------------------

class TCurTotal : public TObject
{
public:
	Currency Total;
	UnicodeString Note;
	TDateTime TimeStamp;
	int Count;
};
//---------------------------------------------------------------------------
class TCurPriceAdj : public TObject
{
public:
	UnicodeString StaffName;
	Currency TotalUp;
	Currency TotalDown;
};
//---------------------------------------------------------------------------


#endif

