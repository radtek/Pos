//---------------------------------------------------------------------------

#ifndef ManagerMallSetupH
#define ManagerMallSetupH
//---------------------------------------------------------------------------

#include "Vector.h"
#include "MM_DBCore.h"
#include <list>
#include "Mall.h"
class TManagerMallSetup
{
 private:
     static bool IsSettingIdExist(Database::TDBTransaction &dbTransaction,int settingKey);
 public:
     static std::vector<UnicodeString> LoadAllMalls(Database::TDBTransaction &dbTransaction);
     static void UpdateActiveMall(Database::TDBTransaction &dbTransaction, int mallKey);
     static void InsertOrUpdateMallExportSettingValues(Database::TDBTransaction &dbTransaction,UnicodeString tenantCode, UnicodeString mallPath, UnicodeString terminalNumber);
     static TMall LoadActiveMallSettings(Database::TDBTransaction &dbTransaction);
     static void UpdateMallExportSettingValues(Database::TDBTransaction &dbTransaction, TMall &mallInfo);
     static int CheckActiveMallExist(Database::TDBTransaction &dbTransaction);
     static void UpdateINActiveMall(Database::TDBTransaction &dbTransaction);
};
#endif


