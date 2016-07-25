//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <FileCtrl.hpp>
#include "ChefMateData.h"
#include "Registration.h"
#include "Consts.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdmChefMateData *dmChefMateData;
//---------------------------------------------------------------------------
__fastcall TdmChefMateData::TdmChefMateData(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
bool TdmChefMateData::Connect(TLabel *Label)
{
	dbChefMate->Connected = false;
	if (CurrentConnection.ChefMateDB.RemoteConnection)
	{
		if (!CurrentConnection.ChefMateDB.Connected)
		{
			if (Application->MessageBox(AnsiString("Do you wish to dial " + CurrentConnection.ChefMateDB.RASEntry + " to connect to ChefMate now?").c_str(),
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
						if (!CurrentConnection.ChefMateDB.Dial(ErrorMessage))
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
				if (!CurrentConnection.ChefMateDB.Connected)
				{
					return false;
				}
			}
			else return false;
		}
	}
	if (CurrentConnection.ChefMateDB.DBName == "")
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
			dmChefMateData->dbChefMate->DatabaseName	= CurrentConnection.ChefMateDB.DBName;
			dmChefMateData->dbChefMate->Connected		= true;
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
void TdmChefMateData::CloseDB()
{
	try
	{
		dmChefMateData->dbChefMate->Connected = false;
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
		dmChefMateData->dbChefMate->ForceClose();
	}
}
//---------------------------------------------------------------------------
void TdmChefMateData::Disconnect()
{
	if (CurrentConnection.ChefMateDB.RemoteConnection)
	{
		if (CurrentConnection.ChefMateDB.Connected)
		{
			if (Application->MessageBox(("You are connected to " + CurrentConnection.ChefMateDB.RASEntry +
												  ". Do you wish to disconnect?").c_str(),
												  "Question",
												  MB_ICONQUESTION + MB_YESNO) == ID_YES)
			{
				Screen->Cursor = crHourGlass;
				try
				{
					CurrentConnection.ChefMateDB.HangUp();
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
void TdmChefMateData::Sweep(AnsiString DatabaseName)
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
void __fastcall TdmChefMateData::IBSQLMonitor1SQL(AnsiString EventText,
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

