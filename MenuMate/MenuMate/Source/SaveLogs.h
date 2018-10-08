//---------------------------------------------------------------------------

#ifndef SaveLogsH
#define SaveLogsH
#include <classes.hpp>
#include <Memory>

//---------------------------------------------------------------------------
class TSaveLogs
{
private:
    static void WriteLogsToFile(AnsiString directoryName, AnsiString fileName, TStringList *List);
public:
    static void RecordFiscalLogs(TStringList* logList);
    static void RecordEFTPOSLogs(TStringList* logList);
};
#endif
