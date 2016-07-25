// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Rprinter.pas' rev: 6.00

#ifndef RprinterHPP
#define RprinterHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpCanvas.hpp>	// Pascal unit
#include <RpDevice.hpp>	// Pascal unit
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

namespace Rprinter
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TReportPrinter;
class PASCALIMPLEMENTATION TReportPrinter : public Rpcanvas::TCanvasReport 
{
	typedef Rpcanvas::TCanvasReport inherited;
	
public:
	__fastcall virtual TReportPrinter(Classes::TComponent* AOwner);
	__fastcall virtual ~TReportPrinter(void);
	virtual void __fastcall Start(void);
	virtual void __fastcall Finish(void);
	virtual void __fastcall NewPage(void);
	
__published:
	__property FirstPage  = {default=1};
	__property LastPage  = {default=9999};
	__property StatusLabel ;
	__property StatusFormat ;
	__property StatusText ;
	__property LineHeightMethod  = {default=1};
	__property LinesPerInch  = {default=6};
	__property Units  = {default=0};
	__property UnitsFactor ;
	__property MarginLeft ;
	__property MarginRight ;
	__property MarginTop ;
	__property MarginBottom ;
	__property TabShade  = {default=0};
	__property Title ;
	__property TextBKMode  = {default=0};
	__property Copies  = {default=1};
	__property Orientation ;
	__property ScaleX ;
	__property ScaleY ;
	__property OnPrint ;
	__property OnPrintPage ;
	__property OnBeforePrint ;
	__property OnAfterPrint ;
	__property OnNewPage ;
	__property OnNewColumn ;
	__property OnPrintHeader ;
	__property OnPrintFooter ;
	__property OnEndOfSection ;
	__property OnDecodeImage ;
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rprinter */
using namespace Rprinter;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Rprinter
