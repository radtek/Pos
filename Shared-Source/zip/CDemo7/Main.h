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
class TForm1 : public TForm
{
__published:	// IDE-managed Components
  TImage *Image1;
  TStaticText *StaticText1;
  TButton *btnPic1;
  TButton *btnPic2;
  TButton *btnPic3;
  TZipBuilder *ZipBuilder1;
  void __fastcall btnPic1Click(TObject *Sender);
  void __fastcall btnPic2Click(TObject *Sender);
  void __fastcall btnPic3Click(TObject *Sender);
private:	// User declarations
  void __fastcall UnzipToImage(AnsiString FileName);
public:		// User declarations
  __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

