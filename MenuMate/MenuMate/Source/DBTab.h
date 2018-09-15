//---------------------------------------------------------------------------

#ifndef DBTabH
#define DBTabH

//---------------------------------------------------------------------------

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>
#include <set>
#include "MM_DBCore.h"
#include "enumTab.h"
#include "MMContactInfo.h"
#include "PatronType.h"

//---------------------------------------------------------------------------

class TDBTab
{
private :

	static bool DeleteTab(Database::TDBTransaction &DBTransaction,long inTabKey);
	static bool DeletePaymentType(Database::TDBTransaction &DBTransaction,long inTabKey);

public:
	TDBTab();
	~TDBTab();

	static bool CloseTab(Database::TDBTransaction &DBTransaction,long inTabKey, bool isNewTabAccess = false);

	static void SetTabType(Database::TDBTransaction &DBTransaction,int inTabKey, TMMTabType inType);
	static void SetTabName(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString PartyName);
	static void SetTabPIN(Database::TDBTransaction &DBTransaction,int inTabKey,UnicodeString PIN);
	static void SetTabCredit(Database::TDBTransaction &DBTransaction,int inTabKey, Currency Amount);
	static void SetTabCreditLimit(Database::TDBTransaction &DBTransaction,int inTabKey, Currency Amount);

    static void SetTabDetails(Database::TDBTransaction &DBTransaction, int inTabKey, UnicodeString Name, UnicodeString Type,
                                UnicodeString Number, UnicodeString Date, TMMTabType TabType);
    static int SetTabDetails(Database::TDBTransaction &DBTransaction, int inTabKey, UnicodeString Name, Currency tabCreditLimit, TMMTabType TabType);
    static int SetTabDetails(Database::TDBTransaction &DBTransaction, int inTabKey, Currency tabCreditLimit);

    static void SetTabPermanent(Database::TDBTransaction &DBTransaction,int inTabKey,bool Permanent);
	static void SetTabLocked(Database::TDBTransaction &DBTransaction, int inTabKey, bool Locked, UnicodeString Reason);
	static void SetTabCard(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString Card);
	static void SetTabOwner(Database::TDBTransaction &DBTransaction,int TabKey,int inContactKey,TMMTabType TabType);
	static void SetTabToInvoice(Database::TDBTransaction &DBTransaction,int TabKey, int InvoiceKey);
	static void SetTabOrdersLoyalty(Database::TDBTransaction &DBTransaction,int inTabKey, int inContactKey);
	static void SetSplitDivisions(Database::TDBTransaction &DBTransaction,long TabKey,int DivisionsLeft);

	static bool ClearTab(Database::TDBTransaction &DBTransaction,long TabKey);

    static int GetOrCreateTab(Database::TDBTransaction &DBTransaction, int inTabKey);

	static int GetTab(Database::TDBTransaction &DBTransaction,UnicodeString Name,TMMTabType TabType);
	static int GetTabByCard(Database::TDBTransaction &DBTransaction,UnicodeString Card);
	static UnicodeString GetTabName(Database::TDBTransaction &DBTransaction,int TabKey);
	static UnicodeString GetTabPIN(Database::TDBTransaction &DBTransaction,int inTabKey);
	static UnicodeString GetTabCard(Database::TDBTransaction &DBTransaction,int TabKey);

	static Currency GetTabBalance(Database::TDBTransaction &DBTransaction,long TabKey);
	static Currency GetTabCredit(Database::TDBTransaction &DBTransaction,long TabKey);
	static Currency GetTabsCredit(Database::TDBTransaction &DBTransaction,std::set<__int64> &TabKeys);
	static Currency GetTabCreditLimit(Database::TDBTransaction &DBTransaction,long TabKey);
	static Currency GetTabTotal(Database::TDBTransaction &DBTransaction,long TabKey);
	static Currency GetTabDiscount(Database::TDBTransaction &DBTransaction,long TabKey);
	static Currency GetTabTotalExcl(Database::TDBTransaction &DBTransaction,long TabKey);

	static TMMTabType GetTabType(Database::TDBTransaction &DBTransaction,int inTabKey,bool isTabSelected=true);



	static void GetTabDetails(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString &Name,UnicodeString &IDType,
                        UnicodeString &Number,UnicodeString &Date,TMMTabType &TabType);


    static bool GetIsEmpty(Database::TDBTransaction &DBTransaction,int inTabKey);

    static bool GetTabExists(Database::TDBTransaction &DBTransaction, int tabKey);
    

