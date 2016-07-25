//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportSalesType.h"

//---------------------------------------------------------------------------
TMallExportSalesType::TMallExportSalesType(int dbKey){
    DBKey = dbKey;
}

TMallExportSalesType::TMallExportSalesType(int dbKey, UnicodeString name){
    DBKey = dbKey;
    Name = name;
}

TMallExportSalesTypeGroup::TMallExportSalesTypeGroup(int dbKey)
{
    DBKey = dbKey;
}

TMallExportSalesTypeGroup::TMallExportSalesTypeGroup(int dbKey, UnicodeString name, bool isDefault)
{
    DBKey = dbKey;
    Name = name;
    IsDefault = isDefault;
}
#pragma package(smart_init)
