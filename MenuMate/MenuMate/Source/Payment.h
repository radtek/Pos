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

enum ePaymentAttribute
{
    ePayTypeOpensCashDrawer = 1,
    ePayTypeAllowCashOut,          //2
    ePayTypeElectronicTransaction, //3
    ePayTypeCheckAccepted,         //4
    ePayTypeGetVoucherDetails,     //5
    ePayTypeGetCardDetails,        //6
    ePayTypeCustomSurcharge,       //7
    ePayTypeSecure1,               //8
    ePayTypeSecure2,               //9
    ePayTypeSecure3,               //10
    ePayTypeTaxFree,               //11
    ePayTypeReqNote,               //12
    ePayTypeCash,                  //13
    ePayTypeCSV,                   //14
    ePayTypeSurcharge,             //15
    ePayTypeRoomInterface,         //16
    ePayTypePoints,                //17
    ePayTypeCredit,                //18
    ePayTypeIntegratedEFTPOS,      //19
    ePayTypeAllowReversal,         //20
    ePayTypeAllowMANPAN,           //21
    ePayTypeCheckSignature,        //22
    ePayTypeChequeVerify,          //23
    ePayTypePhoenixTransaction,    //24
    ePayTypeInvoiceExport,         //25
    ePayTypeSecondaryPMSExport,    //26
    ePayTypeDispPVMsg,             //27
    ePayTypePocketVoucher,         //28
    ePayTypeChargeToAccount,       //29
    ePayTypeChargeToXero,          //30
    ePayTypeRMSInterface,          //31
    ePayTypeAllowTips,             //32
    ePayTypeClipp,                 //33
};

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
    std::vector<TPaymentTypeGroup> _assignedPaymentGroups;

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

    int DisplayOrder;
    int TabCreditKey;
    int GroupNumber;
    int CSVNumber;
    bool Visible;
    bool CreditTransaction; // EftPos Reversal. Negitives Money Amounts Stored in DB.
    bool SuppressEftPosReceipt;
    bool Export;
    bool RefundPoints;
    bool AutoPopulateBlindBalance;
    double PercentAdjust; // Surcharge applied by using this payment type.
    double TaxRate;
    int TabKey;
    Currency RefundPointsVal;
    Currency AmountAdjust; // Surcharge Amount i.e 15c Cheque Fee.
    Currency RoundTo; // Amount to Round To, 5c 10c etc.
    Currency TipAmount;
    UnicodeString AdjustmentReason;
    AnsiString VoucherCode;
    AnsiString TabName;
    eTransactionResult Result;
    TMMContactInfo InvoiceContact;
    WideString CardType; // when used an eftpos machine (specially dps) this property will be set to actual card type used. Visa, Mastercard, BankCard etc
    UnicodeString EftposTransactionID; // unique eftpos transaction id returned from the eftpos machine
    std::set<int> Properties;
    void Reset();
    void Failed();
    void SetAssignedGroups( std::vector<TPaymentTypeGroup> groups );
    void SetAssignedGroups( TPaymentTypeGroup group );
    bool IsLoyaltyVoucher();
    bool IsLoyaltyGiftCard();
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
    void SetPaymentAttribute(ePaymentAttribute attributeIndex,bool attributeValue = true);
    void AssignPaymentAttribute(std::set<int> inProperties);
    void ClearPaymentAttribute();
    bool GetPaymentAttribute(ePaymentAttribute attributeIndex);
    AnsiString GetPropertyString();
};

#endif
