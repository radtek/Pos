//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportCaptialandManager.h"
#include "MallExportCapitalandMall.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportCapitalandManager::TMallExportCapitalandManager() : TMallExportIMallManager()
{
    initMallExport();
}
//---------------------------------------------------------------------------
__fastcall TMallExportCapitalandManager::~TMallExportCapitalandManager()
{
    delete mallExport;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS
void TMallExportCapitalandManager::initMallExport()
{
    mallExport = new TMallExportCapitalandMall();

    //TODO:
    mallExport->CreateFileURI(TGlobalSettings::Instance().MallPath);
}
//---------------------------------------------------------------------------