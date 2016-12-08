// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ManagerLoyaltyMate.h"
#include "ManagerMembershipGUI.h"
#include "MembershipGUI.h"
#include "MembershipEBetGUI.h"
#include "MembershipExternalGUI.h"
#include "MembershipThorlinkGUI.h"
#include "EditCustomer.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "DBTab.h"
#include "ManagerDiscount.h"
#include "GlobalSettings.h"
#include "DBContacts.h"
#include "DBGroups.h"
#include "MemberCreation.h"
#include "ContactStaff.h"
// ---------------------------------------------------------------------------
#include "MembershipERSGUI.h"
#include "MembershipGUI.h"
#include "LoyaltyMateUtilities.h"




#pragma package(smart_init)

TManagerMembershipGUI::TManagerMembershipGUI(Database::TDBControl &inDBControl,TMMSettings &inSystem, TModules &inModules)
: TManagerMembershipSmartCards(inDBControl,inModules)
{
   DefaultInterfaceDisplay = eMemKeypad;
   DefaultSearchType = eSearchByName;
}

bool TManagerMembershipGUI::GetMembershipDetailsFromGUI(Database::TDBTransaction &tr, TMMContactInfo &contact_info, const bool triggered_by_preloaded_card)
{
	if (!triggered_by_preloaded_card && TGlobalSettings::Instance().AllowMemberDetailscreen)
	{
       bool addGiftCard = false;
       if(TGlobalSettings::Instance().GiftCardOnly)
         {
           if (MessageBox("This will create a gift card, do you wish to continue?", "Membership", MB_YESNO | MB_ICONWARNING) == ID_YES)
              {
                addGiftCard = true;
              }
             if(!GiftCardSecurityCheck(tr)) return false;

         }

        if(addGiftCard)
        {
            return AddGiftCard(tr,contact_info);
        }
        else
        {
          std::auto_ptr<TfrmMemberCreation> add_member_screen(new TfrmMemberCreation(Screen->ActiveForm,contact_info));
          add_member_screen->Editing = false;
          return add_member_screen->ShowModal() == mrOk;
        }
	}
	else
	{
       bool addGiftCard = false;
       if(TGlobalSettings::Instance().GiftCardOnly)
         {
           if (MessageBox("This will create a gift card, do you wish to continue?", "Membership", MB_YESNO | MB_ICONWARNING) == ID_YES)
              {
                addGiftCard = true;
              }
              if(!GiftCardSecurityCheck(tr)) return false;

         }

        if(addGiftCard)
        {
            return AddGiftCard(tr,contact_info);
        }
        else
        {
            std::vector<eDiscountFilter> discountFilter;
            discountFilter.push_back(exclude_member_exempt);
            discountFilter.push_back(exclude_combos);
            std::auto_ptr<TfrmEditCustomer> add_member_screen(TfrmEditCustomer::Create(Screen->ActiveForm));
            add_member_screen->Editing = false;
            ManagerDiscount->GetDiscountList(tr, add_member_screen->DiscountList.get(), discountFilter);
            add_member_screen->Info = contact_info;
			AnsiString str = "";
			if(contact_info.ProxStr.Length() != 0)
			{
				 str = contact_info.ProxStr;
			}
            if (add_member_screen->ShowModal() == mrOk)
            {
                contact_info = add_member_screen->Info;
                contact_info.MemberType = 1;
				if(add_member_screen->Info.ProxStr.Length() == 0)
				{
					contact_info.ProxStr = str;
				}
            }
            return add_member_screen->ModalResult == mrOk;
        }
	}
}

bool TManagerMembershipGUI::GiftCardSecurityCheck(Database::TDBTransaction &tr)
{
    bool canAddGiftCard = false;
    TMMContactInfo TempUserInfo;
    std::auto_ptr <TContactStaff> Staff(new TContactStaff(tr));
    TLoginSuccess Result = Staff->Login(Screen->ActiveForm, tr, TempUserInfo, CheckAccountManager);
    if (Result == lsAccepted)
    {
       canAddGiftCard = true;
    }
    else if (Result == lsDenied)
    {
      MessageBox("You do not have access to Membership.", "Error", MB_OK + MB_ICONERROR);
    }
    else if (Result == lsPINIncorrect)
    {
      MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
    }
     return canAddGiftCard;
}

