//---------------------------------------------------------------------------


#pragma hdrstop

#include "WalletPaymentsInterface.h"

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
       walletActionResponse = walletPaymentsClient->DoTransaction(walletAccount, walletTransaction);
       return CreateResponse(walletActionResponse);
    }
    catch( Exception& exc )
    {
        return CreateErrorResponse(exc.Message);
    }
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
