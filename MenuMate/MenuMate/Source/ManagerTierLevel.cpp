//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerTierLevel.h"
#include "ManagerLoyaltyMate.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

bool TManagerTierLevel::createTierOnLoyaltyMate(TTierLevel* tierLevel)
{
  doTierOperation(tierLevel);
}

bool TManagerTierLevel::updateTierOnLoyaltyMate(TTierLevel* tierLevel)
{
   doTierOperation(tierLevel,true);
}

bool TManagerTierLevel::doTierOperation(TTierLevel* tierLevel,bool isUpdate)
{
    bool result = false;
    bool running = false;
    TManagerSyndCode managerSyndCode;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    managerSyndCode.Initialise(DBTransaction);
    TSyndCode syndicateCode =  managerSyndCode.GetDefaultSyndCode();
    DBTransaction.Commit();
    // initiate loyaltymate member create thread and create member
    TLoyaltyMateTierThread* tierThread = new TLoyaltyMateTierThread(syndicateCode,tierLevel);
    tierThread->OnTerminate = loyaltyMateTierOperationCompleted;
    tierThread->FreeOnTerminate = true;
    tierThread->IsUpdate = isUpdate;
    tierThread->Start();

    // display dialog box
    _lmOperationDialogBox = new TfrmLoyaltyMateOperationDialogBox(Screen->ActiveForm);

    _lmOperationDialogBox->OperationDescription = "Performing tier level operation on LoyaltyMate...Please Wait.";
    _lmOperationDialogBox->OperationTitle = "LoyaltyMate Operation";
    _lmOperationDialogBox->PreventCancelOperation = true;
    _lmOperationDialogBox->ShowModal();

    result = tierThread->OperationSuccessful;

    if(!result)
        MessageBox(tierThread->ErrorMessage,"Failed to perform tier level operation", MB_ICONERROR + MB_OK);

    // cleanup
    delete _lmOperationDialogBox;
    return result;
}

void __fastcall TManagerTierLevel::loyaltyMateTierOperationCompleted(TObject* sender)
{
    _lmOperationDialogBox->Close();
}

void TManagerTierLevel::SyncTierLevels()
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
        TLoyaltyMateTierSyncThread* tierThread = new TLoyaltyMateTierSyncThread(syndicateCode);
        tierThread->OnTerminate = loyaltyMateTierOperationCompleted;
        tierThread->FreeOnTerminate = true;
        tierThread->Start();

        // display dialog box
        _lmOperationDialogBox = new TfrmLoyaltyMateOperationDialogBox(Screen->ActiveForm);

        _lmOperationDialogBox->OperationDescription = "Syncing Tier Levels with cloud...Please Wait.";
        _lmOperationDialogBox->OperationTitle = "LoyaltyMate Operation";
        _lmOperationDialogBox->PreventCancelOperation = true;
        _lmOperationDialogBox->ShowModal();

        bool result = tierThread->OperationSuccessful;

        if(!result)
            MessageBox(tierThread->ErrorMessage,"Failed to perform tier level operation", MB_ICONERROR + MB_OK);

        // cleanup
        delete _lmOperationDialogBox;
    }

}

void TManagerTierLevel::DeleteTierLevel(TTierLevel* tierLevel)
{
    bool result = false;
    bool running = false;
    TManagerSyndCode managerSyndCode;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    managerSyndCode.Initialise(DBTransaction);
    TSyndCode syndicateCode =  managerSyndCode.GetDefaultSyndCode();
    DBTransaction.Commit();
    // initiate loyaltymate member create thread and create member
    TLoyaltyMateTierThread* tierThread = new TLoyaltyMateTierThread(syndicateCode,tierLevel);
    tierThread->OnTerminate = loyaltyMateTierOperationCompleted;
    tierThread->FreeOnTerminate = true;
    tierThread->IsDelete = true;
    tierThread->Start();

    // display dialog box
    _lmOperationDialogBox = new TfrmLoyaltyMateOperationDialogBox(Screen->ActiveForm);

    _lmOperationDialogBox->OperationDescription = "Performing tier level operation on LoyaltyMate...Please Wait.";
    _lmOperationDialogBox->OperationTitle = "LoyaltyMate Operation";
    _lmOperationDialogBox->PreventCancelOperation = true;
    _lmOperationDialogBox->ShowModal();

    result = tierThread->OperationSuccessful;

    if(!result)
        MessageBox(tierThread->ErrorMessage,"Failed to perform tier level operation", MB_ICONERROR + MB_OK);

    // cleanup
    delete _lmOperationDialogBox;
    //return result;
}