//---------------------------------------------------------------------------


#pragma hdrstop

#include "PanasonicModels.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

void TPanasonicModels::SetStoreId(UnicodeString storeId)
{
    _storeId = storeId;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetTerminalId(UnicodeString terminalId)
{
    _terminalId = terminalId;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetOperatorId(UnicodeString operatorId)
{
    _operatorId = operatorId;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetOperatorName(UnicodeString operatorName)
{
    _operatorName = operatorName;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetCustomerId(UnicodeString customerId)
{
    _customerId = customerId;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetCustomerName(UnicodeString customerName)
{
    _customerName = customerName;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetTransactionId(UnicodeString transactionId)
{
    _transactionId = transactionId;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetTransactionType(UnicodeString transactionType)
{
    _transactionType = transactionType;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetProductListId(LongInt productListId)
{
    _productListId = productListId;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetTenderType(UnicodeString tenderType)
{
    _tenderType = tenderType;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetTransactionAmount(double transactionAmount)
{
    _transactionAmount = transactionAmount;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetVoidAmount(double voidAmount)
{
    _voidAmount = voidAmount;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetRefundAmount(double refundAmount)
{
    _refundAmount = refundAmount;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetTenderCash(double tenderCash)
{
    _tenderCash = tenderCash;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetSuspended(bool suspended)
{
    _suspended = suspended;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetCashOut(bool cashOut)
{
    _cashOut = cashOut;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetAgeRestricted(bool ageRestricted)
{
    _ageRestricted = ageRestricted;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetStartTime(TDateTime startTime)
{
    _startTime = startTime;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetTimeZoneOfST(TDateTime timeZoneOfST)
{
    _timeZoneOfST = timeZoneOfST;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetDayLightTimeOfST(TDateTime dayLightTimeOfST)
{
    _dayLightTimeOfST = dayLightTimeOfST;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetEndTime(TDateTime endTime)
{
    _endTime = endTime;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetTimeZoneOfET(TDateTime timeZoneOfET)
{
    _timeZoneOfET = timeZoneOfET;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetDayLightTimeOfET(TDateTime dayLightTimeOfET)
{
    _dayLightTimeOfET = dayLightTimeOfET;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetCashFlag(bool cash)
{
    _cash = cash;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetCreditCardFlag(bool creditCard)
{
    _creditCard = creditCard;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetChequeFlag(bool cheque)
{
    _cheque = cheque;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetEFTPOSFlag(bool eFTPOS)
{
    _eFTPOS = eFTPOS;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetGiftCardFlag(bool giftCard)
{
    _giftCard = giftCard;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetIncomeSupportFlag(bool incomeSupport)
{
    _incomeSupport = incomeSupport;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetClubCardFlag(bool clubCard)
{
    _clubCard = clubCard;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetCanceledOrderFlag(bool canceledOrder)
{
    _canceledOrder = canceledOrder;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetPurchaseFlag(bool purchase)
{
    _purchase = purchase;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetOperatorSignOnFlag(bool operatorSignOn)
{
    _operatorSignOn = operatorSignOn;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetOperatorSignOffFlag(bool operatorSignOff)
{
    _operatorSignOff = operatorSignOff;
}
//---------------------------------------------------------------------------
void TPanasonicModels::SetLastReceipt(UnicodeString lastrecipt)
{
    _lastReceipt = lastrecipt;
}
//---------------------------------------------------------------------------
TPanasonicModels::TPanasonicModels()
{
}
void TPanasonicItemList::SetProductCode(UnicodeString productCode)
{
    _productCode = productCode;
}
//---------------------------------------------------------------------------
void TPanasonicItemList::SetProductDescription(UnicodeString productDescription)
{
    _productDescription = productDescription;
}
//---------------------------------------------------------------------------
void TPanasonicItemList::SetInternalTransactionId(LongInt internalTransactionId)
{
    _internalTransactionId = internalTransactionId;
}
//---------------------------------------------------------------------------
void TPanasonicItemList::SetUnitPrice(double unitPrice)
{
    _unitPrice = unitPrice;
}
//---------------------------------------------------------------------------
void TPanasonicItemList::SetQuantity(double quantity)
{
    _quantity = quantity;
}
//---------------------------------------------------------------------------
void TPanasonicItemList::SetAmount(double amount)
{
    _amount = amount;
}
//---------------------------------------------------------------------------
void TPanasonicItemList::SetWeight(double weight)
{
    _weight = weight;
}
//---------------------------------------------------------------------------
void TPanasonicItemList::SetPriceInquiry(bool priceInquiry)
{
    _priceInquiry = priceInquiry;
}
//---------------------------------------------------------------------------
void TPanasonicItemList::SetVoid(bool cancel)
{
    _void = cancel;
}
//---------------------------------------------------------------------------
void TPanasonicItemList::SetStaffPurchase(bool staffPurchase)
{
    _staffPurchase = staffPurchase;
}
//---------------------------------------------------------------------------
void TPanasonicItemList::SetRefund(bool refund)
{
    _refund = refund;
}
//---------------------------------------------------------------------------
void TPanasonicItemList::SetTrainingMode(bool trainingMode)
{
    _trainingMode = trainingMode;
}
//---------------------------------------------------------------------------
TPanasonicItemList::TPanasonicItemList()
{
}
void TPanasonicProduct::SetProductCode(UnicodeString productCode)
{
    _productCode = productCode;
}
//---------------------------------------------------------------------------
void TPanasonicProduct::SetProductDescription(UnicodeString productDescription)
{
    _productDescription = productDescription;
}
//---------------------------------------------------------------------------
TPanasonicProduct::TPanasonicProduct()
{
}
//---------------------------------------------------------------------------
void TPanasonicTransactionDBServerInformation::SetPosSystemType(UnicodeString posSystemType)
{
    _posSystemType = posSystemType;
}
//---------------------------------------------------------------------------
void TPanasonicTransactionDBServerInformation::SetPosSystemName(UnicodeString posSystemName)
{
    _posSystemName = posSystemName;
}
//---------------------------------------------------------------------------
void TPanasonicTransactionDBServerInformation::SetPosSystemVersion(UnicodeString posSystemVersion)
{
    _posSystemVersion = posSystemVersion;
}
//---------------------------------------------------------------------------
void TPanasonicTransactionDBServerInformation::SetTransactioDBServerType(UnicodeString transactionDBServerType)
{
    _transactioDBServerType = transactionDBServerType;
}
//---------------------------------------------------------------------------
void TPanasonicTransactionDBServerInformation::SetTransactionDBServerName(UnicodeString transactionDBServerName)
{
    _transactionDBServerName = transactionDBServerName;
}
//---------------------------------------------------------------------------
void TPanasonicTransactionDBServerInformation::SetTransactionDBServerVersion(UnicodeString transactionDBServerVersion)
{
    _transactionDBServerVersion = transactionDBServerVersion;
}
//---------------------------------------------------------------------------
TPanasonicTransactionDBServerInformation::TPanasonicTransactionDBServerInformation()
{
}
