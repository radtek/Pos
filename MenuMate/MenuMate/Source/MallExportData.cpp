//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportData.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

 void TMallExportPrepareData::SetMallSettings(std::list<TMallExportSettings> mallSettings)
{
    _mallSettings = mallSettings;
}
//-----------------------------------------------------------------------------
 void TMallExportPrepareData::SetMallExportSalesData(std::list<TMallExportSalesData> salesData)
{
    _salesData = salesData;
}
//-----------------------------------------------------------------------------