//---------------------------------------------------------------------------

#ifndef MallExportSalesTypeH
#define MallExportSalesTypeH

#include <System.hpp>

#define FOOD_SALES_TYPE "Food"
#define NONFOOD_SALES_TYPE "Non-Food"
#define GROCERIES_SALES_TYPE "Groceries"
#define MEDICINES_SALES_TYPE "Medicines"
#define OTHERS_SALES_TYPE "Others"
//---------------------------------------------------------------------------

class TMallExportSalesType
{
    public:
        int DBKey;
        UnicodeString Name;
        TMallExportSalesType(int dbKey);
        TMallExportSalesType(int dbKey, UnicodeString name);
};

class TMallExportSalesTypeGroup
{
    public:
        TMallExportSalesTypeGroup(int dbkey);
        TMallExportSalesTypeGroup(int dbKey, UnicodeString name, bool isDefault);
        int DBKey;
        UnicodeString Name;
        bool IsDefault;
        int SaleTypeGroup;
};
#endif
