//---------------------------------------------------------------------------

#ifndef ManagerCloudSyncH
#define ManagerCloudSyncH
#include "LoyaltyMateOperationDialogBox.h"
//---------------------------------------------------------------------------
class TManagerCloudSync
{
   public:
    bool SyncCompanyDetails();
    void CheckSyndCodes();
    bool SyncOnlineOrderingDetails();
    bool UnsetSinalRConnectionStatus();
   private:
    TfrmLoyaltyMateOperationDialogBox* _lmOperationDialogBox;
    void __fastcall loyaltyMateOperationCompleted(TObject* sender);

};
#endif
