//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerCloudSync.h"
#include "MM_DBCore.h"
#include "ManagerLoyaltyMate.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "DeviceRealTerminal.h"
#include "ManagerSyndCode.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
void __fastcall TManagerCloudSync::loyaltyMateOperationCompleted(TObject* sender)
{
    _lmOperationDialogBox->Close();
}

void TManagerCloudSync::SyncCompanyDetails()
{
    TManagerSyndCode managerSyndCode;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    managerSyndCode.Initialise(DBTransaction);
    TSyndCode syndicateCode =  managerSyndCode.GetDefaultSyndCode();
    DBTransaction.Commit();

    if(syndicateCode.Valid())
     {
        // initiate loyaltymate member create thread and create member
        TLoyaltyMateSyncThread* syncThread = new TLoyaltyMateSyncThread(syndicateCode);
        syncThread->OnTerminate = loyaltyMateOperationCompleted;
        syncThread->FreeOnTerminate = true;
        syncThread->Start();

        // display dialog box
        _lmOperationDialogBox = new TfrmLoyaltyMateOperationDialogBox(Screen->ActiveForm);

        _lmOperationDialogBox->OperationDescription = "Syncing Company Details with server...Please Wait.";
        _lmOperationDialogBox->OperationTitle = "LoyaltyMate Operation";
        _lmOperationDialogBox->PreventCancelOperation = true;
        _lmOperationDialogBox->ShowModal();

        bool result = syncThread->OperationSuccessful;

        if(!result)
            MessageBox(syncThread->ErrorMessage,"Failed to perform sync operation", MB_ICONERROR + MB_OK);

        // cleanup
        delete _lmOperationDialogBox;
    }

}