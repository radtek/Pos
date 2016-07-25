//---------------------------------------------------------------------------
#ifndef MMRegistryH
#define MMRegistryH
//---------------------------------------------------------------------------
bool RegistryKeyExists(UnicodeString KeyName);
bool RegistryValueExists(UnicodeString KeyName, UnicodeString ValueName);
bool RegistryRemoveValue(UnicodeString KeyName,UnicodeString Value);
bool RegistryRemoveKey(UnicodeString KeyName);
int RegistryGetSize(UnicodeString KeyName,UnicodeString ValueName);
//---------------------------------------------------------------------------
bool RegistryGetKeys(UnicodeString KeyName, TStrings* Strings);
bool CreateKey(UnicodeString KeyName);
//---------------------------------------------------------------------------
bool RegistryMoveKey(UnicodeString OldKeyName, UnicodeString NewKeyName);
bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, int Value);
bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, long Value);
bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, unsigned long Value);
bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, TDateTime Value);
bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, UnicodeString Value);
bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, char *Value);
bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, bool Value);
bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, UINT Value);
bool RegistryWrite(UnicodeString KeyName, UnicodeString ValueName, Currency Value);
bool RegistryWrite(UnicodeString KeyName,UnicodeString ValueName, char *Buffer,int BufferSize);
bool RegistryWrite(UnicodeString KeyName,UnicodeString ValueName, TMemoryStream *Buffer);
//---------------------------------------------------------------------------
bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, int &Value);
bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, long &Value);
bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, unsigned long &Value);
bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, TDateTime &Value);
bool RegistryRead(const char * KeyName, const char * ValueName, UnicodeString &Value);
bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, UnicodeString &Value);
bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, bool &Value);
bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, UINT &Value);
bool RegistryRead(UnicodeString KeyName, UnicodeString ValueName, Currency &Value);
bool RegistryRead(UnicodeString KeyName,UnicodeString ValueName, char *Buffer,unsigned int BufferSize);
bool RegistryRead(UnicodeString KeyName,UnicodeString ValueName, TMemoryStream *Buffer);
//---------------------------------------------------------------------------

#endif
