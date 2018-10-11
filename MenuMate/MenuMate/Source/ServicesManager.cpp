//---------------------------------------------------------------------------


#pragma hdrstop

#include "ServicesManager.h"
#include "MMLogging.h"
#include "MMMessageBox.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TServicesManager::TServicesManager()
{
    listLogs = new TStringList;
}
//---------------------------------------------------------------------------
TServicesManager::~TServicesManager()
{
    delete listLogs;
    listLogs = NULL;
}
//---------------------------------------------------------------------------
bool TServicesManager::RestartService(UnicodeString serviceName)
{
    listLogs->Add("Inside RestartService(UnicodeString serviceName) method");
    listLogs->Add("Name of Service " + serviceName);
    listLogs->Add("Time:-        " + Now().FormatString("hh:nn:ss am/pm"));
    bool retValue = false;
    ServiceName = serviceName;
    listLogs->Add("Name of Service that was assigned" + ServiceName);
    TServiceProcess stopValue = eStatusNone;
    listLogs->Add("Going to stop service" + ServiceName);
    stopValue = DoStopSvc();
    listLogs->Add("Returned from stopping service" + ServiceName);
    TServiceProcess startValue = eStatusNone;
    //if(stopValue == eStopSuccessful || stopValue == eServiceAlreadyStopped)
    if(WasStopOperationSuccessful(stopValue))
    {
       listLogs->Add("Going to start service" + ServiceName);
       startValue = DoStartSvc();
       listLogs->Add("Returned from starting service" + ServiceName);
    }
    retValue = WasStartOperationSuccessful(startValue);
    AnsiString fileName = GetFileName();
    if(fileName != NULL && fileName != "")
    {
        MakeServiceLogFile(fileName);
    }
    return retValue;
}
//---------------------------------------------------------------------------
void TServicesManager::RestartServices()
{

}
//---------------------------------------------------------------------------
TServiceProcess __stdcall TServicesManager::DoStopSvc()
{
    TServiceProcess retValue;

    try
    {
        SERVICE_STATUS_PROCESS ssp;
        DWORD dwStartTime = GetTickCount();
        DWORD dwBytesNeeded;
        DWORD dwTimeout = 30000; // 30-second time-out
        DWORD dwWaitTime;

        // Get a handle to the SCM database.

        schSCManager = OpenSCManager(
            NULL,                    // local computer
            NULL,                    // ServicesActive database
            SC_MANAGER_ALL_ACCESS);  // full access rights

        if (NULL == schSCManager)
        {
            retValue = eServiceManagerFailed;
            listLogs->Add("ServiceControlManager opening failed");
            return retValue;
        }

        // Get a handle to the service.

        schService = OpenService(
            schSCManager,         // SCM database
            ServiceName.t_str(),
    //        szSvcName,            // name of service
            SERVICE_STOP |
            SERVICE_QUERY_STATUS |
            SERVICE_ENUMERATE_DEPENDENTS);

        if (schService == NULL)
        {
            retValue = eServiceOpenFailed;
            listLogs->Add(ServiceName +" Service opening failed");
            CloseServiceHandle(schSCManager);
            return retValue;
        }

        // Make sure the service is not already stopped.

        if ( !QueryServiceStatusEx(
                schService,
                SC_STATUS_PROCESS_INFO,
                (LPBYTE)&ssp,
                sizeof(SERVICE_STATUS_PROCESS),
                &dwBytesNeeded ) )
        {
            retValue = eServiceStatusFailed;
            listLogs->Add("Service status failed");
            CloseHandles();
            return retValue;
        }

        if ( ssp.dwCurrentState == SERVICE_STOPPED )
        {
            retValue = eServiceAlreadyStopped;
            listLogs->Add("Service Already Stopped");
            CloseHandles();
            return retValue;
        }

        // If a stop is pending, wait for it.

        while ( ssp.dwCurrentState == SERVICE_STOP_PENDING )
        {

            // Do not wait longer than the wait hint. A good interval is
            // one-tenth of the wait hint but not less than 1 second
            // and not more than 10 seconds.

            dwWaitTime = ssp.dwWaitHint / 10;

            if( dwWaitTime < 1000 )
                dwWaitTime = 1000;
            else if ( dwWaitTime > 10000 )
                dwWaitTime = 10000;

            Sleep( dwWaitTime );

            if ( !QueryServiceStatusEx(
                     schService,
                     SC_STATUS_PROCESS_INFO,
                     (LPBYTE)&ssp,
                     sizeof(SERVICE_STATUS_PROCESS),
                     &dwBytesNeeded ) )
            {
                retValue = eServiceStatusFailed;
                listLogs->Add("Service status failed");
                CloseHandles();
                return retValue;
            }

            if ( ssp.dwCurrentState == SERVICE_STOPPED )
            {
                retValue = eStopSuccessful;
                listLogs->Add("Service Stopped");
                CloseHandles();
                return retValue;
            }

            if ( GetTickCount() - dwStartTime > dwTimeout )
            {
                retValue = eServiceStopTimeOut;
                listLogs->Add("Service Stopping Timed Out");
                CloseHandles();
                return retValue;
            }
        }

        // If the service is running, dependencies must be stopped first.

        StopDependentServices();

        // Send a stop code to the service.

        if ( !ControlService(
                schService,
                SERVICE_CONTROL_STOP,
                (LPSERVICE_STATUS) &ssp ) )
        {
            retValue = eStopFailed;
            CloseHandles();
            return retValue;
        }

        // Wait for the service to stop.

        while ( ssp.dwCurrentState != SERVICE_STOPPED )
        {
            Sleep( ssp.dwWaitHint );
            if ( !QueryServiceStatusEx(
                    schService,
                    SC_STATUS_PROCESS_INFO,
                    (LPBYTE)&ssp,
                    sizeof(SERVICE_STATUS_PROCESS),
                    &dwBytesNeeded ) )
            {
                retValue = eServiceStatusFailed;
                listLogs->Add("Service status failed");
                CloseHandles();
                return retValue;
            }

            if ( ssp.dwCurrentState == SERVICE_STOPPED )
            {
                retValue = eStopSuccessful;
                listLogs->Add("Service Stopped");
                break;
            }
            if ( GetTickCount() - dwStartTime > dwTimeout )
            {
                retValue = eServiceStopTimeOut;
                listLogs->Add("Service Stopping Timed Out");
                CloseHandles();
                return retValue;
            }
        }

//        stop_cleanup:
//        CloseServiceHandle(schService);
//        CloseServiceHandle(schSCManager);
            CloseHandles();

        //

    }
    catch(Exception &ex)
    {
        listLogs->Add("Exception Occured in Service Stop Method: " + ex.Message);
    }
    return retValue;
}
//---------------------------------------------------------------------------
TServiceProcess __stdcall TServicesManager::DoStartSvc()
{
    TServiceProcess retValue;
    try
    {
        SERVICE_STATUS_PROCESS ssStatus;
        DWORD dwOldCheckPoint;
        DWORD dwStartTickCount;
        DWORD dwWaitTime;
        DWORD dwBytesNeeded;

        // Get a handle to the SCM database.

        schSCManager = OpenSCManager(
            NULL,                    // local computer
            NULL,                    // servicesActive database
            SC_MANAGER_ALL_ACCESS);  // full access rights

        if (NULL == schSCManager)
        {
            retValue = eServiceManagerFailed;
            listLogs->Add("ServiceControlManager opening failed");
            return retValue;
        }

        // Get a handle to the service.

        schService = OpenService(
            schSCManager,         // SCM database
            ServiceName.t_str(), // name of service
            //szSvcName,
            SERVICE_ALL_ACCESS);  // full access

        if (schService == NULL)
        {
            retValue = eServiceOpenFailed;
            listLogs->Add(ServiceName + " Service opening failed");
            CloseServiceHandle(schSCManager);
            return retValue;
        }

        // Check the status in case the service is not stopped.

        if (!QueryServiceStatusEx(
                schService,                     // handle to service
                SC_STATUS_PROCESS_INFO,         // information level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // size needed if buffer is too small
        {
            retValue = eServiceStatusFailed;
            listLogs->Add("Service status failed");
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return retValue;
        }

        // Check if the service is already running. It would be possible
        // to stop the service here, but for simplicity this example just returns.

        if(ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
        {
            retValue = eServiceAlreadyStarted;
            listLogs->Add("Service already running");
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return retValue;
        }

        // Save the tick count and initial checkpoint.

        dwStartTickCount = GetTickCount();
        dwOldCheckPoint = ssStatus.dwCheckPoint;

        // Wait for the service to stop before attempting to start it.

        while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
        {
            // Do not wait longer than the wait hint. A good interval is
            // one-tenth of the wait hint but not less than 1 second
            // and not more than 10 seconds.

            dwWaitTime = ssStatus.dwWaitHint / 10;

            if( dwWaitTime < 1000 )
                dwWaitTime = 1000;
            else if ( dwWaitTime > 10000 )
                dwWaitTime = 10000;

            Sleep( dwWaitTime );

            // Check the status until the service is no longer stop pending.

            if (!QueryServiceStatusEx(
                    schService,                     // handle to service
                    SC_STATUS_PROCESS_INFO,         // information level
                    (LPBYTE) &ssStatus,             // address of structure
                    sizeof(SERVICE_STATUS_PROCESS), // size of structure
                    &dwBytesNeeded ) )              // size needed if buffer is too small
            {
                retValue = eServiceStatusFailed;
                listLogs->Add("Service status failed");
                CloseServiceHandle(schService);
                CloseServiceHandle(schSCManager);
                return retValue;
            }

            if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
            {
                // Continue to wait and check.

                dwStartTickCount = GetTickCount();
                dwOldCheckPoint = ssStatus.dwCheckPoint;
            }
            else
            {
                if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
                {
                    retValue = eServiceStartTimeOut;
                    listLogs->Add("Service start Timedout");
                    CloseServiceHandle(schService);
                    CloseServiceHandle(schSCManager);
                    return retValue;
                }
            }
        }

        // Attempt to start the service.

        if (!StartService(
                schService,  // handle to service
                0,           // number of arguments
                NULL) )      // no arguments
        {
            retValue = eStartFailed;
            listLogs->Add("Service start failed");
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return retValue;
        }

        // Check the status until the service is no longer start pending.

        if (!QueryServiceStatusEx(
                schService,                     // handle to service
                SC_STATUS_PROCESS_INFO,         // info level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // if buffer too small
        {
            retValue = eServiceStatusFailed;
            listLogs->Add("Service status failed");
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return retValue;
        }

        // Save the tick count and initial checkpoint.

        dwStartTickCount = GetTickCount();
        dwOldCheckPoint = ssStatus.dwCheckPoint;

        while (ssStatus.dwCurrentState == SERVICE_START_PENDING)
        {
            // Do not wait longer than the wait hint. A good interval is
            // one-tenth the wait hint, but no less than 1 second and no
            // more than 10 seconds.

            dwWaitTime = ssStatus.dwWaitHint / 10;

            if( dwWaitTime < 1000 )
                dwWaitTime = 1000;
            else if ( dwWaitTime > 10000 )
                dwWaitTime = 10000;

            Sleep( dwWaitTime );

            // Check the status again.

            if (!QueryServiceStatusEx(
                schService,             // handle to service
                SC_STATUS_PROCESS_INFO, // info level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // if buffer too small
            {
                retValue = eServiceStatusFailed;
                listLogs->Add("Service status failed");
                break;
            }

            if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
            {
                // Continue to wait and check.

                dwStartTickCount = GetTickCount();
                dwOldCheckPoint = ssStatus.dwCheckPoint;
            }
            else
            {
                if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
                {
                    // No progress made within the wait hint.
                    break;
                }
            }
        }

        // Determine whether the service is running.

        if (ssStatus.dwCurrentState == SERVICE_RUNNING)
        {
            retValue = eStartSuccessful;
            listLogs->Add("Service start successful");
        }
        else
        {
//            MessageBox("Service not started.\n","",MB_OK);
//            MessageBox("  Current State:\n"+ ssStatus.dwCurrentState,"",MB_OK);
//            MessageBox("  Exit Code:\n"+ ssStatus.dwWin32ExitCode,"",MB_OK);
//            MessageBox("  Check Point: \n"+ ssStatus.dwCheckPoint,"",MB_OK);
//            MessageBox("  Wait Hint: \n"+ ssStatus.dwWaitHint,"",MB_OK);
        }

        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
    }
    catch(Exception &ex)
    {
        listLogs->Add("Exception Occured in Service start Method:- " + ex.Message);
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool __stdcall TServicesManager::StopDependentServices()
{
    DWORD i;
    DWORD dwBytesNeeded;
    DWORD dwCount;

    LPENUM_SERVICE_STATUS   lpDependencies = NULL;
    ENUM_SERVICE_STATUS     ess;
    SC_HANDLE               hDepService;
    SERVICE_STATUS_PROCESS  ssp;

    DWORD dwStartTime = GetTickCount();
    DWORD dwTimeout = 30000; // 30-second time-out

    // Pass a zero-length buffer to get the required buffer size.
    if ( EnumDependentServices( schService, SERVICE_ACTIVE,
         lpDependencies, 0, &dwBytesNeeded, &dwCount ) )
    {
         // If the Enum call succeeds, then there are no dependent
         // services, so do nothing.
         return false;
    }
    else
    {
        if ( GetLastError() != ERROR_MORE_DATA )
            return false; // Unexpected error

        // Allocate a buffer for the dependencies.
        lpDependencies = (LPENUM_SERVICE_STATUS) HeapAlloc(
            GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded );

        if ( !lpDependencies )
            return false;

        __try {
            // Enumerate the dependencies.
            if ( !EnumDependentServices( schService, SERVICE_ACTIVE,
                lpDependencies, dwBytesNeeded, &dwBytesNeeded,
                &dwCount ) )
            return false;

            for ( i = 0; i < dwCount; i++ )
            {
                ess = *(lpDependencies + i);
                // Open the service.
                hDepService = OpenService( schSCManager,
                   ess.lpServiceName,
                   SERVICE_STOP | SERVICE_QUERY_STATUS );

                if ( !hDepService )
                   return false;

                __try {
                    // Send a stop code.
                    if ( !ControlService( hDepService,
                            SERVICE_CONTROL_STOP,
                            (LPSERVICE_STATUS) &ssp ) )
                    return false;

                    // Wait for the service to stop.
                    while ( ssp.dwCurrentState != SERVICE_STOPPED )
                    {
                        Sleep( ssp.dwWaitHint );
                        if ( !QueryServiceStatusEx(
                                hDepService,
                                SC_STATUS_PROCESS_INFO,
                                (LPBYTE)&ssp,
                                sizeof(SERVICE_STATUS_PROCESS),
                                &dwBytesNeeded ) )
                        return false;

                        if ( ssp.dwCurrentState == SERVICE_STOPPED )
                            break;

                        if ( GetTickCount() - dwStartTime > dwTimeout )
                            return false;
                    }
                }
                __finally
                {
                    // Always release the service handle.
                    CloseServiceHandle( hDepService );
                }
            }
        }
        __finally
        {
            // Always free the enumeration buffer.
            HeapFree( GetProcessHeap(), 0, lpDependencies );
        }
    }
    return true;
}
//---------------------------------------------------------------------------
void __stdcall TServicesManager::DoUpdateSvcDacl()
{
}
//---------------------------------------------------------------------------
bool TServicesManager::AnalyseReturnValue(TServiceProcess stopValue, TServiceProcess startValue)
{
    bool retValue = false;
    if(startValue != eStatusNone)
    {
        if(startValue == eServiceAlreadyStarted || startValue == eStartSuccessful)
            retValue = true;
    }
    if(!retValue)
    {
        if(stopValue != eStatusNone)
        {
            MessageBox("Till can not be closed at this time.\r"
            "Please ensure firebird services are running.","Error",MB_OK + MB_ICONERROR);
        }
    }
    return retValue;
}
//--------------------------------------------------------------------------
void TServicesManager::MakeServiceLogFile(AnsiString infileName)
{
    try
    {
        listLogs->Add("===========================================================================================================");
        listLogs->Add("\n");
        listLogs->SaveToFile(infileName);
        listLogs->Clear();
    }
    catch(Exception &Exc)
    {
        //TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//---------------------------------------------------------------------------
AnsiString TServicesManager::GetFileName()
{
    AnsiString directoryName = "";
    AnsiString fileName = "";
    directoryName = ExtractFilePath(Application->ExeName) + "/Logs";
    if (!DirectoryExists(directoryName))
    {
        CreateDir(directoryName);
    }
    directoryName = directoryName + "/Service Restart Logs";
    if (!DirectoryExists(directoryName))
    {
        CreateDir(directoryName);
    }
    AnsiString name = "ServiceRestart " + Now().CurrentDate().FormatString("DDMMMYYYY")+ ".txt";
    fileName = directoryName + "/" + name;
    return fileName;
}
//---------------------------------------------------------------------------
void TServicesManager::CloseHandles()
{
    try
    {
        CloseHandle1();
        CloseHandle2();
    }
    catch(Exception &ex)
    {
    }
}
//---------------------------------------------------------------------------
void TServicesManager::CloseHandle1()
{
    CloseServiceHandle(schService);
}
//---------------------------------------------------------------------------
void TServicesManager::CloseHandle2()
{
    CloseServiceHandle(schSCManager);
}
//---------------------------------------------------------------------------
bool TServicesManager::WasStopOperationSuccessful(TServiceProcess stopValue)
{
    if(stopValue == eStopSuccessful || stopValue == eServiceAlreadyStopped)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
bool TServicesManager::WasStartOperationSuccessful(TServiceProcess startValue)
{
    bool retValue = false;
    if(startValue == eStartSuccessful || startValue == eServiceAlreadyStarted
       || startValue == eStatusNone)
        return true;
    else
    {
        if(startValue == eStartFailed || startValue == eServiceStartTimeOut)
        {
                MessageBox("Till can not be closed at this time.\r"
                 "Firebird Service is not running.","Error",MB_OK);
        }
    }
    return retValue;
}
//---------------------------------------------------------------------------
