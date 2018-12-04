//---------------------------------------------------------------------------


#pragma hdrstop

#include "AustriaFiscalInterface.h"
#include "DateUtils.hpp"
//---------------------------------------------------------------------------

#pragma package(smart_init)
TAustriaFiscalInterface::TAustriaFiscalInterface()
{
    initAustriaClient();
}
//---------------------------------------------------------------------------
void TAustriaFiscalInterface::initAustriaClient()
{
    bool useWSDL = false;
    AnsiString austriaURL = AnsiString("http://localhost:8748/MenumateServices/AustriaService/");

    AustriaClient = GetIAustriaFiscalIntegrationWebService(
                            useWSDL, austriaURL, NULL );
}
UnicodeString TAustriaFiscalInterface::InitAustriaFiscal(UnicodeString url, UnicodeString cashBoxId)
{
    UnicodeString response = "";
    try
    {
        response = AustriaClient->InitAustriaFiscal(url,cashBoxId);
    }
    catch(Exception &ex)
    {
        response = ex.Message;
    }
    return response;
}
//---------------------------------------------------------------------------
TReceiptResponseAustriaFiscal TAustriaFiscalInterface::PostDataToAustriaFiscal(TReceiptRequestAustriaFiscal receiptRequest)
{
    TReceiptResponseAustriaFiscal response;
    ReceiptReponseLocal* responseLocal;
    try
    {
        ReceiptRequestLocal* receiptAustriaLocal = ApplyAdapterToRequest(receiptRequest);
        CoInitialize(NULL);
        responseLocal = AustriaClient->PostData(receiptAustriaLocal);
        if(responseLocal != NULL)
           response = AppyAdapterToResponse(responseLocal);
    }
    catch(Exception &ex)
    {
        if(responseLocal == NULL)
            response.UnableToConnectToMenumateService = true;
        else if(response.CashBoxID == NULL || response.CashBoxID == "")
            response.UnableToConnectToMenumateService = true;
    }
    return response;
}
//---------------------------------------------------------------------------
ReceiptRequestLocal* TAustriaFiscalInterface::ApplyAdapterToRequest(TReceiptRequestAustriaFiscal receiptRequest)
{
    ReceiptRequestLocal* receiptLocal = new ReceiptRequestLocal();
    try
    {
        receiptLocal->ftCashBoxID = receiptRequest.CashBoxID;
        receiptLocal->TerminalID  = receiptRequest.TerminalID;
        receiptLocal->ftReceiptCase  = receiptRequest.ReceiptCase;
        receiptLocal->ReceiptReference  = receiptRequest.ReceiptReference;
        receiptLocal->ReceiptMoment = new TXSDateTime();
        TDateTime dateTime = Now();
        receiptLocal->ReceiptMoment->AsDateTime  = Dateutils::EncodeDateTime(YearOf(dateTime),
                                                      MonthOf(dateTime),DayOf(dateTime),HourOf(dateTime),MinuteOf(dateTime),
                                                      SecondOf(dateTime),MilliSecondOf(dateTime));



       ArrayOfChargeItemLocal chargeItemsLocal;
       for(std::vector<TChargeItemAustriaFiscal>::iterator itchargeItems = receiptRequest.ChargeItems.begin();
        itchargeItems != receiptRequest.ChargeItems.end(); ++itchargeItems)
        {
           ChargeItemLocal* chargeItemLocal = new ChargeItemLocal;
           chargeItemLocal->Amount = itchargeItems->Amount;
           chargeItemLocal->ChargeItemCase = itchargeItems->ChargeItemCase;
           chargeItemLocal->Description = itchargeItems->Description;
           chargeItemLocal->VATRate = itchargeItems->VATRate;
           chargeItemLocal->Quantity = itchargeItems->Quantity;
           chargeItemsLocal.Length = chargeItemsLocal.Length + 1;
           chargeItemsLocal[chargeItemsLocal.Length - 1] = chargeItemLocal;
        }
       receiptLocal->ChargeItems = chargeItemsLocal;

       ArrayOfPayItemLocal payItemsLocal;
       for(std::vector<TPayItemAustriaFiscal>::iterator itpayItems = receiptRequest.PayItems.begin();
        itpayItems != receiptRequest.PayItems.end(); ++itpayItems)
        {
           PayItemLocal* payItemLocal = new PayItemLocal;
           payItemLocal->Amount = itpayItems->Amount;
           payItemLocal->PayItemCase = itpayItems->PayItemCase;
           payItemLocal->Description = itpayItems->Description;
           payItemLocal->Quantity = itpayItems->Quantity;
           payItemsLocal.Length = payItemsLocal.Length + 1;
           payItemsLocal[payItemsLocal.Length - 1] = payItemLocal;
        }
       receiptLocal->PayItems = payItemsLocal;
    }
    catch(Exception &ex)
    {
    }
    return receiptLocal;
}
//---------------------------------------------------------------------------
TReceiptResponseAustriaFiscal TAustriaFiscalInterface::AppyAdapterToResponse(ReceiptReponseLocal* responseLocal)
{
    TReceiptResponseAustriaFiscal response;
    if(!responseLocal->UnableToConnectToFiscalService)
    {
        response.CashBoxID = responseLocal->CashBoxID;
        response.QueueID = responseLocal->QueueID;
        response.QueueItemID = responseLocal->QueueItemID;
        response.QueueRow = responseLocal->QueueRow;
        response.TerminalID = responseLocal->TerminalID;
        response.ReceiptReference = responseLocal->ReceiptReference;
        response.CashBoxIdentification = responseLocal->CashBoxIdentification;
        response.ReceiptIdentification = responseLocal->ReceiptIdentification;
        response.ReceiptMoment = responseLocal->ReceiptMoment->AsUTCDateTime;
        response.State = responseLocal->State;
        response.StateData = responseLocal->StateData;
        response.Signatures.clear();
        for(int indexSignatures = 0; indexSignatures< responseLocal->Signatures.Length;indexSignatures++)
        {
            TSignaturItemAustriaFiscal signaturesAustria;
            signaturesAustria.SignatureFormat = responseLocal->Signatures[indexSignatures]->SignatureFormat;
            signaturesAustria.SignatureType = responseLocal->Signatures[indexSignatures]->SignatureType;
            signaturesAustria.Caption = responseLocal->Signatures[indexSignatures]->Caption;
            signaturesAustria.Data = responseLocal->Signatures[indexSignatures]->Data;
            response.Signatures.push_back(signaturesAustria);
        }
    }
    else
    {
        response.UnableToConnectToFiscalService = true;
    }
    return response;
}
//---------------------------------------------------------------------------
bool TAustriaFiscalInterface::CommissionAustriaFiscal(UnicodeString url, UnicodeString cashBoxId,UnicodeString terminalId)
{
    bool retValue = false;
    try
    {
        CoInitialize(NULL);
        retValue = AustriaClient->CommissionAustriaFiscal(url,cashBoxId,terminalId);
    }
    catch(Exception &ex)
    {
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TAustriaFiscalInterface::SendZeroReceipt(UnicodeString url, UnicodeString cashBoxId,UnicodeString terminalId)
{
    bool retValue = false;
    try
    {
        CoInitialize(NULL);
        retValue = AustriaClient->SendZeroReceipt(url,cashBoxId,terminalId);
    }
    catch(Exception &ex)
    {
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TAustriaFiscalInterface::SendMonthlyReceipt(UnicodeString url, UnicodeString cashBoxId,UnicodeString terminalId)
{
    bool retValue = false;
    try
    {
        CoInitialize(NULL);
        retValue = AustriaClient->SendMonthlyReceipt(url,cashBoxId,terminalId);
    }
    catch(Exception &ex)
    {
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TAustriaFiscalInterface::SendAnnualReceipt(UnicodeString url, UnicodeString cashBoxId,UnicodeString terminalId)
{
    bool retValue = false;
    try
    {
        CoInitialize(NULL);
        retValue = AustriaClient->SendAnnualReceipt(url,cashBoxId,terminalId);
    }
    catch(Exception &ex)
    {
    }
    return retValue;
}
//---------------------------------------------------------------------------
