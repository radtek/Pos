//---------------------------------------------------------------------------

#ifndef ManagerPMSH
#define ManagerPMSH
//#include "BasePMS.h"
#include "DeviceRealterminal.h"
#include "MM_DBCore.h"
#include <memory>
//---------------------------------------------------------------------------

class TManagerPMS
{
    private:

    public:
       TManagerPMS();
       ~TManagerPMS();
        void Initialize(Database::TDBControl &inDBControl, int pmsIdentifier);
};
#endif
