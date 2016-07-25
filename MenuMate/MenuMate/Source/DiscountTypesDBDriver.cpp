//---------------------------------------------------------------------------


#pragma hdrstop

#include "DiscountTypesDBDriver.h"
#include "DiscountTypes.h"
//---------------------------------------------------------------------------

void TDiscountTypesDBDriver::LoadAllDiscountTypes(TList* types)
{

    Database::TDBTransaction transaction(TDeviceRealTerminal::Instance().DBControl);
    transaction.StartTransaction();
    _loadAllDiscountTypes(transaction, types);
};
//---------------------------------------------------------------------------

void TDiscountTypesDBDriver::_loadAllDiscountTypes(Database::TDBTransaction &transaction, TList* types)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "SELECT DISCOUNT_KEY, NAME FROM DISCOUNTS";
    query->ExecQuery();

    while(!query->Eof)
    {
        TDiscountTypes* type = new TDiscountTypes(
                                        query->FieldByName("DISCOUNT_KEY")->AsInteger,
                                        query->FieldByName("NAME")->AsString);
        types->Add(type);
        query->Next();
    }
};
//---------------------------------------------------------------------------
#pragma package(smart_init)
