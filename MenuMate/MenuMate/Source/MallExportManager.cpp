//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportManager.h"
#include "MallExportFactory.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TMallExportManager::TMallExportManager()
{
	IMallManager = TMallExportFactory::CreateManager();
}
//---------------------------------------------------------------------------
