//---------------------------------------------------------------------------

#ifndef RegistrationH
#define RegistrationH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <nb30.h>
#include <Controls.hpp>


enum eRegisteredModules {eRegKitchenScreen = 0x0001,eRegRooms = 0x0002,eRegSaleTurnAround = 0x0004,eRegMembers = 0x0008,
								 eMenuMateDirect = 0x0010,eEFTPOS = 0x0020,eExpires = 0x0040,
								 eOfficeMYOB = 0x1000};
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

	AnsiString		CompanyName;
	AnsiString		TerminalName;
	AnsiString		HardwareKey;
	AnsiString		CustomerKey;			// MD5 ([KeyHardware] [Company Name])
	AnsiString		RegistrationKey;		// MD5 ([Prod Name] [Liciences] [Cust Key])
	AnsiString		ProductName;
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
	TIBDatabase		*IBDatabase;
	TIBTransaction	*IBTransaction;
	TIBQuery			*IBQuery;

	bool				IPHlpAvailable;

	bool				LoadRegistrationInformation(TMachineInfo *Info);
	AnsiString		GenerateRegistrationKey(TMachineInfo *Info);
	AnsiString		GenerateHardwareKey();
	int				GetVolumeSerial();
	AnsiString		GetMACAddress();
	AnsiString		GetNetbiosMACAddress();
	AnsiString		GetHlpMACAddress();
	AnsiString		GetSystemInformation(WideString Query);

	bool				OldRegistered(TMachineInfo *Info);
	bool				UpdateRegistration(TMachineInfo *Info);
	bool				LoadOldRegistrationInformation(TMachineInfo *Info);
	AnsiString		GetOldMACAddress();
	AnsiString		GetOldDriveSerial();
	AnsiString		GenOldCustomerKey(TMachineInfo * Info, AnsiString MachineKey);
	AnsiString		GenOldKeyHardware(AnsiString ProductName);
	AnsiString		GenOldRegistrationKey(TMachineInfo *Info, AnsiString MachineKey);

public:
	TRegInfo(TIBDatabase *ibDatabase);
	~TRegInfo();

	bool				Registered(TMachineInfo * Info);
	void				AddRegistrationInformation(TMachineInfo *Info);

	bool				Register(TMachineInfo *Info);
	void				Unregister(TMachineInfo *Info);
	void				Delete(TMachineInfo *Info);

	AnsiString		GenerateCustomerKey(TMachineInfo *Info);
};
//---------------------------------------------------------------------------
//AnsiString KeyToFriendlyKey(AnsiString Key, int ProductID, int Index);
AnsiString KeyToFriendlyKey(AnsiString Key, int Index);
AnsiString CheckSum(AnsiString Key);
//---------------------------------------------------------------------------

#endif
