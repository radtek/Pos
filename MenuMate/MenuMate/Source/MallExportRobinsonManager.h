//---------------------------------------------------------------------------

#ifndef MallExportRobinsonManagerH
#define MallExportRobinsonManagerH

#include "MallExportIMallManager.h"
#include <vcl.h>
//---------------------------------------------------------------------------

class TMallExportRobinsonManager : public TMallExportIMallManager
{
public:
    TMallExportRobinsonManager();
    virtual __fastcall ~TMallExportRobinsonManager();

protected:
    void initMallExport();
};
//---------------------------------------------------------------------------
#endif
