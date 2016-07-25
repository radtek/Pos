//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MMRegistry.h"
#include "SelectCompany.h"
#include "Consts.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TfrmSelectCompany *frmSelectCompany;
//---------------------------------------------------------------------------
__fastcall TfrmSelectCompany::TfrmSelectCompany(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectCompany::FormShow(TObject *Sender)
{
	FormResize(NULL);

	RegistryGetKeys(OfficeKey, cbCompanyNames->Items);
	if (cbCompanyNames->Items->Count > 0)
	{
		cbCompanyNames->ItemIndex = 0;
	}
	AnsiString DefaultCompany;
	RegistryRead(OfficeKey, "DefaultCompany", DefaultCompany);
	if (DefaultCompany != "")
	{
		for (int i=0; i<frmSelectCompany->cbCompanyNames->Items->Count; i++)
		{
			if (frmSelectCompany->cbCompanyNames->Items->Strings[i] == DefaultCompany)
			{
				frmSelectCompany->cbCompanyNames->ItemIndex = i;
				break;
			}
		}
	}
	cbCompanyNames->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectCompany::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectCompany::FormResize(TObject *Sender)
{
/*	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;

		ScaleBy(Screen->Width, Temp);
	}
	ClientHeight						= btnCancel->Top + btnCancel->Height + Panel1->Top;
	ClientWidth							= Panel1->Width + (Panel1->Left * 2);
	Left									= (Screen->Width - Width) / 2;
	Top									= (Screen->Height - Height) / 2;*/
}
//---------------------------------------------------------------------------

