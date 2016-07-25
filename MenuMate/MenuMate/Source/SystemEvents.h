//---------------------------------------------------------------------------

#ifndef SystemEventsH
#define SystemEventsH

#include <System.hpp>
#include <vector>

//---------------------------------------------------------------------------
class TSystemEvents;

typedef void (__closure *TMMEvent)(TSystemEvents *Sender);

class TSystemEvents
{
   private:
	std::vector<TMMEvent> Events;
   bool RecursionProtected;
   bool Enabled;
   bool Asleep;
   bool FiredWhileAsleep;
	public:
	void RegisterForEvent(TMMEvent Function);
	void DeregisterForEvent(TMMEvent Function);
	std::vector<TMMEvent> swap_handlers(
	  const std::vector<TMMEvent> &handlers);
	void set_handlers(const std::vector<TMMEvent> &handlers);
	void Occured();
    void Occured(long itemKey, long itemSizeKey , AnsiString qtyValue , AnsiString billValue);
	UnicodeString ErrorMessage;
	UnicodeString ErrorTitle;
   long ItemKey;
   long ItemSizeKey;
   AnsiString QtyValue ;
   AnsiString BillValue;
	bool HaltProceedingEvents;
	bool EventsFailed;
	void Enable();
	void Disable();
   void Sleep();
   void Wakeup();
   int EventCount();
   TSystemEvents();
};

#endif

