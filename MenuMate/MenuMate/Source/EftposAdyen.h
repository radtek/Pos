//---------------------------------------------------------------------------

#ifndef EftposAdyenH
#define EftposAdyenH
#include "Eftpos.h"
#include "AdyenWSDL.h"
#include "DropDown.h"
#include "DropDownVar.h"
//---------------------------------------------------------------------------
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
//        bool  PingTerminal(eEFTTransactionType TxnType);
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
        bool  PingTerminal(eEFTTransactionType TxnType);
        bool IsCashOutSupported();
};
#endif
