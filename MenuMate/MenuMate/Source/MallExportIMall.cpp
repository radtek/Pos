//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportIMall.h"
#include "MallExportInputFileDriver.h"
#include "MallExportOutputFileDriver.h"
#include "MallExportOutputDBDriver.h"

#include "Analysis.h"
#include "Main.h"
#include "SelectDish.h"
#include "MallExportSMMall.h"
#include "MallExportRobinsonMall.h"
#include "MallExportAyalaMall.h"
#include "MallExportPowerPlantMall.h"
#include "MallExportAlphalandMall.h"
#include "MallExportMegaworldMall.h"
#include "MallExportShangrilaMall.h"
#include "MallExportFederalLandMall.h"
#include "MallExportUpdateAdaptor.h"
#include "ShowPrintout.h"
#include <string>
#include <ios>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

// Abstract Menumate MallExport class

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportIMall::TMallExportIMall() : TObject()
{
    initTimingExportResponse( FTimingExportResponse );
    InputManager = new TMallExportInputFileDriver();
    if(TGlobalSettings::Instance().MallIndex == AYALAMALL ||
       TGlobalSettings::Instance().MallIndex == POWERPLANTMALL ||
       TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
    {
        OutputManager = new TMallExportOutputDBDriver();
    } else {
        OutputManager = new TMallExportOutputFileDriver();
    }

    InitialMallSet = false;
    FilePath = TGlobalSettings::Instance().MallPath;
    BasePath = ExtractFilePath(Application->ExeName) + MALLEXPORTFOLDERPATH;
}
//---------------------------------------------------------------------------
_fastcall TMallExportIMall::~TMallExportIMall()
{
}
//---------------------------------------------------------------------------

// Virtual methods
TExportResponse TMallExportIMall::ZExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    setExportResponse(
       false,                   // Succesful = false
       emrExportNotImplemented, // Result = emrExportNotImplemented
       "This class does not implement this mall export", // Message
       "",                                               // Description
       result );

    //::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
TExportResponse TMallExportIMall::TransactionExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    setExportResponse(
       false,                   // Succesful = false
       emrExportNotImplemented, // Result = emrExportNotImplemented
       "This class does not implement this mall export", // Message
       "",                                               // Description
       result );

    //::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
void TMallExportIMall::SetExportTiming( TTimer* inTimer )
{
    exportTimer = inTimer;
    exportTimer->OnTimer = exportOnTimer;
    ResumeExportTiming();
}
//---------------------------------------------------------------------------

void TMallExportIMall::PauseExportTiming()
{
    exportTimer = frmMain->tiMallExportTiming;
    exportTimer->Enabled = false;
}

//---------------------------------------------------------------------------

void TMallExportIMall::ResumeExportTiming()
{
    exportTimer = frmMain->tiMallExportTiming;
    exportTimer->Enabled = true;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS

void TMallExportIMall::initTimingExportResponse( TExportResponse& outResponse )
{
    setExportResponse(
       false,                   // Succesful = false
       emrExportNotImplemented, // Result = emrExportNotImplemented
       "This class does not implement this mall export", // Message
       "",                                               // Description
       outResponse );
}
//---------------------------------------------------------------------------
void TMallExportIMall::setExportResponse(
                       bool              inSuccesful,
                       TExportMallResult inResult,
                       AnsiString        inMessage,
                       AnsiString        inDescription,
                       TExportResponse&  outResponse )
{
    outResponse.Succesful   = inSuccesful;
    outResponse.Result      = inResult;
    outResponse.Message     = inMessage;
    outResponse.Description = inDescription;
}
//---------------------------------------------------------------------------

// Property Getters
AnsiString TMallExportIMall::GetFileURI()
{
    return FFileURI;
}
//---------------------------------------------------------------------------
TExportResponse TMallExportIMall::GetLastTimingExportResponse()
{
    return FTimingExportResponse;
}
//---------------------------------------------------------------------------

// Timing Mall Export
void __fastcall TMallExportIMall::exportOnTimer( TObject *Sender )
{
    CheckZFlag();
    GenerateHourlyExport();
}
//---------------------------------------------------------------------------

// Virtual methods
TExportResponse TMallExportIMall::timingExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    setExportResponse(
       false,                   // Succesful = false
       emrExportNotImplemented, // Result = emrExportNotImplemented
       "This class does not implement this mall export", // Message
       "",                                               // Description
       result );

    //::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

void TMallExportIMall::SetTimingZedCheck(TTimer* inZedTimer, bool FirstMallSet)
{
    InitialMallSet = FirstMallSet;
    zedcheckTimer = inZedTimer;
    zedcheckTimer->OnTimer = zedcheckOnTimer;
    ResumeTimingZedCheck();
}
//---------------------------------------------------------------------------
void TMallExportIMall::PauseTimingZedCheck()
{
    zedcheckTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void TMallExportIMall::ResumeTimingZedCheck()
{
    zedcheckTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMallExportIMall::zedcheckOnTimer( TObject *Sender )
{
    UnicodeString CheckDate = Now().FormatString("mmddyy");

    if(TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0)
    {
        InitialMallSet = false;
    }
    (!InitialMallSet) ? CheckPreviousEODFile() : SetZedTimer();
}
//---------------------------------------------------------------------------

void TMallExportIMall::CheckPreviousEODFile()
{
  // Virtual Method
}

//---------------------------------------------------------------------------

TExportResponse TMallExportIMall::CheckZFlag()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    setExportResponse(
       false,                   // Succesful = false
       emrExportNotImplemented, // Result = emrExportNotImplemented
       "This class does not implement this mall export", // Message
       "",                                               // Description
       result );

    //::::::::::::::::::::::::::::::::::::::

    return result;
}

//---------------------------------------------------------------------------TExportResponse TMallExportIMall::GenerateHourlyExport(){
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    setExportResponse(
       false,                   // Succesful = false
       emrExportNotImplemented, // Result = emrExportNotImplemented
       "This class does not implement this mall export", // Message
       "",                                               // Description
       result );

    //::::::::::::::::::::::::::::::::::::::

    return result;
}//---------------------------------------------------------------------------
TExportResponse TMallExportIMall::GetExportInfo(UnicodeString &FilePath, UnicodeString &TenantID,
                     UnicodeString &TerminalNo, UnicodeString &FileExt, UnicodeString &DateFormat)
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    setExportResponse(
       false,                   // Succesful = false
       emrExportNotImplemented, // Result = emrExportNotImplemented
       "This class does not implement this mall export", // Message
       "",                                               // Description
       result );

    //::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

void TMallExportIMall::SetZedTimer()
{
  // Virtual Method
}
//---------------------------------------------------------------------------

void TMallExportIMall::GetReportsList(TStringList* reportsList, UnicodeString folder)
{
  // Virtual Method
}
//---------------------------------------------------------------------------

void TMallExportIMall::ResendReports(TStringList* resendReports, UnicodeString folder, bool status)
{
  // Virtual Method
}
//---------------------------------------------------------------------------
bool TMallExportIMall::CheckBackUpPath()
{
  // Virtual Method
}
//---------------------------------------------------------------------------
bool TMallExportIMall::CheckTodaysZed()
{
  // Virtual Method
}
//---------------------------------------------------------------------------
bool TMallExportIMall::ScheduledExitOnSelectDish()
{
  // Virtual Method
}
//---------------------------------------------------------------------------
