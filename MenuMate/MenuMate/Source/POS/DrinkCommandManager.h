//---------------------------------------------------------------------------

#ifndef DrinkCommandManagerH
#define DrinkCommandManagerH
#include "PaymentTransaction.h"

enum RequestType {eStartSession=1,eSendCash,eSessionSummary,eCloseSession,eSessionBill,eProductList};

class TDrinkCommandManager
{
    public:
         TDrinkCommandManager();
	    ~TDrinkCommandManager();
//
         UnicodeString StartSession(AnsiString path , int port , AnsiString memberNo , int groupKey);
         void SendCashInfo(AnsiString path , int port , AnsiString memberPoints , UnicodeString session_uuid);
         void CloseSession(AnsiString path , int port ,AnsiString session_id);
         void GenerateSessionId();
         void AskDCSummary(AnsiString path , int port , AnsiString sessionId) ;
         Currency GetPoints(TPaymentTransaction PointsTransaction);
         void SendRequestForProductList(AnsiString path , int port);
         AnsiString PrepareSessionRequest(AnsiString memberNo, int groupKey , AnsiString temp_data,AnsiString session_uuid);

     private:
         int val;
};
//---------------------------------------------------------------------------
#endif
