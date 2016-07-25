//---------------------------------------------------------------------------

#ifndef MallExportAyalaMallH
#define MallExportAyalaMallH

#include "MallExportIMall.h"
#include "ManagerVariable.h"
#include <vector>
#include <fstream>

#include "ReportUtilities.h"
#include "ReportFinancialCalculations.h"

//---------------------------------------------------------------------------

class TMallExportAyalaMall : public TMallExportIMall
{
public:
    TMallExportAyalaMall();
    virtual __fastcall ~TMallExportAyalaMall();

	// Override methods
    AnsiString CreateFileURI( AnsiString inBaseURL );

    // Delete the methods below if not needed
	TExportResponse ZExport();
	TExportResponse TransactionExport();
    TExportResponse GetExportInfo(UnicodeString &LocalPath, UnicodeString &TenantID,
                                  UnicodeString &TerminalNo, UnicodeString &FileExt,
                                  UnicodeString &DateFormat);
    TExportResponse CreateFilename(UnicodeString mode, UnicodeString MallPath, UnicodeString LocalPath,
                                   UnicodeString &LocalPathFileName, UnicodeString &MallPathFileName,
                                   UnicodeString FileExt);
    TExportResponse CreateOutputFormatZed(std::map<UnicodeString, UnicodeString> DataRead,
                            std::vector<UnicodeString> &DataToWrite, UnicodeString MallPathFileName,
                            UnicodeString PrevDate, UnicodeString PrevDateYesterday, bool PrevDateFlag,
                            UnicodeString FileFormat);
    TExportResponse CreateOutputFormatHourly(std::map<UnicodeString, UnicodeString> DataRead,
                            std::vector<UnicodeString> &DataToWrite, UnicodeString MallPathFileName,
                            UnicodeString PrevDate, UnicodeString PrevDateNext, UnicodeString PrevDateYesterday,
                            bool PrevDateFlag, UnicodeString FileFormat);
    TExportResponse CopyToBackUpPath(UnicodeString LocalPath, UnicodeString LocalPathFileName,
                                     UnicodeString MallPath, UnicodeString MallPathFileName);
    TExportResponse HourlyDetailedExportOnZed();

    UnicodeString CheckDir(UnicodeString Path);
    UnicodeString getAyalaQuery(UnicodeString mode);
    UnicodeString CreateYearlyFolder(UnicodeString Path, UnicodeString Year);
    UnicodeString GetTime(UnicodeString DateTime);
    UnicodeString GetDate(UnicodeString DateTime);
    UnicodeString getDateFormat(bool ZReportStatus, UnicodeString DateFormat);
    UnicodeString GetHourlyFormat(UnicodeString TerminalName, UnicodeString TenantName,
                                  UnicodeString DateValue, UnicodeString TimeValue);
    UnicodeString GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                                UnicodeString &DateValue, UnicodeString &TimeValue,
                                int &TransactionTotal, UnicodeString &Amount);
    UnicodeString FixDecimalPlaces(Currency AmountTotal);

    void SaveIntVariable(vmVariables vmVar, int CompName);
    void SaveBoolVariable(vmVariables vmVar, bool CompName);
    void SaveStrVariable(vmVariables vmVar, UnicodeString CompName);
    void SetZedTimer();
    void CheckPreviousEODFile();
    void CopyProcedure(UnicodeString in, UnicodeString out);
    void GetReportsList(TStringList* reportsList, UnicodeString folder);
    void ResendReports(TStringList* resendReports, UnicodeString folder, bool status);
    void CreateHeaderFormatHourly(UnicodeString MallPathFileName, std::vector<UnicodeString> &DataToWrite,
                                  UnicodeString FileFormat);
    void CreateHeaderFormatZed(UnicodeString MallPathFileName, std::vector<UnicodeString> &DataToWrite,
                               UnicodeString FileFormat);
    void GenerateFileForDaysPOSNotUsed(UnicodeString LocalPath, UnicodeString MallPath,
                                       UnicodeString TerminalNo, UnicodeString CheckDate,
                                       UnicodeString TenantID, UnicodeString FileExt,
                                       UnicodeString DateFormat);

    bool CheckPreviousExportFile(UnicodeString MallPath, UnicodeString TenantID,
                                 UnicodeString FileExt, UnicodeString &DateFormat, UnicodeString mode);
    bool CheckBackUpPath();
    bool CheckPreviousZed();
    bool CheckPreviousZedDateTime();

    std::fstream CheckPreviousFile;
    std::fstream OutputFile;
    std::fstream CheckFile;
    std::ofstream CreateColumn;
    std::fstream CheckCurrentFile;

    ReportFinancialCalculations* reportFinancialCalculations;

protected:
	// Override methods
    void initTimingExportResponse( TExportResponse& outResponse );

    // Delete this method if not needed
//    TExportResponse timingExport();
};

//---------------------------------------------------------------------------
#endif


