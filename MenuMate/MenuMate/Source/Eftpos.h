//---------------------------------------------------------------------------

#ifndef EftposH
#define EftposH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>

#include "ListPaymentSystem.h"
//---------------------------------------------------------------------------
enum eEFTPOSType
{
  eTEftPosUnknown,
  eTEftPosANZ,
  eTEftPosIngenico,
  eTEftPosSyncro,
  eTEftPosCadmus,
  eTEftPosICELink,
  eTEftPosDPS,
  eTEftPosCadmusCronos,
};

enum TEftPosEvent {ePrintReceiptEvent,eChequeVerifyEvent,
						 eEftPosEventEnd};

enum eEFTTransactionType
{
  TransactionType_NONE = 0,
  TransactionType_PURCHASE = 1,
  TransactionType_CASH_ADVANCE = 2,
  TransactionType_PURCHASE_PLUS_CASH = 3,
  TransactionType_REFUND = 4,
  TransactionType_INQUIRY = 5,
  TransactionType_TIP = 6,
  TransactionType_QR_Merchant = 7,
  TransactionType_QR_Refund = 8
};

enum ePANSource
{
  PANSource_None = 0,
  PANSource_Manual = 1,
  PANSource_CardReader = 2,
};

class TEftPosTransaction
{
public:
	TEftPosTransaction(AnsiString inID,eEFTTransactionType inTransType);
	AnsiString UniqueID;
	eEFTTransactionType TransType;
	eTransactionResult Result;
	AnsiString ResultText;
	bool SuppressReceipt;
	bool EventCompleted;
	WideString CardType;
	AnsiString EftposTransactionID;
    AnsiString FinalAmount;
    bool TimeOut;
    AnsiString TipAmount;
    AnsiString SurchargeAmount;
};

class TListEftPosTransContainer : public TList
{
	public:
	void __fastcall Clear();
	void __fastcall Delete(TEftPosTransaction *inEftPosTrans);
	void __fastcall Delete(AnsiString UniqueID);
	int __fastcall Add(TEftPosTransaction *inEftPosTrans);
	TEftPosTransaction *GetByID(AnsiString UniqueID);
	TEftPosTransaction *Get(int Index);
	TEftPosTransaction *Get(eEFTTransactionType inTransType);
	__fastcall virtual ~TListEftPosTransContainer();
};

class TEftPos
{
   protected :
		bool fEnabled;
      void __fastcall SetEnabled(bool value);
      bool __fastcall GetEnabled();
		bool AllowTimeOut;
		bool AllowsTipping;
		std::vector<AnsiString> TippableCardTypes;
	private:
		TListEftPosTransContainer *EftPosTransContainer;
	public:
	__property bool Enabled  = { read=GetEnabled, write=SetEnabled };
	std::auto_ptr<TStringList> LastEftPosReceipt;	
	eEFTPOSType EFTPOSType;
   TEftPos();
   ~TEftPos();
   virtual void Initialise();

	virtual void DoControlPannel() = 0;
    virtual void ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase,
											Currency AmtCash, UnicodeString TxnRef,
											ePANSource PANSource, UnicodeString CardSwipe,
                                 int ExpiryMonth, int ExpiryYear) = 0;

	virtual void ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
			   						AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef = "") = 0;
	virtual void __fastcall DoSettlementCutover() = 0;
	virtual void __fastcall DoSettlementEnquiry() = 0;
	virtual void __fastcall ReprintReceipt() = 0;
	virtual AnsiString GetRefNumber() = 0;
	virtual bool WaitOnEftPosEvent(AnsiString ReferenceNumber);

	virtual bool CheckChequeBranch(AnsiString Branch) = 0;
	virtual bool CheckChequeSerial(AnsiString ChequeSerial) = 0;
	virtual bool CheckChequeAccount(AnsiString ChequeAccount) = 0;
	virtual void CancelTransaction(AnsiString inID) = 0;

   virtual bool CheckOverLimitExceeded(Currency Total);

	AnsiString ResultText;
	AnsiString WaitingOnReferenceNumber;
    AnsiString AcquirerRefSmartPay;
    AnsiString AcquirerRefSmartConnect;
    AnsiString AcquirerRefAdyen;
	int ChequeAccountMaxLength;
	int ChequeBranchMaxLength;
	int ChequeSerialMaxLength;

   Currency OverLimitAmount;

	void SetTransactionEvent(AnsiString inID,eEFTTransactionType inTransType);
	TEftPosTransaction *GetTransactionEvent(AnsiString inID);
	TEftPosTransaction *GetTransactionEvent(eEFTTransactionType inTransType);
	TEftPosTransaction *TEftPos::GetTransactionEventInWaiting();
	TEftPosTransaction *TEftPos::GetTransactionEvent();
	void DelTransactionEvent(AnsiString inID);

	//Supported Features default to true.
	bool EnquiryEnabled;

	// checks whether en eftpos supports tip transactions. default is false
	virtual bool AllowsTipsOnTransactions();
	virtual void ProcessTip(WideString OriginalDpsTxnRef, Currency OriginalAmount, Currency TipAmount, UnicodeString MerchantRef );

	std::vector<AnsiString> GetTippableCardTypes();
    virtual bool IsCashOutSupported();
};

extern TEftPos *EftPos;

#endif
