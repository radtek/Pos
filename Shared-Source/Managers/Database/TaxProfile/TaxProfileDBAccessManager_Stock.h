//---------------------------------------------------------------------------

#ifndef TaxProfileDBAccessManager_StockH
#define TaxProfileDBAccessManager_StockH

#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include "MM_DBCore.h"
#include "TaxProfile.h"
#include "ITaxProfileDBAccessManager.h"
#include "GeneratorManager.h"
//---------------------------------------------------------------------------
class TTaxProfileDBAccessManager_Stock : public ITaxProfileDBAccessManager
{
   public:
      TaxProfileMap   LoadTaxProfiles(Database::TDBTransaction &transaction);
      bool            InsertTaxProfile(Database::TDBTransaction &transaction, TaxProfile taxProfile, int &taxProfileKey);
      bool            UpdateTaxProfile(Database::TDBTransaction &transaction, TaxProfile taxProfile);
      TaxProfile      LoadTaxProfileFromKey(Database::TDBTransaction &transaction, int taxProfileDBKey);
      int             GetTaxProfileGeneratorValue(Database::TDBTransaction &transaction);
      void            SetTaxProfileGeneratorValue(Database::TDBTransaction &transaction, int val);

   private:
      bool            UpdateTaxProfileReferences(Database::TDBTransaction &transaction, int oldTaxProfileKey, int newTaxProfileKey);
      bool            DisableTaxProfile(Database::TDBTransaction &transaction, int taxProfileKey);
      void            getTaxProfileFromQuery(TIBSQL* query, TaxProfile* taxProfile);
      bool            updateTaxProfilePartial( Database::TDBTransaction &transaction, TaxProfile* taxProfile);
};
#endif

