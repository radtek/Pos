//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBRegistration.h"
#include "RegistrationManager.h"
#include "RegistrationInterface.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TRegistrationManager::CheckRegistrationStatus()
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    try
    {
        AnsiString syndCode = "";

        if(TGlobalSettings::Instance().SiteID)
        {
            syndCode = TDBRegistration::GetSyndCode(dbTransaction);
        }

        if(syndCode.Trim() != "")
        {
            //Checking POS Resgistration Status
            if(TGlobalSettings::Instance().IsRegistrationVerified)
            {
                if(TGlobalSettings::Instance().IsCloudSyncRequired)
                {
                    if(UploadRegistrationInfo(dbTransaction, syndCode))
                        TDBRegistration::UpdateIsCloudSyncRequiredFlag(false);
                }
            }
            else
            {
                if(ValidateCompanyInfo(dbTransaction,syndCode, TGlobalSettings::Instance().SiteID))
                {    
                    TDBRegistration::UpdateIsRegistrationVerifiedFlag(dbTransaction, true);

                    if(UploadRegistrationInfo(dbTransaction, syndCode))
                        TDBRegistration::UpdateIsCloudSyncRequiredFlag(false);
                }

            }
        }
        dbTransaction.Commit();
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
        dbTransaction.Rollback();
        throw;
    }
}
//-----------------------------------------------------------------------
bool TRegistrationManager::UploadRegistrationInfo(Database::TDBTransaction &dbTransaction, AnsiString syndicateCode)
{
    bool retval = false;
    try
    {
        TMMProcessingState State(Screen->ActiveForm, "Depicting registration verification in process Please Wait...", "Registration verification");
        TDeviceRealTerminal::Instance().ProcessingController.Push(State);
        AnsiString ErrorMessage;
        TTerminalModel terminalInfo = TDBRegistration::GetTerminalInfo(dbTransaction);
        TRegistrationInterface* registrationInterface = new TRegistrationInterface();
        MMRegistrationServiceResponse createResponse = registrationInterface->UploadRegistrationInfo(terminalInfo, syndicateCode);
        TDeviceRealTerminal::Instance().ProcessingController.Pop();
        if(createResponse.IsSuccesful)
            retval = true;
        else
        {
            bool showMessage = true;
            if(createResponse.ResponseCode == AuthenticationFailed)
            {
                ErrorMessage = "Authentication failed with Registration Service.";
            }
            else if(createResponse.ResponseCode == NoNewSettingChange)
            {
                 showMessage = false;
                 ErrorMessage = "No new Setting found for Update.";
                 TDBRegistration::UpdateIsCloudSyncRequiredFlag(false);
            }
            else if(createResponse.ResponseCode == BadRequestError)
            {
                 ErrorMessage = "Bad Request Error.";
            }
            else if(createResponse.ResponseCode == SiteCodeNotExist)
            {
                 ErrorMessage = "Site Code doesn't exist.";
            }
            else if(createResponse.ResponseCode == SiteCodeInAcive)
            {
                 ErrorMessage = "Site Code inactive/not found.";
                 TDBRegistration::UpdateIsRegistrationVerifiedFlag(dbTransaction, false);
            }
            else
            {
                ErrorMessage = "Registration update failed.";
                TDBRegistration::UpdateIsRegistrationVerifiedFlag(dbTransaction, false);
            }
            if(showMessage)
                MessageBox(ErrorMessage,"Error", MB_OK + MB_ICONERROR);
        }
        delete registrationInterface;
        registrationInterface = NULL;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return retval;
}
//-----------------------------------------------------------------------
bool TRegistrationManager::ValidateCompanyInfo(Database::TDBTransaction &dbTransaction, AnsiString syndicateCode, int siteId)
{
    bool retval = false;
    try
    {
        TMMProcessingState State(Screen->ActiveForm, "Validating Company Info Please Wait...", "Validating Company Info");
        TDeviceRealTerminal::Instance().ProcessingController.Push(State);
        AnsiString ErrorMessage;
        TRegistrationInterface* registrationInterface = new TRegistrationInterface();
        MMRegistrationServiceResponse createResponse = registrationInterface->ValidateCompanyInfo(syndicateCode, siteId);
        TDeviceRealTerminal::Instance().ProcessingController.Pop();
        if(createResponse.IsSuccesful)
        {  
            retval = true;
            TGlobalSettings::Instance().CompanyName = createResponse.Message;
            TManagerVariable::Instance().SetDeviceStr(dbTransaction,vmCompanyName,TGlobalSettings::Instance().CompanyName);
            ResetHeaderFooter();
        }
        else
        {
            ErrorMessage = createResponse.Message;
            if((ErrorMessage.Pos("Site Code inactive/not found.") != 0)||
                (ErrorMessage.Pos("Menumate Registration is Failed. Please Enter correct Syndicate Code and Site Id")!= 0))
            {
                TDBRegistration::UpdateIsRegistrationVerifiedFlag(dbTransaction, false);
                MessageBox(ErrorMessage,"Registration Failed", MB_OK + MB_ICONERROR);
            }
            else
            {
                MessageBox(ErrorMessage,"Error", MB_OK + MB_ICONERROR);
            }
        }

        delete registrationInterface;
        registrationInterface = NULL;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return retval;
}
//---------------------------------------------------------------------------------------
void TRegistrationManager::ResetHeaderFooter()
{
    try
    {
        if (FileExists(ExtractFilePath(Application->ExeName) + RECEIPT_HEADER))
        {
            TGlobalSettings::Instance().Header->Clear();
            TGlobalSettings::Instance().Header->LoadFromFile(ExtractFilePath(Application->ExeName) + RECEIPT_HEADER);
        }
        if (FileExists(ExtractFilePath(Application->ExeName) + RECEIPT_PRELIM_HEADER))
        {
            TGlobalSettings::Instance().PHeader->Clear();
            TGlobalSettings::Instance().PHeader->LoadFromFile(ExtractFilePath(Application->ExeName) + RECEIPT_PRELIM_HEADER);
        }
        if (FileExists(ExtractFilePath(Application->ExeName) + RECEIPT_FOOTER))
        {
            TGlobalSettings::Instance().Footer->Clear();
            TGlobalSettings::Instance().Footer->LoadFromFile(ExtractFilePath(Application->ExeName) + RECEIPT_FOOTER);
        }

        if (FileExists(ExtractFilePath(Application->ExeName) + RECEIPT_ZEDHEADER))
        {
            TGlobalSettings::Instance().ZedHeader->Clear();
            TGlobalSettings::Instance().ZedHeader->LoadFromFile(ExtractFilePath(Application->ExeName) + RECEIPT_ZEDHEADER);
        }
        if (FileExists(ExtractFilePath(Application->ExeName) + RECEIPT_VOID_FOOTER))
        {
            TGlobalSettings::Instance().VoidFooter->Clear();
            TGlobalSettings::Instance().VoidFooter->LoadFromFile(ExtractFilePath(Application->ExeName) + RECEIPT_VOID_FOOTER);
        }
        if (FileExists(ExtractFilePath(Application->ExeName) + RECEIPT_SUBHEADER))
        {
            TGlobalSettings::Instance().SubHeader->Clear();
            TGlobalSettings::Instance().SubHeader->LoadFromFile(ExtractFilePath(Application->ExeName) + RECEIPT_SUBHEADER);
        }
        for(int i = 0; i < TGlobalSettings::Instance().Header->Count; i++)
        {
            UnicodeString str = "NOT FOR RESALE";
            if(TGlobalSettings::Instance().Header->Strings[i].Pos(str) != 0)
            {
                TGlobalSettings::Instance().Header->Clear();
                TGlobalSettings::Instance().Header->Add(TGlobalSettings::Instance().CompanyName);
                TGlobalSettings::Instance().Header->SaveToFile(ExtractFilePath(Application->ExeName) + RECEIPT_HEADER);
                break;
            }
        }
        for(int i = 0; i < TGlobalSettings::Instance().PHeader->Count; i++)
        {
            UnicodeString str = "NOT FOR RESALE";
            if(TGlobalSettings::Instance().PHeader->Strings[i].Pos(str) != 0)
            {
                TGlobalSettings::Instance().PHeader->Clear();
                TGlobalSettings::Instance().PHeader->Add(TGlobalSettings::Instance().CompanyName);
                TGlobalSettings::Instance().PHeader->SaveToFile(ExtractFilePath(Application->ExeName) + RECEIPT_PRELIM_HEADER);
                break;
            }
        }
        for(int i = 0; i < TGlobalSettings::Instance().Footer->Count; i++)
        {
            UnicodeString str = "NOT FOR RESALE";
            if(TGlobalSettings::Instance().Footer->Strings[i].Pos(str) != 0)
            {
                TGlobalSettings::Instance().Footer->Clear();
                TGlobalSettings::Instance().Footer->Add(TGlobalSettings::Instance().CompanyName);
                TGlobalSettings::Instance().Footer->SaveToFile(ExtractFilePath(Application->ExeName) + RECEIPT_FOOTER);
                break;
            }
        }
        for(int i = 0; i < TGlobalSettings::Instance().ZedHeader->Count; i++)
        {
            UnicodeString str = "NOT FOR RESALE";
            if(TGlobalSettings::Instance().ZedHeader->Strings[i].Pos(str) != 0)
            {
                TGlobalSettings::Instance().ZedHeader->Clear();
                TGlobalSettings::Instance().ZedHeader->Add(TGlobalSettings::Instance().CompanyName);
                TGlobalSettings::Instance().ZedHeader->SaveToFile(ExtractFilePath(Application->ExeName) + RECEIPT_ZEDHEADER);
                break;
            }
        }
        for(int i = 0; i < TGlobalSettings::Instance().VoidFooter->Count; i++)
        {
            UnicodeString str = "NOT FOR RESALE";
            if(TGlobalSettings::Instance().VoidFooter->Strings[i].Pos(str) != 0)
            {
                TGlobalSettings::Instance().VoidFooter->Clear();
                TGlobalSettings::Instance().VoidFooter->Add(TGlobalSettings::Instance().CompanyName);
                TGlobalSettings::Instance().VoidFooter->SaveToFile(ExtractFilePath(Application->ExeName) + RECEIPT_VOID_FOOTER);
                break;
            }
        }
        for(int i = 0; i < TGlobalSettings::Instance().SubHeader->Count; i++)
        {
            UnicodeString str = "NOT FOR RESALE";
            if(TGlobalSettings::Instance().SubHeader->Strings[i].Pos(str) != 0)
            {
                TGlobalSettings::Instance().SubHeader->Clear();
                TGlobalSettings::Instance().SubHeader->Add(TGlobalSettings::Instance().CompanyName);
                TGlobalSettings::Instance().SubHeader->SaveToFile(ExtractFilePath(Application->ExeName) + RECEIPT_SUBHEADER);
                break;
            }
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

