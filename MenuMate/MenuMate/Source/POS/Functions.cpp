//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Functions.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "ContactStaff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

#include "Analysis.h"

//---------------------------------------------------------------------------
__fastcall TfrmFunctions::TfrmFunctions(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::btnOkMouseClick(TObject *Sender)
{
   	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::CallAwayMouseClick(TObject *Sender)
{
	BtnSelection = 1;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::btnDiscountAllMouseClick(TObject *Sender)
{
	BtnSelection = 3;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::btnHappyHourMouseClick(TObject *Sender)
{
	BtnSelection = 6;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::btnLuckyMembersMouseClick(TObject *Sender)
{
	BtnSelection = 4;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::btnFloatManagementMouseClick(TObject *Sender)
{
	BtnSelection = 8;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::btnProgramsMouseClick(TObject *Sender)
{
	BtnSelection = 2;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::btnReportsMouseClick(TObject *Sender)
{
	if (TfrmAnalysis::AuthenticateReportsAccess(TReportSource::SelectDish)
	    != lsAccepted)
		return;

	BtnSelection = 5;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::btnWastageMouseClick(TObject *Sender)
{
	BtnSelection = 7;
	Close();
}
//---------------------------------------------------------------------------
int TfrmFunctions::GetBtnSelection(void)
{
	return BtnSelection;
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::FormCreate(TObject *Sender)
{
	if (TGlobalSettings::Instance().ForceHappyHour)
	{
			BtnHappyHour->Caption = "Cancel Forced Happy Hour";
	}
	else
	{
			BtnHappyHour->Caption = "Force Happy Hour Now";
	}


	if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"])
	{
			BtnLuckyMember->Visible = true;
	}
       btnSync->Visible = TGlobalSettings::Instance().LoyaltyMateEnabled;
       btnGiftCard->Visible = TGlobalSettings::Instance().LoyaltyMateEnabled;

}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::btnPaxCountMouseClick(TObject *Sender)
{
	BtnSelection = 9;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::TouchBtn1MouseClick(TObject *Sender)
{
	BtnSelection = 10;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::tbtnSearchItemMouseClick(TObject *Sender)
{
    BtnSelection = 11;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::btnChangeCardMouseClick(TObject *Sender)
{
    BtnSelection = 12;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::btnLiveTransactionMouseClick(TObject *Sender)
{
	BtnSelection = 13;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFunctions::btnSyncMouseClick(TObject *Sender)
{
   BtnSelection = 15;
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmFunctions::btnGiftCardMouseClick(TObject *Sender)
{
   BtnSelection = 16;
    Close();
}
//---------------------------------------------------------------------------

