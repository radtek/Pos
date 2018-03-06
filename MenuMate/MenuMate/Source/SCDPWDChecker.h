//---------------------------------------------------------------------------

#ifndef SCDPWDCheckerH
#define SCDPWDCheckerH

#include <set>
#include "PnMOrder.h"
#include "Discount.h"
#include "MM_DBCore.h"
#include "ItemMinorComplete.h"
//---------------------------------------------------------------------------

class TSCDPWDChecker
{
public:
    TSCDPWDChecker();

    bool SeniorCitizensCheck(TDiscount inDiscount, TList* Orders, bool isClippSale = false);
    bool PWDCheck(TDiscount inDiscount, TList* Orders, bool isClippSale = false);
    bool ItemSelectionCheck(Database::TDBTransaction &DBTransaction, __int64 OrderItemToCheckKey, std::set<__int64> SelectedOrderItems, bool showMessage = true);
    bool ItemSelectionCheckPWD(Database::TDBTransaction &DBTransaction, __int64 OrderItemToCheckKey, std::set<__int64> SelectedOrderItems, bool showMessage = true);
    bool SeniorCitizensCheckForItem(TDiscount inDiscount, TItemMinorComplete *SelectedItem);
    bool PWDCheckForItem(TDiscount inDiscount, TItemMinorComplete *SelectedItem);
    bool CanAddItemToSelectedList(Database::TDBTransaction &DBTransaction, TPnMOrder Order,std::map<__int64,TPnMOrder> SelectedOrders);
    bool CheckForBillingWithPWD(Database::TDBTransaction &DBTransaction,std::set <__int64> ItemsToBill);
private:
    bool checkItemsHaveSCDOrPWDDiscount(Database::TDBTransaction &DBTransaction, std::set<__int64> SelectedOrderItems, UnicodeString discountGroup);
    bool checkItemsHaveDiscount(Database::TDBTransaction &DBTransaction, std::set<__int64> SelectedOrderItems);
    UnicodeString getOrderKeysList(std::set<__int64> SelectedOrderItems);
    bool checkItemsHaveNormaliscount(Database::TDBTransaction &DBTransaction, std::set<__int64> OrderKeys);
};
#endif
