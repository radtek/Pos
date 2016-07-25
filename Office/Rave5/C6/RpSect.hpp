// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpSect.pas' rev: 6.00

#ifndef RpSectHPP
#define RpSectHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rpsect
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TDistMethod { dmPage, dmMargin, dmSpecial };
#pragma option pop

#pragma option push -b-
enum THeightMethod { hmUnits, hmLines };
#pragma option pop

class DELPHICLASS TReportSection;
class PASCALIMPLEMENTATION TReportSection : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	double FLeft;
	double FRight;
	double FTop;
	double FBottom;
	double FMinHeight;
	TDistMethod FLeftMethod;
	TDistMethod FRightMethod;
	TDistMethod FTopMethod;
	TDistMethod FBottomMethod;
	THeightMethod FHeightMethod;
	bool FEnabled;
	int FTabIndex;
	int FFontIndex;
	bool FNoNewPage;
	virtual void __fastcall AssignTo(Classes::TPersistent* Dest);
	void __fastcall SetLeft(double Value);
	void __fastcall SetRight(double Value);
	void __fastcall SetTop(double Value);
	void __fastcall SetBottom(double Value);
	void __fastcall SetMinHeight(double Value);
	void __fastcall SetLeftMethod(TDistMethod Value);
	void __fastcall SetRightMethod(TDistMethod Value);
	void __fastcall SetTopMethod(TDistMethod Value);
	void __fastcall SetBottomMethod(TDistMethod Value);
	void __fastcall SetTabIndex(int Value);
	void __fastcall SetFontIndex(int Value);
	void __fastcall SetSections(Rpbase::TBaseReport* BaseReport);
	
public:
	bool __fastcall SetupSection(Rpbase::TBaseReport* BaseReport);
	__fastcall TReportSection(void);
	__fastcall virtual ~TReportSection(void);
	__property bool NoNewPage = {read=FNoNewPage, write=FNoNewPage, nodefault};
	
__published:
	__property double Left = {read=FLeft, write=SetLeft};
	__property double Right = {read=FRight, write=SetRight};
	__property double Top = {read=FTop, write=SetTop};
	__property double Bottom = {read=FBottom, write=SetBottom};
	__property double MinHeight = {read=FMinHeight, write=SetMinHeight};
	__property TDistMethod LeftMethod = {read=FLeftMethod, write=SetLeftMethod, default=1};
	__property TDistMethod RightMethod = {read=FRightMethod, write=SetRightMethod, default=1};
	__property TDistMethod TopMethod = {read=FTopMethod, write=SetTopMethod, default=2};
	__property TDistMethod BottomMethod = {read=FBottomMethod, write=SetBottomMethod, default=1};
	__property THeightMethod HeightMethod = {read=FHeightMethod, write=FHeightMethod, default=1};
	__property bool Enabled = {read=FEnabled, write=FEnabled, default=1};
	__property int TabIndex = {read=FTabIndex, write=SetTabIndex, default=0};
	__property int FontIndex = {read=FFontIndex, write=SetFontIndex, default=0};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rpsect */
using namespace Rpsect;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpSect
