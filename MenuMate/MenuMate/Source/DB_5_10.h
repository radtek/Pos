//---------------------------------------------------------------------------

#ifndef DB_5_10H
#define DB_5_10H
//---------------------------------------------------------------------------

#include "MM_DBCore.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
class DB_5_10
{
   public:
		static void Up_5_10(Database::TDBControl &DBControl);
		static void Down_5_10(Database::TDBControl &DBControl);
};



#endif
