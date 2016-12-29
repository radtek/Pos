//---------------------------------------------------------------------------

#ifndef CSVExportTcpStreamSenderH
#define CSVExportTcpStreamSenderH

//---------------------------------------------------------------------------

#include <IdTCPClient.hpp>
#include "tinyxml.h"

#pragma link "IndyCore.bpi"
#pragma link "IndyProtocols.bpi"

//---------------------------------------------------------------------------

#define _DEFAULT_CONNECT_TIME_OUT    1000
#define _DEFAULT_READ_TIME_OUT      60000
#define _DEFAULT_RETRY_COUNT            3
#define _DEFAULT_SLEEP_ON_EXCEPTION   300

// 1000 = 1 sec
#define _CONNECT_TIME_OUT 3000
#define _READ_TIME_OUT    3000

#define _RESPONSE_BUFFER_SIZE 1024
#include "CSV.h"

//---------------------------------------------------------------------------

class ETcpStreamSenderCSVExportInvalidIPPort : public Exception
{
	public:
		ETcpStreamSenderCSVExportInvalidIPPort( UnicodeString inMsg );
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TCSVExportTcpStreamSenderThread;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TCSVExportTcpStreamSender
{
	protected:
		AnsiString FIPAddr;
		__int32    FIPPort;

		void SetIPAddr( AnsiString inIPAddr );
		void SetIPPort( __int32    inIPPort );

	public:
		static TCSVExportTcpStreamSender& Instance()
		{
			static TCSVExportTcpStreamSender singleton;
			return singleton;
		}

		TCSVExportTcpStreamSender();
		TCSVExportTcpStreamSender( AnsiString inIPAddress, __int32 inIPPort );
		~TCSVExportTcpStreamSender();

		__property AnsiString IPAddress = { read=FIPAddr, write=SetIPAddr };
		__property __int32    IPPort    = { read=FIPPort, write=SetIPPort };

		void Send(TStringList* csvData );

	private:
		TIdTCPClient* createTcpClient();
		void __fastcall sendThreadTerminated( TObject *Sender );
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TCSVExportTcpStreamSenderThread : public TThread
{
	protected:
		TIdTCPClient*  _tcpClient;
	    TMemoryStream* _csvStream;

		virtual void __fastcall Execute();

	public:
		__fastcall TCSVExportTcpStreamSenderThread(
							TIdTCPClient* inTcpClient,
							TStringList* csvData );
		__fastcall ~TCSVExportTcpStreamSenderThread();

	private:
		void SendAndFetch();
		void sendData( TIdTCPClient* tcpClient);
		void fetchResponse( TIdTCPClient* tcpClient, TiXmlDocument* outResponse );
		TBytes createByteArray( const char* inBuffer, __int32 inBufferSize );
};

//---------------------------------------------------------------------------

#endif
