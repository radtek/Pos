// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpFPrint.pas' rev: 6.00

#ifndef RpFPrintHPP
#define RpFPrintHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpCanvas.hpp>	// Pascal unit
#include <RpDevice.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rpfprint
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TFilePrinter;
class PASCALIMPLEMENTATION TFilePrinter : public Rpcanvas::TCanvasReport 
{
	typedef Rpcanvas::TCanvasReport inherited;
	
protected:
	Classes::TStream* FStream;
	Rpdefine::TStreamMode FStreamMode;
	AnsiString FInputFileName;
	int PrevPagePos;
	int CurrPagePos;
	int NextPagePos;
	double PaperWidth;
	double PaperHeight;
	int FileVer;
	bool ReuseGraphic;
	bool FIgnoreFileSettings;
	Rpdefine::TOrientation NextOrientation;
	Classes::TStringList* VarList;
	bool JobHeaderRead;
	int __fastcall GetPages(void);
	Byte __fastcall ReadByte(void);
	short __fastcall ReadInteger(void);
	int __fastcall ReadLongint(void);
	AnsiString __fastcall ReadText();
	double __fastcall ReadFloat(void);
	void __fastcall ReadXY(double &X, double &Y);
	void __fastcall LoadGraphic(Graphics::TGraphic* Value);
	void __fastcall ReadGraphic(Graphics::TGraphic* Value);
	void __fastcall CreateGraphic(Graphics::TGraphic* &Value);
	Types::TPoint __fastcall ReadPoint();
	Types::TRect __fastcall ReadRect();
	void __fastcall ReadBuf(void *Buf, int Len);
	void __fastcall MatchCode(Byte Value);
	void __fastcall ReadBrush(void);
	void __fastcall ReadFont(void);
	void __fastcall ReadPen(void);
	void __fastcall ReadJobHeader(void);
	void __fastcall ReadPageHeader(void);
	AnsiString __fastcall ReadPIVar();
	void __fastcall ClearVarList(void);
	bool __fastcall InitInputStream(void);
	virtual void __fastcall GotoPage(Word PageNum);
	void __fastcall SetStream(Classes::TStream* Value);
	void __fastcall SetStreamMode(Rpdefine::TStreamMode Value);
	
public:
	__fastcall virtual TFilePrinter(Classes::TComponent* AOwner);
	__fastcall virtual ~TFilePrinter(void);
	virtual AnsiString __fastcall GetPIVar(AnsiString VarName);
	virtual void __fastcall SetPIVar(AnsiString VarName, AnsiString VarValue);
	virtual void __fastcall PrintPage(Word PageNum);
	void __fastcall ExecuteCustom(int NewFirstPage, int NewLastPage, int NewCopies);
	virtual void __fastcall Execute(void);
	virtual void __fastcall Start(void);
	virtual void __fastcall Finish(void);
	__property int Pages = {read=GetPages, nodefault};
	__property Classes::TStream* Stream = {read=FStream, write=SetStream};
	__property bool IgnoreFileSettings = {read=FIgnoreFileSettings, write=FIgnoreFileSettings, nodefault};
	
__published:
	__property AnsiString FileName = {read=FInputFileName, write=FInputFileName};
	__property Rpdefine::TStreamMode StreamMode = {read=FStreamMode, write=SetStreamMode, default=0};
	__property OnDecodeImage ;
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rpfprint */
using namespace Rpfprint;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpFPrint
