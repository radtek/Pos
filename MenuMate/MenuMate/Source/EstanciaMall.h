//---------------------------------------------------------------------------

#ifndef EstanciaMallH
#define EstanciaMallH
#include "MallExport.h"
//---------------------------------------------------------------------------

class TEstanciaMall : public TMallExport
{
    private:
    int GetPatronCount(TPaymentTransaction &paymentTransaction);
    long GenerateSaleKey(Database::TDBTransaction &dbTransaction);
    void PushFieldsInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, UnicodeString field, UnicodeString dataType, UnicodeString fieldValue, int fieldIndex, int arcbillKey);

    TMallExportPrepareData PrepareDataForInvoiceSalesFile(Database::TDBTransaction &dBTransaction);
    TMallExportPrepareData PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction);
    TMallExportPrepareData PrepareDataForDailySalesFile(Database::TDBTransaction &dBTransaction);
    void LoadMallSettingsForFile(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareForDSF);

    protected:
    std::list<TMallExportSalesData> PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey);
    void PrepareDataForExport();
    void CreateExportMedium();

    public:
    TEstanciaMall();
};

class TEstanciaMallField
{
    private:
    Currency _oldAccSalesVatable;
    Currency _newAccSalesVatable;
    Currency _grossAmountVatable;
    Currency _deductionVatable;
    Currency _promoSalesAmountVatable;
    Currency _pWDDiscountVatable;
    Currency _refundAmountVatable;
    Currency _returnedItemsAmountVatable;
    Currency _otherTaxesVatable;
    Currency _serviceChargeAmountVatable;
    Currency _adjustmentDiscountVatable;
    Currency _voidAmountVatable;
    Currency _discountCardsVatable;
    Currency _deliveryChargesVatable;
    Currency _giftCertificatesChecksRedeemedVatable;
    Currency _sSDiscount1Vatable;
    Currency _sSDiscount2Vatable;
    Currency _sSDiscount3Vatable;
    Currency _sSDiscount4Vatable;
    Currency _sSDiscount5Vatable;
    Currency _totalOfallNonApprovedStoreDiscountsVatable;
    Currency _sSDiscount1NonApprovedVatable;
    Currency _sSDiscount2NonApprovedVatable;
    Currency _sSDiscount3NonApprovedVatable;
    Currency _sSDiscount4NonApprovedVatable;
    Currency _sSDiscount5NonApprovedVatable;
    Currency _vATAmountVatable;
    Currency _netSalesAmountVatable;
    Currency _oldAccSalesNonVatable;
    Currency _newAccSalesNonVatable;
    Currency _grossAmountNonVatable;
    Currency _deductionNonVatable;
    Currency _promoSalesAmountNonVatable;
    Currency _sCDDiscountNonVatable;
    Currency _refundAmountNonVatable;
    Currency _returnedItemsAmountNonVatable;
    Currency _otherTaxesNonVatable;
    Currency _serviceChargeAmountNonVatable;
    Currency _adjustmentDiscountNonVatable;
    Currency _voidAmountNonVatable;
    Currency _discountCardsNonVatable;
    Currency _deliveryChargesNonVatable;
    Currency _giftCertificatesChecksRedeemedNonVatable;
    Currency _sSDiscount1NonVatable;
    Currency _sSDiscount2NonVatable;
    Currency _sSDiscount3NonVatable;
    Currency _sSDiscount4NonVatable;
    Currency _sSDiscount5NonVatable;
    Currency _totalOfallNonApprovedStoreDiscountsNonVatable;
    Currency _sSDiscount1NonApprovedNonVatable;
    Currency _sSDiscount2NonApprovedNonVatable;
    Currency _sSDiscount3NonApprovedNonVatable;
    Currency _sSDiscount4NonApprovedNonVatable;
    Currency _sSDiscount5NonApprovedNonVatable;
    Currency _vATAmountNonVatable;
    Currency _netSalesAmountNonVatable;
    int _coverCount;

