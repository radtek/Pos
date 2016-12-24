//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop


#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h> // for open & read constants
#include <math.h>
#include <MMTouchNumpad.h>
#include "DeviceWeb.h"
#include "FileCtrl.hpp"
#ifdef MenuMate
#include "DeviceRealTerminal.h"

#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#include "Maintain.h"
#include "Main.h"
#include "Login.h"
#include "SelectTable.h"
#include "Message.h"
#include "DrinkCommandManager.h"
#include "MMTouchKeyboard.h"
#include "NewPaymentType.h"
#include "UserMaintenance.h"
#include "HappyHourSettings.h"
#include "GeneralMaintenance.h"
#include "MessageMaintenance.h"
#include "PaymentMaintenance.h"
#include "Analysis.h"
#include "DbMod.h"
#include "Rooms.h"
#include "MMMessageBox.h"
#include "ManagerDiscount.h"
#include "Discounts.h"
#include "DealsSetup.h"
#include "ManagerVariable.h"
#include "TableManager.h"
#include "Locations.h"
#include "RoomConfiguration.h"
#include "PHSConfiguration.h"
#include "PhoenixHotelSystem.h"
#include "PrinterControlPanel.h"
#include "Membership.h"
#include "ContactStaff.h"
#include "SmartCardConfigGUI.h"
#include "ChitNumberController.h"
#include "IntaMateConfiguration.h"
#include "POS_XMLBase.h"
#include "DBTables.h"
#include "GroupGUI.h"
#include "DBGroups.h"

//#include "VerticalSelect.h"
#include "ManagerPatron.h"
#include "EnableFloorPlan.h"
#include "WebMate.h"

#include <process.h>

#include "MMColourPicker.h"
#include "SmartcardPreloader.h"

#include "TaxMaintenance.h"
#include "TierLevels.h"
#include "FolderManager.h"

#include "SetupGlCodes.h"
#include "ManagerClippIntegration.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define MAX_SHORT_NAME_LENGTH 10
#define MAX_LOCATION_LENGTH 10

//TfrmMaintain *frmMaintain;
//---------------------------------------------------------------------------
__fastcall TfrmMaintain::TfrmMaintain(TComponent* Owner)
: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::FormResize(TObject *Sender)
{
	//if (Tag != Screen->Width)
  //	{
		int Temp = Tag;
		Tag = Screen->Width;
		if(double(Screen->Width) / Screen->Height < 1.4)
		{
			ScaleBy(Screen->Width, Temp);
		}
   //	}
	//add frn
	pnlButtons1->Top = Pages->Left;
	pnlButtons1->Left = ClientWidth - pnlButtons1->Width - Pages->Left;
	pnlButtons1->Height = ClientHeight - (Pages->Left * 2);
	pnlButtons1->Width = pnlButtons1->Width + 1;
	Pages->Top = Pages->Left;
	Pages->Width = pnlButtons1->Left - (Pages->Left * 2);
	Pages->Height =  ClientHeight - (Pages->Left * 2);

}
//-------------------------------------------------------------------------
void __fastcall TfrmMaintain::FormShow(TObject *Sender)
{
	FormResize(this);

	Top = 0;
	Left = 0;
	Width = Screen->Width;
	Height = Screen->Height;

	btnChangeRooms->Enabled = TRooms::Instance().Enabled;
	btnChangeTable->Enabled = TGlobalSettings::Instance().TablesEnabled;

	btnLoyalty->Enabled = static_cast<bool>(TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"]) &&
	static_cast<bool>(!TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["ReadOnly"]);

	tbtnLocations->Caption  = "Location \r" + TDeviceRealTerminal::Instance().ID.Location;
    if((TDeviceRealTerminal::Instance().IMManager->Registered))
    {
    TouchBtnThorlink->Enabled = true;
    }
    else
    {
    TouchBtnThorlink->Enabled = false;
    }

	tbPHSInterface->Enabled = PhoenixHM->Registered;
    if(tbPHSInterface->Enabled)
    tbPHSInterface->Caption = "P.M.S Interface \r[Disabled]";
	if(PhoenixHM->Enabled)
	{
		tbPHSInterface->Caption = "P.M.S Interface \r[Enabled]";
		tbPHSInterface->ButtonColor = clGreen;
	}
	else
	{
		tbPHSInterface->Caption = "P.M.S Interface\r[Unregistered]";
	}

	if(TDeviceRealTerminal::Instance().IMManager->Registered)
	{
		if(TDeviceRealTerminal::Instance().IMManager->Enabled)
		{
			TDeviceRealTerminal::Instance().IMManager->Status();
			if(TDeviceRealTerminal::Instance().IMManager->OffLine)
			{
				tbIntaMate->Caption = "IntaMate Interface \r[Enabled][OffLine]";
                TouchBtnBarExchange->Caption = "Bar Exchange \r[Enabled][OffLine]";
				tbIntaMate->ButtonColor = clMaroon;
                TouchBtnBarExchange->ButtonColor = clMaroon;
			}
			else
			{
				tbIntaMate->Caption = "IntaMate Interface \r[Enabled][Online]";
                 TouchBtnBarExchange->Caption = "Bar Exchange \r[Enabled][Online]";
                 TouchBtnBarExchange->ButtonColor = clGreen;
				tbIntaMate->ButtonColor = clGreen;
			}
		}
		else
		{
			tbIntaMate->Caption = "IntaMate Interface \r[Disabled]";
            TouchBtnBarExchange->Caption = "Bar Exchange \r[Disabled]";
			tbIntaMate->ButtonColor = clRed;
            TouchBtnBarExchange->ButtonColor = clRed;
		}
	}
	else
	{
		tbIntaMate->Caption = "IntaMate Interface \r[Unregistered]";
		tbIntaMate->Enabled = TDeviceRealTerminal::Instance().IMManager->Registered;
        TouchBtnBarExchange->Caption = "Bar Exchange \r[Unregistered]";
        TouchBtnBarExchange->Enabled = TDeviceRealTerminal::Instance().IMManager->Registered;
	}

	RefreshReservationBtnColor();
	if (TGlobalSettings::Instance().ReservationsEnabled)
	btnChangeTable->Caption = "Edit Tables";

	tbtnSmartCards->Enabled =  static_cast<bool>(TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Registered"]);

	Pages->ActivePage = tsMaintenance;
	RedrawButtons(tbtnMaintenance);
	SaveServerCheck();
	RefreshWebMateBtnColor( );

	RefreshLoyaltyMateBtnColor();
	RefreshBarExchangeBtnColor();
    InitXeroIntegrationConfig();
    RefreshDrinkCommandButtonColor();
    RefreshThorlinkButtonColor();                                                // Refreshes the Thorlink button color
	//Changing the color of clipp button
    if(TGlobalSettings::Instance().IsClippIntegrationEnabled)
    {
        TouchBtnClipInterface->ButtonColor = clGreen;
        TouchBtnClipInterface->Caption = "Clipp Interface \r[Enabled]";
    }
    else
    {
        TouchBtnClipInterface->ButtonColor = clRed;
        TouchBtnClipInterface->Caption = "Clipp Interface \r[Disabled]";
    }
     if(TGlobalSettings::Instance().IsRunRateBoardEnabled)
    {
        TouchBtnRunRateBoard->ButtonColor = clGreen;
        TouchBtnRunRateBoard->Caption = "Run Rate Board\r[Enabled]";
    }
    else
    {
        TouchBtnRunRateBoard->ButtonColor = clRed;
       TouchBtnRunRateBoard->Caption = "Run Rate Board\r[Disabled]";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::imgExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::btnPrinterMaintenanceClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckPrinters);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		std::auto_ptr<TfrmPrinterControlPanel>(frmPrinterControlPanel)(TfrmPrinterControlPanel::Create<TfrmPrinterControlPanel>(this,TDeviceRealTerminal::Instance().DBControl));
		frmPrinterControlPanel->ShowModal();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the printer settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::btnUsersClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckUsers);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		std::auto_ptr<TfrmUserMaintenance>(frmUserMaintenance)(TfrmUserMaintenance::Create<TfrmUserMaintenance>(this,TDeviceRealTerminal::Instance().DBControl));
		frmUserMaintenance->UserMode = umStaff;
		frmUserMaintenance->ShowModal();
		ExportIntaMateListStaff();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the user settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::btnLoyaltyClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);

	TMMContactInfo UserInfo;
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,UserInfo, CheckAccountManager);

	TDeviceRealTerminal::Instance().ManagerMembership->SetPhoneOrderFlowMemberSelection(false);

	if (Result == lsAccepted)
	{
		TMMContactInfo ContactInfo;

		if(TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Enabled"])
		{
			//If there is a card inserted go stright to editing that user.
			if(TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardOk)
			{
				eMemberSource MemberSource;
				TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction,ContactInfo,MemberSource);
				if (TSmartcard_Preloader::is_preloaded_card(ContactInfo)) {
					DBTransaction.Commit();
					return;
				}

				TDeviceRealTerminal::Instance().ManagerMembership->EditMember(DBTransaction,ContactInfo);
			}
			else if(TDeviceRealTerminal::Instance().ManagerMembership->AllowSmartCardNotPresentMemberTransactions)
			{
				Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
				TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
				DBTransaction.StartTransaction();
				eMemberSource MemberSource;
				TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction,ContactInfo,MemberSource);
				DBTransaction.Commit();
			}
			else
			{
				MessageBox("No Smart Card inserted.", "Error", MB_OK + MB_ICONERROR);
			}
		}
		else if(TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
		{
			Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
			eMemberSource MemberSource;
			TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction,ContactInfo,MemberSource);
			DBTransaction.Commit();
		}
		else
		{
			MessageBox("Membership is not Enabled.", "Error", MB_OK + MB_ICONERROR);
		}
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the loyalty settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::btnTableNameClick(TObject *Sender)
{
	TLoginSuccess lresult;
	std::auto_ptr<TContactStaff> staff;
	TMMContactInfo tmp_ui;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();
	staff.reset(new TContactStaff(tr));
	lresult = staff->Login(this, tr, tmp_ui, CheckMaintenance);
	tr.Commit();

	switch (lresult) {
	case lsDenied:
		MessageBox("You do not have access to Setup.", "Error",
		MB_OK | MB_ICONERROR);
		return;
	case lsPINIncorrect:
		MessageBox("The login was unsuccessful.", "Error",
		MB_OK | MB_ICONERROR);
	case lsCancel:
		return;
	}

	if (TGlobalSettings::Instance().ReservationsEnabled)
    {
		if (_wspawnl(
					_P_NOWAIT,
					(ExtractFilePath(Application->ExeName)
						+ "Floorplan\\GuiClient\\"
						"FloorPlanEditor.exe").w_str(),
					(ExtractFilePath(Application->ExeName)
						+ "Floorplan\\GuiClient\\"
						"FloorPlanEditor.exe").w_str(),
					NULL) != -1)
		return;
    }

	std::auto_ptr<TfrmSelectTable> frm_seltbl(
	TfrmSelectTable::Create<TfrmSelectTable>(
	this, TDeviceRealTerminal::Instance().DBControl));

	std::auto_ptr<TfrmTouchKeyboard> frm_tchkb(
	TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));

	bool must_signal_changes = 0;

	frm_seltbl->ShowAll = true;
	frm_seltbl->ChangingName = true;

	frm_tchkb->MaxLength = 25;
	frm_tchkb->AllowCarriageReturn = false;
	frm_tchkb->Caption = "Enter Table Name";

	for (TNetMessageInfoSync *req; frm_seltbl->ShowModal() == mrOk; ) {
		tr.StartTransaction();
		frm_tchkb->KeyboardText =
		TDBTables::GetTableName(tr, frm_seltbl->SelectedTabContainerNumber);
		if (frm_tchkb->ShowModal() == mrOk) {
			TDBTables::SetTableName(tr, frm_seltbl->SelectedTabContainerNumber,
			frm_tchkb->KeyboardText);
			must_signal_changes |= 1;
		}
		tr.Commit();

		frm_seltbl->ShowAll = true;
		frm_seltbl->ChangingName = true;
	}

	if (must_signal_changes) {
		TNetMessageInfoSync *r = new TNetMessageInfoSync();
		try {
			r->Broadcast = true;
			TDeviceRealTerminal::Instance().UpdateInfoSync(r);
		} __finally {
			delete r;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::btnHappyHourClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		TfrmHappyHourSettings *frmHappyHourSettings = new TfrmHappyHourSettings(this);
		try
		{
			frmHappyHourSettings->ShowModal();
		}
		__finally
		{
			delete frmHappyHourSettings;
		}
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the Happy Hour settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::BitBtn2Click(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		std::auto_ptr<TfrmMessageMaintenance>(frmMessageMaintenance)(TfrmMessageMaintenance::Create<TfrmMessageMaintenance>(this,TDeviceRealTerminal::Instance().DBControl));
		if (Sender == btnBroadcasts)
		{
			frmMessageMaintenance->MessageType = eMessage;
			frmMessageMaintenance->ShowModal();
		}
		else if (Sender == btnMsgCancels)
		{
			frmMessageMaintenance->MessageType = eCancelReason;
			frmMessageMaintenance->ShowModal();
		}
		else if (Sender == btnMsgCredits)
		{
			frmMessageMaintenance->MessageType = eCreditReason;
			frmMessageMaintenance->ShowModal();
		}
		else if (Sender == tbRunPrograms)
		{
			frmMessageMaintenance->MessageType = eRunProgram;
			frmMessageMaintenance->ShowModal();
		}
		else if (Sender == tbtnPatronTypes)
		{
			frmMessageMaintenance->MessageType = ePatronTypes;
			frmMessageMaintenance->ShowModal();
		}
		else if (Sender == tbtnCashDrawer)
		{
			frmMessageMaintenance->MessageType = eCashDrawer;
			frmMessageMaintenance->ShowModal();
		}
		else if (Sender == tbtnCashDenomination)
		{
			frmMessageMaintenance->MessageType = eCashDenomination;
			frmMessageMaintenance->ShowModal();
		}
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the messages settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::BtnPaymentClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckPaymentTypes);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		std::auto_ptr<TfrmPaymentMaintenance>(frmPaymentMaintenance)(TfrmPaymentMaintenance::Create(this,TDeviceRealTerminal::Instance().DBControl,TDeviceRealTerminal::Instance().PaymentSystem));
		frmPaymentMaintenance->ShowModal();
		ExportIntaMateDataPayments();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the payment settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}

void __fastcall TfrmMaintain::btnChangeRoomsClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		DBTransaction.StartTransaction();
		std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		while (TRooms::Instance().SelectRoom(DBTransaction,eRoomFilterAll) == mrOk)
		{
			frmTouchKeyboard->MaxLength = 25;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->KeyboardText =  TRooms::Instance().SelectedRoom->Name;
			frmTouchKeyboard->Caption = "Enter Room Name";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
				DBTransaction1.StartTransaction();
				TDBRooms::SetRoomName(DBTransaction1,TRooms::Instance().SelectedRoom->RoomNo,frmTouchKeyboard->KeyboardText);
				DBTransaction1.Commit();
			}
		}
		DBTransaction.Commit();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to alter room names.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::btnDiscountsClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
        TfrmDiscounts *frmDiscounts = new TfrmDiscounts(this,TDeviceRealTerminal::Instance().DBControl);
		frmDiscounts->ShowModal();
        delete frmDiscounts;
		ExportIntaMateDiscounts();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the discount settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::tbtnLocationsClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{

		MessageBox("Please select a new location for this terminal.", "Location Request", MB_ICONINFORMATION + MB_OK);
		std::auto_ptr<TfrmLocations>(frmLocations)(TfrmLocations::Create<TfrmLocations>(this,TDeviceRealTerminal::Instance().DBControl));
		do
		{
			frmLocations->ShowModal();
			if(frmLocations->SelectedLocation == "")
			{
				MessageBox("All termianls must have a location.", "Error", MB_OK + MB_ICONWARNING);
			}
		}while(frmLocations->SelectedLocation == "");
		TDeviceRealTerminal::Instance().ID.Location = frmLocations->SelectedLocation;
		TDeviceRealTerminal::Instance().ID.LocationKey = frmLocations->SelectedLocationKey;
		tbtnLocations->Caption = "Location \r" + TDeviceRealTerminal::Instance().ID.Location;
		DBTransaction.StartTransaction();
		TDeviceRealTerminal::Instance().Create(DBTransaction);
		DBTransaction.Commit();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the PHS Interface settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::tbPHSInterfaceClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		std::auto_ptr<TfrmPHSConfiguration>(frmPHSConfiguration)(TfrmPHSConfiguration::Create<TfrmPHSConfiguration>(this));
		frmPHSConfiguration->ShowModal();
		if(PhoenixHM->Enabled)
		{
			tbPHSInterface->Caption = "P.M.S Interface \r[Enabled]";
			tbPHSInterface->Color = clGreen;
		}
		else
		{
			tbPHSInterface->Caption = "P.M.S Interface \r[Disabled]";
			tbPHSInterface->Color = clMaroon;
		}
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the PHS Interface settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::TouchBtn3MouseClick(TObject *Sender)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::tbtnSmartCardsMouseClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckAccountManager);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		std::auto_ptr<TfrmSmartCardConfiguration>(frmSmartCardConfiguration)(TfrmSmartCardConfiguration::Create<TfrmSmartCardConfiguration>(this));
		frmSmartCardConfiguration->ShowModal();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the Smart Card Interface settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::TouchBtn1MouseClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TChitNumberController ChitNumberController(this,DBTransaction);
	ChitNumberController.Run();
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::tbIntaMateMouseClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		// Display Launch List
		std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

		TVerticalSelection Item;
		Item.Title = "Cancel";
		Item.Properties["Color"] = IntToStr(clMaroon);
		Item.CloseSelection = true;
		SelectionForm->Items.push_back(Item);

		TVerticalSelection Item1;
		Item1.Title = "Configure";
		Item1.Properties["Action"] = IntToStr(1);
		Item1.Properties["Color"] = IntToStr(clNavy);
		Item1.CloseSelection = true;
		SelectionForm->Items.push_back(Item1);

		Item1.Title = "Export Data";
		Item1.Properties["Action"] = IntToStr(2);
		Item1.Properties["Color"] = IntToStr(clNavy);
		Item1.CloseSelection = true;
		SelectionForm->Items.push_back(Item1);

		SelectionForm->ShowModal();
		TVerticalSelection SelectedItem;
		if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
		{
			int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
			switch(Action)
			{
			case 1 :
				{
					std::auto_ptr<TfrmIntaMateConfiguration>(frmIntaMateConfiguration)(TfrmIntaMateConfiguration::Create<TfrmIntaMateConfiguration>(this));

					frmIntaMateConfiguration->TCPIPAddress 	= TGlobalSettings::Instance().IntaMateIPAddress;
					frmIntaMateConfiguration->TCPPort      	= TGlobalSettings::Instance().IntaMatePort;
					frmIntaMateConfiguration->TCPTimeOut_ms   = TGlobalSettings::Instance().IntaMateTCPTimeOut_ms;
					frmIntaMateConfiguration->POSID        	= TGlobalSettings::Instance().IntaMateTerminalID;

					frmIntaMateConfiguration->ShowModal();

					TGlobalSettings::Instance().IntaMateIPAddress    	= frmIntaMateConfiguration->TCPIPAddress;
					TGlobalSettings::Instance().IntaMatePort         	= frmIntaMateConfiguration->TCPPort;
					TGlobalSettings::Instance().IntaMateTerminalID   	= frmIntaMateConfiguration->POSID;
					TGlobalSettings::Instance().IntaMateTCPTimeOut_ms	= frmIntaMateConfiguration->TCPTimeOut_ms;

					DBTransaction.StartTransaction();
					TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmIntaMateIPAddress,TGlobalSettings::Instance().IntaMateIPAddress);
					TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmIntaMatePort,TGlobalSettings::Instance().IntaMatePort);
					TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmIntaMateTerminalID,TGlobalSettings::Instance().IntaMateTerminalID);
					TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmIntaMateIPTimeOut,TGlobalSettings::Instance().IntaMateTCPTimeOut_ms);
					DBTransaction.Commit();

					TDeviceRealTerminal::Instance().IMManager->Initialise(TGlobalSettings::Instance().IntaMateIPAddress,
					TGlobalSettings::Instance().IntaMatePort,
					TGlobalSettings::Instance().IntaMateTerminalID,
					TGlobalSettings::Instance().IntaMateTCPTimeOut_ms);
					TDeviceRealTerminal::Instance().IMManager->Status();
					if(TDeviceRealTerminal::Instance().IMManager->Enabled)
					{
						if(TDeviceRealTerminal::Instance().IMManager->OffLine)
						{
							tbIntaMate->Caption = "IntaMate Interface \r[Enabled][OffLine]";
							tbIntaMate->ButtonColor = clMaroon;
						}
						else
						{
							tbIntaMate->Caption = "IntaMate Interface \r[Enabled][Online]";
							tbIntaMate->ButtonColor = clGreen;
						}
					}
					else
					{
						MessageBox("Intamate Interface Disabled: " + TDeviceRealTerminal::Instance().IMManager->LastErrorMessage, "Intamate Interface Disabled.", MB_OK + MB_ICONERROR);
						tbIntaMate->Caption = "IntaMate Interface \r[Disabled]";
						tbIntaMate->ButtonColor = clRed;
					}
				}
				break;
			case 2 :
				{
					ExportIntaMateData();
				}
				break;
			}
		}
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the IntaMate Interface settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::ExportIntaMateData()
{
	// Display Launch List
	std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

	TVerticalSelection Item;
	Item.Title = "Cancel";
	Item.Properties["Color"] = IntToStr(clMaroon);
	Item.CloseSelection = true;
	SelectionForm->Items.push_back(Item);

	TVerticalSelection Item1;
	Item1.Title = "Export All";
	Item1.Properties["Action"] = IntToStr(1);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "Export Version";
	Item1.Properties["Action"] = IntToStr(2);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "Export Products";
	Item1.Properties["Action"] = IntToStr(3);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "Export Payments";
	Item1.Properties["Action"] = IntToStr(4);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "Export Categories and Groups";
	Item1.Properties["Action"] = IntToStr(5);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "Staff";
	Item1.Properties["Action"] = IntToStr(6);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "Members";
	Item1.Properties["Action"] = IntToStr(7);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "Discounts";
	Item1.Properties["Action"] = IntToStr(8);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "Fixed Totals";
	Item1.Properties["Action"] = IntToStr(9);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "Patron Counts";
	Item1.Properties["Action"] = IntToStr(10);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "Member Groups";
	Item1.Properties["Action"] = IntToStr(11);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);



	SelectionForm->ShowModal();
	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
	{
		ExportIntaMate( StrToIntDef( SelectedItem.Properties["Action"],0 ) );
	}
}

