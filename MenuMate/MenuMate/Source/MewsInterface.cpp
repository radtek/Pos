//---------------------------------------------------------------------------
#pragma hdrstop
#include "MewsInterface.h"
#include <memory>
#pragma package(smart_init)
//---------------------------------------------------------------------------
TMewsInterface::TMewsInterface()
{
    initMewsClient();
}
//---------------------------------------------------------------------------
void TMewsInterface::initMewsClient()
{
    bool useWSDL = false;
    AnsiString mewsURL = AnsiString("http://localhost:8747/MenumateServices/MewsService/");

    mewsClient = GetIMewsIntegrationWebService(
                            useWSDL, mewsURL, NULL );
}
//---------------------------------------------------------------------------
std::vector<TOutlet> TMewsInterface::GetOutlets(UnicodeString address,UnicodeString clientToken,UnicodeString accessToken)
{
    std::vector<TOutlet> outletsList;
    outletsList.clear();
    try
    {
        CoInitialize(NULL);
        BasicInquiry* basicInquiry = new BasicInquiry();
        basicInquiry->ClientToken = clientToken;
        basicInquiry->AccessToken = accessToken;
        ArrayOfOutlet outlets = mewsClient->GetMewsOutlets(address,basicInquiry);
        for(int i = 0; i< outlets.get_length(); i++)
        {
            TOutlet outlet;
            outlet.Id = outlets[i]->Id;
            outlet.IsActive = outlets[i]->IsActive;
            outlet.Name = outlets[i]->Name;
            outletsList.push_back(outlet);
        }
    }
    catch(Exception & E)
    {

	}
    return outletsList;
}
//---------------------------------------------------------------------------
std::vector<TServiceMews> TMewsInterface::GetServices(UnicodeString address,UnicodeString clientToken,UnicodeString accessToken)
{
    std::vector<TServiceMews> serviceList;
    serviceList.clear();
    try
    {
        CoInitialize(NULL);
        BasicInquiry* basicInquiry = new BasicInquiry();
        basicInquiry->ClientToken = clientToken;
        basicInquiry->AccessToken = accessToken;
        ArrayOfService services = mewsClient->GetMewsServices(address,basicInquiry);
        for(int i = 0; i< services.get_length(); i++)
        {
            TServiceMews service;
            service.Id = services[i]->Id;
            service.IsActive = services[i]->IsActive;
            service.Name = services[i]->Name;
            service.StartTime = services[i]->StartTime;
            service.EndTime = services[i]->EndTime;
            serviceList.push_back(service);
        }
    }
    catch(Exception & E)
    {

	}
    return serviceList;
}
//---------------------------------------------------------------------------
TSpaceDetails TMewsInterface::GetSpaces(UnicodeString address,UnicodeString clientToken,UnicodeString accessToken)
{
    TSpaceDetails spaceDetails;
    try
    {
        CoInitialize(NULL);
        BasicInquiry* basicInquiry = new BasicInquiry();
        basicInquiry->ClientToken = clientToken;
        basicInquiry->AccessToken = accessToken;
        Extent *extent = new Extent();
        extent->Inactive = false;
        extent->Spaces   = true;
        extent->SpaceCategories = true;
        extent->SpaceFeatures   = false;
        basicInquiry->Extent = new Extent();
        basicInquiry->Extent->Inactive = false;
        basicInquiry->Extent->Spaces   = true;
        basicInquiry->Extent->SpaceCategories = true;
        basicInquiry->Extent->SpaceFeatures   = false;
        SpaceDetails* spaces = mewsClient->GetSpaceIds(address,basicInquiry);
        if(spaces != NULL)
        {
            for(int i = 0; i < spaces->Spaces.get_length(); i++)
            {
                TSpace space;
                space.Id = spaces->Spaces[i]->Id;
                space.IsActive = spaces->Spaces[i]->IsActive;
                space.ParentSpaceId = spaces->Spaces[i]->ParentSpaceId;
                space.CategoryId = spaces->Spaces[i]->CategoryId;
                space.Type = spaces->Spaces[i]->Type;
                space.Number = spaces->Spaces[i]->Number;
                space.FloorNumber = spaces->Spaces[i]->FloorNumber;
                space.BuildingNumber = spaces->Spaces[i]->BuildingNumber;
                space.State = spaces->Spaces[i]->State;
                spaceDetails.Spaces.push_back(space);
            }
        }
    }
    catch(Exception & E)
    {

	}
    return spaceDetails;
}
//---------------------------------------------------------------------------
std::vector<TAccountingCategory> TMewsInterface::GetMewsAccountingCategories(UnicodeString address,UnicodeString clientToken,UnicodeString accessToken)
{
    std::vector<TAccountingCategory> categories;
    categories.clear();
    try
    {
        CoInitialize(NULL);
        BasicInquiry* basicInquiry = new BasicInquiry();
        basicInquiry->ClientToken = clientToken;
        basicInquiry->AccessToken = accessToken;
        ArrayOfAccountingCategory accountingCategories = mewsClient->GetMewsAccountingCategories(address,basicInquiry);
        for(int i = 0; i < accountingCategories.get_length(); i++)
        {
            TAccountingCategory accountingcategory;
            accountingcategory.Id = accountingCategories[i]->Id;
            accountingcategory.IsActive = accountingCategories[i]->IsActive;
            accountingcategory.Name = accountingCategories[i]->Name;
            accountingcategory.Code = accountingCategories[i]->Code;
            accountingcategory.ExternalCode = accountingCategories[i]->ExternalCode;
            accountingcategory.LedgerAccountCode = accountingCategories[i]->LedgerAccountCode;
            accountingcategory.PostingAccountCode = accountingCategories[i]->PostingAccountCode;
            accountingcategory.CostCenterCode = accountingCategories[i]->CostCenterCode;
            accountingcategory.Classification = accountingCategories[i]->Classification;
            categories.push_back(accountingcategory);
        }
    }
    catch(Exception & E)
    {

	}
    return categories;
}
std::vector<TCustomerMews> TMewsInterface::GetCustomers(UnicodeString platformAddress,TCustomerSearch customerSearch)
{
    std::vector<TCustomerMews> customerMewsList;
    customerMewsList.clear();
    std::auto_ptr<TStringList> List(new TStringList);
    try
    {
        CustomerSearch *customerSearchMews = new CustomerSearch();
        customerSearchMews->AccessToken = customerSearch.AccessToken;
        customerSearchMews->ClientToken = customerSearch.ClientToken;
        customerSearchMews->Name = customerSearch.Name;
        customerSearchMews->SpaceId = customerSearch.SpaceId;
        ArrayOfCustomerDetailsMews customerArray = mewsClient->SearchCustomers(platformAddress,customerSearchMews);
        for(int i = 0; i < customerArray.get_length(); i++)
        {
            TCustomerMews customerMews;
            customerMews.Classifications.clear();
            customerMews.FirstName  =  customerArray[i]->FirstName;
            customerMews.LastName   =  customerArray[i]->LastName;
            customerMews.Id         =  customerArray[i]->Id;
            customerMews.Number     =  customerArray[i]->Number;
            customerMews.Email      =  customerArray[i]->Email;
            customerMews.Phone      =  customerArray[i]->Phone;
            for(int j = 0; j < customerArray[i]->Classifications.get_length(); j++)
            {
                customerMews.Classifications.push_back(customerArray[i]->Classifications[j]);
            }
            customerMewsList.push_back(customerMews);
        }
        return customerMewsList;
    }
    catch(Exception &ex)
    {
         return customerMewsList;
    }
}
UnicodeString TMewsInterface::PostMewsOrder(UnicodeString platformAddress,TOrder order)
{
    UnicodeString retValue = "";
    try
    {
        Order* orderMews = new Order();
        orderMews->AccessToken = order.AccessToken;
        orderMews->ClientToken = order.ClientToken;
        orderMews->CustomerId = order.CustomerId;
        orderMews->ServiceId  = order.ServiceId;
        orderMews->ConsumptionUtc = order.ConsumptionUtc;
        ArrayOfItem arrayOfItems;
        for(int i = 0; i < order.Items.size(); i++)
        {
            Item *item = new Item();
            item->Name = order.Items[i].Name;
            item->UnitCount = order.Items[i].UnitCount;
            item->UnitCost = new UnitCost();
            item->UnitCost->Amount = order.Items[i].UnitCost.Amount;
            item->UnitCost->Currency = order.Items[i].UnitCost.Currency;
            item->UnitCost->Tax  = order.Items[i].UnitCost.Tax;
            item->Category = new Category();
            item->Category->Name = order.Items[i].Category.Name;
            arrayOfItems.Length = (arrayOfItems.Length + 1);
            arrayOfItems[arrayOfItems.Length - 1] = item;
        }
        orderMews->Items = arrayOfItems;
        retValue = mewsClient->PostOrder(platformAddress,orderMews);
    }
    catch(Exception &ex)
    {
         return "";
    }
    return retValue;
}
bool TMewsInterface::PostMewsBill(UnicodeString platformAddress,TOrder order)
{
    bool retValue = false;
    try
    {
        Order* orderMews = new Order();
        orderMews->AccessToken = order.AccessToken;
        orderMews->ClientToken = order.ClientToken;
        ArrayOfBill arrayOfBills;
        for(int i = 0; i < order.Bills.size(); i++)
        {
            Bill *billMews = new Bill();
            billMews->Number = order.Bills[i].Number;
            billMews->OutletId = order.Bills[i].OutletId;
            ArrayOfItem arrayOfItems;
            for(int j = 0; j < order.Bills[i].Items.size();j++)
            {
                Item *item = new Item();
                item->Type = order.Bills[i].Items[j].Type;
                item->Name = order.Bills[i].Items[j].Name;
                item->UnitCost = new UnitCost();
                item->UnitCost->Amount = order.Bills[i].Items[j].UnitCost.Amount;
                item->UnitCost->Currency = order.Bills[i].Items[j].UnitCost.Currency;
                item->UnitCost->Tax      = order.Bills[i].Items[j].UnitCost.Tax;
                item->UnitCount = order.Bills[i].Items[j].UnitCount;
//                if(order.Bills[i].Items[j].Type == "Revenue")
//                {
                    item->Category = new Category();
                    item->Category->Name = order.Bills[i].Items[j].Category.Name;
//                }
                arrayOfItems.Length = (arrayOfItems.Length + 1);
                arrayOfItems[arrayOfItems.Length - 1] = item;
            }
            billMews->Items = arrayOfItems;
            arrayOfBills.Length = (arrayOfBills.Length + 1);
            arrayOfBills[arrayOfBills.Length - 1] = billMews;
        }
        orderMews->Bills = arrayOfBills;
        retValue = mewsClient->PostBill(platformAddress,orderMews);
    }
    catch(Exception &ex)
    {
         return false;
    }
    return retValue;
}
