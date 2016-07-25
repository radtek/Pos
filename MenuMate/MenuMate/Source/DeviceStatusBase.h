//---------------------------------------------------------------------------

#ifndef DeviceStatusBaseH
#define DeviceStatusBaseH
//---------------------------------------------------------------------------
#include <Classes.hpp>

class TDeviceStatusBase
{
      public:
      bool Active;
      long LastActive;
		UnicodeString ErrorMsg;
      int Error;
      int TransactionNumber;
};
#endif
