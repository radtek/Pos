//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportPowerPlantManager.h"
#include "MallExportPowerPlantMall.h"
#include "GlobalSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportPowerPlantManager::TMallExportPowerPlantManager() : TMallExportIMallManager()
{
    initMallExport();
}
//---------------------------------------------------------------------------
__fastcall TMallExportPowerPlantManager::~TMallExportPowerPlantManager()
{
    delete mallExport;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS
void TMallExportPowerPlantManager::initMallExport()
{
    mallExport = new TMallExportPowerPlantMall();

    //TODO:
    mallExport->CreateFileURI(TGlobalSettings::Instance().MallPath);
}
//---------------------------------------------------------------------------