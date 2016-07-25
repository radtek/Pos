// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntDesignEditors_Design.pas' rev: 6.00

#ifndef TntDesignEditors_DesignHPP
#define TntDesignEditors_DesignHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <VCLEditors.hpp>	// Pascal unit
#include <DesignEditors.hpp>	// Pascal unit
#include <DesignIntf.hpp>	// Pascal unit
#include <TypInfo.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntdesigneditors_design
{
//-- type declarations -------------------------------------------------------
typedef IDesigner ICreateDesigner;
;

class DELPHICLASS TWideStringProperty;
class PASCALIMPLEMENTATION TWideStringProperty : public Designeditors::TPropertyEditor 
{
	typedef Designeditors::TPropertyEditor inherited;
	
private:
	bool FActivateWithoutGetValue;
	Designeditors::TInstProp *FPropList;
	
protected:
	virtual void __fastcall SetPropEntry(int Index, Classes::TPersistent* AInstance, Typinfo::PPropInfo APropInfo);
	WideString __fastcall GetWideStrValueAt(int Index);
	WideString __fastcall GetWideStrValue();
	void __fastcall SetWideStrValue(const WideString Value);
	WideString __fastcall GetWideVisualValue();
	
public:
	__fastcall virtual TWideStringProperty(const Designintf::_di_IDesigner ADesigner, int APropCount);
	__fastcall virtual ~TWideStringProperty(void);
	virtual void __fastcall Activate(void);
	void __fastcall PropDrawName(Graphics::TCanvas* ACanvas, const Types::TRect &ARect, bool ASelected);
	void __fastcall PropDrawValue(Graphics::TCanvas* ACanvas, const Types::TRect &ARect, bool ASelected);
	virtual bool __fastcall AllEqual(void);
	virtual int __fastcall GetEditLimit(void);
	virtual AnsiString __fastcall GetValue();
	virtual void __fastcall SetValue(const AnsiString Value);
private:
	void *__ICustomPropertyDrawing;	/* Vcleditors::ICustomPropertyDrawing */
	
public:
	operator ICustomPropertyDrawing*(void) { return (ICustomPropertyDrawing*)&__ICustomPropertyDrawing; }
	
};


class DELPHICLASS TWideCaptionProperty;
class PASCALIMPLEMENTATION TWideCaptionProperty : public TWideStringProperty 
{
	typedef TWideStringProperty inherited;
	
public:
	virtual Designintf::TPropertyAttributes __fastcall GetAttributes(void);
public:
	#pragma option push -w-inl
	/* TWideStringProperty.Create */ inline __fastcall virtual TWideCaptionProperty(const Designintf::_di_IDesigner ADesigner, int APropCount) : TWideStringProperty(ADesigner, APropCount) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TWideStringProperty.Destroy */ inline __fastcall virtual ~TWideCaptionProperty(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall Register(void);
extern PACKAGE Forms::TCustomForm* __fastcall GetObjectInspectorForm(void);
extern PACKAGE void __fastcall EditPropertyWithDialog(Classes::TPersistent* Component, const AnsiString PropName, const Designintf::_di_IDesigner Designer);

}	/* namespace Tntdesigneditors_design */
using namespace Tntdesigneditors_design;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntDesignEditors_Design
