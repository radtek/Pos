//---------------------------------------------------------------------------

#ifndef EftposSyncroH
#define EftposSyncroH

#include "EFT2000_OCX.h"
#include "EFT2000_TLB.h"
#include <OleServer.hpp>

#include "Eftpos.h"
//---------------------------------------------------------------------------
class TEftPosSyncro : public TEftPos
{
   protected :
		TPOSInterface *SyncEftPos;

		void __fastcall TransactionEvent(TObject *Sender,long lUniqueID, long lResult);

		void __fastcall TEftPosSyncro::EndPrintBanner(TObject *Sender,
      					 ReceiptTypeEnum eReceiptType);
		void __fastcall TEftPosSyncro::StartPrintBanner(TObject *Sender,
  	      				 ReceiptTypeEnum eReceiptType);
	public:
      TEftPosSyncro();
      ~TEftPosSyncro();
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
		void __fastcall DoSettlementCutover();
		void __fastcall DoSettlementEnquiry();
		void __fastcall DoPrintLocalTotals();
		void __fastcall DoLogon();
		void __fastcall DoUtilityFunctions();		
      void __fastcall ReprintReceipt();
      bool IsCashOutSupported();
};

#endif
