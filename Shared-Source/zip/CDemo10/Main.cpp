//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "ZipUtils.h"
#include "Main.h"
#include "ZBCompat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ZipBuilder"
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
  : TForm(Owner)
{
  SampleZipName = ExtractFilePath(Application->ExeName) + "samples.zip";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AddBtnClick(TObject* /*Sender*/)
{
   // create an archive from all the files in this demo
  // filenames will be changed in the OnsetAddName event

  ZipBuilder->ZipFileName = SampleZipName;
  ZipBuilder->AddOptions = ZipBuilder->AddOptions << AddDirNames;
  ZipBuilder->FSpecArgs->Clear();
  ZipBuilder->FSpecArgs->Add(ExtractFilePath(Application->ExeName) + "*.*");
  ZipBuilder->FSpecArgsExcl->Clear();
  ZipBuilder->FSpecArgsExcl->Add(ExtractFilePath(Application->ExeName) + "*.~*");
  ZipBuilder->FSpecArgsExcl->Add(ExtractFilePath(Application->ExeName) + "*.zip");
  ZipBuilder->Add();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExtBtnClick(TObject* /*Sender*/)
{
  // extract the files from the archive and ask to overwrite when newer
  // filenames will be changed in the OnsetExtName event
  if(!FileExists(SampleZipName))
  {
    Memo1->Lines->Add("************ ERROR ***********");
    Memo1->Lines->Add(SampleZipName + " does not exists- create it first");
    return;
  }
  ZipBuilder->ZipFileName = SampleZipName;
  ZipBuilder->ExtrBaseDir = ExtractFilePath(Application->ExeName);
  ZipBuilder->ExtrOptions = ZipBuilder->ExtrOptions << ExtrDirNames;
  ZipBuilder->FSpecArgs->Clear();   // extract all files
  ZipBuilder->Extract();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZipBuilderSetAddName(TObject* /*Sender*/,
      String &FileName, const String ExtName, bool &IsChanged)
{
  // for each file add the drive letter to the filespec in format
  // [?]/filespec  were ? is the drive letter
  String Drive = ExtractFileDrive(ExtName);
     // following line is only for showing the result in the demo and is not needed
  String OrigFileName = FileName;
  FileName = "[" + String(Drive[1]) + "]\\" + FileName;
  IsChanged = true;
    // following line is only for showing the result in the demo and is not needed
  Memo1->Lines->Add(OrigFileName + " is changed into : " + FileName);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZipBuilderSetExtName(TObject* /*Sender*/,
      String &OldFileName, bool &IsChanged)
{
  if((OldFileName[1] == '[') && (OldFileName[3] == ']') && (OldFileName[4] == '\\'))
  {
     // the filename probably starts with a drive spec
    String drive = String(OldFileName[2]) + ":\\";
     // following line is only for showing the result in the demo and is not needed
    String OrigFileName = OldFileName;

    OldFileName = drive + OldFileName.SubString(5, OldFileName.Length() - 4);
    IsChanged = true;
      // following line is only for showing the result in the demo and is not needed
    Memo1->Lines->Add(OrigFileName + " is changed into : " + OldFileName);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZipBuilderExtractOverwrite(TObject* /*Sender*/,
      String ForFile, bool IsOlder, bool &DoOverwrite, int /*DirIndex*/)
{
  if(IsOlder)  // Only when existing file is newer(or equal) we will ask what to do.
  {
    String q = "Do you want to overwrite the file:\n\r" + ForFile;
    unsigned DefBtn = DoOverwrite ? MB_DEFBUTTON1 : MB_DEFBUTTON2;
    int res = MessageBox(this->Handle, q.T_STR(), TEXT("Confirm"), MB_YESNO | MB_ICONQUESTION | DefBtn);
    DoOverwrite = (res == ID_YES) ? true : false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ContentBtnClick(TObject* /*Sender*/)
{
  if(!FileExists(SampleZipName))
  {
    Memo1->Lines->Add("************ ERROR ***********");
    Memo1->Lines->Add(SampleZipName + " does not exists- create it first");
    return;
  }
   ZipBuilder->ZipFileName = SampleZipName; // to execute list
   Memo1->Lines->Add(" ");
   Memo1->Lines->Add("Content of " + SampleZipName + " = ");
    Memo1->Lines->Add(" ");
   for(int i = 0; i < ZipBuilder->Count; ++i)
   {
      ZipDirEntry* Entry = ZipBuilder->DirEntry[i];
      Memo1->Lines->Add(Entry->FileName);
   }
}
//---------------------------------------------------------------------------

