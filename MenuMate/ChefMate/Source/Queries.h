//---------------------------------------------------------------------------

#ifndef QueriesH
#define QueriesH

#include "NetMessageChefMate.h"
#include "MM_DBCore.h"
#include "KitchenMod.h"

int GetTotalDailyOrders(Database::TDBTransaction &DBTransaction);
TDateTime GetSalesAverage(Database::TDBTransaction &DBTransaction);
TDateTime GetMakeAverage(Database::TDBTransaction &DBTransaction);
TDateTime GetMakeLongest(Database::TDBTransaction &DBTransaction);
int CloseMakeStartTime(Database::TDBTransaction &DBTransaction,int TimeKey);
bool GetOutStandingOrder(Database::TDBTransaction &DBTransaction,TDateTime Minutes,TListChefDisplay *List);
bool OutStandingOrders(Database::TDBTransaction &DBTransaction,TDateTime Minutes);
void StreamToStringList(TMemoryStream *Stream, TStringList *Lines);
//---------------------------------------------------------------------------
#endif
