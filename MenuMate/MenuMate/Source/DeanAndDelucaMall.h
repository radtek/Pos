//---------------------------------------------------------------------------

#ifndef DeanAndDelucaMallH
#define DeanAndDelucaMallH
#include "MallExport.h"
#include "MallExportTextFile.h"
//---------------------------------------------------------------------------
class TDeanAndDelucaMallField;

struct TDeanAndDelucaTaxes
{
    double salesTax;
    double serviceCharge;
    double serviceChargeTax;
    double localTax;
};

class TDeanAndDelucaMall : public TMallExport
{
private:

    //For terminal condition
    AnsiString terminalCondition;

    //check whether terminal is master terminal
    bool isMasterTerminal;

    //Store device key
    int deviceKey;

    //For storing bill time.
    TDateTime billedTime;

    //Fetch Mall Setting for file writing
    void LoadMallSettingsForFile(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareForDSF, UnicodeString indexKeysList,
                                int index, int zKey = 0);

    //store keys in string format seperated by commas
    UnicodeString GetFieldIndexList(std::set<int> indexKeys);

    //Get File Name According to file type.
    UnicodeString GetFileName(Database::TDBTransaction &dBTransaction, UnicodeString indexKeysList, int zKey = 0);

    //Load File Setting For Invoice file Writing
    void LoadMallSettingsForInvoiceFile(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareForDSF, std::set<int> keysToSelect,
                                        int index, int zKey = 0);

    //insert field into list
    void InsertFieldInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, TDeanAndDelucaMallField &fieldData, int arcBillKey);

    //Check whether item is assigned to any sales type..
    int GetItemSalesId(Database::TDBTransaction &dbTransaction, int itemKey);

    //Get Max Zed Key Present in mall Table..
    int GetMaxZedKey(Database::TDBTransaction &dbTransaction, int zKey = 0);

     //Prepare data for Invoice Sales File
    void PrepareDataForDiscountFile(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareDataForDiscount, int index, int zKey = 0);

    //Fetch Data For Invoice Sales File writing
    void PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction, UnicodeString indexKeysList, UnicodeString indexKeysList2, int indexKey3,
                                        TMallExportPrepareData &prepareDataForHSF, int index, int zKey = 0);

    //Fetch Data For Daily Sales File writing
    void PrepareDataForDailySalesFile(Database::TDBTransaction &dBTransaction, UnicodeString indexKeysList, int zIndex,
                                        TMallExportPrepareData &prepareDataForDSF, int index, int zKey = 0);

    //Get month code in hex
    UnicodeString GetMonthCode(int month);

     //Check Whether Item is vatable
    bool IsItemVatable(TItemMinorComplete *order, TDeanAndDelucaTaxes &delucaTaxes);

protected:

    //Override TMallExport class 's pure virtual function PrepareDataForDatabase(...............)
    TMallExportSalesWrapper PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey, TDateTime currentTime);

    //Override TMallExport class 's pure virtual function PrepareDataForExport() according to malltype
    TMallExportPrepareData PrepareDataForExport(int zKey = 0);

    //Override TMallExport class 's pure virtual function CreateExportMedium() according to malltype
    IExporterInterface* CreateExportMedium();

public:
    //Constructor
    TDeanAndDelucaMall();

    //Prepare DataBy Item initilize mall field by item
    void PrepareDataByItem(Database::TDBTransaction &dbTransaction, TItemMinorComplete *Order, TDeanAndDelucaMallField &fieldData);

};

class TDeanAndDelucaMallField
{
private:
    int _terminalNumber;
    UnicodeString _tenantCode;
    double _oldAccSalesTotal;
    double _newAccSalesTotal;
    double _grossSaleAmount;
    double _nonTaxableSaleAmount;
    double _totalSCDAndPWDAmount;
    double _totalOtherDiscount;
    double _totalRefundAmount;
    double _totalTax;
    double _totalServiceCharge;
    double _totalNonTaxableSC;
    double _totalNetSaleAmount;
    double _totalCashSales;
    double _totalChargedSales;
    double _totalGCSales;
    double _totalVoidAmount;
    double _scdAmount;
    double _pwdAmount;
    int _customerCount;
    int _salesCount;
    int _hourCode;
    int _zkey;
    std::map<int, double> _salesBysalesType;


