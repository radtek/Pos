//---------------------------------------------------------------------------

#ifndef MallExportSettingsH
#define MallExportSettingsH
//---------------------------------------------------------------------------

#include <System.hpp>

class TMallExportSettings
{
private:
    int _mallExportSettingMappingId;
    int _mallExportSettingId;
    UnicodeString _name;
    UnicodeString _controlName;
    UnicodeString _value;
    UnicodeString _valueType;

    void SetMallExportSettingMappingId(int mallExportSettingMappingId);
    void SetMallExportSettingId(int mallExportSettingId);
    void SetName(UnicodeString name);
    void SetControlName(UnicodeString controlName);
    void SetValue(UnicodeString value);
    void SetValueType(UnicodeString valueType);

public:
    __property int MallExportSettingMappingId = {read = _mallExportSettingMappingId, write = SetMallExportSettingMappingId};
    __property int MallExportSettingId = {read = _mallExportSettingId, write = SetMallExportSettingId};
    __property UnicodeString Name = {read = _name, write = SetName};
    __property UnicodeString ControlName = {read = _controlName, write = SetControlName};
    __property UnicodeString Value = {read = _value, write = SetValue};
    __property UnicodeString ValueType = {read = _valueType, write = SetValueType};
};
#endif
