//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MemberCreation.h"
#include "EditCustomer.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "CardSwipe.h"
#include "ContactStaff.h"
#include "MMMessageBox.h"
#include "Membership.h"
#include "SmartCardAPI.h"
#include "VerticalSelect.h"
#include "DateUtils.hpp"
#include "MMLogging.h"
#include "DBGroups.h"
#include "Users.h"
#include "MM_DBCore.h"
#include "DeviceRealControl.h"
#include "DeviceRealTerminal.h"
#include "DBContacts.h"
#include "GlobalSettings.h"
#include "ManagerLoyaltyMate.h"
#include "DeviceRealTerminal.h"
#include "PointsRulesSetUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmMemberCreation::TfrmMemberCreation(TComponent* Owner, TMMContactInfo &info)
    : TForm(Owner),Info(info)
{
}

// ---------------------------------------------------------------------------

void TfrmMemberCreation::DisplayCustomerDataFromPointers()
{
     lbeEmail->Caption = CustomerInfoPointers[0];
     lbeName->Caption = CustomerInfoPointers[1];
     lbeLastName->Caption = CustomerInfoPointers[2];
     lbeContactPhone->Caption = CustomerInfoPointers[3];

}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::EditCustomerBasicDetails(TObject *Sender)
{

    TTouchBtn *btn = reinterpret_cast<TTouchBtn*>(Sender);

    if(btn->Tag > 10)
        return;

    AnsiString Caption = "";
    AnsiString KeyboardText = "";
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
    frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;


     switch(btn->Tag)
    {
        case 0:
            Caption = "Enter Email";
            frmTouchKeyboard->KeyboardText = Info.EMail;
            break;
        case 1:
            Caption = "Enter First Name";
            frmTouchKeyboard->KeyboardText = Info.Name;
            break;
        case 2:
        Caption = "Enter Last Name";
        frmTouchKeyboard->KeyboardText = Info.Surname;
        break;
        case 3:
            Caption = "Enter phone Number";
            frmTouchKeyboard->KeyboardText = Info.Phone;
            break;
        default:
            break;
    }
    frmTouchKeyboard->Caption = Caption;
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
         CustomerInfoPointers[btn->Tag] =  frmTouchKeyboard->KeyboardText;
         Info.EMail=  CustomerInfoPointers[0];
         Info.Name=  CustomerInfoPointers[1];
         Info.Surname = CustomerInfoPointers[2];
         Info.Phone =  CustomerInfoPointers[3];
		 DisplayCustomerDataFromPointers();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::btnOkMouseClick(TObject *Sender)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT count(EMAIL) FROM CONTACTS where EMAIL='" + Info.EMail + "'";
    IBInternalQuery->ExecQuery();
    DBTransaction.Commit();
    int	emailcount = IBInternalQuery->Fields[0]->AsInteger;
    AnsiString firstNameMessage = "First Name should be ";
    AnsiString lastNameMessage = "Last Name should be ";
    if (TGlobalSettings::Instance().LoyaltyMateEnabled &&
        Info.CloudUUID != TLoyaltyMateUtilities::GetLoyaltyMateDisabledCloudUUID()  &&
       !Info.ValidEmail())
    {
      MessageBox("You must enter a valid Email.", "Error", MB_OK + MB_ICONERROR);
    }
    else if(Info.EMail != "" && emailcount > 0)
    {
        MessageBox("Member already exists!!!", "Error", MB_OK + MB_ICONERROR);
    }
    else if(Info.Name == NULL || Info.Name.Trim() == "")
    {
       MessageBox("You must enter first name.", "Error", MB_ICONERROR);
    }
    else if(!Info.ValidateFirstName(firstNameMessage))
    {
       MessageBox(firstNameMessage + ".", "Error", MB_OK + MB_ICONERROR);
    }
    else if(Info.Surname == NULL || Info.Surname.Trim() == "")
    {
       MessageBox("You must enter last name.", "Error", MB_ICONERROR);
    }
    else if(!Info.ValidateLastName(lastNameMessage))
    {
       MessageBox(lastNameMessage + ".", "Error", MB_OK + MB_ICONERROR);
    }
    else if(Info.Phone != "" && Info.Phone != NULL && Info.Phone.Length() < 5)
    {
       MessageBox("Phone number should be greater than 4 digits.", "Invalid Moble Number", MB_ICONERROR);
    }
    else
    {
      try
      {
        Info.LastModified = Now();
        Info.MemberType=1;
        ModalResult = mrOk;
        if (Info.SiteID == 0)
         {
            Info.SiteID = TGlobalSettings::Instance().SiteID;
         }

      }
      catch(Exception & Err)
      {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
         MessageBox("Unable to Save Contact Information\r" + Err.Message, "Error", MB_OK + MB_ICONERROR);
      }
    }

}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::FormShow(TObject *Sender)
{
  //FormResize(NULL);
  if(Info.EMail == "")
     Info.Clear();
  bool lmEnabled = TGlobalSettings::Instance().LoyaltyMateEnabled;
  gbLoyaltyMateActivation->Visible = lmEnabled && Info.MemberType < 2;
  if (lmEnabled && Info.MemberType < 2)
   {
        toggleActivateAccountButton();
   }
  DrawContactDetail();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::FormResize(TObject *Sender)
{
   if (Tag != Screen->Width)
   {
	  int Temp = Tag;
	  Tag = Screen->Width;

	  ScaleBy(Screen->Width, Temp);
   }
   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;
}

void __fastcall TfrmMemberCreation::TouchBtn1MouseClick(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::TouchBtn2MouseClick(TObject *Sender)
{
   TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl );
   DBTransaction.StartTransaction();
   int key = TDBContacts::GenerateLoyaltymateKey(DBTransaction);
   DBTransaction.Commit();
   AnsiString siteId=  TGlobalSettings::Instance().SiteID  ;
   GenMailId="GiftCard" + siteId +  AnsiString(key) + "@loyalty.com";
   Info.EMail=GenMailId;

    if (TGlobalSettings::Instance().LoyaltyMateEnabled
        && Info.CloudUUID != TLoyaltyMateUtilities::GetLoyaltyMateDisabledCloudUUID()
        && !Info.ValidEmail())
       {
          MessageBox("You must enter a valid Email.", "Error", MB_OK + MB_ICONERROR);
       }
       else
       {
          try
          {
              Info.LastModified = Now();
              Info.Name="GIFTCARD";
              Info.Points.PointsRules << eprNeverEarnsPoints ;
              Info.MemberType=2;
              ModalResult = mrOk;
              if (Info.SiteID == 0)
              {
                Info.SiteID = TGlobalSettings::Instance().SiteID;
              }
          }
          catch(Exception & Err)
          {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
             MessageBox("Unable to Save Contact Information\r" + Err.Message, "Error", MB_OK + MB_ICONERROR);
          }
       }

}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::tbtnDayMouseClick(TObject *Sender)
{
 std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
   frmTouchNumpad->Caption = "Enter the Day 1st - 31st";
   frmTouchNumpad->btnSurcharge->Caption = "Ok";
   frmTouchNumpad->btnSurcharge->Visible = true;
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->PreSelect = true;
   frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = DayOf(Info.DateOfBirth);

   if (frmTouchNumpad->ShowModal() == mrOk)
   {
	  tbtnDay->Caption = IntToStr(frmTouchNumpad->INTResult);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::tbtnMonthMouseClick(TObject *Sender)
{
std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
   frmTouchNumpad->Caption = "Enter the Month 1 - 12";
   frmTouchNumpad->btnSurcharge->Caption = "Ok";
   frmTouchNumpad->btnSurcharge->Visible = true;
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->PreSelect = true;
   frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = MonthOf(Info.DateOfBirth); ;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
	  tbtnMonth->Caption = IntToStr(frmTouchNumpad->INTResult);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::tbtnYearMouseClick(TObject *Sender)
{
 std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
   frmTouchNumpad->Caption = "Enter the Year xxxx";
   frmTouchNumpad->btnSurcharge->Caption = "Ok";
   frmTouchNumpad->btnSurcharge->Visible = true;
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->PreSelect = true;
   frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = YearOf(Info.DateOfBirth); ; ;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
	  tbtnYear->Caption = IntToStr(frmTouchNumpad->INTResult);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::tbtnClearBirthdayMouseClick(TObject *Sender)
{
 Info.DateOfBirth = 0;
   tbtnYear->Caption = "1899";
   tbtnMonth->Caption = "12";
   tbtnDay->Caption = "30";
}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::tbtnSetBirthdayMouseClick(TObject *Sender)
{
TDateTime Birthday;
   if (!TryEncodeDate(StrToInt(tbtnYear->Caption), StrToInt(tbtnMonth->Caption), StrToInt(tbtnDay->Caption), Birthday))
   {
	  MessageBox("Invalid Date, Please Fix and Try again", "Invalid Date", MB_OK + MB_ICONERROR);
   }
   else
   {
	  Info.DateOfBirth = Birthday;
   }
}
//---------------------------------------------------------------------------
void TfrmMemberCreation::DrawContactDetail()
{

   SetupCustomerInfoPointers();
   DisplayCustomerDataFromPointers();
   tbtnYear->Caption = IntToStr(YearOf(Info.DateOfBirth));
   tbtnMonth->Caption = IntToStr(MonthOf(Info.DateOfBirth));
   tbtnDay->Caption = IntToStr(DayOf(Info.DateOfBirth));
}
//---------------------------------------------------------------------------
void TfrmMemberCreation::SetupCustomerInfoPointers()
{
    CustomerInfoPointers[0] = Info.EMail;
    CustomerInfoPointers[1] = Info.Name;
    CustomerInfoPointers[1] = Info.Surname;
    CustomerInfoPointers[3] = Info.Phone;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::btnActivateLoyaltyMateMouseClick(TObject *Sender)

{
	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = "";
	frmTouchKeyboard->Caption = "Enter Valid Email Address for LoyaltyMate";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		Info.EMail = frmTouchKeyboard->KeyboardText;
        if(Info.ValidEmail())
        {
            getMemberDetailsFromActivationEmail();
            toggleActivateAccountButton();
        }
	}
}

void TfrmMemberCreation::getMemberDetailsFromActivationEmail()
{
    TManagerSyndCode managerSyndCode = TDeviceRealTerminal::Instance().ManagerMembership->GetSyndicateCodeManager();
    CurrentSyndicateCode =  managerSyndCode.GetCommunicationSyndCode();

    bool replacePointsFromCloud = true;
	TLoyaltyMateDownloadMemberThread* loyaltyMemberDownloadThread = new TLoyaltyMateDownloadMemberThread(CurrentSyndicateCode,replacePointsFromCloud);
	loyaltyMemberDownloadThread->FreeOnTerminate = true;
	loyaltyMemberDownloadThread->UUID = Info.CloudUUID;
	loyaltyMemberDownloadThread->DownLoadFromUUID = false;
    loyaltyMemberDownloadThread->MemberCode = Info.MemberCode;
	loyaltyMemberDownloadThread->DownLoadFromCode = false;
    loyaltyMemberDownloadThread->MemberEmail = Info.EMail;
	loyaltyMemberDownloadThread->DownLoadFromEmail = true;

    //display the dialog box
	std::auto_ptr<TfrmLoyaltyMateOperationDialogBox> loyaltyMateOperationDialogBox = std::auto_ptr<TfrmLoyaltyMateOperationDialogBox>(TfrmLoyaltyMateOperationDialogBox::Create<TfrmLoyaltyMateOperationDialogBox>(this));
	loyaltyMateOperationDialogBox->OperationDescription = "Downloading member information from LoyaltyMate... Please Wait.";
	loyaltyMateOperationDialogBox->OperationTitle 		= "LoyaltyMate Operation";
	loyaltyMateOperationDialogBox->DownloadThread 		= loyaltyMemberDownloadThread;
	loyaltyMateOperationDialogBox->Info			  		= Info;
	loyaltyMemberDownloadThread->Start();
    ShortInt dialogResult = loyaltyMateOperationDialogBox->ShowModal();

	if(dialogResult == mrOk)
	{
        //download complete, copy the values across and display the member information
        //download complete, copy the values across and display the member information
		Info.CloudUUID       = loyaltyMateOperationDialogBox->Info.CloudUUID;
		Info.Phone          = loyaltyMateOperationDialogBox->Info.Phone;
		Info.Mobile          = loyaltyMateOperationDialogBox->Info.Mobile;
		Info.EMail           = loyaltyMateOperationDialogBox->Info.EMail;
		Info.Name            = loyaltyMateOperationDialogBox->Info.Name;
		Info.Surname         = loyaltyMateOperationDialogBox->Info.Surname;
		Info.MailingAddress         = loyaltyMateOperationDialogBox->Info.MailingAddress;
        Info.LocationAddress         = loyaltyMateOperationDialogBox->Info.LocationAddress;
		Info.Title         = loyaltyMateOperationDialogBox->Info.Title;
		Info.ActivationToken = loyaltyMateOperationDialogBox->Info.ActivationToken;
		Info.DateOfBirth     = loyaltyMateOperationDialogBox->Info.DateOfBirth;
		Info.LastVisit       = loyaltyMateOperationDialogBox->Info.LastVisit;
		Info.TierLevel       = loyaltyMateOperationDialogBox->Info.TierLevel;
		Info.LastBirthdayProcessed       = loyaltyMateOperationDialogBox->Info.LastBirthdayProcessed;
		Info.MemberType       = loyaltyMateOperationDialogBox->Info.MemberType;
		Info.ActivationDate       = loyaltyMateOperationDialogBox->Info.ActivationDate;
        Info.PreviousYearPoint       = loyaltyMateOperationDialogBox->Info.PreviousYearPoint;
        Info.CurrentYearPoint       = loyaltyMateOperationDialogBox->Info.CurrentYearPoint;
        Info.AvailableBDPoint       = loyaltyMateOperationDialogBox->Info.AvailableBDPoint;
        Info.AvailableFVPoint       = loyaltyMateOperationDialogBox->Info.AvailableFVPoint;
        Info.MemberCode       = loyaltyMateOperationDialogBox->Info.MemberCode;
        Info.MembershipNumber  = loyaltyMateOperationDialogBox->Info.MembershipNumber;
        Info.SiteID  = loyaltyMateOperationDialogBox->Info.SiteID;
        Info.LastModified  = loyaltyMateOperationDialogBox->Info.LastModified;
        Info.IsFirstVisitRewarded  = loyaltyMateOperationDialogBox->Info.IsFirstVisitRewarded;
        Info.MemberCode  = loyaltyMateOperationDialogBox->Info.MemberCode;
        TPointsRulesSetUtils().Expand(loyaltyMateOperationDialogBox->Info.PointRule, Info.Points.PointsRules);
        if(Info.Points.getPointsBalance() == 0)
        {
            Info.Points = loyaltyMateOperationDialogBox->Info.Points;
        }
         if (Info.SiteID == 0)
         {
            Info.SiteID = TGlobalSettings::Instance().SiteID;
         }
         Info.LastModified = Now();
		 Info.MemberType=1;
         ModalResult = mrOk;

        //if user is already in the database (i.e. this is being used to replace a card) then set contact key, site id and member number
        /*Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        int currentContactKey = Info.ContactKey;
        int currentSiteID = Info.SiteID;
        if(TLoyaltyMateUtilities::GetContactByCloudUUID(DBTransaction, Info.CloudUUID, Info.ContactKey, Info.SiteID))
        {
            TMMContactInfo tempInfo;
            TDBContacts::GetContactDetails(DBTransaction, Info.ContactKey, tempInfo);
            Info.SiteID = tempInfo.SiteID;
            Info.MembershipNumber = tempInfo.MembershipNumber;
        }
        else
        {
            Info.ContactKey = currentContactKey;
            Info.SiteID = currentSiteID;
        }
        DBTransaction.Commit(); */

        //DrawContactDetail();
	}
}

void TfrmMemberCreation::toggleActivateAccountButton()
{
    bool cardActivated = TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(Info.CloudUUID);

    if (cardActivated)
    {
        lbeLoyaltyMateActive->Caption = "Account Activated";
        lbeLoyaltyMateActive->Font->Color = clGreen;
        btnActivateLoyaltyMate->Enabled = false;
    }
}


