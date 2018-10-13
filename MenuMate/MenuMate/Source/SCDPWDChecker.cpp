//---------------------------------------------------------------------------


#pragma hdrstop

#include "SCDPWDChecker.h"
#include "MMMessageBox.h"
#include "OrderUtils.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TSCDPWDChecker::TSCDPWDChecker()
{
}

bool TSCDPWDChecker::SeniorCitizensCheck(TDiscount inDiscount, TList* Orders, bool isClippSale)
{
    bool discIsSeniorCitizenDisc = inDiscount.IsSeniorCitizensDiscount();
    bool SeniorCitizenDiscCanBeApplied = !TOrderUtils::AnyDiscountsApplied(Orders) ||
                                         (TOrderUtils::SeniorCitizensDiscountApplied(Orders));
    bool discountValid = discIsSeniorCitizenDisc ? SeniorCitizenDiscCanBeApplied : !TOrderUtils::SeniorCitizensDiscountApplied(Orders);
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
    return discountValid;
}
bool TSCDPWDChecker::PWDCheck(TDiscount inDiscount, TList* Orders, bool isClippSale)
{
    bool discIsPWDDisc = inDiscount.IsPersonWithDisabilityDiscount();
    bool PWDDiscCanBeApplied = !TOrderUtils::AnyDiscountsApplied(Orders) || TOrderUtils::PWDDiscountApplied(Orders);
    bool discountValid = discIsPWDDisc ? PWDDiscCanBeApplied : !TOrderUtils::PWDDiscountApplied(Orders);
    if(!discountValid)
    {
        UnicodeString errorMessage;

        if(discIsPWDDisc)
        {
            errorMessage = "Can not apply a PWD Discount to a sale with a non PWD Discount applied.";
        }
        else
        {
            errorMessage = "Can not apply a Non PWD Discount to a sale with a PWD Discount applied.";
        }

		MessageBox(errorMessage, "Error", MB_ICONWARNING + MB_OK);
    }
    return discountValid;
}
bool TSCDPWDChecker::ItemSelectionCheck(Database::TDBTransaction &DBTransaction, __int64 OrderItemToCheckKey, std::set<__int64> SelectedOrderItems, bool showMessage)
{
    bool retVal = true;
    bool itemHasDiscount = true;
    bool itemHasSCDDiscount = true;
    std::set<__int64> keyToCheck;
    keyToCheck.insert(OrderItemToCheckKey);

    if(showMessage)
    {
        itemHasDiscount = checkItemsHaveDiscount(DBTransaction, keyToCheck);
        itemHasSCDDiscount = checkItemsHaveSCDOrPWDDiscount(DBTransaction, keyToCheck, "Senior Citizen");
    }

    if(SelectedOrderItems.size() > 0 && itemHasDiscount && checkItemsHaveDiscount(DBTransaction, SelectedOrderItems))
    {
        retVal = checkItemsHaveSCDOrPWDDiscount(DBTransaction, SelectedOrderItems, "Senior Citizen") == itemHasSCDDiscount;
    }

    if(!retVal && showMessage)
    {
        MessageBox("Items with Senior Citizens Discounts and items with Non Senior Citizens Discounts can not be billed at the same time.", "Error", MB_ICONWARNING + MB_OK);
    }

    return retVal;
}
bool TSCDPWDChecker::ItemSelectionCheckPWD(Database::TDBTransaction &DBTransaction, __int64 OrderItemToCheckKey, std::set<__int64> SelectedOrderItems, bool showMessage)
{
    bool retVal = true;
    bool itemHasDiscount = true;
    bool itemHasPWDDiscount = true;
    std::set<__int64> keyToCheck;
    keyToCheck.insert(OrderItemToCheckKey);

    if(showMessage)
    {
        itemHasDiscount = checkItemsHaveDiscount(DBTransaction, keyToCheck);
        itemHasPWDDiscount = checkItemsHaveSCDOrPWDDiscount(DBTransaction, keyToCheck, "Person with Disability");
    }

    if(SelectedOrderItems.size() > 0 && itemHasDiscount && checkItemsHaveDiscount(DBTransaction, SelectedOrderItems))
    {
        retVal = checkItemsHaveSCDOrPWDDiscount(DBTransaction, SelectedOrderItems, "Person with Disability") == itemHasPWDDiscount;
    }

    if(!retVal && showMessage)
    {
        MessageBox("Items with PWD Discounts and items with Non PWD Discounts can not be billed at the same time.", "Error", MB_ICONWARNING + MB_OK);
    }

    return retVal;
}

