//---------------------------------------------------------------------------

#ifndef TaxProfileDBManagerH
#define TaxProfileDBManagerH

#include "MMData.h"
#include "StockData.h"
#include "ITaxProfileDBAccessManager.h"
#include "TaxProfileDBAccessManager_MM.h"
#include "TaxProfileDBAccessManager_Stock.h"

//---------------------------------------------------------------------------
class TTaxProfileDBManager
{
   public:
      TTaxProfileDBManager();
      TaxProfileMap  LoadTaxProfiles();
      bool           SaveTaxProfiles(TaxProfileMap taxProfiles);

   private:
      TaxProfileMap  loadTaxProfiles(Database::TDBTransaction &transaction, ITaxProfileDBAccessManager* manager);
      void           seperateTaxProfilesByDB(const TaxProfileMap &allProfiles, TaxProfileMap &stockProfiles, TaxProfileMap &mmProfiles);
      void           seperateInsertsAndUpdates(const TaxProfileMap &allProfiles, TaxProfileMap &inserts, TaxProfileMap &updates);
      bool           performInserts(Database::TDBTransaction &transaction, ITaxProfileDBAccessManager* manager, const TaxProfileMap taxProfiles);
      bool           performUpdates(Database::TDBTransaction &transaction, ITaxProfileDBAccessManager* manager, const TaxProfileMap taxProfiles);
      void           syncTaxProfileGeneratorValues(Database::TDBTransaction &stockTransaction, Database::TDBTransaction &mmTransaction);
      int            getTaxProfileGeneratorValue(Database::TDBTransaction &transaction, ITaxProfileDBAccessManager* manager);
      void           setTaxProfileGeneratorValue(Database::TDBTransaction &transaction, ITaxProfileDBAccessManager* manager, int value);

      std::auto_ptr<ITaxProfileDBAccessManager> stockDB_AccessManager;
      std::auto_ptr<ITaxProfileDBAccessManager> mmDB_AccessManager;
};
#endif
