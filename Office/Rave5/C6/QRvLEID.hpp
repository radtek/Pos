// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvLEID.pas' rev: 6.00

#ifndef QRvLEIDHPP
#define QRvLEIDHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRvUtil.hpp>	// Pascal unit
#include <QRvLEType.hpp>	// Pascal unit
#include <QRvLEUtil.hpp>	// Pascal unit
#include <QRvLEDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvleid
{
//-- type declarations -------------------------------------------------------
typedef TMetaClass*TBaseIDClass;

class DELPHICLASS TBaseID;
class PASCALIMPLEMENTATION TBaseID : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Qrvletype::TBaseType* FBaseType;
	AnsiString FID;
	Qrvledefine::TIDKind FIDKind;
	int FModule;
	virtual void __fastcall SetModule(int Value);
	
public:
	__fastcall TBaseID(Qrvledefine::TIDKind AIDKind, AnsiString AID, Qrvletype::TBaseType* ABaseType);
	__fastcall virtual TBaseID(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper);
	__property Qrvletype::TBaseType* BaseType = {read=FBaseType};
	__property AnsiString ID = {read=FID};
	__property Qrvledefine::TIDKind IDKind = {read=FIDKind, nodefault};
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
	__fastcall TTypeID(AnsiString AID, Qrvletype::TBaseType* ABaseType);
public:
	#pragma option push -w-inl
	/* TBaseID.CreateFromStreamHelper */ inline __fastcall virtual TTypeID(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper) : TBaseID(TypeHelper, StreamHelper) { }
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
	Qrvledefine::TAddress FAddress;
	virtual void __fastcall SetModule(int Value);
	
public:
	__fastcall TVarID(AnsiString AID, Qrvletype::TBaseType* ABaseType, int AAddress, Qrvledefine::TAddressLoc AAddressLoc);
	__fastcall virtual TVarID(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper);
	__property Qrvledefine::TAddress Address = {read=FAddress, write=FAddress};
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
	__fastcall TConstID(AnsiString AID, Qrvletype::TBaseType* ABaseType, void *AData, int ADataSize);
	__fastcall virtual TConstID(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper);
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
	Qrvledefine::TCodeLabel FCodeLabel;
	
public:
	__fastcall TFuncID(AnsiString AID, Qrvletype::TBaseType* ABaseType);
	__fastcall virtual TFuncID(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper);
	__property Byte DefineLevel = {read=FDefineLevel, write=FDefineLevel, nodefault};
	__property int ExecAddr = {read=FExecAddr, write=FExecAddr, nodefault};
	__property Qrvledefine::TCodeLabel CodeLabel = {read=FCodeLabel, write=FCodeLabel};
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
	__fastcall TMethodID(AnsiString AID, Qrvletype::TBaseType* ABaseType);
	__fastcall virtual TMethodID(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper);
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
	Qrvledefine::TOpKind FOpKind;
	
public:
	__fastcall TOpID(AnsiString AID, Qrvledefine::TOpKind AOpKind);
	__fastcall virtual TOpID(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper);
	virtual void __fastcall SaveToStreamHelper(Qrvletype::TTypeHelper* TypeHelper, Qrvutil::TStreamHelper* StreamHelper);
	__property Qrvledefine::TOpKind OpKind = {read=FOpKind, nodefault};
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
	Qrvletype::TTypeHelper* TypeHelper;
	
public:
	__fastcall TIDManager(Qrvletype::TTypeHelper* ATypeHelper);
	__fastcall virtual ~TIDManager(void);
	void __fastcall LoadFromStreamHelper(Qrvutil::TStreamHelper* StreamHelper);
	void __fastcall SaveToStreamHelper(Qrvutil::TStreamHelper* StreamHelper);
	void __fastcall Clear(void);
	TBaseID* __fastcall Add(TBaseID* IDObject);
	TBaseID* __fastcall Find(AnsiString IDName);
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TMetaClass*IDTypes[6];

}	/* namespace Qrvleid */
using namespace Qrvleid;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvLEID
