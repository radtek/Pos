//---------------------------------------------------------------------------

#ifndef TablesH
#define TablesH
//---------------------------------------------------------------------------
#include "Manager.h"

class TTables : public TManager
{
   public :
   void SetSeatName(Database::TDBControl &IBDatabase,UnicodeString Name, int TableNo, int SeatNumber);
};
#endif
