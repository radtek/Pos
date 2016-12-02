#ifndef ListPaymentSystemH
#define ListPaymentSystemH

#include <Dateutils.hpp>
#include "PaymentTransaction.h"
#include "enum.h"
#include "Money.h"
#include <set>
#include <map>
#include <DBClient.hpp>
#include "SystemEvents.h"
#include "SplitMoney.h"
#include "ItemMinorComplete.h"
#include "ItemComplete.h"
#include "XeroIntegration.h"
#include "XeroInvoiceBuilder.h"
#include <functional>
#include "MMPaymentSystem.h"
#include "PaymentTypeGroup.h"
#include "DBContacts.h"
#include "DrinkCommandData.h"
#include "ThorlinkDataObjects.h"
#include "CaptureCustomerDetails.h"
//#include "ThorlinkClient.h"

class TReqPrintJob;
class TItemComplete;
class TListSecurityRefContainer;

class TListPaymentSystem : public TMMPaymentSystem
{
	public:

	bool Busy;
	bool ForceTender;
	bool FTBypassElecTranTyp;
	bool ForceReceiptWithCashContent;
	bool EftPosEnabled;
    bool isClippTabFullyPaid;
    bool isPaymentProcessed;
    bool splittedDivisionLeft;
    bool IsClippSale;
	TSystemEvents OnAfterTransactionComplete;
	TReqPrintJob *LastReceipt;
	Currency LastTotal,LastChange;
	int CurrentSourceTable;
	TMMContactInfo TempDestUserInfo;
    Currency Pts;
    Currency BillPts;
    UnicodeString MemberNo;
    UnicodeString Card_Id;
    Currency RefundPts;
    Currency Points;
    double PtsRefund , PtsBuy;
    AnsiString PointsBuy;
    int positionCash;
    int positionPoints;
    int positionVoucher;
     TDateTime TrasactionDate;
    std::vector<TTenderDetails> tenderDetailsList;
    std::vector<TItemDetailsThor> itemsList;
    TListPaymentSystem();
    virtual __fastcall ~TListPaymentSystem();

	void CreateXeroInvoiceAndSend( TPaymentTransaction &inPaymentTransaction );
	bool SendInvoiceToXero( TXeroInvoice* inXeroInvoice );

	void PaymentsLoadTypes(TPaymentTransaction &PaymentTransaction);   // From DB.
    void LoadMemberPaymentTypes(TPaymentTransaction &PaymentTransaction);
	void PaymentsReload(TPaymentTransaction &PaymentTransaction);
    void PaymentsLoadTypes(Database::TDBTransaction &DBTransaction, std::vector<TPayment> &Payments);

	void PaymentLoad(Database::TDBTransaction &DBTransaction,TStringList *List);
	void PaymentLoad(Database::TDBTransaction &DBTransaction,int PaymentKey,TPayment &Payment);
	void PaymentSave(Database::TDBTransaction &DBTransaction,int &PaymentKey,TPayment &Payment);
	void PaymentDelete(Database::TDBTransaction &DBTransaction,int PaymentKey);

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
	bool SplitPayment(TPaymentTransaction &PaymentTransaction,TSplitMoney *SplitMoney, double &SplitPercentage,bool &DelayAll);
	void AdjustCredit(TPaymentTransaction &PaymentTransaction);

	bool ProcessTransaction(TPaymentTransaction &PaymentTransaction, bool isRecovery = false, bool isClippPayment = false);

	int GetPartialPayment(TPaymentTransaction &PaymentTransaction,double &SplitPercentage,Currency &RequestedTotal);

	// Eftpos Functions.
	void ProcessRecoveryInfo();
    void ProcessRewardSchemes(TPaymentTransaction & PaymentTransaction);
    bool ProcessTipOnVisaTransaction(int arcBillKey, WideString paymentRefNumber, Currency OriginalAmount, Currency tipAmount);
	bool AllowsTipsOnTransactions();
	std::vector<AnsiString> GetTippableCardTypes();
     int GetPaymentTabName(Database::TDBTransaction &DBTransaction,AnsiString PAYMENT_NAME);
    void LoadClippPaymentTypes(TPaymentTransaction &PaymentTransaction);
    bool PrepareThorRequest(TPaymentTransaction &paymentTransaction);
    AnsiString CreateFilename(TDateTime date);
     UnicodeString CreateTextFile(AnsiString LocalPath,AnsiString value);
     void SaveIntVariable(vmVariables vmVar, int CompName);
        void SaveCompValueinDBStrUnique(vmVariables vmVar, UnicodeString CompName);
     //void GetDLFMallInfo(TItemComplete &ItemComplete);
     void GetDLFMallCMDCodeFirst(AnsiString invoiceNumber,AnsiString fileStatus) ;
     void GetDLFMallCMDCodeSec(TPaymentTransaction &paymentTransaction);
      void GetDLFMallCMDCodeThird(TItemComplete *ItemComplete,AnsiString catkey,Currency discount);
       void GetDLFMallCMDCodeSubOrderThird(TItemCompleteSub *ItemComplete,AnsiString catkey,Currency discount);

