//---------------------------------------------------------------------------

#ifndef DB_5_9H
#define DB_5_9H
//---------------------------------------------------------------------------

#include "MM_DBCore.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
class DB_5_9
{
   public:
		static void Up_5_9(Database::TDBControl &DBControl);
		static void Down_5_9(Database::TDBControl &DBControl);
};



#endif
