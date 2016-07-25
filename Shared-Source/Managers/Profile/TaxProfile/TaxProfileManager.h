//---------------------------------------------------------------------------

#ifndef TaxProfileManagerH
#define TaxProfileManagerH

#include "DBManager.h"

/*
 * TTaxProfileManager handles all the tasks related to tax profiles
 * any logic should be implemented here
 * database access should be done via the DBManager interface
 */
class TTaxProfileManager
{
   public:
		TaxProfileMap LoadTaxProfiles();
		bool  SaveTaxProfiles(TaxProfileMap taxProfiles);
};
#endif
