//---------------------------------------------------------------------------

#ifndef ManagerLoyaltyVoucherH
#define ManagerLoyaltyVoucherH
#include "LoyaltyMateOperationDialogBox.h"
#include "MM_DBCore.h"
#include "MMContactInfo.h"
//---------------------------------------------------------------------------
class TManagerLoyaltyVoucher
{
   public:
    void GetPocketVoucherDetail(AnsiString voucherCode,TVoucherDetail& VoucherDetail);
    double GetGiftVoucherDetail(AnsiString voucherCode,bool &isValidGiftCard);
    bool ProcessVouchers(TVoucherUsageDetail VoucherUsageDetail);
    bool ReleaseVouchers(TReleasedVoucherDetail inReleasedVoucherDetail);
    void DisplayMemberVouchers(Database::TDBTransaction &DBTransaction,TMMContactInfo &MemberInfo);
   private:
    TfrmLoyaltyMateOperationDialogBox* _lmOperationDialogBox;
    void __fastcall loyaltyMateOperationCompleted(TObject* sender);
};
#endif
