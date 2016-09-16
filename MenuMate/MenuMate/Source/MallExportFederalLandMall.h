//---------------------------------------------------------------------------

#ifndef MallExportFederalLandMallH
#define MallExportFederalLandMallH

#include "MallExportIMall.h"
#include "ManagerVariable.h"
#include <vector>
#include <fstream>

#include "ReportUtilities.h"
#include "ReportFinancialCalculations.h"

//---------------------------------------------------------------------------

class TMallExportFederalLandMall : public TMallExportIMall
{
public:
    TMallExportFederalLandMall();
    virtual __fastcall ~TMallExportFederalLandMall();

	// Override methods
    AnsiString CreateFileURI( AnsiString inBaseURL );

    // Delete the methods below if not needed
	TExportResponse ZExport();
    TExportResponse CreateFilename(UnicodeString mode, UnicodeString MallPath, UnicodeString &MallPathFileName);
    TExportResponse GetExportInfo(UnicodeString &MonthValue, UnicodeString &DayValue, TDateTime DateValue);
    TExportResponse CreateOutputFormatModeS(std::map<UnicodeString, UnicodeString> DataRead,
                            std::vector<UnicodeString> &DataToWrite);
    TExportResponse CreateOutputFormatModeD(std::map<UnicodeString, UnicodeString> DataRead,
                            std::vector<UnicodeString> &DataToWrite, UnicodeString Query);
    TExportResponse CreateOutputFormatModeP(std::map<UnicodeString, UnicodeString> DataRead,
                            std::vector<UnicodeString> &DataToWrite, UnicodeString Query);
    TExportResponse CreateOutputFormatModeR(std::map<UnicodeString, UnicodeString> DataRead,
                            std::vector<UnicodeString> &DataToWrite, UnicodeString Query);
    TExportResponse CreateOutputFormatModeH(std::map<UnicodeString, UnicodeString> DataRead,
                            std::vector<UnicodeString> &DataToWrite, UnicodeString MallPathFileName, UnicodeString Query);

    UnicodeString CheckDir(UnicodeString Path, TDateTime DateValue);
    UnicodeString getFederalLandQuery(UnicodeString mode);
    UnicodeString GetHourlyFormat(UnicodeString TerminalName, UnicodeString TenantName,
                                  UnicodeString DateValue, UnicodeString TimeValue, int PatronCount,
                                  UnicodeString MinuteValue, int SCDiscountCount);
    UnicodeString GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                                UnicodeString &DateValue, UnicodeString &TimeValue,
                                int &TransactionTotal, UnicodeString &Amount,
                                UnicodeString &MinuteValue, int &SCDiscountCount, int &PatronCount);
    UnicodeString FixDecimalPlaces(Currency AmountTotal);

    void CreateHeaderFormat(UnicodeString MallPathFileName, std::vector<UnicodeString> &DataToWrite, UnicodeString Status);
    void SaveStrVariable(vmVariables vmVar, UnicodeString CompName);

    TDateTime CheckLastZed();

    void SetZedTimer();
    void CheckPreviousEODFile();

    std::fstream CheckPreviousFile;
    std::fstream CheckFile;

    ReportFinancialCalculations* reportFinancialCalculations;

    void GenerateFileForDaysPOSNotUsed(UnicodeString MallPath, UnicodeString MerchantCode,
                                       UnicodeString CheckDate, UnicodeString Format, UnicodeString DateFormat);

    bool CheckPreviousZedDateTime();
    void SaveIntVariable(vmVariables vmVar, int CompName);

protected:
	// Override methods
    void initTimingExportResponse( TExportResponse& outResponse );
};

//---------------------------------------------------------------------------
#endif
