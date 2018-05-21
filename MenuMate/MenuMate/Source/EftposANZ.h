//---------------------------------------------------------------------------

#ifndef EftposANZH
#define EftposANZH

#include <OleCtrls.hpp>

#include "Eftpos.h"
#include "ENZEFTLib_OCX.h"

//---------------------------------------------------------------------------
// EFTPOS NZ LTD
class TEftPosANZ : public TEftPos
{
   protected :
		TENZEFT *OcxEftPos;
		void __fastcall PrintReceiptEvent(TObject *Sender, BSTR ReceiptType);
		void __fastcall TransactionEvent(TObject *Sender);
		void __fastcall ChequeVerifyEvent(TObject *Sender);
		void __fastcall GetLastTransactionEvent(TObject *Sender);
		void __fastcall GetLastReceiptEvent(TObject *Sender);
		void __fastcall GetMerchantsEvent(TObject *Sender);
   public:
   TEftPosANZ();
   ~TEftPosANZ();
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
	void __fastcall ReprintReceipt();
   AnsiString GetRefNumber();
   bool IsCashOutSupported();
};

#endif
