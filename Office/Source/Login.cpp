//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <memory>
#include "Login.h"
#include "StockData.h"
#include "AddUser.h"
#include "MMRegistry.h"
#include "Consts.h"
#include "Utilities.h"
#include "AuthenticationService.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
__fastcall TfrmLogin::TfrmLogin(TComponent* Owner)
	: TForm(Owner)
{
	this->useOldLogin = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::FormCreate(TObject *Sender)
{
//	edUserName->Text = "";
	fCurrentUserID = "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::FormShow(TObject *Sender)
{
	Security::AuthenticationService auth;

	this->useOldLogin = auth.GetRoles().empty() || LoginMode == lmLoginAdmin;

	if (this->useOldLogin)
		this->InitialiseOldLogin();
	else
		this->InitialiseNewLogin();
}
//---------------------------------------------------------------------------
void TfrmLogin::InitialiseOldLogin()
{
	pnlOldLogin->Visible = true;
	pnlNewLogin->Visible = false;

	cbUserName->Items->Clear();
	if (LoginMode == lmLoginAdmin)
	{
		lbeInstructions->Caption = "Please enter the PIN to enter the administration system.";
		cbUserName->Style = csDropDown;
		cbUserName->Text = "Administrator";
		cbUserName->Enabled = false;
		edPIN->SetFocus();
	}
	else
	{
		cbUserName->Style = csDropDownList;
		lbeInstructions->Caption = "Please enter your user name and PIN to enter the stock system.";
		if (dmStockData->dbStock->Connected)
		{
			if (!dmStockData->trStock->InTransaction)
			{
				dmStockData->trStock->StartTransaction();
			}
			qrUsers->Close();
			qrUsers->Open();
			if (qrUsers->Eof)
			{
				qrUsers->Close();
				std::auto_ptr<TfrmAddUser>frmAddUser(new TfrmAddUser(NULL));
				frmAddUser->UserMode = umAdd;
				if (frmAddUser->ShowModal() != mrOk)
				{
					ModalResult = mrCancel;
				}
			}
			for (qrUsers->Open(); !qrUsers->Eof; qrUsers->Next())
			{
				AnsiString UserID = qrUsers->FieldByName("User_ID")->AsString.LowerCase();
				if (UserID.Length() > 0)
				{
					UserID[1] = toupper(UserID[1]);
					for (int i=1; i<UserID.Length(); i++)
					{
						if (UserID[i] == ' ')
						{
							UserID[i+1] = toupper(UserID[i+1]);
						}
					}
					cbUserName->Items->Add(UserID);
				}
			}
			qrUsers->Close();
		}

		AnsiString LastUser = "";
		RegistryRead(OfficeKey, "LastUser", LastUser);
		if (LastUser == "")
		{
			cbUserName->SetFocus();
		}
		else
		{
			cbUserName->ItemIndex = cbUserName->Items->IndexOf(LastUser);
			if (cbUserName->ItemIndex == -1 && cbUserName->Items->Count > 0)
			{
				cbUserName->ItemIndex = 0;
			}
			edPIN->SetFocus();
		}
		cbUserName->Enabled = true;
	}
	edPIN->Text = "";
#ifdef DEVELOPER
	Timer1->Enabled = true;
#endif
}
//---------------------------------------------------------------------------
void TfrmLogin::InitialiseNewLogin()
{
	pnlNewLogin->Visible = true;
	pnlOldLogin->Visible = false;
	lbeInstructions->Caption = "Please enter your user name and password.";
	edUserName->Text = "";
	edPassword->Text = "";
	edUserName->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (dmStockData->trStock->InTransaction)
	{
		dmStockData->trStock->Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::edPINKeyPress(TObject *Sender, char &Key)
{
	if (!((Key >= '0' && Key <= '9') || Key == VK_BACK || Key == VK_DELETE)) Key = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::btnOkClick(TObject *Sender)
{
	if (LoginMode == lmLoginAdmin)
	{
		if (edPIN->Text == "1202")
		{                        
			ModalResult = mrOk;
		}
		else
		{
			ModalResult = mrCancel;
		}
	}
	else
	{
		if (this->useOldLogin)
		{
			this->UseOldLogin();
		}
		else
		{
			this->UseNewLogin();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmLogin::UseOldLogin()
{
	if (dmStockData->dbStock->Connected)
	{
		qrLogin->Close();
		qrLogin->ParamByName("UserID")->AsString = cbUserName->Text;
		qrLogin->ParamByName("PIN")->AsString = edPIN->Text;
		qrLogin->Open();
		if (!qrLogin->IsEmpty())
		{
			RegistryWrite(OfficeKey, "LastUser", cbUserName->Text);
			if (LoginMode == lmLogin)
			{
				fCurrentUserID = qrLogin->FieldByName("USER_ID")->AsString;
				fCurrentUserName = qrLogin->FieldByName("User_Name")->AsString;
				GetUserInfo(CurrentUser);
			}
			else if (LoginMode == lmTestAccess)
			{
				fLastAccessUserID = qrLogin->FieldByName("USER_ID")->AsString;
			}
			ModalResult = mrOk;
		}
		else
		{
			Application->MessageBox("The username or password does not match.", "Error", MB_ICONERROR + MB_OK);
			edPIN->Text = "";
			edPIN->SetFocus();
		}
		qrLogin->Close();
	}
	else
	{
		Application->MessageBox("Database not found.", "Error", MB_ICONERROR + MB_OK);
		ModalResult = mrCancel;
	}
}
//---------------------------------------------------------------------------
void TfrmLogin::UseNewLogin()
{
	Security::AuthenticationService auth;
	Security::Identity id;

	if (auth.Authenticate(edUserName->Text, edPassword->Text, id))
	{
		//RegistryWrite(OfficeKey, "LastUser", edPassword->Text);
		if (LoginMode == lmLogin)
		{
			fCurrentUserID = id.UserID;
			fCurrentUserName = id.UserName;
			GetUserInfo(CurrentUser);
		}
		else if (LoginMode == lmTestAccess)
		{
			fLastAccessUserID = id.UserID;
		}

		ModalResult = mrOk;
	}
	else
	{
		Application->MessageBox("The username or password is incorrect.", "Error", MB_ICONERROR + MB_OK);
		edUserName->SetFocus();
	}
}
//---------------------------------------------------------------------------
void TfrmLogin::LogoutCurrentUser()
{
	fCurrentUserID = "";
}
//---------------------------------------------------------------------------
void TfrmLogin::LogoutAccessUser()
{
	fLastAccessUserID = "";
}
//---------------------------------------------------------------------------
bool TfrmLogin::GetUserInfo(TLoginInfo &UserInfo)
{
   // bug here didnt change User ID's if they were already logged on, fix in 3.3.1.16
	//if (UserInfo.UserID == "") UserInfo.UserID = CurrentUserID;
	qrInfo->Close();
	qrInfo->ParamByName("UserID")->AsString = CurrentUserID;
	//qrInfo->ParamByName("UserID")->AsString = UserInfo.UserID;
	qrInfo->Open();
	if (!qrInfo->IsEmpty())
	{
		UserInfo.UserID		= qrInfo->FieldByName("USER_ID")->AsString;
		UserInfo.UserName		= qrInfo->FieldByName("USER_NAME")->AsString;
		UserInfo.FirstName	= qrInfo->FieldByName("FIRST_NAME")->AsString;
		UserInfo.MiddleName	= qrInfo->FieldByName("MIDDLE_NAME")->AsString;
		UserInfo.LastName		= qrInfo->FieldByName("LAST_NAME")->AsString;
		UserInfo.Initials		= qrInfo->FieldByName("INITIALS")->AsString;
		UserInfo.Address		= qrInfo->FieldByName("ADDRESS")->AsString;
		UserInfo.Phone			= qrInfo->FieldByName("PHONE")->AsString;
		UserInfo.EMail			= qrInfo->FieldByName("EMAIL")->AsString;
//		Access;
		UserInfo.SwipeCard	= qrInfo->FieldByName("SWIPE_CARD")->AsString;
//		NextOrderNumber;
		qrInfo->Close();
		return true;
	}
	else
	{
		qrInfo->Close();
		return false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::cbUserNameCloseUp(TObject *Sender)
{
	edPIN->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::Timer1Timer(TObject *Sender)
{
	Timer1->Enabled = false;
	AnsiString ComputerName = dmUtilities->GetPCName();
	AnsiString ComputerIP   = dmUtilities->GetComputerIP(ComputerName);
	if ( ComputerName.AnsiPos("QWERTY") != 0 ) //&& ComputerIP == "192.168.0.16")
	{
		edPIN->Text = "1";
		btnOkClick(btnOk);
	}
}
//---------------------------------------------------------------------------

