//---------------------------------------------------------------------------

#ifndef MallExportAyalaManagerH
#define MallExportAyalaManagerH

#include "MallExportIMallManager.h"
#include <vcl.h>
//---------------------------------------------------------------------------

class TMallExportAyalaManager : public TMallExportIMallManager
{
public:
    TMallExportAyalaManager();
    virtual __fastcall ~TMallExportAyalaManager();

protected:
    void initMallExport();
};

//---------------------------------------------------------------------------
#endif
