//---------------------------------------------------------------------------


#pragma hdrstop

#include "SignalRUtility.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "ManagerSyndCode.h"
#include "MMMessageBox.h"
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include "Processing.h"
#pragma package(smart_init)
//----------------------------------------------------------------------------
void TSignalRUtility::MakeOnlineOrderingSeedFile()
{
    try
    {
        AnsiString fileName = GetFileName();
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
        {
            List->LoadFromFile(fileName);
            List->Clear();
        }
        AnsiString siteId = TGlobalSettings::Instance().SiteID;
        AnsiString syndicateCode = GetSyndicateCode();
        List->Add(siteId);
        List->Add(syndicateCode);
        List->SaveToFile(fileName );
    }
    catch(Exception &Ex)
    {
        MessageBox(Ex.Message,"Exception in MakeOnlineOrderingSeedFile",MB_OK);
    }
}
//----------------------------------------------------------------------------
bool TSignalRUtility::UnloadSignalRUtility()
{
    bool retValue = false;
    std::auto_ptr<TfrmProcessing>(Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
    Processing->Message = "Managing Online Ordering Module";
    Processing->Show();
    try
    {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        if (Process32First(snapshot, &entry) == TRUE)
        {
            while (Process32Next(snapshot, &entry) == TRUE)
            {
                if (stricmp(entry.szExeFile, "OnlineOrderingApp.exe") == 0)
                {
                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                    TerminateProcess(hProcess,0);
                    CloseHandle(hProcess);
                    retValue = true;
                    break;
                }
            }
        }
        CloseHandle(snapshot);
    }
    catch(Exception &Ex)
    {

    }
    Processing->Close();
    return retValue;
}
//----------------------------------------------------------------------------
bool TSignalRUtility::LoadSignalRUtility()
{
    bool isProcOpen = false;
    std::auto_ptr<TfrmProcessing>(Processing)(TfrmProcessing::Create<TfrmProcessing>(NULL));
    Processing->Message = "Managing Online Ordering Module";
    Processing->Show();
    MakeOnlineOrderingSeedFile();
    try
    {
        STARTUPINFO siSignalRApp;
        PROCESS_INFORMATION piSignalRApp;
        ZeroMemory( &siSignalRApp, sizeof(siSignalRApp) );
        siSignalRApp.cb = sizeof(siSignalRApp);
        ZeroMemory( &piSignalRApp, sizeof(piSignalRApp) );
        if(UnloadSignalRUtility())
        {
            Sleep(10000);
        }
        UnicodeString strPath = GetUtilityName();
        isProcOpen = CreateProcess( strPath.t_str(),   // the path
        NULL,
        //argv[1],        // Command line
        NULL,                   // Process handle not inheritable
        NULL,                   // Thread handle not inheritable
        FALSE,                  // Set handle inheritance to FALSE
        0,                      // No creation flags
        NULL,                   // Use parent's environment block
        NULL,                   // Use parent's starting directory
        &siSignalRApp,            // Pointer to STARTUPINFO structure
        &piSignalRApp             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
        );
        Sleep(2000);
    }
    catch(Exception &Ex)
    {
    }
    Processing->Close();
    return isProcOpen;
}
//----------------------------------------------------------------------------
bool TSignalRUtility::IsSignalRRunning()
{
    bool retValue = false;
    try
    {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        if (Process32First(snapshot, &entry) == TRUE)
        {
            while (Process32Next(snapshot, &entry) == TRUE)
            {
                if (stricmp(entry.szExeFile, "OnlineOrderingApp.exe") == 0)
                {
                    retValue = true;
                }
            }
        }
    }
    catch(Exception &Ex)
    {
    }
    return retValue;
}
//----------------------------------------------------------------------------
AnsiString TSignalRUtility::GetUtilityName()
{
    return ExtractFilePath(Application->ExeName) + "OnlineOrderingApp.exe" ;
}
//----------------------------------------------------------------------------
AnsiString TSignalRUtility::GetFileName()
{
    return ExtractFilePath(Application->ExeName) + "OnlineOrderingSeed.txt" ;
}
//----------------------------------------------------------------------------
AnsiString TSignalRUtility::GetSyndicateCode()
{
    TManagerSyndCode managerSyndCode = TDeviceRealTerminal::Instance().ManagerMembership->GetSyndicateCodeManager();
    TSyndCode syndicateCode =  managerSyndCode.GetCommunicationSyndCode();
    return syndicateCode.GetSyndCode();
}
//----------------------------------------------------------------------------

