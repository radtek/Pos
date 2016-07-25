//---------------------------------------------------------------------------

#ifndef TcpStreamSenderSettingsH
#define TcpStreamSenderSettingsH


#include <vcl.h>

//---------------------------------------------------------------------------

#define _DEFAULT_IP_ADDR "localhost"
#define _DEFAULT_IP_PORT 56001

class TTcpStreamSenderSettings
{
	protected:
		AnsiString FIPAddr;
		__int32    FIPPort;

		void SetIPAddr( AnsiString inIPAddr );
		void SetIPPort( __int32    inIPPort );

	public:
		static TTcpStreamSenderSettings& Instance()
		{
			static TTcpStreamSenderSettings singleton;
			return singleton;
		}

		TTcpStreamSenderSettings();

		__property AnsiString IPAddress = { read=FIPAddr, write=SetIPAddr };
		__property __int32    IPPort    = { read=FIPPort, write=SetIPPort };
};
//---------------------------------------------------------------------------

#endif
