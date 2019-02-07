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
MMRegistrationServiceResponse TRegistrationInterface::UploadRegistrationInfo(TTerminalModel terminalInfo, AnsiString syndicateCode)
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

        TXSDateTime* transactionDate = new TXSDateTime;
        transactionDate->AsDateTime = terminalInfo.RegistrationTime;
        wcfInfo->RegistrationTime = transactionDate;

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
        wcfResponse = registrationClient->UpdateTerminalRegistrationInfo(syndicateCode, wcfInfo);

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
MMRegistrationServiceResponse TRegistrationInterface::CreateMMResponse(RegistrationResponse* inWCFResponse )
{
    return MMRegistrationServiceResponse(inWCFResponse->Successful, AnsiString( inWCFResponse->Message.t_str() ),
                        AnsiString( inWCFResponse->Description.t_str() ), ( MMRegistrationResponseCode )inWCFResponse->ResponseCode);
}
//---------------------------------------------------------------------------
MMRegistrationServiceResponse TRegistrationInterface::CreateMMResponse(CompanySiteModelResponse* inWCFResponse )
{
    if(inWCFResponse->CompanyName.Trim() == "")
        inWCFResponse->CompanyName = inWCFResponse->Message;
    return MMRegistrationServiceResponse(inWCFResponse->IsSuccessful, AnsiString( inWCFResponse->CompanyName.t_str() ), AnsiString(inWCFResponse->Message.t_str()), Successful);
}
//---------------------------------------------------------------------------
MMRegistrationServiceResponse TRegistrationInterface::ValidateCompanyInfo(AnsiString syndicateCode, int siteId)
{
    try
    {
        CompanySiteModelResponse* response;
        CoInitialize(NULL);
        response = registrationClient->ValidateCompanyInfo(syndicateCode, siteId);
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
