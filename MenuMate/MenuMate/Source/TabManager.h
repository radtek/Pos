//---------------------------------------------------------------------------
#ifndef TabManagerH
#define TabManagerH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>
#include "Enum.h"
#include <set>
#include "MM_DBCore.h"
//---------------------------------------------------------------------------


class TTabManager
{
   private :
		static bool DeleteTab(Database::TDBTransaction &DBTransaction,long inTabKey);
	public:
	TTabManager();
	~TTabManager();

	static bool CloseTab(Database::TDBTransaction &DBTransaction,long inTabKey);

	static void SetTabType(Database::TDBTransaction &DBTransaction,int inTabKey, TMMTabType inType);
	static void SetTabName(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString PartyName);
	static void SetTabPIN(Database::TDBTransaction &DBTransaction,int inTabKey,UnicodeString PIN);
	static void SetTabCredit(Database::TDBTransaction &DBTransaction,int inTabKey, Currency Amount);
	static void SetTabCreditLimit(Database::TDBTransaction &DBTransaction,int inTabKey, Currency Amount);
	static void SetTabDetails(Database::TDBTransaction &DBTransaction,int inTabKey,UnicodeString Name,UnicodeString Type,
                      UnicodeString Number,UnicodeString Date,TMMTabType TabType);
	static void SetTabPermanent(Database::TDBTransaction &DBTransaction,int inTabKey,bool Permanent);
	static void SetTabLocked(Database::TDBTransaction &DBTransaction,int inTabKey,bool Locked,UnicodeString Reason);
	static void SetTabCard(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString Card);
	static void SetTabOwner(Database::TDBTransaction &DBTransaction,int TabKey,int inContactKey,TMMTabType TabType);
	static void SetTabToInvoice(Database::TDBTransaction &DBTransaction,int TabKey, int InvoiceKey);
	static void SetTabOrdersLoyalty(Database::TDBTransaction &DBTransaction,int inTabKey, int inContactKey);
	static void SetSplitDivisions(Database::TDBTransaction &DBTransaction,long TabKey,int DivisionsLeft);

	static bool ClearTab(Database::TDBTransaction &DBTransaction,long TabKey);
	static int GetTab(Database::TDBTransaction &DBTransaction,UnicodeString Name,TMMTabType TabType);
	static int GetTabByCard(Database::TDBTransaction &DBTransaction,UnicodeString Card);
	static UnicodeString GetTabName(Database::TDBTransaction &DBTransaction,int TabKey);
	static UnicodeString GetTabPIN(Database::TDBTransaction &DBTransaction,int inTabKey);
	static UnicodeString GetTabCard(Database::TDBTransaction &DBTransaction,int TabKey);
	static int GetOrCreateTab(Database::TDBTransaction &DBTransaction,int inTabKey);
	static Currency GetTabBalance(Database::TDBTransaction &DBTransaction,long TabKey);
	static Currency GetTabCredit(Database::TDBTransaction &DBTransaction,long TabKey);
	static Currency GetTabCreditLimit(Database::TDBTransaction &DBTransaction,long TabKey);
	static Currency GetTabTotal(Database::TDBTransaction &DBTransaction,long TabKey);
	static Currency GetTabDiscount(Database::TDBTransaction &DBTransaction,long TabKey);
	static Currency GetTabTotalExcl(Database::TDBTransaction &DBTransaction,long TabKey);

	static TMMTabType GetTabType(Database::TDBTransaction &DBTransaction,int inTabKey);

	static void GetTabDetails(Database::TDBTransaction &DBTransaction,int inTabKey,UnicodeString &Name,UnicodeString &IDType,
							 UnicodeString &Number,UnicodeString &Date,TMMTabType &TabType);
	static bool GetIsEmpty(Database::TDBTransaction &DBTransaction,int inTabKey);
	static bool GetTabExists(Database::TDBTransaction &DBTransaction,int TabKey);
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
	static void GetPartialPaymentList(Database::TDBTransaction &DBTransaction,int inTabKey, TStringList *List);
	static void SetPartialPayment(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString InvoiceNumber);
	static bool GetTabRemovable(Database::TDBTransaction &DBTransaction,int TabKey);
};

#endif

