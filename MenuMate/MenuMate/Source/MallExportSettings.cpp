//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TMallExportSettings::SetMallExportSettingMappingId(int mallExportSettingMappingId)
{
    _mallExportSettingMappingId = mallExportSettingMappingId;
}

//---------------------------------------------------------------------------

void TMallExportSettings::SetMallExportSettingId(int mallExportSettingId)
{
    _mallExportSettingId = mallExportSettingId;
}

//---------------------------------------------------------------------------

void TMallExportSettings::SetName(UnicodeString name)
{
    _name = name;
}

//---------------------------------------------------------------------------

void TMallExportSettings::SetControlName(UnicodeString controlName)
{
    _controlName = controlName;
}

//---------------------------------------------------------------------------

void TMallExportSettings::SetValue(UnicodeString value)
{
    _value = value;
}

//---------------------------------------------------------------------------

void TMallExportSettings::SetValueType(UnicodeString valueType)
{
    _valueType = valueType;
}

//---------------------------------------------------------------------------
