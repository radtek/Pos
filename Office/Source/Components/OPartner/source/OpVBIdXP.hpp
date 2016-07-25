// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpVBIdXP.pas' rev: 6.00

#ifndef OpVBIdXPHPP
#define OpVBIdXPHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <OpOfcXP.hpp>	// Pascal unit
#include <StdVCL.hpp>	// Pascal unit
#include <OleCtrls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <ActiveX.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opvbidxp
{
//-- type declarations -------------------------------------------------------
typedef Activex::TOleEnum vbextFileTypes;

typedef Activex::TOleEnum vbext_WindowType;

typedef Activex::TOleEnum vbext_WindowState;

typedef Activex::TOleEnum vbext_ProjectType;

typedef Activex::TOleEnum vbext_ProjectProtection;

typedef Activex::TOleEnum vbext_VBAMode;

typedef Activex::TOleEnum vbext_ComponentType;

typedef Activex::TOleEnum vbext_ProcKind;

typedef Activex::TOleEnum vbext_CodePaneview;

typedef Activex::TOleEnum vbext_RefKind;

__interface _Windows_;
typedef System::DelphiInterface<_Windows_> _di__Windows_;
typedef _Windows_ Windows;
;

__interface _LinkedWindows;
typedef System::DelphiInterface<_LinkedWindows> _di__LinkedWindows;
typedef _LinkedWindows LinkedWindows;
;

__interface _ReferencesEvents;
typedef System::DelphiInterface<_ReferencesEvents> _di__ReferencesEvents;
typedef _ReferencesEvents ReferencesEvents;
;

__interface _CommandBarControlEvents;
typedef System::DelphiInterface<_CommandBarControlEvents> _di__CommandBarControlEvents;
typedef _CommandBarControlEvents CommandBarEvents;
;

__interface _ProjectTemplate;
typedef System::DelphiInterface<_ProjectTemplate> _di__ProjectTemplate;
typedef _ProjectTemplate ProjectTemplate;
;

__interface _VBProject;
typedef System::DelphiInterface<_VBProject> _di__VBProject;
typedef _VBProject VBProject;
;

__interface _VBProjects;
typedef System::DelphiInterface<_VBProjects> _di__VBProjects;
typedef _VBProjects VBProjects;
;

__interface _Components;
typedef System::DelphiInterface<_Components> _di__Components;
typedef _Components Components;
;

__interface _VBComponents;
typedef System::DelphiInterface<_VBComponents> _di__VBComponents;
typedef _VBComponents VBComponents;
;

__interface _Component;
typedef System::DelphiInterface<_Component> _di__Component;
typedef _Component Component;
;

__interface _VBComponent;
typedef System::DelphiInterface<_VBComponent> _di__VBComponent;
typedef _VBComponent VBComponent;
;

__interface _Properties;
typedef System::DelphiInterface<_Properties> _di__Properties;
typedef _Properties Properties;
;

__interface _AddIns;
typedef System::DelphiInterface<_AddIns> _di__AddIns;
typedef _AddIns Addins;
;

__interface _CodeModule;
typedef System::DelphiInterface<_CodeModule> _di__CodeModule;
typedef _CodeModule CodeModule;
;

__interface _CodePanes;
typedef System::DelphiInterface<_CodePanes> _di__CodePanes;
typedef _CodePanes CodePanes;
;

__interface _CodePane;
typedef System::DelphiInterface<_CodePane> _di__CodePane;
typedef _CodePane CodePane;
;

__interface _References;
typedef System::DelphiInterface<_References> _di__References;
typedef _References References;
;

typedef Word *PWordBool1;

__interface Application_;
typedef System::DelphiInterface<Application_> _di_Application_;
__interface INTERFACE_UUID("{0002E158-0000-0000-C000-000000000046}") Application_  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Version(WideString &Get_Version_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Version() { WideString r; HRESULT hr = Get_Version(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Version = {read=_scw_Get_Version};
};

__dispinterface Application_Disp;
typedef System::DelphiInterface<Application_Disp> _di_Application_Disp;
__dispinterface INTERFACE_UUID("{0002E158-0000-0000-C000-000000000046}") Application_Disp  : public IDispatch 
{
	
};

__interface VBE;
typedef System::DelphiInterface<VBE> _di_VBE;
__interface Events;
typedef System::DelphiInterface<Events> _di_Events;
__interface Window_;
typedef System::DelphiInterface<Window_> _di_Window_;
__interface INTERFACE_UUID("{0002E166-0000-0000-C000-000000000046}") VBE  : public Application_ 
{
	
public:
	virtual HRESULT __safecall Get_VBProjects(_di__VBProjects &Get_VBProjects_result) = 0 ;
	virtual HRESULT __safecall Get_CommandBars(Opofcxp::_di__CommandBars &Get_CommandBars_result) = 0 ;
	virtual HRESULT __safecall Get_CodePanes(_di__CodePanes &Get_CodePanes_result) = 0 ;
	virtual HRESULT __safecall Get_Windows(_di__Windows_ &Get_Windows_result) = 0 ;
	virtual HRESULT __safecall Get_Events(_di_Events &Get_Events_result) = 0 ;
	virtual HRESULT __safecall Get_ActiveVBProject(_di__VBProject &Get_ActiveVBProject_result) = 0 ;
	virtual HRESULT __safecall Set_ActiveVBProject(const _di__VBProject lppptReturn) = 0 ;
	virtual HRESULT __safecall Get_SelectedVBComponent(_di__VBComponent &Get_SelectedVBComponent_result) = 0 ;
	virtual HRESULT __safecall Get_MainWindow(_di_Window_ &Get_MainWindow_result) = 0 ;
	virtual HRESULT __safecall Get_ActiveWindow(_di_Window_ &Get_ActiveWindow_result) = 0 ;
	virtual HRESULT __safecall Get_ActiveCodePane(_di__CodePane &Get_ActiveCodePane_result) = 0 ;
	virtual HRESULT __safecall Set_ActiveCodePane(const _di__CodePane ppCodePane) = 0 ;
	virtual HRESULT __safecall Get_Addins(_di__AddIns &Get_Addins_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__VBProjects _scw_Get_VBProjects() { _di__VBProjects r; HRESULT hr = Get_VBProjects(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__VBProjects VBProjects = {read=_scw_Get_VBProjects};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opofcxp::_di__CommandBars _scw_Get_CommandBars() { Opofcxp::_di__CommandBars r; HRESULT hr = Get_CommandBars(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opofcxp::_di__CommandBars CommandBars = {read=_scw_Get_CommandBars};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__CodePanes _scw_Get_CodePanes() { _di__CodePanes r; HRESULT hr = Get_CodePanes(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__CodePanes CodePanes = {read=_scw_Get_CodePanes};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Windows_ _scw_Get_Windows() { _di__Windows_ r; HRESULT hr = Get_Windows(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Windows_ Windows = {read=_scw_Get_Windows};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Events _scw_Get_Events() { _di_Events r; HRESULT hr = Get_Events(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Events Events = {read=_scw_Get_Events};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__VBProject _scw_Get_ActiveVBProject() { _di__VBProject r; HRESULT hr = Get_ActiveVBProject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__VBProject ActiveVBProject = {read=_scw_Get_ActiveVBProject, write=Set_ActiveVBProject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__VBComponent _scw_Get_SelectedVBComponent() { _di__VBComponent r; HRESULT hr = Get_SelectedVBComponent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__VBComponent SelectedVBComponent = {read=_scw_Get_SelectedVBComponent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Window_ _scw_Get_MainWindow() { _di_Window_ r; HRESULT hr = Get_MainWindow(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Window_ MainWindow = {read=_scw_Get_MainWindow};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Window_ _scw_Get_ActiveWindow() { _di_Window_ r; HRESULT hr = Get_ActiveWindow(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Window_ ActiveWindow = {read=_scw_Get_ActiveWindow};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__CodePane _scw_Get_ActiveCodePane() { _di__CodePane r; HRESULT hr = Get_ActiveCodePane(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__CodePane ActiveCodePane = {read=_scw_Get_ActiveCodePane, write=Set_ActiveCodePane};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__AddIns _scw_Get_Addins() { _di__AddIns r; HRESULT hr = Get_Addins(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__AddIns Addins = {read=_scw_Get_Addins};
};

__dispinterface VBEDisp;
typedef System::DelphiInterface<VBEDisp> _di_VBEDisp;
__dispinterface INTERFACE_UUID("{0002E166-0000-0000-C000-000000000046}") VBEDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E16B-0000-0000-C000-000000000046}") Window_  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Get_Collection(_di__Windows_ &Get_Collection_result) = 0 ;
	virtual HRESULT __safecall Close(void) = 0 ;
	virtual HRESULT __safecall Get_Caption(WideString &Get_Caption_result) = 0 ;
	virtual HRESULT __safecall Get_Visible(Word &Get_Visible_result) = 0 ;
	virtual HRESULT __safecall Set_Visible(Word pfVisible) = 0 ;
	virtual HRESULT __safecall Get_Left(int &Get_Left_result) = 0 ;
	virtual HRESULT __safecall Set_Left(int plLeft) = 0 ;
	virtual HRESULT __safecall Get_Top(int &Get_Top_result) = 0 ;
	virtual HRESULT __safecall Set_Top(int plTop) = 0 ;
	virtual HRESULT __safecall Get_Width(int &Get_Width_result) = 0 ;
	virtual HRESULT __safecall Set_Width(int plWidth) = 0 ;
	virtual HRESULT __safecall Get_Height(int &Get_Height_result) = 0 ;
	virtual HRESULT __safecall Set_Height(int plHeight) = 0 ;
	virtual HRESULT __safecall Get_WindowState(Activex::TOleEnum &Get_WindowState_result) = 0 ;
	virtual HRESULT __safecall Set_WindowState(Activex::TOleEnum plWindowState) = 0 ;
	virtual HRESULT __safecall SetFocus(void) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall SetKind(Activex::TOleEnum eKind) = 0 ;
	virtual HRESULT __safecall Get_LinkedWindows(_di__LinkedWindows &Get_LinkedWindows_result) = 0 ;
	virtual HRESULT __safecall Get_LinkedWindowFrame(_di_Window_ &Get_LinkedWindowFrame_result) = 0 ;
	virtual HRESULT __safecall Detach(void) = 0 ;
	virtual HRESULT __safecall Attach(int lWindowHandle) = 0 ;
	virtual HRESULT __safecall Get_HWnd(int &Get_HWnd_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Windows_ _scw_Get_Collection() { _di__Windows_ r; HRESULT hr = Get_Collection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Windows_ Collection = {read=_scw_Get_Collection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Caption() { WideString r; HRESULT hr = Get_Caption(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Caption = {read=_scw_Get_Caption};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Visible() { Word r; HRESULT hr = Get_Visible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Visible = {read=_scw_Get_Visible, write=Set_Visible};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Left() { int r; HRESULT hr = Get_Left(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Left = {read=_scw_Get_Left, write=Set_Left};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Top() { int r; HRESULT hr = Get_Top(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Top = {read=_scw_Get_Top, write=Set_Top};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Width() { int r; HRESULT hr = Get_Width(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Width = {read=_scw_Get_Width, write=Set_Width};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Height() { int r; HRESULT hr = Get_Height(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Height = {read=_scw_Get_Height, write=Set_Height};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_WindowState() { Activex::TOleEnum r; HRESULT hr = Get_WindowState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum WindowState = {read=_scw_Get_WindowState, write=Set_WindowState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__LinkedWindows _scw_Get_LinkedWindows() { _di__LinkedWindows r; HRESULT hr = Get_LinkedWindows(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__LinkedWindows LinkedWindows = {read=_scw_Get_LinkedWindows};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Window_ _scw_Get_LinkedWindowFrame() { _di_Window_ r; HRESULT hr = Get_LinkedWindowFrame(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Window_ LinkedWindowFrame = {read=_scw_Get_LinkedWindowFrame};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_HWnd() { int r; HRESULT hr = Get_HWnd(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int HWnd = {read=_scw_Get_HWnd};
};

__dispinterface Window_Disp;
typedef System::DelphiInterface<Window_Disp> _di_Window_Disp;
__dispinterface INTERFACE_UUID("{0002E16B-0000-0000-C000-000000000046}") Window_Disp  : public IDispatch 
{
	
};

__interface _Windows_old;
typedef System::DelphiInterface<_Windows_old> _di__Windows_old;
__interface INTERFACE_UUID("{0002E16A-0000-0000-C000-000000000046}") _Windows_old  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_Application_ &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant index, _di_Window_ &Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Application_ _scw_Get_Parent() { _di_Application_ r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Application_ Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface _Windows_oldDisp;
typedef System::DelphiInterface<_Windows_oldDisp> _di__Windows_oldDisp;
__dispinterface INTERFACE_UUID("{0002E16A-0000-0000-C000-000000000046}") _Windows_oldDisp  : public IDispatch 
{
	
};

__interface AddIn;
typedef System::DelphiInterface<AddIn> _di_AddIn;
__interface INTERFACE_UUID("{F57B7ED0-D8AB-11D1-85DF-00C04F98F42C}") _Windows_  : public _Windows_old 
{
	
public:
	virtual HRESULT __safecall CreateToolWindow(const _di_AddIn AddInInst, const WideString ProgId, const WideString Caption, const WideString GuidPosition, _di_IDispatch &DocObj, _di_Window_ &CreateToolWindow_result) = 0 ;
};

__dispinterface _Windows_Disp;
typedef System::DelphiInterface<_Windows_Disp> _di__Windows_Disp;
__dispinterface INTERFACE_UUID("{F57B7ED0-D8AB-11D1-85DF-00C04F98F42C}") _Windows_Disp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E16C-0000-0000-C000-000000000046}") _LinkedWindows  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_Window_ &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant index, _di_Window_ &Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	virtual HRESULT __safecall Remove(const _di_Window_ Window_) = 0 ;
	virtual HRESULT __safecall Add(const _di_Window_ Window_) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Window_ _scw_Get_Parent() { _di_Window_ r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Window_ Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface _LinkedWindowsDisp;
typedef System::DelphiInterface<_LinkedWindowsDisp> _di__LinkedWindowsDisp;
__dispinterface INTERFACE_UUID("{0002E16C-0000-0000-C000-000000000046}") _LinkedWindowsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E167-0000-0000-C000-000000000046}") Events  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_ReferencesEvents(const _di__VBProject VBProject, _di__ReferencesEvents &Get_ReferencesEvents_result) = 0 ;
	virtual HRESULT __safecall Get_CommandBarEvents(const _di_IDispatch CommandBarControl, _di__CommandBarControlEvents &Get_CommandBarEvents_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__ReferencesEvents _scw_Get_ReferencesEvents(const _di__VBProject VBProject) { _di__ReferencesEvents r; HRESULT hr = Get_ReferencesEvents(VBProject, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__ReferencesEvents ReferencesEvents[_di__VBProject VBProject] = {read=_scw_Get_ReferencesEvents};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__CommandBarControlEvents _scw_Get_CommandBarEvents(const _di_IDispatch CommandBarControl) { _di__CommandBarControlEvents r; HRESULT hr = Get_CommandBarEvents(CommandBarControl, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__CommandBarControlEvents CommandBarEvents[_di_IDispatch CommandBarControl] = {read=_scw_Get_CommandBarEvents};
};

__dispinterface EventsDisp;
typedef System::DelphiInterface<EventsDisp> _di_EventsDisp;
__dispinterface INTERFACE_UUID("{0002E167-0000-0000-C000-000000000046}") EventsDisp  : public IDispatch 
{
	
};

__interface _VBProjectsEvents;
typedef System::DelphiInterface<_VBProjectsEvents> _di__VBProjectsEvents;
__interface INTERFACE_UUID("{0002E113-0000-0000-C000-000000000046}") _VBProjectsEvents  : public IInterface 
{
	
};

__dispinterface _dispVBProjectsEvents;
typedef System::DelphiInterface<_dispVBProjectsEvents> _di__dispVBProjectsEvents;
__dispinterface INTERFACE_UUID("{0002E103-0000-0000-C000-000000000046}") _dispVBProjectsEvents  : public IDispatch 
{
	
};

__interface _VBComponentsEvents;
typedef System::DelphiInterface<_VBComponentsEvents> _di__VBComponentsEvents;
__interface INTERFACE_UUID("{0002E115-0000-0000-C000-000000000046}") _VBComponentsEvents  : public IInterface 
{
	
};

__dispinterface _dispVBComponentsEvents;
typedef System::DelphiInterface<_dispVBComponentsEvents> _di__dispVBComponentsEvents;
__dispinterface INTERFACE_UUID("{0002E116-0000-0000-C000-000000000046}") _dispVBComponentsEvents  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E11A-0000-0000-C000-000000000046}") _ReferencesEvents  : public IInterface 
{
	
};

__dispinterface _dispReferencesEvents;
typedef System::DelphiInterface<_dispReferencesEvents> _di__dispReferencesEvents;
__interface Reference;
typedef System::DelphiInterface<Reference> _di_Reference;
__dispinterface INTERFACE_UUID("{0002E118-0000-0000-C000-000000000046}") _dispReferencesEvents  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E130-0000-0000-C000-000000000046}") _CommandBarControlEvents  : public IInterface 
{
	
};

__dispinterface _dispCommandBarControlEvents;
typedef System::DelphiInterface<_dispCommandBarControlEvents> _di__dispCommandBarControlEvents;
__dispinterface INTERFACE_UUID("{0002E131-0000-0000-C000-000000000046}") _dispCommandBarControlEvents  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E159-0000-0000-C000-000000000046}") _ProjectTemplate  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application_(_di_Application_ &Get_Application__result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_Application_ &Get_Parent_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Application_ _scw_Get_Application_() { _di_Application_ r; HRESULT hr = Get_Application_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Application_ Application_ = {read=_scw_Get_Application_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Application_ _scw_Get_Parent() { _di_Application_ r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Application_ Parent = {read=_scw_Get_Parent};
};

__dispinterface _ProjectTemplateDisp;
typedef System::DelphiInterface<_ProjectTemplateDisp> _di__ProjectTemplateDisp;
__dispinterface INTERFACE_UUID("{0002E159-0000-0000-C000-000000000046}") _ProjectTemplateDisp  : public IDispatch 
{
	
};

__interface _VBProject_Old;
typedef System::DelphiInterface<_VBProject_Old> _di__VBProject_Old;
__interface INTERFACE_UUID("{0002E160-0000-0000-C000-000000000046}") _VBProject_Old  : public _ProjectTemplate 
{
	
public:
	virtual HRESULT __safecall Get_HelpFile(WideString &Get_HelpFile_result) = 0 ;
	virtual HRESULT __safecall Set_HelpFile(const WideString lpbstrHelpFile) = 0 ;
	virtual HRESULT __safecall Get_HelpContextID(int &Get_HelpContextID_result) = 0 ;
	virtual HRESULT __safecall Set_HelpContextID(int lpdwContextID) = 0 ;
	virtual HRESULT __safecall Get_Description(WideString &Get_Description_result) = 0 ;
	virtual HRESULT __safecall Set_Description(const WideString lpbstrDescription) = 0 ;
	virtual HRESULT __safecall Get_Mode(Activex::TOleEnum &Get_Mode_result) = 0 ;
	virtual HRESULT __safecall Get_References(_di__References &Get_References_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString lpbstrName) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Get_Collection(_di__VBProjects &Get_Collection_result) = 0 ;
	virtual HRESULT __safecall Get_Protection(Activex::TOleEnum &Get_Protection_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_VBComponents(_di__VBComponents &Get_VBComponents_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HelpFile() { WideString r; HRESULT hr = Get_HelpFile(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HelpFile = {read=_scw_Get_HelpFile, write=Set_HelpFile};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_HelpContextID() { int r; HRESULT hr = Get_HelpContextID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int HelpContextID = {read=_scw_Get_HelpContextID, write=Set_HelpContextID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Description() { WideString r; HRESULT hr = Get_Description(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Description = {read=_scw_Get_Description, write=Set_Description};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Mode() { Activex::TOleEnum r; HRESULT hr = Get_Mode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Mode = {read=_scw_Get_Mode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__References _scw_Get_References() { _di__References r; HRESULT hr = Get_References(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__References References = {read=_scw_Get_References};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__VBProjects _scw_Get_Collection() { _di__VBProjects r; HRESULT hr = Get_Collection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__VBProjects Collection = {read=_scw_Get_Collection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Protection() { Activex::TOleEnum r; HRESULT hr = Get_Protection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Protection = {read=_scw_Get_Protection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__VBComponents _scw_Get_VBComponents() { _di__VBComponents r; HRESULT hr = Get_VBComponents(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__VBComponents VBComponents = {read=_scw_Get_VBComponents};
};

__dispinterface _VBProject_OldDisp;
typedef System::DelphiInterface<_VBProject_OldDisp> _di__VBProject_OldDisp;
__dispinterface INTERFACE_UUID("{0002E160-0000-0000-C000-000000000046}") _VBProject_OldDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{EEE00915-E393-11D1-BB03-00C04FB6C4A6}") _VBProject  : public _VBProject_Old 
{
	
public:
	virtual HRESULT __safecall SaveAs(const WideString FileName) = 0 ;
	virtual HRESULT __safecall MakeCompiledFile(void) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_FileName(WideString &Get_FileName_result) = 0 ;
	virtual HRESULT __safecall Get_BuildFileName(WideString &Get_BuildFileName_result) = 0 ;
	virtual HRESULT __safecall Set_BuildFileName(const WideString lpbstrBldFName) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FileName() { WideString r; HRESULT hr = Get_FileName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FileName = {read=_scw_Get_FileName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BuildFileName() { WideString r; HRESULT hr = Get_BuildFileName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BuildFileName = {read=_scw_Get_BuildFileName, write=Set_BuildFileName};
};

__dispinterface _VBProjectDisp;
typedef System::DelphiInterface<_VBProjectDisp> _di__VBProjectDisp;
__dispinterface INTERFACE_UUID("{EEE00915-E393-11D1-BB03-00C04FB6C4A6}") _VBProjectDisp  : public IDispatch 
{
	
};

__interface _VBProjects_Old;
typedef System::DelphiInterface<_VBProjects_Old> _di__VBProjects_Old;
__interface INTERFACE_UUID("{0002E165-0000-0000-C000-000000000046}") _VBProjects_Old  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Item(const OleVariant index, _di__VBProject &Item_result) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_VBE &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_Parent() { _di_VBE r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface _VBProjects_OldDisp;
typedef System::DelphiInterface<_VBProjects_OldDisp> _di__VBProjects_OldDisp;
__dispinterface INTERFACE_UUID("{0002E165-0000-0000-C000-000000000046}") _VBProjects_OldDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{EEE00919-E393-11D1-BB03-00C04FB6C4A6}") _VBProjects  : public _VBProjects_Old 
{
	
public:
	virtual HRESULT __safecall Add(Activex::TOleEnum Type_, _di__VBProject &Add_result) = 0 ;
	virtual HRESULT __safecall Remove(const _di__VBProject lpc) = 0 ;
};

__dispinterface _VBProjectsDisp;
typedef System::DelphiInterface<_VBProjectsDisp> _di__VBProjectsDisp;
__dispinterface INTERFACE_UUID("{EEE00919-E393-11D1-BB03-00C04FB6C4A6}") _VBProjectsDisp  : public IDispatch 
{
	
};

__interface SelectedComponents;
typedef System::DelphiInterface<SelectedComponents> _di_SelectedComponents;
__interface INTERFACE_UUID("{BE39F3D4-1B13-11D0-887F-00A0C90F2744}") SelectedComponents  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Item(int index, _di__Component &Item_result) = 0 ;
	virtual HRESULT __safecall Get_Application_(_di_Application_ &Get_Application__result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di__VBProject &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Application_ _scw_Get_Application_() { _di_Application_ r; HRESULT hr = Get_Application_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Application_ Application_ = {read=_scw_Get_Application_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__VBProject _scw_Get_Parent() { _di__VBProject r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__VBProject Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface SelectedComponentsDisp;
typedef System::DelphiInterface<SelectedComponentsDisp> _di_SelectedComponentsDisp;
__dispinterface INTERFACE_UUID("{BE39F3D4-1B13-11D0-887F-00A0C90F2744}") SelectedComponentsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E161-0000-0000-C000-000000000046}") _Components  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Item(const OleVariant index, _di__Component &Item_result) = 0 ;
	virtual HRESULT __safecall Get_Application_(_di_Application_ &Get_Application__result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di__VBProject &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	virtual HRESULT __safecall Remove(const _di__Component Component) = 0 ;
	virtual HRESULT __safecall Add(Activex::TOleEnum ComponentType, _di__Component &Add_result) = 0 ;
	virtual HRESULT __safecall Import(const WideString FileName, _di__Component &Import_result) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Application_ _scw_Get_Application_() { _di_Application_ r; HRESULT hr = Get_Application_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Application_ Application_ = {read=_scw_Get_Application_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__VBProject _scw_Get_Parent() { _di__VBProject r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__VBProject Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
};

__dispinterface _ComponentsDisp;
typedef System::DelphiInterface<_ComponentsDisp> _di__ComponentsDisp;
__dispinterface INTERFACE_UUID("{0002E161-0000-0000-C000-000000000046}") _ComponentsDisp  : public IDispatch 
{
	
};

__interface _VBComponents_Old;
typedef System::DelphiInterface<_VBComponents_Old> _di__VBComponents_Old;
__interface INTERFACE_UUID("{0002E162-0000-0000-C000-000000000046}") _VBComponents_Old  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Item(const OleVariant index, _di__VBComponent &Item_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di__VBProject &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	virtual HRESULT __safecall Remove(const _di__VBComponent VBComponent) = 0 ;
	virtual HRESULT __safecall Add(Activex::TOleEnum ComponentType, _di__VBComponent &Add_result) = 0 ;
	virtual HRESULT __safecall Import(const WideString FileName, _di__VBComponent &Import_result) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__VBProject _scw_Get_Parent() { _di__VBProject r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__VBProject Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
};

__dispinterface _VBComponents_OldDisp;
typedef System::DelphiInterface<_VBComponents_OldDisp> _di__VBComponents_OldDisp;
__dispinterface INTERFACE_UUID("{0002E162-0000-0000-C000-000000000046}") _VBComponents_OldDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{EEE0091C-E393-11D1-BB03-00C04FB6C4A6}") _VBComponents  : public _VBComponents_Old 
{
	
public:
	virtual HRESULT __safecall AddCustom(const WideString ProgId, _di__VBComponent &AddCustom_result) = 0 ;
};

__dispinterface _VBComponentsDisp;
typedef System::DelphiInterface<_VBComponentsDisp> _di__VBComponentsDisp;
__dispinterface INTERFACE_UUID("{EEE0091C-E393-11D1-BB03-00C04FB6C4A6}") _VBComponentsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E163-0000-0000-C000-000000000046}") _Component  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application_(_di_Application_ &Get_Application__result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di__Components &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_IsDirty(Word &Get_IsDirty_result) = 0 ;
	virtual HRESULT __safecall Set_IsDirty(Word lpfReturn) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString pbstrReturn) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Application_ _scw_Get_Application_() { _di_Application_ r; HRESULT hr = Get_Application_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Application_ Application_ = {read=_scw_Get_Application_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Components _scw_Get_Parent() { _di__Components r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Components Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsDirty() { Word r; HRESULT hr = Get_IsDirty(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsDirty = {read=_scw_Get_IsDirty, write=Set_IsDirty};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
};

__dispinterface _ComponentDisp;
typedef System::DelphiInterface<_ComponentDisp> _di__ComponentDisp;
__dispinterface INTERFACE_UUID("{0002E163-0000-0000-C000-000000000046}") _ComponentDisp  : public IDispatch 
{
	
};

__interface _VBComponent_Old;
typedef System::DelphiInterface<_VBComponent_Old> _di__VBComponent_Old;
__interface INTERFACE_UUID("{0002E164-0000-0000-C000-000000000046}") _VBComponent_Old  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString pbstrReturn) = 0 ;
	virtual HRESULT __safecall Get_Designer(_di_IDispatch &Get_Designer_result) = 0 ;
	virtual HRESULT __safecall Get_CodeModule(_di__CodeModule &Get_CodeModule_result) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Export(const WideString FileName) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Get_Collection(_di__VBComponents &Get_Collection_result) = 0 ;
	virtual HRESULT __safecall Get_HasOpenDesigner(Word &Get_HasOpenDesigner_result) = 0 ;
	virtual HRESULT __safecall Get_Properties(_di__Properties &Get_Properties_result) = 0 ;
	virtual HRESULT __safecall DesignerWindow(_di_Window_ &DesignerWindow_result) = 0 ;
	virtual HRESULT __safecall Activate(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Designer() { _di_IDispatch r; HRESULT hr = Get_Designer(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Designer = {read=_scw_Get_Designer};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__CodeModule _scw_Get_CodeModule() { _di__CodeModule r; HRESULT hr = Get_CodeModule(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__CodeModule CodeModule = {read=_scw_Get_CodeModule};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__VBComponents _scw_Get_Collection() { _di__VBComponents r; HRESULT hr = Get_Collection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__VBComponents Collection = {read=_scw_Get_Collection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_HasOpenDesigner() { Word r; HRESULT hr = Get_HasOpenDesigner(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word HasOpenDesigner = {read=_scw_Get_HasOpenDesigner};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Properties _scw_Get_Properties() { _di__Properties r; HRESULT hr = Get_Properties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Properties Properties = {read=_scw_Get_Properties};
};

__dispinterface _VBComponent_OldDisp;
typedef System::DelphiInterface<_VBComponent_OldDisp> _di__VBComponent_OldDisp;
__dispinterface INTERFACE_UUID("{0002E164-0000-0000-C000-000000000046}") _VBComponent_OldDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{EEE00921-E393-11D1-BB03-00C04FB6C4A6}") _VBComponent  : public _VBComponent_Old 
{
	
public:
	virtual HRESULT __safecall Get_DesignerID(WideString &Get_DesignerID_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_DesignerID() { WideString r; HRESULT hr = Get_DesignerID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString DesignerID = {read=_scw_Get_DesignerID};
};

__dispinterface _VBComponentDisp;
typedef System::DelphiInterface<_VBComponentDisp> _di__VBComponentDisp;
__dispinterface INTERFACE_UUID("{EEE00921-E393-11D1-BB03-00C04FB6C4A6}") _VBComponentDisp  : public IDispatch 
{
	
};

__interface Property_;
typedef System::DelphiInterface<Property_> _di_Property_;
__interface INTERFACE_UUID("{0002E18C-0000-0000-C000-000000000046}") Property_  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Value(OleVariant &Get_Value_result) = 0 ;
	virtual HRESULT __safecall Set_Value(const OleVariant lppvReturn) = 0 ;
	virtual HRESULT __safecall Get_IndexedValue(const OleVariant Index1, const OleVariant Index2, const OleVariant Index3, const OleVariant Index4, OleVariant &Get_IndexedValue_result) = 0 ;
	virtual HRESULT __safecall Set_IndexedValue(const OleVariant Index1, const OleVariant Index2, const OleVariant Index3, const OleVariant Index4, const OleVariant lppvReturn) = 0 ;
	virtual HRESULT __safecall Get_NumIndices(short &Get_NumIndices_result) = 0 ;
	virtual HRESULT __safecall Get_Application_(_di_Application_ &Get_Application__result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di__Properties &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Get_Collection(_di__Properties &Get_Collection_result) = 0 ;
	virtual HRESULT __safecall Get_Object_(System::_di_IInterface &Get_Object__result) = 0 ;
	virtual HRESULT __safecall Set_Object_(const System::_di_IInterface lppunk) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_Value() { OleVariant r; HRESULT hr = Get_Value(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant Value = {read=_scw_Get_Value, write=Set_Value};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_IndexedValue(const OleVariant Index1, const OleVariant Index2, const OleVariant Index3, const OleVariant Index4) { OleVariant r; HRESULT hr = Get_IndexedValue(Index1, Index2, Index3, Index4, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant IndexedValue[OleVariant Index1][OleVariant Index2][OleVariant Index3][OleVariant Index4] = {read=_scw_Get_IndexedValue, write=Set_IndexedValue};
	#pragma option push -w-inl
	/* safecall wrapper */ inline short _scw_Get_NumIndices() { short r; HRESULT hr = Get_NumIndices(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property short NumIndices = {read=_scw_Get_NumIndices};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Application_ _scw_Get_Application_() { _di_Application_ r; HRESULT hr = Get_Application_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Application_ Application_ = {read=_scw_Get_Application_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Properties _scw_Get_Parent() { _di__Properties r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Properties Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Properties _scw_Get_Collection() { _di__Properties r; HRESULT hr = Get_Collection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Properties Collection = {read=_scw_Get_Collection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_Object_() { System::_di_IInterface r; HRESULT hr = Get_Object_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface Object_ = {read=_scw_Get_Object_, write=Set_Object_};
};

__dispinterface Property_Disp;
typedef System::DelphiInterface<Property_Disp> _di_Property_Disp;
__dispinterface INTERFACE_UUID("{0002E18C-0000-0000-C000-000000000046}") Property_Disp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E188-0000-0000-C000-000000000046}") _Properties  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Item(const OleVariant index, _di_Property_ &Item_result) = 0 ;
	virtual HRESULT __safecall Get_Application_(_di_Application_ &Get_Application__result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Application_ _scw_Get_Application_() { _di_Application_ r; HRESULT hr = Get_Application_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Application_ Application_ = {read=_scw_Get_Application_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
};

__dispinterface _PropertiesDisp;
typedef System::DelphiInterface<_PropertiesDisp> _di__PropertiesDisp;
__dispinterface INTERFACE_UUID("{0002E188-0000-0000-C000-000000000046}") _PropertiesDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{DA936B62-AC8B-11D1-B6E5-00A0C90F2744}") _AddIns  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Item(const OleVariant index, _di_AddIn &Item_result) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	virtual HRESULT __safecall Update(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface _AddInsDisp;
typedef System::DelphiInterface<_AddInsDisp> _di__AddInsDisp;
__dispinterface INTERFACE_UUID("{DA936B62-AC8B-11D1-B6E5-00A0C90F2744}") _AddInsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{DA936B64-AC8B-11D1-B6E5-00A0C90F2744}") AddIn  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Description(WideString &Get_Description_result) = 0 ;
	virtual HRESULT __safecall Set_Description(const WideString lpbstr) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Get_Collection(_di__AddIns &Get_Collection_result) = 0 ;
	virtual HRESULT __safecall Get_ProgId(WideString &Get_ProgId_result) = 0 ;
	virtual HRESULT __safecall Get_Guid(WideString &Get_Guid_result) = 0 ;
	virtual HRESULT __safecall Get_Connect(Word &Get_Connect_result) = 0 ;
	virtual HRESULT __safecall Set_Connect(Word lpfConnect) = 0 ;
	virtual HRESULT __safecall Get_Object_(_di_IDispatch &Get_Object__result) = 0 ;
	virtual HRESULT __safecall Set_Object_(const _di_IDispatch lppdisp) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Description() { WideString r; HRESULT hr = Get_Description(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Description = {read=_scw_Get_Description, write=Set_Description};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__AddIns _scw_Get_Collection() { _di__AddIns r; HRESULT hr = Get_Collection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__AddIns Collection = {read=_scw_Get_Collection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ProgId() { WideString r; HRESULT hr = Get_ProgId(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ProgId = {read=_scw_Get_ProgId};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Guid() { WideString r; HRESULT hr = Get_Guid(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Guid = {read=_scw_Get_Guid};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Connect() { Word r; HRESULT hr = Get_Connect(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Connect = {read=_scw_Get_Connect, write=Set_Connect};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Object_() { _di_IDispatch r; HRESULT hr = Get_Object_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Object_ = {read=_scw_Get_Object_, write=Set_Object_};
};

__dispinterface AddInDisp;
typedef System::DelphiInterface<AddInDisp> _di_AddInDisp;
__dispinterface INTERFACE_UUID("{DA936B64-AC8B-11D1-B6E5-00A0C90F2744}") AddInDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E16D-0000-0000-C000-000000000046}") _CodeModule  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di__VBComponent &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString pbstrName) = 0 ;
	virtual HRESULT __safecall AddFromString(const WideString String_) = 0 ;
	virtual HRESULT __safecall AddFromFile(const WideString FileName) = 0 ;
	virtual HRESULT __safecall Get_Lines(int StartLine, int Count, WideString &Get_Lines_result) = 0 ;
	virtual HRESULT __safecall Get_CountOfLines(int &Get_CountOfLines_result) = 0 ;
	virtual HRESULT __safecall InsertLines(int Line, const WideString String_) = 0 ;
	virtual HRESULT __safecall DeleteLines(int StartLine, int Count) = 0 ;
	virtual HRESULT __safecall ReplaceLine(int Line, const WideString String_) = 0 ;
	virtual HRESULT __safecall Get_ProcStartLine(const WideString ProcName, Activex::TOleEnum ProcKind, int &Get_ProcStartLine_result) = 0 ;
	virtual HRESULT __safecall Get_ProcCountLines(const WideString ProcName, Activex::TOleEnum ProcKind, int &Get_ProcCountLines_result) = 0 ;
	virtual HRESULT __safecall Get_ProcBodyLine(const WideString ProcName, Activex::TOleEnum ProcKind, int &Get_ProcBodyLine_result) = 0 ;
	virtual HRESULT __safecall Get_ProcOfLine(int Line, /* out */ Activex::TOleEnum &ProcKind, WideString &Get_ProcOfLine_result) = 0 ;
	virtual HRESULT __safecall Get_CountOfDeclarationLines(int &Get_CountOfDeclarationLines_result) = 0 ;
	virtual HRESULT __safecall CreateEventProc(const WideString EventName, const WideString ObjectName, int &CreateEventProc_result) = 0 ;
	virtual HRESULT __safecall Find(const WideString Target, int &StartLine, int &StartColumn, int &EndLine, int &EndColumn, Word WholeWord, Word MatchCase, Word PatternSearch, Word &Find_result) = 0 ;
	virtual HRESULT __safecall Get_CodePane(_di__CodePane &Get_CodePane_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__VBComponent _scw_Get_Parent() { _di__VBComponent r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__VBComponent Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Lines(int StartLine, int Count) { WideString r; HRESULT hr = Get_Lines(StartLine, Count, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Lines[int StartLine][int Count] = {read=_scw_Get_Lines};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_CountOfLines() { int r; HRESULT hr = Get_CountOfLines(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int CountOfLines = {read=_scw_Get_CountOfLines};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ProcStartLine(const WideString ProcName, Activex::TOleEnum ProcKind) { int r; HRESULT hr = Get_ProcStartLine(ProcName, ProcKind, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int ProcStartLine[WideString ProcName][Activex::TOleEnum ProcKind] = {read=_scw_Get_ProcStartLine};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ProcCountLines(const WideString ProcName, Activex::TOleEnum ProcKind) { int r; HRESULT hr = Get_ProcCountLines(ProcName, ProcKind, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int ProcCountLines[WideString ProcName][Activex::TOleEnum ProcKind] = {read=_scw_Get_ProcCountLines};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ProcBodyLine(const WideString ProcName, Activex::TOleEnum ProcKind) { int r; HRESULT hr = Get_ProcBodyLine(ProcName, ProcKind, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int ProcBodyLine[WideString ProcName][Activex::TOleEnum ProcKind] = {read=_scw_Get_ProcBodyLine};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ProcOfLine(int Line, /* out */ Activex::TOleEnum &ProcKind) { WideString r; HRESULT hr = Get_ProcOfLine(Line, ProcKind, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ProcOfLine[int Line][Activex::TOleEnum ProcKind] = {read=_scw_Get_ProcOfLine};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_CountOfDeclarationLines() { int r; HRESULT hr = Get_CountOfDeclarationLines(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int CountOfDeclarationLines = {read=_scw_Get_CountOfDeclarationLines};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__CodePane _scw_Get_CodePane() { _di__CodePane r; HRESULT hr = Get_CodePane(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__CodePane CodePane = {read=_scw_Get_CodePane};
};

__dispinterface _CodeModuleDisp;
typedef System::DelphiInterface<_CodeModuleDisp> _di__CodeModuleDisp;
__dispinterface INTERFACE_UUID("{0002E16D-0000-0000-C000-000000000046}") _CodeModuleDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E171-0000-0000-C000-000000000046}") _CodePanes  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_VBE &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant index, _di__CodePane &Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	virtual HRESULT __safecall Get_Current(_di__CodePane &Get_Current_result) = 0 ;
	virtual HRESULT __safecall Set_Current(const _di__CodePane CodePane) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_Parent() { _di_VBE r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__CodePane _scw_Get_Current() { _di__CodePane r; HRESULT hr = Get_Current(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__CodePane Current = {read=_scw_Get_Current, write=Set_Current};
};

__dispinterface _CodePanesDisp;
typedef System::DelphiInterface<_CodePanesDisp> _di__CodePanesDisp;
__dispinterface INTERFACE_UUID("{0002E171-0000-0000-C000-000000000046}") _CodePanesDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E175-0000-0000-C000-000000000046}") _CodePane  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Collection(_di__CodePanes &Get_Collection_result) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Get_Window_(_di_Window_ &Get_Window__result) = 0 ;
	virtual HRESULT __safecall GetSelection(/* out */ int &StartLine, /* out */ int &StartColumn, /* out */ int &EndLine, /* out */ int &EndColumn) = 0 ;
	virtual HRESULT __safecall SetSelection(int StartLine, int StartColumn, int EndLine, int EndColumn) = 0 ;
	virtual HRESULT __safecall Get_TopLine(int &Get_TopLine_result) = 0 ;
	virtual HRESULT __safecall Set_TopLine(int TopLine) = 0 ;
	virtual HRESULT __safecall Get_CountOfVisibleLines(int &Get_CountOfVisibleLines_result) = 0 ;
	virtual HRESULT __safecall Get_CodeModule(_di__CodeModule &Get_CodeModule_result) = 0 ;
	virtual HRESULT __safecall Show(void) = 0 ;
	virtual HRESULT __safecall Get_CodePaneView(Activex::TOleEnum &Get_CodePaneView_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__CodePanes _scw_Get_Collection() { _di__CodePanes r; HRESULT hr = Get_Collection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__CodePanes Collection = {read=_scw_Get_Collection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Window_ _scw_Get_Window_() { _di_Window_ r; HRESULT hr = Get_Window_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Window_ Window_ = {read=_scw_Get_Window_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_TopLine() { int r; HRESULT hr = Get_TopLine(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int TopLine = {read=_scw_Get_TopLine, write=Set_TopLine};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_CountOfVisibleLines() { int r; HRESULT hr = Get_CountOfVisibleLines(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int CountOfVisibleLines = {read=_scw_Get_CountOfVisibleLines};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__CodeModule _scw_Get_CodeModule() { _di__CodeModule r; HRESULT hr = Get_CodeModule(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__CodeModule CodeModule = {read=_scw_Get_CodeModule};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_CodePaneView() { Activex::TOleEnum r; HRESULT hr = Get_CodePaneView(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum CodePaneView = {read=_scw_Get_CodePaneView};
};

__dispinterface _CodePaneDisp;
typedef System::DelphiInterface<_CodePaneDisp> _di__CodePaneDisp;
__dispinterface INTERFACE_UUID("{0002E175-0000-0000-C000-000000000046}") _CodePaneDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E179-0000-0000-C000-000000000046}") _References  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di__VBProject &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant index, _di_Reference &Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	virtual HRESULT __safecall AddFromGuid(const WideString Guid, int Major, int Minor, _di_Reference &AddFromGuid_result) = 0 ;
	virtual HRESULT __safecall AddFromFile(const WideString FileName, _di_Reference &AddFromFile_result) = 0 ;
	virtual HRESULT __safecall Remove(const _di_Reference Reference) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__VBProject _scw_Get_Parent() { _di__VBProject r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__VBProject Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface _ReferencesDisp;
typedef System::DelphiInterface<_ReferencesDisp> _di__ReferencesDisp;
__dispinterface INTERFACE_UUID("{0002E179-0000-0000-C000-000000000046}") _ReferencesDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0002E17D-0000-0000-C000-000000000046}") Reference  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Collection(_di__References &Get_Collection_result) = 0 ;
	virtual HRESULT __safecall Get_VBE(_di_VBE &Get_VBE_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Guid(WideString &Get_Guid_result) = 0 ;
	virtual HRESULT __safecall Get_Major(int &Get_Major_result) = 0 ;
	virtual HRESULT __safecall Get_Minor(int &Get_Minor_result) = 0 ;
	virtual HRESULT __safecall Get_FullPath(WideString &Get_FullPath_result) = 0 ;
	virtual HRESULT __safecall Get_BuiltIn(Word &Get_BuiltIn_result) = 0 ;
	virtual HRESULT __safecall Get_IsBroken(Word &Get_IsBroken_result) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_Description(WideString &Get_Description_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__References _scw_Get_Collection() { _di__References r; HRESULT hr = Get_Collection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__References Collection = {read=_scw_Get_Collection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_VBE _scw_Get_VBE() { _di_VBE r; HRESULT hr = Get_VBE(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_VBE VBE = {read=_scw_Get_VBE};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Guid() { WideString r; HRESULT hr = Get_Guid(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Guid = {read=_scw_Get_Guid};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Major() { int r; HRESULT hr = Get_Major(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Major = {read=_scw_Get_Major};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Minor() { int r; HRESULT hr = Get_Minor(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Minor = {read=_scw_Get_Minor};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FullPath() { WideString r; HRESULT hr = Get_FullPath(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FullPath = {read=_scw_Get_FullPath};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_BuiltIn() { Word r; HRESULT hr = Get_BuiltIn(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word BuiltIn = {read=_scw_Get_BuiltIn};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsBroken() { Word r; HRESULT hr = Get_IsBroken(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsBroken = {read=_scw_Get_IsBroken};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Description() { WideString r; HRESULT hr = Get_Description(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Description = {read=_scw_Get_Description};
};

__dispinterface ReferenceDisp;
typedef System::DelphiInterface<ReferenceDisp> _di_ReferenceDisp;
__dispinterface INTERFACE_UUID("{0002E17D-0000-0000-C000-000000000046}") ReferenceDisp  : public IDispatch 
{
	
};

__dispinterface _dispReferences_Events;
typedef System::DelphiInterface<_dispReferences_Events> _di__dispReferences_Events;
__dispinterface INTERFACE_UUID("{0002E19A-0000-0000-C000-000000000046}") _dispReferences_Events  : public IDispatch 
{
	
};

class DELPHICLASS CoWindows;
class PASCALIMPLEMENTATION CoWindows : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Windows_ __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Windows_ __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoWindows(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoWindows(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoLinkedWindows;
class PASCALIMPLEMENTATION CoLinkedWindows : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__LinkedWindows __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__LinkedWindows __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoLinkedWindows(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoLinkedWindows(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoReferencesEvents;
class PASCALIMPLEMENTATION CoReferencesEvents : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__ReferencesEvents __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__ReferencesEvents __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoReferencesEvents(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoReferencesEvents(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoCommandBarEvents;
class PASCALIMPLEMENTATION CoCommandBarEvents : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__CommandBarControlEvents __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__CommandBarControlEvents __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoCommandBarEvents(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoCommandBarEvents(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoProjectTemplate;
class PASCALIMPLEMENTATION CoProjectTemplate : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__ProjectTemplate __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__ProjectTemplate __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoProjectTemplate(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoProjectTemplate(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoVBProject;
class PASCALIMPLEMENTATION CoVBProject : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__VBProject __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__VBProject __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoVBProject(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoVBProject(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoVBProjects;
class PASCALIMPLEMENTATION CoVBProjects : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__VBProjects __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__VBProjects __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoVBProjects(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoVBProjects(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoComponents;
class PASCALIMPLEMENTATION CoComponents : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Components __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Components __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoComponents(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoComponents(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoVBComponents;
class PASCALIMPLEMENTATION CoVBComponents : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__VBComponents __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__VBComponents __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoVBComponents(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoVBComponents(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoComponent;
class PASCALIMPLEMENTATION CoComponent : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Component __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Component __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoComponent(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoComponent(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoVBComponent;
class PASCALIMPLEMENTATION CoVBComponent : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__VBComponent __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__VBComponent __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoVBComponent(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoVBComponent(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoProperties;
class PASCALIMPLEMENTATION CoProperties : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Properties __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Properties __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoProperties(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoProperties(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoAddins;
class PASCALIMPLEMENTATION CoAddins : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__AddIns __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__AddIns __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoAddins(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoAddins(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoCodeModule;
class PASCALIMPLEMENTATION CoCodeModule : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__CodeModule __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__CodeModule __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoCodeModule(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoCodeModule(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoCodePanes;
class PASCALIMPLEMENTATION CoCodePanes : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__CodePanes __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__CodePanes __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoCodePanes(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoCodePanes(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoCodePane;
class PASCALIMPLEMENTATION CoCodePane : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__CodePane __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__CodePane __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoCodePane(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoCodePane(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoReferences;
class PASCALIMPLEMENTATION CoReferences : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__References __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__References __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoReferences(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoReferences(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE GUID LIBID_VBIDE;
extern PACKAGE GUID IID_Application_;
extern PACKAGE GUID IID_VBE;
extern PACKAGE GUID IID_Window_;
extern PACKAGE GUID IID__Windows_old;
extern PACKAGE GUID IID__Windows_;
extern PACKAGE GUID CLASS_Windows;
extern PACKAGE GUID IID__LinkedWindows;
extern PACKAGE GUID CLASS_LinkedWindows;
extern PACKAGE GUID IID_Events;
extern PACKAGE GUID IID__VBProjectsEvents;
extern PACKAGE GUID DIID__dispVBProjectsEvents;
extern PACKAGE GUID IID__VBComponentsEvents;
extern PACKAGE GUID DIID__dispVBComponentsEvents;
extern PACKAGE GUID IID__ReferencesEvents;
extern PACKAGE GUID DIID__dispReferencesEvents;
extern PACKAGE GUID CLASS_ReferencesEvents;
extern PACKAGE GUID IID__CommandBarControlEvents;
extern PACKAGE GUID DIID__dispCommandBarControlEvents;
extern PACKAGE GUID CLASS_CommandBarEvents;
extern PACKAGE GUID IID__ProjectTemplate;
extern PACKAGE GUID CLASS_ProjectTemplate;
extern PACKAGE GUID IID__VBProject_Old;
extern PACKAGE GUID IID__VBProject;
extern PACKAGE GUID CLASS_VBProject;
extern PACKAGE GUID IID__VBProjects_Old;
extern PACKAGE GUID IID__VBProjects;
extern PACKAGE GUID CLASS_VBProjects;
extern PACKAGE GUID IID_SelectedComponents;
extern PACKAGE GUID IID__Components;
extern PACKAGE GUID CLASS_Components;
extern PACKAGE GUID IID__VBComponents_Old;
extern PACKAGE GUID IID__VBComponents;
extern PACKAGE GUID CLASS_VBComponents;
extern PACKAGE GUID IID__Component;
extern PACKAGE GUID CLASS_Component;
extern PACKAGE GUID IID__VBComponent_Old;
extern PACKAGE GUID IID__VBComponent;
extern PACKAGE GUID CLASS_VBComponent;
extern PACKAGE GUID IID_Property_;
extern PACKAGE GUID IID__Properties;
extern PACKAGE GUID CLASS_Properties;
extern PACKAGE GUID IID__AddIns;
extern PACKAGE GUID CLASS_Addins;
extern PACKAGE GUID IID_AddIn;
extern PACKAGE GUID IID__CodeModule;
extern PACKAGE GUID CLASS_CodeModule;
extern PACKAGE GUID IID__CodePanes;
extern PACKAGE GUID CLASS_CodePanes;
extern PACKAGE GUID IID__CodePane;
extern PACKAGE GUID CLASS_CodePane;
extern PACKAGE GUID IID__References;
extern PACKAGE GUID IID_Reference;
extern PACKAGE GUID DIID__dispReferences_Events;
extern PACKAGE GUID CLASS_References;
static const Shortint vbextFileTypeForm = 0x0;
static const Shortint vbextFileTypeModule = 0x1;
static const Shortint vbextFileTypeClass = 0x2;
static const Shortint vbextFileTypeProject = 0x3;
static const Shortint vbextFileTypeExe = 0x4;
static const Shortint vbextFileTypeFrx = 0x5;
static const Shortint vbextFileTypeRes = 0x6;
static const Shortint vbextFileTypeUserControl = 0x7;
static const Shortint vbextFileTypePropertyPage = 0x8;
static const Shortint vbextFileTypeDocObject = 0x9;
static const Shortint vbextFileTypeBinary = 0xa;
static const Shortint vbextFileTypeGroupProject = 0xb;
static const Shortint vbextFileTypeDesigners = 0xc;
static const Shortint vbext_wt_CodeWindow = 0x0;
static const Shortint vbext_wt_Designer = 0x1;
static const Shortint vbext_wt_Browser = 0x2;
static const Shortint vbext_wt_Watch = 0x3;
static const Shortint vbext_wt_Locals = 0x4;
static const Shortint vbext_wt_Immediate = 0x5;
static const Shortint vbext_wt_ProjectWindow = 0x6;
static const Shortint vbext_wt_PropertyWindow = 0x7;
static const Shortint vbext_wt_Find = 0x8;
static const Shortint vbext_wt_FindReplace = 0x9;
static const Shortint vbext_wt_Toolbox = 0xa;
static const Shortint vbext_wt_LinkedWindowFrame = 0xb;
static const Shortint vbext_wt_MainWindow = 0xc;
static const Shortint vbext_wt_ToolWindow = 0xf;
static const Shortint vbext_ws_Normal = 0x0;
static const Shortint vbext_ws_Minimize = 0x1;
static const Shortint vbext_ws_Maximize = 0x2;
static const Shortint vbext_pt_HostProject = 0x64;
static const Shortint vbext_pt_StandAlone = 0x65;
static const Shortint vbext_pp_none = 0x0;
static const Shortint vbext_pp_locked = 0x1;
static const Shortint vbext_vm_Run = 0x0;
static const Shortint vbext_vm_Break = 0x1;
static const Shortint vbext_vm_Design = 0x2;
static const Shortint vbext_ct_StdModule = 0x1;
static const Shortint vbext_ct_ClassModule = 0x2;
static const Shortint vbext_ct_MSForm = 0x3;
static const Shortint vbext_ct_ActiveXDesigner = 0xb;
static const Shortint vbext_ct_Document = 0x64;
static const Shortint vbext_pk_Proc = 0x0;
static const Shortint vbext_pk_Let = 0x1;
static const Shortint vbext_pk_Set = 0x2;
static const Shortint vbext_pk_Get = 0x3;
static const Shortint vbext_cv_ProcedureView = 0x0;
static const Shortint vbext_cv_FullModuleView = 0x1;
static const Shortint vbext_rk_TypeLib = 0x0;
static const Shortint vbext_rk_Project = 0x1;

}	/* namespace Opvbidxp */
using namespace Opvbidxp;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpVBIdXP
