//---------------------------------------------------------------------------

#ifndef DBOnlineOrdeingH
#define DBOnlineOrdeingH
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include "MM_DBCore.h"
#include "OnlineOrderingAttributes.h"
//---------------------------------------------------------------------------
class TDBOnlineOrdering
{
public:
    static TSiteMenuInfo GetMenuInfo(Database::TDBTransaction &dbTransaction);
private:
    static std::list<TCourseInfo> GetCourseInfo(Database::TDBTransaction &dbTransaction, int menuKey);
    static std::list<TSiteItemInfo> GetItemInfo(Database::TDBTransaction &dbTransaction, int courseId);
    static std::list<TItemSizeInfo> GetItemSizeInfo(Database::TDBTransaction &dbTransaction, int itemSizeKey);
    static std::list<TItemSizeTaxProfileInfo> GetItemSizeTaxProfileInfo(Database::TDBTransaction &dbTransaction, int itemSizeKey);
    static std::list<TSideGroupInfo> GetSideGroupInfo(Database::TDBTransaction &dbTransaction, int masterItemKey);
    static std::list<TItemSideInfo> GetItemSideInfo(Database::TDBTransaction &dbTransaction, int masterKey, int groupNo);
};
#endif


