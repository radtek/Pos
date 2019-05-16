//---------------------------------------------------------------------------

#ifndef DevicesH
#define DevicesH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <set>
#include "MM_DBCore.h"
#include "typedefChits.h"

//---------------------------------------------------------------------------
class TManagerDevices
{
	private :
	protected :
		bool fEnabled;
		void __fastcall SetEnabled(bool value);
		bool __fastcall GetEnabled();
	public:
		__property bool Enabled  = { read=GetEnabled, write=SetEnabled };
		TManagerDevices();
		virtual ~TManagerDevices();

		AnsiString GetDeviceName(Database::TDBTransaction &DBTransaction,long DeviceKey);
		void GetPrinterProfilesList(Database::TDBTransaction &DBTransaction,long DeviceKey, std::set<__int64> &DevicePrinterProfiles);
		void SetPrinterProfilesList(Database::TDBTransaction &DBTransaction,long DeviceKey, const std::set<__int64> &DevicePrinterProfiles);
		void GetDeviceList(Database::TDBTransaction &DBTransaction,int DeviceType,TStringList *Devices);
		void GetChitProfilesList(Database::TDBTransaction &DBTransaction,long DeviceKey, typChitDefaultMap &DeviceChitsDefaults);
		void SetChitProfilesList(Database::TDBTransaction &DBTransaction,long DeviceKey, const typChitDefaultMap &DeviceChitProfiles);
        void GetDeviceNameList(Database::TDBTransaction &DBTransaction,int DeviceType,TStringList *Devices);
        int GetDevicType(Database::TDBTransaction &DBTransaction,long DeviceKey);
};

#endif
 