#include <vcl.h>
#pragma hdrstop

#include "AddFiles.h"
#include "ZipUtils.h"
//---------------------------------------------------------------------------
#pragma package( smart_init )
#pragma resource "*.dfm"

TAddFilesForm *AddFilesForm;
//---------------------------------------------------------------------------
__fastcall TAddFilesForm::TAddFilesForm( TComponent *Owner ) : TForm( Owner )
{
  	InMouseClick = false;
}
//---------------------------------------------------------------------------
void __fastcall TAddFilesForm::OKButClick(TObject* /*Sender*/)
{
	MainForm->Cancelled = false;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TAddFilesForm::CancelButClick(TObject* /*Sender*/)
{
	MainForm->Cancelled = true;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TAddFilesForm::SortButClick(TObject* /*Sender*/)
{
	SelectedList->Sorted = true;
	SortBut->Enabled     = false;  // List will remain sorted.
}
//---------------------------------------------------------------------------
void __fastcall TAddFilesForm::RemoveButClick(TObject* /*Sender*/)
{
	for(int i = SelectedList->Items->Count - 1; i >= 0 ; i--)
		if(SelectedList->Selected[i]) SelectedList->Items->Delete(i);
}
//---------------------------------------------------------------------------
void __fastcall TAddFilesForm::SelectAllButClick(TObject* /*Sender*/)
{
	for(int i = 0; i < FileListBox1->Items->Count; i++)
		FileListBox1->Selected[i] = true;
}
//---------------------------------------------------------------------------
void __fastcall TAddFilesForm::AddDirButClick(TObject* /*Sender*/)
{
	String FullName;

	MainForm->Cancelled = true;  // Default.
	for(int i = 0; i < DirectoryListBox1->Items->Count; i++)
  {
		if(DirectoryListBox1->Selected[i])
    {
			// Add this file if it isn't already in listbox.
			FullName = DelimitPath(DirectoryListBox1->Directory, true) + "*.*";

			if(SelectedList->Items->IndexOf( FullName ) < 0)	SelectedList->Items->Add( FullName );
			// Never de-select dirnames from the DirectoryList!
			// DirectoryListBox1->Selected[i] = false;
		}
	}
	// Position the "SelectedList" listbox at the bottom.
	SelectedList->Selected[SelectedList->Items->Count - 1] = true;
	SelectedList->Selected[SelectedList->Items->Count - 1] = false;
}
//---------------------------------------------------------------------------
void __fastcall TAddFilesForm::AddFileButClick(TObject* /*Sender*/)
{
	String FullName;

	MainForm->Cancelled = true;  // default
	for(int i = 0; i < FileListBox1->Items->Count; i++)
  {
		if(FileListBox1->Selected[i])
    {
			// Add this file if it isn't already in listbox.
			FullName =  DelimitPath(DirectoryListBox1->Directory, true) +
                  FileListBox1->Items->Strings[i];
			if(SelectedList->Items->IndexOf(FullName) < 0)	SelectedList->Items->Add(FullName);
			FileListBox1->Selected[i] = false;
		}
	}
	// Position the "SelectedList" listbox at the bottom.
	SelectedList->Selected[SelectedList->Items->Count - 1] = true;
	SelectedList->Selected[SelectedList->Items->Count - 1] = false;
}

