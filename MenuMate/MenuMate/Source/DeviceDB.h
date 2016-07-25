// ---------------------------------------------------------------------------

#ifndef DeviceDBH
#define DeviceDBH
// ---------------------------------------------------------------------------
#include "DeviceImage.h"
#include "DeviceStatusBase.h"
#include "MM_DBCore.h"
#include "ManagerVariable.h"
#include <memory>

class TDeviceDB : public TDeviceImage
{
public:
   TDeviceStatusBase Status;

   TDeviceDB();
   void Copy(TDeviceDB *Image);

   virtual void Load(Database::TDBTransaction &DBTransaction);
   int Locate(Database::TDBTransaction &DBTransaction);
   int IPToKey(Database::TDBTransaction &DBTransaction, UnicodeString DeviceIP);
   int NameToKey(Database::TDBTransaction &DBTransaction, UnicodeString DeviceName);
   void Create(Database::TDBTransaction &DBTransaction);
   void Destroy(Database::TDBTransaction &DBTransaction);
   virtual __fastcall ~TDeviceDB();
   virtual void Initialise(Database::TDBTransaction &DBTransaction);
   virtual void CheckLocation(Database::TDBTransaction & DBTransaction) = 0;
   void SetProfileKey(Database::TDBTransaction &DBTransaction, int ProfileKey);
   void CreateProfile(Database::TDBTransaction &DBTransaction);
   void CreateDevice(Database::TDBTransaction &DBTransaction);
};

#endif
