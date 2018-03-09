//---------------------------------------------------------------------------


#pragma hdrstop

#include "FiscalPrinting.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TFiscalPrinting::TFiscalPrinting()
{
    initFIClient();
}
//---------------------------------------------------------------------------
void TFiscalPrinting::initFIClient()
{
    bool useWSDL = false;
    AnsiString fiscalURL = AnsiString("http://localhost:8744/MenumateServices/FiscalPrinter/");

    fiscalClient = GetIWCFServiceFiscalPrinter(
                            useWSDL, fiscalURL, NULL );
}
//---------------------------------------------------------------------------
TFiscalPrinterResponse TFiscalPrinting::PrintFiscalReceipt(TFiscalBillDetails receiptData)
{
    TFiscalPrinterResponse response;
    response.IsSuccessful = false;
    try
    {
        FiscalResponseDetails* fiscalPrinterResponse = new FiscalResponseDetails();
        FiscalDataDetails *dataDetails = new  FiscalDataDetails();
        dataDetails->Billno = receiptData.Billno;
        dataDetails->Cashier = receiptData.Cashier;
        dataDetails->Cashno = receiptData.Cashno;
        dataDetails->Date = receiptData.Date;
        dataDetails->InvoiceNumber = receiptData.InvoiceNumber;
        dataDetails->Source = receiptData.Source;
        dataDetails->Time = receiptData.Time;

        ArrayOfDiscountDetails arrayOfDiscountDetails;
        ArrayOfFiscalService arrayOfFiscalService;
        ArrayOfFiscalPayment arrayOfFiscalPayment;

        for(std::vector<TFiscalItemDetails>::iterator i = receiptData.ItemList.begin(); i != receiptData.ItemList.end() ; ++i)
        {
            FiscalService *fiscalService = GetItemList(i);
            arrayOfFiscalService.Length = (arrayOfFiscalService.Length + 1);
            arrayOfFiscalService[arrayOfFiscalService.Length - 1] = fiscalService;
        }
         dataDetails->ItemList = arrayOfFiscalService;

        for(std::vector<TFiscalPaymentDetails>::iterator i = receiptData.PaymentList.begin(); i != receiptData.PaymentList.end() ; ++i)
        {
            FiscalPayment *fiscalPrinting = GetPayment(i);
            arrayOfFiscalPayment.Length = (arrayOfFiscalPayment.Length + 1);
            arrayOfFiscalPayment[arrayOfFiscalPayment.Length - 1] = fiscalPrinting;
        }
         dataDetails->PaymentList = arrayOfFiscalPayment;

        for(std::vector<TFiscalDiscountDetails>::iterator i = receiptData.DiscountList.begin(); i != receiptData.DiscountList.end() ; ++i)
        {
            DiscountDetails *discountDetails = GetDiscount(i);
            arrayOfDiscountDetails.Length = (arrayOfDiscountDetails.Length + 1);
            arrayOfDiscountDetails[arrayOfDiscountDetails.Length - 1] = discountDetails;
        }
        dataDetails->DiscountList = arrayOfDiscountDetails;
        CoInitialize(NULL);
        fiscalPrinterResponse = fiscalClient->PrintFiscalReceipt(dataDetails);
        response.IsSuccessful = fiscalPrinterResponse->IsSuccessful;
        response.ResponseMessage = fiscalPrinterResponse->Response;
    }
    catch(Exception & E)
    {
       response.ResponseMessage = "Exception found in PrintFiscalReceipt()";
	}
    return response;
}
//----------------------------------------------------------------------------------
FiscalService* TFiscalPrinting::GetItemList(std::vector<TFiscalItemDetails>::iterator it)
{
    FiscalService *service = new FiscalService;
    service->ChitNumber = it->ChitNumber;
    service->GuestName = it->GuestName;
    service->ItemCategory = it->ItemCategory;
    service->ItemDescription = it->ItemDescription;
    service->MemberName = it->MemberName;
    service->PartyName = it->PartyName;
    service->PricePerUnit = it->PricePerUnit;
    service->PriceTotal = it->PriceTotal;
    service->SizeName = it->SizeName;
    service->TableNo = it->TableNo;
    service->VATPercentage = it->VATPercentage;
    service->quantity = it->Quantity;
    return service;
}
//---------------------------------------------------------------------------
FiscalPayment* TFiscalPrinting::GetPayment(std::vector<TFiscalPaymentDetails>::iterator it)
{
    FiscalPayment *payment = new FiscalPayment;
    payment->Amount = it->Amount;
    payment->Billno = it->Billno;
    payment->Cashier = it->Cashier;
    payment->Cashno = it->Cashno;
    payment->Description = it->Description;
    payment->Source = it->Source;
    payment->Type = it->Type;
    return payment;
}
//---------------------------------------------------------------------------
DiscountDetails* TFiscalPrinting::GetDiscount(std::vector<TFiscalDiscountDetails>::iterator it)
{
    DiscountDetails *discount = new DiscountDetails;
    discount->Amount = it->Amount;
    discount->Description = it->Description;
    discount->DiscountGroup = it->DiscountGroup;
    discount->DiscountMode = it->DiscountMode;
    discount->Type = it->Type;
    return discount;
}
