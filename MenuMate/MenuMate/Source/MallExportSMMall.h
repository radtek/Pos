//---------------------------------------------------------------------------

#ifndef MallExportSMMallH
#define MallExportSMMallH

#include "MallExportIMall.h"
#include "ManagerVariable.h"
#include <vector>
#include <fstream>
//---------------------------------------------------------------------------

class TMallExportSMMall : public TMallExportIMall
{
public:
    TMallExportSMMall();
    virtual __fastcall ~TMallExportSMMall();

	// Override methods
    AnsiString CreateFileURI( AnsiString inBaseURL );

    // Delete the methods below if not needed
	TExportResponse ZExport();
	TExportResponse TransactionExport();
    TExportResponse CheckZFlag();
    TExportResponse CreateOutputFormat(std::map<UnicodeString, UnicodeString> DataRead,
                                       std::vector<UnicodeString> &DataToWrite);

    UnicodeString CheckDir(UnicodeString FilePath);
    void CreateFilename();
    UnicodeString getSMQuery();
    void CreateOutputFlag();
    void SaveZCount(vmVariables vmVar, int CompName);
    void CreateLocalDir();
    UnicodeString GetTime(UnicodeString DateTime);
    UnicodeString GetDate(UnicodeString DateTime);
    UnicodeString CheckDir();

    std::fstream OutputFlag;
    std::fstream CheckFlag;

protected:
	// Override methods
    void initTimingExportResponse( TExportResponse& outResponse );

    // Delete this method if not needed
    TExportResponse timingExport();
};

//---------------------------------------------------------------------------
#endif
