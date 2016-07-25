//---------------------------------------------------------------------------


#pragma hdrstop

#include "TaxProfileDBManager.h"

//---------------------------------------------------------------------------

TTaxProfileDBManager::TTaxProfileDBManager()
         :stockDB_AccessManager(new TTaxProfileDBAccessManager_Stock()),
          mmDB_AccessManager(new TTaxProfileDBAccessManager_MM())
{
}

TaxProfileMap TTaxProfileDBManager::LoadTaxProfiles()
{
   Database::TDBTransaction stockTransaction(dmStockData->dbStock);
   Database::TDBTransaction mmTransaction(dmMMData->dbMenuMate);

   stockTransaction.StartTransaction();
   mmTransaction.StartTransaction();

   TaxProfileMap profilesMap;
   TaxProfileMap stockProfiles = loadTaxProfiles(stockTransaction, stockDB_AccessManager.get());
   TaxProfileMap menumateProfiles = loadTaxProfiles(mmTransaction, mmDB_AccessManager.get());

   std::map<TaxProfileKey, TaxProfile*>::iterator it;

   for(it=stockProfiles.begin(); it != stockProfiles.end(); it++)
   {
      profilesMap[it->first] = it->second;
   }

   for(it=menumateProfiles.begin(); it != menumateProfiles.end(); it++)
   {
      profilesMap[it->first] = it->second;
   }

   return profilesMap;
}

bool TTaxProfileDBManager::SaveTaxProfiles(TaxProfileMap taxProfiles)
{
   bool status = false;

   try
   {
      Database::TDBTransaction stockTransaction(dmStockData->dbStock);
      Database::TDBTransaction mmTransaction(dmMMData->dbMenuMate);

      stockTransaction.StartTransaction();
      mmTransaction.StartTransaction();

      TaxProfileMap stockProfiles;
      TaxProfileMap mmProfiles;
      TaxProfileMap updates;
      TaxProfileMap inserts;

      //seperate tax profiles by the db
      seperateTaxProfilesByDB(taxProfiles, stockProfiles, mmProfiles);

      //seperate inserts and updates for stock db
      seperateInsertsAndUpdates(stockProfiles, inserts, updates);

      //perform operations for stock db
      performInserts(stockTransaction, stockDB_AccessManager.get(), inserts);
      performUpdates(stockTransaction, stockDB_AccessManager.get(), updates);

      inserts.clear();
      updates.clear();

      // sync generator values across the two databases to prevent overlapping of the tax profile db keys
      syncTaxProfileGeneratorValues(stockTransaction, mmTransaction);

      //seperate inserts and updates for menumate db
      seperateInsertsAndUpdates(mmProfiles, inserts, updates);

      //perform operations for menumate db
      performInserts(mmTransaction, mmDB_AccessManager.get(), inserts);
      performUpdates(mmTransaction, mmDB_AccessManager.get(), updates);

      stockTransaction.Commit();
      mmTransaction.Commit();
   }
   catch(Exception &Ex)
   {
      //todo: log the exception
#ifdef _DEBUG
      throw;
#endif
   }

   return status;
}

TaxProfileMap TTaxProfileDBManager::loadTaxProfiles(
                                          Database::TDBTransaction &transaction,
                                          ITaxProfileDBAccessManager* manager)
{
   return manager->LoadTaxProfiles(transaction);
}

void TTaxProfileDBManager::seperateTaxProfilesByDB(
                           const TaxProfileMap &allProfiles,
                           TaxProfileMap &stockProfiles,
                           TaxProfileMap &mmProfiles)
{
   std::map<TaxProfileKey, TaxProfile*>::const_iterator it;

   for(it = allProfiles.begin(); it != allProfiles.end(); it++)
   {
      if(it->first.second == PurchaseTax)
         stockProfiles[it->first] = it->second;
      else
         mmProfiles[it->first] = it->second;
   }
}

void TTaxProfileDBManager::seperateInsertsAndUpdates(
                           const TaxProfileMap &allProfiles,
                           TaxProfileMap &inserts,
                           TaxProfileMap &updates)
{
   std::map<TaxProfileKey, TaxProfile*>::const_iterator it;

   for(it = allProfiles.begin(); it != allProfiles.end(); it++)
   {
      if(it->second->GetTaxProfileDBKey() > 0)
         updates[it->first] = it->second;
      else
         inserts[it->first] = it->second;
   }
}

bool TTaxProfileDBManager::performInserts(
                     Database::TDBTransaction &transaction,
                     ITaxProfileDBAccessManager* manager,
                     const TaxProfileMap taxProfiles)
{
   std::map<TaxProfileKey,TaxProfile*>::const_iterator it;
   int taxProfileKey = 0;
   for(it=taxProfiles.begin(); it != taxProfiles.end(); it++)
   {
      manager->InsertTaxProfile(transaction, *(it->second), taxProfileKey);
   }
}

bool TTaxProfileDBManager::performUpdates(
                     Database::TDBTransaction &transaction,
                     ITaxProfileDBAccessManager* manager,
                     const TaxProfileMap taxProfiles)
{
   std::map<TaxProfileKey,TaxProfile*>::const_iterator it;
   int taxProfileKey = 0;
   for(it=taxProfiles.begin(); it != taxProfiles.end(); it++)
   {
      manager->UpdateTaxProfile(transaction, *(it->second));
   }
}

void TTaxProfileDBManager::syncTaxProfileGeneratorValues(Database::TDBTransaction &stockTransaction, Database::TDBTransaction &mmTransaction)
{
   // current generator key which is unique across two databases
   // this is to prevent either menumate or stock database having tax profiles with same db key,
   // which leads to issues in the gui. we set the current generator to an appropriate value before performing operations

   int stockTaxProfileGeneratorVal = getTaxProfileGeneratorValue(stockTransaction, stockDB_AccessManager.get());
   int mmTaxProfileGeneratorVal = getTaxProfileGeneratorValue(mmTransaction, mmDB_AccessManager.get());

   if( mmTaxProfileGeneratorVal < stockTaxProfileGeneratorVal )
   {
      // adjust the generator value to prevent overlapping
      setTaxProfileGeneratorValue( mmTransaction, mmDB_AccessManager.get(), stockTaxProfileGeneratorVal );
   }
}

int TTaxProfileDBManager::getTaxProfileGeneratorValue(Database::TDBTransaction &transaction, ITaxProfileDBAccessManager* manager)
{
   return manager->GetTaxProfileGeneratorValue(transaction);
}

void TTaxProfileDBManager::setTaxProfileGeneratorValue(Database::TDBTransaction &transaction, ITaxProfileDBAccessManager* manager, int value)
{
   manager->SetTaxProfileGeneratorValue(transaction, value);
}

#pragma package(smart_init)
