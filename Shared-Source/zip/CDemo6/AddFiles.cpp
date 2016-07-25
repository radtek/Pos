#include <vcl.h>
#pragma hdrstop

#include "AddFiles.h"
#include "ZipUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TAddFile *AddFile;
//---------------------------------------------------------------------------
__fastcall TAddFile::TAddFile( TComponent *Owner ) : TForm( Owner )
{
	InMouseClick = false;
	String SpecFolder = "";

	MainForm->GetSpecialFolder( CSIDL_DESKTOPDIRECTORY, SpecFolder );
	DriveComboBox1->Drive = ExtractFileDrive( SpecFolder )[1];
	DirectoryListBox1->Directory = ExtractFilePath( SpecFolder );
}
//---------------------------------------------------------------------------
void __fastcall TAddFile::OKButClick( TObject* /*Sender*/ )
{
	MainForm->Canceled = false;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TAddFile::CancelButClick( TObject* /*Sender*/ )
{
	MainForm->Canceled = true;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TAddFile::SortButClick( TObject* /*Sender*/ )
{
	SelectedList->Sorted = true;
	SortBut->Enabled     = false;  // List will remain sorted.
}
//---------------------------------------------------------------------------
void __fastcall TAddFile::RemoveButClick( TObject* /*Sender*/ )
{
	for ( int i = SelectedList->Items->Count - 1; i >= 0 ; i-- )
  {
		if ( SelectedList->Selected[i] )
			SelectedList->Items->Delete( i );
	}
}
//---------------------------------------------------------------------------
void __fastcall TAddFile::SelectAllButClick( TObject* /*Sender*/ )
{
	for ( int i = 0; i < FileListBox1->Items->Count; i++ )
		FileListBox1->Selected[i] = true;
}
//---------------------------------------------------------------------------
void __fastcall TAddFile::AddDirButClick( TObject* /*Sender*/ )
{
	String FullName;

	MainForm->Canceled = true;  // Default.
	for ( int i = 0; i < DirectoryListBox1->Items->Count; i++ ) {
		if ( DirectoryListBox1->Selected[i] ) {
			// Add this file if it isn't already in listbox.
			FullName = DelimitPath(DirectoryListBox1->Directory, true) + "*.*";

			if ( SelectedList->Items->IndexOf( FullName ) < 0 )
				SelectedList->Items->Add( FullName );
			// Never de-select dirnames from the DirectoryList!
			// DirectoryListBox1->Selected[i] = false;
		}
	}
	// Position the "SelectedList" listbox at the bottom.
	SelectedList->Selected[SelectedList->Items->Count - 1] = true;
	SelectedList->Selected[SelectedList->Items->Count - 1] = false;
}
//---------------------------------------------------------------------------
void __fastcall TAddFile::AddFileButClick( TObject* /*Sender*/ )
{
	String FullName;

	MainForm->Canceled = true;  // default
	for ( int i = 0; i < FileListBox1->Items->Count; i++ )
  {
		if ( FileListBox1->Selected[i] ) {
			// Add this file if it isn't already in listbox.
			FullName = DelimitPath(DirectoryListBox1->Directory, true) + FileListBox1->Items->Strings[i];
			if ( SelectedList->Items->IndexOf( FullName ) < 0 )
				SelectedList->Items->Add( FullName );
			FileListBox1->Selected[i] = false;
		}
	}
	// Position the "SelectedList" listbox at the bottom.
	SelectedList->Selected[SelectedList->Items->Count - 1] = true;
	SelectedList->Selected[SelectedList->Items->Count - 1] = false;
}
//---------------------------------------------------------------------------
void __fastcall TAddFile::AddWildBttnClick( TObject* /*Sender*/ )
{
	if ( WildEdit->Text.Length() && SelectedList->Items->IndexOf( WildEdit->Text ) < 0 )
  {
		SelectedList->Items->Add( WildEdit->Text );
		// Position the "SelectedList" listbox at the bottom.
		SelectedList->Selected[SelectedList->Items->Count - 1] = true;
		SelectedList->Selected[SelectedList->Items->Count - 1] = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TAddFile::AddWildPathBttnClick( TObject* /*Sender*/ )
{
	if ( WildEdit->Text.Length() )
  {
		String FullName = DelimitPath(DirectoryListBox1->Directory, true) + WildEdit->Text;

		if ( SelectedList->Items->IndexOf( FullName ) < 0 ) {
			SelectedList->Items->Add( FullName );
			// Position the "SelectedList" listbox at the bottom.
			SelectedList->Selected[SelectedList->Items->Count - 1] = true;
			SelectedList->Selected[SelectedList->Items->Count - 1] = false;
		}
	}
}

