//---------------------------------------------------------------------------

#ifndef DB_5_12H
#define DB_5_12H
//---------------------------------------------------------------------------


#include "MM_DBCore.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
class DB_5_12
{
   public:
		static void Up_5_12(Database::TDBControl &DBControl);
		static void Down_5_12(Database::TDBControl &DBControl);
};


#endif
