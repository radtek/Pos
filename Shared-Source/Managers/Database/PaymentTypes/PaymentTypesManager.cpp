//---------------------------------------------------------------------------


#pragma hdrstop

#include "PaymentTypesManager.h"
#include "PaymentTypesDBDriver.h"

//---------------------------------------------------------------------------
TPaymentTypesManager::TPaymentTypesManager()
{
    _driver.reset(new TPaymentTypesDBDriver());
}
//---------------------------------------------------------------------------
void TPaymentTypesManager::LoadAllPaymentTypes(TList* types)
{
    _driver->LoadAllPaymentTypes(types);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
