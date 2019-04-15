//---------------------------------------------------------------------------

#ifndef DBOnlineOrdeingH
#define DBOnlineOrdeingH
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include "MM_DBCore.h"
#include "OnlineOrderingAttributes.h"
#include "DBOrder.h"

//---------------------------------------------------------------------------
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
    static UnicodeString GetOnlineOrderGUIDForWaiterApp(Database::TDBTransaction &DBTransaction);
    static void SetOnlineOrderStatusForWaiterApp(Database::TDBTransaction &dbTransaction, UnicodeString orderUniqueIdForWaiterApp);
    static void GetItemKeysFromOrderGUID(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID, std::vector<UnicodeString>* sizeNameList, std::vector<int>* itemKeysList);
    static long GetItemSizeKeyFromItemKeyAndSizeName(Database::TDBTransaction &dbTransaction, UnicodeString sizeName, int itemKey);


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


