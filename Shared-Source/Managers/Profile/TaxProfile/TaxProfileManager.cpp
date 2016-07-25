//---------------------------------------------------------------------------


#pragma hdrstop

#include "TaxProfileManager.h"

//---------------------------------------------------------------------------

TaxProfileMap TTaxProfileManager::LoadTaxProfiles()
{
   //todo: create a thread and call the database from there
   TDBManager dbManager;
   return dbManager.LoadTaxProfiles();
}

bool  TTaxProfileManager::SaveTaxProfiles(TaxProfileMap taxProfiles)
{
   //todo: create a thread and call database from there
   TDBManager dbManager;
   return dbManager.SaveTaxProfiles(taxProfiles);
}

#pragma package(smart_init)
