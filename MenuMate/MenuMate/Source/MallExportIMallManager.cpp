//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportIMallManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------

TExportResponse TMallExportIMallManager::ZExport()
{
    return mallExport->ZExport();
}
//---------------------------------------------------------------------------
TExportResponse TMallExportIMallManager::TransactionExport()
{
    return mallExport->TransactionExport();
}
//---------------------------------------------------------------------------

void TMallExportIMallManager::SetExportTiming( TTimer* inTimer )
{
    return mallExport->SetExportTiming( inTimer );
}
//---------------------------------------------------------------------------

void TMallExportIMallManager::PauseExportTiming()
{
    return mallExport->PauseExportTiming();
}
//---------------------------------------------------------------------------

void TMallExportIMallManager::ResumeExportTiming()
{
    return mallExport->ResumeExportTiming();
}
//---------------------------------------------------------------------------

TExportResponse TMallExportIMallManager::GetLastTimingExportResponse()
{
    return mallExport->LastTimingExportResponse;
}
//---------------------------------------------------------------------------

void TMallExportIMallManager::SetTimingZedCheck( TTimer* inZedTimer, bool FirstMallSet )
{
    return mallExport->SetTimingZedCheck( inZedTimer, FirstMallSet );
}
//---------------------------------------------------------------------------
void TMallExportIMallManager::PauseTimingZedCheck()
{
    return mallExport->PauseTimingZedCheck();
}
//---------------------------------------------------------------------------
void TMallExportIMallManager::ResumeTimingZedCheck()
{
    return mallExport->ResumeTimingZedCheck();
}
//---------------------------------------------------------------------------
void TMallExportIMallManager::GetReportsList(TStringList* reportsList, UnicodeString folder)
{
    return mallExport->GetReportsList(reportsList, folder);
}
//---------------------------------------------------------------------------
void TMallExportIMallManager::ResendReports(TStringList* resendReports, UnicodeString folder, bool status)
{
    return mallExport->ResendReports(resendReports, folder, status);
}
//---------------------------------------------------------------------------
bool TMallExportIMallManager::CheckBackUpPath()
{
    return mallExport->CheckBackUpPath();
}
//---------------------------------------------------------------------------
bool TMallExportIMallManager::CheckTodaysZed()
{
    return mallExport->CheckTodaysZed();
}
//---------------------------------------------------------------------------
bool TMallExportIMallManager::ScheduledExitOnSelectDish()
{
    return mallExport->ScheduledExitOnSelectDish();
}
//---------------------------------------------------------------------------
