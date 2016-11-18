//---------------------------------------------------------------------------

#ifndef MallExportTextFileH
#define MallExportTextFileH
#include "IExporterInterface.h"
//---------------------------------------------------------------------------
class TMallExportTextFile: public IExporterInterface
{
    public:
    TMallExportTextFile();
    void WriteToFile(TMallExportPrepareData preparedData);
};
#endif
