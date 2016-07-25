//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportDataManager.h"
#include <vector>
#include <map>
//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall TMallExportDataManager::TMallExportDataManager()
{
}
//---------------------------------------------------------------------------

void TMallExportDataManager::ExportFile(TMallExportIOutput* IOutput, std::vector<UnicodeString> DataToWrite, UnicodeString FileURI)
{
    IOutput->WriteToFile(DataToWrite,FileURI);
}
//---------------------------------------------------------------------------

void TMallExportDataManager::ExportFile(TMallExportIOutput* IOutput, std::vector<UnicodeString> DataToWrite, UnicodeString FileURI, UnicodeString FileStatus)
{
    IOutput->WriteToFile(DataToWrite,FileURI,FileStatus);
}
//---------------------------------------------------------------------------
