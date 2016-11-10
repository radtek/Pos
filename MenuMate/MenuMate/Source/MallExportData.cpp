//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportData.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

 void TMallExportData::SetMallSettings(std::list<TMallExportSettings> mallSettings)
{
    _mallSettings = mallSettings;
}
//-----------------------------------------------------------------------------
 void TMallExportData::SetMallExportSalesData(std::list<TMallExportSalesData> salesData)
{
    _salesData = salesData;
}
//-----------------------------------------------------------------------------