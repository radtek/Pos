//---------------------------------------------------------------------------

#ifndef WalletPaymentsInterfaceH
#define WalletPaymentsInterfaceH

#include <StdCtrls.hpp>
#include "MMContactInfo.h"
#include "Membership.h"
#include "WalletPaymentsWSDL.h"
#include "Payment.h"
//---------------------------------------------------------------------------

struct TWalletTransactionResponse
{
   public:
   bool IsSuccessful;
   AnsiString ResponseMessage;
   AnsiString OrderReference;
   int ExpiresIn;
   AnsiString SecurityToken;
};

class TWalletPaymentsInterface
{
    private:
        // Private Members
        _di_IWCFServiceWalletPayments walletPaymentsClient; // WCF client
        void CreateWalletAccountInfo(WalletAccount* walletAccount,TPayment &Payment);
        void CreateWalletTransactionInfo(WalletTransaction* walletTransaction,TPayment &Payment);
        TWalletTransactionResponse CreateResponse(WalletActionResponse* inWalletActionResponse);
        TWalletTransactionResponse CreateErrorResponse(AnsiString inWalletActionResponse);
    public:
        // Constructor and Destructor
        TWalletPaymentsInterface();
        ~TWalletPaymentsInterface();
        TWalletTransactionResponse DoTransaction(TPayment &Payment);
};

#endif
