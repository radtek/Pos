//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MemberCreation.h"
#include "EditCustomer.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "CardSwipe.h"
#include "MMMessageBox.h"
#include "Membership.h"
#include "SmartCardAPI.h"
#include "VerticalSelect.h"
#include "DateUtils.hpp"
#include "MMLogging.h"
#include "DBGroups.h"
#include "Users.h"
#include "DeviceRealTerminal.h"
#include "DBContacts.h"
#include "GlobalSettings.h"
#include "ManagerLoyaltyMate.h"
#include "PointsRulesSetUtils.h"
#include "LoyaltyMateUtilities.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmMemberCreation::TfrmMemberCreation(TComponent* Owner, TMMContactInfo &info)
    : TForm(Owner),Info(info)
{
   if(Info.EMail == "")
     Info.Clear();
}

// ---------------------------------------------------------------------------

void TfrmMemberCreation::DisplayCustomerDataFromPointers()
{
     lbeEmail->Caption = CustomerInfoPointers[0];
     lbeName->Caption = CustomerInfoPointers[1].Trim();
     lbeLastName->Caption = CustomerInfoPointers[2].Trim();
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
            frmTouchKeyboard->MaxLength = 50;
            break;
        case 1:
            Caption = "Enter First Name";
            frmTouchKeyboard->KeyboardText = Info.Name;
            break;
        case 2:
        Caption = "Enter Last Name";
        frmTouchKeyboard->KeyboardText = Info.Surname;
        break;
        default:
            break;
    }
    frmTouchKeyboard->Caption = Caption;
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
         CustomerInfoPointers[btn->Tag] =  frmTouchKeyboard->KeyboardText;
         Info.EMail=  CustomerInfoPointers[0].Trim();
         Info.Name=  CustomerInfoPointers[1].Trim();
         Info.Surname = CustomerInfoPointers[2].Trim();
         Info.Phone =  CustomerInfoPointers[3].Trim();
		 DisplayCustomerDataFromPointers();
	}
}
//---------------------------------------------------------------------------
int TfrmMemberCreation::CheckEmailInDB(AnsiString email)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    int count = 0;
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        if(TGlobalSettings::Instance().LoyaltyMateEnabled )
        {
            IBInternalQuery->SQL->Text = "SELECT count(EMAIL) FROM CONTACTS where EMAIL='" + Info.EMail + "'";
        }
        else
        {
            IBInternalQuery->SQL->Text = "SELECT count(EMAIL) FROM CONTACTS where EMAIL='" + Info.EMail + "'" "AND CONTACT_TYPE <> :CONTACT_TYPE";
            IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = eDeletedMember;
        }
        IBInternalQuery->ExecQuery();
        DBTransaction.Commit();
        count = IBInternalQuery->Fields[0]->AsInteger;
    }
    catch(Exception & Err)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
    }
    return count;
}
//---------------------------------------------------------------------------
void TfrmMemberCreation::RefreshScreen()
{
    Info.Clear();
    Info.Surname = "";
//    tbtnClearBirthdayMouseClick(NULL);
    ClearBirthday();
    SetupCustomerInfoPointers();
    DisplayCustomerDataFromPointers();
//    toggleActivateAccountButton();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::btnOkMouseClick(TObject *Sender)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    if(cbNoEmail->Checked && TGlobalSettings::Instance().LoyaltyMateEnabled)
       Info.EMail = TLoyaltyMateUtilities::GetAutoGeneratedMemberEmail(DBTransaction);
//    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
//    IBInternalQuery->Close();
//    IBInternalQuery->SQL->Text = "SELECT count(EMAIL) FROM CONTACTS where EMAIL='" + Info.EMail + "'";
//    IBInternalQuery->ExecQuery();
//    DBTransaction.Commit();
    int	emailcount = CheckEmailInDB(Info.EMail);
//    IBInternalQuery->Fields[0]->AsInteger;
    AnsiString firstNameMessage = "First Name ";
    AnsiString lastNameMessage = "Last Name ";
    AnsiString missingMessage = "You have missed following manadatory field/s :- \n";
    AnsiString errorMessage = "";
    int missingMessageSize = missingMessage.Length();
    if (TGlobalSettings::Instance().LoyaltyMateEnabled &&
        Info.CloudUUID != TLoyaltyMateUtilities::GetLoyaltyMateDisabledCloudUUID()  &&
       !Info.ValidEmail())
    {
        errorMessage += "You must enter a valid Email. \n";
    }
    else if(Info.EMail != "" && emailcount > 0)
    {
        RefreshScreen();
        MessageBox("Member already exists!!!", "Error", MB_OK + MB_ICONERROR);
        return;
    }
    if(Info.Name == NULL || Info.Name.Trim() == "")
    {
       missingMessage += "Name \n";
    }
    if(Info.Surname == NULL || Info.Surname.Trim() == "")
    {
       missingMessage += "Surname \n";
    }
    if(missingMessage.Length() != missingMessageSize)
    {
        errorMessage += missingMessage;
    }
    if(errorMessage.Length() != 0)
        MessageBox(errorMessage,"Error", MB_OK + MB_ICONERROR);

    else if(Info.ValidEmail() && Info.Name != NULL && Info.Name.Trim() != "" &&
       Info.Surname != NULL || Info.Surname.Trim() != "")
    {
        if(!Info.ValidateFirstName(firstNameMessage))
           MessageBox(firstNameMessage + ".", "Error", MB_OK + MB_ICONERROR);
        else if(!Info.ValidateLastName(lastNameMessage))
           MessageBox(lastNameMessage + ".", "Error", MB_OK + MB_ICONERROR);
//        else if(Info.Phone != "" && Info.Phone != NULL && Info.Phone.Length() < 5)
//           MessageBox("Phone number should be greater than 4 digits.", "Invalid Moble Number", MB_ICONERROR);
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
             if(!TGlobalSettings::Instance().UseMemberSubs)
             {
                int PointRule = 0;
                PointRule |= eprAllowDiscounts;
                PointRule |= eprFinancial;
                TPointsRulesSetUtils().ExpandSubs(PointRule, Info.Points.PointsRulesSubs);
             }
          }
          catch(Exception & Err)
          {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
             MessageBox("Unable to Save Contact Information\r" + Err.Message, "Error", MB_OK + MB_ICONERROR);
          }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMemberCreation::FormShow(TObject *Sender)
{
  bool lmEnabled = TGlobalSettings::Instance().LoyaltyMateEnabled;
//  gbLoyaltyMateActivation->Visible = lmEnabled && Info.MemberType < 2;
//  if (lmEnabled && Info.MemberType < 2)
//   {
//        toggleActivateAccountButton();
//   }
  DrawContactDetail();
  cbNoEmail->Enabled = Info.EMail == "" || Info.EMail == NULL;
  if(TGlobalSettings::Instance().MembershipType != MembershipTypeMenuMate || !lmEnabled)
  {
    cbNoEmail->Visible = false;
  }
//  TouchBtn2->Enabled = TGlobalSettings::Instance().LoyaltyMateEnabled;
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
//void __fastcall TfrmMemberCreation::TouchBtn2MouseClick(TObject *Sender)
//{
//   TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl );
//   DBTransaction.StartTransaction();
//   Info.EMail = TLoyaltyMateUtilities::GetAutoGeneratedGiftCardEmail(DBTransaction);
//   DBTransaction.Commit();
//   if (TGlobalSettings::Instance().LoyaltyMateEnabled
//        && Info.CloudUUID != TLoyaltyMateUtilities::GetLoyaltyMateDisabledCloudUUID()
//        && !Info.ValidEmail())
//       {
//          MessageBox("You must enter a valid Email.", "Error", MB_OK + MB_ICONERROR);
//       }
//       else
//       {
//          try
//          {
//              Info.LastModified = Now();
//              Info.Name="GIFTCARD";
//              Info.Points.PointsRules << eprNeverEarnsPoints ;
//              Info.MemberType=2;
//              ModalResult = mrOk;
//              if (Info.SiteID == 0)
//              {
//                Info.SiteID = TGlobalSettings::Instance().SiteID;
//              }
//          }
//          catch(Exception & Err)
//          {
//             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
//             MessageBox("Unable to Save Contact Information\r" + Err.Message, "Error", MB_OK + MB_ICONERROR);
//          }
//       }
//}
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
   if(!SetBirthday())
     tbtnDayMouseClick(NULL);
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
   if(!SetBirthday())
     tbtnMonthMouseClick(NULL);
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
   if(!SetBirthday())
      tbtnYearMouseClick(NULL);
}
//---------------------------------------------------------------------------
void TfrmMemberCreation::ClearBirthday()
{
   Info.DateOfBirth = 0;
   tbtnYear->Caption = "1899";
   tbtnMonth->Caption = "12";
   tbtnDay->Caption = "30";
}
//---------------------------------------------------------------------------
bool TfrmMemberCreation::SetBirthday()
{
   bool retValue = false;
   TDateTime Birthday;
   if (!TryEncodeDate(StrToInt(tbtnYear->Caption), StrToInt(tbtnMonth->Caption), StrToInt(tbtnDay->Caption), Birthday) ||
      (StrToInt(tbtnYear->Caption) < 1899) || (YearOf(Now()) < StrToInt(tbtnYear->Caption)))
   {
	  MessageBox("Invalid Date of Birth, Please Fix and Try again", "Invalid DOB", MB_OK + MB_ICONERROR);
   }
   else
   {
      if(Birthday <= Now())
      {
          Info.DateOfBirth = Birthday;
          retValue = true;
      }
      else
          MessageBox("Invalid Date of Birth, Please Fix and Try again", "Invalid DOB", MB_OK + MB_ICONERROR);
   }
   return retValue;
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
    CustomerInfoPointers[2] = Info.Surname;
    CustomerInfoPointers[3] = Info.Phone;
}
//---------------------------------------------------------------------------
//void __fastcall TfrmMemberCreation::btnActivateLoyaltyMateMouseClick(TObject *Sender)
//
//{
//    if(Info.Name != "" || Info.Surname != "" || Info.EMail != "")
//    {
//        if(MessageBox("Using Activate Member will erase current details.Do you wish to Proceed?", "Warning",
//			MB_ICONWARNING + MB_OKCANCEL) != ID_OK)
//        {
//            return;
//        }
//        else
//        {
//            RefreshScreen();
//        }
//    }
//	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
//	frmTouchKeyboard->AllowCarriageReturn = false;
//	frmTouchKeyboard->StartWithShiftDown = false;
//	frmTouchKeyboard->KeyboardText = "";
//	frmTouchKeyboard->Caption = "Enter Valid Email Address for LoyaltyMate";
//	if (frmTouchKeyboard->ShowModal() == mrOk)
//	{
//		Info.EMail = frmTouchKeyboard->KeyboardText;
//        if(Info.ValidEmail())
//        {
//            getMemberDetailsFromActivationEmail();
//            toggleActivateAccountButton();
//        }
//	}
//}
//
//void TfrmMemberCreation::getMemberDetailsFromActivationEmail()
//{
//    TManagerSyndCode managerSyndCode = TDeviceRealTerminal::Instance().ManagerMembership->GetSyndicateCodeManager();
//    CurrentSyndicateCode =  managerSyndCode.GetCommunicationSyndCode();
//
//    bool replacePointsFromCloud = true;
//	TLoyaltyMateDownloadMemberThread* loyaltyMemberDownloadThread = new TLoyaltyMateDownloadMemberThread(CurrentSyndicateCode,replacePointsFromCloud);
//	loyaltyMemberDownloadThread->FreeOnTerminate = true;
//	loyaltyMemberDownloadThread->UUID = Info.CloudUUID;
//	loyaltyMemberDownloadThread->DownLoadFromUUID = false;
//    loyaltyMemberDownloadThread->MemberCode = Info.MemberCode;
//	loyaltyMemberDownloadThread->DownLoadFromCode = false;
//    loyaltyMemberDownloadThread->MemberEmail = Info.EMail;
//	loyaltyMemberDownloadThread->DownLoadFromEmail = true;
//
//    //display the dialog box
//	std::auto_ptr<TfrmLoyaltyMateOperationDialogBox> loyaltyMateOperationDialogBox = std::auto_ptr<TfrmLoyaltyMateOperationDialogBox>(TfrmLoyaltyMateOperationDialogBox::Create<TfrmLoyaltyMateOperationDialogBox>(this));
//	loyaltyMateOperationDialogBox->OperationDescription = "Downloading member information from LoyaltyMate... Please Wait.";
//	loyaltyMateOperationDialogBox->OperationTitle 		= "LoyaltyMate Operation";
//	loyaltyMateOperationDialogBox->DownloadThread 		= loyaltyMemberDownloadThread;
//	loyaltyMateOperationDialogBox->Info			  		= Info;
//	loyaltyMemberDownloadThread->Start();
//    ShortInt dialogResult = loyaltyMateOperationDialogBox->ShowModal();
//
//	if(dialogResult == mrOk)
//	{
//        //download complete, copy the values across and display the member information
//        //download complete, copy the values across and display the member information
//		Info.CloudUUID       = loyaltyMateOperationDialogBox->Info.CloudUUID;
//		Info.Phone          = loyaltyMateOperationDialogBox->Info.Phone;
//		Info.Mobile          = loyaltyMateOperationDialogBox->Info.Mobile;
//		Info.EMail           = loyaltyMateOperationDialogBox->Info.EMail;
//		Info.Name            = loyaltyMateOperationDialogBox->Info.Name;
//		Info.Surname         = loyaltyMateOperationDialogBox->Info.Surname;
//		Info.MailingAddress         = loyaltyMateOperationDialogBox->Info.MailingAddress;
//        Info.LocationAddress         = loyaltyMateOperationDialogBox->Info.LocationAddress;
//		Info.Title         = loyaltyMateOperationDialogBox->Info.Title;
//		Info.ActivationToken = loyaltyMateOperationDialogBox->Info.ActivationToken;
//		Info.DateOfBirth     = loyaltyMateOperationDialogBox->Info.DateOfBirth;
//		Info.LastVisit       = loyaltyMateOperationDialogBox->Info.LastVisit;
//		Info.TierLevel       = loyaltyMateOperationDialogBox->Info.TierLevel;
//		Info.LastBirthdayProcessed       = loyaltyMateOperationDialogBox->Info.LastBirthdayProcessed;
//		Info.MemberType       = loyaltyMateOperationDialogBox->Info.MemberType;
//		Info.ActivationDate       = loyaltyMateOperationDialogBox->Info.ActivationDate;
//        Info.PreviousYearPoint       = loyaltyMateOperationDialogBox->Info.PreviousYearPoint;
//        Info.CurrentYearPoint       = loyaltyMateOperationDialogBox->Info.CurrentYearPoint;
//        Info.AvailableBDPoint       = loyaltyMateOperationDialogBox->Info.AvailableBDPoint;
//        Info.AvailableFVPoint       = loyaltyMateOperationDialogBox->Info.AvailableFVPoint;
//        Info.MemberCode       = loyaltyMateOperationDialogBox->Info.MemberCode;
//        Info.MembershipNumber  = loyaltyMateOperationDialogBox->Info.MembershipNumber;
//        Info.SiteID  = loyaltyMateOperationDialogBox->Info.SiteID;
//        Info.LastModified  = loyaltyMateOperationDialogBox->Info.LastModified;
//        Info.IsFirstVisitRewarded  = loyaltyMateOperationDialogBox->Info.IsFirstVisitRewarded;
//        Info.MemberCode  = loyaltyMateOperationDialogBox->Info.MemberCode;
//        TPointsRulesSetUtils().Expand(loyaltyMateOperationDialogBox->Info.PointRule, Info.Points.PointsRules);
//        if(Info.Points.getPointsBalance() == 0)
//        {
//            Info.Points = loyaltyMateOperationDialogBox->Info.Points;
//        }
//         if (Info.SiteID == 0)
//         {
//            Info.SiteID = TGlobalSettings::Instance().SiteID;
//         }
//         Info.LastModified = Now();
//		 Info.MemberType=1;
//        CustomerInfoPointers[0] =  Info.EMail;
//        CustomerInfoPointers[1] =  Info.Name;
//        CustomerInfoPointers[2] =  Info.Surname;
//        CustomerInfoPointers[3] =  Info.Phone;
//        DisplayCustomerDataFromPointers();
//        AnsiString monthYear = "";
//        if(Info.DateOfBirth.DateString().Pos("/") != 0)
//        {
//            tbtnDay->Caption = Info.DateOfBirth.DateString().SubString(0,Info.DateOfBirth.DateString().Pos("/") -1);
//
//            monthYear = Info.DateOfBirth.DateString().SubString(Info.DateOfBirth.DateString().Pos("/")+1,
//                        Info.DateOfBirth.DateString().Length() - Info.DateOfBirth.DateString().Pos("/")+1);
//            tbtnMonth->Caption = monthYear.SubString(0,monthYear.Pos("/") -1 );
//
//            tbtnYear->Caption = monthYear.SubString(monthYear.Pos("/")+1,monthYear.Length()-monthYear.Pos("/")+1);
//        }
//        if(CheckEmailInDB(Info.EMail) > 0)
//        {
//            RefreshScreen();
//            MessageBox("Member already exists!!! \nPlease try with different details.", "Error", MB_OK + MB_ICONERROR);
//        }
////         ModalResult = mrOk;
////
//        //if user is already in the database (i.e. this is being used to replace a card) then set contact key, site id and member number
//        /*Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
//        DBTransaction.StartTransaction();
//        int currentContactKey = Info.ContactKey;
//        int currentSiteID = Info.SiteID;
//        if(TLoyaltyMateUtilities::GetContactByCloudUUID(DBTransaction, Info.CloudUUID, Info.ContactKey, Info.SiteID))
//        {
//            TMMContactInfo tempInfo;
//            TDBContacts::GetContactDetails(DBTransaction, Info.ContactKey, tempInfo);
//            Info.SiteID = tempInfo.SiteID;
//            Info.MembershipNumber = tempInfo.MembershipNumber;
//        }
//        else
//        {
//            Info.ContactKey = currentContactKey;
//            Info.SiteID = currentSiteID;
//        }
//        DBTransaction.Commit(); */
//
//        //DrawContactDetail();
//	}
//    else
//    {
//        Info.Clear();
//        Info.Surname = "";
////        tbtnClearBirthdayMouseClick(NULL);
//        ClearBirthday();
//        SetupCustomerInfoPointers();
//        DisplayCustomerDataFromPointers();
//        toggleActivateAccountButton();
//        MessageBox("Email entered is not a valid Email.","Information",MB_ICONINFORMATION+MB_OK);
//    }
//}

//void TfrmMemberCreation::toggleActivateAccountButton()
//{
//    bool cardActivated = TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(Info.CloudUUID);
//
//    if (cardActivated)
//    {
//        lbeLoyaltyMateActive->Caption = "Member Activated";
//        lbeLoyaltyMateActive->Font->Color = clGreen;
//        btnActivateLoyaltyMate->Enabled = false;
//    }
//    else
//    {
//        lbeLoyaltyMateActive->Caption = "Member Not Activated";
//        lbeLoyaltyMateActive->Font->Color = clMaroon;
//        btnActivateLoyaltyMate->Enabled = true;
//    }
//}

void __fastcall TfrmMemberCreation::cbNoEmailMouseClick(TObject *Sender)
{
   btnEmail->Enabled = !cbNoEmail->Checked;
}


