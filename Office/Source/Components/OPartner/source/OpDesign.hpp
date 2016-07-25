// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpDesign.pas' rev: 6.00

#ifndef OpDesignHPP
#define OpDesignHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <OpMSO.hpp>	// Pascal unit
#include <TypInfo.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <OpModels.hpp>	// Pascal unit
#include <OpPower.hpp>	// Pascal unit
#include <OpOutlk.hpp>	// Pascal unit
#include <OpWord.hpp>	// Pascal unit
#include <OpExcel.hpp>	// Pascal unit
#include <OpShared.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <RTLConsts.hpp>	// Pascal unit
#include <VCLEditors.hpp>	// Pascal unit
#include <DesignEditors.hpp>	// Pascal unit
#include <DesignIntf.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opdesign
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TOpNestedCollectionEditor;
class PASCALIMPLEMENTATION TOpNestedCollectionEditor : public Designeditors::TPropertyEditor 
{
	typedef Designeditors::TPropertyEditor inherited;
	
public:
	virtual Designintf::TPropertyAttributes __fastcall GetAttributes(void);
	virtual AnsiString __fastcall GetValue();
	virtual void __fastcall Edit(void);
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TOpNestedCollectionEditor(const Designintf::_di_IDesigner ADesigner, int APropCount) : Designeditors::TPropertyEditor(ADesigner, APropCount) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TOpNestedCollectionEditor(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpModelProperty;
class PASCALIMPLEMENTATION TOpModelProperty : public Designeditors::TComponentProperty 
{
	typedef Designeditors::TComponentProperty inherited;
	
private:
	Classes::TGetStrProc FOldStrProc;
	void __fastcall FilterModels(const AnsiString S);
	
public:
	virtual void __fastcall GetValues(Classes::TGetStrProc Proc);
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TOpModelProperty(const Designintf::_di_IDesigner ADesigner, int APropCount) : Designeditors::TComponentProperty(ADesigner, APropCount) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TOpModelProperty(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpOfficeFileProperty;
class PASCALIMPLEMENTATION TOpOfficeFileProperty : public Designeditors::TStringProperty 
{
	typedef Designeditors::TStringProperty inherited;
	
public:
	virtual void __fastcall Edit(void);
	virtual Designintf::TPropertyAttributes __fastcall GetAttributes(void);
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TOpOfficeFileProperty(const Designintf::_di_IDesigner ADesigner, int APropCount) : Designeditors::TStringProperty(ADesigner, APropCount) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TOpOfficeFileProperty(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpOfficeAssistantProperty;
class PASCALIMPLEMENTATION TOpOfficeAssistantProperty : public Designeditors::TStringProperty 
{
	typedef Designeditors::TStringProperty inherited;
	
public:
	virtual void __fastcall Edit(void);
	virtual Designintf::TPropertyAttributes __fastcall GetAttributes(void);
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TOpOfficeAssistantProperty(const Designintf::_di_IDesigner ADesigner, int APropCount) : Designeditors::TStringProperty(ADesigner, APropCount) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TOpOfficeAssistantProperty(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpMachineNameProperty;
class PASCALIMPLEMENTATION TOpMachineNameProperty : public Designeditors::TStringProperty 
{
	typedef Designeditors::TStringProperty inherited;
	
public:
	virtual void __fastcall Edit(void);
	virtual Designintf::TPropertyAttributes __fastcall GetAttributes(void);
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TOpMachineNameProperty(const Designintf::_di_IDesigner ADesigner, int APropCount) : Designeditors::TStringProperty(ADesigner, APropCount) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TOpMachineNameProperty(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpOfficeComponentInfo;
class PASCALIMPLEMENTATION TOpOfficeComponentInfo : public Designeditors::TComponentEditor 
{
	typedef Designeditors::TComponentEditor inherited;
	
public:
	virtual int __fastcall GetVerbCount(void);
	virtual AnsiString __fastcall GetVerb(int index);
	virtual void __fastcall ExecuteVerb(int index);
	virtual void __fastcall Edit(void);
public:
	#pragma option push -w-inl
	/* TComponentEditor.Create */ inline __fastcall virtual TOpOfficeComponentInfo(Classes::TComponent* AComponent, Designintf::_di_IDesigner ADesigner) : Designeditors::TComponentEditor(AComponent, ADesigner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TOpOfficeComponentInfo(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall Register(void);

}	/* namespace Opdesign */
using namespace Opdesign;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpDesign
