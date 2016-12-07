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
        bool CheckDataExist(TDateTime fromSessionDate,TDateTime toSessionDate);
    private:
        void CategorizeEJournal(TDateTime fromSessionDate,TDateTime toSessionDate);
};
#endif
