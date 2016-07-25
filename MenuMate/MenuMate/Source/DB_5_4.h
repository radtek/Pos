//---------------------------------------------------------------------------

#ifndef DB_5_4H
#define DB_5_4H

#include "MM_DBCore.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
class DB_5_4
{
   public:
		static void Up_5_4(Database::TDBControl &DBControl);
		static void Down_5_4(Database::TDBControl &DBControl);
};
#endif
