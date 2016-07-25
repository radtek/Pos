//---------------------------------------------------------------------------

#ifndef MallExportRobinsonMallH
#define MallExportRobinsonMallH

#include "MallExportIMall.h"
#include "ManagerVariable.h"
#include <string>
#include <vector>
#include <fstream>
//---------------------------------------------------------------------------

class TMallExportRobinsonMall : public TMallExportIMall
{
public:
    TMallExportRobinsonMall();
    virtual __fastcall ~TMallExportRobinsonMall();

	// Override methods
    AnsiString CreateFileURI( AnsiString inBaseURL );

    // Delete the methods below if not needed
	TExportResponse ZExport();
	TExportResponse TransactionExport();
    TExportResponse GetExportInfo(UnicodeString &LocalPath, UnicodeString &TenantID,
                                  UnicodeString &TerminalNo, UnicodeString &FileExt,
                                  UnicodeString &DateFormat);
    TExportResponse CreateFilename(UnicodeString &FileName, UnicodeString &PrevName,
                                   UnicodeString LocalPath, UnicodeString &FailedPathFileName,
                                   UnicodeString &FailedPathPrevFileName);
    TExportResponse CreateOutputFormat(std::map<UnicodeString, UnicodeString> DataRead,
                                       std::vector<UnicodeString> &DataToWrite,
                                       UnicodeString PrevDate, bool PrevDateFlag);
    TExportResponse CopyToMallLocation(UnicodeString MallPathFileName, UnicodeString LocalPathFileName,
                                       UnicodeString MallPath, UnicodeString LocalPathPrevName,
                                       UnicodeString &FailedPath, UnicodeString FailedPathFileName,
                                       UnicodeString FailedPathPrevFileName);
    TExportResponse GenerateHourlyExport();

    UnicodeString CheckDir(UnicodeString LocalPath);
    UnicodeString getRobinsonQuery();
    UnicodeString CreateYearlyFolder(UnicodeString LocalPath);
    UnicodeString GetDate(UnicodeString DateTime);

    void SaveIntVariable(vmVariables vmVar, int CompName);
    void SaveStrVariable(vmVariables vmVar, UnicodeString CompName);
    void SaveBoolVariable(vmVariables vmVar, bool CompName);
    void CheckPreviousEODFile();
    void CopyProcedure(UnicodeString in, UnicodeString out);
    void SetZedTimer();
    void GenerateFileForDaysPOSNotUsed(UnicodeString LocalPath, UnicodeString FailedPath,
                                       UnicodeString TerminalNo, UnicodeString CheckDate,
                                       UnicodeString TenantID, UnicodeString FileExt,
                                       UnicodeString DateFormat);
    void UploadingUnsentFiles(UnicodeString FailedPath);
    void GetReportsList(TStringList* reportsList, UnicodeString folder);
    void ResendReports(TStringList* resendReports, UnicodeString folder, bool status);

    bool ExportUploadMessage(UnicodeString PathName, UnicodeString &FailedPath);
    bool CheckPreviousExportFile(UnicodeString LocalPath, UnicodeString TenantID, UnicodeString TerminalNo,
                                 UnicodeString FileExt, UnicodeString &DateFormat);
    bool CheckBackUpPath();
    bool CheckPreviousZed();
    bool CheckTodaysZed();
    bool ScheduledExitOnSelectDish();

    std::fstream CheckFile;
    std::fstream CheckPreviousFile;

protected:
	// Override methods
    void initTimingExportResponse( TExportResponse& outResponse );

    TExportResponse timingExport();
};

//---------------------------------------------------------------------------
#endif
