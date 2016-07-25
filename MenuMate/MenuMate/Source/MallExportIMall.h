//---------------------------------------------------------------------------

#ifndef MallExportIMallH
#define MallExportIMallH

#include <Classes.hpp>
#include <vcl.h>
#include "MallExportIInput.h"
#include "MallExportIOutput.h"
#include <vector>
#include <map>
#include <fstream>
#include "MallExportDefines.h"
//---------------------------------------------------------------------------

enum TExportMallResult
{
    emrExportNotImplemented,
    emrExportNotDoneYet,
    emrExportSuccesful,
    emrExportFailed,
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

struct TExportResponse
{
    TDateTime  Time;
    bool       Succesful;
    __int32    Result;
    AnsiString Message;
    AnsiString Description;
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

struct dirent* entry;

// Abstract Menumate MallExport class
class TMallExportIMall : public TObject
{
protected:
    // Property Getters
    AnsiString      GetFileURI();
    TExportResponse GetLastTimingExportResponse();

public:
    TMallExportIMall();
    virtual __fastcall ~TMallExportIMall();

	// Abstract methods
    virtual AnsiString CreateFileURI( AnsiString inBaseURL ) = 0;

	// Virtual methods
	virtual TExportResponse ZExport();
	virtual TExportResponse TransactionExport();

    virtual TExportResponse CheckZFlag();
    virtual TExportResponse GenerateHourlyExport();
    virtual TExportResponse GetExportInfo(UnicodeString &FilePath, UnicodeString &TenantID,
                                          UnicodeString &TerminalNo, UnicodeString &FileExt,
                                          UnicodeString &DateFormat);
    virtual void SetZedTimer();
    virtual void CheckPreviousEODFile();
    virtual void GetReportsList(TStringList* reportsList, UnicodeString folder);
    virtual void ResendReports(TStringList* resendReports, UnicodeString folder, bool status);
    virtual bool CheckBackUpPath();
    virtual bool CheckTodaysZed();
    virtual bool ScheduledExitOnSelectDish();

	void SetExportTiming( TTimer* inTimer );
    void PauseExportTiming();
    void ResumeExportTiming();
	void SetTimingZedCheck( TTimer* inZedTimer, bool FirstMallSet );
    void ResumeTimingZedCheck();
    void PauseTimingZedCheck();

    __property AnsiString      FileURI = { read=GetFileURI };
    __property TExportResponse LastTimingExportResponse = { read=GetLastTimingExportResponse };

    TMallExportIInput* InputManager;
    TMallExportIOutput* OutputManager;

    UnicodeString FilePath;
    UnicodeString DataQuery;
    UnicodeString BasePath;
    std::vector<UnicodeString> DataToWrite;
    std::map<UnicodeString, UnicodeString> DataRead;
    bool InitialMallSet;
    int ZCounter;

protected:
    AnsiString      FFileURI;
    TExportResponse FTimingExportResponse;
    TTimer*  exportTimer;
    TTimer*  zedcheckTimer;

    virtual void initTimingExportResponse( TExportResponse& outResponse );
    void setExportResponse(
           bool              inSuccesful,
           TExportMallResult inResult,
           AnsiString        inMessage,
           AnsiString        inDescription,
           TExportResponse&  outResponse );

    void __fastcall exportOnTimer( TObject *Sender );
    void __fastcall zedcheckOnTimer( TObject *Sender );

	// Virtual methods
    virtual TExportResponse timingExport();
};
#endif
