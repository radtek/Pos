//---------------------------------------------------------------------------

#ifndef ManagerTierLevelH
#define ManagerTierLevelH
#include "Membership.h"
#include "LogsDB.h"
#include "MM_DBCore.h"
#include "DeviceRealTerminal.h"
#include "LoyaltyMateInterface.h"
#include "SalesforceAuthInfo.h"
#include "Comms.h"
#include "LoyaltyPrinting.h"
#include "LoyaltyMateUtilities.h"
#include "GlobalSettings.h"
#include "DBTierLevel.h"
#include "LoyaltyMateOperationDialogBox.h"
#include "ManagerSyndCode.h"
class TManagerTierLevel
{
   public:
    bool createTierOnLoyaltyMate(TTierLevel* tierLevel);
    bool updateTierOnLoyaltyMate(TTierLevel* tierLevel);
    void SyncTierLevels();
    void DeleteTierLevel(TTierLevel* tierLevel);
   private:
    TfrmLoyaltyMateOperationDialogBox* _lmOperationDialogBox;
    void __fastcall loyaltyMateTierOperationCompleted(TObject* sender);
    bool doTierOperation(TTierLevel* tierLevel,bool isUpdate = false);

};
//---------------------------------------------------------------------------
#endif
