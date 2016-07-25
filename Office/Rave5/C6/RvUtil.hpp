// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvUtil.pas' rev: 6.00

#ifndef RvUtilHPP
#define RvUtilHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpDefine.hpp>	// Pascal unit
#include <RvDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <ImgList.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvutil
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRaveStackList;
class PASCALIMPLEMENTATION TRaveStackList : public Classes::TList 
{
	typedef Classes::TList inherited;
	
protected:
	bool __fastcall GetEmpty(void);
	void * __fastcall GetTop(void);
	
public:
	void __fastcall Push(void * Item);
	void * __fastcall Pop(void);
	void * __fastcall Peek(int Down);
	__property bool Empty = {read=GetEmpty, nodefault};
	__property void * Top = {read=GetTop};
public:
	#pragma option push -w-inl
	/* TList.Destroy */ inline __fastcall virtual ~TRaveStackList(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveStackList(void) : Classes::TList() { }
	#pragma option pop
	
};


class DELPHICLASS TStreamHelper;
class PASCALIMPLEMENTATION TStreamHelper : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Classes::TStream* FStream;
	Byte FCompressMethod;
	int LastPos;
	TRaveStackList* StreamStack;
	int __fastcall CreateMarker(void);
	void __fastcall UpdateMarker(int Marker);
	
public:
	__fastcall TStreamHelper(Classes::TStream* AStream);
	__fastcall virtual ~TStreamHelper(void);
	Classes::TStream* __fastcall PopStream(void);
	void __fastcall PushStream(Classes::TStream* AStream);
	void __fastcall SavePos(void);
	void __fastcall RestorePos(void);
	void __fastcall WriteByte(Byte Value);
	void __fastcall WriteString(AnsiString Value);
	void __fastcall WriteIndex(int Value);
	void __fastcall WriteBuf(void *Buf, int Len);
	void __fastcall StartWriteBlock(void);
	void __fastcall FinishWriteBlock(void);
	void __fastcall StartReadBlock(void);
	void __fastcall FinishReadBlock(void);
	void __fastcall SaveBlock(Classes::TStream* Source);
	void __fastcall LoadBlock(Classes::TStream* Dest);
	void __fastcall SkipBlock(void);
	bool __fastcall Empty(void);
	Byte __fastcall ReadByte(void);
	AnsiString __fastcall ReadString();
	int __fastcall ReadIndex(void);
	void __fastcall ReadBuf(void *Buf, int Len);
	__property Classes::TStream* Stream = {read=FStream};
	__property Byte CompressMethod = {read=FCompressMethod, write=FCompressMethod, nodefault};
};


//-- var, const, procedure ---------------------------------------------------
static const Shortint NoCompression = 0x0;
static const Shortint CompressMethod1 = 0x1;
static const Shortint CompressMethod2 = 0x2;
extern PACKAGE int DefaultCompressMethod;
extern PACKAGE void __fastcall (*CompressStreamProc)(Byte CompressMethod, Classes::TStream* InStream, Classes::TStream* OutStream);
extern PACKAGE void __fastcall (*UncompressStreamProc)(Byte CompressMethod, Classes::TStream* InStream, Classes::TStream* OutStream);
extern PACKAGE void __fastcall CloneObject(Classes::TPersistent* Source, Classes::TPersistent* Dest);
extern PACKAGE void __fastcall MergeMainMenu(Menus::TMenu* MainMenu, Menus::TMenu* SubMenu);
extern PACKAGE Graphics::TColor __fastcall ModColor(Graphics::TColor Color);
extern PACKAGE AnsiString __fastcall ValidID(AnsiString Value);
extern PACKAGE void __fastcall CopyList(Classes::TList* FromList, Classes::TList* ToList);
extern PACKAGE void __fastcall AbortReport(Classes::TComponent* Component);
extern PACKAGE AnsiString __fastcall CutWord(AnsiString &Value, char &SepCh, AnsiString SepStr);
extern PACKAGE AnsiString __fastcall StripJoinChars(AnsiString Value);
extern PACKAGE AnsiString __fastcall AddJoinChars(AnsiString Value);
extern PACKAGE AnsiString __fastcall ExtractQuotedString(AnsiString &Value, char Quote);
extern PACKAGE void __fastcall SetControlsEnabled(bool State, const Controls::TControl* const * Controls, const int Controls_Size);
extern PACKAGE int __fastcall OffsetToIndex(int Offset);
extern PACKAGE int __fastcall IndexToOffset(int Index);
extern PACKAGE void __fastcall ClearObjectList(Classes::TList* List);
extern PACKAGE void __fastcall ClearStringList(Classes::TStrings* List);
extern PACKAGE int __fastcall FindString(AnsiString InStr, AnsiString SearchString);
extern PACKAGE bool __fastcall SameStr(AnsiString S1, AnsiString S2);
extern PACKAGE AnsiString __fastcall Flt2Str(double Value, Byte Digits);
extern PACKAGE int __fastcall MinI(int I1, int I2);
extern PACKAGE int __fastcall MaxI(int I1, int I2);
extern PACKAGE void __fastcall MinMaxI(int &Out1, int &Out2, int In1, int In2);
extern PACKAGE double __fastcall MinF(double F1, double F2);
extern PACKAGE double __fastcall MaxF(double F1, double F2);
extern PACKAGE void __fastcall MinMaxF(double &Out1, double &Out2, double In1, double In2);
extern PACKAGE bool __fastcall FEQ(double F1, double F2);
extern PACKAGE bool __fastcall FGT(double F1, double F2);
extern PACKAGE bool __fastcall FLT(double F1, double F2);
extern PACKAGE bool __fastcall FNE(double F1, double F2);
extern PACKAGE bool __fastcall FLE(double F1, double F2);
extern PACKAGE bool __fastcall FGE(double F1, double F2);
extern PACKAGE Rvdefine::TRaveRect __fastcall RaveRect(Rvdefine::TRaveUnits Left, Rvdefine::TRaveUnits Top, Rvdefine::TRaveUnits Right, Rvdefine::TRaveUnits Bottom);
extern PACKAGE Rvdefine::TRavePoint __fastcall RavePoint(Rvdefine::TRaveUnits X, Rvdefine::TRaveUnits Y);
extern PACKAGE bool __fastcall PtInRaveRect(const Rvdefine::TRaveRect &Rect, const Rvdefine::TRavePoint &Point);
extern PACKAGE bool __fastcall IntersectRaveRect(Rvdefine::TRaveRect &SharedRect, const Rvdefine::TRaveRect &Rect1, const Rvdefine::TRaveRect &Rect2);
extern PACKAGE AnsiString __fastcall StripFloatStr(AnsiString Value);
extern PACKAGE double __fastcall StrToFloatDef(AnsiString Value, double Def);
extern PACKAGE System::Currency __fastcall StrToCurrDef(AnsiString Value, System::Currency Def);
extern PACKAGE System::TDateTime __fastcall StrToDateTimeDef(AnsiString Value, System::TDateTime Def);

}	/* namespace Rvutil */
using namespace Rvutil;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvUtil
