//---------------------------------------------------------------------------

#ifndef DB_5_11H
#define DB_5_11H
//---------------------------------------------------------------------------

#include "MM_DBCore.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
class DB_5_11
{
   public:
		static void Up_5_11(Database::TDBControl &DBControl);
		static void Down_5_11(Database::TDBControl &DBControl);
};



#endif
