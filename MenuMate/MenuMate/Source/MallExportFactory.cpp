//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportFactory.h"
#include "GlobalSettings.h"
#include "MallExportDefines.h"
#include "MallExportSMManager.h"
#include "MallExportRobinsonManager.h"
#include "MallExportAyalaManager.h"
#include "MallExportPowerPlantManager.h"
#include "MallExportCaptialandManager.h"
#include "MallExportAlphalandManager.h"
#include "MallExportMegaworldManager.h"
#include "MallExportShangrilaManager.h"
#include "MallExportFederalLandManager.h"
//---------------------------------------------------------------------------

__fastcall TMallExportFactory::TMallExportFactory()
{
}

TMallExportIMallManager* IMallManager;
TMallExportIMallManager *TMallExportFactory::CreateManager()
{
    switch(TGlobalSettings::Instance().MallIndex)
    {
        case SMMALL:
            IMallManager = new TMallExportSMManager();
            break;
        case ROBINSONMALL:
            IMallManager = new TMallExportRobinsonManager();
            break;
        case AYALAMALL:
            IMallManager = new TMallExportAyalaManager();
            break;
        case POWERPLANTMALL:
            IMallManager = new TMallExportPowerPlantManager();
            break;
        case CAPITALAND:
            IMallManager = new TMallExportCapitalandManager();
            break;
        case ALPHALANDMALL:
            IMallManager = new TMallExportAlphalandManager();
            break;
        case MEGAWORLDMALL:
            IMallManager = new TMallExportMegaworldManager();
            break;
        case SHANGRILAMALL:
            IMallManager = new TMallExportShangrilaManager();
            break;
        case DLFMALL:
            break;
        case FEDERALLANDMALL:
            IMallManager = new TMallExportFederalLandManager();
            break;

        default:
            break;
    }
    return IMallManager;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
