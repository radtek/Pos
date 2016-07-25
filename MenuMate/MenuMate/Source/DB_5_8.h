//---------------------------------------------------------------------------

#ifndef DB_5_8H
#define DB_5_8H
//---------------------------------------------------------------------------

#include "MM_DBCore.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
class DB_5_8
{
   public:
		static void Up_5_8(Database::TDBControl &DBControl);
		static void Down_5_8(Database::TDBControl &DBControl);
};



#endif
