//---------------------------------------------------------------------------

#ifndef DeanAndDelucaMallH
#define DeanAndDelucaMallH
#include "MallExport.h"
#include "MallExportTextFile.h"
#include <DateUtils.hpp>
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
class TDeanAndDelucaMallField;
class TDeanAndDelucaMall : public TMallExport
{
private:

    //For terminal condition
    AnsiString terminalCondition;

    //check whether terminal is master terminal
    bool isMasterTerminal;

    //Store device key
    int deviceKey;

    //Get Total Patron count for a Bill
    int GetPatronCount(TPaymentTransaction &paymentTransaction);

    //Get OldAccumulated Sale
    double GetOldAccumulatedSales(Database::TDBTransaction &dbTransaction, int fieldIndex);

    //Generate SalesKey for MallExport_sales Table
    long GenerateSaleKey(Database::TDBTransaction &dbTransaction);

    //Insert Data into MallExport_sales table
    void PushFieldsInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, UnicodeString field,
                                UnicodeString dataType, UnicodeString fieldValue, int fieldIndex, int arcbillKey);

    //Fetch Mall Setting for file writing
    void LoadMallSettingsForFile(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareForDSF, std::set<int> keysToSelect,
                                int index, int zKey = 0);

    //store keys in string format seperated by commas
    UnicodeString GetFieldIndexList(std::set<int> indexKeys);

    //Get File Name According to file type.
    UnicodeString GetFileName(Database::TDBTransaction &dBTransaction, std::set<int> keysToSelect, int zKey = 0);

    //Load File Setting For Invoice file Writing
    void LoadMallSettingsForInvoiceFile(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareForDSF, std::set<int> keysToSelect,
                                        int index, int zKey = 0);

    //insert field into list
    void InsertFieldInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, TDeanAndDelucaMallField fieldData, int arcBillKey);

    protected:

    //Override TMallExport class 's pure virtual function PrepareDataForDatabase(...............)
    std::list<TMallExportSalesData> PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey);

    //Override TMallExport class 's pure virtual function PrepareDataForExport() according to malltype
    TMallExportPrepareData PrepareDataForExport(int zKey = 0);

    //Override TMallExport class 's pure virtual function CreateExportMedium() according to malltype
    IExporterInterface* CreateExportMedium();

    //get which type of file will be exported
    UnicodeString GetExportType();

public:

    //Constructor
    TDeanAndDelucaMall();

     //Prepare data for Invoice Sales File
    void PrepareDataForDiscountFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, int indexKey2, TMallExportPrepareData &prepareDataForInvoice,
                                            int index, int zKey = 0);

    //Fetch Data For Invoice Sales File writing
    void PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, std::set<int> indexKeys2, int indexKey3,
                                        TMallExportPrepareData &prepareDataForHSF, int index, int zKey = 0);

    //Fetch Data For Daily Sales File writing
    void PrepareDataForDailySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys, std::set<int> indexKeys2,
                                        TMallExportPrepareData &prepareDataForDSF, int index, int zKey = 0);

    //Insert Array into set.
    std::set<int> InsertInToSet(int arr[], int size);

};

class TDeanAndDelucaMallField
{
private:
    int _terminalNumber;
    UnicodeString _tenantCode;
    Currency _oldAccSalesTotal;
    Currency _newAccSalesTotal;
    Currency _grossSaleAmount;
    Currency _nonTaxableSaleAmount;
    Currency _totalSCDAmount;
    Currency _totalOtherDiscount;
    Currency _totalRefundAmount;
    Currency _totalTax;
    Currency _totalServiceCharge;
    Currency _totalNetSaleAmount;
    Currency _totalCashSales;
    Currency _totalChargeSales;
    Currency _totalGCSales;
    Currency _totalVoidAmount;
    int _customerCount;
    int _salesCount;
    int _salesType;
    Currency _netSalesAmountPerSalesType;
    int _hourCode;
    int _zkey;


    void SetTerminalNumber(int terminalNumber);
    void SetTenantCode(UnicodeString tenantCode);
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
    void SetCoverCount(int coverCount);
    void SetControlNumber(int controlNumber);
    void SetNoOfSaleTransaction(int noOfSaleTransaction);
    void SetSalesType(int salesType);
    void SetAmount(Currency amount);
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
    void SetInvoiceNumber(UnicodeString invoiceNumber);

public:
    __property int TerminalNumber = {read = _terminalNumber, write = SetTerminalNumber};
    __property UnicodeString TenantCode = {read = _tenantCode, write = SetTenantCode};

};
#endif
