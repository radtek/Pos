// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvDataField.pas' rev: 6.00

#ifndef RvDataFieldHPP
#define RvDataFieldHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvClass.hpp>	// Pascal unit
#include <RvDefine.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvdatafield
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TDataRowType { rtFirst, rtMiddle, rtLast };
#pragma option pop

class DELPHICLASS TRaveDataRow;
class PASCALIMPLEMENTATION TRaveDataRow : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	char *DataPtr;
	int DataSize;
	TDataRowType DataRowType;
	TRaveDataRow* Next;
	__fastcall virtual ~TRaveDataRow(void);
	void __fastcall MoveTo(TRaveDataRow* Dest);
	void __fastcall Clear(void);
	void __fastcall Init(int Size);
	void * __fastcall GetNativePtr(int FieldIdx);
	void * __fastcall GetFormattedPtr(int FieldIdx);
	void __fastcall GetNativeData(int FieldIdx, void * &CalcPtr, int &CalcSize);
	AnsiString __fastcall GetFormattedData(int FieldIdx);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveDataRow(void) : System::TObject() { }
	#pragma option pop
	
};


class DELPHICLASS TRaveDataField;
class PASCALIMPLEMENTATION TRaveDataField : public Rvclass::TRaveComponent 
{
	typedef Rvclass::TRaveComponent inherited;
	
protected:
	int FDataIndex;
	Rpdefine::TRPDataType FDataType;
	AnsiString FFieldName;
	int FSize;
	AnsiString FFullName;
	AnsiString FDescription;
	AnsiString FNullText;
	bool FCalculated;
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void) = 0 ;
	virtual AnsiString __fastcall GetAsString();
	virtual bool __fastcall GetAsBoolean(void);
	virtual int __fastcall GetAsInteger(void);
	virtual __int64 __fastcall GetAsInt64(void);
	virtual Extended __fastcall GetAsFloat(void);
	virtual System::Currency __fastcall GetAsCurrency(void);
	virtual System::TDateTime __fastcall GetAsDateTime(void);
	virtual TDate __fastcall GetAsDate(void);
	virtual TTime __fastcall GetAsTime(void);
	virtual void * __fastcall GetAsBlob(void);
	int __fastcall GetDataSize(void);
	bool __fastcall GetIsNull(void);
	void __fastcall SetFieldName(AnsiString Value);
	
public:
	__fastcall virtual TRaveDataField(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveDataField(void);
	__property Rpdefine::TRPDataType DataType = {read=GetDataType, nodefault};
	__property int DataIndex = {read=FDataIndex, write=FDataIndex, nodefault};
	__property AnsiString AsString = {read=GetAsString};
	__property bool AsBoolean = {read=GetAsBoolean, nodefault};
	__property int AsInteger = {read=GetAsInteger, nodefault};
	__property __int64 AsInt64 = {read=GetAsInt64};
	__property Extended AsFloat = {read=GetAsFloat};
	__property System::Currency AsCurrency = {read=GetAsCurrency};
	__property System::TDateTime AsDateTime = {read=GetAsDateTime};
	__property TDate AsDate = {read=GetAsDate};
	__property TTime AsTime = {read=GetAsTime};
	__property void * AsBlob = {read=GetAsBlob};
	__property int DataSize = {read=GetDataSize, nodefault};
	__property bool IsNull = {read=GetIsNull, nodefault};
	
__published:
	__property bool Calculated = {read=FCalculated, write=FCalculated, default=0};
	__property AnsiString Description = {read=FDescription, write=FDescription};
	__property AnsiString FieldName = {read=FFieldName, write=SetFieldName};
	__property AnsiString FullName = {read=FFullName, write=FFullName};
	__property AnsiString NullText = {read=FNullText, write=FNullText};
	__property int Size = {read=FSize, write=FSize, default=10};
};


typedef TMetaClass*TRaveDataFieldClass;

