//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportShangrilaManager.h"
#include "MallExportShangrilaMall.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportShangrilaManager::TMallExportShangrilaManager() : TMallExportIMallManager()
{
    initMallExport();
}
//---------------------------------------------------------------------------
__fastcall TMallExportShangrilaManager::~TMallExportShangrilaManager()
{
    delete mallExport;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS
void TMallExportShangrilaManager::initMallExport()
{
    mallExport = new TMallExportShangrilaMall();

    //TODO:
    mallExport->CreateFileURI(TGlobalSettings::Instance().MallPath);
}
//---------------------------------------------------------------------------