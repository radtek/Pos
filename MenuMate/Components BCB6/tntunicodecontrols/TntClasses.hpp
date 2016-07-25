// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntClasses.pas' rev: 6.00

#ifndef TntClassesHPP
#define TntClassesHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ActiveX.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntclasses
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TTntStreamCharSet { csAnsi, csUnicode, csUnicodeSwapped, csUtf8 };
#pragma option pop

class DELPHICLASS TTntFileStream;
class PASCALIMPLEMENTATION TTntFileStream : public Classes::THandleStream 
{
	typedef Classes::THandleStream inherited;
	
public:
	__fastcall TTntFileStream(const WideString FileName, Word Mode);
	__fastcall virtual ~TTntFileStream(void);
};


class DELPHICLASS TTntMemoryStream;
class PASCALIMPLEMENTATION TTntMemoryStream : public Classes::TMemoryStream 
{
	typedef Classes::TMemoryStream inherited;
	
public:
	HIDESBASE void __fastcall LoadFromFile(const WideString FileName);
	HIDESBASE void __fastcall SaveToFile(const WideString FileName);
public:
	#pragma option push -w-inl
	/* TMemoryStream.Destroy */ inline __fastcall virtual ~TTntMemoryStream(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TTntMemoryStream(void) : Classes::TMemoryStream() { }
	#pragma option pop
	
};


class DELPHICLASS TTntResourceStream;
class PASCALIMPLEMENTATION TTntResourceStream : public Classes::TCustomMemoryStream 
{
	typedef Classes::TCustomMemoryStream inherited;
	
private:
	unsigned HResInfo;
	unsigned HGlobal;
	void __fastcall Initialize(unsigned Instance, wchar_t * Name, wchar_t * ResType);
	
public:
	__fastcall TTntResourceStream(unsigned Instance, const WideString ResName, wchar_t * ResType);
	__fastcall TTntResourceStream(unsigned Instance, Word ResID, wchar_t * ResType);
	__fastcall virtual ~TTntResourceStream(void);
	virtual int __fastcall Write(const void *Buffer, int Count);
	HIDESBASE void __fastcall SaveToFile(const WideString FileName);
};


__interface IWideStringsAdapter;
typedef System::DelphiInterface<IWideStringsAdapter> _di_IWideStringsAdapter;
class DELPHICLASS TTntStrings;
__interface INTERFACE_UUID("{D79A8683-A07D-42DE-BEB4-9BAEA1CAEDD0}") IWideStringsAdapter  : public IInterface 
{
	
public:
	virtual void __fastcall ReferenceStrings(TTntStrings* S) = 0 ;
	virtual void __fastcall ReleaseStrings(void) = 0 ;
};

class DELPHICLASS TAnsiStrings;
class PASCALIMPLEMENTATION TAnsiStrings : public Classes::TStrings 
{
	typedef Classes::TStrings inherited;
	
public:
	HIDESBASE void __fastcall LoadFromFile(const WideString FileName);
	HIDESBASE void __fastcall SaveToFile(const WideString FileName);
	void __fastcall LoadFromFileEx(const WideString FileName, unsigned CodePage);
	void __fastcall SaveToFileEx(const WideString FileName, unsigned CodePage);
	virtual void __fastcall LoadFromStreamEx(Classes::TStream* Stream, unsigned CodePage) = 0 ;
	virtual void __fastcall SaveToStreamEx(Classes::TStream* Stream, unsigned CodePage) = 0 ;
public:
	#pragma option push -w-inl
	/* TStrings.Destroy */ inline __fastcall virtual ~TAnsiStrings(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TAnsiStrings(void) : Classes::TStrings() { }
	#pragma option pop
	
};


#pragma option push -b-
enum TntClasses__5 { sdDelimiter, sdQuoteChar, sdNameValueSeparator };
#pragma option pop

typedef Set<TntClasses__5, sdDelimiter, sdNameValueSeparator>  TWideStringsDefined;

class PASCALIMPLEMENTATION TTntStrings : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
public:
	WideString operator[](int Index) { return Strings[Index]; }
	
private:
	TAnsiStrings* FAnsiStrings;
	void __fastcall SetAnsiStrings(const TAnsiStrings* Value);
	TTntStreamCharSet FLastFileCharSet;
	TWideStringsDefined FDefined;
	wchar_t FDelimiter;
	wchar_t FQuoteChar;
	wchar_t FNameValueSeparator;
	int FUpdateCount;
	_di_IWideStringsAdapter FAdapter;
	WideString __fastcall GetCommaText();
	WideString __fastcall GetDelimitedText();
	WideString __fastcall GetName(int Index);
	WideString __fastcall GetValue(const WideString Name);
	void __fastcall ReadData(Classes::TReader* Reader);
	void __fastcall ReadDataUTF7(Classes::TReader* Reader);
	void __fastcall ReadDataUTF8(Classes::TReader* Reader);
	void __fastcall SetCommaText(const WideString Value);
	void __fastcall SetDelimitedText(const WideString Value);
	void __fastcall SetStringsAdapter(const _di_IWideStringsAdapter Value);
	void __fastcall SetValue(const WideString Name, const WideString Value);
	void __fastcall WriteData(Classes::TWriter* Writer);
	void __fastcall WriteDataUTF7(Classes::TWriter* Writer);
	wchar_t __fastcall GetDelimiter(void);
	void __fastcall SetDelimiter(const wchar_t Value);
	wchar_t __fastcall GetQuoteChar(void);
	void __fastcall SetQuoteChar(const wchar_t Value);
	wchar_t __fastcall GetNameValueSeparator(void);
	void __fastcall SetNameValueSeparator(const wchar_t Value);
	WideString __fastcall GetValueFromIndex(int Index);
	void __fastcall SetValueFromIndex(int Index, const WideString Value);
	
protected:
	virtual void __fastcall AssignTo(Classes::TPersistent* Dest);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	void __fastcall Error(const WideString Msg, int Data)/* overload */;
	void __fastcall Error(System::PResStringRec Msg, int Data)/* overload */;
	WideString __fastcall ExtractName(const WideString S);
	virtual WideString __fastcall Get(int Index) = 0 ;
	virtual int __fastcall GetCapacity(void);
	virtual int __fastcall GetCount(void) = 0 ;
	virtual System::TObject* __fastcall GetObject(int Index);
	virtual WideString __fastcall GetTextStr();
	virtual void __fastcall Put(int Index, const WideString S);
	virtual void __fastcall PutObject(int Index, System::TObject* AObject);
	virtual void __fastcall SetCapacity(int NewCapacity);
	virtual void __fastcall SetTextStr(const WideString Value);
	virtual void __fastcall SetUpdateState(bool Updating);
	__property int UpdateCount = {read=FUpdateCount, nodefault};
	virtual int __fastcall CompareStrings(const WideString S1, const WideString S2);
	
public:
	__fastcall TTntStrings(void);
	__fastcall virtual ~TTntStrings(void);
	virtual int __fastcall Add(const WideString S);
	virtual int __fastcall AddObject(const WideString S, System::TObject* AObject);
	void __fastcall Append(const WideString S);
	virtual void __fastcall AddStrings(TTntStrings* Strings);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	void __fastcall BeginUpdate(void);
	virtual void __fastcall Clear(void) = 0 ;
	virtual void __fastcall Delete(int Index) = 0 ;
	void __fastcall EndUpdate(void);
	bool __fastcall Equals(TTntStrings* Strings);
	virtual void __fastcall Exchange(int Index1, int Index2);
	virtual wchar_t * __fastcall GetTextW(void);
	virtual int __fastcall IndexOf(const WideString S);
	virtual int __fastcall IndexOfName(const WideString Name);
	virtual int __fastcall IndexOfObject(System::TObject* AObject);
	virtual void __fastcall Insert(int Index, const WideString S) = 0 ;
	virtual void __fastcall InsertObject(int Index, const WideString S, System::TObject* AObject);
	virtual void __fastcall LoadFromFile(const WideString FileName);
	virtual void __fastcall LoadFromStream(Classes::TStream* Stream, bool WithBOM = true);
	virtual void __fastcall Move(int CurIndex, int NewIndex);
	virtual void __fastcall SaveToFile(const WideString FileName);
	virtual void __fastcall SaveToStream(Classes::TStream* Stream, bool WithBOM = true);
	virtual void __fastcall SetTextW(const wchar_t * Text);
	__property int Capacity = {read=GetCapacity, write=SetCapacity, nodefault};
	__property WideString CommaText = {read=GetCommaText, write=SetCommaText};
	__property int Count = {read=GetCount, nodefault};
	__property wchar_t Delimiter = {read=GetDelimiter, write=SetDelimiter, nodefault};
	__property WideString DelimitedText = {read=GetDelimitedText, write=SetDelimitedText};
	__property TTntStreamCharSet LastFileCharSet = {read=FLastFileCharSet, nodefault};
	__property WideString Names[int Index] = {read=GetName};
	__property System::TObject* Objects[int Index] = {read=GetObject, write=PutObject};
	__property wchar_t QuoteChar = {read=GetQuoteChar, write=SetQuoteChar, nodefault};
	__property WideString Values[WideString Name] = {read=GetValue, write=SetValue};
	__property WideString ValueFromIndex[int Index] = {read=GetValueFromIndex, write=SetValueFromIndex};
	__property wchar_t NameValueSeparator = {read=GetNameValueSeparator, write=SetNameValueSeparator, nodefault};
	__property WideString Strings[int Index] = {read=Get, write=Put/*, default*/};
	__property WideString Text = {read=GetTextStr, write=SetTextStr};
	__property _di_IWideStringsAdapter StringsAdapter = {read=FAdapter, write=SetStringsAdapter};
	
__published:
	__property TAnsiStrings* AnsiStrings = {read=FAnsiStrings, write=SetAnsiStrings, stored=false};
};


struct TWideStringItem;
typedef TWideStringItem *PWideStringItem;

#pragma pack(push, 4)
struct TWideStringItem
{
	WideString FString;
	System::TObject* FObject;
} ;
#pragma pack(pop)

typedef TWideStringItem TWideStringItemList[134217728];

typedef TWideStringItem *PWideStringItemList;

class DELPHICLASS TTntStringList;
typedef int __fastcall (*TWideStringListSortCompare)(TTntStringList* List, int Index1, int Index2);

class PASCALIMPLEMENTATION TTntStringList : public TTntStrings 
{
	typedef TTntStrings inherited;
	
private:
	TWideStringItem *FList;
	int FCount;
	int FCapacity;
	bool FSorted;
	Classes::TDuplicates FDuplicates;
	bool FCaseSensitive;
	Classes::TNotifyEvent FOnChange;
	Classes::TNotifyEvent FOnChanging;
	void __fastcall ExchangeItems(int Index1, int Index2);
	void __fastcall Grow(void);
	void __fastcall QuickSort(int L, int R, TWideStringListSortCompare SCompare);
	void __fastcall SetSorted(bool Value);
	void __fastcall SetCaseSensitive(const bool Value);
	
protected:
	virtual void __fastcall Changed(void);
	virtual void __fastcall Changing(void);
	virtual WideString __fastcall Get(int Index);
	virtual int __fastcall GetCapacity(void);
	virtual int __fastcall GetCount(void);
	virtual System::TObject* __fastcall GetObject(int Index);
	virtual void __fastcall Put(int Index, const WideString S);
	virtual void __fastcall PutObject(int Index, System::TObject* AObject);
	virtual void __fastcall SetCapacity(int NewCapacity);
	virtual void __fastcall SetUpdateState(bool Updating);
	virtual int __fastcall CompareStrings(const WideString S1, const WideString S2);
	virtual void __fastcall InsertItem(int Index, const WideString S, System::TObject* AObject);
	
public:
	__fastcall virtual ~TTntStringList(void);
	virtual int __fastcall Add(const WideString S);
	virtual int __fastcall AddObject(const WideString S, System::TObject* AObject);
	virtual void __fastcall Clear(void);
	virtual void __fastcall Delete(int Index);
	virtual void __fastcall Exchange(int Index1, int Index2);
	virtual bool __fastcall Find(const WideString S, int &Index);
	virtual int __fastcall IndexOf(const WideString S);
	virtual int __fastcall IndexOfName(const WideString Name);
	virtual void __fastcall Insert(int Index, const WideString S);
	virtual void __fastcall InsertObject(int Index, const WideString S, System::TObject* AObject);
	virtual void __fastcall Sort(void);
	virtual void __fastcall CustomSort(TWideStringListSortCompare Compare);
	__property Classes::TDuplicates Duplicates = {read=FDuplicates, write=FDuplicates, nodefault};
	__property bool Sorted = {read=FSorted, write=SetSorted, nodefault};
	__property bool CaseSensitive = {read=FCaseSensitive, write=SetCaseSensitive, nodefault};
	__property Classes::TNotifyEvent OnChange = {read=FOnChange, write=FOnChange};
	__property Classes::TNotifyEvent OnChanging = {read=FOnChanging, write=FOnChanging};
public:
	#pragma option push -w-inl
	/* TTntStrings.Create */ inline __fastcall TTntStringList(void) : TTntStrings() { }
	#pragma option pop
	
};


typedef int __fastcall (*TListTargetCompare)(void * Item, void * Target);

class DELPHICLASS TBufferedAnsiString;
class PASCALIMPLEMENTATION TBufferedAnsiString : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	AnsiString FStringBuffer;
	int LastWriteIndex;
	
public:
	void __fastcall Clear(void);
	void __fastcall AddChar(const char wc);
	void __fastcall AddString(const AnsiString s);
	void __fastcall AddBuffer(char * Buff, int Chars);
	AnsiString __fastcall Value();
	char * __fastcall BuffPtr(void);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TBufferedAnsiString(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TBufferedAnsiString(void) { }
	#pragma option pop
	
};


class DELPHICLASS TBufferedWideString;
class PASCALIMPLEMENTATION TBufferedWideString : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	WideString FStringBuffer;
	int LastWriteIndex;
	
public:
	void __fastcall Clear(void);
	void __fastcall AddChar(const wchar_t wc);
	void __fastcall AddString(const WideString s);
	void __fastcall AddBuffer(wchar_t * Buff, int Chars);
	WideString __fastcall Value();
	wchar_t * __fastcall BuffPtr(void);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TBufferedWideString(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TBufferedWideString(void) { }
	#pragma option pop
	
};


typedef DynamicArray<Byte >  TntClasses__11;

class DELPHICLASS TBufferedStreamReader;
class PASCALIMPLEMENTATION TBufferedStreamReader : public Classes::TStream 
{
	typedef Classes::TStream inherited;
	
private:
	Classes::TStream* FStream;
	int FStreamSize;
	DynamicArray<Byte >  FBuffer;
	int FBufferSize;
	int FBufferStartPosition;
	int FVirtualPosition;
	void __fastcall UpdateBufferFromPosition(int StartPos);
	
public:
	__fastcall TBufferedStreamReader(Classes::TStream* Stream, int BufferSize);
	virtual int __fastcall Read(void *Buffer, int Count);
	virtual int __fastcall Write(const void *Buffer, int Count);
	virtual int __fastcall Seek(int Offset, Word Origin)/* overload */;
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TBufferedStreamReader(void) { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline __int64 __fastcall  Seek(const __int64 Offset, Classes::TSeekOrigin Origin){ return TStream::Seek(Offset, Origin); }
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool RuntimeUTFStreaming;
extern PACKAGE void __fastcall TntPersistent_AfterInherited_DefineProperties(Classes::TFiler* Filer, Classes::TPersistent* Instance);
extern PACKAGE TTntStreamCharSet __fastcall AutoDetectCharacterSet(Classes::TStream* Stream);
extern PACKAGE bool __fastcall FindSortedListByTarget(Classes::TList* List, TListTargetCompare TargetCompare, void * Target, int &Index);
extern PACKAGE bool __fastcall ClassIsRegistered(const GUID &clsid);

}	/* namespace Tntclasses */
using namespace Tntclasses;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntClasses
