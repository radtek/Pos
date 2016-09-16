//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LoyaltyMateOperationDialogBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmLoyaltyMateOperationDialogBox *frmLoyaltyMateOperationDialogBox;
//---------------------------------------------------------------------------
__fastcall TfrmLoyaltyMateOperationDialogBox::TfrmLoyaltyMateOperationDialogBox(TComponent* Owner)
    : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoyaltyMateOperationDialogBox::btnCancelOperationMouseClick(TObject *Sender)

{
	loyaltyMemberDownloadThread->OnTerminate = NULL;
	loyaltyMemberDownloadThread->Terminate();
	ModalResult = mrCancel;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoyaltyMateOperationDialogBox::FormShow(TObject *Sender)
{
    this->Caption = OperationTitle;
    lblDescription->Caption = OperationDescription;
    btnCancelOperation->Enabled = !PreventCancelOperation;
    BarcodeMemberNotExist = false;
}
//---------------------------------------------------------------------------
void TfrmLoyaltyMateOperationDialogBox::setDownloadThread(TLoyaltyMateDownloadMemberThread* inDownloadThread)
{
	loyaltyMemberDownloadThread = inDownloadThread;
	if (inDownloadThread != NULL)
	{
		loyaltyMemberDownloadThread->FreeOnTerminate = true;
		loyaltyMemberDownloadThread->OnTerminate = loyaltyMateDownloadMemberThreadCompleted;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoyaltyMateOperationDialogBox::loyaltyMateDownloadMemberThreadCompleted(TObject* sender)
{
	//download complete, copy the values across and display the member information
	if(loyaltyMemberDownloadThread->OperationSuccessful)
	{
        Info.CloudUUID       = loyaltyMemberDownloadThread->ReturnContactInfo.CloudUUID;
        Info.Mobile          = loyaltyMemberDownloadThread->ReturnContactInfo.Mobile;
        Info.Phone          = loyaltyMemberDownloadThread->ReturnContactInfo.Phone;
        Info.EMail           = loyaltyMemberDownloadThread->ReturnContactInfo.EMail;
        Info.Name            = loyaltyMemberDownloadThread->ReturnContactInfo.Name;
        Info.Surname         = loyaltyMemberDownloadThread->ReturnContactInfo.Surname;
        Info.MailingAddress         = loyaltyMemberDownloadThread->ReturnContactInfo.MailingAddress;
        Info.LocationAddress         = loyaltyMemberDownloadThread->ReturnContactInfo.LocationAddress;
        Info.Title         = loyaltyMemberDownloadThread->ReturnContactInfo.Title;
        Info.ActivationToken = loyaltyMemberDownloadThread->ReturnContactInfo.ActivationToken;
        Info.DateOfBirth     = loyaltyMemberDownloadThread->ReturnContactInfo.DateOfBirth;
        Info.Points 		 = loyaltyMemberDownloadThread->ReturnContactInfo.Points;
        Info.LastVisit       = loyaltyMemberDownloadThread->ReturnContactInfo.LastVisit;
        Info.TierLevel       = loyaltyMemberDownloadThread->ReturnContactInfo.TierLevel;
        Info.LastBirthdayProcessed       = loyaltyMemberDownloadThread->ReturnContactInfo.LastBirthdayProcessed;
        Info.MemberType       = loyaltyMemberDownloadThread->ReturnContactInfo.MemberType;
        Info.ActivationDate       = loyaltyMemberDownloadThread->ReturnContactInfo.ActivationDate;
        Info.PreviousYearPoint       = loyaltyMemberDownloadThread->ReturnContactInfo.PreviousYearPoint;
        Info.CurrentYearPoint       = loyaltyMemberDownloadThread->ReturnContactInfo.CurrentYearPoint;
        Info.AvailableBDPoint       = loyaltyMemberDownloadThread->ReturnContactInfo.AvailableBDPoint;
        Info.AvailableFVPoint       = loyaltyMemberDownloadThread->ReturnContactInfo.AvailableFVPoint;
        Info.MembershipNumber  = loyaltyMemberDownloadThread->ReturnContactInfo.MembershipNumber;
        Info.SiteID  = loyaltyMemberDownloadThread->ReturnContactInfo.SiteID;
        Info.PointRule  = loyaltyMemberDownloadThread->ReturnContactInfo.PointRule;
        Info.LastModified = loyaltyMemberDownloadThread->ReturnContactInfo.LastModified;
        Info.IsFirstVisitRewarded = loyaltyMemberDownloadThread->ReturnContactInfo.IsFirstVisitRewarded;
        Info.MemberVouchers = loyaltyMemberDownloadThread->ReturnContactInfo.MemberVouchers;
        Info.MemberCode = loyaltyMemberDownloadThread->ReturnContactInfo.MemberCode;
		ModalResult = mrOk;
	}
	else
	{
        if(loyaltyMemberDownloadThread->ErrorMessage  == "Member Not Exist.")
          {
              BarcodeMemberNotExist = true;
              MessageBox("Member Not Exist on server.","Member Not Exist", MB_ICONERROR + MB_OK);
          }
        else
          {
             MessageBox("Failed to download member details.","Failed download", MB_ICONERROR + MB_OK);
          }
		ModalResult = mrAbort;
	}
}
//---------------------------------------------------------------------------
