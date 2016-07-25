// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvLEDefine.pas' rev: 6.00

#ifndef RvLEDefineHPP
#define RvLEDefineHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvledefine
{
//-- type declarations -------------------------------------------------------
typedef AnsiString TIdentifier;

typedef void * *PIntList;

typedef int TIntList[16380];

struct TVMT;
typedef TVMT *PVMT;

#pragma pack(push, 4)
struct TVMT
{
	TVMT *ParentVMT;
	void *Module;
	void * *MethodList;
	AnsiString ClassName;
	void *ClassType;
} ;
#pragma pack(pop)

#pragma option push -b-
enum TTokenType { ttID, ttString, ttInteger, ttFloat, ttCurrency, ttOperator, ttEOF };
#pragma option pop

typedef Set<TTokenType, ttID, ttEOF>  TTokenTypeSet;

#pragma option push -b-
enum TScope { scPrivate, scProtected, scPublic };
#pragma option pop

typedef Set<TScope, scPrivate, scPublic>  TScopeSet;

#pragma option push -b-
enum TIDKind { idkVar, idkType, idkConst, idkFunc, idkMethod, idkOp };
#pragma option pop

#pragma option push -b-
enum TTypeStruc { tsSimple, tsEnumerated, tsArray, tsRecord, tsFunction, tsClass };
#pragma option pop

#pragma option push -b-
enum TTypeKind { tkBoolean, tkShortint, tkByte, tkSmallint, tkWord, tkInteger, tkSingle, tkDouble, tkExtended, tkCurrency, tkChar, tkString, tkPointer, tkComplex };
#pragma option pop

typedef Set<TTypeKind, tkBoolean, tkComplex>  TypeKindSet;

#pragma option push -b-
enum TMemberType { mtElement, mtField, mtMethod, mtProperty };
#pragma option pop

#pragma option push -b-
enum TOpKind { okTerm, okAssign, okAddAssign, okSubAssign, okMulAssign, okDivAssign, okEQ, okNE, okLT, okGT, okLE, okGE, okIS, okAdd, okSub, okOR, okXOR, okMul, okDiv, okIDiv, okMod, okAND, okSHL, okSHR, okAs, okPos, okNeg, okNOT, okLeftAssign, okRightAssign, okFunction, okMethod };
#pragma option pop

#pragma option push -b-
enum TAddressLoc { alBP, alSP, alDS };
#pragma option pop

#pragma pack(push, 4)
struct TAddress
{
	TAddressLoc Loc;
	Word Module;
	int Offset;
} ;
#pragma pack(pop)

struct TCodeLabel;
typedef TCodeLabel *PCodeLabel;

#pragma pack(push, 4)
struct TCodeLabel
{
	int Offset;
	Classes::TList* AssignList;
} ;
#pragma pack(pop)

class DELPHICLASS TCodeLabelObject;
class PASCALIMPLEMENTATION TCodeLabelObject : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	TCodeLabel CodeLabel;
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TCodeLabelObject(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TCodeLabelObject(void) { }
	#pragma option pop
	
};


struct TExternalFunc;
typedef TExternalFunc *PExternalFunc;

#pragma pack(push, 4)
struct TExternalFunc
{
	int Offset;
} ;
#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
static const Shortint MAXIDLENGTH = 0x40;
extern PACKAGE bool UseWideChar;
static const Shortint RAVE_STACK = 0x0;
static const Shortint RAVEPUSHADDR = 0x1;
static const Shortint RAVEPUSHMETHODADDR = 0x2;
static const Shortint RAVEPUSHVAR = 0x3;
static const Shortint RAVEPUSHDATA = 0x4;
static const Shortint RAVEPUSHCONST = 0x5;
static const Shortint RAVEPUSHSTR = 0x6;
static const Shortint RAVEPUSHMETHODSTR = 0x7;
static const Shortint RAVEPUSHDEFAULT = 0x8;
static const Shortint RAVEPOP = 0x9;
static const Shortint RAVE_CALL = 0x9;
static const Shortint RAVEENTER = 0xa;
static const Shortint RAVECOPYPARAMDATA = 0xb;
static const Shortint RAVECOPYPARAMSTR = 0xc;
static const Shortint RAVECOPYPARAMDATADEF = 0xd;
static const Shortint RAVECOPYPARAMSTRDEF = 0xe;
static const Shortint RAVEFREESTR = 0xf;
static const Shortint RAVEFREEREC = 0x10;
static const Shortint RAVELOADVAR = 0x11;
static const Shortint RAVELOADDATA = 0x12;
static const Shortint RAVERETURN = 0x13;
static const Shortint RAVERETURNRES = 0x14;
static const Shortint RAVECALL = 0x15;
static const Shortint RAVECALLEXTERNALFUNC = 0x16;
static const Shortint RAVECALLEXTERNALPROC = 0x17;
static const Shortint RAVELONGCALL = 0x18;
static const Shortint RAVE_JUMP = 0x18;
static const Shortint RAVEJUMPEQ = 0x19;
static const Shortint RAVEJUMPNE = 0x1a;
static const Shortint RAVEJUMPLT = 0x1b;
static const Shortint RAVEJUMPLE = 0x1c;
static const Shortint RAVEJUMPGT = 0x1d;
static const Shortint RAVEJUMPGE = 0x1e;
static const Shortint RAVEJUMP = 0x1f;
static const Shortint RAVELONGJUMP = 0x20;
static const Shortint RAVE_COMPARE = 0x20;
static const Shortint RAVEEQBOL = 0x21;
static const Shortint RAVENEBOL = 0x22;
static const Shortint RAVELTBOL = 0x23;
static const Shortint RAVEGTBOL = 0x24;
static const Shortint RAVELEBOL = 0x25;
static const Shortint RAVEGEBOL = 0x26;
static const Shortint RAVEEQINT = 0x27;
static const Shortint RAVENEINT = 0x28;
static const Shortint RAVELTINT = 0x29;
static const Shortint RAVEGTINT = 0x2a;
static const Shortint RAVELEINT = 0x2b;
static const Shortint RAVEGEINT = 0x2c;
static const Shortint RAVEEQEXT = 0x2d;
static const Shortint RAVENEEXT = 0x2e;
static const Shortint RAVELTEXT = 0x2f;
static const Shortint RAVEGTEXT = 0x30;
static const Shortint RAVELEEXT = 0x31;
static const Shortint RAVEGEEXT = 0x32;
static const Shortint RAVEEQCUR = 0x33;
static const Shortint RAVENECUR = 0x34;
static const Shortint RAVELTCUR = 0x35;
static const Shortint RAVEGTCUR = 0x36;
static const Shortint RAVELECUR = 0x37;
static const Shortint RAVEGECUR = 0x38;
static const Shortint RAVEEQCHR = 0x39;
static const Shortint RAVENECHR = 0x3a;
static const Shortint RAVELTCHR = 0x3b;
static const Shortint RAVEGTCHR = 0x3c;
static const Shortint RAVELECHR = 0x3d;
static const Shortint RAVEGECHR = 0x3e;
static const Shortint RAVEEQSTR = 0x3f;
static const Shortint RAVENESTR = 0x40;
static const Shortint RAVELTSTR = 0x41;
static const Shortint RAVEGTSTR = 0x42;
static const Shortint RAVELESTR = 0x43;
static const Shortint RAVEGESTR = 0x44;
static const Shortint RAVE_BINARY = 0x44;
static const Shortint RAVEORBOL = 0x45;
static const Shortint RAVEXORBOL = 0x46;
static const Shortint RAVEANDBOL = 0x47;
static const Shortint RAVEADDINT = 0x48;
static const Shortint RAVESUBINT = 0x49;
static const Shortint RAVEMULINT = 0x4a;
static const Shortint RAVEDIVINT = 0x4b;
static const Shortint RAVEMODINT = 0x4c;
static const Shortint RAVEORINT = 0x4d;
static const Shortint RAVEXORINT = 0x4e;
static const Shortint RAVEANDINT = 0x4f;
static const Shortint RAVESHLINT = 0x50;
static const Shortint RAVESHRINT = 0x51;
static const Shortint RAVEADDEXT = 0x52;
static const Shortint RAVESUBEXT = 0x53;
static const Shortint RAVEMULEXT = 0x54;
static const Shortint RAVEDIVEXT = 0x55;
static const Shortint RAVEADDCUR = 0x56;
static const Shortint RAVESUBCUR = 0x57;
static const Shortint RAVEMULCUR = 0x58;
static const Shortint RAVEDIVCUR = 0x59;
static const Shortint RAVE_UNARY = 0x59;
static const Shortint RAVENOTBOL = 0x5a;
static const Shortint RAVENOTINT = 0x5b;
static const Shortint RAVENEGINT = 0x5c;
static const Shortint RAVENEGEXT = 0x5d;
static const Shortint RAVENEGCUR = 0x5e;
static const Shortint RAVE_CONV = 0x5e;
static const Shortint RAVESHT2INT = 0x5f;
static const Shortint RAVEBYT2INT = 0x60;
static const Shortint RAVESML2INT = 0x61;
static const Shortint RAVEWRD2INT = 0x62;
static const Shortint RAVEINT2SHT = 0x63;
static const Shortint RAVEINT2BYT = 0x64;
static const Shortint RAVEINT2SML = 0x65;
static const Shortint RAVEINT2WRD = 0x66;
static const Shortint RAVEINT2SNG = 0x67;
static const Shortint RAVEINT2DBL = 0x68;
static const Shortint RAVEINT2EXT = 0x69;
static const Shortint RAVEINT2CUR = 0x6a;
static const Shortint RAVESHT2EXT = 0x6b;
static const Shortint RAVEBYT2EXT = 0x6c;
static const Shortint RAVESML2EXT = 0x6d;
static const Shortint RAVEWRD2EXT = 0x6e;
static const Shortint RAVESNG2EXT = 0x6f;
static const Shortint RAVEDBL2EXT = 0x70;
static const Shortint RAVECUR2EXT = 0x71;
static const Shortint RAVEEXT2SNG = 0x72;
static const Shortint RAVEEXT2DBL = 0x73;
static const Shortint RAVEEXT2CUR = 0x74;
static const Shortint RAVECHR2STR = 0x75;
static const Shortint RAVEDOUBLECONV = 0x76;
static const Shortint RAVE_ASSIGN = 0x76;
static const Shortint RAVEASSIGN = 0x77;
static const Shortint RAVELEFTASSIGN = 0x78;
static const Shortint RAVERIGHTASSIGN = 0x79;
static const Shortint RAVEASSIGNSTR = 0x7a;
static const Shortint RAVELEFTASSIGNSTR = 0x7b;
static const Shortint RAVERIGHTASSIGNSTR = 0x7c;
static const Shortint RAVEADDINTASSIGN = 0x7d;
static const Shortint RAVESUBINTASSIGN = 0x7e;
static const Shortint RAVEMULINTASSIGN = 0x7f;
static const Byte RAVEADDEXTASSIGN = 0x80;
static const Byte RAVESUBEXTASSIGN = 0x81;
static const Byte RAVEMULEXTASSIGN = 0x82;
static const Byte RAVEDIVEXTASSIGN = 0x83;
static const Byte RAVEADDCURASSIGN = 0x84;
static const Byte RAVESUBCURASSIGN = 0x85;
static const Byte RAVEMULCURASSIGN = 0x86;
static const Byte RAVEDIVCURASSIGN = 0x87;
static const Byte RAVE_MISC = 0x87;
static const Byte RAVENOP = 0x88;
static const Byte RAVETERMINATE = 0x89;
static const Byte RAVEADDSTR = 0x8a;
static const Byte RAVETEST = 0x8b;
static const Byte RAVESUSPEND = 0x8c;
static const Byte RAVE_DEBUG = 0x8c;
static const Byte RAVEBREAKPOINT = 0x8d;
#define OpChars "!\"#$%&'()*+,-./:;<=>?@[\\]^{|}~"
extern PACKAGE TScopeSet AllScopes;
#define IntegerTypes (System::Set<TTypeKind, tkBoolean, tkComplex> () << TTypeKind(1) << TTypeKind(2) << TTypeKind(3) << TTypeKind(4) << TTypeKind(5) )
#define FloatTypes (System::Set<TTypeKind, tkBoolean, tkComplex> () << TTypeKind(6) << TTypeKind(7) << TTypeKind(8) << TTypeKind(9) )
#define OrdinalTypes (System::Set<TTypeKind, tkBoolean, tkComplex> () << TTypeKind(0) << TTypeKind(1) << TTypeKind(2) << TTypeKind(3) << TTypeKind(4) << TTypeKind(5) << TTypeKind(10) )
#define StringTypes (System::Set<TTypeKind, tkBoolean, tkComplex> () << TTypeKind(10) << TTypeKind(11) )
#define MathTypes (System::Set<TTypeKind, tkBoolean, tkComplex> () << TTypeKind(1) << TTypeKind(2) << TTypeKind(3) << TTypeKind(4) << TTypeKind(5) << TTypeKind(6) << TTypeKind(7) << TTypeKind(8) << TTypeKind(9) )
#define AllTypes (System::Set<TTypeKind, tkBoolean, tkComplex> () << TTypeKind(0) << TTypeKind(1) << TTypeKind(2) << TTypeKind(3) << TTypeKind(4) << TTypeKind(5) << TTypeKind(6) << TTypeKind(7) << TTypeKind(8) << TTypeKind(9) << TTypeKind(10) << TTypeKind(11) << TTypeKind(12) )
extern PACKAGE TTypeKind UpgradeType[14];
extern PACKAGE Byte TypeSize[14];
static const Byte RAVEINVALID = 0xfe;
static const Byte RAVEDEFAULT = 0xff;
extern PACKAGE Byte TypeConv1[9][9];
extern PACKAGE Byte TypeConv2[9][9];
extern PACKAGE Byte OpOrder[32];
#define OpUnary (System::Set<TOpKind, okTerm, okMethod> () << TOpKind(25) << TOpKind(26) << TOpKind(27) )
#define OpSimpleAssign (System::Set<TOpKind, okTerm, okMethod> () << TOpKind(1) << TOpKind(28) << TOpKind(29) )
#define OpMathAssign (System::Set<TOpKind, okTerm, okMethod> () << TOpKind(2) << TOpKind(3) << TOpKind(4) << TOpKind(5) )
#define OpStatementAssign (System::Set<TOpKind, okTerm, okMethod> () << TOpKind(1) << TOpKind(2) << TOpKind(3) << TOpKind(4) << TOpKind(5) )
#define OpAssign (System::Set<TOpKind, okTerm, okMethod> () << TOpKind(1) << TOpKind(2) << TOpKind(3) << TOpKind(4) << TOpKind(5) << TOpKind(28) << TOpKind(29) )
extern PACKAGE TypeKindSet OpTypes[32];
extern PACKAGE Byte OpBolCommand[32];
extern PACKAGE Byte OpIntCommand[32];
extern PACKAGE Byte OpExtCommand[32];
extern PACKAGE Byte OpCurCommand[32];
extern PACKAGE Byte OpChrCommand[32];
extern PACKAGE Byte OpStrCommand[32];
extern PACKAGE bool OpPostConv[32];

}	/* namespace Rvledefine */
using namespace Rvledefine;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvLEDefine
