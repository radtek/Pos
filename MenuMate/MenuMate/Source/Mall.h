//---------------------------------------------------------------------------

#ifndef MallH
#define MallH
//---------------------------------------------------------------------------

#include "MallExportSettings.h"
#include <list>
#include <vector>

class TMall
{
private:
    int _mallId;
    int _deviceKey;
    UnicodeString _mallName;
    UnicodeString _isActive;
    std::list<TMallExportSettings> _mallSettings;
    std::vector<UnicodeString> _fileNameList;

    void SetMallId(int mallID);
    void SetDeviceKey(int deviceKey);
    void SetMallName(UnicodeString mallName);
    void SetActiveMall(UnicodeString isActive);
    void SetMallSettings(std::list<TMallExportSettings> mallSettings);
    void SetFileNameList(std::vector<UnicodeString> fileNameList);

public:
    __property int MallId = {read = _mallId, write = SetMallId};
    __property int DeviceKey = {read = _deviceKey, write = SetDeviceKey};
    __property UnicodeString MallName = {read = _mallName, write = SetMallName};
    __property UnicodeString IsActive = {read = _isActive, write = SetActiveMall};
    __property std::list<TMallExportSettings> MallSettings ={read = _mallSettings, write = SetMallSettings};
    __property std::vector<UnicodeString> FileNameList ={read = _fileNameList, write = SetFileNameList};
};

#endif
