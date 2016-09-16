//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportOutputDBDriver.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <ios>
#include "MallExportDBFFile.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TMallExportOutputDBDriver::TMallExportOutputDBDriver()
{
}
//---------------------------------------------------------------------------

// This function is now in use since the Export file should be appending if the report
// is created within the same date.
void TMallExportOutputDBDriver::WriteToFile(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI)
{
    try
    {

        UnicodeString Minutes = Now().FormatString("nn");
        UnicodeString TempText = "";
        UnicodeString MallFile = "";

        if(TGlobalSettings::Instance().MallIndex == POWERPLANTMALL)
        {
            MallFile = ExtractFilePath(Application->ExeName) + "PowerPlant.txt";
        }

        if(TGlobalSettings::Instance().MallIndex == POWERPLANTMALL)
        {
            int loop = 0;

            TempFile.open(MallFile.c_str());

            if(TempFile.is_open())
            {
                TempText = ExtractFilePath(Application->ExeName) + "temp.txt";
                CopyOutputContent(MallFile, TempText);
                std::ifstream myfile(TempText.c_str());
                std::ofstream(MallFile.c_str(), std::ios_base::out).close();
                OutputFile.open(FileURI.c_str(), std::ios_base::app);

                loop = TGlobalSettings::Instance().PPlantCount - 1;
                int i = 0;
                while(std::getline(myfile,line))
                {
                    if(i<loop)
                    {
                        OutputFile << line;
                        OutputFile << "\n";
                    }
                    i++;
                }
                OutputFile.close();
                OutputFile.open(FileURI.c_str(), std::ios_base::app);
            }
            else
            {
                TGlobalSettings::Instance().PPlantCount = 1;
                SaveVariableInt(vmPPlantCount, TGlobalSettings::Instance().PPlantCount);
                OutputFile.open(FileURI.c_str(), std::ios_base::out);
            }
            TempFile.close();
        }
        else
        {
            OutputFile.open(FileURI.c_str(), std::ios_base::app);
        }

        for(int i=0;i<DataToWrite.size();i++)
        {
            OutputFile << DataToWrite.at(i).t_str();
        }

        if(TGlobalSettings::Instance().MallIndex != POWERPLANTMALL)
        {
            OutputFile << "\n";
        }
        OutputFile.close();

        if((TGlobalSettings::Instance().PPlantFlag == 1) &&
           (TGlobalSettings::Instance().MallIndex == POWERPLANTMALL))
        {
            CopyOutputContent(FileURI, MallFile);
        }
    }
    catch( Exception& exc )
    {
        //ignore the exception
    }
}

// This function is now in use since the Export file should be appending if the report
// is created within the same date.
void TMallExportOutputDBDriver::WriteToFile(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI, UnicodeString FileStatus)
{
    try
    {
        GenerateDBF(DataToWrite, FileURI, FileStatus);
        GenerateTXT(DataToWrite, FileURI, FileStatus);
    }
    catch( Exception& exc )
    {
        //ignore the exception
    }
}
//---------------------------------------------------------------------------
void TMallExportOutputDBDriver::CopyOutputContent(AnsiString FileURI, AnsiString TempText)
{
    try
    {
        std::ifstream FirstFile(FileURI.c_str());
        std::ofstream SecondFile(TempText.c_str());

        SecondFile << FirstFile.rdbuf();
    }
    catch( Exception& exc )
    {
        //ignore the exception
    }
}
//---------------------------------------------------------------------------

