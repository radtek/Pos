//---------------------------------------------------------------------------


#pragma hdrstop

#include "FiscalPrinterAdapter.h"
#include "IBillCalculator.h"
#include <Math.hpp>
#include "MMMessageBox.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TFiscalPrinterAdapter::TFiscalPrinterAdapter()
{

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
    std::auto_ptr<TFiscalPrinting> fiscalPrinting(new TFiscalPrinting());
    fiscalPrinting->PrintFiscalReceipt(billDetails);
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
        itemDetails.SizeName = order->Size;
        itemDetails.Quantity = Double(order->GetQty()); 
        itemDetails.ItemCategory = order->Categories->FinancialCategory;
        double priceTotal = 0;
        double ItemPrice = 0;
        ItemPrice = (double)(order->BillCalcResult.FinalPrice/order->GetQty());
         priceTotal =  (double)(order->BillCalcResult.FinalPrice);
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
        itemDetails.VATPercentage = taxPercentage;
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
            itemDetails.ItemDescription = order->Item;
            itemDetails.SizeName = order->Size;
            itemDetails.Quantity = order->GetQty();
            itemDetails.ItemCategory = order->Categories->FinancialCategory;
            priceTotal = 0;
            ItemPrice = 0;
            ItemPrice = (double)(currentSubOrder->BillCalcResult.FinalPrice/currentSubOrder->GetQty());
            priceTotal =  (double)(currentSubOrder->BillCalcResult.FinalPrice);
            itemDetails.PricePerUnit = ItemPrice;
            itemDetails.PriceTotal = priceTotal;
            double taxPercentage = 0;
            std::vector<BillCalculator::TTaxResult> taxInfomation = currentSubOrder->BillCalcResult.Tax;
            for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
            {
                if(!itTaxes->TaxType)
                    taxPercentage = taxPercentage + itTaxes->Percentage;
            }
            itemDetails.VATPercentage = taxPercentage;//RoundTo(taxPercentage, -2);
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
        MessageBox(discountAmount,"Error1",MB_OK);
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

