//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportSMManager.h"
#include "MallExportSMMall.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportSMManager::TMallExportSMManager() : TMallExportIMallManager()
{
    initMallExport();
}
//---------------------------------------------------------------------------
__fastcall TMallExportSMManager::~TMallExportSMManager()
{
    delete mallExport;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS
void TMallExportSMManager::initMallExport()
{
    mallExport = new TMallExportSMMall();

    //TODO: Base URL for SM
    mallExport->CreateFileURI(TGlobalSettings::Instance().MallPath);
}
//---------------------------------------------------------------------------