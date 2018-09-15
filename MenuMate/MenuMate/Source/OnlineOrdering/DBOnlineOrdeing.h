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
    static UnicodeString GetOnlineOrderId(Database::TDBTransaction &DBTransaction);
    static void GetOrdersByOnlineOrderId(Database::TDBTransaction &DBTransaction,TList *Orders, UnicodeString orderUniqueId);
    static void SetOnlineOrderStatus(Database::TDBTransaction &DBTransaction, UnicodeString orderUniqueId);
    static int GetMemberKey(Database::TDBTransaction &dbTransaction, int orderKey);
    static UnicodeString GetChitTypeByOnlineOrderId(Database::TDBTransaction &dbTransaction, UnicodeString onlineOrderId);
private:
    static std::list<TCourseInfo> GetCourseInfo(Database::TDBTransaction &dbTransaction, int menuKey);
    static std::list<TSiteItemInfo> GetItemInfo(Database::TDBTransaction &dbTransaction, int courseId);
    static std::list<TItemSizeInf> GetItemSizeInfo(Database::TDBTransaction &dbTransaction, int itemSizeKey);
    static std::list<TItemSizeTaxProfileInfo> GetItemSizeTaxProfileInfo(Database::TDBTransaction &dbTransaction, int itemSizeKey);
    static std::list<TSideGroupInfo> GetSideGroupInfo(Database::TDBTransaction &dbTransaction, int masterItemKey);
    static std::list<TItemSideInfo> GetItemSideInfo(Database::TDBTransaction &dbTransaction, int masterKey, int groupNo);
};
#endif


