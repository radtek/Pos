//---------------------------------------------------------------------------
#ifndef MM_RegistrySettingsH
#define MM_RegistrySettingsH
//---------------------------------------------------------------------------
#include <system.hpp>
#include <registry.hpp>
#include <memory>
//---------------------------------------------------------------------------
//const AnsiString KeyName = "Software\\MenuMate";
//---------------------------------------------------------------------------
class TRegistrySettings
{
public:
	TRegistrySettings(AnsiString KeyName, HKEY RootKey=HKEY_LOCAL_MACHINE) : FKeyName(KeyName), FRootKey(RootKey) {}

	inline bool GetKeys(TStrings *Keys);

	inline template <typename T> bool Get(const AnsiString &SettingName, T &Value);
	inline template <typename T> bool Get(const AnsiString &SettingName, T &Value, const T &Default);
	inline template <typename T> bool Set(const AnsiString &SettingName, const T &Value);
	
	inline bool DeleteKey();
	inline bool Delete(const AnsiString &SettingName);
private:
	const AnsiString FKeyName;
	const HKEY FRootKey;
	inline template <typename T> T Read(TRegistry *Registry, const AnsiString &SettingName);
	inline template <typename T> void Write(TRegistry *Registry, const AnsiString &SettingName, const T &Value);
};
//---------------------------------------------------------------------------
bool TRegistrySettings::GetKeys(TStrings *Keys)
{
	bool Success = false;
	std::auto_ptr<TRegistry>Registry(new TRegistry);

	Registry->RootKey = FRootKey;
	if (Registry->OpenKeyReadOnly(FKeyName))
	{
		Registry->GetKeyNames(Keys);
		Registry->CloseKey();

		Success = true;
	}
	return Success;
}
//---------------------------------------------------------------------------
bool TRegistrySettings::DeleteKey()
{
	bool Success = false;
	std::auto_ptr<TRegistry>Registry(new TRegistry);

	Registry->RootKey = FRootKey;
	if (Registry->DeleteKey(FKeyName))
	{
		Success = true;
		Registry->CloseKey();
	}
	return Success;
}
//---------------------------------------------------------------------------
bool TRegistrySettings::Delete(const AnsiString &SettingName)
{
	bool Success = false;
	std::auto_ptr<TRegistry>Registry(new TRegistry);

	Registry->RootKey = FRootKey;
	if (Registry->OpenKey(FKeyName, false))
	{
		if (Registry->ValueExists(SettingName))
		{
			Registry->DeleteValue(SettingName);
			Success = true;
		}
		Registry->CloseKey();
	}
	return Success;
}
//---------------------------------------------------------------------------
inline template <typename T>
bool TRegistrySettings::Get(const AnsiString &SettingName, T &Value)
{
	bool Success = false;
	std::auto_ptr<TRegistry>Registry(new TRegistry);

	Registry->RootKey = FRootKey;
	if (Registry->OpenKey(FKeyName, false))
	{
		if (Registry->ValueExists(SettingName))
		{
			Value = Read<T>(Registry.get(), SettingName);
			Success = true;
		}
		Registry->CloseKey();
	}
	return Success;
}
//---------------------------------------------------------------------------
inline template <typename T>
bool TRegistrySettings::Get(const AnsiString &SettingName, T &Value, const T &Default)
{
	bool Success = false;
	std::auto_ptr<TRegistry>Registry(new TRegistry);

	Registry->RootKey = FRootKey;
	if (Registry->OpenKey(FKeyName, true))
	{
		if (Registry->ValueExists(SettingName))
		{
			Value = Read<T>(Registry.get(), SettingName);
		}
		else
		{
			Write<T>(Registry.get(), SettingName, Default);
		}
		Registry->CloseKey();
		Success = true;
	}
	return Success;
}
//---------------------------------------------------------------------------
inline template <typename T>
bool TRegistrySettings::Set(const AnsiString &SettingName, const T &Value)
{
	bool Success = false;
	std::auto_ptr<TRegistry>Registry(new TRegistry);

	Registry->RootKey = FRootKey;
	if (Registry->OpenKey(FKeyName, true))
	{
//		if (Registry->ValueExists(SettingName))
//		{
			Write<T>(Registry.get(), SettingName, Value);
			Registry->CloseKey();
			Success = true;
//		}
	}
	return Success;
}
//---------------------------------------------------------------------------
inline template <>
AnsiString TRegistrySettings::Read<AnsiString>(TRegistry *Registry, const AnsiString &SettingName)
{
	return Registry->ReadString(SettingName);
}
//---------------------------------------------------------------------------
inline template <>
int TRegistrySettings::Read<int>(TRegistry *Registry, const AnsiString &SettingName)
{
	return Registry->ReadInteger(SettingName);
}
//---------------------------------------------------------------------------
inline template <>
TDateTime TRegistrySettings::Read<TDateTime>(TRegistry *Registry, const AnsiString &SettingName)
{
	return Registry->ReadDateTime(SettingName);
}
//---------------------------------------------------------------------------
inline template <>
bool TRegistrySettings::Read<bool>(TRegistry *Registry, const AnsiString &SettingName)
{
	return Registry->ReadBool(SettingName);
}
//---------------------------------------------------------------------------
inline template <>
Currency TRegistrySettings::Read<Currency>(TRegistry *Registry, const AnsiString &SettingName)
{
	return Registry->ReadCurrency(SettingName);
}
//---------------------------------------------------------------------------
inline template <>
double TRegistrySettings::Read<double>(TRegistry *Registry, const AnsiString &SettingName)
{
	return Registry->ReadFloat(SettingName);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline template <>
void TRegistrySettings::Write<AnsiString>(TRegistry *Registry, const AnsiString &SettingName, const AnsiString &Value)
{
	Registry->WriteString(SettingName, Value);
}
//---------------------------------------------------------------------------
inline template <>
void TRegistrySettings::Write<int>(TRegistry *Registry, const AnsiString &SettingName, const int &Value)
{
	Registry->WriteInteger(SettingName, Value);
}
//---------------------------------------------------------------------------
inline template <>
void TRegistrySettings::Write<TDateTime>(TRegistry *Registry, const AnsiString &SettingName, const TDateTime &Value)
{
	Registry->WriteDateTime(SettingName, Value);
}
//---------------------------------------------------------------------------
inline template <>
void TRegistrySettings::Write<bool>(TRegistry *Registry, const AnsiString &SettingName, const bool &Value)
{
	Registry->WriteBool(SettingName, Value);
}
//---------------------------------------------------------------------------
inline template <>
void TRegistrySettings::Write<Currency>(TRegistry *Registry, const AnsiString &SettingName, const Currency &Value)
{
	Registry->WriteCurrency(SettingName, Value);
}
//---------------------------------------------------------------------------
inline template <>
void TRegistrySettings::Write<double>(TRegistry *Registry, const AnsiString &SettingName, const double &Value)
{
	Registry->WriteFloat(SettingName, Value);
}
//---------------------------------------------------------------------------
#endif
