//---------------------------------------------------------------------------

#ifndef DBManagerH
#define DBManagerH

#include "TaxProfileDBManager.h"

/*
 * TDBManager is the one central place where DB access calls will go through
 * This class uses set of other DBManager classes to perform specific tasks. ie: TaxProfileDBManager for tax profile tasks
 */
class TDBManager
{
   public:
      TDBManager();
      TaxProfileMap  LoadTaxProfiles();
      bool           SaveTaxProfiles(TaxProfileMap taxProfiles);
};
#endif
