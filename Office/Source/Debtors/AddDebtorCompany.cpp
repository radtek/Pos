//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddDebtorCompany.h"
#include "MMData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAddDebtorCompany *frmAddDebtorCompany;
//---------------------------------------------------------------------------
__fastcall TfrmAddDebtorCompany::TfrmAddDebtorCompany(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
int TfrmAddDebtorCompany::AddCompany()
{
	try
	{
		if (!Transaction->InTransaction) Transaction->StartTransaction();

		dtCompanyEdit->Close();
		Caption = "Add Company";

		dtCompanyEdit->Open();
		dtCompanyEdit->Append();

		return ShowModal();
	}
	catch (Exception &E)
	{
		if (Transaction->InTransaction) Transaction->Rollback();
		Application->ShowException(&E);
		return mrCancel;
	}
}
//---------------------------------------------------------------------------
int TfrmAddDebtorCompany::EditCompany(int iCompanyKey)
{
	try
	{
		if (!Transaction->InTransaction) Transaction->StartTransaction();

		Caption = "Edit Company";

		CompanyKey = iCompanyKey;

		dtCompanyEdit->Close();
		dtCompanyEdit->ParamByName("Company_Key")->AsInteger = CompanyKey;
		dtCompanyEdit->Open();
		dtCompanyEdit->Edit();

		return ShowModal();
	}
	catch (Exception &E)
	{
		if (Transaction->InTransaction) Transaction->Rollback();
		Application->ShowException(&E);
		return mrCancel;
	}
}
//---------------------------------------------------------------------------
int __fastcall TfrmAddDebtorCompany::ShowModal()
{
	return TForm::ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDebtorCompany::FormShow(TObject *Sender)
{
	FormResize(NULL);
	dbeCompanyName->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDebtorCompany::FormResize(TObject *Sender)
{
	ClientHeight						= btnCancel->Top + btnCancel->Height + GroupBox2->Left;
	ClientWidth							= GroupBox2->Width + (GroupBox2->Left * 2);
	Left									= (Screen->Width - Width) / 2;
	Top									= (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDebtorCompany::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	try
	{
		if (Transaction->InTransaction) Transaction->Rollback();
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDebtorCompany::btnOkClick(TObject *Sender)
{
	btnOk->SetFocus(); // Post current editing field.
	if (dtCompanyEdit->FieldByName("Company_Name")->AsString == "")
	{
		Application->MessageBox("You must enter a unique name.", "Error", MB_ICONERROR + MB_OK);
		dbeCompanyName->SetFocus();
		return;
	}
	dtCompanyEdit->Post();

	CompanyKey = dtCompanyEdit->FieldByName("Company_Key")->AsInteger;

	Transaction->Commit();
}
//---------------------------------------------------------------------------
