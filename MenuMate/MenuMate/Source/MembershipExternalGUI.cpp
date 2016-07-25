//---------------------------------------------------------------------------

#pragma hdrstop

#include "MembershipExternalGUI.h"
#include "CardSwipe.h"
#include "MMMessageBox.h"
#include "MMContactInfo.h"
#include "MMTouchNumpad.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TMembershipExternalGUI::TMembershipExternalGUI(TModules &Modules, AnsiString inIP, int inPort, int inTillID)
: TMembershipExternal(Modules,inIP,inPort,inTillID)
{

}

TLoginSuccess TMembershipExternalGUI::Login(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo)
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
   { /* Look the member up from DB fill if with in Time Peroid */
		LoginSuccess = lsAccepted;
   }
   else
   {
   		LoginSuccess = lsAccepted;
   }



   return LoginSuccess;
}

/* Membership systems can overide this to implement some kind of Auth on Points transactions
If this is not accepted a dilog should be shown to say why.*/
TLoginSuccess TMembershipExternalGUI::Authorise(TMMContactInfo &UserInfo,Currency wrkPayAmount)
{
	if( !fEnabled )return lsDenied;

	TLoginSuccess LoginSuccess = lsDenied;
    bool ManPIN = true;
	try
	{
        AnsiString Message = "";
    	if(UserInfo.PIN != "")
        {
			if(AuthPIN(UserInfo,UserInfo.PIN,Message))
            {
				LoginSuccess = lsAccepted;
                ManPIN = false;
            }
        }

        if(ManPIN)
        {
            AnsiString TempPIN1 = "";
            std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(Screen->ActiveForm));
            frmTouchNumpad->Caption = "Enter PIN";
            frmTouchNumpad->btnOk->Visible = true;
            frmTouchNumpad->btnCancel->Visible = true;
            frmTouchNumpad->Mode = pmPIN;
            if (frmTouchNumpad->ShowModal() == mrOk)
            {
                TempPIN1 = frmTouchNumpad->STRResult;
                if(TempPIN1.Length() > 0)
                {
                    if (AuthPIN(UserInfo,TempPIN1,Message))
                    {
                        LoginSuccess = lsAccepted;
                    }
                    else
                    {
                        MessageBox(Message,"Authentication Failed", MB_OK + MB_ICONERROR);
                    }
                }
                else
                {
                    MessageBox("You must enter a PIN","Authentication Failed", MB_OK + MB_ICONERROR);
                }
            }
        }
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		MessageBox(E.Message,"Authentication Error", MB_OK + MB_ICONERROR);
	}
	return LoginSuccess;
}

bool TMembershipExternalGUI::AuthPIN(TMMContactInfo &UserInfo,AnsiString PIN,AnsiString &Message)
{
    bool RetVal = false;
	if(PIN.Length() > 0)
    {
        std::auto_ptr<TMSXMLEnquiry> request(new TMSXMLEnquiry());
        request->SetTransNo(1);
        request->SetTerminalID(TillID);
        request->SetTerminalName("MenuMate");
        request->SetCard(UserInfo.CardStr);
        request->SetPIN(PIN);
        #ifdef _DEBUG
        request->Build();
        request->SaveToFile();
        #endif
        MembershipGeneralLedgerTCP->Process(IP,Port,*request.get());
        if (request->Result == eMSAccepted && request->Authorised != 0)
        {
            RetVal = true;
        }
        Message = request->ResultText;
    }
    return RetVal;
}

