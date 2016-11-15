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
bool TEstanciaMall::PushToDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey)
{
    try
    {
        TMallExportPrepareData preparedData;
        preparedData = PrepareDataForDatabase(paymentTransaction, arcBillKey);
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------------------------------------------------
TMallExportPrepareData TEstanciaMall::PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey)
{
    TMallExportPrepareData mallExportData;
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

        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

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
        PushFieldsInToList(dbTransaction, mallExportData, "Tenant Code", "UnicodeString", tenantCode, 1, arcBillKey);//01
        PushFieldsInToList(dbTransaction, mallExportData, "POS Terminal Number", "int", terminalNumber, 2, arcBillKey);//02
        PushFieldsInToList(dbTransaction, mallExportData, "Date (mmddyyyy)", "TDateTime", Now().FormatString("mmddyyyy"), 3, arcBillKey);//03
        PushFieldsInToList(dbTransaction, mallExportData, "Old Accumulated Sales", "Currency", 0, 4, arcBillKey);//04    //todo
        PushFieldsInToList(dbTransaction, mallExportData, "New Accumulated Sales", "Currency", fieldData.NewAccumulatedSalesVatable, 5, arcBillKey);//05
        PushFieldsInToList(dbTransaction, mallExportData, "Total Gross Amount", "Currency", fieldData.GrossAmountVatable, 6, arcBillKey);//06
        PushFieldsInToList(dbTransaction, mallExportData, "Total Deductions", "Currency", fieldData.DeductionVatable, 7, arcBillKey);//07
        PushFieldsInToList(dbTransaction, mallExportData, "Total Promo Sale Amount", "Currency", fieldData.PromoSalesAmountVatable, 8, arcBillKey);//08
        PushFieldsInToList(dbTransaction, mallExportData, "Total PWD Discount", "Currency", fieldData.PWDDiscountVatable, 9, arcBillKey);//09
        PushFieldsInToList(dbTransaction, mallExportData, "Total Refund Amount", "Currency", fieldData.RefundAmountVatable, 10, arcBillKey);//10
        PushFieldsInToList(dbTransaction, mallExportData, "Total Returned Items Amount", "Currency", fieldData.ReturnedItemsAmountVatable, 11, arcBillKey);//11
        PushFieldsInToList(dbTransaction, mallExportData, "Total Other Taxes", "Currency", fieldData.OtherTaxesVatable, 12, arcBillKey);//12
        PushFieldsInToList(dbTransaction, mallExportData, "Total Service Charge Amount", "Currency", fieldData.ServiceChargeAmountVatable, 13, arcBillKey);//13
        PushFieldsInToList(dbTransaction, mallExportData, "Total Adjustment Discount", "Currency", fieldData.AdjustmentDiscountVatable, 14, arcBillKey);//14
        PushFieldsInToList(dbTransaction, mallExportData, "Total Void Amount", "Currency", fieldData.VoidAmountVatable, 15, arcBillKey);//15
        PushFieldsInToList(dbTransaction, mallExportData, "Total Discount Cards", "Currency", fieldData.DiscountCardsVatable, 16, arcBillKey);//16
        PushFieldsInToList(dbTransaction, mallExportData, "Total Delivery Charges", "Currency", fieldData.DeliveryChargesVatable, 17, arcBillKey);//17
        PushFieldsInToList(dbTransaction, mallExportData, "Total Gift Certificates/Checks Redeemed", "Currency", fieldData.GiftCertificatesChecksRedeemedVatable, 18, arcBillKey);//18
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 1", "Currency", fieldData.SSDiscount1Vatable, 19, arcBillKey);//19
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 2", "Currency", fieldData.SSDiscount2Vatable, 20, arcBillKey);//20
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 3", "Currency", fieldData.SSDiscount3Vatable, 21, arcBillKey);//21
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 4", "Currency", fieldData.SSDiscount4Vatable, 22, arcBillKey);//22
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 5", "Currency", fieldData.SSDiscount5Vatable, 23, arcBillKey);//23
        PushFieldsInToList(dbTransaction, mallExportData, "Total of all Non-Approved Store Discounts", "Currency", fieldData.TotalOfallNonApprovedSDVatable, 24, arcBillKey);//24
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 1", "Currency", fieldData.SSDiscount1NonVatable, 25, arcBillKey);//25
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 2", "Currency", fieldData.SSDiscount2NonVatable, 26, arcBillKey);//26
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 3", "Currency", fieldData.SSDiscount3NonVatable, 27, arcBillKey);//27
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 4", "Currency", fieldData.SSDiscount4NonVatable, 28, arcBillKey);//28
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 5", "Currency", fieldData.SSDiscount5NonVatable, 29, arcBillKey);//29
        PushFieldsInToList(dbTransaction, mallExportData, "Total VAT/Tax Amount", "Currency", fieldData.VATTaxAmountVatable, 30, arcBillKey);//30
        PushFieldsInToList(dbTransaction, mallExportData, "Total Net Sales Amount", "Currency", fieldData.NetSalesAmountVatable, 31, arcBillKey);//31
        PushFieldsInToList(dbTransaction, mallExportData, "Total Cover Count", "int", fieldData.CoverCount, 32, arcBillKey);//32
        PushFieldsInToList(dbTransaction, mallExportData, "Control Number", "int", "", 33, arcBillKey);//33  //todo
        PushFieldsInToList(dbTransaction, mallExportData, "Total Number of Sales Transaction", "int", 1, 34, arcBillKey);//34  //todo
        PushFieldsInToList(dbTransaction, mallExportData, "Sales Type", "int", 1, 35, arcBillKey);//35  //todo
        PushFieldsInToList(dbTransaction, mallExportData, "Amount", "Currency", fieldData.NetSalesAmountVatable, 36, arcBillKey);//36
        PushFieldsInToList(dbTransaction, mallExportData, "Old Accumulated Sales", "Currency", fieldData.OldAccumulatedSalesNonVatable, 37, arcBillKey);//37
        PushFieldsInToList(dbTransaction, mallExportData, "New Accumulated Sales", "Currency", fieldData.NewAccumulatedSalesNonVatable, 38, arcBillKey);//38
        PushFieldsInToList(dbTransaction, mallExportData, "Total Gross Amount", "Currency", fieldData.GrossAmountNonVatable, 39, arcBillKey);//39
        PushFieldsInToList(dbTransaction, mallExportData, "Total Deductions", "Currency", fieldData.DeductionNonVatable, 40, arcBillKey);//40
        PushFieldsInToList(dbTransaction, mallExportData, "Total Promo Sale Amount", "Currency", fieldData.PromoSalesAmountNonVatable, 41, arcBillKey);//41
        PushFieldsInToList(dbTransaction, mallExportData, "Total SCD Discount", "Currency", fieldData.SCDDiscountNonVatable, 42, arcBillKey);//42
        PushFieldsInToList(dbTransaction, mallExportData, "Total Refund Amount", "Currency", fieldData.RefundAmountNonVatable, 43, arcBillKey);//43
        PushFieldsInToList(dbTransaction, mallExportData, "Total Returned Items Amount", "Currency", fieldData.ReturnedItemsAmountNonVatable, 44, arcBillKey);//44
        PushFieldsInToList(dbTransaction, mallExportData, "Total Other Taxes", "Currency", fieldData.OtherTaxesNonVatable, 45, arcBillKey);//45
        PushFieldsInToList(dbTransaction, mallExportData, "Total Service Charge Amount", "Currency", fieldData.ServiceChargeAmountNonVatable, 46, arcBillKey);//46
        PushFieldsInToList(dbTransaction, mallExportData, "Total Adjustment Discount", "Currency", fieldData.AdjustmentDiscountNonVatable, 47, arcBillKey);//47
        PushFieldsInToList(dbTransaction, mallExportData, "Total Void Amount", "Currency", fieldData.VoidAmountNonVatable, 48, arcBillKey);//48
        PushFieldsInToList(dbTransaction, mallExportData, "Total Discount Cards", "Currency", fieldData.DiscountCardsNonVatable, 49, arcBillKey);//49
        PushFieldsInToList(dbTransaction, mallExportData, "Total Delivery Charges", "Currency", fieldData.DeliveryChargesNonVatable, 50, arcBillKey);//50
        PushFieldsInToList(dbTransaction, mallExportData, "Total Gift Certificates/Checks Redeemed", "Currency", fieldData.GiftCertificatesChecksRedeemedNonVatable, 51, arcBillKey);//51
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 1", "Currency", fieldData.SSDiscount1NonVatable, 52, arcBillKey);//52
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 2", "Currency", fieldData.SSDiscount2NonVatable, 53, arcBillKey);//53
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 3", "Currency", fieldData.SSDiscount3NonVatable, 54, arcBillKey);//54
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 4", "Currency", fieldData.SSDiscount4NonVatable, 55, arcBillKey);//55
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 5", "Currency", fieldData.SSDiscount5NonVatable, 56, arcBillKey);//56
        PushFieldsInToList(dbTransaction, mallExportData, "Total of all Non-Approved Store Discounts", "Currency", fieldData.TotalOfallNonApprovedSDiscountsNonVatable, 57, arcBillKey);//57
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 1", "Currency", fieldData.SSDiscount1NonApprovedVatable, 58, arcBillKey);//58
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 2", "Currency", fieldData.SSDiscount2NonApprovedVatable, 59, arcBillKey);//59
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 3", "Currency", fieldData.SSDiscount3NonApprovedVatable, 60, arcBillKey);//60
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 4", "Currency", fieldData.SSDiscount4NonApprovedVatable, 61, arcBillKey);//61
        PushFieldsInToList(dbTransaction, mallExportData, "Store Specific Discount 5", "Currency", fieldData.SSDiscount5NonApprovedVatable, 62, arcBillKey);//62
        PushFieldsInToList(dbTransaction, mallExportData, "Total VAT/Tax Amount", "Currency", fieldData.VATTaxAmountNonVatable, 63, arcBillKey);//63
        PushFieldsInToList(dbTransaction, mallExportData, "Total Net Sales Amount", "Currency", fieldData.NetSalesAmountNonVatable, 64, arcBillKey);//64
        PushFieldsInToList(dbTransaction, mallExportData, "Grand Total Net Sales", "Currency", fieldData.NetSalesAmountVatable + fieldData.NetSalesAmountNonVatable, 65, arcBillKey);//65
        PushFieldsInToList(dbTransaction, mallExportData, "Hour Code", "int", HourOf(Now().FormatString("dd-mm-yy HH:nn:ss")), 66, arcBillKey);//66
        PushFieldsInToList(dbTransaction, mallExportData, "Status", "int", 1, 67, arcBillKey);//67    todo
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return mallExportData;
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
void TEstanciaMall::PushFieldsInToList(Database::TDBTransaction &dbTransaction, TMallExportPrepareData &mallExportData, UnicodeString field, UnicodeString dataType, UnicodeString fieldValue, int fieldIndex, int arcBillKey)
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
    mallExportData.SalesData.push_back(salesData);
}
//--------------------------------------------------------------------------------------------------------
void TEstanciaMall::PrepareDataForExport()
{
///todo

}
//-----------------------------------------------------------------------------------------------
void TEstanciaMall::CreateExportMedium()
{
///todo
}

