//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

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
  SamplePath = ExtractFilePath(Application->ExeName) + "samples\\";
  SampleZipName = ExtractFilePath(Application->ExeName) + "samples.zip";
  SampleShortName = "samples.zip";
  randomize();
  count = 0;
  StatusBar1->Panels->Items[0]->Text = "Not run yet - Push \"Create Sample\" to start";
	StatusBar1->Panels->Items[1]->Text = "0";
  DWORD dwStyle = GetWindowLong(TreeView1->Handle, GWL_STYLE);
  dwStyle = dwStyle | TVS_CHECKBOXES;
  SetWindowLong(TreeView1->Handle, GWL_STYLE, dwStyle);
}
//---------------------------------------------------------------------------
String TMainForm::RandomName(void)
{
  String Result;
  for(int i = 0; i < 3 + random(4); ++i)
  {
    Result += String(char('A' + random(25)));
  }
  return Result + ".txt";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CrFBtnClick(TObject* /*Sender*/)
{
  // first create sample files
  TStringList* strngs = new TStringList();
  try
  {
    for(int i = 0; i < 4; ++i)
    {
      strngs->Clear();
      SampleNames[i] = RandomName();
      String fn = SamplePath + SampleNames[i];
      if(FileExists(fn)) strngs->LoadFromFile(fn);
      for(int k = 0; k < 1 + random(5); ++k)
        strngs->Add("added this line ( " + IntToStr(strngs->Count+1) +  " , " +
                    IntToStr(i) + " , " + " ) " + DateTimeToStr(Now()));
      strngs->SaveToFile(fn);
    }
  }
  __finally
  {
    delete strngs;
  }
  // next create the sample archive
  ZipBuilder->ZipFileName = SampleZipName;
  ZipBuilder->AddOptions = ZipBuilder->AddOptions << AddResetArchive << AddArchiveOnly;
  ZipBuilder->RootDir = SamplePath;
  ZipBuilder->FSpecArgs->Clear();
  ZipBuilder->FSpecArgs->Add("*.txt");
  ZipBuilder->Add();
  StatusBar1->Panels->Items[0]->Text = SampleShortName + " created";
  StatusBar1->Panels->Items[2]->Text = "Files in zip: " + IntToStr(ZipBuilder->Count);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZipBuilderMessage(TObject* /*Sender*/, int ErrCode,
                                             String Message)
{
  if((ErrCode != 0) && (ErrCode != 255) && (ErrCode != 13))
        StatusBar1->Panels->Items[0]->Text = "{" + IntToStr(ErrCode) + "} " + Message;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ShContBtnClick(TObject* /*Sender*/)
{
  TreeView1->Items->Clear();
  TTreeNode* root = TreeView1->Items->AddChild(NULL, "Files in \"" + SampleShortName + "\" ");
  ZipBuilder->ZipFileName = SampleZipName; // to repeat list
  if(ZipBuilder->Count)
    root->Text = root->Text + DateTimeToStr(FileDateToDateTime(FileAge(SampleZipName))) +
                 "  " + IntToStr(ZipBuilder->ZipFileSize);
  else return; // nothing to show
  for(int idx = ZipBuilder->Count-1; idx >= 0; --idx)
  {
    ZipDirEntry* Entry = ZipBuilder->DirEntry[idx];
    String fn = NoPrefix(Entry->FileName);
    TTreeNode* node = root->getFirstChild();
    int cmp = -2;
    // already in list?
    while(node && cmp)
    {
      cmp = AnsiCompareFileName(fn, node->Text);
      if(cmp) node = node->GetNext(); // cmp !=  is not found
    }
    if(!node) node = TreeView1->Items->AddChild(root, fn); // add it
    TTreeNode* child = TreeView1->Items->AddChild(node,
                       DateTimeToStr(FileDateToDateTime(Entry->DateTime))
                       + "  " + IntToStr(Entry->UncompressedSize));
    child->Data = reinterpret_cast<void*>(idx);
    child->Text = child->Text + "  (" + Entry->FileName + ")";
  }
  root->Expand(true);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ChangeBtnClick(TObject* /*Sender*/)
{
  // change some sample files
  TStringList* strngs = new TStringList();
  try
  {
    for(int i = 0; i < 1 + random(4); ++i)
    {
      int j = random(4);
      strngs->Clear();
      String fn = SamplePath + SampleNames[j];
      strngs->LoadFromFile(fn);
      for(int k = 0; k < 1 + random(5); ++k)
        strngs->Add("added this line ( " + IntToStr(strngs->Count+1) +  " , " +
                    IntToStr(i) + " , " + " ) " + DateTimeToStr(Now()));
      strngs->SaveToFile(fn);
      ++count;
    }
  }
  __finally
  {
    delete strngs;
  }
  StatusBar1->Panels->Items[1]->Text = "Files changed: " + IntToStr(count);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::UpdateBtnClick(TObject* /*Sender*/)
{
    // update the sample archive
  ZipBuilder->ZipFileName = SampleZipName;
  ZipBuilder->AddOptions = ZipBuilder->AddOptions << AddDirNames << AddResetArchive << AddArchiveOnly;
  ZipBuilder->RootDir = SamplePath;
  ZipBuilder->FSpecArgs->Clear();
  ZipBuilder->FSpecArgs->Add("*.txt");
  ZipBuilder->Add();
  count = 0;
  StatusBar1->Panels->Items[0]->Text = SampleShortName + " updated";
  StatusBar1->Panels->Items[1]->Text = "Files changed: " + IntToStr(count);
  StatusBar1->Panels->Items[2]->Text = "Files in zip: " + IntToStr(ZipBuilder->Count);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZipBuilderSetNewName(TObject* /*Sender*/,
      String &OldFileName, bool &IsChanged)
{
  if(FileInZip(OldFileName))
  {
    String Dirtemp = ExtractFilePath(OldFileName);
    Dirtemp = NoPrefix(Dirtemp);
    String Timedir = "\\#" + FormatDateTime("mmddhhnn", Now()) + String("#\\");
    Dirtemp += Timedir;
    OldFileName = Dirtemp + ExtractFileName(OldFileName);
    IsChanged = true;
  }
}
//---------------------------------------------------------------------------
bool TMainForm::FileInZip(String &File)
{
  if(!ZipBuilder->Count) return false; // if empty then file does not exists
  bool Result = false;
  for(int idx = 0; idx < ZipBuilder->Count; ++idx)
  {
    ZipDirEntry* Entry = ZipBuilder->DirEntry[idx];
    String fn = NoPrefix(Entry->FileName);
    if(!AnsiCompareFileName(fn, File))
    {
      Result = true;
      break;
    }
  }
  return Result;
}
//---------------------------------------------------------------------------
String TMainForm::NoPrefix(String& Path)
{ // remove versioning dir
  String Result = Path;
  int flen = Path.Length();
  int fleft = Path.Pos("\\#");  // versioning dir  style ..\#TIME#\file.ext
  int fright = Path.Pos("#\\");
  if((!fleft && fright && Path.Pos("#")) || ((fleft == 1) && (fright == 11)))
    // if "\\#" is starting path then the first "\\# is removed
       Result = Path.SubString(fright + 2, flen);
  return Result;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DelBtnClick(TObject* /*Sender*/)
{
  TSearchRec SRec;
  String Name;
  if(FindFirst(SamplePath + "*.txt", faAnyFile, SRec) == 0)
  {
    do
    {
      DeleteFile(SamplePath + String(SRec.Name));
    } while (FindNext(SRec) == 0);
    FindClose(SRec);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DelAllBtnClick(TObject* /*Sender*/)
{
  if(FileExists(SampleZipName)) DeleteFile(SampleZipName);
  StatusBar1->Panels->Items[0]->Text = SampleShortName + " deleted";
  StatusBar1->Panels->Items[1]->Text = "Files changed: 0";
  StatusBar1->Panels->Items[2]->Text = "Files in zip: 0";
	DelBtnClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RestLatBtnClick(TObject* /*Sender*/)
{
  if(TreeView1->Items->Count == 0) return; // treeview is not showing files
  TTreeNode* root = TreeView1->Items->Item[0];
  if(!root) return; // empty
  ZipBuilder->FSpecArgs->Clear();
  ZipBuilder->ExtrBaseDir = SamplePath;
  TTreeNode* node = root->getFirstChild();
  while(node)
  {
    TTreeNode* child = node->getFirstChild();  //first node is always latest
    if(child)
    {
      int idx = reinterpret_cast<int>(child->Data);
      if((idx >=0) && (idx < ZipBuilder->Count))
      {
        ZipDirEntry* Entry = ZipBuilder->DirEntry[idx];
//        ZipBuilder->FSpecArgs->Add(NoPrefix(Entry->FileName));
        ZipBuilder->FSpecArgs->Add(Entry->FileName);
      }
    }
    node = node->GetNext();
  }
  ZipBuilder->Extract();
  if(ZipBuilder->ErrCode)
     MessageDlg("Could not Extract files : " + ZipBuilder->Message, mtError ,
                TMsgDlgButtons() << mbOK,0);
  else MessageDlg("Extracted " + IntToStr(ZipBuilder->SuccessCnt) + " files", mtInformation ,
                TMsgDlgButtons() << mbOK,0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZipBuilderExtractOverwrite(TObject* /*Sender*/,
      String ForFile, bool IsOlder, bool &DoOverwrite, int /*DirIndex*/)
{
  if(IsOlder)  // Only when existing file is older (or equal) we will ask what to do.
  {
    String q = "Do you want to overwrite the file:\n\r" + ForFile;
    unsigned DefBtn = DoOverwrite ? MB_DEFBUTTON1 : MB_DEFBUTTON2;
    int res = MessageBox(this->Handle, q.T_STR(), TEXT("Confirm"), MB_YESNO | MB_ICONQUESTION | DefBtn);
    DoOverwrite = (res == ID_YES);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RestBtnClick(TObject* /*Sender*/)
{
  if(TreeView1->Items->Count == 0) return; // treeview is not showing files
  TTreeNode* root = TreeView1->Items->Item[0];
  if(!root) return; // empty
  ZipBuilder->FSpecArgs->Clear();
  ZipBuilder->ExtrBaseDir = SamplePath;
  TTreeNode* node = root->getFirstChild();
  while(node)
  {
    TTreeNode* child = node->getFirstChild();
    while(child)
    {
      if(IsNodeChecked(child))
      {
        int idx = reinterpret_cast<int>(child->Data);
        if((idx >=0) && (idx < ZipBuilder->Count))
        {
          ZipDirEntry* Entry = ZipBuilder->DirEntry[idx];
          ZipBuilder->FSpecArgs->Add(NoPrefix(Entry->FileName));
        }
      }
      child = node->GetNextChild(child);
    }
    node = node->GetNext();
  }
  if(ZipBuilder->FSpecArgs->Count)
  {
    ZipBuilder->Extract();
    if(ZipBuilder->ErrCode)
       MessageDlg("Could not Extract files : " + ZipBuilder->Message, mtError ,
                TMsgDlgButtons() << mbOK,0);
    else MessageDlg("Extracted " + IntToStr(ZipBuilder->SuccessCnt) + " files", mtInformation ,
                TMsgDlgButtons() << mbOK,0);
  }
  else
   MessageDlg("No files selected for extraction", mtError ,
                TMsgDlgButtons() << mbOK,0);
}
//---------------------------------------------------------------------------
bool TMainForm::IsNodeChecked(TTreeNode* Node)
{
  TVITEM tvi;
  tvi.mask = TVIF_HANDLE | TVIF_STATE;
  tvi.hItem = Node->ItemId;
  tvi.stateMask = TVIS_STATEIMAGEMASK;
  SNDMSG(Node->TreeView->Handle, TVM_GETITEM,0,reinterpret_cast<LPARAM>(&tvi));
  return(tvi.state & 0x2000);
}
//---------------------------------------------------------------------------

