//---------------------------------------------------------------------------


#pragma hdrstop

#include "ReceiptUtility.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


void TReceiptUtility::PrintTaxInvoice(TReqPrintJob *PrintJob, TPrintFormat *pPrinter)
{
    if(!TGlobalSettings::Instance().HideTaxInvoice)
    {
        pPrinter->Line->Columns[0]->Text = "Tax Invoice";
        pPrinter->AddLine();
    }
}
//-----------------------------------------------------------------------------
void TReceiptUtility::PrintVoidOnReceipt(TReqPrintJob *PrintJob,TPrintFormat *pPrinter)
{
        pPrinter->Line->ColCount = 1;
        pPrinter->Line->Columns[0]->Width = pPrinter->Width;
        pPrinter->Line->Columns[0]->Alignment = taCenter;
        pPrinter->Line->Columns[0]->Text = "VOID";
        pPrinter->AddLine();
}
//-----------------------------------------------------------------------------
void TReceiptUtility::ShowRefundReference(TReqPrintJob *PrintJob,TPrintFormat *pPrinter,int  size)
{
    if((PrintJob->Transaction->CreditTransaction||IsCancelTransaction(PrintJob->Transaction)) && TGlobalSettings::Instance().CaptureRefundRefNo)
    {
        pPrinter->Line->ColCount = 1;
        pPrinter->Line->Columns[0]->Width = pPrinter->Width;
        pPrinter->Line->Columns[0]->Alignment = taCenter;
        pPrinter->Line->Columns[0]->Text = "OR No. ";
        ModifyInvoiceNumber(PrintJob->Transaction->RefundRefReceipt,
                                                   pPrinter,size);
        pPrinter->AddLine();
    }
}
//-----------------------------------------------------------------------------
void TReceiptUtility::CustomizeReceiptNoLabel(TReqPrintJob *PrintJob,TPrintFormat *pPrinter)
{
}
//-----------------------------------------------------------------------------
void TReceiptUtility::PrintReceiptHeaderSecond(TReqPrintJob *PrintJob,TPrintFormat *pPrinter)
{

}
//-----------------------------------------------------------------------------
bool TReceiptUtility::PrintReceiptFooterSecond(TReqPrintJob *PrintJob,TPrintFormat *pPrinter)
{
    bool empty = false;
    if((PrintJob->Transaction->CreditTransaction||IsCancelTransaction(PrintJob->Transaction))
                && TGlobalSettings::Instance().SetVoidFooter)
    {
        if (PrintJob->ReceiptVoidFooter->Count == 0)
        {
            empty = true;
        }
        else
        {
            pPrinter->Line->ColCount = 1;
            pPrinter->Line->FontInfo.Bold = false;
            pPrinter->Line->FontInfo.Height = fsNormalSize;
            pPrinter->Line->FontInfo.Width = fsNormalSize;
            pPrinter->Line->Columns[0]->Width = pPrinter->Width;
            pPrinter->Line->Columns[0]->Alignment = taCenter;
            for (int i = 0; i < PrintJob->ReceiptVoidFooter->Count; i++)
            {
                pPrinter->Line->Columns[0]->Text = PrintJob->ReceiptVoidFooter->Strings[i];
                pPrinter->AddLine();
            }
        }
    }
    return empty;
}
//-----------------------------------------------------------------------------
void TReceiptUtility::ModifyInvoiceNumber(AnsiString inInvoiceNumber,TPrintFormat *pPrinter,int size)
{
    if(StrToInt(TGlobalSettings::Instance().ReceiptDigits) > 0)
    {
        pPrinter->Line->Columns[0]->Text += LeftPadString( inInvoiceNumber, "0",
                                                          StrToInt(TGlobalSettings::Instance().ReceiptDigits));
    }
    else
        pPrinter->Line->Columns[0]->Text += LeftPadString( inInvoiceNumber, "0", size);
}
//-----------------------------------------------------------------------------
UnicodeString TReceiptUtility::LeftPadString(UnicodeString inString, UnicodeString inChar, int strLen)
{
	for(int i = inString.Length(); i < strLen; i++)
	inString = inChar + inString;
	return inString;
}
//-----------------------------------------------------------------------------
bool TReceiptUtility::IsCancelTransaction(TPaymentTransaction *PaymentTransaction)
{
    bool retValue = true;
    for(int i = 0; i < PaymentTransaction->Orders->Count;i++)
    {
        if(((TItemComplete*)PaymentTransaction->Orders->Items[i])->OrderType != CanceledOrder)
        {
           retValue = false;
           break;
        }
    }
    return retValue;
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
    return retValue;
}
//-----------------------------------------------------------------------------

