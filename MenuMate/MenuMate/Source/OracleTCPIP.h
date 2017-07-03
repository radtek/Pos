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
class TOracleTCP
{
    public:
		static TOracleTCP& Instance()
		{
			static TOracleTCP singleton;
			return singleton;
		};

        TOracleTCP();
        ~TOracleTCP();

        void SendAndFetch(AnsiString data,AnsiString address,int portNumber);

        AnsiString SerializeOut(TPostRoomInquiry roomInquiry);
        AnsiString SerializeOut(TPostRequest postRequest);

        TRoomInquiryItem SerializeIn(AnsiString data, TRoomInquiryAnswer roomAnswer);
        TPostRequestAnswer SerializeIn(AnsiString data);

        OracleRequestType oracleRequestType;
};
#endif
