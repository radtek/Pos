//---------------------------------------------------------------------------

#ifndef MenuTaxProfileProviderH
#define MenuTaxProfileProviderH

#include <Classes.hpp>

#include "MM_DBCore.h"
#include "TaxProfile.h"

#include <vector>
#include <set>

#include "TaxProfile.h"
#include "TaxProfileAssigner.h"

//---------------------------------------------------------------------------

class TLoadMenu;

class TMenuTaxProfileProvider
{
   public:
      TMenuTaxProfileProvider();

      void Reset();
      void Reset( TIBDatabase* inDatabase );
      void Reset( TLoadMenu*   inLoadMenu );

      void ReadAllTaxProfiles( TaxProfileList& inAllTPs );
      void ReadTaxProfilesWithKeys(
              TaxProfileKeyList inTPKeys,
              TaxProfileList&   outTPs );
   private:
      TaxProfileMap taxProfileMap;

      TaxProfileMap loadTaxProfiles( TLoadMenu* inLoadMenu );
      TaxProfile*   createTaxProfileAtIndex( __int32 inIndex, TLoadMenu* inLoadMenu );

};

//---------------------------------------------------------------------------

#endif
