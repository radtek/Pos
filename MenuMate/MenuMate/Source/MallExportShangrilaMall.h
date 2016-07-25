//---------------------------------------------------------------------------

#ifndef MallExportShangrilaMallH
#define MallExportShangrilaMallH
//---------------------------------------------------------------------------

#include "MallExportIMall.h"
#include "ManagerVariable.h"
#include <string>
#include <vector>
#include <fstream>
//---------------------------------------------------------------------------

class TMallExportShangrilaMall : public TMallExportIMall
{
public:
    TMallExportShangrilaMall();
    virtual __fastcall ~TMallExportShangrilaMall();

	// Override methods
    AnsiString CreateFileURI( AnsiString inBaseURL );

    // Delete the methods below if not needed
	TExportResponse ZExport();
	TExportResponse TransactionExport();
    TExportResponse CreateOutputFormatZed(std::map<UnicodeString, UnicodeString> DataRead, std::vector<UnicodeString> &DataToWrite,
                                          UnicodeString RIdentifier);
    TExportResponse CreateFilename();

    UnicodeString CheckDir(UnicodeString MallPath);
    UnicodeString getSharngrilaQuery();
    UnicodeString GetTime(UnicodeString DateTime);
    UnicodeString GetDate(UnicodeString DateTime);
    UnicodeString GetRegTime(UnicodeString DateTime);
    UnicodeString FixDate(UnicodeString DateTime);
    UnicodeString FixDecimalPlaces(Currency AmountTotal);
    UnicodeString GetHourlyFormat(UnicodeString TenantName, UnicodeString DateValue, UnicodeString TimeValue,
                                                        UnicodeString AmountValue, UnicodeString TaxValue, UnicodeString AdjValue, UnicodeString SChargeValue,
                                                        int TransactionTotal, UnicodeString SalesDept);

    void SaveIntVariable(vmVariables vmVar, int CompName);
    void SaveBoolVariable(vmVariables vmVar, bool CompName);
    void SaveStrVariable(vmVariables vmVar, UnicodeString CompName);
    void CheckPreviousEODFile();
    bool PerformedLastZed();

    std::fstream CheckPreviousFile;

    UnicodeString RIdentifier;

    TExportResponse CreateOutputFormatHourly(std::vector<UnicodeString> &DataToWrite, UnicodeString RIdentifier);

protected:
	// Override methods
    void initTimingExportResponse( TExportResponse& outResponse );

    // Delete this method if not needed
    TExportResponse timingExport();
};

#endif
