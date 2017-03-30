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
    static std::map<int, std::map<int, UnicodeString> > LoadAssignedItemsBySalesType();

    //Assign Items to Sales Type Group
    static void SaveAssignedItemsToSalesTYpeGroup(std::map<int, std::map<int, UnicodeString> > &assignedItems);

    //Add New Sales Type
    static void SaveSalesType(UnicodeString name, UnicodeString code);

    //Delete SalesType
    static void DeleteSalesType(int id);

    //Check Whether entered sales type code already exist.
    static bool IsSalesTypeCodeExist(UnicodeString code);

    //Load Sales Type for Editing
    static void LoadSelectedSalesType(int id, UnicodeString &code, UnicodeString &name);

    //Update Existing sales type
    static void UpdateSalesType(int id, UnicodeString name, UnicodeString code);

};
#endif
