//---------------------------------------------------------------------------

#ifndef PanasonicModelsH
#define PanasonicModelsH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <DB.hpp>

class TPanasonicModels
{
private:
    UnicodeString _storeId;
    int _terminalId;
    UnicodeString _operatorId;
    UnicodeString _operatorName;
    UnicodeString _customerId;
    UnicodeString _customerName;
    UnicodeString _transactionId;
    UnicodeString _transactionType;
    LongInt _productListId;
    UnicodeString _tenderType;
    double _transactionAmount;
    double _voidAmount;
    double _refundAmount;
    double _tenderCash;
    bool _suspended;
    bool _cashOut;
    bool _ageRestricted;
    TDateTime _startTime;
    TDateTime _timeZoneOfST;
    TDateTime _dayLightTimeOfST;
    TDateTime _endTime;
    TDateTime _timeZoneOfET;
    TDateTime _dayLightTimeOfET;
    bool _cash;
    bool _creditCard;
    bool _cheque;
    bool _eFTPOS;
    bool _giftCard;
    bool _incomeSupport;
    bool _clubCard;
    bool _canceledOrder;
    bool _purchase;
    bool _operatorSignOn;
    bool _operatorSignOff;
    UnicodeString _lastReceipt;
    void SetStoreId(UnicodeString storeId);
    void SetTerminalId(int terminalId);
    void SetOperatorId(UnicodeString operatorId);
    void SetOperatorName(UnicodeString operatorName);
    void SetCustomerId(UnicodeString customerId);
    void SetCustomerName(UnicodeString customerName);
    void SetTransactionId(UnicodeString transactionId);
    void SetTransactionType(UnicodeString transactionType);
    void SetProductListId(LongInt productListId);
    void SetTenderType(UnicodeString tenderType);
    void SetTransactionAmount(double transactionAmount);
    void SetVoidAmount(double voidAmount);
    void SetRefundAmount(double refundAmount);
    void SetTenderCash(double tenderCash);
    void SetSuspended(bool suspended);
    void SetCashOut(bool cashOut);
    void SetAgeRestricted(bool ageRestricted);
    void SetStartTime(TDateTime startTime);
    void SetTimeZoneOfST(TDateTime timeZoneOfST);
    void SetDayLightTimeOfST(TDateTime dayLightTimeOfST);
    void SetEndTime(TDateTime endTime);
    void SetTimeZoneOfET(TDateTime timeZoneOfET);
    void SetDayLightTimeOfET(TDateTime dayLightTimeOfET);
    void SetCashFlag(bool cash);
    void SetCreditCardFlag(bool creditCard);
    void SetChequeFlag(bool cheque);
    void SetEFTPOSFlag(bool eFTPOS);
    void SetGiftCardFlag(bool giftCard);
    void SetIncomeSupportFlag(bool incomeSupport);
    void SetClubCardFlag(bool clubCard);
    void SetCanceledOrderFlag(bool canceledOrder);
    void SetPurchaseFlag(bool purchase);
    void SetOperatorSignOnFlag(bool operatorSignOn);
    void SetOperatorSignOffFlag(bool operatorSignOff);
    void SetLastReceipt(UnicodeString lastrecipt);

public:
    __property UnicodeString StoreId = {read = _storeId, write = SetStoreId};
    __property int Terminald = {read = _terminalId, write = SetTerminalId};
    __property UnicodeString OperatorId = {read = _operatorId, write = SetOperatorId};
    __property UnicodeString OperatorName = {read = _operatorName, write = SetOperatorName};
    __property UnicodeString CustomerId = {read = _customerId, write = SetCustomerId};
    __property UnicodeString CustomerName = {read = _customerName, write = SetCustomerName};
    __property UnicodeString TransactionId = {read = _transactionId, write = SetTransactionId};
    __property UnicodeString TransactionType = {read = _transactionType, write = SetTransactionType};
    __property LongInt ProductListId = {read = _productListId, write = SetProductListId};
    __property UnicodeString TenderType = {read = _tenderType, write = SetTenderType};
    __property double TransactionAmount = {read = _transactionAmount, write = SetTransactionAmount};
    __property double VoidAmount = {read = _voidAmount, write = SetVoidAmount};
    __property double RefundAmount = {read = _refundAmount, write = SetRefundAmount};
    __property double TenderCash = {read = _tenderCash, write = SetTenderCash};
    __property bool Suspended = {read = _suspended, write = SetSuspended};
    __property bool CashOut = {read = _cashOut, write = SetCashOut};
    __property bool AgeRestricted = {read = _ageRestricted, write = SetAgeRestricted};
    __property TDateTime StartTime = {read = _startTime, write = SetStartTime};
    __property TDateTime TimeZoneOfST = {read = _timeZoneOfST, write = SetTimeZoneOfST};
    __property TDateTime DayLightTimeOfST = {read = _dayLightTimeOfST, write = SetDayLightTimeOfST};
    __property TDateTime EndTime = {read = _endTime, write = SetEndTime};
    __property TDateTime TimeZoneOfET = {read = _timeZoneOfET, write = SetTimeZoneOfET};
    __property TDateTime DayLightTimeOfET = {read = _dayLightTimeOfET, write = SetDayLightTimeOfET};
    __property bool Cash = {read = _cash, write = SetCashFlag};
    __property bool CreditCard = {read = _creditCard, write = SetCreditCardFlag};
    __property bool Cheque = {read = _cheque, write = SetChequeFlag};
    __property bool EFTPOS = {read = _eFTPOS, write = SetEFTPOSFlag};
    __property bool GiftCard = {read = _giftCard, write = SetGiftCardFlag};
    __property bool IncomeSupport = {read = _incomeSupport, write = SetIncomeSupportFlag};
    __property bool ClubCard = {read = _clubCard, write = SetClubCardFlag};
    __property bool CanceledOrder = {read = _canceledOrder, write = SetCanceledOrderFlag};
    __property bool Purchase = {read = _purchase, write = SetPurchaseFlag};
    __property bool OperatorSignOn = {read = _operatorSignOn, write = SetOperatorSignOnFlag};
    __property bool OperatorSignOff = {read = _operatorSignOff, write = SetOperatorSignOffFlag};
    __property UnicodeString LastReceipt = {read = _lastReceipt, write = SetLastReceipt};
    TPanasonicModels();
};

