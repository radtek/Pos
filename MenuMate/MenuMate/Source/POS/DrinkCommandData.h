
//---------------------------------------------------------------------------

#ifndef DrinkCommandDataH
#define DrinkCommandDataH
#include "PaymentTransaction.h"
#include "SystemEvents.h"

struct TOpenSessionDetails
{
  public:
      AnsiString Card_Id;
      AnsiString Session_Id;
};
class TDrinkCommandData
{
    private:
         TDrinkCommandData();
	    //~TDrinkCommandData();
    public:
    	static TDrinkCommandData& Instance() {
		    static TDrinkCommandData singleton;
		    return singleton;
	    }
//
         int GenerateSessionId();
         void InsertDataToTable (AnsiString memberNo , int session_uuid);
         int GenerateLineItemId();
         int FetchSession(AnsiString memNo);
         Currency GetPoints(TPaymentTransaction PointsTransaction);
         AnsiString ReadResponse(AnsiString data , int type);
         AnsiString CheckForOpenSession(AnsiString memberNo) ;
         void UpdateClose(AnsiString session_uuid);
         void UpdateLineItemID(UnicodeString session_uuid , int line_item);
         void getItemFromThirdPartyCode(AnsiString thirdPartyCode ,AnsiString qtyValue , AnsiString billValue,bool lastProduct);
         void fetchItemKey(int thirdPartyCodeKey , AnsiString qtyValue , AnsiString billValue , AnsiString thirdPartyCode,bool lastProduct);
         void getCompleteItem(long itemKey, long itemSizeKey , AnsiString qtyValue , AnsiString billValue , bool lastProduct);
         AnsiString FetchTimeStamp(AnsiString session_uuid);
         void UpdateTimeStamp(AnsiString session_id , AnsiString time_Stamp);
         void UpdateTimeStampToNull(AnsiString memberDetails);
         int GetGroupKey(int contactKey);
         AnsiString GetCardIdFromMemberNo(AnsiString memberNo);
         AnsiString ItemDetails;
         TSystemEvents ItemDetailsReceived;
         TSystemEvents OrderComplete;
         std::vector<TOpenSessionDetails> GetAllDCOpenSessions();
         std::vector<TOpenSessionDetails> CollectOpenSessionDetails;

//
     private:
         AnsiString session_uuid;
         int val;
};
//---------------------------------------------------------------------------
#endif
