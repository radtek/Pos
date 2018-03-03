//---------------------------------------------------------------------------

#ifndef SCDPWDCheckerH
#define SCDPWDCheckerH

#include <set>
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

private:
    bool checkItemsHaveDiscount(Database::TDBTransaction &DBTransaction, std::set<__int64> SelectedOrderItems);
    bool checkItemsHaveSCDOrPWDDiscount(Database::TDBTransaction &DBTransaction, std::set<__int64> SelectedOrderItems, UnicodeString discountGroup);
    UnicodeString getOrderKeysList(std::set<__int64> SelectedOrderItems);
};
#endif
