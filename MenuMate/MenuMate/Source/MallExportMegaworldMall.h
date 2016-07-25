//---------------------------------------------------------------------------

#ifndef MallExportMegaworldMallH
#define MallExportMegaworldMallH

#include "MallExportIMall.h"
#include "ManagerVariable.h"
#include <fstream>

//---------------------------------------------------------------------------

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
    TExportResponse HourlyDetailedExportOnZed();

    TExportResponse CreateFilename(UnicodeString mode, UnicodeString MallPath, UnicodeString LocalPath,
                                   UnicodeString &LocalPathFileName, UnicodeString &MallPathFileName);
    TExportResponse CreateOutputFormatZed(std::map<UnicodeString, UnicodeString> DataRead,
                                          std::vector<UnicodeString> &DataToWrite, UnicodeString MallPathFileName);
    TExportResponse CreateOutputFormatHourly(std::map<UnicodeString, UnicodeString> DataRead,
                                             std::vector<UnicodeString> &DataToWrite, UnicodeString MallPathFileName);
    TExportResponse CreateOutputFormatDiscount(std::map<UnicodeString, UnicodeString> DataRead,
                                              std::vector<UnicodeString> &DataToWrite, UnicodeString MallPathFileName);
    TExportResponse GetExportInfo(UnicodeString &MallPath, UnicodeString &TenantID,
                                  UnicodeString &TerminalNo, UnicodeString &BatchNo,
                                  UnicodeString &MonthValue, UnicodeString &DayValue,
                                  TDateTime DateValue);

    UnicodeString getMegaworldQuery();
    UnicodeString CheckDir(UnicodeString Path);
    UnicodeString RemoveDecimalValue(UnicodeString amountValue);
    UnicodeString FixDecimalPlaces(Currency AmountTotal);

    void SaveIntVariable(vmVariables vmVar, int CompName);
    void SaveStrVariable(vmVariables vmVar, UnicodeString CompName);
    void SetZedTimer();
    void CheckPreviousEODFile();

    int GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                      UnicodeString &DateValue, UnicodeString &TimeValue,
                      int &TransactionTotal, UnicodeString &Amount, int &PatronCount);

    TDateTime CheckLastZed();

    std::fstream CheckPreviousFile;

protected:
	// Override methods
    void initTimingExportResponse( TExportResponse& outResponse );

    // Delete this method if not needed
//    TExportResponse timingExport();
};

//---------------------------------------------------------------------------
#endif
