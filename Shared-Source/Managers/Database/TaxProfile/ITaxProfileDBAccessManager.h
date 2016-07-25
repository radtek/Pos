//---------------------------------------------------------------------------

#ifndef ITaxProfileDBAccessManagerH
#define ITaxProfileDBAccessManagerH

#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include "MM_DBCore.h"
#include "TaxProfile.h"
//---------------------------------------------------------------------------
class ITaxProfileDBAccessManager
{
   public:
      virtual TaxProfileMap   LoadTaxProfiles(Database::TDBTransaction &transaction) = 0;
      virtual bool            InsertTaxProfile(Database::TDBTransaction &transaction, TaxProfile taxProfile, int &taxProfileKey) = 0;
      virtual bool            UpdateTaxProfile(Database::TDBTransaction &transaction, TaxProfile taxProfile) = 0;
      virtual int             GetTaxProfileGeneratorValue(Database::TDBTransaction &transaction) = 0;
      virtual void            SetTaxProfileGeneratorValue(Database::TDBTransaction &transaction, int val) = 0;

   protected:
      virtual bool            UpdateTaxProfileReferences(Database::TDBTransaction &transaction, int oldTaxProfileKey, int newTaxProfileKey) = 0;
      virtual bool            DisableTaxProfile(Database::TDBTransaction &transaction, int taxProfileKey) = 0;
};
#endif
