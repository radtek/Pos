//---------------------------------------------------------------------------


#pragma hdrstop
#include "MallExportTextFile.h"
#include <stdio.h>
#include <ios>
#include <fstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)

TMallExportTextFile::TMallExportTextFile()
{
}
//----------------------------------------------------------------------------------------------
void TMallExportTextFile::WriteToFile(TMallExportPrepareData preparedData)
{
    for( int index=1; index< preparedData.MallSettings.size(); index++)
    {
        std::map<int,list<TMallExportSettings> > setting = preparedData.MallSettings;
        //std::list<TMallExportSettings> settings = preparedData.MallSettings.find(index);

        std::map<int,list<TMallExportSettings> >::iterator itSettings = preparedData.MallSettings.find(2);
        std::map<int,list<TMallExportSalesData> >::iterator itSalesData = preparedData.SalesData.find(2);

        WriteFileAccordingToIndex(preparedData, itSettings, itSalesData, 2);
    }
}
//------------------------------------------------------------------------------------------------------
void TMallExportTextFile::WriteFileAccordingToIndex(TMallExportPrepareData preparedData, std::map<int,list<TMallExportSettings> >::iterator itSettings,
                                                        std::map<int,list<TMallExportSalesData> >::iterator itSalesData, int index)
{
        std::list<TMallExportSettings>::iterator itFileSettings;
        std::list<TMallExportSalesData>::iterator it;

        std::fstream OutputFile;
        std::fstream TempFile;

            std::ofstream OutFile;
            UnicodeString FileURI = "C:\\Estancia\\EstanciaText.txt";
             TempFile.open(FileURI.c_str());

             if(TempFile.is_open())
             {
                    OutFile.open(FileURI.c_str(), std::ios_base::out);
                    for(itFileSettings = itSettings->second.begin(); itFileSettings != itSettings->second.end(); itFileSettings++)
                    {
                       // OutFile << itFileSettings->Value.t_str();
                        OutFile << itFileSettings->Value.t_str();
                        OutFile << "\n";
                    }
                    for(it = itSalesData->second.begin(); it != itSalesData->second.end(); it++)
                    {
                      //  OutFile << it->FieldIndex.t_str();
                        OutFile << it->DataValue.t_str();
                        OutFile << "\n";
                    }
             }
            OutFile.close();
            TempFile.close();
          //  }

//    switch(index)
//    {
//        case 1:
//            break;
//
//    }


}
