//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportFederalLandManager.h"
#include "MallExportFederalLandMall.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportFederalLandManager::TMallExportFederalLandManager() : TMallExportIMallManager()
{
    initMallExport();
}
//---------------------------------------------------------------------------
__fastcall TMallExportFederalLandManager::~TMallExportFederalLandManager()
{
    delete mallExport;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS
void TMallExportFederalLandManager::initMallExport()
{
    mallExport = new TMallExportFederalLandMall();

    //TODO: Base URL for Ayala
    mallExport->CreateFileURI(TGlobalSettings::Instance().MallPath);
}
//---------------------------------------------------------------------------
