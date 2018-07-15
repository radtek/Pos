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
    TExportResponse PrepareDateForDaily(TDateTime DateValueInHourlyFile);
    TExportResponse PrepareDiscounts(TDateTime DateValueInHourlyFile);


    void GetMaxZedKeyAndSecondMaxZedKey(int &maxzedkey,int &maxzedkey2) ;
    void GetOldAndNewGrandTotal(int maxzedkey,Currency &oldgrandtotal,Currency &NewGrandTotal) ;
    bool IsConsolidatedOrNot(bool &IsBreakConSolidateDateForCurrentDate, int MaxZed, int SecondMaxZed);
    void GetFirstDateValueForMaxZedAndSecondMaxZed(TDateTime &DateValue ,UnicodeString &MaxZedDate , UnicodeString &SecondMaxZedDate,int maxzed,int SecondMaxZed);

    TMallExportMegaworldMall* megaworldExport;
    UnicodeString OutputValue;
    UnicodeString TENANT_NAME;

    Currency NEW_GRANDTOTAL;
    Currency OLD_GRANDTOTAL;

    Currency REGDISCOUNT_AMOUNT;

    Currency SCDISCOUNT_AMOUNT;

    Currency VAT_SALES;

    Currency DAILY_SALES;
    Currency VOID_AMOUNT;
    Currency REFUND_AMOUNT;

    Currency SCHARGE_AMOUNT;

    int TRANSACTION_COUNT;

    UnicodeString TERMINAL_NAME;

    TDateTime TRANSACTIONDATE;
    Currency GROSS_SALES;

    Currency CASH_SALES;

    int FINEDINECUST_COUNT;

    int CURRENTEODCOUNTER;

    Currency CARD_SALES;
    Currency OTHER_SALES;

    int VATEXEMPT_SALES_COUNT;
    Currency VATEXEMPT_SALES;
    Currency SALESTYPE_FOOD;
    Currency SALESTYPE_NONFOOD;
    Currency SALESTYPE_GROCERIES;
    Currency SALESTYPE_MEDICINES;
    Currency SALESTYPE_OTHERS;

protected:
	// Override methods
    void initTimingExportResponse( TExportResponse& outResponse );

};

//---------------------------------------------------------------------------
#endif
