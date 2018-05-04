//---------------------------------------------------------------------------

#ifndef DocketLogsH
#define DocketLogsH
#include <Classes.hpp>
#include <Memory>
//---------------------------------------------------------------------------
class TDocketLogs
{
    public:
        static void SaveLogs(std::auto_ptr<TStringList> List);
        static std::auto_ptr<TStringList> AddItemsToList(TList *Orders);
    private:
        static UnicodeString GetFileName();
};
#endif
