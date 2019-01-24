//---------------------------------------------------------------------------


#pragma hdrstop

#include "SaveLogs.h"
#include "GlobalSettings.h"
#include <SysUtils.hpp>
#include "Processing.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


void TSaveLogs::WriteLogsToFile(AnsiString directoryName, AnsiString fileName, TStringList *List)
{
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
     fileName =  directoryName + "/" + fileName;

    if (FileExists(fileName))
    {
        std::auto_ptr <TStringList> OldLogs(new TStringList);
        OldLogs->LoadFromFile(fileName);
        OldLogs->Add("---------------------------------------------------------------------------------------------------------------" );
        OldLogs->AddStrings(List);
        OldLogs->SaveToFile(fileName );
    }
    else
    {
        List->SaveToFile(fileName );
    }
}
//------------------------------------------------------------------------------------------
void TSaveLogs::RecordFiscalLogs(TStringList* logList)
{
    try
    {
        if(TGlobalSettings::Instance().UseItalyFiscalPrinter)
        {
            AnsiString path = ExtractFilePath(Application->ExeName) + "/Fiscal POS Logs";
            AnsiString fileName = Now().CurrentDate().FormatString("DDMMYYYY")+ ".txt";
            TSaveLogs::WriteLogsToFile(path, fileName, logList);
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ex.Message);
    }
}