void TMallExportOutputDBDriver::SaveVariableInt(vmVariables vmVar, int CompName)
{
    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
        if(GlobalProfileKey == 0)
        {
            GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
        }
        TManagerVariable::Instance().SetProfileInt(DBTransaction, GlobalProfileKey, vmVar, CompName);
        DBTransaction.Commit();
    }
    catch( Exception& exc )
    {
        //ignore the exception
    }
}
//---------------------------------------------------------------------------
void TMallExportOutputDBDriver::GenerateDBF(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI, UnicodeString FileStatus)
{
    MallExportDBFFile newdbf;

    if(FileStatus == "ZHEADER")
    {
        TempFile.open(FileURI.c_str());
        if(!TempFile.is_open())
        {
            int nRet = newdbf.create(FileURI.t_str(),DataToWrite.size());
            if( nRet == 0 )
            {
                for(int i=0;i<DataToWrite.size();i++)
                {
                    fieldDefinition fd;
                    strncpy(fd.cFieldName,DataToWrite.at(i).t_str(),21);
                    fd.FieldFlags=0;
                    fd.uAutoIncrementStep=0;
                    fd.uFieldOffset=0;

                    switch(i)
                    {
                        case 0:
                            fd.cFieldType='D';
                            fd.uLength=8;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        case 1:
                            fd.cFieldType='N';
                            fd.uLength=15;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 2:
                            fd.cFieldType='N';
                            fd.uLength=15;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 3:
                            fd.cFieldType='N';
                            fd.uLength=11;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 4:
                            fd.cFieldType='N';
                            fd.uLength=11;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 5:
                            fd.cFieldType='N';
                            fd.uLength=11;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 6:
                            fd.cFieldType='N';
                            fd.uLength=11;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 7:
                            fd.cFieldType='N';
                            fd.uLength=11;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 8:
                            fd.cFieldType='C';
                            fd.uLength=15;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        case 9:
                            fd.cFieldType='N';
                            fd.uLength=15;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        case 10:
                            fd.cFieldType='N';
                            fd.uLength=15;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        case 11:
                            fd.cFieldType='N';
                            fd.uLength=15;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        case 12:
                            fd.cFieldType='N';
                            fd.uLength=15;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        case 13:
                            fd.cFieldType='N';
                            fd.uLength=9;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        case 14:
                            fd.cFieldType='N';
                            fd.uLength=11;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 15:
                            fd.cFieldType='N';
                            fd.uLength=11;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 16:
                            fd.cFieldType='N';
                            fd.uLength=11;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 17:
                            fd.cFieldType='N';
                            fd.uLength=11;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 18:
                            fd.cFieldType='N';
                            fd.uLength=11;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 19:
                            fd.cFieldType='N';
                            fd.uLength=11;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        case 20:
                            fd.cFieldType='N';
                            fd.uLength=3;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        default:
                            fd.cFieldType='N';
                            fd.uLength=20;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                   }
                   newdbf.assignField(fd,i);
                }
            }
            newdbf.close();
        }
        TempFile.close();
    }
    else if(FileStatus == "ZDATA")
    {
        int nRet = newdbf.open(FileURI.t_str(),true);
        if( nRet == 0 )
        {
            string b[21];
            for(int i=0;i<DataToWrite.size();i++)
            {
                b[i] = DataToWrite.at(i).t_str();
            }
            string s1[21] = {b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],b[8],b[9],b[10],
                             b[11],b[12],b[13],b[14],b[15],b[16],b[17],b[18],b[19],b[20]};
            newdbf.appendRecord(s1,21);

        }
        newdbf.close();
    }
    else if(FileStatus == "HHEADER")
    {
        TempFile.open(FileURI.c_str());
        if(!TempFile.is_open())
        {
            int nRet = newdbf.create(FileURI.t_str(),DataToWrite.size());
            if( nRet == 0 )
            {
                for(int i=0;i<DataToWrite.size();i++)
                {
                    fieldDefinition fd;
                    strncpy(fd.cFieldName,DataToWrite.at(i).t_str(),21);
                    fd.FieldFlags=0;
                    fd.uAutoIncrementStep=0;
                    fd.uFieldOffset=0;

                    switch(i)
                    {
                        case 0:
                            fd.cFieldType='D';
                            fd.uLength=8;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        case 1:
                            fd.cFieldType='C';
                            fd.uLength=5;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        case 2:
                            fd.cFieldType='N';
                            fd.uLength=11;
                            fd.uNumberOfDecimalPlaces=2;
                            break;
                        case 3:
                            fd.cFieldType='N';
                            fd.uLength=9;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        case 4:
                            fd.cFieldType='C';
                            fd.uLength=15;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        case 5:
                            fd.cFieldType='N';
                            fd.uLength=3;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                        default:
                            fd.cFieldType='N';
                            fd.uLength=20;
                            fd.uNumberOfDecimalPlaces=0;
                            break;
                   }
                    newdbf.assignField(fd,i);
                }
                newdbf.close();
            }
        }
        TempFile.close();
    }
    else if(FileStatus == "HDATA")
    {
        int nRet = newdbf.open(FileURI.t_str(),true);

        if( nRet == 0 )
        {
            string b[6];
            for(int i=0;i<DataToWrite.size();i++)
            {
                b[i] = DataToWrite.at(i).t_str();
            }
            string s1[6] = {b[0],b[1],b[2],b[3],b[4],b[5]};
            newdbf.appendRecord(s1,6);
        }
        newdbf.close();
    }
}
//---------------------------------------------------------------------------

void TMallExportOutputDBDriver::GenerateTXT(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI, UnicodeString FileStatus)
{
    std::ofstream OutFile;

    if(FileStatus == "ZTXTHEADER" || FileStatus == "HTXTHEADER")
    {
        TempFile.open(FileURI.c_str());
        if(!TempFile.is_open())
        {
            OutFile.open(FileURI.c_str(), std::ios_base::out);
            for(int i=0;i<DataToWrite.size();i++)
            {
                OutFile << DataToWrite.at(i).t_str();
            }
        }
        OutFile.close();
        TempFile.close();
    }
    else if(FileStatus == "ZTXTDATA" || FileStatus == "HTXTDATA")
    {
        TempFile.open(FileURI.c_str());
        if(TempFile.is_open())
        {
            OutFile.open(FileURI.c_str(), std::ios_base::app);
            if(OutFile.is_open())
            {
                for(int i=0;i<DataToWrite.size();i++)
                {
                    OutFile << DataToWrite.at(i).t_str();
                }
            }
            OutFile.close();
        }
        TempFile.close();
    }
}
//---------------------------------------------------------------------------
