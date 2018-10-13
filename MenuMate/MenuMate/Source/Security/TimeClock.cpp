//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "TimeClock.h"
#include "Enum.h"
#include "Secure.h"
#include "Login.h"
#include "MMTouchNumpad.h"
#include "MMMessageBox.h"
#include "ManagerTimeClock.h"
#include "MMTouchKeyboard.h"
#include "enumContacts.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmTimeClock::TfrmTimeClock(TComponent* Owner,Database::TDBControl &inIBDatabase)
	: TZForm(Owner), IBDatabase(inIBDatabase)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClock::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClock::FormShow(TObject *Sender)
{
	FormResize(NULL);

	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckPINChange);
	if (Result == lsAccepted)
	{
      std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
		CurrentUser = TempUserInfo;
		if(ManagerTimeClock->ClockedCount(DBTransaction,CurrentUser.ContactKey) == 0 || !ManagerTimeClock->ClockedIn(DBTransaction,CurrentUser.ContactKey))
		{
			pnlLable->Caption = "Select a department for " + CurrentUser.Name + " to Clock In";
			UpdateButtonGrid(true);
			tbtnClockOut->Enabled = false;
			btnBreakTimes->Enabled = false;
		}
		else if(ManagerTimeClock->ClockedIn(DBTransaction,CurrentUser.ContactKey))
		{
			pnlLable->Caption = "Clock Out For " + CurrentUser.Name;
			UpdateButtonGrid(false);
			tbtnClockOut->Enabled = true;
			btnChangePIN->Enabled = true;
			btnBreakTimes->Enabled = true;
		}
	}
	else if (Result == lsDenied)
	{
		MessageBox("You require change PIN rights to Clock In.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
	else
		btnOKClick(Sender);
	DBTransaction.Commit();

}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClock::btnClockOutClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
   std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
	if( (double(Now() - ManagerTimeClock->ClockedInDateTime(DBTransaction,CurrentUser.ContactKey)) * 24.0) > 5.5 )
	{
		MessageBox("You have been logged in for greater than 5.5 hours. Do not forget to log out when taking your breaks. ",
								  "Health And Safety Warning",
								  MB_OK + MB_ICONWARNING);
	}

   TDateTime LogOutTime = Now();
   TDateTime LogInTime;
   int contact_time_key = ManagerTimeClock->ClockOut(DBTransaction,CurrentUser.ContactKey,LogInTime,LogOutTime);
    DBTransaction.Commit();
    ManagerTimeClock->UpdateClockInOut(DBTransaction, contact_time_key, CurrentUser.ContactKey);
    tbtnClockOut->Enabled = false;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClock::btnOKClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClock::btnChangePINClick(TObject *Sender)
{
//	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
//	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckPINChange);
//	if (Result == lsAccepted)
	{
		AnsiString TempPIN1,TempPIN2;
      std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
		frmTouchNumpad->Caption = "Enter New PIN";
		frmTouchNumpad->btnSurcharge->Caption = "Ok";
		frmTouchNumpad->btnDiscount->Visible = false;
		frmTouchNumpad->btnSurcharge->Visible = true;
      frmTouchNumpad->Mode = pmPIN;
      if (frmTouchNumpad->ShowModal() == mrOk)
		{
         TempPIN1 = frmTouchNumpad->STRResult;
			if (TempPIN1.Length() > PIN_Length || TempPIN1.Length() < MIN_PIN_Length)
         {
            MessageBox("Please enter a valid PIN number of at least 2 characters in length and no more than " + IntToStr(PIN_Length), "Error", MB_OK);
            return;
         }

			frmTouchNumpad->Caption = "Confirm PIN";
         frmTouchNumpad->btnSurcharge->Caption = "Ok";
         frmTouchNumpad->btnDiscount->Visible = false;
			frmTouchNumpad->btnSurcharge->Visible = true;
			frmTouchNumpad->Mode = pmPIN;
         if (frmTouchNumpad->ShowModal() == mrOk)
         {
            TempPIN2 = frmTouchNumpad->STRResult;
         }
         else return;
		}
		else return;

      if (TempPIN1 != TempPIN2)
		{
         MessageBox("Mismatch PINs, please try again.", "Error", MB_OK);
         return;
      }
		else if (TempPIN1.Length() < MIN_PIN_Length)
      {
         MessageBox("The user's PIN must be at least 2 digits", "Error", MB_OK);
         return;
      }
      else
      {
			CurrentUser.PIN = TempPIN1;
			Staff->SetContactDetails(DBTransaction,CurrentUser.ContactKey,CurrentUser);
		}
	}
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClock::CardSwipe(TMessage& Message)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();

	AnsiString Data = *((AnsiString *)Message.WParam);
	TempUserInfo.CardStr = Data.SubString(1,80);
	TempUserInfo.ProxStr = Data.SubString(1,80);
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
   std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckPINChange);
	if (Result == lsAccepted)
	{
		CurrentUser = TempUserInfo;
		if(ManagerTimeClock->ClockedCount(DBTransaction,CurrentUser.ContactKey) == 0 || !ManagerTimeClock->ClockedIn(DBTransaction,CurrentUser.ContactKey))
		{
			pnlLable->Caption = "Clock In For " + CurrentUser.Name;
			UpdateButtonGrid(true);
			tbtnClockOut->Enabled = false;
			btnChangePIN->Enabled = false;
		}
		else if(ManagerTimeClock->ClockedIn(DBTransaction,CurrentUser.ContactKey))
		{
			pnlLable->Caption = "Clock Out For " + CurrentUser.Name;
			UpdateButtonGrid(false);
			tbtnClockOut->Enabled = true;
			btnChangePIN->Enabled = false;
		}
	}
	else if (Result == lsDenied)
	{
		MessageBox("You require change PIN rights to Clock In.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClock::FormResize(TObject *Sender)
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
	Left				= (Screen->Width - Width) / 2;
   Top				= (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClock::FormCloseQuery(TObject *Sender,
		bool &CanClose)
{
/*	if (tbgLogin->Enabled == true)
	{
		CanClose = (MessageBox( "You have NOT logged in.\r"
													"Press one of the 'Clock In' buttons to clock in.",
													"", MB_OKCANCEL) == ID_CANCEL);
	}
	else if (tbtnClockOut->Enabled == true)
	{
		CanClose = (MessageBox( "You have NOT logged out.\r"
													"Press the 'Clock Out' button to clock out.",
													"", MB_OKCANCEL) == ID_CANCEL);
	}
*/
	CanClose = true;
}
//---------------------------------------------------------------------------
void TfrmTimeClock::DisableLoginButtons()
{
	UpdateButtonGrid(false);
	btnChangePIN->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClock::ClockInClick(TObject *Sender)
{
 //	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
 /*	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckPINChange);
	if (Result == lsAccepted)
	{*/
      std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
 //		CurrentUser = TempUserInfo;
		if(ManagerTimeClock->ClockedCount(DBTransaction,CurrentUser.ContactKey) == 0 || !ManagerTimeClock->ClockedIn(DBTransaction,CurrentUser.ContactKey))
		{
			pnlLable->Caption = "Clock In For " + CurrentUser.Name;
			UpdateButtonGrid(true);
			tbtnClockOut->Enabled = false;
			btnChangePIN->Enabled = false;
		}
		else if(ManagerTimeClock->ClockedIn(DBTransaction,CurrentUser.ContactKey))
		{
			pnlLable->Caption = "Clock Out For " + CurrentUser.Name;
			UpdateButtonGrid(false);
			tbtnClockOut->Enabled = true;
			btnChangePIN->Enabled = false;
		}
/*	}
	else if (Result == lsDenied)
	{
		MessageBox("You require change PIN rights to Clock In.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}                       */
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTimeClock::tbgLoginMouseClick(TObject *Sender,
		TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
   std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
	ManagerTimeClock->ClockIn(DBTransaction,CurrentUser.ContactKey,GridButton->Tag,Now());
	DBTransaction.Commit();
	DisableLoginButtons();
	Close();
}
//---------------------------------------------------------------------------

void TfrmTimeClock::UpdateButtonGrid(bool GridEnabled)
{
	tbgLogin->ColCount = 0;
	tbgLogin->RowCount = 0;

	std::auto_ptr<TStringList> ClockInDeptList(new TStringList);

	Database::TDBTransaction DBTransaction(IBDatabase);
	DBTransaction.StartTransaction();
   std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
	ManagerTimeClock->GetClockInDeptList(DBTransaction,ClockInDeptList.get());
	DBTransaction.Commit();

	tbgLogin->ColCount = 1;
	tbgLogin->RowCount = ClockInDeptList->Count;

	for (int i = 0; i < tbgLogin->RowCount ; i++)
	{
      tbgLogin->Buttons[i][0]->Enabled = GridEnabled;
      tbgLogin->Buttons[i][0]->Caption = ClockInDeptList->Strings[i];
      tbgLogin->Buttons[i][0]->Tag = (int)(ClockInDeptList->Objects[i]);
	}
	tbgLogin->Enabled = GridEnabled;
}

//---------------------------------------------------------------------------


void __fastcall TfrmTimeClock::btnBreakTimesMouseClick(TObject *Sender)
{
	if (tbgLogin->Enabled == false)
	{
		std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
		frmTouchNumpad->Mode = pmNumber;
		frmTouchNumpad->Caption = "Enter the break time in minutes";
		frmTouchNumpad->btnDiscount->Visible = false;
		frmTouchNumpad->btnSurcharge->Visible = false;
		frmTouchNumpad->btnOk->Visible = true;

		if (frmTouchNumpad->ShowModal() == mrOk)
		{
//			btnOnBreak->Caption = (AnsiString)frmTouchNumpad->INTResult + " mins";
			int Break = frmTouchNumpad->INTResult;

			if(MessageBox( "Are you going on a " + (AnsiString)Break + " minute break?",
								"", MB_YESNO) == ID_YES)
			{
				int ContactTimeKey;
				Database::TDBTransaction DBTransaction(IBDatabase);
				DBTransaction.StartTransaction();
				std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
				TTime breaks = ManagerTimeClock->GetUserBreak(DBTransaction,CurrentUser.ContactKey, ContactTimeKey);

				int hrs = 0;
				int mins = Break;
				if(Break >= 60)
				{
					hrs = Break / 60;
					mins = Break % 60;
				}


				ManagerTimeClock->SetUserBreak(DBTransaction,ContactTimeKey,breaks + TTime(hrs, mins, 0, 0));
				DBTransaction.Commit();
				Close();
			}
		}
	}
}
//---------------------------------------------------------------------------

