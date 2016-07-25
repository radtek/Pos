#include <vcl.h>
#include <sharemem.hpp>
#pragma hdrstop

#include "Unit3.h"
#include "ZBCompat.h"
#pragma package(smart_init)
#pragma link "ZipBuilder"
#pragma resource "*.dfm"

TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent *Owner) : TForm(Owner)
{
    // The easiest way to handle DLL loading and unloading is to load them in
   // the form's Creator, and unload them in the form's Destructor
  ZipBuilder1->Dll_Load = true;
}
//---------------------------------------------------------------------------
__fastcall TForm3::~TForm3(void)
{
  ZipBuilder1->Dll_Load = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::AddButClick(TObject* /*Sender*/)
{
	Memo1->Clear();
	Memo1->Lines->Add( "Adding test*.txt to test1.zip" );
	ZipBuilder1->ZipFileName = "test1.zip";
	ZipBuilder1->Verbose = true;

	// Add as many filespecs as we want to:
	// (MS-DOS Wildcards are OK)
	ZipBuilder1->FSpecArgs->Add( "test*.txt" );
	// Set the AddOptions; these also appear in Property Inspector:
	// NOTE: This is the new "set" syntax
	ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddHiddenFiles;
	ZipBuilder1->AddOptions = ZipBuilder1->AddOptions >> AddEncrypt; // no password
	ZipBuilder1->Add();  // do it!
}
//---------------------------------------------------------------------------
void __fastcall TForm3::ZipBuilder1Message(TObject* /*Sender*/, int /*ErrCode*/, String Message )
{
	// Add a message from the DLLs to the memo box
	Memo1->Lines->Add( Message );
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button2Click(TObject* /*Sender*/)
{
	Close();  // exit program
}
//---------------------------------------------------------------------------
void __fastcall TForm3::ExtrButClick(TObject* /*Sender*/)
{
	Memo1->Clear();
	if(!FileExists("test1.zip"))
  {
		ShowMessage( "Error - test1.zip not found; do a Add first" );
		return;
	}
	Memo1->Lines->Add( "Extracting from test1.zip" );
	ZipBuilder1->ZipFileName = "test1.zip";
	ZipBuilder1->Verbose = true;

	// We can specify as many filespecs as we want to extract:
	// (MS-DOS Wildcards are OK)
	ZipBuilder1->FSpecArgs->Add( "test*.txt" );

	// Set the ExtrOptions; these also appear in Property Inspector:
	ZipBuilder1->ExtrOptions = ZipBuilder1->ExtrOptions << ExtrOverWrite;
	ZipBuilder1->Extract();  // do it!
}
//---------------------------------------------------------------------------
void __fastcall TForm3::VersButClick(TObject* /*Sender*/)
{
  // from version 1.7.0 ZipBuilder has a FullVersionString metod that gives us all info we need
  String aString = ZipBuilder1->FullVersionString();
  Application->MessageBox(aString.T_STR(), TEXT("VERSIONS"), MB_OK | MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall TForm3::ListButClick(TObject* /*Sender*/)
{
	TCHAR fname[80], datetime[20], outbuf[120];

	Memo1->Clear();
	if(!FileExists("test1.zip"))
  {
		ShowMessage( "Error - test1.zip not found; do a Add first" );
		return;
	}
	Memo1->Lines->Add( "Listing test1.zip" );
	// The ZipContents TList is automatically filled with the contents of the
	// zipfile when the filename is assigned.
	ZipBuilder1->ZipFileName = "test1.zip";

	if(!ZipBuilder1->Count) return;     // no files in archive, or bad archive

	// Print some column headers
	Memo1->Lines->Add( "Filename         Cmp byt  Unc byt      Date/Time" );
	for(int i = 0; i < ZipBuilder1->Count; i++)
  {
      // From Release 1.70 we can use the DirEntry[] property from ZipBuilder
      ZipDirEntry* dirp = ZipBuilder1->DirEntry[i];
#ifdef UNICODE
			wcscpy(fname, dirp->FileName.c_bstr());
      wcscpy(datetime, FormatDateTime( TEXT("ddddd  t"), FileDateToDateTime(dirp->DateTime)).c_bstr());
      swprintf(outbuf, TEXT("%-16s %6d   %6d    %-16s"), fname, dirp->CompressedSize,
                                                         dirp->UncompressedSize, datetime );
#else
      strcpy(fname, dirp->FileName.c_str());
      strcpy(datetime, FormatDateTime( "ddddd  t", FileDateToDateTime(dirp->DateTime)).c_str());
      sprintf(outbuf, "%-16s %6d   %6d    %-16s", fname, dirp->CompressedSize,
                                                         dirp->UncompressedSize, datetime );
#endif
      Memo1->Lines->Add( outbuf );
   } // end for
}
//---------------------------------------------------------------------------
void __fastcall TForm3::TestButClick(TObject* /*Sender*/)
{
   Memo1->Clear();
   if(!FileExists("test1.zip"))
   {
      ShowMessage( "Error - test1.zip not found; do a Zip first" );
      return;
   }
   Memo1->Lines->Add( "Testing test1.zip" );
   ZipBuilder1->ZipFileName = "test1.zip";
   ZipBuilder1->Verbose = true;

   // TEST ONLY, do NOT extract!
   ZipBuilder1->ExtrOptions = ZipBuilder1->ExtrOptions << ExtrTest;

   ZipBuilder1->Extract();  // do it! (just test, don't save extracted files)
}
//---------------------------------------------------------------------------
void __fastcall TForm3::DeleteButClick(TObject* /*Sender*/)
{
   Memo1->Clear();
   if(!FileExists("test1.zip"))
   {
      ShowMessage( "Error - test1.zip not found; do a Zip first" );
      return;
   }
   Memo1->Lines->Add( "Deleting from test1.zip" );
   ZipBuilder1->ZipFileName = "test1.zip";
   ZipBuilder1->Verbose = true;

   // We can specify as many filespecs as we want to delete:
   // (MS-DOS Wildcards are OK)
   ZipBuilder1->FSpecArgs->Add( "testa.txt" );
   ZipBuilder1->FSpecArgs->Add( "testb.txt" );

   ZipBuilder1->Delete();  // do it!
}
//---------------------------------------------------------------------------
void __fastcall TForm3::AddPwdButClick(TObject* /*Sender*/)
{
   Memo1->Clear();
   Memo1->Lines->Add( "Adding test*.txt to test1.zip" );
   ZipBuilder1->ZipFileName = "test1.zip";
   ZipBuilder1->Verbose     = true;

   // Add as many filespecs as we want to:
   // (MS-DOS Wildcards are OK)
   ZipBuilder1->FSpecArgs->Add( "test*.txt" );
   // Set the AddOptions; these also appear in Property Inspector:
   // NOTE: This is the new "set" syntax
   ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddEncrypt;
   ZipBuilder1->Add();  // do it!
}

