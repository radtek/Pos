//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "ZipBuilder.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
  TStatusBar *StatusBar1;
  TPanel *Panel1;
  TButton *CrFBtn;
  TButton *ShContBtn;
  TButton *ChangeBtn;
  TButton *UpdateBtn;
  TButton *DelBtn;
  TButton *DelAllBtn;
  TButton *RestLatBtn;
  TButton *RestBtn;
  TTreeView *TreeView1;
  TZipBuilder *ZipBuilder;
  void __fastcall CrFBtnClick(TObject *Sender);
  void __fastcall ZipBuilderMessage(TObject *Sender, int ErrCode,
          AnsiString Message);
  void __fastcall ShContBtnClick(TObject *Sender);
  void __fastcall ChangeBtnClick(TObject *Sender);
  void __fastcall UpdateBtnClick(TObject *Sender);
  void __fastcall ZipBuilderSetNewName(TObject *Sender,
          AnsiString &OldFileName, bool &IsChanged);
  void __fastcall DelBtnClick(TObject *Sender);
  void __fastcall DelAllBtnClick(TObject *Sender);
  void __fastcall RestLatBtnClick(TObject *Sender);
  void __fastcall ZipBuilderExtractOverwrite(TObject *Sender,
          AnsiString ForFile, bool IsOlder, bool &DoOverwrite,
          int DirIndex);
  void __fastcall RestBtnClick(TObject *Sender);
private:	// User declarations
  int count;
  AnsiString SamplePath;
  AnsiString SampleZipName;
  AnsiString SampleShortName;
  AnsiString SampleNames[4]; // fixed filenames
  AnsiString RandomName(void);
  AnsiString NoPrefix(AnsiString& Path); // remove versioning dir
  bool FileInZip(AnsiString &File);
  bool IsNodeChecked(TTreeNode* Node);
public:		// User declarations
  __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
