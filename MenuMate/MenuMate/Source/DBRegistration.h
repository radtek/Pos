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
    static TLicenceSettingMapping GetLicenseSettingsMapping(Database::TDBTransaction &dbTransaction, int licenceType);
    static TLicenceSetting GetLicenseSettings(Database::TDBTransaction &dbTransaction, AnsiString name, AnsiString description, bool status, int subType);
    static void LoadEFTPosSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
    static void LoadLoyaltySettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping);
    static void LoadAccountsSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping);
    static void LoadTimeTrackingSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
//    static void LoadChefmateSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
    static void LoadPropertyManagementSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
//    static void LoadRoomSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
    static void LoadFloorPlanSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
//    static void LoadPosCashierSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
    static void LoadPosOrderSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
//    static void LoadPosHandHeldSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
    static void LoadFiscalSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
    static void LoadWebMatSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
//    static void LoadPocketVoucherSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
    static void LoadBarExchangeSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
    static void LoadRunRateBoardSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
    static void LoadOnlineOrderingSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
};
#endif
