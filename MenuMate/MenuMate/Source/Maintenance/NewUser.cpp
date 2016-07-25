//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#include "enum.h"
#include "NewUser.h"
#include "Secure.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "CardSwipe.h"
#include "DBTab.h"
#include "MMMessageBox.h"
#include "ContactStaff.h"
#include "NetMessageInfoSync.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmNewUser::TfrmNewUser(TComponent* Owner,Database::TDBControl &inDBControl)
	: TZForm(Owner), DBControl(inDBControl)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewUser::btnAcceptClick(TObject *Sender)
{
	try
   {
      if(btnName->Caption == "")
      {
			MessageBox("You must enter a user name.", "Error", MB_OK);
         return;
      }
      if(btnInitials->Caption == "")
      {
         MessageBox("You must enter the user's initials.", "Error", MB_OK);
         return;
      }
		if(btnInitials->Caption.Length() > Max_User_Initials_Length)
      {
         MessageBox("Initials can not be longer than " + IntToStr(Max_User_Initials_Length) + " characters.", "Error", MB_OK);
			return;
		}
		if(btnLogoff->Caption == "")
		{
			MessageBox("Please supply an auto loggoff duration.", "Error", MB_OK);
			return;
		}
		try
		{
			StrToInt(btnLogoff->Caption);
		}
		catch (EConvertError &Err)
		{
			MessageBox("Please supply an auto loggoff duration.", "Error", MB_OK);
		}
		if (PIN1.Length() > PIN_Length || PIN2.Length() < 2)
		{
			MessageBox("Please enter a valid PIN number of at least 2 characters in length and no more than " + IntToStr(PIN_Length), "Error", MB_OK);
			return;
		}

		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
		int StaffCount = Staff->RecordCount(DBTransaction);
		DBTransaction.Commit();

		if(StaffCount == 0)
		{
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();

			int NextContactID = Staff->GetNextContactID(DBTransaction);

			Info.Name 					= btnName->Caption;
//         Info.Phone 					= ContactPhone;
			Info.Initials				= btnInitials->Caption.UpperCase();
			Info.PIN						= PIN1;
			Info.ContactID   			= NextContactID;
			Info.CardStr	   		= CardString;
			Info.ProxStr	   		= ProxString;
			Info.AccessLevel  		= mmAdmin;
			Info.Points.Clear();
			Info.TabEnabled			= btnAllowedTab->Latched;

			Info.PalmUserSettings.AdvanceUser	= cbAdvancedUser->Latched;
			Info.PalmUserSettings.AutoDetails	= cbAutoPopup->Latched;
			Info.PalmUserSettings.LargeFont		= cbLargeFont->Latched;
			Info.PalmUserSettings.ScreenLock		= cbScreenLock->Latched;
			Info.PalmUserSettings.ClassicMode	= cbClassicMode->Latched;
			Info.PalmUserSettings.SoundLevel		= static_cast<TPalmUserSettings::TPalmSoundLevel>(rgSoundLevel->ItemIndex);
			Info.PalmUserSettings.GetAllTables	= cbGetAllTables->Latched;

		 if(tcbeprNoPointsRedemption->Latched)
            Info.Points.PointsRules << eprNoPointsRedemption;
         else
            Info.Points.PointsRules >> eprNoPointsRedemption;

		 if(tcbeprNoPointsPurchases->Latched)
            Info.Points.PointsRules << eprNoPointsPurchases;
         else
            Info.Points.PointsRules >> eprNoPointsPurchases;

		 if(tcbeprEarnsPointsWhileRedeemingPoints->Latched)
            Info.Points.PointsRules << eprEarnsPointsWhileRedeemingPoints;
         else
            Info.Points.PointsRules >> eprEarnsPointsWhileRedeemingPoints;

		 if(tcbeprOnlyEarnsPointsWhileRedeemingPoints->Latched)
            Info.Points.PointsRules << eprOnlyEarnsPointsWhileRedeemingPoints;
         else
            Info.Points.PointsRules >> eprOnlyEarnsPointsWhileRedeemingPoints;

		 if(tcbeprNeverEarnsPoints->Latched)
            Info.Points.PointsRules << eprNeverEarnsPoints;
         else
            Info.Points.PointsRules >> eprNeverEarnsPoints;

		 if(tcbeprAllowedNegitive->Latched)
            Info.Points.PointsRules << eprAllowedNegitive;
         else
            Info.Points.PointsRules >> eprAllowedNegitive;

			if(btnPayrollID->Caption != "")
				Info.PayRollID   		= btnPayrollID->Caption;

			if(btnLogoff->Caption != "")
				Info.PalmTimeOut  		= StrToInt(btnLogoff->Caption);

		 if(dtpStart1->Checked)
         {
            Info.StartTime1 = dtpStart1->DateTime - (int)dtpStart1->DateTime;
         }
         else
         {
				Info.StartTime1 = 0;
         }

         if(dtpStart2->Checked)
         {
            Info.StartTime2 = dtpStart2->DateTime - (int)dtpStart2->DateTime;
         }
         else
			{
				Info.StartTime2 = 0;
			}

         if(dtpStart3->Checked)
			{
            Info.StartTime3 = dtpStart3->DateTime - (int)dtpStart3->DateTime;
         }
			else
         {
            Info.StartTime3 = 0;
         }

         if(dtpStart4->Checked)
         {
            Info.StartTime4 = dtpStart4->DateTime - (int)dtpStart4->DateTime;
			}
			else
			{
            Info.StartTime4 = 0;
         }

         if(dtpStart5->Checked)
         {
				Info.StartTime5 = dtpStart5->DateTime - (int)dtpStart5->DateTime;
         }
         else
         {
				Info.StartTime5 = 0;
         }

			if(dtpStart6->Checked)
			{
				Info.StartTime6 = dtpStart6->DateTime - (int)dtpStart6->DateTime;
			}
			else
			{
				Info.StartTime6 = 0;
			}

			Staff->SetContactDetails(DBTransaction,0,Info);
			DBTransaction.Commit();

			MessageBox("You have been granted administrative rights.", "", MB_OK);

			TNetMessageInfoSync *Request = new TNetMessageInfoSync;
			try
			{
				Request->Broadcast = true;
				TDeviceRealTerminal::Instance().UpdateInfoSync(Request);
			}
			__finally
			{
				delete Request;
			}

			Close();
		}
		else
		{
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();

			if(Editing)
			{
				double temp = Info.HourlyRate;
				Staff->GetContactDetails(DBTransaction,User_Key,Info);
				Info.HourlyRate = temp;
			}
			else
			{
				if(Staff->GetContactByName(DBTransaction,btnName->Caption) != 0 )
				{
					MessageBox("A staff member with the name " + btnName->Caption + " already exists.", "Error", MB_OK + MB_ICONERROR);
					return;
				}
   			Info.ContactID = Staff->GetNextContactID(DBTransaction);
			}

			Info.Name 					= btnName->Caption;
			Info.Initials				= btnInitials->Caption.UpperCase();
			Info.PIN						= PIN1;
			Info.CardStr	   		= CardString;
			Info.ProxStr	   		= ProxString;
			Info.AccessLevel  		= AccessLevel;
			Info.Points.Clear();

			Info.PalmUserSettings.AdvanceUser	= cbAdvancedUser->Latched;
			Info.PalmUserSettings.AutoDetails	= cbAutoPopup->Latched;
			Info.PalmUserSettings.LargeFont		= cbLargeFont->Latched;
			Info.PalmUserSettings.ScreenLock		= cbScreenLock->Latched;
			Info.PalmUserSettings.ClassicMode	= cbClassicMode->Latched;
			Info.PalmUserSettings.SoundLevel		= static_cast<TPalmUserSettings::TPalmSoundLevel>(rgSoundLevel->ItemIndex);
			Info.PalmUserSettings.GetAllTables	= cbGetAllTables->Latched;

		 if(tcbeprNoPointsRedemption->Latched)
            Info.Points.PointsRules << eprNoPointsRedemption;
         else
            Info.Points.PointsRules >> eprNoPointsRedemption;

		 if(tcbeprNoPointsPurchases->Latched)
            Info.Points.PointsRules << eprNoPointsPurchases;
         else
            Info.Points.PointsRules >> eprNoPointsPurchases;

		 if(tcbeprEarnsPointsWhileRedeemingPoints->Latched)
            Info.Points.PointsRules << eprEarnsPointsWhileRedeemingPoints;
         else
            Info.Points.PointsRules >> eprEarnsPointsWhileRedeemingPoints;

		 if(tcbeprOnlyEarnsPointsWhileRedeemingPoints->Latched)
            Info.Points.PointsRules << eprOnlyEarnsPointsWhileRedeemingPoints;
         else
            Info.Points.PointsRules >> eprOnlyEarnsPointsWhileRedeemingPoints;

		 if(tcbeprNeverEarnsPoints->Latched)
            Info.Points.PointsRules << eprNeverEarnsPoints;
         else
            Info.Points.PointsRules >> eprNeverEarnsPoints;

		 if(tcbeprAllowedNegitive->Latched)
            Info.Points.PointsRules << eprAllowedNegitive;
         else
            Info.Points.PointsRules >> eprAllowedNegitive;

			if(btnPayrollID->Caption != "")
				Info.PayRollID   			= btnPayrollID->Caption;

			if(btnLogoff->Caption != "")
				Info.PalmTimeOut  		= StrToInt(btnLogoff->Caption);

			if(dtpStart1->Checked)
			{
				Info.StartTime1 = dtpStart1->DateTime - (int)dtpStart1->DateTime;
			}
			else
			{
				Info.StartTime1 = 0;
			}

			if(dtpStart2->Checked)
			{
				Info.StartTime2 = dtpStart2->DateTime - (int)dtpStart2->DateTime;
			}
			else
			{
				Info.StartTime2 = 0;
			}

			if(dtpStart3->Checked)
			{
				Info.StartTime3 = dtpStart3->DateTime - (int)dtpStart3->DateTime;
			}
			else
			{
				Info.StartTime3 = 0;
			}

			if(dtpStart4->Checked)
			{
				Info.StartTime4 = dtpStart4->DateTime - (int)dtpStart4->DateTime;
			}
			else
			{
				Info.StartTime4 = 0;
			}

			if(dtpStart5->Checked)
			{
				Info.StartTime5 = dtpStart5->DateTime - (int)dtpStart5->DateTime;
			}
			else
			{
				Info.StartTime5 = 0;
			}

			if(dtpStart6->Checked)
			{
				Info.StartTime6 = dtpStart6->DateTime - (int)dtpStart6->DateTime;
			}
			else
			{
				Info.StartTime6 = 0;
			}

			int TabKey = TDBTab::GetTabByOwner(DBTransaction,User_Key);
			if (btnAllowedTab->Latched == true && TabKey == 0)
			{
				Info.TabEnabled = true;
			}
			else if (btnAllowedTab->Latched == true && TabKey != 0)
			{
				Info.TabEnabled = true;
			}
			else if(btnAllowedTab->Latched == false && TabKey != 0)
			{
				Info.TabEnabled = false;
			}

			if(ClearAllCards)
         {
				Staff->DeleteContactCards(DBTransaction,User_Key);
         }

			Staff->SetContactDetails(DBTransaction,User_Key,Info);

			if (btnAllowedTab->Latched == true && TabKey == 0)
			{
				TabKey = TDBTab::GetOrCreateTab(DBTransaction,0);
				TDBTab::SetTabOwner(DBTransaction,TabKey,Info.ContactKey,TabStaff);
				TDBTab::SetTabName(DBTransaction,TabKey,Info.Name);
				TDBTab::SetTabPermanent(DBTransaction,TabKey,true);
				Info.TabEnabled = true;
			}
			else if (btnAllowedTab->Latched == true && TabKey != 0)
			{
				TDBTab::SetTabPermanent(DBTransaction,TabKey,true);
				TDBTab::SetTabType(DBTransaction,TabKey,TabStaff);
				Info.TabEnabled = true;
			}
			else if(btnAllowedTab->Latched == false && TabKey != 0)
			{
				TDBTab::SetTabPermanent(DBTransaction,TabKey,false);
				Info.TabEnabled = false;
			}

			if (duplicate_prox_detected) {
				TIBSQL *remove_duplicate_prox;

				DBTransaction.StartTransaction();
				remove_duplicate_prox =
				  DBTransaction.Query(DBTransaction.AddQuery());

				remove_duplicate_prox->Close();
				remove_duplicate_prox->SQL->Add(
				  "update contacts "
				  "       set prox_card = '' "
				  "       where contacts_key in "
				  "             (select c.contacts_key "
              "                     from contacts c "
				  "                     where c.contacts_key <> :this_user "
              "                           and c.prox_card = :new_prox_str);");

         	remove_duplicate_prox->ParamByName(
				  "this_user")->AsInteger = Info.ContactKey;
				remove_duplicate_prox->ParamByName(
				  "new_prox_str")->AsString = Info.ProxStr;

				remove_duplicate_prox->ExecQuery();
			}

			DBTransaction.Commit();


			TNetMessageInfoSync *Request = new TNetMessageInfoSync;
			try
			{
				Request->Broadcast = true;
				TDeviceRealTerminal::Instance().UpdateInfoSync(Request);
			}
			__finally
			{
				delete Request;
			}
			Close();
		}
	}
	catch(Exception &E)
	{
		MessageBox(E.Message, "Error",MB_OK + MB_ICONWARNING);
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
		MessageBox("A user of this name may already exists.\rPlease enter a different user name.\r" + E.Message, "Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmNewUser::btnCancelClick(TObject *Sender)
{
  Editing = false;
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewUser::FormShow(TObject *Sender)
{
	FormResize(NULL);

	pgControl->ActivePage = tsUser;
   ClearAllCards = false;

	if(Editing)
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
		Staff->GetContactDetails(DBTransaction,User_Key,Info);
		DBTransaction.Commit();		
	}

	btnName->Caption		= Info.Name;
	PIN1						= Info.PIN;
	PIN2						= PIN1;
	if (PIN1 == "")
		btnPIN->Caption	= "Set PIN Number";
	else
		btnPIN->Caption	= "Change PIN Number";

	btnInitials->Caption	= Info.Initials;
	btnLogoff->Caption		= Info.PalmTimeOut;
	btnAllowedTab->Latched 	= Info.TabEnabled;
	AccessLevel				= Info.AccessLevel;
	btnAccess->Enabled 	= true;
	switch (AccessLevel)
	{
		case 0:
			btnAccess->Caption = "None Assigned";
			break;
		case mmSales:
			btnAccess->Caption = "Sales";
			break;
		case mmPowerUser:
			btnAccess->Caption = "Supervisor";
			break;
		case mmAdmin:
			btnAccess->Caption = "Administrator";
			break;
        case mmWaiter:
			btnAccess->Caption = "Waiter";
			break;
		default:
			btnAccess->Caption = "Custom";
			break;
	}
	CardString = Info.CardStr;
	ProxString = Info.ProxStr;

	cbAdvancedUser->Latched    = Info.PalmUserSettings.AdvanceUser;
	cbAutoPopup->Latched       = Info.PalmUserSettings.AutoDetails;
	cbLargeFont->Latched       = Info.PalmUserSettings.LargeFont;
	cbScreenLock->Latched      = Info.PalmUserSettings.ScreenLock;
	cbClassicMode->Latched     = Info.PalmUserSettings.ClassicMode;
	cbGetAllTables->Latched		= Info.PalmUserSettings.GetAllTables;

   if(Info.Points.PointsRules.Contains(eprNoPointsRedemption))
	  tcbeprNoPointsRedemption->Latched = true;
   else
	  tcbeprNoPointsRedemption->Latched = false;

   if(Info.Points.PointsRules.Contains(eprNoPointsPurchases))
	  tcbeprNoPointsPurchases->Latched = true;
   else
	  tcbeprNoPointsPurchases->Latched = false;

   if(Info.Points.PointsRules.Contains(eprEarnsPointsWhileRedeemingPoints))
	  tcbeprEarnsPointsWhileRedeemingPoints->Latched = true;
   else
	  tcbeprEarnsPointsWhileRedeemingPoints->Latched = false;

   if(Info.Points.PointsRules.Contains(eprOnlyEarnsPointsWhileRedeemingPoints))
	  tcbeprOnlyEarnsPointsWhileRedeemingPoints->Latched = true;
   else
	  tcbeprOnlyEarnsPointsWhileRedeemingPoints->Latched = false;

   if(Info.Points.PointsRules.Contains(eprNeverEarnsPoints))
	  tcbeprNeverEarnsPoints->Latched = true;
   else
	  tcbeprNeverEarnsPoints->Latched = false;

   if(Info.Points.PointsRules.Contains(eprAllowedNegitive))
	  tcbeprAllowedNegitive->Latched = true;
   else
	  tcbeprAllowedNegitive->Latched = false;


	rgSoundLevel->ItemIndex = static_cast<int>(Info.PalmUserSettings.SoundLevel);

	btnPayrollID->Caption = Info.PayRollID;
	dtpStart1->DateTime = Info.StartTime1;
	dtpStart2->DateTime = Info.StartTime2;
	dtpStart3->DateTime = Info.StartTime3;
	dtpStart4->DateTime = Info.StartTime4;
	dtpStart5->DateTime = Info.StartTime5;
	dtpStart6->DateTime = Info.StartTime6;
	if(static_cast<double>(Info.StartTime1) != 0.0)  dtpStart1->Checked = true; else dtpStart1->Checked = false;
	if(static_cast<double>(Info.StartTime2) != 0.0)  dtpStart2->Checked = true; else dtpStart2->Checked = false;
	if(static_cast<double>(Info.StartTime3) != 0.0)  dtpStart3->Checked = true; else dtpStart3->Checked = false;
	if(static_cast<double>(Info.StartTime4) != 0.0)  dtpStart4->Checked = true; else dtpStart4->Checked = false;
	if(static_cast<double>(Info.StartTime5) != 0.0)  dtpStart5->Checked = true; else dtpStart5->Checked = false;
	if(static_cast<double>(Info.StartTime6) != 0.0)  dtpStart6->Checked = true; else dtpStart6->Checked = false;
   btnHourRate->Caption = FormatFloat("$0.00", Info.HourlyRate);
	RedrawButtons(tbtnUserProfile);
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewUser::FormCreate(TObject *Sender)
{
	Editing = false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfrmNewUser::btnNameClick(TObject *Sender)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 25;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = btnName->Caption;
   frmTouchKeyboard->Caption = "Enter User Name";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
   	btnName->Caption = frmTouchKeyboard->KeyboardText;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewUser::btnInitialsClick(TObject *Sender)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = Max_User_Initials_Length;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = btnInitials->Caption;
   frmTouchKeyboard->Caption = "Enter User Initials";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
   	btnInitials->Caption = frmTouchKeyboard->KeyboardText;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewUser::btnAccessClick(TObject *Sender)
{
   frmSecurity->TempAccess = AccessLevel;
   if (frmSecurity->ShowModal() == mrOk)
   {
   	AccessLevel = frmSecurity->TempAccess;
      switch (AccessLevel)
      {
         case 0:
         	btnAccess->Caption = "None Assigned";
         	break;
      	case mmSales:
         	btnAccess->Caption = "Sales";
         	break;
         case mmPowerUser:
         	btnAccess->Caption = "Supervisor";
         	break;
         case mmAdmin:
         	btnAccess->Caption = "Administrator";
         	break;
         case mmWaiter:
			btnAccess->Caption = "Waiter";
			break;
         default:
         	btnAccess->Caption = "Custom";
            break;
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewUser::btnPINClick(TObject *Sender)
{
   AnsiString TempPIN1, TempPIN2;

	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter PIN";
	frmTouchNumpad->btnOk->Visible = true;
	frmTouchNumpad->Mode = pmPIN;
	if (frmTouchNumpad->ShowModal() == mrOk)
   {
      TempPIN1 = frmTouchNumpad->STRResult;
		if (TempPIN1.Length() > PIN_Length || TempPIN1.Length() < MIN_PIN_Length)
		{
			MessageBox("The user's PIN must be between 2 and 10 digits.", "Error", MB_OK);
         return;
      }
      frmTouchNumpad->Caption = "Confirm PIN";
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
	else if (TempPIN1.Length() > PIN_Length || TempPIN1.Length() < MIN_PIN_Length)
   {
		MessageBox("The user's PIN must be between 2 and 10 digits.", "Error", MB_OK);
		return;
   }
   PIN1 = TempPIN1;
   PIN2 = TempPIN2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewUser::btnLogoffClick(TObject *Sender)
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
   frmTouchNumpad->Caption = "Enter number of minutes";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = false;
	frmTouchNumpad->btnOk->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
   if (frmTouchNumpad->INTResult > 255) frmTouchNumpad->INTResult = 255;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
   	btnLogoff->Caption = frmTouchNumpad->INTResult;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewUser::btnSwipeClick(TObject *Sender)
{
	std::auto_ptr<TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create<TfrmCardSwipe>(this));
	frmCardSwipe->tbOtherOpt->Visible = false;
	frmCardSwipe->ShowModal();
	if (frmCardSwipe->ModalResult == mrOk)
   {
		Info.CardsToAdd.insert(AnsiString(frmCardSwipe->SwipeString).SubString(1,80));
      Info.CardStr = AnsiString(frmCardSwipe->SwipeString).SubString(1,80);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewUser::btnRemoveStaffCardClick(TObject *Sender)
{
	std::auto_ptr<TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create<TfrmCardSwipe>(this));
	frmCardSwipe->tbOtherOpt->Visible = false;
	frmCardSwipe->ShowModal();
	if (frmCardSwipe->ModalResult == mrOk)
   {
		Info.CardsToAdd.erase(AnsiString(frmCardSwipe->SwipeString).SubString(1,80));
		Info.CardsToRemove.insert(AnsiString(frmCardSwipe->SwipeString).SubString(1,80));
      Info.CardStr = "";      
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmNewUser::FormResize(TObject *Sender)
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
   Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
	pgControl->Width = pnlButtons->Left - BorderWidth;
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewUser::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------



void __fastcall TfrmNewUser::btnAllpyPayrollTimesToAllClick(TObject *Sender)
{
	if (MessageBox("This will Apply the current payroll time configuration to all users.\rYou will still need to set the payroll ID for each user.\rDo you wish to continue?",
			"Warning",
			MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
	{
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"UPDATE CONTACTS "
				"SET "
					"START_TIME1 = :ST1, "
					"START_TIME2 = :ST2, "
					"START_TIME3 = :ST3, "
					"START_TIME4 = :ST4, "
					"START_TIME5 = :ST5, "
               "START_TIME6 = :ST6  "
				"WHERE "
					"CONTACT_TYPE = :CONTACT_TYPE";
			 IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = eStaff;
         if(dtpStart1->Checked)
         {
            IBInternalQuery->ParamByName("ST1")->AsDateTime = dtpStart1->DateTime - (int)dtpStart1->DateTime;
         }
         else
         {
            IBInternalQuery->ParamByName("ST1")->Clear();
         }

			if(dtpStart2->Checked)
         {
            IBInternalQuery->ParamByName("ST2")->AsDateTime = dtpStart2->DateTime - (int)dtpStart2->DateTime;
         }
         else
         {
            IBInternalQuery->ParamByName("ST2")->Clear();
         }

         if(dtpStart3->Checked)
         {
            IBInternalQuery->ParamByName("ST3")->AsDateTime = dtpStart3->DateTime - (int)dtpStart3->DateTime;
         }
         else
         {
            IBInternalQuery->ParamByName("ST3")->Clear();
         }

         if(dtpStart4->Checked)
         {
            IBInternalQuery->ParamByName("ST4")->AsDateTime = dtpStart4->DateTime - (int)dtpStart4->DateTime;
         }
			else
         {
            IBInternalQuery->ParamByName("ST4")->Clear();
         }

         if(dtpStart5->Checked)
         {
            IBInternalQuery->ParamByName("ST5")->AsDateTime = dtpStart5->DateTime - (int)dtpStart5->DateTime;
         }
         else
         {
            IBInternalQuery->ParamByName("ST5")->Clear();
         }

         if(dtpStart6->Checked)
         {
				IBInternalQuery->ParamByName("ST6")->AsDateTime = dtpStart6->DateTime - (int)dtpStart6->DateTime;
         }
         else
         {
            IBInternalQuery->ParamByName("ST6")->Clear();
         }
			IBInternalQuery->ExecQuery();
			DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmNewUser::btnPayrollIDClick(TObject *Sender)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 25;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = btnPayrollID->Caption;
   frmTouchKeyboard->Caption = "Enter Payroll ID";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
		btnPayrollID->Caption = frmTouchKeyboard->KeyboardText;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmNewUser::tbProximityClick(TObject *Sender)
{
	TIBSQL *check_dup_prox_query;
	std::auto_ptr<
	  TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create<TfrmCardSwipe>(this));
	Database::TDBTransaction trans(TDeviceRealTerminal::Instance().DBControl);
	AnsiString new_prox_str;
	TModalResult result;

	ProxString = "";
	frmCardSwipe->ShowModal();
	if ((result = frmCardSwipe->ModalResult) != mrOk
	    && result != mrAbort)
		return;

	new_prox_str = frmCardSwipe->SwipeString.SubString(1, 80);

	TDeviceRealTerminal::Instance().RegisterTransaction(trans);
	check_dup_prox_query = trans.Query(trans.AddQuery());

	check_dup_prox_query->Close();
	check_dup_prox_query->SQL->Add(
	  "select name from contacts where prox_card = :new_prox_card"
	  "                                and contacts_key <> :user_key;");

	trans.StartTransaction();
	check_dup_prox_query->ParamByName("new_prox_card")->AsString = new_prox_str;
	check_dup_prox_query->ParamByName("user_key")->AsInteger = User_Key;
	check_dup_prox_query->ExecQuery();

	if (!(duplicate_prox_detected = check_dup_prox_query->RecordCount > 0))
		ProxString = new_prox_str;
	else {
		switch (MessageBox(
		          "This proximity card is current held by "
		          + check_dup_prox_query->FieldByName("name")->AsString
		          + ". Would you like to transfer it to this user?",
		          "Duplicate proximity card detected", MB_YESNO)) {
		case IDYES:
			ProxString = new_prox_str;
			break;
      case IDNO:
			MessageBox("The proximity card has not been assigned to this user.",
			           "Duplicate proximity card detected", MB_OK);
			duplicate_prox_detected = false;
			break;
		}
   }

	trans.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmNewUser::btnAllowedTabClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	bool HasTab = (TDBTab::GetTabByOwner(DBTransaction,User_Key) != 0);
	DBTransaction.Commit();
	if(btnAllowedTab->Latched == false && Editing && HasTab )
	{
		MessageBox("This staff member already has a Tab.\r It will be removed next time it is billed off.", "Error", MB_OK + MB_ICONERROR);
   }
}

//---------------------------------------------------------------------------

void TfrmNewUser::RedrawButtons(TObject * Sender)
{
	TTouchBtn* CurrentButton = dynamic_cast<TTouchBtn*>(Sender);
	if(CurrentButton)
	{
		CurrentButton->Color = clNavy;

		for (int i = 0; i < pnlButtons->ControlCount ; i++)
		{
			TTouchBtn* Button = dynamic_cast<TTouchBtn*>(pnlButtons->Controls[i]);
			if(Button)
			{
				if(Button->Tag != -1 && Button != CurrentButton)
				{
					Button->Color = clMaroon;
				}
			}
		}
	}
}
void __fastcall TfrmNewUser::tbUserProfileClick(TObject *Sender)
{
	pgControl->ActivePage = tsUser;
	RedrawButtons(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmNewUser::tbPalmProfileClick(TObject *Sender)
{
	pgControl->ActivePage = tsPalm;
	RedrawButtons(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmNewUser::tbAcePayrollClick(TObject *Sender)
{
	pgControl->ActivePage = tsPayroll;
	RedrawButtons(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TfrmNewUser::tbtnClearAllCardsClick(TObject *Sender)
{
	if (MessageBox("This will Clear all swipe cards associate with this user.",
			"Warning",
			MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
	{
      ClearAllCards = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmNewUser::tbtnPointsPageMouseClick(TObject *Sender)
{
	pgControl->ActivePage = tsPoints;
	RedrawButtons(Sender);
}
//---------------------------------------------------------------------------



void __fastcall TfrmNewUser::btnHourRateMouseClick(TObject *Sender)
{



	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter Staff Hourly Rate";
	frmTouchNumpad->CURInitial = 0;
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmCurrency;
	if (frmTouchNumpad->ShowModal() == mrOk)
		Info.HourlyRate = frmTouchNumpad->CURResult;

	btnHourRate->Caption = FormatFloat("$0.00", Info.HourlyRate);
}
//---------------------------------------------------------------------------


