//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportData.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

 void TMallExportPrepareData::SetMallSettings(std::map<int,list<TMallExportSettings> > mallSettings)
{
    _mallSettings = mallSettings;
}
//-----------------------------------------------------------------------------
 void TMallExportPrepareData::SetMallExportSalesData(std::map<int,list<TMallExportSalesData> > salesData)
{
    _salesData = salesData;
}
//-----------------------------------------------------------------------------
TMallExportPrepareData::TMallExportPrepareData()
{
}
