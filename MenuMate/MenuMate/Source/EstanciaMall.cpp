//---------------------------------------------------------------------------


#pragma hdrstop
#include "EstanciaMall.h"
#include "MallExportData.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include <Math.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)

TEstanciaMall::TEstanciaMall()
{
}
void TEstanciaMall::PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey)
{
    Currency promoDiscount = 0.00;
    Currency scdDiscount = 0.00;
    Currency pwdDiscount = 0.00;
    Currency discountGroup1 = 0.00;
    Currency discountGroup2 = 0.00;
    Currency discountGroup3 = 0.00;
    Currency discountGroup4 = 0.00;
    Currency totalNonApprovedDiscount = 0.00;
    Currency salesTax = 0.00;
    Currency serviceCharge = 0.00;
    Currency serviceChargeTax = 0.00;
    Currency localTax = 0.00;
    Currency profitTax = 0.00;

    Currency OldAccumulatedSalesVatable;
    Currency NewAccumulatedSalesVatable;
    Currency TotalGrossAmountVatable;
    Currency TotalDeductionsVatable;
    Currency TotalPromoSalesAmountVatable;
    Currency TotalPWDDiscountVatable;
    Currency TotalRefundAmountVatable;
    Currency TotalReturnedItemsAmountVatable;
    Currency TotalOtherTaxesVatable;
    Currency TotalServiceChargeAmountVatable;
    Currency TotalAdjustmentDiscountVatable;
    Currency TotalVoidAmountVatable;
    Currency TotalDiscountCardsVatable;
    Currency TotalDeliveryChargesVatable;
    Currency TotalGiftCertificatesChecksRedeemedVatable;
    Currency StoreSpecificDiscount1Vatable;
    Currency StoreSpecificDiscount2Vatable;
    Currency StoreSpecificDiscount3Vatable;
    Currency StoreSpecificDiscount4Vatable;
    Currency StoreSpecificDiscount5Vatable;
    Currency TotalofallNonApprovedStoreDiscountsVatable;
    Currency StoreSpecificDiscount1NonApprovedVatable;
    Currency StoreSpecificDiscount2NonApprovedVatable;
    Currency StoreSpecificDiscount3NonApprovedVatable;
    Currency StoreSpecificDiscount4NonApprovedVatable;
    Currency StoreSpecificDiscount5NonApprovedVatable;
    Currency TotalVATTaxAmountVatable;
    Currency TotalNetSalesAmountVatable;

    Currency OldAccumulatedSalesNonVatable;
    Currency NewAccumulatedSalesNonVatable;
    Currency TotalGrossAmountNonVatable;
    Currency TotalDeductionsNonVatable;
    Currency TotalPromoSalesAmountNonVatable;
    Currency TotalPWDDiscountNonVatable;
    Currency TotalRefundAmountNonVatable;
    Currency TotalReturnedItemsAmountNonVatable;
    Currency TotalOtherTaxesNonVatable;
    Currency TotalServiceChargeAmountNonVatable;
    Currency TotalAdjustmentDiscountNonVatable;
    Currency TotalVoidAmountNonVatable;
    Currency TotalDiscountCardsNonVatable;
    Currency TotalDeliveryChargesNonVatable;
    Currency TotalGiftCertificatesChecksRedeemedNonVatable;
    Currency StoreSpecificDiscount1NonVatable;
    Currency StoreSpecificDiscount2NonVatable;
    Currency StoreSpecificDiscount3NonVatable;
    Currency StoreSpecificDiscount4NonVatable;
    Currency StoreSpecificDiscount5NonVatable;
    Currency TotalofallNonApprovedStoreDiscountsNonVatable;
    Currency StoreSpecificDiscount1NonApprovedNonVatable;
    Currency StoreSpecificDiscount2NonApprovedNonVatable;
    Currency StoreSpecificDiscount3NonApprovedNonVatable;
    Currency StoreSpecificDiscount4NonApprovedNonVatable;
    Currency StoreSpecificDiscount5NonApprovedNonVatable;
    Currency TotalVATTaxAmountNonVatable;
    Currency TotalNetSalesAmountNonVatable;

    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    TMallExportData mallExportData;
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

     //05 Load New Accumulated Sale Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = Now().FormatString("mmddyyyy");
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
    salesData.DataValue = paymentTransaction.Money.RoundedGrandTotal + fabs(paymentTransaction.Money.TotalAdjustment);
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
    salesData.DataValue = paymentTransaction.Money.RoundedGrandTotal + fabs(paymentTransaction.Money.TotalAdjustment);
    salesData.Field = "Total Deductions";
    salesData.FieldIndex = 7;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    for (int CurrentIndex = 0; CurrentIndex < PaymentTransaction.Orders->Count; CurrentIndex++)
    {
			TItemComplete *Order = (TItemComplete*)(PaymentTransaction.Orders->Items[CurrentIndex]);

            std::vector<BillCalculator::TTaxResult> taxInfomation = Order->BillCalcResult.Tax;
            bool isVatable = false;

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
                        promoDiscount + = Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Senior Citizen")
                        scdDiscount + = Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Person with Disability")
                        pwdDiscount + = Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 1")
                        discountGroup1 + = Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 2")
                        discountGroup2 + = Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 3")
                        discountGroup3 + = Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 4")
                        discountGroup4 + = Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Discount 5")
                        discountGroup5 + = Order->DiscountValue_BillCalc(ptrDiscounts);
                    else if(ptrDiscounts->DiscountGroupList[0].Name == "Other Discount")
                        totalNonApprovedDiscount + = Order->DiscountValue_BillCalc(ptrDiscounts);
                }
            }



            if(isVatable)
            {
                //OldAccumulatedSalesVatable
                NewAccumulatedSalesVatable +=
                TotalGrossAmountVatable += Order->PriceLevelCustom > 0 ? Order->PriceLevelCustom + fabs(Order->TotalAdjustment()) : Order->PriceLevel1 + fabs(Order->TotalAdjustment());
                TotalDeductionsVatable +=  ;
                TotalPromoSalesAmountVatable += promoDiscount;
                TotalPWDDiscountVatable += pwsDiscount;
                TotalRefundAmountVatable += Order->GetQty() < 0?
                TotalReturnedItemsAmountVatable +=
                TotalOtherTaxesVatable +=
                TotalServiceChargeAmountVatable +=
                TotalAdjustmentDiscountVatable +=
                TotalVoidAmountVatable +=
                TotalDiscountCardsVatable +=
                TotalDeliveryChargesVatable +=
                TotalGiftCertificatesChecksRedeemedVatable +=
                StoreSpecificDiscount1Vatable += discountGroup1;
                StoreSpecificDiscount2Vatable += discountGroup2;
                StoreSpecificDiscount3Vatable += discountGroup3;
                StoreSpecificDiscount4Vatable += discountGroup4;
                StoreSpecificDiscount5Vatable += discountGroup5;
                TotalofallNonApprovedStoreDiscountsVatable += totalNonApprovedDiscount;
                StoreSpecificDiscount1NonApprovedVatable +=
                StoreSpecificDiscount2NonApprovedVatable +=
                StoreSpecificDiscount3NonApprovedVatable +=
                StoreSpecificDiscount4NonApprovedVatable  +=
                StoreSpecificDiscount5NonApprovedVatable +=
                TotalVATTaxAmountVatable +=
                TotalNetSalesAmountVatable +=
            }
            else
            {
                //OldAccumulatedSalesNonVatable
                NewAccumulatedSalesNonVatable
                TotalGrossAmountNonVatable += Order->PriceLevelCustom > 0 ? Order->PriceLevelCustom + fabs(Order->TotalAdjustment()) : Order->PriceLevel1 + fabs(Order->TotalAdjustment());
                TotalDeductionsNonVatable +=  ;
                TotalPromoSalesAmountNonVatable += promoDiscount;
                TotalPWDDiscountNonVatable +=
                TotalRefundAmountNonVatable +=
                TotalReturnedItemsAmountNonVatable +=
                TotalOtherTaxesNonVatable +=
                TotalServiceChargeAmountNonVatable +=
                TotalAdjustmentDiscountNonVatable  +=
                TotalVoidAmountNonVatable +=
                TotalDiscountCardsNonVatable +=
                TotalDeliveryChargesNonVatable +=
                TotalGiftCertificatesChecksRedeemedNonVatable +=
                StoreSpecificDiscount1NonVatable += discountGroup1;
                StoreSpecificDiscount2NonVatable += discountGroup2;
                StoreSpecificDiscount3NonVatable += discountGroup3;
                StoreSpecificDiscount4NonVatable += discountGroup4;
                StoreSpecificDiscount5NonVatable += discountGroup5;
                TotalofallNonApprovedStoreDiscountsNonVatable += totalNonApprovedDiscount;
                StoreSpecificDiscount1NonApprovedNonVatable +=
                StoreSpecificDiscount2NonApprovedNonVatable +=
                StoreSpecificDiscount3NonApprovedNonVatable +=
                StoreSpecificDiscount4NonApprovedNonVatable +=
                StoreSpecificDiscount5NonApprovedNonVatable +=
                TotalVATTaxAmountNonVatable +=
                TotalNetSalesAmountNonVatable +=
            }


    }

    //08 Total Promo Sale Amount Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = promoDiscount;
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
    salesData.DataValue = pwdDiscount;
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
    salesData.DataValue = paymentTransaction.Money.RoundedGrandTotal < 0 ? paymentTransaction.Money.RoundedGrandTotal: 0.00;
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
    salesData.DataValue = paymentTransaction.Money.RoundedGrandTotal < 0 ? paymentTransaction.Money.RoundedGrandTotal: 0.00;
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
    salesData.DataValue = paymentTransaction.Money.RoundedGrandTotal < 0 ? paymentTransaction.Money.RoundedGrandTotal: 0.00;
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
    salesData.DataValue = paymentTransaction.Money.RoundedGrandTotal < 0 ? paymentTransaction.Money.RoundedGrandTotal: 0.00;
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
    salesData.DataValue = paymentTransaction.Money.RoundedGrandTotal < 0 ? paymentTransaction.Money.RoundedGrandTotal: 0.00;
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
    salesData.DataValue = paymentTransaction.Money.RoundedGrandTotal < 0 ? paymentTransaction.Money.RoundedGrandTotal: 0.00;
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
    salesData.DataValue = paymentTransaction.Money.RoundedGrandTotal < 0 ? paymentTransaction.Money.RoundedGrandTotal: 0.00;
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
    salesData.DataValue = paymentTransaction.Money.RoundedGrandTotal < 0 ? paymentTransaction.Money.RoundedGrandTotal: 0.00;
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
    salesData.DataValue = 0;
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
    salesData.DataValue = discountGroup1;
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
    salesData.DataValue = discountGroup2;
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
    salesData.DataValue = discountGroup3;
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
    salesData.DataValue = discountGroup4;
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
    salesData.DataValue = discountGroup5;
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
    salesData.DataValue = totalNonApprovedDiscount;
    salesData.Field = "Total of all Non-Approved Store Discounts";
    salesData.FieldIndex = 24;
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);



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


