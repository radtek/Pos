//---------------------------------------------------------------------------
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

#pragma hdrstop

#include "LanManager.h"
#include "MMMessageBox.h"



//---------------------------------------------------------------------------

#pragma package(smart_init)


TLanManager::TLanManager(AnsiString inCurrentDir)
{
   CurrentDir = inCurrentDir;
}

TLanManager::~TLanManager()
{
   //TODO: Add your source code here
}

AnsiString TLanManager::GetUpdateFile()
{
   AnsiString RetVal = "";
   SelectionType = ftpDir;
   while(SelectionType == ftpDir)
   {
      SelectionType = SelectDirOrFile();
      if(SelectionType == ftpDir)
      {
         ChangeDir(Selection);
      }
      else if (SelectionType == ftpFile)
      {
         if(GetFile(Selection))
         {
            RetVal = Selection;
         }
         else
         {
             SelectionType = ftpDir;
         }
      }
   }

   return RetVal;
}

bool TLanManager::GetFile(AnsiString File)
{
   bool Retval = true;
   try
   {
      Selection = CurrentDir + "\\" + File;
   }
   catch(Exception &E)
   {
      MessageBox(E.Message,"Error",MB_ICONERROR + MB_OK);
      throw;
   }
   return Retval;
}


void TLanManager::ChangeDir(AnsiString Dir)
{
   try
   {
      if(Dir != "" && Dir != ".")
      {
         if(Dir == "..")
         {
            CurrentDir = ExtractFilePath(ExcludeTrailingBackslash(CurrentDir));
            CurrentDir = IncludeTrailingBackslash(CurrentDir);
         }
         else
         {
            CurrentDir = IncludeTrailingBackslash(CurrentDir) + Dir;
         }
      }
   }
   catch(Exception &E)
   {
      MessageBox(E.Message,"Error",MB_ICONERROR + MB_OK);
      throw;
   }
}

bool TLanManager::GetCurrentDir(TfrmVerticalSelect *SelectionForm)
{
  	try
	{
      std::auto_ptr<TIdFTPListItems> DirectoryListing(new TIdFTPListItems);
		List(DirectoryListing.get());

      for(int i = 0; i < DirectoryListing->Count; i++)
      {
         TIdFTPListItem *FTPItem = DirectoryListing->Items[i];
         bool AddNode = false;
         if(FTPItem->ItemType == ditDirectory)
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
            if(FTPItem->ItemType == ditDirectory )
            {
					Item.Properties["Directory"] = true;
               Item.Properties["Color"] = IntToStr(clNavy);
               if(FTPItem->FileName == "MenuMateRelease")
               {
                  Item.Title = "[MenuMate Public Release]";
               }
               else if(FTPItem->FileName == "MenuMate")
               {
                  Item.Title = "[MenuMate Internal Beta]";
               }
               else
               {
                  Item.Title = "[" + FTPItem->FileName + "]";
               }
            }
            if (FTPItem->ItemType == ditFile)
				{
					Item.Properties["File"] = true;
					Item.Properties["Color"] = IntToStr(clWhite);
            }
            SelectionForm->Items.push_back(Item);
         }
      }

      for(int i = 0; i < DirectoryListing->Count; i++)
      {
			TIdFTPListItem *FTPItem = DirectoryListing->Items[i];
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
            if(FTPItem->ItemType == ditDirectory )
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
	catch(...)
	{
	}
}

TftpControl TLanManager::SelectDirOrFile()
{
	TftpControl RetVal = ftpUnknown;

	std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(Screen->ActiveForm));

	TVerticalSelection Item;
	Item.Title = "Close";
	Item.CloseSelection = true;
	SelectionForm->Items.push_back(Item);

	GetCurrentDir(SelectionForm.get());

	SelectionForm->ShowModal();

	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem))
	{
		if(SelectedItem.Title == "Close")
		{
			RetVal = ftpQuit;
			Selection = "Quit";
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

void TLanManager::List(TIdFTPListItems * DirectoryListing)
{
	//TODO: Add your source code here
	TSearchRec List;
	AnsiString SearchDir = IncludeTrailingPathDelimiter(CurrentDir) + "*.*";
	int FFResult = FindFirst(SearchDir, faAnyFile	, List);
   if (FFResult == 0)
   {
      do
      {
         if(List.Name != ".")
         {
            TIdFTPListItem *File = DirectoryListing->Add();
            File->FileName = List.Name;
            File->Size = List.Size;
            if ((List.Attr & faDirectory) == faDirectory)
            {
               File->ItemType = ditDirectory;
            }
            else
            {
               File->ItemType = ditFile;
            }
         }
      } while (FindNext(List) == 0);
      FindClose(List);
   }
}
