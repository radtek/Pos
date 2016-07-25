// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvLEExpr.pas' rev: 6.00

#ifndef QRvLEExprHPP
#define QRvLEExprHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRvClass.hpp>	// Pascal unit
#include <QRvUtil.hpp>	// Pascal unit
#include <QRvLECode.hpp>	// Pascal unit
#include <QRvLEModule.hpp>	// Pascal unit
#include <QRvLEID.hpp>	// Pascal unit
#include <QRvLEType.hpp>	// Pascal unit
#include <QRvLEUtil.hpp>	// Pascal unit
#include <QRvLEDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvleexpr
{
//-- type declarations -------------------------------------------------------
struct TCodeListItem;
typedef TCodeListItem *PCodeListItem;

#pragma pack(push, 1)
struct TCodeListItem
{
	Qrvletype::TBaseType* FromType;
	Qrvletype::TBaseType* ToType;
	Word TempSize;
	int TempLoc;
	int Line;
	Word Col;
	Qrvledefine::TIDKind Kind;
	Qrvledefine::TAddress Address;
	bool DefaultParam;
	bool FuncResult;
	Qrvleid::TBaseID* VarID;
	Qrvutil::TRaveStackList* OpList;
	Qrvletype::TFuncType* FuncType;
	int FuncModule;
	Qrvledefine::TCodeLabel *FuncCodeLabel;
	bool FuncMethod;
	Word NumParams;
	Qrvledefine::TOpKind OpKind;
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
	Qrvledefine::TOpKind Kind;
	Qrvletype::TFuncType* OSFuncType;
	Word OSFuncModule;
	Qrvledefine::TCodeLabel *OSFuncCodeLabel;
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
	Qrvletype::TBaseType* BaseType;
	Qrvledefine::TAddress Address;
} ;
#pragma pack(pop)

typedef TFreeVar *PFreeVar;

class DELPHICLASS TExpression;
class PASCALIMPLEMENTATION TExpression : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Qrvutil::TRaveStackList* FCodeList;
	Qrvutil::TRaveStackList* FOpStack;
	TCodeListItem *FResultItem;
	bool FNeedResult;
	Qrvutil::TRaveStackList* CalcStack;
	Word CurrLevel;
	Qrvleutil::TRaveParser* Parser;
	Qrvlecode::TCodeManager* CM;
	Qrvlemodule::TRCPUModule* Module;
	bool __fastcall CalcOpResult(Qrvledefine::TTypeKind &TK1, Qrvledefine::TTypeKind &TK2, Qrvledefine::TTypeKind &TK3, Qrvledefine::TTypeKind &TK4, Qrvledefine::TOpKind OpKind);
	Qrvletype::TBaseType* __fastcall GetBaseType(Qrvledefine::TTypeKind TypeKind);
	void __fastcall AddCodeList(PCodeListItem CodeListItem);
	void __fastcall ProcessOpStack(Qrvledefine::TOpKind NewOpKind);
	bool __fastcall FoldConstant(POpStackItem OpStackItem, Qrvledefine::TOpKind OpKind);
	void __fastcall FreeCodeListItem(PCodeListItem CLI);
	void __fastcall PushCLIStack(Qrvutil::TRaveStackList* Stack, PCodeListItem CLI);
	void __fastcall PopCLIStack(Qrvutil::TRaveStackList* Stack);
	void __fastcall PushOSIStack(Qrvutil::TRaveStackList* Stack, POpStackItem OSI);
	void __fastcall PopOSIStack(Qrvutil::TRaveStackList* Stack);
	
public:
	__fastcall TExpression(Qrvlemodule::TRCPUModule* AModule, Qrvleutil::TRaveParser* AParser, Qrvlecode::TCodeManager* ACM);
	__fastcall virtual ~TExpression(void);
	void __fastcall CreateCodeList(AnsiString Terminators, bool LeaveResultOnStack);
	void __fastcall FreeCodeList(void);
	void __fastcall FreeType(const Qrvledefine::TAddress &Addr, Qrvletype::TBaseType* BaseType);
	void __fastcall CreateCode(int &TempMax, int TempStart);
	__property Qrvutil::TRaveStackList* OpStack = {read=FOpStack, write=FOpStack};
	__property Qrvutil::TRaveStackList* CodeList = {read=FCodeList, write=FCodeList};
	__property PCodeListItem ResultItem = {read=FResultItem};
	__property bool NeedResult = {read=FNeedResult, write=FNeedResult, nodefault};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Qrvleexpr */
using namespace Qrvleexpr;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvLEExpr
