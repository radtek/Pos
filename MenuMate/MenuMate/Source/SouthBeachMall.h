//---------------------------------------------------------------------------

#ifndef SouthBeachMallH
#define SouthBeachMallH
#include "MallExport.h"
#include "MallExportTextFile.h"
//---------------------------------------------------------------------------

class TSouthBeachMallField
{
private:
    UnicodeString _machineID;
    int _batchID;           //for zkey
    UnicodeString _hour;
    int _receiptcount;
    double _netsalesamount;
    double _gst;
    double _totaldiscount;
    int _noOfPax;
    int _noOfRefund;
    double _totalserviceCahrge;
    std::map<int, double> _salesBysalesType;
    int _ResetCount;

    void SetMachineID(UnicodeString machineID) ;
    void SetBatchID(int batchID ) ;
    void SetHour(UnicodeString hour) ;
    void SetReceiptcount(int receiptcount) ;
    void SetNetsalesamount(double netsalesamount) ;
    void SetGst(double gst) ;
    void SetTotalDiscount(double totaldiscount) ;
    void SetNoOfPax(int noOfPax) ;
    void SetTotalServiceCharge(double totalserviceCahrge) ;
    void SetResetCount(int resetcount) ;

public:
    __property UnicodeString MachineID = {read = _machineID, write = SetMachineID};
    __property int BatchID = {read = _batchID, write = SetBatchID};
    __property UnicodeString Hour = {read = _hour, write = SetHour};
    __property int ReceiptCount = {read = _receiptcount, write = SetReceiptcount};
    __property double NetSalesAmount = {read = _netsalesamount, write = SetNetsalesamount};
    __property double GST = {read = _gst, write = SetGst};
    __property double TotalDiscount = {read = _totaldiscount, write = SetTotalDiscount};
    __property int NoOfPax = {read = _noOfPax, write = SetNoOfPax};
    __property double TotalServiceCharge = {read = _totalserviceCahrge, write = SetTotalServiceCharge};
    __property int ResetCount = {read = _ResetCount, write = SetResetCount};
     TSouthBeachMallField();
};

class TSouthBeachMall : public TMallExport
{
private:
    void PrepareDataByItem(Database::TDBTransaction &dbTransaction, TItemMinorComplete *Order, TSouthBeachMallField &fieldData,bool &IsFAndBOutlet);
    bool IsItemVatable(TItemMinorComplete *order, TSouthBeachMallField &fieldData, double &Vat);
    void InsertFieldInToList(Database::TDBTransaction &dbTransaction, std::list<TMallExportSalesData> &mallExportSalesData, TSouthBeachMallField &fieldData, int arcBillKey);
    UnicodeString GetFileName(Database::TDBTransaction &dBTransaction, int zKey = 0);
    int GetItemSalesId(Database::TDBTransaction &dbTransaction, int itemKey);
    bool IsConsolidatedOrNot(int MaxZed, int SecondMaxZed);
    int GetMinimumArcBillkeyForMaxZed(int Zed_Key,Database::TDBTransaction &dBTransaction);
    void SaveIntVariable(vmVariables vmVar, int CompName);
    UnicodeString GetDateValueForMaxZed(int zedkey);
    void PrepareDataForDailySalesFile(Database::TDBTransaction &dBTransaction,TMallExportPrepareData &prepareDataForDSF,int index, int zKey);
    void UpdateBatchIDForSouthBeach(int fieldindex,int ZedKey,int BatchNo);
    bool IsAnyZeroSalesDoneBeforeMaxZed(int ZedKey);
    bool CompareZedAndSaleDate(int ZedKey);
    bool IsAnyTransactionPerformed();
    TDateTime GetZedDateForSecondMaxZed(int SecondMaxZed,int &BatchNumber) ;
    TDateTime GetMinimumDateForMaxZed(int MaxZed, UnicodeString &TimeFormat) ;
    void GetMaxZedKeyInEachIteration(int &Zed_Key_File,int index);
    TDateTime GetMallZedDate(int MaxZed) ;
    TDateTime GetZedsZedDate(int MaxZed,double &terminalearnings) ;
    TMallExportSalesData GetSalesDataQuery(Database::TDBTransaction &dBTransaction,int ZKey,int BatchNumber,int BatchID);
    TMallExportSalesData PrepareZeroSalesData(UnicodeString MachineID , UnicodeString SalesDateValue,int BatchID);
    UnicodeString GetMaxDateInZed(int Zedkey) ;
    void PrepareDataforFilename(int Zedkey,UnicodeString &MachineId ,UnicodeString &dateformat ,UnicodeString &timeformat,UnicodeString &ValidateDateFormat);
    TDateTime GetDateAtFieldIndex(int ZedKey,int fieldindex);
    TDateTime FetchMinimumDate(int ZedKey);
    //Code declaration for regenerate of southbeach mall
    UnicodeString Machine_ID;
    UnicodeString Hour_Data;
    UnicodeString Date_Data ;
    UnicodeString TimeFormat;
    int BatchId_Data;
    int ReceiptCount_Data;
    int NoOfPax_Data;
    double GTO_Data;
    double GST_Data;
    double Discount_Data;
    void GetListOfDatesBetwSdateEndDate(TDateTime sDate, TDateTime eDate);
    void GetTotalZedCorrespondingDate(TDateTime Startdate,TDateTime EndDate,UnicodeString Datevalue,TDateTime DateValueFormat) ;
    void CheckFirstSaleOfEachZed(int Zedkey,TDateTime Startdate,TDateTime EndDate,UnicodeString Datevalue) ;
    void PrepareDataForRegenerateFile(TMallExportPrepareData &prepareDataForDSF,int Zedkey,UnicodeString DateForFile);
    void ResetVariablesForConsolidatedData();
    void CheckNoSaleDataForDateValue(TDateTime DateValue);
    UnicodeString GetTimeForMallOutletClosureFile(TDateTime DateValue);
    int GetPatronCount(TPaymentTransaction &paymentTransaction);
    TMallExportSalesData PrepareDataForSingleDaySalesFile(Database::TDBTransaction &dBTransaction,int Z_Key);
    UnicodeString GetQueryForSalesData();
    //
protected:
    TMallExportSalesWrapper PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey, TDateTime currentTime);
    TMallExportPrepareData PrepareDataForExport(int zKey = 0) ;

public:
    TSouthBeachMall();
    void RegenerateMallReport(TDateTime sDate, TDateTime eDate);
};
#endif

