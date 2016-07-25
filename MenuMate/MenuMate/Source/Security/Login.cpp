// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Login.h"
#include "MMLogging.h"
#include "enum.h"
#include "MMMessageBox.h"
#include "GUIScale.h"
#include "MMContactInfo.h"

#ifdef MenuMate
#include "DBSaleTimes.h"
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchNumpad"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TfrmLogin::TfrmLogin(TComponent* Owner, Database::TDBTransaction &inDBTransaction) : TZForm(Owner),
DBTransaction(inDBTransaction)
{
}

// ---------------------------------------------------------------------------

void __fastcall TfrmLogin::FormShow(TObject *Sender)
{
	 FormResize(NULL);
	 Top = 0;
	 Left = 0;

	 SetWindowPos(Handle, HWND_TOP, Left, Top, Width, Height, 0);
	 pnlDisabled->Visible = false;
	 pnlKeyboard->Visible = true;

	 AdminAttempt = false;
	 NumpadDisplay->SetPIN("");
	 if (Contacts != NULL)
	 {
		  pnlDisabled->Visible = false;
		  Caption = "Security Check";
		  if (DisableManualLogin)
		  {
				TouchNumpad1->Visible = false;
				NumpadDisplay->Visible = TouchNumpad1->Visible;
				pnlDisabled->Visible = true;
		  }
		  else
		  {
				pnlDisabled->Visible = false;
				TouchNumpad1->Visible = true;
				NumpadDisplay->Visible = TouchNumpad1->Visible;
		  }
	 }

	 if (memStaffMessage->Text == "")
	 {
		  pnlStaffMessage->Visible = false;
	 }
	 else
	 {
		  pnlStaffMessage->Visible = true;
	 }

	 //SetGridColors(tgUsers);
	 SetNumpadColors(TouchNumpad1);
	 RefreshDisplay();
	 Password = "";
	 FlashTimer = 0;
	 ClockDisplay->SetTime(Now());
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void __fastcall TfrmLogin::btnCLRClick(TObject *Sender)
{
	 Password = "";
	 SearchString = "";
}
// ---------------------------------------------------------------------------

void __fastcall TfrmLogin::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
                if(double(Screen->Width) / Screen->Height < 1.4)
                {
       			ScaleBy(Screen->Width, Temp);
                        GUIScale::ParentHeight(pnlCaption, 0.3);
                        GUIScale::ParentWidth(Panel4, 0.5);
                }
	}
        //this->Height = Screen->Height;
        /*
        this->Height = Screen->Height - 100;
        this->Width = Screen->Width - 100;
	Left	= (Screen->Width - Width) / 2;
    Top		= (Screen->Height - Height) / 2;

  //  GUIScale::ParentHeight(, 0.9);
    GUIScale::ParentHeight(pnlCaption, 0.3);
    GUIScale::ParentWidth(Panel4, 0.5);
    //GUIScale::ChildrenHeight(pnlKeyboard, 0.7);

	 /* pnlKeyboard->Top = pnlCaption->Top + pnlCaption->Height + this->BorderWidth;
	 pnlKeyboard->Left = this->BorderWidth;
	 pnlKeyboard->Height = ClientHeight - (pnlKeyboard->Top + this->BorderWidth);
	 pnlLogin->Top = pnlCaption->Top + pnlCaption->Height + this->BorderWidth;
	 pnlLogin->Width = ClientWidth - (pnlKeyboard->Width + this->BorderWidth);
	 pnlLogin->Left = ClientWidth - pnlLogin->Width - this->BorderWidth;
	 pnlLogin->Height = ClientHeight - (pnlKeyboard->Top + this->BorderWidth);
         */
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLogin::WMDisplayChange(TWMDisplayChange& Message)
{
	 FormResize(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLogin::CardSwipe(TMessage& Message)
{
	 UnicodeString Data = *((UnicodeString*)Message.WParam);
#ifdef MenuMate
	 /* if(Data.SubString(1,3) == "MMK") // Kitchen Docket.
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
	 else
	 { */
#endif
	 try
	 {
     	  TMMContactInfo Info;
          Info.CardStr = Data.SubString(1, 80);
          int SearchKey = Contacts->GetContactByCard(DBTransaction, Info);
		  if (SearchKey != 0)
		  {
				Contacts->GetContactDetails(DBTransaction, SearchKey, Info);
				if (Contacts->ContactType == eERSMember)
				{
					 SearchKey = Info.ExternalKey;
				}

				for (int i = 0; i < tgUsers->RowCount; i++)
				{
					 for (int j = 0; j < tgUsers->ColCount; j++)
					 {
						  if (tgUsers->Buttons[i][j]->Tag == SearchKey)
						  {
								NumpadDisplay->SetPIN(Info.PIN);
								tgUsersMouseClick(NULL, mbLeft, TShiftState(), tgUsers->Buttons[i][j]);
						  }
					 }
				}
		  }
		  else if (Contacts->GetContactByProx(DBTransaction, Data.SubString(1, 80)) != 0)
		  {
				int SearchKey = Contacts->GetContactByProx(DBTransaction, Data.SubString(1, 80));
				TMMContactInfo Info;
				Contacts->GetContactDetails(DBTransaction, SearchKey, Info);
				if (Contacts->ContactType == eERSMember)
				{
					 SearchKey = Info.ExternalKey;
				}

				for (int i = 0; i < tgUsers->RowCount; i++)
				{
					 for (int j = 0; j < tgUsers->ColCount; j++)
					 {
						  if (tgUsers->Buttons[i][j]->Tag == SearchKey)
						  {
								NumpadDisplay->SetPIN(Info.PIN);
								tgUsersMouseClick(NULL, mbLeft, TShiftState(), tgUsers->Buttons[i][j]);
						  }
					 }
				}
		  }
		  else if (Data.Length() == 1) // Single Character
		  {
				NumpadDisplay->SetPIN(NumpadDisplay->PIN() + Data);
		  }
	 }
	 catch(Exception & E)
	 {
		  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Card Swipe Error" + E.Message);
	 }
#ifdef MenuMate
	 /* } */
#endif
}

void __fastcall TfrmLogin::btnClearClick(TObject *Sender)
{
	 SearchString = "";
}
// ---------------------------------------------------------------------------

void TfrmLogin::RefreshDisplay()
{


     std::auto_ptr <TStringList> List(new TStringList);
	 if(Contacts != NULL)
	 {
          SearchString = "";
		  UserID.clear();

        if(TGlobalSettings::Instance().EnableApprovedLogin)
        {
           if(TGlobalSettings::Instance().ClockInOutApprovedLogin)
           {
              if(CheckRequiredAccess == CheckPINChange)
              {
                 CheckPOSAccessFilter(List.get());
              }
              else
              {
                GetApprovedMemberList(List.get(), true);
              }
           }
           else
           {
              if(CheckRequiredAccess == CheckPINChange)
              {
                CheckPOSAccessFilter(List.get());
              }
              else
              {
                GetApprovedMemberList(List.get());
              }
           }
        }
        else
        {
           if(CheckRequiredAccess == CheckPINChange)
           {
              CheckPOSAccessFilter(List.get());
           }
           else
           {
             Contacts->GetContactList(DBTransaction, List.get());
           }
        }
		  tgUsers->ColCount = 0;
		  tgUsers->RowCount = 0;

		  tgUsers->ColCount = 4;
		  int RowCount = List->Count / tgUsers->ColCount;
		  if (RowCount < 6)
				RowCount = 6;
		  if (List->Count % 4 != 0)
		  {
				RowCount++;
		  }

		  tgUsers->RowCount = RowCount;

		  for (int i = 0; i < tgUsers->RowCount; i++)
		  {
				for (int j = 0; j < tgUsers->ColCount; j++)
				{
					 int Index = (i * tgUsers->ColCount) + j;
					 if (Index < List->Count)
					 {
						  int UserKey = int(List->Objects[Index]);
						  UnicodeString Name = List->Strings[Index];
						  UserID.push_back(UserKey);
						  Name = Name.SubString(1, 1).UpperCase() + Name.SubString(2, Name.Length() - 1).LowerCase();
						  if (Name.Pos(" ") != 0)
						  {
								Name = Name.SubString(1, Name.Pos(" ")) + Name.SubString(Name.Pos(" ") + 1, 1).UpperCase() + Name.SubString
									 (Name.Pos(" ") + 2, Name.Length() - 1).LowerCase();
						  }
						  tgUsers->Buttons[i][j]->Tag = UserKey;
						  tgUsers->Buttons[i][j]->Caption = Name;
						  tgUsers->Buttons[i][j]->Visible = true;
						  /* tgUsers->Buttons[i][j]->Color = clCream;
						  tgUsers->Buttons[i][j]->DisabledColor = clBtnFace;
						  tgUsers->Buttons[i][j]->FontColor = clBlack; */
						  tgUsers->Buttons[i][j]->Latched = false;

						  if (Contacts->ContactType == eERSMember)
						  {
								if (UserKey == Last3rdPartyMemberKey)
								{
									 // tgUsers->Buttons[i][j]->Latched = true;
								}
						  }
						  else if (UserKey == LastUserKey)
						  {
								// tgUsers->Buttons[i][j]->Latched = true;
						  }
					 }
					 else
					 {
						  tgUsers->Buttons[i][j]->Visible = false;
					 }
				}
		  }

		  for (int k = 0; k < tgUsers->RowCount; k++)
		  {
				for (int j = 0; j < tgUsers->ColCount; j++)
				{
					 if (tgUsers->Buttons[k][j]->Caption == "")
					 {
						  tgUsers->Buttons[k][j]->Visible = false;
					 }
				}
		  }
	 }
}

void __fastcall TfrmLogin::tbtnReenableMouseClick(TObject *Sender)
{
	 //
	 TMMContactInfo TempUserInfo;
	 std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
	 Staff->DisableManualLogin = false;
	 TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckMaintenance);
	 if (Result == lsAccepted)
	 {
		  TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmDisableManualLogin, false);
		  DisableManualLogin = false;
		  FormShow(Sender);
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
// ---------------------------------------------------------------------------

void __fastcall TfrmLogin::tgUsersMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	 LastUserKey = GridButton->Tag;
	 LastUserName = GridButton->Caption;
	 Password = NumpadDisplay->PIN();
	 if (Password != "")
	 {
		  if (DisableManualLogin)
		  {
				lbeFlashPin->Caption = "Please Swipe Your Card";
		  }
		  else
		  {
				lbeFlashPin->Caption = "Please Enter Your PIN First";
		  }
		  tmFlashEnterPin->Enabled = false;
		  if (AdminAttempt)
		  {
				ModalResult = mrRetry;
		  }
		  else
		  {
				ModalResult = mrOk;
				TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "Login Returned User : " + LastUserName);
		  }
	 }
	 else
	 {
		  if (DisableManualLogin)
		  {
				lbeFlashPin->Caption = "Please Swipe Your Card";
		  }
		  else
		  {
				lbeFlashPin->Caption = "Please Enter Your PIN First";
		  }
		  tmFlashEnterPin->Enabled = true;
	 }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmLogin::tbtnCancelMouseClick(TObject *Sender)
{
	 ModalResult = mrCancel;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmLogin::tmFlashEnterPinTimer(TObject *Sender)
{
	 switch(FlashTimer)
	 {
	 case 0:
		  lbeFlashPin->Visible = true;
		  lbeFlashPin->Font->Color = clRed;
		  FlashTimer++;
		  break;
	 case 1:
		  lbeFlashPin->Font->Color = clBlack;
		  FlashTimer++;
		  break;
	 case 2:
		  lbeFlashPin->Font->Color = clRed;
		  FlashTimer++;
		  break;
	 case 3:
		  lbeFlashPin->Font->Color = clBlack;
		  FlashTimer++;
		  break;
	 case 4:
		  lbeFlashPin->Font->Color = clRed;
		  FlashTimer++;
		  break;
	 case 5:
		  lbeFlashPin->Visible = false;
		  lbeFlashPin->Font->Color = clBlack;
		  FlashTimer = 0;
		  tmFlashEnterPin->Enabled = false;
		  break;

	 }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmLogin::TouchNumpad1Click(TObject *Sender, TNumpadKey Key)
{
	 lbeFlashPin->Visible = false;
	 lbeFlashPin->Font->Color = clBlack;
	 FlashTimer = 0;
	 tmFlashEnterPin->Enabled = false;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmLogin::ClockTimerTimer(TObject *Sender)
{
	 ClockDisplay->SetTime(Now());
}
// ---------------------------------------------------------------------------

void __fastcall TfrmLogin::Image1Click(TObject *Sender)
{
	 AdminAttempt = true;
}
// ---------------------------------------------------------------------------

void TfrmLogin::GetApprovedMemberList(TStringList *List, bool isClockedInMember)
{
   TMMContactInfo CheckUserInfo;
   UnicodeString Description;
   std::auto_ptr <TStringList> GetList(new TStringList);
   Contacts->GetContactList(DBTransaction, GetList.get());
   std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
   int key;
   for(int i = 0; i < GetList->Count; i++)
   {
     key = (int)GetList->Objects[i];
     bool Allowed = Staff->CheckUserAccessLogin(DBTransaction, key, CheckRequiredAccess, Description, CheckUserInfo);
     if(Allowed)
     {
        UnicodeString str = "Zadmin";
        int a = AnsiCompareText(CheckUserInfo.Name, str);
        if(a == 0)
        {
           int key = (int)GetList->Objects[i];
           List->AddObject(GetList->Strings[i], (TObject *)key);
        }
        else
        {
            int key = (int)GetList->Objects[i];
            if(isClockedInMember)
            {
               bool result = CheckClockedInMember(DBTransaction, key);
               if(result)
               {
                 List->AddObject(GetList->Strings[i], (TObject *)key);
               }
            }
            else
            {
              List->AddObject(GetList->Strings[i], (TObject *)key);
            }
        }
     }
   }
}


bool TfrmLogin::CheckClockedInMember(Database::TDBTransaction &DBTransaction, int contact_key)//
{
   bool Success = false;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "select MAX(LOGIN_DATETIME) as LOGIN, MAX(LOGOUT_DATETIME) as LOGOUT, CONTACTS_KEY from CONTACTTIME where CONTACTS_KEY = :CONTACTS_KEY group by CONTACTS_KEY ";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contact_key;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount)
	  {
		 TDateTime dIn = IBInternalQuery->FieldByName("LOGIN")->AsDateTime;
         TDateTime dOut = IBInternalQuery->FieldByName("LOGOUT")->AsDateTime;
         if(dIn > dOut)
         {
           Success = true;
         }
         else
         {
           Success = false;
         }
	  }
	  else
	  {
	  }
	  IBInternalQuery->Close();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return Success;
}

void TfrmLogin::CheckPOSAccessFilter(TStringList *List)
{
    CheckRequiredAccess = CheckPOS;
    GetApprovedMemberList(List);
}

