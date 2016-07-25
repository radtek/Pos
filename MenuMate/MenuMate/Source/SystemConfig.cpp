//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SystemConfig.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "MMRegistry.h"
#include "MMMessageBox.h"
#include "Enum.h"
#include "GlobalSettings.h"

#ifdef  PalmMate
	#include "Palm.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

TForm * TfrmSystemConfig::WinOwner = NULL;
//---------------------------------------------------------------------------
__fastcall TfrmSystemConfig::TfrmSystemConfig(TComponent* Owner)
	: TForm(Owner)
{
   RequestClose = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemConfig::CreateParams(Controls::TCreateParams &params)
{
	TForm::CreateParams( params );
	if( WinOwner )
	{
		params.WndParent = WinOwner->Handle;
		WinOwner = NULL;
	}
}
//---------------------------------------------------------------------------
TfrmSystemConfig *TfrmSystemConfig::Create(TForm* Owner)
{
	WinOwner = Owner;
	return new TfrmSystemConfig(Owner);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemConfig::btnExit(TObject *Sender)
{
   RequestClose = true;
   ModalResult = mrAbort;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemConfig::tbOkClick(TObject *Sender)
{
   RegistryWrite(MMBaseKey "Database", "Reconfigure", false);
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemConfig::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
                if((double)Screen->Width / Screen->Height < 1.4)
                {
			ScaleBy(Screen->Width, Temp);
                }
	}
//   ClientWidth = pButtons->Width;
//   ClientHeight = pButtons->Height;
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;

}
//---------------------------------------------------------------------------

void __fastcall TfrmSystemConfig::FormShow(TObject *Sender)
{
	FormResize(Sender);
	tgridButtons->Buttons[0][0]->Caption = "Interbase Server IP Address\r" + TGlobalSettings::Instance().InterbaseIP;
	tgridButtons->Buttons[1][0]->Caption = "Database Location\r" + TGlobalSettings::Instance().DatabasePath;
	tgridButtons->Buttons[2][0]->Caption = "Stock Server IP Address\r" + TGlobalSettings::Instance().StockInterbaseIP;
	tgridButtons->Buttons[3][0]->Caption = "Stock Database Location\r" + TGlobalSettings::Instance().StockDatabasePath;
	tgridButtons->Buttons[4][0]->Caption = "Membership Server IP Address\r" + TGlobalSettings::Instance().MembershipDatabaseIP;
	tgridButtons->Buttons[5][0]->Caption = "Membership Database Location\r" + TGlobalSettings::Instance().MembershipDatabasePath;
	tgridButtons->Buttons[6][0]->Caption = "Membership Interface Port\r" + IntToStr(TGlobalSettings::Instance().MembershipDatabasePort);
}
//---------------------------------------------------------------------------





void __fastcall TfrmSystemConfig::tgridButtonsMouseClick(TObject *Sender,
		TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	switch(tgridButtons->Row(GridButton))
	{
		case 0:
		{
      	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
			frmTouchKeyboard->MaxLength = 15;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->StartWithShiftDown = false;
			if(TGlobalSettings::Instance().InterbaseIP == "")
			{
				TGlobalSettings::Instance().InterbaseIP = TGlobalSettings::Instance().CurrentIPAddress;
			}
			frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().InterbaseIP;
			frmTouchKeyboard->Caption = "IP Address of MenuMate Interbase Server";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				TGlobalSettings::Instance().InterbaseIP = frmTouchKeyboard->KeyboardText;
				GridButton->Caption = "MenuMate Server IP Address\r" + TGlobalSettings::Instance().InterbaseIP;
				RegistryWrite(MMBaseKey "Database","InterbaseIP",TGlobalSettings::Instance().InterbaseIP);
			}
		}
		break;
		case 1:
		{
			std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
			frmTouchKeyboard->MaxLength = 250;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->StartWithShiftDown = false;
			frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().DatabasePath;
			if(frmTouchKeyboard->KeyboardText == "")
			{
				if(FileExists(ExtractFilePath(Application->ExeName) + "MenuMate.fdb"))
				{
					frmTouchKeyboard->KeyboardText = ExtractFilePath(Application->ExeName) + "MenuMate.fdb";
				}
				else
				{
					frmTouchKeyboard->KeyboardText = "C:\\Program Files\\MenuMate\\MenuMate.fdb";
				}
			}
			frmTouchKeyboard->Caption = "Path of MenuMate.fdb File.";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				TGlobalSettings::Instance().DatabasePath = frmTouchKeyboard->KeyboardText;
				GridButton->Caption = "Database Location\r" + TGlobalSettings::Instance().DatabasePath;
				RegistryWrite(MMBaseKey "Database","DatabasePath",TGlobalSettings::Instance().DatabasePath);
			}

		}
		break;
		case 2:
		{
      	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
			frmTouchKeyboard->MaxLength = 15;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->StartWithShiftDown = false;
			if(TGlobalSettings::Instance().StockInterbaseIP == "" && TGlobalSettings::Instance().InterbaseIP != "")
			{
				TGlobalSettings::Instance().StockInterbaseIP = TGlobalSettings::Instance().InterbaseIP;
			}
			else if (TGlobalSettings::Instance().StockInterbaseIP == "" && TGlobalSettings::Instance().InterbaseIP == "")
			{
				TGlobalSettings::Instance().StockInterbaseIP = TGlobalSettings::Instance().CurrentIPAddress;
			}

			frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().StockInterbaseIP;
			frmTouchKeyboard->Caption = "IP Address of Stock Database Server";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				TGlobalSettings::Instance().StockInterbaseIP = frmTouchKeyboard->KeyboardText;
				GridButton->Caption = "Stock Server IP Address\r" + TGlobalSettings::Instance().StockInterbaseIP;
			}
		}
		break;
		case 3:
		{
      	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
			frmTouchKeyboard->MaxLength = 250;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->StartWithShiftDown = false;
			frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().StockDatabasePath;
			if(frmTouchKeyboard->KeyboardText == "")
			{
				if(FileExists(ExtractFilePath(Application->ExeName) + "Stock.fdb"))
				{
					frmTouchKeyboard->KeyboardText = ExtractFilePath(Application->ExeName) + "Stock.fdb";
				}
				else
				{
					frmTouchKeyboard->KeyboardText = "C:\\Program Files\\MenuMate\\Stock.fdb";
				}
			}
			frmTouchKeyboard->Caption = "Path of Stock.fdb File.";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				TGlobalSettings::Instance().StockDatabasePath = frmTouchKeyboard->KeyboardText;
				GridButton->Caption = "Stock Database Location\r" + TGlobalSettings::Instance().StockDatabasePath;
			}
		}
		break;
		case 4:
		{
      	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
			frmTouchKeyboard->MaxLength = 15;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->StartWithShiftDown = false;
			if(TGlobalSettings::Instance().MembershipDatabaseIP == "" && TGlobalSettings::Instance().InterbaseIP != "")
			{
				TGlobalSettings::Instance().MembershipDatabaseIP = TGlobalSettings::Instance().InterbaseIP;
			}
			else if (TGlobalSettings::Instance().MembershipDatabaseIP == "" && TGlobalSettings::Instance().InterbaseIP == "")
			{
				TGlobalSettings::Instance().MembershipDatabaseIP = TGlobalSettings::Instance().CurrentIPAddress;
			}

			frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().MembershipDatabaseIP;
			frmTouchKeyboard->Caption = "IP Address of Membership Database Server";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				TGlobalSettings::Instance().MembershipDatabaseIP = frmTouchKeyboard->KeyboardText;
				GridButton->Caption = "Membership Server IP Address\r" + TGlobalSettings::Instance().MembershipDatabaseIP;
				RegistryWrite(MMBaseKey "Database","MembershipInterbaseIP",TGlobalSettings::Instance().MembershipDatabaseIP);
			}
		}
		break;
		case 5:
		{
			std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
			frmTouchKeyboard->MaxLength = 250;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->StartWithShiftDown = false;
			frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().MembershipDatabasePath;
			if(frmTouchKeyboard->KeyboardText == "")
			{
				if(FileExists(ExtractFilePath(Application->ExeName) + "Membership.fdb"))
				{
					frmTouchKeyboard->KeyboardText = ExtractFilePath(Application->ExeName) + "Membership.fdb";
				}
				else
				{
					frmTouchKeyboard->KeyboardText = "C:\\Program Files\\MenuMate\\Membership.fdb";
				}
			}
			frmTouchKeyboard->Caption = "Path of Membership.fdb File.";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				TGlobalSettings::Instance().MembershipDatabasePath = frmTouchKeyboard->KeyboardText;
				GridButton->Caption = "Membership Database Location\r" + TGlobalSettings::Instance().MembershipDatabasePath;
				RegistryWrite(MMBaseKey "Database","MembershipDatabasePath",TGlobalSettings::Instance().MembershipDatabasePath);
			}
		}
		break;
		case 6:
		{
			std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
			frmTouchNumpad->Caption = "Enter the Port Number of the Membership Interface.";
			frmTouchNumpad->btnSurcharge->Caption = "Ok";
			frmTouchNumpad->btnSurcharge->Visible = true;
			frmTouchNumpad->btnDiscount->Visible = false;
			frmTouchNumpad->Mode = pmNumber;
			frmTouchNumpad->INTInitial = TGlobalSettings::Instance().MembershipDatabasePort;
			if (frmTouchNumpad->ShowModal() == mrOk)
			{
				TGlobalSettings::Instance().MembershipDatabasePort = frmTouchNumpad->INTResult;
				GridButton->Caption = "Membership Interface Port\r" + IntToStr(TGlobalSettings::Instance().MembershipDatabasePort);
				RegistryWrite(MMBaseKey "Database","MembershipDatabasePort",TGlobalSettings::Instance().MembershipDatabasePort);
			}
		}
		break;
	}
}
//---------------------------------------------------------------------------


