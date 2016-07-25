//---------------------------------------------------------------------------

#pragma hdrstop

#include "MembershipThorlinkGUI.h"
#include "CardSwipe.h"
#include "MMMessageBox.h"
#include "MMContactInfo.h"
#include "MMTouchNumpad.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TMembershipThorlinkGUI::TMembershipThorlinkGUI(TModules &Modules,AnsiString AppKey, AnsiString SiteNo, AnsiString MerchantCode,AnsiString DeviceCode)
: TMembershipThorlink(Modules,AppKey,SiteNo,MerchantCode,DeviceCode)
{

}




TLoginSuccess TMembershipThorlinkGUI::Login(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo)
{
	if( !fEnabled )return lsUserNotFound;
	TLoginSuccess LoginSuccess = lsUserNotFound;

   if (UserInfo.ContactKey == 0)
   {
	  std::auto_ptr<TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create<TfrmCardSwipe>(Screen->ActiveForm));
	  frmCardSwipe->tbOtherOpt->Visible = false;
	  frmCardSwipe->ShowModal();
	  if (frmCardSwipe->ModalResult == mrOk)
	  {
		 UserInfo.CardStr = AnsiString(frmCardSwipe->SwipeString).SubString(1,80);

		 LoginSuccess = TestCardLogin(DBTransaction,UserInfo);
		 if(UserInfo.ContactKey == 0)
		 {
			  MessageBox("Unable to find member","Error Retrieving Member", MB_OK + MB_ICONERROR);
		 }
	  }
   }

   else if(UserInfo.ExternalKey == 0 && GetExternalContactDetialsFromKey(DBTransaction, UserInfo))
   {
		LoginSuccess = lsAccepted;
   }

   else
   {
   		LoginSuccess = lsAccepted;
   }



   return LoginSuccess;
}




