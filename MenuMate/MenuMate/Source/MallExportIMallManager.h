//---------------------------------------------------------------------------

#ifndef MallExportIMallManagerH
#define MallExportIMallManagerH

#include "MallExportIMall.h"
//---------------------------------------------------------------------------

class TMallExportIMallManager : public TObject
{
public:
    virtual TExportResponse ZExport();
    virtual TExportResponse TransactionExport();

    virtual void SetExportTiming( TTimer* inTimer );
    virtual void PauseExportTiming();
    virtual void ResumeExportTiming();

    virtual TExportResponse GetLastTimingExportResponse();

    // Added
    virtual void SetTimingZedCheck( TTimer* inZedTimer, bool FistMallSet );
    virtual void PauseTimingZedCheck();
    virtual void ResumeTimingZedCheck();
    virtual void GetReportsList(TStringList* reportsList, UnicodeString folder);
    virtual void ResendReports(TStringList* resendReports, UnicodeString folder, bool status);
    virtual bool CheckBackUpPath();
    virtual bool CheckTodaysZed();
    virtual bool ScheduledExitOnSelectDish();

protected:
    TMallExportIMall *mallExport;

    virtual void initMallExport() = 0;
};
#endif
