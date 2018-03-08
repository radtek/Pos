//---------------------------------------------------------------------------


#pragma hdrstop

#include "FiscalPrinterAdapter.h"
#include "IBillCalculator.h"

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
        itemDetails.Quantity = order->GetQty();
        itemDetails.ItemCategory = order->Categories->FinancialCategory;
        if(order->HappyHour)
        {
             itemDetails.PricePerUnit = CurrToStrF(order->PriceLevel0, ffCurrency, CurrencyDecimals);
        }
        else
        {
            itemDetails.PricePerUnit = CurrToStrF(order->PriceLevel1, ffCurrency, CurrencyDecimals);
        }
        itemDetails.PriceTotal = itemDetails.Quantity*itemDetails.PricePerUnit;
        itemDetails.VATPercentage = 0;
        std::vector<BillCalculator::TTaxResult> taxInfomation = order->BillCalcResult.Tax;
        for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
	    {
            if(!itTaxes->TaxType)
                itemDetails.VATPercentage += itTaxes->Percentage;
        }
        itemList.push_back(itemDetails);
        PrepareDiscountDetails(discountList, order);
        for (int i = 0; i < order->SubOrders->Count; i++)
		{
		    TItemCompleteSub *currentSubOrder = (TItemCompleteSub*)order->SubOrders->Items[i];
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
            if(order->HappyHour)
            {
                 itemDetails.PricePerUnit = CurrToStrF(order->PriceLevel0, ffCurrency, CurrencyDecimals);
            }
            else
            {
                itemDetails.PricePerUnit = CurrToStrF(order->PriceLevel1, ffCurrency, CurrencyDecimals);
            }
            itemDetails.PriceTotal = itemDetails.Quantity*itemDetails.PricePerUnit;
            itemDetails.VATPercentage = 0;
            std::vector<BillCalculator::TTaxResult> taxInfomation = order->BillCalcResult.Tax;
            for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
            {
                if(!itTaxes->TaxType)
                    itemDetails.VATPercentage += itTaxes->Percentage;
            }
            itemList.push_back(itemDetails);
            PrepareDiscountDetails(discountList, order);
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
            paymentDetails.Amount = RoundToNearest(SubPayment->GetPayTendered(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
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
        TFiscalDiscountDetails discountDetails;
        discountDetails.Type = ptrDiscounts->Type;
        discountDetails.Amount = ptrDiscounts->Amount;
        discountDetails.Description = ptrDiscounts->Description;
        discountDetails.DiscountGroup = ptrDiscounts->DiscountGroupList.size() > 0 ? ptrDiscounts->DiscountGroupList[0].Name : UnicodeString::UnicodeString("");
        discountDetails.DiscountMode = ptrDiscounts->Mode;
        discountList.push_back(discountDetails);
    }
}

