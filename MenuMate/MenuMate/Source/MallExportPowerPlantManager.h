//---------------------------------------------------------------------------

#ifndef MallExportPowerPlantManagerH
#define MallExportPowerPlantManagerH

#include "MallExportIMallManager.h"
#include <vcl.h>
//---------------------------------------------------------------------------

class TMallExportPowerPlantManager : public TMallExportIMallManager
{
public:
    TMallExportPowerPlantManager();
    virtual __fastcall ~TMallExportPowerPlantManager();

protected:
    void initMallExport();
};
//---------------------------------------------------------------------------
#endif
