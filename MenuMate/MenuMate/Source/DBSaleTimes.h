//---------------------------------------------------------------------------

#ifndef DBSaleTimesH
#define DBSaleTimesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include "MM_DBCore.h"

class TDBSaleTimes
{
   static int GetTimeKey(Database::TDBTransaction &DBTransaction);
   public :

   static int OpenSaleStartTime(Database::TDBTransaction &DBTransaction,int CurrentTimeKey);
   static void CloseSaleStartTime(Database::TDBTransaction &DBTransaction,int TimeKey); // Opens a Make Time.
   static void CloseMakeStartTime(Database::TDBTransaction &DBTransaction,int TimeKey);
   static void VoidSaleTime(Database::TDBTransaction &DBTransaction,int TimeKey);

	static TDateTime GetSalesAverage(Database::TDBTransaction &DBTransaction);
   static TDateTime GetMakeAverage(Database::TDBTransaction &DBTransaction);
   static double GetAverageWaitTimeMins(Database::TDBTransaction &DBTransaction);

	static bool GetLongestSaleTrans(Database::TDBTransaction &DBTransaction,int Offest,UnicodeString &Doc,UnicodeString &Opr,UnicodeString &Qty,
   								 UnicodeString &Dur,UnicodeString &Avg,UnicodeString &Val);
	static void GetLongestMakeTrans(Database::TDBTransaction &DBTransaction,int Offest,UnicodeString &Doc,UnicodeString &Opr,UnicodeString &Qty,
									 UnicodeString &Dur,UnicodeString &Avg,UnicodeString &Val);

	static TDateTime GetSaleStartTimeFromTimeKey(Database::TDBTransaction &DBTransaction, int inTimeKey);
    static TDateTime GetSalesAverage(Database::TDBTransaction &DBTransaction, TDateTime &startTime, TDateTime &endTime);
    static TDateTime GetMakeAverage(Database::TDBTransaction &DBTransaction, TDateTime &startTime, TDateTime &endTime);
    static bool GetLongestSaleTransForConsolidatedZed(Database::TDBTransaction &DBTransaction,int Offest,UnicodeString &Doc,UnicodeString &Opr,
				UnicodeString &Qty,UnicodeString &Dur,UnicodeString &Avg,UnicodeString &Val, TDateTime &startTime, TDateTime &endTime);
    static void GetLongestMakeTransForConsolidatedZed(Database::TDBTransaction &DBTransaction,int Offest,UnicodeString &Doc,UnicodeString &Opr,
				UnicodeString &Qty,UnicodeString &Dur,UnicodeString &Avg,UnicodeString &Val, TDateTime &startTime, TDateTime &endTime);
};
#endif
