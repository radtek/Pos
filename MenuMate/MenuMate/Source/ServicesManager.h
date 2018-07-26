//---------------------------------------------------------------------------

#ifndef ServicesManagerH
#define ServicesManagerH
#include <memory>
#include <System.hpp>
#include <Classes.hpp>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <aclapi.h>

//---------------------------------------------------------------------------
enum TServiceProcess {eStopSuccessful = 0, eStopFailed, eServiceManagerFailed,
                      eServiceOpenFailed, eServiceStatusFailed, eServiceAlreadyStopped,
                      eServiceStopTimeOut,
                      eStartSuccessful = 20,eStartFailed, eServiceAlreadyStarted,eServiceStartTimeOut, eStatusNone};
class TServicesManager
{
    public:
    TServicesManager();
    ~TServicesManager();
    bool RestartService(UnicodeString serviceName);
    void RestartServices();
    TStringList *listLogs;

    private:
    UnicodeString ServiceName;
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    TServiceProcess __stdcall DoStopSvc(void);
    TServiceProcess __stdcall DoStartSvc(void);
    void __stdcall DoUpdateSvcDacl(void);
    bool __stdcall StopDependentServices(void);
    bool AnalyseReturnValue(TServiceProcess stopValue, TServiceProcess startValue);
    AnsiString GetFileName();
    void MakeServiceLogFile(AnsiString infileName);
    void CloseHandles();
    void CloseHandle1();
    void CloseHandle2();
    bool WasStopOperationSuccessful(TServiceProcess stopValue);
    bool WasStartOperationSuccessful(TServiceProcess startValue);
};
#endif
