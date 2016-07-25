//---------------------------------------------------------------------------

#ifndef DB_5_2H
#define DB_5_2H

#include "MM_DBCore.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------
class DB_5_2
{
   public:
		static void Up_5_2(Database::TDBControl &DBControl);
		static void Down_5_2(Database::TDBControl &DBControl);
};
#endif
