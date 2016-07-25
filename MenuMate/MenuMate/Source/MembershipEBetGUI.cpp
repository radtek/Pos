//---------------------------------------------------------------------------
#pragma hdrstop

#include "MembershipEBetGUI.h"
#include "CardSwipe.h"
#include "MMMessageBox.h"
#include "MMContactInfo.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TMembershipEBetGUI::TMembershipEBetGUI(TModules &Modules, AnsiString inIP, int inPort, int inTillID)
: TMembershipEBet(Modules,inIP,inPort,inTillID)
{

}

TLoginSuccess TMembershipEBetGUI::Login(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo)
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
             MessageBox(ErrorMessage(),"Error Retrieving EBet Member", MB_OK + MB_ICONERROR);
         }
      }
   }
   else
   {
		LoginSuccess = lsAccepted;
   }


   return LoginSuccess;
}

