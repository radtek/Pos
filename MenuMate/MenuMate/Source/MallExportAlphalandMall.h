//---------------------------------------------------------------------------

#ifndef MallExportAlphalandMallH
#define MallExportAlphalandMallH

#include "MallExportIMall.h"
#include "ManagerVariable.h"
#include <vector>
#include <fstream>

#include "ReportUtilities.h"
#include "ReportFinancialCalculations.h"

//---------------------------------------------------------------------------

class TMallExportAlphalandMall : public TMallExportIMall
{
public:
    TMallExportAlphalandMall();
    virtual __fastcall ~TMallExportAlphalandMall();

	// Override methods
    AnsiString CreateFileURI( AnsiString inBaseURL );

    // Delete the methods below if not needed
	TExportResponse ZExport();
	TExportResponse TransactionExport();

    TExportResponse CreateFilename(UnicodeString mode, UnicodeString MallPath, UnicodeString LocalPath,
                                   UnicodeString &LocalPathFileName, UnicodeString &MallPathFileName);
    TExportResponse CreateOutputFormat(std::map<UnicodeString, UnicodeString> DataRead,
                                       std::vector<UnicodeString> &DataToWrite);
    TExportResponse CreateOutputFormatTransaction(std::map<UnicodeString, UnicodeString> DataRead,
                            std::vector<UnicodeString> &DataToWrite);

    UnicodeString CheckDir(UnicodeString Path);
    UnicodeString CreateYearlyFolder(UnicodeString Path, UnicodeString Year);
    UnicodeString AddZeroesToValue(int ZeroCount, int Value);
    UnicodeString CheckValue(Currency Amount);
    UnicodeString getAlphalandQuery();

    void SaveZCount(vmVariables vmVar, int CompName);

protected:
	// Override methods
    void initTimingExportResponse( TExportResponse& outResponse );
};
#endif
