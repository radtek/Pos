// ---------------------------------------------------------------------------

#ifndef MembershipH
#define MembershipH

// ---------------------------------------------------------------------------
#include <DBWeb.hpp>
#include <HTTPApp.hpp>

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

// ---------------------------------------------------------------------------
#include<list>
#include "Modules.h"
#include "MM_DBCore.h"
#include "Processing.h"
#include "Contact.h"
#include "ThorlinkDataObjects.h"
#include "PointsTransaction.h"
#include <map>
// ---------------------------------------------------------------------------


//---------------------------------------------------------------------------
struct TVoucherDetail
{
    public:
        AnsiString VoucherNumber;
        AnsiString VoucherName;
        AnsiString DiscountCode;
        int NumberOfUsesRemaining;
};

class TVoucherUsageDetail
{
  public :
    //Gift Card
    AnsiString GiftCardNumber;
    double PointsRedeemed;
    //Member Voucher
    AnsiString VoucherName;
    double MemberVoucherDiscountAmount;
    //Pocket Voucher
    AnsiString PocketVoucherNumber;
    double PocketVoucherDiscountAmount;
    //Discounts
    std::map<AnsiString,double> DiscountUsage;
    TDateTime TransactionDate;
    double TotalSaleAmount;
    AnsiString MemberUniqueId;
    long SiteCode;
    AnsiString ReferenceNumber;
    AnsiString InvoiceNumber;
};

class TReleasedVoucherDetail
{
public :
   AnsiString ReferenceNumber;
   AnsiString GiftCardNumber;
   AnsiString VoucherName;
   AnsiString PocketVoucherNumber;
   std::vector<AnsiString> DiscountUsage;
};
// ---------------------------------------------------------------------------
enum eMemberNameOnPoleDisplay
{
   eMNPDNone, eMNPDAlias, eMNPDFullName, eMNPDCurrencySymbol
};

// ---------------------------------------------------------------------------
// typedef std::pair<int,UnicodeString> TQuickMember;
// ---------------------------------------------------------------------------
class TQuickMember
{
public:
   TQuickMember();

   TQuickMember(int inKey, UnicodeString inName) : Key(inKey), Name(inName)
   {
   }

   int Key;
   UnicodeString Name;

   friend bool operator == (const TQuickMember & f1, const TQuickMember & f2);
};

class TransData
{
	public:
	Currency Points;
	TDateTime Time;
};


class InvoiceData
{
	public:
	InvoiceData(int inInvoiceNumber, AnsiString inContact, std::vector<TransData> inTransaction);
	int InvoiceNumber;
	AnsiString Contact;
	std::vector<TransData> Transaction;
};


bool operator == (const TQuickMember & x, const TQuickMember & y)
{
   if ((x.Key == y.Key) && (x.Name == y.Name))
   {
	  return true;
   }
   return false;
};

class TMembership : public TContact
{
private:
   TLoginSuccess GetSubsPaid(Database::TDBTransaction &DBTransaction, int ContactKey);

   void __fastcall GetReportCardInfoFormatCell(TObject *Sender, int CellRow, int CellColumn, THTMLBgColor &BgColor, THTMLAlign &Align,
	  THTMLVAlign &VAlign, UnicodeString &CustomAttrs, UnicodeString &CellData);

   void __fastcall GetReportThisCardsInfoFormatCell(TObject *Sender, int CellRow, int CellColumn, THTMLBgColor &BgColor, THTMLAlign &Align,
	  THTMLVAlign &VAlign, UnicodeString &CustomAttrs, UnicodeString &CellData);

protected:
public:
    bool ReadOnlyInterface;
    bool EnableIncrementalSearch;
    int MaxRemembered;
    eMemberNameOnPoleDisplay NameOnPoleDisplay;
    int MinMemberNumber;
    bool RecycleMemberNumber;
    double CurrentYearPoints;
    double PreviousYearPoints;
    double AvailableBDPoint;
    double AvailableFVPoint;
    double AvailableEarnedPoint;
    double AvailableLoadedPoint;
    AnsiString RedeemedVoucherName;
    AnsiString RedeemedVoucherDiscount;
    std::vector<TVoucherDetail> MemberVouchers;
    TContactEvents OnBeforeSaveMember;
    std::vector <TQuickMember> LastAccessed;
    void ResetPoints();
    void Initialise(Database::TDBTransaction &DBTransaction);

    TMembership(TModules &Modules);

    void virtual InitTransaction(Database::TDBTransaction &DBTransaction);

