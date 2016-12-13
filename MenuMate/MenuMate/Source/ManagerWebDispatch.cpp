//---------------------------------------------------------------------------


#pragma hdrstop

#include "SelectReceipt.h"
#include "ReceiptManager.h"
#include "MMMessageBox.h"
#include "Contact.h"
#include "MM_DBCore.h"
#include "ReportDisplayNav.h"
#include "ContactStaff.h"
#include "ManagerWebDispatch.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerWebDispatch::TManagerWebDispatch(TForm *inOwner,
   TWebBrowser *inBrowser)
{
   Enabled = false;
   Owner = inOwner;
   Browser = inBrowser;
   if(Owner != NULL && Browser != NULL)
   {
      Enabled = true;
   }
}

void TManagerWebDispatch::Execute(UnicodeString CommandUrl,WordBool &Cancel)
{
   if(Enabled)
   {
      if(CommandUrl.Pos(":internal:") != 0)
      {
         AnsiString Command = "";
         if(CommandUrl.Pos("[") != 0 && CommandUrl.Pos("]") != 0)
         {
            Command = CommandUrl.SubString(CommandUrl.Pos("[")+1,CommandUrl.Pos("]") - CommandUrl.Pos("[")-1);
         }

         if(Command.LowerCase() == "showreceipt")
         {
            AnsiString ReceiptNumber = 0;
            ReceiptNumber = CommandUrl.SubString(CommandUrl.Pos("{")+1,CommandUrl.Pos("}") - CommandUrl.Pos("{")-1);
            if (ManagerReceipt->Find(ReceiptNumber))
            {
                  bool canShow = false;
                  TMMContactInfo TempUserInfo;
                  Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
                  DBTransaction.StartTransaction();
                  std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
                  bool AccessForReprint = Staff->TestAccessLevel( TDeviceRealTerminal::Instance().User, CheckReprintReceipt);
                  if(AccessForReprint)
                  {
                        DBTransaction.Commit();
                        TDeviceRealTerminal::Instance().User.LatestLoginContactKey =TDeviceRealTerminal::Instance().User.ContactKey;
                        TDeviceRealTerminal::Instance().User.LatestLoginName  = TDeviceRealTerminal::Instance().User.Name;
                        canShow = true;
                  }
                  else
                  {
                      TLoginSuccess Result = Staff->Login(Owner,DBTransaction,TempUserInfo, CheckReprintReceipt);
                      DBTransaction.Commit();
                      TDeviceRealTerminal::Instance().User.LatestLoginContactKey =0;
                      TDeviceRealTerminal::Instance().User.LatestLoginName  = "";
                      if (Result == lsAccepted)
                      {
                            TDeviceRealTerminal::Instance().User.LatestLoginContactKey =TempUserInfo.ContactKey;
                            TDeviceRealTerminal::Instance().User.LatestLoginName  = TempUserInfo.Name;
                            canShow = true;
                      }
                      else if (Result == lsDenied)
                      {
                            MessageBox("You do not have access privileges to reprint receipt.", "Error", MB_OK + MB_ICONERROR);
                      }
                      else if (Result == lsPINIncorrect)
                      {
                            MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
                      }
                  }

                  if(canShow)
                  {
                        std::auto_ptr<TfrmSelectReceipt> frmSelectReceipt(new TfrmSelectReceipt(Owner));
                        frmSelectReceipt->Execute(ReceiptNumber);
                  }
            }
            else
            {
               MessageBox("Unable to find Receipt #" + ReceiptNumber , "Receipt Not Found",MB_OK + MB_ICONERROR);
            }
         }
         else if(Command.LowerCase() == "showreportpointsinformation")
         {
      		std::auto_ptr<TStringList> Report(new TStringList);
            UnicodeString ReportLocation = "";
            AnsiString MemberKey = "0";
			MemberKey = CommandUrl.SubString(CommandUrl.Pos("{")+1,CommandUrl.Pos("}") - CommandUrl.Pos("{")-1);
            TMMContactInfo TempUserInfo;
            eMemberSource MemberSource;
            TempUserInfo.ContactKey = StrToIntDef(MemberKey,0);
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
            DBTransaction.StartTransaction();

         	TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction,TempUserInfo,MemberSource);
            if(Result == lsUserNotFound)
            {

               MessageBox("Member "+ TempUserInfo.Name+" is not found.","Member not found",MB_OK + MB_ICONINFORMATION);
            }
            else
            {
               TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStart(DBTransaction,TempUserInfo,Report.get());
               TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberPointsInfo(DBTransaction,TempUserInfo,Report.get());
               TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStop(DBTransaction,TempUserInfo,Report.get());
            }

            DBTransaction.Commit();

            if(!DirectoryExists(ExtractFilePath(Application->ExeName) + "html"))
            {
               CreateDir(ExtractFilePath(Application->ExeName) + "html");
            }
            ReportLocation = ExtractFilePath(Application->ExeName) + "html\\MembershipReport.html";
				Report->SaveToFile(ReportLocation);
				Browser->Navigate(ReportLocation.w_str());
			}
         else if(Command.LowerCase() == "smartcarddetails")
         {
      		std::auto_ptr<TStringList> Report(new TStringList);
            AnsiString SmartCardKey = "0";

            SmartCardKey = CommandUrl.SubString(CommandUrl.Pos("{")+1,CommandUrl.Pos("}") - CommandUrl.Pos("{")-1);
            int intSmartCardKey = StrToIntDef(SmartCardKey,0);

            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
            DBTransaction.StartTransaction();

            TDeviceRealTerminal::Instance().ManagerMembership->GetSmartCardReportForCardKey(DBTransaction,intSmartCardKey,Report.get());

            DBTransaction.Commit();
            std::auto_ptr<TfrmReportDisplayNav> frmReportDisplay(TfrmReportDisplayNav::Create<TfrmReportDisplayNav>(Screen->ActiveForm));
            frmReportDisplay->Navigate(Report.get());
            frmReportDisplay->ShowModal();
         }
         Cancel = -1;
      }
   }
   else
   {
      Cancel = -1;
   }
}
