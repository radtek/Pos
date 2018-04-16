//---------------------------------------------------------------------------

#ifndef MallExportEviaSalFileH
#define MallExportEviaSalFileH
#include "IExporterInterface.h"


//---------------------------------------------------------------------------
class TMallExportSalFile: public IExporterInterface
{
    private:

    //Write File According to File type index 1 is For Daily Sales File //2 For Hourly //3 for invoce File
    void WriteFileAccordingToIndex(TMallExportPrepareData preparedData,
                            std::map<int,list<TMallExportSalesData> >::iterator itSalesData, std::map<int,UnicodeString>::iterator itFileName, int index);
    public:
    TMallExportSalFile();

    //Get Preapared Data For Writing
    void WriteToFile(TMallExportPrepareData preparedData);
};
#endif