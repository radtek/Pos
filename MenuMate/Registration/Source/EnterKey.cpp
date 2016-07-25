//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EnterKey.h"
#include "Registration.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmEnterKey *frmEnterKey;
//---------------------------------------------------------------------------
__fastcall TfrmEnterKey::TfrmEnterKey(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnterKey::FormShow(TObject *Sender)
{
	edRegKey->Text = KeyToFriendlyKey(CustomerKey, Index);//ProductID, Index);

	Edit1->SetFocus(); 
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnterKey::Edit1KeyPress(TObject *Sender, char &Key)
{
	TEdit *Edit = (TEdit *)Sender;
	if (Key >= 'a' && Key <= 'f')
	{
		Key = Key - 'a' + 'A';
	}
	else if (!((Key >= '0' && Key <= '9') || (Key >= 'A' && Key <= 'F') || Key == VK_BACK))
	{
		Key = NULL;
	}
	if (Edit->Text.Length() >= 6 && Edit->SelLength == 0 && Key != VK_BACK)
	{
		Key = NULL;
		SelectNext(Edit, true, true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnterKey::Edit1Change(TObject *Sender)
{
	TEdit *Edit = (TEdit *)Sender;
	if (Edit->Text.Length() >= 6)
	{
		Edit->Text = Edit->Text.SetLength(6);
		SelectNext(Edit, true, true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnterKey::Edit1KeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	TEdit *Edit = (TEdit *)Sender;
	if (Key == VK_RIGHT && Edit->SelStart == 6)
	{
		Edit2->AutoSelect = false;
		Edit2->SetFocus();
		Edit2->SelStart = 0;
		Edit2->SelLength = 0;
		Edit2->AutoSelect = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnterKey::Edit2KeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	TEdit *Edit = (TEdit *)Sender;
	if (Key == VK_LEFT && Edit->SelStart == 0)
	{
		Edit1->AutoSelect = false;
		Edit1->SetFocus();
		Edit1->SelStart = 6;
		Edit1->SelLength = 0;
		Edit1->AutoSelect = true;
	}
	if (Key == VK_RIGHT && Edit->SelStart == 6)
	{
		Edit3->AutoSelect = false;
		Edit3->SetFocus();
		Edit3->SelStart = 0;
		Edit3->SelLength = 0;
		Edit3->AutoSelect = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnterKey::Edit3KeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	TEdit *Edit = (TEdit *)Sender;
	if (Key == VK_LEFT && Edit->SelStart == 0)
	{
		Edit2->AutoSelect = false;
		Edit2->SetFocus();
		Edit2->SelStart = 6;
		Edit2->SelLength = 0;
		Edit2->AutoSelect = true;
	}
	if (Key == VK_RIGHT && Edit->SelStart == 6)
	{
		Edit4->AutoSelect = false;
		Edit4->SetFocus();
		Edit4->SelStart = 0;
		Edit4->SelLength = 0;
		Edit4->AutoSelect = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnterKey::Edit4KeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	TEdit *Edit = (TEdit *)Sender;
	if (Key == VK_LEFT && Edit->SelStart == 0)
	{
		Edit3->AutoSelect = false;
		Edit3->SetFocus();
		Edit3->SelStart = 6;
		Edit3->SelLength = 0;
		Edit3->AutoSelect = true;
	}
	if (Key == VK_RIGHT && Edit->SelStart == 6)
	{
		Edit5->AutoSelect = false;
		Edit5->SetFocus();
		Edit5->SelStart = 0;
		Edit5->SelLength = 0;
		Edit5->AutoSelect = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnterKey::Edit5KeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	TEdit *Edit = (TEdit *)Sender;
	if (Key == VK_LEFT && Edit->SelStart == 0)
	{
		Edit4->AutoSelect = false;
		Edit4->SetFocus();
		Edit4->SelStart = 6;
		Edit4->SelLength = 0;
		Edit4->AutoSelect = true;
	}
	if (Key == VK_RIGHT && Edit->SelStart == 6)
	{
		Edit6->AutoSelect = false;
		Edit6->SetFocus();
		Edit6->SelStart = 0;
		Edit6->SelLength = 0;
		Edit6->AutoSelect = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnterKey::Edit6KeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	TEdit *Edit = (TEdit *)Sender;
	if (Key == VK_LEFT && Edit->SelStart == 0)
	{
		Edit5->AutoSelect = false;
		Edit5->SetFocus();
		Edit5->SelStart = 6;
		Edit5->SelLength = 0;
		Edit5->AutoSelect = true;
	}
	if (Key == VK_RIGHT && Edit->SelStart == 6)
	{
		Edit7->AutoSelect = false;
		Edit7->SetFocus();
		Edit7->SelStart = 0;
		Edit7->SelLength = 0;
		Edit7->AutoSelect = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnterKey::Edit7KeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	TEdit *Edit = (TEdit *)Sender;
	if (Key == VK_LEFT && Edit->SelStart == 0)
	{
		Edit6->AutoSelect = false;
		Edit6->SetFocus();
		Edit6->SelStart = 6;
		Edit6->SelLength = 0;
		Edit6->AutoSelect = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnterKey::BitBtn1Click(TObject *Sender)
{
	RegistrationKey = "";
	bool KeyOK = true;
	for (int i=0; i<7; i++)
	{
		TEdit *Edit = (TEdit *)FindComponent("Edit" + IntToStr(i+1));
		if (Edit->Text.Length() < 6 || CheckSum(Edit->Text.SubString(1, 5)) != Edit->Text.SubString(6, 1))
		{
			MessageBox("The key you have entered is incorrect.", "Error", MB_ICONERROR);
			Edit->SetFocus();
			KeyOK = false;
			break;
		}
		else
		{
			RegistrationKey += Edit->Text.SubString(1, 5);
		}
	}
	if (KeyOK)
	{
		ModalResult = mrOk;
		RegistrationKey = RegistrationKey.SubString(1, 32);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmEnterKey::BitBtn3Click(TObject *Sender)
{
	Edit1->Text = "";
	Edit2->Text = "";
	Edit3->Text = "";
	Edit4->Text = "";
	Edit5->Text = "";
	Edit6->Text = "";
	Edit7->Text = "";
	Edit1->SetFocus();   
}
//---------------------------------------------------------------------------