    void SetOldAccSalesVatable(Currency oldAccSalesVatable);
    void SetNewAccSalesVatable(Currency newAccSaleVatable);
    void SetGrossAmountVatable(Currency grossAmountVatable);
    void SetDeductionVatable(Currency deductionVatable);
    void SetPromoSalesAmountVatable(Currency promoSaleAmountVatable);
    void SetPWDDiscountVatable(Currency pWDDiscountVatable);
    void SetRefundAmountVatable(Currency refundAmountVatable);
    void SetReturnedItemsAmountVatable(Currency returnedItemsAmount);
    void SetOtherTaxesVatable(Currency otherTaxesVatable);
    void SetServiceChargeAmountVatable(Currency serviceChargeAmountVatable);
    void SetAdjustmentDiscountVatable(Currency adjustmentDiscountVatable);
    void SetVoidAmountVatable(Currency voidAmountVatable);
    void SetDiscountCardsVatable(Currency discountCardsVatable);
    void SetDeliveryChargesVatable(Currency deliveryChargesVatable);
    void SetGiftCertificatesChecksRedeemedVatable(Currency giftCertificatesChecksRedeemedVatable);
    void SetSSDiscount1Vatable(Currency sSDiscount1Vatable);
    void SetSSDiscount2Vatable(Currency sSDiscount2Vatable);
    void SetSSDiscount3Vatable(Currency sSDiscount3Vatable);
    void SetSSDiscount4Vatable(Currency sSDiscount4Vatable);
    void SetSSDiscount5Vatable(Currency sSDiscount5Vatable);
    void SetTotalOfallNonApprovedStoreDiscountsVatable(Currency nonApprovedDiscountVatable);
    void SetSSDiscount1NonApprovedVatable(Currency sSDiscount1NonApprovedVatable);
    void SetSSDiscount2NonApprovedVatable(Currency sSDiscount2NonApprovedVatable);
    void SetSSDiscount3NonApprovedVatable(Currency sSDiscount3NonApprovedVatable);
    void SetSSDiscount4NonApprovedVatable(Currency sSDiscount4NonApprovedVatable);
    void SetSSDiscount5NonApprovedVatable(Currency sSDiscount5NonApprovedVatable);
    void SetVATAmountVatable(Currency vatAmount);
    void SetNetSalesAmountVatable(Currency netSaleAmountVatable);
    void SetOldAccSalesNonVatable(Currency oldAccSalesNonVatable);
    void SetNewAccSalesNonVatable(Currency newAccSalesNonVatable);
    void SetGrossAmountNonVatable(Currency grossAmountNonVatble);
    void SetDeductionNonVatable(Currency deductionNonVatable);
    void SetPromoSalesAmountNonVatable(Currency promoSaleAmountNonVatable);
    void SetSCDDiscountNonVatable(Currency sCDDiscountNonVatable);
    void SetRefundAmountNonVatable(Currency refundAmountNonVatable);
    void SetReturnedItemsAmountNonVatable(Currency returnedItemsAmountNonVatable);
    void SetOtherTaxesNonVatable(Currency otherTaxesNonVatable);
    void SetServiceChargeAmountNonVatable(Currency serviceChargeAmountNonVatable);
    void SetAdjustmentDiscountNonVatable(Currency adjustmentDiscountNonVatable);
    void SetVoidAmountNonVatable(Currency voidAmountNonVatable);
    void SetDiscountCardsNonVatable(Currency discountCardsNonVatable);
    void SetDeliveryChargesNonVatable(Currency deliveryChargeNonVatable);
    void SetGiftCertificatesChecksRedeemedNonVatable(Currency giftCertificatesNonVatable);
    void SetSSDiscount1NonVatable(Currency sSDiscount1NonVatable);
    void SetSSDiscount2NonVatable(Currency sSDiscount2NonVatable);
    void SetSSDiscount3NonVatable(Currency sSDiscount3NonVatable);
    void SetSSDiscount4NonVatable(Currency sSDiscount4NonVatable);
    void SetSSDiscount5NonVatable(Currency sSDiscount5NonVatable);
    void SetTotalOfallNonApprovedStoreDiscountsNonVatable(Currency ofallNonApprovedStoreDiscountsNonVatable);
    void SetSSDiscount1NonApprovedNonVatable(Currency sSDiscount1NonApprovedNonVatable);
    void SetSSDiscount2NonApprovedNonVatable(Currency sSDiscount2NonApprovedNonVatable);
    void SetSSDiscount3NonApprovedNonVatable(Currency sSDiscount3NonApprovedNonVatable);
    void SetSSDiscount4NonApprovedNonVatable(Currency sSDiscount4NonApprovedNonVatable);
    void SetSSDiscount5NonApprovedNonVatable(Currency sSDiscount5NonApprovedNonVatable);
    void SetVATAmountNonVatable(Currency vATAmountNonVatable);
    void SetNetSalesAmountNonVatable(Currency netSaleAmountNonVatable);
    void SetCoverCount(int _coverCount);

