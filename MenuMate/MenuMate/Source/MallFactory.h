//---------------------------------------------------------------------------

#ifndef MallFactoryH
#define MallFactoryH
#include "EstanciaMall.h"
#include "DeanAndDelucaMall.h"
#include "Eviamall.h"
#include "SouthBeachMall.h"
//---------------------------------------------------------------------------

class TMallFactory
{
public:
    static TMallExport* GetMallType();
};
#endif
