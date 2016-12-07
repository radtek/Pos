//---------------------------------------------------------------------------

#ifndef EJournalEngineH
#define EJournalEngineH
#include <DateUtils.hpp>
//---------------------------------------------------------------------------
class TEJournalEngine
{
    public:
         TEJournalEngine();
	    ~TEJournalEngine();
        void CategorizeEJournal(TDateTime fromSessionDate,TDateTime toSessionDate);
};
#endif
