//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AddUser.h"
#include "StockData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmAddUser *frmAddUser;
//---------------------------------------------------------------------------
__fastcall TfrmAddUser::TfrmAddUser(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddUser::FormShow(TObject *Sender)
{
	Security::AuthenticationService auth;
	this->roles = auth.GetRoles();

	if (this->roles.empty())
	{
		tsOldSecurity->TabVisible = true;
		tsSecurity->TabVisible = false;
	}
	else
	{
		tsSecurity->TabVisible = true;
		tsOldSecurity->TabVisible = false;
	}

	if (!Transaction->InTransaction) Transaction->StartTransaction();
	dtUsers->Open();
	if (UserMode == umAdd)
	{
		this->PageControl1->ActivePageIndex = 0;

		dbtLoginID->Visible = false;
		dbeLoginID->Visible = true;
		dtUsers->Insert();
		dbeLoginID->SetFocus();
		Caption = "Add User";
		edPINConfirm->Text = "";
		edPasswordConfirm->Text = "";

		if (this->roles.empty())
		{
			this->PINChanged = true;
			this->passwordChanged = false;
		}
		else
		{
			this->PINChanged = false;
			this->passwordChanged = true;
		}

		this->userRoles = std::set<Security::Role>();
	}
	else if (UserMode == umEdit)
	{
		dbtLoginID->Visible = true;
		dbeLoginID->Visible = false;
		dtUsers->Locate("USER_ID", UserID, TLocateOptions());
		dtUsers->Edit();

		if (this->roles.empty())
		{
			this->PageControl1->ActivePageIndex = 1;
			this->userRoles = std::set<Security::Role>();
			dbePIN->SetFocus();
			edPINConfirm->Text = dtUsers->FieldByName("PIN")->AsString;
		}
		else
		{
			this->PageControl1->ActivePageIndex = 2;
			this->userRoles = auth.GetUserRoles(this->UserID);
			dbePassword->SetFocus();
			edPasswordConfirm->Text = dtUsers->FieldByName("PASSWRD")->AsString;
		}

		Caption = "Edit User";
		this->PINChanged = false;
		this->passwordChanged = false;
	}

	lbSecurityRoles->Items->Clear();
	for (std::set<Security::Role>::iterator i = roles.begin(); i!= roles.end(); i++)
	{
		int index = lbSecurityRoles->Items->AddObject(i->RoleName, (TObject*)&(*i));

		if (this->userRoles.find(*i) != this->userRoles.end())
			lbSecurityRoles->Checked[index] = true;
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmAddUser::btnCancelClick(TObject *Sender)
{
	try
	{
		dtUsers->Cancel();
		if (Transaction->InTransaction) Transaction->Rollback();
	}
	__finally
	{
		ModalResult = mrCancel;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddUser::btnOkClick(TObject *Sender)
{
	btnOk->SetFocus(); // Post current field
	if (dtUsers->FieldByName("USER_ID")->IsNull)
	{
		this->PageControl1->ActivePageIndex = 0;
		Application->ProcessMessages();
		Application->MessageBox("Please fill in all fields.", "Error", MB_ICONERROR + MB_OK);
		dbeLoginID->SetFocus();
		return;
	}
	if (dtUsers->FieldByName("FIRST_NAME")->IsNull)
	{
		this->PageControl1->ActivePageIndex = 0;
		Application->MessageBox("Please fill in all fields.", "Error", MB_ICONERROR + MB_OK);
		dbeFirstName->SetFocus();
		return;
	}
/*	else if (dtUsers->FieldByName("LAST_NAME")->IsNull)
	{
		Application->MessageBox("Please fill in all fields.", "Error", MB_ICONERROR + MB_OK);
		dbeLastName->SetFocus();
		return;
	}*/
	else if (dtUsers->FieldByName("PIN")->IsNull && this->roles.empty())
	{
		this->PageControl1->ActivePageIndex = 1;
		Application->MessageBox("Please fill in all fields.", "Error", MB_ICONERROR + MB_OK);
		dbePIN->SetFocus();
		return;
	}
	else if (dtUsers->FieldByName("PASSWRD")->IsNull && !this->roles.empty())
	{
		this->PageControl1->ActivePageIndex = 2;
		Application->MessageBox("Please fill in all fields.", "Error", MB_ICONERROR + MB_OK);
		dbePassword->SetFocus();
		return;
	}
	else if (dtUsers->FieldByName("NEXT_ORDER_NUMBER")->AsInteger <= 0)
	{
		this->PageControl1->ActivePageIndex = 0;
		Application->MessageBox("Please enter a number greater than 0.", "Error", MB_ICONERROR + MB_OK);
		dbeNextOrderNumber->SetFocus();
		return;
	}
	else
	{
		if (this->PINChanged)
		{
			if (edPINConfirm->Text != dtUsers->FieldByName("PIN")->AsString)
			{
				this->PageControl1->ActivePageIndex = 1;
				Application->MessageBox("PIN does not match!.", "Error", MB_ICONERROR + MB_OK);
				dtUsers->FieldByName("PIN")->AsString = "";
				edPINConfirm->Text = "";
				dbePIN->SetFocus();
				return;
			}
		}

		if (this->passwordChanged)
		{
			if (edPasswordConfirm->Text != dtUsers->FieldByName("PASSWRD")->AsString)
			{
				this->PageControl1->ActivePageIndex = 1;
				Application->MessageBox("Password does not match!.", "Error", MB_ICONERROR + MB_OK);
				dtUsers->FieldByName("PASSWRD")->AsString = "";
				edPasswordConfirm->Text = "";
				dbePassword->SetFocus();
				return;
			}
		}

		this->PINChanged = false;
		this->passwordChanged = false;

		if (roles.empty())
		{
				if (dtUsers->FieldByName("PASSWRD")->IsNull)
					dtUsers->FieldByName("PASSWRD")->AsString = dtUsers->FieldByName("PIN")->AsString;
		}
		else
		{
				if (dtUsers->FieldByName("PIN")->IsNull)
					dtUsers->FieldByName("PIN")->AsString = dtUsers->FieldByName("PASSWRD")->AsString;
		}

		if (dtUsers->FieldByName("LAST_NAME")->AsString == "")
		{
			dtUsers->FieldByName("USER_NAME")->AsString = dtUsers->FieldByName("FIRST_NAME")->AsString;
		}
		else
		{
			dtUsers->FieldByName("USER_NAME")->AsString = dtUsers->FieldByName("FIRST_NAME")->AsString + " " + dtUsers->FieldByName("LAST_NAME")->AsString;
		}
		try
		{
			dtUsers->Post();
			UserID = dtUsers->FieldByName("USER_ID")->AsString.UpperCase();

			std::set<Security::Role> newRoles;
			for (int index=0; index<lbSecurityRoles->Items->Count; index++)
			{
				if (lbSecurityRoles->Checked[index])
					newRoles.insert(*((Security::Role*)lbSecurityRoles->Items->Objects[index]));
			}

			if (Transaction->InTransaction) Transaction->Commit();

			try
			{
				Security::AuthenticationService auth;
				auth.SaveUserRoles(this->UserID, newRoles);
			}
			catch (Exception &E)
			{
				Application->MessageBox("Security information failed to be saved.", "Warning", MB_ICONWARNING + MB_OK);
			}
		}
		catch (Exception &E)
		{
			this->PageControl1->ActivePageIndex = 0;
			Application->MessageBox("The User Name must be unique.", "Error", MB_ICONERROR + MB_OK);
			dbeLoginID->SetFocus();
			return;
		}
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddUser::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	if (dtUsers->State == dsBrowse)
	{
		dtUsers->Cancel();
	}
	if (Transaction->InTransaction) Transaction->Rollback();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddUser::dbeKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN)
	{
		SelectNext((TWinControl *)Sender, true, true);
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddUser::dbePINChange(TObject *Sender)
{
	PINChanged = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddUser::dbePINKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN)
	{
		SelectNext((TWinControl *)Sender, true, true);
		Key = NULL;
	}
	if (!((Key >= '0' && Key <= '9') || Key == VK_BACK)) Key = NULL;
}
//---------------------------------------------------------------------------

