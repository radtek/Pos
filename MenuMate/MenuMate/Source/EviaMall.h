

#ifndef EviamallH
#define EviamallH
#include "MallExport.h"
#include "MallExportEviaSalFile.h"
#include "MallExportTextFile.h"
//---------------------------------------------------------------------------


class TEviaMallField;
class TEviaMall : public TMallExport
{
   private:


        AnsiString terminalCondition;
       bool isMasterTerminal;
       int deviceKey;
       void PrepareDataByItem(Database::TDBTransaction &dbTransaction, TItemMinorComplete *Order, TEviaMallField &fieldData);
       void CalculateTaxesFields(TItemMinorComplete *order, TEviaMallField &fieldData);
       void InsertFieldInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, TEviaMallField &fieldData, int arcBillKey);
     //  double grosssaleamount;
       double GetOldGrandTotal(Database::TDBTransaction &dbTransaction, int fieldIndex);
       int GetItemSalesId(Database::TDBTransaction &dbTransaction, int itemKey);
       UnicodeString GetFieldIndexList(std::set<int> indexKeys);
       UnicodeString GetFileName(Database::TDBTransaction &dBTransaction, std::set<int> keysToSelect, int zKey = 0);
       UnicodeString GetSaleDeptName(Database::TDBTransaction &dbTransaction,int itemKey );
       int GetMaxZedKey(Database::TDBTransaction &dbTransaction, int zKey = 0);
       bool CheckSingleOrMultiplePos(Database::TDBTransaction &dbTransaction, int zKey);
       UnicodeString GetMaxTimeDateCreated(Database::TDBTransaction &dbTransaction, int zKey = 0);
       void LoadCommonFields(Database::TDBTransaction &dBTransaction, TMallExportPrepareData &prepareForHSF, std::list<TMallExportSettings> &mallSettings, std::set<int> keysToSelect,
                                        int index, int zKey = 0);
       int Getdevicekey(Database::TDBTransaction &dbTransaction, int zKey ,std::vector<int> &devicekeyvalue);



   protected:

    TMallExportSalesWrapper PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey, TDateTime currentTime);
    TMallExportPrepareData PrepareDataForExport(int zKey = 0) ;
    IExporterInterface* CreateExportMedium();
    UnicodeString GetExportType();


   public:
     TEviaMall();
   //  void PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys1, std::set<int> indexKeys2,std::set<int> indexKeys3,
                                      //  TMallExportPrepareData &prepareDataForHSF, int index, int zKey = 0);
     void PrepareDataForHourlySalesFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys1, std::set<int> indexKeys2,std::set<int> indexKeys3,
                                        TMallExportPrepareData &prepareDataForHSF,std::list<TMallExportSalesData> &prepareListForPreparedata, int index,int poskey,int zKey = 0);
     void PrepareDataForDailySalesPerDeptFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys1, std::set<int> indexKeys2,int index1,int index2,
                                        TMallExportPrepareData &prepareDataForDSFPD,std::list<TMallExportSalesData> &prepareListForHSF, int index,int terminalkey, int zKey = 0);

    void PrepareDataForGrandTotalsFile(Database::TDBTransaction &dBTransaction, std::set<int> indexKeys1,int index1,
                                        TMallExportPrepareData &prepareDataForDGT,std::list<TMallExportSalesData> &prepareListForGrand, int index,int terminalkey, int zKey = 0);

     std::set<int> InsertInToSet(int arr[], int size);
     std::vector<int> vec1;
     GetPosCount(Database::TDBTransaction &dbTransaction, int zKey);

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
    int _hourCode;

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
    std::map<int, double> SalesBySalesType;


    TEviaMallField();


};
#endif






