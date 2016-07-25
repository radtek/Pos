//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Connections.h"
#include "MMRegistry.h"
#include "MMMessageBox.h"
#include <string.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/*//---------------------------------------------------------------------------
bool TConnectionDetails::RASGetEntries(TStrings *Strings)
{
	return RASDial.GetEntries(Strings);
}
//---------------------------------------------------------------------------
bool TConnectionDetails::RASCreateEntry()
{
	return RASDial.CreateEntry();
}
//---------------------------------------------------------------------------
bool TDBDetails::Dial(AnsiString &ErrorMsg)
{
	HRASCONN Handle = RASDial.Dial(RASEntry, ErrorMsg);
	ClientIP = RASDial.ClientIP;
	ServerIP = RASDial.ServerIP;
	return (Handle != 0);
}*/

//---------------------------------------------------------------------------
TRASDial::TRASDial()
{
	hLib = LoadLibrary("rasapi32.dll");
	if (hLib != NULL)
	{
		RasGetEntryDialParams	= (TRasGetEntryDialParams) GetProcAddress(hLib, "RasGetEntryDialParamsA");
		RasDial						= (TRasDial) GetProcAddress(hLib, "RasDialA");
		RasGetProjectionInfo		= (TRasGetProjectionInfo) GetProcAddress(hLib, "RasGetProjectionInfoA");
		RasGetErrorString			= (TRasGetErrorString) GetProcAddress(hLib, "RasGetErrorStringA");
		RasHangUp					= (TRasHangUp) GetProcAddress(hLib, "RasHangUpA");
		RasGetConnectStatus		= (TRasGetConnectStatus) GetProcAddress(hLib, "RasGetConnectStatusA");
		RasEnumEntries				= (TRasEnumEntries) GetProcAddress(hLib, "RasEnumEntriesA");
		RasEnumConnections		= (TRasEnumConnections) GetProcAddress(hLib, "RasEnumConnectionsA");
		RasCreatePhonebookEntry	= (TRasCreatePhonebookEntry) GetProcAddress(hLib, "RasCreatePhonebookEntryA");
   	RasGetEntryProperties 	= (TRasGetEntryProperties) GetProcAddress(hLib, "RasGetEntryPropertiesA");
   	RasSetEntryDialParams 	= (TRasSetEntryDialParams) GetProcAddress(hLib, "RasSetEntryDialParamsA");
   	RasEditPhonebookEntry 	= (TRasEditPhonebookEntry) GetProcAddress(hLib, "RasEditPhonebookEntryA");

		FreeLibrary(hLib);
	}
	else
	{
		RasGetEntryDialParams	= NULL;
		RasDial						= NULL;
		RasGetProjectionInfo		= NULL;
		RasGetErrorString			= NULL;
		RasHangUp					= NULL;
		RasGetConnectStatus		= NULL;
		RasEnumEntries				= NULL;
		RasEnumConnections		= NULL;
		RasCreatePhonebookEntry	= NULL;
   	RasGetEntryProperties 	= NULL;
   	RasSetEntryDialParams 	= NULL;
   	RasEditPhonebookEntry 	= NULL;
	}
   Handle = 0;
}
//---------------------------------------------------------------------------
TRASDial::~TRASDial()
{
	if (hLib != NULL)
	{
		FreeLibrary(hLib);
	}
}
//---------------------------------------------------------------------------
void TRASDial::Dial(AnsiString Entry, AnsiString &ErrorMsg)
{
	if (!hLib || !RasGetEntryDialParams || !RasGetProjectionInfo || !RasDial || !RasGetErrorString || Handle != 0)
	{
		return ;
	}

	BOOL				RetrievePassword;
	RASDIALPARAMS	DialParams;
   RASENTRYA		RasEntry;
	memset(&DialParams, sizeof(RASDIALPARAMS), 0);
	memset(&RasEntry, sizeof(RASENTRYA), 0);
	DialParams.dwSize = sizeof(RASDIALPARAMS);
   unsigned long RasSize = sizeof(RASENTRYA);
	bool proceed = false;

   RasGetEntryProperties(
       NULL,										// pointer to full path and filename of phone-book file
       AnsiString ("LogsMate").c_str(),	// pointer to an entry name
       &RasEntry,									// buffer that receives entry information
       &RasSize,						// size, in bytes, of the lpRasEntry buffer
       NULL,										// buffer that receives device-specific configuration information
       NULL											// size, in bytes, of the lpbDeviceInfo buffer
      );

   StrPCopy(DialParams.szEntryName, "LogsMate");
   if (RasGetEntryDialParams(NULL, &DialParams, &RetrievePassword) == 0)
   {
      if(strcmp(DialParams.szUserName,AnsiString("gel/cblaikie").c_str()) == 0)
      {
         StrPCopy(DialParams.szPassword, "logix500");
      }
      else if(UserName == "gel/cblaikie")
      {
         StrPCopy(DialParams.szPassword, "logix500");
      }
      else
      {
         StrPCopy(DialParams.szUserName, UserName);
         StrPCopy(DialParams.szPassword, Password);
      }
   }

   int Result = RasDial(NULL, NULL, &DialParams, 0, NULL, &Handle);
   if (Result == 0)
   {
      RASPPPIP	RASPppIp;
      DWORD		lpcb;

      memset(&RASPppIp, sizeof(RASPPPIP), 0);
      RASPppIp.dwSize = sizeof(RASPPPIP);
      lpcb = RASPppIp.dwSize;

   }
   else
   {
      char TempMessage[256];
      RasGetErrorString(Result, TempMessage, 256);
      ErrorMsg = TempMessage;
      if (Handle != 0)
      {
         HangUp();
         Handle = 0;
      }
   }
}
//---------------------------------------------------------------------------
bool TRASDial::HangUp()
{
	if (!hLib || !RasHangUp || !RasGetConnectStatus)
	{
		return false;
	}
	RASCONNSTATUS Status;

	memset(&Status, sizeof(RASCONNSTATUS), 0);
	Status.dwSize = sizeof(RASCONNSTATUS);

	if (RasHangUp(Handle) != 0)
	{
		return false;
	}
//	DWORD Result = RasGetConnectStatus(Handle, &Status);
	while (RasGetConnectStatus(Handle, &Status) != ERROR_INVALID_HANDLE)
	{
//		char TempMessage[256];
//		RasGetErrorString(Result, TempMessage, 256);

		Sleep(0);
		Application->ProcessMessages();
	}
   Handle = 0;
	return true;
}
//---------------------------------------------------------------------------
bool TRASDial::GetEntries(TStrings *Strings)
{
	if (!hLib || !RasEnumEntries)
	{
		return false;
	}
	bool Success = false;

	RASENTRYNAME	Entries[MaxEntries];
	DWORD				BufferSize;
	DWORD				EntryCount;

	Strings->Clear();
	Entries[0].dwSize = sizeof(RASENTRYNAME);
	BufferSize = sizeof(RASENTRYNAME) * MaxEntries;

	if (RasEnumEntries(NULL, NULL, Entries, &BufferSize, &EntryCount) == 0)
	{
		Success = true;
		for (DWORD i=0; i<EntryCount; i++)
		{
			Strings->Add(Entries[i].szEntryName);
		}
	}
	return Success;
}
//---------------------------------------------------------------------------
HRASCONN TRASDial::GetConnected(AnsiString Entry)
{
	if (!hLib || !RasEnumConnections)
	{
		return NULL;
	}
	bool Success = false;
	HRASCONN Handle;//hrasconn

	RASCONN			Entries[MaxEntries];
	DWORD				BufferSize;
	DWORD				EntryCount;
	RASCONNSTATUS	Status;

	Entries[0].dwSize = sizeof(RASCONN);
	BufferSize = sizeof(RASCONN) * MaxEntries;

	DWORD RasResult = RasEnumConnections(Entries, &BufferSize, &EntryCount);
	if (RasResult == 0)
	{
		for (DWORD i=0; i<EntryCount; i++)
		{
			if (Entry == Entries[i].szEntryName)
			{
//				RasGetConnectStatus(Entries[i].hrasconn, &Status);
//				if (Status.rasconnstate == RASCS_Connected)
//				{
					Success = true;
					Handle = Entries[i].hrasconn;
					break;
//				}
			}
		}
	}
	else
	{
		MessageBox(UnicodeString("RasEnumConnections failed - Err:" + IntToStr((int)RasResult)).c_str(), "Error", MB_OK + MB_ICONERROR);
	}
	if (Success) return Handle;
	else return 0;
}
//---------------------------------------------------------------------------
bool TRASDial::GetEntryDetails(AnsiString Entry, AnsiString &UserName, AnsiString &Password)
{
	if (!hLib || !RasGetEntryDialParams)
	{
		return false;
	}
	bool Success = false;

	RASDIALPARAMS	DialParams;
	BOOL				RetrievePassword;

	memset(&DialParams, sizeof(RASDIALPARAMS), 0);
	DialParams.dwSize = sizeof(RASDIALPARAMS);
	StrPCopy(DialParams.szEntryName, Entry);

	if (RasGetEntryDialParams(NULL, &DialParams, &RetrievePassword) == 0)
	{
		UserName = DialParams.szUserName;
		if (RetrievePassword)
		{
			Password = DialParams.szPassword;
		}
		else
		{
			Password = "";
		}
		Success = true;
	}
	return Success;
}
//---------------------------------------------------------------------------
bool TRASDial::CreateEntry()
{
   bool RetVal = true;
	if (!hLib || !RasCreatePhonebookEntry)
	{
		return false;
	}
   else
   {
		MessageBox("You need to Name the Dial up connection 'LogsMate'", "Info", MB_OK + MB_ICONINFORMATION);
      TStringList *Strings = new TStringList;
		try
      {
         GetEntries(Strings);
         Strings->CaseSensitive == false;
         if(Strings->IndexOf("LogsMate") == -1)
         {
				RetVal = RasCreatePhonebookEntry(NULL,NULL);
         }
         else
         {
            RasEditPhonebookEntry(
                         NULL,	// handle to the parent window of the dialog box
                         NULL,	// pointer to the full path and filename of the phone-book file
                         AnsiString("LogsMate").c_str()	// pointer to the phone-book entry name
                        );
         }
      }
      __finally
      {
      	delete Strings;
      }
   }
	return RetVal;
}
//---------------------------------------------------------------------------

