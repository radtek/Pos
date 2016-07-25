//---------------------------------------------------------------------------
#pragma hdrstop


#include "ManagerVoucherElectronic.h"
#include "MMMessageBox.h"
#include "ReferenceManager.h"
#include "VoucherAPIService.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#pragma package(smart_init)

TManagerVoucherElectronic::TManagerVoucherElectronic()
{

}

bool TManagerVoucherElectronic::ProcessVoucher(TPaymentTransaction &PaymentTransaction,TPayment *Payment)
{
   try
   {
       _di_VoucherAPI VoucherAPI  = NS_VoucherAPIService::GetVoucherAPI(false,Payment->UniVoucherURL,NULL);
       Payment->UniVoucherToken = VoucherAPI->AuthToken(Payment->UniVoucherUser,Payment->UniVoucherPass);

       voucherResponse *Responce = NULL;
       std::auto_ptr<TXSDecimal> Value(new TXSDecimal());
       Value->DecimalString = FormatFloat("0.00",Payment->GetPayTotal());
       if(PaymentTransaction.CreditTransaction)
       {
            Responce = VoucherAPI->CreditVoucher(Payment->UniVoucherToken,
                                        Payment->VoucherMerchantID,
                                        Payment->VoucherCode,
                                        Value.get());

       }
       else
       {
            Responce = VoucherAPI->RedeemVoucher(Payment->UniVoucherToken,
                                        Payment->VoucherMerchantID,
                                        Payment->VoucherCode,
                                        Value.get());
       }

       if(Responce != NULL)
       {
          switch(Responce->Status)
          {
              case 200 :
                    Payment->Result = eAccepted;
                    if(Payment->Properties & ePayTypeDispPVMsg)
                    {
                        MessageBox("Voucher Accepted","Voucher Accepted", MB_OK + MB_ICONINFORMATION);
                    }
              break;
              case 400 :
                Payment->Result = eFailed;
                if(Payment->Properties & ePayTypeDispPVMsg)
                {
                    MessageBox("(400) Voucher Already Redeemed or Refunded.","Voucher Declined", MB_OK + MB_ICONINFORMATION);
                }
              break;
              case 401 :
                Payment->Result = eFailed;
                if(Payment->Properties & ePayTypeDispPVMsg)
                {
                    MessageBox("(401) Voucher Auth Expired.","Voucher Declined", MB_OK + MB_ICONINFORMATION);
                }
              break;
              case 403 :
                Payment->Result = eFailed;
                if(Payment->Properties & ePayTypeDispPVMsg)
                {
                    MessageBox("(403) Merchant ID is invalid.","Voucher Declined", MB_OK + MB_ICONINFORMATION);
                }
              break;
              case 404 :
                Payment->Result = eFailed;
                if(Payment->Properties & ePayTypeDispPVMsg)
                {
                    MessageBox("(404) Voucher with that code was not found.","Voucher Declined", MB_OK + MB_ICONINFORMATION);
                }
              break;
              case 500 :
                Payment->Result = eFailed;
                if(Payment->Properties & ePayTypeDispPVMsg)
                {
                    MessageBox("(500) Voucher Server suffered an Error.","Voucher Declined", MB_OK + MB_ICONINFORMATION);
                }
              break;
              default :
                Payment->Result = eFailed;
                if(Payment->Properties & ePayTypeDispPVMsg)
                {
                    MessageBox("(500) Unknown Voucher Result.","Voucher Declined", MB_OK + MB_ICONINFORMATION);
                }
          }
       }
       else
       {
            Payment->Result = eFailed;
            MessageBox("Voucher System Failed to respond","Voucher Declined", MB_OK + MB_ICONERROR);
       }
   	}
   	catch(Exception & E)
    {
        Payment->Result = eFailed;
        MessageBox(E.Message,"Voucher Declined", MB_OK + MB_ICONERROR);
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

