//---------------------------------------------------------------------------


#pragma hdrstop
#include "DBOrder.h"
#include "SCDPatronUtility.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "ManagerDiscount.h"
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
extern int OptionsCompare(TListOptionContainer * Options1, TListOptionContainer * Options2);
extern int __fastcall SubOrdersCompare(TListSubOrderContainer * List1, TListSubOrderContainer * List2);
//---------------------------------------------------------------------------
TSCDPatronUtility::TSCDPatronUtility()
{
}
//---------------------------------------------------------------------------
TSCDPatronUtility::~TSCDPatronUtility()
{
}
//---------------------------------------------------------------------------
bool TSCDPatronUtility::CheckBillRestructureRequiredAfterPatron(TPaymentTransaction paymentTransaction)
{
    return CheckPatronsAvailable(paymentTransaction) && CheckDiscountsAvailable(paymentTransaction);
}
//---------------------------------------------------------------------------
void TSCDPatronUtility::MergeItems(TPaymentTransaction &paymentTransaction)
{
    std::vector<int> indexToBeExcluded;
    indexToBeExcluded.clear();
    std::auto_ptr <TList> formattedOrders(new TList);
    for(int index = 0; index < paymentTransaction.Orders->Count; index++)
    {
       TItemComplete *ModifyingItem = new TItemComplete;
       TItemComplete *currentOrder = (TItemComplete *)paymentTransaction.Orders->Items[index];
       currentOrder->Assign(ModifyingItem);

       if(!ModifyingItem->WeightedPrice.WeightedItem &&
          (std::find(indexToBeExcluded.begin(), indexToBeExcluded.end(), index) == indexToBeExcluded.end()))
          AddIfPossible(ModifyingItem, paymentTransaction,indexToBeExcluded,index);

       if(ModifyingItem->GetQty() != 0 && (std::find(indexToBeExcluded.begin(), indexToBeExcluded.end(), index) == indexToBeExcluded.end()))
        formattedOrders->Add(ModifyingItem);
    }
    paymentTransaction.Orders->Clear();
    for(int i = 0; i < formattedOrders->Count; i++)
    {
        TItemComplete *ModifyingItem = (TItemComplete *)formattedOrders->Items[i];
        paymentTransaction.Orders->Add(ModifyingItem);
    }
    paymentTransaction.Money.Recalc(paymentTransaction);
}
//---------------------------------------------------------------------------
void TSCDPatronUtility::AddIfPossible(TItemComplete *oldOrder,TPaymentTransaction &paymentTransaction,std::vector<int> &indexToBeExcluded,int oldIndex)
{
    for(int index2 = 0; index2 < paymentTransaction.Orders->Count; index2++)
    {
        TItemComplete *itemNow = (TItemComplete *)paymentTransaction.Orders->Items[index2];
        if(oldIndex != index2)
        {
            if(isSameItemFromInitial(oldOrder,itemNow))
            {
                if(!itemNow->WeightedPrice.WeightedItem)
                {
                    if (std::find(indexToBeExcluded.begin(), indexToBeExcluded.end(), index2) == indexToBeExcluded.end())
                    {
                        double newQty = oldOrder->GetQty() + itemNow->GetQty();
                        oldOrder->SetQtyCustom(newQty);
                        newQty = itemNow->GetQty() - itemNow->GetQty();
                        itemNow->SetQtyCustom(newQty);
                        indexToBeExcluded.push_back(index2);
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
bool TSCDPatronUtility::isSameItemFromInitial(TItemComplete *oldOrder,TItemComplete *newOrder)
{
    bool retValue = false;

    retValue = (oldOrder->ItemKitchenName == newOrder->ItemKitchenName && oldOrder->ItemKey == newOrder->ItemKey) && (
    (OptionsCompare(oldOrder->OptionsSelected, newOrder->OptionsSelected) != 0) 	// Options Differ
  	&& (SubOrdersCompare(oldOrder->SubOrders, newOrder->SubOrders) != 0) 			// SubOrders Differ
	&& (oldOrder->Note == newOrder->Note)
	&& (oldOrder->SetMenuGroup == newOrder->SetMenuGroup)
	&& (oldOrder->MasterContainer == newOrder->MasterContainer)
	&& (oldOrder->Size == newOrder->Size));

	return retValue;
}
//---------------------------------------------------------------------------
void TSCDPatronUtility::DivideBill(TPaymentTransaction &paymentTransaction,
                                   std::auto_ptr <TList> &SCDOrders,std::auto_ptr <TList> &NormalOrders)
{
    try
    {
        TItemComplete *SplittedItem = new TItemComplete;
        std::auto_ptr<TManagerDiscount> managerDiscount(new TManagerDiscount());
        managerDiscount->Initialise();
        std::map<int,TDiscount> discountMap;
        discountMap.clear();
//        MessageBox("Inside Divide Bill","",MB_OK);
        for(int indexOrders = 0; indexOrders < paymentTransaction.Orders->Count; indexOrders++)
        {
            TItemComplete *ic = (TItemComplete*)paymentTransaction.Orders->Items[indexOrders];
            for(int itemDiscIndex = 0; itemDiscIndex < ic->Discounts.size(); itemDiscIndex++)
            {
                discountMap.insert(std::pair<int,TDiscount>(ic->Discounts[itemDiscIndex].DiscountKey,ic->Discounts[itemDiscIndex]));
            }
            for(int indexSides = 0; indexSides < ic->SubOrders->Count; indexSides++)
            {
                TItemComplete *side = (TItemComplete*)ic->SubOrders->Items[indexSides];
                for(int sideDiscIndex = 0; sideDiscIndex < side->Discounts.size(); sideDiscIndex++)
                {
                    discountMap.insert(std::pair<int,TDiscount>(side->Discounts[sideDiscIndex].DiscountKey,side->Discounts[sideDiscIndex]));
                }
                side->ClearAllDiscounts();
                side->SplitMessage = "";
            }
            ic->ClearAllDiscounts();
            ic->SplitMessage = "";
        }

        managerDiscount->ClearDiscounts(paymentTransaction.Orders);
        // Merge Again
        MergeItems(paymentTransaction);
        TDBOrder::SortOrders(paymentTransaction.Orders);
        //Merged

        Currency amountSCD = GetAmountForSCD(paymentTransaction);

        for(int i = 0; i < paymentTransaction.Orders->Count; i++)
        {
            TItemComplete *Order = (TItemComplete *)paymentTransaction.Orders->Items[i];
            Currency OrderPrice =  Order->BillCalcResult.FinalPrice;
            Currency subOrderPrice = 0;
            for(int subOrderIndex = 0; subOrderIndex < Order->SubOrders->Count; subOrderIndex++)
            {
                TItemComplete *subOrder = (TItemComplete *)Order->SubOrders->Items[subOrderIndex];
                subOrderPrice += subOrder->BillCalcResult.FinalPrice;
            }
            OrderPrice += subOrderPrice;
            OrderPrice = RoundToNearest(OrderPrice, 0.01,TGlobalSettings::Instance().MidPointRoundsDown);
            amountSCD = RoundToNearest(amountSCD,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
            if(amountSCD > 0)
            {
                if(amountSCD < OrderPrice)
                {
                    double amountSCD_double = double(amountSCD.Val);
                    double OrderPrice_double = double(OrderPrice.Val);
                    double SplitPercentage = amountSCD_double/OrderPrice_double;
                    Order->Assign(SplittedItem);
                    Order->TransNo = TDBOrder::GetNextTransNumber(paymentTransaction.DBTransaction);
                    TDBOrder::AssignNewSecurityRef(paymentTransaction.DBTransaction,SplittedItem);
                    double newQuantity = SplittedItem->GetQty() * (1 - SplitPercentage);
                    SplittedItem->SetQty(newQuantity,false);
                    TDBOrder::DeductOrder(Order, SplittedItem, false);
                    Order->SplitMessage = "Portion for SCD";
                    SCDOrders->Add(Order);
                    SplittedItem->SplitMessage = "";
                    NormalOrders->Add(SplittedItem);
                }
                else
                {
                    Order->SplitMessage = "Portion for SCD";
                    SCDOrders->Add(Order);
                }
            }
            else
            {
                Order->SplitMessage = "";
                NormalOrders->Add(Order);
            }
            amountSCD -= OrderPrice;
        }

        std::map<int,TDiscount>::iterator itdiscMapNow = discountMap.begin();
        for(;itdiscMapNow != discountMap.end();advance(itdiscMapNow,1))
        {
            TDiscount CurrentDiscount = itdiscMapNow->second;
            bool skipGetDiscount = IsCurrentDiscountOpenType(CurrentDiscount);
            if(skipGetDiscount)
            {
                CurrentDiscount.Amount = CurrentDiscount.OriginalAmount;
            }
            managerDiscount->ClearDiscount(SCDOrders.get(), CurrentDiscount);
            managerDiscount->ClearDiscount(NormalOrders.get(), CurrentDiscount);
            if(!skipGetDiscount)
                managerDiscount->GetDiscount(paymentTransaction.DBTransaction,CurrentDiscount.DiscountKey, CurrentDiscount);
            if(CurrentDiscount.IsSeniorCitizensDiscount())
            {
                managerDiscount->AddDiscount(SCDOrders.get(),CurrentDiscount);
            }
            else
            {
                managerDiscount->AddDiscount(NormalOrders.get(),CurrentDiscount);
            }
        }
    }
    catch(Exception &Ex)
    {
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
        MessageBox(Ex.Message,"Error",MB_OK + MB_ICONERROR);
		throw;
    }
}
//---------------------------------------------------------------------------
bool TSCDPatronUtility::IsCurrentDiscountOpenType(TDiscount CurrentDiscount)
{
    bool retValue = false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Clear();
        IBInternalQuery->SQL->Text =
            "SELECT a.DISCOUNT_TYPE FROM DISCOUNTS a WHERE a.DISCOUNT_KEY = :DISCOUNT_KEY ";

        IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = CurrentDiscount.DiscountKey;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
            if(IBInternalQuery->FieldByName("DISCOUNT_TYPE")->AsInteger != 0)
                retValue = true;
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        retValue = false;
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
        MessageBox(ex.Message,"Error",MB_OK + MB_ICONERROR);
		throw;
    }
    return retValue;
}
//---------------------------------------------------------------------------
//bool TSCDPatronUtility::CanByPassSCDValidity(TPaymentTransaction paymentTransaction, TDiscount CurrentDiscount)
bool TSCDPatronUtility::CanByPassSCDValidity(TList *Orders, std::vector<TPatronType> patrons, TDiscount CurrentDiscount)
{
    bool retValue = false;
    double SCD = 0;
    double nonSCD = 0;
    //if(IsCounterDiscountAvailable(paymentTransaction, CurrentDiscount))
    if(IsCounterDiscountAvailable(Orders,CurrentDiscount))
    {
        GetPatronDistribution(patrons,SCD,nonSCD);
        if(SCD != 0 && nonSCD != 0)
        {
            retValue = true;
        }
    }
    return retValue;
}
//---------------------------------------------------------------------------
//bool TSCDPatronUtility::IsCounterDiscountAvailable(TPaymentTransaction paymentTransaction, TDiscount CurrentDiscount)
bool TSCDPatronUtility::IsCounterDiscountAvailable(TList *Orders, TDiscount CurrentDiscount)
{
    bool retValue = false;
    bool isCurrentDiscountSCD = CurrentDiscount.IsSeniorCitizensDiscount();
    bool isSCDAppliedAlready = false;
    bool isNonSCDAppliedAlready = false;

    for(int indexOrders = 0; indexOrders < Orders->Count; indexOrders++)
    {
        TItemComplete *ic = (TItemComplete*)Orders->Items[indexOrders];
        for(int itemDiscIndex = 0; itemDiscIndex < ic->Discounts.size(); itemDiscIndex++)
        {
            if(ic->Discounts[itemDiscIndex].IsSeniorCitizensDiscount())
                isSCDAppliedAlready = true;
            else if(!ic->Discounts[itemDiscIndex].IsSeniorCitizensDiscount())
               isNonSCDAppliedAlready = true;
        }
    }

    if(isCurrentDiscountSCD)
    {
        if(isNonSCDAppliedAlready)
            retValue = true;
    }
    else
    {
        if(isSCDAppliedAlready)
            retValue = true;
    }

    return retValue;
}
//---------------------------------------------------------------------------
bool TSCDPatronUtility::RestructureBill(TPaymentTransaction &paymentTransaction)
{
    bool retValue = false;
    double nonSCD = 0;
    double SCD = 0;
    try
    {
        std::auto_ptr <TList> SCDOrders(new TList);
        std::auto_ptr <TList> NormalOrders(new TList);
        if(CheckBillRestructureRequiredAfterPatron(paymentTransaction))
        {
            DivideBill(paymentTransaction,SCDOrders,NormalOrders);

            paymentTransaction.Orders->Clear();

            for(int j = 0; j < SCDOrders->Count; j++)
            {
                TItemComplete *Order = (TItemComplete *)SCDOrders->Items[j];
                paymentTransaction.Orders->Add(Order);
            }
            for(int k = 0; k < NormalOrders->Count; k++)
            {
                TItemComplete *Order = (TItemComplete *)NormalOrders->Items[k];
                paymentTransaction.Orders->Add(Order);
            }
            paymentTransaction.Money.Recalc(paymentTransaction);
            retValue = true;
        }
        else
        {
            std::auto_ptr<TManagerDiscount> managerDiscount(new TManagerDiscount());
            GetPatronDistribution(paymentTransaction.Patrons,nonSCD,SCD);
            if(nonSCD == 0 || SCD == 0)
            {
                std::map<int,TDiscount> discountMap;
                discountMap.clear();
                bool isSCDApplied = false;
                bool isNormalApplied = false;
                for(int indexOrders = 0; indexOrders < paymentTransaction.Orders->Count; indexOrders++)
                {
                    TItemComplete *ic = (TItemComplete*)paymentTransaction.Orders->Items[indexOrders];
                    for(int itemDiscIndex = 0; itemDiscIndex < ic->Discounts.size(); itemDiscIndex++)
                    {
                        if(ic->Discounts[itemDiscIndex].IsSeniorCitizensDiscount() && !isSCDApplied)
                           isSCDApplied = true;
                        if(!ic->Discounts[itemDiscIndex].IsSeniorCitizensDiscount() && !isNormalApplied)
                           isNormalApplied = true;

                        discountMap.insert(std::pair<int,TDiscount>(ic->Discounts[itemDiscIndex].DiscountKey,ic->Discounts[itemDiscIndex]));
                    }
                    for(int indexSides = 0; indexSides < ic->SubOrders->Count; indexSides++)
                    {
                        TItemComplete *side = (TItemComplete*)ic->SubOrders->Items[indexSides];
                        for(int sideDiscIndex = 0; sideDiscIndex < side->Discounts.size(); sideDiscIndex++)
                        {
                            if(side->Discounts[sideDiscIndex].IsSeniorCitizensDiscount() && !isSCDApplied)
                               isSCDApplied = true;
                            if(!side->Discounts[sideDiscIndex].IsSeniorCitizensDiscount() && !isNormalApplied)
                               isNormalApplied = true;

                            discountMap.insert(std::pair<int,TDiscount>(side->Discounts[sideDiscIndex].DiscountKey,side->Discounts[sideDiscIndex]));
                        }
                        side->ClearAllDiscounts();
                        side->SplitMessage = "";
                    }
                    ic->ClearAllDiscounts();
                }

                if(isSCDApplied && SCD == 0)
                    MessageBox("Senior Citizens Discount will be removed as patron count is set to 0","Information",MB_OK + MB_ICONINFORMATION);
                if(isNormalApplied && nonSCD == 0)
                    MessageBox("Non Senior Citizens Discount will be removed as patron count is set to 0","Information",MB_OK + MB_ICONINFORMATION);

                std::map<int,TDiscount>::iterator itMap = discountMap.begin();
                for(;itMap != discountMap.end();advance(itMap,1))
                {
                   if(itMap->second.IsSeniorCitizensDiscount() && SCD != 0)
                      managerDiscount->AddDiscount(paymentTransaction.Orders,itMap->second);
                   if(!itMap->second.IsSeniorCitizensDiscount() && nonSCD != 0)
                      managerDiscount->AddDiscount(paymentTransaction.Orders,itMap->second);
                }
                retValue = true;
            }
        }
    }
    catch(Exception &Ex)
    {
        MessageBox(Ex.Message,"Error",MB_OK+MB_ICONERROR);
        throw;
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TSCDPatronUtility::CheckPatronsAvailable(TPaymentTransaction paymentTransaction)
{
    bool isNormalPatronCount = false;
    bool isSCPatronCount = false;
    for(int index = 0; index < paymentTransaction.Patrons.size(); index++)
    {
        if(paymentTransaction.Patrons[index].Name.UpperCase().Trim() != "SENIOR CITIZEN" &&
           paymentTransaction.Patrons[index].Count > 0 && !isNormalPatronCount)
           isNormalPatronCount = true;
        else if(paymentTransaction.Patrons[index].Name.UpperCase().Trim() == "SENIOR CITIZEN" &&
           paymentTransaction.Patrons[index].Count > 0 && !isSCPatronCount)
           isSCPatronCount = true;
    }
    return isNormalPatronCount && isSCPatronCount;
}
//---------------------------------------------------------------------------
bool TSCDPatronUtility::CheckDiscountsAvailable(TPaymentTransaction paymentTransaction)
{
    bool isSCDApplied = false;
    bool isNormalDiscountApplied = false;
    for(int indexItem = 0; indexItem < paymentTransaction.Orders->Count; indexItem++)
    {
        TItemComplete *item = (TItemComplete*)paymentTransaction.Orders->Items[indexItem];
        if(item->HasSeniorCitizensDiscountApplied() && !isSCDApplied)
            isSCDApplied = true;
        else if((item->Discounts.size() > 0) &&!item->HasSeniorCitizensDiscountApplied() && !isNormalDiscountApplied)
            isNormalDiscountApplied = true;
        for(int indexSubItem = 0; indexSubItem < item->SubOrders->Count; indexSubItem++)
        {
            TItemComplete *subItem = (TItemComplete*)item->SubOrders->Items[indexSubItem];
            if(subItem->HasSeniorCitizensDiscountApplied() && !isSCDApplied)
                isSCDApplied = true;
            else if((subItem->Discounts.size() > 0) &&!subItem->HasSeniorCitizensDiscountApplied() && !isNormalDiscountApplied)
                isNormalDiscountApplied = true;
        }
    }
    return isSCDApplied || isNormalDiscountApplied;
}
//---------------------------------------------------------------------------
void TSCDPatronUtility::GetPatronDistribution(std::vector<TPatronType> patrons, double &nonSCD, double &SCD)
{
    for(int index = 0; index < patrons.size(); index++)
    {
        if(patrons[index].Name.UpperCase().Trim() != "SENIOR CITIZEN")
            nonSCD += patrons[index].Count;
        else
            SCD += patrons[index].Count;
    }
}
//---------------------------------------------------------------------------
Currency TSCDPatronUtility::GetAmountForSCD(TPaymentTransaction paymentTransaction)
{
    double nonSCD = 0;
    double SCD = 0;
    Currency total = 0;
    GetPatronDistribution(paymentTransaction.Patrons, nonSCD, SCD);

    for(int indexItem = 0; indexItem < paymentTransaction.Orders->Count; indexItem++)
    {
       TItemComplete *item = ((TItemComplete*)paymentTransaction.Orders->Items[indexItem]);
       total += item->BillCalcResult.FinalPrice;
       for(int indexSubItem = 0; indexSubItem < item->SubOrders->Count; indexSubItem++)
       {
           TItemComplete *subItem = ((TItemComplete*)item->SubOrders->Items[indexSubItem]);
           total += subItem->BillCalcResult.FinalPrice;
       }
    }

    int totalPatrons = nonSCD+SCD;
    double portion = SCD/totalPatrons;
    total = total * portion;
    return total;
}
//---------------------------------------------------------------------------
