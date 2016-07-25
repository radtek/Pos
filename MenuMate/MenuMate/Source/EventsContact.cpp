//---------------------------------------------------------------------------


#pragma hdrstop

#include "EventsContact.h"
#include "Contact.h"
#include <algorithm>
//---------------------------------------------------------------------------

#pragma package(smart_init)


void TContactEvents::Occured(TMMContactInfo &ContactInfo)
{
	ErrorMessage = "";
	ErrorTitle = "";
	HaltProceedingEvents = false;
	EventsFailed = false;
	try
	{
		for (unsigned int i = 0; i < Events.size() ; i++)
		{
			if(!HaltProceedingEvents)
			{
				Events[i](this,ContactInfo);
			}
		}
	}
	catch(Exception &E)
	{
		ErrorMessage = E.Message;
      EventsFailed = true;
      throw;
	}
}

void TContactEvents::RegisterForEvent(TMMContactEvent Function)
{
	std::vector<TMMContactEvent>::iterator itFunction = std::find(Events.begin(), Events.end(), Function);
	if(itFunction == Events.end())
	{
   	Events.push_back(Function);
   }
}

void TContactEvents::DeregisterForEvent(TMMContactEvent Function)
{
	std::vector<TMMContactEvent>::iterator RemoveFunction = std::find(Events.begin(), Events.end(), Function);
	if(RemoveFunction != Events.end())
	{
		Events.erase(RemoveFunction);
	}
}