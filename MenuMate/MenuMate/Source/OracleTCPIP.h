//---------------------------------------------------------------------------

#ifndef OracleTCPIPH
#define OracleTCPIPH

#include "OracleDataObjects.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "tinyxml.h"
//---------------------------------------------------------------------------
#define _DEFAULT_CONNECT_TIME_OUT    1000
#define _DEFAULT_READ_TIME_OUT      60000
#define _DEFAULT_RETRY_COUNT            3
#define _DEFAULT_SLEEP_ON_EXCEPTION   300

// 1000 = 1 sec
#define _CONNECT_TIME_OUT 3000
#define _READ_TIME_OUT    100000

#define _RESPONSE_BUFFER_SIZE 10249886756453434
//---------------------------------------------------------------------------
class TOracleTCPIP
{
    public:
        static TOracleTCPIP& Instance() {
            static TOracleTCPIP singleton;
            return singleton;
        }
        TOracleTCPIP();
        ~TOracleTCPIP();
        void SendAndFetch(AnsiString data,AnsiString address,int portNumber);
        AnsiString SerializeOut(TPostRoomInquiry roomInquiry);
        AnsiString SerializeOut(TPostRequest postRequest);
        AnsiString SerializeOut(TLinkDescription linkDes);
        TRoomInquiryItem SerializeIn(AnsiString data, TRoomInquiryAnswer roomAnswer);
        TPostRequestAnswer SerializeIn(AnsiString data);
        TLinkAlive SerializeIn(AnsiString data, int i);
        OracleRequestType oracleRequestType;
        bool Connect();
    private:
        TIdTCPClient* CreateTCPClient();
        TIdTCPClient* tcpClient;
};
#endif
