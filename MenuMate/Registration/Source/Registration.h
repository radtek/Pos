//---------------------------------------------------------------------------

#ifndef RegistrationH
#define RegistrationH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <nb30.h>
#include <Controls.hpp>
#include "MM_DBCore.h"
#include "RegMods.h"

//---------------------------------------------------------------------------
typedef struct _ASTAT_
{
	ADAPTER_STATUS	adapt;
	NAME_BUFFER		NameBuff[30];
}ASTAT, *PASTAT;
//---------------------------------------------------------------------------
class TMachineInfo
{
private:
public:
	TMachineInfo() : POSLicences(0), PalmLicences(0), OfficeLicences(0), MenuMateModules(0), Index(0),Expiry(0) {};

	UnicodeString		CompanyName;
	UnicodeString		TerminalName;
	UnicodeString		HardwareKey;
	UnicodeString		CustomerKey;			// MD5 ([KeyHardware] [Company Name])
	UnicodeString		RegistrationKey;		// MD5 ([Prod Name] [Liciences] [Cust Key])
	UnicodeString		ProductName;
	int				Index;					// Index into DB ordered by Product:Terminal_Name:MachineKey
//	int				ProductID;

	unsigned int	POSLicences;
	unsigned int	PalmLicences;
	unsigned int	OfficeLicences;

	unsigned int	MenuMateModules;		// Bit field
	TDate		 		Expiry;
};
//---------------------------------------------------------------------------
class TRegInfo
{
private:
	Database::TDBControl &IBDatabase;
	bool				IPHlpAvailable;

	bool				LoadRegistrationInformation(TMachineInfo *Info);
	UnicodeString		GenerateRegistrationKey(TMachineInfo *Info);
	UnicodeString		GenerateHardwareKey();
	int				GetVolumeSerial();
	UnicodeString		GetNetbiosMACAddress();
	UnicodeString		GetHlpMACAddress();
	UnicodeString		GetSystemInformation(UnicodeString Query);

	bool				OldRegistered(TMachineInfo *Info);
	bool				UpdateRegistration(TMachineInfo *Info);
	bool				LoadOldRegistrationInformation(TMachineInfo *Info);
	UnicodeString		GetOldMACAddress();
	UnicodeString		GetOldDriveSerial();
	UnicodeString		GenOldCustomerKey(TMachineInfo * Info, UnicodeString MachineKey);
	UnicodeString		GenOldKeyHardware(UnicodeString ProductName);
	UnicodeString		GenOldRegistrationKey(TMachineInfo *Info, UnicodeString MachineKey);

public:
	TRegInfo(Database::TDBControl &inIBDatabase);
	~TRegInfo();

	bool				Registered(TMachineInfo * Info);
	void				AddRegistrationInformation(TMachineInfo *Info);

	bool				Register(TMachineInfo *Info);
	void				Unregister(TMachineInfo *Info);
	void				Delete(TMachineInfo *Info);

	UnicodeString		GenerateCustomerKey(TMachineInfo *Info);

	static bool IsMagicMemories(Database::TDBTransaction &DBTransaction);
    UnicodeString		GetMACAddress();
};
//---------------------------------------------------------------------------
//UnicodeString KeyToFriendlyKey(UnicodeString Key, int ProductID, int Index);
UnicodeString KeyToFriendlyKey(UnicodeString Key, int Index);
UnicodeString CheckSum(UnicodeString Key);
//---------------------------------------------------------------------------

#endif
