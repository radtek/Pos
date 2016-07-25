//---------------------------------------------------------------------------

#ifndef DB_5_7H
#define DB_5_7H
//---------------------------------------------------------------------------


#include "MM_DBCore.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
class DB_5_7
{
   public:
		static void Up_5_7(Database::TDBControl &DBControl);
		static void Down_5_7(Database::TDBControl &DBControl);
};

#endif
