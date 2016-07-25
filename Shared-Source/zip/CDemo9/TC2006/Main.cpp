//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "ZThrd.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
  // because TurboC++ builds the application in a dir one lower than the files
	// we need to adjust the current dir
	SetCurrentDir("../");
	Fzip = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject* /*Sender*/)
{
  if(Fzip) Fzip->Terminate();
  else Memo1->Lines->Add("***** Not Running ****");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject* /*Sender*/)
{
  if(!Fzip)
  {
    TStringList* s1 = new TStringList();
    try
		{
			s1->Add("*.cpp");
			s1->Add("*.dfm");
			s1->Add("*.h");
			s1->Add("*.bdsproj*");

			Fzip = new TZipThread("test.zip", s1, Memo1, false);
    }
    __finally
    {
      delete s1;
    }
  }
}
//---------------------------------------------------------------------------

