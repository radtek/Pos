//---------------------------------------------------------------------------

#ifndef MallExportCapitalandMallH
#define MallExportCapitalandMallH

#include "MallExportIMall.h"
#include "ManagerVariable.h"
//---------------------------------------------------------------------------

class TMallExportCapitalandMall : public TMallExportIMall
{
public:
    TMallExportCapitalandMall();
    virtual __fastcall ~TMallExportCapitalandMall();

	// Override methods
    AnsiString CreateFileURI( AnsiString inBaseURL );

    // Delete the methods below if not needed
	TExportResponse ZExport();
    TExportResponse CreateOutputFormatDaily(std::map<UnicodeString, UnicodeString> DataRead,
                                            std::vector<UnicodeString> &DataToWrite,
                                            UnicodeString DocType);
    UnicodeString getCapitalandQuery();
    UnicodeString GetDate(UnicodeString DateTime, UnicodeString DocType);
    UnicodeString CheckDir(UnicodeString Path);
    UnicodeString CreateYearlyFolder(UnicodeString Path);
    void CreateFileName(UnicodeString &LocalPath,UnicodeString &LocalPathFileName, UnicodeString &LocalFileName,
                        UnicodeString DocType, UnicodeString MachineID, int FileSerialNumber);
    void SaveIntVariable(vmVariables vmVar, int CompName);
    void FileSubmit(const char * hostName, const char * userName,
                    const char * userPassword, const char * userPath,
                    UnicodeString LocalPathFileName, UnicodeString LocalFileName,
                    int FCount);

    void GetReportsList(TStringList* reportsList, UnicodeString folder);
    void ResendReports(TStringList* resendReports, UnicodeString folder, bool status);
    bool CheckBackUpPath();

protected:
	// Override methods
    void initTimingExportResponse( TExportResponse& outResponse );
};

//---------------------------------------------------------------------------
#endif
