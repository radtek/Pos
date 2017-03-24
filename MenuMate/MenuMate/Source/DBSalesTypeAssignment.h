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
    std::map<int, UnicodeString> LoadAllSalesType();

    //Load All Items into map
    std::map<int, UnicodeString> LoadAllItems();

    //Load Items Which are assigned to sales Type
    std::map<int, UnicodeString> LoadAssignedItemsBySalesType();

    //Assign Items to Sales Type Group
    void SaveAssignedItemsToSalesTYpeGroup(std::map<int, std::map<int, UnicodeString> >);

    //Add New Sales Type
    void AddSalesType(UnicodeString name, UnicodeString code);

    //Edit Existing Sales Type
    void EditSalesType(UnicodeString name, UnicodeString code);

    //Delete SalesType
    void DeleteSalesType(UnicodeString name, UnicodeString code);

};
#endif
