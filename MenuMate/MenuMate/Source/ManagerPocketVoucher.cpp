//---------------------------------------------------------------------------
#pragma hdrstop


#include "ManagerPocketVoucher.h"
#include "ReferenceManager.h"
#include "PocketVoucherInterface.h"
#include "DBOrder.h"
#include "OrderUtils.h"
//---------------------------------------------------------------------------
#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#pragma package(smart_init)

TManagerPocketVoucher::TManagerPocketVoucher() :
	XML_HTTP(new TXML_Transport_Http)
{

}

void TManagerPocketVoucher::Initialise(Database::TDBTransaction &DBTransaction)
{

	URL = TManagerVariable::Instance().GetStr(DBTransaction, vmPocketVoucherURL,
     TDeviceRealTerminal::Instance().PocketVouchers->URL);

}void TManagerPocketVoucher::ProcessVoucher(TPaymentTransaction &PaymentTransaction,TPayment *Payment)
{
     try
     {
         if(TDeviceRealTerminal::Instance().PocketVouchers->URL == "https://services.host.pocketvouchers.com/redemption.svc")
         {
             std::auto_ptr<TPocketVoucherInterface> Voucher (new TPocketVoucherInterface());
             Request request;
             request.voucherCode = Payment->VoucherCode;
             // Merchnat Id serves the purpose of usename to log into the services
             // and also serves as merchant Id which is passed as arugument during
             // redeeming via services
             request.merchantId =  Payment->VoucherMerchantID;
             request.User = Payment->VoucherMerchantID;
             // Transaction Amount needs to be converted to decimal form
             request.voucherAmount =  FormatFloat("0.00", Payment->GetPayTendered());
             Currency transactionAmount = 0;
             if((PaymentTransaction.Type == eTransPartialPayment) ||(PaymentTransaction.Type == eTransSplitPayment))
             {
                  TItemComplete *Order = (TItemComplete *)PaymentTransaction.Orders->Items[0];
                  std::set <__int64> TabKeys;
                  TabKeys.insert(Order->TabKey);
                  TList *Orders = new TList;
                  TDBOrder::GetOrdersFromTabKeys(PaymentTransaction.DBTransaction,Orders,TabKeys);
                  if(Orders->Count > 0)
                  {
                      for(int itemsCount = 0 ; itemsCount < Orders->Count ; itemsCount++)
                      {
                          TItemComplete *Order = (TItemComplete *)Orders->Items[itemsCount];
                          Order->RunBillCalculator();
                          transactionAmount += Order->BillCalcResult.FinalPrice;
                      }
                  }
                  request.transactionAmount = FormatFloat("0.00", transactionAmount);
             }
             else
             {
                request.transactionAmount = FormatFloat("0.00", PaymentTransaction.Money.GrandTotal);
             }
             request.merchantReference = Payment->ReferenceNumber;
             // Universal User serves as Terminal ID of the POS for Redemption
             request.terminalId = Payment->UniVoucherUser;
             request.Password = Payment->UniVoucherPass;
             UnicodeString result         = "";
             RedemptionResponse redemptionResponse;
             redemptionResponse = Voucher->PrepareRedeemRequest(request);
             if(redemptionResponse.status.CompareIC("Successful") == 0)
             {
                Payment->Result = eAccepted;
                if(Payment->Properties & ePayTypeDispPVMsg)
                {
                   MessageBox(redemptionResponse.responseMessage,"Pocket Voucher Accepted", MB_OK + MB_ICONINFORMATION);
                }
             }
             else
             {
                Payment->Result = eFailed;
                if(redemptionResponse.responseMessage.Length() != 0)
                {
                    MessageBox(redemptionResponse.responseMessage + "\n" + "For further assistance, please contact:" + "\n" +
                   "help@pocketvouchers.com" ,"Pocket Voucher Rejected" , MB_OK + MB_ICONERROR);
                }
                // Response when Services were not hit
                else
                {
                    MessageBox("Could not process transaction" ,"Communication Error" , MB_OK + MB_ICONERROR);
                }
             }
         }
         else if((TDeviceRealTerminal::Instance().PocketVouchers->URL == "http://pos.host.pocketvouchers.com/menumate.aspx") || (TDeviceRealTerminal::Instance().PocketVouchers->URL == ""))
         {
               std::auto_ptr<TXMLVoucher> Voucher (new TXMLVoucher(Payment->ReferenceNumber));
               if(PaymentTransaction.CreditTransaction)
               {
                Voucher->SetTransactionType(_T("1"));
               }
               else
               {
                Voucher->SetTransactionType(_T("0"));
               }
               Voucher->SetVoucherCode(Payment->VoucherCode);
                Voucher->SetTenderAmount(FormatFloat("0.00", Payment->GetPayTendered()));
               Currency transactionAmount = 0;
               if((PaymentTransaction.Type == eTransPartialPayment) ||(PaymentTransaction.Type == eTransSplitPayment))
               {
                  TItemComplete *Order = (TItemComplete *)PaymentTransaction.Orders->Items[0];
                  std::set <__int64> TabKeys;
                  TabKeys.insert(Order->TabKey);
                  TList *Orders = new TList;
                  TDBOrder::GetOrdersFromTabKeys(PaymentTransaction.DBTransaction,Orders,TabKeys);

                  if(Orders->Count > 0)
                  {
                      for(int itemsCount = 0 ; itemsCount < Orders->Count ; itemsCount++)
                      {
                          TItemComplete *Order = (TItemComplete *)Orders->Items[itemsCount];
                          Order->RunBillCalculator();
                          transactionAmount += Order->BillCalcResult.FinalPrice;
                      }
                  }
                  Voucher->SetSaleAmount(FormatFloat("0.00", transactionAmount));
               }
               else
               {
                  Voucher->SetSaleAmount(FormatFloat("0.00", PaymentTransaction.Money.GrandTotal));
               }
                Voucher->SetMerchantID(Payment->VoucherMerchantID);
                Voucher->SetLoyaltyID(PaymentTransaction.Membership.Member.MembershipNumber);

               XML_HTTP->Process(URL,80,*Voucher.get());

               Voucher->Parse();
               if(Voucher->Result == eXMLAccepted)
               {
                  Payment->Result = eAccepted;
                  Payment->ExternalReferenceNumber = Voucher->GetPVTransCode();
                  if(Payment->Properties & ePayTypeDispPVMsg)
                  {
                     MessageBox(Voucher->ResultText,"Pocket Voucher Accepted", MB_OK + MB_ICONINFORMATION);
                  }
               }
               else
               {
                    Payment->Result = eFailed;
                    if(Voucher->ResultText.Pos("No Result Received") != 0)
                    {
                        MessageBox(Voucher->ResultText /*+"\n"+Voucher->ContactNumber*/,"Pocket Voucher Rejected" , MB_OK + MB_ICONERROR);
                    }
                    else
                    {
                        MessageBox(Voucher->ResultText + "\n" + "For further assistance, please contact:" + "\n" +
                       "help@pocketvouchers.com "/*+"\n"+Voucher->ContactNumber*/,"Pocket Voucher Rejected" , MB_OK + MB_ICONERROR);
                    }
               }
         }
         else
         {
             // MessageBox("The Url for Pocket Voucher is not correct" ,"Communication Error" , MB_OK + MB_ICONERROR);
             MessageBox("Payment type for Pocket Vouchers is not fully configured. Please check the PV setup" ,"Communication Error" , MB_OK + MB_ICONERROR);
         }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}