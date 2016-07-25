//---------------------------------------------------------------------------


#pragma hdrstop

#include "PaymentTypesDBDriver.h"
#include "PaymentType.h"

//---------------------------------------------------------------------------
void TPaymentTypesDBDriver::LoadAllPaymentTypes(TList* types)
{

    Database::TDBTransaction transaction(TDeviceRealTerminal::Instance().DBControl);
    transaction.StartTransaction();
    _loadAllPaymentTypes(transaction, types);
}

void TPaymentTypesDBDriver::_loadAllPaymentTypes(Database::TDBTransaction &transaction, TList* types)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "SELECT PAYMENT_KEY, PAYMENT_NAME FROM PAYMENTTYPES";
    query->ExecQuery();

    while(!query->Eof)
    {
        TPaymentType* type = new TPaymentType(
                                        query->FieldByName("PAYMENT_KEY")->AsInteger,
                                        query->FieldByName("PAYMENT_NAME")->AsString);
        types->Add(type);
        query->Next();
    }
}
#pragma package(smart_init)
