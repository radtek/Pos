//---------------------------------------------------------------------------

#ifndef MallExportMegaworldManagerH
#define MallExportMegaworldManagerH
//---------------------------------------------------------------------------

#include "MallExportIMallManager.h"
#include <vcl.h>
//---------------------------------------------------------------------------

class TMallExportMegaworldManager : public TMallExportIMallManager
{
public:
    TMallExportMegaworldManager();
    virtual __fastcall ~TMallExportMegaworldManager();

protected:
    void initMallExport();
};

//---------------------------------------------------------------------------
#endif
