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


TMallExportPrepareData TEstanciaMall::PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey)
{
    Currency promoDiscount = 0.00;
    Currency scdDiscount = 0.00;
    Currency pwdDiscount = 0.00;
    Currency discountGroup1 = 0.00;
    Currency discountGroup2 = 0.00;
    Currency discountGroup3 = 0.00;
    Currency discountGroup4 = 0.00;
    Currency discountGroup5 = 0.00;
    Currency totalNonApprovedDiscount = 0.00;
    Currency salesTax = 0.00;
    Currency serviceCharge = 0.00;
    Currency serviceChargeTax = 0.00;
    Currency localTax = 0.00;
    Currency profitTax = 0.00;

    TEstanciaMallField fieldData;

    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    TMallExportPrepareData mallExportData;
    TMallExportSalesData salesData;
    int terminalNumber;
    std::list<TMallExportSettings>::iterator it;

    //01 load Tenant code Row
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;

    for(it = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); it != TGlobalSettings::Instance().mallInfo.MallSettings.end(); it++)
    {
        if(it->ControlName == "edTenantNo1")
        {
            salesData.DataValue = it->Value;
        }
        else if(it->ControlName == "edTerminalNo1")
        {
            terminalNumber = StrToInt(it->Value);
        }
    }
    salesData.Field = "Tenant Code";
    salesData.DataValueType = "UnicodeString";
    salesData.FieldIndex = 1;
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //02 Load pos Terminal Number Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = terminalNumber;
    salesData.FieldIndex = 2;
    salesData.Field = "POS Terminal Number";
    salesData.DataValueType = "Integer";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //03 Load Date Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = Now().FormatString("mmddyyyy");
    salesData.Field = "Date (mmddyyyy)";
    salesData.FieldIndex = 3;
    salesData.DataValueType = "TDateTime";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //04 Load Old Accumulated Row;          //todo
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = Now().FormatString("mmddyyyy");
    salesData.Field = "Old Accumulated Sales";
    salesData.FieldIndex = 4;
    salesData.DataValueType = "TDateTime";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
    {
			TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);

            std::vector<BillCalculator::TTaxResult> taxInfomation = Order->BillCalcResult.Tax;
            bool isVatable = false;
            salesTax = localTax = profitTax = serviceCharge = serviceChargeTax = 0.00;
            promoDiscount = scdDiscount =  pwdDiscount = discountGroup1 = discountGroup2 = discountGroup3 = discountGroup4 = discountGroup5 = totalNonApprovedDiscount = 0.00;
            ///load all taxes value seperate
            for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
	        {
                switch( itTaxes->TaxType )
                {
                    case TTaxType::ttSale:
                        isVatable = true;
                        salesTax = itTaxes->Value;
                        break;
                    case TTaxType::ttLocal:
                         localTax = itTaxes->Value;
                         break;
                    case TTaxType::ttProfit:
                         profitTax = itTaxes->Value;
                         break;
                }
                if (Order->BillCalcResult.ServiceCharge.Percentage != 0)
	            {
                    serviceCharge = Order->BillCalcResult.ServiceCharge.Value;
                    if (Order->BillCalcResult.ServiceCharge.TaxPercentage != 0)
		            {
                        serviceChargeTax = Order->BillCalcResult.ServiceCharge.TaxValue;
                    }
                }
            }

            //categories discounts according to group
            for (std::vector <TDiscount> ::const_iterator ptrDiscounts = Order->Discounts.begin(); ptrDiscounts != Order->Discounts.end();std::advance(ptrDiscounts, 1))
	        {
                if(Order->DiscountValue_BillCalc(ptrDiscounts) == 0)
                    continue;

                if(ptrDiscounts->DiscountGroupList.size())
                {
                    if(ptrDiscounts->DiscountGroupList[0].Name == "Promo Discount")
                    {
                        promoDiscount += Order->DiscountValue_BillCalc(ptrDiscounts);
                    }
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Senior Citizen")
                    {
                        scdDiscount += Order->DiscountValue_BillCalc(ptrDiscounts);
                        isVatable = false;
                    }
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Person with Disability")
                        pwdDiscount += Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 1")
                        discountGroup1 += Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 2")
                        discountGroup2 += Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 3")
                        discountGroup3 += Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 4")
                        discountGroup4 += Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 5")
                        discountGroup5 += Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Other Discount")
                        totalNonApprovedDiscount += Order->DiscountValue_BillCalc(ptrDiscounts);
                }
            }

            if(isVatable)
            {
                fieldData.GrossAmountVatable += Order->PriceEach_BillCalc() + fabs(Order->TotalAdjustment());   //6
                fieldData.PromoSalesAmountVatable += promoDiscount; //8
                fieldData.PWDDiscountVatable += pwdDiscount;  //9
                fieldData.RefundAmountVatable += Order->GetQty() < 0 ? Order->PriceEach_BillCalc() : 0;   //10
                fieldData.ReturnedItemsAmountVatable += 0;                //11
                fieldData.OtherTaxesVatable +=  localTax;                        //12
                fieldData.ServiceChargeAmountVatable +=  serviceCharge;  //13
                fieldData.AdjustmentDiscountVatable += 0;  //14
                fieldData.VoidAmountVatable += 0;//todo          //15
                fieldData.DiscountCardsVatable += 0;//todo         //16
                fieldData.DeliveryChargesVatable += 0;//todo       //17
                fieldData.GiftCertificatesChecksRedeemedVatable +=  0;//todo  //18
                fieldData.SSDiscount1Vatable += discountGroup1;  //19
                fieldData.SSDiscount2Vatable += discountGroup2;  //20
                fieldData.SSDiscount3Vatable += discountGroup3;  //21
                fieldData.SSDiscount4Vatable += discountGroup4;  //22
                fieldData.SSDiscount5Vatable += discountGroup5;  //23
                fieldData.TotalOfallNonApprovedSDVatable += totalNonApprovedDiscount;  //24
                fieldData.SSDiscount1NonApprovedVatable += 0;//todo       //25
                fieldData.SSDiscount2NonApprovedVatable +=  0;//todo      //26
                fieldData.SSDiscount3NonApprovedVatable +=  0;//todo      //27
                fieldData.SSDiscount4NonApprovedVatable  += 0;//todo      //28
                fieldData.SSDiscount5NonApprovedVatable +=  0;//todo      //29
            }
            else
            {
                fieldData.GrossAmountNonVatable += Order->PriceLevelCustom > 0 ? Order->PriceLevelCustom + fabs(Order->TotalAdjustment()) : Order->PriceLevel1 + fabs(Order->TotalAdjustment());   ////39
                fieldData.PromoSalesAmountNonVatable += promoDiscount;    ///41
                fieldData.SCDDiscountNonVatable += scdDiscount;      //42
                fieldData.RefundAmountNonVatable += Order->GetQty() < 0 ? Order->PriceEach_BillCalc() : 0;     //43
                fieldData.ReturnedItemsAmountNonVatable += 0;     //44
                fieldData.OtherTaxesNonVatable += localTax;     //45
                fieldData.ServiceChargeAmountNonVatable +=  serviceCharge;   //46
                fieldData.AdjustmentDiscountNonVatable  += 0;      //47
                fieldData.VoidAmountNonVatable += 0;//todo             //48
                fieldData.DiscountCardsNonVatable +=  0;//todo         //49
                fieldData.DeliveryChargesNonVatable +=  0;//todo       //50
                fieldData.GiftCertificatesChecksRedeemedNonVatable +=  0;//todo   //51
                fieldData.SSDiscount1NonVatable += discountGroup1;  //52
                fieldData.SSDiscount2NonVatable += discountGroup2;  //53
                fieldData.SSDiscount3NonVatable += discountGroup3;  //54
                fieldData.SSDiscount4NonVatable += discountGroup4;  //55
                fieldData.SSDiscount5NonVatable += discountGroup5;  //56
                fieldData.TotalOfallNonApprovedSDiscountsNonVatable += totalNonApprovedDiscount;    //57
                fieldData.SSDiscount1NonApprovedNonVatable += 0;//todo         ///58
                fieldData.SSDiscount2NonApprovedNonVatable += 0;//todo         //59
                fieldData.SSDiscount3NonApprovedNonVatable += 0;//todo         //60
                fieldData.SSDiscount4NonApprovedNonVatable += 0;//todo        //61
                fieldData.SSDiscount5NonApprovedNonVatable += 0;//todo        ///62
            }
    }
    ///7
    fieldData.DeductionVatable = fieldData.PromoSalesAmountVatable + fieldData.PWDDiscountVatable + fieldData.RefundAmountVatable + fieldData.ReturnedItemsAmountVatable +
                            fieldData.OtherTaxesVatable + fieldData.ServiceChargeAmountVatable + fieldData.AdjustmentDiscountVatable + fieldData.VoidAmountVatable +
                            fieldData.DiscountCardsVatable + fieldData.DeliveryChargesVatable + fieldData.GiftCertificatesChecksRedeemedVatable + fieldData.SSDiscount1Vatable +
                            fieldData.SSDiscount2Vatable + fieldData.SSDiscount3Vatable + fieldData.SSDiscount4Vatable + fieldData.SSDiscount5Vatable;
    fieldData.VATTaxAmountVatable = (((fieldData.GrossAmountVatable - fieldData.DeductionVatable)*.12)/1.12);        //30
    fieldData.NetSalesAmountVatable = fieldData.GrossAmountVatable - fieldData.DeductionVatable - fieldData.VATTaxAmountVatable;  ////31

    ///40
    fieldData.DeductionNonVatable = fieldData.PromoSalesAmountNonVatable + fieldData.SCDDiscountNonVatable + fieldData.RefundAmountNonVatable + fieldData.ReturnedItemsAmountNonVatable +
                            fieldData.OtherTaxesNonVatable + fieldData.ServiceChargeAmountNonVatable + fieldData.AdjustmentDiscountNonVatable + fieldData.VoidAmountNonVatable +
                            fieldData.DiscountCardsNonVatable + fieldData.DeliveryChargesNonVatable + fieldData.GiftCertificatesChecksRedeemedNonVatable + fieldData.SSDiscount1NonVatable +
                            fieldData.SSDiscount2NonVatable + fieldData.SSDiscount3NonVatable + fieldData.SSDiscount4NonVatable + fieldData.SSDiscount5NonVatable;
    fieldData.VATTaxAmountNonVatable = 0;  ///63
    fieldData.NetSalesAmountNonVatable = fieldData.GrossAmountVatable - fieldData.DeductionNonVatable ;  //64
    fieldData.NewAccumulatedSalesVatable  = fieldData.OldAccumulatedSalesVatable + fieldData.NetSalesAmountVatable; //5
    fieldData.NewAccumulatedSalesNonVatable  = fieldData.OldAccumulatedSalesNonVatable + fieldData.NetSalesAmountNonVatable; //38

     //05 Load New Accumulated Sale Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = NewAccumulatedSalesVatable;
    salesData.Field = "New Accumulated Sales";
    salesData.FieldIndex = 5;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);


    //06 Total Gross Amount Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalGrossAmountVatable;
    salesData.Field = "Total Gross Amount";
    salesData.FieldIndex = 6;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //07 Total Deductions Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalDeductionVatable;
    salesData.Field = "Total Deductions";
    salesData.FieldIndex = 7;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //08 Total Promo Sale Amount Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalPromoSalesAmountVatable;
    salesData.Field = "Total Promo Sale Amount";
    salesData.FieldIndex = 8;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //09 Total PWD Discount Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalPWDDiscountVatable;
    salesData.Field = "Total PWD Discount";
    salesData.FieldIndex = 9;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //10 Total Refund Amount;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalRefundAmountVatable;
    salesData.Field = "Total Refund Amount ";
    salesData.FieldIndex = 10;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //11 Total Refund Amount;    //todo
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalReturnedItemsAmountVatable;
    salesData.Field = "Total Returned Items Amount";
    salesData.FieldIndex = 11;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //12 Total Other taxes //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalOtherTaxesVatable;
    salesData.Field = "Total Other Taxes";
    salesData.FieldIndex = 12;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //13 Total Service Charge Amount //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalServiceChargeAmountVatable;
    salesData.Field = "Total Service Charge Amount ";
    salesData.FieldIndex = 13;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //14 Total Adjustment Discount //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalAdjustmentDiscountVatable;
    salesData.Field = "Total Adjustment Discount";
    salesData.FieldIndex = 14;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

      //15 Total Void Amount //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalVoidAmountVatable;
    salesData.Field = "Total Void Amount";
    salesData.FieldIndex = 15;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

      //16 Total Discount Cards  //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalDiscountCardsVatable;
    salesData.Field = "Total Discount Cards";
    salesData.FieldIndex = 16;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //17 Total Delivery Charges  //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalDeliveryChargesVatable;
    salesData.Field = "Total Delivery Charges";
    salesData.FieldIndex = 17;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //18 Total Gift Cards  //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalGiftCertificatesChecksRedeemedVatable;
    salesData.Field = "Total Gift Certificates/Checks Redeemed";
    salesData.FieldIndex = 18;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //19 Store Specific Discount 1  //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount1Vatable;
    salesData.Field = "Store Specific Discount 1";
    salesData.FieldIndex = 19;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //20 Store Specific Discount 2  //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount2Vatable;
    salesData.Field = "Store Specific Discount 2";
    salesData.FieldIndex = 20;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //21 Store Specific Discount 3  //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount3Vatable;
    salesData.Field = "Store Specific Discount 3";
    salesData.FieldIndex = 21;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //22 Store Specific Discount 4 //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount4Vatable;
    salesData.Field = "Store Specific Discount 4";
    salesData.FieldIndex = 22;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //23 Store Specific Discount 5 //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount5Vatable;
    salesData.Field = "Store Specific Discount 5";
    salesData.FieldIndex = 23;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //24 Total of all Non-Approved Store Discounts //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalofallNonApprovedStoreDiscountsVatable;
    salesData.Field = "Total of all Non-Approved Store Discounts";
    salesData.FieldIndex = 24;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //25 Store Specific Discount1//todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount1Vatable;
    salesData.Field = "Store Specific Discount 1";
    salesData.FieldIndex = 25 ;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //26 Store Specific Discount2//todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount2Vatable;
    salesData.Field = "Store Specific Discount 2";
    salesData.FieldIndex = 26 ;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //27 Store Specific Discount3//todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount3Vatable;
    salesData.Field = "Store Specific Discount 3";
    salesData.FieldIndex = 27 ;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //28 Store Specific Discount4//todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount4Vatable;
    salesData.Field = "Store Specific Discount 4";
    salesData.FieldIndex = 28 ;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //29 Store Specific Discount5//todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount5Vatable;
    salesData.Field = "Store Specific Discount 5";
    salesData.FieldIndex = 29 ;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //30 Total VAT/Tax Amount//todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalVATTaxAmountVatable;
    salesData.Field = "Total VAT/Tax Amount";
    salesData.FieldIndex = 30 ;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //31 Total Net Sales Amount ;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalNetSalesAmountVatable;
    salesData.Field = "Total Net Sales Amount";
    salesData.FieldIndex = 31 ;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //32 Total Cover Count  ;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = GetPatronCount(paymentTransaction);
    salesData.Field = "Total Cover Count";
    salesData.FieldIndex = 32 ;
    salesData.DataValueType = "int";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //33 Control Number //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalNetSalesAmountVatable;
    salesData.Field = "Control Number";
    salesData.FieldIndex = 33 ;
    salesData.DataValueType = "int";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

      //34 Total Number of Sales Transaction //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalRefundAmountVatable < 0 ? 0 :1;
    salesData.Field = "Total Number of Sales Transaction";
    salesData.FieldIndex = 34 ;
    salesData.DataValueType = "int";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //35 Sales Type //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = 1;
    salesData.Field = "Sales Type";
    salesData.FieldIndex = 35 ;
    salesData.DataValueType = "int";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //36 Sales Type //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalNetSalesAmountVatable;
    salesData.Field = "Sales Type";
    salesData.FieldIndex = 36 ;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //37 Old Accumulated Sales //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = 0;///todo
    salesData.Field = "Old Accumulated Sales";
    salesData.FieldIndex = 36 ;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //38 Load New Accumulated Sale Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = NewAccumulatedSalesNonVatable;
    salesData.Field = "New Accumulated Sales";
    salesData.FieldIndex = 38;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);


    //39 Total Gross Amount Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalGrossAmountNonVatable;
    salesData.Field = "Total Gross Amount";
    salesData.FieldIndex = 39;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //40 Total Deductions Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalDeductionNonVatable;
    salesData.Field = "Total Deductions";
    salesData.FieldIndex = 40;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //41 Total Promo Sale Amount Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalPromoSalesAmountNonVatable;
    salesData.Field = "Total Promo Sale Amount";
    salesData.FieldIndex = 41;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //42 Total SCDDiscount Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = scdDiscount;
    salesData.Field = "Total SCD Discount";
    salesData.FieldIndex = 42;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //43 Total Refund Amount;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalRefundAmountNonVatable;
    salesData.Field = "Total Refund Amount ";
    salesData.FieldIndex = 43;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //44 Total Returned Items Amount;    //todo
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalReturnedItemsAmountNonVatable;
    salesData.Field = "Total Returned Items Amount";
    salesData.FieldIndex = 44;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //45 Total Other taxes //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalOtherTaxesNonVatable;
    salesData.Field = "Total Other Taxes";
    salesData.FieldIndex = 45;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //46 Total Service Charge Amount //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalServiceChargeAmountNonVatable;
    salesData.Field = "Total Service Charge Amount ";
    salesData.FieldIndex = 46;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //47 Total Adjustment Discount //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalAdjustmentDiscountNonVatable;
    salesData.Field = "Total Adjustment Discount";
    salesData.FieldIndex = 47;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

      //48 Total Void Amount //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalVoidAmountNonVatable;
    salesData.Field = "Total Void Amount";
    salesData.FieldIndex = 48;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

      //49 Total Discount Cards  //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalDiscountCardsNonVatable;
    salesData.Field = "Total Discount Cards";
    salesData.FieldIndex = 49;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //50 Total Delivery Charges  //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalDeliveryChargesNonVatable;
    salesData.Field = "Total Delivery Charges";
    salesData.FieldIndex = 50;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //51 Total Gift Cards  //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalGiftCertificatesChecksRedeemedNonVatable;
    salesData.Field = "Total Gift Certificates/Checks Redeemed";
    salesData.FieldIndex = 51;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //52 Store Specific Discount 1  //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount1NonVatable;
    salesData.Field = "Store Specific Discount 1";
    salesData.FieldIndex = 52;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //53 Store Specific Discount 2  //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount2NonVatable;
    salesData.Field = "Store Specific Discount 2";
    salesData.FieldIndex = 53;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //54 Store Specific Discount 3  //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount3NonVatable;
    salesData.Field = "Store Specific Discount 3";
    salesData.FieldIndex = 54;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //55 Store Specific Discount 4 //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount4NonVatable;
    salesData.Field = "Store Specific Discount 4";
    salesData.FieldIndex = 55;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //56 Store Specific Discount 5 //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount5NonVatable;
    salesData.Field = "Store Specific Discount 5";
    salesData.FieldIndex = 56;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //57 Total of all Non-Approved Store Discounts //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalofallNonApprovedStoreDiscountsNonVatable;
    salesData.Field = "Total of all Non-Approved Store Discounts";
    salesData.FieldIndex = 57;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //58 Store Specific Discount1;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount1NonVatable;
    salesData.Field = "Store Specific Discount 1";
    salesData.FieldIndex = 58;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //59 Store Specific Discount2//todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount2NonVatable;
    salesData.Field = "Store Specific Discount 2";
    salesData.FieldIndex = 59;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //60 Store Specific Discount3//todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount3NonVatable;
    salesData.Field = "Store Specific Discount 3";
    salesData.FieldIndex = 60;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //61 Store Specific Discount4;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount4NonVatable;
    salesData.Field = "Store Specific Discount 4";
    salesData.FieldIndex = 27 ;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //62 Store Specific Discount5;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = StoreSpecificDiscount5NonVatable;
    salesData.Field = "Store Specific Discount 5";
    salesData.FieldIndex = 62;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //63 Total VAT/Tax Amount//todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalVATTaxAmountNonVatable;
    salesData.Field = "Total VAT/Tax Amount";
    salesData.FieldIndex = 63 ;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //64 Total Net Sales Amount ;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalNetSalesAmountNonVatable;
    salesData.Field = "Total Net Sales Amount";
    salesData.FieldIndex = 31 ;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //65 Grand Total Net Sales //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = TotalNetSalesAmountVatable + TotalNetSalesAmountNonVatable;
    salesData.Field = "Grand Total Net Sales";
    salesData.FieldIndex = 65;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //66 Hour code //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = HourOf(Now().FormatString("dd-mm-yy HH:nn:ss"));
    salesData.Field = "Hour Code";
    salesData.FieldIndex = 66;
    salesData.DataValueType = "int";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //67 Status //todo;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = 1;
    salesData.Field = "Status";
    salesData.FieldIndex = 67;
    salesData.DataValueType = "int";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    return mallExportData;
}
//-----------------------------------------------------------------------------------------------
void TEstanciaMall::PrepareDataForExport()
{
///todo

}
//-----------------------------------------------------------------------------------------------
void TEstanciaMall::CreateExportMedium()
{
///todo
}
//----------------------------------------------------------------------------------------------
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



