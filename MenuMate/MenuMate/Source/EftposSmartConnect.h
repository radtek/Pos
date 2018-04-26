//---------------------------------------------------------------------------

#ifndef EftposSmartConnectH
#define EftposSmartConnectH
#include <OleCtrls.hpp>
#include "Eftpos.h"
#include "SmartConnectWSDL.h"
#include "DropDown.h"
#include "DropDownVar.h"
//---------------------------------------------------------------------------
class TEftPosSmartConnect : public TEftPos
{
   protected :
		void __fastcall TransactionEvent(TObject *Sender);

 private:
        // Private Members
        _di_IWCFSmartConnect smartConnectClient;
        void InitSmartConnectClient();
        TransactionTypes *transactionType;
        void InitializeProperties();
        PairingTerminal  *pairingTerminal;
        void AuthoriseTransaction();
        void FinalizeTransaction();
        void GetTransResult();
        void GetStatus();
        void ReadCard();
        void SmartPayReceipt(SmartConnectResponse *wcfResponse) ;
public:
        TEftPosSmartConnect();
        ~TEftPosSmartConnect();
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
        void __fastcall DoPairing();
        AnsiString GetRefNumber();
        bool DoQRCodeTransaction(TPayment &Payment);

};
#endif
