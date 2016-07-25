#ifndef PaymentsH
#define PaymentsH


#include "enum.h"
#include "Money.h"
#include <set>
#include <map>
#include <DBClient.hpp>
#include "SystemEvents.h"
#include "SplitMoney.h"
#include "PaymentTransaction.h"

class TReqPrintJob;
class TItemComplete;
class TListPaymentSystem;
class TListSecurityRefContainer;

class TListPaymentSystem
{
	public:
	TListPaymentSystem();
	virtual __fastcall ~TListPaymentSystem();

	bool Busy;
	bool ForceTender;
	bool FTBypassElecTranTyp;
	bool ForceReceiptWithCashContent;
	bool EftPosEnabled;

	void PaymentsLoadTypes(TPaymentTransaction &PaymentTransaction);   // From DB.
	void PaymentsReload(TPaymentTransaction &PaymentTransaction);

	void PaymentLoad(Database::TDBTransaction &DBTransaction,TStringList *List);
	void PaymentLoad(Database::TDBTransaction &DBTransaction,int PaymentKey,TPayment &Payment);
	void PaymentSave(Database::TDBTransaction &DBTransaction,int &PaymentKey,TPayment &Payment);
	void PaymentDelete(Database::TDBTransaction &DBTransaction,int PaymentKey);

	TSystemEvents OnAfterTransactionComplete;
	TReqPrintJob *LastReceipt;
	Currency LastTotal,LastChange;

	TListSecurityRefContainer *Security;
   void SaveToFileCSV(TPaymentTransaction &PaymentTransaction);
	void Initialise(Database::TDBTransaction &DBTransaction);
	void RemoveTabs(TPaymentTransaction &PaymentTransaction);
	void OpenCashDrawer(TPaymentTransaction &PaymentTransaction);
	void ReceiptPrint(TPaymentTransaction &PaymentTransaction,bool RequestEFTPOSReceipt, bool CloseAndPrint = false);
	void ReceiptPrepare(TPaymentTransaction &PaymentTransaction,bool &RequestEFTPOSReceipt);
	void SetInvoiceNumber(TPaymentTransaction &PaymentTransaction);
	void Reset(TPaymentTransaction &PaymentTransaction);
	void RemoveOrders(TPaymentTransaction &PaymentTransaction);
	bool ProcessThirdPartyModules(TPaymentTransaction &PaymentTransaction,bool &RequestEFTPOSReceipt);
	bool SplitPayment(TPaymentTransaction &PaymentTransaction,TSplitMoney *SplitMoney, double &SplitPercentage);
	void AdjustCredit(TPaymentTransaction &PaymentTransaction);
	bool ProcessTransaction(TPaymentTransaction &PaymentTransaction);
	int GetPartialPayment(TPaymentTransaction &PaymentTransaction,double &SplitPercentage,Currency &RequestedTotal);
	// Eftpos Functions.
	void ProcessRecoveryInfo();
   void ProcessRewardSchemes(TPaymentTransaction & PaymentTransaction);
private:

	std::auto_ptr<TManagerReference> ManagerReference;

	AnsiString RecoveryFilePayments;
	AnsiString RecoveryFileOrders;
	AnsiString RecoveryFilePaymentSystem;
	AnsiString RecoveryFileSecurity;

	void TransRetriveElectronicResult(TPaymentTransaction &PaymentTransaction,TPayment *Payment);
   void TransRetriveInvoiceResult(TPaymentTransaction &PaymentTransaction,TPayment *Payment);
	bool TransRetrivePhoenixResult(TPaymentTransaction &PaymentTransaction);
	bool BuildXMLTransaction(TPaymentTransaction &PaymentTransaction);   
	void GetChequeVerifyResult(TPayment *Payment);

	void PaymentsArchiveReceipt(TMoney *CurrentMoney,TReqPrintJob *Request,eTransactionType SalesType);
	void ArchiveReferences(TPaymentTransaction &PaymentTransaction,long ArcBillKey);
   void ArchiveRewards(TPaymentTransaction &PaymentTransaction,long ArcBillKey);   
	void ArchiveTransaction(TPaymentTransaction &PaymentTransaction);
	long ArchiveBill(TPaymentTransaction &PaymentTransaction);
	void ArchivePatronCount(TPaymentTransaction &PaymentTransaction,long ArcBillKey);
	void ArchiveOrder(TPaymentTransaction &PaymentTransaction,long ArcBillLK);
   void ArchivePoints(TPaymentTransaction &PaymentTransaction);
   void ArchiveOrderDiscounts(Database::TDBTransaction &DBTransaction,int ArchiveKey,TItemMinorComplete *Order);   
	bool PaymentsRemoveSingleOrder(TItemComplete * Order);
	void ClearDiscounts(TPaymentTransaction &PaymentTransaction);
	void PreparePayments(TPaymentTransaction &Transaction);
	void StoreInfo(TPaymentTransaction &PaymentTransaction);
	void ProcessSecurity(TPaymentTransaction &PaymentTransaction);
	bool ProcessComfirmationDialogs(TPaymentTransaction &PaymentTransaction);
	// Eftpos Functions.
	void ClearRecoveryInfo();
	void CreateRecoveryInfo();
	bool LoadRecoveryInfo(TPaymentTransaction &RecoverTransaction);
	void SaveRecoveryInfo(TPaymentTransaction &PaymentTransaction);
	//ApplyRounding(TPaymentTransaction &PaymentTransaction);
	void UpdateFreebieRewards(TPaymentTransaction &PaymentTransaction);
   void SetLastVisit(TPaymentTransaction &PaymentTransaction);
   void FormatSpendChit(TPaymentTransaction &PaymentTransaction);
   void PrintSpendChit(TStringList *Docket);         
};

#endif
