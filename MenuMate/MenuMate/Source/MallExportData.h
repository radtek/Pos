//---------------------------------------------------------------------------

#ifndef MallExportDataH
#define MallExportDataH
#include "MallExportSettings.h"
#include <list>
#include "MallExportSalesData.h"
#include <map.h>
//---------------------------------------------------------------------------
class TMallExportPrepareData
{
    private:
    std::map<int,list<TMallExportSettings> > _mallSettings;
    std::map<int,list<TMallExportSalesData> > _salesData;
    std::map<int,UnicodeString> _fileName;

    void SetMallSettings(std::map<int,list<TMallExportSettings> > mallSettings);
    void SetMallExportSalesData(std::map<int,list<TMallExportSalesData> > salesData);
    void SetFileName(std::map<int,UnicodeString> fileName);

    public:
    TMallExportPrepareData();
     __property std::map<int,list<TMallExportSettings> > MallSettings = {read = _mallSettings, write = SetMallSettings};
     __property std::map<int,list<TMallExportSalesData> > SalesData = {read = _salesData, write = SetMallExportSalesData};
     __property std::map<int,UnicodeString> FileName = {read = _fileName, write = SetFileName};
};
#endif