bool TManagerMembershipGUI::AddGiftCard(Database::TDBTransaction &tr,TMMContactInfo &Info)
{
    Info.SiteID = TGlobalSettings::Instance().SiteID ;
    Info.EMail = TLoyaltyMateUtilities::GetAutoGeneratedGiftCardEmail();
    Info.LastModified = Now();
    Info.Name="GIFTCARD";
    Info.Points.PointsRules << eprNeverEarnsPoints ;
    Info.MemberType=2;
    return true;
}

TModalResult TManagerMembershipGUI::AddMember(TMMContactInfo & Info,bool IsBarcodeCard,const bool triggered_by_preloaded_card)
{
   AnsiString cardCode = Info.MemberCode;
   MembershipSystem->ResetPoints();
   TModalResult Result = mrCancel;
   if (MembershipSystem->ReadOnlyInterface)
   {
      MessageBox("You must Add,Edit and Delete Members from your 3rd Party Membership software.", "Warning", MB_ICONWARNING + MB_OK);
   }
   else
   {
      CardIssueInProgress = true;
      try
      {
		 try
		 {
			Database::TDBTransaction DBTransaction(DBControl);
			RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
			TDBContacts::GetAvailableGroups(DBTransaction, Info);

		   if (GetMembershipDetailsFromGUI(DBTransaction, Info,
											 triggered_by_preloaded_card))
		   {
               Info.ActivationDate = Now();
               Info.MemberCode = cardCode;
			   ManagerDiscount->DiscountKeyToCode(DBTransaction, Info.AutoAppliedDiscountsID, Info.AutoAppliedDiscounts);
			   // Update Member & Assign Member Number.
			   MembershipSystem->SetContactDetails(DBTransaction, Info.ContactKey, Info);
			   // Ensure the member is written into the DB prior to updating the
			   // smart card which may fire events that requires a DM member lookup.
			   // i.e updating the card restore point.
               bool IsSmartCardEnabled =TManagerVariable::Instance().GetBool(DBTransaction,vmSmartCardMembership,false);
			   DBTransaction.Commit();

			   if (TGlobalSettings::Instance().LoyaltyMateEnabled && Info.CloudUUID == "" && (IsSmartCardEnabled || IsBarcodeCard) )
			   {
                   // calling the protected method from MembershipManagerSmartCards
                    TSyndCode syndicateCode =  GetSyndicateCodeManager().GetCommunicationSyndCode();
                    bool memberCreationSuccess = TManagerMembershipSmartCards::createMemberOnLoyaltyMate(syndicateCode, Info);
			   }

               if(!IsBarcodeCard)
                {
                   SaveContactInfoAddedToSmartCard(Info, true);
                   /*if(TGlobalSettings::Instance().LoyaltyMateEnabled && Info.Points.getPointsBalance() != 0)
                   {
                       SavePointsTransactionsToSmartCard(Info.Points,"",true);
                   }*/
                }

               DBTransaction.StartTransaction();
               if(!IsBarcodeCard)
                {
                   TMMContactInfo creationDateInfo;
                   ManagerSmartCards->GetContactInfo(creationDateInfo);
                   TDBContacts::SetCardCreationDate(DBTransaction, Info.ContactKey, creationDateInfo.CardCreationDate);
                   TManagerMembershipSmartCards::UpdateMemberCardCodeToDB(DBTransaction,Info,Info.CloudUUID);
                }
               else
                {
                   Info.CardCreationDate = Now();
                   TDBContacts::SetCardCreationDate(DBTransaction, Info.ContactKey, Now());
                }

			   for(int i = 0; i < Info.currentGroups.size(); i++)
			   {
					TDBContacts::SetCurrentGroups(DBTransaction, Info.ContactKey, Info.currentGroups.at(i));
			   }

			  TDBContacts::SetSummaGroup(DBTransaction, Info.ContactKey, Info);

			  if(!Info.currentGroups.size())
			  {
				ContactGroup GroupKey;
				GroupKey.Key = TDBGroups::FindGroup(DBTransaction, "No Contacts Group");
				TDBContacts::SetCurrentGroups(DBTransaction, Info.ContactKey, GroupKey);
			  }


			   // Update Tab.
			   if (Info.ContactKey != 0 && Info.TabEnabled)
			   {
                  int TabKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
                  TDBTab::SetTabOwner(DBTransaction, TabKey, Info.ContactKey, TabMember);
                  TDBTab::SetTabName(DBTransaction, TabKey, Info.Name);
                  TDBTab::SetTabCard(DBTransaction, TabKey, Info.CardStr);
                  TDBTab::SetTabPermanent(DBTransaction, TabKey, true);
			   }
			}
			DBTransaction.Commit();
		 }
		 catch(Exception & E)
		 {
			MessageBox(E.Message, "Failed to Add Member.", MB_OK + MB_ICONERROR);
		 }
      }
      __finally
      {
	 CardIssueInProgress = false;
      }
   }
   return Result;
}

