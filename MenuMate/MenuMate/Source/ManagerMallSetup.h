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
 public:
     static std::vector<UnicodeString> LoadAllMalls(Database::TDBTransaction &dbTransaction);
     static void UpdateActiveMall(Database::TDBTransaction &dbTransaction, int mallKey);
     static TMall LoadActiveMallSettings(Database::TDBTransaction &dbTransaction);
     static void UpdateMallExportSettingValues(Database::TDBTransaction &dbTransaction, TMall &mallInfo);
     static int CheckActiveMallExist(Database::TDBTransaction &dbTransaction);
     static void UpdateINActiveMall(Database::TDBTransaction &dbTransaction);
     static void InsertInToMallExport_Settings_Values(int mallKey);
     static std::map<int, std::set<int> > LoadMezzanineAreaTablesByLocations(Database::TDBTransaction &DBTransaction);
 private:
     static void InsertSettingValuesForEstancia(Database::TDBTransaction &dbTransaction, int deviceKey, int mallId);
     static void InsertSettingValuesForDeanAndDeluca(Database::TDBTransaction &dbTransaction, int deviceKey, int mallId);
     static bool IsSettingExistInDB(Database::TDBTransaction &dbTransaction, int deviceKey, int mallID);
};
#endif


