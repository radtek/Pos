//---------------------------------------------------------------------------

#ifndef MallExportDataH
#define MallExportDataH
#include "MallExportSettings.h"
#include <list>
#include "MallExportSalesData.h"
//---------------------------------------------------------------------------
class TMallExportData
{
    private:
    std::list<TMallExportSettings> _mallSettings;
    std::list<TMallExportSalesData> _salesData;

    void SetMallSettings(std::list<TMallExportSettings> mallSettings);
    void SetMallExportSalesData(std::list<TMallExportSalesData> salesData);

    public:
     __property std::list<TMallExportSettings> MallSettings ={read = _mallSettings, write = SetMallSettings};
     __property std::list<TMallExportSalesData> SalesData ={read = _salesData, write = SetMallExportSalesData};

};
#endif
