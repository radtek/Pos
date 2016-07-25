// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvLDCompiler.pas' rev: 6.00

#ifndef QRvLDCompilerHPP
#define QRvLDCompilerHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDefine.hpp>	// Pascal unit
#include <QRvUtil.hpp>	// Pascal unit
#include <QRvLEExpr.hpp>	// Pascal unit
#include <QRvLECode.hpp>	// Pascal unit
#include <QRvLEModule.hpp>	// Pascal unit
#include <QRvLEID.hpp>	// Pascal unit
#include <QRvLEType.hpp>	// Pascal unit
#include <QRvLEUtil.hpp>	// Pascal unit
#include <QRvLEDefine.hpp>	// Pascal unit
#include <QRvClass.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QStdCtrls.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvldcompiler
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TCompileMode { cmType, cmConst, cmVar, cmFunc };
#pragma option pop

#pragma option push -b-
enum TCompileLoc { clStart, clDefinition, clImplementation, clCreation, clDestruction };
#pragma option pop

class DELPHICLASS TRaveDelphiCompiler;
class PASCALIMPLEMENTATION TRaveDelphiCompiler : public Qrvlemodule::TRaveCompiler 
{
	typedef Qrvlemodule::TRaveCompiler inherited;
	
protected:
	bool DefineMode;
	TCompileLoc CompileLoc;
	Classes::TList* GlobalFreeList;
	Classes::TList* BreakStack;
	Classes::TList* ContinueStack;
	Qrvledefine::TCodeLabel ExitLabel;
	Qrvleutil::TRaveParser* Parser;
	Qrvlecode::TCodeManager* CM;
	Qrvleexpr::TExpression* Expr;
	Qrvlemodule::TRCPUModule* Module;
	int TempStart;
	int TempMax;
	Qstdctrls::TLabel* FStatusLabel;
	void __fastcall StartLoopStatement(void);
	void __fastcall FinishLoopStatement(void);
	Qrvledefine::TCodeLabelObject* __fastcall ContinueObject(void);
	Qrvledefine::TCodeLabelObject* __fastcall BreakObject(void);
	void __fastcall InitCompiler(void);
	void __fastcall CompileConst(void);
	void __fastcall GetOrdinalConst(int &Value, Qrvletype::TBaseType* &BaseType, AnsiString Terminators);
	void __fastcall GetRange(Qrvletype::TRange &Range);
	Qrvletype::TEnumeratedType* __fastcall CreateEnumeratedType(void);
	Qrvletype::TArrayType* __fastcall CreateArrayType(void);
	Qrvletype::TRecordType* __fastcall CreateRecordType(AnsiString AID);
	Qrvletype::TFuncType* __fastcall CreateFuncType(void);
	Qrvletype::TClassType* __fastcall CreateClassType(AnsiString AID);
	Qrvletype::TBaseType* __fastcall CreateTypeDef(bool Simple, AnsiString AID);
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
	virtual void __fastcall CompileStream(Classes::TStream* Stream, AnsiString FileName, Qrvlemodule::TRCPUModule* AModule, bool Definition);
	void __fastcall CompileFile(AnsiString FileName, Qrvlemodule::TRCPUModule* AModule);
	void __fastcall CompileExpr(AnsiString Source, Qrvlemodule::TRCPUModule* AModule);
	__property Qstdctrls::TLabel* StatusLabel = {read=FStatusLabel, write=FStatusLabel};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Qrvldcompiler */
using namespace Qrvldcompiler;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvLDCompiler
