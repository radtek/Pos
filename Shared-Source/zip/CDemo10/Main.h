//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ZipBuilder.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
  TZipBuilder *ZipBuilder;
  TPanel *Panel1;
  TMemo *Memo1;
  TButton *AddBtn;
  TButton *ExtBtn;
  TButton *ContentBtn;
  void __fastcall AddBtnClick(TObject *Sender);
  void __fastcall ExtBtnClick(TObject *Sender);
  void __fastcall ZipBuilderSetAddName(TObject *Sender,
          AnsiString &FileName, const AnsiString ExtName, bool &IsChanged);
  void __fastcall ZipBuilderSetExtName(TObject *Sender,
          AnsiString &OldFileName, bool &IsChanged);
  void __fastcall ZipBuilderExtractOverwrite(TObject *Sender,
          AnsiString ForFile, bool IsOlder, bool &DoOverwrite,
          int DirIndex);
  void __fastcall ContentBtnClick(TObject *Sender);
private:	// User declarations
  AnsiString SampleZipName;
public:		// User declarations
  __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif

