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
void TEstanciaMallField::SetCoverCount(int coverCount)
{
    _coverCount = coverCount;
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
        int terminalNumber;
        UnicodeString tenantCode;
        std::list<TMallExportSettings>::iterator it;

        for(it = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); it != TGlobalSettings::Instance().mallInfo.MallSettings.end(); it++)
        {
            if(it->ControlName == "edTenantNo1")
            {
                tenantCode = it->Value;
            }
            else if(it->ControlName == "edTerminalNo1")
            {
                terminalNumber = StrToInt(it->Value);
            }
        }

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
        fieldData.CoverCount = GetPatronCount(paymentTransaction);

         ///call function to insert all fields into list
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Tenant Code", "UnicodeString", tenantCode, 1, arcBillKey);//01
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "POS Terminal Number", "int", terminalNumber, 2, arcBillKey);//02
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Date (mmddyyyy)", "TDateTime", Now().FormatString("mmddyyyy"), 3, arcBillKey);//03
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Old Accumulated Sales", "Currency", 0, 4, arcBillKey);//04    //todo
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "New Accumulated Sales", "Currency", fieldData.NewAccumulatedSalesVatable, 5, arcBillKey);//05
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Gross Amount", "Currency", fieldData.GrossAmountVatable, 6, arcBillKey);//06
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Deductions", "Currency", fieldData.DeductionVatable, 7, arcBillKey);//07
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Promo Sale Amount", "Currency", fieldData.PromoSalesAmountVatable, 8, arcBillKey);//08
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total PWD Discount", "Currency", fieldData.PWDDiscountVatable, 9, arcBillKey);//09
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Refund Amount", "Currency", fieldData.RefundAmountVatable, 10, arcBillKey);//10
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Returned Items Amount", "Currency", fieldData.ReturnedItemsAmountVatable, 11, arcBillKey);//11
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Other Taxes", "Currency", fieldData.OtherTaxesVatable, 12, arcBillKey);//12
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Service Charge Amount", "Currency", fieldData.ServiceChargeAmountVatable, 13, arcBillKey);//13
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Adjustment Discount", "Currency", fieldData.AdjustmentDiscountVatable, 14, arcBillKey);//14
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Void Amount", "Currency", fieldData.VoidAmountVatable, 15, arcBillKey);//15
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Discount Cards", "Currency", fieldData.DiscountCardsVatable, 16, arcBillKey);//16
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Delivery Charges", "Currency", fieldData.DeliveryChargesVatable, 17, arcBillKey);//17
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Gift Certificates/Checks Redeemed", "Currency", fieldData.GiftCertificatesChecksRedeemedVatable, 18, arcBillKey);//18
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 1", "Currency", fieldData.SSDiscount1Vatable, 19, arcBillKey);//19
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 2", "Currency", fieldData.SSDiscount2Vatable, 20, arcBillKey);//20
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 3", "Currency", fieldData.SSDiscount3Vatable, 21, arcBillKey);//21
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 4", "Currency", fieldData.SSDiscount4Vatable, 22, arcBillKey);//22
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 5", "Currency", fieldData.SSDiscount5Vatable, 23, arcBillKey);//23
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total of all Non-Approved Store Discounts", "Currency", fieldData.TotalOfallNonApprovedSDVatable, 24, arcBillKey);//24
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 1", "Currency", fieldData.SSDiscount1NonVatable, 25, arcBillKey);//25
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 2", "Currency", fieldData.SSDiscount2NonVatable, 26, arcBillKey);//26
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 3", "Currency", fieldData.SSDiscount3NonVatable, 27, arcBillKey);//27
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 4", "Currency", fieldData.SSDiscount4NonVatable, 28, arcBillKey);//28
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 5", "Currency", fieldData.SSDiscount5NonVatable, 29, arcBillKey);//29
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total VAT/Tax Amount", "Currency", fieldData.VATTaxAmountVatable, 30, arcBillKey);//30
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Net Sales Amount", "Currency", fieldData.NetSalesAmountVatable, 31, arcBillKey);//31
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Cover Count", "int", fieldData.CoverCount, 32, arcBillKey);//32
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Control Number", "int", 0, 33, arcBillKey);//33  //todo
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Number of Sales Transaction", "int", 1, 34, arcBillKey);//34  //todo
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Sales Type", "int", 1, 35, arcBillKey);//35
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Amount", "Currency", fieldData.NetSalesAmountVatable, 36, arcBillKey);//36
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Old Accumulated Sales", "Currency", fieldData.OldAccumulatedSalesNonVatable, 37, arcBillKey);//37
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "New Accumulated Sales", "Currency", fieldData.NewAccumulatedSalesNonVatable, 38, arcBillKey);//38
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Gross Amount", "Currency", fieldData.GrossAmountNonVatable, 39, arcBillKey);//39
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Deductions", "Currency", fieldData.DeductionNonVatable, 40, arcBillKey);//40
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Promo Sale Amount", "Currency", fieldData.PromoSalesAmountNonVatable, 41, arcBillKey);//41
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total SCD Discount", "Currency", fieldData.SCDDiscountNonVatable, 42, arcBillKey);//42
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Refund Amount", "Currency", fieldData.RefundAmountNonVatable, 43, arcBillKey);//43
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Returned Items Amount", "Currency", fieldData.ReturnedItemsAmountNonVatable, 44, arcBillKey);//44
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Other Taxes", "Currency", fieldData.OtherTaxesNonVatable, 45, arcBillKey);//45
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Service Charge Amount", "Currency", fieldData.ServiceChargeAmountNonVatable, 46, arcBillKey);//46
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Adjustment Discount", "Currency", fieldData.AdjustmentDiscountNonVatable, 47, arcBillKey);//47
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Void Amount", "Currency", fieldData.VoidAmountNonVatable, 48, arcBillKey);//48
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Discount Cards", "Currency", fieldData.DiscountCardsNonVatable, 49, arcBillKey);//49
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Delivery Charges", "Currency", fieldData.DeliveryChargesNonVatable, 50, arcBillKey);//50
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Gift Certificates/Checks Redeemed", "Currency", fieldData.GiftCertificatesChecksRedeemedNonVatable, 51, arcBillKey);//51
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 1", "Currency", fieldData.SSDiscount1NonVatable, 52, arcBillKey);//52
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 2", "Currency", fieldData.SSDiscount2NonVatable, 53, arcBillKey);//53
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 3", "Currency", fieldData.SSDiscount3NonVatable, 54, arcBillKey);//54
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 4", "Currency", fieldData.SSDiscount4NonVatable, 55, arcBillKey);//55
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 5", "Currency", fieldData.SSDiscount5NonVatable, 56, arcBillKey);//56
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total of all Non-Approved Store Discounts", "Currency", fieldData.TotalOfallNonApprovedSDiscountsNonVatable, 57, arcBillKey);//57
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 1", "Currency", fieldData.SSDiscount1NonApprovedVatable, 58, arcBillKey);//58
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 2", "Currency", fieldData.SSDiscount2NonApprovedVatable, 59, arcBillKey);//59
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 3", "Currency", fieldData.SSDiscount3NonApprovedVatable, 60, arcBillKey);//60
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 4", "Currency", fieldData.SSDiscount4NonApprovedVatable, 61, arcBillKey);//61
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Store Specific Discount 5", "Currency", fieldData.SSDiscount5NonApprovedVatable, 62, arcBillKey);//62
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total VAT/Tax Amount", "Currency", fieldData.VATTaxAmountNonVatable, 63, arcBillKey);//63
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Total Net Sales Amount", "Currency", fieldData.NetSalesAmountNonVatable, 64, arcBillKey);//64
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Grand Total Net Sales", "Currency", fieldData.NetSalesAmountVatable + fieldData.NetSalesAmountNonVatable, 65, arcBillKey);//65
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Hour Code", "int", HourOf(Now()), 66, arcBillKey);//66
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Status", "int", 1, 67, arcBillKey);//67
        PushFieldsInToList(paymentTransaction.DBTransaction, mallExportSalesData, "Invoice Number", "UnicodeString", paymentTransaction.InvoiceNumber, 68, arcBillKey);//68
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return mallExportSalesData;
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
//--------------------------------------------------------------------------------------------------------
void TEstanciaMall::PushFieldsInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, UnicodeString field, UnicodeString dataType, UnicodeString fieldValue, int fieldIndex, int arcBillKey)
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
    mallExportSalesData.push_back(salesData);
}
//--------------------------------------------------------------------------------------------------------
TMallExportPrepareData TEstanciaMall::PrepareDataForExport()
{
    TMallExportPrepareData preparedData;

    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    try
    {
        std::set<int> keyToCheck;
        int dailySalekeys[8] = {1, 2, 3, 33, 35, 66, 67, 68};
        for(int index = 0; index < 8; index++)
            keyToCheck.insert(dailySalekeys[index]);

        //Prepare Data For Daily Sales File
        PrepareDataForDailySalesFile(dbTransaction, keyToCheck, preparedData, 1);

        int  hourIndexkeys[3] = {65, 64, 32};
        keyToCheck.clear();

        for(int index = 0; index < 3; index++)
            keyToCheck.insert(hourIndexkeys[index]);

        //Prepare Data For Hourly File
        PrepareDataForHourlySalesFile(dbTransaction, keyToCheck, preparedData, 2);

        int invoiceIndex[3] = {65, 67, 68};
        keyToCheck.clear();
        for(int index = 0; index < 3; index++)
            keyToCheck.insert(invoiceIndex[index]);

        //Prepare Data For Invoice File
        PrepareDataForInvoiceSalesFile(dbTransaction, keyToCheck, preparedData, 3);

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
void TEstanciaMall::PrepareDataForInvoiceSalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys,
                                                                                        TMallExportPrepareData &prepareDataForInvoice, int index)
{
    //Create List Of SalesData for invoice file
    std::list<TMallExportSalesData> salesDataForISF;
    try
    {
        UnicodeString fileName = "I";
        UnicodeString indexKeysList = GetFieldIndexList(indexKeys);
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        std::set<int>keysToSelect;
        int  fileNameKeys[4] = {1, 2, 3, 33};

        for(int index = 0; index < 4; index++)
            keysToSelect.insert(fileNameKeys[index]);

        //Write File name into map
        fileName = fileName + "" + GetFileName(dBTransaction, keysToSelect);
        prepareDataForInvoice.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

        int invoiceIndexKeys[4] = {1, 2, 3, 35};
        keysToSelect.clear();
        for(int index = 0; index < 4; index++)
            keysToSelect.insert(invoiceIndexKeys[index]);

         ///Load MallSetting For writing into file
        LoadMallSettingsForInvoiceFile(dBTransaction, prepareDataForInvoice, keysToSelect, index);

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT a.ARCBILL_KEY, "
                                               "a.CREATED_BY, "
                                               "a.DATE_CREATED, "
                                               "a.FIELD, "
                                               "LPAD((CASE WHEN (a.FIELD_INDEX = 65) THEN 6 "
                                                          "WHEN (a.FIELD_INDEX = 67) THEN 7 "
                                                          "WHEN (a.FIELD_INDEX = 68) THEN 5 "
                                                          "ELSE (a.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                                "(CASE WHEN (a.FIELD_INDEX = 65) THEN (TOTALNETSALE.FIELD_VALUE*100) "
                                                      "WHEN (a.FIELD_INDEX = 68) THEN LPAD(a.FIELD_VALUE,5,0) "
                                                      "ELSE a.FIELD_VALUE END ) FIELD_VALUE, "
                                                "a.VALUE_TYPE, "
                                                "meh.MM_NAME "
                                        "FROM MALLEXPORT_SALES a "
                                        "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                        "LEFT JOIN(SELECT a.ARCBILL_KEY, CAST(a.FIELD_VALUE AS numeric(17,2))FIELD_VALUE "
                                                        "FROM MALLEXPORT_SALES a WHERE a.FIELD_INDEX = 65)TOTALNETSALE ON a.ARCBILL_KEY = TOTALNETSALE.ARCBILL_KEY "
                                "WHERE a.FIELD_INDEX IN(" + indexKeysList + " ) AND meh.IS_ACTIVE = :IS_ACTIVE "
                                "AND a.Z_KEY = (SELECT MAX(Z_KEY) FROM MALLEXPORT_SALES) "
                                "ORDER BY 1,5 ASC; ";
        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ExecQuery();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
       {
          TMallExportSalesData salesData;
          salesData.ArcBillKey =  IBInternalQuery->Fields[0]->AsInteger;
          salesData.CreatedBy  =IBInternalQuery->Fields[1]->AsString;
          salesData.DateCreated =IBInternalQuery->Fields[2]->AsDateTime;
          salesData.Field = IBInternalQuery->Fields[3]->AsString;
          salesData.FieldIndex = IBInternalQuery->Fields[4]->AsInteger;
          salesData.DataValue = IBInternalQuery->Fields[4]->AsString + "" + IBInternalQuery->Fields[5]->AsString;
          salesData.DataValueType = IBInternalQuery->Fields[6]->AsString;
          salesDataForISF.push_back(salesData);
       }
       prepareDataForInvoice.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, salesDataForISF ));
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------
void TEstanciaMall::PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys,
                                                                                TMallExportPrepareData &prepareDataForHSF, int index)
{
    std::list<TMallExportSalesData> prepareListForHSF;
    try
    {
        UnicodeString fileName = "H";
        UnicodeString indexKeysList = GetFieldIndexList(indexKeys);

        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        std::set<int>keysToSelect;
        int  fileNameKeys[4] = {1, 2, 3, 33};

        for(int index = 0; index < 4; index++)
            keysToSelect.insert(fileNameKeys[index]);

        //Write File name into map
        fileName = fileName + "" + GetFileName(dBTransaction, keysToSelect);
        prepareDataForHSF.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

        int invoiceIndexKeys[3] = {1, 2, 3};
        keysToSelect.clear();
        for(int index = 0; index < 3; index++)
            keysToSelect.insert(invoiceIndexKeys[index]);

        ///Load MallSetting For writing into file
        LoadMallSettingsForFile(dBTransaction, prepareDataForHSF, keysToSelect, index);

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
                                 "WHERE a.FIELD_INDEX IN(32,34,66) AND meh.IS_ACTIVE = :IS_ACTIVE  "
                                 "AND a.Z_KEY = (SELECT MAX(Z_KEY) FROM MALLEXPORT_SALES) "
                                 "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA  "
                            "GROUP BY 1,2,4,5 "

                            "UNION ALL "

                            "SELECT LPAD((CASE WHEN (HOURLYDATA.FIELD_INDEX = 65) THEN 5 ELSE (HOURLYDATA.FIELD_INDEX) END),2,0) FIELD_INDEX, "
                                    "HOURLYDATA.FIELD, CAST(SUM(HOURLYDATA.FIELD_VALUE)*100 AS INT ) FIELD_VALUE , HOURLYDATA.VALUE_TYPE, HOURLYDATA.Hour_code "
                            "FROM "
                                    "(SELECT a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE, meh.MM_NAME, "
                                            "Extract (Hour From a.DATE_CREATED) Hour_code "
                                     "FROM MALLEXPORT_SALES a "
                                     "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID"
                                    " WHERE a.FIELD_INDEX IN(65) AND meh.IS_ACTIVE = :IS_ACTIVE "
                                    "AND a.Z_KEY = (SELECT MAX(Z_KEY) FROM MALLEXPORT_SALES) "
                                     "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA "
                            "GROUP BY 1,2,4 ,5 "

                            "ORDER BY 5 ASC, 1 ASC ";

        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
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
                                "WHERE a.FIELD_INDEX IN(65, 34) AND meh.IS_ACTIVE = :IS_ACTIVE "
                                "AND a.Z_KEY = (SELECT MAX(Z_KEY) FROM MALLEXPORT_SALES) "
                                "ORDER BY A.MALLEXPORT_SALE_KEY ASC )HOURLYDATA "
                                "GROUP BY 1,2,4 "
                                "ORDER BY 1 ASC ";

        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;
          salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsCurrency;
          salesData.DataValueType = IBInternalQuery->Fields[3]->AsString;
          prepareListForHSF.push_back(salesData);
        }

        prepareDataForHSF.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, prepareListForHSF ));
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------------------
void TEstanciaMall::PrepareDataForDailySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys,
                                                                                TMallExportPrepareData &prepareDataForDSF, int index)
{
    std::list<TMallExportSalesData> prepareListForDSF;
    try
    {
        UnicodeString fileName = "D";
        UnicodeString indexKeysList = GetFieldIndexList(indexKeys);
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        std::set<int>keysToSelect;
        int  fileNameKeys[4] = {1, 2, 3, 33};

        for(int index = 0; index < 4; index++)
            keysToSelect.insert(fileNameKeys[index]);

        //Write File name into map
        fileName = fileName + "" + GetFileName(dBTransaction, keysToSelect);
        prepareDataForDSF.FileName.insert( std::pair<int,UnicodeString >(index, fileName ));

        int invoiceIndexKeys[3] = {1, 2, 3};
        keysToSelect.clear();
        for(int index = 0; index < 3; index++)
            keysToSelect.insert(invoiceIndexKeys[index]);

        ///Load MallSetting For writing into file
        LoadMallSettingsForFile(dBTransaction, prepareDataForDSF, keysToSelect, index);

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT DAILYDATA.FIELD_INDEX, DAILYDATA.FIELD, CAST(SUM(DAILYDATA.FIELD_VALUE)*100 AS INT) FIELD_VALUE , DAILYDATA.VALUE_TYPE, DAILYDATA.Z_KEY, DAILYDATA.MM_NAME "
                                      "FROM "
                                            "(SELECT a.ARCBILL_KEY, a.FIELD, LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, CAST((a.FIELD_VALUE) AS NUMERIC(17,2)) FIELD_VALUE, a.VALUE_TYPE, meh.MM_NAME, MAX(A.Z_KEY) Z_KEY "
                                             "FROM MALLEXPORT_SALES a "
                                             "INNER JOIN MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                             "WHERE a.FIELD_INDEX NOT IN(" + indexKeysList + ") AND meh.IS_ACTIVE = :IS_ACTIVE  "
                                             "AND a.Z_KEY = (SELECT MAX(Z_KEY) FROM MALLEXPORT_SALES) "
                                             "GROUP BY a.ARCBILL_KEY, a.FIELD, a.FIELD_INDEX,  a.VALUE_TYPE, meh.MM_NAME, a.FIELD_VALUE  "
                                             "ORDER BY A.ARCBILL_KEY ASC )DAILYDATA "
                                    "GROUP BY 1,2,4,5,6 "
                                    "UNION ALL "
                                     "SELECT LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, a.FIELD, cast(a.FIELD_VALUE as int ) FIELD_VALUE , a.VALUE_TYPE, a.Z_KEY, meh.MM_NAME  "
                                     "FROM "
                                        "MALLEXPORT_SALES a inner join MALLEXPORT_HEADER meh on a.FIELD_INDEX = meh.MALLEXPORT_HEADER_ID "
                                        "where a.FIELD_INDEX IN( 33, 35 ) AND meh.IS_ACTIVE = 'T' AND a.Z_KEY = (SELECT MAX(Z_KEY) FROM MALLEXPORT_SALES) "
                                    "ORDER BY 1 ASC  ";   //TODO AFTER DISCUSSION
        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ExecQuery();

       for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
       {
          TMallExportSalesData salesData;
          salesData.FieldIndex  = IBInternalQuery->Fields[0]->AsInteger;
          salesData.Field = IBInternalQuery->Fields[1]->AsString;
          salesData.DataValue = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsCurrency;
          salesData.DataValueType = IBInternalQuery->Fields[3]->AsString;
          salesData.ZKey = IBInternalQuery->Fields[4]->AsInteger;
          prepareListForDSF.push_back(salesData);
       }
       prepareDataForDSF.SalesData.insert( std::pair<int,list<TMallExportSalesData> >(index, prepareListForDSF ));
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------------------------
void TEstanciaMall::LoadMallSettingsForFile(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareData, std::set<int> keysToSelect, int index)
{
    try
    {
        std::list<TMallExportSettings> mallSettings;
        UnicodeString indexKeysList = GetFieldIndexList(keysToSelect);
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT LPAD(a.FIELD_INDEX,2,0) FIELD_INDEX, a.FIELD, CASE WHEN(a.FIELD_INDEX = 2) THEN LPAD(a.FIELD_VALUE,2,0) "
                                        "ELSE (a.FIELD_VALUE) END FIELD_VALUE, a.VALUE_TYPE "
                                      "FROM MALLEXPORT_SALES a "
                                      "INNER JOIN MALLEXPORT_HEADER MEH ON A.FIELD_INDEX = MEH.MALLEXPORT_HEADER_ID "
                                      "WHERE a.FIELD_INDEX IN(" + indexKeysList + ") AND meh.IS_ACTIVE = :IS_ACTIVE "
                                      "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) "
                                      "GROUP BY 1,2,3,4 ";
        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
          TMallExportSettings settings;
          settings.Name =   IBInternalQuery->Fields[1]->AsString;
          settings.Value  = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsString;
          settings.ValueType = IBInternalQuery->Fields[3]->AsString;
          mallSettings.push_back(settings);
        }
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
      return new TMallExportTextFile;
}
//--------------------------------------------------------------------------------------------------------------------
UnicodeString TEstanciaMall::GetFileName(Database::TDBTransaction &dBTransaction, std::set<int> keysToSelect)
{
    UnicodeString fileName = "";
    try
    {
        std::list<TMallExportSettings> mallSettings;
        UnicodeString indexKeysList = GetFieldIndexList(keysToSelect);
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT a.FIELD_INDEX, a.FIELD, CASE WHEN(a.FIELD_INDEX = 2) THEN LPAD(a.FIELD_VALUE,2,0) "
                                                                            "WHEN(a.FIELD_INDEX = 33) THEN LPAD(a.FIELD_VALUE,3,0) "
                                                                        "ELSE (a.FIELD_VALUE) END FIELD_VALUE, "
                                                "a.VALUE_TYPE , a.Z_KEY "
                                    "FROM MALLEXPORT_SALES a "
                                    "INNER JOIN MALLEXPORT_HEADER MEH ON A.FIELD_INDEX = MEH.MALLEXPORT_HEADER_ID "
                                    "WHERE a.FIELD_INDEX IN(" + indexKeysList + " ) AND meh.IS_ACTIVE = :IS_ACTIVE "
                                    "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) "
                                    "GROUP BY 1,2,3,4,5 "
                                    "ORDER BY 1 ASC ";
        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            fileName = fileName + "" + IBInternalQuery->Fields[2]->AsString;
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
void TEstanciaMall::LoadMallSettingsForInvoiceFile(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareData, std::set<int> keysToSelect, int index)
{
    try
    {
        std::list<TMallExportSettings> mallSettings;
        UnicodeString indexKeysList = GetFieldIndexList(keysToSelect);
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dBTransaction.RegisterQuery(IBInternalQuery);

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT LPAD((CASE WHEN a.FIELD_INDEX = 35 THEN 2 "
                                                    "WHEN a.FIELD_INDEX = 2 THEN 4 "
                                                    "ELSE a.FIELD_INDEX END),2,0) FIELD_INDEX, a.FIELD, CASE WHEN(a.FIELD_INDEX = 2) THEN LPAD(a.FIELD_VALUE,2,0) "
                                                    "ELSE (a.FIELD_VALUE) END FIELD_VALUE, a.VALUE_TYPE "
                                      "FROM MALLEXPORT_SALES a "
                                      "INNER JOIN MALLEXPORT_HEADER MEH ON A.FIELD_INDEX = MEH.MALLEXPORT_HEADER_ID "
                                      "WHERE a.FIELD_INDEX IN(" + indexKeysList + " ) AND meh.IS_ACTIVE = :IS_ACTIVE "
                                      "AND a.Z_KEY = (SELECT MAX(Z_KEY)FROM MALLEXPORT_SALES) "
                                      "GROUP BY 1,2,3,4 ";
        IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = "T";
        IBInternalQuery->ExecQuery();

        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
          TMallExportSettings settings;
          settings.Name =   IBInternalQuery->Fields[1]->AsString;
          settings.Value  = IBInternalQuery->Fields[0]->AsString + "" + IBInternalQuery->Fields[2]->AsString;
          settings.ValueType = IBInternalQuery->Fields[3]->AsString;
          mallSettings.push_back(settings);
        }
        prepareData.MallSettings.insert( std::pair<int,list<TMallExportSettings> >(index, mallSettings));
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

