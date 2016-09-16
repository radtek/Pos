//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop
#include <FileCtrl.hpp>
#include "MMData.h"
#include "Registration.h"
#include "MMRegistry.h"
#include "Consts.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdmMMData *dmMMData;
//---------------------------------------------------------------------------
__fastcall TdmMMData::TdmMMData(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
bool TdmMMData::Connect(TLabel *Label)
{
    //UnicodeString s = "";
	dbMenuMate->Connected = false;
	if (CurrentConnection.MenuMateDB.RemoteConnection)
	{
		if (!CurrentConnection.MenuMateDB.Connected)
		{
			if (Application->MessageBox(AnsiString("Do you wish to dial " + CurrentConnection.MenuMateDB.RASEntry + " to connect to MenuMate now?").c_str(),
												 "Dial?", MB_ICONQUESTION + MB_YESNO) == ID_YES)
			{
				Screen->Cursor = crHourGlass;
				try
				{
					bool Retry = true;
					while (Retry)
					{
						Retry = false;
						if (Label)
						{
							Label->Caption = "Dialing...";
							Label->Update();
						}
						AnsiString ErrorMessage;
						if (!CurrentConnection.MenuMateDB.Dial(ErrorMessage))
						{
							Retry = (Application->MessageBox((ErrorMessage + "\rDo you wish to try again?").c_str(),
															  "Dial Error", MB_ICONERROR + MB_YESNO) == ID_YES);
						}
					}
				}
				__finally
				{
					Screen->Cursor = crDefault;
				}
				if (!CurrentConnection.MenuMateDB.Connected)
				{
					return false;
				}
			}
			else return false;
		}
	}
	if (CurrentConnection.MenuMateDB.DBName == "")
	{
		return false;
	}
	else
	{
		try
		{
			if (Label)
			{
				Label->Caption = "Opening database...";
				Label->Update();
			}
			dmMMData->dbMenuMate->DatabaseName	= CurrentConnection.MenuMateDB.DBName;
			dmMMData->dbMenuMate->Connected		= true;
			return true;
		}
		catch (Exception &E)
		{
			Application->MessageBox(("Could not connect to Database.\r" + E.Message).c_str(), "Error", MB_OK + MB_ICONERROR);
			return false;
		}
	}
}
//---------------------------------------------------------------------------
void TdmMMData::CloseDB()
{
	try
	{
		dmMMData->dbMenuMate->Connected = false;
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
		dmMMData->dbMenuMate->ForceClose();
	}
}
//---------------------------------------------------------------------------
void TdmMMData::Disconnect()
{
	if (CurrentConnection.MenuMateDB.RemoteConnection)
	{
		if (CurrentConnection.MenuMateDB.Connected)
		{
			if (Application->MessageBox(("You are connected to " + CurrentConnection.MenuMateDB.RASEntry +
												  ". Do you wish to disconnect?").c_str(),
												  "Question",
												  MB_ICONQUESTION + MB_YESNO) == ID_YES)
			{
				Screen->Cursor = crHourGlass;
				try
				{
					CurrentConnection.MenuMateDB.HangUp();
				}
				__finally
				{
					Screen->Cursor = crDefault;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void TdmMMData::Sweep(AnsiString DatabaseName)
{
	AnsiString ServerName	= DatabaseName.SubString(1, DatabaseName.Pos(":"));
	AnsiString FileName		= DatabaseName.SubString(DatabaseName.Pos(":")+1, DatabaseName.Length() - DatabaseName.Pos(":") + 1);

	ValidationService->ServerName		= ServerName;
	ValidationService->DatabaseName	= FileName;
	ValidationService->Active			= true;
	try
	{
		Screen->Cursor = crHourGlass;
		ValidationService->Options		= TValidateOptions() << SweepDB;
		ValidationService->ServiceStart();
		while (ValidationService->IsServiceRunning)
		{
			Application->ProcessMessages();
			Screen->Cursor = crHourGlass;
		}
	}
	__finally
	{
		Screen->Cursor = crDefault;
		ValidationService->Active		= false;
	}
	Application->MessageBox("Sweep Complete", "Complete", MB_OK + MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall TdmMMData::IBSQLMonitor1SQL(AnsiString EventText,
      TDateTime EventTime)
{
	TFileStream *FileStream;
	if (FileExists("\\SQL.txt"))
	{
		FileStream = new TFileStream("\\SQL.txt", fmOpenWrite);
	}
	else
	{
		FileStream = new TFileStream("\\SQL.txt", fmCreate);
	}
	FileStream->Seek(0, soFromEnd);
	FileStream->WriteBuffer(EventText.c_str(), EventText.Length());
	delete FileStream;
}
//---------------------------------------------------------------------------
AnsiString TdmMMData::GetTerminalName()
{
	AnsiString TerminalName = "";
	RegistryRead(OfficeKey, "TerminalName", TerminalName);
	if (TerminalName == "")
	{
		DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
		char szBuf[MAX_COMPUTERNAME_LENGTH + 1];
		szBuf[0] = '\0';

		if (!GetComputerName(szBuf, &dwSize))
		{
			return "";
		}
		TerminalName = szBuf;
//		TerminalName += " Office";
	}
	RegistryWrite(OfficeKey, "TerminalName", TerminalName);
	return TerminalName;
}
//---------------------------------------------------------------------------
void TdmMMData::Registered(bool *pRegistered, unsigned int *pRegisteredModules, AnsiString *pRegisteredName)
{
   const AnsiString PRODUCT_NAME = "Office";
	*pRegistered = false;
	if (pRegisteredName) *pRegisteredName = "";
	try
	{
		if (dmMMData->dbMenuMate->Connected)
		{
			AnsiString TerminalName = GetTerminalName();
			if (TerminalName != "")
			{
				TMachineInfo Info;
				Info.ProductName	= PRODUCT_NAME;
				Info.TerminalName	= TerminalName;

				TRegInfo RegInfo(dmMMData->dbMenuMate);
				if (!RegInfo.Registered(&Info))
				{
					Info.ProductName	= PRODUCT_NAME;
					Info.TerminalName	= TerminalName;
					RegInfo.AddRegistrationInformation(&Info);
					if (pRegisteredName) *pRegisteredName = "";
					*pRegistered = false;
				}
				else
				{
					if (pRegisteredName)		*pRegisteredName		= Info.CompanyName;
					if (pRegisteredModules)	*pRegisteredModules	= Info.MenuMateModules;

					*pRegistered = true;
				}
			}
		}
	}
	catch (Exception &E)
	{
		*pRegistered = false;
		if (pRegisteredName) *pRegisteredName = "";
		Application->ShowException(&E);
	}
}
//---------------------------------------------------------------------------

