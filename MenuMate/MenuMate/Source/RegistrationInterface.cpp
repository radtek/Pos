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
MMRegistrationServiceResponse TRegistrationInterface::UploadRegistrationInfo(TTerminal terminalInfo)
{
    RegistrationResponse *wcfResponse;
    try
    {
        Terminal *wcfInfo = new Terminal();
        wcfInfo->ComputerName = terminalInfo.ComputerName;
        wcfInfo->Description = terminalInfo.Description;
        wcfInfo->MacAdress = terminalInfo.MacAdress;
        wcfInfo->MenumateVersion = terminalInfo.MenumateVersion;
        wcfInfo->Name = terminalInfo.Name;
        wcfInfo->OperatingSystemName = terminalInfo.OperatingSystemName;
        wcfInfo->SiteId = terminalInfo.SiteId;
        wcfInfo->StaffName = terminalInfo.StaffName;
        wcfInfo->TerminalProfileId = terminalInfo.TerminalProfileId;

        if(!terminalInfo.LicenceSettingMappings.empty())
        {
            ArrayOfLicenceSettingMapping licenseSettingMappingArray;

            for(std::list<TLicenceSettingMapping>::iterator itLicenseSettingInfo = terminalInfo.LicenceSettingMappings.begin();
                    itLicenseSettingInfo != terminalInfo.LicenceSettingMappings.end(); ++itLicenseSettingInfo)
            {
                LicenceSettingMapping* licenseSetting = new LicenceSettingMapping;
                licenseSetting->IsEnabled = itLicenseSettingInfo->IsEnabled;
                licenseSetting->LicenceSettingId = itLicenseSettingInfo->LicenceSettingId;
                licenseSetting->TerminalId = itLicenseSettingInfo->TerminalId;
                licenseSetting->Text = itLicenseSettingInfo->Text;
                licenseSetting->Value = itLicenseSettingInfo->Value;

              //  LicenceSetting* setting = new LicenseSetting;
                licenseSetting->LicenceSettingSetting->Description =  itLicenseSettingInfo->LicenceSettingSetting.Description;
                licenseSetting->LicenceSettingSetting->IsEnabledByDefault = itLicenseSettingInfo->LicenceSettingSetting.IsEnabledByDefault;
                licenseSetting->LicenceSettingSetting->Name =  itLicenseSettingInfo->LicenceSettingSetting.Name;
                licenseSetting->LicenceSettingSetting->SettingSubType = itLicenseSettingInfo->LicenceSettingSetting.SettingSubType;
                licenseSetting->LicenceSettingSetting->SettingType =  itLicenseSettingInfo->LicenceSettingSetting.SettingType;
                //itLicenseSettingInfo->LicenceSettingSetting.SettingType.;
               // licenseSetting->LicenceSettingSetting = setting;

                licenseSetting->Terminal = new Terminal;

                licenseSettingMappingArray.Length = (licenseSettingMappingArray.Length + 1);
                licenseSettingMappingArray[licenseSettingMappingArray.Length - 1] = licenseSetting;
            }
            wcfInfo->LicenceSettingMappings = licenseSettingMappingArray;
        }
        CoInitialize(NULL);
        AnsiString SyndicateCode = GetSyndCodeForRegistration();
        wcfResponse = registrationClient->UpdateTerminalRegistrationInfo(SyndicateCode, wcfInfo);
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
    return MMRegistrationServiceResponse(
                inWCFResponse->Successful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMRegistrationResponseCode )inWCFResponse->ResponseCode);
}
//---------------------------------------------------------------------------
