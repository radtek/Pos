//---------------------------------------------------------------------------

#ifndef DB_5_3H
#define DB_5_3H

#include "MM_DBCore.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------
class DB_5_3
{
   public:
		static void Up_5_3(Database::TDBControl &DBControl);
		static void Down_5_3(Database::TDBControl &DBControl);
};
#endif
