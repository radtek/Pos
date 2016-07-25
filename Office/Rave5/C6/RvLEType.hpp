// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvLEType.pas' rev: 6.00

#ifndef RvLETypeHPP
#define RvLETypeHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvUtil.hpp>	// Pascal unit
#include <RvLEUtil.hpp>	// Pascal unit
#include <RvLEDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvletype
{
//-- type declarations -------------------------------------------------------
typedef TMetaClass*TBaseTypeClass;

class DELPHICLASS TBaseType;
class DELPHICLASS TTypeHelper;
class PASCALIMPLEMENTATION TBaseType : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Rvledefine::TTypeKind FTypeKind;
	Rvledefine::TTypeStruc FTypeStruc;
	int FSize;
	System::TObject* FOwner;
	int FIndex;
	virtual int __fastcall GetSize(void);
	void __fastcall SetOwner(System::TObject* Value);
	
public:
	__fastcall TBaseType(Rvledefine::TTypeKind ATypeKind);
	__fastcall virtual TBaseType(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual bool __fastcall NeedFree(void);
	virtual void __fastcall FreeData(void * Data);
	__property Rvledefine::TTypeStruc TypeStruc = {read=FTypeStruc, nodefault};
	__property int Size = {read=GetSize, nodefault};
	__property int Index = {read=FIndex, write=FIndex, nodefault};
	__property System::TObject* Owner = {read=FOwner, write=SetOwner};
	__property Rvledefine::TTypeKind TypeKind = {read=FTypeKind, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TBaseType(void) { }
	#pragma option pop
	
};


class DELPHICLASS TSimpleType;
class PASCALIMPLEMENTATION TSimpleType : public TBaseType 
{
	typedef TBaseType inherited;
	
protected:
	int FLow;
	int FHigh;
	virtual int __fastcall GetSize(void);
	
public:
	__fastcall TSimpleType(Rvledefine::TTypeKind ATypeKind, int ALow, int AHigh);
	__fastcall virtual TSimpleType(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	__property int Low = {read=FLow, nodefault};
	__property int High = {read=FHigh, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TSimpleType(void) { }
	#pragma option pop
	
};


class DELPHICLASS TEnumeratedType;
class PASCALIMPLEMENTATION TEnumeratedType : public TBaseType 
{
	typedef TBaseType inherited;
	
protected:
	Byte FHigh;
	virtual int __fastcall GetSize(void);
	
public:
	__fastcall TEnumeratedType(void);
	__fastcall virtual ~TEnumeratedType(void);
	__fastcall virtual TEnumeratedType(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	__property Byte High = {read=FHigh, write=FHigh, nodefault};
};


struct TRange;
typedef TRange *PRange;

#pragma pack(push, 4)
struct TRange
{
	int Low;
	int High;
	TBaseType* BaseType;
} ;
#pragma pack(pop)

class DELPHICLASS TArrayType;
class PASCALIMPLEMENTATION TArrayType : public TBaseType 
{
	typedef TBaseType inherited;
	
protected:
	TBaseType* FElementType;
	Classes::TList* FDim;
	int __fastcall GetDims(void);
	int __fastcall GetLow(int Index);
	int __fastcall GetHigh(int Index);
	int __fastcall GetCount(int Index);
	TBaseType* __fastcall GetDimType(int Index);
	int __fastcall GetDimSize(int Index);
	virtual int __fastcall GetSize(void);
	
public:
	__fastcall TArrayType(void);
	__fastcall virtual ~TArrayType(void);
	__fastcall virtual TArrayType(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	void __fastcall AddDim(int Low, int High, TBaseType* BaseType);
	virtual bool __fastcall NeedFree(void);
	virtual void __fastcall FreeData(void * Data);
	__property TBaseType* ElementType = {read=FElementType, write=FElementType};
	__property int Dims = {read=GetDims, nodefault};
	__property int Low[int Index] = {read=GetLow};
	__property int High[int Index] = {read=GetHigh};
	__property int Count[int Index] = {read=GetCount};
	__property TBaseType* DimType[int Index] = {read=GetDimType};
	__property int DimSize[int Index] = {read=GetDimSize};
};


#pragma option push -b-
enum TParamType { ptValue, ptVar };
#pragma option pop

struct TFuncParam;
typedef TFuncParam *PFuncParam;

#pragma pack(push, 1)
struct TFuncParam
{
	AnsiString ID;
	TBaseType* BaseType;
	TParamType ParamType;
	int DefaultValue;
	int Offset;
} ;
#pragma pack(pop)

class DELPHICLASS TFuncType;
class PASCALIMPLEMENTATION TFuncType : public TBaseType 
{
	typedef TBaseType inherited;
	
protected:
	Classes::TList* ParamList;
	TBaseType* FResultType;
	PFuncParam __fastcall GetParam(int Index);
	Rvledefine::TAddress __fastcall GetParamAddr(int Index);
	Word __fastcall GetParams(void);
	
public:
	__fastcall TFuncType(void);
	__fastcall virtual ~TFuncType(void);
	__fastcall virtual TFuncType(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	PFuncParam __fastcall AddParam(AnsiString AID, TParamType AParamType);
	bool __fastcall EqualTo(TFuncType* FuncType);
	__property PFuncParam Param[int Index] = {read=GetParam};
	__property Rvledefine::TAddress ParamAddr[int Index] = {read=GetParamAddr};
	__property Word Params = {read=GetParams, nodefault};
	__property TBaseType* ResultType = {read=FResultType, write=FResultType};
};


class DELPHICLASS TMethodType;
class PASCALIMPLEMENTATION TMethodType : public TFuncType 
{
	typedef TFuncType inherited;
	
public:
	#pragma option push -w-inl
	/* TFuncType.Create */ inline __fastcall TMethodType(void) : TFuncType() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TFuncType.Destroy */ inline __fastcall virtual ~TMethodType(void) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TFuncType.CreateFromStreamHelper */ inline __fastcall virtual TMethodType(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper) : TFuncType(TypeHelper, StreamHelper) { }
	#pragma option pop
	
};


class DELPHICLASS TStructureMember;
class DELPHICLASS TStructureType;
class PASCALIMPLEMENTATION TStructureType : public TBaseType 
{
	typedef TBaseType inherited;
	
protected:
	Classes::TStringList* FTypeList;
	int BaseSize;
	virtual int __fastcall GetSize(void);
	
public:
	__fastcall TStructureType(void);
	__fastcall virtual ~TStructureType(void);
	__fastcall virtual TStructureType(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	TStructureMember* __fastcall AddElement(AnsiString ID, TBaseType* BaseType);
	virtual bool __fastcall FindMember(AnsiString ID, Rvledefine::TScopeSet ValidScopes, int &Offset, TStructureMember* &Member, int &Index);
	TStructureMember* __fastcall IndexToMember(int Index, AnsiString &ID);
	virtual bool __fastcall NeedFree(void);
	virtual void __fastcall FreeData(void * Data);
	__property Classes::TStringList* TypeList = {read=FTypeList};
};


class PASCALIMPLEMENTATION TStructureMember : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	TBaseType* FBaseType;
	Rvledefine::TMemberType FMemberType;
	TStructureType* FStructureType;
	
public:
	__fastcall virtual TStructureMember(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper, TStructureType* Structure);
	virtual void __fastcall SaveToStreamHelper(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	__property TBaseType* BaseType = {read=FBaseType, write=FBaseType};
	__property Rvledefine::TMemberType MemberType = {read=FMemberType, write=FMemberType, nodefault};
	__property TStructureType* StructureType = {read=FStructureType, write=FStructureType};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TStructureMember(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TStructureMember(void) { }
	#pragma option pop
	
};


typedef TMetaClass*TStructureMemberClass;

class DELPHICLASS TClassMember;
class PASCALIMPLEMENTATION TClassMember : public TStructureMember 
{
	typedef TStructureMember inherited;
	
protected:
	Rvledefine::TScope FScope;
	
public:
	__fastcall virtual TClassMember(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper, TStructureType* Structure);
	virtual void __fastcall SaveToStreamHelper(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	__property Rvledefine::TScope Scope = {read=FScope, write=FScope, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TClassMember(void) : TStructureMember() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TClassMember(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TMethodOptions { moVirtual, moOverride };
#pragma option pop

typedef Set<TMethodOptions, moVirtual, moOverride>  TMethodOptionsSet;

class DELPHICLASS TMethodMember;
class PASCALIMPLEMENTATION TMethodMember : public TClassMember 
{
	typedef TClassMember inherited;
	
protected:
	TMethodOptionsSet FOptions;
	
public:
	__fastcall virtual TMethodMember(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper, TStructureType* Structure);
	virtual void __fastcall SaveToStreamHelper(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	__property TMethodOptionsSet Options = {read=FOptions, write=FOptions, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TMethodMember(void) : TClassMember() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TMethodMember(void) { }
	#pragma option pop
	
};


class DELPHICLASS TPropertyMember;
class PASCALIMPLEMENTATION TPropertyMember : public TClassMember 
{
	typedef TClassMember inherited;
	
protected:
	int FReadIndex;
	int FWriteIndex;
	
public:
	__fastcall virtual TPropertyMember(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper, TStructureType* Structure);
	virtual void __fastcall SaveToStreamHelper(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	__property int ReadIndex = {read=FReadIndex, write=FReadIndex, nodefault};
	__property int WriteIndex = {read=FWriteIndex, write=FWriteIndex, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TPropertyMember(void) : TClassMember() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TPropertyMember(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRecordType;
class PASCALIMPLEMENTATION TRecordType : public TStructureType 
{
	typedef TStructureType inherited;
	
public:
	__fastcall TRecordType(void);
public:
	#pragma option push -w-inl
	/* TStructureType.Destroy */ inline __fastcall virtual ~TRecordType(void) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TStructureType.CreateFromStreamHelper */ inline __fastcall virtual TRecordType(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper) : TStructureType(TypeHelper, StreamHelper) { }
	#pragma option pop
	
};


class DELPHICLASS TClassType;
class PASCALIMPLEMENTATION TClassType : public TStructureType 
{
	typedef TStructureType inherited;
	
protected:
	int FRaveInstanceSize;
	TClassType* FRaveClassParent;
	AnsiString FRaveClassName;
	virtual int __fastcall GetSize(void);
	int __fastcall GetRaveInstanceSize(void);
	
public:
	__fastcall TClassType(TClassType* AClassParent, AnsiString AClassName);
	__fastcall virtual TClassType(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual bool __fastcall FindMember(AnsiString ID, Rvledefine::TScopeSet ValidScopes, int &Offset, TStructureMember* &Member, int &Index);
	TClassMember* __fastcall AddField(AnsiString ID, TBaseType* BaseType, Rvledefine::TScope Scope);
	TMethodMember* __fastcall AddMethod(AnsiString ID, TBaseType* BaseType, Rvledefine::TScope Scope, TMethodOptionsSet Options);
	TPropertyMember* __fastcall AddProperty(AnsiString ID, TBaseType* BaseType, Rvledefine::TScope Scope, int ReadIndex, int WriteIndex);
	__property TClassType* RaveClassParent = {read=FRaveClassParent};
	__property AnsiString RaveClassName = {read=FRaveClassName};
	__property int RaveInstanceSize = {read=GetRaveInstanceSize, nodefault};
public:
	#pragma option push -w-inl
	/* TStructureType.Destroy */ inline __fastcall virtual ~TClassType(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TTypeHelper : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	virtual TBaseType* __fastcall IndexToType(int ModuleIndex, int TypeIndex) = 0 ;
	virtual void __fastcall TypeToIndex(TBaseType* BaseType, int &ModuleIndex, int &TypeIndex) = 0 ;
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TTypeHelper(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TTypeHelper(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTypeManager;
class PASCALIMPLEMENTATION TTypeManager : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	Classes::TList* TypeList;
	TTypeHelper* TypeHelper;
	
protected:
	System::TObject* FOwner;
	
public:
	__fastcall TTypeManager(System::TObject* AOwner, TTypeHelper* ATypeHelper);
	__fastcall virtual ~TTypeManager(void);
	void __fastcall Clear(void);
	void __fastcall LoadFromStreamHelper(Rvutil::TStreamHelper* StreamHelper);
	void __fastcall SaveToStreamHelper(Rvutil::TStreamHelper* StreamHelper);
	TBaseType* __fastcall Add(TBaseType* BaseType);
	TBaseType* __fastcall Get(int Index);
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TMetaClass*TypeTypes[6];
extern PACKAGE TMetaClass*MemberTypes[4];
extern PACKAGE TSimpleType* BooleanType;
extern PACKAGE TSimpleType* IntegerType;
extern PACKAGE TSimpleType* ExtendedType;
extern PACKAGE TSimpleType* CurrencyType;
extern PACKAGE TSimpleType* CharType;
extern PACKAGE TSimpleType* StringType;
extern PACKAGE TFuncType* OrdFunc;

}	/* namespace Rvletype */
using namespace Rvletype;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvLEType
