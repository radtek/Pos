//---------------------------------------------------------------------------

#ifndef EFTPOSPaymentSenseH
#define EFTPOSPaymentSenseH
#include "Eftpos.h"
#include "DropDown.h"
#include "DropDownVar.h"
//---------------------------------------------------------------------------
class TEftPosPaymentSense : public TEftPos
{
 private:
        void InitPaymentSenseClient();
public:
        TEftPosPaymentSense();
        ~TEftPosPaymentSense();
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
};
#endif