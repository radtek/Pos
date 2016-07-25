//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Registry.hpp"
#include "MMRegistry.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Checks for Keys or values existance
//---------------------------------------------------------------------------
bool RegistryKeyExists(AnsiString KeyName)
{
	bool RetVal = false;
	TRegistry *Registry = new TRegistry;
	try
	{
   	Registry->RootKey = HKEY_LOCAL_MACHINE;
		RetVal = Registry->KeyExists(KeyName);
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryValueExists(AnsiString KeyName, AnsiString ValueName)
{
	bool RetVal = false;
	TRegistry *Registry = new TRegistry;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
		RetVal = Registry->OpenKeyReadOnly(KeyName);
		if (RetVal == true)
      {
      	RetVal = Registry->ValueExists(ValueName);
      }
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryGetKeys(AnsiString KeyName, TStrings* Strings)
{
	bool RetVal = false;
	TRegistry *Registry = new TRegistry;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
		RetVal = Registry->OpenKeyReadOnly(KeyName);
		if (RetVal)
		{
			Registry->GetKeyNames(Strings);
      }
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
//	Registry write routines
//---------------------------------------------------------------------------
bool CreateKey(AnsiString KeyName)
{
	bool RetVal = false;
	TRegistry *Registry = new TRegistry;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
		if (Registry->OpenKey(KeyName, true))
		{
			RetVal = true;
		}
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryWrite(AnsiString KeyName, AnsiString ValueName, int Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
		if (Registry->OpenKey(KeyName, true))
		{
			Registry->WriteInteger(ValueName, Value);
			RetVal = true;
		}
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryWrite(AnsiString KeyName, AnsiString ValueName, TDateTime Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, true))
      {
			Registry->WriteDateTime(ValueName, Value);
      	RetVal = true;
      }
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryWrite(AnsiString KeyName, AnsiString ValueName, AnsiString Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, true))
      {
			Registry->WriteString(ValueName, Value);
      	RetVal = true;
      }
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryWrite(AnsiString KeyName, AnsiString ValueName, char *Value)
{
   return RegistryWrite(KeyName, ValueName, AnsiString(Value));
}
//---------------------------------------------------------------------------
bool RegistryWrite(AnsiString KeyName, AnsiString ValueName, bool Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, true))
      {
			Registry->WriteBool(ValueName, Value);
      	RetVal = true;
      }
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryWrite(AnsiString KeyName, AnsiString ValueName, UINT Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, true))
      {
			Registry->WriteInteger(ValueName, Value);
      	RetVal = true;
      }
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
// Registry read routines
//---------------------------------------------------------------------------
bool RegistryRead(AnsiString KeyName, AnsiString ValueName, int &Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, false))
      {
			if (Registry->ValueExists(ValueName))
         {
				Value = Registry->ReadInteger(ValueName);
	      	RetVal = true;
         }
         else
         {
	      	RetVal = false;
         }
      }
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryRead(AnsiString KeyName, AnsiString ValueName, TDateTime &Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, false))
      {
			if (Registry->ValueExists(ValueName))
         {
				Value = Registry->ReadDateTime(ValueName);
      		RetVal = true;
         }
         else
         {
	      	RetVal = false;
         }
      }
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryRead(AnsiString KeyName, AnsiString ValueName, AnsiString &Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, false))
      {
			if (Registry->ValueExists(ValueName))
         {
				Value = Registry->ReadString(ValueName);
      		RetVal = true;
         }
         else
         {
	      	RetVal = false;
         }
      }
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryRead(AnsiString KeyName, AnsiString ValueName, bool &Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, false))
      {
			if (Registry->ValueExists(ValueName))
         {
				Value = Registry->ReadBool(ValueName);
      		RetVal = true;
         }
         else
         {
      		RetVal = false;
         }
      }
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryRead(AnsiString KeyName, AnsiString ValueName, UINT &Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, false))
      {
			if (Registry->ValueExists(ValueName))
         {
         	Value = Registry->ReadInteger(ValueName);
      		RetVal = true;
         }
         else
         {
      		RetVal = false;
         }
      }
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryDelete(AnsiString KeyName, AnsiString ValueName)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, false))
      {
			if (Registry->ValueExists(ValueName))
         {
				RetVal = Registry->DeleteValue(ValueName);
			}
         else
         {
      		RetVal = false;
         }
      }
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryDelete(AnsiString KeyName)
{
	bool RetVal = false;
	TRegistry *Registry = new TRegistry;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;

		TStringList *Keys = new TStringList;
		try
		{
			Registry->GetKeyNames(Keys);
			for (int i=0; i<Keys->Count; i++)
			{
				RegistryDelete(KeyName + "\\" + Keys->Strings[i]);
			}
			RetVal = Registry->DeleteKey(KeyName);
		}
		__finally
		{
			delete Keys;
		}
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryMoveKey(AnsiString OldKeyName, AnsiString NewKeyName)
{
	bool RetVal = false;
	TRegistry *Registry = new TRegistry;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
		Registry->MoveKey(OldKeyName, NewKeyName, false);
		RegistryDelete(OldKeyName);
	}
	__finally
	{
		delete Registry;
	}
	return RetVal;
}
//---------------------------------------------------------------------------

