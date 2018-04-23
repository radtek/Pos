

#ifndef EviamallH
#define EviamallH
#include "MallExport.h"
#include "MallExportTextFile.h"

//---------------------------------------------------------------------------


class TEviaMallField;
struct TEviaMallDiscount
{
    double scdDiscount;
    double pwdDiscount;
    double otherDiscount;
};

class TEviaMall : public TMallExport
{
   private:
   AnsiString terminalCondition;
   bool isMasterTerminal;
   int deviceKey;
   void PrepareDataByItem(Database::TDBTransaction &dbTransaction, TItemMinorComplete *Order, TEviaMallField &fieldData,TPaymentTransaction &paymentTransaction);
   void InsertFieldInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, TEviaMallField &fieldData, int arcBillKey);
   int GetItemSalesId(Database::TDBTransaction &dbTransaction, int itemKey);
   UnicodeString GetFieldIndexList(std::set<int> indexKeys);
   UnicodeString GetFileName(Database::TDBTransaction &dBTransaction, std::set<int> keysToSelect, int zKey = 0);
   UnicodeString GetSaleDeptName(Database::TDBTransaction &dbTransaction,int itemKey, int saletypeid );
   int GetMaxZedKey(Database::TDBTransaction &dbTransaction, int zKey = 0);
   bool CheckSingleOrMultiplePos(Database::TDBTransaction &dbTransaction, int zKey);
   void Getdevicekey(Database::TDBTransaction &dbTransaction, int zKey ,std::vector<int> &devicekeyvalue);
   bool IsItemVatable(TItemMinorComplete *order, TEviaMallField &fieldData);
   TEviaMallDiscount PrepareDiscounts(Database::TDBTransaction &dbTransaction, TItemMinorComplete *order);
   void PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys1, std::set<int> indexKeys2,std::set<int> indexKeys3,
                                                TMallExportPrepareData &prepareDataForHSF,std::list<TMallExportSalesData> &prepareListForPreparedata, int index,int poskey,int zKey = 0);
   void PrepareDataForDailySalesPerDeptFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys1, std::set<int> indexKeys2,int index1,int index2,
                                                TMallExportPrepareData &prepareDataForDSFPD,std::list<TMallExportSalesData> &prepareListForHSF, int index,int terminalkey, int zKey = 0);

   void PrepareDataForGrandTotalsFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys1,int index1,
                                                TMallExportPrepareData &prepareDataForDGT,std::list<TMallExportSalesData> &prepareListForGrand, int index,int terminalkey, int zKey = 0);

   std::set<int> InsertInToSet(int arr[], int size);

   protected:

    TMallExportSalesWrapper PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey, TDateTime currentTime);
    TMallExportPrepareData PrepareDataForExport(int zKey = 0) ;
    IExporterInterface* CreateExportMedium();
   public:
     TEviaMall();

} ;


class TEviaMallField
{
private:
    UnicodeString _recordId;
    UnicodeString _stallCode;
    TDateTime _salesDate;
    UnicodeString _salesTime;
    double _grossSales;
    double _totalvat;
    double _totaldiscount;
    double _totalserviceCahrge;
    int _noOfTrans;
    UnicodeString _salesDept;
    int _noOfRefund;
    double _totalrefund;
    int _noOfcancelledTransaction;
    double _totalcancelledAmount;
    double _nonVatableGrossSales;
    int _posNumber;
    double _oldGrandTotal;
    double _newGrandTotal;
    int _zkey;
    double _dailysalestotalperDept;
    double _totaltaxwithoutvat  ;
    UnicodeString _invoiceNumber;
    std::map<int, double> _salesBysalesType;

    void SetRecordID(UnicodeString recordId) ;
    void SetStallCode(UnicodeString stallCode) ;
    void SetSalesDate(TDateTime salesDate) ;
    void SetSalesTime(UnicodeString salesTime) ;
    void SetGrossSales(double grossSales) ;
    void SetTotalVat(double totalvat) ;
    void SetTotalDiscount(double totaldiscount) ;
    void SetTotalServiceCharge(double totalserviceCahrge) ;
    void SetNoOfTransactions(int noOfTrans) ;
    void SetSalesDept(UnicodeString salesDept) ;
    void SetNoOfRefund(int noOfRefund) ;
    void SetTotalRefund(double totalrefund) ;
    void SetNoOfcancelledTransaction(int noOfcancelledTransaction) ;
    void SetTotalcancelledAmount(double totalcancelledAmount) ;
    void SetNonVatableGrossSales(double nonVatableGrossSales) ;
    void SetPosNumber(int posNumber) ;
    void SetOldGrandTotal(double oldGrandTotal) ;
    void SetNewGrandTotal(double newGrandTotal) ;
    void SetZKey(int zKey);
    void SetTotaltaxWithoutVat(double totaltaxwithoutvat) ;
    void SetSalesBySalesType(std::map<int, double> salesBySalestype);

public:
    __property UnicodeString RecordId = {read = _recordId, write = SetRecordID};
    __property UnicodeString StallCode = {read = _stallCode, write = SetStallCode};
    __property TDateTime SalesDate = {read = _salesDate, write = SetSalesDate};
    __property UnicodeString SalesTime = {read = _salesTime, write = SetSalesTime};
    __property double GrossSales = {read = _grossSales, write = SetGrossSales};
    __property double TotalVat = {read = _totalvat, write = SetTotalVat};
    __property double TotalDiscount = {read = _totaldiscount, write = SetTotalDiscount};
    __property double TotalServiceCharge = {read = _totalserviceCahrge, write = SetTotalServiceCharge};
    __property int NoOfTransactions = {read = _noOfTrans, write = SetNoOfTransactions};
    __property UnicodeString SalesDept = {read = _salesDept, write = SetSalesDept};
    __property int NoOfRefund = {read = _noOfRefund, write = SetNoOfRefund};
    __property double TotalRefund = {read = _totalrefund, write = SetTotalRefund};
    __property int NoOfcancelledTransaction = {read = _noOfcancelledTransaction, write = SetNoOfcancelledTransaction};
    __property double TotalcancelledAmount = {read = _totalcancelledAmount, write = SetTotalcancelledAmount};
    __property double NonVatableGrossSales = {read = _nonVatableGrossSales, write = SetNonVatableGrossSales};
    __property int PosNumber = {read = _posNumber, write = SetPosNumber};
    __property double OldGrandTotal = {read = _oldGrandTotal, write = SetOldGrandTotal};
    __property double NewGrandTotal = {read = _newGrandTotal, write = SetNewGrandTotal};
    __property int ZKey = {read = _zkey, write = SetZKey};
    __property double TotaltaxWithoutVat = {read = _totaltaxwithoutvat, write = SetTotaltaxWithoutVat};
    __property std::map<int, double> SalesBySalesType = {read = _salesBysalesType, write =  SetSalesBySalesType};

    TEviaMallField();
};
#endif






