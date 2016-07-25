// ---------------------------------------------------------------------------
#pragma hdrstop

#include "MembershipERSGUI.h"
#include "SelectMember.h"
#include "Modules.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

// ---------------------------------------------------------------------------
#pragma package(smart_init)

TContactMemberERSGUI::TContactMemberERSGUI(TModules &Modules, Database::TDBControl &inIBDatabase)
: TMembershipERS(Modules, inIBDatabase)
{
}

void TContactMemberERSGUI::Initialise(Database::TDBTransaction &DBTransaction)
{
   TMembershipERS::Initialise(DBTransaction);

   NameOnPoleDisplay = (eMemberNameOnPoleDisplay)TManagerVariable::Instance().GetInt(DBTransaction, vmMemberNameOnPoleDisplay, (int)eMNPDNone);  //req or not
   EnableIncrementalSearch = TManagerVariable::Instance().GetBool(DBTransaction, vmMemberIncrementalSearch, false);
   MinMemberNumber = TManagerVariable::Instance().GetInt(DBTransaction, vmMinMemberNumber, 1000);
   RecycleMemberNumber = TManagerVariable::Instance().GetBool(DBTransaction, vmRecycleMemberNumber, false);
   MaxRemembered = TManagerVariable::Instance().GetInt(DBTransaction, vmMemberQuickButtonCount, MaxRemembered);
}

TLoginSuccess TContactMemberERSGUI::Login(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
   if (!fEnabled)
	  return lsUserNotFound;
   TLoginSuccess LoginSuccess = lsUserNotFound;

   if (UserInfo.CardStr != "")
   {
	  // Automatically login this user.
	  UnicodeString Description;
	  LoginSuccess = TestCardLogin(DBTransaction, UserInfo);
   }
   else if (UserInfo.ContactKey != 0)
   {
	  // Try to log this user in.
	  UnicodeString Description;
	  LoginSuccess = TestKeyLogin(DBTransaction, UserInfo);
   }
   else
   {
	  MaxRemembered = TManagerVariable::Instance().GetInt(DBTransaction, vmMemberQuickButtonCount, MaxRemembered);
	  std::auto_ptr <TfrmSelectMember> frmSelectMember(TfrmSelectMember::Create <TfrmSelectMember> (Screen->ActiveForm));
	  if (frmSelectMember->SelectMember(LastAccessed) == mrOk)
	  {
		 if (find(LastAccessed.begin(), LastAccessed.end(), TQuickMember(frmSelectMember->SelectedUser.ExternalKey,
				  frmSelectMember->SelectedUser.Name)) == LastAccessed.end())
		 {
			LastAccessed.insert(LastAccessed.begin(), TQuickMember(frmSelectMember->SelectedUser.ExternalKey,
				  frmSelectMember->SelectedUser.Name));
		 }
		 while (LastAccessed.size() > MaxRemembered)
		 {
			LastAccessed.pop_back();
		 }

		 UserInfo = frmSelectMember->SelectedUser;
		 LoginSuccess = TestKeyLogin(DBTransaction, UserInfo);
	  }
	  else
	  {
		 LoginSuccess = lsCancel;
	  }
   }

   if (LoginSuccess == lsAccepted)
   {
	  GetContactDetails(DBTransaction, UserInfo.ContactKey, UserInfo);
   }

   return LoginSuccess;
}
