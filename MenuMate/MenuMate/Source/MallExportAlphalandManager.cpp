//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportAlphalandManager.h"
#include "MallExportAlphalandMall.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportAlphalandManager::TMallExportAlphalandManager() : TMallExportIMallManager()
{
    initMallExport();
}
//---------------------------------------------------------------------------
__fastcall TMallExportAlphalandManager::~TMallExportAlphalandManager()
{
    delete mallExport;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS
void TMallExportAlphalandManager::initMallExport()
{
    mallExport = new TMallExportAlphalandMall();

    //TODO: Base URL for Ayala
    mallExport->CreateFileURI(TGlobalSettings::Instance().MallPath);
}
//---------------------------------------------------------------------------