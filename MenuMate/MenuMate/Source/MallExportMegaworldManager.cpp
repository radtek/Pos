//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportMegaworldManager.h"
#include "MallExportMegaworldMall.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportMegaworldManager::TMallExportMegaworldManager() : TMallExportIMallManager()
{
    initMallExport();
}
//---------------------------------------------------------------------------
__fastcall TMallExportMegaworldManager::~TMallExportMegaworldManager()
{
    delete mallExport;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS
void TMallExportMegaworldManager::initMallExport()
{
    mallExport = new TMallExportMegaworldMall();

    //TODO: Base URL for Ayala
    mallExport->CreateFileURI(TGlobalSettings::Instance().MallPath);
}
//---------------------------------------------------------------------------
