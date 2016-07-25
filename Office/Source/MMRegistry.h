//---------------------------------------------------------------------------
#ifndef MMRegistryH
#define MMRegistryH
//---------------------------------------------------------------------------
bool RegistryKeyExists(AnsiString KeyName);
bool RegistryValueExists(AnsiString KeyName, AnsiString ValueName);
bool RegistryGetKeys(AnsiString KeyName, TStrings* Strings);
//---------------------------------------------------------------------------
bool CreateKey(AnsiString KeyName);

bool RegistryWrite(AnsiString KeyName, AnsiString ValueName, int Value);
bool RegistryWrite(AnsiString KeyName, AnsiString ValueName, TDateTime Value);
bool RegistryWrite(AnsiString KeyName, AnsiString ValueName, AnsiString Value);
bool RegistryWrite(AnsiString KeyName, AnsiString ValueName, bool Value);
bool RegistryWrite(AnsiString KeyName, AnsiString ValueName, UINT Value);
bool RegistryWrite(AnsiString KeyName, AnsiString ValueName, char *Value);
//bool RegistryWriteInteger(AnsiString KeyName, AnsiString ValueName, int Value);
//bool RegistryWriteDateTime(AnsiString KeyName, AnsiString ValueName, TDateTime Value);
//---------------------------------------------------------------------------
bool RegistryRead(AnsiString KeyName, AnsiString ValueName, int &Value);
bool RegistryRead(AnsiString KeyName, AnsiString ValueName, TDateTime &Value);
bool RegistryRead(AnsiString KeyName, AnsiString ValueName, AnsiString &Value);
bool RegistryRead(AnsiString KeyName, AnsiString ValueName, bool &Value);
bool RegistryRead(AnsiString KeyName, AnsiString ValueName, UINT &Value);
//bool RegistryReadInteger(AnsiString KeyName, AnsiString ValueName, int &Value);
//bool RegistryReadDateTime(AnsiString KeyName, AnsiString ValueName, TDateTime &Value);
//---------------------------------------------------------------------------
bool RegistryDelete(AnsiString KeyName, AnsiString ValueName);
bool RegistryDelete(AnsiString KeyName);
//---------------------------------------------------------------------------
bool RegistryMoveKey(AnsiString OldKeyName, AnsiString NewKeyName);
//---------------------------------------------------------------------------

#endif
