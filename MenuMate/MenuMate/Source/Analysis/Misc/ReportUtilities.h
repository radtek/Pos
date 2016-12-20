#ifndef ReportUtilitiesH
#define ReportUtilitiesH

#include <map>
#include "BlindBalanceController.h"
#include "MM_DBCore.h"
#include "ManagerReports.h"
#include "GlobalSettings.h"
#include "ReportEnums.h"

struct TPointTransaction
{
   public:
        int   adjustmentType;
        int  adjustmentSubType;
        Currency  adjustment;
        AnsiString  invoiceNo;
};

struct TPointTransactions
{
   public:
   std::vector<TPointTransaction> PointsTransactions;
};

const char* const eStrCalculatedTotals[] =
{
    NULL,
    "Total GrossSales", // Summa call this Gross ( Its our Raw , excluding discount value)
    "Total NetSales",   // Summa call this Net ( Its our Gross , Including discount value)
    "Cash",
    "Initial Float",
    "Float Adjustments",
    "Change",
    "Points Sold",
    "Total Discounts",
    "Points Redeemed",
    "Points Movement",
    "Points Held",
    "Total Patron Count",
    "Tabs Credit Sold",
    "Tabs Credit Redeemed",
    "Credit Movement",
    "Vouchers Sold",
    "Vouchers Redeemed",
    "Voucher Movement",
    "Total Surcharges",
    "Total Cancels",
    "Total Cash drawer Opens",
    "Total Loyalty Sales",
    "Patron Count Saved Sales",
    "Patron Count Cash Sales",
    "Patron Count Tab Sales",
    "Patron Count Room Sales",
    "Patron Count Account Sales",
    "Patron Count Credited Sales",
    NULL
};

class TTransactionCount
{
public:
	int Count;
	int CashOutCount;
	int SurchargeCount;
	bool Counted;
	bool CashOutCounted;
	bool SurchargeCounted;
	__fastcall TTransactionCount();
};

class TCalculatedTotals
{
public :
	TCalculatedTotals(): Type(etcSOF), RawTotal(0), Total(0), TaxContent(0), Qty(0), TimeStamp(0) { };

	TCalculatedTotals(CalculatedTotals calculatedTotals, Currency rawTotal, Currency totalExclusive, Currency taxContent, int quantity)
	    : Type(calculatedTotals), RawTotal(rawTotal), Total(totalExclusive), TaxContent(taxContent), Qty(quantity), TimeStamp(0), Name(eStrCalculatedTotals[calculatedTotals]) { };

	TCalculatedTotals(UnicodeString name, Currency rawTotal, Currency totalExclusive, Currency taxContent, int quantity)
	    : Name(name), RawTotal(rawTotal), Total(totalExclusive), TaxContent(taxContent), Qty(quantity), Type(etcSOF), TimeStamp(0) {};

	UnicodeString Name;
	Currency Total;
	Currency RawTotal;
	Currency TaxContent;
	TDateTime TimeStamp;
	int Type;
	int Qty;
};

class TTransactionInfo
{
public:

	std::map<int, std::map<UnicodeString, TTransactionCount> >  TransactionsCountGroups;
	std::map<UnicodeString, TCalculatedTotals> CalculatedTotals;
	std::map<int, std::map<UnicodeString, TSumPayments> > Payments;

	Currency TotalSpentInSavedSales;
	int PatronCountInSavedSales;

	Currency TotalSpentInCashSales;
	int PatronCountInCashSales;

	Currency TotalSpentInCreditedSales;
	int PatronCountInCreditedSales;

	Currency TotalSpentInTabSales;
	int PatronCountInTabSales;

	Currency TotalSpentInRoomSales;
	int PatronCountInRoomSales;

	Currency TotalSpentInAccountSales;
	int PatronCountInAccountSales;
	int PatronCountInPurchasingCredit;

	Currency TotalCreditRedeemed;
	Currency TotalSalesTax;
	Currency TotalServiceCharge;
	Currency TotalServiceChargeTax;
};
// This class is made singleton in order to save the time that was consumed in repetitive execution
// of method GetTransactionInfo
class TTransactionInfoProcessor
{
private:
    std::map<UnicodeString, TTransactionInfo*> deviceTransactions;

public:

