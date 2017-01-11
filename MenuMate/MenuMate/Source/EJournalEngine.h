//---------------------------------------------------------------------------

#ifndef EJournalEngineH
#define EJournalEngineH
#include <DateUtils.hpp>
#include "ReceiptManager.h"
//---------------------------------------------------------------------------
enum EJournalType {eZed,eZedX,eZEDReceiptX,eZedReceipt, eConsolidatedZed};

class TEJournalEngine
{
    public:
         TEJournalEngine();
	    ~TEJournalEngine();
        EJournalType CategorizeEJournal(TDateTime fromSessionDate,TDateTime toSessionDate);
        TMemoryStream* ExtractZedReport(TDateTime fromSessionDate,TDateTime toSessionDate, AnsiString deviceName);
        TMemoryStream* ExtractZedReceiptReport(TDateTime fromSessionDate,TDateTime toSessionDate, AnsiString deviceName);
        TMemoryStream* ExtractZedReceiptAndXReport(TDateTime fromSessionDate,TDateTime toSessionDate, AnsiString deviceName);
        TMemoryStream* ExtractZedAndXReport(TDateTime fromSessionDate,TDateTime toSessionDate, AnsiString deviceName);
        TMemoryStream* ExtractConsolidatedZedReport(TDateTime fromSessionDate,TDateTime toSessionDate, AnsiString deviceName);
        bool CheckZedDataExistsForConolidatedZed(TDateTime from, TDateTime to, AnsiString deviceName);
    private:
        EJournalType journalType;
        void CheckDataExist(TDateTime fromSessionDate,TDateTime toSessionDate);
        void GetZReport(TIBSQL *IBInternalQuery,TDateTime fromSessionDate,TDateTime toSessionDate, AnsiString deviceName);
        bool IsXReportAvailable(TIBSQL *IBInternalQuery, int z_key, AnsiString deviceName);
        void GetReceipt(TIBSQL *IBGetReciptQuery, int z_key, AnsiString deviceName);
        void DisplayXReport(TMemoryStream* XReceipt);
        void GetCurrentRunningReceipt(TIBSQL *IBGetCurrentRunningReciptQuery,AnsiString deviceName);
        bool IsCurrentReceiptAvailable(TIBSQL *IBInternalQuery, TDateTime toSessionDate, AnsiString deviceName);
        int GetStartTimeForConolidatedZed(TDateTime from, AnsiString deviceName);
        TDateTime GetEndTimeForConolidatedZed(TDateTime from, AnsiString deviceName, int z_key);


};
#endif
