//---------------------------------------------------------------------------

#ifndef EftposAdyenH
#define EftposAdyenH
#include "Eftpos.h"
#include "AdyenWSDL.h"
#include "DropDown.h"
#include "DropDownVar.h"
//---------------------------------------------------------------------------
enum AdyenRequestType {eAdyenNormalSale = 1, eAdyenRefundSale, eAdyenPingRequest, eAdyenLoginRequest, eAdyenLogOutRequest, eAdyenTransactionStatus};
class TEftposAdyen : public TEftPos
{
   protected :
		void __fastcall TransactionEvent(TObject *Sender);

 private:
        // Private Members
        void initAdyenClient();
        Envelop* GetPingTerminalEnvelop();
        _di_IAdyenIntegrationWebService AdyenClient;
        ResourceDetails* GetResourceDetails();
        SaleToPOIResponse* TriggerSaleTransaction(Currency AmtPurchase, UnicodeString TxnRef);
        SaleToPOIResponse* TriggerRefundTransaction(Currency AmtPurchase, UnicodeString TxnRef);
        bool  PingTerminal(eEFTTransactionType TxnType);
        Envelop* GetSaleEnvelop(Currency AmtPurchase, AdyenRequestType requestType, UnicodeString TxnRef);
        MessageHeader* GetMessageHeader(AdyenRequestType requestType, UnicodeString TxnRef);
        bool GetResponseStatus(eEFTTransactionType TxnType, SaleToPOIResponse *response);
        Envelop* GetLoginLogOutEnvelop(AdyenRequestType requestType);
        void LoadEftPosReceipt(ArrayOfstring receipt);
        void LoadEftPosReceiptSecond(ArrayOfstring receipt);
        SaleToPOIResponse* TriggerEnquiry(Currency AmtPurchase, UnicodeString TxnRef);
        Envelop* GetEnquiryEnvelop(Currency AmtPurchase, AdyenRequestType requestType, UnicodeString TxnRef);
        void PrintEFTPOSReceipt(std::auto_ptr<TStringList> &eftPosReceipt);
public:
        TEftposAdyen();
        ~TEftposAdyen();
        void Initialise();
        void DoControlPannel();

        void ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase,
                            Currency AmtCash, UnicodeString TxnRef,
                            ePANSource PANSource, UnicodeString CardSwipe,
                            int ExpiryMonth, int ExpiryYear);

        void ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
                            AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef = "");

        bool CheckChequeBranch(AnsiString Branch);
        bool CheckChequeSerial(AnsiString ChequeSerial);
        bool CheckChequeAccount(AnsiString ChequeAccount);

        void CancelTransaction(AnsiString inID){};
        void __fastcall DoSettlementCutover();
        void __fastcall DoSettlementEnquiry();
        void __fastcall DoLogon();
        void __fastcall ReprintReceipt();
        void __fastcall DoLogOut();
        AnsiString GetRefNumber();
        bool IsCashOutSupported();
};
#endif
