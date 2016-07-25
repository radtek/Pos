// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpRenderText.pas' rev: 6.00

#ifndef RpRenderTextHPP
#define RpRenderTextHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpDefine.hpp>	// Pascal unit
#include <RpRender.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <jpeg.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rprendertext
{
//-- type declarations -------------------------------------------------------
struct TTextItem;
typedef TTextItem *PTextItem;

#pragma pack(push, 1)
struct TTextItem
{
	int Col;
	int Line;
	AnsiString Data;
} ;
#pragma pack(pop)

class DELPHICLASS TRvRenderText;
class PASCALIMPLEMENTATION TRvRenderText : public Rprender::TRPRenderStream 
{
	typedef Rprender::TRPRenderStream inherited;
	
protected:
	double FCPI;
	double FLPI;
	Classes::TList* PageList;
	double FLeftBorder;
	double FTopBorder;
	bool FFormFeed;
	AnsiString FFormFeedStr;
	double __fastcall TextWidth(AnsiString Text);
	int __fastcall ColAt(double Pos);
	int __fastcall LineAt(double Pos);
	AnsiString __fastcall MakeSpaces(Byte Count);
	void __fastcall ClearPageList(void);
	void __fastcall AddTextItem(PTextItem TextItem, int Line);
	void __fastcall WriteStr(AnsiString Value);
	virtual void __fastcall PageEnd(void);
	virtual void __fastcall LeftText(const AnsiString AText, const double AX, const double AY);
	virtual void __fastcall CenterText(const AnsiString AText, const double AX, const double AY);
	virtual void __fastcall RightText(const AnsiString AText, const double AX, const double AY);
	virtual void __fastcall PrintSpaces(const AnsiString AText, const double AX, const double AY, const double AWidth);
	virtual void __fastcall PrintRightWidth(double &AX, double &AY, AnsiString AText, double Width);
	
public:
	__fastcall virtual TRvRenderText(Classes::TComponent* AOwner);
	__fastcall virtual ~TRvRenderText(void);
	tagSIZE __fastcall GetTextExtent(AnsiString Value);
	virtual void __fastcall PrintRender(Classes::TStream* NDRStream, AnsiString OutputFileName);
	virtual void __fastcall DocBegin(void);
	virtual void __fastcall DocEnd(void);
	__property AnsiString FormFeedStr = {read=FFormFeedStr, write=FFormFeedStr};
	
__published:
	__property double TopBorder = {read=FTopBorder, write=FTopBorder};
	__property double LeftBorder = {read=FLeftBorder, write=FLeftBorder};
	__property double CPI = {read=FCPI, write=FCPI};
	__property double LPI = {read=FLPI, write=FLPI};
	__property bool FormFeed = {read=FFormFeed, write=FFormFeed, default=1};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rprendertext */
using namespace Rprendertext;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpRenderText
