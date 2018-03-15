//---------------------------------------------------------------------------


#pragma hdrstop
#include "main.h"
#include "FiscalPrinting.h"
#include "MMMessageBox.h"
#include "FiscalIntegration_OCX.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TFiscalPrinting::TFiscalPrinting()
{
    initFIClient();
}
//---------------------------------------------------------------------------
void TFiscalPrinting::initFIClient()
{
//    bool useWSDL = false;
//    AnsiString fiscalURL = AnsiString("http://localhost:8744/MenumateServices/FiscalPrinter/");
//
//    fiscalClient = GetIWCFServiceFiscalPrinter(
//                            useWSDL, fiscalURL, NULL );
}
//---------------------------------------------------------------------------
TFiscalPrinterResponse TFiscalPrinting::PrintFiscalReceipt(TFiscalBillDetails receiptData)
{
    TFiscalPrinterResponse response;
    response.IsSuccessful = false;
    try
    {
        TFiscalLibraryClass *fpclass = new TFiscalLibraryClass(frmMain);
        fpclass->Billno = WideString(receiptData.Billno).c_bstr();
        fpclass->Cashier = WideString(receiptData.Cashier).c_bstr();
        fpclass->Cashno = WideString(receiptData.Cashno).c_bstr();
        fpclass->Date = WideString(receiptData.Date).c_bstr();
        fpclass->Source = WideString(receiptData.Source).c_bstr();
        fpclass->Time = WideString(receiptData.Time).c_bstr();

        for(std::vector<TFiscalItemDetails>::iterator i = receiptData.ItemList.begin(); i != receiptData.ItemList.end() ; ++i)
        {
            fpclass->InitializeItemProperties();
            fpclass->LoadReceiptItemInfo(0, WideString(i->ChitNumber).c_bstr());
            fpclass->LoadReceiptItemInfo(1, WideString(i->GuestName).c_bstr());
            fpclass->LoadReceiptItemInfo(2, WideString(i->ItemCategory).c_bstr());
            fpclass->LoadReceiptItemInfo(3, WideString(i->ItemDescription).c_bstr());
            fpclass->LoadReceiptItemInfo(4, WideString(i->MemberName).c_bstr());
            fpclass->LoadReceiptItemInfo(5, WideString(i->PartyName).c_bstr());
            fpclass->LoadReceiptItemInfo(6, WideString(i->PricePerUnit).c_bstr());
            fpclass->LoadReceiptItemInfo(7, WideString(i->PriceTotal).c_bstr());
            fpclass->LoadReceiptItemInfo(8, WideString(i->Quantity).c_bstr());
            fpclass->LoadReceiptItemInfo(9, WideString(i->SizeName).c_bstr());
            fpclass->LoadReceiptItemInfo(10, WideString(i->TableNo).c_bstr());
            fpclass->LoadReceiptItemInfo(11, WideString(i->VATPercentage).c_bstr());
            fpclass->AddItemToList();
        }
//        MessageBox("dddd","",MB_OK);
          int number = fpclass->PrintReceipt();
          MessageBox(number,number,MB_OK);
//          number = fpclass->PrintReceipt(s1.c_str(),s2.c_str());
//          MessageBox(number,number,MB_OK);
//        fpclass->PrintReceipt(str1.c_str(),str2.c_str());
//        FiscalResponseDetails* fiscalPrinterResponse = new FiscalResponseDetails();
//        FiscalDataDetails *dataDetails = new  FiscalDataDetails();
//        dataDetails->Billno = receiptData.Billno;
//        dataDetails->Cashier = receiptData.Cashier;
//        dataDetails->Cashno = receiptData.Cashno;
//        dataDetails->Date = receiptData.Date;
//        dataDetails->InvoiceNumber = receiptData.InvoiceNumber;
//        dataDetails->Source = receiptData.Source;
//        dataDetails->Time = receiptData.Time;
//
//        ArrayOfDiscountDetails arrayOfDiscountDetails;
//        ArrayOfFiscalService arrayOfFiscalService;
//        ArrayOfFiscalPayment arrayOfFiscalPayment;

//
        for(std::vector<TFiscalPaymentDetails>::iterator i = receiptData.PaymentList.begin(); i != receiptData.PaymentList.end() ; ++i)
        {
//            FiscalPayment *fiscalPrinting = GetPayment(i);
//            arrayOfFiscalPayment.Length = (arrayOfFiscalPayment.Length + 1);
//            arrayOfFiscalPayment[arrayOfFiscalPayment.Length - 1] = fiscalPrinting;
        }
//         dataDetails->PaymentList = arrayOfFiscalPayment;
//
        for(std::vector<TFiscalDiscountDetails>::iterator i = receiptData.DiscountList.begin(); i != receiptData.DiscountList.end() ; ++i)
        {
//            DiscountDetails *discountDetails = GetDiscount(i);
//            arrayOfDiscountDetails.Length = (arrayOfDiscountDetails.Length + 1);
//            arrayOfDiscountDetails[arrayOfDiscountDetails.Length - 1] = discountDetails;
        }
//        dataDetails->DiscountList = arrayOfDiscountDetails;
//        CoInitialize(NULL);
//        fiscalPrinterResponse = fiscalClient->PrintFiscalReceipt(dataDetails);
//        response.IsSuccessful = fiscalPrinterResponse->IsSuccessful;
//        response.ResponseMessage = fiscalPrinterResponse->Response;
    }
    catch(Exception & E)
    {
       response.ResponseMessage = E.Message;//"Exception found in PrintFiscalReceipt()";
	}
    return response;
}
//----------------------------------------------------------------------------------
//TFiscalService* TFiscalPrinting::GetItemList(std::vector<TFiscalItemDetails>::iterator it)
//{
//    TFiscalService *service = new TFiscalService(frmMain);
////    service->ChitNumber = it->ChitNumber;
////    service->GuestName = it->GuestName;
////    service->ItemCategory = it->ItemCategory;
////    service->ItemDescription = it->ItemDescription;
////    service->MemberName = it->MemberName;
////    service->PartyName = it->PartyName;
////    service->PricePerUnit = it->PricePerUnit;
////    service->PriceTotal = it->PriceTotal;
////    service->SizeName = it->SizeName;
////    service->TableNo = it->TableNo;
////    service->VATPercentage = it->VATPercentage;
////    service->quantity = it->Quantity;
//    return service;
//}

