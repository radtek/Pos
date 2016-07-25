//---------------------------------------------------------------------------


#pragma hdrstop

#include "SalesForceInterface.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
TSalesForceInterface::TSalesForceInterface()
{
   initSFClient();
}
void TSalesForceInterface::initSFClient()
{
    bool useWSDL = false;
    AnsiString addr = AnsiString("http://localhost:8741/MenumateServices/SalesForceService/");
    sfClient = GetISalesForceIntegrationWebService(useWSDL,  addr, NULL);
}
void TSalesForceInterface::UpdateSalesForceFields(SalesForceFields sfFields)
{
    SalesForceModuleStatus *sfModuleStatus = new SalesForceModuleStatus();
    sfModuleStatus->IsMobiToGoActivated =  sfFields.Mobi2Go;
    sfModuleStatus->IsStockActivated =  sfFields.Stock;
    sfModuleStatus->IsThorlinkActivated =  sfFields.Thorlink;
    sfModuleStatus->IsWebMateActivated =  sfFields.WebMate;
    sfModuleStatus->IsXeroActivated =  sfFields.Xero;
    sfModuleStatus->SalesForceVersion =  sfFields.VersionNumber;
    sfModuleStatus->IsChefMateActivated =  sfFields.ChefMate;
    sfModuleStatus->IsPalmMateActivated =  sfFields.PalmMate;
    sfModuleStatus->IsLoyaltyActivated =  sfFields.Loyalty;
    sfModuleStatus->AccountName = sfFields.AccountName;
    sfClient->UpdateSalesForceModulesStatus(sfModuleStatus);
}
