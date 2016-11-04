//---------------------------------------------------------------------------

#ifndef SeniorCitizenDiscountCheckerH
#define SeniorCitizenDiscountCheckerH

#include <set>
#include "Discount.h"
#include "MM_DBCore.h"
//---------------------------------------------------------------------------

class TSeniorCitizenDiscountChecker
{
public:
    TSeniorCitizenDiscountChecker();

    bool SeniorCitizensCheck(TDiscount inDiscount, TList* Orders, bool isClippSale = false);
    bool PWDCheck(TDiscount inDiscount, TList* Orders, bool isClippSale = false);
    bool ItemSelectionCheck(Database::TDBTransaction &DBTransaction, __int64 OrderItemToCheckKey, std::set<__int64> SelectedOrderItems);
    bool ItemSelectionCheckPWD(Database::TDBTransaction &DBTransaction, __int64 OrderItemToCheckKey, std::set<__int64> SelectedOrderItems);

private:
    bool checkItemsHaveDiscount(Database::TDBTransaction &DBTransaction, std::set<__int64> SelectedOrderItems);
    bool checkItemsHaveSeniorCitizenDiscount(Database::TDBTransaction &DBTransaction, std::set<__int64> SelectedOrderItems);
    bool checkItemsHavePWDDiscount(Database::TDBTransaction &DBTransaction, std::set<__int64> OrderKeys);
    UnicodeString getOrderKeysList(std::set<__int64> SelectedOrderItems);
};
#endif