    void SetTerminalNumber(int terminalNumber);
    void SetTenantCode(UnicodeString tenantCode);
    void SetOldAccSalesTotal(double oldAccSalesTotal);
    void SetNewAccSalesTotal(double newAccSaleTotal);
    void SetGrossSaleAmount(double grossSaleAmount);
    void SetNoNTaxableSaleAmount(double nonTaxableSaleAmount);
    void SetSCDAndPWDDiscount(double scdAndPWDDiscount);
    void SetOtherDiscount(double otherDiscount);
    void SetRefundAmount(double refundAmount);
    void SetTax(double tax);
    void SetServiceCharge(double serviceCharge);
    void SetNetSaleAmount(double netSaleAmount);
    void SetCashSales(double cashSales);
    void SetTotalChargedSales(double chargedSales);
    void SetTotalGCSales(double gcSales);
    void SetVoidAmount(double voidAmount);
    void SetSCDDiscount(double scdAmount);
    void SetPWDDiscount(double pwdAmount);
    void SetCustomerCount(int customerCount);
    void SetSalesCount(int salesCount);
    void SetHourCode(int hourCode);
    void SetZKey(int zKey);
    void SetSalesBySalesType(std::map<int, double> salesBySalestype);
    void SetNonTaxableServiceCharge(double nonTaxableServiceCharge);

public:
    __property int TerminalNumber = {read = _terminalNumber, write = SetTerminalNumber};
    __property UnicodeString TenantCode = {read = _tenantCode, write = SetTenantCode};
    __property double OldAccSalesTotal = {read = _oldAccSalesTotal, write = SetOldAccSalesTotal};
    __property double NewAccSalesTotal = {read = _newAccSalesTotal, write = SetNewAccSalesTotal};
    __property double GrossSaleAmount = {read = _grossSaleAmount, write = SetGrossSaleAmount};
    __property double NonTaxableSaleAmount = {read = _nonTaxableSaleAmount, write = SetNoNTaxableSaleAmount};
    __property double TotalSCDAndPWDAmount = {read = _totalSCDAndPWDAmount, write = SetSCDAndPWDDiscount};
    __property double TotalOtherDiscount = {read = _totalOtherDiscount, write = SetOtherDiscount};
    __property double TotalRefundAmount = {read = _totalRefundAmount, write = SetRefundAmount};
    __property double TotalTax = {read = _totalTax, write = SetTax};
    __property double TotalServiceCharge = {read = _totalServiceCharge, write = SetServiceCharge};
    __property double TotalNonTaxableSC = {read = _totalNonTaxableSC, write = SetNonTaxableServiceCharge};
    __property double TotalNetSaleAmount = {read = _totalNetSaleAmount, write = SetNetSaleAmount};
    __property double TotalCashSales = {read = _totalCashSales, write = SetCashSales};
    __property double TotalChargedSales = {read = _totalChargedSales, write = SetTotalChargedSales};
    __property double TotalGCSales = {read = _totalGCSales, write = SetTotalGCSales};
    __property double TotalVoidAmount = {read = _totalVoidAmount, write = SetVoidAmount};
    __property double TotalSCDAmount = {read = _scdAmount, write = SetSCDDiscount};
    __property double TotalPWDAmount = {read = _pwdAmount, write = SetPWDDiscount};
    __property int CustomerCount = {read = _customerCount, write = SetCustomerCount};
    __property int SalesCount = {read = _salesCount, write = SetSalesCount};
    __property int HourCode = {read = _hourCode, write = SetHourCode};
    __property int ZKey = {read = _zkey, write = SetZKey};
    __property std::map<int, double> SalesBySalesType = {read = _salesBysalesType, write =  SetSalesBySalesType};
    TDeanAndDelucaMallField();
};
#endif
