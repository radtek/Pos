//---------------------------------------------------------------------------

#ifndef MallExportMegaworldMallH
#define MallExportMegaworldMallH

#include "MallExportIMall.h"
#include "ManagerVariable.h"
#include <fstream>

//---------------------------------------------------------------------------

struct TMegaworldDiscount
{
    UnicodeString Discount_Name;
    UnicodeString Description;
    Currency Discount_Value;
};

struct TMegaworldDiscountMatched
{
    UnicodeString Discount_Name_Matched;
    UnicodeString Description_Matched;
    Currency Discount_Value_Matched;
};

class TMallExportMegaworldMall : public TMallExportIMall
{
public:
    TMallExportMegaworldMall();
    virtual __fastcall ~TMallExportMegaworldMall();

	// Override methods
    AnsiString CreateFileURI( AnsiString inBaseURL );

    // Delete the methods below if not needed
	TExportResponse ZExport();
	TExportResponse TransactionExport();

    TExportResponse CreateFilename(UnicodeString mode, UnicodeString MallPath, UnicodeString LocalPath,
                                   UnicodeString &LocalPathFileName, UnicodeString &MallPathFileName,TDateTime &DateValueInHourlyFile);
    TExportResponse GetExportInfo(UnicodeString &MallPath, UnicodeString &TenantID,
                                  UnicodeString &TerminalNo, UnicodeString &BatchNo,
                                  UnicodeString &MonthValue, UnicodeString &DayValue,
                                  TDateTime DateValue);

    UnicodeString CheckDir(UnicodeString Path);
    UnicodeString RemoveDecimalValue(UnicodeString amountValue);
    UnicodeString FixDecimalPlaces(Currency AmountTotal);

    void SaveIntVariable(vmVariables vmVar, int CompName);
    void SaveStrVariable(vmVariables vmVar, UnicodeString CompName);

    int GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                      UnicodeString &DateValue, UnicodeString &TimeValue,
                      int &TransactionTotal, UnicodeString &Amount, int &PatronCount);

    TExportResponse PrepareDateForHourly(TDateTime DateValueInHourlyFile);
    TExportResponse PrepareDateForDaily(TDateTime DateValueInHourlyFile, bool prepareZeroDateForDaily = true);
    TExportResponse PrepareDiscounts(TDateTime DateValueInHourlyFile);


    void GetMaxZedKeyAndSecondMaxZedKey(int &maxzedkey,int &maxzedkey2) ;
    void GetOldAndNewGrandTotal(int maxzedkey,Currency &oldgrandtotal,Currency &NewGrandTotal) ;
    bool IsConsolidatedOrNot(bool &IsBreakConSolidateDateForCurrentDate, int MaxZed, int SecondMaxZed);
    void GetFirstDateValueForMaxZedAndSecondMaxZed(TDateTime &DateValue ,UnicodeString &MaxZedDate , UnicodeString &SecondMaxZedDate,int maxzed,int SecondMaxZed);

    TMallExportMegaworldMall* megaworldExport;
    UnicodeString OutputValue;

protected:
	// Override methods
    void initTimingExportResponse( TExportResponse& outResponse );
private:
    void WriteDataAccordingToSalesType(UnicodeString saleTypeCode, Currency SaleTypaAmount, int index, int salesTypeSize);
    void WriteDataAccordingToDifferentSalesTotal(UnicodeString salesTotalCode, Currency salesTotalAmount);
    void WriteDataAccordingToDifferentCountType(UnicodeString countTypeCode, int countTotal);
    void CheckTransactionDoneBeforeZed(TDateTime &dateValueField, int &zeroZCount, bool &zeroZDoneBeforeAnySale, bool &prepareDataForZeroSales);
    void GetMaxZKeyFromArcMallExport(int &maxzed);
    TExportResponse PrepareZeroSalesDateForHourly(TDateTime DateFieldInHourlyData);
    TExportResponse PrepareZeroSalesDiscounts(TDateTime DateFieldInDailyData);
    void LoadMaxZkeyAndDateFromZed(int &maxZKeyFromZed, TDateTime &maxZDateFromZed);
    void LoadMaxZkeyAndDateFromArcMallExportHourly(int &maxZKeyFromArcMallExportHourly, TDateTime &maxZDateFromArcMallExportHourly);
    int GetZCountForZedsDoneBeforeAnySale(int maxZKeyFromArcMallExportHourly);
    bool CheckIfZeroZedDoneBeforeAnySale(int maxZKeyFromArcMallExportHourly);

};

//---------------------------------------------------------------------------
#endif
