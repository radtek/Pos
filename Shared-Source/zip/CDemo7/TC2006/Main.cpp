//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// progam link changed for TurboC++ 2006 Explorer
#pragma link "ZBldr179C10.bpi"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
// code added for static linking in TurboC++ 2006 Explorer
	ZipBuilder1 = new TZipBuilder(this);
	// because TurboC++ builds the application in a dir one lower than the files
	// we need to adjust the current dir
	SetCurrentDir("../");
// end added code
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnPic1Click(TObject* /*Sender*/)
{
  UnzipToImage("paintbrush.bmp");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnPic2Click(TObject* /*Sender*/)
{
  UnzipToImage("Spectrum.bmp");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnPic3Click(TObject* /*Sender*/)
{
  UnzipToImage("eye.bmp");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UnzipToImage(AnsiString FileName)
{
  if(!ZipBuilder1->Busy)
  {
    ZipBuilder1->ZipFileName = "resource.zip";
    ZipBuilder1->Password = "password";
    TZipStream* Stream1 = ZipBuilder1->ExtractFileToStream(FileName);
    Stream1->Position = 0;      // reset to the beginning of the stream
    StaticText1->Caption = "File Size: "+ IntToStr(Stream1->Size);
    Image1->Picture->Bitmap->LoadFromStream(Stream1);
  }
}
