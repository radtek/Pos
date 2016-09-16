//---------------------------------------------------------------------------

#ifndef MallExportFederalLandManagerH
#define MallExportFederalLandManagerH

#include "MallExportIMallManager.h"
#include <vcl.h>
//---------------------------------------------------------------------------

class TMallExportFederalLandManager : public TMallExportIMallManager
{
public:
    TMallExportFederalLandManager();
    virtual __fastcall ~TMallExportFederalLandManager();

protected:
    void initMallExport();
};

#endif
