//---------------------------------------------------------------------------

#ifndef MewsInterfaceH
#define MewsInterfaceH
#include <System.hpp>
#include<list>
#include "MewsDataObjects.h"
#include "MewsWSDL.h"
//---------------------------------------------------------------------------
class TMewsInterface
{
    private:
        _di_IMewsIntegrationWebService mewsClient; // WCF Client
    public:
        TMewsInterface();
        void initMewsClient();
        std::vector<TOutlet> GetOutlets(UnicodeString address,UnicodeString clientToken,UnicodeString AccessToken);
        std::vector<TServiceMews> GetServices(UnicodeString address,UnicodeString clientToken,UnicodeString accessToken);
        TSpaceDetails GetSpaces(UnicodeString address,UnicodeString clientToken,UnicodeString accessToken);
        std::vector<TAccountingCategory> GetMewsAccountingCategories(UnicodeString address,UnicodeString clientToken,UnicodeString accessToken);
        std::vector<TCustomerMews> GetCustomers(UnicodeString platformAddress,TCustomerSearch customerSearch);
        UnicodeString PostMewsOrder(UnicodeString platformAddress,TOrder order);
        UnicodeString PostMewsBill(UnicodeString platformAddress,TOrder order);
};
#endif
