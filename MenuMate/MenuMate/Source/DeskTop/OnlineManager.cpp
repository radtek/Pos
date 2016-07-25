//---------------------------------------------------------------------------
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

#pragma hdrstop

#include "OnlineManager.h"
#include "MMMessageBox.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdUDPServer.hpp>
#include <IdFTPCommon.hpp>
#include <IdAllFTPListParsers.hpp>
#include <IdGlobal.hpp>


//---------------------------------------------------------------------------

#pragma package(smart_init)

#pragma link "IdAllFTPListParsers"

TOnlineManager::TOnlineManager(): ftp(new TIdFTP(NULL)), frmFTP(new TfrmFTP(NULL))
{
//	ftp->UseMLIS = true;
}

TOnlineManager::~TOnlineManager()
{
   //TODO: Add your source code here
}

AnsiString TOnlineManager::GetUpdateFile(AnsiString Password)
{
   AnsiString RetVal = "";
   if(ftpConnect(Password))
   {
      SelectionType = ftpDir;
      while(SelectionType == ftpDir)
      {
         SelectionType = ftpSelectDirOrFile();
         if(SelectionType == ftpDir)
         {
            ftpChangeDir(Selection);
         }
         else if (SelectionType == ftpFile)
         {
            if(ftpGetFile(Selection))
            {
               RetVal = Selection;
            }
            else
            {
                SelectionType = ftpQuit;
            }
         }
      }
		ftpDisconnect();
	}
   return RetVal;
}

bool TOnlineManager::ftpConnect(AnsiString Password)
{
   bool Connected = false;
   try
   {
      #ifdef _DEBUG
      ftp->Host = "192.168.2.12";
      #endif
      #ifndef _DEBUG
      ftp->Host = "deskmate.menumate.com";
      #endif
      ftp->Passive = true;
      ftp->Username = "deskmate";
      ftp->Password = Password;
      ftp->TransferType = ftBinary;
      ftp->Connect();
      ftp->TransferType = ftBinary;
      ftp->TransferMode(dmStream);
      Connected = true;
	}
	catch(Exception &E)
	{
		MessageBox(E.Message,"Error",MB_ICONERROR + MB_OK);
	}
   return Connected;
}

bool TOnlineManager::ftpGetFile(AnsiString File)
{
   bool Retval = true;
   try
   {
		 AnsiString Path = (ExtractFilePath(Application->ExeName) + "Updates");
		 AnsiString FTPDir = UnixPathToDOSPath(ftp->RetrieveCurrentDir());
		 AnsiString UnixDir = ftp->RetrieveCurrentDir();

      Path = Path + FTPDir;
      ForceDirectories(Path);

		ftpDisconnect();
      ftp->Connect();
		frmFTP->Show();

      ftp->OnWorkEnd = ftpGetWorkEnd;
      ftp->OnWork    = ftpWork;
      ftp->OnStatus  = ftpStatus;
      ftp->TransferType = ftBinary;

		ftp->Get(UnixDir + "/" + File,Path + "\\" + File,true);
      while(frmFTP->Result == -1)
      {
         Application->ProcessMessages();
      }
      if(frmFTP->Result == 1)
      {
         ftp->Abort();
         Retval = false;
         MessageBox("FTP Transfer Canceled","FTP Canceled", MB_OK + MB_ICONINFORMATION);
      }
      else
      {
         Selection = Path + "\\" + File;
         MessageBox("FTP Transfer Completed","FTP Completed", MB_OK + MB_ICONINFORMATION);
      }
   }
   catch(Exception &E)
   {
      MessageBox(E.Message,"Error",MB_ICONERROR + MB_OK);
      throw;
   }
   return Retval;
}

void __fastcall TOnlineManager::ftpGetWorkEnd(TObject *Sender, TWorkMode AWorkMode)
{
   if(frmFTP->Visible)
   {
      if(AWorkMode == wmRead)
      {
         frmFTP->Result = 2;
         frmFTP->Close();
         ftp->OnWorkEnd = NULL;
      }
   }
}

void __fastcall TOnlineManager::ftpWork(System::TObject* ASender, TWorkMode AWorkMode, __int64 AWorkCount)
{
   if(frmFTP->Visible)
   {
      AnsiString BytesSoFar = IntToStr(AWorkCount);
      frmFTP->lbeData->Caption = "Bytes : " + BytesSoFar + " of " + SelectionFileSize;
      frmFTP->Update();
      Application->ProcessMessages();      
   }
}

