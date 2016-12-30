//---------------------------------------------------------------------------

#ifndef ManagerEJournalH
#define ManagerEJournalH
#include <system.hpp>
#include <memory>
#include "GlobalSettings.h"
//---------------------------------------------------------------------------
class TManagerEJournal
{
    public:
         TManagerEJournal();
	    ~TManagerEJournal();
        void TriggerEJournal(bool isConsolidatedZed = false);
};
#endif
