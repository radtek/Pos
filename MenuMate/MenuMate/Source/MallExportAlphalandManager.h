//---------------------------------------------------------------------------

#ifndef MallExportAlphalandManagerH
#define MallExportAlphalandManagerH
//---------------------------------------------------------------------------

#include "MallExportIMallManager.h"
#include <vcl.h>
//---------------------------------------------------------------------------

class TMallExportAlphalandManager : public TMallExportIMallManager
{
public:
    TMallExportAlphalandManager();
    virtual __fastcall ~TMallExportAlphalandManager();

protected:
    void initMallExport();
};

//---------------------------------------------------------------------------
#endif
