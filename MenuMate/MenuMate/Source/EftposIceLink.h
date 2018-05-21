//---------------------------------------------------------------------------

#ifndef EftposIceLinkH
#define EftposIceLinkH

#include "Eftpos.h"
#include <CPort.hpp>
//---------------------------------------------------------------------------
class TEftPosIceLink : public TEftPos
{
	private :
		TComPort *Port;
		bool Busy;
		bool CloseingPort;
		bool OpeningPort;
		bool OpenPort(int PortNum);
		void ClosePort();

	AnsiString BuildPurchasePacket(Currency Purchase, AnsiString Card,AnsiString TxnRef,AnsiString ManPAN = "", int Month = 00, int Year = 00);
	AnsiString BuildPurchasePlusCashPacket(Currency Purchase,Currency Cash,AnsiString Card,AnsiString TxnRef,AnsiString ManPAN = "", int Month = 00, int Year = 00);
	AnsiString BuildCashOutPacket(Currency CashOut, AnsiString Card,AnsiString TxnRef);
	AnsiString BuildChequeAuthPacket(Currency ChequeAmount, AnsiString BankAndBranch,
												AnsiString AccountNumber,AnsiString ChequeSerialNumber);
	AnsiString BuildLogonPacket();
	AnsiString BuildCancelPacket();
	AnsiString BuildReprintReceiptPacket();
	AnsiString BuildSettlementCutoverPacket();
	AnsiString BuildSettlementEnquiryPacket(TDateTime Date);
	AnsiString BuildRefundPacket(Currency Refund, AnsiString Card,AnsiString TxnRef,AnsiString ManPAN = "", int Month = 00, int Year = 00);

	void SendPacket(AnsiString Data);
	protected :
		void __fastcall TransactionEvent(TObject *Sender,long lUniqueID, long lResult);
	public:
		TEftPosIceLink();
		~TEftPosIceLink();

		int PortNumber;

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