     void GetDLFMallCMDCodeForth(TPaymentTransaction &paymentTransaction);
    void GetDLFMallCMDCodeFifth(TPaymentTransaction &PaymentTransaction)  ;

protected:

	std::auto_ptr<TManagerReference> ManagerReference;

    void _processOrderSetTransaction(TPaymentTransaction &PaymentTransaction);
    void _processSplitPaymentTransaction( TPaymentTransaction &PaymentTransaction );
    void _processPartialPaymentTransaction( TPaymentTransaction &PaymentTransaction );
    void _processQuickTransaction( TPaymentTransaction &PaymentTransaction );
    void _processCreditTransaction( TPaymentTransaction &PaymentTransaction );
    void _processEftposRecoveryTransaction( TPaymentTransaction &PaymentTransaction );
    void _processRewardsRecoveryTransaction( TPaymentTransaction &PaymentTransaction );

    TMMProcessingState _createProcessingStateMessage();
    bool _isSmartCardPresent();

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
    void ArchiveOrderTaxes(Database::TDBTransaction &DBTransaction,int ArchiveKey,TItemMinorComplete *Order);
	void ArchiveWebOrders(TPaymentTransaction &PaymentTransaction, long ArcBillKey);
	bool PaymentsRemoveSingleOrder(TItemComplete * Order);
	void ClearDiscounts(TPaymentTransaction &PaymentTransaction);
	void PreparePayments(TPaymentTransaction &Transaction);
	void StoreInfo(TPaymentTransaction &PaymentTransaction);
	void ProcessSecurity(TPaymentTransaction &PaymentTransaction);
	bool ProcessComfirmationDialogs(TPaymentTransaction &PaymentTransaction);
	void ReturnOrderedItemsToStock( TPaymentTransaction &transaction );
	void PerformPostTransactionOperations( TPaymentTransaction &PaymentTransaction );

	//ApplyRounding(TPaymentTransaction &PaymentTransaction);
	void UpdateFreebieRewards(TPaymentTransaction &PaymentTransaction);
    void SetLastVisit(TPaymentTransaction &PaymentTransaction);
    void FormatSpendChit(TPaymentTransaction &PaymentTransaction);
    void PrintSpendChit(TStringList *Docket);
    bool ProcessCSVRoomExport( TPaymentTransaction &inPaymentTransaction );

   // tip related functions
    void InsertOrUpdateTipTransactionRecordToDB(int arcBillKey, Currency tipAmount, WideString originalPaymentRef);

    // loads up the payment groups for a given payment with its db key
    void loadPaymentTypeGroupsForPaymentType( int paymentDbKey, TPayment &payment );

    // for cash payments only. loads up the cash payment group
    TPaymentTypeGroup getDefaultCashPaymentGroup();

    // can be used to export current transaction to required formats
    void exportTransactionInformation( TPaymentTransaction &paymentTransaction );

    // calls TMallExportUpdateAdaptor to update Mall Export tables
    void updateMallExportTables( TPaymentTransaction &paymentTransaction );

    // generates transaction export file based on the mall export table
    void generateTransactionExportFile();
    void CalculateTierLevel(TPaymentTransaction &PaymentTransaction);
    void UpdateCachedRewardPoints(TPaymentTransaction &PaymentTransaction);
    bool IsAllItemSelected(TPaymentTransaction &PaymentTransaction);
    void DelayAllPayments(TPaymentTransaction &PaymentTransaction);

    bool ProcessCSVNewBookExport( TPaymentTransaction &inPaymentTransaction );

   private:
     bool MakePatronCountZero;
     bool sessionStartedAlready;
     bool skipPaymentFormDeletion;
     bool isSCDOrPWDApplied;
     SCDPWDCustomerDetails customerDetails;
     void CheckPatronByOrderIdentification(TPaymentTransaction &inPaymentTransaction);
     bool CheckForCard(TPaymentTransaction &PaymentTransaction);
     bool ProcessLoyaltyVouchers(TPaymentTransaction &PaymentTransaction);
     bool PrepareThorPurchaseRequest(TPaymentTransaction &paymentTransaction);
     bool PrepareThorRefundRequest(TPaymentTransaction &paymentTransaction);
     void ExportReceipt(TStringList *StringReceipt,TPaymentTransaction &PaymentTransaction);
     bool CaptureSCDOrPWDCustomerDetails(TPaymentTransaction &PaymentTransaction);
     bool IsSCDOrPWDApplied(TPaymentTransaction &PaymentTransaction);
     void InsertSCDOrPWDCustomerDetails(long arcbillKey);
};

#endif
