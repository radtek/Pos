// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvLEID.pas' rev: 6.00

#ifndef RvLEIDHPP
#define RvLEIDHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvUtil.hpp>	// Pascal unit
#include <RvLEType.hpp>	// Pascal unit
#include <RvLEUtil.hpp>	// Pascal unit
#include <RvLEDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvleid
{
//-- type declarations -------------------------------------------------------
typedef TMetaClass*TBaseIDClass;

class DELPHICLASS TBaseID;
class PASCALIMPLEMENTATION TBaseID : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Rvletype::TBaseType* FBaseType;
	AnsiString FID;
	Rvledefine::TIDKind FIDKind;
	int FModule;
	virtual void __fastcall SetModule(int Value);
	
public:
	__fastcall TBaseID(Rvledefine::TIDKind AIDKind, AnsiString AID, Rvletype::TBaseType* ABaseType);
	__fastcall virtual TBaseID(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	__property Rvletype::TBaseType* BaseType = {read=FBaseType};
	__property AnsiString ID = {read=FID};
	__property Rvledefine::TIDKind IDKind = {read=FIDKind, nodefault};
	__property int Module = {read=FModule, write=SetModule, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TBaseID(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTypeID;
class PASCALIMPLEMENTATION TTypeID : public TBaseID 
{
	typedef TBaseID inherited;
	
public:
	__fastcall TTypeID(AnsiString AID, Rvletype::TBaseType* ABaseType);
public:
	#pragma option push -w-inl
	/* TBaseID.CreateFromStreamHelper */ inline __fastcall virtual TTypeID(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper) : TBaseID(TypeHelper, StreamHelper) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TTypeID(void) { }
	#pragma option pop
	
};


class DELPHICLASS TVarID;
class PASCALIMPLEMENTATION TVarID : public TBaseID 
{
	typedef TBaseID inherited;
	
protected:
	Rvledefine::TAddress FAddress;
	virtual void __fastcall SetModule(int Value);
	
public:
	__fastcall TVarID(AnsiString AID, Rvletype::TBaseType* ABaseType, int AAddress, Rvledefine::TAddressLoc AAddressLoc);
	__fastcall virtual TVarID(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	__property Rvledefine::TAddress Address = {read=FAddress, write=FAddress};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TVarID(void) { }
	#pragma option pop
	
};


class DELPHICLASS TConstID;
class PASCALIMPLEMENTATION TConstID : public TBaseID 
{
	typedef TBaseID inherited;
	
public:
	void *FData;
	int FDataSize;
	__fastcall TConstID(AnsiString AID, Rvletype::TBaseType* ABaseType, void *AData, int ADataSize);
	__fastcall virtual TConstID(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	__fastcall virtual ~TConstID(void);
	void __fastcall GetData(void *Buffer);
	__property void * Data = {read=FData};
	__property int DataSize = {read=FDataSize, nodefault};
};


class DELPHICLASS TFuncID;
class PASCALIMPLEMENTATION TFuncID : public TBaseID 
{
	typedef TBaseID inherited;
	
protected:
	Byte FDefineLevel;
	int FExecAddr;
	Rvledefine::TCodeLabel FCodeLabel;
	
public:
	__fastcall TFuncID(AnsiString AID, Rvletype::TBaseType* ABaseType);
	__fastcall virtual TFuncID(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	__property Byte DefineLevel = {read=FDefineLevel, write=FDefineLevel, nodefault};
	__property int ExecAddr = {read=FExecAddr, write=FExecAddr, nodefault};
	__property Rvledefine::TCodeLabel CodeLabel = {read=FCodeLabel, write=FCodeLabel};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TFuncID(void) { }
	#pragma option pop
	
};


class DELPHICLASS TMethodID;
class PASCALIMPLEMENTATION TMethodID : public TFuncID 
{
	typedef TFuncID inherited;
	
public:
	__fastcall TMethodID(AnsiString AID, Rvletype::TBaseType* ABaseType);
	__fastcall virtual TMethodID(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TMethodID(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpID;
class PASCALIMPLEMENTATION TOpID : public TBaseID 
{
	typedef TBaseID inherited;
	
private:
	Rvledefine::TOpKind FOpKind;
	
public:
	__fastcall TOpID(AnsiString AID, Rvledefine::TOpKind AOpKind);
	__fastcall virtual TOpID(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Rvletype::TTypeHelper* TypeHelper, Rvutil::TStreamHelper* StreamHelper);
	__property Rvledefine::TOpKind OpKind = {read=FOpKind, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TOpID(void) { }
	#pragma option pop
	
};


class DELPHICLASS TIDManager;
class PASCALIMPLEMENTATION TIDManager : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	Classes::TList* IDList;
	int IDIdx;
	Rvletype::TTypeHelper* TypeHelper;
	
public:
	__fastcall TIDManager(Rvletype::TTypeHelper* ATypeHelper);
	__fastcall virtual ~TIDManager(void);
	void __fastcall LoadFromStreamHelper(Rvutil::TStreamHelper* StreamHelper);
	void __fastcall SaveToStreamHelper(Rvutil::TStreamHelper* StreamHelper);
	void __fastcall Clear(void);
	TBaseID* __fastcall Add(TBaseID* IDObject);
	TBaseID* __fastcall Find(AnsiString IDName);
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TMetaClass*IDTypes[6];

}	/* namespace Rvleid */
using namespace Rvleid;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvLEID
