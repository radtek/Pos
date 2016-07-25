//---------------------------------------------------------------------------

#ifndef TcpStreamSenderH
#define TcpStreamSenderH

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

class ETcpStreamSenderInvalidIPPort : public Exception
{
	public:
		ETcpStreamSenderInvalidIPPort( UnicodeString inMsg );
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TTcpStreamSenderThread;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TTcpStreamSender
{
	protected:
		AnsiString FIPAddr;
		__int32    FIPPort;

		void SetIPAddr( AnsiString inIPAddr );
		void SetIPPort( __int32    inIPPort );

	public:
		static TTcpStreamSender& Instance()
		{
			static TTcpStreamSender singleton;
			return singleton;
		}

		TTcpStreamSender();
		TTcpStreamSender( AnsiString inIPAddress, __int32 inIPPort );
		~TTcpStreamSender();

		__property AnsiString IPAddress = { read=FIPAddr, write=SetIPAddr };
		__property __int32    IPPort    = { read=FIPPort, write=SetIPPort };

		void Send( TiXmlDocument *inXMLOrderDoc );

	private:
		TIdTCPClient* createTcpClient();
		void __fastcall sendThreadTerminated( TObject *Sender );
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TTcpStreamSenderThread : public TThread
{
	protected:
		TIdTCPClient*  _tcpClient;
		TiXmlDocument* _xmlOrderDoc;
		TiXmlDocument* FResponse;

		virtual void __fastcall Execute();

	public:
		__fastcall TTcpStreamSenderThread(
							TIdTCPClient* inTcpClient,
							TiXmlDocument* inXMLOrderDoc );
		__fastcall ~TTcpStreamSenderThread();

		__property TiXmlDocument* response  = { read=FResponse };

	private:
		TiXmlDocument* SendAndFetch( TiXmlDocument* inXMLOrderDoc );
		void sendData( TIdTCPClient* tcpClient, TiXmlDocument* inXMLOrderDoc );
		void fetchResponse( TIdTCPClient* tcpClient, TiXmlDocument* outResponse );

		TBytes createByteArray( const char* inBuffer, __int32 inBufferSize );

		TiXmlDocument* buildResponse( AnsiString inOrderNo,
									  bool inSuccess,
									  AnsiString inMsg );
		void setResponse( TiXmlDocument* inXmlDoc,
						  AnsiString     inOrderNo,
						  bool           inSuccess,
						  AnsiString     inMsg );

		void addElement(  TiXmlElement* inParentElem,
						  AnsiString    inName,
						  TiXmlElement* &inElem );
		void setNodeAttr( TiXmlElement* inElem,
						  AnsiString    inAttrName,
						  AnsiString    inAttrValue );

		AnsiString SerializeOut( TiXmlDocument* inXMLOrderDoc );
		void SerializeIn( TiXmlDocument* inXMLOrderDoc, AnsiString inData );
};

//---------------------------------------------------------------------------

#endif
