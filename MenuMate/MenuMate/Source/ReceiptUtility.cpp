//---------------------------------------------------------------------------


#pragma hdrstop

#include "ReceiptUtility.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//-----------------------------------------------------------------------------
AnsiString TReceiptUtility::ModifyInvoiceNumber(AnsiString inInvoiceNumber,int size)
{
    AnsiString invoiceNumber = "";
    if(StrToInt(TGlobalSettings::Instance().ReceiptDigits) > 0)
    {
        invoiceNumber = LeftPadString( inInvoiceNumber, "0",
                                                          StrToInt(TGlobalSettings::Instance().ReceiptDigits));
    }
    else
        invoiceNumber = LeftPadString( inInvoiceNumber, "0", size);

    return invoiceNumber;
}
//-----------------------------------------------------------------------------
UnicodeString TReceiptUtility::LeftPadString(UnicodeString inString, UnicodeString inChar, int strLen)
{
	for(int i = inString.Length(); i < strLen; i++)
	inString = inChar + inString;
	return inString;
}

//-----------------------------------------------------------------------------
bool TReceiptUtility::IsCancelTransaction(TPaymentTransaction PaymentTransaction)
{
    bool retValue = true;
    for(int i = 0; i < PaymentTransaction.Orders->Count;i++)
    {
        if(((TItemComplete*)PaymentTransaction.Orders->Items[i])->OrderType != CanceledOrder)
        {
           retValue = false;
           break;
        }
    }
    if(PaymentTransaction.Orders->Count == 0)
        retValue = false;
    return retValue;
}
//-----------------------------------------------------------------------------
bool TReceiptUtility::CheckRefundCancelTransaction(TPaymentTransaction PaymentTransaction)
{
    bool retValue =  ((PaymentTransaction.Orders->Count > 0) && PaymentTransaction.CreditTransaction)
         || IsCancelTransaction(PaymentTransaction);
    return retValue;
}
//-----------------------------------------------------------------------------
AnsiString TReceiptUtility::ExtractInvoiceNumber(AnsiString &invoiceNumber)
{
    AnsiString prefix = "";
    if(invoiceNumber.Pos("RV ") != 0)
    {
        invoiceNumber = invoiceNumber.SubString(4,invoiceNumber.Length()-3);
        prefix = "RV ";
    }
    else if(invoiceNumber.Pos("NC ") != 0)
    {
        invoiceNumber = invoiceNumber.SubString(4,invoiceNumber.Length()-3);
        prefix = "NC ";
    }
    else if(invoiceNumber.Pos("Comp ") != 0)
    {
        invoiceNumber = invoiceNumber.SubString(6,invoiceNumber.Length()-5);
        prefix = "Comp ";
    }
    else if(invoiceNumber.Pos("L-") != 0)
    {
        invoiceNumber = invoiceNumber.SubString(3,invoiceNumber.Length()-2);
        prefix = "L-";
    }
    return prefix;
}
//-----------------------------------------------------------------------------

