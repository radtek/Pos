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
        //Checking POS Resgistration Status
        if(TGlobalSettings::Instance().IsRegistrationVerified)
        {
//            if(TGlobalSettings::Instance().IsCloudSyncRequired)
//            {
//                //Send Updated Settings to the Web
//              //  UpdateTerminalSettings();
//            }
        }
        else
        {
//            if(SyndicateCode && SiteID)
//            {
                //Register Pos
                //Update IsRegistrationVerified flag as per response received
             //   RegisterTerminal();
//            }
        }
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//-----------------------------------------------------------------------
void TRegistrationManager::UploadRegistrationInfo()
{
    try
    {
        Database::TDBTransaction dBTransaction(TDeviceRealTerminal::Instance().DBControl);
	    dBTransaction.StartTransaction();

        TMMProcessingState State(Screen->ActiveForm, "Depicting registration verification in process Please Wait...", "Registration verification");
        TDeviceRealTerminal::Instance().ProcessingController.Push(State);
        AnsiString ErrorMessage;
        TTerminal terminalInfo = TDBRegistration::GetTerminalInfo(dBTransaction);
        TRegistrationInterface* registrationInterface = new TRegistrationInterface();
        MMRegistrationServiceResponse createResponse = registrationInterface->UploadRegistrationInfo(terminalInfo);
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
