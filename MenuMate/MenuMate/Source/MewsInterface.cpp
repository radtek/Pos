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
            service.Promotions.BeforeCheckIn =  services[i]->Promotions->BeforeCheckIn;
            service.Promotions.AfterCheckIn  =  services[i]->Promotions->AfterCheckIn;
            service.Promotions.DuringStay    =  services[i]->Promotions->DuringStay;
            service.Promotions.BeforeCheckOut=  services[i]->Promotions->BeforeCheckOut;
            service.Promotions.AfterCheckOut =  services[i]->Promotions->AfterCheckOut;
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
            for(int i = 0; i < spaces->SpaceCategories.get_length(); i++)
            {
                TSpaceCategory spacecategory;
                spacecategory.Id = spaces->SpaceCategories[i]->Id;
                spacecategory.IsActive = spaces->SpaceCategories[i]->IsActive;
                spacecategory.Name = spaces->SpaceCategories[i]->Name;
                spacecategory.ShortName = spaces->SpaceCategories[i]->ShortName;
                spacecategory.Description = spaces->SpaceCategories[i]->Description;
                spacecategory.Ordering = spaces->SpaceCategories[i]->Ordering;
                spacecategory.UnitCount = spaces->SpaceCategories[i]->UnitCount;
                spacecategory.ExtraUnitCount = spaces->SpaceCategories[i]->ExtraUnitCount;
                spaceDetails.SpaceCategories.push_back(spacecategory);
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
//---------------------------------------------------------------------------

std::vector<TCustomerMews> TMewsInterface::GetCustomers(UnicodeString platformAddress,TCustomerSearch customerSearch)
{
    std::vector<TCustomerMews> customerMewsList;
    customerMewsList.clear();
    std::auto_ptr<TStringList> List(new TStringList);
    try
    {
        //CoInitialize(NULL);
        CustomerSearch *customerSearchMews = new CustomerSearch();
        customerSearchMews->AccessToken = customerSearch.AccessToken;
        customerSearchMews->ClientToken = customerSearch.ClientToken;
        customerSearchMews->Name = customerSearch.Name;
        customerSearchMews->SpaceId = customerSearch.SpaceId;
        ArrayOfCustomer customerArray;
        CoInitialize(NULL);
        customerArray = mewsClient->SearchCustomers(platformAddress,customerSearchMews);
        List->Add("After receiving response.");
        for(int i = 0; i < customerArray.get_length(); i++)
        {
            List->Add("Value of i is: " + i);
            TCustomerMews customerMews;
            customerMews.FirstName = customerArray[i]->FirstName;
            customerMews.LastName = customerArray[i]->LastName;
            customerMews.Id = customerArray[i]->Id;
//            if(customerArray[i]->Classifications != NULL)
//            {
                for(int j = 0; j < customerArray[i]->Classifications.get_length();j++)
                {
                    customerMews.Classifications.push_back(customerArray[i]->Classifications[j]);
                }
            //}
            if(customerArray[i]->Number != NULL)
                customerMews.Number = customerArray[i]->Number;
            else
                customerMews.Number = "";
            if(customerArray[i]->Phone != NULL)
                customerMews.Phone = customerArray[i]->Phone;
            else
                customerMews.Phone = "";
            if(customerArray[i]->SecondLastName != NULL)
                customerMews.SecondLastName = customerArray[i]->SecondLastName;
            else
                customerMews.SecondLastName = "";

            customerMewsList.push_back(customerMews);
            List->Add("Added an instance of customer to vector");
        }
        return customerMewsList;
    }
    catch(Exception &ex)
    {
         return customerMewsList;
    }

}
//---------------------------------------------------------------------------
