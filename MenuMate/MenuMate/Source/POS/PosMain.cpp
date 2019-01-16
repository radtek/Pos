#include <vcl.h>
#pragma hdrstop

#include "PosMain.h"
#include "enum.h"
#include "Paytype.h"
#include "EnableFloorPlan.h"
#include "SelectDish.h"
#include "MMTouchNumpad.h"
#include "GetMoney.h"
#include "Printing.h"
#include "MenuManager.h"
#include "BillGroup.h"
#include "Comms.h"
#include "Secure.h"
#include "Transfer.h"
#include "Message.h"
#include "SelectReceipt.h"
#include "Loyalty.h"
#include "GetMoney.h"
#include "Login.h"
#include "SelectLoyalty.h"
#include "TabManager.h"
#include "DBSaleTimes.h"
#include "MMMessageBox.h"
#include "Eftpos.h"
#include "ManagerNet.h"
#include "SelectKitchenDocket.h"
#include "VerticalSelect.h"
#include "TableManager.h"
#include "TabManagerGUI.h"
#include "DBTables.h"
#include "DBRooms.h"
#include "Rooms.h"
#include "ChefmateClient.h"
#include "ManagerLoyaltyMate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdIPWatch"
#pragma resource "*.dfm"
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("POS\Transfer.cpp", frmTransfer);
//---------------------------------------------------------------------------
__fastcall TfrmPOSMain::TfrmPOSMain(TComponent* Owner,Database::TDBControl &inDBControl)
: TZForm(Owner), DBControl(inDBControl)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TfrmPOSMain::FormCreate(TObject *Sender)
{
	TDeviceRealTerminal::Instance().EventLockOutTimer.RegisterForEvent(OnLockOutTimer);

	randomize(); // Lucky Draw Member.
}
//---------------------------------------------------------------------------