void __fastcall TOnlineManager::ftpStatus(System::TObject* ASender, const TIdStatus AStatus, const System::UnicodeString AStatusText)
{
   if(frmFTP->Visible)
   {
      frmFTP->Memo->Lines->Add(AStatusText);
      Application->ProcessMessages();
   }
}

void TOnlineManager::ftpChangeDir(AnsiString Dir)
{
   try
   {
      if(Dir != "")
      {
         ftp->ChangeDir(Dir);
      }
   }
   catch(Exception &E)
   {
      MessageBox(E.Message,"Error",MB_ICONERROR + MB_OK);
      throw;
   }
}

void TOnlineManager::ftpGetCurrentDir(TfrmVerticalSelect *SelectionForm)
{
  	try
	{
		std::auto_ptr<TStringList> AFiles(new TStringList);
		ftp->List(AFiles.get(),"", true);
		for(int i = 0; i < ftp->DirectoryListing->Count; i++)
		{
			TIdFTPListItem *FTPItem = ftp->DirectoryListing->Items[i];
         bool AddNode = false;
         if(FTPItem->ItemType == ditDirectory || FTPItem->ItemType == ditSymbolicLink)
         {
				AddNode = true;
         }

			if(AddNode)
			{
				TVerticalSelection Item;
				Item.Title = FTPItem->FileName;
				Item.CloseSelection = true;
				Item.Properties["Size"] = FTPItem->Size;
				Item.Properties["FileName"] = FTPItem->FileName;
				if(FTPItem->ItemType == ditDirectory || FTPItem->ItemType == ditSymbolicLink )
				{
					Item.Properties["Directory"] = true;
					Item.Properties["Color"] = IntToStr(clNavy);
					Item.Title = "[" + FTPItem->FileName + "]";
				}
            if (FTPItem->ItemType == ditFile)
            {
					Item.Properties["File"] = true;
               Item.Properties["Color"] = IntToStr(clWhite);
            }
            SelectionForm->Items.push_back(Item);
         }
      }
      for(int i = 0; i < ftp->DirectoryListing->Count; i++)
      {
         TIdFTPListItem *FTPItem = ftp->DirectoryListing->Items[i];
         bool AddNode = false;
         if(FTPItem->ItemType == ditFile)
         {
            AddNode = true;
         }

         if(AddNode)
         {
            TVerticalSelection Item;
            Item.Title = FTPItem->FileName;
            Item.CloseSelection = true;
            Item.Properties["Size"] = FTPItem->Size;
            Item.Properties["FileName"] = FTPItem->FileName;
            if(FTPItem->ItemType == ditDirectory || FTPItem->ItemType == ditSymbolicLink)
            {
					Item.Properties["Directory"] = true;
               Item.Properties["Color"] = IntToStr(clNavy);
               Item.Title = "[" + FTPItem->FileName + "]";
            }
            if (FTPItem->ItemType == ditFile)
            {
					Item.Properties["File"] = true;
               Item.Properties["Color"] = IntToStr(clWhite);
            }
            SelectionForm->Items.push_back(Item);
         }
      }

	}
	catch(Exception &E)
	{
		MessageBox(E.Message,"Error",MB_ICONERROR + MB_OK);
	}
}

void TOnlineManager::ftpDisconnect()
{
   ftp->Quit();
}

TftpControl TOnlineManager::ftpSelectDirOrFile()
{
   TftpControl RetVal = ftpUnknown;

   std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(Screen->ActiveForm));

   TVerticalSelection Item;
   Item.Title = "Close";
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

   Item.Title = "Back";
   Item.CloseSelection = true;
   SelectionForm->Items.push_back(Item);

	ftpGetCurrentDir(SelectionForm.get());

   SelectionForm->ShowModal();

	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem))
	{
		if(SelectedItem.Title == "Close")
		{
			RetVal = ftpQuit;
			Selection = "Quit";
		}
		else if(SelectedItem.Title == "Back")
		{
			RetVal = ftpDir;
			Selection = "..";
		}
		else if(static_cast<bool>(SelectedItem.Properties["Directory"]) == true)
		{
			RetVal = ftpDir;
			Selection = SelectedItem.Properties["FileName"];
		}
		else if(static_cast<bool>(SelectedItem.Properties["File"]) == true)
		{
			RetVal = ftpFile;
			Selection = SelectedItem.Properties["FileName"];
			SelectionFileSize = SelectedItem.Properties["Size"];
		}
	}
	return RetVal;
}


