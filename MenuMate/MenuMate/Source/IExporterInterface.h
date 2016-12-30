//---------------------------------------------------------------------------

#ifndef IExporterInterfaceH
#define IExporterInterfaceH
#include "MallExportData.h"
//---------------------------------------------------------------------------
class IExporterInterface
{
    public:
    virtual void WriteToFile(TMallExportPrepareData preparedData) = 0;
};
#endif