void __fastcall TfrmPOSMain::FormDestroy(TObject *Sender)
{
	TDeviceRealTerminal::Instance().EventLockOutTimer.DeregisterForEvent(OnLockOutTimer);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPOSMain::FormShow(TObject *Sender)
{
	FormResize(this);
	TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplayDefault();
	ExitPOS = false;
	RedrawMenus = false;
	tbEftPosManager->Visible = EftPos->Enabled;
	tbSendHeldOrders->Visible = TGlobalSettings::Instance().EnableHoldSend;
    btnDelayedPayment->Visible = TGlobalSettings::Instance().TransferTableOnPrintPrelim ;
    if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot && TGlobalSettings::Instance().EnableCustomerJourney)
        tbtnTransfer->Enabled = false;
    MenuEdited = false;
    TransferSelected = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPOSMain::CardSwipe(Messages::TMessage& Message)
{
	AnsiString Data = *((AnsiString *)Message.WParam);
	if(Data.SubString(1,3) == "MMK") // Kitchen Docket.
	{
		int TimeKey = StrToInt("0x" + Data.SubString(4,Data.Length() - 3));
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TDBSaleTimes::CloseMakeStartTime(DBTransaction,TimeKey);
		DBTransaction.Commit();
	}
	else if(Data.SubString(1,5) == "{AMMK") // Kitchen Docket.
	{
		int TimeKey = StrToInt("0x" + Data.SubString(6,Data.Length() - 5));
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TDBSaleTimes::CloseMakeStartTime(DBTransaction,TimeKey);
		DBTransaction.Commit();
	}
}

float TfrmPOSMain::CurrStrToFloat(AnsiString inStr)
{
	inStr = Strip(inStr, CurrencyString);
	while (inStr.Pos(ThousandSeparator))
	{
		inStr = Strip(inStr, ThousandSeparator);
	}
	switch (NegCurrFormat)
	{
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:	if (inStr.Pos("-"))
		{
			inStr = "-" + Strip(inStr, "-");
		}
		break;
	case 0:
	case 4:
	case 14:
	case 15:	if (inStr.Pos("(") && inStr.Pos(")"))
		{
			inStr = Strip(inStr, "(");
			inStr = Strip(inStr, ")");
			inStr = "-" + inStr;
		}
		break;
	}
	float ReturnValue;
	try
	{
		ReturnValue = StrToFloat(inStr);
	}
	catch (Exception &E)
	{
		ReturnValue = 0;
		Application->ShowException(&E);
	}
	return ReturnValue;
}
//---------------------------------------------------------------------------
AnsiString TfrmPOSMain::Strip(AnsiString inStr, AnsiString inStripStr)
{
	AnsiString First, Second;
	if (inStr.Pos(inStripStr))
	{
		First = inStr.SubString(1, inStr.Pos(inStripStr)-1);
		Second = inStr.SubString(inStr.Pos(inStripStr) + inStripStr.Length(),
		inStr.Length() - inStr.Pos(inStripStr) + inStripStr.Length());

		inStr = First + Second;
	}
	return inStr.Trim();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPOSMain::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPOSMain::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}
	ClientWidth = this->BorderWidth + Panel2->Width + Panel1->Width;
	ClientHeight = Panel2->Height + (Panel2->Top * 2);
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPOSMain::tbtnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPOSMain::tbtnBillingClick(TObject *Sender)
{
	std::auto_ptr<TfrmBillGroup> frmBillGroup(TfrmBillGroup::Create<TfrmBillGroup>(this,TDeviceRealTerminal::Instance().DBControl));
	frmBillGroup->CurrentDisplayMode = eNoDisplayMode;
	frmBillGroup->CurrentTable = 1;
    frmBillGroup->WebOrderChitNumber = WebOrderChitNumber;
    frmBillGroup->MemberInfo = MemberInfo;
	frmBillGroup->ShowModal();
	ShowTableScreen();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPOSMain::tbtnReprintOrdersClick(TObject *Sender)
{


    TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
    std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
    bool AccessForReprint = Staff->TestAccessLevel( TDeviceRealTerminal::Instance().User, CheckAllowReprintOrders);

    if(AccessForReprint)
    {
        TDeviceRealTerminal::Instance().User.LatestLoginContactKey =TDeviceRealTerminal::Instance().User.ContactKey;
        TDeviceRealTerminal::Instance().User.LatestLoginName  = TDeviceRealTerminal::Instance().User.Name;


        std::auto_ptr<TfrmSelectKitchenDocket> frmSelectKitchenDocket(TfrmSelectKitchenDocket::Create<TfrmSelectKitchenDocket>(this));
        frmSelectKitchenDocket->Execute();
        ShowTableScreen();

    }
    else
    {
        TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckAllowReprintOrders);
        TDeviceRealTerminal::Instance().User.LatestLoginContactKey =0;
        TDeviceRealTerminal::Instance().User.LatestLoginName  = "";

        if (Result == lsAccepted)
        {
            TDeviceRealTerminal::Instance().User.LatestLoginContactKey =TempUserInfo.ContactKey;
            TDeviceRealTerminal::Instance().User.LatestLoginName  = TempUserInfo.Name;

             std::auto_ptr<TfrmSelectKitchenDocket> frmSelectKitchenDocket(TfrmSelectKitchenDocket::Create<TfrmSelectKitchenDocket>(this));
             frmSelectKitchenDocket->Execute();
             ShowTableScreen();
        }
        else if (Result == lsDenied)
        {
            MessageBox("You do not have access privileges to reprint order.", "Error", MB_OK + MB_ICONERROR);
        }
        else if (Result == lsPINIncorrect)
        {
            MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
        }
    }
     DBTransaction.Commit();


}
//---------------------------------------------------------------------------
void __fastcall TfrmPOSMain::tbtnReprintReceiptClick(TObject *Sender)
{
  	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
    std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
    bool AccessForReprint = Staff->TestAccessLevel( TDeviceRealTerminal::Instance().User, CheckReprintReceipt);

    if(AccessForReprint)
    {
        TDeviceRealTerminal::Instance().User.LatestLoginContactKey =TDeviceRealTerminal::Instance().User.ContactKey;
        TDeviceRealTerminal::Instance().User.LatestLoginName  = TDeviceRealTerminal::Instance().User.Name;
        ShowReprintReceipt();


    }
    else
    {
        TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckReprintReceipt);
        DBTransaction.Commit();
        TDeviceRealTerminal::Instance().User.LatestLoginContactKey =0;
        TDeviceRealTerminal::Instance().User.LatestLoginName  = "";

        if (Result == lsAccepted)
        {
            TDeviceRealTerminal::Instance().User.LatestLoginContactKey =TempUserInfo.ContactKey;
            TDeviceRealTerminal::Instance().User.LatestLoginName  = TempUserInfo.Name;
            ShowReprintReceipt();
        }
        else if (Result == lsDenied)
        {
            MessageBox("You do not have access privileges to reprint receipt.", "Error", MB_OK + MB_ICONERROR);
        }
        else if (Result == lsPINIncorrect)
        {
            MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
        }
    }

}
//---------------------------------------------------------------------------
void __fastcall TfrmPOSMain::tbMenuManagerClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	if(frmSelectDish->ParkedSales->GetCount(DBTransaction) == 0)
	{
		std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));

		TMMContactInfo TempUserInfo;
		TempUserInfo = TDeviceRealTerminal::Instance().User;
		bool AllowDisable = Staff->TestAccessLevel(TempUserInfo, CheckDisable);
		// If not, prompt for a login.
		if (!AllowDisable)
		{
			TempUserInfo.Clear();
			TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckDisable);
			if (Result == lsAccepted)
			{
				AllowDisable = true;
			}
			else if (Result == lsDenied)
			{
				MessageBox("You do not have access rights to the Menu Manager.", "Error", MB_OK + MB_ICONERROR);
			}
			else if (Result == lsPINIncorrect)
			{
				MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
			}
		}

		if(AllowDisable)
		{
			std::auto_ptr<TfrmMenuManager> frmMenuManager(TfrmMenuManager::Create(this,TDeviceRealTerminal::Instance().Menus,DBControl));
			frmMenuManager->ShowModal();
			RedrawMenus = true;
            MenuEdited = frmMenuManager->MenuEdited;
			ShowTableScreen();
		}
	}
	else
	{
		MessageBox("You cannot alter the menus while there are parked sales.", "Error", MB_OK + MB_ICONERROR);
	}
	DBTransaction.Commit();

}
//---------------------------------------------------------------------------
void __fastcall TfrmPOSMain::tbtnTransferClick(TObject *Sender)
{
	TfrmTransfer* Transfer = new TfrmTransfer(this, DBControl);
    Database::TDBTransaction DBTransaction(DBControl);
    Transfer->DBTransaction = &DBTransaction;
	if(Transfer->ShowModal() != mrOk)
    {
       DBTransaction.Rollback();
    }
    delete Transfer;
    Transfer = NULL;
    TransferSelected = true;
	ShowTableScreen();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPOSMain::btnMessageClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	TempUserInfo = TDeviceRealTerminal::Instance().User;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	bool Allowed = Staff->TestAccessLevel(TempUserInfo, CheckPOS);

	// If not, prompt for a login.
	if (!Allowed)
	{
		TempUserInfo.Clear();
		TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckPOS);
		if (Result == lsAccepted)
		{
			Allowed = true;
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access rights to Messaging.", "Error", MB_OK + MB_ICONERROR);
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
		}
	}

	if(Allowed)
	{
		std::auto_ptr<TfrmMessage> frmMessage(TfrmMessage::Create<TfrmMessage>(this,DBControl));
		frmMessage->MessageType = eMessage;
		if(frmMessage->ShowModal() == mrOk)
		{
			TNetMessageMsg *ReqMessage = new TNetMessageMsg;
			ReqMessage->Device = TDeviceRealTerminal::Instance().ID.Name;
			ReqMessage->User = TDeviceRealTerminal::Instance().User.Name;
			ReqMessage->Message = frmMessage->TextResult;
			try
			{
				TDeviceRealTerminal::Instance().ManagerNet->SendToAll(ReqMessage);
			}
			__finally
			{
				delete ReqMessage;
			}
		}
		ShowTableScreen();
	}
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------


