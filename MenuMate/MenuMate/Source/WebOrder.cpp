//---------------------------------------------------------------------------


#pragma hdrstop

#include "WebOrder.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TWebOrder::TWebOrder()
{
	Clear();
}


TWebOrder::Clear()
{
   GUID = "";
   StoreName = "";
   DateOrdered = 0;
   DateExpected = 0;
   WebKey = 0;
   TabKey = 0;
   TimeKey = 0;
   ContactKey = 0;
   PrePaid = 0;
}

bool TWebOrder::Valid()
{
   return WebKey != 0;
}
