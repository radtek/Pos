//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <utilcls.h>
#include <wbemcli.h>
#include <iphlpapi.h>

#include "MM_DBCore.h"

#include "Registration.h"
#include "MD5Checksum.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TRegInfo::TRegInfo(Database::TDBControl &inIBDatabase) : IBDatabase(inIBDatabase)
{
}
//---------------------------------------------------------------------------
TRegInfo::~TRegInfo()
{
}
//---------------------------------------------------------------------------
// Will create entry in db ready for registering.
// Requires HardwareKey, TerminalName, ProductName
//---------------------------------------------------------------------------
void TRegInfo::AddRegistrationInformation(TMachineInfo *Info)
{
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	Info->HardwareKey	= GenerateHardwareKey();

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
		"Select * "
		"From "
			"Registration "
		"Where "
			"MachineKey = :MachineKey and "
			"Terminal_Name = :Terminal_Name and "
			"Product = :Product";
	IBInternalQuery->ParamByName("MachineKey")->AsString		= Info->HardwareKey;
	IBInternalQuery->ParamByName("Terminal_Name")->AsString	= Info->TerminalName;
	IBInternalQuery->ParamByName("Product")->AsString			= Info->ProductName;
	IBInternalQuery->ExecQuery();

	if(IBInternalQuery->RecordCount == 0)
	{
		IBInternalQuery->Close();
		
      
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"Insert into Registration ("
				"MachineKey,"
				"Terminal_Name,"
				"Product)"
			"Values ("
				":MachineKey,"
				":Terminal_Name,"
				":Product);";
		IBInternalQuery->ParamByName("MachineKey")->AsString		= Info->HardwareKey;
		IBInternalQuery->ParamByName("Terminal_Name")->AsString	= Info->TerminalName;
		IBInternalQuery->ParamByName("Product")->AsString			= Info->ProductName;
		IBInternalQuery->ExecQuery();
   }
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
// Returns true if registered. Will fill in TMachineInfo if entry in db exists.
// Will return true if registered for either the old system or the new system.
// Requires HardwareKey, TerminalName, ProductName
//---------------------------------------------------------------------------
bool TRegInfo::Registered(TMachineInfo *Info)
{
	bool Registered = false;

	Info->HardwareKey = GenerateHardwareKey();
	if (LoadRegistrationInformation(Info))
	{
		if (Info->RegistrationKey != "")
		{
			if(Info->RegistrationKey == GenerateRegistrationKey(Info))
			{
				if((Info->MenuMateModules & eExpires) &&
					(Info->Expiry >= Date()) && ((Date()+ 91) > Info->Expiry ))
				{
					Registered = true;
				}
				else if(!(Info->MenuMateModules & eExpires))
				{
					Registered = true;
				}
			}
		}
	}
	if (!Registered)
	{
		Info->HardwareKey = GenOldKeyHardware(Info->ProductName);
		if (LoadOldRegistrationInformation(Info))
		{
			if (OldRegistered(Info))
			{
				Registered = true;
				UpdateRegistration(Info);
			}
		}
	}
	return Registered;
}
//---------------------------------------------------------------------------
// Completes the TMachineInfo structure.
// Requires HardwareKey, TerminalName, ProductName
//---------------------------------------------------------------------------
bool TRegInfo::LoadRegistrationInformation(TMachineInfo *Info)
{
	bool FoundInfo = false;

	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
		"Select * "
		"From "
			"Registration "
		"Where "
			"MachineKey = :MachineKey and "
			"Terminal_Name = :Terminal_Name and "
			"Product = :Product";
	IBInternalQuery->ParamByName("MachineKey")->AsString		= Info->HardwareKey;
	IBInternalQuery->ParamByName("Terminal_Name")->AsString	= Info->TerminalName;
	IBInternalQuery->ParamByName("Product")->AsString			= Info->ProductName;
	IBInternalQuery->ExecQuery();

	if (IBInternalQuery->RecordCount)
	{
		Info->POSLicences				= IBInternalQuery->FieldByName("Pos_Licences")->AsInteger;
		Info->PalmLicences			= IBInternalQuery->FieldByName("Palm_Licences")->AsInteger;
		Info->OfficeLicences			= IBInternalQuery->FieldByName("Office_Licences")->AsInteger;      
		Info->MenuMateModules		= IBInternalQuery->FieldByName("MM_Modules")->AsInteger;
		Info->ProductName				= IBInternalQuery->FieldByName("Product")->AsString;
		Info->CompanyName				= IBInternalQuery->FieldByName("Company")->AsString;
		Info->RegistrationKey		= IBInternalQuery->FieldByName("RegKey")->AsString;
		Info->Expiry					= IBInternalQuery->FieldByName("Expiry")->AsDateTime;

		FoundInfo						= true;
	}
	else
	{
		Info->POSLicences				= 0;
		Info->PalmLicences			= 0;
		Info->OfficeLicences			= 0;
		Info->MenuMateModules		= 0;
		Info->CompanyName				= "";
		Info->RegistrationKey		= "";
		Info->Expiry					= 0;
	}

	DBTransaction.Commit();
	
	return FoundInfo;
}
//---------------------------------------------------------------------------
// Returns the registration key. Is stored in db
// Requires HardwareKey, TerminalName, ProductName, CompanyName
//---------------------------------------------------------------------------
UnicodeString TRegInfo::GenerateRegistrationKey(TMachineInfo *Info)
{
	Info->CustomerKey = GenerateCustomerKey(Info);
	CMD5Checksum CheckSum;
	if(Info->MenuMateModules & eExpires)
	{
		return CheckSum.GetMD5(	Info->CustomerKey +
										IntToHex(int(Info->POSLicences), 4) +
										IntToHex(int(Info->PalmLicences), 4) +
										IntToHex(int(Info->MenuMateModules), 4) +
										Info->Expiry.FormatString("dd-mm-yyyy") +
										IntToHex(int(Info->OfficeLicences), 4) +
										UnicodeString::StringOfChar('0', 32)); // Allow for future additions.
	}
	else
	{
		return CheckSum.GetMD5(	Info->CustomerKey +
										IntToHex(int(Info->POSLicences), 4) +
										IntToHex(int(Info->PalmLicences), 4) +
										IntToHex(int(Info->MenuMateModules), 4) +
										IntToHex(int(Info->OfficeLicences), 4) +
										UnicodeString::StringOfChar('0', 32)); // Allow for future additions.
	}
}
//---------------------------------------------------------------------------
// Returns the customer key. Is sent to IQW to register.
// Requires TerminalName, ProductName, CompanyName
//---------------------------------------------------------------------------
UnicodeString TRegInfo::GenerateCustomerKey(TMachineInfo *Info)
{
	if(Info->HardwareKey == "")
	{
		Info->HardwareKey = GenerateHardwareKey();
	}
	CMD5Checksum CheckSum;
	return CheckSum.GetMD5(Info->CompanyName + Info->HardwareKey + Info->ProductName + Info->TerminalName);
}
//---------------------------------------------------------------------------
// Attempts to register the system. If successful, updates and returns true.
// If unsuccessful, doesn't affect db and returns false.
// Requires almost everything
//---------------------------------------------------------------------------
bool TRegInfo::Register(TMachineInfo *Info)
{
	bool RetVal = false;

	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if (Info->RegistrationKey == GenerateRegistrationKey(Info))
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"Update "
				"Registration "
			"Set "
				"POS_Licences = :POS_Licences,"
				"Palm_Licences = :Palm_Licences,"
				"Office_Licences = :Office_Licences,"
				"MM_Modules = :MM_Modules,"
				"Company	 = :Company,"
				"RegKey	 = :RegKey, "
				"Expiry	 = :Expiry "
			"Where "
				"MachineKey	= :MachineKey and "
				"Terminal_Name = :Terminal_Name and "
				"Product = :Product";

		IBInternalQuery->ParamByName("POS_Licences")->AsInteger			= Info->POSLicences;
		IBInternalQuery->ParamByName("Palm_Licences")->AsInteger		= Info->PalmLicences;
		IBInternalQuery->ParamByName("Office_Licences")->AsInteger		= Info->OfficeLicences;
		IBInternalQuery->ParamByName("MM_Modules")->AsInteger			= Info->MenuMateModules;
		IBInternalQuery->ParamByName("Company")->AsString					= Info->CompanyName;
		IBInternalQuery->ParamByName("RegKey")->AsString					= Info->RegistrationKey;
		IBInternalQuery->ParamByName("MachineKey")->AsString				= Info->HardwareKey;
		IBInternalQuery->ParamByName("Product")->AsString					= Info->ProductName;
		IBInternalQuery->ParamByName("Terminal_Name")->AsString			= Info->TerminalName;
		IBInternalQuery->ParamByName("Expiry")->AsDate						= Info->Expiry;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RowsAffected < 1)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text =
				"Insert Into Registration ("
					"MachineKey,"
					"Terminal_Name,"
					"POS_Licences,"
					"Palm_Licences,"
					"MM_Modules,"
					"Office_Licences,"
					"Product,"
					"Company,"
					"RegKey,"
					"Expiry ) "
				"Values ("
					":MachineKey,"
					":Terminal_Name,"
					":POS_Licences,"
					":Palm_Licences,"
					":MM_Modules,"
					":Office_Licences,"
					":Product,"
					":Company,"
					":RegKey,"
					":Expiry);";
			IBInternalQuery->ParamByName("POS_Licences")->AsInteger		= Info->POSLicences;
			IBInternalQuery->ParamByName("Palm_Licences")->AsInteger	= Info->PalmLicences;
			IBInternalQuery->ParamByName("MM_Modules")->AsInteger		= Info->MenuMateModules;
			IBInternalQuery->ParamByName("Office_Licences")->AsInteger	= Info->OfficeLicences;
			IBInternalQuery->ParamByName("Product")->AsString				= Info->ProductName;
			IBInternalQuery->ParamByName("Company")->AsString				= Info->CompanyName;
			IBInternalQuery->ParamByName("RegKey")->AsString				= Info->RegistrationKey;
			IBInternalQuery->ParamByName("MachineKey")->AsString			= Info->HardwareKey;
			IBInternalQuery->ParamByName("Terminal_Name")->AsString		= Info->TerminalName;
			IBInternalQuery->ParamByName("Expiry")->AsDate					= Info->Expiry;
			IBInternalQuery->ExecQuery();
		}
		RetVal = true;
	}
	DBTransaction.Commit();
	return RetVal;
}
//---------------------------------------------------------------------------
UnicodeString TRegInfo::GenerateHardwareKey()
{
	UnicodeString		VolumeSerialNumber	= IntToHex(GetVolumeSerial(), 8);
	UnicodeString		MACAddress				= GetMACAddress();
	CMD5Checksum	CheckSum;

	return			CheckSum.GetMD5(VolumeSerialNumber + MACAddress);
}
//---------------------------------------------------------------------------
int TRegInfo::GetVolumeSerial()
{
	DWORD		SerialNumber;

	GetVolumeInformation(_T("C:\\"), NULL, NULL, &SerialNumber, NULL, NULL, NULL, NULL);
	return SerialNumber;
}
//---------------------------------------------------------------------------
UnicodeString TRegInfo::GetMACAddress()
{
	UnicodeString MACAddress = GetHlpMACAddress();
	if (MACAddress == "")
	{
		MACAddress = GetNetbiosMACAddress();
		if (MACAddress == "")
		{
			MACAddress = GetSystemInformation("SELECT MACAddress FROM Win32_NetworkAdapterConfiguration WHERE IPEnabled=TRUE").Trim();
		}
	}
	return MACAddress;
}
//---------------------------------------------------------------------------
UnicodeString TRegInfo::GetNetbiosMACAddress()
{
	ASTAT						Adapter;

	NCB						Ncb;
	UCHAR						uRetCode;
	char						NetName[50];
	LANA_ENUM				lenum;

	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command		= NCBENUM;
	Ncb.ncb_buffer			= (UCHAR *)&lenum;
	Ncb.ncb_length			= sizeof(lenum);
	uRetCode					= Netbios(&Ncb);

	for(int i=0; i<lenum.length; i++)
	{
		memset(&Ncb, 0, sizeof(Ncb));
		Ncb.ncb_command	= NCBRESET;
		Ncb.ncb_lana_num	= lenum.lana[i];

		uRetCode				= Netbios( &Ncb );

		memset(&Ncb, 0, sizeof (Ncb));
		Ncb.ncb_command	= NCBASTAT;
		Ncb.ncb_lana_num	= lenum.lana[i];

		strcpy(Ncb.ncb_callname, "*               ");
		Ncb.ncb_buffer		= (char *) &Adapter;
		Ncb.ncb_length		= sizeof(Adapter);

		uRetCode				= Netbios(&Ncb);
		if(uRetCode == 0)
		{
			UnicodeString Text;
			Text.printf(UnicodeString("%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X").w_str(),
								Adapter.adapt.adapter_address[0],
								Adapter.adapt.adapter_address[1],
								Adapter.adapt.adapter_address[2],
								Adapter.adapt.adapter_address[3],
								Adapter.adapt.adapter_address[4],
								Adapter.adapt.adapter_address[5]);
			return Text;
		}
	}
	return "";
}
//---------------------------------------------------------------------------
UnicodeString TRegInfo::GetHlpMACAddress()
{
	UnicodeString MACAddress;

	IP_ADAPTER_INFO *AdapterInfo = NULL;
	unsigned long Length = 0;

	HINSTANCE hLib = LoadLibrary(_T("rasapi32.dll"));
	if (hLib != NULL)
	{
		FreeLibrary(hLib);

		typedef DWORD (FAR WINAPI *TGetAdaptersInfo)(PIP_ADAPTER_INFO pAdapterInfo,PULONG pOutBufLen);
		hLib = LoadLibrary(_T("iphlpapi.dll"));
		if (hLib != NULL)
		{
			TGetAdaptersInfo GetAdaptersInfo = (TGetAdaptersInfo) GetProcAddress( hLib, "GetAdaptersInfo" );
			if (GetAdaptersInfo != NULL)
			{
				int Result = GetAdaptersInfo(NULL, &Length);
				if(Result == ERROR_BUFFER_OVERFLOW)
				{
					AdapterInfo = new IP_ADAPTER_INFO[Length];
					try
					{
						Result = GetAdaptersInfo(AdapterInfo, &Length);
						if(Result == ERROR_SUCCESS)
						{
							IP_ADAPTER_INFO *pAdapterInfo = AdapterInfo;
							do
							{
								if (UnicodeString(pAdapterInfo->Description).Pos("PPP") == 0)
								{
									MACAddress = "";
									for (unsigned int i=0; i<pAdapterInfo->AddressLength; i++)
									{
										if (i == pAdapterInfo->AddressLength - 1)
										{
											MACAddress = MACAddress + IntToHex(pAdapterInfo->Address[i], 2);
										}
										else
										{
											MACAddress = MACAddress + IntToHex(pAdapterInfo->Address[i], 2) + ":";
										}
									}
								}
								pAdapterInfo = pAdapterInfo->Next;
							}
							while(pAdapterInfo);
						}
					}
					__finally
					{
						delete[] AdapterInfo;
					}
				}
			}
			FreeLibrary(hLib);
		}
	}
	return MACAddress;
}
//---------------------------------------------------------------------------
UnicodeString TRegInfo::GetSystemInformation(UnicodeString Query)
{
	Variant Result;

	CoInitializeEx(0, COINIT_MULTITHREADED);
	CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,	NULL, EOAC_NONE, 0);

	IWbemLocator *t_Locator = NULL;
	HRESULT t_Result = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,	IID_IUnknown, (void **)&t_Locator);

	if (SUCCEEDED(t_Result))
	{
		BSTR t_Namespace = SysAllocString(L"root\\cimv2");
		if (t_Namespace)
		{
			IWbemServices *t_Service = NULL;
			t_Result = t_Locator->ConnectServer(t_Namespace, NULL, NULL, NULL, 0, NULL, NULL, &t_Service);

			if (SUCCEEDED(t_Result))
			{
				BSTR t_Query = SysAllocString(Query.w_str());
				BSTR t_QueryLanguage = SysAllocString( L"WQL" );

				if (t_Query && t_QueryLanguage)
				{
					IEnumWbemClassObject *t_Enumeration = NULL;
					t_Result = t_Service->ExecQuery(t_QueryLanguage, t_Query, 0, NULL, &t_Enumeration);

					if (SUCCEEDED(t_Result))
					{
						t_Enumeration->Reset();
						IWbemClassObject *t_ClassObject = NULL;
						ULONG t_Count = 0;

						while (t_Enumeration->Next(INFINITE, 1, &t_ClassObject, &t_Count) == WBEM_NO_ERROR)
						{
							BSTR t_PropertyName = NULL;
							CIMTYPE t_CimType;
							VARIANT t_Variant;
							VariantInit(&t_Variant);

							t_ClassObject->BeginEnumeration(WBEM_FLAG_NONSYSTEM_ONLY);
							while ( t_ClassObject->Next(0, &t_PropertyName, &t_Variant, &t_CimType, NULL) == WBEM_NO_ERROR)
							{
								Result = t_Variant;
								SysFreeString(t_PropertyName);
								VariantClear(&t_Variant);
							}
							t_ClassObject->Release();
						}
						t_Enumeration->Release();
					}
				}
				SysFreeString(t_Query);
				SysFreeString(t_QueryLanguage);
			}
			t_Service->Release();

			SysFreeString(t_Namespace);
		}
	}
	if (t_Locator) t_Locator->Release();

	return Result;
}
//---------------------------------------------------------------------------
void TRegInfo::Unregister(TMachineInfo *Info)
{
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
		"Update Registration "
		"Set "
			"RegKey = :RegKey "
		"Where "
			"MachineKey	= :MachineKey and "
			"Terminal_Name = :Terminal_Name and "
			"Product = :Product";
	IBInternalQuery->ParamByName("RegKey")->AsString	   			= "";
	IBInternalQuery->ParamByName("MachineKey")->AsString				= Info->HardwareKey;
	IBInternalQuery->ParamByName("Product")->AsString					= Info->ProductName;
	IBInternalQuery->ParamByName("Terminal_Name")->AsString			= Info->TerminalName;
	IBInternalQuery->ExecQuery();

	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void TRegInfo::Delete(TMachineInfo *Info)
{
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
		"Delete From "
			"Registration "
		"Where "
			"MachineKey	= :MachineKey and "
			"Terminal_Name = :Terminal_Name and "
			"Product = :Product";
	IBInternalQuery->ParamByName("MachineKey")->AsString		= Info->HardwareKey;
	IBInternalQuery->ParamByName("Terminal_Name")->AsString	= Info->TerminalName;
	IBInternalQuery->ParamByName("Product")->AsString			= Info->ProductName;
	IBInternalQuery->ExecQuery();

	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
UnicodeString KeyToFriendlyKey(UnicodeString Key, /*int ProductID, */int Index)
{
	UnicodeString TempKey = Key.UpperCase();
	UnicodeString SubSection;
	UnicodeString NewKey;

	for (int i=1; i<=30; i+= 5)
	{
		SubSection = TempKey.SubString(i, 5);
		NewKey += SubSection + CheckSum(SubSection) + " - ";
	}
	SubSection = TempKey.SubString(31, 2);
//	SubSection += IntToHex(ProductID, 1);
//	SubSection += IntToHex(Index, 2);
	SubSection += IntToHex(Index, 3);
	NewKey += SubSection + CheckSum(SubSection);
	return NewKey;
}
//---------------------------------------------------------------------------
UnicodeString CheckSum(UnicodeString Section)
{
	int Check = 0;
	for (int i=0; i<5; i++)
	{
		int Hex = 0;
		if (Section[i+1] >= '0' && Section[i+1] <= '9')
		{
			Hex = Section[i+1] - '0';
		}
		else if (Section[i+1] >= 'A' && Section[i+1] <= 'F')
		{
			Hex = Section[i+1] - 'A' + 10;
		}
		else if (Section[i+1] >= 'a' && Section[i+1] <= 'f')
		{
			Hex = Section[i+1] - 'a' + 10;
		}
		Check += Hex;
		if ((i + 1) % 2 == 0)
		{
			Check += Hex;
		}
	}
	Check /= 7;
	return IntToHex(Check, 1);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool TRegInfo::UpdateRegistration(TMachineInfo *Info)
{
	Info->HardwareKey = GenOldKeyHardware(Info->ProductName);
	if (LoadRegistrationInformation(Info))
	{
		UnicodeString OldHardwareKey = Info->HardwareKey;

		Info->HardwareKey = GenerateHardwareKey();
		Info->RegistrationKey = GenerateRegistrationKey(Info);

		if (Register(Info))
		{
			Database::TDBTransaction DBTransaction(IBDatabase);
			DBTransaction.StartTransaction();

			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery->Close();


			IBInternalQuery->SQL->Text = "Delete From Registration Where MachineKey = :MachineKey";
			IBInternalQuery->ParamByName("MachineKey")->AsString = OldHardwareKey;
			IBInternalQuery->ExecQuery();
			DBTransaction.Commit();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool TRegInfo::OldRegistered(TMachineInfo *Info)
{
	bool Retval = false;
	Info->HardwareKey = GenOldKeyHardware(Info->ProductName);
	if (LoadRegistrationInformation(Info))
	{
		if (Info->RegistrationKey != "")
		{
			if(Info->RegistrationKey == GenOldRegistrationKey(Info, GenOldKeyHardware(Info->ProductName)))
			{
				Retval = true;
			}
		}
	}
	return Retval;
}
//---------------------------------------------------------------------------
bool TRegInfo::LoadOldRegistrationInformation(TMachineInfo *Info)
{
	bool FoundInfo = false;

	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "Select * from Registration Where MachineKey = :MachineKey";
	IBInternalQuery->ParamByName("MachineKey")->AsString		= Info->HardwareKey;
	IBInternalQuery->ExecQuery();

	if (IBInternalQuery->RecordCount)
	{
		Info->POSLicences				= IBInternalQuery->FieldByName("Pos_Licences")->AsInteger;
		Info->PalmLicences			= IBInternalQuery->FieldByName("Palm_Licences")->AsInteger;
		Info->OfficeLicences			= IBInternalQuery->FieldByName("Office_Licences")->AsInteger;
		Info->MenuMateModules		= IBInternalQuery->FieldByName("MM_Modules")->AsInteger;
		Info->ProductName				= IBInternalQuery->FieldByName("Product")->AsString;
		Info->CompanyName				= IBInternalQuery->FieldByName("Company")->AsString;
		Info->RegistrationKey		= IBInternalQuery->FieldByName("RegKey")->AsString;
		Info->Expiry					= IBInternalQuery->FieldByName("Expiry")->AsDateTime;

		FoundInfo						= true;
	}
	else
	{
		Info->POSLicences				= 0;
		Info->PalmLicences			= 0;
		Info->OfficeLicences			= 0;
		Info->MenuMateModules		= 0;
		Info->CompanyName				= "";
		Info->RegistrationKey		= "";
	}

	DBTransaction.Commit();

	return FoundInfo;
}
//---------------------------------------------------------------------------
UnicodeString TRegInfo::GetOldMACAddress()
{
   typedef struct _ASTAT_
   {
      ADAPTER_STATUS adapt;
      NAME_BUFFER    NameBuff [30];
   } ASTAT, * PASTAT;

   ASTAT Adapter;

   NCB Ncb;
   UCHAR uRetCode;
   char NetName[50];
   int LanCardNumber = 0;
	UnicodeString ReturnCode = "123456789012"; // Default Mac Address.

   memset( &Ncb, 0, sizeof(Ncb) );
   Ncb.ncb_command = NCBRESET;
   Ncb.ncb_lana_num = LanCardNumber;

   uRetCode = Netbios( &Ncb );
   while(uRetCode == NRC_BRIDGE && LanCardNumber < 20)
   {
      LanCardNumber++;
      Ncb.ncb_lana_num = LanCardNumber;
      uRetCode = Netbios( &Ncb );
   }
   if(uRetCode != NRC_BRIDGE)
   {
      memset( &Ncb, 0, sizeof (Ncb) );
      Ncb.ncb_command = NCBASTAT;
      Ncb.ncb_lana_num = LanCardNumber;
      strcpy( Ncb.ncb_callname,  "*               " );
      Ncb.ncb_buffer = (char *) &Adapter;
      Ncb.ncb_length = sizeof(Adapter);
      uRetCode = Netbios( &Ncb );
      if ( uRetCode == 0 )
      {
			ReturnCode.printf( UnicodeString("The Ethernet Number is: %02x%02x%02x%02x%02x%02x").w_str(),
                  Adapter.adapt.adapter_address[0],
                  Adapter.adapt.adapter_address[1],
                  Adapter.adapt.adapter_address[2],
                  Adapter.adapt.adapter_address[3],
						Adapter.adapt.adapter_address[4],
						Adapter.adapt.adapter_address[5] );
      }
   }
   return ReturnCode;
}
//---------------------------------------------------------------------------
UnicodeString TRegInfo::GetOldDriveSerial()
{
	DWORD			VolumeSerialNumber;
	DWORD			MaximumComponentLength;
	DWORD			FileSystemFlags;
	UnicodeString	TheSerialNumber;

	if (GetVolumeInformation( _T("C:\\"),NULL,0,&VolumeSerialNumber,
						   &MaximumComponentLength,&FileSystemFlags,NULL,0))
	{
        TheSerialNumber = IntToHex(HIWORD(VolumeSerialNumber), 4) + IntToHex(LOWORD(VolumeSerialNumber), 4);
   }
   return TheSerialNumber;
}
//---------------------------------------------------------------------------
UnicodeString TRegInfo::GenOldCustomerKey(TMachineInfo * Info, UnicodeString MachineKey)
{
	if(MachineKey == "")
   {
		MachineKey = GenOldKeyHardware(Info->ProductName);
   }
	CMD5Checksum CheckSum;
	return CheckSum.GetMD5(Info->CompanyName + MachineKey);
}
//---------------------------------------------------------------------------
UnicodeString TRegInfo::GenOldKeyHardware(UnicodeString ProductName)
{
	UnicodeString		HDSerialNumber	= GetOldDriveSerial();
	UnicodeString		MACAddress		= GetOldMACAddress();
	CMD5Checksum	CheckSum;

	return CheckSum.GetMD5(HDSerialNumber + MACAddress + ProductName);
}
//---------------------------------------------------------------------------
UnicodeString TRegInfo::GenOldRegistrationKey(TMachineInfo *Info, UnicodeString MachineKey)
{
	Info->CustomerKey = GenOldCustomerKey(Info, MachineKey);
	CMD5Checksum CheckSum;
	return CheckSum.GetMD5(Info->ProductName +
									IntToStr((int)Info->POSLicences) +
									IntToStr((int)Info->PalmLicences) +
									Info->CustomerKey);
}
//---------------------------------------------------------------------------

bool TRegInfo::IsMagicMemories(Database::TDBTransaction &DBTransaction)
{
	DBTransaction.StartTransaction();

	bool RetVal = false;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "Select * from Registration Where Company Like 'Magic%';";
	IBInternalQuery->ExecQuery();

	if (IBInternalQuery->RecordCount)
		RetVal = true;
	return RetVal;
}

