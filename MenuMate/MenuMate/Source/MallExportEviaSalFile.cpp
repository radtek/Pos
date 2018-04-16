//---------------------------------------------------------------------------


#pragma hdrstop
#include "MallExportEviaSalFile.h"
#include <stdio.h>
#include <ios>
#include <fstream>
#include "GlobalSettings.h"
#include "MMLogging.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TMallExportSalFile::TMallExportSalFile()
{
}
//----------------------------------------------------------------------------------------------
void TMallExportSalFile::WriteToFile(TMallExportPrepareData preparedData)
{

        int index = 1;
        std::map<int,list<TMallExportSalesData> >::iterator itSalesData = preparedData.SalesData.find(index);

        std::map<int,UnicodeString>::iterator itFileName = preparedData.FileName.find(index);

        WriteFileAccordingToIndex(preparedData, itSalesData, itFileName, index);

}
//------------------------------------------------------------------------------------------------------
void TMallExportSalFile::WriteFileAccordingToIndex(TMallExportPrepareData preparedData,
                            std::map<int,list<TMallExportSalesData> >::iterator itSalesData, std::map<int,UnicodeString>::iterator itFileName, int index)
{
    try
    {

        UnicodeString appendNewLine = "false";
        UnicodeString value = "" ;
        UnicodeString filePath = "";

        UnicodeString format = ".sal";

        std::list<TMallExportSalesData>::iterator it;


        std::list<TMallExportSettings> ::iterator itUISettings;

        for(itUISettings = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); itUISettings != TGlobalSettings::Instance().mallInfo.MallSettings.end(); itUISettings++)
        {
            if(itUISettings->ControlName == "edNewMallPath")
                filePath = itUISettings->Value;

        }

        if (!DirectoryExists(filePath))
        {
             CreateDir(filePath);
        }

        filePath = filePath + itFileName->second  + format;
        MessageBox(filePath,"filePath in writetofile",MB_OK) ;


        std::auto_ptr<TStringList> List(new TStringList);

        if (FileExists(filePath ))
        {

            List->LoadFromFile(filePath );

        }


        for(it = itSalesData->second.begin(); it != itSalesData->second.end(); it++)
        {
            value += it->DataValue ;
        }
        List->Add(value);
        List->SaveToFile(filePath );
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
