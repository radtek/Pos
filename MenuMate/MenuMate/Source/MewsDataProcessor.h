//---------------------------------------------------------------------------

#ifndef MewsDataProcessorH
#define MewsDataProcessorH
#include "PaymentTransaction.h"
#include "MewsDataObjects.h"
#include "Grids.hpp"
//---------------------------------------------------------------------------
class TMewsDataProcessor
{
    public :
    TMewsDataProcessor();
    ~TMewsDataProcessor();
    void GetRevenueCodesDetails(TStringGrid * StringGrid,Database::TDBTransaction &DBTransaction);
    std::vector<TAccountingCategory> GetCategoriesFromDB(Database::TDBTransaction &DBTransaction);
    std::vector<TAccountingCategoriesMapping> GetMewsCategoriesList(Database::TDBTransaction &DBTransaction);
    void UpdateOutlets(std::vector<TOutlet> outlets,Database::TDBTransaction &DBTransaction);
    void UpdateServices(std::vector<TServiceMews> services,Database::TDBTransaction &DBTransaction);
    void UpdateSpaces(TSpaceDetails spaces,Database::TDBTransaction &DBTransaction);
    void UpdateCategories(std::vector<TAccountingCategory> categories,Database::TDBTransaction &DBTransaction);
    void UpdateCategories(int categoryKey, UnicodeString name, bool isDeleted,
                                            Database::TDBTransaction &DBTransaction);
    void InitializeMewsCategories(Database::TDBTransaction &DBTransaction);
    std::map<int,UnicodeString> GetFreshMenumateCategories(Database::TDBTransaction &DBTransaction);
    void UpdateMewsMapToMMCategory(int key,UnicodeString code,UnicodeString id,Database::TDBTransaction &DBTransaction);
    AnsiString GetMewsName(Database::TDBTransaction &DBTransaction, UnicodeString code, int identifier);
    std::map<AnsiString,AnsiString> GetAllMewsDetailsFromDB(Database::TDBTransaction &DBTransaction,int identifier);
    TCustomerSearch CreateInquiryForRoom(UnicodeString queryString,bool isSpace);
};
#endif
