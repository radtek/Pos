//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerPMS.h"
#include <System.hpp>
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "PhoenixHotelSystem.h"
#include "ManagerSiHot.h"
#include "ManagerOraclePMS.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerPMS::TManagerPMS()
{
}
TManagerPMS::~TManagerPMS()
{
}
void TManagerPMS::Initialize(Database::TDBControl &inDBControl, int pmsIdentifier)
{
    switch(pmsIdentifier)
    {
        case 2:
        {
            TDeviceRealTerminal::Instance().BasePMS.reset(new TManagerSiHot());
            break;
        }
        default:
        {
             TDeviceRealTerminal::Instance().BasePMS.reset(new TPhoenixHM());
             break;
        }
        case 3:
        {

             TDeviceRealTerminal::Instance().BasePMS.reset(new TManagerOraclePMS());
             break;
        }
    }
}