TManagerMembershipGUI::EditMember(Database::TDBTransaction & DBTransaction,TMMContactInfo & Info)
{
    TModalResult Result = mrCancel;

    if (MembershipSystem->ReadOnlyInterface)
    {
        MessageBox
          ("You must Add,Edit and Delete Members from your 3rd Party Membership software.",
           "Warning", MB_ICONWARNING + MB_OK);
    }
    else
     {
        CardIssueInProgress = true;
        try {
              try
               {
                    if (ManagerSmartCards->CardBlank)
                    {
                        Database::TDBTransaction DBTransaction(DBControl);
                        RegisterTransaction(DBTransaction);
                        TMMContactInfo Info;
                        AddMember(Info);
                    }
                    else
                    {
					    MembershipSystem->GetContactDetails(DBTransaction, Info.ContactKey, Info);
                        TDBContacts::GetAvailableGroups(DBTransaction, Info);
                        TDBContacts::GetSummaGroup(DBTransaction, Info.ContactKey, Info);

                        BeginMemberTransaction();
                        if (TDBTab::GetTabByOwner(DBTransaction, Info.ContactKey) != 0)
                        {
                            Info.TabEnabled = true;
                        }

                       std::vector<eDiscountFilter> discountFilter;
                       discountFilter.push_back(exclude_member_exempt);
                       discountFilter.push_back(exclude_combos);
                       std::auto_ptr < TfrmEditCustomer >
					   frmEditCustomer(TfrmEditCustomer::Create(Screen->ActiveForm));
					   ManagerDiscount->GetDiscountList( DBTransaction, frmEditCustomer->DiscountList.get(), discountFilter);
                       frmEditCustomer->Editing = true;
                       frmEditCustomer->Info = Info;
                       frmEditCustomer->MemberType = Info.MemberType;
                       TModalResult Result = frmEditCustomer->ShowModal();

                        if (Result == mrOk)
                        {
                            Info = frmEditCustomer->Info;

                            if (Info.MembershipNumber == "")
                            {
                                if (MembershipSystem->RecycleMemberNumber)
                                {
                                  Info.MembershipNumber =  MembershipSystem->GetNextRecycledMemberNumber(DBTransaction);
                                }
                                else
                                {
                                    Info.MembershipNumber = MembershipSystem->GetNextMemberNumber(DBTransaction);
                                }
                        }

                        ManagerDiscount->DiscountKeyToCode(DBTransaction, Info.AutoAppliedDiscountsID, Info.AutoAppliedDiscounts);


                        /* Even though we have a parent transaction for all the updates, we need a seperate transaction for this setContactdetails method
                            because while the current transaction is not committed yet, the SaveContactInfoEditedToSmartCard will fire card update event,
                            SelectDish will catch the event, tries to load the contact from database, at that point old information will be picked up. */

                        Database::TDBTransaction DBSetMemberTransaction(DBControl);
                        DBSetMemberTransaction.StartTransaction();
                        // Update Member & Get MemberNumber.
                        MembershipSystem->SetContactDetails(DBSetMemberTransaction,
                                                            Info.ContactKey,
                                                            Info);
                        DBSetMemberTransaction.Commit();

                        if(!Info.IsCodePresent())
                        {
						  // Information has already been saved to database..Update Smart Card.
						  SaveContactInfoEditedToSmartCard(Info);
                        }
                        else
                        {
                          SyncBarcodeMemberDetailWithCloud(Info);
                        }

                        // Update Tab Removal.
						int TabKey = TDBTab::GetTabByOwner(DBTransaction, Info.ContactKey);
                        if (Info.TabEnabled == true && TabKey == 0) {
							TabKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
							TDBTab::SetTabOwner(DBTransaction, TabKey, Info.ContactKey, TabMember);
							TDBTab::SetTabName(DBTransaction, TabKey, Info.Name);
							TDBTab::SetTabPermanent(DBTransaction, TabKey, true);
						}
						else if (Info.TabEnabled == true && TabKey != 0)
						{
							TDBTab::SetTabPermanent(DBTransaction, TabKey, true);
						}
						else if (Info.TabEnabled == false && TabKey != 0)
						{
							TDBTab::SetTabPermanent(DBTransaction, TabKey, false);
                        }

						if (frmEditCustomer->ClearAllCards)
						{
							MembershipSystem->DeleteContactCards(DBTransaction, Info.ContactKey);
                        }

                        TDBContacts::RemoveCurrentGroups(DBTransaction,
                                                         Info.ContactKey);
                        for (int i = 0; i < Info.currentGroups.size(); i++) {
                            TDBContacts::SetCurrentGroups(DBTransaction,
                                                          Info.ContactKey,
                                                          Info.
                                                          currentGroups.
                                                          at(i));
                        }
                        TDBContacts::SetSummaGroup(DBTransaction,
                                                   Info.ContactKey, Info);

                        if (!Info.currentGroups.size()) {
                            ContactGroup GroupKey;

                            GroupKey.Key =
                              TDBGroups::FindGroup(DBTransaction,
                                                   "No Contacts Group");
                            TDBContacts::SetCurrentGroups(DBTransaction,
                                                          Info.ContactKey,
                                                          GroupKey);
                        }

                    }
                    EndMemberTransaction();
                }
            }
            catch(Exception & E) {
                MessageBox(E.Message, "Failed to Edit Member.",
                           MB_OK + MB_ICONERROR);
            }
        }
        __finally {
            CardIssueInProgress = false;
        }
    }
    return Result;
}

