//---------------------------------------------------------------------------

#ifndef EftposSmartLinkH
#define EftposSmartLinkH
#include <OleCtrls.hpp>
#include "Eftpos.h"
#include "SmartLinkWSDL.h"
#include "DropDown.h"
#include "DropDownVar.h"
//---------------------------------------------------------------------------

class TEftPosSmartLink : public TEftPos
{
   protected :
		void __fastcall TransactionEvent(TObject *Sender);

 private:
        // Private Members
        _di_IWCFServiceSmartLink smartLinkClient;
        void initSmartLinkClient();
        bool  PingTerminal(eEFTTransactionType TxnType);
public:
        TEftPosSmartLink();
        ~TEftPosSmartLink();
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
        AnsiString GetRefNumber();
        bool IsCashOutSupported();
};


#endif
