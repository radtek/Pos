//---------------------------------------------------------------------------

#ifndef MallH
#define MallH
//---------------------------------------------------------------------------

#include "MallExportSettings.h"
#include <list>

class TMall
{
private:
    int _mallId;
    UnicodeString _mallName;
    UnicodeString _isActive;
    std::list<TMallExportSettings> _mallSettings;

    void SetMallId(int mallID);
    void SetMallName(UnicodeString mallName);
    void SetActiveMall(UnicodeString isActive);
    void SetMallSettings(std::list<TMallExportSettings> mallSettings);

public:
    __property int MallId = {read = _mallId, write = SetMallId};
    __property UnicodeString MallName = {read = _mallName, write = SetMallName};
    __property UnicodeString IsActive = {read = _isActive, write = SetActiveMall};
    __property std::list<TMallExportSettings> MallSettings ={read = _mallSettings, write = SetMallSettings};
};

#endif
