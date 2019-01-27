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
                    if(UploadRegistrationInfo(syndCode))
                        TDBRegistration::UpdateIsCloudSyncRequiredFlag(false);
                }
            }
            else
            {
                if(ValidateCompanyInfo(syndCode, TGlobalSettings::Instance().SiteID))
                {    
                    TDBRegistration::UpdateIsRegistrationVerifiedFlag(dbTransaction, true);

                    if(UploadRegistrationInfo(syndCode))
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
bool TRegistrationManager::UploadRegistrationInfo(AnsiString syndicateCode)
{
    bool retval = false;
    try
    {
        Database::TDBTransaction dBTransaction(TDeviceRealTerminal::Instance().DBControl);
	    dBTransaction.StartTransaction();

        TMMProcessingState State(Screen->ActiveForm, "Depicting registration verification in process Please Wait...", "Registration verification");
        TDeviceRealTerminal::Instance().ProcessingController.Push(State);
        AnsiString ErrorMessage;
        TTerminalModel terminalInfo = TDBRegistration::GetTerminalInfo(dBTransaction);
        TRegistrationInterface* registrationInterface = new TRegistrationInterface();
        MMRegistrationServiceResponse createResponse = registrationInterface->UploadRegistrationInfo(terminalInfo, syndicateCode);
        TDeviceRealTerminal::Instance().ProcessingController.Pop();

        if(createResponse.IsSuccesful)
            retval = true;
        else
        {
            if(createResponse.ResponseCode == AuthenticationFailed)
            {
                ErrorMessage = "Authentication failed with Registration Service.";
            }
            else if(createResponse.ResponseCode == NoNewSettingChange)
            {
                 ErrorMessage = "No new Setting found for Update.";
            }
            else
            {
                if(createResponse.Description == "Failed to update registration info to server.")
                  ErrorMessage = "Failed to update registration to server.";          //message to be changed..
                else
                  ErrorMessage = "Failed to update registration to server.";
            }
            MessageBox(ErrorMessage,"Error", MB_OK + MB_ICONERROR);
        }

        delete registrationInterface;
        registrationInterface = NULL;

        dBTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return retval;
}
//-----------------------------------------------------------------------
bool TRegistrationManager::ValidateCompanyInfo(AnsiString syndicateCode, int siteId)
{
    bool retval = false;
    try
    {
        Database::TDBTransaction dBTransaction(TDeviceRealTerminal::Instance().DBControl);
	    dBTransaction.StartTransaction();

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
            TManagerVariable::Instance().SetDeviceStr(dBTransaction,vmCompanyName,TGlobalSettings::Instance().CompanyName);
        }
        else
        {
            ErrorMessage = createResponse.Message;
            MessageBox(ErrorMessage,"Error", MB_OK + MB_ICONERROR);
        }

        delete registrationInterface;
        registrationInterface = NULL;

        dBTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return retval;
}
//---------------------------------------------------------------------------------------