void TManagerMembershipGUI::Initialise(Database::TDBTransaction &DBTransaction)
{
   switch(TGlobalSettings::Instance().MembershipType)
   {
   case MembershipTypeMenuMate:
      MembershipSystem.reset(new TMembershipGUI(Modules));
      break;
   case MembershipTypeERS:
      MembershipSystem.reset(new TContactMemberERSGUI(Modules, IBMembershipDatabase));
      break;
   case MembershipTypeEBet:
	  MembershipSystem.reset(new TMembershipEBetGUI(Modules, TGlobalSettings::Instance().MembershipDatabaseIP, TGlobalSettings::Instance().MembershipDatabasePort, TGlobalSettings::Instance().MembershipTillID));
	  break;
   case MembershipTypeExternal:
	  MembershipSystem.reset(new TMembershipExternalGUI(Modules, TGlobalSettings::Instance().MembershipDatabaseIP, TGlobalSettings::Instance().MembershipDatabasePort, TGlobalSettings::Instance().MembershipTillID));
	  break;
   case MembershipTypeThor:
	  MembershipSystem.reset(new TMembershipThorlinkGUI(Modules, TGlobalSettings::Instance().ThorlinkAppKey, TGlobalSettings::Instance().ThorlinkSiteNo, TGlobalSettings::Instance().ThorlinkMerchantCode, TGlobalSettings::Instance().ThorlinkDeviceCode));
	  break;
   default:
	  MembershipSystem.reset(new TMembershipGUI(Modules));
   }
   MembershipSystem->Initialise(DBTransaction);
   TManagerMembershipSmartCards::Initialise(DBTransaction);
}