void __fastcall TfrmPOSMain::tbtnExitPOSClick(TObject *Sender)
{
	ExitPOS = true;
	Close();
}
//---------------------------------------------------------------------------

void TfrmPOSMain::OnLockOutTimer(TSystemEvents *Sender)
{
	if(Active)
	{
		Close();
	}
}

void __fastcall TfrmPOSMain::tbAccountManagerClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	TempUserInfo = TDeviceRealTerminal::Instance().User;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	bool Allow = Staff->TestAccessLevel(TempUserInfo, CheckAccountManager);

	// If not, prompt for a login.
	if (!Allow)
	{
		TempUserInfo.Clear();
		TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckAccountManager);
		if (Result == lsAccepted)
		{
			Allow = true;
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access rights to the Tab Manager.", "Error", MB_OK + MB_ICONERROR);
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
		}
	}
	DBTransaction.Commit();

	if(Allow)
	{
		std::auto_ptr<TfrmTabManager> frmTabManager(TfrmTabManager::Create<TfrmTabManager>(this,TDeviceRealTerminal::Instance().DBControl));
		frmTabManager->ShowModal();
        frmTabManager.reset();
		ShowTableScreen();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPOSMain::tbEftPosManagerClick(TObject *Sender)
{
	EftPos->DoControlPannel();
	ShowTableScreen();
}
//---------------------------------------------------------------------------



void __fastcall TfrmPOSMain::tbTableManagerMouseClick(TObject *Sender)
{
	TMMDisplayMode CurrentDisplayMode = eNoDisplayMode;
	// Display Reports List
	std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

	TVerticalSelection Item;
	Item.Title = "Cancel";
	Item.Properties["Color"] = IntToStr(clMaroon);
	Item.CloseSelection = true;
	SelectionForm->Items.push_back(Item);

	TVerticalSelection Item1;
	Item1.Title = "Tables";
	Item1.Properties["Action"] = IntToStr(1);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

//	if(TDeviceRealTerminal::Instance().Modules.Status[eRegRooms]["Registered"])
//	{
		TVerticalSelection Item2;
		Item2.Title = "Rooms";
		Item2.Properties["Action"] = IntToStr(2);
		Item2.Properties["Color"] = IntToStr(clNavy);
		Item2.CloseSelection = true;
		SelectionForm->Items.push_back(Item2);
//	}

	SelectionForm->ShowModal();
	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
	{
		int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
		switch(Action)
		{
		case 1:
			{
				CurrentDisplayMode = eTables;
			}
			break;
		case 2:
			{
				CurrentDisplayMode = eRooms;
			}
		}
	}

	if (CurrentDisplayMode == eTables)
	{
		//std::auto_ptr<TEnableFloorPlan>(FloorPlan)(new TEnableFloorPlan((TForm*)this));
		TFloorPlanReturnParams floorPlanReturnParams;
        std::auto_ptr<TEnableFloorPlan> floorPlan(new TEnableFloorPlan());
		// Runs new web app of floorPlan
        if(floorPlan->Run( ( TForm* )this, true, floorPlanReturnParams ))
//		if( TEnableFloorPlan::Instance()->Run( ( TForm* )this, true, floorPlanReturnParams ) )
		{
			Database::TDBTransaction DBTransaction(DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
			std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
			frmTouchKeyboard->MaxLength    = 25;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->KeyboardText = TDBTables::GetPartyName( DBTransaction, floorPlanReturnParams.TabContainerNumber );
			frmTouchKeyboard->Caption      = "Enter Party Name";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				TDBTables::SetPartyName( DBTransaction, floorPlanReturnParams.TabContainerNumber, frmTouchKeyboard->KeyboardText );
			}
			DBTransaction.Commit();
		}
        floorPlan.reset();
	}
	else if (CurrentDisplayMode == eRooms)
	{
		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		if (TRooms::Instance().SelectRoom(DBTransaction) == mrOk)
		{
			std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
			frmTouchKeyboard->MaxLength = 25;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->KeyboardText = TDBRooms::GetPartyName(DBTransaction,TRooms::Instance().SelectedRoom->RoomNo);
			frmTouchKeyboard->Caption = "Enter Party Name";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				TDBRooms::SetPartyName(DBTransaction,TRooms::Instance().SelectedRoom->RoomNo,frmTouchKeyboard->KeyboardText);
			}
		}
		DBTransaction.Commit();

	}
	ShowTableScreen();
}


void __fastcall TfrmPOSMain::tbtnSendHeldOrdersClick(TObject *Sender)
{
	SendHeldOrders = true;
	ShowTableScreen();
}

void TfrmPOSMain::ShowTableScreen()
{
	ShowTablePicker = true;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPOSMain::btnDelayedPaymentMouseClick(TObject *Sender)
{
	std::auto_ptr<TfrmBillGroup> frmBillGroup(TfrmBillGroup::Create<TfrmBillGroup>(this,TDeviceRealTerminal::Instance().DBControl));
	frmBillGroup->CurrentDisplayMode = eTabs;
        frmBillGroup->CurrentTabType = TabDelayedPayment;
	frmBillGroup->CurrentTable = 1;
	frmBillGroup->ShowModal();
	ShowTableScreen();
}
//---------------------------------------------------------------------------
 void TfrmPOSMain::ShowReprintReceipt()
{   
     std::auto_ptr<TfrmSelectReceipt> frmSelectReceipt(new TfrmSelectReceipt(this));
     frmSelectReceipt->Execute();
     ShowTableScreen();
}
