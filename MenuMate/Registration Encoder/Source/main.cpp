//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdlib.h>
#include "main.h"
#include "Password.h"
#include "MD5Checksum.h"
#include <SysUtils.hpp>
#include "Registration.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	Edit1->Text = "";
	Edit2->Text = "";
	Edit3->Text = "";
	Edit4->Text = "";
	Edit5->Text = "";
	Edit6->Text = "";
	Edit7->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	AnsiString Password = "";
	if (frmPassword->InputQuery(Password))
	{
		CMD5Checksum CheckSum;
		Password = CheckSum.GetMD5(Password);
		if (Password != "7e1ef024f9248b6195b78c9e15b5167f")
		{
			PostMessage(Handle, WM_CLOSE, 0, 0);
			return;
		}
		pcLicences->ActivePage = tsSingleLicence;
		edPOSLicences->SetFocus();
	}
	else
	{
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
	dtpExpiry->Date = Date() + 40;
	dtpExpiry->MaxDate = Date() + 90;		
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::BitBtn1Click(TObject *Sender)
{
	EncodeKey();
}
//---------------------------------------------------------------------------
void TfrmMain::EncodeKey()
{
	if (edPOSLicences->Text != "" && edPalmLicences->Text != "" && edOfficeLicences->Text != "")
	{
		edRegKey->Text				= "";

		AnsiString CustomerKey	= "";
		AnsiString RegKey			= "";
		int DBIndex					= 0;
		bool KeyOK					= true;
		for (int i=0; i<7; i++)
		{
			TEdit *Edit = (TEdit *)FindComponent("Edit" + IntToStr(i+1));
			if (Edit->Text.Length() < 6 || CheckSum(Edit->Text.SubString(1, 5)) != Edit->Text.SubString(6, 1))
			{
				Application->MessageBox("The key you have entered is incorrect.", "Error", MB_ICONERROR);
				Edit->SetFocus();
				KeyOK = false;
				break;
			}
			else
			{
				CustomerKey += Edit->Text.SubString(1, 5);
			}
		}
		if (KeyOK)
		{
			int Result;
//			HexToBin(CustomerKey.SubString(33, 3).LowerCase().c_str(), (char *)&DBIndex, 1);
			HexToBin((AnsiString("0") + CustomerKey.SubString(33, 1)).LowerCase().c_str(), (char *)&DBIndex+1, 1);
			HexToBin(CustomerKey.SubString(34, 2).LowerCase().c_str(), (char *)&DBIndex, 1);
			CustomerKey			= CustomerKey.SubString(1, 32).LowerCase();

         int Modules = 0;
			if(chbKitchenModule->Checked) Modules |= eRegKitchenScreen;
         if(cbRooms->Checked) Modules |= eRegRooms;
			if(cbTurnAroundTimes->Checked) Modules |= eRegSaleTurnAround;
         if(cbMembership->Checked) Modules |= eRegMembers;
			if(cbMYOB->Checked) Modules |= eOfficeMYOB;
			if(cbMenuMateDirect->Checked) Modules |= eMenuMateDirect;
			if(cbIntergratedEftPos->Checked) Modules |= eEFTPOS;
			if(chPhoenixSystem->Checked) Modules |= ePhoenixHotelSystem;
			if(cbExpires->Checked) Modules |= eExpires;
			AnsiString RegMods = IntToHex(Modules,4);

			CMD5Checksum CheckSum;
			if(cbExpires->Checked)
			{
				RegKey = CheckSum.GetMD5(	CustomerKey +
													IntToHex(StrToInt(edPOSLicences->Text), 4) +
													IntToHex(StrToInt(edPalmLicences->Text), 4) +
													(RegMods) +                                        // MenuMate Modules
													IntToHex(int(dtpExpiry->Date),4) +						// Expiry Date
													IntToHex(StrToInt(edOfficeLicences->Text), 4) +		// Office Modules
													AnsiString::StringOfChar('0', 32)).UpperCase();		// Future proof
			}
			else
			{
				RegKey = CheckSum.GetMD5(	CustomerKey +
													IntToHex(StrToInt(edPOSLicences->Text), 4) +
													IntToHex(StrToInt(edPalmLicences->Text), 4) +
													(RegMods) +			                                 // MenuMate Modules
													IntToHex(StrToInt(edOfficeLicences->Text), 4) +		// Office Modules
													AnsiString::StringOfChar('0', 32)).UpperCase();		// Future proof
			}
			edRegKey->Text	= KeyToFriendlyKey(RegKey, DBIndex);
			if (chbAddToMulti->Checked)
			{
				memCustomerKeys->Lines->Add(RegKey + IntToHex(DBIndex, 3));
				pcLicences->ActivePage = tsMultipleLicences;
			}
		}
	}
	else
	{
		Application->MessageBox("Please enter the number of licences.", "Error", MB_ICONERROR);
		edPOSLicences->SetFocus();
	}
}
//---------------------------------------------------------------------------
void TfrmMain::EncodeKeys()
{
	if (edPOSLicences->Text != "" && edPalmLicences->Text != "" && edOfficeLicences->Text != "")
	{
		memRegistrationKeys->Lines->Clear();

		AnsiString CustomerKey	= "";
		AnsiString RegKey			= "";
		int DBIndex					= 0;

		for (int i=0; i<memCustomerKeys->Lines->Count; i++)
		{
			CustomerKey			= memCustomerKeys->Lines->Strings[i];
			HexToBin((AnsiString("0") + CustomerKey.SubString(33, 1)).LowerCase().c_str(), (char *)&DBIndex+1, 1);
			HexToBin(CustomerKey.SubString(34, 2).LowerCase().c_str(), (char *)&DBIndex, 1);
			CustomerKey			= CustomerKey.SubString(1, 32).LowerCase();

         int Modules = 0;
			if(chbKitchenModule->Checked) Modules |= eRegKitchenScreen;
         if(cbRooms->Checked) Modules |= eRegRooms;
         if(cbTurnAroundTimes->Checked) Modules |= eRegSaleTurnAround;
         if(cbMembership->Checked) Modules |= eRegMembers;
			if(cbMYOB->Checked) Modules |= eOfficeMYOB;
			if(cbMenuMateDirect->Checked) Modules |= eMenuMateDirect;
			if(cbIntergratedEftPos->Checked) Modules |= eEFTPOS;
			if(chPhoenixSystem->Checked) Modules |= ePhoenixHotelSystem;			
			if(cbExpires->Checked) Modules |= eExpires;
                  
			AnsiString RegMods = IntToHex(Modules,4);
			CMD5Checksum CheckSum;
			AnsiString EncodedDate = dtpExpiry->Date.FormatString("dd-mm-yyyy");

			if(cbExpires->Checked)
			{
				RegKey = CheckSum.GetMD5(	CustomerKey +
													IntToHex(StrToInt(edPOSLicences->Text), 4) +
													IntToHex(StrToInt(edPalmLicences->Text), 4) +
													(RegMods) +                                        // MenuMate Modules
													EncodedDate +						// Expiry Date
													IntToHex(StrToInt(edOfficeLicences->Text), 4) +		// Office Modules
													AnsiString::StringOfChar('0', 32)).UpperCase();		// Future proof
			}
			else
			{
				RegKey = CheckSum.GetMD5(	CustomerKey +
													IntToHex(StrToInt(edPOSLicences->Text), 4) +
													IntToHex(StrToInt(edPalmLicences->Text), 4) +
													(RegMods) +			                                 // MenuMate Modules
													IntToHex(StrToInt(edOfficeLicences->Text), 4) +		// Office Modules
													AnsiString::StringOfChar('0', 32)).UpperCase();		// Future proof
			}
			memRegistrationKeys->Lines->Add(RegKey + IntToHex(DBIndex, 3));
		}
	}
	else
	{
		Application->MessageBox("Please enter the number of licences.", "Error", MB_ICONERROR);
		edPOSLicences->SetFocus();
	}
}
//---------------------------------------------------------------------------
AnsiString TfrmMain::KeyToFriendlyKey(AnsiString Key, int Index)
{
	AnsiString TempKey = Key.UpperCase();
	AnsiString SubSection;
	AnsiString NewKey;

	for (int i=1; i<=30; i+= 5)
	{
		SubSection = TempKey.SubString(i, 5);
		NewKey += SubSection + CheckSum(SubSection) + " - ";
	}
	SubSection = TempKey.SubString(31, 2);
	SubSection += IntToHex(Index, 3);//2);
	NewKey += SubSection + CheckSum(SubSection);
	return NewKey;
}
//---------------------------------------------------------------------------
AnsiString TfrmMain::CheckSum(AnsiString Section)
{
	int Check = 0;
	for (int i=0; i<5; i++)
	{
		int Hex = 0;
		if (Section[i+1] >= '0' && Section[i+1] <= '9')
		{
			Hex = Section[i+1] - '0';
		}
		else if (Section[i+1] >= 'A' && Section[i+1] <= 'F')
		{
			Hex = Section[i+1] - 'A' + 10;
		}
		else if (Section[i+1] >= 'a' && Section[i+1] <= 'f')
		{
			Hex = Section[i+1] - 'a' + 10;
		}
		Check += Hex;
		if ((i + 1) % 2 == 0)
		{
			Check += Hex;
		}
	}
	Check /= 7;
	return IntToHex(Check, 1);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::memCustomerKeysChange(TObject *Sender)
{
	EncodeKeys();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::edPOSLicencesChange(TObject *Sender)
{
	if (edPOSLicences->Text != "" && edPalmLicences->Text != "" && edOfficeLicences->Text != "")
	{
		if (pcLicences->ActivePage == tsMultipleLicences)
		{
			EncodeKeys();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::Edit1Change(TObject *Sender)
{
	TEdit *Edit = (TEdit *)Sender;
	if (Edit->Text.Length() >= 6)
	{
		Edit->Text = Edit->Text.SetLength(6);
		SelectNext(Edit, true, true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::Edit1KeyPress(TObject *Sender, char &Key)
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
void __fastcall TfrmMain::Edit1KeyDown(TObject *Sender, WORD &Key,
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
void __fastcall TfrmMain::Edit2KeyDown(TObject *Sender, WORD &Key,
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
void __fastcall TfrmMain::Edit3KeyDown(TObject *Sender, WORD &Key,
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
void __fastcall TfrmMain::Edit4KeyDown(TObject *Sender, WORD &Key,
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
void __fastcall TfrmMain::Edit5KeyDown(TObject *Sender, WORD &Key,
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
void __fastcall TfrmMain::Edit6KeyDown(TObject *Sender, WORD &Key,
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
void __fastcall TfrmMain::Edit7KeyDown(TObject *Sender, WORD &Key,
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
void __fastcall TfrmMain::pcLicencesChange(TObject *Sender)
{
	Panel2->Parent = pcLicences->ActivePage;
	Panel2->TabOrder = 0;
	edPOSLicences->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnClearClick(TObject *Sender)
{
	Edit1->Clear();
	Edit2->Clear();
	Edit3->Clear();
	Edit4->Clear();
	Edit5->Clear();
	Edit6->Clear();
	Edit7->Clear();
	edRegKey->Clear();
}
//---------------------------------------------------------------------------

