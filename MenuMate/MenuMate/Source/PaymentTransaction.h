//---------------------------------------------------------------------------

#ifndef PaymentTransactionH
#define PaymentTransactionH
//---------------------------------------------------------------------------
#include "ReferenceManager.h"
#include "POS_XMLBase.h"
#include "Money.h"
#include "TabCredit.h"
#include "ContactMemberApplied.h"
#include "SplitMoney.h"
#include "Payment.h"
#include "ChitNumber.h"
#include "PatronType.h"
#include "Discount.h"
#include "Payment.h"
#include "CaptureCustomerDetails.h"
#include "PaySubsUtility.h"


enum eTransactionType {eTab,eTableSeat,eCash,eAccount,eRoomSale,eCredited,eCreditPurchase,eWeb};

enum TPaymentTransactionType {eTransUnknown,
										eTransTabSet, 			// Paying a set of Tabs.
										eTransOrderSet, 		// Paying a set of Orders.
										eTransPickNMix, 		// Pick and Mixing a Set of orders.
										eTransQuickSale, 		// Paying a set of Orders using Preselected Payment type.
										eTransSplitPayment, 	// Partailly paying a set of orders by spliting the total.
										eTransPartialPayment,	// Partailly paying a set of orders by entering an amount to pay.
										eTransCreditPurchase,	// Putting a payment though for No product, ( Tab credit ).
										eTransEFTPOSRecovery,	// Recovering From EFTPOS Transaction.
										eTransRewardsRecovery,	// Recovering From EFTPOS Rewards Transaction.
										eTransInvoicePayment	// Paying set of Invoices
										};

enum TSaleType{ RegularSale,ComplimentarySale,NonChargableSale};

// eNormal Entire Tab / Tabs are being paid
// eSplitPayment Several Tabs are being Combined and Split
// ePickNMix Individual Items are being billed of one or mare tabs.
// eCashSale Quick Cash sale.

typedef std::pair<AnsiString, AnsiString> TCustomerOrder;
typedef int __fastcall (*TListPaymentCompare)(void * Item1, void * Item2);

struct TRedeemPointsInformation
{
    Currency RemainingPoints;
    Currency TotalPoints;
    Currency TotalValue;
};

struct TRedeemPocketVoucherInformation
{
    AnsiString VoucherNumber;
    Currency TotalSaleAmount;
    Currency RedeemedAmount;
};

struct TRedeemGiftVoucherInformation
{
    AnsiString VoucherNumber;
    Currency TotalSaleAmount;
    Currency RedeemedAmount;
    Currency GiftVoucherAmount;
    TDateTime ExpiryDate;
};

class TRewardsTransaction
{
   public:
      TRewardsTransaction() : CardNumber(""),EFTPOSRef(""),Amount(0){};
      UnicodeString CardNumber;
      UnicodeString EFTPOSRef;
      Currency Amount;
};

class TPaymentTransaction
{

  private:
    std::auto_ptr<TList> PaymentList;
    TListPaymentCompare PaymentComp;
    bool IsCopy;	// Used if the copy constructor or assignment operator is used to make sure that the orders in Orders are deleted

    void copyBasicDetailsFrom( const TPaymentTransaction *OtherTransaction );
    void copyOrdersListFrom(TList* OrdersList);
    void copyPaymentsDetails(TList* PaymentsList);
    void SetRedeemPoints(Currency PointsRedeemed);
    void SetRedeemBDayPoints(Currency &PointsRedeemed);
    void SetRedeemFVPoints(Currency &PointsRedeemed);
    void CheckDiscountsWithMembership(TItemMinorComplete *Order);
  public:
        AnsiString PartyName;
        Database::TDBTransaction &DBTransaction;
        void BuildXMLPaymentTypes(TPOS_XMLBase &Data);
        TPaymentTransactionType Type; // eTransTabSet,eTransOrderSet,ePickNMix,eCashSale,eSplitPayment,
        eTransactionType SalesType;
        TMoney Money;
        TSplitMoney SplitMoney;
        TSaleType TypeOfSale;
        TRewardsTransaction RewardsTransaction;
        std::map<long,TTabCredit> TabCredit;
        std::vector<RefRefType> References;
        TList *Orders;
        TChitNumber ChitNumber;
        TContactMemberApplied Membership;
        TContactMemberApplied Account;
        TMMContactInfo StaffMember;
        std::vector<TDiscount> Discounts;
        Currency TotalAdjustment;
        UnicodeString DiscountReason;
        UnicodeString InvoiceNumber;    // Receipt Number
        UnicodeString RefundRefReceipt;  // Refund Reference Receipt Number
        int InvoiceKey;            // Key of invoice this payment pays for if applicable.
        int RoomNumber;
        int BookingID;
        int TimeKey;
        int Redeamedpoint ;
        TCustomerOrder CustomerOrder; //Customer name, OrderType
        Currency RequestPartialPayment;
        bool CreditTransaction;
        bool IsQuickPayTransaction;
        AnsiString QuickPaymentName;
        class TransPhoenixInfo
        {
            public :
                UnicodeString AccountNumber;
                UnicodeString AccountName;
                UnicodeString ReferenceNumber;
                int FolderNumber;
                UnicodeString RoomNumber;
                UnicodeString FirstName;
                UnicodeString LastName;
        } Phoenix;

