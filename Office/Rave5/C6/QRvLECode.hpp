// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvLECode.pas' rev: 6.00

#ifndef QRvLECodeHPP
#define QRvLECodeHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRvUtil.hpp>	// Pascal unit
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

namespace Qrvlecode
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TCodeManager;
class PASCALIMPLEMENTATION TCodeManager : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	int FSizeInc;
	Qrvlemodule::TRCPUModule* FModule;
	int FMaxSize;
	int FCodeSize;
	void *FCodeSeg;
	void __fastcall SetCodeSeg(void * Value);
	void __fastcall SetMaxSize(int Value);
	
public:
	__fastcall TCodeManager(int InitSize, Qrvlemodule::TRCPUModule* AModule);
	__fastcall virtual ~TCodeManager(void);
	void __fastcall FinishBuf(void);
	void __fastcall Write(void *Data, int DataLen, int DataPos);
	void __fastcall WriteByte(Byte Data, int &DataPos);
	int __fastcall WriteIndex(int Data, int DataPos);
	void __fastcall Append(void *Data, int DataLen);
	void __fastcall AppendByte(Byte Data);
	void __fastcall AppendCode(Byte Data);
	void __fastcall AppendIndex(int Data);
	void __fastcall AppendLength(int Length);
	void __fastcall AppendPos(int Pos);
	void __fastcall AppendOffset(int Offset);
	void __fastcall AppendAddr(const Qrvledefine::TAddress &Addr);
	Qrvledefine::TCodeLabel __fastcall GetLabel();
	Qrvledefine::TCodeLabel __fastcall CreateLabel();
	void __fastcall FixLabel(Qrvledefine::TCodeLabel &CodeLabel);
	int __fastcall GetLabelOfs(const Qrvledefine::TCodeLabel &CodeLabel);
	int __fastcall GetLabelAbs(const Qrvledefine::TCodeLabel &CodeLabel);
	void __fastcall PushAddr(const Qrvledefine::TAddress &Addr);
	void __fastcall PushMethodAddr(const Qrvledefine::TAddress &Addr);
	void __fastcall PushVar(const Qrvledefine::TAddress &Addr);
	void __fastcall PushData(const Qrvledefine::TAddress &Addr, int Length);
	void __fastcall PushConst(void *Data, int Length);
	void __fastcall PushStr(AnsiString S1, const Qrvledefine::TAddress &Addr);
	void __fastcall PushMethodStr(AnsiString S1, const Qrvledefine::TAddress &Addr);
	void __fastcall PushDefault(void);
	void __fastcall Pop(int Length);
	int __fastcall Enter(void);
	void __fastcall UpdateEnter(int Length, int EnterPos);
	void __fastcall CopyParamData(const Qrvledefine::TAddress &Addr, const Qrvledefine::TAddress &Addr2, int Length);
	void __fastcall CopyParamStr(const Qrvledefine::TAddress &Addr, const Qrvledefine::TAddress &Addr2);
	void __fastcall CopyParamDataDef(const Qrvledefine::TAddress &Addr, const Qrvledefine::TAddress &Addr2, int Length, int DefaultValue);
	void __fastcall CopyParamStrDef(const Qrvledefine::TAddress &Addr, const Qrvledefine::TAddress &Addr2, int DefaultValue);
	void __fastcall FreeStr(const Qrvledefine::TAddress &Addr);
	void __fastcall FreeRec(const Qrvledefine::TAddress &Addr, int ModuleIndex, int TypeIndex);
	void __fastcall LoadVar(const Qrvledefine::TAddress &Addr);
	void __fastcall LoadData(const Qrvledefine::TAddress &Addr, int Length);
	void __fastcall Return(int Length, bool IsResult);
	void __fastcall Call(const Qrvledefine::TCodeLabel &CodeLabel);
	int __fastcall CallExternalFunc(Qrvledefine::TTypeKind TypeKind, const Qrvledefine::TAddress &Addr);
	int __fastcall CallExternalProc(void);
	void __fastcall LongCall(int Module, int Offset);
	void __fastcall JumpEQ(const Qrvledefine::TCodeLabel &CodeLabel);
	void __fastcall JumpNE(const Qrvledefine::TCodeLabel &CodeLabel);
	void __fastcall JumpLT(const Qrvledefine::TCodeLabel &CodeLabel);
	void __fastcall JumpLE(const Qrvledefine::TCodeLabel &CodeLabel);
	void __fastcall JumpGT(const Qrvledefine::TCodeLabel &CodeLabel);
	void __fastcall JumpGE(const Qrvledefine::TCodeLabel &CodeLabel);
	void __fastcall Jump(const Qrvledefine::TCodeLabel &CodeLabel);
	void __fastcall LongJump(int Module, int Offset);
	void __fastcall Test(void);
	void __fastcall RaveMath(Byte Command, const Qrvledefine::TAddress &Addr);
	bool __fastcall ConvType(Qrvledefine::TTypeKind TK1, Qrvledefine::TTypeKind TK2, const Qrvledefine::TAddress &Addr);
	void __fastcall Assign(Byte Command, int Size);
	void __fastcall StrAssign(Byte Command);
	void __fastcall MathAssign(Byte Command);
	void __fastcall NOP(void);
	void __fastcall Terminate(void);
	__property Qrvlemodule::TRCPUModule* Module = {read=FModule};
	__property int SizeInc = {read=FSizeInc, write=FSizeInc, nodefault};
	__property void * CodeSeg = {read=FCodeSeg, write=SetCodeSeg};
	__property int MaxSize = {read=FMaxSize, write=SetMaxSize, nodefault};
	__property int CodeSize = {read=FCodeSize, nodefault};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Qrvlecode */
using namespace Qrvlecode;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvLECode
