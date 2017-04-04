//---------------------------------------------------------------------------

#ifndef DBSalesTypeAssignmentH
#define DBSalesTypeAssignmentH
#include "DeviceRealTerminal.h"
#include "SalesTypeItemDetails.h"
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

    //this function will receive only items which are eiether added to selected sales type or removed from selected sales type..
    //it is added or removed can be find using enum' status which is property of TItemDetails structure.
    static void SaveItemRelationWithSalesType(std::map<int, std::map<int, TItemDetails> > modifieldItemsWithSalesType);

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

private:

     // this will insert record into db. it will be called from SaveItemRelationWithSalesType function if enum 's status is added.
    static void InsertRecordInToDB(Database::TDBTransaction &dbTransaction, int saleTypeId, int itemId);

    // this will delete record from db. it will be called from SaveItemRelationWithSalesType function if enum 's status is removed.
    static void DeleteRecordFromDB(Database::TDBTransaction &dbTransaction, int saleTypeId, int itemId);

};
#endif
