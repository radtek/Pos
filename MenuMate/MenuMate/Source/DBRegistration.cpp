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
        terminalInfo.StaffName = TDeviceRealTerminal::Instance().ID.Name;
        terminalInfo.MacAdress = TDeviceRealTerminal::Instance().ID.Name;
        terminalInfo.ComputerName = TDeviceRealTerminal::Instance().ID.Name;;
        terminalInfo.OperatingSystemName = TDeviceRealTerminal::Instance().ID.Name;
        terminalInfo.MenumateVersion = TDeviceRealTerminal::Instance().ID.Name;
        terminalInfo.SiteId = 1;
        terminalInfo.TerminalProfileId = 1;
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
    std::list<TLicenceSettingMapping> licenseSettingMappingList;

    licenseSettingMappingList.push_back(GetLicenseSettingsMapping(dbTransaction));
}
//-------------------------------------------------------------------------------------------------
TLicenceSettingMapping TDBRegistration::GetLicenseSettingsMapping(Database::TDBTransaction &dbTransaction)
{
    TLicenceSettingMapping licenseSettingMapping;
    try
    {
        licenseSettingMapping.TerminalId = 1;
        licenseSettingMapping.LicenceSettingId = 1;//todo
        licenseSettingMapping.IsEnabled = true;//todo
        licenseSettingMapping.Value = "";//todo
        licenseSettingMapping.Text = "";//todo
        licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction);
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
