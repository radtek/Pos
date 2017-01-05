//---------------------------------------------------------------------------

#ifndef MallExportTextFileH
#define MallExportTextFileH
#include "IExporterInterface.h"
//---------------------------------------------------------------------------
class TMallExportTextFile: public IExporterInterface
{
    private:

    //Write File According to File type index 1 is For Daily Sales File //2 For Hourly //3 for invoce File
    void WriteFileAccordingToIndex(TMallExportPrepareData preparedData, std::map<int,list<TMallExportSettings> >::iterator itSettings,
                            std::map<int,list<TMallExportSalesData> >::iterator itSalesData, std::map<int,UnicodeString>::iterator itFileName, int index);
    public:
    TMallExportTextFile();

    //Get Preapared Data For Writing
    void WriteToFile(TMallExportPrepareData preparedData);
};
#endif