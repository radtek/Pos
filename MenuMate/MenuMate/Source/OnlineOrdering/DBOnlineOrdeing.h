//---------------------------------------------------------------------------

#ifndef DBOnlineOrdeingH
#define DBOnlineOrdeingH
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include "MM_DBCore.h"
#include "OnlineOrderingAttributes.h"
#include "DBOrder.h"

//---------------------------------------------------------------------------
struct TItemInfoKey
{
     long itemId;
     long itemSizeKey;
     long itemSizeIdentifierKey;
     TItemInfoKey(): itemId(0), itemSizeKey(0), itemSizeIdentifierKey(0){}
};
struct TPaymentInfo
{
    UnicodeString payType;
    double amount;
    TPaymentInfo(): payType(""), amount(0){}
};
class TDBOnlineOrdering
{
public:
    static TSiteMenuInfo GetMenuInfo(Database::TDBTransaction &dbTransaction);
    static TSiteTaxSettingsInfo GetTaxSettings(Database::TDBTransaction &dbTransaction);
    static UnicodeString GetOnlineOrderGUID(Database::TDBTransaction &DBTransaction);
    static void GetOrdersByOnlineOrderGUID(Database::TDBTransaction &DBTransaction,TList *Orders, UnicodeString orderUniqueId);
    static void SetOnlineOrderStatus(Database::TDBTransaction &DBTransaction, UnicodeString orderUniqueId);
    static int GetMemberKey(Database::TDBTransaction &dbTransaction, int orderKey);
    static int GetItemSizeKey(Database::TDBTransaction &dbTransaction, int itemKey, UnicodeString sizeName);
    static Currency GetHappyHourPrice(Database::TDBTransaction &dbTransaction, int itemsizeKey, int priceLevelKey);
    static void UpdateHappyHourPriceForItem(Database::TDBTransaction &dbTransaction, int itemKey, UnicodeString sizeName,
                    UnicodeString onlineOrderId, Currency hhPrice, Currency basePrice);
    static UnicodeString GetOnlineOrderGUIDForWaiterApp(Database::TDBTransaction &DBTransaction, bool IsDayArcBill);
    static void SetStatusAndSaveReceiptForWApp(Database::TDBTransaction &dbTransaction, UnicodeString orderUniqueIdForWaiterApp,TMemoryStream *ReceiptToArchive
                , bool IsDayArcBill);
    static std::list<TItemInfoKey> GetItemInfoKeyListFromOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID);
    static TMemoryStream* GetEFTPOSReceiptForOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID);
    static std::list<TPaymentInfo> GetPaymentInfoForOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID);
    static void LoadItemCompleteInfoForOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID,
                            TItemComplete *itemComplete, int itemSizeIdentifier);
    static UnicodeString GetTerminalNameForOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID);
    static UnicodeString GetInvoiceNumberForOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID);
    static void LoadSiteIDAndOOIdForOrderGuid(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID, int &siteID, int &onlineOrderID);
private:
    static std::list<TCourseInfo> GetCourseInfo(Database::TDBTransaction &dbTransaction, int menuKey);
    static std::list<TSiteItemInfo> GetItemInfo(Database::TDBTransaction &dbTransaction, int courseId);
    static std::list<TItemSizeInf> GetItemSizeInfo(Database::TDBTransaction &dbTransaction, int itemSizeKey);
    static std::list<TItemSizeTaxProfileInfo> GetItemSizeTaxProfileInfo(Database::TDBTransaction &dbTransaction, int itemSizeKey);
    static std::list<TSideGroupInfo> GetSideGroupInfo(Database::TDBTransaction &dbTransaction, int masterItemKey);
    static std::list<TItemSideInfo> GetItemSideInfo(Database::TDBTransaction &dbTransaction, int masterKey, int groupNo);
    static int GetItemIdByItemKey(Database::TDBTransaction &dbTransaction, int itemKey);
};

#endif


