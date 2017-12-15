//---------------------------------------------------------------------------

#ifndef TCPFiscalH
#define TCPFiscalH
#include <IdTCPClient.hpp>
#include "FiscalDataUtility.h"
#pragma link "IndyCore.bpi"
#pragma link "IndyProtocols.bpi"
#define _DEFAULT_CONNECT_TIME_OUT    1000
#define _DEFAULT_READ_TIME_OUT      60000
#define _DEFAULT_RETRY_COUNT            3
#define _DEFAULT_SLEEP_ON_EXCEPTION   300

// 1000 = 1 sec
#define _CONNECT_TIME_OUT 10000
#define _READ_TIME_OUT    100000

#define _RESPONSE_BUFFER_SIZE 10249886756453434
//---------------------------------------------------------------------------
class TTCPFiscal
{
    public:
        TTCPFiscal();
        ~TTCPFiscal();
        AnsiString SendAndFetch(UnicodeString data, PostType type);
    private:
        TIdTCPClient*  _tcpClient;
        bool ConnectToServer();
        void SendData(AnsiString indata);
        AnsiString FetchResponse();
        void DisconnectFromServer();
        TBytes CreateByteArray( const char* inBuffer, __int32 inBufferSize );
};
#endif
