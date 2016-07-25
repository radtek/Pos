//---------------------------------------------------------------------------
#include <vcl.h>
#include <algorithm>
#pragma hdrstop

#include "SystemEvents.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

void TSystemEvents::Occured()
{
	ErrorMessage = "";
	ErrorTitle = "";
	HaltProceedingEvents = false;
	EventsFailed = false;
	try
	{
      try
      {
         if(Asleep)
         {
            FiredWhileAsleep = true;
         }
         else if(Enabled)
         {  // Prevents recursion of events.
            if(false == RecursionProtected)
            {
               RecursionProtected = true;
               for (unsigned int i = 0; i < Events.size() ; i++)
               {
                  if(!HaltProceedingEvents)
                  {
                     Events[i](this);
                  }
               }
            }
         }
      }
      __finally
      {
         RecursionProtected = false;
      }
	}
	catch(Exception &E)
	{
		ErrorMessage = E.Message;
      EventsFailed = true;
	}
}

void TSystemEvents::RegisterForEvent(TMMEvent Function)
{
	std::vector<TMMEvent>::iterator itFunction = std::find(Events.begin(), Events.end(), Function);
	if(itFunction == Events.end())
	{
   	Events.push_back(Function);
   }
}

void TSystemEvents::DeregisterForEvent(TMMEvent Function)
{
	std::vector<TMMEvent>::iterator RemoveFunction = std::find(Events.begin(), Events.end(), Function);
	if(RemoveFunction != Events.end())
	{
		Events.erase(RemoveFunction);
	}
}

TSystemEvents::TSystemEvents()
{
   Enabled = true;
   Asleep = false;
   FiredWhileAsleep = false;
   RecursionProtected = false;
}

void TSystemEvents::Enable()
{
   Enabled = true;
}
void TSystemEvents::Disable()
{
   Enabled = false;
}

int TSystemEvents::EventCount()
{
   return Events.size();
}

void TSystemEvents::Sleep()
{
   FiredWhileAsleep = false;
   Asleep = true;
}

void TSystemEvents::Wakeup()
{
   Asleep = false;
   if(FiredWhileAsleep)
   {
      Occured();
   }
}

std::vector<TMMEvent>
TSystemEvents::swap_handlers(const std::vector<TMMEvent> &handlers)
{
	std::vector<TMMEvent> temp;

	Sleep();
   temp = Events, Events = handlers;
	Wakeup();

	return temp;
}

void
TSystemEvents::set_handlers(const std::vector<TMMEvent> &handlers)
{
   Sleep();
	Events = handlers;
	Wakeup();
}


void TSystemEvents::Occured(long itemKey, long itemSizeKey , AnsiString qtyValue , AnsiString billValue)
{
	ErrorMessage = "";
	ErrorTitle = "";
	HaltProceedingEvents = false;
	EventsFailed = false;
	try
	{
      try
      {
         if(Asleep)
         {
            FiredWhileAsleep = true;
         }
         else if(Enabled)
         {  // Prevents recursion of events.
            if(false == RecursionProtected)
            {
               RecursionProtected = true;
               for (unsigned int i = 0; i < Events.size() ; i++)
               {
                  if(!HaltProceedingEvents)
                  {
                     ItemKey = itemKey;
                     ItemSizeKey = itemSizeKey;
                     QtyValue = qtyValue;
                     BillValue = billValue;
                     Events[i](this);
                  }
               }
            }
         }
      }
      __finally
      {
         RecursionProtected = false;
      }
	}
	catch(Exception &E)
	{
		ErrorMessage = E.Message;
      EventsFailed = true;
	}
}

