//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBManager.h"

//---------------------------------------------------------------------------

TDBManager::TDBManager()
{
}

TaxProfileMap TDBManager::LoadTaxProfiles()
{
   TTaxProfileDBManager taxProfileDBManager;
   return taxProfileDBManager.LoadTaxProfiles();
}

bool TDBManager::SaveTaxProfiles(TaxProfileMap taxProfiles)
{
   TTaxProfileDBManager taxProfileDBManager;
   return taxProfileDBManager.SaveTaxProfiles(taxProfiles);
}

#pragma package(smart_init)
