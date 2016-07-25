//---------------------------------------------------------------------------

#ifndef MallExportSMManagerH
#define MallExportSMManagerH

#include "MallExportIMallManager.h"
#include <vcl.h>
//---------------------------------------------------------------------------

class TMallExportSMManager : public TMallExportIMallManager
{
public:
    TMallExportSMManager();
    virtual __fastcall ~TMallExportSMManager();

protected:
    void initMallExport();
};
#endif