void __fastcall TfrmMaintain::ExportIntaMate( int inAction )
{
	switch( inAction )
	{
	case  1 : ExportIntaMateAll();	                break;
	case  2 : ExportIntaMateVersion();	            break;
	case  3 : ExportIntaMateDataProduct();          break;
	case  4 : ExportIntaMateDataPayments();         break;
	case  5 : ExportIntaMateDataCategoriesGroups(); break;
	case  6 : ExportIntaMateListStaff();            break;
	case  7 : ExportIntaMateDataMembers();          break;
	case  8 : ExportIntaMateDiscounts();            break;
	case  9 : ExportIntaMateFixed();                break;
	case 10 : ExportIntaMatePatronCounts();         break;
	case 11 : ExportIntaMateMemberGroups();         break;
	}
}

void __fastcall TfrmMaintain::ExportIntaMateAll()
{
	ExportIntaMateVersion();
	ExportIntaMateDataProduct();
	ExportIntaMateDataPayments();
	ExportIntaMateDataCategoriesGroups();
	ExportIntaMateListStaff();
	ExportIntaMateDataMembers();
	ExportIntaMateDiscounts();
	ExportIntaMatePatronCounts();
	ExportIntaMateFixed();
	ExportIntaMateMemberGroups();
}

void __fastcall TfrmMaintain::ExportIntaMateVersion()
{
	std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(Screen->ActiveForm));
	frmProcessing->CanCancel = false;
	frmProcessing->Message = "Exporting Version...";
	frmProcessing->ShowProgress = false;
	frmProcessing->Show();

	TPOS_XMLBase POSXML("Version Export");
	TDeviceRealTerminal::Instance().BuildXMLVersion( POSXML, TGlobalSettings::Instance().SiteID );
	TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
	POSXML.SaveToFile();
	frmProcessing->Close();
}

void __fastcall TfrmMaintain::ExportIntaMateDataProduct()
{
	std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(Screen->ActiveForm));
	frmProcessing->CanCancel = false;
	frmProcessing->Message = "Exporting Products...";
	frmProcessing->ShowProgress = false;
	frmProcessing->Show();

	// Update IntaMate.
	TPOS_XMLBase POSXML("List Product Export");
	TDeviceRealTerminal::Instance().Menus->BuildXMLMenu(TDeviceRealTerminal::Instance().DBControl,POSXML,TGlobalSettings::Instance().SiteID);
	TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
	POSXML.SaveToFile();
	frmProcessing->Close();
}

void __fastcall TfrmMaintain::ExportIntaMateDataPayments()
{
	std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(Screen->ActiveForm));
	frmProcessing->CanCancel = false;
	frmProcessing->Message = "Exporting Payments...";
	frmProcessing->ShowProgress = false;
	frmProcessing->Show();

	TPOS_XMLBase POSXML("List Payments Export");
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TPaymentTransaction PaymentTransaction(DBTransaction);
	TDeviceRealTerminal::Instance().PaymentSystem->PaymentsLoadTypes(PaymentTransaction);
	PaymentTransaction.BuildXMLPaymentTypes(POSXML);
	DBTransaction.Commit();
	TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
	POSXML.SaveToFile();
	frmProcessing->Close();
}

void __fastcall TfrmMaintain::ExportIntaMateDataCategoriesGroups()
{
	std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(Screen->ActiveForm));
	frmProcessing->CanCancel = false;
	frmProcessing->Message = "Exporting Categories and Groups...";
	frmProcessing->ShowProgress = false;
	frmProcessing->Show();

	TPOS_XMLBase POSXML("List Categories Export");
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TDeviceRealTerminal::Instance().Menus->BuildXMLListCategories(DBTransaction,POSXML,TGlobalSettings::Instance().SiteID);
	DBTransaction.Commit();
	TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
	POSXML.SaveToFile();

	TPOS_XMLBase POSXML1("List Category Groups Export");
	DBTransaction.StartTransaction();
	//TDBGroups::BuildXMLListGroups(DBTransaction,POSXML1);
	TDeviceRealTerminal::Instance().Menus->BuildXMLListGroup(DBTransaction,POSXML1,TGlobalSettings::Instance().SiteID);
	DBTransaction.Commit();
	TDeviceRealTerminal::Instance().IMManager->Export(POSXML1);
	POSXML1.SaveToFile();
	frmProcessing->Close();
}

void __fastcall TfrmMaintain::ExportIntaMateListStaff()
{
	std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(Screen->ActiveForm));
	frmProcessing->CanCancel = false;
	frmProcessing->Message = "Exporting Staff...";
	frmProcessing->ShowProgress = false;
	frmProcessing->Show();

	TPOS_XMLBase POSXML("List Staff Export");
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	Staff->BuildXMLListStaff(DBTransaction,POSXML);
	DBTransaction.Commit();
	TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
	POSXML.SaveToFile();

	frmProcessing->Close();
}

void __fastcall TfrmMaintain::ExportIntaMatePatronCounts()
{
	std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(Screen->ActiveForm));
	frmProcessing->CanCancel = false;
	frmProcessing->Message = "Exporting Patron Types...";
	frmProcessing->ShowProgress = false;
	frmProcessing->Show();

	TPOS_XMLBase POSXML("List Patron Export");
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TManagerPatron::Instance().BuildXMLListPatronCounts(DBTransaction,POSXML);
	DBTransaction.Commit();
	TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
	POSXML.SaveToFile();
	frmProcessing->Close();
}

void __fastcall TfrmMaintain::ExportIntaMateDataMembers()
{
	std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(Screen->ActiveForm));
	frmProcessing->CanCancel = false;
	frmProcessing->Message = "Exporting Members...";
	frmProcessing->ShowProgress = false;
	frmProcessing->Show();

	TPOS_XMLBase POSXML("List Members Export");
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TDeviceRealTerminal::Instance().ManagerMembership->BuildXMLListMembership(DBTransaction,POSXML);
	DBTransaction.Commit();
	TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
	POSXML.SaveToFile();

	frmProcessing->Close();
}

