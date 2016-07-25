//---------------------------------------------------------------------------

#ifndef MallExportShangrilaManagerH
#define MallExportShangrilaManagerH

#include "MallExportIMallManager.h"
#include <vcl.h>
//---------------------------------------------------------------------------

class TMallExportShangrilaManager : public TMallExportIMallManager
{
public:
    TMallExportShangrilaManager();
    virtual __fastcall ~TMallExportShangrilaManager();

protected:
    void initMallExport();
};
//---------------------------------------------------------------------------
#endif
