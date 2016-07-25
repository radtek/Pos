//---------------------------------------------------------------------------

#ifndef ThorlinkDataObjectsH
#define ThorlinkDataObjectsH
#include <memory>
#include <vector>
#include <Classes.hpp>
//---------------------------------------------------------------------------
enum TenderTypeThor
{
    eThorCash,
    eThorDebitCard,
    eThorPoints,
    eThorVoucher,
    eThorCheque
};
enum ThorResponseCodes
{
    eTransactionSuccessful = 0,
    eTransactionInSAF = -5,
    eTransactionFailedValidation = -6,
    eOnlineStatusRequired = -7 ,
    eRefundError = 25
};
struct TItemDetailsThor
{
    UnicodeString thirdPartyCode;
    double unitPrice;
    double qty;
};
struct TTenderDetails
{
    int tenderType;
    UnicodeString tenderIdentifier;
    double tenderValue;
    UnicodeString cardNo;
    bool sendTransactionValue;
};
struct TPurchaseResponse
{
    int code;
    UnicodeString responseMessage;
    UnicodeString cardNumber;
    double creditValue;
    double transactionValue;
    double loyaltyValue;
    UnicodeString transactionNumber;
};
struct TRefundTransaction
{
    UnicodeString cardNumber;
    double creditValue;
    double transactionValue;
    double loyaltyValue;
};
#endif
