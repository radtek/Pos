//---------------------------------------------------------------------------

#ifndef EviaMallH
#define EviaMallH
#include "MallExport.h"
//---------------------------------------------------------------------------
class TEviaMallField
{
public:
    UnicodeString recordId;
    UnicodeString stallCode;
    TDateTime salesDate;
    TTime salesTime;
    double grossSales;
    double vat;
    double discount;
    double serviceCahrge;
    int noOfTrans;
    UnicodeString salesDept;
    int noOfRecord;
    double refund;
    int cancelledTransaction;
    double cancelledAmount;
    double nonVatableGrossSales;
    int posNumber;
    double oldGrandTotal;
    double newGrandTotal;
    TEviaMallField();
};


class TEviaMall : public TMallExport
{
private:
    //Prepare DataBy Item initilize mall field by item
    void PrepareDataByItem(Database::TDBTransaction &dbTransaction, TItemMinorComplete *Order, TEviaMallField &fieldData);
    bool IsItemVatable(TItemMinorComplete *order, TEviaMallField &fieldData);

public:
    TEviaMall();

protected:
    //Override TMallExport class 's pure virtual function PrepareDataForDatabase(...............)
    TMallExportSalesWrapper PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey, TDateTime currentTime);
};
#endif
