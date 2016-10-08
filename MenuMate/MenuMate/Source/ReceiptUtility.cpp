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
void TReceiptUtility::CustomizeReceiptNo(TReqPrintJob *PrintJob,TPrintFormat *pPrinter)
{
}
//-----------------------------------------------------------------------------
void TReceiptUtility::ShowRefundReference(TReqPrintJob *PrintJob,TPrintFormat *pPrinter)
{
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
    bool empty;
    if (PrintJob->ReceiptFooter->Count == 0)
    {
        empty = true;
    }
    else
    {
        pPrinter->Line->Columns[0]->Text = TGlobalSettings::Instance().SaveVoidFooter;
        pPrinter->AddLine();
    }
    return empty;
}
//-----------------------------------------------------------------------------
void TReceiptUtility::ModifyInvoiceNumber(TReqPrintJob *PrintJob,TPrintFormat *pPrinter,int size)
{
    if(StrToInt(TGlobalSettings::Instance().ReceiptDigits) > 0)
        pPrinter->Line->Columns[0]->Text += LeftPadString( PrintJob->Transaction->InvoiceNumber, "0",
                                                          StrToInt(TGlobalSettings::Instance().ReceiptDigits));
    else
        pPrinter->Line->Columns[0]->Text += LeftPadString( PrintJob->Transaction->InvoiceNumber, "0", size);
}
//-----------------------------------------------------------------------------
UnicodeString TReceiptUtility::LeftPadString(UnicodeString inString, UnicodeString inChar, int strLen)
{
	for(int i = inString.Length(); i < strLen; i++)
	inString = inChar + inString;
	return inString;
}
//-----------------------------------------------------------------------------

