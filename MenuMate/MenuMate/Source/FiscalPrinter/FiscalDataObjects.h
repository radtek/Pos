//---------------------------------------------------------------------------

#ifndef FiscalDataObjectsH
#define FiscalDataObjectsH
#include <memory>
#include <vector>
#include <Classes.hpp>
//---------------------------------------------------------------------------
struct TFiscalItemDetails
{
    UnicodeString TableNo;
    UnicodeString GuestName;
    UnicodeString ChitNumber;
    UnicodeString MemberName;
    UnicodeString PartyName;
    UnicodeString ItemDescription;
    UnicodeString SizeName;
    UnicodeString Quantity;
    UnicodeString ItemCategory;
    UnicodeString PricePerUnit;
    UnicodeString PriceTotal;
    UnicodeString VATPercentage;
    UnicodeString ServiceCharge;
    UnicodeString ServiceChargeTax;
    UnicodeString Note;
};

struct TFiscalPaymentDetails
{
    UnicodeString Type;
    UnicodeString Amount;
    UnicodeString Billno;
    UnicodeString Cashno;
    UnicodeString Cashier;
    UnicodeString Source;
    UnicodeString Description;
    UnicodeString PaymentSurcharge;
    UnicodeString TipAmount;
    UnicodeString Change;
    UnicodeString TipAppliedFromPOS;
};

struct TFiscalDiscountDetails
{
    UnicodeString Type;
    UnicodeString Amount;
    UnicodeString Description;
    UnicodeString DiscountGroup;
    UnicodeString DiscountMode;
    int DiscountKey;
};

struct TFiscalBillDetails
{
    UnicodeString InvoiceNumber;
    std::vector<TFiscalItemDetails> ItemList;
    std::vector<TFiscalPaymentDetails> PaymentList;
    std::vector<TFiscalDiscountDetails> DiscountList;
    UnicodeString Date;
    UnicodeString Time;
    UnicodeString Billno;
    UnicodeString Cashno;
    UnicodeString Cashier;
    UnicodeString Source;
    UnicodeString TerminalName;
    UnicodeString PointPurchased;
    UnicodeString TabCredit;
    UnicodeString PrinterType;
    UnicodeString PrinterLogicalName;
    UnicodeString SaleType;
};

struct TFiscalPrinterResponse
{
    bool IsSuccessful;
    AnsiString ResponseMessage;
};
#endif
