//---------------------------------------------------------------------------

#ifndef PaymentH
#define PaymentH
//---------------------------------------------------------------------------

#include "MMContactInfo.h"
#include "enumPayment.h"
#include "RMSRoom.h"
#include "PaymentTypeGroup.h"
#include "NewBookRoom.h"

#define CHANGE   	"Change"
#define CASH   	"Cash"
#define ROOMCHARGE 	"Room Charge"
#define CREDIT   	"Credit"
#define TIPS "Tips"
#define CLIPP "Clipp"

// Used for Archiving the bills payment types.
#define ePayTypeOpensCashDrawer        0x01
#define ePayTypeAllowCashOut           0x02
#define ePayTypeElectronicTransaction  0x04
#define ePayTypeCheckAccepted          0x08
#define ePayTypeGetVoucherDetails      0x10
#define ePayTypeGetCardDetails         0x20
#define ePayTypeCustomSurcharge        0x40
#define ePayTypeSecure1                0x80
#define ePayTypeSecure2                0x100
#define ePayTypeSecure3                0x200
#define ePayTypeTaxFree                0x400
#define ePayTypeReqNote                0x800
#define ePayTypeCash                   0x1000
#define ePayTypeCSV                    0x2000
#define ePayTypeSurcharge              0x4000
#define ePayTypeRoomInterface          0x8000
#define ePayTypePoints                 0x10000
#define ePayTypeCredit                 0x20000
#define ePayTypeIntegratedEFTPOS       0x40000
#define ePayTypeAllowReversal          0x80000
#define ePayTypeAllowMANPAN            0x100000
#define ePayTypeCheckSignature         0x200000
#define ePayTypeChequeVerify           0x400000
#define ePayTypePhoenixTransaction     0x800000
#define ePayTypeInvoiceExport          0x1000000
#define ePayTypeSecondaryPMSExport     0x2000000
#define ePayTypeDispPVMsg         	   0x4000000
#define ePayTypePocketVoucher    	   0x8000000
#define ePayTypeChargeToAccount	       0x10000000
#define ePayTypeChargeToXero	       0x20000000
#define ePayTypeRMSInterface	       0x40000000
#define ePayTypeAllowTips	       	   0x80000000
#define ePayTypeClipp                  0x100000000

class TPaymentTransaction;

class TPayment
{
private:
   Currency FPay;
   Currency FPayRounding; // CashIn.
   Currency FCashOut;
   Currency FCashOutRounding;
   Currency FChange;
   Currency FAdjustment;
   Currency FAdjustmentRounding;
   std::vector<TPaymentTypeGroup> _assignedPaymentGroups;    // assigned payment groups for this payment type
   // CashIn.
public:
   TPayment(TPaymentTransaction *inOwner = NULL);
   TPaymentTransaction *Owner;
   UnicodeString Name;
   UnicodeString Note;
   UnicodeString NameOveride;
   UnicodeString ReferenceNumber;
   UnicodeString ExternalReferenceNumber;
   UnicodeString FixedVoucherCode;
   UnicodeString VoucherMerchantID;
   UnicodeString SysNameOveride;
   UnicodeString PaymentThirdPartyID;
   UnicodeString UniVoucherURL;
   UnicodeString UniVoucherUser;
   UnicodeString UniVoucherPass;
   UnicodeString UniVoucherToken;
   UnicodeString GLCode;
   /* Used for the RMS room interface as the location to read and write csv files*/
   UnicodeString CVSReadLocation;
   UnicodeString CVSWriteLocation;
   /*Holds the RMS Room Info*/
   TRMSRoom RMSRoom;
   TNewBookRoom NewBookRoom;
   AnsiString SecondaryPMSIPAddress;
   int SecondaryPMSPortNumber;
   int Colour;
   int Properties;
   int DisplayOrder;
   int TabCreditKey;
   int GroupNumber;
   int CSVNumber;
   bool Visible;
   bool CreditTransaction; // EftPos Reversal. Negitives Money Amounts Stored in DB.
   bool SuppressEftPosReceipt;
   bool Export;
   bool RefundPoints;
   Currency RefundPointsVal;

   double PercentAdjust; // Surcharge applied by using this payment type.
   // i.e 15% amex charge.
   Currency AmountAdjust; // Surcharge Amount i.e 15c Cheque Fee.
   UnicodeString AdjustmentReason;
   AnsiString VoucherCode;

   eTransactionResult Result;
   double TaxRate;
    int TabKey;
    AnsiString TabName;
   TMMContactInfo InvoiceContact;

   Currency RoundTo; // Amount to Round To, 5c 10c etc.

   void Reset();
   void Failed();
   void operator = (const TPayment & Data);
   void __fastcall SetPay(Currency value);
   void __fastcall SetAdjustment(Currency value);
   void __fastcall SetChange(Currency value);
   void __fastcall SetCashOut(Currency value);
   void __fastcall SetRefundPointsValue(Currency value);
   Currency __fastcall GetPay();
   Currency __fastcall GetCashOut();
   Currency __fastcall GetPayTendered();
   Currency __fastcall GetPayRounding();
   Currency __fastcall GetChange();
   Currency __fastcall GetPayTotal();
   Currency __fastcall GetRefundPointsValue();

   Currency __fastcall GetAdjustment();
   Currency __fastcall GetAdjustmentRounding();

   Currency __fastcall GetSurcharge();
   Currency __fastcall GetSurchargeExcGST();
   Currency __fastcall GetSurchargeGSTContent();

   Currency __fastcall GetDiscountExcGST();
	Currency __fastcall GetDiscountGSTContent();

   Currency __fastcall GetSurchargeRounding();
   Currency __fastcall GetSurchargeTotal();

   Currency __fastcall GetDiscount();
   Currency __fastcall GetDiscountRounding();
   Currency __fastcall GetDiscountTotal();

   Currency __fastcall GetCashOutTotal();
   Currency __fastcall GetCashOutRounding();
   Currency __fastcall GetPayCashOutTotal();
   Currency __fastcall GetRoundingTotal();

   std::vector<TPaymentTypeGroup> GetAssignedGroups() const;
   TPaymentTypeGroup GetFirstAssignedGroup();
   void SetAssignedGroups( std::vector<TPaymentTypeGroup> groups );
   void SetAssignedGroups( TPaymentTypeGroup group );

   WideString CardType; // when used an eftpos machine (specially dps) this property will be set to actual card type used. Visa, Mastercard, BankCard etc
   UnicodeString EftposTransactionID; // unique eftpos transaction id returned from the eftpos machine
   bool IsLoyaltyVoucher();
   bool IsLoyaltyGiftCard();
   Currency TipAmount;
};

#endif
