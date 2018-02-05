//---------------------------------------------------------------------------

#ifndef ManagerPMSCodesH
#define ManagerPMSCodesH

#include "MM_DBCore.h"
#include "Grids.hpp"
#include <memory>
//---------------------------------------------------------------------------
enum RecordStatus
{
    oldRecord = 0, newRecord, modifiedRecord, deletedrecord
};
struct TaxCodesDetails
{
    double VatPercentage;
    double ServiceChargePercentage;
    double TaxOnServiceChargePercentage;
};
struct TTimeSlots
{
    int key;
    AnsiString MealName;
    TDateTime StartTime;
    TDateTime EndTime;
    bool IsDefault;
};
struct TRevenueCodeDetails
{
    bool IsDefault;
    AnsiString RevenueCodeDescription;
};
class TManagerPMSCodes
{
    private:
    public:
        TManagerPMSCodes();
        ~TManagerPMSCodes();
        std::map<int,TaxCodesDetails> TaxCodesMap;
        std::map<int,TRevenueCodeDetails> RevenueCodesMap;
        void GetTaxCodesDetails(Database::TDBTransaction &DBTransaction, TStringGrid * StringGrid,
                                std::map<int,TaxCodesDetails> &TaxCodesMap);
        void GetRevenueCodesDetails(Database::TDBTransaction &DBTransaction, TStringGrid * StringGrid,
                                std::map<int,TRevenueCodeDetails> &RevenueCodesMap);
        void GetTaxCodesFromDB(Database::TDBTransaction &DBTransaction,
                               std::map<int,TaxCodesDetails> TaxCodesMap);
        void GetRevenueCodesFromDB(Database::TDBTransaction &DBTransaction,
                               std::map<int,TRevenueCodeDetails> &RevenueCodeMap);
        void PopulateTaxesToGrid(std::map<int,TaxCodesDetails> &TaxCodesMap,TStringGrid * StringGrid);
        void PopulateRevenuesToGrid(std::map<int,TRevenueCodeDetails> &RevenueCodeMap,TStringGrid * StringGrid);
        void SaveRevenueCodesToDB(Database::TDBTransaction &DBTransaction,std::map<int,TRevenueCodeDetails> &RevenueCodeMap);
        void DeleteRevenueCode(Database::TDBTransaction &DBTransaction, int key);
        void EditRevenueCode(Database::TDBTransaction &DBTransaction,int key, int newCode, TRevenueCodeDetails codeDetails);
        void InsertTimeSlots(Database::TDBTransaction &DBTransaction,TTimeSlots mealDetails);
        std::vector<TTimeSlots> TimeSlots;
        void GetMealDetails(Database::TDBTransaction &DBTransaction,TStringGrid * StringGrid,std::vector<TTimeSlots> &TimeSlots);
        void PopulateMealsToGrid(TStringGrid * StringGrid,std::vector<TTimeSlots> TimeSlots);
        void EditMeal(Database::TDBTransaction &DBTransaction,TTimeSlots slots);
        void DeleteMealDetails(Database::TDBTransaction &DBTransaction,int key);
        void UpdateItemSizes(Database::TDBTransaction &DBTransaction,int newValue,int key);
};
#endif
