//---------------------------------------------------------------------------


#pragma hdrstop

#include "WalletPaymentsInterface.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


TWalletPaymentsInterface::TWalletPaymentsInterface()
{
    bool useWSDL = false;
    AnsiString serviceURL = "";
    walletPaymentsClient = GetIWCFServiceWalletPayments(useWSDL, serviceURL, NULL );
}
//---------------------------------------------------------------------------
TWalletPaymentsInterface::~TWalletPaymentsInterface()
{
//
}
//---------------------------------------------------------------------------
TWalletTransactionResponse TWalletPaymentsInterface::DoTransaction(TPayment &Payment)
{
    try
    {
       WalletActionResponse* walletActionResponse;
       CoInitialize(NULL);
       WalletAccount* walletAccount = new WalletAccount();
       WalletTransaction* walletTransaction = new WalletTransaction();
       CreateWalletAccountInfo(walletAccount,Payment);
       CreateWalletTransactionInfo(walletTransaction,Payment);

       if(Payment.GetPay() > 0)
            walletActionResponse = walletPaymentsClient->DoPurchaseTransaction(walletAccount, walletTransaction);
       else
            walletActionResponse = walletPaymentsClient->DoRefundTransaction(walletAccount, walletTransaction);

       delete walletAccount;
       delete walletTransaction;
       if(!walletActionResponse->ResponseSuccessful)
       {
         CreateErrorResponse(walletActionResponse->ResponseMessage);
       }
       return CreateResponse(walletActionResponse);
    }
    catch( Exception& exc )
    {
        return CreateErrorResponse(exc.Message);
    }
}

void TWalletPaymentsInterface::CreateWalletAccountInfo(WalletAccount* walletAccount,TPayment &Payment)
{
    walletAccount->WalletType = Payment.WalletType;
    walletAccount->MerchentId = Payment.MerchentId;
    walletAccount->TerminalId = Payment.TerminalId;
    walletAccount->Password = Payment.WalletPassword;
    walletAccount->UserName = Payment.WalletUserName;
    walletAccount->SignKey = Payment.WalletSecurityToken;
}

void TWalletPaymentsInterface::CreateWalletTransactionInfo(WalletTransaction* walletTransaction,TPayment &Payment)
{
    walletTransaction->ScannedCode = Payment.WalletQrCode;
    walletTransaction->Amount = Payment.GetPayTendered();
    walletTransaction->ReferenceNumber = Payment.ReferenceNumber;
}


//---------------------------------------------------------------------------
TWalletTransactionResponse TWalletPaymentsInterface::CreateResponse(WalletActionResponse* inWalletActionResponse)
{
  TWalletTransactionResponse response;
  response.ResponseMessage = inWalletActionResponse->ResponseMessage;
  response.IsSuccessful = inWalletActionResponse->ResponseSuccessful;
  response.OrderReference = inWalletActionResponse->OrderId;
  response.ExpiresIn = inWalletActionResponse->ExpiresIn;
  response.SecurityToken = inWalletActionResponse->SecurityToken;
  return response;
}
//---------------------------------------------------------------------------
TWalletTransactionResponse TWalletPaymentsInterface::CreateErrorResponse(AnsiString inWalletActionResponse)
{
  TWalletTransactionResponse response;
  response.ResponseMessage = inWalletActionResponse;
  response.IsSuccessful = false;
  return response;
}
//---------------------------------------------------------------------------
