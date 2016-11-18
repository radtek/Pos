//---------------------------------------------------------------------------

#ifndef MallExportTextFileH
#define MallExportTextFileH
#include "IExporterInterface.h"
//---------------------------------------------------------------------------
class TMallExportTextFile: public IExporterInterface
{
    private:
        void WriteFileAccordingToIndex(TMallExportPrepareData preparedData, std::map<int,list<TMallExportSettings> >::iterator itSettings, std::map<int,list<TMallExportSalesData> >::iterator itSalesData, int index);
    public:
    TMallExportTextFile();
    void WriteToFile(TMallExportPrepareData preparedData);
};
#endif