class TPanasonicItemList
{
private:
    UnicodeString _productCode;
    UnicodeString _productDescription;
    LongInt _internalTransactionId;
    double _unitPrice;
    double _quantity;
    double _amount;
    double _weight;
    bool _priceInquiry;
    bool _void;
    bool _staffPurchase;
    bool _refund;
    bool _trainingMode;
    void SetProductCode(UnicodeString productCode);
    void SetProductDescription(UnicodeString productDescription);
    void SetInternalTransactionId(LongInt internalTransactionId);
    void SetUnitPrice(double transactionAmount);
    void SetQuantity(double quantity);
    void SetAmount(double amount);
    void SetWeight(double weight);
    void SetPriceInquiry(bool priceInquiry);
    void SetVoid(bool cancel);
    void SetStaffPurchase(bool staffPurchase);
    void SetRefund(bool refund);
    void SetTrainingMode(bool trainingMode);
public:
    __property UnicodeString ProductCode = {read = _productCode, write = SetProductCode};
    __property UnicodeString ProductDescription = {read = _productDescription, write = SetProductDescription};
    __property LongInt InternalTransactionId = {read = _internalTransactionId, write = SetInternalTransactionId};
    __property double UnitPrice = {read = _unitPrice, write = SetUnitPrice};
    __property double Quantity = {read = _quantity, write = SetQuantity};
    __property double Amount = {read = _amount, write = SetAmount};
    __property double Weight = {read = _weight, write = SetWeight};
    __property bool PriceInquiry = {read = _priceInquiry, write = SetPriceInquiry};
    __property bool Void = {read = _void, write = SetVoid};
    __property bool StaffPurchase = {read = _staffPurchase, write = SetStaffPurchase};
    __property bool Refund = {read = _refund, write = SetRefund};
    __property bool TrainingMode = {read = _trainingMode, write = SetTrainingMode};
    TPanasonicItemList();
};

class TPanasonicProduct
{
private:
    UnicodeString _productCode;
    UnicodeString _productDescription;
    void SetProductCode(UnicodeString productCode);
    void SetProductDescription(UnicodeString productDescription);
public:
    __property UnicodeString ProductCode = {read = _productCode, write = SetProductCode};
    __property UnicodeString ProductDescription = {read = _productDescription, write = SetProductDescription};
    TPanasonicProduct();
};

class TPanasonicTransactionDBServerInformation
{
private:
    UnicodeString _posSystemType;
    UnicodeString _posSystemName;
    UnicodeString _posSystemVersion;
    UnicodeString _transactioDBServerType;
    UnicodeString _transactionDBServerName;
    UnicodeString _transactionDBServerVersion;
    void SetPosSystemType(UnicodeString posSystemType);
    void SetPosSystemName(UnicodeString posSystemName);
    void SetPosSystemVersion(UnicodeString posSystemVersion);
    void SetTransactioDBServerType(UnicodeString posTransactionDBServerType);
    void SetTransactionDBServerName(UnicodeString posTransactionDBServerName);
    void SetTransactionDBServerVersion(UnicodeString posTransactionDBServerVersion);
public:
    __property UnicodeString PosSystemType = {read = _posSystemType, write = SetPosSystemType};
    __property UnicodeString PosSystemName = {read = _posSystemName, write = SetPosSystemName};
    __property UnicodeString PosSystemVersion = {read = _posSystemVersion, write = SetPosSystemVersion};
    __property UnicodeString TransactioDBServerType = {read = _transactioDBServerType, write = SetTransactioDBServerType};
    __property UnicodeString TransactionDBServerName = {read = _transactionDBServerName, write = SetTransactionDBServerName};
    __property UnicodeString TransactionDBServerVersion = {read = _transactionDBServerVersion, write = SetTransactionDBServerVersion};
    TPanasonicTransactionDBServerInformation();
};
#endif
