//---------------------------------------------------------------------------

#ifndef DBRegistrationH
#define DBRegistrationH
#include "RegistrationAttributes.h"
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include "MM_DBCore.h"
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
