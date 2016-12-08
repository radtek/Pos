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
        EJournalType CategorizeEJournal(TDateTime fromSessionDate,TDateTime toSessionDate);
        TMemoryStream* ExtractZedReport(TDateTime fromSessionDate,TDateTime toSessionDate, std::vector<TMemoryStream*> &CollectReceipts);
        TMemoryStream* ExtractZedReceiptReport(TDateTime fromSessionDate,TDateTime toSessionDate, std::vector<TMemoryStream*> &CollectReceipts);
        TMemoryStream* ExtractZedReceiptAndXReport(TDateTime fromSessionDate,TDateTime toSessionDate, std::vector<TMemoryStream*> &CollectReceipts);
        TMemoryStream* ExtractZedAndXReport(TDateTime fromSessionDate,TDateTime toSessionDate, std::vector<TMemoryStream*> &CollectReceipts);
    private:
        EJournalType journalType;
        void CheckDataExist(TDateTime fromSessionDate,TDateTime toSessionDate);
        void GetZReport(TIBSQL *IBInternalQuery,TDateTime fromSessionDate,TDateTime toSessionDate);
        bool IsXReportAvailable(TIBSQL *IBInternalQuery, int z_key);
        void GetReceipt(TIBSQL *IBGetReciptQuery, int z_key);
        void DisplayXReport(TMemoryStream* XReceipt, TMemoryStream* CollectXReceipt);
        void GetCurrentRunningReceipt(TIBSQL *IBGetCurrentRunningReciptQuery);
};
#endif
