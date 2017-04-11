//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallFactory.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TMallExport* TMallFactory::GetMallType()
{
    TMallExport* mallType;
    int mallID = TGlobalSettings::Instance().mallInfo.MallId;

    switch(mallID)
    {
        case 1:
            mallType = new TEstanciaMall();
            break;
        case 2:
            mallType = new TDeanAndDelucaMall();
            break;
        default:
           mallType = NULL;
    }
    return mallType;
}
