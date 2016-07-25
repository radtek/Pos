//---------------------------------------------------------------------------

#ifndef ServiceManagerH
#define ServiceManagerH

//#include <winsvc.h>

#include <vcl.h>
#include "RemoteHostManager.h"


//---------------------------------------------------------------------------

typedef SC_HANDLE TServiceHandle;

enum TServiceStatus
{
	ssErrorOpening = -1,
	ssNotInstalled = 0,
	ssStopped,
	ssStartPending,
	ssStopPending,
	ssRunning,
	ssContinuePending,
	ssPausePending,
	ssPaused
};

class TServiceManager
{
	public:
		TServiceManager();
		~TServiceManager();

		void SetAndValidate( AnsiString inServiceMachineName, AnsiString inUserName, AnsiString inPassword );
		void Validate();

		TServiceStatus GetServiceStatus( AnsiString inServiceName );

		bool IsServiceInstalled( AnsiString inServiceName );
		bool IsServiceRunning(   AnsiString inServiceName );
		bool IsServiceStopped(   AnsiString inServiceName );
		bool IsServicePaused(    AnsiString inServiceName );

		__property AnsiString ServiceMachineName = { read = FSvcMachineName, write = writeServiceMachineName };
		__property AnsiString UserName           = { read = FUserName,       write = writeUserName };
		__property AnsiString Password           = { read = FPassword,       write = writePassword };
		// Username and Password to the remote host where the service is running
		// They are ignored if the service is running localy

	protected:
		TRemoteHostManager *FRemoteHostManager;

		AnsiString FSvcMachineName;
		AnsiString FUserName;
		AnsiString FPassword;

		#ifdef _UNICODE
			LPCWSTR _host;
			LPCWSTR _db;
		#else
			LPCSTR _host;
			LPCSTR _db;
		#endif

		TServiceHandle _scm;
		TServiceHandle _sc;

		void OpenServiceManager();
		void ResetServiceManager();
		void CloseServiceManager();

		TServiceHandle OpenServiceHdl( AnsiString inServiceName );
		void CloseService();

		void writeServiceMachineName( AnsiString inMachineName );
		void writeUserName( AnsiString inUserName );
		void writePassword( AnsiString inPassword );

		AnsiString ResolveHostName( AnsiString inHostName );
		bool IsLocalHost( AnsiString inHostName );
		AnsiString LocalHost();
		bool ConnectToRemoteHost();
};
#endif
//---------------------------------------------------------------------------