// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvLEExpr.pas' rev: 6.00

#ifndef RvLEExprHPP
#define RvLEExprHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvClass.hpp>	// Pascal unit
#include <RvUtil.hpp>	// Pascal unit
#include <RvLECode.hpp>	// Pascal unit
#include <RvLEModule.hpp>	// Pascal unit
#include <RvLEID.hpp>	// Pascal unit
#include <RvLEType.hpp>	// Pascal unit
#include <RvLEUtil.hpp>	// Pascal unit
#include <RvLEDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvleexpr
{
//-- type declarations -------------------------------------------------------
struct TCodeListItem;
typedef TCodeListItem *PCodeListItem;

#pragma pack(push, 1)
struct TCodeListItem
{
	Rvletype::TBaseType* FromType;
	Rvletype::TBaseType* ToType;
	Word TempSize;
	int TempLoc;
	int Line;
	Word Col;
	Rvledefine::TIDKind Kind;
	Rvledefine::TAddress Address;
	bool DefaultParam;
	bool FuncResult;
	Rvleid::TBaseID* VarID;
	Rvutil::TRaveStackList* OpList;
	Rvletype::TFuncType* FuncType;
	int FuncModule;
	Rvledefine::TCodeLabel *FuncCodeLabel;
	bool FuncMethod;
	Word NumParams;
	Rvledefine::TOpKind OpKind;
	AnsiString StringData;
	void *Data;
	Byte RefCount;
} ;
#pragma pack(pop)

struct TOpStackItem;
typedef TOpStackItem *POpStackItem;

#pragma pack(push, 1)
struct TOpStackItem
{
	Word Level;
	Byte Order;
	Rvledefine::TOpKind Kind;
	Rvletype::TFuncType* OSFuncType;
	Word OSFuncModule;
	Rvledefine::TCodeLabel *OSFuncCodeLabel;
	Word OSNumParams;
	int OpLine;
	Word OpCol;
	AnsiString OpToken;
	Byte RefCount;
} ;
#pragma pack(pop)

#pragma pack(push, 4)
struct TFreeVar
{
	Rvletype::TBaseType* BaseType;
	Rvledefine::TAddress Address;
} ;
#pragma pack(pop)

typedef TFreeVar *PFreeVar;

class DELPHICLASS TExpression;
class PASCALIMPLEMENTATION TExpression : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Rvutil::TRaveStackList* FCodeList;
	Rvutil::TRaveStackList* FOpStack;
	TCodeListItem *FResultItem;
	bool FNeedResult;
	Rvutil::TRaveStackList* CalcStack;
	Word CurrLevel;
	Rvleutil::TRaveParser* Parser;
	Rvlecode::TCodeManager* CM;
	Rvlemodule::TRCPUModule* Module;
	bool __fastcall CalcOpResult(Rvledefine::TTypeKind &TK1, Rvledefine::TTypeKind &TK2, Rvledefine::TTypeKind &TK3, Rvledefine::TTypeKind &TK4, Rvledefine::TOpKind OpKind);
	Rvletype::TBaseType* __fastcall GetBaseType(Rvledefine::TTypeKind TypeKind);
	void __fastcall AddCodeList(PCodeListItem CodeListItem);
	void __fastcall ProcessOpStack(Rvledefine::TOpKind NewOpKind);
	bool __fastcall FoldConstant(POpStackItem OpStackItem, Rvledefine::TOpKind OpKind);
	void __fastcall FreeCodeListItem(PCodeListItem CLI);
	void __fastcall PushCLIStack(Rvutil::TRaveStackList* Stack, PCodeListItem CLI);
	void __fastcall PopCLIStack(Rvutil::TRaveStackList* Stack);
	void __fastcall PushOSIStack(Rvutil::TRaveStackList* Stack, POpStackItem OSI);
	void __fastcall PopOSIStack(Rvutil::TRaveStackList* Stack);
	
public:
	__fastcall TExpression(Rvlemodule::TRCPUModule* AModule, Rvleutil::TRaveParser* AParser, Rvlecode::TCodeManager* ACM);
	__fastcall virtual ~TExpression(void);
	void __fastcall CreateCodeList(AnsiString Terminators, bool LeaveResultOnStack);
	void __fastcall FreeCodeList(void);
	void __fastcall FreeType(const Rvledefine::TAddress &Addr, Rvletype::TBaseType* BaseType);
	void __fastcall CreateCode(int &TempMax, int TempStart);
	__property Rvutil::TRaveStackList* OpStack = {read=FOpStack, write=FOpStack};
	__property Rvutil::TRaveStackList* CodeList = {read=FCodeList, write=FCodeList};
	__property PCodeListItem ResultItem = {read=FResultItem};
	__property bool NeedResult = {read=FNeedResult, write=FNeedResult, nodefault};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rvleexpr */
using namespace Rvleexpr;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvLEExpr
