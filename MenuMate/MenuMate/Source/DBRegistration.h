//---------------------------------------------------------------------------

#ifndef DBRegistrationH
#define DBRegistrationH
#include "RegistrationAttributes.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
class TDBRegistration
{
public:
    static TTerminal GetTerminalInfo(Database::TDBTransaction &dbTransaction);
private:
    static std::list<TLicenceSettingMapping> GetLicenseSettingMappingList(Database::TDBTransaction &dbTransaction);
    static TLicenceSettingMapping GetLicenseSettingsMapping(Database::TDBTransaction &dbTransaction);
    static TLicenceSetting GetLicenseSettings(Database::TDBTransaction &dbTransaction);
};
#endif
