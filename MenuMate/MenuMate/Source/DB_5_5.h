//---------------------------------------------------------------------------
#ifndef DB_5_5H
#define DB_5_5H
//---------------------------------------------------------------------------

#include "MM_DBCore.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
class DB_5_5
{
   public:
		static void Up_5_5(Database::TDBControl &DBControl);
		static void Down_5_5(Database::TDBControl &DBControl);
};

#endif