class DELPHICLASS TRaveFormattedField;
class PASCALIMPLEMENTATION TRaveFormattedField : public TRaveDataField 
{
	typedef TRaveDataField inherited;
	
protected:
	AnsiString FDisplayFormat;
	
__published:
	__property AnsiString DisplayFormat = {read=FDisplayFormat, write=FDisplayFormat};
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveFormattedField(Classes::TComponent* AOwner) : TRaveDataField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveFormattedField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveStringField;
class PASCALIMPLEMENTATION TRaveStringField : public TRaveDataField 
{
	typedef TRaveDataField inherited;
	
protected:
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
	virtual AnsiString __fastcall GetAsString();
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveStringField(Classes::TComponent* AOwner) : TRaveDataField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveStringField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveBooleanField;
class PASCALIMPLEMENTATION TRaveBooleanField : public TRaveDataField 
{
	typedef TRaveDataField inherited;
	
protected:
	AnsiString FTextTrue;
	AnsiString FTextFalse;
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
	virtual AnsiString __fastcall GetAsString();
	virtual bool __fastcall GetAsBoolean(void);
	
__published:
	__property AnsiString TextTrue = {read=FTextTrue, write=FTextTrue};
	__property AnsiString TextFalse = {read=FTextFalse, write=FTextFalse};
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveBooleanField(Classes::TComponent* AOwner) : TRaveDataField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveBooleanField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveIntegerField;
class PASCALIMPLEMENTATION TRaveIntegerField : public TRaveFormattedField 
{
	typedef TRaveFormattedField inherited;
	
protected:
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
	virtual AnsiString __fastcall GetAsString();
	virtual int __fastcall GetAsInteger(void);
	virtual __int64 __fastcall GetAsInt64(void);
	virtual Extended __fastcall GetAsFloat(void);
	virtual System::Currency __fastcall GetAsCurrency(void);
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveIntegerField(Classes::TComponent* AOwner) : TRaveFormattedField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveIntegerField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveInt64Field;
class PASCALIMPLEMENTATION TRaveInt64Field : public TRaveFormattedField 
{
	typedef TRaveFormattedField inherited;
	
protected:
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
	virtual AnsiString __fastcall GetAsString();
	virtual int __fastcall GetAsInteger(void);
	virtual __int64 __fastcall GetAsInt64(void);
	virtual Extended __fastcall GetAsFloat(void);
	virtual System::Currency __fastcall GetAsCurrency(void);
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveInt64Field(Classes::TComponent* AOwner) : TRaveFormattedField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveInt64Field(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveFloatField;
class PASCALIMPLEMENTATION TRaveFloatField : public TRaveFormattedField 
{
	typedef TRaveFormattedField inherited;
	
protected:
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
	virtual AnsiString __fastcall GetAsString();
	virtual int __fastcall GetAsInteger(void);
	virtual __int64 __fastcall GetAsInt64(void);
	virtual Extended __fastcall GetAsFloat(void);
	virtual System::Currency __fastcall GetAsCurrency(void);
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveFloatField(Classes::TComponent* AOwner) : TRaveFormattedField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveFloatField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveCurrencyField;
class PASCALIMPLEMENTATION TRaveCurrencyField : public TRaveFormattedField 
{
	typedef TRaveFormattedField inherited;
	
protected:
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
	virtual AnsiString __fastcall GetAsString();
	virtual int __fastcall GetAsInteger(void);
	virtual __int64 __fastcall GetAsInt64(void);
	virtual Extended __fastcall GetAsFloat(void);
	virtual System::Currency __fastcall GetAsCurrency(void);
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveCurrencyField(Classes::TComponent* AOwner) : TRaveFormattedField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveCurrencyField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveBCDField;
class PASCALIMPLEMENTATION TRaveBCDField : public TRaveCurrencyField 
{
	typedef TRaveCurrencyField inherited;
	
protected:
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveBCDField(Classes::TComponent* AOwner) : TRaveCurrencyField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveBCDField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveDateTimeField;
class PASCALIMPLEMENTATION TRaveDateTimeField : public TRaveFormattedField 
{
	typedef TRaveFormattedField inherited;
	
protected:
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
	virtual AnsiString __fastcall GetAsString();
	virtual Extended __fastcall GetAsFloat(void);
	virtual System::TDateTime __fastcall GetAsDateTime(void);
	virtual TDate __fastcall GetAsDate(void);
	virtual TTime __fastcall GetAsTime(void);
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveDateTimeField(Classes::TComponent* AOwner) : TRaveFormattedField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveDateTimeField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveDateField;
class PASCALIMPLEMENTATION TRaveDateField : public TRaveDateTimeField 
{
	typedef TRaveDateTimeField inherited;
	
protected:
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
	virtual AnsiString __fastcall GetAsString();
	virtual System::TDateTime __fastcall GetAsDateTime(void);
	virtual TDate __fastcall GetAsDate(void);
	virtual TTime __fastcall GetAsTime(void);
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveDateField(Classes::TComponent* AOwner) : TRaveDateTimeField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveDateField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveTimeField;
class PASCALIMPLEMENTATION TRaveTimeField : public TRaveDateTimeField 
{
	typedef TRaveDateTimeField inherited;
	
protected:
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
	virtual AnsiString __fastcall GetAsString();
	virtual System::TDateTime __fastcall GetAsDateTime(void);
	virtual TDate __fastcall GetAsDate(void);
	virtual TTime __fastcall GetAsTime(void);
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveTimeField(Classes::TComponent* AOwner) : TRaveDateTimeField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveTimeField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveBlobField;
class PASCALIMPLEMENTATION TRaveBlobField : public TRaveDataField 
{
	typedef TRaveDataField inherited;
	
protected:
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
	virtual AnsiString __fastcall GetAsString();
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveBlobField(Classes::TComponent* AOwner) : TRaveDataField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveBlobField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveMemoField;
class PASCALIMPLEMENTATION TRaveMemoField : public TRaveBlobField 
{
	typedef TRaveBlobField inherited;
	
protected:
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
	virtual AnsiString __fastcall GetAsString();
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveMemoField(Classes::TComponent* AOwner) : TRaveBlobField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveMemoField(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveGraphicField;
class PASCALIMPLEMENTATION TRaveGraphicField : public TRaveBlobField 
{
	typedef TRaveBlobField inherited;
	
protected:
	virtual Rpdefine::TRPDataType __fastcall GetDataType(void);
	virtual AnsiString __fastcall GetAsString();
public:
	#pragma option push -w-inl
	/* TRaveDataField.Create */ inline __fastcall virtual TRaveGraphicField(Classes::TComponent* AOwner) : TRaveBlobField(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveDataField.Destroy */ inline __fastcall virtual ~TRaveGraphicField(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TMetaClass*DataFieldTypes[13];
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Rvdatafield */
using namespace Rvdatafield;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvDataField
