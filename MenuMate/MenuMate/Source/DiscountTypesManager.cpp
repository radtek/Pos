//---------------------------------------------------------------------------


#pragma hdrstop

#include "DiscountTypesManager.h"
#include "DiscountTypesDBDriver.h"
//---------------------------------------------------------------------------

TDiscountTypesManager::TDiscountTypesManager()
{
    _driver.reset(new TDiscountTypesDBDriver());
}
//---------------------------------------------------------------------------
void TDiscountTypesManager::LoadAllDiscountTypes(TList* types)
{
    _driver->LoadAllDiscountTypes(types);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
