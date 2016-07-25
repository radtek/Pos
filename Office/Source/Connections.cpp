//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Connections.h"
#include "Consts.h"
#include "MMRegistry.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TConnectionDetails CurrentConnection;
//---------------------------------------------------------------------------
bool TConnectionDetails::LoadSettings()
{
	if (RegistryKeyExists(OfficeKey + "\\" + CompanyName))
	{
		AnsiString Key = OfficeKey + "\\" + CompanyName;
		AnsiString Text;

		Text = "";
		RegistryRead(Key, "MMDataFile", Text);
		CurrentConnection.MenuMateDB.DBName					= Text;
		Text = "";
		RegistryRead(Key, "StockDataFile", Text);
		CurrentConnection.StockDB.DBName					= Text;
		Text = "";
		RegistryRead(Key, "ChefMateDataFile", Text);
		CurrentConnection.ChefMateDB.DBName					= Text;

		Text = "";
		RegistryRead(Key, "CompanyDetails", Text);
		CurrentConnection.ReportHeader						= Text;
		Text = "";
		RegistryRead(Key, "DeliveryAddress", Text);
		CurrentConnection.DeliveryAddress					= Text;
		Text = "";
		RegistryRead(Key, "OrderInstructions", Text);
		CurrentConnection.OrderInstructions					= Text;

		Text = "";
		RegistryRead(Key, "MenuCommitPath", Text);
		CurrentConnection.ServerPath							= Text;
		Text = "";
		RegistryRead(Key, "MenuSavePath", Text);
		CurrentConnection.MenuSavePath						= Text;

		Text = "";
		RegistryRead(Key + "\\Payroll", "PayrollSystem", Text);
		CurrentConnection.PayrollSystem						= Text;
		Text = "";
		RegistryRead(Key + "\\Payroll", "ExportPath", Text);
		CurrentConnection.PayrollExportPath					= Text;

		Text = "";
		RegistryRead(Key + "\\MYOB", "AccountSystem", Text);
		CurrentConnection.AccountSystem						= Text;
		Text = "";
		RegistryRead(Key + "\\MYOB", "MYOBPath", Text);
		CurrentConnection.MYOBPath								= Text;
		Text = "";
		RegistryRead(Key + "\\MYOB", "MYOBJobCode", Text);
		CurrentConnection.MYOBJobCode							= Text;
		Text = "";
		RegistryRead(Key + "\\MYOB", "SalesInvoiceExportAccount", Text);
		CurrentConnection.SalesInvoiceExportAccount		= Text;
		Text = "";
		RegistryRead(Key + "\\MYOB", "SalesInvoiceExportType", Text);
		CurrentConnection.SalesInvoiceExportType			= Text;

		Text = "";
		RegistryRead(Key + "\\Stocktake", "StocktakePath", Text);
		CurrentConnection.StocktakePath = Text;
		Text = "Product.txt";
		RegistryRead(Key + "\\Stocktake", "StocktakeExportFile", Text);
		CurrentConnection.StocktakeExportFile = Text;
		Text = "Stock.txt";
		RegistryRead(Key + "\\Stocktake", "StocktakeImportFile", Text);
		CurrentConnection.StocktakeImportFile = Text;


		Text = "-1";
		RegistryRead(Key + "\\Stocktake", "StocktakeBarcodePos", Text);
		CurrentConnection.StocktakeBarcodePos = StrToInt(Text);
		Text = "-1";
		RegistryRead(Key + "\\Stocktake", "StocktakeQtyPos", Text);
		CurrentConnection.StocktakeQtyPos = StrToInt(Text);

//		Text = "0";
//		RegistryRead(Key, "IncludeGST", Text);
//		CurrentConnection.IncludeGST	= (Text == "1");

		Text = "0";
		RegistryRead(Key, "UseSerialBarcodeReader", Text);
		CurrentConnection.UseSerialBarcodeReader	= (Text == "1");
		Text = "-1";
		RegistryRead(Key, "SerialBarcodeReaderPort", Text);
		CurrentConnection.SerialBarcodeReaderPort	= StrToInt(Text);



		Text = "";  // cww
		RegistryRead(Key, "VIPCreditName", Text);
		CurrentConnection.VIPCreditName	= Text;

		Text = "0";
		RegistryRead(Key, "HideStocktakeOnHand", Text);
		CurrentConnection.HideStocktakeOnHand = (Text == "1");

		Text = "15";
		RegistryRead(Key, "DefaultMenuGST", Text);
		CurrentConnection.DefaultMenuGST	= StrToFloat(Text);
		Text = "15";
		RegistryRead(Key, "DefaultStockGST", Text);
		CurrentConnection.DefaultStockGST	= StrToFloat(Text);

		Text = "";
		RegistryRead(Key, "GSTNumber", Text);
		CurrentConnection.GSTNumber = Text;

		Text = "0";
		RegistryRead(Key, "SingleLocation", Text);
		CurrentConnection.SingleLocation	= (Text == "1");
		Text = "";
		RegistryRead(Key, "DefaultLocation", Text);
		CurrentConnection.DefaultLocation	= Text;

		Text = "0";
		RegistryRead(Key, "RemoteMM", Text);
		CurrentConnection.MenuMateDB.RemoteConnection	    = (Text == "1");
		Text = "";
		RegistryRead(Key, "RemoteMMEntry", Text);
		CurrentConnection.MenuMateDB.RASEntry				= Text;
		Text = "";
		RegistryRead(Key, "RemoteMMUsername", Text);
		CurrentConnection.MenuMateDB.RASUserName			= Text;
		Text = "";
		RegistryRead(Key, "RemoteMMPassword", Text);
		CurrentConnection.MenuMateDB.RASPassword			= Text;

		Text = "0";
		RegistryRead(Key, "RemoteStock", Text);
		CurrentConnection.StockDB.RemoteConnection		    = (Text == "1");
		Text = "";
		RegistryRead(Key, "RemoteStockEntry", Text);
		CurrentConnection.StockDB.RASEntry					= Text;
		Text = "";
		RegistryRead(Key, "RemoteStockUsername", Text);
		CurrentConnection.StockDB.RASUserName				= Text;
		Text = "";
		RegistryRead(Key, "RemoteStockPassword", Text);
		CurrentConnection.StockDB.RASPassword				= Text;

		Text = "0";
		RegistryRead(Key, "RemoteChefMate", Text);
		CurrentConnection.ChefMateDB.RemoteConnection		= (Text == "1");
		Text = "";
		RegistryRead(Key, "RemoteChefMateEntry", Text);
		CurrentConnection.ChefMateDB.RASEntry				= Text;
		Text = "";
		RegistryRead(Key, "RemoteChefMateUsername", Text);
		CurrentConnection.ChefMateDB.RASUserName			= Text;
		Text = "";
		RegistryRead(Key, "RemoteChefMatePassword", Text);
		CurrentConnection.ChefMateDB.RASPassword			= Text;

          Text = "0";
        RegistryRead(Key , "SettingDecimalPlaces", Text);
        CurrentConnection.SettingDecimalPlaces = StrToInt(Text);

        RegistryRead(Key, "DontShowItemCost",
                     CurrentConnection.DontShowItemCostInPurchaseOrder);



        Text = "0";
		RegistryRead(Key, "AutoPrintStockTransferAudit", Text);
		CurrentConnection.AutoPrintStockTransferAudit	= (Text == "1");
		Text = "";

        Text = "0";
		RegistryRead(Key, "AutoPrintReceiveTransferAudit", Text);
		CurrentConnection.AutoPrintReceiveTransferAudit	= (Text == "1");
		Text = "";

        Text = "0";
		RegistryRead(Key, "SuppliersFromDefaultLocationsOnly", Text);
		CurrentConnection.SuppliersFromDefaultLocationsOnly	= (Text == "1");
		Text = "";

    /*    Text = "15";
		RegistryRead(Key, "NoOfPriceLevels", Text);
		CurrentConnection.NoOfPriceLevels	= StrToInt(Text); */


		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
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
	HRASCONN Handle = RASDial.Dial(RASEntry, RASUserName, RASPassword, ErrorMsg);
	ClientIP = RASDial.ClientIP;
	ServerIP = RASDial.ServerIP;
	return (Handle != 0);
}
//---------------------------------------------------------------------------
HRASCONN TDBDetails::GetRASHandle()
{
	return RASDial.GetConnected(RASEntry);
}
//---------------------------------------------------------------------------
bool TDBDetails::HangUp()
{
	return RASDial.HangUp(RASHandle);
}
//---------------------------------------------------------------------------
bool TDBDetails::GetConnected()
{
	return RASDial.GetConnected(RASEntry);
}
//---------------------------------------------------------------------------
bool TDBDetails::GetEntryDetails(AnsiString &UserName, AnsiString &Password)
{
	return RASDial.GetEntryDetails(RASEntry, UserName, Password);
}
//---------------------------------------------------------------------------
AnsiString TDBDetails::GetDBFilePath()
{
	AnsiString SubSection = DBName.SubString(DBName.Pos(":")+1, DBName.Length() - DBName.Pos(":") + 1);
	if (SubSection.Pos(":") == 0)
	{
		return ExtractFilePath(DBName);
	}
	else
	{
		return ExtractFilePath(SubSection);
	}
}
//---------------------------------------------------------------------------
TRASDial::TRASDial()
{
	hLib = LoadLibrary("rasapi32.dll");
	if (hLib != NULL)
	{
		RasGetEntryDialParams	= (TRasGetEntryDialParams) GetProcAddress(hLib, "RasGetEntryDialParamsA");
		RasSetEntryDialParams	= (TRasSetEntryDialParams) GetProcAddress(hLib, "RasSetEntryDialParamsA");
		RasDial						= (TRasDial) GetProcAddress(hLib, "RasDialA");
		RasGetProjectionInfo		= (TRasGetProjectionInfo) GetProcAddress(hLib, "RasGetProjectionInfoA");
		RasGetErrorString			= (TRasGetErrorString) GetProcAddress(hLib, "RasGetErrorStringA");
		RasHangUp					= (TRasHangUp) GetProcAddress(hLib, "RasHangUpA");
		RasGetConnectStatus		= (TRasGetConnectStatus) GetProcAddress(hLib, "RasGetConnectStatusA");
		RasEnumEntries				= (TRasEnumEntries) GetProcAddress(hLib, "RasEnumEntriesA");
		RasEnumConnections		= (TRasEnumConnections) GetProcAddress(hLib, "RasEnumConnectionsA");
		RasCreatePhonebookEntry	= (TRasCreatePhonebookEntry) GetProcAddress(hLib, "RasCreatePhonebookEntryA");

		FreeLibrary(hLib);
	}
	else
	{
		RasGetEntryDialParams	= NULL;
		RasSetEntryDialParams	= NULL;
		RasDial						= NULL;
		RasGetProjectionInfo		= NULL;
		RasGetErrorString			= NULL;
		RasHangUp					= NULL;
		RasGetConnectStatus		= NULL;
		RasEnumEntries				= NULL;
		RasEnumConnections		= NULL;
		RasCreatePhonebookEntry	= NULL;
	}
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
HRASCONN TRASDial::Dial(AnsiString Entry, AnsiString UserName, AnsiString Password, AnsiString &ErrorMsg)
{
	if (!hLib || !RasGetEntryDialParams || !RasGetProjectionInfo || !RasDial || !RasGetErrorString)
	{
		return NULL;
	}
	RASDIALPARAMS	DialParams;
	BOOL				RetrievePassword;
	HRASCONN			Handle=0;

	memset(&DialParams, sizeof(RASDIALPARAMS), 0);
	DialParams.dwSize = sizeof(RASDIALPARAMS);
	StrPCopy(DialParams.szEntryName, Entry);

	if (RasGetEntryDialParams(NULL, &DialParams, &RetrievePassword) == 0)
	{
		StrPCopy(DialParams.szUserName, UserName);
		StrPCopy(DialParams.szPassword, Password);
//		if (RasSetEntryDialParams(NULL, DialParams, false))
//		{
//		}
		int Result = RasDial(NULL, NULL, &DialParams, 0, NULL, &Handle);
		if (Result == 0)
		{
			RASPPPIP	RASPppIp;
			DWORD		lpcb;
			
			memset(&RASPppIp, sizeof(RASPPPIP), 0);
			RASPppIp.dwSize = sizeof(RASPPPIP);
			lpcb = RASPppIp.dwSize;

			if (RasGetProjectionInfo(Handle, RASP_PppIp, &RASPppIp, &lpcb) == 0)
			{
				ClientIP = RASPppIp.szIpAddress;
				ServerIP = RASPppIp.szServerIpAddress;
			}
			else
			{
				ClientIP = "";
				ServerIP = "";
			}
		}
		else
		{
			char TempMessage[256];
			RasGetErrorString(Result, TempMessage, 256);
			ErrorMsg = TempMessage;
			if (Handle != 0)
			{
				HangUp(Handle);
				Handle = 0;
			}
		}
	}
	return Handle;
}
//---------------------------------------------------------------------------
bool TRASDial::HangUp(HRASCONN Handle)
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
		Application->MessageBox(AnsiString("RasEnumConnections failed - Err:" + IntToStr(RasResult)).c_str(), "Error", MB_OK + MB_ICONERROR);
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
	if (!hLib || !RasCreatePhonebookEntry)
	{
		return false;
	}
	return RasCreatePhonebookEntry(NULL, NULL);
}
//---------------------------------------------------------------------------

