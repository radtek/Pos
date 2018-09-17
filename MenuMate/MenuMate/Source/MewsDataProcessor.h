//---------------------------------------------------------------------------

#ifndef MewsDataProcessorH
#define MewsDataProcessorH
#include "PaymentTransaction.h"
#include "MewsDataObjects.h"
//---------------------------------------------------------------------------
class TMewsDataProcessor
{
    public:
        TMewsDataProcessor();
        ~TMewsDataProcessor();
        void UpdateOutlets(std::vector<TOutlet> outlets);
        void UpdateServices(std::vector<TServiceMews> services);
        void UpdateSpaces(TSpaceDetails spaces);
        void UpdateCategories(std::vector<TAccountingCategory> categories);
        std::vector<TOutlet> GetOutlets();
        std::vector<TServiceMews> GetServices();
        TCustomerSearch CreateInquiryForRoomBySpace(UnicodeString queryString);
        TCustomerSearch CreateInquiryForRoomByName(UnicodeString queryString);
};
#endif