        std::vector<TPatronType> Patrons;
        std::map<UnicodeString,UnicodeString> MiscPrintData;
        void ProcessPoints();
        TPaymentTransaction(Database::TDBTransaction &inDBTransaction);
        virtual __fastcall ~TPaymentTransaction();
        TPaymentTransaction(const TPaymentTransaction &OtherTransaction);
        TPaymentTransaction& operator=(const TPaymentTransaction &OtherTransaction);
        int PaymentAdd(TPayment *in);
        int PaymentIndex(TPayment *in);
        void PaymentDelete(TPayment *in);
        int PaymentsCount() {return PaymentList->Count;}
        int VisiablePaymentsCount();
        void PaymentsReset();  // Clears all double Values.
        void PaymentsClear();
        void PaymentsSort();
        void __fastcall ApplyDiscounts();
        void DeleteOrders();
        void Recalc();
        void LoadDiscount(int DiscountKey, TDiscountSource DiscountSource);
        void __fastcall ApplyDiscount(TDiscount CurrentDiscount, int ContactKey);
        void ApplyMembership(TMMContactInfo inMember, eMemberSource inMemberSource);
        void ApplyMembership(TContactMemberApplied inMemberApplied);
        void ApplyMembershipDiscounts();
        void RemoveMembership();
        TPayment * PaymentFind(UnicodeString in);
        TPayment* GetRefundPointsPayment();
        int GetRefundPointPaymentIndex();
        TPayment * PaymentFindByProperty(int ePayTypeProperty);
        TPayment * PaymentGet(int Index);
        bool TransElectronicPayment();
        bool TransInvoicePayment();
        bool PhoenixHSPayment();
        bool TransIntegratedEFTPOS();
        bool TransVerifyCheque();
        bool TransOpenCashDraw();
        Currency RecalculateGSTContent();
        void CalculatePatronCountFromMenu();
        TCustomer Customer;
        bool IsBirtdayProcessed;
        int Birthdaypoints;
        bool IsFirstVisitProcessed;
        TRedeemPointsInformation *RedeemPointsInformation;
        TRedeemPointsInformation *RedeemWeightInformation;
        TRedeemPocketVoucherInformation *RedeemPocketVoucherInformation;
        TRedeemGiftVoucherInformation  *RedeemGiftVoucherInformation;
        TRedeemGiftVoucherInformation  *PurchasedGiftVoucherInformation;
        bool HasOrders();			// whether this transaction has orders in it
        void ReCalculateAmounts();	// recalculate TMoney instance
        bool CopyFrom( const TPaymentTransaction *inOtherTransaction );
        TDateTime DeliveryTime;
        int SplittedItemKey;
        int WebOrderKey;
        bool CheckThorVoucherExistAsDiscount(AnsiString voucher_code) ;
        Currency TaxOnClippDiscount;
        Currency ServiceChargeWithTax;
        bool CheckDiscountApplied(TDiscount CurrentDiscount);
        bool IsVouchersProcessed;
        void makeLogFile(UnicodeString str);
        SCDPWDCustomerDetails customerDetails;
        bool IgnoreLoyaltyKey;
        bool WasSavedSales;
};

#endif
