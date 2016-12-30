//---------------------------------------------------------------------------


#pragma hdrstop
#include "MallExportTextFile.h"
#include <stdio.h>
#include <ios>
#include <fstream>
#include "GlobalSettings.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TMallExportTextFile::TMallExportTextFile()
{
}
//----------------------------------------------------------------------------------------------
void TMallExportTextFile::WriteToFile(TMallExportPrepareData preparedData)
{
    for( int index = 1; index <= preparedData.MallSettings.size(); index++)
    {
        //Iterator for iterating settings
        std::map<int,list<TMallExportSettings> >::iterator itSettings = preparedData.MallSettings.find(index);

        //Iterator for iterating salesData
        std::map<int,list<TMallExportSalesData> >::iterator itSalesData = preparedData.SalesData.find(index);

        //iterater for iterating file name
        std::map<int,UnicodeString>::iterator itFileName = preparedData.FileName.find(index);

        //Write into File According to File Type
        WriteFileAccordingToIndex(preparedData, itSettings, itSalesData, itFileName, index);
    }
}
//------------------------------------------------------------------------------------------------------
void TMallExportTextFile::WriteFileAccordingToIndex(TMallExportPrepareData preparedData, std::map<int,list<TMallExportSettings> >::iterator itSettings,
                            std::map<int,list<TMallExportSalesData> >::iterator itSalesData, std::map<int,UnicodeString>::iterator itFileName, int index)
{
    try
    {
        //print new column in new line or not
        UnicodeString appendNewLine = "false";

        //Creating Temporary File Stream
        std::fstream tempFile;

        //Creating output File Stream
        std::ofstream outFile;

        //filePath where the file will be written
        UnicodeString filePath = "";

        //Format of the file
        UnicodeString format = ".txt";

        //Iterate the file Settings
        std::list<TMallExportSettings>::iterator itFileSettings;

        //Iterate mallExportSalesData.
        std::list<TMallExportSalesData>::iterator it;

        //Iterate FileNaming Convention
        std::map<int,UnicodeString>::iterator itName;

        //Iterate UI Settings of FilPath
        std::list<TMallExportSettings> ::iterator itUISettings;

        //Get File Path
        for(itUISettings = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); itUISettings != TGlobalSettings::Instance().mallInfo.MallSettings.end(); itUISettings++)
        {
            if(itUISettings->ControlName == "edNewMallPath")
                filePath = itUISettings->Value;
            else if(itUISettings->ControlName == "New Line")
                appendNewLine = itUISettings->Value;
        }

        //Check For Directory Existence ..Create if not created already
        if (!DirectoryExists(filePath))
        {
             CreateDir(filePath);
        }

        //Getting filepath + fileName
        filePath = filePath + itFileName->second  + format;

        //Open the file.. file will be opened if already Created
        tempFile.open(filePath.c_str());

        //If writing first time in the file than tempFile.is_open() will return false so control will go inside if
         if(!tempFile.is_open())
         {
            //Open file for writing
            outFile.open(filePath.c_str(), std::ios_base::out);

            for(itUISettings = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); itUISettings != TGlobalSettings::Instance().mallInfo.MallSettings.end(); itUISettings++)
            {
                //Write file Headers
                if(itUISettings->ControlName == "File Header")
                {
                    outFile << itUISettings->Value.t_str();
                    if(appendNewLine == "true")
                        outFile << "\n";
                }
            }

            //First Write Settings in the file ex:- Tenant Code, TerminalNumber etc
            for(itFileSettings = itSettings->second.begin(); itFileSettings != itSettings->second.end(); itFileSettings++)
            {
                //Convert Value into String
                outFile << itFileSettings->Value.t_str();

                //Append new line if setting is on
                if(appendNewLine == "true")
                    outFile << "\n";
            }

            //Secondly Write data in the file
            for(it = itSalesData->second.begin(); it != itSalesData->second.end(); it++)
            {
                //Convert Value into String
                outFile << it->DataValue.t_str();

                //Append new line if setting is on
                if(appendNewLine == "true")
                    outFile << "\n";
            }
        }
         //As we have written to the file so close it now.
        outFile.close();
        tempFile.close();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
