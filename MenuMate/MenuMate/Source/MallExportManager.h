//---------------------------------------------------------------------------

#ifndef MallExportManagerH
#define MallExportManagerH

#include "MallExportIMallManager.h"
//---------------------------------------------------------------------------
enum ExportTiming {ZED, TRANSACTION, TIMING};
//---------------------------------------------------------------------------

class TMallExportManager
{
public:

    TMallExportIMallManager* IMallManager;

    __fastcall TMallExportManager();
};
#endif
