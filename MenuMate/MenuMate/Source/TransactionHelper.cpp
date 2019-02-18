//---------------------------------------------------------------------------


#pragma hdrstop

#include "TransactionHelper.h"
#include "MMLogging.h"
#include "GlobalSettings.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
/* Intention for creating this class is to make code clean gradually
   This class can be used as helper with limited functionality as
   per the data provided through arguments.
   The class deals primarily with TPaymentTransaction prinicpally from
   TListPaymentSystem, TSelectDish and TBillgroup
*/
//---------------------------------------------------------------------------
bool TTransactionHelper::CheckRoomPaytypeWhenFiscalSettingEnable(TPaymentTransaction PaymentTransaction)
{
	bool retVal = true;
    try
    {
        if(TGlobalSettings::Instance().IsFiscalPostingDisable)
        {

            if(IsPaymentDoneForFiscal(PaymentTransaction))
            {
               retVal = false;
            }
        }
    }
    catch(Exception &err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, err.Message);
        retVal = true;
    }
	return retVal;
}
//-----------------------------------------------------------------------
bool TTransactionHelper::IsPaymentDoneForFiscal(TPaymentTransaction paymentTransaction)
{
    bool retVal = false;
    try
    {
        for (int i = 0; i < paymentTransaction.PaymentsCount(); i++)
        {
            TPayment *payment = paymentTransaction.PaymentGet(i);
            if(payment->GetPaymentAttribute(ePayTypeRoomInterface) && payment->GetPayTendered() != 0)
            {
                retVal = true;
                break;
            }
        }
    }
    catch(Exception &err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, err.Message);
        retVal = true;
    }
    return retVal;
}
//-----------------------------------------------------------------------
bool TTransactionHelper::IsAnyDiscountApplied(TPaymentTransaction &paymentTransaction)
{
    try
    {
        for(int index = 0 ; index < paymentTransaction.Orders->Count ; index++)
        {
            TItemComplete *itemComplete = (TItemComplete*)paymentTransaction.Orders->Items[index];

            BillCalculator::DISCOUNT_RESULT_LIST::iterator drIT = itemComplete->BillCalcResult.Discount.begin();

            for( ; drIT != itemComplete->BillCalcResult.Discount.end(); drIT++ )
            {
                return true;
            }
        }
    }
    catch(Exception &err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, err.Message);
    }
    return false;
}
//-----------------------------------------------------------------------
bool TTransactionHelper::IsNormalPrintRequiredWithFiscal(TReceipt *receipt, TPaymentTransaction paymentTransaction)
{
    return (((IsAnyDiscountApplied(paymentTransaction) && TGlobalSettings::Instance().PrintSignatureWithDiscountSales) &&
            (( paymentTransaction.Type != eTransQuickSale && receipt->AlwaysPrintReceiptTenderedSales ) ||
            (receipt->AlwaysPrintReceiptCashSales &&  paymentTransaction.Type == eTransQuickSale) ||
            (receipt->AlwaysPrintReceiptDiscountSales) ||
            (receipt->AlwaysPrintReceiptTenderedSales && receipt->AlwaysPrintReceiptCashSales )))
            ||
            ((receipt->AlwaysPrintReceiptTenderedSales || (receipt->AlwaysPrintReceiptDiscountSales &&
            IsAnyDiscountApplied(paymentTransaction))) && TGlobalSettings::Instance().PrintSignatureWithRoomSales &&
            (IsPaymentDoneWithParamPaymentType(paymentTransaction, ePayTypeRMSInterface) ||
            IsPaymentDoneWithParamPaymentType(paymentTransaction, ePayTypeRoomInterface))));
}
//-----------------------------------------------------------------------
bool TTransactionHelper::IsPaymentDoneWithParamPaymentType(TPaymentTransaction &paymentTransaction, ePaymentAttribute attributeIndex)
{
    bool retVal = false;
    try
    {
        for (int i = 0; i < paymentTransaction.PaymentsCount(); i++)
        {
            TPayment *payment = paymentTransaction.PaymentGet(i);
            if(payment->GetPaymentAttribute(attributeIndex) && payment->GetPayTendered() != 0)
            {
                retVal = true;
                break;
            }
        }
    }
    catch(Exception &err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, err.Message);
        retVal = true;
    }
    return retVal;
}
//--------------------------------------------------------------------------
bool TTransactionHelper::IsSiHotConfigured()
{
    try
    {
        return (TGlobalSettings::Instance().PMSType == SiHot &&
            TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress.Trim() != "" &&
            TDeviceRealTerminal::Instance().BasePMS->POSID != 0 &&
            TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount.Trim() != "");
    }
    catch(Exception &err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, err.Message);
    }
    return false;
}
//--------------------------------------------------------------------------
bool TTransactionHelper::IsOracleConfigured()
{
    try
    {
        return (TGlobalSettings::Instance().PMSType == Oracle &&
            TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress.Trim() != "" &&
            TDeviceRealTerminal::Instance().BasePMS->Slots.size() > 0 &&
            TDeviceRealTerminal::Instance().BasePMS->RevenueCodesMap.size() > 0 &&
            (TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory.Trim() != "" && TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory != NULL)&&
            (TDeviceRealTerminal::Instance().BasePMS->PointsCategory.Trim() != "" && TDeviceRealTerminal::Instance().BasePMS->PointsCategory != NULL) &&
            (TDeviceRealTerminal::Instance().BasePMS->CreditCategory.Trim() != "" && TDeviceRealTerminal::Instance().BasePMS->CreditCategory != NULL));
    }
    catch(Exception &err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, err.Message);
    }
    return false;
}
//--------------------------------------------------------------------------
bool TTransactionHelper::IsMewsConfigured()
{
    try
    {
        return (TGlobalSettings::Instance().PMSType == Mews &&
        TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress.Trim() != "" &&
        TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress.Trim() != 0  &&
        TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount.Trim() != "" &&
        TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount.Trim() != 0 &&
        TDeviceRealTerminal::Instance().BasePMS->RevenueCentre.Trim() != "" &&
        TDeviceRealTerminal::Instance().BasePMS->RevenueCentre.Trim() != 0 &&
        TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount.Trim() != "" &&
        TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount.Trim() != 0 &&
        TDeviceRealTerminal::Instance().BasePMS->PointsCategory.Trim() != "" &&
        TDeviceRealTerminal::Instance().BasePMS->PointsCategory.Trim() != 0 &&
        TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount.Trim() != "" &&
        TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount.Trim() != 0 &&
        TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount.Trim() != "" &&
        TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount.Trim() != 0 &&
        TDeviceRealTerminal::Instance().BasePMS->CreditCategory.Trim() != "" &&
        TDeviceRealTerminal::Instance().BasePMS->CreditCategory.Trim() != 0 &&
        TGlobalSettings::Instance().OracleInterfaceIPAddress.Trim() != "" &&
        TGlobalSettings::Instance().OracleInterfaceIPAddress.Trim() != 0 &&
        TDeviceRealTerminal::Instance().BasePMS->TipAccount.Trim() != "");
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    return false;
}
