//---------------------------------------------------------------------------

#ifndef MallExportPowerPlantMallH
#define MallExportPowerPlantMallH
//---------------------------------------------------------------------------

#include "MallExportIMall.h"
#include "ManagerVariable.h"
#include <string>
#include <vector>
#include <fstream>
//---------------------------------------------------------------------------

class TMallExportPowerPlantMall : public TMallExportIMall
{
public:
    TMallExportPowerPlantMall();
    virtual __fastcall ~TMallExportPowerPlantMall();

	// Override methods
    AnsiString CreateFileURI( AnsiString inBaseURL );

    // Delete the methods below if not needed
	TExportResponse ZExport();
	TExportResponse TransactionExport();

    TExportResponse CreateOutputFormatZed(std::map<UnicodeString, UnicodeString> DataRead, std::vector<UnicodeString> &DataToWrite,
                                          AnsiString FileURI, UnicodeString RIdentifier);
    TExportResponse CreateOutputFormatGeneric(std::map<UnicodeString, UnicodeString> DataRead, std::vector<UnicodeString> &DataToWrite,
                                              UnicodeString mode, AnsiString FileURI, UnicodeString RIdentifier);
    TExportResponse CreateFilename(UnicodeString mode, AnsiString &prevFName);
    TExportResponse CopyOutputContent(AnsiString FileURI, AnsiString prevPath);
    TExportResponse GenerateHourlyExport();
    TExportResponse GetExportInfo(UnicodeString &MallPath, UnicodeString &TenantID,
                                  UnicodeString &TerminalNo, UnicodeString &FileExt,
                                  UnicodeString &DateFormat);

    UnicodeString GetTime(UnicodeString DateTime, UnicodeString mode);
    UnicodeString GetDate(UnicodeString DateTime);
    UnicodeString CheckDir(UnicodeString MallPath);
    UnicodeString getPowerPlantQuery(UnicodeString mode);

    Currency CalculateZed(int number, AnsiString TempPath);

    bool CheckPreviousExportFile(UnicodeString MallPath, UnicodeString FileExt, UnicodeString &DateFormat);
    bool CheckPreviousZed();

    void CheckPreviousEODFile();
    void SetZedTimer();
    void SaveIntVariable(vmVariables vmVar, int CompName);
    void SaveBoolVariable(vmVariables vmVar, bool CompName);

    std::fstream CheckFile;
    std::fstream CheckPreviousFile;

    UnicodeString RIdentifier;
    AnsiString prevFName;

    std::fstream TempFile;
    std::string line;

protected:
	// Override methods
    void initTimingExportResponse( TExportResponse& outResponse );

    // Delete this method if not needed
    TExportResponse timingExport();
};
#endif
