//---------------------------------------------------------------------------

#ifndef DB_5_16H
#define DB_5_16H
//---------------------------------------------------------------------------

#include "MM_DBCore.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
class DB_5_16
{
   public:
        static void Apply(     Database::TDBControl &DBControl );
		static void Up_5_16(   Database::TDBControl &DBControl );
		static void Down_5_16( Database::TDBControl &DBControl );
};

#endif