    static TTransactionInfoProcessor& Instance()
    {
       static TTransactionInfoProcessor singleton;
       return singleton;
    }

    TTransactionInfo GetTransactionInfo(Database::TDBTransaction &dbTransaction, UnicodeString deviceName, bool showendingbal = false);
	TTransactionInfo GetBalanceInfo(TBlindBalances &balance);

    //We will remove the transaction info object from the map, since we have completed the transaction for the current ZED..
    void RemoveEntryFromMap(UnicodeString deviceName);
    void NormalZedTransaction(TIBSQL *qrXArcBill, bool showendingbal);
    void LoadArcPayTransaction(TTransactionInfo* TransactionInfo, TIBSQL *qrXArcPay, bool loop, TIBSQL *qrXArcBill, std::map<AnsiString, TPointTransactions> pointTransaction, std::set<AnsiString> countedInvoiceNumbers, AnsiString currentInvoiceKey);
    void GetArcPayForNormalZed(TIBSQL *qrXArcPay);
    void GetArcPointsForNormalZed(TIBSQL *qXArcPoints1);
    void LoadArcPointTransaction(TIBSQL *qXArcPoints1, TTransactionInfo* TransactionInfo, TIBSQL *qrXArcPay, std::vector<TPointTransaction>::iterator ptrPoints);
    void GetArcSurchargeForNormalZed(TIBSQL *qXArcSurcharge);
    void LoadArcPointTransaction(TIBSQL *qXArcSurcharge, TTransactionInfo* TransactionInfo, std::set<AnsiString> countedInvoiceNumbers, AnsiString currentInvoiceKey);
};

class THourlyTotals
{
public :
	THourlyTotals(): TimeStamp(0), Value(0), Qty(0) { };

	TDateTime TimeStamp;
	Currency Value;
	int Qty;
};

class SkimCalculations
{
public:
    SkimCalculations() : CurrentFloat(0), CurrentSkimsTotal(0), Skims(0), Refloats(0) { };

    void CalculateSkims(Database::TDBTransaction &dbTransaction, UnicodeString deviceName);

    Currency CurrentFloat;
    Currency CurrentSkimsTotal;
    Currency Skims;
    Currency Refloats;
};

class TCurrencyTotal : public TObject
{
public:
	Currency Total;
	UnicodeString Note;
	TDateTime TimeStamp;
	int Count;
    UnicodeString Name; //MM-4385
    UnicodeString StaffName;
};

class TCurrencyPriceAdjustment : public TObject
{
public:
	UnicodeString StaffName;
	Currency TotalUp;
	Currency TotalDown;
};

class DataFormatUtilities
{
public:
    UnicodeString FormatMMReportCurrency(double inValue, bool inShowCurrencySymbol = false);
	UnicodeString FormatMMReportPoints(double inValue);
	UnicodeString FormatMMReportRedeemCredit(double inValue, bool inShowCurrencySymbol = false );
	UnicodeString FormatMMReportRedeemPoints(double inValue);
};

class DataCalculationUtilities
{
public:
    int GetZedKey(Database::TDBTransaction &dbTransaction);
    Currency GetTotalEarnings(Database::TDBTransaction &dbTransaction, UnicodeString deviceName, bool showendingbal = false);
    Currency GetAccumulatedZedTotal(Database::TDBTransaction &dbTransaction);
    TCalculatedTotals GetCashDrawerOpenTotals(Database::TDBTransaction &dbTransaction, TGlobalSettings* globalSettings, UnicodeString deviceName);
    TDateTime GetPreviousZedTimeForTerminal(Database::TDBTransaction &dbTransaction, UnicodeString terminalName);
    void DataCalculationUtilities::PrinterFormatinTwoSections(TPrintout* printOut);
    void DataCalculationUtilities::PrinterFormatinThreeSections(TPrintout* printOut);
    int CalculateLastDayOfMonth(int month);
    TDateTime CalculateSessionTransactionDate(TDateTime trans_date);
};



#endif

