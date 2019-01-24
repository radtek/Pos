//---------------------------------------------------------------------------

#ifndef DBRegistrationH
#define DBRegistrationH
#include "RegistrationAttributes.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
class TDBRegistration
{
public:
    static TTerminalModel GetTerminalInfo(Database::TDBTransaction &dbTransaction);
    static AnsiString GetSyndCode(Database::TDBTransaction &dbTransaction);
    static void UpdateIsCloudSyncRequiredFlag(bool status);
    static void SetIsIsRegistrationVerifiedFlag();
private:
    static std::list<TLicenceSettingModel>  GetLicenseSettingsModelList(Database::TDBTransaction &dbTransaction);
    static void LoadLicenseSettingsModelList(Database::TDBTransaction &dbTransaction , int licenceType, std::list<TLicenceSettingModel> &licenceSettingModelList);
    static void LoadEFTPosSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static bool GetEFTPosSetting(int eftPosSubType);
    static void LoadLoyaltySettingsForTerminal(Database::TDBTransaction &dbTransaction,std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static bool GetLoyaltySetting(int loyaltySubType);
    static void LoadAccountsSettingsForTerminal(Database::TDBTransaction &dbTransaction,std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static bool GetAccountSetting(int accountSubType);
    static void LoadTimeTrackingSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static void LoadChefmateSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static bool GetChefmateSetting(Database::TDBTransaction &dbTransaction);
    static void LoadPropertyManagementSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static bool GetPropertyManagementSetting(int propertySubType);
    static void LoadRoomSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static bool GetRoomSetting(int roomSubType);
    static void LoadFloorPlanSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
//    static void LoadPosCashierSettingsForTerminal(Database::TDBTransaction &dbTransaction, TLicenceSettingMapping &licenseSettingMapping);
    static void LoadPosOrderSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static void LoadPosHandHeldSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static bool GetPosHandHeldSetting(Database::TDBTransaction &dbTransaction);
    static void LoadFiscalSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static bool GetFiscalSetting(int fiscalSubType);
    static void LoadWebMatSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static void LoadPocketVoucherSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static bool GetPocketVoucherSetting(Database::TDBTransaction &dbTransaction);
    static void LoadBarExchangeSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static void LoadRunRateBoardSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);
    static void LoadOnlineOrderingSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType);

};
#endif
