// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntForms_Design.pas' rev: 6.00

#ifndef TntForms_DesignHPP
#define TntForms_DesignHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ToolsAPI.hpp>	// Pascal unit
#include <DesignIntf.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntforms_design
{
//-- type declarations -------------------------------------------------------
typedef unsigned HICON;

class DELPHICLASS TTntNewFormWizard;
class PASCALIMPLEMENTATION TTntNewFormWizard : public Toolsapi::TNotifierObject 
{
	typedef Toolsapi::TNotifierObject inherited;
	
protected:
	WideString __fastcall ThisFormName();
	virtual TMetaClass* __fastcall ThisFormClass(void) = 0 ;
	
public:
	AnsiString __fastcall GetIDString();
	virtual AnsiString __fastcall GetName();
	Toolsapi::TWizardState __fastcall GetState(void);
	void __fastcall Execute(void);
	AnsiString __fastcall GetAuthor();
	virtual AnsiString __fastcall GetComment(void) = 0 ;
	AnsiString __fastcall GetPage();
	unsigned __fastcall GetGlyph(void);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TTntNewFormWizard(void) : Toolsapi::TNotifierObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TTntNewFormWizard(void) { }
	#pragma option pop
	
private:
	void *__IOTAFormWizard;	/* Toolsapi::IOTAFormWizard */
	
public:
	operator IOTAFormWizard*(void) { return (IOTAFormWizard*)&__IOTAFormWizard; }
	operator IOTARepositoryWizard*(void) { return (IOTARepositoryWizard*)&__IOTAFormWizard; }
	operator IOTAWizard*(void) { return (IOTAWizard*)&__IOTAFormWizard; }
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall Register(void);

}	/* namespace Tntforms_design */
using namespace Tntforms_design;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntForms_Design
