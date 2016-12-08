//---------------------------------------------------------------------------

#ifndef EJournalEngineH
#define EJournalEngineH
#include <DateUtils.hpp>
#include "ReceiptManager.h"
//---------------------------------------------------------------------------
enum EJournalType {eZed,eZedX,eZEDReceiptX,eZedReceipt};

class TEJournalEngine
{
    public:
         TEJournalEngine();
	    ~TEJournalEngine();
        //bool CheckDataExist(TDateTime fromSessionDate,TDateTime toSessionDate);
        EJournalType CategorizeEJournal(TDateTime fromSessionDate,TDateTime toSessionDate);
        TMemoryStream* ExtractZedReport(TDateTime fromSessionDate,TDateTime toSessionDate);
        TMemoryStream* ExtractZedReceiptReport(TDateTime fromSessionDate,TDateTime toSessionDate);
        TMemoryStream* ExtractZedReceiptAndXReport(TDateTime fromSessionDate,TDateTime toSessionDate);
    private:
        EJournalType journalType;
        void CheckDataExist(TDateTime fromSessionDate,TDateTime toSessionDate);
        void GetZReport(TIBSQL *IBInternalQuery,TDateTime fromSessionDate,TDateTime toSessionDate);
        bool IsXReportGenerate(TIBSQL *IBInternalQuery, int z_key);
        void GetReceipt(TIBSQL *IBGetReciptQuery, int z_key);
        void DisplayXReport(TMemoryStream* XReceipt);
        void GetCurrentRunningReceipt(TIBSQL *IBGetCurrentRunningReciptQuery);
};
#endif
