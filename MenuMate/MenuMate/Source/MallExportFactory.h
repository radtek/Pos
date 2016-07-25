//---------------------------------------------------------------------------

#ifndef MallExportFactoryH
#define MallExportFactoryH

#include "MallExportIMallManager.h"
//---------------------------------------------------------------------------

class TMallExportFactory
{
public:

    static TMallExportIMallManager *CreateManager();

    __fastcall TMallExportFactory();
};
#endif
