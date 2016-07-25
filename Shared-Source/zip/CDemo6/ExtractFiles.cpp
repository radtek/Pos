#include <vcl\vcl.h>
#pragma hdrstop

#include "ExtractFiles.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TExtract *Extract;
//---------------------------------------------------------------------------
__fastcall TExtract::TExtract( TComponent *Owner ) : TForm( Owner )
{
  String SpecFolder = "";

	MainForm->GetSpecialFolder( CSIDL_DESKTOPDIRECTORY, SpecFolder );
	DriveComboBox1->Drive = ExtractFileDrive( SpecFolder )[1];
	DirectoryListBox1->Directory = ExtractFilePath( SpecFolder );
}
//---------------------------------------------------------------------------
void __fastcall TExtract::OKButClick( TObject* /*Sender*/ )
{
	MainForm->Canceled   = false;
	MainForm->ExtractDir = DirectoryListBox1->Directory;
	MainForm->ExpandDirs = ( !RadioGroup1->ItemIndex ) ? false : true;
	MainForm->OverWr	  = ( !RadioGroup2->ItemIndex ) ? false : true;
	MainForm->AllFiles	  = true;
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TExtract::CancelButClick( TObject* /*Sender*/ )
{
	MainForm->ExtractDir = "";
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TExtract::FormActivate(TObject* /*Sender*/)
{
	MainForm->Canceled = true;					 // Default.
}
//---------------------------------------------------------------------------
