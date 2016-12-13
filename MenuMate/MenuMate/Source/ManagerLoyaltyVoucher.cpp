//---------------------------------------------------------------------------
#pragma hdrstop

#include "ManagerLoyaltyVoucher.h"

#include "ManagerLoyaltyMate.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "DeviceRealTerminal.h"
#include "ManagerSyndCode.h"
#include "LoyaltyMateUtilities.h"
#include "ManagerDiscount.h"
#include "VerticalSelect.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


void __fastcall TManagerLoyaltyVoucher::loyaltyMateOperationCompleted(TObject* sender)
{
    _lmOperationDialogBox->Close();
}
//------------------------------------------------------------------------------
void TManagerLoyaltyVoucher::GetPocketVoucherDetail(AnsiString voucherCode,TVoucherDetail &VoucherDetail)
{
    bool result = false;
    TManagerSyndCode managerSyndCode = TDeviceRealTerminal::Instance().ManagerMembership->GetSyndicateCodeManager();
    TSyndCode syndicateCode =  managerSyndCode.GetCommunicationSyndCode();
    if(syndicateCode.Valid())
     {
        TLoyaltyMatePocketVoucherThread* voucherThread = new TLoyaltyMatePocketVoucherThread(syndicateCode);
        voucherThread->OnTerminate = loyaltyMateOperationCompleted;
        voucherThread->FreeOnTerminate = false;
        voucherThread->VoucherNumber = voucherCode;
        voucherThread->VoucherDetail = VoucherDetail;
        voucherThread->Start();
        // display dialog box
        _lmOperationDialogBox = new TfrmLoyaltyMateOperationDialogBox(Screen->ActiveForm);
        _lmOperationDialogBox->OperationDescription = "Getting Voucher Details with server...Please Wait.";
        _lmOperationDialogBox->OperationTitle = "LoyaltyMate Operation";
        _lmOperationDialogBox->PreventCancelOperation = true;
        _lmOperationDialogBox->ShowModal();

        VoucherDetail.VoucherName  = voucherThread->VoucherDetail.VoucherName;
        VoucherDetail.DiscountCode  = voucherThread->VoucherDetail.DiscountCode;
        VoucherDetail.NumberOfUsesRemaining  = voucherThread->VoucherDetail.NumberOfUsesRemaining;
        result = voucherThread->OperationSuccessful;
        AnsiString errorMessage =  voucherThread->ErrorMessage;
        if(!result)
        {
            if(errorMessage == NULL || errorMessage == "")
               errorMessage == "Failed to get voucher details." ;
            else if(errorMessage == "Invalid Pocket Voucher Number")
               MessageBox("Supplied voucher is not valid. please use another voucher or payment method.","Menumate", MB_ICONERROR + MB_OK);
            else
              MessageBox(errorMessage,"Menumate", MB_ICONERROR + MB_OK);
        }
        // cleanup
        delete voucherThread;
        delete _lmOperationDialogBox;
    }
}
//------------------------------------------------------------------------------
void TManagerLoyaltyVoucher::GetGiftVoucherDetail(AnsiString voucherCode,TGiftCardDetail &GiftCardDetail)
{
    TManagerSyndCode managerSyndCode = TDeviceRealTerminal::Instance().ManagerMembership->GetSyndicateCodeManager();
    TSyndCode syndicateCode =  managerSyndCode.GetCommunicationSyndCode();

    if(syndicateCode.Valid())
     {
        TLoyaltyMateGiftVoucherThread* voucherThread = new TLoyaltyMateGiftVoucherThread(syndicateCode);
        voucherThread->OnTerminate = loyaltyMateOperationCompleted;
        voucherThread->FreeOnTerminate = false;
        voucherThread->GiftCardNumber = voucherCode;
        voucherThread->GiftCardDetail = GiftCardDetail;
        voucherThread->Start();
        // display dialog box
        _lmOperationDialogBox = new TfrmLoyaltyMateOperationDialogBox(Screen->ActiveForm);
        _lmOperationDialogBox->OperationDescription = "Getting Gift Card Details with server...Please Wait.";
        _lmOperationDialogBox->OperationTitle = "LoyaltyMate Operation";
        _lmOperationDialogBox->PreventCancelOperation = true;
        _lmOperationDialogBox->ShowModal();

        GiftCardDetail.ResponseMessage  = voucherThread->GiftCardDetail.ResponseMessage;
        GiftCardDetail.StatusCode  = voucherThread->GiftCardDetail.StatusCode;
        GiftCardDetail.PointBalance  = voucherThread->GiftCardDetail.PointBalance;
        GiftCardDetail.ExpiryDate  = voucherThread->GiftCardDetail.ExpiryDate;

        bool result = voucherThread->OperationSuccessful;
        AnsiString errorMessage =  voucherThread->ErrorMessage;
        if(!result)
        {
            if(errorMessage == NULL || errorMessage == "")
               MessageBox("Failed to get Gift card details.","Menumate", MB_ICONERROR + MB_OK);
            else if(errorMessage == "Invalid Gift Voucher Number")
               MessageBox("Supplied voucher is not valid. please use another voucher or payment method.","Menumate", MB_ICONERROR + MB_OK);
            else
               MessageBox(voucherThread->ErrorMessage,"Menumate", MB_ICONERROR + MB_OK);
         }
        delete voucherThread;
        delete _lmOperationDialogBox;
    }
}
//------------------------------------------------------------------------------
bool TManagerLoyaltyVoucher::ProcessVouchers(TVoucherUsageDetail& VoucherUsageDetail)
{
    bool transactionStatus = false;
    TManagerSyndCode managerSyndCode = TDeviceRealTerminal::Instance().ManagerMembership->GetSyndicateCodeManager();
    TSyndCode syndicateCode =  managerSyndCode.GetCommunicationSyndCode();

    if(syndicateCode.Valid())
     {
        TLoyaltyMateVoucherProcessThread* voucherThread = new TLoyaltyMateVoucherProcessThread(syndicateCode);
        voucherThread->OnTerminate = loyaltyMateOperationCompleted;
        voucherThread->FreeOnTerminate = false;
        voucherThread->VoucherUsageDetail = VoucherUsageDetail;
        voucherThread->Start();
        // display dialog box
        _lmOperationDialogBox = new TfrmLoyaltyMateOperationDialogBox(Screen->ActiveForm);
        _lmOperationDialogBox->OperationDescription = "Processing Loyaltymate Operations...Please Wait.";
        _lmOperationDialogBox->OperationTitle = "LoyaltyMate Operation";
        _lmOperationDialogBox->PreventCancelOperation = true;
        _lmOperationDialogBox->ShowModal();
        VoucherUsageDetail.GiftCardExpiryDate  = voucherThread->VoucherUsageDetail.GiftCardExpiryDate;
        transactionStatus = voucherThread->OperationSuccessful;
        AnsiString errorMessage =  voucherThread->ErrorMessage;
        if(!transactionStatus)
        {
            if(errorMessage == NULL || errorMessage != "")
               errorMessage == "Failed to process vouchers and discounts.";
            MessageBox(errorMessage,"Menumate", MB_ICONERROR + MB_OK);
        }
        delete voucherThread;
        delete _lmOperationDialogBox;
    }
    return transactionStatus;

}
//------------------------------------------------------------------------------
bool TManagerLoyaltyVoucher::ReleaseVouchers(TReleasedVoucherDetail ReleasedVoucherDetail)
{
    bool transactionStatus = false;
    TManagerSyndCode managerSyndCode = TDeviceRealTerminal::Instance().ManagerMembership->GetSyndicateCodeManager();
    TSyndCode syndicateCode =  managerSyndCode.GetCommunicationSyndCode();

    if(syndicateCode.Valid())
     {
        TLoyaltyMateReleaseVoucherThread* voucherThread = new TLoyaltyMateReleaseVoucherThread(syndicateCode);
        voucherThread->OnTerminate = loyaltyMateOperationCompleted;
        voucherThread->FreeOnTerminate = false;
        voucherThread->ReleasedVoucherDetail = ReleasedVoucherDetail;
        voucherThread->Start();
        // display dialog box
        _lmOperationDialogBox = new TfrmLoyaltyMateOperationDialogBox(Screen->ActiveForm);
        _lmOperationDialogBox->OperationDescription = "Processing Loyaltymate Operations...Please Wait.";
        _lmOperationDialogBox->OperationTitle = "LoyaltyMate Operation";
        _lmOperationDialogBox->PreventCancelOperation = true;
        _lmOperationDialogBox->ShowModal();

        transactionStatus = voucherThread->OperationSuccessful;
        AnsiString errorMessage =  voucherThread->ErrorMessage;
        if(!transactionStatus)
        {
            if(errorMessage == NULL || errorMessage != "")
               errorMessage == "Failed to process vouchers and discounts.";
            MessageBox(errorMessage,"Menumate", MB_ICONERROR + MB_OK);
        }
        delete voucherThread;
        delete _lmOperationDialogBox;
    }
    return transactionStatus;
}
// ---------------------------------------------------------------------------
void TManagerLoyaltyVoucher::DisplayMemberVouchers(Database::TDBTransaction &DBTransaction,TMMContactInfo &MemberInfo)
{
  if( TGlobalSettings::Instance().LoyaltyMateEnabled  && !TGlobalSettings::Instance().IsPOSOffline
     && TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->MemberVouchers.size() > 0)
  {
	// Display Reports List
	std::auto_ptr<TfrmVerticalSelect>SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(Screen->ActiveForm));

	TVerticalSelection Item;
	Item.Title = "Cancel";
	Item.Properties["Color"] = IntToStr(clMaroon);
	Item.CloseSelection = true;
	SelectionForm->Items.push_back(Item);

    for(std::vector<TVoucherDetail>::iterator itVoucher = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->MemberVouchers.begin() ;
        itVoucher != TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->MemberVouchers.end(); ++itVoucher)
    {
       	TVerticalSelection Item1;
        Item1.Title = itVoucher->VoucherName;
        Item1.Properties["Action"] = itVoucher->DiscountCode;
        Item1.Properties["Color"] = IntToStr(clNavy);
        Item1.CloseSelection = true;
        SelectionForm->Items.push_back(Item1);
    }


	SelectionForm->ShowModal();
	TVerticalSelection SelectedItem;
	if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
	{
       //Get old Discount Id
       if(TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount != "")
       {

          int oldDiscountKey = ManagerDiscount->GetDiscountKeyByCode(DBTransaction,
                         TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount);
          if(oldDiscountKey > 0)
           {
             if(MemberInfo.AutoAppliedDiscounts.find(oldDiscountKey) != MemberInfo.AutoAppliedDiscounts.end())
                MemberInfo.AutoAppliedDiscounts.erase(oldDiscountKey);
             int size = MemberInfo.AutoAppliedDiscounts.size();
           }

       }
       AnsiString discountCode = SelectedItem.Properties["Action"];
       int discountKey = ManagerDiscount->GetDiscountKeyByCode(DBTransaction,discountCode);
       if(discountKey > 0)
       {
            TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount =  discountCode;
            TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherName = SelectedItem.Title;
            MemberInfo.AutoAppliedDiscounts.insert(discountKey);
       }
    }
    else
    {
            TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount =  "";
            TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherName = "";
    }
  }
}

