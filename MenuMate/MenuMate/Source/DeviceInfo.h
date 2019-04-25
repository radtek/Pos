//---------------------------------------------------------------------------

#ifndef DeviceInfoH
#define DeviceInfoH
//---------------------------------------------------------------------------
#include <System.hpp>

enum DeviceType {devAll,devPC,devPalm,devPrinter,devIRModule,devPrinterModule,devUnknown,devWaiter,devKiosk};

class TDeviceInfo
{
   public:
   DeviceType Type;
//      int DeviceID;
   int LocationKey;
   int DeviceKey;
   UnicodeString Name;
   UnicodeString ComputerName;
   UnicodeString Product;
   UnicodeString Location;
   UnicodeString IP;
   UnicodeString TerminalID;
   int ProfileKey;
};

#endif
