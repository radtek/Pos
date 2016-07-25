//---------------------------------------------------------------------------
#pragma hdrstop

#include "ZForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TForm * TZForm::WinOwner = NULL;
//---------------------------------------------------------------------------
__fastcall TZForm::TZForm(TComponent* Owner) : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TZForm::CreateParams(Controls::TCreateParams &params)
{ 
	TForm::CreateParams( params );
	if( WinOwner )
	{
		params.WndParent = WinOwner->Handle;
		WinOwner = NULL;
	}
} 
//---------------------------------------------------------------------------