    static bool GetTabPermanent(Database::TDBTransaction &DBTransaction,int inTabKey);
	static bool GetTabLocked(Database::TDBTransaction &DBTransaction,int inTabKey);
	static int GetTabByOwner(Database::TDBTransaction &DBTransaction,int ContactKey);
	static int GetTabOwner(Database::TDBTransaction &DBTransaction,int TabKey);
	static int GetTabInvoice(Database::TDBTransaction &DBTransaction,int TabKey);
	static int GetSplitDivisions(Database::TDBTransaction &DBTransaction,long TabKey);
	static UnicodeString GetTabLockedReason(Database::TDBTransaction &DBTransaction,int inTabKey);

	static bool GetOutStanding(Database::TDBTransaction &DBTransaction,int ContactKey);
	static void GetTabs(Database::TDBTransaction &DBTransaction,TStringList * TabList, TMMTabType TabType);
	static void GetOrderKeysExcludeSides(Database::TDBTransaction &DBTransaction,int TabKey,std::set<__int64> *OrderKeys);
	static void GetOrderKeys(Database::TDBTransaction &DBTransaction,int TabKey,std::set<__int64> &OrderKeys);
	static void GetPartialPaymentList(Database::TDBTransaction &DBTransaction,int inTabKey, TStringList *List,bool isTable);
	static void SetPartialPayment(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString InvoiceNumber,bool isTable);
	static bool GetTabRemovable(Database::TDBTransaction &DBTransaction,int TabKey);

	static bool LockTab(Database::TDBTransaction &DBTransaction,UnicodeString TerminalName,long TabKey);
	static bool LockTabs(Database::TDBTransaction &DBTransaction,std::set<__int64> TabsToLock, UnicodeString TerminalName, bool Override = false);

	static void ReleaseTab(Database::TDBTransaction &DBTransaction,UnicodeString TerminalName,long TabKey = 0);
	static void ReleaseTabs(Database::TDBTransaction &DBTransaction,UnicodeString TerminalName,std::set<__int64> TabsToRelease);
	static UnicodeString GetLocker(Database::TDBTransaction &DBTransaction,long TabKey);
	static bool GetLocked(Database::TDBTransaction &DBTransaction,long TabKey);
	static bool DeleteTabType(Database::TDBTransaction &DBTransaction,eTabType TabType);
	static int GetTabByDelyedInvoiceNumber(Database::TDBTransaction &DBTransaction,AnsiString DelayedInvoiceNumber,AnsiString &TabName);
	static int GetDelyedInvoiceNumberCount(Database::TDBTransaction &DBTransaction);
	static void SetTabDiscounts(Database::TDBTransaction &DBTransaction, int inTabKey, int DiscountKey);
	static void DeleteTabDiscounts(Database::TDBTransaction &DBTransaction, int inTabKey);
	static TMMContactInfo GetDiscountDetails(Database::TDBTransaction &DBTransaction, int inTabKey, TMMContactInfo &Info) ;
	static int GetTotalDivisions(Database::TDBTransaction &DBTransaction,long TabKey);
	static void SetTotalDivisions(Database::TDBTransaction &DBTransaction,long TabKey,int TotalDivisions);
	static void ClearTabHistory(Database::TDBTransaction &DBTransaction,int inTabKey,bool isTable);
	static bool GetTabDiscountExists(Database::TDBTransaction &DBTransaction,int TabKey);
    static bool SetVisible(Database::TDBTransaction &dbTransaction, int itemKey, UnicodeString sizeName, int menuKey);
    static void UpdateTabName(Database::TDBTransaction &dbTransaction, long SourceKey, UnicodeString clipTabName,bool isTabSelected);
    static void UpdateGuestLimit(Database::TDBTransaction &dbTransaction, long sourceKey, long destKey);
    static void UpdateTabType(Database::TDBTransaction &dbTransaction, long SourceKey,TMMTabType tabType =TabClipp);
    static TMMTabType GetLinkedTableAndClipTab(Database::TDBTransaction &DBTransaction,int inTabKey,bool isTabSelected=true,TMMTabType tabType=TabNormal);
    static int GetTableKeyFromSeat(Database::TDBTransaction &dbTransaction,long SourceKey);
    static int GetMaxAvailableSeatKey(Database::TDBTransaction &dbTransaction, int destTableKey);
    static int GetInvoiceNo(Database::TDBTransaction &dbTransaction, int invoiceKey);
    static AnsiString GetInvoiceNoFromTabKey(Database::TDBTransaction &dbTransaction, int tabKey);
    static UnicodeString GetAccountNumber(Database::TDBTransaction &dbTransaction, int tabKey);
    static void SetDelayedPatronCount(Database::TDBTransaction &dbTransaction, int _tabKey, std::vector<TPatronType> _patrons);
    static void DeleteDelayedPatronCount(Database::TDBTransaction &dbTransaction, int _tabKey);
    static std::vector<TPatronType> GetDelayedPatronCount(Database::TDBTransaction &dbTransaction, int _tabKey);
    static bool HasOnlineOrders(int tabKey);
    static UnicodeString GetMemberEmail(int tabKey);
};

#endif

