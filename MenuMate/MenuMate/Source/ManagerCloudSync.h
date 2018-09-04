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
   private:
    TfrmLoyaltyMateOperationDialogBox* _lmOperationDialogBox;
    void __fastcall loyaltyMateOperationCompleted(TObject* sender);

};
#endif
