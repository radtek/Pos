//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportAyalaManager.h"
#include "MallExportAyalaMall.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportAyalaManager::TMallExportAyalaManager() : TMallExportIMallManager()
{
    initMallExport();
}
//---------------------------------------------------------------------------
__fastcall TMallExportAyalaManager::~TMallExportAyalaManager()
{
    delete mallExport;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS
void TMallExportAyalaManager::initMallExport()
{
    mallExport = new TMallExportAyalaMall();

    //TODO: Base URL for Ayala
    mallExport->CreateFileURI(TGlobalSettings::Instance().MallPath);
}
//---------------------------------------------------------------------------
