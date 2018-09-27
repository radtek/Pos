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
        std::vector<TAccountingCategory> GetCategoriesFromDB();
        TUnitCost GetUnitCost(TItemComplete* itemComplete, double portion);
        std::vector<TAccountingCategoriesMapping> GetMewsCategoriesList();
        std::vector<TItemMews> GetMewsOrder(TPaymentTransaction &paymentTransaction, double portion, int index, double tipPortion, std::map<int,TAccountingCategory> accountingMap);
        std::vector<TBill> GetMewsBill(TPaymentTransaction &paymentTransaction, double portion, int index, double tipPortion, std::map<int,TAccountingCategory> accountingMap);
        std::map<int,UnicodeString> GetFreshMenumateCategories();
        void InitializeMewsCategories();
        std::map<int,TAccountingCategory> GetMewsCategoriesMap();
        void UpdateCategories(int categoryKey, UnicodeString name, bool isDeleted);
        void InsertCategories(int categoryKey, UnicodeString name);
};
#endif
