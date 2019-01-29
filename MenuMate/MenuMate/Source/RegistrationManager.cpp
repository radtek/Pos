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

