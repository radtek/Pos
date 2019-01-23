//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "RegistrationInterface.h"



//---------------------------------------------------------------------------

#pragma package(smart_init)

TRegistrationInterface::TRegistrationInterface()
{
    InitRegClient();
}
//---------------------------------------------------------------------------
TRegistrationInterface::~TRegistrationInterface()
{
//
}
//------------------------------------------------------------------------
void TRegistrationInterface::InitRegClient()
{
    bool useWSDL = false;
    AnsiString registrationURL = "http://localhost:8749/MenumateServices/RegistrationService";
    registrationClient = GetIRegistrationIntegrationWebService(useWSDL, registrationURL, NULL );
}
//-----------------------------------------------------------------------
MMRegistrationServiceResponse TRegistrationInterface::UploadRegistrationInfo(TTerminalModel terminalInfo)
{
    RegistrationResponse *wcfResponse;
    try
    {
        TerminalModel *wcfInfo = new TerminalModel;
        wcfInfo->ComputerName = terminalInfo.ComputerName;
        wcfInfo->MacAdress = terminalInfo.MacAdress;
        wcfInfo->MenumateVersion = terminalInfo.MenumateVersion;
        wcfInfo->OperatingSystemName = terminalInfo.OperatingSystemName;
        wcfInfo->StaffName = terminalInfo.StaffName;
        wcfInfo->SiteCode = terminalInfo.SiteCode;
        wcfInfo->SyndicateCode = terminalInfo.SyndicateCode;
        wcfInfo->TerminalDescription = terminalInfo.TerminalDescription;
        wcfInfo->TerminalName = terminalInfo.TerminalName;

        if(!terminalInfo.LicenceSettingsModel.empty())
        {
            ArrayOfLicenceSettingModel licenseSettingModelArray;

            for(std::list<TLicenceSettingModel>::iterator itLicenseSettingInfo = terminalInfo.LicenceSettingsModel.begin();
                    itLicenseSettingInfo != terminalInfo.LicenceSettingsModel.end(); ++itLicenseSettingInfo)
            {
                LicenceSettingModel* licenseSetting = new LicenceSettingModel;

                licenseSetting->IsActive = itLicenseSettingInfo->IsActive;
                licenseSetting->SettingSubType = itLicenseSettingInfo->SettingSubType;
                licenseSetting->SettingType = itLicenseSettingInfo->SettingType;

                licenseSettingModelArray.Length = (licenseSettingModelArray.Length + 1);
                licenseSettingModelArray[licenseSettingModelArray.Length - 1] = licenseSetting;
            }
            wcfInfo->LicenceSettingsModel = licenseSettingModelArray;
        }
        CoInitialize(NULL);
        AnsiString SyndicateCode = GetSyndCodeForRegistration();
        MessageBox(SyndicateCode,"1.4",MB_OK);
        wcfResponse = registrationClient->UpdateTerminalRegistrationInfo(SyndicateCode, wcfInfo);
        MessageBox("1.5","1.5",MB_OK);
        delete wcfInfo;
        wcfInfo = NULL;
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
       // return CreateExceptionFailedResponse( exc.Message );
    }
}
//-------------------------------------------------------------------------------------
AnsiString TRegistrationInterface::GetSyndCodeForRegistration()
{
    AnsiString syndicateCode = "";

    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        TManagerSyndCode ManagerSyndicateCode;
        ManagerSyndicateCode.Initialise(dbTransaction);
        TSyndCode currentSyndicateCode = ManagerSyndicateCode.GetCommunicationSyndCode();
        syndicateCode = currentSyndicateCode.GetSyndCode();
        dbTransaction.Commit();
    }
    catch( Exception& exc )
    {
        dbTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,exc.Message);
		throw;
    }
    return syndicateCode;
}
//-----------------------------------------------------------------------------
MMRegistrationServiceResponse TRegistrationInterface::CreateMMResponse(RegistrationResponse* inWCFResponse )
{
    return MMRegistrationServiceResponse(inWCFResponse->Successful, AnsiString( inWCFResponse->Message.t_str() ),
                        AnsiString( inWCFResponse->Description.t_str() ), ( MMRegistrationResponseCode )inWCFResponse->ResponseCode);
}
//---------------------------------------------------------------------------
MMRegistrationServiceResponse TRegistrationInterface::CreateMMResponse(RegistrationWebResponse* inWCFResponse )
{
    return MMRegistrationServiceResponse(inWCFResponse->IsSuccessful, AnsiString( inWCFResponse->ResponseText.t_str() ), "", Successful);
}
//---------------------------------------------------------------------------
MMRegistrationServiceResponse TRegistrationInterface::ValidateCompanyInfo()
{
    try
    {
        RegistrationWebResponse* response;
        CoInitialize(NULL);
        AnsiString SyndicateCode = GetSyndCodeForRegistration();
        response = registrationClient->ValidateCompanyInfo(SyndicateCode,TGlobalSettings::Instance().SiteID);
        return CreateMMResponse( response );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMRegistrationServiceResponse TRegistrationInterface::CreateExceptionFailedResponse(AnsiString inMessage )
{
   if(inMessage.Pos("XML") > 0 || inMessage.Pos("The handle") > 0 )
   return MMRegistrationServiceResponse(false,"Not able to connect with server.","",FailedDueToException );
   else
     return MMRegistrationServiceResponse(false,inMessage,"",FailedDueToException );
}
