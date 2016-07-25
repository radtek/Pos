//---------------------------------------------------------------------------
#include <vcl.h>
#include <tchar.h>
#pragma hdrstop

#include <Registry.hpp>
#include "MMRegistry.h"
#include "MMMessageBox.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Checks for Keys or values existance
//---------------------------------------------------------------------------
bool RegistryKeyExists(UnicodeString KeyName)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal;
	try
	{
   	Registry->RootKey = HKEY_LOCAL_MACHINE;
		RetVal = Registry->KeyExists(KeyName);
	}
   catch (Exception &E)
   {
		delete Registry;
		MessageBox(E.Message.w_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryRemoveValue(UnicodeString KeyName,UnicodeString Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
   	Registry->RootKey = HKEY_LOCAL_MACHINE;
      if(Registry->KeyExists(KeyName) == true)
   	{
         if(Registry->OpenKey(KeyName, false))
         {
            if(Registry->DeleteValue(Value))
            {
               RetVal = true;
            }
         }
         Registry->CloseKey();
      }
	}
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
		MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
		return false;
   }
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------
int RegistryGetSize(UnicodeString KeyName,UnicodeString ValueName)
{
	TRegistry *Registry = new TRegistry;
	int RetVal = 0;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, false))
      {
			if (Registry->ValueExists(ValueName))
         {
            RetVal = Registry->GetDataSize(ValueName);
         }
         else
         {
      		RetVal = -1;
         }
      }
	}
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
		MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
		return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryRemoveKey(UnicodeString KeyName)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
   	Registry->RootKey = HKEY_LOCAL_MACHINE;
      if(Registry->KeyExists(KeyName) == true)
   	{
         if(Registry->DeleteKey(KeyName))
         {
            RetVal = true;
         }
      }
	}
   catch (Exception &E)
   {
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryValueExists(UnicodeString KeyName, UnicodeString ValueName)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
		RetVal = Registry->OpenKeyReadOnly(KeyName);
		if (RetVal == true)
      {
      	RetVal = Registry->ValueExists(ValueName);
      }
	}
   catch (Exception &E)
   {
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryGetKeys(UnicodeString KeyName, TStrings* Strings)
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
//---------------------------------------------------------------------------
bool CreateKey(UnicodeString KeyName)
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
//---------------------------------------------------------------------------
//	Registry write routines
//---------------------------------------------------------------------------
bool RegistryWrite(UnicodeString KeyName,UnicodeString ValueName, char *Buffer,int BufferSize)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, true))
      {
			Registry->WriteBinaryData(ValueName,Buffer,BufferSize);
      	RetVal = true;
      }
	}
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}

bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, int Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------

bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, long Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------

bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, unsigned long Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------

bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, TDateTime Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------

bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, UnicodeString Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------
bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, char *Value)
{
	return RegistryWrite(KeyName, ValueName, UnicodeString(Value));
}
//---------------------------------------------------------------------------

bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, bool Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------

bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, UINT Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}

bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, Currency Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, true))
      {
			Registry->WriteCurrency(ValueName, Value);
      	RetVal = true;
      }
	}
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}

bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, double Value)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
		if (Registry->OpenKey(KeyName, true))
		{
			Registry->WriteFloat(ValueName, Value);
			RetVal = true;
		}
	}
	catch (Exception &E)
	{
		Registry->CloseKey();
		delete Registry;
		MessageBox(E.Message.w_str(), _T("Error"), MB_ICONERROR + MB_OK);
		return false;
	}
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}

//---------------------------------------------------------------------------
// Registry read routines
//---------------------------------------------------------------------------
bool RegistryRead(UnicodeString KeyName,UnicodeString ValueName, char *Buffer,unsigned int BufferSize)
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
				Registry->ReadBinaryData(ValueName,Buffer,BufferSize);
	      	RetVal = true;
         }
         else
         {
	      	RetVal = false;
         }
      }
	}
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}



bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, int &Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------

bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, long &Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------

bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, TDateTime &Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------

bool RegistryRead(const char * KeyName, const char * ValueName, UnicodeString &Value)
{
	return RegistryRead(UnicodeString(KeyName), UnicodeString(ValueName),Value);
}

bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, UnicodeString &Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------

bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, bool &Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
//---------------------------------------------------------------------------

bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, UINT &Value)
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
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}

bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, Currency &Value)
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
         	Value = Registry->ReadCurrency(ValueName);
      		RetVal = true;
         }
         else
         {
      		RetVal = false;
         }
      }
	}
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}

//---------------------------------------------------------------------------

bool RegistryWrite(UnicodeString KeyName,UnicodeString ValueName, TMemoryStream *Buffer)
{
	TRegistry *Registry = new TRegistry;
	bool RetVal = false;
	try
	{
		Registry->RootKey = HKEY_LOCAL_MACHINE;
      if (Registry->OpenKey(KeyName, true))
      {
         Buffer->Position = 0;
         RegistryWrite(KeyName,ValueName,(char *)(Buffer->Memory),int(Buffer->Size));
      	RetVal = true;
      }
	}
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
      MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
      return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}

bool RegistryRead(UnicodeString KeyName,UnicodeString ValueName, TMemoryStream *Buffer)
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
            int Size = Registry->GetDataSize(ValueName);
            char *data = new unsigned char[Size];
            try
            {
               ZeroMemory(data,sizeof(data));
               RegistryRead(KeyName,ValueName,data,Size);
               Buffer->Position = 0;
               Buffer->Write(data,Size);
            }
            __finally
            {
               delete []data;
            }
            RetVal = true;
         }
         else
         {
      		RetVal = false;
         }
      }
	}
   catch (Exception &E)
   {
      Registry->CloseKey();
		delete Registry;
	  MessageBox(E.Message.c_str(), _T("Error"), MB_ICONERROR + MB_OK);
	  return false;
   }
	Registry->CloseKey();
	delete Registry;
	return RetVal;
}