    TLoginSuccess virtual FindMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo);

    TLoginSuccess virtual TestKeyLogin(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo);

    void virtual LoyaltyAddValue(Database::TDBTransaction &DBTransaction, TPointsTransaction &PointsTransaction);
    Currency virtual LoyaltyGetValue(Database::TDBTransaction &DBTransaction, int inContactKey);
    Currency LoyaltyGetSpendValue(Database::TDBTransaction &DBTransaction, int inContactKey);
    void virtual GetContactDetails(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info);
    bool virtual GetContactExists(Database::TDBTransaction &DBTransaction, int inContactKey);
    int virtual GetContactByCode(Database::TDBTransaction &DBTransaction, int inContactCode);
    void virtual SetContactCode(Database::TDBTransaction &DBTransaction, int inContactKey, int inContactID);
    void SetContactDetails(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info);
    void SetContactLoyaltyAttributes(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info);
    void GetInvoices(Database::TDBTransaction &DBTransaction, TStringList * TabList, int inContactKey);
    void GetLuckyMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &LuckyMember, int Hours, UnicodeString Location);

    bool virtual CheckVersionNumber(Database::TDBTransaction &DBTransaction)
    {
    return true;
    }
    bool CheckSiteIndex(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info);

    void virtual UpdatePointsTransactions(Database::TDBTransaction &DBTransaction, std::vector <TPointsTransaction> &PointsTransactions);
    void virtual GetValidPointsTransactions(Database::TDBTransaction &DBTransaction, int ContactKey,
    TPointsTransactionContainer &PointsTransactions);

    virtual ~TMembership();

    bool DemoMode;

    void virtual GetReportMemberStart(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report);
    void virtual GetReportMemberOnPremisesStart(Database::TDBTransaction &DBTransaction, TStringList *Report);
    void virtual GetReportMemberInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report);
    void virtual GetReportMemberInfoRestricted(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report);
    void virtual GetReportMemberPurchaseInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report);
    void virtual GetReportMemberFavouritesInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report);
    void virtual GetReportMemberPointsInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report);
    void virtual GetReportPointsTransactions(Database::TDBTransaction &DBTransaction, TStringList *Report);
    void virtual GetReportPointsTransactions(Database::TDBTransaction &DBTransaction, TStringList *Report, bool t);
    void virtual GetReportPointsTransferred(Database::TDBTransaction &DBTransaction, TStringList *Report);
    void virtual GetReportPointsTransferred(Database::TDBTransaction &DBTransaction, TStringList *Report, bool t);
    void virtual GetReportMemberAudit(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report,
    UnicodeString TerminalName);
    void virtual GetReportMemberCardsInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report);
    void virtual GetReportThisCardsInfo(Database::TDBTransaction &DBTransaction,int inSmartCardKey, UnicodeString GUID, TStringList *Report);
    void virtual GetReportMemberOnPremisesInfo(Database::TDBTransaction &DBTransaction, TStringList *Report);
    Currency virtual GetPointsHeldTotal(Database::TDBTransaction &DBTransaction);
    void virtual GetReportMemberStop(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report);
    void virtual GetReportStop(Database::TDBTransaction &DBTransaction, TStringList *Report);
    TDateTime GetDateOfBirth(Database::TDBTransaction &DBTransaction, int ContactKey);
    TDateTime GetLastVisit(Database::TDBTransaction &DBTransaction, int ContactKey);
    TDateTime GetLastBirthdayProcessed(Database::TDBTransaction &DBTransaction, int ContactKey);
    UnicodeString GetNextRecycledMemberNumber(Database::TDBTransaction &DBTransaction);
    UnicodeString GetNextMemberNumber(Database::TDBTransaction &DBTransaction);

    void virtual SyncMembers(Database::TDBTransaction &DBTransaction, TfrmProcessing *Progress);
    void virtual GetKioskDisplayMsg(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info, std::map <UnicodeString,UnicodeString> &Data);
    bool virtual ValidateVisit(TMMContactInfo &UserInfo);
    void virtual SetVisitCount(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo);
    TLoginSuccess virtual Login(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo);
    void SetLastVisit(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member);
    void SetLastBirthdayProcessed(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member);
    void ClearLastVisitTimes(Database::TDBTransaction &DBTransaction);
    void ResetLastVisitTimes(Database::TDBTransaction &DBTransaction);
    Currency GetEarnedPointsInDateRange(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member,TDateTime startDate,TDateTime endDate);
    virtual void FastContactListing(TIBSQL **, Database::TDBTransaction &,int, int, bool);
    const char *FastFormatMemberName(AnsiString);
    // TODO : Clean up remove this.
    //void InitialiseEarntPoints(Database::TDBTransaction &DBTransaction);
    virtual TLoginSuccess Authorise(TMMContactInfo &UserInfo,Currency wrkPayAmount);
    virtual TLoginSuccess IsCard(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
    virtual void TransactionClosed(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member);
    static bool MembershipApplied(Database::TDBTransaction &DBTransaction,__int64 MemberKey);
    bool CheckForBirthdayBenifit(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member);
    bool DisplayBirthDayNotification(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member);
    bool DisplayFirstVisitNotification(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member);
    double GetEarnedPointsForCurrentYear(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member);
    virtual bool MemberCodeScanned(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
    virtual bool UpdateMemberCardCode(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo,AnsiString memberCardCode);
    virtual AnsiString SendRequestForPay(std::vector<TTenderDetails> tenderDetailsList,std::vector<TItemDetailsThor> itemsList);
   virtual AnsiString SendRequestForRefund(TRefundTransaction refundTransaction);
   virtual bool CallInitMethod();
};
#endif