//---------------------------------------------------------------------------
//FiscalPayment* TFiscalPrinting::GetPayment(std::vector<TFiscalPaymentDetails>::iterator it)
//{
//    FiscalPayment *payment = new FiscalPayment;
//    payment->Amount = it->Amount;
//    payment->Billno = it->Billno;
//    payment->Cashier = it->Cashier;
//    payment->Cashno = it->Cashno;
//    payment->Description = it->Description;
//    payment->Source = it->Source;
//    payment->Type = it->Type;
//    return payment;
//}
//---------------------------------------------------------------------------
//DiscountDetails* TFiscalPrinting::GetDiscount(std::vector<TFiscalDiscountDetails>::iterator it)
//{
//    DiscountDetails *discount = new DiscountDetails;
//    discount->Amount = it->Amount;
//    discount->Description = it->Description;
//    discount->DiscountGroup = it->DiscountGroup;
//    discount->DiscountMode = it->DiscountMode;
//    discount->Type = it->Type;
//    return discount;
//}
//----------------------------------------------------------------------------------
//TFiscalPrinterResponse TFiscalPrinting::PrintZReport()
//{
//    TFiscalPrinterResponse response;
//    response.IsSuccessful = false;
//    try
//    {
//        FiscalResponseDetails* fiscalPrinterResponse = new FiscalResponseDetails();
//        CoInitialize(NULL);
//        fiscalPrinterResponse = fiscalClient->PrintZSettlement();
//        response.IsSuccessful = fiscalPrinterResponse->IsSuccessful;
//        response.ResponseMessage = fiscalPrinterResponse->Response;
//    }
//    catch(Exception & E)
//    {
//       response.ResponseMessage = "Exception found in PrintZReport()";
//	}
//    return response;
//}
