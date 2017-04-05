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
       MessageBox("Wallet Payment Initiated", "Error", MB_OK + MB_ICONINFORMATION);
       WalletActionResponse* walletActionResponse;
       CoInitialize(NULL);
       WalletAccount* walletAccount = new WalletAccount();
       WalletTransaction* walletTransaction = new WalletTransaction();
       CreateWalletAccountInfo(walletAccount,Payment);
       CreateWalletTransactionInfo(walletTransaction,Payment);
       walletActionResponse = walletPaymentsClient->DoTransaction(walletAccount, walletTransaction);
       MessageBox("Wallet Payment Finished", "Error", MB_OK + MB_ICONINFORMATION);
       delete walletAccount;
       delete walletTransaction;
       return CreateResponse(walletActionResponse);
    }
    catch( Exception& exc )
    {
        MessageBox(AnsiString(exc.Message), "Error", MB_OK + MB_ICONINFORMATION);
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
