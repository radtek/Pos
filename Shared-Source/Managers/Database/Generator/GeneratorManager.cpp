//---------------------------------------------------------------------------


#pragma hdrstop

#include "GeneratorManager.h"

//---------------------------------------------------------------------------

int TGeneratorManager::GetNextGeneratorKey(
                              Database::TDBTransaction &transaction,
                              AnsiString generatorName)
{
   int generatorValue = 0;
   TIBSQL* query = transaction.Query(transaction.AddQuery());
   query->SQL->Text = "select gen_id(" + generatorName + ",1) from RDB$DATABASE";
   query->ExecQuery();

   generatorValue = query->Fields[0]->AsInteger;
   query->Close();

   return generatorValue;
}

#pragma package(smart_init)

