//---------------------------------------------------------------------------


#pragma hdrstop

#include "SeniorCitizenDiscountChecker.h"
#include "MMMessageBox.h"
#include "OrderUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TSeniorCitizenDiscountChecker::TSeniorCitizenDiscountChecker()
{
}

bool TSeniorCitizenDiscountChecker::SeniorCitizensCheck(TDiscount inDiscount, TList* Orders, bool isClippSale)
{
    bool discIsSeniorCitizenDisc = inDiscount.IsSeniorCitizensDiscount();
    bool SeniorCitizenDiscCanBeApplied = !TOrderUtils::AnyDiscountsApplied(Orders) || TOrderUtils::SeniorCitizensDiscountApplied(Orders);

    bool discountValid = discIsSeniorCitizenDisc ? SeniorCitizenDiscCanBeApplied : !TOrderUtils::SeniorCitizensDiscountApplied(Orders);

    if(isClippSale && discIsSeniorCitizenDisc)
    {
       discountValid = isClippSale;
    }
    if(!discountValid)
    {
        UnicodeString errorMessage;

        if(discIsSeniorCitizenDisc)
        {
            errorMessage = "Can not apply a Senior Citizens Discount to a sale with a Non Senior Citizens Discount applied.";
        }
        else
        {
            errorMessage = "Can not apply a Non Senior Citizens Discount to a sale with a Senior Citizens Discount applied.";
        }

		MessageBox(errorMessage, "Error", MB_ICONWARNING + MB_OK);
    }

    if(isClippSale && !discIsSeniorCitizenDisc)
    {
       discountValid = false;
    }

    return discountValid;
}

bool TSeniorCitizenDiscountChecker::ItemSelectionCheck(Database::TDBTransaction &DBTransaction, __int64 OrderItemToCheckKey, std::set<__int64> SelectedOrderItems)
{
    bool retVal = true;
    std::set<__int64> keyToCheck;
    keyToCheck.insert(OrderItemToCheckKey);

    if(SelectedOrderItems.size() > 0 && checkItemsHaveDiscount(DBTransaction, SelectedOrderItems) && checkItemsHaveDiscount(DBTransaction, keyToCheck))
    {
        retVal = checkItemsHaveSeniorCitizenDiscount(DBTransaction, SelectedOrderItems) == checkItemsHaveSeniorCitizenDiscount(DBTransaction, keyToCheck);
    }

    if(!retVal)
    {
        MessageBox("Items with Senior Citizens Discounts and items with Non Senior Citizens Discounts can not be billed at the same time.", "Error", MB_ICONWARNING + MB_OK);
    }

    return retVal;
}

bool TSeniorCitizenDiscountChecker::checkItemsHaveDiscount(Database::TDBTransaction &DBTransaction, std::set<__int64> OrderKeys)
{
    UnicodeString orderKeysList = getOrderKeysList(OrderKeys);

    std::set<int> orderKeyContainer;
    std::set<int> orderdiscountsContainer;

    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Clear();
    IBInternalQuery->SQL->Text =
        "SELECT "
            "O.ORDER_KEY "
        "FROM "
            "ORDERS O " ;
        IBInternalQuery->ExecQuery();

       for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
       {
           orderKeyContainer.insert(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger);
       }

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Clear();
    IBInternalQuery->SQL->Text =
        "SELECT "
            "Od.ORDER_KEY "
        "FROM "
            "ORDERDISCOUNTS Od " ;
     IBInternalQuery->ExecQuery();

    for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
    {
       orderdiscountsContainer.insert(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger);
    }

    std::set<__int64>::iterator orderKeysIt = OrderKeys.begin();

    for(; orderKeysIt != OrderKeys.end(); orderKeysIt++)
    {
        int orderKey =  *orderKeysIt;

      if( orderKeyContainer.find(orderKey)!= orderKeyContainer.end() && (orderdiscountsContainer.find(orderKey)!= orderdiscountsContainer.end()) )
      {
          return true;
      }

    }
    return false;

}

bool TSeniorCitizenDiscountChecker::checkItemsHaveSeniorCitizenDiscount(Database::TDBTransaction &DBTransaction, std::set<__int64> OrderKeys)
{
    UnicodeString orderKeysList = getOrderKeysList(OrderKeys);

    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Clear();
    IBInternalQuery->SQL->Text =
        "SELECT "
            "O.ORDER_KEY "
        "FROM "
            "ORDERS O INNER JOIN ORDERDISCOUNTS OD "
            "ON O.ORDER_KEY = OD.ORDER_KEY "
            "INNER JOIN DISCOUNTGROUPS_DISCOUNTTYPES DGD "
            "ON OD.DISCOUNT_KEY = DGD.DISCOUNTTYPE_KEY "
            "INNER JOIN DISCOUNT_GROUPS DG "
            "ON DGD.DISCOUNTGROUPS_KEY = DG.DISCOUNTGROUPS_KEY "
        "WHERE "
            "O.ORDER_KEY IN (" + orderKeysList + ") "
        "AND "
            "DG.DISCOUNTGROUP_NAME = 'Senior Citizen';";
    IBInternalQuery->ExecQuery();

    return !IBInternalQuery->Eof;
}

UnicodeString TSeniorCitizenDiscountChecker::getOrderKeysList(std::set<__int64> OrderKeys)
{
    std::set<__int64>::iterator orderKeysIt = OrderKeys.begin();
    UnicodeString orderKeyList = IntToStr(*orderKeysIt);
    orderKeysIt++;
    for(; orderKeysIt != OrderKeys.end(); orderKeysIt++)
    {
        orderKeyList += ", " + IntToStr(*orderKeysIt);
    }
    return orderKeyList;
}
