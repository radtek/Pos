// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvLDCompiler.pas' rev: 6.00

#ifndef RvLDCompilerHPP
#define RvLDCompilerHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpDefine.hpp>	// Pascal unit
#include <RvUtil.hpp>	// Pascal unit
#include <RvLEExpr.hpp>	// Pascal unit
#include <RvLECode.hpp>	// Pascal unit
#include <RvLEModule.hpp>	// Pascal unit
#include <RvLEID.hpp>	// Pascal unit
#include <RvLEType.hpp>	// Pascal unit
#include <RvLEUtil.hpp>	// Pascal unit
#include <RvLEDefine.hpp>	// Pascal unit
#include <RvClass.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvldcompiler
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TCompileMode { cmType, cmConst, cmVar, cmFunc };
#pragma option pop

#pragma option push -b-
enum TCompileLoc { clStart, clDefinition, clImplementation, clCreation, clDestruction };
#pragma option pop

class DELPHICLASS TRaveDelphiCompiler;
class PASCALIMPLEMENTATION TRaveDelphiCompiler : public Rvlemodule::TRaveCompiler 
{
	typedef Rvlemodule::TRaveCompiler inherited;
	
protected:
	bool DefineMode;
	TCompileLoc CompileLoc;
	Classes::TList* GlobalFreeList;
	Classes::TList* BreakStack;
	Classes::TList* ContinueStack;
	Rvledefine::TCodeLabel ExitLabel;
	Rvleutil::TRaveParser* Parser;
	Rvlecode::TCodeManager* CM;
	Rvleexpr::TExpression* Expr;
	Rvlemodule::TRCPUModule* Module;
	int TempStart;
	int TempMax;
	Stdctrls::TLabel* FStatusLabel;
	void __fastcall StartLoopStatement(void);
	void __fastcall FinishLoopStatement(void);
	Rvledefine::TCodeLabelObject* __fastcall ContinueObject(void);
	Rvledefine::TCodeLabelObject* __fastcall BreakObject(void);
	void __fastcall InitCompiler(void);
	void __fastcall CompileConst(void);
	void __fastcall GetOrdinalConst(int &Value, Rvletype::TBaseType* &BaseType, AnsiString Terminators);
	void __fastcall GetRange(Rvletype::TRange &Range);
	Rvletype::TEnumeratedType* __fastcall CreateEnumeratedType(void);
	Rvletype::TArrayType* __fastcall CreateArrayType(void);
	Rvletype::TRecordType* __fastcall CreateRecordType(AnsiString AID);
	Rvletype::TFuncType* __fastcall CreateFuncType(void);
	Rvletype::TClassType* __fastcall CreateClassType(AnsiString AID);
	Rvletype::TBaseType* __fastcall CreateTypeDef(bool Simple, AnsiString AID);
	void __fastcall CompileType(void);
	void __fastcall CompileExpression(void);
	void __fastcall CompileIf(void);
	void __fastcall CompileFor(void);
	void __fastcall CompileWhile(void);
	void __fastcall CompileRepeat(void);
	void __fastcall CompileBreak(void);
	void __fastcall CompileContinue(void);
	void __fastcall CompileExit(void);
	void __fastcall CompileSuspend(void);
	void __fastcall CompileWith(void);
	bool __fastcall CompileStatement(AnsiString Terminators);
	void __fastcall CompileVar(Classes::TList* FreeList);
	void __fastcall CompileFunc(void);
	void __fastcall CompileImport(void);
	void __fastcall CompileModule(void);
	void __fastcall CompileSection(bool Definition);
	
public:
	__fastcall virtual TRaveDelphiCompiler(void);
	__fastcall virtual ~TRaveDelphiCompiler(void);
	/* virtual class method */ virtual AnsiString __fastcall SyntaxName(TMetaClass* vmt);
	void __fastcall CompileSystemModule(void);
	virtual void __fastcall CompileStream(Classes::TStream* Stream, AnsiString FileName, Rvlemodule::TRCPUModule* AModule, bool Definition);
	void __fastcall CompileFile(AnsiString FileName, Rvlemodule::TRCPUModule* AModule);
	void __fastcall CompileExpr(AnsiString Source, Rvlemodule::TRCPUModule* AModule);
	__property Stdctrls::TLabel* StatusLabel = {read=FStatusLabel, write=FStatusLabel};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Rvldcompiler */
using namespace Rvldcompiler;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvLDCompiler
