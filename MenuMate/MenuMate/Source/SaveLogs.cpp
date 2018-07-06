//---------------------------------------------------------------------------


#pragma hdrstop

#include "SaveLogs.h"

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