void TfrmSystemConfig::EnableStock()
{
	tgridButtons->Buttons[0][0]->Enabled = false;
	tgridButtons->Buttons[1][0]->Enabled = false;
	tgridButtons->Buttons[2][0]->Enabled = true;
	tgridButtons->Buttons[3][0]->Enabled = true;
	tgridButtons->Buttons[4][0]->Enabled = false;
	tgridButtons->Buttons[5][0]->Enabled = false;
	tgridButtons->Buttons[6][0]->Enabled = false;
}

void TfrmSystemConfig::EnableMembership()
{
	tgridButtons->Buttons[0][0]->Enabled = false;
	tgridButtons->Buttons[1][0]->Enabled = false;
	tgridButtons->Buttons[2][0]->Enabled = false;
	tgridButtons->Buttons[3][0]->Enabled = false;
	tgridButtons->Buttons[4][0]->Enabled = true;
	tgridButtons->Buttons[5][0]->Enabled = true;
	tgridButtons->Buttons[6][0]->Enabled = true;
}

void TfrmSystemConfig::EnableEBetMembership()
{
	tgridButtons->Buttons[0][0]->Enabled = false;
	tgridButtons->Buttons[1][0]->Enabled = false;
	tgridButtons->Buttons[2][0]->Enabled = false;
	tgridButtons->Buttons[3][0]->Enabled = false;
	tgridButtons->Buttons[4][0]->Enabled = true;
	tgridButtons->Buttons[5][0]->Enabled = false;
	tgridButtons->Buttons[6][0]->Enabled = true;
}

void TfrmSystemConfig::EnableGeneralLedgerMembership()
{
	tgridButtons->Buttons[0][0]->Enabled = false;
	tgridButtons->Buttons[1][0]->Enabled = false;
	tgridButtons->Buttons[2][0]->Enabled = false;
	tgridButtons->Buttons[3][0]->Enabled = false;
	tgridButtons->Buttons[4][0]->Enabled = true;
	tgridButtons->Buttons[5][0]->Enabled = false;
	tgridButtons->Buttons[6][0]->Enabled = true;
}


void TfrmSystemConfig::EnableMenuMate()
{
	tgridButtons->Buttons[0][0]->Enabled = true;
	tgridButtons->Buttons[1][0]->Enabled = true;
	tgridButtons->Buttons[2][0]->Enabled = false;
	tgridButtons->Buttons[3][0]->Enabled = false;
	tgridButtons->Buttons[4][0]->Enabled = false;
	tgridButtons->Buttons[5][0]->Enabled = false;
	tgridButtons->Buttons[6][0]->Enabled = false;
}
