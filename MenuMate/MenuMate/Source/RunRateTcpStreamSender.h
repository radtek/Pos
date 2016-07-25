//---------------------------------------------------------------------------

#ifndef RunRateTcpStreamSenderH
#define RunRateTcpStreamSenderH

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


//---------------------------------------------------------------------------

class ETcpStreamSenderRunRateInvalidIPPort : public Exception
{
	public:
		ETcpStreamSenderRunRateInvalidIPPort( UnicodeString inMsg );
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TRunRateTcpStreamSenderThread;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TRunRateTcpStreamSender
{
	protected:
		AnsiString FIPAddr;
		__int32    FIPPort;

		void SetIPAddr( AnsiString inIPAddr );
		void SetIPPort( __int32    inIPPort );

	public:
		static TRunRateTcpStreamSender& Instance()
		{
			static TRunRateTcpStreamSender singleton;
			return singleton;
		}

		TRunRateTcpStreamSender();
		TRunRateTcpStreamSender( AnsiString inIPAddress, __int32 inIPPort );
		~TRunRateTcpStreamSender();

		__property AnsiString IPAddress = { read=FIPAddr, write=SetIPAddr };
		__property __int32    IPPort    = { read=FIPPort, write=SetIPPort };

		void Send(UnicodeString inData);

	private:
		TIdTCPClient* createTcpClient();
		void __fastcall sendThreadTerminated( TObject *Sender );
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TRunRateTcpStreamSenderThread : public TThread
{
	protected:
		TIdTCPClient*  _tcpClient;
		AnsiString _data;
		TiXmlDocument* FResponse;

		virtual void __fastcall Execute();

	public:
		__fastcall TRunRateTcpStreamSenderThread(
							TIdTCPClient* inTcpClient,
							AnsiString inData );
		__fastcall ~TRunRateTcpStreamSenderThread();

		__property TiXmlDocument* response  = { read=FResponse };

	private:
		void SendAndFetch( UnicodeString data );
		void sendData( TIdTCPClient* tcpClient, UnicodeString data );
		void fetchResponse( TIdTCPClient* tcpClient, TiXmlDocument* outResponse );

		TBytes createByteArray( const char* inBuffer, __int32 inBufferSize );
};

//---------------------------------------------------------------------------

#endif
