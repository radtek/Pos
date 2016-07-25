//---------------------------------------------------------------------------

#ifndef DB_5_6H
#define DB_5_6H
//---------------------------------------------------------------------------

#include "MM_DBCore.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
class DB_5_6
{
   public:
		static void Up_5_6(Database::TDBControl &DBControl);
		static void Down_5_6(Database::TDBControl &DBControl);
};



#endif
