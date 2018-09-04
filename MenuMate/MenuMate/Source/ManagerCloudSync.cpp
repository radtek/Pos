//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerCloudSync.h"
#include "MM_DBCore.h"
#include "ManagerLoyaltyMate.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "DeviceRealTerminal.h"
#include "ManagerSyndCode.h"
#include "SyndCodeController.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
void __fastcall TManagerCloudSync::loyaltyMateOperationCompleted(TObject* sender)
{
    _lmOperationDialogBox->Close();
}

void TManagerCloudSync::CheckSyndCodes()
{
    TManagerSyndCode managerSyndCode = TDeviceRealTerminal::Instance().ManagerMembership->GetSyndicateCodeManager();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    AnsiString errorMessage = "";
    bool syndCodesValid = managerSyndCode.ValidateSyndCodes(errorMessage);
    if(!syndCodesValid)
    {
       MessageBox(errorMessage, "Warning", MB_OK);
       TSyndCodeController SyndCodeController(Screen->ActiveForm,DBTransaction,managerSyndCode);
       SyndCodeController.Run();
    }
   DBTransaction.Commit();
}

bool TManagerCloudSync::SyncCompanyDetails()
{
    bool result = false;
    TManagerSyndCode managerSyndCode = TDeviceRealTerminal::Instance().ManagerMembership->GetSyndicateCodeManager();
    TSyndCode syndicateCode =  managerSyndCode.GetCommunicationSyndCode();
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

        result = syncThread->OperationSuccessful;

        if(!result)
            MessageBox(syncThread->ErrorMessage,"Failed to perform sync operation", MB_ICONERROR + MB_OK);

        // cleanup
        delete _lmOperationDialogBox;
    }
    return result;
}
