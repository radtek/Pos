//---------------------------------------------------------------------------


#pragma hdrstop

#include "DrinkCommandManager.h"
#include <system.hpp>
#include "TCPStreamPrepare.h"

//---------------------------------------------------------------------------

#include <math.h>
#include "DeviceRealTerminal.h"
#include "TcpStreamPrepare.h"
#include "DrinkCommandData.h"
#include "enumContacts.h"
//------------------------------------------------------------------

#pragma package(smart_init)

TDrinkCommandManager::TDrinkCommandManager()
{
}
//---------------------------------------------------------------------------

TDrinkCommandManager::~TDrinkCommandManager()
{
}
//---------------------------------------------------------------------------

UnicodeString TDrinkCommandManager::StartSession(AnsiString path , int port , AnsiString memberNo , int groupKey)
{
    int session_id = TDrinkCommandData::Instance().GenerateSessionId() ;
    char* char_id = new char[10];
    itoa( session_id , char_id ,10);
    UnicodeString session_uuid = char_id;
    AnsiString temp_data = "";

    if(IsNumeric(memberNo))
    {
        __int64 i = _atoi64(memberNo.c_str());
        UnicodeString hex_token = IntToHex(i, 2);
        AnsiString hex_ansi = hex_token;
        if (hex_ansi.Length() >= 2)
        {
            if (hex_ansi.Length() % 2 == 0)
            {
                for(int i = hex_ansi.Length()-1 ; i >= 1 ; i = i-2 )
                {
                    temp_data += hex_ansi.SubString( i,2 );
                }
            }
            else
            {
                for(int i = hex_ansi.Length()-1 ; i >= 1 ; i = i-2 )
                {
                    temp_data += hex_ansi.SubString( i,2 );
                }
                temp_data += hex_ansi.SubString( 1,1 );
            }
        }
    }

    int requestType = eStartSession;
    UnicodeString str1 = "";
    UnicodeString userType = "";
    UnicodeString lineitemid = "";


    str1 = PrepareSessionRequest( memberNo ,  groupKey , temp_data , session_uuid);

    std::auto_ptr<TTCPStreamPrepare> streamPrepare(
                                                new TTCPStreamPrepare(
                                                    path,
                                                    port ) );
                                                        // Call for TCP connection

    streamPrepare->Connect( str1 , requestType,session_uuid , memberNo , lineitemid);

    return session_uuid;
}


AnsiString TDrinkCommandManager::PrepareSessionRequest(AnsiString memberNo, int groupKey , AnsiString temp_data , AnsiString session_uuid)
{
    AnsiString userType = "";
    AnsiString str1 = "";

    switch(groupKey)
    {
       case eDCNoGroup:
           userType = "NoGROUP";
           break;

       case eDCManager:
           userType = "MANAGER";
           break;

       case eDCStaff:
           userType = "STAFF";
           break;
    }

    if((userType == "MANAGER") || (userType == "STAFF"))
    {
        UnicodeString strtId =",";
        strtId += session_uuid;
        strtId += ",";
        strtId += userType;
        strtId += ",";
        strtId +=temp_data ;

        str1 = "sessions.create\n";                                           //  To Start Session
        str1 += "sessions\n";
        str1 += ",session_uuid,user_type,session_token\n";
        str1 += strtId;
        str1 += "\n";
        str1 += ".\n";

    }
    else
    {

		UnicodeString strtId =",";
        strtId += session_uuid;
        strtId += ",";
        strtId +=temp_data ;

        str1 = "sessions.create\n";                                           //  To Start Session
        str1 += "sessions\n";
        str1 += ",session_uuid,session_token\n";
        str1 += strtId;
        str1 += "\n";
        str1 += ".\n";
	
    }
    return str1;

}
void TDrinkCommandManager::SendCashInfo(AnsiString path , int port , AnsiString memberPoints , UnicodeString session_uuid)
{
    int memPts = atoi(memberPoints.c_str());
    int line_item = TDrinkCommandData::Instance().GenerateLineItemId();
    char* item_id = new char[10];
    itoa (line_item , item_id , 10);
    UnicodeString line_item_id = item_id;

    UnicodeString cashInfo = "line_items.add_cash\n";
    cashInfo += "line_items\n";
    cashInfo += ",line_item_id,session_uuid,money_units\n";
    cashInfo += ",";
    cashInfo += line_item_id;
    cashInfo +=  ",";
    cashInfo += session_uuid;
    cashInfo += ",";
    cashInfo += memPts;
    cashInfo += "\n";
    cashInfo += ".\n";

    int requestType = eSendCash;
    AnsiString memberNo = "";
    std::auto_ptr<TTCPStreamPrepare> streamPrepare(
												new TTCPStreamPrepare(
													path,
													port ) );
    streamPrepare->Connect( cashInfo , requestType , session_uuid , memberNo , line_item_id);

}
void TDrinkCommandManager::CloseSession(AnsiString path , int port , AnsiString session_id)
{
        UnicodeString session_uuid = session_id;
        UnicodeString str2 = "sessions.close\n";
        str2 += "sessions\n";
        str2 += ",session_uuid\n";
        str2 += ",";
        str2 += session_uuid;
        str2 += "\n" ;
        str2 += ".\n";

    int requestType = eCloseSession;
    AnsiString memberNo = "";
    UnicodeString lineitemid = "";
    std::auto_ptr<TTCPStreamPrepare> streamPrepare(
												new TTCPStreamPrepare(
													path,
													port ) );

    streamPrepare->Connect( str2 , requestType , session_uuid , memberNo , lineitemid);

}
void TDrinkCommandManager::AskDCSummary(AnsiString path , int port , AnsiString session_id)   // gets details of shooping done
{
    UnicodeString str2 = "line_items.list\n";
    str2 += ",session_uuid,";
    str2 += session_id;
    str2 += "\n" ;
    //str2 += ",include_zero_pours,y\n";
    str2 += ".\n";
    int requestType = eSessionSummary;
    UnicodeString session_uuid = session_id;
    AnsiString memberNo = "";
    UnicodeString lineitemid = "";

    std::auto_ptr<TTCPStreamPrepare> streamPrepare(
                                                    new TTCPStreamPrepare(
                                                     path,
                                                    port ) );
    streamPrepare->Connect( str2 , requestType, session_uuid , memberNo , lineitemid);
}
void TDrinkCommandManager::SendRequestForProductList(AnsiString path , int port)
{
   UnicodeString str2 = "products.list\n";
   str2 += ".\n";
   int requestType = eProductList;
   UnicodeString session_uuid = "";
   AnsiString memberNo = "";
   UnicodeString lineitemid = "";
   std::auto_ptr<TTCPStreamPrepare> streamPrepare(
                                                    new TTCPStreamPrepare(
                                                     path,
                                                    port ) );
   streamPrepare->Connect( str2 , requestType, session_uuid , memberNo , lineitemid);
}



