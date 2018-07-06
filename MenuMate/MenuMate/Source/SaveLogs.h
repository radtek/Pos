//---------------------------------------------------------------------------

#ifndef SaveLogsH
#define SaveLogsH
#include <classes.hpp>
#include <Memory>

//---------------------------------------------------------------------------
class TSaveLogs
{
public:
    void WriteLogsToFile(AnsiString directoryName, AnsiString fileName, TStringList *List);
};
#endif
