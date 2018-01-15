// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

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
#include "MM_DBCore.h"
#include "DeviceRealControl.h"
#include "DeviceRealTerminal.h"
#include "DBContacts.h"
#include "GlobalSettings.h"
#include "ManagerLoyaltyMate.h"
#include "LoyaltyMateOperationDialogBox.h"
#include "PointsRulesSetUtils.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"
#pragma link "TouchKeyboard"

#define check_space 0x20

// TfrmEditCustomer *frmEditCustomer;
TForm * TfrmEditCustomer::WinOwner = NULL;

// ---------------------------------------------------------------------------
__fastcall TfrmEditCustomer::TfrmEditCustomer(TComponent* Owner) :
	TZForm(Owner), DiscountList(new TStringList)
{

}
// ---------------------------------------------------------------------------
__fastcall TfrmEditCustomer::~TfrmEditCustomer()
{
}

void __fastcall TfrmEditCustomer::CreateParams(Controls::TCreateParams &params)
{
   TForm::CreateParams(params);
   if (WinOwner)
   {
	  params.WndParent = WinOwner->Handle;
	  WinOwner = NULL;
   }
}
// ---------------------------------------------------------------------------
TfrmEditCustomer *TfrmEditCustomer::Create(TForm* Owner)
{
   WinOwner = Owner;
   return new TfrmEditCustomer(Owner);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::FormShow(TObject *Sender)
{
   FormResize(NULL);
   ClearAllCards = false;
   IsProxCardChange = false;
   if (!Editing)
   {
	std::vector<ContactGroup> temp;
	temp = Info.availableGroups;
	Info.Clear();
	Info.availableGroups = temp;
    tbtnAllowedTab->Caption = "Not Allowed Tab";
   }
   tbtnAllowedTab->Latched = Info.TabEnabled;

   if (tbtnAllowedTab->Latched)
   {
	  tbtnAllowedTab->Caption = "Allowed Tab";
   }

   DrawContactDetails();
   UpdateSmartCardSpace();
   tbtnUserProfileMouseClick(tbtnUserProfile);
   SetGridColors(tgDiscounts);
   edFirstName->SetFocus();
   if(Info.DateOfBirth.Val == 0)
   {
     DateTimePicker1->Date = Now();
   }
   cbNoEmail->Enabled = Info.EMail == "" || Info.EMail == NULL;
   edEmail->Enabled = Info.EMail == "" || Info.EMail == NULL;
   if(TGlobalSettings::Instance().MembershipType != MembershipTypeMenuMate || !TGlobalSettings::Instance().LoyaltyMateEnabled)
   {
        cbNoEmail->Visible = false;
        if(!TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
              edEmail->Enabled= true;
        }
   }
   btnRemoveCard->Visible = !TGlobalSettings::Instance().LoyaltyMateEnabled;
   tbtnClearAllCards->Visible = !TGlobalSettings::Instance().LoyaltyMateEnabled;
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction.StartTransaction();
   int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, "Globals");
   TManagerVariable::Instance().GetProfileBool(DBTransaction,  GlobalProfileKey, vmUseMemberSubs, TGlobalSettings::Instance().UseMemberSubs);
   DBTransaction.Commit();
}
// ---------------------------------------------------------------------------
void TfrmEditCustomer::DrawContactDetails()
{

    SetupCustomerInfoPointers();
    DisplayCustomerDataFromPointers();

    lbePoints->Caption = "Points Total : " + FloatToStrF(Info.Points.getPointsBalance(), ffFixed, 15, 2);

   //CheckBoxCharges->Checked = Info.Charges;
   if(!Editing)
      UpdatePointsRuleNewMember();
   else
      UpdatePointsRuleOldMember();
}
//----------------------------------------------------------------------------
void TfrmEditCustomer::UpdatePointsRuleNewMember()
{
    if(TGlobalSettings::Instance().UseMemberSubs)
    {
        tcbeprNoPointsRedemption->Latched = true;
        tcbeprNoPointsPurchases->Latched = true;
        tcbeprNeverEarnsPoints->Latched = true;
        int PointRule = 0;
        PointRule |= eprNeverEarnsPoints;
        PointRule |= eprNoPointsPurchases;
        PointRule |= eprNoPointsRedemption;
        TPointsRulesSetUtils().Expand(PointRule, Info.Points.PointsRules);
        TPointsRulesSetUtils().ExpandSubs(PointRule, Info.Points.PointsRulesSubs);
    }
    else
    {
        tcbeprFinancial->Visible = false;
        tcbeprNoPointsRedemption->Latched = false;
        tcbeprNoPointsPurchases->Latched = false;
        tcbeprEarnsPointsWhileRedeemingPoints->Latched = false;
        tcbeprNeverEarnsPoints->Latched = false;
        tcbeprAllowedNegitive->Latched = false;
        tcbeprFinancial->Latched = true;
        if(!TPaySubsUtility::IsLocalLoyalty())
        {
            tcbeprFinancial->Visible = false;
            tcbeprAllowDiscounts->Visible = false;
        }
        else
        {
            tcbeprAllowDiscounts->Visible = true;
        }
        tcbeprAllowDiscounts->Latched = true;
        int PointRule = 0;
        PointRule |= eprAllowDiscounts;
        PointRule |= eprFinancial;
        TPointsRulesSetUtils().ExpandSubs(PointRule, Info.Points.PointsRulesSubs);
    }
}
//----------------------------------------------------------------------------
void TfrmEditCustomer::UpdatePointsRuleOldMember()
{
   if (Info.Points.PointsRules.Contains(eprNoPointsRedemption))
	  tcbeprNoPointsRedemption->Latched = true;
   else
	  tcbeprNoPointsRedemption->Latched = false;

   if (Info.Points.PointsRules.Contains(eprNoPointsPurchases))
	  tcbeprNoPointsPurchases->Latched = true;
   else
	  tcbeprNoPointsPurchases->Latched = false;

   if (Info.Points.PointsRules.Contains(eprEarnsPointsWhileRedeemingPoints))
	  tcbeprEarnsPointsWhileRedeemingPoints->Latched = true;
   else
	  tcbeprEarnsPointsWhileRedeemingPoints->Latched = false;

   if (Info.Points.PointsRules.Contains(eprOnlyEarnsPointsWhileRedeemingPoints))
	  tcbeprOnlyEarnsPointsWhileRedeemingPoints->Latched = true;
   else
	  tcbeprOnlyEarnsPointsWhileRedeemingPoints->Latched = false;

   if (Info.Points.PointsRules.Contains(eprNeverEarnsPoints))
	  tcbeprNeverEarnsPoints->Latched = true;
   else
	  tcbeprNeverEarnsPoints->Latched = false;

   if (Info.Points.PointsRules.Contains(eprAllowedNegitive))
	  tcbeprAllowedNegitive->Latched = true;
   else
	  tcbeprAllowedNegitive->Latched = false;


   if(!TGlobalSettings::Instance().UseMemberSubs)
   {
      tcbeprFinancial->Visible = false;
   }
   else
   {
       if(Info.Points.PointsRulesSubs.Contains(eprFinancial))
          tcbeprFinancial->Latched = true;
       else
          tcbeprFinancial->Latched = false;
   }
   if(!TPaySubsUtility::IsLocalLoyalty())
   {
      tcbeprAllowDiscounts->Visible = false;
   }
   else
   {
     if(Info.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
        tcbeprAllowDiscounts->Latched = true;
     else
       tcbeprAllowDiscounts->Latched = false;
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::WMDisplayChange(TWMDisplayChange& Message)
{
   FormResize(NULL);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::FormResize(TObject *Sender)
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
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::btnOkClick(TObject *Sender)
{
    edFirstName->Text = edFirstName->Text.Trim();
    edLastName->Text = edLastName->Text.Trim();
    edEmail->Text = edEmail->Text.Trim();
    if(cbNoEmail->Checked && TGlobalSettings::Instance().LoyaltyMateEnabled)
    {
       Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
       DBTransaction.StartTransaction();
       Info.EMail = TLoyaltyMateUtilities::GetAutoGeneratedMemberEmail(DBTransaction);
       edEmail->Text = Info.EMail;
       DBTransaction.Commit();
    }
   if(edFirstName->Text == ""  && edEmail->Text == "")
   {
     MessageBox("You must enter all mandatoy fields.", "Error", MB_OK + MB_ICONERROR);
   }
   else
   {
        bool isNameValid = true;
        bool isEmailValid = false;
        bool isMobileValid = false;
        bool isPhoneValid = true;
        Info.EMail = edEmail->Text;
        Info.Name = edFirstName->Text;
        Info.Surname = edLastName->Text;

        AnsiString firstNameMessage = "First Name ";
        AnsiString lastNameMessage = "Last Name ";
        if (edFirstName->Text.Trim() == "")
        {
              MessageBox("You must enter a First Name.", "Error", MB_OK + MB_ICONERROR);
              edFirstName->SetFocus();
              isNameValid = false;
        }
        if(isNameValid && !Info.ValidateFirstName(firstNameMessage))
        {
           MessageBox(firstNameMessage + ".", "Error", MB_OK + MB_ICONERROR);
           edFirstName->SetFocus();
           isNameValid = false;
        }
        if (isNameValid && edLastName->Text.Trim() == "")
        {
              MessageBox("You must enter a Last Name.", "Error", MB_OK + MB_ICONERROR);
              edFirstName->SetFocus();
              isNameValid = false;
        }
        if(isNameValid && !Info.ValidateLastName(lastNameMessage))
        {
           MessageBox(lastNameMessage + ".", "Error", MB_OK + MB_ICONERROR);
           edFirstName->SetFocus();
           isNameValid = false;
        }
        if(isNameValid)
          isEmailValid = ValidateEmailId();

        if(isEmailValid)
          isMobileValid = CheckMobileNumber(edMobile->Text);

        Info.Phone = edPhone->Text;
        if(Info.Phone != "" && Info.Phone != NULL && Info.Phone.Length() < 5 )
           {
              MessageBox("Phone number should be greater than 4 digits.", "Invalid Moble Number", MB_ICONERROR);
              isPhoneValid = false;
           }

      if(isNameValid && isEmailValid && isMobileValid && isPhoneValid)
      {
          ProcessData();
      }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::btnCancelClick(TObject *Sender)
{
   ModalResult = mrCancel;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tbtnAllowedTabClick(TObject *Sender)
{
   if (tbtnAllowedTab->Latched == false && Editing)
   {
	  MessageBox("If this member already has a Tab.\r It will be removed next time it is billed off.", "Error", MB_OK + MB_ICONERROR);
	  tbtnAllowedTab->Caption = "Not Allowed Tab";
   }
   else if (tbtnAllowedTab->Latched == true && Editing)
   {
	  tbtnAllowedTab->Caption = "Allowed Tab";
   }
   else
   {
     if(tbtnAllowedTab->Latched)
     {
       tbtnAllowedTab->Caption = "Allowed Tab";
     }
     else
     {
       tbtnAllowedTab->Caption = "Not Allowed Tab";
     }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::UpdateSmartCardSpace()
{
   std::auto_ptr <TMemoryStream> Stream(new TMemoryStream);
   Info.SaveToStream(Stream.get());
   unsigned short CalculatedCRC = 0xFF;

   lbeSmartCardSpace->Caption = "Smart Card Space Used " + IntToStr((int)Stream->Size) + " of " + IntToStr
	   ((int)(SMART_CARD_CURRENT_VERSION_CARD_MEMBER_DATA_LENGTH -sizeof(CalculatedCRC)));
   if ((Stream->Size +sizeof(CalculatedCRC)) > (SMART_CARD_CURRENT_VERSION_CARD_MEMBER_DATA_LENGTH))
   {
	  lbeSmartCardSpace->Font->Color = clRed;
   }
   else if ((Stream->Size +sizeof(CalculatedCRC)) > ((SMART_CARD_CURRENT_VERSION_CARD_MEMBER_DATA_LENGTH) / 2))
   {
	  lbeSmartCardSpace->Font->Color = clOlive;
   }
   else
   {
	  lbeSmartCardSpace->Font->Color = clGreen;
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tbtnUserProfileMouseClick(TObject *Sender)
{
   pgControl->ActivePage = tsUser;
   RedrawButtons(Sender);
}
// ---------------------------------------------------------------------------
void TfrmEditCustomer::RedrawButtons(TObject * Sender)
{
   TTouchBtn* CurrentButton = dynamic_cast <TTouchBtn*> (Sender);
   if (CurrentButton)
   {
	  CurrentButton->Color = clNavy;

	  for (int i = 0; i < pnlButtons->ControlCount; i++)
	  {
		 TTouchBtn* Button = dynamic_cast <TTouchBtn*> (pnlButtons->Controls[i]);
		 if (Button)
		 {
			if (Button->Tag != -1 && Button != CurrentButton)
			{
			   Button->Color = clMaroon;
			}
		 }
	  }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tbtnPalmProfileMouseClick(TObject *Sender)
{
   if((TPaySubsUtility::IsLocalLoyalty()) && !Info.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
   {
      MessageBox("Discounts cannot be configured for the Member. Please allow Discounts for the Member first.", "Information", MB_OK + MB_ICONINFORMATION);
      return;
   }
   pgControl->ActivePage = tsDiscounts;
   RedrawButtons(Sender);
   RedrawDiscounts();
}
// ---------------------------------------------------------------------------
void TfrmEditCustomer::RedrawDiscounts()
{
   tgDiscounts->ColCount = 0;
   tgDiscounts->RowCount = 0;

   tgDiscounts->RowCount = DiscountList->Count/3;
   tgDiscounts->RowCount += 1;
   tgDiscounts->ColCount = 3;
   for (int i = 0; i < tgDiscounts->RowCount; i++)
   {
	  for (int j = 0; j < tgDiscounts->ColCount; j++)
	  {
		 int Index = ((i * 3) + j);
		 if (DiscountList->Count > Index)
		 {
			tgDiscounts->Buttons[i][j]->Visible = true;
			tgDiscounts->Buttons[i][j]->Caption = DiscountList->Strings[Index];
			tgDiscounts->Buttons[i][j]->Color = clNavy;
			tgDiscounts->Buttons[i][j]->DisabledColor = clBtnFace;
			tgDiscounts->Buttons[i][j]->FontColor = clWhite;
			tgDiscounts->Buttons[i][j]->Tag = (int)DiscountList->Objects[Index];
		 }
		 else
		 {
			tgDiscounts->Buttons[i][j]->Caption = "";
			tgDiscounts->Buttons[i][j]->Visible = false;
		 }
	  }
   }

   for (int i = 0; i < tgDiscounts->RowCount; i++)
   {
	  for (int j = 0; j < tgDiscounts->ColCount; j++)
	  {
		 if (std::count(Info.AutoAppliedDiscounts.begin(), Info.AutoAppliedDiscounts.end(), tgDiscounts->Buttons[i][j]->Tag))
		 {
			tgDiscounts->Buttons[i][j]->Latched = true;
		 }
		 else
		 {
			tgDiscounts->Buttons[i][j]->Latched = false;
		 }
	  }
   }

}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tgDiscountsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
   std::set <int> ::iterator ptrDiscount;
   ptrDiscount = Info.AutoAppliedDiscounts.find(GridButton->Tag);

    try
    {
        if (ptrDiscount == Info.AutoAppliedDiscounts.end())
        { // Not Found.
	        Info.AutoAppliedDiscounts.insert(GridButton->Tag);
	        GridButton->Latched = true;
        }
        else
        {
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		    DBTransaction.StartTransaction();
            AnsiString discountCode = ManagerDiscount->GetDiscountCodeByKey(DBTransaction,*ptrDiscount);
            DBTransaction.Commit();

            if(discountCode != "")
                Info.AutoAppliedDiscountsID.erase(discountCode);

	        Info.AutoAppliedDiscounts.erase(ptrDiscount);
	        GridButton->Latched = false;
        }
    }
    catch(Exception &err)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,err.Message);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tcbeprNoPointsRedemptionClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().UseMemberSubs && !Info.Points.PointsRulesSubs.Contains(eprFinancial))
    {
       tcbeprNoPointsRedemption->Latched = Info.Points.PointsRules.Contains(eprNoPointsRedemption);
    }
    else
    {
       if (tcbeprNoPointsRedemption->Latched)
          Info.Points.PointsRules << eprNoPointsRedemption;
       else
          Info.Points.PointsRules >> eprNoPointsRedemption;
    }
}
// ---------------------------------------------------------------------------
 void __fastcall TfrmEditCustomer::tcbeprNoPointsPurchasesClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().UseMemberSubs && !Info.Points.PointsRulesSubs.Contains(eprFinancial))
    {
       tcbeprNoPointsPurchases->Latched = Info.Points.PointsRules.Contains(eprNoPointsPurchases);
    }
    else
    {
       if (tcbeprNoPointsPurchases->Latched)
          Info.Points.PointsRules << eprNoPointsPurchases;
       else
          Info.Points.PointsRules >> eprNoPointsPurchases;
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tcbeprEarnsPointsWhileRedeemingPointsClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().UseMemberSubs && !Info.Points.PointsRulesSubs.Contains(eprFinancial))
    {
       tcbeprEarnsPointsWhileRedeemingPoints->Latched = Info.Points.PointsRules.Contains(eprEarnsPointsWhileRedeemingPoints);
    }
    else
    {
       if (tcbeprEarnsPointsWhileRedeemingPoints->Latched)
          Info.Points.PointsRules << eprEarnsPointsWhileRedeemingPoints;
       else
          Info.Points.PointsRules >> eprEarnsPointsWhileRedeemingPoints;
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tcbeprOnlyEarnsPointsWhileRedeemingPointsClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().UseMemberSubs && !Info.Points.PointsRulesSubs.Contains(eprFinancial))
    {
       tcbeprOnlyEarnsPointsWhileRedeemingPoints->Latched = Info.Points.PointsRules.Contains(eprOnlyEarnsPointsWhileRedeemingPoints);
    }
    else
    {
       if (tcbeprOnlyEarnsPointsWhileRedeemingPoints->Latched)
          Info.Points.PointsRules << eprOnlyEarnsPointsWhileRedeemingPoints;
       else
          Info.Points.PointsRules >> eprOnlyEarnsPointsWhileRedeemingPoints;
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tcbeprNeverEarnsPointsClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().UseMemberSubs && !Info.Points.PointsRulesSubs.Contains(eprFinancial))
    {
       tcbeprNeverEarnsPoints->Latched = Info.Points.PointsRules.Contains(eprNeverEarnsPoints);
    }
    else
    {
       if (tcbeprNeverEarnsPoints->Latched)
          Info.Points.PointsRules << eprNeverEarnsPoints;
       else
          Info.Points.PointsRules >> eprNeverEarnsPoints;
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tcbeprAllowedNegitiveClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().UseMemberSubs && !Info.Points.PointsRulesSubs.Contains(eprFinancial))
    {
       tcbeprAllowedNegitive->Latched = Info.Points.PointsRules.Contains(eprAllowedNegitive);
    }
    else
    {
       if (tcbeprAllowedNegitive->Latched)
          Info.Points.PointsRules << eprAllowedNegitive;
       else
          Info.Points.PointsRules >> eprAllowedNegitive;
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tbtnPointsMouseClick(TObject *Sender)
{
   pgControl->ActivePage = tsPoints;
   RedrawButtons(Sender);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::btnGroupsMouseClick(TObject *Sender)
{
	pgControl->ActivePage = tsGroups;
	RedrawButtons(Sender);
	GroupInfo = Info.availableGroups;
	DisplayPage();

}
//---------------------------------------------------------------------------
void TfrmEditCustomer::DisplayPage(void)
{
    DisplayCurrentGroups();
	DisplayAvailableGroups();

}
//----------------------------------------------------------------------
void TfrmEditCustomer::DisplayAvailableGroups(void)
{
	AvailableGroupsGrid->RowCount = Info.availableGroups.size();

		if(Info.Charges == 1 && !Info.currentGroups.empty())
		{
			for (int i = 0; i < Info.availableGroups.size() ; i++)
			{
				AvailableGroupsGrid->Buttons[i][0]->Caption = Info.availableGroups.at(i).Name;
				AvailableGroupsGrid->Buttons[i][1]->Caption = Info.availableGroups.at(i).Type;
				AvailableGroupsGrid->Buttons[i][0]->Tag = Info.availableGroups.at(i).Key;
				AvailableGroupsGrid->Buttons[i][0]->Enabled = false;
				AvailableGroupsGrid->Buttons[i][1]->Enabled = false;
			}
		  return;
		}

		for (int i = 0; i < Info.availableGroups.size() ; i++)
		{
			AvailableGroupsGrid->Buttons[i][0]->Enabled = true;
			AvailableGroupsGrid->Buttons[i][0]->Caption = Info.availableGroups.at(i).Name;
			AvailableGroupsGrid->Buttons[i][1]->Caption = Info.availableGroups.at(i).Type;
			AvailableGroupsGrid->Buttons[i][0]->Tag = Info.availableGroups.at(i).Key;
			AvailableGroupsGrid->Buttons[i][1]->Enabled = false;

				for(int j = 0; j < Info.currentGroups.size(); j++)
				{
					if((AvailableGroupsGrid->Buttons[i][1]->Caption == Info.currentGroups.at(j).Type && AvailableGroupsGrid->Buttons[i][1]->Caption != "0"))
						{
							AvailableGroupsGrid->Buttons[i][0]->Enabled = false;
						}
						if((AvailableGroupsGrid->Buttons[i][0]->Caption == Info.currentGroups.at(j).Name))
						{
							AvailableGroupsGrid->Buttons[i][0]->Enabled = false;
						}
				}

	}
}
//---------------------------------------------------------------------------
void TfrmEditCustomer::DisplayCurrentGroups(void)
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TDBGroups DBGroups;
	bool SummaGroup;

	Info.summagroup.Name == "" ? SummaGroup = false : SummaGroup = true;
	CurrentGroupsGrid->RowCount = Info.currentGroups.size();

       if(Info.Charges == 1)
		{
			if(Info.currentGroups.size() > 1)
			{
			Info.currentGroups.erase(Info.currentGroups.begin()+1, Info.currentGroups.end());
			CurrentGroupsGrid->RowCount = 1;
			}
		}

	for (int i = 0; i < Info.currentGroups.size(); i++)
	{
			CurrentGroupsGrid->Buttons[i][0]->Caption = Info.currentGroups.at(i).Name;
				CurrentGroupsGrid->Buttons[i][1]->Caption = Info.currentGroups.at(i).Type;
				CurrentGroupsGrid->Buttons[i][0]->Tag = Info.currentGroups.at(i).Key;
				CurrentGroupsGrid->Buttons[i][1]->Tag = Info.currentGroups.at(i).Key;

				if(Info.currentGroups.at(i).Key == Info.summagroup.Key)
				{
						CurrentGroupsGrid->Buttons[i][1]->Color = clBlue;
                }
				else
				{
						CurrentGroupsGrid->Buttons[i][1]->Color = clWhite;
				}

	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::AvailableGroupsGridMouseClick(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, TGridButton *GridButton)
{
		CurrentGroupsGrid->ColCount = 2;
		Info.currentGroups.push_back(FindGroupData(GridButton->Caption));

		if(Info.summagroup.Name == "")
		{
			if(MessageBox("Would you like to set " + GridButton->Caption + " as the default group", "", MB_YESNO) == ID_YES)
					Info.summagroup = FindGroupData(GridButton->Caption);
		}
		GridButton->Enabled = false;
		DisplayPage();
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::CurrentGroupsGridMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton)
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TDBGroups DBGroups;

		if(CurrentGroupsGrid->Col(GridButton))
		{
			if(GridButton->Tag == Info.summagroup.Key)
				{
					Info.summagroup.Name = "";
					Info.summagroup.Key = NULL;
				}
				else
				{
					if(MessageBox("Would you like " + CurrentGroupsGrid->Buttons[CurrentGroupsGrid->Row(GridButton)][0]->Caption + " to be the new Summa Group?", "", MB_YESNO) == ID_YES)
					{
						Info.summagroup = FindGroupData(GridButton->Tag);
					}
				}
		}
		else
		{
		ContactGroup groupkey = FindGroupData(GridButton->Tag);
			for(int i = 0; i < Info.currentGroups.size(); i++)
			{
				if(groupkey.Key == Info.currentGroups.at(i).Key)
					Info.currentGroups.erase(Info.currentGroups.begin() + i);

				if(groupkey.Key == Info.summagroup.Key)
				{
					Info.summagroup.Name = "";
					Info.summagroup.Key = NULL;
				}
			}
		}

	DisplayPage();
}
//---------------------------------------------------------------------------
void TfrmEditCustomer::UpdateDataBase(void)
{

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TDBGroups DBGroups;

	DBGroups.GetGroupsList(DBTransaction, Info.availableGroups);
	DBTransaction.Commit();

}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::btnAccountProfileMouseClick(TObject *Sender)
{
	pgControl->ActivePage = tsProfile;
	RedrawButtons(Sender);
	CheckBoxCharges->Checked = Info.Charges;
	DisplayPage();
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::CheckBoxChargesMouseClick(TObject *Sender)
{
		Info.Charges = CheckBoxCharges->Checked;
}
//--------------------------------------------------------------------------
ContactGroup TfrmEditCustomer::FindGroupData(UnicodeString groupname)
{
	for(int i = 0; GroupInfo.size(); i++)
	{
		if(groupname == GroupInfo.at(i).Name)
			return GroupInfo.at(i);
	}

}
//--------------------------------------------------------------------------
ContactGroup TfrmEditCustomer::FindGroupData(int groupkey)
{
	for(int i = 0; GroupInfo.size(); i++)
	{
		if(groupkey == GroupInfo.at(i).Key)
			return GroupInfo.at(i);
	}

}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tbProximityMouseClick(TObject *Sender)
{
	TIBSQL *check_dup_prox_query;
	std::auto_ptr<TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create<TfrmCardSwipe>(this));
	Database::TDBTransaction trans(TDeviceRealTerminal::Instance().DBControl);
	AnsiString new_prox_str;
	TModalResult result;
   bool duplicate_prox_detected = false;

	frmCardSwipe->ShowModal();
	if ((result = frmCardSwipe->ModalResult) != mrOk  && result != mrAbort)
		return;

	new_prox_str = frmCardSwipe->SwipeString.SubString(1, 80);

	TDeviceRealTerminal::Instance().RegisterTransaction(trans);
	check_dup_prox_query = trans.Query(trans.AddQuery());

	check_dup_prox_query->Close();
	check_dup_prox_query->SQL->Add(
	  "select name from contacts where prox_card = :new_prox_card and contacts_key <> :user_key;");
	trans.StartTransaction();
	check_dup_prox_query->ParamByName("new_prox_card")->AsString = new_prox_str;
	check_dup_prox_query->ParamByName("user_key")->AsInteger = Info.ContactKey;
	check_dup_prox_query->ExecQuery();

	if (!(duplicate_prox_detected = check_dup_prox_query->RecordCount > 0))
    {
		Info.ProxStr = new_prox_str;
        IsProxCardChange = true;
    }
	else
    {
        if(MessageBox("This proximity card is current held by "
		          + check_dup_prox_query->FieldByName("name")->AsString
		          + ". Would you like to transfer it to this user?",
		          "Duplicate proximity card detected", MB_YESNO) == IDYES)
        {
            Info.ProxStr = new_prox_str;
            IsProxCardChange = true;
        }
        else
        {
			MessageBox("The proximity card has not been assigned to this user.",
			           "Duplicate proximity card detected", MB_OK);
			duplicate_prox_detected = false;
        }
   }

	trans.Commit();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::btnRemoveCardClick(TObject *Sender)
{
   std::auto_ptr <TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create <TfrmCardSwipe> (this));
   frmCardSwipe->tbOtherOpt->Visible = false;
   frmCardSwipe->ShowModal();
   if (frmCardSwipe->ModalResult == mrOk)
   {
	  Info.CardsToAdd.erase(AnsiString(frmCardSwipe->SwipeString).SubString(1, 80));
	  Info.CardsToRemove.insert(AnsiString(frmCardSwipe->SwipeString).SubString(1, 80));
	  Info.CardStr = "";
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tbtnClearAllCardsClick(TObject *Sender)
{
   if (MessageBox("This will Clear all swipe cards associate with this Member.", "Warning", MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
   {
	  ClearAllCards = true;
   }
}
// ---------------------------------------------------------------------------
void TfrmEditCustomer::SetupCustomerInfoPointers()
{
    CustomerInfoPointers[0] = &Info.Name;
    CustomerInfoPointers[1] = &Info.Phone;
    CustomerInfoPointers[2] = &Info.EMail;
    CustomerInfoPointers[3] = &Info.Mobile;
    CustomerInfoPointers[4] = &Info.LocationAddress;
    CustomerInfoPointers[5] = &Info.MailingAddress;
    CustomerInfoPointers[6] = &Info.MembershipNumber;
    CustomerInfoPointers[7] = &Info.Alias;
    CustomerInfoPointers[8] = &Info.Sex;
    CustomerInfoPointers[9] = &Info.Note;
    CustomerInfoPointers[10] = &Info.Surname;
}
//---------------------------------------------------------------------------
void TfrmEditCustomer::DisplayCustomerDataFromPointers()
{
    //if(!SeparateName(*CustomerInfoPointers[0]))
    {
      edFirstName->Text = *CustomerInfoPointers[0];
    }
    edPhone->Text = *CustomerInfoPointers[1];
    edEmail->Text = *CustomerInfoPointers[2];
    edMobile->Text = *CustomerInfoPointers[3];
    reLocationAddress->Text = RemoveLineBreaks(*CustomerInfoPointers[4]);
    reAddress->Text = RemoveLineBreaks(*CustomerInfoPointers[5]);
    edMemberNumber->Text = *CustomerInfoPointers[6];
    edKnownas->Text =  *CustomerInfoPointers[7];
    edSex->Text = *CustomerInfoPointers[8];
    edNote->Text = *CustomerInfoPointers[9];
    edLastName->Text = *CustomerInfoPointers[10];
    DateTimePicker1->DateTime = Info.DateOfBirth;
}
//---------------------------------------------------------------------------
UnicodeString TfrmEditCustomer::RemoveLineBreaks(UnicodeString line)
{
    UnicodeString retVal = StringReplace(line,UnicodeString("\r"),"", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
    retVal = StringReplace(retVal,UnicodeString("\n"),"", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
    return  retVal;
}
//----------------------------------------------------------------------------
bool TfrmEditCustomer::SeparateName(UnicodeString name)
{
    bool retVal = false;
    AnsiString strname = name;
	char* temp_char = new char[strname.Length()+1];

	strcpy(temp_char, strname.c_str());

    for(int i = 0; i < strname.Length()+1; i++)
    {
        if(temp_char[i] == check_space)
        {
          edFirstName->Text = strname.SubString(0, i);
          edLastName->Text =  strname.SubString(i+2, strname.Length()+1);
          retVal = true;
        }
    }
    return retVal;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::TouchKeyboard1Click(TObject *Sender, char Char)
{
  if(Char == '\t')
  {
    ChangeTextBoxFocus();
  }
  else if(Char == '\r')
  {
    ValidateTextBox();
  }
  TextBoxValue(Char);
  if(Char != '\t')
  {
    RichTextBoxValue(Char);
  }
}
// ---------------------------------------------------------------------------
void TfrmEditCustomer::ChangeTextBoxFocus()
{
   if(edFirstName->Focused())
   {
     edLastName->SetFocus();
   }
   else if(edLastName->Focused())
   {
     edPhone->SetFocus();
   }
   else if(edPhone->Focused())
   {
     edEmail->SetFocus();
   }
   else if(edEmail->Focused())
   {
     edMobile->SetFocus();
   }
   else if(edMobile->Focused())
   {
     reLocationAddress->SetFocus();
   }
   else if(reLocationAddress->Focused())
   {
     reAddress->SetFocus();
   }
   else if(reAddress->Focused())
   {
     edMemberNumber->SetFocus();
   }
   else if(edMemberNumber->Focused())
   {
     edKnownas->SetFocus();
   }
   else if(edKnownas->Focused())
   {
     edSex->SetFocus();
   }
   else if(edSex->Focused())
   {
     edNote->SetFocus();
   }
   else if(edNote->Focused())
   {
      DateTimePicker1->SetFocus();
   }
   else
   {
      edFirstName->SetFocus();
   }
}
// ---------------------------------------------------------------------------
void TfrmEditCustomer::PopulateTextBox(char valueChar, TEdit *edTextbox)
{

    PostMessage(edTextbox->Handle, WM_CHAR, valueChar, 0);
    edTextbox->SetFocus();
}
// ---------------------------------------------------------------------------
void TfrmEditCustomer::PopulateRichTextBox(char valueChar, TRichEdit *edRichTextbox)
{
    if(valueChar == '\b')
    {
       int selStart =  edRichTextbox->SelStart;
       AnsiString text = edRichTextbox->Text;
       if(selStart > 0)
       {
          int textLenth = text.Length();
          AnsiString prefix =  text.SubString(0,selStart-1);
          AnsiString postFix = text.SubString(selStart+1, textLenth - selStart);
          edRichTextbox->Text = prefix + postFix;
          edRichTextbox->SetFocus();
          edRichTextbox->SelStart = selStart -1;
       }

    }
    else
    {
         PostMessage(edRichTextbox->Handle, WM_CHAR, valueChar, 0);
         edRichTextbox->SetFocus();
    }


}
// ---------------------------------------------------------------------------
void TfrmEditCustomer::RichTextBoxValue(char valueChar)
{
  if(reLocationAddress->Focused())
  {
    PopulateRichTextBox(valueChar, reLocationAddress);
  }
  else if(reAddress->Focused())
  {
     PopulateRichTextBox(valueChar, reAddress);
  }
  UpdateSmartCardSpace();
}
// ---------------------------------------------------------------------------
void TfrmEditCustomer::TextBoxValue(char valueChar)
{
  if(edFirstName->Focused())
  {
    PopulateTextBox(valueChar, edFirstName);
  }
  else if(edLastName->Focused())
  {
    PopulateTextBox(valueChar, edLastName);
  }
  else if(edPhone->Focused())
  {
    PopulateTextBox(valueChar, edPhone);
  }
  else if(edMobile->Focused())
  {
    PopulateTextBox(valueChar, edMobile);
  }
  else if(edEmail->Focused())
  {
    PopulateTextBox(valueChar, edEmail);
  }
  else if(edMemberNumber->Focused())
  {
    PopulateTextBox(valueChar, edMemberNumber);
  }
  else if(edKnownas->Focused())
  {
    PopulateTextBox(valueChar, edKnownas);
  }
  else if(edSex->Focused())
  {
    PopulateTextBox(valueChar, edSex);
  }
  else if(edNote->Focused())
  {
    PopulateTextBox(valueChar, edNote);
  }
  else if(DateTimePicker1->Focused())
  {
      PopulateDateTimePicker(valueChar, DateTimePicker1);
  }
  UpdateSmartCardSpace();
}
// ---------------------------------------------------------------------------
void TfrmEditCustomer::ValidateTextBox()
{
    if(edFirstName->Text == "")
    {
       edFirstName->SetFocus();
    }
    else if(edLastName->Text == "")
    {
       edLastName->SetFocus();
    }
    else if(edPhone->Text == "")
    {
       edPhone->SetFocus();
    }
    else if(edEmail->Text == "")
    {
       edEmail->SetFocus();
    }
    else if(edMobile->Text == "")
    {
       edMobile->SetFocus();
    }
    else if(reLocationAddress->Text == "")
    {
       reLocationAddress->SetFocus();
    }
    else if(reAddress->Text == "")
    {
       reAddress->SetFocus();
    }
    else if(edMemberNumber->Text == "")
    {
       edMemberNumber->SetFocus();
    }
    else if(edKnownas->Text == "")
    {
       edKnownas->SetFocus();
    }
    else if(edSex->Text == "")
    {
       edSex->SetFocus();
    }
    else if(edNote->Text == "")
    {
       edNote->SetFocus();
    }
    else if(edNote->Focused())
    {
       DateTimePicker1->SetFocus();
    }
    else if(DateTimePicker1->Focused())
    {
       btnOkClick(btnClose);
    }
    else
    {
       btnOkClick(btnClose);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if(Key == VK_RETURN)
  {
     ValidateTextBox();
  }
  else if(Key == VK_TAB)
  {
     ChangeTextBoxFocus();
  }
}
// ---------------------------------------------------------------------------
void TfrmEditCustomer::SaveData()
{
    Info.Name = edFirstName->Text;
    Info.Surname = edLastName->Text;
    Info.Phone = edPhone->Text;
    Info.EMail = edEmail->Text;
    Info.Mobile = edMobile->Text;
    Info.LocationAddress = reLocationAddress->Text;
    Info.MailingAddress = reAddress->Text;
    Info.MembershipNumber = edMemberNumber->Text;
    Info.Alias = edKnownas->Text;
    Info.Sex = edSex->Text;
    Info.Note = edNote->Text;
    if(Editing)
      Info.MemberType = MemberType;
}
// ---------------------------------------------------------------------------
bool TfrmEditCustomer::ValidateEmailId()
{
  bool isEmailValid = true;
  if (TGlobalSettings::Instance().LoyaltyMateEnabled &&
        Info.CloudUUID != TLoyaltyMateUtilities::GetLoyaltyMateDisabledCloudUUID()  &&
       !Info.ValidEmail())
  {
     MessageBox("You must enter a valid Email.", "Error", MB_OK + MB_ICONERROR);
     isEmailValid = false;
  }
  else if(!TGlobalSettings::Instance().LoyaltyMateEnabled && Info.EMail.Trim() == "")
  {
    isEmailValid = true;
  }
  else
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        if(Info.ContactKey > 0)
        {
          if (TGlobalSettings::Instance().LoyaltyMateEnabled )
          {
            IBInternalQuery->SQL->Text = "SELECT count(EMAIL) FROM CONTACTS where EMAIL =:EMAIL AND CONTACTS_KEY <> :CONTACTS_KEY";          }
          else
          {
            IBInternalQuery->SQL->Text = "SELECT count(EMAIL) FROM CONTACTS where EMAIL =:EMAIL AND CONTACTS_KEY <> :CONTACTS_KEY AND CONTACT_TYPE <> :CONTACT_TYPE";            IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = eDeletedMember;
          }

          IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Info.ContactKey;
        }
        else
        {
            if (TGlobalSettings::Instance().LoyaltyMateEnabled )
            {
              IBInternalQuery->SQL->Text = "SELECT count(EMAIL) FROM CONTACTS where EMAIL =:EMAIL ";
            }
            else
            {
                IBInternalQuery->SQL->Text = "SELECT count(EMAIL) FROM CONTACTS where EMAIL =:EMAIL AND CONTACT_TYPE <> :CONTACT_TYPE ";                IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = eDeletedMember;
            }
        }

        IBInternalQuery->ParamByName("EMAIL")->AsString = Info.EMail;

        IBInternalQuery->ExecQuery();
        DBTransaction.Commit();
        int	emailcount = IBInternalQuery->Fields[0]->AsInteger;
        if(emailcount > 0)
         {
            MessageBox("Member with same email already exists!!!", "Error", MB_OK + MB_ICONERROR);
            isEmailValid = false;
         }
    }
   if(!isEmailValid)
     edEmail->SetFocus();
   return isEmailValid;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::cbCopyAddressClick(TObject *Sender)
{
  if(cbCopyAddress->Checked)
  {
    reAddress->Text = reLocationAddress->Text;
  }
  else
  {
     reAddress->Text = "";
  }
}
// ---------------------------------------------------------------------------
bool TfrmEditCustomer::ValidateDate(TDateTime datetime)
{
    bool retVal = true;
    if(datetime > Now())
    {
      if(MessageBox("Select Date is not valid.", "Invalid Date", MB_RETRYCANCEL) == IDRETRY)
      {
        DateTimePicker1->SetFocus();
      }
       retVal = false;
    }
   return retVal;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::DateTimePicker1OnChange(TObject *Sender)
{
   Info.DateOfBirth = DateTimePicker1->DateTime;
}
// ---------------------------------------------------------------------------
void TfrmEditCustomer::ProcessData()
{
  if (Info.SiteID == 0)
  {
        Info.SiteID = TGlobalSettings::Instance().SiteID;
  }
  SaveData();
  try
  {
     bool CheckDateOfBirth = true;

     if(!ValidateDate(Info.DateOfBirth))
     {
        CheckDateOfBirth = false;
     }

     if(CheckDateOfBirth)
     {
         Info.LastModified = Now();
         if (tbtnAllowedTab->Latched == true)
         {
            Info.TabEnabled = true;
         }
         else if (tbtnAllowedTab->Latched == false)
         {
            Info.TabEnabled = false;
         }
         ModalResult = mrOk;
     }
  }
  catch(Exception & Err)
  {
     TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
     MessageBox("Unable to Save Contact Information\r" + Err.Message, "Error", MB_OK + MB_ICONERROR);
  }
}
//----------------------------------------------------------------------------------------------------
void TfrmEditCustomer::PopulateDateTimePicker(char valueChar, TDateTimePicker *dateTime)
{
   PostMessage(dateTime->Handle, WM_CHAR, valueChar, 0);
   dateTime->SetFocus();
}
//-----------------------------------------------------------------------
bool TfrmEditCustomer::CheckMobileNumber(UnicodeString mobile)
{
   if(mobile.Trim() == "")
     return true;

   if(mobile.Length() < 5)
   {
      MessageBox("Mobile number should be greater than 4 digits.", "Invalid Moble Number", MB_ICONERROR);
      return false;
   }

   bool retVal = true;
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction.StartTransaction();
   try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        if(Info.ContactKey == 0)
        {
           IBInternalQuery->SQL->Text = " select CONTACTS.MOBILE  from  CONTACTS where CONTACTS.MOBILE = :MOBILE and  CONTACT_TYPE !=3  ";
        }
        else
        {
           IBInternalQuery->SQL->Text = " select CONTACTS.MOBILE from  CONTACTS where CONTACTS.MOBILE = :MOBILE and  CONTACT_TYPE !=3  and CONTACTS.CONTACTS_KEY != :CONTACTS_KEY ";
           IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Info.ContactKey;
        }
        IBInternalQuery->ParamByName("MOBILE")->AsString = mobile;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
            retVal = false;
        }
        if (!retVal)
        {
            MessageBox("This mobile number already exits in our record.Please provide different mobile number for your registration.", "Different mobile number for your registration", MB_ICONERROR);
        }
       DBTransaction.Commit();
       return retVal;
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
      DBTransaction.Rollback();
	  return false;
   }
}
//-----------------------------------------------------------------------
void __fastcall TfrmEditCustomer::cbNoEmailMouseClick(TObject *Sender)
{
   edEmail->Enabled = !cbNoEmail->Checked;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tcbeprFinancialClick(TObject *Sender)
{
   if (Info.Points.PointsRulesSubs.Contains(eprFinancial))
      tcbeprFinancial->Latched = true;
   else
      tcbeprFinancial->Latched = false;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmEditCustomer::tcbeprAllowDiscountsClick(TObject *Sender)
{
   if(TGlobalSettings::Instance().UseMemberSubs)
   {
       if(Info.Points.PointsRulesSubs.Contains(eprFinancial))
       {
           if (tcbeprAllowDiscounts->Latched)
           {
              Info.Points.PointsRulesSubs << eprAllowDiscounts;
           }
           else
           {
              Info.Points.PointsRulesSubs >> eprAllowDiscounts;
              Info.AutoAppliedDiscounts.clear();
           }
       }
       else
       {
//          Info.Points.PointsRulesSubs >> eprAllowDiscounts;
//          Info.AutoAppliedDiscounts.clear();
//         tcbeprAllowDiscounts->Latched = false;
          tcbeprAllowDiscounts->Latched = Info.Points.PointsRulesSubs.Contains(eprAllowDiscounts);
       }
   }
   else
   {
       if (tcbeprAllowDiscounts->Latched)
       {
          Info.Points.PointsRulesSubs << eprAllowDiscounts;
       }
       else
       {
          Info.Points.PointsRulesSubs >> eprAllowDiscounts;
          Info.AutoAppliedDiscounts.clear();
       }
   }
}
//-----------------------------------------------------------------------------




