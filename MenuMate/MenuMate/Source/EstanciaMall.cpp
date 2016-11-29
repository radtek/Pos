//---------------------------------------------------------------------------


#pragma hdrstop
#include "EstanciaMall.h"
#include "MallExportData.h"
#include "GlobalSettings.h"
#include "IBillCalculator.h"
#include "Comms.h"
#include "DeviceRealTerminal.h"
#include <Math.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)

void TEstanciaMallField::SetTerminalNumber(int terminalNumber)
{
    _terminalNumber = terminalNumber;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetTenantCode(UnicodeString tenantCode)
{
    _tenantCode = tenantCode;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetOldAccSalesVatable(Currency oldAccSalesVatable)
{
    _oldAccSalesVatable = oldAccSalesVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetNewAccSalesVatable(Currency newAccSaleVatable)
{
    _newAccSalesVatable = newAccSaleVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetGrossAmountVatable(Currency grossAmountVatable)
{
    _grossAmountVatable = grossAmountVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetDeductionVatable(Currency deductionVatable)
{
    _deductionVatable = deductionVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetPromoSalesAmountVatable(Currency promoSaleAmountVatable)
{
    _promoSalesAmountVatable = promoSaleAmountVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetPWDDiscountVatable(Currency pWDDiscountVatable)
{
    _pWDDiscountVatable = pWDDiscountVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetRefundAmountVatable(Currency refundAmountVatable)
{
    _refundAmountVatable = refundAmountVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetReturnedItemsAmountVatable(Currency returnedItemsAmountVatable)
{
    _returnedItemsAmountVatable = returnedItemsAmountVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetOtherTaxesVatable(Currency otherTaxesVatable)
{
    _otherTaxesVatable = otherTaxesVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetServiceChargeAmountVatable(Currency serviceChargeAmountVatable)
{
    _serviceChargeAmountVatable = serviceChargeAmountVatable;
}
//----------------------------------------------------------------------------------------)
void TEstanciaMallField::SetAdjustmentDiscountVatable(Currency adjustmentDiscountVatable)
{
    _adjustmentDiscountVatable = adjustmentDiscountVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetVoidAmountVatable(Currency voidAmountVatable)
{
    _voidAmountVatable = voidAmountVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetDiscountCardsVatable(Currency discountCardsVatable)
{
    _discountCardsVatable = discountCardsVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetDeliveryChargesVatable(Currency deliveryChargesVatable)
{
    _deliveryChargesVatable = deliveryChargesVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetGiftCertificatesChecksRedeemedVatable(Currency giftCertificatesChecksRedeemedVatable)
{
    _giftCertificatesChecksRedeemedVatable = giftCertificatesChecksRedeemedVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount1Vatable(Currency sSDiscount1Vatable)
{
    _sSDiscount1Vatable = sSDiscount1Vatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount2Vatable(Currency sSDiscount2Vatable)
{
    _sSDiscount2Vatable = sSDiscount2Vatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount3Vatable(Currency sSDiscount3Vatable)
{
    _sSDiscount3Vatable = sSDiscount3Vatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount4Vatable(Currency sSDiscount4Vatable)
{
    _sSDiscount4Vatable = sSDiscount4Vatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount5Vatable(Currency sSDiscount5Vatable)
{
    _sSDiscount5Vatable = sSDiscount5Vatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetTotalOfallNonApprovedStoreDiscountsVatable(Currency totalOfallNonApprovedStoreDiscountsVatable)
{
    _totalOfallNonApprovedStoreDiscountsVatable = totalOfallNonApprovedStoreDiscountsVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount1NonApprovedVatable(Currency sSDiscount1NonApprovedVatable)
{
    _sSDiscount1NonApprovedVatable = sSDiscount1NonApprovedVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount2NonApprovedVatable(Currency sSDiscount2NonApprovedVatable)
{
    _sSDiscount2NonApprovedVatable = sSDiscount2NonApprovedVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount3NonApprovedVatable(Currency sSDiscount3NonApprovedVatable)
{
    _sSDiscount3NonApprovedVatable = sSDiscount3NonApprovedVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount4NonApprovedVatable(Currency sSDiscount4NonApprovedVatable)
{
    _sSDiscount4NonApprovedVatable = sSDiscount4NonApprovedVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount5NonApprovedVatable(Currency sSDiscount5NonApprovedVatable)
{
    _sSDiscount5NonApprovedVatable = sSDiscount5NonApprovedVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetVATAmountVatable(Currency vATAmountVatable)
{
    _vATAmountVatable = vATAmountVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetNetSalesAmountVatable(Currency netSalesAmountVatable)
{
    _netSalesAmountVatable = netSalesAmountVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetCoverCount(int coverCount)
{
    _coverCount = coverCount;
}
//-----------------------------------------------------------------------------------------
void TEstanciaMallField::SetControlNumber(int controlNumber)
{
    _controlNumber = controlNumber;
}
//-------------------------------------------------------------------------------------------
void TEstanciaMallField::SetNoOfSaleTransaction(int noOfSalesTransaction)
{
    _noOfSalesTransaction = noOfSalesTransaction;
}
//-------------------------------------------------------------------------------------------------
void TEstanciaMallField::SetSalesType(int salesType)
{
    _salesType = salesType;
}
//--------------------------------------------------------------------------------------------------
void TEstanciaMallField::SetAmount(Currency amount)
{
    _amount = amount;
}
//------------------------------------------------------------------------------------------------------
void TEstanciaMallField::SetOldAccSalesNonVatable(Currency oldAccSalesNonVatable)
{
    _oldAccSalesNonVatable = oldAccSalesNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetNewAccSalesNonVatable(Currency newAccSalesNonVatable)
{
    _newAccSalesNonVatable = newAccSalesNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetGrossAmountNonVatable(Currency grossAmountNonVatable)
{
    _grossAmountNonVatable = grossAmountNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetDeductionNonVatable(Currency deductionNonVatable)
{
    _deductionNonVatable = deductionNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetPromoSalesAmountNonVatable(Currency promoSalesAmountNonVatable)
{
    _promoSalesAmountNonVatable = promoSalesAmountNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSCDDiscountNonVatable(Currency sCDDiscountNonVatable)
{
    _sCDDiscountNonVatable = sCDDiscountNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetRefundAmountNonVatable(Currency refundAmountNonVatable)
{
    _refundAmountNonVatable = refundAmountNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetReturnedItemsAmountNonVatable(Currency returnedItemsAmountNonVatable)
{
    _returnedItemsAmountNonVatable = returnedItemsAmountNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetOtherTaxesNonVatable(Currency otherTaxesNonVatable)
{
    _otherTaxesNonVatable = otherTaxesNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetServiceChargeAmountNonVatable(Currency serviceChargeAmountNonVatable)
{
    _serviceChargeAmountNonVatable = serviceChargeAmountNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetAdjustmentDiscountNonVatable(Currency adjustmentDiscountNonVatable)
{
    _adjustmentDiscountNonVatable = adjustmentDiscountNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetVoidAmountNonVatable(Currency voidAmountNonVatable)
{
    _voidAmountNonVatable = voidAmountNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetDiscountCardsNonVatable(Currency discountCardsNonVatable)
{
    _discountCardsNonVatable = discountCardsNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetDeliveryChargesNonVatable(Currency deliveryChargesNonVatable)
{
    _deliveryChargesNonVatable = deliveryChargesNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetGiftCertificatesChecksRedeemedNonVatable(Currency giftCertificatesChecksRedeemedNonVatable)
{
    _giftCertificatesChecksRedeemedNonVatable = giftCertificatesChecksRedeemedNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount1NonVatable(Currency sSDiscount1NonVatable)
{
    _sSDiscount1NonVatable = sSDiscount1NonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount2NonVatable(Currency sSDiscount2NonVatable)
{
    _sSDiscount2NonVatable = sSDiscount2NonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount3NonVatable(Currency sSDiscount3NonVatable)
{
    _sSDiscount3NonVatable = sSDiscount3NonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount4NonVatable(Currency sSDiscount4NonVatable)
{
    _sSDiscount4NonVatable = sSDiscount4NonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount5NonVatable(Currency sSDiscount5NonVatable)
{
    _sSDiscount5NonVatable = sSDiscount5NonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetTotalOfallNonApprovedStoreDiscountsNonVatable(Currency totalOfallNonApprovedStoreDiscountsNonVatable)
{
    _totalOfallNonApprovedStoreDiscountsNonVatable = totalOfallNonApprovedStoreDiscountsNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount1NonApprovedNonVatable(Currency sSDiscount1NonApprovedNonVatable)
{
    _sSDiscount1NonApprovedNonVatable = sSDiscount1NonApprovedNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount2NonApprovedNonVatable(Currency sSDiscount2NonApprovedNonVatable)
{
    _sSDiscount2NonApprovedNonVatable = sSDiscount2NonApprovedNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount3NonApprovedNonVatable(Currency sSDiscount3NonApprovedNonVatable)
{
    _sSDiscount3NonApprovedNonVatable = sSDiscount3NonApprovedNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount4NonApprovedNonVatable(Currency sSDiscount4NonApprovedNonVatable)
{
    _sSDiscount4NonApprovedNonVatable = sSDiscount4NonApprovedNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetSSDiscount5NonApprovedNonVatable(Currency sSDiscount5NonApprovedNonVatable)
{
    _sSDiscount5NonApprovedNonVatable = sSDiscount5NonApprovedNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetVATAmountNonVatable(Currency vATAmountNonVatable)
{
    _vATAmountNonVatable = vATAmountNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetNetSalesAmountNonVatable(Currency netSalesAmountNonVatable)
{
    _netSalesAmountNonVatable = netSalesAmountNonVatable;
}
//----------------------------------------------------------------------------------------
void TEstanciaMallField::SetInvoiceNumber(UnicodeString invoiceNumber)
{
    _invoiceNumber = invoiceNumber;
}
//----------------------------------------------------------------------------------------
TEstanciaMall::TEstanciaMall()
{
}
//-------------------------------------------------------------------------------------------------------------
std::list<TMallExportSalesData> TEstanciaMall::PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey)
{
    std::list<TMallExportSalesData> mallExportSalesData;
    try
    {
        TEstanciaMallField fieldData;
        int terminalNumber;
        UnicodeString tenantCode;
        std::list<TMallExportSettings>::iterator it;

        for(it = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); it != TGlobalSettings::Instance().mallInfo.MallSettings.end(); it++)
        {
            if(it->ControlName == "edMallTenantNo")
            {
                fieldData.TenantCode = it->Value;
            }
            else if(it->ControlName == "edMallTerminalNo")
            {
                fieldData.TerminalNumber = StrToInt(it->Value);
            }
        }

        for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
        {
                TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);

                //this will call all taxes and discount calculation inside it
                PrepareItem(Order, fieldData);

                //For SubOrder
                for (int i = 0; i < Order->SubOrders->Count; i++)
				{
					TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];

                    //this will call all taxes and discount calculation inside it
                    PrepareItem(CurrentSubOrder, fieldData);
                }
        }

        fieldData.OldAccumulatedSalesVatable = GetOldAccumulatedSales(paymentTransaction.DBTransaction, 5);
        fieldData.ControlNumber = 0;
        fieldData.NoOfSalesTransaction = (double)(fieldData.RefundAmountVatable <0 ? 0 : 1);
        fieldData.SalesType = 1;
        fieldData.Amount = (double)fieldData.NetSalesAmountVatable;
        fieldData.OldAccumulatedSalesNonVatable = GetOldAccumulatedSales(paymentTransaction.DBTransaction, 38);

        fieldData.DeductionVatable = (double)(fieldData.PromoSalesAmountVatable + fieldData.PWDDiscountVatable + fieldData.RefundAmountVatable + fieldData.ReturnedItemsAmountVatable +
                                fieldData.OtherTaxesVatable + fieldData.ServiceChargeAmountVatable + fieldData.AdjustmentDiscountVatable + fieldData.VoidAmountVatable +
                                fieldData.DiscountCardsVatable + fieldData.DeliveryChargesVatable + fieldData.GiftCertificatesChecksRedeemedVatable + fieldData.SSDiscount1Vatable +
                                fieldData.SSDiscount2Vatable + fieldData.SSDiscount3Vatable + fieldData.SSDiscount4Vatable + fieldData.SSDiscount5Vatable);

        fieldData.VATTaxAmountVatable = (double)(((fieldData.GrossAmountVatable - fieldData.DeductionVatable)*.12)/1.12);
        fieldData.NetSalesAmountVatable = (double)(fieldData.GrossAmountVatable - fieldData.DeductionVatable - fieldData.VATTaxAmountVatable);

        fieldData.DeductionNonVatable = (double)(fieldData.PromoSalesAmountNonVatable + fieldData.SCDDiscountNonVatable + fieldData.RefundAmountNonVatable +
                                        fieldData.ReturnedItemsAmountNonVatable + fieldData.OtherTaxesNonVatable + fieldData.ServiceChargeAmountNonVatable +
                                        fieldData.AdjustmentDiscountNonVatable + fieldData.VoidAmountNonVatable +  fieldData.DiscountCardsNonVatable +
                                        fieldData.DeliveryChargesNonVatable + fieldData.GiftCertificatesChecksRedeemedNonVatable + fieldData.SSDiscount1NonVatable +
                                        fieldData.SSDiscount2NonVatable + fieldData.SSDiscount3NonVatable + fieldData.SSDiscount4NonVatable + fieldData.SSDiscount5NonVatable);

        fieldData.VATTaxAmountNonVatable = 0;
        fieldData.NetSalesAmountNonVatable = (double)(fieldData.GrossAmountNonVatable - fieldData.DeductionNonVatable) ;
        fieldData.NewAccumulatedSalesVatable  = (double)(fieldData.OldAccumulatedSalesVatable + fieldData.NetSalesAmountVatable);
        fieldData.NewAccumulatedSalesNonVatable  = (double)(fieldData.OldAccumulatedSalesNonVatable + fieldData.NetSalesAmountNonVatable);
        fieldData.CoverCount = GetPatronCount(paymentTransaction);
        fieldData.InvoiceNumber = paymentTransaction.InvoiceNumber;

        //call For inserting into list
        InsertFieldInToList(paymentTransaction.DBTransaction, mallExportSalesData, fieldData, arcBillKey);
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return mallExportSalesData;
}
//----------------------------------------------------------------------------------------------
void TEstanciaMall::PrepareItem(TItemMinorComplete *Order, TEstanciaMallField &fieldData)
{
    //Create Taxes Object to collect all taxes details
    TEstanciaTaxes estanciaTaxes;

    //Get all Taxes stored them in TEstanciaTaxes type structure
    bool isVatable = IsItemVatable(Order, estanciaTaxes);

    /////////////////////////////////////////
    TEstanciaDiscounts estanciaDiscounts;

    PrepareAllDiscounts(Order, estanciaDiscounts, isVatable);

    //Set Discount and Taxes and store it into  TEstanciaMallField 's properies
    SetDiscountAndTaxes(fieldData, estanciaTaxes, estanciaDiscounts, Order, isVatable);
}
//----------------------------------------------------------------------------------------------
bool TEstanciaMall::IsItemVatable(TItemMinorComplete *Order, TEstanciaTaxes &estanciaTaxes)
{
    std::vector<BillCalculator::TTaxResult> taxInfomation = Order->BillCalcResult.Tax;
    bool isVatable = false;

    ///load all taxes value seperate
    for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
    {
        switch( itTaxes->TaxType )
        {
            case TTaxType::ttSale:
                isVatable = true;
                estanciaTaxes.salesTax += itTaxes->Value;
                break;
            case TTaxType::ttLocal:
                 estanciaTaxes.localTax += itTaxes->Value;
                 break;
            case TTaxType::ttProfit:
                 estanciaTaxes.profitTax += itTaxes->Value;
                 break;
        }
        if (Order->BillCalcResult.ServiceCharge.Percentage != 0)
        {
            estanciaTaxes.serviceCharge += Order->BillCalcResult.ServiceCharge.Value;
            if (Order->BillCalcResult.ServiceCharge.TaxPercentage != 0)
            {
                estanciaTaxes.serviceChargeTax += Order->BillCalcResult.ServiceCharge.TaxValue;
            }
        }
    }
}
//--------------------------------------------------------------------------------------------------
void TEstanciaMall::PrepareAllDiscounts(TItemMinorComplete *Order, TEstanciaDiscounts &estanciaDiscounts, bool &isVatable)
{
     //categories discounts according to group
    for (std::vector <TDiscount> ::const_iterator ptrDiscounts = Order->Discounts.begin(); ptrDiscounts != Order->Discounts.end();std::advance(ptrDiscounts, 1))
    {
        if(Order->DiscountValue_BillCalc(ptrDiscounts) == 0)
            continue;

        if(ptrDiscounts->DiscountGroupList.size())
        {
            if(ptrDiscounts->DiscountGroupList[0].Name == "Promo Discount")
            {
                estanciaDiscounts.promoDiscount += Order->DiscountValue_BillCalc(ptrDiscounts);
            }
            else if(ptrDiscounts->DiscountGroupList[0].Name == "Senior Citizen")
            {
                estanciaDiscounts.scdDiscount += Order->DiscountValue_BillCalc(ptrDiscounts);
                isVatable = false;
            }
            else if(ptrDiscounts->DiscountGroupList[0].Name == "Person with Disability")
                estanciaDiscounts.pwdDiscount += Order->DiscountValue_BillCalc(ptrDiscounts);
            else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 1")
                estanciaDiscounts.discountGroup1 += Order->DiscountValue_BillCalc(ptrDiscounts);
            else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 2")
                estanciaDiscounts.discountGroup2 += Order->DiscountValue_BillCalc(ptrDiscounts);
            else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 3")
                estanciaDiscounts.discountGroup3 += Order->DiscountValue_BillCalc(ptrDiscounts);
            else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 4")
                estanciaDiscounts.discountGroup4 += Order->DiscountValue_BillCalc(ptrDiscounts);
            else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 5")
                estanciaDiscounts.discountGroup5 += Order->DiscountValue_BillCalc(ptrDiscounts);
            else if(ptrDiscounts->DiscountGroupList[0].Name == "Non Approved Discount")
                estanciaDiscounts.totalNonApprovedDiscount += Order->DiscountValue_BillCalc(ptrDiscounts);
        }
    }
}
//--------------------------------------------------------------------------------------------------
void TEstanciaMall::SetDiscountAndTaxes(TEstanciaMallField &fieldData, TEstanciaTaxes estanciaTaxes, TEstanciaDiscounts estanciaDiscounts,
                                            TItemMinorComplete *order, bool isVatable)
{
    if(isVatable)
    {
        fieldData.GrossAmountVatable += (double)(order->PriceEach_BillCalc() + fabs(order->TotalAdjustment()));   //6
        fieldData.PromoSalesAmountVatable += (double)estanciaDiscounts.promoDiscount; //8
        fieldData.PWDDiscountVatable += (double)estanciaDiscounts.pwdDiscount;  //9
        fieldData.RefundAmountVatable += (double)order->GetQty() < 0 ? order->PriceEach_BillCalc() : 0;   //10
        fieldData.ReturnedItemsAmountVatable += 0;                //11
        fieldData.OtherTaxesVatable +=  (double)estanciaTaxes.localTax;                        //12
        fieldData.ServiceChargeAmountVatable +=  (double)estanciaTaxes.serviceCharge;  //13
        fieldData.AdjustmentDiscountVatable += 0;  //14
        fieldData.VoidAmountVatable += 0;//todo          //15
        fieldData.DiscountCardsVatable += 0;//todo         //16
        fieldData.DeliveryChargesVatable += 0;//todo       //17
        fieldData.GiftCertificatesChecksRedeemedVatable +=  0;//todo  //18
        fieldData.SSDiscount1Vatable += (double)estanciaDiscounts.discountGroup1;  //19
        fieldData.SSDiscount2Vatable += (double)estanciaDiscounts.discountGroup2;  //20
        fieldData.SSDiscount3Vatable += (double)estanciaDiscounts.discountGroup3;  //21
        fieldData.SSDiscount4Vatable += (double)estanciaDiscounts.discountGroup4;  //22
        fieldData.SSDiscount5Vatable += (double)estanciaDiscounts.discountGroup5;  //23
        fieldData.TotalOfallNonApprovedSDVatable += (double)estanciaDiscounts.totalNonApprovedDiscount;  //24
        fieldData.SSDiscount1NonApprovedVatable += 0;//todo       //25
        fieldData.SSDiscount2NonApprovedVatable +=  0;//todo      //26
        fieldData.SSDiscount3NonApprovedVatable +=  0;//todo      //27
        fieldData.SSDiscount4NonApprovedVatable  += 0;//todo      //28
        fieldData.SSDiscount5NonApprovedVatable +=  0;//todo      //29
    }
    else
    {
        fieldData.GrossAmountNonVatable += (double)(order->PriceLevelCustom > 0 ? order->PriceLevelCustom + fabs(order->TotalAdjustment()) : order->PriceLevel1 + fabs(order->TotalAdjustment()));   ////39
        fieldData.PromoSalesAmountNonVatable += (double)estanciaDiscounts.promoDiscount;    ///41
        fieldData.SCDDiscountNonVatable += (double)estanciaDiscounts.scdDiscount;      //42
        fieldData.RefundAmountNonVatable += (double)(order->GetQty() < 0 ? order->PriceEach_BillCalc() : 0);     //43
        fieldData.ReturnedItemsAmountNonVatable += 0;     //44
        fieldData.OtherTaxesNonVatable += (double)estanciaTaxes.localTax;     //45
        fieldData.ServiceChargeAmountNonVatable +=  (double)estanciaTaxes.serviceCharge;   //46
        fieldData.AdjustmentDiscountNonVatable  += 0;      //47
        fieldData.VoidAmountNonVatable += 0;//todo             //48
        fieldData.DiscountCardsNonVatable +=  0;//todo         //49
        fieldData.DeliveryChargesNonVatable +=  0;//todo       //50
        fieldData.GiftCertificatesChecksRedeemedNonVatable +=  0;//todo   //51
        fieldData.SSDiscount1NonVatable += (double)estanciaDiscounts.discountGroup1;  //52
        fieldData.SSDiscount2NonVatable += (double)estanciaDiscounts.discountGroup2;  //53
        fieldData.SSDiscount3NonVatable += (double)estanciaDiscounts.discountGroup3;  //54
        fieldData.SSDiscount4NonVatable += (double)estanciaDiscounts.discountGroup4;  //55
        fieldData.SSDiscount5NonVatable += (double)estanciaDiscounts.discountGroup5;  //56
        fieldData.TotalOfallNonApprovedSDiscountsNonVatable += (double)estanciaDiscounts.totalNonApprovedDiscount;    //57
        fieldData.SSDiscount1NonApprovedNonVatable += 0;//todo         ///58
        fieldData.SSDiscount2NonApprovedNonVatable += 0;//todo         //59
        fieldData.SSDiscount3NonApprovedNonVatable += 0;//todo         //60
        fieldData.SSDiscount4NonApprovedNonVatable += 0;//todo        //61
        fieldData.SSDiscount5NonApprovedNonVatable += 0;//todo        ///62
    }
}
//----------------------------------------------------------------------------------------------------------------------------
long TEstanciaMall::GenerateSaleKey(Database::TDBTransaction &dbTransaction)
{
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	dbTransaction.RegisterQuery(IBInternalQuery);
    long saleKey;
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_MALLEXPORT_SALE_KEY, 1) FROM RDB$DATABASE";
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
            saleKey = IBInternalQuery->Fields[0]->AsInteger;
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return saleKey;
}
//------------------------------------------------------------------------------------------------
int TEstanciaMall::GetPatronCount(TPaymentTransaction &paymentTransaction)
{
    //patron count
    int totalPatronCount = 0;
    std::vector <TPatronType> ::iterator ptrPatronTypes;
    for (ptrPatronTypes = paymentTransaction.Patrons.begin(); ptrPatronTypes != paymentTransaction.Patrons.end(); ptrPatronTypes++)
    {
        totalPatronCount += ptrPatronTypes->Count;
    }
    return totalPatronCount;
}
//---------------------------------------------------------------------------------
double TEstanciaMall::GetOldAccumulatedSales(Database::TDBTransaction &dbTransaction, int fieldIndex)
{
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	dbTransaction.RegisterQuery(IBInternalQuery);
    double oldAccumulatedSales = 0.00;
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT a.FIELD_INDEX, A.FIELD, A.FIELD_VALUE, a.Z_KEY "
                                        "FROM MALLEXPORT_SALES a "
                                        "WHERE A.FIELD_INDEX  = :FIELD_INDEX "
                                        "AND a.Z_KEY = (SELECT MAX(a.Z_KEY) FROM MALLEXPORT_SALES a) ";

        IBInternalQuery->ParamByName("FIELD_INDEX")->AsString = fieldIndex;
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
            oldAccumulatedSales = IBInternalQuery->Fields[2]->AsCurrency;
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return oldAccumulatedSales;
}
//--------------------------------------------------------------------------------------------------------
void TEstanciaMall::InsertFieldInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData,
                                        TEstanciaMallField fieldData, int arcBillKey)
{
    ///call function to insert all fields into list
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Tenant Code", "UnicodeString", fieldData.TenantCode, 1, arcBillKey);//01
    PushFieldsInToList(dbTransaction, mallExportSalesData, "POS Terminal Number", "int", fieldData.TerminalNumber, 2, arcBillKey);//02
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Date (mmddyyyy)", "TDateTime", Now().FormatString("mmddyyyy"), 3, arcBillKey);//03
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Old Accumulated Sales", "Currency", fabs(fieldData.OldAccumulatedSalesVatable), 4, arcBillKey);//04    //todo
    PushFieldsInToList(dbTransaction, mallExportSalesData, "New Accumulated Sales", "Currency", fabs(fieldData.NewAccumulatedSalesVatable), 5, arcBillKey);//05
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Gross Amount", "Currency", fabs(fieldData.GrossAmountVatable), 6, arcBillKey);//06
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Deductions", "Currency", fabs(fieldData.DeductionVatable), 7, arcBillKey);//07
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Promo Sale Amount", "Currency", fabs(fieldData.PromoSalesAmountVatable), 8, arcBillKey);//08
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total PWD Discount", "Currency", fabs(fieldData.PWDDiscountVatable), 9, arcBillKey);//09
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Refund Amount", "Currency", fabs(fieldData.RefundAmountVatable), 10, arcBillKey);//10
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Returned Items Amount", "Currency", fabs(fieldData.ReturnedItemsAmountVatable), 11, arcBillKey);//11
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Other Taxes", "Currency", fabs(fieldData.OtherTaxesVatable), 12, arcBillKey);//12
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Service Charge Amount", "Currency", fabs(fieldData.ServiceChargeAmountVatable), 13, arcBillKey);//13
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Adjustment Discount", "Currency", fabs(fieldData.AdjustmentDiscountVatable), 14, arcBillKey);//14
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Void Amount", "Currency", fabs(fieldData.VoidAmountVatable), 15, arcBillKey);//15
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Discount Cards", "Currency", fabs(fieldData.DiscountCardsVatable), 16, arcBillKey);//16
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Delivery Charges", "Currency", fabs(fieldData.DeliveryChargesVatable), 17, arcBillKey);//17
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Gift Certificates/Checks Redeemed", "Currency", fabs(fieldData.GiftCertificatesChecksRedeemedVatable), 18, arcBillKey);//18
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 1", "Currency", fabs(fieldData.SSDiscount1Vatable), 19, arcBillKey);//19
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 2", "Currency", fabs(fieldData.SSDiscount2Vatable), 20, arcBillKey);//20
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 3", "Currency", fabs(fieldData.SSDiscount3Vatable), 21, arcBillKey);//21
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 4", "Currency", fabs(fieldData.SSDiscount4Vatable), 22, arcBillKey);//22
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 5", "Currency", fabs(fieldData.SSDiscount5Vatable), 23, arcBillKey);//23
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total of all Non-Approved Store Discounts", "Currency", fabs(fieldData.TotalOfallNonApprovedSDVatable), 24, arcBillKey);//24
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 1", "Currency", fabs(fieldData.SSDiscount1NonVatable), 25, arcBillKey);//25
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 2", "Currency", fabs(fieldData.SSDiscount2NonVatable), 26, arcBillKey);//26
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 3", "Currency", fabs(fieldData.SSDiscount3NonVatable), 27, arcBillKey);//27
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 4", "Currency", fabs(fieldData.SSDiscount4NonVatable), 28, arcBillKey);//28
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 5", "Currency", fabs(fieldData.SSDiscount5NonVatable), 29, arcBillKey);//29
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total VAT/Tax Amount", "Currency", fabs(fieldData.VATTaxAmountVatable), 30, arcBillKey);//30
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Net Sales Amount", "Currency", fabs(fieldData.NetSalesAmountVatable), 31, arcBillKey);//31
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Cover Count", "int", fieldData.CoverCount, 32, arcBillKey);//32
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Control Number", "int", fieldData.ControlNumber, 33, arcBillKey);//33  //todo
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Number of Sales Transaction", "int", fieldData.NoOfSalesTransaction, 34, arcBillKey);//34  //todo
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Sales Type", "int", fieldData.SalesType, 35, arcBillKey);//35
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Amount", "Currency", fabs(fieldData.NetSalesAmountVatable), 36, arcBillKey);//36
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Old Accumulated Sales", "Currency", fabs(fieldData.OldAccumulatedSalesNonVatable), 37, arcBillKey);//37
    PushFieldsInToList(dbTransaction, mallExportSalesData, "New Accumulated Sales", "Currency", fabs(fieldData.NewAccumulatedSalesNonVatable), 38, arcBillKey);//38
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Gross Amount", "Currency", fabs(fieldData.GrossAmountNonVatable), 39, arcBillKey);//39
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Deductions", "Currency", fabs(fieldData.DeductionNonVatable), 40, arcBillKey);//40
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Promo Sale Amount", "Currency", fabs(fieldData.PromoSalesAmountNonVatable), 41, arcBillKey);//41
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total SCD Discount", "Currency", fabs(fieldData.SCDDiscountNonVatable), 42, arcBillKey);//42
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Refund Amount", "Currency", fabs(fieldData.RefundAmountNonVatable), 43, arcBillKey);//43
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Returned Items Amount", "Currency", fieldData.ReturnedItemsAmountNonVatable, 44, arcBillKey);//44
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Other Taxes", "Currency", fabs(fieldData.OtherTaxesNonVatable), 45, arcBillKey);//45
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Service Charge Amount", "Currency", fabs(fieldData.ServiceChargeAmountNonVatable), 46, arcBillKey);//46
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Adjustment Discount", "Currency", fabs(fieldData.AdjustmentDiscountNonVatable), 47, arcBillKey);//47
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Void Amount", "Currency", fabs(fieldData.VoidAmountNonVatable), 48, arcBillKey);//48
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Discount Cards", "Currency", fabs(fieldData.DiscountCardsNonVatable), 49, arcBillKey);//49
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Delivery Charges", "Currency", fabs(fieldData.DeliveryChargesNonVatable), 50, arcBillKey);//50
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Gift Certificates/Checks Redeemed", "Currency", fabs(fieldData.GiftCertificatesChecksRedeemedNonVatable), 51, arcBillKey);//51
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 1", "Currency", fabs(fieldData.SSDiscount1NonVatable), 52, arcBillKey);//52
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 2", "Currency", fabs(fieldData.SSDiscount2NonVatable), 53, arcBillKey);//53
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 3", "Currency", fabs(fieldData.SSDiscount3NonVatable), 54, arcBillKey);//54
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 4", "Currency", fabs(fieldData.SSDiscount4NonVatable), 55, arcBillKey);//55
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 5", "Currency", fabs(fieldData.SSDiscount5NonVatable), 56, arcBillKey);//56
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total of all Non-Approved Store Discounts", "Currency", fabs(fieldData.TotalOfallNonApprovedSDiscountsNonVatable), 57, arcBillKey);//57
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 1", "Currency", fabs(fieldData.SSDiscount1NonApprovedVatable), 58, arcBillKey);//58
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 2", "Currency", fabs(fieldData.SSDiscount2NonApprovedVatable), 59, arcBillKey);//59
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 3", "Currency", fabs(fieldData.SSDiscount3NonApprovedVatable), 60, arcBillKey);//60
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 4", "Currency", fabs(fieldData.SSDiscount4NonApprovedVatable), 61, arcBillKey);//61
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Store Specific Discount 5", "Currency", fabs(fieldData.SSDiscount5NonApprovedVatable), 62, arcBillKey);//62
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total VAT/Tax Amount", "Currency", fabs(fieldData.VATTaxAmountNonVatable), 63, arcBillKey);//63
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Total Net Sales Amount", "Currency", fabs(fieldData.NetSalesAmountNonVatable), 64, arcBillKey);//64
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Grand Total Net Sales", "Currency", fabs(fieldData.NetSalesAmountVatable + fieldData.NetSalesAmountNonVatable), 65, arcBillKey);//65
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Hour Code", "int", HourOf(Now()), 66, arcBillKey);//66
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Status", "int", 1, 67, arcBillKey);//67
    PushFieldsInToList(dbTransaction, mallExportSalesData, "Invoice Number", "UnicodeString", fieldData.InvoiceNumber, 68, arcBillKey);//68
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void TEstanciaMall::PushFieldsInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, UnicodeString field, UnicodeString dataType, UnicodeString fieldValue, int fieldIndex, int arcBillKey)
{
    try
    {
        TMallExportSalesData salesData;
        salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
        salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
        salesData.DataValue = fieldValue;
        salesData.Field = field;
        salesData.DataValueType = dataType;
        salesData.FieldIndex = fieldIndex;
        salesData.DateCreated = Now();
        salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
        salesData.ArcBillKey = arcBillKey;
        salesData.ZKey = 0;
        mallExportSalesData.push_back(salesData);
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//--------------------------------------------------------------------------------------------------------
TMallExportPrepareData TEstanciaMall::PrepareDataForExport(int zKey)
{
    //Create TMallExportPrepareData  for returning prepared data
    TMallExportPrepareData preparedData;

    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        //Set for inserting index. these indexes will be used for fetching data
        std::set<int> keyToCheck;
        std::set<int> keyToCheck2;

        //Indexes for which data will not selected
        int dailySalekeys[8] = {1, 2, 3, 33, 35, 66, 67, 68};
        int dailySalekeys2[2] = {33, 35};

        //insert these indexes into set.
        keyToCheck = InsertInToSet(dailySalekeys, 8);
        keyToCheck2 = InsertInToSet(dailySalekeys2, 2);

        //Prepare Data For Daily Sales File
        PrepareDataForDailySalesFile(dbTransaction, keyToCheck, keyToCheck2, preparedData, 1, zKey);

       //indexes for selecting total Net sale, patron count, etc
        int  hourIndexkeys[3] = {32,34,66};
        int hourIndexKeys2[2] = {65,34};

        //Clear the map because same map is used for many time insertion
        keyToCheck.clear();
        keyToCheck2.clear();

        //insert these indexes into set.
        keyToCheck = InsertInToSet(hourIndexkeys, 3);
        keyToCheck2 = InsertInToSet(hourIndexKeys2, 2);

        //Prepare Data For Hourly File
        PrepareDataForHourlySalesFile(dbTransaction, keyToCheck, keyToCheck2, 65, preparedData, 2, zKey);

        //indexes for selecting total Net sale, invoice number , status
        int invoiceIndex[3] = {65, 67, 68};

         //Clear the map because same map is used for many time insertion
        keyToCheck.clear();

        //insert these indexes into set.
        keyToCheck = InsertInToSet(invoiceIndex, 3);

        //Prepare Data For Invoice File
        PrepareDataForInvoiceSalesFile(dbTransaction, keyToCheck, 65, preparedData, 3, zKey);

       //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return preparedData;
}
//-----------------------------------------------------------------------------------------------
void TEstanciaMall::PrepareDataForInvoiceSalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, int indexKey2,
                                                    TMallExportPrepareData &prepareDataForInvoice, int index, int zKey)
{
    //Create List Of SalesData for invoice file
    std::list<TMallExportSalesData> salesDataForISF;
    try
    {
        ///Store First Letter of file name
        UnicodeString fileName = "I";

        //Seperate key with commas in the form of string.
        UnicodeString indexKeysList = GetFieldIndexList(indexKeys);

        ///Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Declare Set For storing index
        std::set<int>keysToSelect;

        //Create array for storing index by which file name will be prepared
        int  fileNameKeys[4] = {1, 2, 3, 33};

        //Store keys into set
        keysToSelect = InsertInToSet(fileNameKeys, 4);

        //Get file name according to field index.
        fileName = fileName + "" + GetFileName(dBTransaction, keysToSelect, zKey);

        //insert filename into map according to index and file type
        prepareDataForInvoice.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

        //insert indexes into array for fetching tenant code, date , terminal number, sales type
        int invoiceIndexKeys[4] = {1, 2, 3, 35};

        //clear the map.
        keysToSelect.clear();

        //Store keys into set
        keysToSelect = InsertInToSet(invoiceIndexKeys, 4);

         ///Load MallSetting For writing into file
        LoadMallSettingsForInvoiceFile(dBTransaction, prepareDataForInvoice, keysToSelect, index, zKey);

        //Query for selecting data for invoice file
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT a.ARCBILL_KEY, "
                                               "a.CREATED_BY, "
                                               "a.DATE_CREATED, "
                                               "a.FIELD, "
                                               "LPAD((CASE WHEN (a.FIELD_INDEX = 65) THEN 6 "
                                                          "WHEN (a.FIELD_INDEX = 67) THEN 7 "
                                                          "WHEN (a.FIELD_INDEX = 68) THEN 5 "
                                                          "ELSE (a.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                                "(CASE WHEN (a.FIELD_INDEX = 65) THEN (TOTALNETSALE.FIELD_VALUE) "
                                                      "WHEN (a.FIELD_INDEX = 68) THEN LPAD(a.FIELD_VALUE,5,0) "
                                                      "WHEN (a.FIELD_INDEX = 67) THEN LPAD(a.FIELD_VALUE,2,0) "
                                                      "ELSE a.FIELD_VALUE END ) FIELD_VALUE, "
                                                "a.VALUE_TYPE, "
                                                "meh.MM_NAME "
                                        "FROM MALLEXPORT_SALES a "
                                        "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                        "LEFT JOIN(SELECT ARCBILL_KEY, CAST(FIELD_VALUE*100 AS INT)FIELD_VALUE FROM "
                                            "(SELECT a.ARCBILL_KEY, CAST(a.FIELD_VALUE AS numeric(17,2))FIELD_VALUE "
                                                        "FROM MALLEXPORT_SALES a WHERE a.FIELD_INDEX = :FIELD_INDEX))TOTALNETSALE ON a.ARCBILL_KEY = TOTALNETSALE.ARCBILL_KEY "
                                "WHERE a.FIELD_INDEX IN(" + indexKeysList + " ) AND meh.IS_ACTIVE = :IS_ACTIVE ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY 1,5 ASC; ";

        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ParamByName("FIELD_INDEX")->AsInteger = indexKey2;

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
       {
          ///prepare sales data
          TMallExportSalesData salesData;
          salesData.ArcBillKey =  IBInternalQuery->Fields[0]->AsInteger;
          salesData.CreatedBy  =IBInternalQuery->Fields[1]->AsString;
          salesData.DateCreated =IBInternalQuery->Fields[2]->AsDateTime;
          salesData.Field = IBInternalQuery->Fields[3]->AsString;
          salesData.FieldIndex = IBInternalQuery->Fields[4]->AsInteger;
          salesData.DataValue = IBInternalQuery->Fields[4]->AsString + "" + IBInternalQuery->Fields[5]->AsString;
          salesData.DataValueType = IBInternalQuery->Fields[6]->AsString;

          //insert sales data object into   list
          salesDataForISF.push_back(salesData);
       }

       //insert list into TMallExportPrepareData's map
       prepareDataForInvoice.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, salesDataForISF ));
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------
void TEstanciaMall::PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, std::set<int> indexKeys2, int indexKey3,
                                                    TMallExportPrepareData &prepareDataForHSF, int index, int zKey)
{
    //Create List Of SalesData for hourly file
    std::list<TMallExportSalesData> prepareListForHSF;
    try
    {
        ///Store First Letter of file name
        UnicodeString fileName = "H";

        //Seperate key with commas in the form of string.
        UnicodeString indexKeysList = GetFieldIndexList(indexKeys);
        UnicodeString indexKeysList2 = GetFieldIndexList(indexKeys2);

        ///Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Declare Set For storing index
        std::set<int>keysToSelect;

        //Create array for storing index by which file name will be prepared
        int  fileNameKeys[4] = {1, 2, 3, 33};

        //Store keys into set
        keysToSelect = InsertInToSet(fileNameKeys, 4);

        //Get file name according to field index.
        fileName = fileName + "" + GetFileName(dBTransaction, keysToSelect, zKey);

        //insert filename into map according to index and file type
        prepareDataForHSF.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

         //insert indexes into array for fetching tenant code, date , terminal number
        int hourIndexKeys[3] = {1, 2, 3};

        //clear the map
        keysToSelect.clear();

        //Store keys into set
        keysToSelect = InsertInToSet(hourIndexKeys, 3);

        ///Load MallSetting For writing into file
        LoadMallSettingsForFile(dBTransaction, prepareDataForHSF, keysToSelect, index, zKey);

        //Query for selecting data for hourly file
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                             "SELECT LPAD((CASE WHEN (HOURLYDATA.FIELD_INDEX = 32) THEN 7  "
                                            "WHEN (HOURLYDATA.FIELD_INDEX = 34) THEN 6 "
                                            "WHEN (HOURLYDATA.FIELD_INDEX = 66) THEN 4 "
                                            "ELSE (HOURLYDATA.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                    "HOURLYDATA.FIELD, "
                                    "SUM(HOURLYDATA.FIELD_VALUE) FIELD_VALUE , "
                                    "HOURLYDATA.VALUE_TYPE , "
                                    "HOURLYDATA.Hour_code  "
                            "FROM "
                                "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS int) FIELD_VALUE, a.VALUE_TYPE, "
                                                        "meh.MM_NAME,Extract (Hour From a.DATE_CREATED) Hour_code "
                                 "FROM MALLEXPORT_SALES a "
                                 "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                 "WHERE a.FIELD_INDEX IN(" + indexKeysList + ") AND meh.IS_ACTIVE = :IS_ACTIVE  ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA  "
                            "GROUP BY 1,2,4,5 "

       "UNION ALL "

                            "SELECT LPAD((CASE WHEN (HOURLYDATA.FIELD_INDEX = 65) THEN 5 ELSE (HOURLYDATA.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                    "HOURLYDATA.FIELD, CAST(SUM(HOURLYDATA.FIELD_VALUE)*100 AS INT ) FIELD_VALUE , HOURLYDATA.VALUE_TYPE, HOURLYDATA.Hour_code "
                            "FROM "
                                    "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE, meh.MM_NAME, "
                                            "Extract (Hour From a.DATE_CREATED) Hour_code "
                                     "FROM MALLEXPORT_SALES a "
                                     "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID"
                                    " WHERE a.FIELD_INDEX = :FIELD_INDEX AND meh.IS_ACTIVE = :IS_ACTIVE ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA "
                            "GROUP BY 1,2,4 ,5 "
                            "ORDER BY 5 ASC, 1 ASC ";

        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ParamByName("FIELD_INDEX")->AsInteger = indexKey3;

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;
          salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsCurrency;
          salesData.DataValueType = IBInternalQuery->Fields[3]->AsString;
          salesData.MallExportSalesId = IBInternalQuery->Fields[4]->AsInteger;
          prepareListForHSF.push_back(salesData);
        }

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                                "SELECT LPAD((CASE WHEN (HOURLYDATA.FIELD_INDEX = 34) THEN 9  "
                                                  "WHEN (HOURLYDATA.FIELD_INDEX = 65) THEN 8 "
                                                  "ELSE (HOURLYDATA.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                        "HOURLYDATA.FIELD, "
                                        "CAST(SUM(CASE WHEN HOURLYDATA.FIELD_INDEX = 65 THEN (HOURLYDATA.FIELD_VALUE*100) ELSE (HOURLYDATA.FIELD_VALUE) END )  AS INT ) FIELD_VALUE, "
                                         "HOURLYDATA.VALUE_TYPE "
                                "FROM "
                                "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE "
                                "FROM MALLEXPORT_SALES a  "
                                "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                "WHERE a.FIELD_INDEX IN(" + indexKeysList2 + ") AND meh.IS_ACTIVE = :IS_ACTIVE ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA "
                                "GROUP BY 1,2,4 "
                                "ORDER BY 1 ASC ";

        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           ///prepare sales data
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;
          salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsCurrency;
          salesData.DataValueType = IBInternalQuery->Fields[3]->AsString;

          //insert prepared data into list
          prepareListForHSF.push_back(salesData);
        }

        //insert list into TMallExportPrepareData's map
        prepareDataForHSF.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, prepareListForHSF ));
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------------------
void TEstanciaMall::PrepareDataForDailySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, std::set<int> indexKeys2,
                                                   TMallExportPrepareData &prepareDataForDSF, int index, int zKey)
{
    //Create List Of SalesData for hourly file
    std::list<TMallExportSalesData> prepareListForDSF;
    try
    {
        ///Store First Letter of file name ie; file type
        UnicodeString fileName = "D";

        //Seperate key with commas in the form of string.
        UnicodeString indexKeysList = GetFieldIndexList(indexKeys);
        UnicodeString indexKeysList2 = GetFieldIndexList(indexKeys2);

        //Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Declare Set For storing index
        std::set<int>keysToSelect;

        //Create array for storing index by which file name will be prepared
        int  fileNameKeys[4] = {1, 2, 3, 33};

         //Store keys into set
        keysToSelect = InsertInToSet(fileNameKeys, 4);

        //Get file name according to field index.
        fileName = fileName + "" + GetFileName(dBTransaction, keysToSelect, zKey);

        //insert filename into map according to index and file type
        prepareDataForDSF.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

         //insert indexes into array for fetching tenant code, date , terminal number
        int dailyIndexKeys[3] = {1, 2, 3};

        //clear the map
        keysToSelect.clear();

        //Store keys into set
        keysToSelect = InsertInToSet(dailyIndexKeys, 3);

        ///Load MallSetting For writing into file
        LoadMallSettingsForFile(dBTransaction, prepareDataForDSF, keysToSelect, index, zKey);

        //Query for fetching data for writing into daily sales file.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD, CAST(SUM(DAILYDATA.FIELD_VALUE)*100 AS INT) FIELD_VALUE , DAILYDATA.VALUE_TYPE, DAILYDATA.Z_KEY, DAILYDATA.MM_NAME "
                                      "FROM "
                                            "(SELECT a.ARCBILL_KEY, a.FIELD, LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE, meh.MM_NAME, MAX(A.Z_KEY) Z_KEY "
                                             "FROM MALLEXPORT_SALES a "
                                             "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                             "WHERE a.FIELD_INDEX NOT IN(" + indexKeysList + ") AND meh.IS_ACTIVE = :IS_ACTIVE  ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, meh.MM_NAME, a.FIELD_VALUE  "
                                             "ORDER BY A.ARCBILL_KEY ASC )DAILYDATA "
                                    "GROUP BY 1,2,4,5,6 "

                                    "UNION ALL "

                                     "SELECT LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, a.FIELD, cast(a.FIELD_VALUE as int ) FIELD_VALUE , a.VALUE_TYPE, a.Z_KEY, meh.MM_NAME  "
                                     "FROM "
                                        "MALLEXPORT_SALES a inner join MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                        "where a.FIELD_INDEX IN( " + indexKeysList2 + " ) AND meh.IS_ACTIVE = :IS_ACTIVE ";    //TODO AFTER DISCUSSION
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY 1,2,3,4,5,6 "
                                    "ORDER BY 1 ASC  ";

        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
       {
           ///prepare sales data
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;
          salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsCurrency;
          salesData.DataValueType = IBInternalQuery->Fields[3]->AsString;
          salesData.ZKey = IBInternalQuery->Fields[4]->AsInteger;

          //insert prepared data into list
          prepareListForDSF.push_back(salesData);
       }

       //insert list into TMallExportPrepareData's map
       prepareDataForDSF.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, prepareListForDSF ));
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------------------
void TEstanciaMall::LoadMallSettingsForFile(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareData, std::set<int> keysToSelect,
                                                int index, int zKey)
{
    try
    {
        //Create List Of mallExportSetting
        std::list<TMallExportSettings> mallSettings;

        //Seperate key with commas in the form of string.
        UnicodeString indexKeysList = GetFieldIndexList(keysToSelect);

        //Register Query.
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Query for fetching setting for files according to file type and index keys.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, a.FIELD, "
                                                "CASE WHEN(a.FIELD_INDEX = 1) THEN LPAD(a.FIELD_VALUE,5,0) "
                                                "WHEN(a.FIELD_INDEX = 2) THEN LPAD(a.FIELD_VALUE,2,0) "
                                                "ELSE (a.FIELD_VALUE) END FIELD_VALUE, a.VALUE_TYPE "
                                      "FROM MALLEXPORT_SALES a "
                                      "INNER JOIN MALLEXPORT_HEADER MEH ON A.FIELD_INDEX = MEH.MALLEXPORT_HEADER_ID "
                                      "WHERE a.FIELD_INDEX IN(" + indexKeysList + ") AND meh.IS_ACTIVE = :IS_ACTIVE ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY 1,2,3,4 ";

        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
          //Prepare Setting data
          TMallExportSettings settings;
          settings.Name =   IBInternalQuery->Fields[1]->AsString;
          settings.Value  = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsString;
          settings.ValueType = IBInternalQuery->Fields[3]->AsString;

          //Insert setting data into list.
          mallSettings.push_back(settings);
        }

        ////insert list into TMallExportPrepareData's setting map
        prepareData.MallSettings.insert( std::pair<int,list<TMallExportSettings> >(index, mallSettings));
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------------------------------------------
UnicodeString TEstanciaMall::GetFieldIndexList(std::set<int> indexKeys)
{
    //Seperate keys with commas and store in the form of string and return
    std::set<int>::iterator indexKeysIt = indexKeys.begin();
    UnicodeString indexKeyList = IntToStr(*indexKeysIt);
    indexKeysIt++;
    for(; indexKeysIt != indexKeys.end(); indexKeysIt++)
    {
        indexKeyList += ", " + IntToStr(*indexKeysIt);
    }
    return indexKeyList;
}
//----------------------------------------------------------------------------------------------------------------------------
IExporterInterface* TEstanciaMall::CreateExportMedium()
{
    UnicodeString exportType = GetExportType();
    if(exportType == ".txt")
    {
        return new TMallExportTextFile;
    }
    else
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Export Type not found");
		throw;
    }
}
//--------------------------------------------------------------------------------------------------------------------
UnicodeString TEstanciaMall::GetFileName(Database::TDBTransaction &dBTransaction, std::set<int> keysToSelect, int zKey)
{
    UnicodeString fileName = "";
    try
    {
        //create list of mallexportsetting
        std::list<TMallExportSettings> mallSettings;

        //seperate keys with comma in the form of string
        UnicodeString indexKeysList = GetFieldIndexList(keysToSelect);

        //Register Query.
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Query for fetching file name.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT a.FIELD_INDEX, a.FIELD, CASE WHEN(a.FIELD_INDEX = 1) THEN LPAD(a.FIELD_VALUE,5,0) "
                                                                        "WHEN(a.FIELD_INDEX = 2) THEN LPAD(a.FIELD_VALUE,2,0) "
                                                                            "WHEN(a.FIELD_INDEX = 33) THEN LPAD(a.FIELD_VALUE,3,0) "
                                                                        "ELSE (a.FIELD_VALUE) END FIELD_VALUE, "
                                                "a.VALUE_TYPE , a.Z_KEY "
                                    "FROM MALLEXPORT_SALES a "
                                    "INNER JOIN MALLEXPORT_HEADER MEH ON A.FIELD_INDEX = MEH.MALLEXPORT_HEADER_ID "
                                    "WHERE a.FIELD_INDEX IN(" + indexKeysList + " ) AND meh.IS_ACTIVE = :IS_ACTIVE ";
        if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY 1,2,3,4,5 "
                                                                "ORDER BY 1 ASC ";

        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            fileName = fileName + "" + IBInternalQuery->Fields[2]->AsString;

            if(IBInternalQuery->Fields[0]->AsInteger == 3)
                fileName = fileName + ".";
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return fileName;
}
//--------------------------------------------------------------------------------------------------------------------
void TEstanciaMall::LoadMallSettingsForInvoiceFile(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareData, std::set<int> keysToSelect,
                                                        int index, int zKey)
{
    try
    {
        //Create list of mallexport setting
        std::list<TMallExportSettings> mallSettings;

        //seperate keys with comm in the form of string
        UnicodeString indexKeysList = GetFieldIndexList(keysToSelect);

        //Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        //Query for fetching setting for invoice file.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT LPAD((CASE WHEN a.FIELD_INDEX = 35 THEN 2 "
                                                    "WHEN a.FIELD_INDEX = 2 THEN 4 "
                                                    "ELSE a.FIELD_INDEX END),2,0) FIELD_INDEX, "
                                            "a.FIELD, "
                                            "CASE WHEN(a.FIELD_INDEX = 1) THEN LPAD(a.FIELD_VALUE,5,0) "
                                                "WHEN(a.FIELD_INDEX = 2) THEN LPAD(a.FIELD_VALUE,2,0) "
                                                    "ELSE (a.FIELD_VALUE) END FIELD_VALUE, a.VALUE_TYPE "
                                      "FROM MALLEXPORT_SALES a "
                                      "INNER JOIN MALLEXPORT_HEADER MEH ON A.FIELD_INDEX = MEH.MALLEXPORT_HEADER_ID "
                                      "WHERE a.FIELD_INDEX IN(" + indexKeysList + " ) AND meh.IS_ACTIVE = :IS_ACTIVE ";
         if(zKey == 0)
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) ";
        }
        else
        {
            IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "AND a.Z_KEY = :Z_KEY ";
        }

        IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + "GROUP BY 1,2,3,4 ";

        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";

        if(zKey != 0)
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zKey;

        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
          TMallExportSettings settings;
          settings.Name =   IBInternalQuery->Fields[1]->AsString;
          settings.Value  = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsString;
          settings.ValueType = IBInternalQuery->Fields[3]->AsString;

          //push setting into list
          mallSettings.push_back(settings);
        }

        //insert setting into map
        prepareData.MallSettings.insert( std::pair<int,list<TMallExportSettings> >(index, mallSettings));
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//--------------------------------------------------------------------------------------------------------------------
std::set<int> TEstanciaMall::InsertInToSet(int arr[], int size)
{
    //function for inserting array into set.
    std::set<int> keyToCheck;
    for(int index = 0; index < size; index++)
            keyToCheck.insert(arr[index]);

    return keyToCheck;
}
//------------------------------------------------------------------------------------------------------------------------
UnicodeString TEstanciaMall::GetExportType()
{
    UnicodeString typeOfFile = "";
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        ///Register Query
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dbTransaction.RegisterQuery(IBInternalQuery);

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT MES.NAME, MES.MALLEXPORT_SETTING_KEY, MSP.MALL_ID, MSV.FIELD_VALUE  "
                                     "FROM MALLEXPORT_SETTINGS MES "
                                     "INNER JOIN MALLEXPORT_SETTINGS_MAPPING MSP ON MES.MALLEXPORT_SETTING_KEY = MSP.MALLEXPORT_SETTING_ID "
                                     "INNER JOIN MALLEXPORT_SETTINGS_VALUES MSV ON MES.MALLEXPORT_SETTING_KEY = MSV.MALLEXPORTSETTING_ID "
                                     "WHERE MES.NAME = :NAME AND MSP.MALL_ID = :MALL_ID";

        IBInternalQuery->ParamByName("NAME")->AsString = "TYPE_OF_FILE";
        IBInternalQuery->ParamByName("MALL_ID")->AsInteger = 1;
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
            typeOfFile = IBInternalQuery->Fields[3]->AsString;
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return typeOfFile;
}
//-----------------------------------------------------------------------------------------------------------------------------
