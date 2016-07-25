//---------------------------------------------------------------------------

#ifndef DBDealH
#define DBDealH
//---------------------------------------------------------------------------

#include "IDBDeal.h"
#include "DeviceRealTerminal.h"
#include "logsdef.h"
#include "LogsDB.h"
#include "DealTypeDefs.h"

typedef std::vector<IStoredDeal*> DealsPointerCollection;

class DBDeal : public IDBDeal
{
    private:
        Database::TDBControl &DBControl;

		void logError(UnicodeString Function, UnicodeString Type, UnicodeString Msg, UnicodeString Terminal="");

		UnicodeString getNewDealKeyQuery();
        UnicodeString getDealInsertQuery();
        UnicodeString getDealUpdateQuery();
        UnicodeString getDealDeleteQuery();
        UnicodeString getDealSelectSingleQuery();
		UnicodeString getDealSelectAllQuery();

		UnicodeString getNewDealLevelKeyQuery();
		UnicodeString getDealLevelInsertQuery();
		UnicodeString getDealLevelUpdateQuery();
		UnicodeString getDealLevelDeleteQuery();
		UnicodeString getDealLevelSelectOnDealKeyQuery();

		int generateDealKey(Database::TDBTransaction &DBTransaction);
		int generateDealLevelKey(Database::TDBTransaction &DBTransaction);

		StoredDealLevelSet loadDealLevelsForDeal(Database::TDBTransaction &DBTransaction, int inDealKey);

		void loadSingleDealFromQuery(TIBSQL *internalQuery, IStoredDeal *outDeal);
		TList populateDealsCollectionFromQuery(TIBSQL* internalQuery, TList *allDeals);

    public:

        DBDeal();
        ~DBDeal();

		bool Save(IStoredDeal *inItem);

		bool Save(Database::TDBTransaction &DBTransaction, IStoredDeal *inItem);

		bool Update(IStoredDeal *inItem);

		bool Update(Database::TDBTransaction &DBTransaction, IStoredDeal *inItem);

		bool Delete(IStoredDeal *inItem);

		bool Delete(Database::TDBTransaction &DBTransaction, IStoredDeal *inItem);

		void Load(int inItemID, IStoredDeal *outDeal);

        void LoadAll(StoredDealVector &allDeals);
};
#endif
