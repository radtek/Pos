//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportRobinsonManager.h"
#include "MallExportRobinsonMall.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportRobinsonManager::TMallExportRobinsonManager() : TMallExportIMallManager()
{
    initMallExport();
}
//---------------------------------------------------------------------------
__fastcall TMallExportRobinsonManager::~TMallExportRobinsonManager()
{
    delete mallExport;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS
void TMallExportRobinsonManager::initMallExport()
{
    mallExport = new TMallExportRobinsonMall();

    //TODO: Base URL for Ayala
    mallExport->CreateFileURI(TGlobalSettings::Instance().MallPath);
}
//---------------------------------------------------------------------------