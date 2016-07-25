//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"

#include "MMCustomerDisplayManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    cd_manager = TMMCustomerDisplayManager::Instance();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    cd_manager->ShowOrderFromFile( "C:\\Code\\Git\\local menumate\\Second Display\\XMLOrder.xml" );
}
//---------------------------------------------------------------------------

<<<<<<< HEAD
=======
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    TMMCustomerDisplayManager::Instance()->SendOrderFromFile( "C:\\Code\\GIT\\remoteMenumate\\Second Display\\XMLOrder.xml" );
}
//---------------------------------------------------------------------------

>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2
