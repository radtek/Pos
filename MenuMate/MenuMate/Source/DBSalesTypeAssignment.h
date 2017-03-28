//---------------------------------------------------------------------------

#ifndef DBSalesTypeAssignmentH
#define DBSalesTypeAssignmentH
#include<map.h>
#include<System.hpp>
//---------------------------------------------------------------------------

class TDBSalesTypeAssignment
{
    public:

    //Load All Types Of Sales Group
    static std::map<int, UnicodeString> LoadAllSalesTypes();

    //Load All Items into map
    static std::map<int, UnicodeString> LoadAllItems();

    //Load Items Which are assigned to sales Type
    static std::map<int, UnicodeString> LoadAssignedItemsBySalesType();

    //Assign Items to Sales Type Group
    static void SaveAssignedItemsToSalesTYpeGroup(std::map<int, std::map<int, UnicodeString> >);

    //Add New Sales Type
    static void SaveSalesType(UnicodeString name, UnicodeString code);

    //Edit Existing Sales Type
    static void EditSalesType(UnicodeString name, UnicodeString code);

    //Delete SalesType
    static void DeleteSalesType(int id);

    //Check Whether entered sales type code already exist.
    static bool IsSalesTypeCodeExist(UnicodeString code);

};
#endif
