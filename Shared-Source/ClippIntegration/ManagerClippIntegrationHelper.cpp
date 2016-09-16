//---------------------------------------------------------------------------

#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "ManagerClippIntegrationHelper.h"
#include "DBOrder.h"
#include "Invoice.h"

//---------------------------------------------------------------------------

TClippTab TManagerClippIntegrationHelper::CreateClippTab(MMClippMessage mmClippMessage)
{
    //TODO: Will justify a better name here..
    AnsiString tabName = "Clipp-" + mmClippMessage.FirstName + "-" + mmClippMessage.LastName;

    TClippTab clippTab;
    clippTab.ClippTabRef = mmClippMessage.ClippTabRef;
    clippTab.TabCreatedDate = mmClippMessage.CreatedDate;
    clippTab.ClippUserRef = mmClippMessage.ClippUserRef;
    clippTab.CustomerFirstName = mmClippMessage.FirstName;
    clippTab.CustomerLastName = mmClippMessage.LastName;
    clippTab.CustomerPhotoUrl = mmClippMessage.PhotoUrl;
    clippTab.TabName = tabName;
    clippTab.TabType = TabClipp;
    clippTab.TabLimit = mmClippMessage.TabLimit;

    return clippTab;
}

//---------------------------------------------------------------------------

void TManagerClippIntegrationHelper::UpdateClippPaymentAmount(MMPaymentDetailResponse mmPaymentDetailResponse)
{
    try
    {
         //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        //Update the clipp tab payment table with the entry since we have recieved the payments from clipp..
        int clippTabKey = TDBClippTab::CreateClippTabPayment(dbTransaction, StrToInt(mmPaymentDetailResponse.PosTabRef), mmPaymentDetailResponse.ClippPaymentRef,
                                                                mmPaymentDetailResponse.TotalPaymentAmount, ClippPayment);

        //Since for every payment clipp may charge processing fees..
        //so verify if preocessing fees has been charged then persist this too..
        if(mmPaymentDetailResponse.ProcessingFeeAmount != 0.0)
        {
            clippTabKey = TDBClippTab::CreateClippTabPayment(dbTransaction, StrToInt(mmPaymentDetailResponse.PosTabRef), mmPaymentDetailResponse.ClippPaymentRef,
                                                                mmPaymentDetailResponse.ProcessingFeeAmount, ClippProcessingFees);
        }

        //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

TPaymentTransaction TManagerClippIntegrationHelper::CreatePartialPaymentTransaction(Database::TDBTransaction &dbTransaction, int tabKey, std::vector<MMClippDiscount> clippDiscounts, Currency tipAmount )
{
    try
    {
         // create a variable for Calculating total Venue discount
        Currency totalVenueDiscount = 0.0;

        //We need to pull all the data of the tab here..
        //Since we need to send the data to the clipp to synchronize the clipp app and the pos..
        std::set<__int64>clippTabs;
        clippTabs.insert(tabKey);

        //Create the payment transaction..
        //We need this to calculate the total order value..
        TPaymentTransaction clippTabTransaction(dbTransaction);

        //Since this is tab, set sales type as tab..
        clippTabTransaction.SalesType = eTab;

        //We are paying for the set of orders so type will be order set..
        clippTabTransaction.Type = eTransQuickSale;

        // If membership applied on selected tab then get contack key of that member
        contactsKey = TDBContacts::GetLoyaltyKey(dbTransaction, tabKey );

        // get memmber's detail according to contactkey
        TDBContacts::GetContactDetails(dbTransaction, contactsKey, MemberInfo);

        //if contact key > 0 means membership applied.
        if(contactsKey > 0)
        {
            TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(dbTransaction, MemberInfo, MemberSource);

            if(Result == lsAccepted)
            {
                Membership.Assign(MemberInfo, MemberSource);
            }
        }


        //Assigning member's detail and apply membership
        if(Membership.Member.ContactKey > 0)
        {
            clippTabTransaction.Membership = Membership;
            clippTabTransaction.ApplyMembership(Membership);
        }

        //Pull the order data from the database..
        TDBOrder::GetOrdersFromTabKeys(dbTransaction, clippTabTransaction.Orders, clippTabs);

        //Reconcile payment types into the payment transaction..
        TDeviceRealTerminal::Instance().PaymentSystem->PaymentsReload(clippTabTransaction);

        //Get the credit of the tab from the database..
        Currency tabCredit = TDBTab::GetTabCredit(dbTransaction, tabKey);

        //If there is a credit on the tab, this implies that pos has added some credit to the tab..
        //We need to add this credit to payments for this transaction..
        //First get the tab name..
        UnicodeString tabName = TDBTab::GetTabName(dbTransaction, tabKey).SubString(1, 15);
        TPayment* tabCreditPayment = clippTabTransaction.PaymentFind(tabName);
        if(tabCreditPayment && tabCredit > 0.0)
        {
            tabCreditPayment->SetPay(tabCredit);
        }

        //We need to see if there is any payment that has been made through the clipp app previously as a partial payment..
        Currency clippPayments = TDBClippTab::GetClippPaymentsTotal(dbTransaction, tabKey);
        if(clippPayments > 0.0)
        {
            //Now we need to find out clipp payment type and add it to the payment transaction..
            TPayment* clippPayment = clippTabTransaction.PaymentFind(CLIPP);
            if(clippPayment)
            {
                clippPayment->SetPay(clippPayments);
            }
        }

        //calculate GST
        Currency totalGST = clippTabTransaction.Money.GrandTotalGSTContent + clippTabTransaction.Money.ServiceChargeTax;

       //Applying discount when tab is closed from app
        ApplyDiscountOnTabClose(clippTabTransaction, clippDiscounts, &totalVenueDiscount);

        //Calculate the details of the complete order..
        clippTabTransaction.Recalc();

        //If tax setting is recalculate tax than add tax on discount to total amount
        AddtaxOnDiscountToTotal(clippTabTransaction);

        //
        clippTabTransaction.Money.GrandTotalGSTContent = totalGST;

        //Set the invoice number to 0..
        clippTabTransaction.InvoiceNumber = "0";

        return clippTabTransaction;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

TPaymentTransaction* TManagerClippIntegrationHelper::CreatePaymentTransaction(Database::TDBTransaction &dbTransaction, int tabKey, std::vector<MMClippDiscount> clippDiscounts)
{
    try
    {
        // create a variable for Calculating total Venue discount
        Currency totalVenueDiscount = 0.0;

        //We need to pull all the data of the tab here..
        //Since we need to send the data to the clipp to synchronize the clipp app and the pos..
        std::set<__int64>clippTabs;
        clippTabs.insert(tabKey);

        //Create the payment transaction..
        //We need this to calculate the total order value..
        TPaymentTransaction*  clippTabTransaction = new TPaymentTransaction(dbTransaction);

        //Since this is tab, set sales type as tab..
        clippTabTransaction->SalesType = eTab;

        //We are paying for the set of orders so type will be order set..
        clippTabTransaction->Type = eTransQuickSale;

        // If membership applied on selected tab then get contack key of that member
        contactsKey = TDBContacts::GetLoyaltyKey(dbTransaction, tabKey );

        TDBContacts::GetContactDetails(dbTransaction, contactsKey, MemberInfo);

        if(contactsKey > 0)
        {
            TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(dbTransaction,
                                                                                        MemberInfo, MemberSource);
             if(Result == lsAccepted)
             Membership.Assign(MemberInfo, MemberSource);
        }

        //Assigning membership if applied

        //Assigning member's detail
        if(Membership.Member.ContactKey > 0)
        {
            clippTabTransaction->Membership = Membership;//reinterpret_cast<TContactMemberApplied >(*member->membership);
            clippTabTransaction->ApplyMembership(Membership);
        }

        //Pull the order data from the database..
        TDBOrder::GetOrdersFromTabKeys(dbTransaction, clippTabTransaction->Orders, clippTabs);

        //Calculate the order total..
        clippTabTransaction->Recalc();

        //Reconcile payment types into the payment transaction..
        TDeviceRealTerminal::Instance().PaymentSystem->PaymentsReload(*clippTabTransaction);

        //Get the credit of the tab from the database..
        Currency tabCredit = TDBTab::GetTabCredit(dbTransaction, tabKey);

        //If there is a credit on the tab, this implies that pos has added some credit to the tab..
        //We need to add this credit to payments for this transaction..
        //First get the tab name..
        UnicodeString tabName = TDBTab::GetTabName(dbTransaction, tabKey).SubString(1, 15);
        TPayment* tabCreditPayment = clippTabTransaction->PaymentFind(tabName);
        if(tabCreditPayment && tabCredit > 0.0)
        {
            tabCreditPayment->SetPay(tabCredit);
        }

        Currency clippPayments = clippTabTransaction->Money.RoundedPaymentDue + TDBClippTab::GetClippPaymentsTotal(dbTransaction, tabKey);
        //Now we need to find out clipp payment type and add it to the payment transaction..
        TPayment* clippPayment = clippTabTransaction->PaymentFind(CLIPP);
        if(clippPayment)
        {
            clippPayment->SetPay(clippPayments);
            clippPayment->AdjustmentReason = "Clipp Processing Fee";

            //We need to add processing fees to the payment transaction as surcharge..
            Currency processingFeesAmount = TDBClippTab::GetProcessingFeesPaymentsTotal(dbTransaction, tabKey);
            if(processingFeesAmount != 0.0)
            {
                 //Applying processing Fee as surcharge
                ApplyProcessingFeeSurcharge(*clippTabTransaction, processingFeesAmount);
                clippTabTransaction->Recalc();
            }
        }

        //We need to add the tips and processing fees to the payment transaction before we jump into recalculating the payment transacction..
        //Add a tip payment type to the payment transaction..
        Currency tipAmount = TDBClippTab::GetTipPaymentsTotal(dbTransaction, tabKey);
        if(tipAmount != 0.0)
        {
            TPayment* tipPayment = clippTabTransaction->PaymentFind(TIPS);
            if(tipPayment)
            {
                tipPayment->SetAdjustment(tipAmount);
                tipPayment->AdjustmentReason = "Clipp Tip";
            }
        }


        //Applying discount when tab is closed from app
       ApplyDiscountOnTabClose(*clippTabTransaction, clippDiscounts, &totalVenueDiscount);
       clippTabTransaction->Recalc();

        if(clippPayment && totalVenueDiscount > 0.0  )
        {
                clippPayments -= totalVenueDiscount;
                clippPayment->SetPay(clippPayments);
        }

        int invoiceKey = TDBTab::GetTabInvoice(dbTransaction, tabKey);

        //If the invoice has still not been created then its the time to create it..
        if(invoiceKey == 0)
        {
            clippTabTransaction->InvoiceNumber = Invoice->GetNextInvoiceNumber(dbTransaction);
        }
        else
        {
            clippTabTransaction->InvoiceNumber = Invoice->GetInvoiceNumber(dbTransaction, invoiceKey);
        }

         //If tax setting is recalculate tax than add tax on discount to total amount
         AddtaxOnDiscountToTotal(*clippTabTransaction);

        return clippTabTransaction;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

MMClippTabDetail TManagerClippIntegrationHelper::CreateMMClippTabDetail(int tabKey, UnicodeString recievedMessageId, std::vector<MMClippDiscount> clippDiscounts, Currency tipAmount)
{
     try
    {
        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

         //Create the payment transaction object from the tab key..
        //We need to create it from database..
        TPaymentTransaction clippTabTransaction = CreatePartialPaymentTransaction(dbTransaction, tabKey, clippDiscounts, tipAmount );

        //Get the clipp tab details by tab key...
        TClippTab* clippTab = TDBClippTab::GetClippTabDetailsByTabKey(dbTransaction, tabKey);

        //We need to transform the payment transaction to clipp tab details..
        //Create the clipp tab details from the local data..
        MMClippTabDetail clippTabDetail(clippTab->ClippTabRef, recievedMessageId, clippTabTransaction.Money.GrandTotalGSTContent,
                                    clippTabTransaction.Money.RoundedPaymentDue, TDateTime::CurrentDateTime(), clippTab->TabLimit,
                                    clippTabTransaction.Money.RoundedFinalPrice, StrToInt(clippTabTransaction.InvoiceNumber), tabKey);

        //Loop through the items to fill in the clipp items..
        for (int i = 0; i < clippTabTransaction.Orders->Count; i++)
        {
            TItemComplete* currentItem = (TItemComplete*)clippTabTransaction.Orders->Items[i];
            MMClippItem mmClippItem(currentItem->Categories->FinancialCategory, currentItem->Item, currentItem->TotalDiscount_BillCalc(),
                                UnicodeString(currentItem->DiscountReasons()), currentItem->Course, currentItem->PriceLevel0,
                                currentItem->GetQty(), TGlobalSettings::Instance().ItemPriceIncludeTax, currentItem->FinalPrice_BillCalc(),
                                currentItem->ItemKey);

            for( int j = 0; j < currentItem->SubOrders->Count; j++ )
            {
                TItemCompleteSub* subItem = currentItem->SubOrders->SubOrderGet(j);
                MMClippItem mmClippSubItem(subItem->Categories->FinancialCategory, subItem->Item, subItem->TotalDiscount_BillCalc(),
                                UnicodeString(subItem->DiscountReasons()), subItem->Course, subItem->PriceLevel0,
                                subItem->GetQty(), TGlobalSettings::Instance().ItemPriceIncludeTax, subItem->FinalPrice_BillCalc(),
                                subItem->ItemKey);

                mmClippItem.SubItems.push_back(mmClippSubItem);
            }

            clippTabDetail.Items.push_back(mmClippItem);
        }

        //Loop through the payments for the items to create clipp payment..
        for (int i = 0; i < clippTabTransaction.PaymentsCount(); i++)
        {
            TPayment* payment = clippTabTransaction.PaymentGet(i);
            if(payment->GetPay() != 0.0)
            {
                MMClippPayment mmClippPayment(payment->GetPay(), payment->Name, payment->ReferenceNumber);
                clippTabDetail.Payments.push_back(mmClippPayment);
            }
        }


        //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();

         return clippTabDetail;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

MMClippTabDetail TManagerClippIntegrationHelper::CreateMMClippTabDetail(int tabKey, UnicodeString recievedMessageId, Currency tipAmount,
                                                        Currency processingFeesAmount, std::vector<MMClippDiscount> clippDiscounts)
{
    try
    {
        MMClippTabDetail mmClippTabDetail =  CreateMMClippTabDetail(tabKey, recievedMessageId, clippDiscounts, tipAmount);
        mmClippTabDetail.Amount = mmClippTabDetail.OutstandingAmount + tipAmount + processingFeesAmount;
        Currency discountAmount = 0.00;
        Currency taxOnDiscount = 0.00;
        for(std::vector<MMClippDiscount>::iterator it = clippDiscounts.begin(); it != clippDiscounts.end(); ++it)
        {
            discountAmount += it->Amount;
        }
         mmClippTabDetail.OutstandingAmount += discountAmount;
         mmClippTabDetail.TotalAmount += discountAmount;

        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        //Update the tab details with processing fees and tip amounts..
        int clippTabKey = TDBClippTab::CreateClippTabPayment(dbTransaction, tabKey, UnicodeString(""), processingFeesAmount, ClippProcessingFees);
        clippTabKey = TDBClippTab::CreateClippTabPayment(dbTransaction, tabKey, UnicodeString(""), tipAmount, ClippTip);

        //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();

        return mmClippTabDetail;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

MMPaymentDetailRequest TManagerClippIntegrationHelper::CreateMMPaymentDetailRequest(int tabKey, UnicodeString recievedMessageId, Currency totalPaymentAmount,
                                                                        Currency clippProcessingFees)
{
    try
    {
        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        //Get the clipp tab details by tab key...
        TClippTab* clippTab = TDBClippTab::GetClippTabDetailsByTabKey(dbTransaction, tabKey);

        //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();

        MMPaymentDetailRequest mmPaymentDetailRequest = MMPaymentDetailRequest(totalPaymentAmount, recievedMessageId, clippTab->ClippTabRef, UnicodeString(""));

        return mmPaymentDetailRequest;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TManagerClippIntegrationHelper::FinalizeClippTab(MMClippMessageDetail mmClippMessageDetail)
{
    try
    {
        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        //Check if tab key is null than show Message
        if(mmClippMessageDetail.PosTabRef == "")
        {
            MessageBox("Tab cann't be closed.", "Error", MB_OK + MB_ICONERROR);
            dbTransaction.Commit();
        }
        else
        {
            //Get the clipp tab details by tab key...
            TClippTab* clippTab = TDBClippTab::GetClippTabDetailsByTabKey(dbTransaction, StrToInt(mmClippMessageDetail.PosTabRef));

            //Create the payment transaction object from the tab key..
            //We need to create it from database..
            TPaymentTransaction* clippTabTransaction = CreatePaymentTransaction(dbTransaction, clippTab->TabKey, mmClippMessageDetail.Discounts);

            //Update clipp_tab table 's is_tab_closed field since tab is being close
            TDBClippTab::CloseClippTab(dbTransaction, clippTab->TabKey);

            //Save the order to the database..
            TDeviceRealTerminal::Instance().PaymentSystem->ProcessTransaction(*clippTabTransaction, false, true);

            //Commit the transaction as we have completed all the transactions
            dbTransaction.Commit();
        }
    }

    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

///---------------------------------------------------------------------------

 void TManagerClippIntegrationHelper:: ApplyDiscountOnTabClose(TPaymentTransaction &clippTabTransaction, std::vector<MMClippDiscount> clippDiscounts, Currency *totalVenueDiscount)
 {
    //Applying venue discounts when tab is closed
    if(clippDiscounts.size())
    {
        Currency clippVenueDiscount = 0.00;

        for(std::vector<MMClippDiscount>::iterator it = clippDiscounts.begin(); it != clippDiscounts.end(); ++it)
        {
            clippVenueDiscount += it->Amount;
            TDiscount clippDiscount = CreateMMDiscount(it);

            clippTabTransaction.Discounts.push_back(clippDiscount);
        }
        *totalVenueDiscount = clippVenueDiscount;
        CalculateTaxOnClippVenueDiscount(clippTabTransaction, clippVenueDiscount);
    }
 }

 //-------------------------------------------------------------------------------------------------------------
void TManagerClippIntegrationHelper::CalculateTaxOnClippVenueDiscount(TPaymentTransaction &clippTabTransaction, Currency clippVenueDiscount)
{
    //calculate servicecharge + servicetax
    Currency serviceChargeWithTax = clippTabTransaction.Money.ServiceCharge + clippTabTransaction.Money.ServiceChargeTax;

    //if calculate tax after applying discount setting is on and item price exclude tax
    if(TGlobalSettings::Instance().ReCalculateTaxPostDiscount && !TGlobalSettings::Instance().ItemPriceIncludeTax &&
            TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount && TGlobalSettings::Instance().UsingServiceCharge
            && !TGlobalSettings::Instance().ItemPriceIncludeServiceCharge)
    {
        clippTabTransaction.TaxOnClippDiscount = (((clippTabTransaction.Money.GrandTotalGSTContent + serviceChargeWithTax)/(clippTabTransaction.Money.FinalPrice- (clippTabTransaction.Money.GrandTotalGSTContent + serviceChargeWithTax))) * (clippVenueDiscount));
    }
    else
    {
        if(((!TGlobalSettings::Instance().ItemPriceIncludeTax) && (TGlobalSettings::Instance().ItemPriceIncludeServiceCharge) &&
            (TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount && TGlobalSettings::Instance().UsingServiceCharge))
            && ((TGlobalSettings::Instance().ReCalculateTaxPostDiscount)))
        {
            clippTabTransaction.TaxOnClippDiscount = (((clippTabTransaction.Money.GrandTotalGSTContent + clippTabTransaction.Money.ServiceChargeTax)/(clippTabTransaction.Money.FinalPrice- (clippTabTransaction.Money.GrandTotalGSTContent + clippTabTransaction.Money.ServiceChargeTax))) * (clippVenueDiscount));
        }
        else
        {
            if(((TGlobalSettings::Instance().ReCalculateTaxPostDiscount) &&(!TGlobalSettings::Instance().ItemPriceIncludeTax)))
            {
                if(((TGlobalSettings::Instance().ReCalculateTaxPostDiscount) &&(!TGlobalSettings::Instance().ItemPriceIncludeTax)
                    && (!TGlobalSettings::Instance().ItemPriceIncludeServiceCharge) && (!TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount) &&
                    (!TGlobalSettings::Instance().ApplyServiceChargeTax)))
                {
                    clippTabTransaction.TaxOnClippDiscount = (((clippTabTransaction.Money.GrandTotalGSTContent + clippTabTransaction.Money.ServiceChargeTax)/(clippTabTransaction.Money.FinalPrice- (clippTabTransaction.Money.GrandTotalGSTContent + serviceChargeWithTax))) * (clippVenueDiscount));
                }

                else if(((TGlobalSettings::Instance().ReCalculateTaxPostDiscount) &&(!TGlobalSettings::Instance().ItemPriceIncludeTax)
                        && (TGlobalSettings::Instance().ItemPriceIncludeServiceCharge) && (!TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount) &&
                        (!TGlobalSettings::Instance().ApplyServiceChargeTax)))
                {
                    clippTabTransaction.TaxOnClippDiscount = (((clippTabTransaction.Money.GrandTotalGSTContent + clippTabTransaction.Money.ServiceChargeTax)/(clippTabTransaction.Money.FinalPrice- (clippTabTransaction.Money.GrandTotalGSTContent))) * (clippVenueDiscount));
                }

                else if(((TGlobalSettings::Instance().ReCalculateTaxPostDiscount) &&(!TGlobalSettings::Instance().ItemPriceIncludeTax)
                        && (TGlobalSettings::Instance().ItemPriceIncludeServiceCharge) && (!TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount) &&
                        (TGlobalSettings::Instance().ApplyServiceChargeTax)))
                {
                    clippTabTransaction.TaxOnClippDiscount = (((clippTabTransaction.Money.GrandTotalGSTContent)/(clippTabTransaction.Money.FinalPrice- (clippTabTransaction.Money.GrandTotalGSTContent + clippTabTransaction.Money.ServiceChargeTax))) * (clippVenueDiscount));
                }

                else if(((TGlobalSettings::Instance().ReCalculateTaxPostDiscount) &&(!TGlobalSettings::Instance().ItemPriceIncludeTax)
                        && (!TGlobalSettings::Instance().ItemPriceIncludeServiceCharge) && (!TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount) &&
                        (TGlobalSettings::Instance().ApplyServiceChargeTax)))
                {
                    clippTabTransaction.TaxOnClippDiscount = (((clippTabTransaction.Money.GrandTotalGSTContent)/(clippTabTransaction.Money.FinalPrice- (clippTabTransaction.Money.GrandTotalGSTContent + serviceChargeWithTax))) * (clippVenueDiscount));
                }
                else
                {
                    clippTabTransaction.TaxOnClippDiscount = (((clippTabTransaction.Money.GrandTotalGSTContent + clippTabTransaction.Money.ServiceChargeTax)/(clippTabTransaction.Money.FinalPrice- (clippTabTransaction.Money.GrandTotalGSTContent + clippTabTransaction.Money.ServiceChargeTax))) * (clippVenueDiscount));
                }
            }
            else
            {
                clippTabTransaction.TaxOnClippDiscount = (((clippTabTransaction.Money.GrandTotalGSTContent)/(clippTabTransaction.Money.FinalPrice- (clippTabTransaction.Money.GrandTotalGSTContent))) * (clippVenueDiscount));
            }
        }

        if(TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount && !TGlobalSettings::Instance().ItemPriceIncludeTax && ! TGlobalSettings::Instance().ReCalculateTaxPostDiscount)
        {
            clippTabTransaction.ServiceChargeWithTax = ((serviceChargeWithTax)/(clippTabTransaction.Money.FinalPrice- (clippTabTransaction.Money.GrandTotalGSTContent + serviceChargeWithTax)) * clippVenueDiscount);
        }
        else
        {
            if(TGlobalSettings::Instance().ItemPriceIncludeServiceCharge && !TGlobalSettings::Instance().ReCalculateTaxPostDiscount
                && TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount && TGlobalSettings::Instance().UsingServiceCharge
                && !TGlobalSettings::Instance().ItemPriceIncludeTax )
            {
                clippTabTransaction.ServiceChargeWithTax = ((clippTabTransaction.Money.ServiceChargeTax)/(clippTabTransaction.Money.FinalPrice- (clippTabTransaction.Money.GrandTotalGSTContent + clippTabTransaction.Money.ServiceChargeTax)) * clippVenueDiscount);
            }
            else
            {
                clippTabTransaction.ServiceChargeWithTax = ((serviceChargeWithTax)/(clippTabTransaction.Money.FinalPrice- serviceChargeWithTax) * clippVenueDiscount);
            }
        }

        if(TGlobalSettings::Instance().ItemPriceIncludeServiceCharge && !TGlobalSettings::Instance().ReCalculateTaxPostDiscount
                && TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount && TGlobalSettings::Instance().UsingServiceCharge
                 && !TGlobalSettings::Instance().ItemPriceIncludeTax )
        {
            clippTabTransaction.ServiceChargeWithTax = ((clippTabTransaction.Money.ServiceChargeTax)/(clippTabTransaction.Money.FinalPrice- (clippTabTransaction.Money.GrandTotalGSTContent + clippTabTransaction.Money.ServiceChargeTax)) * clippVenueDiscount);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

 void TManagerClippIntegrationHelper::AddtaxOnDiscountToTotal(TPaymentTransaction &clippTabTransaction)
{
    //If tax setting is recalculate tax than add tax on discount to total amount
    if((TGlobalSettings::Instance().ReCalculateTaxPostDiscount && !TGlobalSettings::Instance().ItemPriceIncludeTax)
            && (!TGlobalSettings::Instance().ItemPriceIncludeServiceCharge || (TGlobalSettings::Instance().ItemPriceIncludeServiceCharge &&
            TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount && TGlobalSettings::Instance().UsingServiceCharge)))
    {
       AddTaxOnVenueDiscount(clippTabTransaction);
    }

    else if(TGlobalSettings::Instance().ReCalculateTaxPostDiscount && !TGlobalSettings::Instance().ItemPriceIncludeTax
                && TGlobalSettings::Instance().ItemPriceIncludeServiceCharge && !TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount &&
                TGlobalSettings::Instance().ApplyServiceChargeTax)
    {
        AddTaxOnVenueDiscount(clippTabTransaction);
    }

    else if(TGlobalSettings::Instance().ReCalculateTaxPostDiscount && !TGlobalSettings::Instance().ItemPriceIncludeTax
            && TGlobalSettings::Instance().ItemPriceIncludeServiceCharge && !TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount &&
            !TGlobalSettings::Instance().ApplyServiceChargeTax)
    {
        AddTaxOnVenueDiscount(clippTabTransaction);
    }

    if(TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount && !TGlobalSettings::Instance().ItemPriceIncludeServiceCharge)
    {
        AddServiceChargeWithtax (clippTabTransaction);
    }

    if(TGlobalSettings::Instance().ItemPriceIncludeServiceCharge && !TGlobalSettings::Instance().ReCalculateTaxPostDiscount
            && TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount && TGlobalSettings::Instance().UsingServiceCharge
            && !TGlobalSettings::Instance().ItemPriceIncludeTax )
    {
        AddServiceChargeWithtax (clippTabTransaction);
    }

}

//------------------------------------------------------------------------------------------------------------------------
void TManagerClippIntegrationHelper::AddServiceChargeWithtax (TPaymentTransaction &clippTabTransaction)
{
    clippTabTransaction.Money.RoundedPaymentDue += clippTabTransaction.ServiceChargeWithTax;
    clippTabTransaction.Money.RoundedFinalPrice += clippTabTransaction.ServiceChargeWithTax;
    clippTabTransaction.Money.RoundedGrandTotal += clippTabTransaction.ServiceChargeWithTax;
}

//------------------------------------------------------------------------------------------------------------------------
void TManagerClippIntegrationHelper::AddTaxOnVenueDiscount (TPaymentTransaction &clippTabTransaction)
{
    clippTabTransaction.Money.RoundedPaymentDue += clippTabTransaction.TaxOnClippDiscount;
    clippTabTransaction.Money.RoundedFinalPrice += clippTabTransaction.TaxOnClippDiscount;
    clippTabTransaction.Money.RoundedGrandTotal += clippTabTransaction.TaxOnClippDiscount;
}
//----------------------------------------------------------------------------------------------------------------------

TDiscount TManagerClippIntegrationHelper:: CreateMMDiscount(std::vector<MMClippDiscount>::iterator it)
{
    TDiscount clippDiscount;

    clippDiscount.DiscountKey               =   -99;
    clippDiscount.Name                      =   it->Description;
    clippDiscount.Description               =   it->Description;
    clippDiscount.DiscountCode              =   "";
    clippDiscount.Priority                  =   99;
    clippDiscount.AppearanceOrder           =   0;
    clippDiscount.PercentAmount             =   0;
    clippDiscount.Amount                    =   it->Amount;
    clippDiscount.Rounding                  =   0.01;
    clippDiscount.MembersOnly               =   false;
    clippDiscount.MembersExempt             =   false;
    clippDiscount.Type                      =   0;
    clippDiscount.Mode                      =   DiscModeCurrency;
    clippDiscount.MaximumValue              =   0;
    clippDiscount.Group                     =   0;
    clippDiscount.IsClipp                   =   true;

    return clippDiscount;
}
//-----------------------------------------------------------------------------------------------------------------

void TManagerClippIntegrationHelper:: ApplyProcessingFeeSurcharge(TPaymentTransaction &clippTabTransaction, Currency processingFeesAmount)
{
    //Create Surcharge for processing fee
    TDiscount clippSurcharge = CreateClippSurcharge(processingFeesAmount);
    clippTabTransaction.Discounts.push_back(clippSurcharge);
}

 //-------------------------------------------------------------------------------------------------------------
TDiscount TManagerClippIntegrationHelper:: CreateClippSurcharge(Currency clippProcessingFee)
{
    TDiscount clippSurcharge;

    clippSurcharge.DiscountKey              =   -98;
    clippSurcharge.Name                     =   "Clipp ProceSsing Fee";
    clippSurcharge.Description              =   "Clipp ProceSsing Fee";
    clippSurcharge.DiscountCode             =   "";
    clippSurcharge.Priority                 =   98;
    clippSurcharge.AppearanceOrder          =   0;
    clippSurcharge.PercentAmount            =   0;
    clippSurcharge.Amount                   =   -clippProcessingFee;
    clippSurcharge.Rounding                 =   0.01;
    clippSurcharge.MembersOnly              =   false;
    clippSurcharge.MembersExempt            =   false;
    clippSurcharge.Type                     =   0;
    clippSurcharge.Mode                     =   DiscModeCurrency;
    clippSurcharge.MaximumValue             =   0;
    clippSurcharge.Group                    =   0;
    clippSurcharge.IsClipp                  =   true;

    return clippSurcharge;
}
//-----------------------------------------------------------------------------------------------------------------
