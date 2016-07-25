//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AddContact.h"
#include "StockData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmAddContact *frmAddContact;
//---------------------------------------------------------------------------
__fastcall TfrmAddContact::TfrmAddContact(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddContact::FormShow(TObject *Sender)
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();

	dtContact->Close();
	qrContactGroup->Close();
	qrContactGroup->Open();

	if (qrContactGroup->Eof)
	{
		sqlAddLK->Close();
		sqlAddLK->ExecQuery();

		qrAddContactGroup->Close();
		qrAddContactGroup->ParamByName("Contact_Group_Key")->AsInteger	= sqlAddLK->Fields[0]->AsInteger;
		qrAddContactGroup->ParamByName("Contact_Group")->AsString		= "All";
		qrAddContactGroup->ParamByName("Sort_Order")->AsInteger			= 0;
		qrAddContactGroup->ExecSQL();
		sqlAddLK->Close();

		qrContactGroup->Close();
		qrContactGroup->Open();
		if (qrContactGroup->Eof)
		{
			Close();
			return;
		}
	}

	if (Mode == amAddContact)
	{
		Caption = "Add Contact";
		dtContact->Open();
		dtContact->Insert();
		dtContact->FieldByName("Contact_Group_Key")->AsInteger = qrContactGroup->FieldByName("Contact_Group_Key")->AsInteger;
		dbeCompanyName->SetFocus();
	}
	else
	{
		Caption = "Edit Contact";
		dtContact->ParamByName("CONTACT_LK")->AsInteger = ContactKey;
		dtContact->Open();
		dtContact->Edit();
      dbeContact->SetFocus();
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddContact::btnOkClick(TObject *Sender)
{
    if (dbeWeborderTarget->Text.Length()
        && !validate_weborder_target_uri())
        return;

	btnOk->SetFocus(); // Post current editing field.
	if (dtContact->FieldByName("COMPANY_NAME")->AsString == "")
	{
		Application->MessageBox("You must enter a company name.", "Error", MB_ICONERROR + MB_OK);
		dbeCompanyName->SetFocus();
		return;
	}
	dtContact->Post();
	ContactKey = dtContact->FieldByName("CONTACT_LK")->AsInteger;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddContact::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	if (dtContact->State != dsBrowse)	dtContact->Cancel();
	if (Transaction->InTransaction)		Transaction->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddContact::btnCancelClick(TObject *Sender)
{
	try
	{
		dtContact->Cancel();
	}
	__finally
	{
		ModalResult = mrCancel;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddContact::dbeKeyPress(TObject *Sender,
		char &Key)
{
	if (Key == VK_RETURN)
	{
		SelectNext((TWinControl *)Sender, true, true);
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddContact::BitBtn1Click(TObject *Sender)
{
	dtContact->FieldByName("BILL_ADDRESS")->AsString = dtContact->FieldByName("LOCATION_ADDRESS")->AsString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddContact::BitBtn2Click(TObject *Sender)
{
	if (Application->MessageBox("This will update all Supplier IDs for previously entered invoices for export purposes. Continue?",
			"Continue?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
	{
		qrUpdateInvoiceSupplierID->Close();
		qrUpdateInvoiceSupplierID->ParamByName("SupplierKey")->AsInteger	= dtContact->FieldByName("CONTACT_LK")->AsInteger;
		qrUpdateInvoiceSupplierID->ParamByName("SupplierID")->AsString		= dtContact->FieldByName("Supplier_ID")->AsString;
		qrUpdateInvoiceSupplierID->ExecQuery();
	}
}

bool
TfrmAddContact::validate_weborder_target_uri()
{
    AnsiString uri = dbeWeborderTarget->Text;
    const char *hstr = uri.c_str();
    char *pend;
    const char *pstr;
    const char *strend = hstr + dbeWeborderTarget->Text.Length();
    unsigned short p;

    if (!(pstr = strchr(hstr, ':')) || pstr == hstr || pstr == strend) {
        Application->MessageBox(
          "Invalid URI; The URI you enter must follow the format host:port.",
          "Error", MB_ICONERROR | MB_OK);
        return false;
    }

    if (!(p = strtoul(++pstr, &pend, 10)) || pend != strend) {
        Application->MessageBox(
          "The URI you entered is invalid. Missing or incorrect port.",
          "Error", MB_ICONERROR | MB_OK);
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------