bool TSCDPWDChecker::checkItemsHaveDiscount(Database::TDBTransaction &DBTransaction, std::set<__int64> OrderKeys)
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

bool TSCDPWDChecker::checkItemsHaveSCDOrPWDDiscount(Database::TDBTransaction &DBTransaction, std::set<__int64> OrderKeys, UnicodeString discountGroup)
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
            "DG.DISCOUNTGROUP_NAME = :DISCOUNTGROUP_NAME;";

    IBInternalQuery->ParamByName("DISCOUNTGROUP_NAME")->AsString = discountGroup;
    IBInternalQuery->ExecQuery();

    return !IBInternalQuery->Eof;
}

UnicodeString TSCDPWDChecker::getOrderKeysList(std::set<__int64> OrderKeys)
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
bool TSCDPWDChecker::SeniorCitizensCheckForItem(TDiscount inDiscount, TItemMinorComplete *SelectedItem)
{
    bool discIsSeniorCitizenDisc = inDiscount.IsSeniorCitizensDiscount();
    bool SeniorCitizenDiscCanBeApplied = (SelectedItem->HasSeniorCitizensDiscountApplied() ||(SelectedItem->Discounts.size() == 0))
                                      && ( SidesWithoutCounterDiscount(SelectedItem,discIsSeniorCitizenDisc));
    bool NonSCDCanBeApplied = (!SelectedItem->HasSeniorCitizensDiscountApplied() &&
                               ( SidesWithoutCounterDiscount(SelectedItem,discIsSeniorCitizenDisc))) ;
    bool discountValid = discIsSeniorCitizenDisc ? SeniorCitizenDiscCanBeApplied : NonSCDCanBeApplied;
    if(!discountValid)
    {
        UnicodeString errorMessage = "";
        if(discIsSeniorCitizenDisc)
            errorMessage = "Can not apply a Senior Citizens Discount to an item with a Non Senior Citizens Discount applied.";
        else
            errorMessage = "Can not apply a Non Senior Citizens Discount to an item with a Senior Citizens Discount applied.";
		MessageBox(errorMessage, "Error", MB_ICONWARNING + MB_OK);
    }
    return discountValid;
}
//-----------------------------------------------------------------------------
bool TSCDPWDChecker::SidesWithoutCounterDiscount(TItemMinorComplete *SelectedItem,bool discIsSeniorCitizenDisc)
{
    bool retValue = true;
    if(discIsSeniorCitizenDisc)
    {
        for(int i = 0; i < SelectedItem->SubOrders->Count; i++)
        {
            TItemComplete *subItem = (TItemComplete*)SelectedItem->SubOrders->Items[i];
            if(subItem->Discounts.size() != 0)
            {
                if(!subItem->HasSeniorCitizensDiscountApplied())
                {
                    retValue = false;
                    break;
                }
            }
        }
    }
    else
    {
        for(int i = 0; i < SelectedItem->SubOrders->Count; i++)
        {
            TItemComplete *subItem = (TItemComplete*)SelectedItem->SubOrders->Items[i];
            if(subItem->Discounts.size() != 0)
            {
                if(subItem->HasSeniorCitizensDiscountApplied())
                {
                    retValue = false;
                    break;
                }
            }
        }
    }
    return retValue;
}
//-----------------------------------------------------------------------------
bool TSCDPWDChecker::SidesWithoutCounterSCD(TList *Orders,bool discIsSeniorCitizenDisc)
{
    bool retValue = true;
    for(int orderIndex = 0; orderIndex < Orders->Count; orderIndex++)
    {
        TItemMinorComplete *SelectedItem = (TItemMinorComplete*)Orders->Items[orderIndex];
        if(!SidesWithoutCounterDiscount(SelectedItem,discIsSeniorCitizenDisc))
        {
            retValue = false;
            break;
        }
    }
    return retValue;
}
//-----------------------------------------------------------------------------
bool TSCDPWDChecker::SidesWithoutCounterPWDDiscount(TItemMinorComplete *SelectedItem,bool discIsPWDDisc)
{
    bool retValue = true;
    if(discIsPWDDisc)
    {
        for(int i = 0; i < SelectedItem->SubOrders->Count; i++)
        {
            TItemComplete *subItem = (TItemComplete*)SelectedItem->SubOrders->Items[i];
            if(subItem->Discounts.size() != 0)
            {
                if(!subItem->HasPWDApplied())
                {
                    retValue = false;
                    break;
                }
            }
        }
    }
    else
    {
        for(int i = 0; i < SelectedItem->SubOrders->Count; i++)
        {
            TItemComplete *subItem = (TItemComplete*)SelectedItem->SubOrders->Items[i];
            if(subItem->Discounts.size() != 0)
            {
                if(subItem->HasPWDApplied())
                {
                    retValue = false;
                    break;
                }
            }
        }
    }
    return retValue;
}
//-----------------------------------------------------------------------------
bool TSCDPWDChecker::PWDCheckForItem(TDiscount inDiscount, TItemMinorComplete *SelectedItem)
{
    bool discIsPWDDisc = inDiscount.IsPersonWithDisabilityDiscount();
    bool PWDDiscCanBeApplied = ((SelectedItem->HasPWDApplied() ||(SelectedItem->Discounts.size() == 0)) &&
                                SidesWithoutCounterPWDDiscount(SelectedItem,discIsPWDDisc)) ;
    bool NonPWDCanBeApplied = !SelectedItem->HasPWDApplied() && SidesWithoutCounterPWDDiscount(SelectedItem,discIsPWDDisc);
    bool discountValid = discIsPWDDisc ? PWDDiscCanBeApplied : NonPWDCanBeApplied;

    if(!discountValid)
    {
        UnicodeString errorMessage = "";
        if(discIsPWDDisc)
            errorMessage = "Can not apply a PWD Discount to an item with a Non PWD Discount applied.";
        else
            errorMessage = "Can not apply a Non PWD Discount to an item with a PWD Discount applied.";

		MessageBox(errorMessage, "Error", MB_ICONWARNING + MB_OK);
    }
    return discountValid;
}
//---------------------------------------------------------------------------
bool TSCDPWDChecker::checkItemsHaveNormaliscount(Database::TDBTransaction &DBTransaction, std::set<__int64> OrderKeys)
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
            "DG.DISCOUNTGROUP_NAME <> :DISCOUNTGROUP_NAME1 AND "
            "DG.DISCOUNTGROUP_NAME <> :DISCOUNTGROUP_NAME2;";

    IBInternalQuery->ParamByName("DISCOUNTGROUP_NAME1")->AsString = "Person with Disability";
    IBInternalQuery->ParamByName("DISCOUNTGROUP_NAME2")->AsString = "Senior Citizen";
    IBInternalQuery->ExecQuery();

    return !IBInternalQuery->Eof;
}
//---------------------------------------------------------------------------
bool TSCDPWDChecker::CanAddItemToSelectedList(Database::TDBTransaction &DBTransaction,
                           TPnMOrder Order,std::map<__int64,TPnMOrder> SelectedOrders)
{
    bool retValue = true;
    std::map<__int64,TPnMOrder>::iterator itMap = SelectedOrders.begin();
    std::set<__int64> keyToCheck;

    for(;itMap != SelectedOrders.end(); advance(itMap,1))
    {
        keyToCheck.clear();
        keyToCheck.insert(Order.Key);
        if(checkItemsHaveDiscount(DBTransaction, keyToCheck))
        {
            if(checkItemsHaveSCDOrPWDDiscount(DBTransaction, keyToCheck, "Senior Citizen") || checkItemsHaveNormaliscount(DBTransaction, keyToCheck))
            {
                retValue = false;
                break;
            }
        }
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TSCDPWDChecker::CheckForBillingWithPWD(Database::TDBTransaction &DBTransaction,std::set <__int64> ItemsToBill)
{
    bool retValue = true;
    retValue = !(checkItemsHaveSCDOrPWDDiscount(DBTransaction, ItemsToBill, "Person with Disability") &&
                 (checkItemsHaveSCDOrPWDDiscount(DBTransaction, ItemsToBill, "Senior Citizen") ||
                  checkItemsHaveNormaliscount(DBTransaction, ItemsToBill)));
    return retValue;
}
//---------------------------------------------------------------------------
