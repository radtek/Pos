//---------------------------------------------------------------------------

#ifndef EventsContactH
#define EventsContactH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <vector>

class TContactEvents;
class TMMContactInfo;

typedef void (__closure *TMMContactEvent)(TContactEvents *Sender,TMMContactInfo &ContactInfo);

class TContactEvents
{
	std::vector<TMMContactEvent> Events;
	public:
	void RegisterForEvent(TMMContactEvent Function);
	void DeregisterForEvent(TMMContactEvent Function);
	void Occured(TMMContactInfo &ContactInfo);
	UnicodeString ErrorMessage;
	UnicodeString ErrorTitle;
	bool HaltProceedingEvents;
	bool EventsFailed;
};

#endif
 