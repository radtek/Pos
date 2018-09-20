//---------------------------------------------------------------------------

#ifndef SignalRUtilityH
#define SignalRUtilityH
#include <System.hpp>
//---------------------------------------------------------------------------
class TSignalRUtility
{
public:
    bool LoadSignalRUtility();
    bool UnloadSignalRUtility();
private:
    bool IsSignalRRunning();
    void MakeOnlineOrderingSeedFile();
    AnsiString GetFileName();
    AnsiString GetUtilityName();
    AnsiString GetSyndicateCode();
};
#endif
