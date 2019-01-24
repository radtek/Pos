//---------------------------------------------------------------------------


#pragma hdrstop

#include "SyndCodeController.h"
#include "SyndCodeGui.h"
#include "MMMessageBox.h"
#include "DeviceRealTerminal.h"
#include "DBRegistration.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TSyndCodeController::TSyndCodeController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction,TManagerSyndCode &inManagerSyndCode) :
ManagerSyndCode(inManagerSyndCode),
frmListManager(new TfrmListManager(inDisplayOwner)),
DBTransaction(inDBTransaction)
{
   DisplayOwner = inDisplayOwner;
}

void TSyndCodeController::Run()
{
    ManagerSyndCode.Initialise(DBTransaction);
    frmListManager->Add.RegisterForEvent(OnAdd);
    frmListManager->Edit.RegisterForEvent(OnEdit);
    frmListManager->Delete.RegisterForEvent(OnDelete);
    frmListManager->Close.RegisterForEvent(OnClose);
    PopulateListManager();
    frmListManager->SetCaption("Syndicate Codes");
    frmListManager->ShowModal();
    ValidateSyndCodes();
}

void TSyndCodeController::ValidateSyndCodes()
{
    AnsiString errorMessage = "";
    bool syndCodesValid = ManagerSyndCode.ValidateSyndCodes(errorMessage);
    if(syndCodesValid)
    {
       frmListManager->Add.DeregisterForEvent(OnAdd);
       frmListManager->Edit.DeregisterForEvent(OnEdit);
       frmListManager->Delete.DeregisterForEvent(OnDelete);
       frmListManager->Close.DeregisterForEvent(OnClose);
       MessageBox("You will need to restart MenuMate for this to take effect.", "Restart Required", MB_OK + MB_ICONINFORMATION);
    }
    else
    {
       MessageBox(errorMessage, "Warning", MB_OKCANCEL);
       frmListManager->ShowModal();
       ValidateSyndCodes();
    }
}

void TSyndCodeController::OnClose(int SyndKey, int ColIndex)
{
	frmListManager->ModalResult = mrOk;
}

void TSyndCodeController::OnAdd(int SyndKey, int ColIndex)
{
   std::auto_ptr<TfrmSyndCodeGui> frmSyndCodeGui(new TfrmSyndCodeGui(DisplayOwner,ManagerSyndCode));
   frmSyndCodeGui->SyndCode = TSyndCode();
   TModalResult Result = frmSyndCodeGui->ShowModal();
   if(Result == mrOk)
   {
      ManagerSyndCode.AddCode(DBTransaction,frmSyndCodeGui->SyndCode);
      PopulateListManager();
   }
}

void TSyndCodeController::OnEdit(int SyndKey, int ColIndex)
{
   std::auto_ptr<TfrmSyndCodeGui> frmSyndCodeGui(new TfrmSyndCodeGui(DisplayOwner,ManagerSyndCode));
   frmSyndCodeGui->SyndCode = ManagerSyndCode.SyndCodeByKey(SyndKey);
   TModalResult Result = frmSyndCodeGui->ShowModal();
   if(Result == mrOk)
   {

      if(ManagerSyndCode.CheckIfSynCodeEnabled(frmSyndCodeGui->SyndCode.SyndCodeKey) && !(frmSyndCodeGui->SyndCode.Enabled))
      {   
        //Unsetting IsRegistrationVerified Flag To Unregister POS
        if(TGlobalSettings::Instance().IsRegistrationVerified)
            TDBRegistration::UpdateIsRegistrationVerifiedFlag(DBTransaction, false);
      }
      ManagerSyndCode.UpdateCode(DBTransaction,frmSyndCodeGui->SyndCode);

      PopulateListManager();
   }
}

void TSyndCodeController::OnDelete(int SyndKey, int ColIndex)
{

   if (MessageBox("Are you sure you wish to remove this syndicate code?",
      "Warning",
      MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
   {
      ManagerSyndCode.RemoveCode(DBTransaction,SyndKey);
      PopulateListManager();
   }
}

void TSyndCodeController::PopulateListManager()
{
   try
   {
      frmListManager->sgDisplay->Cols[0]->Clear();
      frmListManager->sgDisplay->Cols[1]->Clear();
      frmListManager->sgDisplay->ColCount = 0;
      frmListManager->sgDisplay->RowCount = 0;
      frmListManager->sgDisplay->ColCount = 2;
      frmListManager->sgDisplay->RowCount = ManagerSyndCode.Size();
      int i = 0;
      for ( ManagerSyndCode.First(false) ; !ManagerSyndCode.Eof() ; ManagerSyndCode.Next(false), i++)
      {
		 TSyndCode SyndCode = ManagerSyndCode.SyndCode();
		 frmListManager->sgDisplay->Cols[0]->AddObject(SyndCode.Name,(TObject *)SyndCode.SyndCodeKey);
		 frmListManager->sgDisplay->Cols[1]->AddObject(SyndCode.DefaultEncryptCode ? _T("Default") : _T("NA"),(TObject *)SyndCode.SyndCodeKey);
      }
   }
	catch(Exception &E)
	{
		ShowMessage(E.Message);
	}
}

