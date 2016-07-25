//---------------------------------------------------------------------------

#ifndef RunRateTcpStreamSenderSettingsH
#define RunRateTcpStreamSenderSettingsH


#include <vcl.h>

//---------------------------------------------------------------------------

#define _DEFAULT_IP_ADDR "localhost"
#define _DEFAULT_IP_PORT 57001

class TRunRateTcpStreamSenderSettings
{
	protected:
		AnsiString FIPAddr;
		__int32    FIPPort;

		void SetIPAddr( AnsiString inIPAddr );
		void SetIPPort( __int32    inIPPort );

	public:
		static TRunRateTcpStreamSenderSettings& Instance()
		{
			static TRunRateTcpStreamSenderSettings singleton;
			return singleton;
		}

		TRunRateTcpStreamSenderSettings();

		__property AnsiString IPAddress = { read=FIPAddr, write=SetIPAddr };
		__property __int32    IPPort    = { read=FIPPort, write=SetIPPort };
};
//---------------------------------------------------------------------------

#endif
