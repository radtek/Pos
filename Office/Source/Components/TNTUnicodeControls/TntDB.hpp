// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntDB.pas' rev: 6.00

#ifndef TntDBHPP
#define TntDBHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <DB.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntdb
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TTntDateTimeField;
class PASCALIMPLEMENTATION TTntDateTimeField : public Db::TDateTimeField 
{
	typedef Db::TDateTimeField inherited;
	
protected:
	virtual void __fastcall SetAsString(const AnsiString Value);
public:
	#pragma option push -w-inl
	/* TDateTimeField.Create */ inline __fastcall virtual TTntDateTimeField(Classes::TComponent* AOwner) : Db::TDateTimeField(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TField.Destroy */ inline __fastcall virtual ~TTntDateTimeField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntDateField;
class PASCALIMPLEMENTATION TTntDateField : public Db::TDateField 
{
	typedef Db::TDateField inherited;
	
protected:
	virtual void __fastcall SetAsString(const AnsiString Value);
public:
	#pragma option push -w-inl
	/* TDateField.Create */ inline __fastcall virtual TTntDateField(Classes::TComponent* AOwner) : Db::TDateField(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TField.Destroy */ inline __fastcall virtual ~TTntDateField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntTimeField;
class PASCALIMPLEMENTATION TTntTimeField : public Db::TTimeField 
{
	typedef Db::TTimeField inherited;
	
protected:
	virtual void __fastcall SetAsString(const AnsiString Value);
public:
	#pragma option push -w-inl
	/* TTimeField.Create */ inline __fastcall virtual TTntTimeField(Classes::TComponent* AOwner) : Db::TTimeField(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TField.Destroy */ inline __fastcall virtual ~TTntTimeField(void) { }
	#pragma option pop
	
};


typedef void __fastcall (__closure *TFieldGetWideTextEvent)(Db::TField* Sender, WideString &Text, bool DoDisplayText);

typedef void __fastcall (__closure *TFieldSetWideTextEvent)(Db::TField* Sender, const WideString Text);

__interface IWideStringField;
typedef System::DelphiInterface<IWideStringField> _di_IWideStringField;
__interface INTERFACE_UUID("{679C5F1A-4356-4696-A8F3-9C7C6970A9F6}") IWideStringField  : public IInterface 
{
	
public:
	virtual WideString __fastcall GetAsWideString(void) = 0 ;
	virtual void __fastcall SetAsWideString(const WideString Value) = 0 ;
	virtual WideString __fastcall GetWideDisplayText(void) = 0 ;
	virtual WideString __fastcall GetWideEditText(void) = 0 ;
	virtual void __fastcall SetWideEditText(const WideString Value) = 0 ;
	__property WideString AsWideString = {read=GetAsWideString, write=SetAsWideString};
	__property WideString WideDisplayText = {read=GetWideDisplayText};
	__property WideString WideText = {read=GetWideEditText, write=SetWideEditText};
};

class DELPHICLASS TTntWideStringField;
class PASCALIMPLEMENTATION TTntWideStringField : public Db::TWideStringField 
{
	typedef Db::TWideStringField inherited;
	
private:
	TFieldGetWideTextEvent FOnGetText;
	TFieldSetWideTextEvent FOnSetText;
	HIDESBASE WideString __fastcall GetAsWideString();
	void __fastcall SetOnGetText(const TFieldGetWideTextEvent Value);
	void __fastcall SetOnSetText(const TFieldSetWideTextEvent Value);
	void __fastcall LegacyGetText(Db::TField* Sender, AnsiString &AnsiText, bool DoDisplayText);
	void __fastcall LegacySetText(Db::TField* Sender, const AnsiString AnsiText);
	WideString __fastcall GetWideDisplayText();
	WideString __fastcall GetWideEditText();
	void __fastcall SetWideEditText(const WideString Value);
	
public:
	__property WideString Value = {read=GetAsWideString, write=SetAsWideString};
	__property WideString DisplayText = {read=GetWideDisplayText};
	__property WideString Text = {read=GetWideEditText, write=SetWideEditText};
	__property WideString AsWideString = {read=GetAsWideString, write=SetAsWideString};
	__property WideString WideDisplayText = {read=GetWideDisplayText};
	__property WideString WideText = {read=GetWideEditText, write=SetWideEditText};
	
__published:
	__property TFieldGetWideTextEvent OnGetText = {read=FOnGetText, write=SetOnGetText};
	__property TFieldSetWideTextEvent OnSetText = {read=FOnSetText, write=SetOnSetText};
public:
	#pragma option push -w-inl
	/* TWideStringField.Create */ inline __fastcall virtual TTntWideStringField(Classes::TComponent* AOwner) : Db::TWideStringField(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TField.Destroy */ inline __fastcall virtual ~TTntWideStringField(void) { }
	#pragma option pop
	
private:
	void *__IWideStringField;	/* Tntdb::IWideStringField */
	
public:
	operator IWideStringField*(void) { return (IWideStringField*)&__IWideStringField; }
	
};


#pragma option push -b-
enum TTntStringFieldEncodingMode { emNone, emUTF8, emUTF7, emFixedCodePage, emFixedCodePage_Safe };
#pragma option pop

#pragma option push -b-
enum TTntStringFieldCodePageEnum { fcpOther, fcpThai, fcpJapanese, fcpSimplifiedChinese, fcpTraditionalChinese, fcpKorean, fcpCentralEuropean, fcpCyrillic, fcpLatinWestern, fcpGreek, fcpTurkish, fcpHebrew, fcpArabic, fcpBaltic, fcpVietnamese };
#pragma option pop

class DELPHICLASS TTntStringField;
class PASCALIMPLEMENTATION TTntStringField : public Db::TStringField 
{
	typedef Db::TStringField inherited;
	
private:
	TFieldGetWideTextEvent FOnGetText;
	TFieldSetWideTextEvent FOnSetText;
	TTntStringFieldEncodingMode FEncodingMode;
	Word FFixedCodePage;
	WideString __fastcall GetAsWideString();
	void __fastcall SetAsWideString(const WideString Value);
	void __fastcall SetOnGetText(const TFieldGetWideTextEvent Value);
	void __fastcall SetOnSetText(const TFieldSetWideTextEvent Value);
	void __fastcall LegacyGetText(Db::TField* Sender, AnsiString &AnsiText, bool DoDisplayText);
	void __fastcall LegacySetText(Db::TField* Sender, const AnsiString AnsiText);
	WideString __fastcall GetWideDisplayText();
	WideString __fastcall GetWideEditText();
	void __fastcall SetWideEditText(const WideString Value);
	TTntStringFieldCodePageEnum __fastcall GetFixedCodePageEnum(void);
	void __fastcall SetFixedCodePageEnum(const TTntStringFieldCodePageEnum Value);
	bool __fastcall IsFixedCodePageStored(void);
	
protected:
	virtual Variant __fastcall GetAsVariant();
	virtual void __fastcall SetVarValue(const Variant &Value);
	virtual AnsiString __fastcall GetAsString();
	virtual void __fastcall SetAsString(const AnsiString Value);
	
public:
	__fastcall virtual TTntStringField(Classes::TComponent* AOwner);
	__property WideString Value = {read=GetAsWideString, write=SetAsWideString};
	__property WideString DisplayText = {read=GetWideDisplayText};
	__property WideString Text = {read=GetWideEditText, write=SetWideEditText};
	__property WideString AsWideString = {read=GetAsWideString, write=SetAsWideString};
	__property WideString WideDisplayText = {read=GetWideDisplayText};
	__property WideString WideText = {read=GetWideEditText, write=SetWideEditText};
	
__published:
	__property TTntStringFieldEncodingMode EncodingMode = {read=FEncodingMode, write=FEncodingMode, default=1};
	__property TTntStringFieldCodePageEnum FixedCodePageEnum = {read=GetFixedCodePageEnum, write=SetFixedCodePageEnum, stored=false, nodefault};
	__property Word FixedCodePage = {read=FFixedCodePage, write=FFixedCodePage, stored=IsFixedCodePageStored, nodefault};
	__property TFieldGetWideTextEvent OnGetText = {read=FOnGetText, write=SetOnGetText};
	__property TFieldSetWideTextEvent OnSetText = {read=FOnSetText, write=SetOnSetText};
public:
	#pragma option push -w-inl
	/* TField.Destroy */ inline __fastcall virtual ~TTntStringField(void) { }
	#pragma option pop
	
private:
	void *__IWideStringField;	/* Tntdb::IWideStringField */
	
public:
	operator IWideStringField*(void) { return (IWideStringField*)&__IWideStringField; }
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE Word TntStringFieldCodePageEnumMap[15];
extern PACKAGE TMetaClass* __fastcall GetTntFieldClass(TMetaClass* FieldClass);
extern PACKAGE WideString __fastcall GetAsWideString(Db::TField* Field);
extern PACKAGE void __fastcall SetAsWideString(Db::TField* Field, const WideString Value);
extern PACKAGE WideString __fastcall GetWideDisplayText(Db::TField* Field);
extern PACKAGE WideString __fastcall GetWideText(Db::TField* Field);
extern PACKAGE void __fastcall SetWideText(Db::TField* Field, const WideString Value);

}	/* namespace Tntdb */
using namespace Tntdb;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntDB
