//---------------------------------------------------------------------------

#pragma hdrstop

#include "MenuTaxProfileProvider.h"

#include "ITaxProfileDBAccessManager.h"
#include "TaxProfileDBAccessManager_MM.h"

#include "LoadMenu.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::
// PUBLIC
//::::::::::::::::::::::::::::::::::::::::

TMenuTaxProfileProvider::TMenuTaxProfileProvider()
{
}
//---------------------------------------------------------------------------

void TMenuTaxProfileProvider::Reset()
{
   taxProfileMap.clear();   
}
//---------------------------------------------------------------------------

void TMenuTaxProfileProvider::Reset( TIBDatabase* inDatabase )
{
   std::auto_ptr<ITaxProfileDBAccessManager> mmDB_AccessManager( new TTaxProfileDBAccessManager_MM() );

   std::auto_ptr<Database::TDBTransaction> dbTransaction( new Database::TDBTransaction( inDatabase ) );

   dbTransaction->StartTransaction();
   taxProfileMap = mmDB_AccessManager.get()->LoadTaxProfiles( *dbTransaction.get() );
   dbTransaction->Commit();
}
//---------------------------------------------------------------------------

void TMenuTaxProfileProvider::Reset( TLoadMenu* inLoadMenu )
{
   taxProfileMap = loadTaxProfiles( inLoadMenu );
}
//---------------------------------------------------------------------------

void TMenuTaxProfileProvider::ReadAllTaxProfiles( TaxProfileList &inAllTPs )
{
   inAllTPs.clear();

   TaxProfileMap::iterator tpIT;

   for( tpIT = taxProfileMap.begin(); tpIT != taxProfileMap.end(); tpIT++ )
   {
      TaxProfile *tp = tpIT->second;

      if( tp->taxSelectable )
      {
        inAllTPs.insert( tp );
      }
   }
}
//---------------------------------------------------------------------------

void TMenuTaxProfileProvider::ReadTaxProfilesWithKeys(
                                TaxProfileKeyList inTPKeys,
                                TaxProfileList&   outTPs )
{
   TaxProfileMap::iterator tpIT;

   for( tpIT = taxProfileMap.begin(); tpIT != taxProfileMap.end(); tpIT++ )
   {
      TaxProfile *tp = tpIT->second;

      int tpKey = tp->GetTaxProfileDBKey();

      if( inTPKeys.find( tpKey ) != inTPKeys.end() )
      {
        outTPs.insert( tp );
      }
   }
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::
// PRIVATE
//::::::::::::::::::::::::::::::::::::::::

TaxProfileMap TMenuTaxProfileProvider::loadTaxProfiles( TLoadMenu* inLoadMenu )
{
   TaxProfileMap result;

   int taxProfileCount = inLoadMenu->AllTaxProfilesCount();

   if( taxProfileCount > 0 )
   {
      try
      {
         for( int i = 0; i < taxProfileCount; i++ )
         {
            TaxProfile *taxProfile = createTaxProfileAtIndex( i, inLoadMenu );

            TaxProfileKey key = taxProfile->GetUniqueProfileKeyPair();

            if( result.find( key ) == result.end() )
            {
               result.insert( std::pair<TaxProfileKey, TaxProfile*>( key, taxProfile ) );
            }
         }
      }
      catch( ... )
      {
      }
   }

   return result;
}
//---------------------------------------------------------------------------

TaxProfile* TMenuTaxProfileProvider::createTaxProfileAtIndex( __int32 inIndex, TLoadMenu* inLoadMenu )
{
   __int32    tpKey;
   WideString tpName;
   Currency   tpRate;
   __int32    tpType;
   __int32    tpPriority;
   //__int32    tpTaxCode;

   __int32 tpHandle = inLoadMenu->TaxProfileAtIndex(
                                    inIndex,
                                    tpKey,
                                    tpName,
                                    tpRate,
                                    tpType,
                                    tpPriority/*,
                                    tpTaxCode*/ );

   if( tpHandle == 0 )
   {
      throw Exception( "Tax Profile not found at index: " + IntToStr( inIndex ) );
   }

   //::::::::::::::::::::::::::::::::

   TaxProfile* result = new TaxProfile( tpKey );

   result->taxProfileName = tpName;
   result->taxPercentage  = tpRate;
   result->taxProfileType = tpType;
   result->taxPriority    = tpPriority;
   result->taxSelectable  = true;

   return result;
}
//---------------------------------------------------------------------------

