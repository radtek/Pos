//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectDebtor.h"
#include "MMData.h"
#include "Debtors.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSelectDebtor *frmSelectDebtor;
//---------------------------------------------------------------------------
__fastcall TfrmSelectDebtor::TfrmSelectDebtor(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDebtor::FormShow(TObject *Sender)
{
	FormResize(NULL);
	if (Reset)
	{
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		Reset = false;
		dbgResult->Visible = false;
		ClientHeight = btnCancel->Top + btnCancel->Height + Panel1->Top;
		Top = (Screen->Height - Height) / 2;
		edSearch->Text = "";
		edSearch->SetFocus();
	}
	else
	{
		edSearch->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDebtor::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	if (Transaction->InTransaction) Transaction->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDebtor::FormResize(TObject *Sender)
{
	if (qrDebtor->Active && !qrDebtor->IsEmpty() && dbgResult->Visible)
	{
		ClientHeight					= btnOk->Top + btnOk->Height + Panel1->Top;
	}
	else
	{
		ClientHeight					= btnCancel->Top + btnCancel->Height + Panel1->Top;
	}
	ClientWidth							= btnCancel->Left + btnCancel->Width + Panel1->Left;
	Left									= (Screen->Width - Width) / 2;
	Top									= (Screen->Height - Height) / 2;
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDebtor::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void TfrmSelectDebtor::ResizeGrids()
{
	dbgResult->TitleFont->Size				= dbgResult->Font->Size;
	dbgResult->Columns->Items[0]->Width	= dbgResult->ClientWidth;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDebtor::btnGoClick(TObject *Sender)
{
	bool Result = frmDebtors->HasMembershipInstalled(false);
	qrDebtor->Close();
	qrDebtor->ParamByName("Name")->AsString = edSearch->Text.UpperCase();
	if (Result)
	{
		// has external MemberShip installed - different Contact_Types
		qrDebtor->ParamByName("ContactType")->AsInteger = 4;
	}
	else
	{
		qrDebtor->ParamByName("ContactType")->AsInteger = 2;
	}
	qrDebtor->Open();
	if (!qrDebtor->IsEmpty())
	{
		qrDebtor->MoveBy(2);
		if (qrDebtor->RecordCount == 1)
		{
			DebtorName	= qrDebtor->FieldByName("Name")->AsString;
			DebtorKey	= qrDebtor->FieldByName("Contacts_Key")->AsInteger;
			qrDebtor->Close();
			ModalResult = mrOk;
		}
		else
		{
			qrDebtor->First();
			dbgResult->Visible = true;
			edSearch->Text = "";
			dbgResult->SetFocus();
			ClientHeight = btnOk->Top + btnOk->Height + Panel1->Top;
			Top = (Screen->Height - Height) / 2;
		}
	}
	else
	{
		qrDebtor->Close();
		qrDebtor->ParamByName("Name")->AsString = "%" + edSearch->Text.UpperCase() + "%";
		qrDebtor->Open();
		if (qrDebtor->IsEmpty())
		{
			Application->MessageBox("No records found. Please try again.", "Error", MB_ICONERROR + MB_OK);
			edSearch->Text = "";
			edSearch->SetFocus();
			dbgResult->Visible = false;
			ClientHeight = btnCancel->Top + btnCancel->Height + Panel1->Top;
			Top = (Screen->Height - Height) / 2;
		}
		else
		{
			qrDebtor->Next();
			qrDebtor->First();
			if (qrDebtor->RecordCount > 1)
			{
				dbgResult->Visible = true;
				edSearch->Text = "";
				dbgResult->SetFocus();
				ClientHeight = btnOk->Top + btnOk->Height + Panel1->Top;
				Top = (Screen->Height - Height) / 2;
			}
			else
			{
				DebtorName	= qrDebtor->FieldByName("Name")->AsString;
				DebtorKey	= qrDebtor->FieldByName("Contacts_Key")->AsInteger;
				qrDebtor->Close();
				ModalResult = mrOk;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDebtor::dbgResultDblClick(TObject *Sender)
{
	if (qrDebtor->IsEmpty())
	{
		Application->MessageBox("No records found. Please try again.", "Error", MB_ICONERROR + MB_OK);
		edSearch->Text = "";
		edSearch->SetFocus();
	}
	else
	{
		DebtorName	= qrDebtor->FieldByName("Name")->AsString;
		DebtorKey	= qrDebtor->FieldByName("Contacts_Key")->AsInteger;
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDebtor::dbgResultKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		dbgResultDblClick(Sender);
	}
	else if (isprint(Key) || Key == VK_BACK)
	{
		PostMessage(edSearch->Handle, WM_CHAR, Key, 0);
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDebtor::dbgResultKeyUp(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if (Key == VK_UP)
	{
		if (qrDebtor->Bof)
		{
			Key = NULL;
			edSearch->SetFocus();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDebtor::edSearchKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		btnGoClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDebtor::btnFindClick(TObject *Sender)
{
	bool Result = frmDebtors->HasMembershipInstalled(true);
	if (Result)
	{
		return;
	}
	frmDebtors->DebtorsMode = mmSelect;
	if (frmDebtors->ShowModal() == mrOk)
	{
		DebtorName	= frmDebtors->DebtorName;
		DebtorKey	= frmDebtors->DebtorKey;
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDebtor::CMDialogChar(TWMKey &Msg)
{
	if (!(HIWORD(Msg.KeyData) & KF_ALTDOWN))
	{
		SendMessage(ActiveControl->Handle, WM_CHAR, Msg.CharCode, 0);
		Msg.Result = 1;
	}
	TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectDebtor::edSearchChange(TObject *Sender)
{
	if (qrDebtor->Active)
	{
		qrDebtor->Locate("Name", edSearch->Text, TLocateOptions() << loPartialKey << loCaseInsensitive);
	}
}
//---------------------------------------------------------------------------

