//---------------------------------------------------------------------------

#ifndef ManagerCloudSyncH
#define ManagerCloudSyncH
#include "LoyaltyMateOperationDialogBox.h"
//---------------------------------------------------------------------------
class TManagerCloudSync
{
   public:
    void SyncCompanyDetails();
    void CheckSyndCodes();
   private:
    TfrmLoyaltyMateOperationDialogBox* _lmOperationDialogBox;
    void __fastcall loyaltyMateOperationCompleted(TObject* sender);

};
#endif
