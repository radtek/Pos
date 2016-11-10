//---------------------------------------------------------------------------

#ifndef MallExportH
#define MallExportH
#include "MallExportInterface.h"
//---------------------------------------------------------------------------
class TMallExport: public TMallExportInterface
{
    protected:
    virtual void PrepareDataForDatabase() = 0;
    virtual void PrepareDataForExport() = 0;
    virtual void CreateExportMedium() = 0;
}
#endif
