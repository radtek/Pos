//---------------------------------------------------------------------------

#ifndef EstanciaMallH
#define EstanciaMallH
#include "MallExport.h"
//---------------------------------------------------------------------------
const int NUMBER_OF_FIELDS = 65;
UnicodeString fieldNames[NUMBER_OF_FIELDS] =
        {
            "Tenant Code" ,"POS Terminal Number" ,"Date (mmddyyyy" ,"Old Accumulated Sales" ,"New Accumulated Sales" ,"Total Gross Amount" ,"Total Deductions" ,
            "Total Promo Sales Amount" ,"Total PWD Discount" ,"Total Refund Amount" ,"Total Returned Items Amount" ,"Total Other Taxes" ,"Total Service Charge Amount" ,
            "Total Adjustment Discount" ,"Total Void Amount" ,"Total Discount Cards" ,"Total Delivery Charges" ,"Total Gift Certificates/Checks Redeemed " ,
            "Store Specific Discount 1 " ,"Store Specific Discount 2" ,"Store Specific Discount 3" ,"Store Specific Discount 4" ,"Store Specific Discount 5" ,
            "Total of all Non-Approved Store Discounts" ,"Store Specific Discount 1" ,"Store Specific Discount 2" ,"Store Specific Discount 3" ,"Store Specific Discount 4" ,
            "Store Specific Discount 5" ,"Total VAT/Tax Amount" ,"Total Net Sales Amount" ,"Total Cover Count" ,"Control Number" ,"Total Number of Sales Transaction" ,"Sales Type" ,
            "Amount" ,"Old Accumulated Sales" ,"New Accumulated Sales" ,"Total Gross Amount" , "Total Deductions" ,"Total Promo Sales Amount" ,"Senior Citizen Discount" ,
            "Total Refund Amount" , "Total Returned Items Amount" ,"Total Other Taxes" ,"Total Service Charge Amount" ,"Total Adjustment Discount" ,"Total Void Amount" ,
            "Total Discount Cards" ,"Total Delivery Charges" , "Total Gift Certificates/Checks Redeemed" ,"Store Specific Discount 1" ,"Store Specific Discount 2" ,
            "Store Specific Discount 3 " , "Store Specific Discount 4" , "Store Specific Discount 5" ,"Total of all Non-Approved Store Discounts" , "Store Specific Discount 1" ,
            "Store Specific Discount 2" , "Store Specific Discount 3" , "Store Specific Discount 4" , "Store Specific Discount 5" ,"Total VAT/Tax Amount" ,
            "Total Net Sales Amount" ,"Grand Total Net Sales"
        };

//enum ExportValueType {STRING, INTEGER, CURRENCY, TIMESTAMP};


class TEstanciaMall : public TMallExport
{
    public:
    TEstanciaMall();
    void PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey);
    void PrepareDataForExport();
    void CreateExportMedium();
    long GenerateSaleKey(Database::TDBTransaction &dbTransaction);
};
#endif
