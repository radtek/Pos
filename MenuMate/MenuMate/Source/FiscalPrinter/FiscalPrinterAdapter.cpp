//---------------------------------------------------------------------------


#pragma hdrstop

#include "FiscalPrinterAdapter.h"
#include "main.h"
#include <Math.hpp>
#include "MMMessageBox.h"
#include "FiscalIntegration_OCX.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TFiscalPrinterAdapter::TFiscalPrinterAdapter()
{
    //billDetails = new TFiscalBillDetails();
}
//---------------------------------------------------------------------------
void TFiscalPrinterAdapter::ConvertInToFiscalData(TPaymentTransaction paymentTransaction)
{
    billDetails.InvoiceNumber = paymentTransaction.InvoiceNumber;
    billDetails.Date = Now().FormatString("dd/mm/yyyy");
    billDetails.Billno = paymentTransaction.InvoiceNumber;
    //billDetails.Cashier = TDeviceRealTerminal::Instance().User.Name;
    billDetails.Time = Now().FormatString("hh:nn");
    PrepareItemInfo(paymentTransaction);
    PrepartePaymnetInfo(paymentTransaction);
    PrintFiscalReceipt(billDetails).ResponseMessage;
}
//------------------------------------------------------------------------------
void TFiscalPrinterAdapter::PrepareItemInfo(TPaymentTransaction paymentTransaction)
{
    std::vector<TFiscalItemDetails> itemList;
    std::vector<TFiscalDiscountDetails> discountList;
    for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
    {
        TItemComplete *order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);

        TFiscalItemDetails itemDetails;
        itemDetails.TableNo = order->TabContainerName;
        itemDetails.GuestName = order->TabContainerName;
        itemDetails.ChitNumber = order->ChitNumber.ChitNumber;
        itemDetails.MemberName = paymentTransaction.Membership.Member.Name;
        itemDetails.PartyName = order->PartyName;
        itemDetails.ItemDescription = order->Item;
        if (UpperCase(order->Size) != "DEFAULT")
                itemDetails.SizeName = order->Size;
        else
            itemDetails.SizeName = "";
        itemDetails.Quantity = Double(order->GetQty()); 
        itemDetails.ItemCategory = order->Categories->FinancialCategory;
        double priceTotal = 0;
        double ItemPrice = 0;
        ItemPrice = (double)RoundToNearest(order->TotalPriceSides()/order->GetQty(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
       // ItemPrice = (double)(order->TotalPriceSides());
         priceTotal =  (double)RoundToNearest((order->TotalPriceSides()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown );
        //itemDetails.PricePerUnit = double(ItemPrice*1000);
       // itemDetails.PriceTotal = double(priceTotal);
		itemDetails.PricePerUnit = ItemPrice;
        itemDetails.PriceTotal = priceTotal;
        itemDetails.VATPercentage;
        std::vector<BillCalculator::TTaxResult> taxInfomation = order->BillCalcResult.Tax;
        double taxPercentage = 0;
        for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
	    {
            if(!itTaxes->TaxType)
                taxPercentage += (double)itTaxes->Percentage;
        }
        itemDetails.VATPercentage = taxPercentage;//double(taxPercentage*1000);
        itemList.push_back(itemDetails);
        PrepareDiscountDetails(discountList, order);
        for (int i = 0; i < order->SubOrders->Count; i++)
		{
		    TItemMinorComplete *currentSubOrder = (TItemMinorComplete*)order->SubOrders->Items[i];
            TFiscalItemDetails itemDetails;
            itemDetails.TableNo = order->TabContainerName;
            itemDetails.GuestName = order->TabContainerName;
            itemDetails.ChitNumber = order->ChitNumber.ChitNumber;
            itemDetails.MemberName = paymentTransaction.Membership.Member.Name;
            itemDetails.PartyName = order->PartyName;
            itemDetails.ItemDescription = currentSubOrder->Item;
            if (UpperCase(currentSubOrder->Size) != "DEFAULT")
                itemDetails.SizeName = currentSubOrder->Size;
            else
                itemDetails.SizeName = "";
            itemDetails.Quantity = currentSubOrder->GetQty();
            itemDetails.ItemCategory = currentSubOrder->Categories->FinancialCategory;
            priceTotal = 0;
            ItemPrice = 0;
            ItemPrice = (double)RoundToNearest(currentSubOrder->TotalPriceSides()/currentSubOrder->GetQty(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
            priceTotal =  (double)(currentSubOrder->TotalPriceSides());
            itemDetails.PricePerUnit = ItemPrice;//double(ItemPrice*1000);
            itemDetails.PriceTotal = priceTotal;//double(priceTotal*1000);
            double taxPercentage = 0;
            std::vector<BillCalculator::TTaxResult> taxInfomation = currentSubOrder->BillCalcResult.Tax;
            for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
            {
                if(!itTaxes->TaxType)
                    taxPercentage = taxPercentage + itTaxes->Percentage;
            }
            itemDetails.VATPercentage = taxPercentage;//double(taxPercentage*1000);//RoundTo(taxPercentage, -2);
            itemList.push_back(itemDetails);
            PrepareDiscountDetails(discountList, currentSubOrder);
        }
    }
    billDetails.ItemList = itemList;
    billDetails.DiscountList = discountList;
}
//-------------------------------------------------------------
void TFiscalPrinterAdapter::PrepartePaymnetInfo(TPaymentTransaction paymentTransaction)
{
    std::vector<TFiscalPaymentDetails> PaymentList;
    for (int i = 0; i < paymentTransaction.PaymentsCount(); i++)
    {
        TPayment *SubPayment = paymentTransaction.PaymentGet(i);
        if (SubPayment->GetPay() != 0)
        {
            TFiscalPaymentDetails paymentDetails;
            double subTotal =  (double)RoundToNearest(SubPayment->GetPayTendered(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
            paymentDetails.Amount = subTotal;
            paymentDetails.Description = SubPayment->Name;
            paymentDetails.Billno = paymentTransaction.InvoiceNumber;
            PaymentList.push_back(paymentDetails);
        }
    }
    billDetails.PaymentList = PaymentList;
}
//------------------------------------------------------------------------------
void TFiscalPrinterAdapter::PrepareDiscountDetails(std::vector<TFiscalDiscountDetails> &discountList,TItemMinorComplete *order)
{
    for (std::vector <TDiscount> ::const_iterator ptrDiscounts = order->Discounts.begin(); ptrDiscounts != order->Discounts.end();
	std::advance(ptrDiscounts, 1))
	{
        if(order->DiscountValue_BillCalc(ptrDiscounts) == 0)
            continue;
        bool isDiscountAlreadyExists = false;
        double discountAmount = 0;
        discountAmount = (double)RoundToNearest(order->DiscountValue_BillCalc(ptrDiscounts),0.01,TGlobalSettings::Instance().MidPointRoundsDown);
        for (std::vector<TFiscalDiscountDetails> ::iterator ptrFiscalDiscounts = discountList.begin(); ptrFiscalDiscounts != discountList.end();
        std::advance(ptrFiscalDiscounts, 1))
        {
            if(ptrFiscalDiscounts->DiscountKey == ptrDiscounts->DiscountKey)
            {
                discountAmount = (double)(StrToCurr(ptrFiscalDiscounts->Amount) + discountAmount);
                ptrFiscalDiscounts->Amount = discountAmount;
                isDiscountAlreadyExists = true;
                break;
            }
        }

        if(!isDiscountAlreadyExists)
        {
            TFiscalDiscountDetails discountDetails;
            discountDetails.DiscountKey = ptrDiscounts->DiscountKey;
            discountDetails.Type = ptrDiscounts->Type;
            discountDetails.Amount = discountAmount;
            discountDetails.Description = ptrDiscounts->Description;
            discountDetails.DiscountGroup = ptrDiscounts->DiscountGroupList.size() > 0 ? ptrDiscounts->DiscountGroupList[0].Name : UnicodeString::UnicodeString("");
            discountDetails.DiscountMode = ptrDiscounts->Mode;
            discountList.push_back(discountDetails);
        }
    }
}
//-----------------------------------------------------------------------------------------------
TFiscalPrinterResponse TFiscalPrinterAdapter::FiscalZReportSettlement()
{
    TFiscalPrinterResponse response;
    try
    {
        response.IsSuccessful = false;
        TFiscalLibraryClass *fpclass = new TFiscalLibraryClass(frmMain);
        fpclass->PrintZReport();
    }
    catch(Exception & E)
    {
       response.ResponseMessage = E.Message;
	}
    return response;
}
//----------------------------------------------------------------------------------------------------
TFiscalPrinterResponse TFiscalPrinterAdapter::PrintFiscalReceipt(TFiscalBillDetails receiptData)
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
    }
    catch(Exception & E)
    {
       response.ResponseMessage = E.Message;
	}
    return response;
}
//------------------------------------------------------------------------------------------


