//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBRegistration.h"
#include "DeviceRealTerminal.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TTerminal TDBRegistration::GetTerminalInfo(Database::TDBTransaction &dbTransaction)
{
    TTerminal terminalInfo;
    try
    {
        terminalInfo.Name = TDeviceRealTerminal::Instance().ID.Name;
        terminalInfo.Description = TDeviceRealTerminal::Instance().ID.Name;
        terminalInfo.StaffName = TDeviceRealTerminal::Instance().User.Name;
//        terminalInfo.MacAdress = TDeviceRealTerminal::Instance();
        terminalInfo.ComputerName = TDeviceRealTerminal::Instance().ID.ComputerName;
        terminalInfo.OperatingSystemName = TDeviceRealTerminal::Instance().OS.SoftwareVersion;
        terminalInfo.MenumateVersion = TDeviceRealTerminal::Instance().OS.MMSoftwareVersion;
        terminalInfo.SiteId = TGlobalSettings::Instance().SiteID;
        terminalInfo.TerminalProfileId = TDeviceRealTerminal::Instance().ID.ProfileKey;
        terminalInfo.LicenceSettingMappings = GetLicenseSettingMappingList(dbTransaction);
//        terminalInfo.
//        TGlobalSettings::Instance().SiteID;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return terminalInfo;
}
//-----------------------------------------------------------------------------------------------
std::list<TLicenceSettingMapping> TDBRegistration::GetLicenseSettingMappingList(Database::TDBTransaction &dbTransaction)
{
    for(int i = eEftpos; i <= eOnlineOrdering; i++)
    {
        std::list<TLicenceSettingMapping> licenseSettingMappingList;

        licenseSettingMappingList.push_back(GetLicenseSettingsMapping(dbTransaction));
    }

}
//-------------------------------------------------------------------------------------------------
TLicenceSettingMapping TDBRegistration::GetLicenseSettingsMapping(Database::TDBTransaction &dbTransaction)
{
    TLicenceSettingMapping licenseSettingMapping;
    try
    {
//        licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);
//        licenseSettingMapping.LicenceSettingId = 1;//todo
//        licenseSettingMapping.IsEnabled = true;//todo
//        licenseSettingMapping.Value = "";//todo
//        licenseSettingMapping.Text = "";//todo
//        licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction);
//
//        switch(1)  //todo
//        {
//            case eEftpos:
//                {
//                    licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);
//                    licenseSettingMapping.LicenceSettingId = 1;//todo
//                    licenseSettingMapping.IsEnabled = true;//todo
//                    licenseSettingMapping.Value = "";//todo
//                    licenseSettingMapping.Text = "";//todo
//                    licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction);
//                    break;
//                }
//            case eLoyalty:
//            case eAccounts:
//            case eTimeTracking :
//            case eChefmate:
//            case ePropertyManagement:
//            case eRoom:
//            case eFloorPlan:
//            case ePosCashier:
//            case ePosOrder:
//            case ePosHandHeld:
//            case eFiscal:
//            case eWebMat:
//            case ePocketVoucher:
//            case eBarExchange:
//            case eRunRateBoard:
//            case eOnlineOrdering:
//            case default:
//                {
//                break;
//                }
//        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return licenseSettingMapping;
}
//---------------------------------------------------------------------
TLicenceSetting TDBRegistration::GetLicenseSettings(Database::TDBTransaction &dbTransaction)
{
    TLicenceSetting licenseSetting;
    try
    {
        licenseSetting.Name = "";
        licenseSetting.Description = "";
        licenseSetting.IsEnabledByDefault = true;
        licenseSetting.SettingType = 0;
       // licenseSetting.SettingSubTyp = 0;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return licenseSetting;
}