    public:
    __property Currency OldAccumulatedSalesVatable = {read =  _oldAccSalesVatable, write = SetOldAccSalesVatable};
    __property Currency NewAccumulatedSalesVatable = {read =  _newAccSalesVatable, write = SetNewAccSalesVatable};
    __property Currency GrossAmountVatable =  {read =  _grossAmountVatable, write = SetGrossAmountVatable};
    __property Currency DeductionVatable = {read =  _deductionVatable, write = SetDeductionVatable};
    __property Currency PromoSalesAmountVatable = {read =  _promoSalesAmountVatable, write = SetPromoSalesAmountVatable};
    __property Currency PWDDiscountVatable = {read = _pWDDiscountVatable, write = SetPWDDiscountVatable};
    __property Currency RefundAmountVatable = {read = _refundAmountVatable, write = SetRefundAmountVatable};
    __property Currency ReturnedItemsAmountVatable = {read = _returnedItemsAmountVatable, write = SetReturnedItemsAmountVatable};
    __property Currency OtherTaxesVatable = {read = _otherTaxesVatable, write = SetOtherTaxesVatable};
    __property Currency ServiceChargeAmountVatable = {read = _serviceChargeAmountVatable, write = SetServiceChargeAmountVatable};
    __property Currency AdjustmentDiscountVatable =  {read = _adjustmentDiscountVatable, write = SetAdjustmentDiscountVatable};
    __property Currency VoidAmountVatable = {read =  _voidAmountVatable, write = SetVoidAmountVatable};
    __property Currency DiscountCardsVatable = {read = _discountCardsVatable, write = SetDiscountCardsVatable};
    __property Currency DeliveryChargesVatable =  {read = _deliveryChargesVatable, write = SetDeliveryChargesVatable};
    __property Currency GiftCertificatesChecksRedeemedVatable = {read = _giftCertificatesChecksRedeemedVatable, write = SetGiftCertificatesChecksRedeemedVatable};
    __property Currency SSDiscount1Vatable = {read = _sSDiscount1Vatable, write = SetSSDiscount1Vatable};
    __property Currency SSDiscount2Vatable = {read = _sSDiscount2Vatable, write = SetSSDiscount2Vatable};
    __property Currency SSDiscount3Vatable = {read = _sSDiscount3Vatable, write = SetSSDiscount3Vatable};
    __property Currency SSDiscount4Vatable = {read = _sSDiscount4Vatable, write = SetSSDiscount4Vatable};
    __property Currency SSDiscount5Vatable = {read = _sSDiscount5Vatable, write = SetSSDiscount5Vatable};
    __property Currency TotalOfallNonApprovedSDVatable = {read = _totalOfallNonApprovedStoreDiscountsVatable, write = SetTotalOfallNonApprovedStoreDiscountsVatable};
    __property Currency SSDiscount1NonApprovedVatable = {read = _sSDiscount1NonApprovedVatable, write = SetSSDiscount1NonApprovedVatable};
    __property Currency SSDiscount2NonApprovedVatable = {read = _sSDiscount2NonApprovedVatable, write = SetSSDiscount2NonApprovedVatable};
    __property Currency SSDiscount3NonApprovedVatable = {read = _sSDiscount3NonApprovedVatable, write = SetSSDiscount3NonApprovedVatable};
    __property Currency SSDiscount4NonApprovedVatable = {read = _sSDiscount4NonApprovedVatable, write = SetSSDiscount4NonApprovedVatable};
    __property Currency SSDiscount5NonApprovedVatable = {read = _sSDiscount5NonApprovedVatable, write = SetSSDiscount5NonApprovedVatable};
    __property Currency VATTaxAmountVatable = {read = _vATAmountVatable, write = SetVATAmountVatable};
    __property Currency NetSalesAmountVatable =  {read = _netSalesAmountVatable, write = SetNetSalesAmountVatable};
    __property Currency OldAccumulatedSalesNonVatable =  {read = _oldAccSalesNonVatable, write = SetOldAccSalesNonVatable};
    __property Currency NewAccumulatedSalesNonVatable =  {read = _newAccSalesNonVatable, write = SetNewAccSalesNonVatable};
    __property Currency GrossAmountNonVatable = {read = _grossAmountNonVatable, write = SetGrossAmountNonVatable};
    __property Currency DeductionNonVatable = {read =  _deductionNonVatable, write = SetDeductionNonVatable};
    __property Currency PromoSalesAmountNonVatable = {read = _promoSalesAmountNonVatable, write = SetPromoSalesAmountNonVatable};
    __property Currency SCDDiscountNonVatable = {read =  _sCDDiscountNonVatable, write = SetSCDDiscountNonVatable};
    __property Currency RefundAmountNonVatable = {read = _refundAmountNonVatable, write = SetRefundAmountNonVatable};
    __property Currency ReturnedItemsAmountNonVatable = {read =  _returnedItemsAmountNonVatable, write = SetReturnedItemsAmountNonVatable};
    __property Currency OtherTaxesNonVatable = {read = _otherTaxesNonVatable, write = SetOtherTaxesNonVatable};
    __property Currency ServiceChargeAmountNonVatable = {read =  _serviceChargeAmountNonVatable, write = SetServiceChargeAmountNonVatable};
    __property Currency AdjustmentDiscountNonVatable = {read = _adjustmentDiscountNonVatable, write = SetAdjustmentDiscountNonVatable};
    __property Currency VoidAmountNonVatable =  {read = _voidAmountNonVatable, write = SetVoidAmountNonVatable};
    __property Currency DiscountCardsNonVatable = {read = _discountCardsNonVatable, write = SetDiscountCardsNonVatable};
    __property Currency DeliveryChargesNonVatable = {read = _deliveryChargesNonVatable, write = SetDeliveryChargesNonVatable};
    __property Currency GiftCertificatesChecksRedeemedNonVatable = {read = _giftCertificatesChecksRedeemedNonVatable, write = SetGiftCertificatesChecksRedeemedNonVatable};
    __property Currency SSDiscount1NonVatable = {read = _sSDiscount1NonVatable, write = SetSSDiscount1NonVatable};
    __property Currency SSDiscount2NonVatable = {read = _sSDiscount2NonVatable, write = SetSSDiscount2NonVatable};
    __property Currency SSDiscount3NonVatable = {read = _sSDiscount3NonVatable, write = SetSSDiscount3NonVatable};
    __property Currency SSDiscount4NonVatable = {read = _sSDiscount4NonVatable, write = SetSSDiscount4NonVatable};
    __property Currency SSDiscount5NonVatable =  {read = _sSDiscount5NonVatable, write = SetSSDiscount5NonVatable};
    __property Currency TotalOfallNonApprovedSDiscountsNonVatable = {read = _totalOfallNonApprovedStoreDiscountsNonVatable, write = SetTotalOfallNonApprovedStoreDiscountsNonVatable};
    __property Currency SSDiscount1NonApprovedNonVatable = {read = _sSDiscount1NonApprovedNonVatable, write = SetSSDiscount1NonApprovedNonVatable};
    __property Currency SSDiscount2NonApprovedNonVatable = {read = _sSDiscount2NonApprovedNonVatable, write = SetSSDiscount2NonApprovedNonVatable};
    __property Currency SSDiscount3NonApprovedNonVatable = {read = _sSDiscount3NonApprovedNonVatable, write = SetSSDiscount3NonApprovedNonVatable};
    __property Currency SSDiscount4NonApprovedNonVatable = {read = _sSDiscount4NonApprovedNonVatable, write = SetSSDiscount4NonApprovedNonVatable};
    __property Currency SSDiscount5NonApprovedNonVatable = {read = _sSDiscount5NonApprovedNonVatable, write = SetSSDiscount5NonApprovedNonVatable};
    __property Currency VATTaxAmountNonVatable = {read = _vATAmountNonVatable, write = SetVATAmountNonVatable};
    __property Currency NetSalesAmountNonVatable = {read = _netSalesAmountNonVatable, write = SetNetSalesAmountNonVatable};
    __property int CoverCount = {read = _coverCount, write = SetCoverCount};

};
#endif
