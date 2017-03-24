//---------------------------------------------------------------------------

#ifndef MallFactoryH
#define MallFactoryH
#include "EstanciaMall.h"
#include "DeanAndDelucaMall.h"
//---------------------------------------------------------------------------

class TMallFactory
{
public:
    static TMallExport* GetMallType();
};
#endif
