//---------------------------------------------------------------------------

#ifndef MallExportCaptialandManagerH
#define MallExportCaptialandManagerH

#include "MallExportIMallManager.h"
#include <vcl.h>
//---------------------------------------------------------------------------

class TMallExportCapitalandManager : public TMallExportIMallManager
{
public:
    TMallExportCapitalandManager();
    virtual __fastcall ~TMallExportCapitalandManager();

protected:
    void initMallExport();
};

//---------------------------------------------------------------------------
#endif
