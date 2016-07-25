//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportOutputFileDriver.h"
#include <vector>
#include <ios>
#include <map>
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TMallExportOutputFileDriver::TMallExportOutputFileDriver()
{
}
//---------------------------------------------------------------------------

void TMallExportOutputFileDriver::WriteToFile(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI)
{
    OutputFile.open(FileURI.c_str(), std::ios_base::out);

    for(int i=0;i<DataToWrite.size();i++)
    {
        OutputFile << DataToWrite.at(i).t_str();
    }
    OutputFile.close();
}
//---------------------------------------------------------------------------
void TMallExportOutputFileDriver::WriteToFile(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI, UnicodeString FileStatus)
{
// Empty declaration
}