void __fastcall TfrmMaintain::ExportIntaMateMemberGroups()
{
	std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(Screen->ActiveForm));
	frmProcessing->CanCancel = false;
	frmProcessing->Message = "Exporting Member Groups...";
	frmProcessing->ShowProgress = false;
	frmProcessing->Show();

	TPOS_XMLBase POSXML("List Member Groups Export");
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TDBGroups::BuildXMLListMemberGroups(DBTransaction,POSXML);
	DBTransaction.Commit();
	TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
	POSXML.SaveToFile();

	frmProcessing->Close();
}

void __fastcall TfrmMaintain::ExportIntaMateDiscounts()
{
	std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(Screen->ActiveForm));
	frmProcessing->CanCancel = false;
	frmProcessing->Message = "Exporting Discounts List...";
	frmProcessing->ShowProgress = false;
	frmProcessing->Show();

	TPOS_XMLBase POSXML("List Discount Export");
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	ManagerDiscount->BuildXMLListDiscounts(DBTransaction,POSXML);
	DBTransaction.Commit();
	TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
	POSXML.SaveToFile();

	frmProcessing->Close();
}

void __fastcall TfrmMaintain::ExportIntaMateFixed()
{
	std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(Screen->ActiveForm));
	frmProcessing->CanCancel = false;
	frmProcessing->Message = "Exporting Calculated List...";
	frmProcessing->ShowProgress = false;
	frmProcessing->Show();

	TPOS_XMLBase POSXML("List Calculated Export");

	std::auto_ptr<TfrmAnalysis>(frmAnalysis)(TfrmAnalysis::Create<TfrmAnalysis>(this));
	frmAnalysis->BuildXMLListCalculated(POSXML);
	TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
	POSXML.SaveToFile();
	frmProcessing->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::ResetWebMate()
{
	TWebMate::Instance().Initialise(TGlobalSettings::Instance().WebMateEnabled, ExtractFilePath(Application->ExeName),TGlobalSettings::Instance().InterbaseIP,TGlobalSettings::Instance().DatabasePath, TGlobalSettings::Instance().WebMatePort);
	TDeviceRealTerminal::Instance().Modules.Status[eWebMate]["Enabled"] = TWebMate::Instance().Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::RefreshWebMateBtnColor()
{
	if( TDeviceRealTerminal::Instance().Modules.Status[eWebMate]["Registered"])
	{
		if(TGlobalSettings::Instance().WebMateEnabled )
		{
			tchbtnWebMate->ButtonColor = clGreen;

			tchbtnWebMate->Caption = "WebMate \r[Enabled]";
		}
		else
		{
			tchbtnWebMate->ButtonColor = clRed;

			tchbtnWebMate->Caption = "WebMate \r[Disabled]";
		}
	}
	else
	{
		tchbtnWebMate->ButtonColor = clRed;
		tchbtnWebMate->Caption = "WebMate \r[Unregistered]";
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::tbtnPocketVouchersMouseClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	if (Result == lsAccepted)
	{
		std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 128;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = TDeviceRealTerminal::Instance().PocketVouchers->URL;
		frmTouchKeyboard->Caption = "Enter the URL of the Pocket Voucher System.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			TDeviceRealTerminal::Instance().PocketVouchers->URL = frmTouchKeyboard->KeyboardText;
			TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPocketVoucherURL,TDeviceRealTerminal::Instance().PocketVouchers->URL);
		}
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the Pocket Voucher settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::tbtnEBetMouseClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		// Display Launch List
		std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

		TVerticalSelection Item;
		Item.Title = "Cancel";
		Item.Properties["Color"] = IntToStr(clMaroon);
		Item.CloseSelection = true;
		SelectionForm->Items.push_back(Item);

		TVerticalSelection Item1;
		Item1.Title = "Till ID \r" + IntToStr(TGlobalSettings::Instance().MembershipTillID );
		Item1.Properties["Action"] = IntToStr(1);
		Item1.Properties["Color"] = IntToStr(clNavy);
		Item1.CloseSelection = true;
		SelectionForm->Items.push_back(Item1);

		SelectionForm->ShowModal();
		TVerticalSelection SelectedItem;
		if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
		{
			int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
			switch(Action)
			{
			case 1 :
				{
					std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
					frmTouchNumpad->Caption = "Enter the EBet ID for this POS.";
					frmTouchNumpad->btnSurcharge->Caption = "Ok";
					frmTouchNumpad->btnSurcharge->Visible = true;
					frmTouchNumpad->btnDiscount->Visible = false;
					frmTouchNumpad->Mode = pmNumber;
					frmTouchNumpad->INTInitial = TGlobalSettings::Instance().MembershipTillID;
					if (frmTouchNumpad->ShowModal() == mrOk)
					{
						TGlobalSettings::Instance().MembershipTillID = frmTouchNumpad->INTResult;

						DBTransaction.StartTransaction();
						TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMembershipTillID,TGlobalSettings::Instance().MembershipTillID);
						DBTransaction.Commit();
						MessageBox("MenuMate Restart Required.", "Info", MB_OK + MB_ICONINFORMATION);
					}
				}
			}
		}
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the IntaMate Interface settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
const SELDIRHELP = 1000;
void __fastcall TfrmMaintain::tchbtnWebMateMouseClick(TObject *Sender)
{
	if(TDeviceRealTerminal::Instance().Modules.Status[eWebMate]["Registered"])
	{
		TMMContactInfo TempUserInfo;
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
		TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
		DBTransaction.Commit();
		if (Result == lsAccepted)
		{
			// Display Launch List
			std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

			TVerticalSelection Item;
			Item.Title = "Cancel";
			Item.Properties["Color"] = IntToStr(clMaroon);
			Item.CloseSelection = true;
			SelectionForm->Items.push_back(Item);

			TVerticalSelection Item1;
			Item1.Title = UnicodeString("Enable/Disable \r") + UnicodeString((TGlobalSettings::Instance().WebMateEnabled ? "Enabled" : "Disabled"));
			Item1.Properties["Action"] = IntToStr(1);

			//:::::::::::::::::::::::::::::::::::::::

			//Item1.Properties["Color"] = IntToStr(clNavy);

			if( TGlobalSettings::Instance().WebMateEnabled )
			{
				Item1.Properties["Color"] = IntToStr(clGreen);
			}
			else
			{
				Item1.Properties["Color"] = IntToStr(clRed);
			}

			//:::::::::::::::::::::::::::::::::::::::

			Item1.CloseSelection = true;
			SelectionForm->Items.push_back(Item1);

			TVerticalSelection Item2;
			Item2.Title = "Listen Port \r" + IntToStr(TGlobalSettings::Instance().WebMatePort);
			Item2.Properties["Action"] = IntToStr(2);
			Item2.Properties["Color"] = IntToStr(clNavy);
			Item2.CloseSelection = true;
			SelectionForm->Items.push_back(Item2);

			TVerticalSelection Item3;

			Item3.Title = "Auto Add Members \r" + UnicodeString(TGlobalSettings::Instance().AutoAddWebMembers ? "Enabled" : "Disabled");
			Item3.Properties["Action"] = IntToStr(3);
			if( TGlobalSettings::Instance().AutoAddWebMembers )
			{
				Item3.Properties["Color"] = IntToStr(clGreen);
			}
			else
			{
				Item3.Properties["Color"] = IntToStr(clRed);
			}
			Item3.CloseSelection = true;
			SelectionForm->Items.push_back(Item3);

			TVerticalSelection Item4;

			Item4.Title = "Auto Accept Web Orders \r" + UnicodeString(TGlobalSettings::Instance().AutoAcceptWebOrders ? "Enabled" : "Disabled");
			Item4.Properties["Action"] = IntToStr(4);
			if( TGlobalSettings::Instance().AutoAcceptWebOrders )
			{
				Item4.Properties["Color"] = IntToStr(clGreen);
			}
			else
			{
				Item4.Properties["Color"] = IntToStr(clRed);
			}
			Item4.CloseSelection = true;
			SelectionForm->Items.push_back(Item4);



			SelectionForm->ShowModal();
			TVerticalSelection SelectedItem;
			if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
			{
				int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
				switch(Action)
				{
				case 1 :
					{
						// Display Launch List
						std::auto_ptr<TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

						TVerticalSelection Item;
						Item.Title = "Cancel";
						Item.Properties["Color"] = IntToStr(clMaroon);
						Item.CloseSelection = true;
						SelectionForm1->Items.push_back(Item);

						TVerticalSelection Item1;
						Item1.Title = "Enable";
						Item1.Properties["Action"] = IntToStr(1);
						Item1.Properties["Color"] = IntToStr(clGreen);
						Item1.CloseSelection = true;
						SelectionForm1->Items.push_back(Item1);

						TVerticalSelection Item2;
						Item2.Title = "Disable";
						Item2.Properties["Action"] = IntToStr(2);
						Item2.Properties["Color"] = IntToStr(clRed);
						Item2.CloseSelection = true;
						SelectionForm1->Items.push_back(Item2);

						SelectionForm1->ShowModal();
						TVerticalSelection SelectedItem1;
						if(SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel" )
						{
							int Action = StrToIntDef(SelectedItem1.Properties["Action"],0);
							switch(Action)
							{
							case 1 :
								{
									TGlobalSettings::Instance().WebMateEnabled = true;

									//If there is no WebMate Device Create One.
									std::auto_ptr<TDeviceWeb> WebDevice(new TDeviceWeb());
									WebDevice->ID.Product = "WebMate";
									WebDevice->ID.Name = "WebMate";
									WebDevice->ID.Type = devPC;
									WebDevice->ID.LocationKey = TDeviceRealTerminal::Instance().ID.LocationKey;
									DBTransaction.StartTransaction();
									if(WebDevice->Locate(DBTransaction) == 0)
									{
										WebDevice->ID.ProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eTerminalProfiles,WebDevice->ID.Name);
										WebDevice->Create(DBTransaction);
									}
									DBTransaction.Commit();

								}  break;
							case 2 :
								{
									TGlobalSettings::Instance().WebMateEnabled = false;
								}  break;
							}

							DBTransaction.StartTransaction();
							TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmWebMateEnabled,TGlobalSettings::Instance().WebMateEnabled);
							DBTransaction.Commit();

							//::::::::::::::::::::::::::::::::::::::::::::::::

							ResetWebMate();
							RefreshWebMateBtnColor();
						}
					}  break;
				case 2 :
					{
						std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
						frmTouchNumpad->Caption = "Enter Port for WebMate to Listen on (30000).";
						frmTouchNumpad->btnSurcharge->Caption = "Ok";
						frmTouchNumpad->btnSurcharge->Visible = true;
						frmTouchNumpad->btnDiscount->Visible = false;
						frmTouchNumpad->Mode = pmNumber;
						frmTouchNumpad->INTInitial = TGlobalSettings::Instance().WebMatePort;
						if (frmTouchNumpad->ShowModal() == mrOk)
						{
							TGlobalSettings::Instance().WebMatePort = frmTouchNumpad->INTResult;
							DBTransaction.StartTransaction();
							TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmWebMatePort,TGlobalSettings::Instance().WebMatePort);
							DBTransaction.Commit();
							MessageBox("MenuMate Restart Required.", "Info", MB_OK + MB_ICONINFORMATION);
						}
					}  break;
				case 3 :
					{
						// Display Launch List
						std::auto_ptr<TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

						TVerticalSelection Item;
						Item.Title = "Cancel";
						Item.Properties["Color"] = IntToStr(clMaroon);
						Item.CloseSelection = true;
						SelectionForm1->Items.push_back(Item);

						TVerticalSelection Item1;
						Item1.Title = "Enable";
						Item1.Properties["Action"] = IntToStr(1);
						Item1.Properties["Color"] = IntToStr(clGreen);
						Item1.CloseSelection = true;
						SelectionForm1->Items.push_back(Item1);

						TVerticalSelection Item2;
						Item2.Title = "Disable";
						Item2.Properties["Action"] = IntToStr(2);
						Item2.Properties["Color"] = IntToStr(clRed);
						Item2.CloseSelection = true;
						SelectionForm1->Items.push_back(Item2);

						SelectionForm1->ShowModal();
						TVerticalSelection SelectedItem1;
						if(SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel" )
						{
							int Action = StrToIntDef(SelectedItem1.Properties["Action"],0);
							switch(Action)
							{
							case 1 :
								{
									TGlobalSettings::Instance().AutoAddWebMembers = true;
								}  break;
							case 2 :
								{
									TGlobalSettings::Instance().AutoAddWebMembers = false;
								}  break;
							}

							DBTransaction.StartTransaction();
							TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAutoAddWebMembers,TGlobalSettings::Instance().WebMateEnabled);
							DBTransaction.Commit();

							//::::::::::::::::::::::::::::::::::::::::::::::::

							ResetWebMate();
							RefreshWebMateBtnColor();
						}
					} break;
				case 4 :
					{
						// Display Launch List
						std::auto_ptr<TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

						TVerticalSelection Item;
						Item.Title = "Cancel";
						Item.Properties["Color"] = IntToStr(clMaroon);
						Item.CloseSelection = true;
						SelectionForm1->Items.push_back(Item);

						TVerticalSelection Item1;
						Item1.Title = "Enable";
						Item1.Properties["Action"] = IntToStr(1);
						Item1.Properties["Color"] = IntToStr(clGreen);
						Item1.CloseSelection = true;
						SelectionForm1->Items.push_back(Item1);

						TVerticalSelection Item2;
						Item2.Title = "Disable";
						Item2.Properties["Action"] = IntToStr(2);
						Item2.Properties["Color"] = IntToStr(clRed);
						Item2.CloseSelection = true;
						SelectionForm1->Items.push_back(Item2);

						SelectionForm1->ShowModal();
						TVerticalSelection SelectedItem1;
						if(SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel" )
						{
							int Action = StrToIntDef(SelectedItem1.Properties["Action"],0);
							switch(Action)
							{
							case 1 :
								{
									TGlobalSettings::Instance().AutoAcceptWebOrders = true;
								}  break;
							case 2 :
								{
									TGlobalSettings::Instance().AutoAcceptWebOrders = false;
								}  break;
							}

							DBTransaction.StartTransaction();
							TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAutoAcceptWebOrders,TGlobalSettings::Instance().AutoAcceptWebOrders);
							DBTransaction.Commit();
							//::::::::::::::::::::::::::::::::::::::::::::::::

							ResetWebMate();
							RefreshWebMateBtnColor();
						}
					} break;
				}
			}
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access to the IntaMate Interface settings.", "Error", MB_OK + MB_ICONERROR);
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
		}
	}
	else
	{
		MessageBox("You are not registered for WebMate", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::btnGUIMouseClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();


	if (Result == lsAccepted)
	{
		std::auto_ptr<TfrmGroupGUI>(GroupGUI)(TfrmGroupGUI::Create<TfrmGroupGUI>(this,TDeviceRealTerminal::Instance().DBControl));
		GroupGUI->ShowModal();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the miscellaneous settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}

void __fastcall TfrmMaintain::btnSkimMouseclick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		std::auto_ptr<TfrmMessageMaintenance>(frmMessageMaintenance)(TfrmMessageMaintenance::Create<TfrmMessageMaintenance>(this,TDeviceRealTerminal::Instance().DBControl));

		frmMessageMaintenance->MessageType = eSkimRefloat;
		frmMessageMaintenance->ShowModal();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the messages settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::WriteOffMouseClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		std::auto_ptr<TfrmMessageMaintenance>(frmMessageMaintenance)(TfrmMessageMaintenance::Create<TfrmMessageMaintenance>(this,TDeviceRealTerminal::Instance().DBControl));

		frmMessageMaintenance->MessageType = eWriteOff;
		frmMessageMaintenance->ShowModal();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the messages settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::TouchBtnReservationsMouseClick(TObject *Sender)
{
	if (TDeviceRealTerminal::Instance().Modules.Status[eReservations]["Registered"])
	{
		TMMContactInfo TempUserInfo;
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
		TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckMaintenance);
		DBTransaction.Commit();
		if (Result == lsAccepted)
		{
			// Display Launch List
			std::auto_ptr <TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create <TfrmVerticalSelect> (this));

			TVerticalSelection Item;
			Item.Title = "Cancel";
			Item.Properties["Color"] = IntToStr(clMaroon);
			Item.CloseSelection = true;
			SelectionForm->Items.push_back(Item);

			TVerticalSelection Item1;
			Item1.Title = UnicodeString("Enable/Disable \r") + UnicodeString
			((TGlobalSettings::Instance().ReservationsEnabled ? "Enabled" : "Disabled"));
			Item1.Properties["Action"] = IntToStr(1);

			// :::::::::::::::::::::::::::::::::::::::

			// Item1.Properties["Color"] = IntToStr(clNavy);

			if (TGlobalSettings::Instance().ReservationsEnabled)
			{
				Item1.Properties["Color"] = IntToStr(clGreen);
			}
			else
			{
				Item1.Properties["Color"] = IntToStr(clRed);
			}

			// :::::::::::::::::::::::::::::::::::::::

			Item1.CloseSelection = true;
			SelectionForm->Items.push_back(Item1);

			TVerticalSelection Item2;
			Item2.Title = "Service URL \r" + TGlobalSettings::Instance().ReservationsAddress;
			Item2.Properties["Action"] = IntToStr(2);
			Item2.Properties["Color"] = IntToStr(clNavy);
			Item2.CloseSelection = true;
			SelectionForm->Items.push_back(Item2);

			TVerticalSelection Item3;
			Item3.Title = "Transparent Tables \r" + UnicodeString
			(TGlobalSettings::Instance().FloorPlanTransparentTables ? "Enabled" : "Disabled");
			Item3.Properties["Action"] = IntToStr(3);
			Item3.Properties["Color"] = IntToStr((TGlobalSettings::Instance().FloorPlanTransparentTables ? clGreen : clNavy));
			Item3.CloseSelection = true;
			SelectionForm->Items.push_back(Item3);

			TVerticalSelection Item4;
			Item4.Title = "Table Colour \r";
			Item4.Properties["Action"] = IntToStr(4);
			Item4.Properties["Color"] = IntToStr(TGlobalSettings::Instance().FloorPlanTableColour);
			Item4.CloseSelection = true;
			SelectionForm->Items.push_back(Item4);

			TVerticalSelection Item5;
			Item5.Title = "Text Colour \r";
			Item5.Properties["Action"] = IntToStr(5);
			Item5.Properties["Color"] = IntToStr(TGlobalSettings::Instance().FloorPlanTextColour);
			Item5.CloseSelection = true;
			SelectionForm->Items.push_back(Item5);

			SelectionForm->ShowModal();
			TVerticalSelection SelectedItem;
			if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
			{
				int Action = StrToIntDef(SelectedItem.Properties["Action"], 0);
				switch(Action)
				{
				case 1:
					{
						// Display Launch List
						std::auto_ptr <TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create <TfrmVerticalSelect> (this));

						TVerticalSelection Item;
						Item.Title = "Cancel";
						Item.Properties["Color"] = IntToStr(clMaroon);
						Item.CloseSelection = true;
						SelectionForm1->Items.push_back(Item);

						TVerticalSelection Item1;
						Item1.Title = "Enable";
						Item1.Properties["Action"] = IntToStr(1);
						Item1.Properties["Color"] = IntToStr(clGreen);
						Item1.CloseSelection = true;
						SelectionForm1->Items.push_back(Item1);

						TVerticalSelection Item2;
						Item2.Title = "Disable";
						Item2.Properties["Action"] = IntToStr(2);
						Item2.Properties["Color"] = IntToStr(clRed);
						Item2.CloseSelection = true;
						SelectionForm1->Items.push_back(Item2);

						SelectionForm1->ShowModal();
						TVerticalSelection SelectedItem1;
						if (SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel")
						{
							int Action = StrToIntDef(SelectedItem1.Properties["Action"], 0);
							switch(Action)
							{
							case 1:
								{
									TGlobalSettings::Instance().ReservationsEnabled = true;
								}break;
							case 2:
								{
									TGlobalSettings::Instance().ReservationsEnabled = false;
								}break;
							}

							DBTransaction.StartTransaction();
							TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmReservationsEnabled,
							TGlobalSettings::Instance().ReservationsEnabled);
							DBTransaction.Commit();

							// ::::::::::::::::::::::::::::::::::::::::::::::::
							RefreshReservationBtnColor();
						}
					}break;
				case 2:
					{
						std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
						frmTouchKeyboard->MaxLength = 255;
						frmTouchKeyboard->AllowCarriageReturn = false;
						frmTouchKeyboard->StartWithShiftDown = false;
						frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().ReservationsAddress;
						frmTouchKeyboard->Caption = "Enter URL Address For Floor Plan System.";
						if (frmTouchKeyboard->ShowModal() == mrOk)
						{
							TGlobalSettings::Instance().ReservationsAddress = frmTouchKeyboard->KeyboardText;
							DBTransaction.StartTransaction();
							TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmReservationsAddress,
							TGlobalSettings::Instance().ReservationsAddress);
							DBTransaction.Commit();
						}
					}break;
				case 3:
					{
						// Display Launch List
						std::auto_ptr <TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create <TfrmVerticalSelect> (this));

						TVerticalSelection Item;
						Item.Title = "Cancel";
						Item.Properties["Color"] = IntToStr(clMaroon);
						Item.CloseSelection = true;
						SelectionForm1->Items.push_back(Item);

						TVerticalSelection Item1;
						Item1.Title = "Enable";
						Item1.Properties["Action"] = IntToStr(1);
						Item1.Properties["Color"] = IntToStr(clGreen);
						Item1.CloseSelection = true;
						SelectionForm1->Items.push_back(Item1);

						TVerticalSelection Item2;
						Item2.Title = "Disable";
						Item2.Properties["Action"] = IntToStr(2);
						Item2.Properties["Color"] = IntToStr(clRed);
						Item2.CloseSelection = true;
						SelectionForm1->Items.push_back(Item2);

						SelectionForm1->ShowModal();
						TVerticalSelection SelectedItem1;
						if (SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel")
						{
							int Action = StrToIntDef(SelectedItem1.Properties["Action"], 0);
							switch(Action)
							{
							case 1:
								{
									TGlobalSettings::Instance().FloorPlanTransparentTables = true;
								}break;
							case 2:
								{
									TGlobalSettings::Instance().FloorPlanTransparentTables = false;
								}break;
							}

							DBTransaction.StartTransaction();
							TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmFloorPlanTransparentTables,
							TGlobalSettings::Instance().FloorPlanTransparentTables);
							DBTransaction.Commit();

							// ::::::::::::::::::::::::::::::::::::::::::::::::
							RefreshReservationBtnColor();
						}
					} break;
				case 4:
					{
						std::auto_ptr <TfrmColourPicker> FrmColour(TfrmColourPicker::Create <TfrmColourPicker> (this));
						FrmColour->SelectedColour = TGlobalSettings::Instance().FloorPlanTableColour;
						if (FrmColour->ShowModal() == mrOk)
						{
							TGlobalSettings::Instance().FloorPlanTableColour = FrmColour->SelectedColour;
							DBTransaction.StartTransaction();
							TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmFloorPlanTableColour,
							TGlobalSettings::Instance().FloorPlanTableColour);
							DBTransaction.Commit();
						}
					} break;
				case 5:
					{
						std::auto_ptr <TfrmColourPicker> FrmColour(TfrmColourPicker::Create <TfrmColourPicker> (this));
						FrmColour->SelectedColour = TGlobalSettings::Instance().FloorPlanTextColour;
						if (FrmColour->ShowModal() == mrOk)
						{
							TGlobalSettings::Instance().FloorPlanTextColour = FrmColour->SelectedColour;
							DBTransaction.StartTransaction();
							TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmFloorPlanTextColour,
							TGlobalSettings::Instance().FloorPlanTextColour);
							DBTransaction.Commit();
						}
					} break;
				}
				if (TGlobalSettings::Instance().ReservationsEnabled)
				btnChangeTable->Caption = "Edit Tables";
				else btnChangeTable->Caption = "Table Names";
			}
			else if (Result == lsDenied)
			{
				MessageBox("You do not have access to the Floor Plan Interface settings.", "Error", MB_OK + MB_ICONERROR);
			}
			else if (Result == lsPINIncorrect)
			{
				MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
			}
		}
		else
		{
			MessageBox("You are not registered for Floor Plan", "Error", MB_OK + MB_ICONERROR);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::RefreshReservationBtnColor()
{
	if (TDeviceRealTerminal::Instance().Modules.Status[eReservations]["Registered"])
	{
		if (TGlobalSettings::Instance().ReservationsEnabled)
		{
			TouchBtnReservations->ButtonColor = clGreen;

			TouchBtnReservations->Caption = "Floor Plan \r[Enabled]";
		}
		else
		{
			TouchBtnReservations->ButtonColor = clRed;

			TouchBtnReservations->Caption = "Floor Plan \r[Disabled]";
		}
	}
	else
	{
		TouchBtnReservations->ButtonColor = clRed;
		TouchBtnReservations->Caption = "Floor Plan \r[Unregistered]";
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::TouchBtnLoyaltyMateMouseClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		while(DisplayLoyaltyMateSettings(DBTransaction) != false);
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the LoyaltyMate interface settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
bool TfrmMaintain::DisplayLoyaltyMateSettings(Database::TDBTransaction &DBTransaction)
{
	bool keepFormAlive = true;

	// Display Launch List
	std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

	TVerticalSelection Item;
	Item.Title = "Cancel";
	Item.Properties["Color"] = "0x000098F5";
	Item.Properties["FontColor"] = IntToStr(clWhite);
	Item.CloseSelection = true;
	SelectionForm->Items.push_back(Item);

	TVerticalSelection Item1;
	Item1.Title = UnicodeString("Enable/Disable \r") + UnicodeString((TGlobalSettings::Instance().LoyaltyMateEnabled ? "Enabled" : "Disabled"));
	Item1.Properties["Action"] = IntToStr(1);
	if( TGlobalSettings::Instance().LoyaltyMateEnabled )
	{
		Item1.Properties["Color"] = IntToStr(clGreen);
	}
	else
	{
		Item1.Properties["Color"] = IntToStr(clRed);
	}
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

    TVerticalSelection Item2;
	Item2.Title = UnicodeString("Gift Card Validation");
	Item2.Properties["Action"] = IntToStr(2);
    Item2.Properties["Color"] = IntToStr(clNavy);
    Item2.CloseSelection = true;
    SelectionForm->Items.push_back(Item2);

	SelectionForm->ShowModal();
	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
	{
		int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
		switch(Action)
		{
		  case 1 :
			{
				std::auto_ptr<TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

				TVerticalSelection Item;
				Item.Title = "Cancel";
				Item.Properties["Color"] = "0x000098F5";
				Item.Properties["FontColor"] = IntToStr(clWhite);;
				Item.CloseSelection = true;
				SelectionForm1->Items.push_back(Item);

				TVerticalSelection Item1;
				Item1.Title = "Enable";
				Item1.Properties["Action"] = IntToStr(1);
				Item1.Properties["Color"] = IntToStr(clGreen);
				Item1.CloseSelection = true;
				SelectionForm1->Items.push_back(Item1);

				TVerticalSelection Item2;
				Item2.Title = "Disable";
				Item2.Properties["Action"] = IntToStr(2);
				Item2.Properties["Color"] = IntToStr(clRed);
				Item2.CloseSelection = true;
				SelectionForm1->Items.push_back(Item2);

				SelectionForm1->ShowModal();
				TVerticalSelection SelectedItem1;
				if(SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel" )
				{
					int Action = StrToIntDef(SelectedItem1.Properties["Action"],0);
					switch(Action)
					{
					case 1 :
						TGlobalSettings::Instance().LoyaltyMateEnabled = true;
						break;
					case 2 :
						TGlobalSettings::Instance().LoyaltyMateEnabled = false;
						break;
					}

					DBTransaction.StartTransaction();
					TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmLoyaltyMateEnabled,TGlobalSettings::Instance().LoyaltyMateEnabled);
                    TGlobalSettings::Instance().UseMemberSubs = false;
                    TManagerVariable &mv = TManagerVariable::Instance();

                    int pk;
                    if (!(pk = mv.GetProfile(DBTransaction, eSystemProfiles, "Globals")))
                    pk = mv.SetProfile(DBTransaction, eSystemProfiles, "Globals");
                    mv.SetProfileBool(DBTransaction, pk, vmUseMemberSubs, TGlobalSettings::Instance().UseMemberSubs);
                    DBTransaction.Commit();
					RefreshLoyaltyMateBtnColor();
				}
			}  break;
            case 2 :
			{
               ManageGiftCardValidations(DBTransaction);
            }  break;
		}
	}
	else
	  keepFormAlive = false;

	return keepFormAlive;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::RefreshLoyaltyMateBtnColor()
{
    if (TGlobalSettings::Instance().LoyaltyMateEnabled)
	{
		TouchBtnLoyaltyMate->ButtonColor = clGreen;

		TouchBtnLoyaltyMate->Caption = "Loyalty Mate \r[Enabled]";
	}
	else
	{
		TouchBtnLoyaltyMate->ButtonColor = clRed;

		TouchBtnLoyaltyMate->Caption = "Loyalty Mate \r[Disabled]";
	}
}

void TfrmMaintain::ManageGiftCardValidations(Database::TDBTransaction &DBTransaction)
{
    std::auto_ptr<TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

    TVerticalSelection Item;
    Item.Title = "Cancel";
    Item.Properties["Color"] = "0x000098F5";
    Item.Properties["FontColor"] = IntToStr(clWhite);;
    Item.CloseSelection = true;
    SelectionForm1->Items.push_back(Item);

    TVerticalSelection Item1;
    Item1.Title = "Open Ended Validation";
    Item1.Properties["Action"] = IntToStr(0);
    Item1.Properties["Color"] = TGlobalSettings::Instance().GiftCardValidation == 0 ? IntToStr(clGreen) : IntToStr(clNavy);
    Item1.CloseSelection = true;
    SelectionForm1->Items.push_back(Item1);

    TVerticalSelection Item2;
    Item2.Title = "MSR Cards Only";
    Item2.Properties["Action"] = IntToStr(1);
    Item2.Properties["Color"] = TGlobalSettings::Instance().GiftCardValidation == 1 ? IntToStr(clGreen) : IntToStr(clNavy);;
    Item2.CloseSelection = true;
    SelectionForm1->Items.push_back(Item2);

    TVerticalSelection Item3;
    Item3.Title = "Cloud Validation";
    Item3.Properties["Action"] = IntToStr(2);
    Item3.Properties["Color"] = TGlobalSettings::Instance().GiftCardValidation == 2 ? IntToStr(clGreen) : IntToStr(clNavy);;
    Item3.CloseSelection = true;
    SelectionForm1->Items.push_back(Item3);


    SelectionForm1->ShowModal();
    TVerticalSelection SelectedItem1;
    if(SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel" )
    {
        TGlobalSettings::Instance().GiftCardValidation = StrToIntDef(SelectedItem1.Properties["Action"],0);
        DBTransaction.StartTransaction();
        TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmGiftCardValidation,TGlobalSettings::Instance().GiftCardValidation);
        DBTransaction.Commit();
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmMaintain::CustomerOrderTypesClick(TObject *Sender)
{
	CustomerOrderTypes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::tbtnDealsMouseClick(TObject *Sender)
{
	TLoginSuccess Result = CheckAccess(CheckMaintenance);
	if (Result == lsAccepted)
	{
        TfrmDeals *frmDeals = new TfrmDeals(this,TDeviceRealTerminal::Instance().DBControl);
		frmDeals->ShowModal();
        delete frmDeals;
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the deal settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
TLoginSuccess __fastcall TfrmMaintain::CheckAccess(SecurityCheck AccessCheck)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, AccessCheck);
	DBTransaction.Commit();

	return Result;
}
//---------------------------------------------------------------------------
void TfrmMaintain::CustomerOrderTypes(void)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		std::auto_ptr<TfrmMessageMaintenance>(frmMessageMaintenance)(TfrmMessageMaintenance::Create<TfrmMessageMaintenance>(this,TDeviceRealTerminal::Instance().DBControl));

		frmMessageMaintenance->MessageType = eCustomerTypes;
		frmMessageMaintenance->ShowModal();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the messages settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::tbMaintenanceClick(TObject *Sender)
{
	Pages->ActivePage = tsMaintenance;
	RedrawButtons(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::tbQuickMessagesClick(TObject *Sender)
{
	Pages->ActivePage = tsQuickMessages;
	RedrawButtons(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::tbInterfacesClick(TObject *Sender)
{
	Pages->ActivePage = tsInterfaces;
	RedrawButtons(Sender);
}
//---------------------------------------------------------------------------
void TfrmMaintain::RedrawButtons(TObject * Sender)
{
	TTouchBtn* CurrentButton = (TTouchBtn*) Sender;
	for (int i = 0; i < pnlButtons1->ControlCount ; i++)
	{
		TTouchBtn* Button = (TTouchBtn*)pnlButtons1->Controls[i];
		if(Button->Tag != -1 && Button != CurrentButton)
		{
			Button->Latched = false;
		}
		else
		{
			Button->Latched = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::tbtnMiscSettingsMouseClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		std::auto_ptr<TfrmGeneralMaintenance>(GeneralMaintenance)(TfrmGeneralMaintenance::Create<TfrmGeneralMaintenance>(this,TDeviceRealTerminal::Instance().DBControl));
		GeneralMaintenance->ShowModal();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the miscellaneous settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::tbtnTaxSettingsMouseClick(TObject *Sender)
{
	//Setting for tax
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
        TfrmTaxMaintenance *frmTaxMaintenance = new TfrmTaxMaintenance(this,TDeviceRealTerminal::Instance().DBControl);
        //std::auto_ptr<TfrmTaxMaintenance>(frmTaxMaintenance)(TfrmTaxMaintenance::Create<TfrmTaxMaintenance>(this,TDeviceRealTerminal::Instance().DBControl));
		frmTaxMaintenance->ShowModal();
        delete frmTaxMaintenance;
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the tax settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::btnTierLevelsMouseClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckAccountManager);
	DBTransaction.Commit();
	if(Result == lsAccepted)
	{
		std::auto_ptr<TfrmTierLevel>(frmTierLevel)(TfrmTierLevel::Create<TfrmTierLevel>(this,TDeviceRealTerminal::Instance().DBControl));
		frmTierLevel->ShowModal();
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result != lsCancel && TempUserInfo.AccessLevel != mmAdmin)
	{
		MessageBox("You do not have access to the tier levels.", "Error", MB_OK + MB_ICONERROR);
	}

}

void __fastcall TfrmMaintain::TouchBtnBarExchangeMouseClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		while(DisplayBarExchangeSettings(DBTransaction) != false);
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the LoyaltyMate interface settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}

TLoginSuccess TfrmMaintain::VerifyUserAuthorization()
{
    TMMContactInfo currentUser;

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();

	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess loginResult = Staff->Login(this, DBTransaction, currentUser, CheckMaintenance);

    DBTransaction.Commit();
    return loginResult;
}

void __fastcall TfrmMaintain::tchbtnDrinkCommandMouseClick(TObject *Sender)
{
    //Will handle drink command interface
    if ( TDeviceRealTerminal::Instance().IMManager->Registered)
    {
        TLoginSuccess Result = VerifyUserAuthorization();

        if ((Result == lsAccepted))
        {
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            while(ShowDrinkCommandSettings(DBTransaction) != false);
        }
        else if (Result == lsDenied)
        {
            MessageBox("You do not have access to the DrinkCommand interface settings.", "Error", MB_OK + MB_ICONERROR);
        }
        else if (Result == lsPINIncorrect)
        {
            MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
        }
    }
}

void __fastcall TfrmMaintain::TouchBtnThorlinkClick(TObject *Sender)
{
    TLoginSuccess Result = VerifyUserAuthorization();

    if ((Result == lsAccepted))
    {
       Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
       while(ShowThorlinkSettings(DBTransaction) != false);
    }
    else if (Result == lsDenied)
    {
       MessageBox("You do not have access to the Thorlink interface settings.", "Error", MB_OK + MB_ICONERROR);
    }
    else if (Result == lsPINIncorrect)
    {
       MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
    }
}

bool TfrmMaintain::ShowThorlinkSettings(Database::TDBTransaction & DBTransaction)
{
    SaveServerCheck();
	bool keepFormAlive = true;

    //Create the selection form to set the setting values..
    std::auto_ptr<TfrmVerticalSelect> frmThorlinkSettingsSelection(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

    //Create Title Item to be added to the selection form..
	TVerticalSelection titleItem = CreateSelectionItem("Cancel", "0x000098F5", true, clWhite, 0);
	frmThorlinkSettingsSelection->Items.push_back(titleItem);
//
    //Create Enable/Disable Item to be added to the selection form..
    TVerticalSelection enableDisableItem = CreateSelectionItem(UnicodeString("Enable/Disable \r") + UnicodeString((TGlobalSettings::Instance().IsThorlinkEnabled ? "Enabled" : "Disabled")),
                                                 TGlobalSettings::Instance().IsThorlinkEnabled ? IntToStr(clGreen) : IntToStr(clRed),
                                                 true, 0, 1);
    frmThorlinkSettingsSelection->Items.push_back(enableDisableItem);

    //Create App Key Item to be added to the selection form..
    TVerticalSelection appKeyItem = CreateSelectionItem("App Key. \r" + TGlobalSettings::Instance().ThorlinkAppKey, IntToStr(clNavy),
                                                true, 0, 2);
    frmThorlinkSettingsSelection->Items.push_back(appKeyItem);

    //Create Site Number Item to be added to the selection form..
    TVerticalSelection siteNoItem = CreateSelectionItem("Site No. \r" + TGlobalSettings::Instance().ThorlinkSiteNo, IntToStr(clNavy),
                                                true, 0, 3);
    frmThorlinkSettingsSelection->Items.push_back(siteNoItem);

    //Create Server Path Item to be added to the selection form..
    TVerticalSelection merchantCodeItem = CreateSelectionItem("Merchant Code. \r" + TGlobalSettings::Instance().ThorlinkMerchantCode, IntToStr(clNavy),
                                                true, 0, 4);
    frmThorlinkSettingsSelection->Items.push_back(merchantCodeItem);

    //Create Server Port Item to be added to the selection form..
    TVerticalSelection deviceCodeItem = CreateSelectionItem("Device Code. \r" + TGlobalSettings::Instance().ThorlinkDeviceCode, IntToStr(clNavy),
                                                true, 0, 5);
    frmThorlinkSettingsSelection->Items.push_back(deviceCodeItem);

//    //Show the pop up for display to user..
    frmThorlinkSettingsSelection->ShowModal();
//
    //Get the selection from user in order to persist the values..
    TVerticalSelection selectedItem;
    if (frmThorlinkSettingsSelection->GetFirstSelectedItem(selectedItem) && selectedItem.Title != "Cancel")
    {
        int action = StrToIntDef(selectedItem.Properties["Action"], 0);
        switch(action)
        {
            case 1:
            {
                // Display Launch List for allowing user to enable or disable the setting..
                std::auto_ptr <TfrmVerticalSelect> enableDisableSelectionForm(TfrmVerticalSelect::Create <TfrmVerticalSelect> (this));
//
                TVerticalSelection cancelSubItem = CreateSelectionItem("Cancel", IntToStr(clMaroon), true, 0, 0);
                enableDisableSelectionForm->Items.push_back(cancelSubItem);

                TVerticalSelection enableSubItem = CreateSelectionItem("Enable", IntToStr(clGreen), true, 0, 1);
                enableDisableSelectionForm->Items.push_back(enableSubItem);

                TVerticalSelection disableSubItem = CreateSelectionItem("Disable", IntToStr(clRed), true, 0, 2);
                enableDisableSelectionForm->Items.push_back(disableSubItem);

                //Get the selection from user in order to persist the values..
                enableDisableSelectionForm->ShowModal();

                TVerticalSelection subSelectedItem;
                if (enableDisableSelectionForm->GetFirstSelectedItem(subSelectedItem) && subSelectedItem.Title != "Cancel")
                {
                    int subAction = StrToIntDef(subSelectedItem.Properties["Action"], 0);
                    switch(subAction)
                    {
                        case 1:
                        {
                            if(TGlobalSettings::Instance().IsThorlinkSelected)
                            {
                               TGlobalSettings::Instance().IsThorlinkEnabled = true;
                            }
                            else
                            {
                              MessageBox("Please Select Thorlink Membership first.", "Warning", MB_ICONWARNING + MB_OK);
                              TGlobalSettings::Instance().IsThorlinkEnabled = false;
                            }
                        }
                        break;
                        case 2:
                        {
                            TGlobalSettings::Instance().IsThorlinkEnabled = false;
                        }
                        break;
                    }

                    DBTransaction.StartTransaction();
                    TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmIsThorlinkEnabled,
                                                                    TGlobalSettings::Instance().IsThorlinkEnabled);
                    DBTransaction.Commit();
                    RefreshThorlinkButtonColor();
                }
            }
            break;
            case 2:
            {
                std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
                frmTouchKeyboard->MaxLength = 255;
                frmTouchKeyboard->AllowCarriageReturn = false;
                frmTouchKeyboard->StartWithShiftDown = false;
                frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().ThorlinkAppKey;
                frmTouchKeyboard->Caption = "Enter App Key";

                if (frmTouchKeyboard->ShowModal() == mrOk)
                {
                    if(frmTouchKeyboard->KeyboardText=="")
                    {
                      MessageBox("Enter App key.", "Warning", MB_ICONWARNING + MB_OK);
                    }
                    else
                    {
                    TGlobalSettings::Instance().ThorlinkAppKey = frmTouchKeyboard->KeyboardText;
                    }

                    DBTransaction.StartTransaction();
                    TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmThorlinkAppKey,
                                                                    TGlobalSettings::Instance().ThorlinkAppKey);
                    DBTransaction.Commit();
                }
            }
            break;
            case 3:
            {
                std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
                frmTouchKeyboard->MaxLength = 255;
                frmTouchKeyboard->AllowCarriageReturn = false;
                frmTouchKeyboard->StartWithShiftDown = false;
                frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().ThorlinkSiteNo;
                frmTouchKeyboard->Caption = "Enter Site Number.";

                if (frmTouchKeyboard->ShowModal() == mrOk)
                {

                    if(frmTouchKeyboard->KeyboardText=="")
                    {
                      MessageBox("Enter Site No.", "Warning", MB_ICONWARNING + MB_OK);
                    }
                    else
                    {
                    TGlobalSettings::Instance().ThorlinkSiteNo = frmTouchKeyboard->KeyboardText;
                    }

                    DBTransaction.StartTransaction();
                    TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmThorlinkSiteNo,
                                                                    TGlobalSettings::Instance().ThorlinkSiteNo);
                    DBTransaction.Commit();
                }
            }
            break;
            case 4:
            {
                std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
                frmTouchKeyboard->MaxLength = 255;
                frmTouchKeyboard->AllowCarriageReturn = false;
                frmTouchKeyboard->StartWithShiftDown = false;
                frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().ThorlinkMerchantCode;
                frmTouchKeyboard->Caption = "Enter Merchant Code.";

                if (frmTouchKeyboard->ShowModal() == mrOk)
                {

                    if(frmTouchKeyboard->KeyboardText=="")
                    {
                      MessageBox("Enter Merchant Code.", "Warning", MB_ICONWARNING + MB_OK);
                    }
                    else
                    {
                    TGlobalSettings::Instance().ThorlinkMerchantCode = frmTouchKeyboard->KeyboardText;
                    }

                    DBTransaction.StartTransaction();
                    TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmThorlinkMerchantCode,
                                                                    TGlobalSettings::Instance().ThorlinkMerchantCode);
                    DBTransaction.Commit();
                }
            }
            break;
            case 5:
            {
                std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
                frmTouchKeyboard->MaxLength = 255;
                frmTouchKeyboard->AllowCarriageReturn = false;
                frmTouchKeyboard->StartWithShiftDown = false;
                frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().ThorlinkDeviceCode;
                frmTouchKeyboard->Caption = "Enter Device Code.";

                if (frmTouchKeyboard->ShowModal() == mrOk)
                {

                    if(frmTouchKeyboard->KeyboardText=="")
                    {
                      MessageBox("Enter Device Code.", "Warning", MB_ICONWARNING + MB_OK);
                    }
                    else
                    {
                    TGlobalSettings::Instance().ThorlinkDeviceCode = frmTouchKeyboard->KeyboardText;
                    }

                    DBTransaction.StartTransaction();
                    TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmThorlinkDeviceCode,
                                                                    TGlobalSettings::Instance().ThorlinkDeviceCode);
                    DBTransaction.Commit();
                }
            }
        }
    }
    else
	    keepFormAlive = false;

    return keepFormAlive;
}

void __fastcall TfrmMaintain::RefreshThorlinkButtonColor()
{

    if ( TDeviceRealTerminal::Instance().IMManager->Registered)
    {
        if (TDeviceRealTerminal::Instance().Modules.Status[eReservations]["Registered"])
        {
            if ((TGlobalSettings::Instance().IsThorlinkEnabled) && (TGlobalSettings::Instance().IsThorlinkSelected))
            {
                TouchBtnThorlink->ButtonColor = clGreen;
                TouchBtnThorlink->Caption = "Thorlink \r[Enabled]";
            }
            else
            {
                TouchBtnThorlink->ButtonColor = clRed;
                TouchBtnThorlink->Caption = "Thorlink \r[Disabled]";
            }
        }
        else
        {
            TouchBtnThorlink->ButtonColor = clRed;
            TouchBtnThorlink->Caption = "Thorlink \r[Unregistered]";
        }
    }
    else
    {
		TouchBtnThorlink->Caption = "Thorlink \r[Unregistered]";
        TouchBtnThorlink->Enabled = TDeviceRealTerminal::Instance().IMManager->Registered;
    }

}

TVerticalSelection TfrmMaintain::CreateSelectionItem(UnicodeString title, UnicodeString color, bool isCloseSelectionAllowed, int fontColor,
                                                int action)
{
    TVerticalSelection subItem;

	subItem.Title = title;
	subItem.Properties["Color"] = color;
    subItem.CloseSelection = isCloseSelectionAllowed;

    if(fontColor != 0)
	    subItem.Properties["FontColor"] = IntToStr(fontColor);

    if(action != 0)
	    subItem.Properties["Action"] = IntToStr(action);

    return subItem;
}

bool TfrmMaintain::ShowDrinkCommandSettings(Database::TDBTransaction & DBTransaction)
{
    SaveServerCheck();
	bool keepFormAlive = true;

    //Create the selection form to set the setting values..
    std::auto_ptr<TfrmVerticalSelect> frmDrinkCommandSettingsSelection(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

    //Create Title Item to be added to the selection form..
	TVerticalSelection titleItem = CreateSelectionItem("Cancel", "0x000098F5", true, clWhite, 0);
	frmDrinkCommandSettingsSelection->Items.push_back(titleItem);

    //Create Enable/Disable Item to be added to the selection form..
    TVerticalSelection enableDisableItem = CreateSelectionItem(UnicodeString("Enable/Disable \r") + UnicodeString((TGlobalSettings::Instance().IsDrinkCommandEnabled ? "Enabled" : "Disabled")),
                                                 TGlobalSettings::Instance().IsDrinkCommandEnabled ? IntToStr(clGreen) : IntToStr(clRed),
                                                 true, 0, 1);
    frmDrinkCommandSettingsSelection->Items.push_back(enableDisableItem);

    //Create Server Path Item to be added to the selection form..
    TVerticalSelection serverPathItem = CreateSelectionItem("Server Path \r" + TGlobalSettings::Instance().DrinkCommandServerPath, IntToStr(clNavy),
                                                true, 0, 2);
    frmDrinkCommandSettingsSelection->Items.push_back(serverPathItem);

    //Create Server Port Item to be added to the selection form..
    TVerticalSelection portItem = CreateSelectionItem("Port No. \r" + IntToStr(TGlobalSettings::Instance().DrinkCommandServerPort), IntToStr(clNavy),
                                                true, 0, 3);
    frmDrinkCommandSettingsSelection->Items.push_back(portItem);

    //Create Get Product List Item to be added to the selection form..
    TVerticalSelection ProductList = CreateSelectionItem("Get Product List. \r" , IntToStr(clNavy),
                                                true, 0, 4);
    frmDrinkCommandSettingsSelection->Items.push_back(ProductList);

    //Show the pop up for display to user..
    frmDrinkCommandSettingsSelection->ShowModal();

    //Get the selection from user in order to persist the values..
    TVerticalSelection selectedItem;
    if (frmDrinkCommandSettingsSelection->GetFirstSelectedItem(selectedItem) && selectedItem.Title != "Cancel")
    {
        int action = StrToIntDef(selectedItem.Properties["Action"], 0);
        switch(action)
        {
            case 1:
            {
                // Display Launch List for allowing user to enable or disable the setting..
                std::auto_ptr <TfrmVerticalSelect> enableDisableSelectionForm(TfrmVerticalSelect::Create <TfrmVerticalSelect> (this));

                TVerticalSelection cancelSubItem = CreateSelectionItem("Cancel", IntToStr(clMaroon), true, 0, 0);
                enableDisableSelectionForm->Items.push_back(cancelSubItem);

                TVerticalSelection enableSubItem = CreateSelectionItem("Enable", IntToStr(clGreen), true, 0, 1);
                enableDisableSelectionForm->Items.push_back(enableSubItem);

                TVerticalSelection disableSubItem = CreateSelectionItem("Disable", IntToStr(clRed), true, 0, 2);
                enableDisableSelectionForm->Items.push_back(disableSubItem);

                //Get the selection from user in order to persist the values..
                enableDisableSelectionForm->ShowModal();

                TVerticalSelection subSelectedItem;
                if (enableDisableSelectionForm->GetFirstSelectedItem(subSelectedItem) && subSelectedItem.Title != "Cancel")
                {
                    int subAction = StrToIntDef(subSelectedItem.Properties["Action"], 0);
                    switch(subAction)
                    {
                        case 1:
                        {
                            TGlobalSettings::Instance().IsDrinkCommandEnabled = true;
                        }
                        break;
                        case 2:
                        {
                            TGlobalSettings::Instance().IsDrinkCommandEnabled = false;
                        }
                        break;
                    }

                    DBTransaction.StartTransaction();
                    TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmIsDrinkCommandEnabled,
                                                                    TGlobalSettings::Instance().IsDrinkCommandEnabled);
                    DBTransaction.Commit();
                    RefreshDrinkCommandButtonColor();
                }
            }
            break;
            case 2:
            {
                std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
                frmTouchKeyboard->MaxLength = 255;
                frmTouchKeyboard->AllowCarriageReturn = false;
                frmTouchKeyboard->StartWithShiftDown = false;
                frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().DrinkCommandServerPath;
                frmTouchKeyboard->Caption = "Enter Drink Server Path.";

                if (frmTouchKeyboard->ShowModal() == mrOk)
                {
                    TGlobalSettings::Instance().DrinkCommandServerPath = frmTouchKeyboard->KeyboardText;

                    DBTransaction.StartTransaction();
                    TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmDrinkCommandServerPath,
                                                                    TGlobalSettings::Instance().DrinkCommandServerPath);
                    DBTransaction.Commit();
                }
            }
            break;
            case 3:
            {
                std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
                frmTouchNumpad->Format = ffFixed;
                frmTouchNumpad->Mode = pmNumber;
                frmTouchNumpad->btnCancel->Visible = true;
                frmTouchNumpad->btnOk->Visible = true;
                frmTouchNumpad->INTInitial = TGlobalSettings::Instance().DrinkCommandServerPort;
                frmTouchNumpad->Caption = "Enter Drink Server Port (Default:15090).";

                if (frmTouchNumpad->ShowModal() == mrOk)
                {
                    TGlobalSettings::Instance().DrinkCommandServerPort = frmTouchNumpad->INTResult;

                    DBTransaction.StartTransaction();
                    TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmDrinkCommandServerPort,
                                                                    TGlobalSettings::Instance().DrinkCommandServerPort);
                    DBTransaction.Commit();
                }
            }
            break;
            case 4:
            {
                std::auto_ptr<TDrinkCommandManager> dcManager(
                                                    new TDrinkCommandManager());
                dcManager->SendRequestForProductList(TGlobalSettings::Instance().DrinkCommandServerPath,TGlobalSettings::Instance().DrinkCommandServerPort);
            }
            break;
        }
    }
    else
	    keepFormAlive = false;

    return keepFormAlive;
}

bool TfrmMaintain::DisplayBarExchangeSettings(Database::TDBTransaction &DBTransaction)
{
	SaveServerCheck();
	bool keepFormAlive = true;

	// Display Launch List
	std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

	TVerticalSelection Item;
	Item.Title = "Cancel";
	Item.Properties["Color"] = "0x000098F5";
	Item.Properties["FontColor"] = IntToStr(clWhite);
	Item.CloseSelection = true;
	SelectionForm->Items.push_back(Item);

	TVerticalSelection Item1;
	Item1.Title = UnicodeString("Enable/Disable \r") + UnicodeString((TGlobalSettings::Instance().BarExchangeEnabled ? "Enabled" : "Disabled"));
	Item1.Properties["Action"] = IntToStr(1);

	//:::::::::::::::::::::::::::::::::::::::

	if( TGlobalSettings::Instance().BarExchangeEnabled )
	{
		Item1.Properties["Color"] = IntToStr(clGreen);
	}
	else
	{
		Item1.Properties["Color"] = IntToStr(clRed);
	}

	//:::::::::::::::::::::::::::::::::::::::

	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	TVerticalSelection Item2;
	Item2.Title = "BAR EXCHANGE \r" + TGlobalSettings::Instance().BarExchangeSelect;
	Item2.Properties["Action"] = IntToStr(2);
	Item2.Properties["Color"] = IntToStr(clNavy);
	Item2.CloseSelection = true;
	SelectionForm->Items.push_back(Item2);

	//:::::::::::::::::::::::::::::::::::::::
	TVerticalSelection Item3;
	Item3.Title = UnicodeString("Bar Exchange Server \r") + UnicodeString((TGlobalSettings::Instance().BarExchangeServerEnabled ? "Enabled" : "Disabled"));
	Item3.Properties["Action"] = IntToStr(3);
	Item3.Properties["Enabled"] = ServerEnable;
	if(ServerEnable)
	{
		if( TGlobalSettings::Instance().BarExchangeServerEnabled  )
		{
			Item3.Properties["Color"] = IntToStr(clGreen);
		}
		else
		{
			Item3.Properties["Color"] = IntToStr(clRed);
		}
	}
	else
	{
		Item3.Title= UnicodeString("Server Connected! \r") ;
		Item3.Properties["Color"] = IntToStr(clGray);
	}
	Item3.CloseSelection = true;
	SelectionForm->Items.push_back(Item3);

	//:::::::::::::::::::::::::::::::::::::::


	TVerticalSelection Item4;

	Item4.Title = "BAR EXCHANGE PATH \r" + TGlobalSettings::Instance().BarExchangePath;
	Item4.Properties["Action"] = IntToStr(4);
	Item4.Properties["Color"] = IntToStr(clNavy);
	Item4.CloseSelection = true;
	SelectionForm->Items.push_back(Item4);



	SelectionForm->ShowModal();
	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
	{
		int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
		switch(Action)
		{
		case 1 :
			{
				std::auto_ptr<TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

				TVerticalSelection Item;
				Item.Title = "Cancel";
				Item.Properties["Color"] = "0x000098F5";
				Item.Properties["FontColor"] = IntToStr(clWhite);;
				Item.CloseSelection = true;
				SelectionForm1->Items.push_back(Item);

				TVerticalSelection Item1;
				Item1.Title = "Enable";
				Item1.Properties["Action"] = IntToStr(1);
				Item1.Properties["Color"] = IntToStr(clGreen);
				Item1.CloseSelection = true;
				SelectionForm1->Items.push_back(Item1);

				TVerticalSelection Item2;
				Item2.Title = "Disable";
				Item2.Properties["Action"] = IntToStr(2);
				Item2.Properties["Color"] = IntToStr(clRed);
				Item2.CloseSelection = true;
				SelectionForm1->Items.push_back(Item2);

				SelectionForm1->ShowModal();
				TVerticalSelection SelectedItem1;
				if(SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel" )
				{
					int Action = StrToIntDef(SelectedItem1.Properties["Action"],0);
					switch(Action)
					{
					case 1 :
						TGlobalSettings::Instance().BarExchangeEnabled = true;
						break;
					case 2 :
						TGlobalSettings::Instance().BarExchangeEnabled = false;
						break;
					}

					DBTransaction.StartTransaction();
					TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmBarExchangeEnabled,TGlobalSettings::Instance().BarExchangeEnabled);
					DBTransaction.Commit();

					//::::::::::::::::::::::::::::::::::::::::::::::::
					RefreshBarExchangeBtnColor();
				}
			}  break;
		case 2 :
			{
				std::auto_ptr<TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

				TVerticalSelection Item;
				Item.Title = "Cancel";
				Item.Properties["Color"] = "0x000098F5";
				Item.CloseSelection = true;
				SelectionForm1->Items.push_back(Item);

				TVerticalSelection Item1;
				Item1.Title = "None";
				Item1.Properties["Action"] = IntToStr(1);
				Item1.Properties["Color"] = IntToStr(clNavy);
				Item1.CloseSelection = true;
				SelectionForm1->Items.push_back(Item1);

				Item1.Title = "Barstock";
				Item1.Properties["Action"] = IntToStr(2);
				Item1.Properties["Color"] = IntToStr(clNavy);
				Item1.CloseSelection = true;
				SelectionForm1->Items.push_back(Item1);

				Item1.Title = "The Exchange";
				Item1.Properties["Action"] = IntToStr(3);
				Item1.Properties["Color"] = IntToStr(clNavy);
				Item1.CloseSelection = true;
				SelectionForm1->Items.push_back(Item1);

				SelectionForm1->ShowModal();
				TVerticalSelection SelectedItem1;
				if(SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel" )
				{
					int Action = StrToIntDef(SelectedItem1.Properties["Action"],0);
					switch(Action)
					{
					case 1 :
						TGlobalSettings::Instance().BarExchangeSelect = "None";
						break;
					case 2 :
						TGlobalSettings::Instance().BarExchangeSelect = "BarStock";
						break;
					case 3 :
						TGlobalSettings::Instance().BarExchangeSelect = "The Exchange";
						break;
					}

					DBTransaction.StartTransaction();
					TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmBarExchangeSelect,TGlobalSettings::Instance().BarExchangeSelect);
					DBTransaction.Commit();

					//::::::::::::::::::::::::::::::::::::::::::::::::
					RefreshBarExchangeBtnColor();
				}
			}  break;

			//---------------------------------------------------------------
		case 3 :
			{
				if(ServerEnable)
				{
					std::auto_ptr<TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

					TVerticalSelection Item1;
					Item1.Title = "Enable";
					Item1.Properties["Action"] = IntToStr(1);
					Item1.Properties["Color"] = IntToStr(clGreen);
					Item1.CloseSelection = true;
					SelectionForm1->Items.push_back(Item1);

					TVerticalSelection Item2;
					Item2.Title = "Disable";
					Item2.Properties["Action"] = IntToStr(2);
					Item2.Properties["Color"] = IntToStr(clRed);
					Item2.CloseSelection = true;
					SelectionForm1->Items.push_back(Item2);

					SelectionForm1->ShowModal();
					TVerticalSelection SelectedItem1;
					if(SelectionForm1->GetFirstSelectedItem(SelectedItem1) )
					{
						int Action = StrToIntDef(SelectedItem1.Properties["Action"],0);
						switch(Action)
						{
						case 1 :
							TGlobalSettings::Instance().BarExchangeServerEnabled = true;
							break;
						case 2 :
							TGlobalSettings::Instance().BarExchangeServerEnabled = false;
							break;
						}

						DBTransaction.StartTransaction();
						TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmBarExchangeServerEnabled,TGlobalSettings::Instance().BarExchangeServerEnabled);
						DBTransaction.Commit();
						//::::::::::::::::::::::::::::::::::::::::::::::::

					}
				}
				else
				{
					TGlobalSettings::Instance().BarExchangeServerEnabled = false;
					DBTransaction.StartTransaction();
					TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmBarExchangeServerEnabled,TGlobalSettings::Instance().BarExchangeServerEnabled);
					DBTransaction.Commit();


				}


			}
			break;
			//---------------------------------------------------------------


		case 4 :
			{

				String Dir;
				if((TGlobalSettings::Instance().BarExchangePath) !="")
				{
					Dir=TGlobalSettings::Instance().BarExchangePath;
				}
				else
				{Dir = ExtractFilePath( Application->ExeName );
				}

				//if (SelectDirectory(Dir, TSelectDirOpts() << sdAllowCreate << sdPerformCreate << sdPrompt,SELDIRHELP))
				if(SelectDirectory("Select a folder:", "", Dir))
				{
					TGlobalSettings::Instance().BarExchangePath=Dir;
					DBTransaction.StartTransaction();
					TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmBarExchangePath,TGlobalSettings::Instance().BarExchangePath);
					DBTransaction.Commit();
				}

			} break;

		}
	}
	else
	keepFormAlive = false;



	return keepFormAlive;
}
//-----------------------------------------------------------------------------
bool TfrmMaintain::DisplayRunRateSettingsOnly(Database::TDBTransaction &DBTransaction)
{
   bool keepFormAlive = true;

	// Display Launch List
	std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

	TVerticalSelection Item;
	Item.Title = "Cancel";
	Item.Properties["Color"] = "0x000098F5";
	Item.Properties["FontColor"] = IntToStr(clWhite);
	Item.CloseSelection = true;
	SelectionForm->Items.push_back(Item);

    TVerticalSelection Item1;
	Item1.Title = UnicodeString("Enable/Disable \r") + UnicodeString((TGlobalSettings::Instance().IsRunRateBoardEnabled? "Enabled" : "Disabled"));
	Item1.Properties["Action"] = IntToStr(1);
	//:::::::::::::::::::::::::::::::::::::::
	if( TGlobalSettings::Instance().IsRunRateBoardEnabled )
	{
		Item1.Properties["Color"] = IntToStr(clGreen);
	}
	else
	{
		Item1.Properties["Color"] = IntToStr(clRed);
	}

	//:::::::::::::::::::::::::::::::::::::::

	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

    TVerticalSelection Item2;
	Item2.Title = UnicodeString("Member Sales Only \r") + UnicodeString((TGlobalSettings::Instance().IsMemberSalesOnlyEnabled ? "Enabled" : "Disabled"));
	Item2.Properties["Action"] = IntToStr(2);

	//:::::::::::::::::::::::::::::::::::::::

	if( TGlobalSettings::Instance().IsMemberSalesOnlyEnabled )
	{
		Item2.Properties["Color"] = IntToStr(clGreen);
	}
	else
	{
		Item2.Properties["Color"] = IntToStr(clRed);
	}
	//:::::::::::::::::::::::::::::::::::::::
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item2);


    TVerticalSelection Item3;
	Item3.Title = "IP Address \r" + TGlobalSettings::Instance().RunRateBoardIP;
	Item3.Properties["Action"] = IntToStr(3);
	Item3.Properties["Color"] = IntToStr(clNavy);
	Item3.CloseSelection = true;
	SelectionForm->Items.push_back(Item3);

	//:::::::::::::::::::::::::::::::::::::::

	SelectionForm->ShowModal();
	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
	{
		int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
		switch(Action)
		{
		case 1 :
			{
				std::auto_ptr<TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

				TVerticalSelection Item;
				Item.Title = "Cancel";
				Item.Properties["Color"] = "0x000098F5";
				Item.Properties["FontColor"] = IntToStr(clWhite);;
				Item.CloseSelection = true;
				SelectionForm1->Items.push_back(Item);

				TVerticalSelection Item1;
				Item1.Title = "Enable";
				Item1.Properties["Action"] = IntToStr(1);
				Item1.Properties["Color"] = IntToStr(clGreen);
				Item1.CloseSelection = true;
				SelectionForm1->Items.push_back(Item1);

				TVerticalSelection Item2;
				Item2.Title = "Disable";
				Item2.Properties["Action"] = IntToStr(2);
				Item2.Properties["Color"] = IntToStr(clRed);
				Item2.CloseSelection = true;
				SelectionForm1->Items.push_back(Item2);

				SelectionForm1->ShowModal();
				TVerticalSelection SelectedItem1;
				if(SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel" )
				{
					int Action = StrToIntDef(SelectedItem1.Properties["Action"],0);
					switch(Action)
					{
					case 1 :
						TGlobalSettings::Instance().IsRunRateBoardEnabled = true;
						break;
					case 2 :
						TGlobalSettings::Instance().IsRunRateBoardEnabled = false;
						break;
					}

					DBTransaction.StartTransaction();
					TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmRunRateBoardEnabled,TGlobalSettings::Instance().IsRunRateBoardEnabled);
					DBTransaction.Commit();

					//::::::::::::::::::::::::::::::::::::::::::::::::
					RefreshRunRateBoard();
				}
			}  break;
			//---------------------------------------------------------------
		case 2 :
		{
				std::auto_ptr<TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

				TVerticalSelection Item;
				Item.Title = "Cancel";
				Item.Properties["Color"] = "0x000098F5";
				Item.Properties["FontColor"] = IntToStr(clWhite);
				Item.CloseSelection = true;
				SelectionForm1->Items.push_back(Item);

				TVerticalSelection Item1;
				Item1.Title = "Enable";
				Item1.Properties["Action"] = IntToStr(1);
				Item1.Properties["Color"] = IntToStr(clGreen);
				Item1.CloseSelection = true;
				SelectionForm1->Items.push_back(Item1);

				TVerticalSelection Item2;
				Item2.Title = "Disable";
				Item2.Properties["Action"] = IntToStr(2);
				Item2.Properties["Color"] = IntToStr(clRed);
				Item2.CloseSelection = true;
				SelectionForm1->Items.push_back(Item2);

				SelectionForm1->ShowModal();
				TVerticalSelection SelectedItem1;
				if(SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel" )
				{
					int Action = StrToIntDef(SelectedItem1.Properties["Action"],0);
					switch(Action)
					{
					case 1 :
						{
							TGlobalSettings::Instance().IsMemberSalesOnlyEnabled = true;
						}  break;
					case 2 :
						{
							TGlobalSettings::Instance().IsMemberSalesOnlyEnabled = false;
						}  break;
					}

					DBTransaction.StartTransaction();
					TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmIsMemberSalesOnlyEnabled,TGlobalSettings::Instance().IsMemberSalesOnlyEnabled);
					DBTransaction.Commit();
				}
			} break;
		case 3 :
			{

				std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
				frmTouchKeyboard->MaxLength = 200;
				frmTouchKeyboard->AllowCarriageReturn = false;
				frmTouchKeyboard->StartWithShiftDown = false;
				frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().RunRateBoardIP;
				frmTouchKeyboard->Caption = "Run Rate Board IP Address";
				if (frmTouchKeyboard->ShowModal() == mrOk)
				{
					TGlobalSettings::Instance().RunRateBoardIP = frmTouchKeyboard->KeyboardText;
					DBTransaction.StartTransaction();
					TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmRunRateBoardIP,TGlobalSettings::Instance().RunRateBoardIP);
					DBTransaction.Commit();
				}
			}
            break;
			//---------------------------------------------------------------
		}
	}
	else
	keepFormAlive = false;

	return keepFormAlive;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::RefreshBarExchangeBtnColor()
{
	if (TDeviceRealTerminal::Instance().Modules.Status[eReservations]["Registered"])
	{
		if (TGlobalSettings::Instance().BarExchangeEnabled)
		{
			TouchBtnBarExchange->ButtonColor = clGreen;

			TouchBtnBarExchange->Caption = "Bar Exchange \r[Enabled]";
		}
		else
		{
			TouchBtnBarExchange->ButtonColor = clRed;

			TouchBtnBarExchange->Caption = "Bar Exchange \r[Disabled]";
		}
	}
	else
	{
		TouchBtnBarExchange->ButtonColor = clRed;
		TouchBtnBarExchange->Caption = "Bar Exchange \r[Unregistered]";
	}
}
//--------------------------------------------------------------
void __fastcall TfrmMaintain::RefreshRunRateBoard()
{
	if (TDeviceRealTerminal::Instance().Modules.Status[eReservations]["Registered"])
	{
		if (TGlobalSettings::Instance().IsRunRateBoardEnabled)
		{
			TouchBtnRunRateBoard->ButtonColor = clGreen;

			TouchBtnRunRateBoard->Caption = "Run Rate Board \r[Enabled]";
		}
		else
		{
			TouchBtnRunRateBoard->ButtonColor = clRed;

			TouchBtnRunRateBoard->Caption = "Run Rate Board \r[Disabled]";
		}
	}
	else
	{
		TouchBtnRunRateBoard->ButtonColor = clRed;
		TouchBtnRunRateBoard->Caption = "Run Rate Board \r[Unregistered]";
	}
}

void __fastcall TfrmMaintain::RefreshDrinkCommandButtonColor()
{
    if ( TDeviceRealTerminal::Instance().IMManager->Registered)
    {
        if (TDeviceRealTerminal::Instance().Modules.Status[eReservations]["Registered"])
        {
            if (TGlobalSettings::Instance().IsDrinkCommandEnabled)
            {
                tchbtnDrinkCommand->ButtonColor = clGreen;
                tchbtnDrinkCommand->Caption = "Drink Command \r[Enabled]";
            }
            else
            {
                tchbtnDrinkCommand->ButtonColor = clRed;
                tchbtnDrinkCommand->Caption = "Drink Command \r[Disabled]";
            }
        }
        else
        {
            tchbtnDrinkCommand->ButtonColor = clRed;
            tchbtnDrinkCommand->Caption = "Drink Command \r[Unregistered]";
        }
    }
    else
    {
		tchbtnDrinkCommand->Caption = "Drink Command \r[Unregistered]";
        tchbtnDrinkCommand->Enabled = TDeviceRealTerminal::Instance().IMManager->Registered;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::SaveServerCheck()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();

	AnsiString  terminalName=  TDeviceRealTerminal::Instance().ID.Name;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT sum(a.INTEGER_VAL) Server_Status FROM VARSPROFILE a "
                                    "left join PROFILE on PROFILE.PROFILE_KEY=a.PROFILE_KEY   "
                                    " where a.VARIABLES_KEY=9044 and PROFILE.NAME!=:NAME " ;

	IBInternalQuery->ParamByName("NAME")->AsString =terminalName  ;
	IBInternalQuery->ExecQuery();

	int  TPKey = IBInternalQuery->FieldByName("Server_Status")->AsInteger;
	DBTransaction.Commit();

	if(TPKey > 0)
	{
		ServerEnable=false;
	}
	else
	{
		ServerEnable=true;
	}
}

void __fastcall TfrmMaintain::TouchBtnClipInterfaceMouseClick(TObject *Sender)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    std::auto_ptr<TfrmVerticalSelect> SelectionForm1(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

				TVerticalSelection Item;
				Item.Title = "Cancel";
				Item.Properties["Color"] = "0x000098F5";
				Item.Properties["FontColor"] = IntToStr(clWhite);;
				Item.CloseSelection = true;
				SelectionForm1->Items.push_back(Item);

				TVerticalSelection Item1;
				Item1.Title = "Enable";
				Item1.Properties["Action"] = IntToStr(1);
				Item1.Properties["Color"] = IntToStr(clGreen);
				Item1.CloseSelection = true;
				SelectionForm1->Items.push_back(Item1);

				TVerticalSelection Item2;
				Item2.Title = "Disable";
				Item2.Properties["Action"] = IntToStr(2);
				Item2.Properties["Color"] = IntToStr(clRed);
				Item2.CloseSelection = true;
				SelectionForm1->Items.push_back(Item2);

				SelectionForm1->ShowModal();
				TVerticalSelection SelectedItem1;
				if(SelectionForm1->GetFirstSelectedItem(SelectedItem1) && SelectedItem1.Title != "Cancel" )
				{
					int Action = StrToIntDef(SelectedItem1.Properties["Action"],0);
					switch(Action)
					{
					case 1 :
						TGlobalSettings::Instance().IsClippIntegrationEnabled = true;
                        TouchBtnClipInterface->ButtonColor = clGreen;
                        TouchBtnClipInterface->Caption = "Clipp Interface \r[Enabled]";
                        TManagerClippIntegration::Instance();
						break;
					case 2 :
						TGlobalSettings::Instance().IsClippIntegrationEnabled = false;
                        TouchBtnClipInterface->ButtonColor = clRed;
                        TouchBtnClipInterface->Caption = "Clipp Interface \r[Disabled]";
						break;
					}

					DBTransaction.StartTransaction();
					TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmIsClippIntegrationEnabled,TGlobalSettings::Instance().IsClippIntegrationEnabled);
					DBTransaction.Commit();
                    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintain::btnAccountingInterfaceMouseClick(TObject *Sender)
{
    std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

    TVerticalSelection Item;
    Item.Title = "Cancel";
    Item.Properties["Color"] = IntToStr(clMaroon);
    Item.CloseSelection = true;
    SelectionForm->Items.push_back(Item);

    TVerticalSelection Item1;
    Item1.Title = "Setup";
    Item1.Properties["Action"] = IntToStr(1);
    Item1.Properties["Color"] = IntToStr(clNavy);
    Item1.CloseSelection = true;
    SelectionForm->Items.push_back(Item1);


    TVerticalSelection Item2;
    Item2.Title = "Xero";
    Item2.Properties["Action"] = IntToStr(2);
    Item2.Properties["Color"] = IntToStr(clNavy);
    Item2.CloseSelection = true;
    Item2.IsDisabled = !TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"];
    SelectionForm->Items.push_back(Item2);

    TVerticalSelection Item3;
    Item3.Title = "MYOB";
    Item3.Properties["Action"] = IntToStr(3);
    Item3.Properties["Color"] = IntToStr(clNavy);
    Item3.CloseSelection = true;
    Item3.IsDisabled = !TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"];
    SelectionForm->Items.push_back(Item3);

    SelectionForm->ShowModal();
    TVerticalSelection SelectedItem;
    if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
    {
        int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
        switch(Action)
			{
			case 1 :
				{
                   SetupGLCodes();
                   break;
                }
			case 2 :
				{
                   XeroSettings();
                   break;
                }
			case 3 :
				{
                   MYOBSettings();
                   break;
                }
           }
    }
}

void TfrmMaintain::InitXeroIntegrationConfig()
{
	XeroMachineName = TGlobalSettings::Instance().XeroMachineName;
	XeroFolderPath  = TGlobalSettings::Instance().XeroFolderPath;
	XeroUserName    = TGlobalSettings::Instance().XeroUserName;
	XeroPassword    = TGlobalSettings::Instance().XeroPassword;
	MYOBMachineName = TGlobalSettings::Instance().MYOBMachineName;
	MYOBFolderPath  = TGlobalSettings::Instance().MYOBFolderPath;
	MYOBUserName    = TGlobalSettings::Instance().MYOBUserName;
	MYOBPassword    = TGlobalSettings::Instance().MYOBPassword;
}
//---------------------------------------------------------------------------
void TfrmMaintain::XeroSettings()
{
	std::auto_ptr<TfrmXeroSettings> frmXeroSettings(TfrmXeroSettings::Create<TfrmXeroSettings>(this));
	frmXeroSettings->SetAndValidate( XeroMachineName, XeroFolderPath, XeroUserName, XeroPassword,eAccountingXero );
	if(frmXeroSettings->ShowModal() == mrOk )
	{
		XeroMachineName = frmXeroSettings->AccountingMachineName;
		XeroFolderPath  = frmXeroSettings->AccountingFolderPath;
		XeroUserName    = frmXeroSettings->AccountingUserName;
		XeroPassword    = frmXeroSettings->AccountingPassword;
        _awaitingPaymentInXero =  frmXeroSettings->AwaitingPayment;
        SaveAccountingConfig(eAccountingXero);
	}
}
//---------------------------------------------------------------------------
void TfrmMaintain::MYOBSettings()
{
	std::auto_ptr<TfrmXeroSettings> frmXeroSettings(TfrmXeroSettings::Create<TfrmXeroSettings>(this));
	frmXeroSettings->SetAndValidate( MYOBMachineName, MYOBFolderPath, MYOBUserName, MYOBPassword, eAccountingMYOB);
	if(frmXeroSettings->ShowModal() == mrOk )
	{
		MYOBMachineName = frmXeroSettings->AccountingMachineName;
		MYOBFolderPath  = frmXeroSettings->AccountingFolderPath;
		MYOBUserName    = frmXeroSettings->AccountingUserName;
		MYOBPassword    = frmXeroSettings->AccountingPassword;
        _awaitingPaymentInXero =  frmXeroSettings->AwaitingPayment;
        SaveAccountingConfig(eAccountingMYOB);
	}
}
//---------------------------------------------------------------------------
void TfrmMaintain::SaveAccountingConfig(AccountingType accountingType)
{
	try
	{
        if( accountingType != eAccountingMYOB )
		    TFolderManager::Instance().SetAndValidate( XeroMachineName, XeroFolderPath, XeroUserName, XeroPassword );
        else
            TFolderManager::Instance().SetAndValidate( MYOBMachineName, MYOBFolderPath, MYOBUserName, MYOBPassword );
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();

		if( TFolderManager::Instance().LastError == fmNO_ERROR )
		{
            if(accountingType == eAccountingXero)
            {
                TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmXeroMachineName, TFolderManager::Instance().HostName );
                TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmXeroFolderPath,  TFolderManager::Instance().FolderName );
                TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmXeroUserName,    TFolderManager::Instance().UserName );
                TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmXeroPassword,    TFolderManager::Instance().Password );
                TGlobalSettings::Instance().AwaitingPaymentInXero    = _awaitingPaymentInXero;
                TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAwaitingPaymentInXero, TGlobalSettings::Instance().AwaitingPaymentInXero);
                TGlobalSettings::Instance().XeroMachineName = TFolderManager::Instance().HostName;
                TGlobalSettings::Instance().XeroFolderPath  = TFolderManager::Instance().FolderName;
                TGlobalSettings::Instance().XeroUserName    = TFolderManager::Instance().UserName;
                TGlobalSettings::Instance().XeroPassword    = TFolderManager::Instance().Password;
                TGlobalSettings::Instance().IsXeroEnabled = true;
                TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmIsXeroEnabled, TGlobalSettings::Instance().IsXeroEnabled);
                TGlobalSettings::Instance().IsMYOBEnabled = false;
                TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmIsMYOBEnabled, TGlobalSettings::Instance().IsMYOBEnabled);
            }
            else if(accountingType == eAccountingMYOB)
            {
                TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmMYOBMachineName, TFolderManager::Instance().HostName );
                TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmMYOBFolderPath,  TFolderManager::Instance().FolderName );
                TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmMYOBUserName,    TFolderManager::Instance().UserName );
                TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmMYOBPassword,    TFolderManager::Instance().Password );
                TGlobalSettings::Instance().AwaitingPaymentInXero    = _awaitingPaymentInXero;
                TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAwaitingPaymentInXero, TGlobalSettings::Instance().AwaitingPaymentInXero);
                TGlobalSettings::Instance().MYOBMachineName = TFolderManager::Instance().HostName;
                TGlobalSettings::Instance().MYOBFolderPath  = TFolderManager::Instance().FolderName;
                TGlobalSettings::Instance().MYOBUserName    = TFolderManager::Instance().UserName;
                TGlobalSettings::Instance().MYOBPassword    = TFolderManager::Instance().Password;
                TGlobalSettings::Instance().IsMYOBEnabled = true;
                TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmIsMYOBEnabled, TGlobalSettings::Instance().IsMYOBEnabled);
                TGlobalSettings::Instance().IsXeroEnabled = false;
                TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmIsXeroEnabled, TGlobalSettings::Instance().IsXeroEnabled);
                TMYOBIntegration::Instance().MYOBMachineName = TGlobalSettings::Instance().MYOBMachineName;
                TMYOBIntegration::Instance().MYOBFolderPath  = TGlobalSettings::Instance().MYOBFolderPath;
                TMYOBIntegration::Instance().MYOBUserName    = TGlobalSettings::Instance().MYOBUserName;
                TMYOBIntegration::Instance().MYOBPassword    = TGlobalSettings::Instance().MYOBPassword;
            }
		}
		else
		{
            TFolderManager::Instance().FolderName = "";
            TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmXeroFolderPath, TFolderManager::Instance().FolderName );
            if(accountingType == eAccountingXero)
            {
                TGlobalSettings::Instance().IsXeroEnabled = false;
    			MessageBox("Failed to save Xero Integration configuration. " + TFolderManager::Instance().LastErrorMsg, "Error", MB_OK);
            }
            else
            {
                TGlobalSettings::Instance().IsMYOBEnabled = false;
                MessageBox("Failed to save MYOB Integration configuration. " + TFolderManager::Instance().LastErrorMsg, "Error", MB_OK);
            }
        }
		DBTransaction.Commit();
	}
	catch( Exception & Exc )
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Exc.Message);
		MessageBox("Failed to save Xero Integration configuration. " + Exc.Message, "Error", MB_OK);
	}
}

