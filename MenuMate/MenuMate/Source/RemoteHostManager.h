//---------------------------------------------------------------------------

#ifndef RemoteHostManagerH
#define RemoteHostManagerH

//---------------------------------------------------------------------------

#include <vcl.h>

//---------------------------------------------------------------------------

enum TRemoteHostManagerError { rhmNO_ERROR, rhmFAILED_TO_CONNECT_TO_REMOTE_HOST };

class TRemoteHostManager
{
	public:
		static TRemoteHostManager& Instance()
		{
			static TRemoteHostManager singleton;
			return singleton;
		}

		TRemoteHostManager();

		void SetAndValidate( AnsiString inHostName, AnsiString inUserName, AnsiString inPassword );
		void Validate();

		AnsiString ResolveHostName( AnsiString inHostName );
		bool IsLocalHost( AnsiString inHostName );
		AnsiString LocalHost();

		__property AnsiString HostName   = { read = FHostName, write = writeHostName };
		__property AnsiString UserName   = { read = FUserName, write = writeUserName };
		__property AnsiString Password   = { read = FPassword, write = writePassword };

		__property TRemoteHostManagerError LastError    = { read = FLastError    };
		__property AnsiString              LastErrorMsg = { read = FLastErrorMsg };

	private:
	static TRemoteHostManager* pInstance;

	protected:
		AnsiString FHostName;
		AnsiString FUserName;
		AnsiString FPassword;

		TRemoteHostManagerError FLastError;
		AnsiString              FLastErrorMsg;

		bool ConnectToRemoteHost();

		void writeHostName( AnsiString inHostName   );
		void writeUserName( AnsiString inUserName );
		void writePassword( AnsiString inPassword );

		void SetLastError( TRemoteHostManagerError inError);
};
//---------------------------------------------------------------------------
#endif
