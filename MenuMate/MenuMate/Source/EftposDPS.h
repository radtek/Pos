//---------------------------------------------------------------------------

#ifndef EftposDPSH
#define EftposDPSH
//---------------------------------------------------------------------------

#include "Eftpos.h"

#include "DPSEFTXLib_OCX.h"

//---------------------------------------------------------------------------
class TEftPosMMDPS : public TEftPos
{
	private:
		void PopulateTippableCardTypes();
   protected :
		TDpsEftX *EftPosDPS;

		void __fastcall TransactionEvent(TObject *Sender);
		void __fastcall PrintBanner(TObject *Sender);
      void __fastcall DoMaintenance();
	  void __fastcall DoConfiguration();
	  void __fastcall EditTenderEvent(TObject *Sender);

	  // this member will hold the original reference number that was used to add a tip amount
	  AnsiString DpsEftposTipOwnerRefNumber;
	public:
      TEftPosMMDPS();
      ~TEftPosMMDPS();
      void Initialise();
      void DoControlPannel();

      void ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase,Currency AmtCash,
									 UnicodeString TxnRef,ePANSource PANSource,
                            UnicodeString CardSwipe,int ExpiryMonth, int ExpiryYear);

      void ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
							 AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef = "");

      bool CheckChequeBranch(AnsiString Branch);
      bool CheckChequeSerial(AnsiString ChequeSerial);
      bool CheckChequeAccount(AnsiString ChequeAccount);

		void CancelTransaction(AnsiString inID){};

		AnsiString GetRefNumber();
  		int MerchantNumber;
      
		void __fastcall DoSettlementCutover();
		void __fastcall DoSettlementEnquiry();
		void __fastcall DoPrintLocalTotals();
		void __fastcall DoLogon();
		void __fastcall DoUtilityFunctions();		
	  void __fastcall ReprintReceipt();

	  bool AllowsTipsOnTransactions();
	  void ProcessTip(WideString OriginalDpsTxnRef, Currency OriginalAmount, Currency TipAmount, UnicodeString MerchantRef);
      bool IsCashOutSupported();
};

#endif
