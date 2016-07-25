
//---------------------------------------------------------------------------

#ifndef TCPStreamPrepareH
#define TCPStreamPrepareH
//---------------------------------------------------------------------------

#include <IdTCPClient.hpp>
#include "tinyxml.h"
#include "SystemEvents.h"

#pragma link "IndyCore.bpi"
#pragma link "IndyProtocols.bpi"

//---------------------------------------------------------------------------

#define _DEFAULT_CONNECT_TIME_OUT    1000
#define _DEFAULT_READ_TIME_OUT      60000
#define _DEFAULT_RETRY_COUNT            3
#define _DEFAULT_SLEEP_ON_EXCEPTION   300

// 1000 = 1 sec
#define _CONNECT_TIME_OUT 3000
#define _READ_TIME_OUT    100000

#define _RESPONSE_BUFFER_SIZE 10249886756453434
 //:::::::::::::::::::::::::::::::::::::::::::::::::::::
 // EXCEPTION CLASS
class ERequestResponse : public Exception
{
	public:
		ERequestResponse( UnicodeString inMsg );
};
//::::::::::::::::::::::::::::::::::::::::::::::::::::::

//class TTCPStreamPrepare;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TTCPStreamPrepare
{
	protected:
		AnsiString FIPAddr;
		__int32    FIPPort;

		void SetIPAddr( AnsiString inIPAddr );
		void SetIPPort( __int32    inIPPort );


	public:
		static TTCPStreamPrepare& Instance()
		{
			static TTCPStreamPrepare singleton;
			return singleton;
		}

		TTCPStreamPrepare();
		TTCPStreamPrepare( AnsiString inIPAddress, __int32 inIPPort );
		~TTCPStreamPrepare();

		__property AnsiString IPAddress = { read=FIPAddr, write=SetIPAddr };
		__property __int32    IPPort    = { read=FIPPort, write=SetIPPort };

        void Connect(UnicodeString str1 , int requestType, UnicodeString session_uuid , AnsiString memberNo , UnicodeString lineitemid);

	private:
		TIdTCPClient* createTCPClient();
		void __fastcall sendThreadTerminated( TObject *Sender );
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TTCPClient
{
	public:
        TTCPClient(TIdTCPClient*  inTcpClient,UnicodeString request , int _requestType ,UnicodeString session_uuid , AnsiString memberNo , UnicodeString lineitemid);
        TTCPClient();
		~TTCPClient();
        void makeConnection();
        void strtTransacDC(TIdTCPClient* inTcpClient , UnicodeString str1);
        TBytes createByteArray( const char* inBuffer, __int32 inBufferSize ) ;
        void strtTransacResponse(TIdTCPClient* inTcpClient);
        void sendStream(TIdTCPClient* inTcpClient , UnicodeString str) ;
        void requestDCResponse(TIdTCPClient* inTcpClient);
        void requestDC(TIdTCPClient* inTcpClient , UnicodeString str);
        AnsiString ReadResponse(AnsiString data , int type);
        void makeLogFile(AnsiString str);
        void writeToProductFile(AnsiString str);

		__property AnsiString Request    = { read=requestDc, write=Setrequestdc };
        bool ResponseOk;
        bool SendRequest;
        void Setrequestdc( AnsiString request );
		void StartProcess();

	private:
	TIdTCPClient*  _tcpClient;
    int RequestType;
    AnsiString requestDc;
    AnsiString memberNo;
    UnicodeString session_id;
    UnicodeString line_item;
    AnsiString Stamp;
    AnsiString Time_Stamp;
    bool CaseOfZeroPour;


};

//---------------------------------------------------------------------------
#endif


