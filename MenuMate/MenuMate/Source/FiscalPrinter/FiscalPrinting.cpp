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
        fpclass->InvoiceNumber = WideString(receiptData.InvoiceNumber).c_bstr();
        // MessageBox(WideString(receiptData.InvoiceNumber).c_bstr(),"Item price",MB_OK);

        for(std::vector<TFiscalItemDetails>::iterator i = receiptData.ItemList.begin(); i != receiptData.ItemList.end() ; ++i)
        {
            fpclass->InitializeItemProperties();
            fpclass->LoadReceiptItemInfo(0, WideString(i->ChitNumber).c_bstr());
            fpclass->LoadReceiptItemInfo(1, WideString(i->GuestName).c_bstr());
            fpclass->LoadReceiptItemInfo(2, WideString(i->ItemCategory).c_bstr());
            fpclass->LoadReceiptItemInfo(3, WideString(i->ItemDescription).c_bstr());
            AnsiString s1 = "Item name: " + i->ItemDescription;
            makeLogFile(s1);
            fpclass->LoadReceiptItemInfo(4, WideString(i->MemberName).c_bstr());
            fpclass->LoadReceiptItemInfo(5, WideString(i->PartyName).c_bstr());
            fpclass->LoadReceiptItemInfo(6, WideString(i->PricePerUnit).c_bstr());
            s1 = "PricePerUnit: " + i->PricePerUnit;
            makeLogFile(s1);
           // MessageBox(WideString(i->PricePerUnit).c_bstr(),"Item price",MB_OK);
            fpclass->LoadReceiptItemInfo(7, WideString(i->PriceTotal).c_bstr());
            s1 = "Item name: " + i->ItemDescription;
            makeLogFile(s1);
            makeLogFile(i->PriceTotal);
            fpclass->LoadReceiptItemInfo(8, WideString(i->Quantity).c_bstr());
            s1 = "Item Quantity: " + i->Quantity;
            makeLogFile(s1);
            fpclass->LoadReceiptItemInfo(9, WideString(i->SizeName).c_bstr());
            fpclass->LoadReceiptItemInfo(10, WideString(i->TableNo).c_bstr());
            fpclass->LoadReceiptItemInfo(11, WideString(i->VATPercentage).c_bstr());
            fpclass->AddItemToList();
        }

        for(std::vector<TFiscalPaymentDetails>::iterator i = receiptData.PaymentList.begin(); i != receiptData.PaymentList.end() ; ++i)
        {
            fpclass->InitializePaymentProperties();
            fpclass->LoadReceiptPaymentInfo(0, WideString(i->Type).c_bstr());
            fpclass->LoadReceiptPaymentInfo(1, WideString(i->Amount).c_bstr());
            fpclass->LoadReceiptPaymentInfo(2, WideString(i->Billno).c_bstr());
            fpclass->LoadReceiptPaymentInfo(3, WideString(i->Cashno).c_bstr());
            fpclass->LoadReceiptPaymentInfo(4, WideString(i->Cashier).c_bstr());
            fpclass->LoadReceiptPaymentInfo(5, WideString(i->Source).c_bstr());
            fpclass->LoadReceiptPaymentInfo(6, WideString(i->Description).c_bstr());
            fpclass->AddPaymentInfoToList();
        }

        for(std::vector<TFiscalDiscountDetails>::iterator i = receiptData.DiscountList.begin(); i != receiptData.DiscountList.end() ; ++i)
        {
            fpclass->InitializeDiscountProperties();
            fpclass->LoadReceiptItemInfo(0, WideString(i->Amount).c_bstr());
            fpclass->LoadReceiptItemInfo(1, WideString(i->Description).c_bstr());
            fpclass->LoadReceiptItemInfo(2, WideString(i->DiscountGroup).c_bstr());
            fpclass->LoadReceiptItemInfo(3, WideString(i->DiscountMode).c_bstr());
            fpclass->LoadReceiptItemInfo(4, WideString(i->Type).c_bstr());
            fpclass->AddDiscountInfoToList();
        }

        int number = fpclass->PrintReceipt();
          MessageBox(number,number,MB_OK);
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
//}void TFiscalPrinting::makeLogFile(AnsiString str1){
    AnsiString directoryName = ExtractFilePath(Application->ExeName) + "/fiscalPrinter Logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    AnsiString name = Now().CurrentDate().FormatString("DDMMYYYY")+ ".txt";
    AnsiString fileName =  directoryName + "/" + name;
    std::auto_ptr<TStringList> List(new TStringList);
    if (FileExists(fileName) )
      List->LoadFromFile(fileName);

    List->Add("Request- "+ str1 +  "\n");

    List->SaveToFile(fileName );
}
