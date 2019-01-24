//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBRegistration.h"
#include "RegistrationManager.h"
#include "RegistrationInterface.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TRegistrationManager::CheckRegistrationStatus()
{
    try
    {
        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();
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
    //            if(TGlobalSettings::Instance().IsCloudSyncRequired)
    //            {
    //                //Send Updated Settings to the Web
    //              //  UpdateTerminalSettings();
                UploadRegistrationInfo(syndCode);
            }
            else
            {
                ValidateCompanyInfo(syndCode, TGlobalSettings::Instance().SiteID);
            }
        }
        else
        {
            MessageBox("Please setup syndicate code first","Syndicate Code Error.",MB_OK);
        }
        dbTransaction.Commit();
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//-----------------------------------------------------------------------
void TRegistrationManager::UploadRegistrationInfo(AnsiString syndicateCode)
{
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
        if(!createResponse.IsSuccesful && createResponse.ResponseCode == AuthenticationFailed)
        {
            throw Exception("Authentication failed with Registration Service");
        }
        else
        {
            if(createResponse.Description == "Failed to update registration info to server.")
              ErrorMessage = "Failed to update registration to server.";
            else
              ErrorMessage = "Failed to update registration to server.";
            throw Exception(ErrorMessage);
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
}
//-----------------------------------------------------------------------
void TRegistrationManager::ValidateCompanyInfo(AnsiString syndicateCode, int siteId)
{
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
        if(!createResponse.IsSuccesful && createResponse.ResponseCode == AuthenticationFailed)
        {
            throw Exception("Authentication failed with Registration Service");
        }
        else
        {
            if(createResponse.Description == "Failed to validate company info to server.")
              ErrorMessage = "Failed to validate company to server.";
            else
              ErrorMessage = "Failed to validate company  to server.";
            throw Exception(ErrorMessage);
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
}
//---------------------------------------------------------------------------------------

