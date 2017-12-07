//---------------------------------------------------------------------------


#pragma hdrstop

#include "EviaMall.h"
#include "IBillCalculator.h"
#include <Math.h>
#include "Comms.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TEviaMallField::TEviaMallField()
{
    recordId = "";
    stallCode = "";
    salesDate = Now();
    salesTime = Now().CurrentTime();
    grossSales = 0.00;
    vat = 0.00;
    discount = 0.00;
    serviceCahrge = 0.00;
    noOfTrans = 0;
    salesDept = "";
    noOfRecord = 0;
    refund = 0.00;
    cancelledTransaction = 0.00;
    cancelledAmount = 0.00;
    nonVatableGrossSales = 0.00;
    posNumber = 0;
    oldGrandTotal = 0;
    newGrandTotal = 0;
}
//------------------------------------------------------------------------------------------------------------------------
TMallExportSalesWrapper TEviaMall::PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey, TDateTime currentTime)
{
    TMallExportSalesWrapper salesWrapper;
    std::list<TMallExportSalesData> mallExportSalesData;
    try
    {
        TEviaMallField *fieldData = new TEviaMallField();
        Currency taxRate = 0.00;
        double amount = 0;
        fieldData->salesDate = currentTime.FormatString("mm/dd/yyyy");
        fieldData->salesTime = currentTime.FormatString("hh:mm");
        std::list<TMallExportSettings>::iterator it;

        for(it = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); it != TGlobalSettings::Instance().mallInfo.MallSettings.end(); it++)
        {
            if(it->Value != "" )
            {
                if(it->ControlName == "edMallTenantNo")
                {
                    fieldData->stallCode = it->Value;
                }
                else if(it->ControlName == "edMallTerminalNo" )
                {
                    fieldData->posNumber = StrToInt(it->Value);
                }
                else if(it->ControlName == "edMallBranchCode")
                {
                    fieldData->salesDept = it->Value;
                }
            }
        }

        for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
        {
                TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);

                //prepare all fields per items
                PrepareDataByItem(paymentTransaction.DBTransaction, Order, *fieldData);

                //For SubOrder
                for (int i = 0; i < Order->SubOrders->Count; i++)
				{
					TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
                   // PrepareDataByItem(paymentTransaction.DBTransaction, CurrentSubOrder, *fieldData);
                }
        }

        for (int i = 0; i < paymentTransaction.PaymentsCount(); i++)
		{
			TPayment *SubPayment = paymentTransaction.PaymentGet(i);
			if (SubPayment->GetPay() != 0)
			{
                amount = (double)(SubPayment->GetPayTendered() - SubPayment->GetChange() - paymentTransaction.Membership.Member.Points.getCurrentPointsPurchased());
                fieldData->grossSales += amount;
            }
        }
        fieldData->oldGrandTotal = 0;//GetOldAccumulatedSales(paymentTransaction.DBTransaction, 5);
        fieldData->newGrandTotal = fieldData->oldGrandTotal + fieldData->grossSales;

        fieldData->refund = paymentTransaction.Money.FinalPrice > 0 ? 0 : fabs(paymentTransaction.Money.FinalPrice);

        //fieldData->ZKey = 0;
       // fieldData->SalesCount =  1;

//        if(fieldData->TotalVoidAmount > 0 && fieldData->TotalNetSaleAmount == 0)
//        {
//            fieldData->CustomerCount = 0;
//            fieldData->SalesCount = 0;
//        }
//        else
//        {
//            fieldData->CustomerCount = GetPatronCount(paymentTransaction);
//        }
//
//        //call For inserting into list
//        InsertFieldInToList(paymentTransaction.DBTransaction, mallExportSalesData, *fieldData, arcBillKey);
//
//        //Assign sales list to wrapper class
//        salesWrapper.SalesData = mallExportSalesData;
//        salesWrapper.SaleBySalsType = fieldData->SalesBySalesType;
		delete fieldData;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return salesWrapper;
}
//------------------------------------------------------------------------------------------------------------
void TEviaMall::PrepareDataByItem(Database::TDBTransaction &dbTransaction, TItemMinorComplete *order, TEviaMallField &fieldData)
{
//    //Create Taxes Object to collect all taxes details
//    TDeanAndDelucaTaxes taxes;
//    taxes.salesTax = 0;
//    taxes.serviceCharge = 0;
//    taxes.serviceChargeTax = 0;
//    taxes.localTax = 0;

    //Get all Taxes stored them in TEstanciaTaxes type structure
    bool isVatable = IsItemVatable(order, fieldData);

    for (std::vector <TDiscount> ::const_iterator ptrDiscounts = order->Discounts.begin(); ptrDiscounts != order->Discounts.end();std::advance(ptrDiscounts, 1))
    {
        fieldData.discount += (double)order->DiscountValue_BillCalc(ptrDiscounts);
    }

    double grossAmount = 0;
    double salesBySalesType = 0;
    if(order->GetQty() > 0 )
        grossAmount = (double)(order->PriceEach_BillCalc()*order->GetQty()) ;

    salesBySalesType =  (double)(order->PriceEach_BillCalc()*order->GetQty());

    //For Cancel Items..
    TItemComplete *cancelOrder =   (TItemComplete*)order;
    fieldData.cancelledAmount += (double)(cancelOrder->TabContainerName != "" && order->BillCalcResult.BasePrice == 0.00 ?
                                            (order->GetQty()*order->PriceLevel0) : 0);

    if(isVatable)
    {
        fieldData.grossSales += fieldData.discount + grossAmount;
    }
    else
    {
       fieldData.nonVatableGrossSales += (salesBySalesType + fieldData.discount);
    }
}
//-------------------------------------------------------------------------------------------
bool TEviaMall::IsItemVatable(TItemMinorComplete *order, TEviaMallField &fieldData)
{
    std::vector<BillCalculator::TTaxResult> taxInfomation = order->BillCalcResult.Tax;
    bool isVatable = false;

    ///load all taxes value seperate
    for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
    {
        switch( itTaxes->TaxType )
        {
            case TTaxType::ttSale:
                isVatable = true;
                fieldData.vat += (double)itTaxes->Value;
                break;
            case TTaxType::ttLocal:
                 fieldData.vat += (double)itTaxes->Value;
                 isVatable = true;
                 break;
        }
    }
    if (order->BillCalcResult.ServiceCharge.Percentage != 0)
    {
        fieldData.serviceCahrge += (double)order->BillCalcResult.ServiceCharge.Value;
        if (order->BillCalcResult.ServiceCharge.TaxPercentage != 0)
        {
            fieldData.vat += (double)order->BillCalcResult.ServiceCharge.TaxValue;
        }
    }
    if(fieldData.vat == 0.00 )
        isVatable = false;

    return isVatable;
}
