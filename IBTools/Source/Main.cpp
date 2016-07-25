//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <registry.hpp>
#include <shlobj.h>
#include <memory>
#include "Main.h"
#include "MMMessageBox.h"
#include "Version.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchPages"
#pragma resource "*.dfm"
TIBTools *IBTools;
//---------------------------------------------------------------------------
using namespace Database;
//---------------------------------------------------------------------------
__fastcall TIBTools::TIBTools(TComponent* Owner) :
	TForm(Owner),
	FDBControl(TDBSettings()),
	FDBUtilities(FDBControl)
{
	FDBUtilities.OnServiceComplete	= ServiceComplete;
	FDBUtilities.OnServiceStatus	= ServiceStatus;

	char computerName[MAX_COMPUTERNAME_LENGTH + 1] = {0};
	DWORD length = MAX_COMPUTERNAME_LENGTH;
	GetComputerName(computerName, &length);
	ComputerName = computerName;

	int paramCount = ParamCount();
	if (paramCount >= 1)
	{
		AnsiString ServerName = "localhost";
		if (paramCount > 1)
		{
			for (int i=2; i<=paramCount; ++i)
			{
				if (ParamStr(i).SubString(1, 7).UpperCase() == "-SERVER")
				{
					if (ParamStr(i).UpperCase() == "-SERVER")
					{
						if (i < paramCount)
						{
							ServerName = ParamStr(i+1);
						}
					}
					else
					{
						ServerName = ParamStr(i).SubString(8, ParamStr(i).Length() - 7).Trim();
					}
				}
			}
		}
		int FirstColonPos = ParamStr(1).Pos(":");
		if (FirstColonPos == 0 || ExtractFileDrive(ParamStr(1)).Pos("\\\\") != 0)
		{
			throw Exception("File must be on a local drive.");
		}
		int SecondColonPos = ParamStr(1).SubString(FirstColonPos + 1, ParamStr(1).Length() - 1).Pos(":");
		if (SecondColonPos != 0)
		{
			FDBControl.Init(TDBSettings("", ParamStr(1), true));
		}
		else
		{
			FDBControl.Init(TDBSettings(ServerName, ParamStr(1), false));
		}
		Execute();
	}
}
//---------------------------------------------------------------------------
void __fastcall TIBTools::FormClose(TObject *Sender, TCloseAction &Action)
{
	FDBUtilities.CancelService();
}
//---------------------------------------------------------------------------
void __fastcall TIBTools::FormShow(TObject *Sender)
{
    lbeVersion->Caption = "Version " + GetFileVersion();
	int paramCount = ParamCount();
/*	if (paramCount >= 1)
	{
		AnsiString ServerName = "localhost";
		if (paramCount > 1)
		{
			for (int i=2; i<=paramCount; ++i)
			{
				if (ParamStr(i).SubString(1, 7).UpperCase() == "-SERVER")
				{
					if (ParamStr(i).UpperCase() == "-SERVER")
					{
						if (i < paramCount)
						{
							ServerName = ParamStr(i+1);
						}
					}
					else
					{
						ServerName = ParamStr(i).SubString(8, ParamStr(i).Length() - 7).Trim();
					}
				}
			}
		}
		int FirstColonPos = ParamStr(1).AnsiPos(":");
		if (FirstColonPos == 0 || ExtractFileDrive(ParamStr(1)).AnsiPos("\\\\") != 0)
		{
			Message("File must be on a local drive.", msError);
			Application->Terminate();
			return;
		}
		int SecondColonPos = ParamStr(1).SubString(FirstColonPos + 1, ParamStr(1).Length() - 1).AnsiPos(":");
		if (SecondColonPos != 0)
		{
			FDBControl.Init(TDBSettings("", ParamStr(1), true));
		}
		else
		{
			FDBControl.Init(TDBSettings(ServerName, ParamStr(1), false));
		}
		Execute();
	}
	else*/ if (paramCount == 0)
	{
		if (MessageBox("Associate with Firebird files?", "Associate?", MB_ICONQUESTION + MB_YESNO) == ID_YES)
		{
			std::auto_ptr<TRegistry> Reg (new TRegistry);
			Reg->RootKey = HKEY_CLASSES_ROOT;
			if (Reg->OpenKey(".gbk", true))
			{
				Reg->WriteString("", "Interbase.Backup.File");
				Reg->CloseKey();
			}
			if (Reg->OpenKey("Interbase.Backup.File", true))
			{
				Reg->WriteString("", "Interbase Backup File");
				Reg->CloseKey();
			}
			if (Reg->OpenKey("Interbase.Backup.File\\Shell\\Open\\Command", true))
			{
				Reg->WriteString("", "\"" + ParamStr(0)+"\" \"\%1\"");
				Reg->CloseKey();
			}
			if (Reg->OpenKey("Interbase.Backup.File\\DefaultIcon", true))
			{
				Reg->WriteString("", "\"" + ParamStr(0) + "\", 0");
				Reg->CloseKey();
			}

			if (Reg->OpenKey(".fbk", true))
			{
				Reg->WriteString("", "Firebird.Backup.File");
				Reg->CloseKey();
			}
			if (Reg->OpenKey("Firebird.Backup.File", true))
			{
				Reg->WriteString("", "Firebird Backup File");
				Reg->CloseKey();
			}
			if (Reg->OpenKey("Firebird.Backup.File\\Shell\\Open\\Command", true))
			{
				Reg->WriteString("", "\"" + ParamStr(0)+"\" \"\%1\"");
				Reg->CloseKey();
			}
			if (Reg->OpenKey("Firebird.Backup.File\\DefaultIcon", true))
			{
				Reg->WriteString("", "\"" + ParamStr(0) + "\", 0");
				Reg->CloseKey();
			}


			if (Reg->OpenKey(".fdb", true))
			{
				Reg->WriteString("", "Firebird.Database.File");
				Reg->CloseKey();
			}
			if (Reg->OpenKey("Firebird.Database.File", true))
			{
				Reg->WriteString("", "Firebird Database File");
				Reg->CloseKey();
			}
			if (Reg->OpenKey("Firebird.Database.File\\Shell\\Open\\Command", true))
			{
				Reg->WriteString("", "\"" + ParamStr(0)+"\" \"\%1\"");
				Reg->CloseKey();
			}
			if (Reg->OpenKey("Firebird.Database.File\\DefaultIcon", true))
			{
				Reg->WriteString("", "\"" + ParamStr(0) + "\", 0");
				Reg->CloseKey();
			}
			SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
		}
		Application->Terminate();
	}
//	else
//	{
//		Application->Terminate();
//	}
}
//---------------------------------------------------------------------------
void TIBTools::Execute()
{
	AnsiString ext = ExtractFileExt(FDBControl.ConnectionFile()).UpperCase();
	if (ext == ".GBK" || ext == ".FBK")
	{
		RestorePanel->Visible	= true;
		BackupPanel->Visible		= false;
		RepairPanel->Visible		= false;
		RestoreCaption->Caption	= FDBControl.ConnectionString();
	}
	else if (ext == ".FDB")
	{
		BackupPanel->Visible		= true;
		RestorePanel->Visible	= false;
		RepairPanel->Visible		= false;
		BackupCaption->Caption	= FDBControl.ConnectionString();
		RepairCaption->Caption	= FDBControl.ConnectionString();
	}
	else
	{
		Message("Unknown file type: " + ext + "\r\rFile must be .GBK, .FBK or .FDB", msError);
		Application->Terminate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TIBTools::BackupButtonMouseClick(TObject *Sender)
{
	TBackupOptions Opts;

	if (chkIgnoreChecksums->Latched)			Opts << IgnoreChecksums;
	if (chkIgnoreLimbo->Latched)				Opts << IgnoreLimbo;
	if (chkNoGarbageCollection->Latched)	Opts << NoGarbageCollection;
	if (chkNonTransportable->Latched)		Opts << NonTransportable;
	if (chkMetadataOnly->Latched)				Opts << MetadataOnly;

	if (RepairBackupFileName != "")
	{
		AnsiString FileName = IncludeTrailingBackslash(FDBControl.ConnectionPath()) + FDBControl.ConnectionFile();
		if (FDBUtilities.StartBackup(FDBControl.ConnectionServer(), FileName, RepairBackupFileName, true, Opts))
		{
			EnableControls(false);
		}
	}
	else
	{
		if (FDBUtilities.StartBackup(ChangeFileExt(FDBControl.ConnectionFile(), ".FBK"), true, Opts))
		{
			EnableControls(false);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TIBTools::RestoreIBButtonMouseClick(TObject *Sender)
{
	TRestoreOptions Opts = TRestoreOptions() << CreateNewDB;

	if (chkDeactivateIndexes->Latched)		Opts << DeactivateIndexes;
	if (chkNoShadow->Latched)					Opts << NoShadow;
	if (chkNoValidityCheck->Latched)			Opts << NoValidityCheck;
	if (chkOneRelationAtATime->Latched)		Opts << OneRelationAtATime;
	if (chkUseAllSpace->Latched)				Opts << UseAllSpace;

	AnsiString FileName = IncludeTrailingBackslash(FDBControl.ConnectionPath()) + FDBControl.ConnectionFile();
	AnsiString DestFileName = ChangeFileExt(FileName, ".FDB");
	if (FDBUtilities.StartRestore(FDBControl.ConnectionServer(), FileName, DestFileName, true, Opts))
	{
		if (RepairBackupFileName != "")
		{
			RepairRestoreFileName = DestFileName;
		}
		EnableControls(false);
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TIBTools::SweepButtonMouseClick(TObject *Sender)
{
	if (FDBUtilities.StartSweep())
	{
		EnableControls(false);
		BackupCaption->Caption = "Sweeping. This may take a while...";
	}
}
//---------------------------------------------------------------------------
void __fastcall TIBTools::RepairButtonMouseClick(TObject *Sender)
{
	RepairPanel->Visible = true;
	BackupPanel->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TIBTools::CheckButtonMouseClick(TObject *Sender)
{
	RepairMemo->Lines->Clear();
	if (FDBUtilities.StartRepairCheck())
	{
		EnableControls(false);
		RepairCaption->Caption = "Checking database integrity...";
	}
}
//---------------------------------------------------------------------------
void __fastcall TIBTools::DoRepairButtonMouseClick(TObject *Sender)
{
	if (FDBControl.ConnectionServer().UpperCase() == "LOCALHOST" ||
			FDBControl.ConnectionServer().UpperCase() == ComputerName.UpperCase())
	{
		if (!FDBControl.Connected())
		{
			FDBControl.Connect();
		}
		if (FDBUtilities.UserCount() > 1)
		{
			Message("There must be no other connections to the database.", msError);
		}
		else
		{
			RepairMemo->Lines->Clear();

			RepairFileName = IncludeTrailingBackslash(ExtractFilePath(ParamStr(1))) + ExtractFileName(ChangeFileExt(ParamStr(1), ".Copy.IB"));
			RepairBackupFileName = IncludeTrailingBackslash(ExtractFilePath(ParamStr(1))) + ExtractFileName(ChangeFileExt(ParamStr(1), ".Fixed.GBK"));

			CopyFile(ParamStr(1).t_str(), RepairFileName.t_str(), false);

			if (FDBUtilities.StartRepair("localhost", RepairFileName))
			{
				EnableControls(false);
				RepairCaption->Caption = "Repairing database integrity...";
			}
		}
	}
	else
	{
		Message("Repair can only be performed on the server (" + FDBControl.ConnectionServer() + ").", msError);
	}
}
//---------------------------------------------------------------------------
/*bool TIBTools::ZipFile(AnsiString FileName, AnsiString ArchiveName)
{
	try
	{
		ZipBuilder1->AddOptions = AddOpts();
		ZipBuilder1->TempDir = "";
		ZipBuilder1->ZipFileName = ArchiveName;
		ZipBuilder1->FSpecArgs->Clear();
		ZipBuilder1->FSpecArgs->Add(FileName);
		ZipBuilder1->Add();
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
		return false;
	}
	return (ZipBuilder1->SuccessCnt == 1);
} */
//---------------------------------------------------------------------------
void __fastcall TIBTools::ServiceComplete(bool Success, UnicodeString ErrorMsg, TIBServiceType ServiceType)
{
	if (ServiceType == stBackup)
	{
		if (Success)
		{
			if (RepairBackupFileName != "")
			{
				Message("Backup finished. Perform restore and test.", msComplete);
				FDBControl.Init(TDBSettings(FDBControl.ConnectionServer(), RepairBackupFileName, false));
				Execute();
			}
			else
			{
				Message("Backup complete.", msComplete);
			}
		}
		else
		{
			Message("Unable to back up: " + ErrorMsg, msError);
		}
	}
	else if (ServiceType == stRestore)
	{
		if (Success)
		{
			if (RepairRestoreFileName != "")
			{
				Message("Restore finished. Click 'Repair' to test.", msComplete);
				FDBControl.Init(TDBSettings(FDBControl.ConnectionServer(), RepairRestoreFileName, false));
				Execute();
			}
			else
			{
				Message("Restore complete.", msComplete);
			}
		}
		else
		{
			Message("Unable to restore: " + ErrorMsg, msError);
		}
	}
	else if (ServiceType == stSweep)
	{
		FDBControl.Disconnect();
		if (Success)
		{
			Message("Sweep complete", msComplete);
		}
		else
		{
			Message("Unable to sweep: " + ErrorMsg, msError);
		}
	}
	else if (ServiceType == stRepairCheck)
	{
		if (Success)
		{
			if (RepairMemo->Lines->Count == 0)
			{
				Message("Check finished and no errors found.", msComplete);
			}
			else
			{
				Message("Check finished. File needs repairing.", msComplete);
			}
		}
		else
		{
			Message("Unable to check: " + ErrorMsg, msError);
		}
	}
	else if (ServiceType == stRepair)
	{
		if (Success)
		{
			if (RepairMemo->Lines->Count == 0)
			{
				Message("Repair finished and no errors found. Perform backup/restore.", msComplete);
			}
			else
			{
				Message("Repair finished. Perform backup/restore.", msComplete);
			}
			FDBControl.Init(TDBSettings(FDBControl.ConnectionServer(), RepairFileName, false));
			Execute();
		}
		else
		{
			Message("Unable to repair: " + ErrorMsg, msError);
		}
	}
	EnableControls(true);
	BackupCaption->Caption	= FDBControl.ConnectionString();
	RestoreCaption->Caption	= FDBControl.ConnectionString();
	RepairCaption->Caption	= FDBControl.ConnectionString();
}
//---------------------------------------------------------------------------
void __fastcall TIBTools::ServiceStatus(UnicodeString Msg, TIBServiceType ServiceType)
{
	if (ServiceType == stBackup || ServiceType == stSweep)
	{
		BackupCaption->Caption = Msg;
	}
	else if (ServiceType == stRestore)
	{
		RestoreCaption->Caption = Msg;
	}
	else if (ServiceType == stRepairCheck || ServiceType == stRepair)
	{
		RepairMemo->Lines->Add(Msg);
	}
}
//---------------------------------------------------------------------------
void TIBTools::EnableControls(bool Enable)
{
	BackupSettingsPanel->Enabled	= Enable;
	BackupButton->Enabled			= Enable;
	SweepButton->Enabled			= Enable;
	RepairButton->Enabled			= Enable;
	RestoreIBButton->Enabled		= Enable;
	RestoreSettingsPanel->Enabled	= Enable;
	CheckButton->Enabled			= Enable;
}
//---------------------------------------------------------------------------
void __fastcall TIBTools::CancelButtonMouseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void TIBTools::Message(const UnicodeString &Msg, TMessageType MessageType)
{
	switch (MessageType)
	{
		case msError:		MessageBox(Msg, "Error", MB_ICONERROR + MB_OK);				break;
		case msComplete:	MessageBox(Msg, "Complete", MB_ICONINFORMATION + MB_OK);	break;
	}
}
//---------------------------------------------------------------------------

