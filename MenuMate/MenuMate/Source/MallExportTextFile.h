//---------------------------------------------------------------------------

#ifndef MallExportTextFileH
#define MallExportTextFileH
#include "IExporterInterface.h"
//---------------------------------------------------------------------------
class TMallExportTextFile: public IExporterInterface
{
    public:
    void WriteToFile(TMallExportPrepareData preparedData);
};
#endif