void __fastcall TfrmMaintain::TouchBtnRunRateBoardMouseClick(TObject *Sender)
{
     TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckMaintenance);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		while(DisplayRunRateSettingsOnly(DBTransaction) != false);
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to the Run rate interface interface settings.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}

}
//---------------------------------------------------------------------------
void TfrmMaintain::SetupGLCodes()
{
  TfrmSetupGlCodes *frmSetupGlCodes = new TfrmSetupGlCodes(this);
  frmSetupGlCodes->PointsPurchased = TGlobalSettings::Instance().PointsPurchasedGLCode;
  frmSetupGlCodes->PointsSpent = TGlobalSettings::Instance().PointsSpentGLCode;
  frmSetupGlCodes->VoucherPurchased = TGlobalSettings::Instance().VoucherPurchasedGLCode;
  frmSetupGlCodes->ServiceCharge = TGlobalSettings::Instance().ServiceChargeGLCode;
  frmSetupGlCodes->ServiceChargeTax = TGlobalSettings::Instance().ServiceChargeTaxGLCode;
  frmSetupGlCodes->Rounding = TGlobalSettings::Instance().RoundingGLCode;
  frmSetupGlCodes->TabDepositCreditReceived = TGlobalSettings::Instance().TabDepositCreditReceivedGLCode;
  frmSetupGlCodes->TabDepositCreditRefunded = TGlobalSettings::Instance().TabDepositCreditRefundedGLCode;
  frmSetupGlCodes->FloatGLCode = TGlobalSettings::Instance().FloatGLCode;
  frmSetupGlCodes->EftPosTip = TGlobalSettings::Instance().EftPosTipGLCode;
  if(frmSetupGlCodes->ShowModal() == mrOk)
   {
        TGlobalSettings::Instance().PointsPurchasedGLCode = frmSetupGlCodes->PointsPurchased;
        TGlobalSettings::Instance().PointsSpentGLCode = frmSetupGlCodes->PointsSpent;
        TGlobalSettings::Instance().VoucherPurchasedGLCode = frmSetupGlCodes->VoucherPurchased ;
        TGlobalSettings::Instance().ServiceChargeGLCode = frmSetupGlCodes->ServiceCharge ;
        TGlobalSettings::Instance().ServiceChargeTaxGLCode = frmSetupGlCodes->ServiceChargeTax ;
        TGlobalSettings::Instance().RoundingGLCode = frmSetupGlCodes->Rounding ;
        TGlobalSettings::Instance().TabDepositCreditReceivedGLCode = frmSetupGlCodes->TabDepositCreditReceived ;
        TGlobalSettings::Instance().TabDepositCreditRefundedGLCode = frmSetupGlCodes->TabDepositCreditRefunded ;
        TGlobalSettings::Instance().FloatGLCode = frmSetupGlCodes->FloatGLCode ;
        TGlobalSettings::Instance().EftPosTipGLCode = frmSetupGlCodes->EftPosTip;
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmPointsPurchasedGLCode, TGlobalSettings::Instance().PointsPurchasedGLCode );
        TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmPointsSpentGLCode, TGlobalSettings::Instance().PointsSpentGLCode );
        TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmVoucherPurchasedGLCode, TGlobalSettings::Instance().VoucherPurchasedGLCode );
        TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmServiceChargeGLCode, TGlobalSettings::Instance().ServiceChargeGLCode );
        TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmServiceChargeTaxGLCode, TGlobalSettings::Instance().ServiceChargeTaxGLCode );
        TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmRoundingGLCode, TGlobalSettings::Instance().RoundingGLCode );
        TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmTabDepositCreditReceivedGLCode, TGlobalSettings::Instance().TabDepositCreditReceivedGLCode );
        TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmTabDepositCreditRefundedGLCode, TGlobalSettings::Instance().TabDepositCreditRefundedGLCode );
        TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmFloatGLCode, TGlobalSettings::Instance().FloatGLCode );
        TManagerVariable::Instance().SetDeviceStr( DBTransaction, vmEftPosTipGLCode, TGlobalSettings::Instance().EftPosTipGLCode);
        DBTransaction.Commit();
   }
   delete frmSetupGlCodes;
}


