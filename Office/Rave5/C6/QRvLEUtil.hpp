// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvLEUtil.pas' rev: 6.00

#ifndef QRvLEUtilHPP
#define QRvLEUtilHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDefine.hpp>	// Pascal unit
#include <QRvUtil.hpp>	// Pascal unit
#include <QRvClass.hpp>	// Pascal unit
#include <QRvLEDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QComCtrls.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvleutil
{
//-- type declarations -------------------------------------------------------
typedef SmallString<255>  TTKStr;

class DELPHICLASS TRaveParser;
class PASCALIMPLEMENTATION TRaveParser : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	char *Buf;
	char *BufPtr;
	Qrvledefine::TTokenType FTokenType;
	TTKStr FTokenName;
	AnsiString FTokenUpper;
	bool FKeepToken;
	int FCurrCol;
	int FCurrLine;
	int FTokenCol;
	int FTokenLine;
	int FTokenInt;
	Extended FTokenExt;
	System::Currency FTokenCurr;
	void __fastcall SetText(AnsiString Value);
	
public:
	__fastcall virtual ~TRaveParser(void);
	void __fastcall LoadFromStream(Classes::TStream* Stream);
	void __fastcall LoadFromFile(AnsiString FileName);
	void __fastcall GetToken(void);
	AnsiString __fastcall GetID();
	bool __fastcall ValidToken(Qrvledefine::TTokenTypeSet ValidTypes);
	bool __fastcall MatchID(AnsiString ID);
	bool __fastcall MatchTerm(AnsiString ID, AnsiString Term);
	bool __fastcall MatchSymbol(AnsiString Symbol);
	void __fastcall ReadUntil(AnsiString ID);
	__property AnsiString Text = {write=SetText};
	__property Qrvledefine::TTokenType TokenType = {read=FTokenType, write=FTokenType, nodefault};
	__property TTKStr TokenName = {read=FTokenName, write=FTokenName};
	__property AnsiString TokenUpper = {read=FTokenUpper, write=FTokenUpper};
	__property bool KeepToken = {read=FKeepToken, write=FKeepToken, nodefault};
	__property int CurrCol = {read=FCurrCol, write=FCurrCol, nodefault};
	__property int CurrLine = {read=FCurrLine, write=FCurrLine, nodefault};
	__property int TokenCol = {read=FTokenCol, write=FTokenCol, nodefault};
	__property int TokenLine = {read=FTokenLine, write=FTokenLine, nodefault};
	__property int TokenInt = {read=FTokenInt, write=FTokenInt, nodefault};
	__property Extended TokenExt = {read=FTokenExt, write=FTokenExt};
	__property System::Currency TokenCurr = {read=FTokenCurr, write=FTokenCurr};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveParser(void) : System::TObject() { }
	#pragma option pop
	
};


typedef bool __fastcall (*TBolCaller)(int Proc, void *StackData, int StackItems);

typedef Shortint __fastcall (*TShtCaller)(int Proc, void *StackData, int StackItems);

typedef Byte __fastcall (*TBytCaller)(int Proc, void *StackData, int StackItems);

typedef short __fastcall (*TSmlCaller)(int Proc, void *StackData, int StackItems);

typedef Word __fastcall (*TWrdCaller)(int Proc, void *StackData, int StackItems);

typedef int __fastcall (*TIntCaller)(int Proc, void *StackData, int StackItems);

typedef float __fastcall (*TSngCaller)(int Proc, void *StackData, int StackItems);

typedef double __fastcall (*TDblCaller)(int Proc, void *StackData, int StackItems);

typedef Extended __fastcall (*TExtCaller)(int Proc, void *StackData, int StackItems);

typedef System::Currency __fastcall (*TCurCaller)(int Proc, void *StackData, int StackItems);

typedef char __fastcall (*TChrCaller)(int Proc, void *StackData, int StackItems);

typedef void * __fastcall (*TPtrCaller)(int Proc, void *StackData, int StackItems);

typedef System::TObject* __fastcall (*TClassCaller)(int Proc, void *StackData, int StackItems);

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool IncludeErrPos;
extern PACKAGE void __fastcall RaiseError(AnsiString ErrorMessage);
extern PACKAGE void __fastcall FuncCaller(void);
extern PACKAGE void __fastcall RecCaller(int Proc, void *StackData, int StackItems, void *RecordData);
extern PACKAGE void __fastcall AddError(int Num, AnsiString Msg);

}	/* namespace Qrvleutil */
using namespace Qrvleutil;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvLEUtil
