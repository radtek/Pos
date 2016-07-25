//---------------------------------------------------------------------------

#ifndef GeneratorManagerH
#define GeneratorManagerH

#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include "MM_DBCore.h"
//---------------------------------------------------------------------------

class TGeneratorManager
{
   public:
      static int GetNextGeneratorKey(
                        Database::TDBTransaction &transaction,
                        AnsiString generatorName);
};

#endif
