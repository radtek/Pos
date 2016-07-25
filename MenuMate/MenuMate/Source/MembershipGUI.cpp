//---------------------------------------------------------------------------
#pragma hdrstop

#include "MembershipGUI.h"
#include "SelectMember.h"
#include "DBContacts.h"
//---------------------------------------------------------------------------
#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#pragma package(smart_init)

TMembershipGUI::TMembershipGUI(TModules &Modules) : TMembership(Modules)
{

}

void TMembershipGUI::Initialise(Database::TDBTransaction &DBTransaction)
{
   TMembership::Initialise(DBTransaction);

   NameOnPoleDisplay = (eMemberNameOnPoleDisplay)TManagerVariable::Instance().GetInt(DBTransaction, vmMemberNameOnPoleDisplay, (int)eMNPDNone);      // req or not..
   EnableIncrementalSearch = TManagerVariable::Instance().GetBool(DBTransaction, vmMemberIncrementalSearch, false);
   MinMemberNumber = TManagerVariable::Instance().GetInt(DBTransaction, vmMinMemberNumber, 1000);
   RecycleMemberNumber = TManagerVariable::Instance().GetBool(DBTransaction, vmRecycleMemberNumber, false);
   MaxRemembered = TManagerVariable::Instance().GetInt(DBTransaction, vmMemberQuickButtonCount, MaxRemembered);
}

TLoginSuccess TMembershipGUI::Login(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo)
{
	if( !fEnabled )return lsUserNotFound;
	TLoginSuccess LoginSuccess = lsUserNotFound;

   if(UserInfo.ContactKey == 0)
	{
      if (UserInfo.CardStr != "")
      {
         UserInfo.ContactKey = GetContactByCard(DBTransaction,UserInfo);
      }
      else if (UserInfo.MembershipNumber != 0 && UserInfo.SiteID != 0)
      {
         UserInfo.ContactKey = TDBContacts::GetContactByMemberNumberSiteID(DBTransaction, UserInfo.MembershipNumber,UserInfo.SiteID);
      }
      TDBContacts::GetTierLevelAndMemberType(DBTransaction, UserInfo);
   }

   if(UserInfo.ContactKey != 0)
   {
		// Try to log this user in.
		UnicodeString Description;
		LoginSuccess = TestKeyLogin(DBTransaction,UserInfo);
   }
	else
	{
		MaxRemembered = TManagerVariable::Instance().GetInt(DBTransaction,vmMemberQuickButtonCount,MaxRemembered);

		std::auto_ptr<TfrmSelectMember> frmSelectMember(TfrmSelectMember::Create<TfrmSelectMember>(Screen->ActiveForm));
		if (frmSelectMember->SelectMember(LastAccessed) == mrOk)
		{
			if(find(LastAccessed.begin(),LastAccessed.end(),TQuickMember(frmSelectMember->SelectedUser.ContactKey,frmSelectMember->SelectedUser.Name)) == LastAccessed.end())
			{
				LastAccessed.insert(LastAccessed.begin(),TQuickMember(frmSelectMember->SelectedUser.ContactKey,frmSelectMember->SelectedUser.Name));
			}
			while(LastAccessed.size() > MaxRemembered)
			{
				LastAccessed.pop_back();
			}

			UserInfo	 = frmSelectMember->SelectedUser;
			LoginSuccess = TestKeyLogin(DBTransaction,UserInfo);
		}
		else
		{
			LoginSuccess = lsCancel;
		}
	}

   if(GetContactBlocked(DBTransaction,UserInfo))
   {
      LoginSuccess = lsAccountBlocked;
   }

   if(LoginSuccess == lsAccepted)
   {
     GetContactDetails(DBTransaction, UserInfo.ContactKey, UserInfo);
   }

	return LoginSuccess;
}
