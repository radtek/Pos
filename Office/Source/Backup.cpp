//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Backup.h"
#include "MMRegistry.h"
#include "Consts.h"
#include "StockData.h"
#include "MMData.h"
#include "Connections.h"
#include "ZipUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ZipBuilder"
#pragma link "ZipBase"
#pragma resource "*.dfm"

TfrmBackup *frmBackup;
//---------------------------------------------------------------------------
__fastcall TfrmBackup::TfrmBackup(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::FormShow(TObject *Sender)
{
	StockBackupKey			= OfficeKey + "\\" + CurrentConnection.CompanyName + "\\Backup\\Stock";
	MMBackupKey				= OfficeKey + "\\" + CurrentConnection.CompanyName + "\\Backup\\MenuMate";
	BackupKey				= OfficeKey + "\\" + CurrentConnection.CompanyName + "\\Backup";
	SystemBackupFilesKey	= OfficeKey + "\\" + CurrentConnection.CompanyName + "\\Backup\\System\\Files";
	SystemBackupKey		= OfficeKey + "\\" + CurrentConnection.CompanyName + "\\Backup\\System";

	PageControl1->ActivePage = tsDataBackup;
	lbeProgress->Caption = "";

	AnsiString BackupType = "Backup";
	RegistryRead(BackupKey, "BackupType", BackupType);
	if (BackupType == "Backup")
	{
		radBackup->Checked = true;
	}
	else
	{
		radPickup->Checked = true;
	}
	radPickupClick(NULL);

	StockBackupPath = "";
	RegistryRead(StockBackupKey, "ServerBackupPath", StockBackupPath);
	if (StockBackupPath == "")
	{
		StockBackupPath = ExcludeTrailingBackslash(CurrentConnection.StockDB.DBFilePath);
		RegistryWrite(StockBackupKey, "ServerBackupPath", StockBackupPath);
	}
	Edit1->Text = StockBackupPath;

	MMBackupPath = "";
	RegistryRead(MMBackupKey, "ServerBackupPath", MMBackupPath);
	if (MMBackupPath == "")
	{
		CurrentConnection.MenuMateDB.DBFilePath;
		MMBackupPath = ExcludeTrailingBackslash(CurrentConnection.MenuMateDB.DBFilePath);
		RegistryWrite(MMBackupKey, "ServerBackupPath", MMBackupPath);
	}
	Edit2->Text = MMBackupPath;

	StockZipSourcePath = "";
	RegistryRead(StockBackupKey, "ZipSourcePath", StockZipSourcePath);
	if (StockZipSourcePath == "")
	{
		if (dmStockData->dbStock->Connected)
		{
			StockZipSourcePath = "\\\\" + dmStockData->dbinfStock->DBSiteName + "\\";
			RegistryWrite(StockBackupKey, "ZipSourcePath", StockZipSourcePath);
		}
	}
	edDataBackupPath->Text = StockZipSourcePath;

	MMZipSourcePath = "";
	RegistryRead(MMBackupKey, "ZipSourcePath", MMZipSourcePath);
	if (MMZipSourcePath == "")
	{
		if (dmMMData->dbMenuMate->Connected)
		{
			MMZipSourcePath = "\\\\" + dmMMData->dbinfMM->DBSiteName + "\\";
			RegistryWrite(MMBackupKey, "ZipSourcePath", MMZipSourcePath);
		}
	}
	Edit3->Text = MMZipSourcePath;

	PickupSourcePath = "";
	RegistryRead(BackupKey, "PickupSourcePath", PickupSourcePath);
	edPickupPath->Text = PickupSourcePath;

	ArchiveDestPath = "";
	RegistryRead(BackupKey, "ArchiveDestPath", ArchiveDestPath);
	edDestPath->Text = ArchiveDestPath;

	SystemDestPath = "";
	RegistryRead(BackupKey, "SystemDestPath", SystemDestPath);
	edSysDestPath->Text = SystemDestPath;

	ListBox1->Items->Clear();
	bool NotFound = false;
	int i=0;
	while (!NotFound)
	{
		AnsiString Path = "";
		if (RegistryRead(SystemBackupFilesKey, "File" + IntToStr(i+1), Path))
		{
			i++;
			ListBox1->Items->Add(Path);
		}
		else
		{
			NotFound = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::btnOkClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::btnDataPathClick(TObject *Sender)
{
	AnsiString Dir = "";
	if (SelectDirectory("Please enter the stock backup file path", "", Dir))
	{
		edDataBackupPath->Text = Dir;
	}
/*	dfsBrowseDirectoryDlg1->Options = TBrowseFlags() << bfDirectoriesOnly;
	if (dfsBrowseDirectoryDlg1->Execute())
	{
		edDataBackupPath->Text = dfsBrowseDirectoryDlg1->Selection;
	} */
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::BitBtn2Click(TObject *Sender)
{
	AnsiString Dir = "";
	if (SelectDirectory("Please enter the MenuMate backup file path", "", Dir))
	{
		Edit3->Text = Dir;
	}
/*	dfsBrowseDirectoryDlg1->Options = TBrowseFlags() << bfDirectoriesOnly;
	if (dfsBrowseDirectoryDlg1->Execute())
	{
		Edit3->Text = dfsBrowseDirectoryDlg1->Selection;
	} */
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::PageControl1Changing(TObject *Sender,
		bool &AllowChange)
{
	if (PageControl1->ActivePage == tsSetup)
	{
		if (Edit1->Text != "")
		{
			StockBackupPath = ExcludeTrailingBackslash(Edit1->Text);
			RegistryWrite(StockBackupKey, "ServerBackupPath", StockBackupPath);
			Edit1->Text = StockBackupPath;
		}
		else
		{
			RegistryRead(StockBackupKey, "ServerBackupPath", StockBackupPath);
			Edit1->Text = ExcludeTrailingBackslash(StockBackupPath);
		}
		if (Edit2->Text != "")
		{
			MMBackupPath = ExcludeTrailingBackslash(Edit2->Text);
			RegistryWrite(MMBackupKey, "ServerBackupPath", MMBackupPath);
			Edit2->Text = MMBackupPath;
		}
		else
		{
			RegistryRead(MMBackupKey, "ServerBackupPath", MMBackupPath);
			Edit2->Text = ExcludeTrailingBackslash(MMBackupPath);
		}
		if (edDataBackupPath->Text != "")
		{
			StockZipSourcePath = ExcludeTrailingBackslash(edDataBackupPath->Text);

			if (!DirectoryExists(StockZipSourcePath))
			{
				if (Application->MessageBox("The stock backup file path cannot be found. Are you sure this is correct?", "Warning", MB_ICONWARNING + MB_YESNO) == ID_NO)
				{
					AllowChange = false;
					edDataBackupPath->SetFocus();

					RegistryWrite(StockBackupKey, "ZipSourcePath", StockZipSourcePath);
					edDataBackupPath->Text = StockZipSourcePath;
				}
			}
			else
			{
				RegistryWrite(StockBackupKey, "ZipSourcePath", StockZipSourcePath);
				edDataBackupPath->Text = StockZipSourcePath;
			}
		}
		else
		{
			RegistryRead(StockBackupKey, "ZipSourcePath", StockZipSourcePath);
			edDataBackupPath->Text = ExcludeTrailingBackslash(StockZipSourcePath);
		}
		if (Edit3->Text != "")
		{
			MMZipSourcePath = ExcludeTrailingBackslash(Edit3->Text);

			if (!DirectoryExists(MMZipSourcePath))
			{
				if (Application->MessageBox("The MenuMate backup file path cannot be found. Are you sure this is correct?", "Warning", MB_ICONWARNING + MB_YESNO) == ID_NO)
				{
					AllowChange = false;
					Edit3->SetFocus();

					RegistryWrite(MMBackupKey, "ZipSourcePath", MMZipSourcePath);
					Edit3->Text = ExcludeTrailingBackslash(MMZipSourcePath);
				}
			}
			else
			{
				RegistryWrite(MMBackupKey, "ZipSourcePath", MMZipSourcePath);
				Edit3->Text = ExcludeTrailingBackslash(MMZipSourcePath);
			}
		}
		else
		{
			RegistryRead(MMBackupKey, "ZipSourcePath", MMZipSourcePath);
			Edit3->Text = ExcludeTrailingBackslash(MMZipSourcePath);
		}
	}
	else if (PageControl1->ActivePage == tsDataBackup)
	{
		if (radBackup->Checked)
		{
			RegistryWrite(BackupKey, "BackupType", "Backup");
		}
		else
		{
			RegistryWrite(BackupKey, "BackupType", "Pickup");
		}

		PickupSourcePath = edPickupPath->Text;
		if (PickupSourcePath.Length() > 0)
		{
			PickupSourcePath = ExcludeTrailingBackslash(edPickupPath->Text);
			RegistryWrite(BackupKey, "PickupSourcePath", PickupSourcePath);
			edPickupPath->Text = PickupSourcePath;
		}

		ArchiveDestPath = edDestPath->Text;
		if (ArchiveDestPath.Length() > 0)
		{
			ArchiveDestPath = ExcludeTrailingBackslash(edDestPath->Text);
			RegistryWrite(BackupKey, "ArchiveDestPath", ArchiveDestPath);
			edDestPath->Text = ArchiveDestPath;
		}
	}
	else if (PageControl1->ActivePage == tsSystemBackup)
	{
		SystemDestPath = ExcludeTrailingBackslash(edSysDestPath->Text);
		RegistryWrite(BackupKey, "SystemDestPath", SystemDestPath);
		edSysDestPath->Text = SystemDestPath;
		bool NotFound = false;
		int i=0;
		while (!NotFound)
		{
			if (!RegistryDelete(SystemBackupFilesKey, "File" + IntToStr(i+1)))
			{
				NotFound = true;
			}
			i++;
		}
		for (int i=0; i<ListBox1->Items->Count; i++)
		{
			RegistryWrite(SystemBackupFilesKey, "File" + IntToStr(i+1), ListBox1->Items->Strings[i]);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::btnSelectDestPathClick(TObject *Sender)
{
	AnsiString Dir = "";
	if (SelectDirectory("Please enter the path to backup to", "", Dir))
	{
		edDestPath->Text = Dir;
	}
/*	dfsBrowseDirectoryDlg1->Options = TBrowseFlags() << bfDirectoriesOnly;
	if (dfsBrowseDirectoryDlg1->Execute())
	{
		edDestPath->Text = dfsBrowseDirectoryDlg1->Selection;
	}  */
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::btnSelectSysDestPathClick(TObject *Sender)
{
	AnsiString Dir = "";
	if (SelectDirectory("Please enter the path to backup to", "", Dir))
	{
		edSysDestPath->Text = Dir;
	}
/*	dfsBrowseDirectoryDlg1->Options = TBrowseFlags() << bfDirectoriesOnly;
	if (dfsBrowseDirectoryDlg1->Execute())
	{
		edSysDestPath->Text = dfsBrowseDirectoryDlg1->Selection;
	} */
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::btnFullDataBackupClick(TObject *Sender)
{
	if (edDestPath->Text == "")
	{
		Application->MessageBox("Please select a directory to back up the files to.",
										"Error", MB_OK + MB_ICONERROR);
		edDestPath->SetFocus();
		return;
	}
	else
	{
		ArchiveDestPath = ExcludeTrailingBackslash(edDestPath->Text);
		RegistryWrite(BackupKey, "ArchiveDestPath", ArchiveDestPath);
		edDestPath->Text = ArchiveDestPath;

		try
		{
			DisableWhileBackingUp(true);
			btnStockDataBackupClick(NULL);
			DisableWhileBackingUp(true);
			btnMMDataBackupClick(NULL);
			lbeProgress->Caption = "All Done.";
			Application->MessageBox("Data backup complete.", "Complete", MB_OK + MB_ICONINFORMATION);
		}
		__finally
		{
			DisableWhileBackingUp(false);
		}
	}
}
//---------------------------------------------------------------------------
void TfrmBackup::DisableWhileBackingUp(bool Disable)
{
	radPickupClick(NULL);

	radBackup->Enabled					= !Disable;
	if (radBackup->Checked)
	{
		btnFullDataBackup->Enabled		= !Disable;
		btnMMDataBackup->Enabled		= !Disable;
		btnStockDataBackup->Enabled	= !Disable;
	}
	else
	{
		btnFullDataBackup->Enabled		= false;
		btnMMDataBackup->Enabled		= false;
		btnStockDataBackup->Enabled	= false;
	}

	radPickup->Enabled					= !Disable;
	if (radPickup->Checked)
	{
		btnPickupCopy->Enabled			= !Disable;
		edPickupPath->Enabled			= !Disable;
		btnSelectSourcePath->Enabled	= !Disable;
	}
	else
	{
		btnPickupCopy->Enabled			= false;
		edPickupPath->Enabled			= false;
		btnSelectSourcePath->Enabled	= false;
	}

	edDestPath->Enabled					= !Disable;
	btnSelectDestPath->Enabled			= !Disable;
	btnOk->Enabled							= !Disable;
	PageControl1->Enabled				= !Disable;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::btnStockDataBackupClick(TObject *Sender)
{
	if (dmMMData->dbMenuMate->Connected)
	{
		if (Sender != NULL)
		{
			if (edDestPath->Text == "")
			{
				Application->MessageBox("Please select a directory to back up the files to.",
												"Error", MB_OK + MB_ICONERROR);
				edDestPath->SetFocus();
				return;
			}
			else
			{
				ArchiveDestPath = ExcludeTrailingBackslash(edDestPath->Text);
				RegistryWrite(BackupKey, "ArchiveDestPath", ArchiveDestPath);
				edDestPath->Text = ArchiveDestPath;
//				DisableWhileBackingUp(true);
			}
		}
		try
		{
			AnsiString BackupFileName = ChangeFileExt(ExtractFileName(dmStockData->dbinfStock->DBFileName), ".gbk");
			if (!DirectoryExists(StockZipSourcePath))
			{
				Application->MessageBox("Please select the path to the backup file from the 'Setup' tab first.",
												"Error", MB_OK + MB_ICONERROR);
				PageControl1->ActivePage = tsSetup;
//				DisableWhileBackingUp(false);
				edDataBackupPath->SetFocus();
				return;
			}
			while (!DirectoryExists(ArchiveDestPath))
			{
				if (Application->MessageBox("Make sure there is a disk in the drive you want to save to.",
													 "Retry", MB_RETRYCANCEL + MB_ICONERROR) == ID_CANCEL)
				{
//					DisableWhileBackingUp(false);
					return;
				}
			}
			try
			{
				DisableWhileBackingUp(true);
				dmStockData->IBBackupService1->Active = false;
				dmStockData->IBBackupService1->ServerName = dmStockData->dbinfStock->DBSiteName; // Needs to resolve to IP
				dmStockData->IBBackupService1->Protocol = TCP;
				dmStockData->IBBackupService1->Attach();

				try
				{
					dmStockData->IBBackupService1->Options.Clear();
					dmStockData->IBBackupService1->Options << NoGarbageCollection << IgnoreLimbo;
					dmStockData->IBBackupService1->Verbose = true;
					dmStockData->IBBackupService1->DatabaseName = dmStockData->dbinfStock->DBFileName;
					dmStockData->IBBackupService1->BackupFile->Clear();
					dmStockData->IBBackupService1->BackupFile->Add(IncludeTrailingBackslash(StockBackupPath) + BackupFileName);

					dmStockData->IBBackupService1->ServiceStart();
					while (dmStockData->IBBackupService1->IsServiceRunning)
					{
						while (!dmStockData->IBBackupService1->Eof)
						{
							lbeProgress->Caption = dmStockData->IBBackupService1->GetNextLine();
							Sleep(0);
							Application->ProcessMessages();
						}
						Sleep(0);
						Application->ProcessMessages();
					}
				}
				__finally
				{
					if (dmStockData->IBBackupService1->Active)
					{
						dmStockData->IBBackupService1->Detach();
					}
				}
			}
			catch (Exception &E)
			{
				if (Sender != NULL)
				{
					DisableWhileBackingUp(false);
				}
				throw;
			}
			lbeProgress->Caption = "Archiving files...";
			AnsiString ZipFileName = "";
			RegistryRead(StockBackupKey, "ZipFileName", ZipFileName);
			if (ZipFileName == "")
			{
				ZipFileName = "StockData.zip";
				RegistryWrite(StockBackupKey, "ZipFileName", ZipFileName);
			}
			if (!ArchiveFile(IncludeTrailingBackslash(StockZipSourcePath) + BackupFileName, IncludeTrailingBackslash(ArchiveDestPath) + ZipFileName))
			{
				Application->MessageBox("Could not save backup file. Please check that\r"
												"the backup path is correct and that there is\r"
												"sufficient space.",
												"Error", MB_OK + MB_ICONERROR);
				DisableWhileBackingUp(false);
				edDestPath->SetFocus();
			}
			else
			{
				if (Sender != NULL)
				{
					lbeProgress->Caption = "All Done.";
					Application->MessageBox("Stock data backup complete.", "Complete", MB_OK + MB_ICONINFORMATION);
					DisableWhileBackingUp(false);
				}
			}
		}
		__finally
		{
			DisableWhileBackingUp(false);
		}
	}
	else // No DB connection
	{
		Application->MessageBox("Office was unable to attach to the Stock database.\rRestart Office to try again.", "Error", MB_ICONERROR + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::btnMMDataBackupClick(TObject *Sender)
{
	if (dmMMData->dbMenuMate->Connected)
	{
		if (Sender != NULL)
		{
			if (edDestPath->Text == "")
			{
				Application->MessageBox("Please select a directory to back up the files to.",
												"Error", MB_OK + MB_ICONERROR);
				edDestPath->SetFocus();
				return;
			}
			else
			{
				ArchiveDestPath = ExcludeTrailingBackslash(edDestPath->Text);
				RegistryWrite(BackupKey, "ArchiveDestPath", ArchiveDestPath);
				edDestPath->Text = ArchiveDestPath;
//				DisableWhileBackingUp(true);
			}
		}
		try
		{
			AnsiString BackupFileName = ChangeFileExt(ExtractFileName(dmMMData->dbinfMM->DBFileName), ".gbk");
			if (!DirectoryExists(MMZipSourcePath))
			{
				Application->MessageBox("Please select the path to the backup file from the 'Setup' tab first.",
												"Error", MB_OK + MB_ICONERROR);
				PageControl1->ActivePage = tsSetup;
//				DisableWhileBackingUp(false);
				Edit3->SetFocus();
				return;
			}
			while (!DirectoryExists(ArchiveDestPath))
			{
				if (Application->MessageBox("Make sure there is a disk in the drive you want to save to.",
													 "Retry", MB_RETRYCANCEL + MB_ICONERROR) == ID_CANCEL)
				{
//					DisableWhileBackingUp(false);
					return;
				}
			}
			try
			{
				DisableWhileBackingUp(true);
				dmStockData->IBBackupService1->Active = false;
				dmStockData->IBBackupService1->ServerName = dmMMData->dbinfMM->DBSiteName; // Needs to resolve to IP
				dmStockData->IBBackupService1->Protocol = TCP;
				dmStockData->IBBackupService1->Attach();
//			if (dmStockData->IBBackupService1->Active)
				try
				{
					dmStockData->IBBackupService1->Options.Clear();
					dmStockData->IBBackupService1->Options << NoGarbageCollection << IgnoreLimbo;
					dmStockData->IBBackupService1->Verbose = true;
					dmStockData->IBBackupService1->DatabaseName = dmMMData->dbinfMM->DBFileName;
					dmStockData->IBBackupService1->BackupFile->Clear();
					dmStockData->IBBackupService1->BackupFile->Add(IncludeTrailingBackslash(MMBackupPath) + BackupFileName);

					dmStockData->IBBackupService1->ServiceStart();
					while (dmStockData->IBBackupService1->IsServiceRunning)
					{
						while (!dmStockData->IBBackupService1->Eof)
						{
							lbeProgress->Caption = dmStockData->IBBackupService1->GetNextLine();
							Sleep(0);
							Application->ProcessMessages();
						}
						Sleep(0);
						Application->ProcessMessages();
					}
				}
				__finally
				{
					if (dmStockData->IBBackupService1->Active)
					{
						dmStockData->IBBackupService1->Detach();
					}
				}
			}
			catch (Exception &E)
			{
				if (Sender != NULL)
				{
					DisableWhileBackingUp(false);
				}
				throw;
			}
			lbeProgress->Caption = "Archiving files...";
			AnsiString ZipFileName = "";
			RegistryRead(MMBackupKey, "ZipFileName", ZipFileName);
			if (ZipFileName == "")
			{
				ZipFileName = "MMData.zip";
				RegistryWrite(MMBackupKey, "ZipFileName", ZipFileName);
			}
			if (!ArchiveFile(IncludeTrailingBackslash(MMZipSourcePath) + BackupFileName, IncludeTrailingBackslash(ArchiveDestPath) + ZipFileName))
			{
				Application->MessageBox("Could not save backup file. Please check that\r"
												"the backup path is correct and that there is\r"
												"sufficient space.",
												"Error", MB_OK + MB_ICONERROR);
				DisableWhileBackingUp(false);
				edDestPath->SetFocus();
			}
			else
			{
				if (Sender != NULL)
				{
					lbeProgress->Caption = "All Done.";
					DisableWhileBackingUp(false);
					Application->MessageBox("MenuMate data backup complete.", "Complete", MB_OK + MB_ICONINFORMATION);
				}
			}
		}
		__finally
		{
			DisableWhileBackingUp(false);
		}
	}
	else // No DB connection
	{
		Application->MessageBox("Office was unable to attach to the MenuMate database.\rRestart Office to try again.", "Error", MB_ICONERROR + MB_OK);
	}
}
//---------------------------------------------------------------------------
bool TfrmBackup::ArchiveFile(AnsiString FileName, AnsiString ArchiveName)
{
	try
	{
		AnsiString TempDir;
		UINT BufferSize				= GetTempPath(0,NULL);
		TempDir.SetLength(BufferSize+1);
		GetTempPath(BufferSize+1, TempDir.c_str());

		ZipBuilder1->DLLDirectory	= ExtractFilePath(Application->ExeName);
		ZipBuilder1->AddOptions		= AddOpts();
		ZipBuilder1->TempDir			= TempDir;//"";
		EraseFile(ArchiveName, htdAllowUndo);
		ZipBuilder1->ZipFileName	= ArchiveName;
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
}
//---------------------------------------------------------------------------
bool TfrmBackup::ArchiveFiles(TStrings *FileNames, AnsiString ArchiveName)
{
	try
	{
		ZipBuilder1->DLLDirectory = ExtractFilePath(Application->ExeName);
		ZipBuilder1->AddOptions = AddOpts() << AddDirNames;
		ZipBuilder1->TempDir = "";
		ZipBuilder1->ZipFileName = ArchiveName;
		ZipBuilder1->FSpecArgs->Clear();
		ZipBuilder1->FSpecArgs->Assign(FileNames);
		ZipBuilder1->Add();
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
		return false;
	}
	return (ZipBuilder1->SuccessCnt == FileNames->Count);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::btnAddSysFileClick(TObject *Sender)
{
	if (OpenDialog1->Execute())
	{
		for (int j=0; j<ListBox1->Items->Count; j++)
		{
			for (int i=0; i<OpenDialog1->Files->Count;)
			{
				if (OpenDialog1->Files->Strings[i].Trim().LowerCase() == ListBox1->Items->Strings[j].Trim().LowerCase())
				{
					OpenDialog1->Files->Delete(i);
				}
				else i++;
			}
		}
		for (int i=0; i<OpenDialog1->Files->Count; i++)
		{
			ListBox1->Items->Add(OpenDialog1->Files->Strings[i].Trim());
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::btnRemoveSysFileClick(TObject *Sender)
{
	for (int i=0; i<ListBox1->Items->Count;)
	{
		if (ListBox1->Selected[i])
		{
			ListBox1->Items->Delete(i);
		}
		else
		{
			i++;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::ZipBuilder1Message(TObject *Sender,
      int ErrCode, AnsiString Message)
{
//	Application->MessageBox(Message.c_str(), AnsiString("Error no. " + ErrCode).c_str(), MB_OK + MB_ICONERROR);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	PageControl1Changing(Sender, CanClose);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::btnSystemBackupClick(TObject *Sender)
{
	if (edSysDestPath->Text == "")
	{
		Application->MessageBox("Please select a directory to back up the files to.",
										"Error", MB_OK + MB_ICONERROR);
		edSysDestPath->SetFocus();
		return;
	}
	else
	{
		try
		{
			btnSystemBackup->Enabled		= false;
			btnAddSysFile->Enabled			= false;
			btnRemoveSysFile->Enabled		= false;
			btnOk->Enabled						= false;
			edSysDestPath->Enabled			= false;
			btnSelectSysDestPath->Enabled	= false;
			PageControl1->Enabled			= false;

			SystemDestPath = ExcludeTrailingBackslash(edSysDestPath->Text);
			AnsiString ZipFileName = "";
			RegistryRead(SystemBackupKey, "ZipFileName", ZipFileName);
			if (ZipFileName == "")
			{
				ZipFileName = "SystemBackup.zip";
				RegistryWrite(SystemBackupKey, "ZipFileName", ZipFileName);
			}
			if (!ArchiveFiles(ListBox1->Items, IncludeTrailingBackslash(SystemDestPath) + ZipFileName))
			{
				Application->MessageBox("Could not save backup files. Please check that\r"
												"the backup path is correct and that there is\r"
												"sufficient space.",
												"Error", MB_OK + MB_ICONERROR);
				btnSystemBackup->Enabled		= true;
				btnAddSysFile->Enabled			= true;
				btnRemoveSysFile->Enabled		= true;
				btnOk->Enabled						= true;
				edSysDestPath->Enabled			= true;
				btnSelectSysDestPath->Enabled	= true;
				PageControl1->Enabled			= true;
				edSysDestPath->SetFocus();
			}
			else
			{
				Application->MessageBox("System backup complete.", "Complete", MB_OK + MB_ICONINFORMATION);
			}
		}
		__finally
		{
			btnSystemBackup->Enabled		= true;
			btnAddSysFile->Enabled			= true;
			btnRemoveSysFile->Enabled		= true;
			btnOk->Enabled						= true;
			edSysDestPath->Enabled			= true;
			btnSelectSysDestPath->Enabled	= true;
			PageControl1->Enabled			= true;
		}
	}
}
//---------------------------------------------------------------------------
//SysErrorMessage
void __fastcall TfrmBackup::Label11Click(TObject *Sender)
{
	radPickup->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::Label12Click(TObject *Sender)
{
	radBackup->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::radPickupClick(TObject *Sender)
{
	edPickupPath->Enabled			= radPickup->Checked;
	btnSelectSourcePath->Enabled	= radPickup->Checked;
	btnPickupCopy->Enabled			= radPickup->Checked;

	btnFullDataBackup->Enabled		= radBackup->Checked;
	btnMMDataBackup->Enabled		= radBackup->Checked;
	btnStockDataBackup->Enabled	= radBackup->Checked;

	lbeProgressCap->Visible			= radBackup->Checked;
	lbeProgress->Visible				= radBackup->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::btnSelectSourcePathClick(TObject *Sender)
{
	AnsiString Dir = "";
	if (SelectDirectory("Please enter the path to pick up files from", "", Dir))
	{
		edPickupPath->Text = Dir;
	}
/*	dfsBrowseDirectoryDlg1->Options = TBrowseFlags() << bfDirectoriesOnly;
	if (dfsBrowseDirectoryDlg1->Execute())
	{
		edPickupPath->Text = dfsBrowseDirectoryDlg1->Selection;
	} */
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::btnPickupCopyClick(TObject *Sender)
{
	PickupSourcePath = ExcludeTrailingBackslash(edPickupPath->Text);
	if (PickupSourcePath == "")
	{
		Application->MessageBox("Please select a directory to copy the file from.",
										"Error", MB_OK + MB_ICONERROR);
		edPickupPath->SetFocus();
		return;
	}
	edPickupPath->Text = PickupSourcePath;
	if (!DirectoryExists(PickupSourcePath))
	{
		Application->MessageBox("Please select the path to the backup file first.",
										"Error", MB_OK + MB_ICONERROR);
		edPickupPath->SetFocus();
		return;
	}

	ArchiveDestPath = ExcludeTrailingBackslash(edDestPath->Text);
	if (edDestPath->Text == "")
	{
		Application->MessageBox("Please select a directory to back up the file to.",
										"Error", MB_OK + MB_ICONERROR);
		edDestPath->SetFocus();
		return;
	}
	edDestPath->Text = ArchiveDestPath;
	while (!DirectoryExists(ArchiveDestPath))
	{
		if (Application->MessageBox("Make sure there is a disk in the drive you want to save to.",
											 "Retry?", MB_RETRYCANCEL + MB_ICONERROR) == ID_CANCEL)
		{
			edDestPath->SetFocus();
			return;
		}
	}

	DisableWhileBackingUp(true);

	TStringList *SourceFiles = new TStringList;
	try
	{
		// Get a list of all zip files in the pickup dir.
		GetFilesList(IncludeTrailingBackslash(PickupSourcePath) + "*.zip", SourceFiles);
		if (SourceFiles->Count > 0)
		{
			AnsiString	LatestMMFileName	= "";
			TDateTime	LatestMMFileDate	= 0;

			AnsiString	LatestStockFileName	= "";
			TDateTime	LatestStockFileDate	= 0;
			// loop thru looking for either MM or stock backup files.
			for (int i=0; i<SourceFiles->Count; i++)
			{
				AnsiString CurrentFileName			= SourceFiles->Strings[i];
				AnsiString CurrentFileFullName	= IncludeTrailingBackslash(PickupSourcePath) + CurrentFileName;
//				AnsiString CurrentFileDateStr		= "";
				AnsiString CurrentFileDateStr		= CurrentFileName.SubString(1, 21);
/*				if (CurrentFileName.SubString(23, 8) == "MenuMate")
//				if (CurrentFileName.SubString(1, 8) == "MenuMate")
				{
//					CurrentFileDateStr = CurrentFileName.SubString(10, 14);
					CurrentFileDateStr = CurrentFileName.SubString(10, 14);
				}
				else if (CurrentFileName.SubString(1, 5) == "Stock")
				{
					CurrentFileDateStr = CurrentFileName.SubString(7, 14);
				}    */
				// The file is one of ours. Convert the date string to get the latest.
//				if (CurrentFileDateStr != "" && CurrentFileDateStr.Length() == 14)
				if (CurrentFileDateStr != "" && CurrentFileDateStr.Length() == 21)
				{
//					"yyyy-mm-dd - hh-nn-ss"
					Word Year, Month, Day, Hour, Min, Sec;
					Year	= StrToInt(CurrentFileDateStr.SubString(1, 4));
					Month	= StrToInt(CurrentFileDateStr.SubString(6, 2));
					Day	= StrToInt(CurrentFileDateStr.SubString(9, 2));
					Hour	= StrToInt(CurrentFileDateStr.SubString(14, 2));
					Min	= StrToInt(CurrentFileDateStr.SubString(17, 2));
					Sec	= StrToInt(CurrentFileDateStr.SubString(20, 2));
					TDateTime CurrentFileDate = EncodeDate(Year, Month, Day) + EncodeTime(Hour, Min, Sec, 0);

					if (CurrentFileName.SubString(23, 8).UpperCase() == "MENUMATE")
					{
						if (CurrentFileDate > LatestMMFileDate)
						{
							LatestMMFileDate = CurrentFileDate;
							LatestMMFileName = CurrentFileFullName;
						}
					}
					else if (CurrentFileName.SubString(23, 5).UpperCase() == "STOCK")
					{
						if (CurrentFileDate > LatestStockFileDate)
						{
							LatestStockFileDate = CurrentFileDate;
							LatestStockFileName = CurrentFileFullName;
						}
					}
				}
			}
			if (LatestMMFileName != "")
			{
				if (CopyFile(LatestMMFileName.c_str(),
								(IncludeTrailingBackslash(ArchiveDestPath) + ExtractFileName(LatestMMFileName)).c_str(), true))
				{
					Application->MessageBox((ExtractFileName(LatestMMFileName) + " copied").c_str(),
													"Copy Complete", MB_OK + MB_ICONINFORMATION);
				}
				else
				{
					Application->MessageBox((ExtractFileName(LatestMMFileName) + " NOT copied.\rAlready Copied?").c_str(),
													"Error", MB_OK + MB_ICONERROR);
				}
			}
			if (LatestStockFileName != "")
			{
				if (CopyFile(LatestStockFileName.c_str(),
								(IncludeTrailingBackslash(ArchiveDestPath) + ExtractFileName(LatestStockFileName)).c_str(), true))
				{
					Application->MessageBox((ExtractFileName(LatestStockFileName) + " copied").c_str(),
													"Copy Complete", MB_OK + MB_ICONINFORMATION);
				}
				else
				{
					Application->MessageBox((ExtractFileName(LatestStockFileName) + " NOT copied.\rAlready Copied?").c_str(),
													"Error", MB_OK + MB_ICONERROR);
				}
			}
			if (LatestMMFileName == "" && LatestStockFileName == "")
			{
				Application->MessageBox("No files to copy!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	__finally
	{
		DisableWhileBackingUp(false);
		delete SourceFiles;
	}
}
//---------------------------------------------------------------------------
// Path is the path and wildcard e.g. 'c:\Temp\*.zip'
//---------------------------------------------------------------------------
void TfrmBackup::GetFilesList(AnsiString Path, TStrings *Strings)
{
	TSearchRec sr;
	int iAttributes = faAnyFile;

	if (FindFirst(Path, iAttributes, sr) == 0)
	{
		do
		{
			if (sr.Attr & iAttributes)
			{
				Strings->Add(sr.Name);
			}
		}
		while (FindNext(sr) == 0);
		FindClose(sr);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmBackup::ZipBuilder1Progress(TObject *Sender,
      ProgressType ProgrType, AnsiString Filename, __int64 FileSize)
{
	static long	TotalSize, TotalProgress;
	int			Step;

	switch (ProgrType)
	{
		case TotalSize2Process:	ProgressBar->Position		= 0;
										TotalSize						= FileSize;
										TotalProgress					= 0;
										break;

		case ProgressUpdate:		TotalProgress	 				+= FileSize;
										if (TotalSize)
										{
											Step 							= (int)( (unsigned __int64)TotalProgress * 10000ui64 /
																						(unsigned __int64)TotalSize);
											ProgressBar->Position	= (PGBCONV)(Step);
										}
										break;

		case EndOfBatch:			ProgressBar->Position 		= 1;
										break;
	}
}
//---------------------------------------------------------------------------

