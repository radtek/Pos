//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditAccountCustomerForm.h"
#include <memory>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmEditAccountCustomer::TfrmEditAccountCustomer(TComponent* Owner)
	: TForm(Owner)
{
	PostMessage(edAccountName->Handle, EM_SETSEL, 0, -1);
}
//---------------------------------------------------------------------------
bool TfrmEditAccountCustomer::Execute(const AnsiString& Name, AnsiString& AccountName, AnsiString& AccountID)
{
	std::auto_ptr<TfrmEditAccountCustomer> form =
		std::auto_ptr<TfrmEditAccountCustomer>(new TfrmEditAccountCustomer(NULL));

	form->lbeName->Caption = Name;
	form->edAccountName->Text = AccountName;
	form->edAccountID->Text = AccountID;

	if (form->ShowModal() == mrOk)
	{
		AccountName = form->edAccountName->Text;
		AccountID = form->edAccountID->Text;
		return true;
	}
	else return false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditAccountCustomer::btnExitMouseClick(TObject *Sender)
{
	if (edAccountName->Text == "" || edAccountID->Text == "")
	{
		::MessageBox(NULL, "Please enter data into both fields.", "Error", MB_ICONERROR | MB_OK);
	}
	else
	{
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditAccountCustomer::btnExportMouseClick(
      TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

