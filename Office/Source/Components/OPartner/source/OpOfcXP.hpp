// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpOfcXP.pas' rev: 6.00

#ifndef OpOfcXPHPP
#define OpOfcXPHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <StdVCL.hpp>	// Pascal unit
#include <OleCtrls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <ActiveX.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opofcxp
{
//-- type declarations -------------------------------------------------------
typedef Activex::TOleEnum MsoLineDashStyle;

typedef Activex::TOleEnum MsoLineStyle;

typedef Activex::TOleEnum MsoArrowheadStyle;

typedef Activex::TOleEnum MsoArrowheadWidth;

typedef Activex::TOleEnum MsoArrowheadLength;

typedef Activex::TOleEnum MsoFillType;

typedef Activex::TOleEnum MsoGradientStyle;

typedef Activex::TOleEnum MsoGradientColorType;

typedef Activex::TOleEnum MsoTextureType;

typedef Activex::TOleEnum MsoPresetTexture;

typedef Activex::TOleEnum MsoPatternType;

typedef Activex::TOleEnum MsoPresetGradientType;

typedef Activex::TOleEnum MsoShadowType;

typedef Activex::TOleEnum MsoPresetTextEffect;

typedef Activex::TOleEnum MsoPresetTextEffectShape;

typedef Activex::TOleEnum MsoTextEffectAlignment;

typedef Activex::TOleEnum MsoPresetLightingDirection;

typedef Activex::TOleEnum MsoPresetLightingSoftness;

typedef Activex::TOleEnum MsoPresetMaterial;

typedef Activex::TOleEnum MsoPresetExtrusionDirection;

typedef Activex::TOleEnum MsoPresetThreeDFormat;

typedef Activex::TOleEnum MsoExtrusionColorType;

typedef Activex::TOleEnum MsoAlignCmd;

typedef Activex::TOleEnum MsoDistributeCmd;

typedef Activex::TOleEnum MsoConnectorType;

typedef Activex::TOleEnum MsoHorizontalAnchor;

typedef Activex::TOleEnum MsoVerticalAnchor;

typedef Activex::TOleEnum MsoOrientation;

typedef Activex::TOleEnum MsoZOrderCmd;

typedef Activex::TOleEnum MsoSegmentType;

typedef Activex::TOleEnum MsoEditingType;

typedef Activex::TOleEnum MsoAutoShapeType;

typedef Activex::TOleEnum MsoShapeType;

typedef Activex::TOleEnum MsoFlipCmd;

typedef Activex::TOleEnum MsoTriState;

typedef Activex::TOleEnum MsoColorType;

typedef Activex::TOleEnum MsoPictureColorType;

typedef Activex::TOleEnum MsoCalloutAngleType;

typedef Activex::TOleEnum MsoCalloutDropType;

typedef Activex::TOleEnum MsoCalloutType;

typedef Activex::TOleEnum MsoBlackWhiteMode;

typedef Activex::TOleEnum MsoMixedType;

typedef Activex::TOleEnum MsoTextOrientation;

typedef Activex::TOleEnum MsoScaleFrom;

typedef Activex::TOleEnum MsoBarPosition;

typedef Activex::TOleEnum MsoBarProtection;

typedef Activex::TOleEnum MsoBarType;

typedef Activex::TOleEnum MsoControlType;

typedef Activex::TOleEnum MsoButtonState;

typedef Activex::TOleEnum MsoControlOLEUsage;

typedef Activex::TOleEnum MsoButtonStyleHidden;

typedef Activex::TOleEnum MsoButtonStyle;

typedef Activex::TOleEnum MsoComboStyle;

typedef Activex::TOleEnum MsoOLEMenuGroup;

typedef Activex::TOleEnum MsoMenuAnimation;

typedef Activex::TOleEnum MsoBarRow;

typedef Activex::TOleEnum MsoCommandBarButtonHyperlinkType;

typedef Activex::TOleEnum MsoHyperlinkType;

typedef Activex::TOleEnum MsoExtraInfoMethod;

typedef Activex::TOleEnum MsoAnimationType;

typedef Activex::TOleEnum MsoButtonSetType;

typedef Activex::TOleEnum MsoIconType;

typedef Activex::TOleEnum MsoBalloonType;

typedef Activex::TOleEnum MsoModeType;

typedef Activex::TOleEnum MsoBalloonErrorType;

typedef Activex::TOleEnum MsoWizardActType;

typedef Activex::TOleEnum MsoWizardMsgType;

typedef Activex::TOleEnum MsoBalloonButtonType;

typedef Activex::TOleEnum DocProperties;

typedef Activex::TOleEnum MsoDocProperties;

typedef Activex::TOleEnum MsoAppLanguageID;

typedef Activex::TOleEnum MsoFarEastLineBreakLanguageID;

typedef Activex::TOleEnum MsoFeatureInstall;

typedef Activex::TOleEnum MsoScriptLanguage;

typedef Activex::TOleEnum MsoScriptLocation;

typedef Activex::TOleEnum MsoFileFindOptions;

typedef Activex::TOleEnum MsoFileFindView;

typedef Activex::TOleEnum MsoFileFindSortBy;

typedef Activex::TOleEnum MsoFileFindListBy;

typedef Activex::TOleEnum MsoLastModified;

typedef Activex::TOleEnum MsoSortBy;

typedef Activex::TOleEnum MsoSortOrder;

typedef Activex::TOleEnum MsoConnector;

typedef Activex::TOleEnum MsoCondition;

typedef Activex::TOleEnum MsoFileType;

typedef Activex::TOleEnum MsoLanguageID;

typedef Activex::TOleEnum MsoScreenSize;

typedef Activex::TOleEnum MsoCharacterSet;

typedef Activex::TOleEnum MsoEncoding;

typedef Activex::TOleEnum MsoHTMLProjectOpen;

typedef Activex::TOleEnum MsoHTMLProjectState;

typedef Activex::TOleEnum MsoFileDialogType;

typedef Activex::TOleEnum MsoFileDialogView;

typedef Activex::TOleEnum MsoAutomationSecurity;

typedef Activex::TOleEnum MailFormat;

typedef Activex::TOleEnum MsoAlertButtonType;

typedef Activex::TOleEnum MsoAlertIconType;

typedef Activex::TOleEnum MsoAlertDefaultType;

typedef Activex::TOleEnum MsoAlertCancelType;

typedef Activex::TOleEnum MsoSearchIn;

typedef Activex::TOleEnum MsoTargetBrowser;

typedef Activex::TOleEnum MsoOrgChartOrientation;

typedef Activex::TOleEnum MsoOrgChartLayoutType;

typedef Activex::TOleEnum MsoRelativeNodePosition;

typedef Activex::TOleEnum MsoDiagramType;

typedef Activex::TOleEnum MsoDiagramNodeType;

typedef Activex::TOleEnum MsoMoveRow;

typedef Activex::TOleEnum MsoFilterComparison;

typedef Activex::TOleEnum MsoFilterConjunction;

typedef Activex::TOleEnum MsoFileNewSection;

typedef Activex::TOleEnum MsoFileNewAction;

typedef Activex::TOleEnum MsoLanguageIDHidden;

__interface _CommandBars;
typedef System::DelphiInterface<_CommandBars> _di__CommandBars;
typedef _CommandBars CommandBars;
;

__interface _CommandBarComboBox;
typedef System::DelphiInterface<_CommandBarComboBox> _di__CommandBarComboBox;
typedef _CommandBarComboBox CommandBarComboBox;
;

__interface _CommandBarButton;
typedef System::DelphiInterface<_CommandBarButton> _di__CommandBarButton;
typedef _CommandBarButton CommandBarButton;
;

__interface IMsoEnvelopeVB;
typedef System::DelphiInterface<IMsoEnvelopeVB> _di_IMsoEnvelopeVB;
typedef IMsoEnvelopeVB MsoEnvelope;
;

typedef OleVariant *POleVariant1;

typedef GUID *PUserType1;

typedef Shortint *PShortint1;

typedef PShortint1 *PPShortint1;

typedef GUID *PUserType2;

typedef int MsoRGBType;

__interface IAccessible;
typedef System::DelphiInterface<IAccessible> _di_IAccessible;
__interface INTERFACE_UUID("{618736E0-3C3D-11CF-810C-00AA00389B71}") IAccessible  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_accParent(_di_IDispatch &Get_accParent_result) = 0 ;
	virtual HRESULT __safecall Get_accChildCount(int &Get_accChildCount_result) = 0 ;
	virtual HRESULT __safecall Get_accChild(const OleVariant varChild, _di_IDispatch &Get_accChild_result) = 0 ;
	virtual HRESULT __safecall Get_accName(const OleVariant varChild, WideString &Get_accName_result) = 0 ;
	virtual HRESULT __safecall Get_accValue(const OleVariant varChild, WideString &Get_accValue_result) = 0 ;
	virtual HRESULT __safecall Get_accDescription(const OleVariant varChild, WideString &Get_accDescription_result) = 0 ;
	virtual HRESULT __safecall Get_accRole(const OleVariant varChild, OleVariant &Get_accRole_result) = 0 ;
	virtual HRESULT __safecall Get_accState(const OleVariant varChild, OleVariant &Get_accState_result) = 0 ;
	virtual HRESULT __safecall Get_accHelp(const OleVariant varChild, WideString &Get_accHelp_result) = 0 ;
	virtual HRESULT __safecall Get_accHelpTopic(/* out */ WideString &pszHelpFile, const OleVariant varChild, int &Get_accHelpTopic_result) = 0 ;
	virtual HRESULT __safecall Get_accKeyboardShortcut(const OleVariant varChild, WideString &Get_accKeyboardShortcut_result) = 0 ;
	virtual HRESULT __safecall Get_accFocus(OleVariant &Get_accFocus_result) = 0 ;
	virtual HRESULT __safecall Get_accSelection(OleVariant &Get_accSelection_result) = 0 ;
	virtual HRESULT __safecall Get_accDefaultAction(const OleVariant varChild, WideString &Get_accDefaultAction_result) = 0 ;
	virtual HRESULT __safecall accSelect(int flagsSelect, const OleVariant varChild) = 0 ;
	virtual HRESULT __safecall accLocation(/* out */ int &pxLeft, /* out */ int &pyTop, /* out */ int &pcxWidth, /* out */ int &pcyHeight, const OleVariant varChild) = 0 ;
	virtual HRESULT __safecall accNavigate(int navDir, const OleVariant varStart, OleVariant &accNavigate_result) = 0 ;
	virtual HRESULT __safecall accHitTest(int xLeft, int yTop, OleVariant &accHitTest_result) = 0 ;
	virtual HRESULT __safecall accDoDefaultAction(const OleVariant varChild) = 0 ;
	virtual HRESULT __safecall Set_accName(const OleVariant varChild, const WideString pszName) = 0 ;
	virtual HRESULT __safecall Set_accValue(const OleVariant varChild, const WideString pszValue) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_accParent() { _di_IDispatch r; HRESULT hr = Get_accParent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch accParent = {read=_scw_Get_accParent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_accChildCount() { int r; HRESULT hr = Get_accChildCount(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int accChildCount = {read=_scw_Get_accChildCount};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_accChild(const OleVariant varChild) { _di_IDispatch r; HRESULT hr = Get_accChild(varChild, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch accChild[OleVariant varChild] = {read=_scw_Get_accChild};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_accName(const OleVariant varChild) { WideString r; HRESULT hr = Get_accName(varChild, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString accName[OleVariant varChild] = {read=_scw_Get_accName, write=Set_accName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_accValue(const OleVariant varChild) { WideString r; HRESULT hr = Get_accValue(varChild, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString accValue[OleVariant varChild] = {read=_scw_Get_accValue, write=Set_accValue};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_accDescription(const OleVariant varChild) { WideString r; HRESULT hr = Get_accDescription(varChild, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString accDescription[OleVariant varChild] = {read=_scw_Get_accDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_accRole(const OleVariant varChild) { OleVariant r; HRESULT hr = Get_accRole(varChild, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant accRole[OleVariant varChild] = {read=_scw_Get_accRole};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_accState(const OleVariant varChild) { OleVariant r; HRESULT hr = Get_accState(varChild, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant accState[OleVariant varChild] = {read=_scw_Get_accState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_accHelp(const OleVariant varChild) { WideString r; HRESULT hr = Get_accHelp(varChild, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString accHelp[OleVariant varChild] = {read=_scw_Get_accHelp};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_accHelpTopic(/* out */ WideString &pszHelpFile, const OleVariant varChild) { int r; HRESULT hr = Get_accHelpTopic(pszHelpFile, varChild, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int accHelpTopic[WideString pszHelpFile][OleVariant varChild] = {read=_scw_Get_accHelpTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_accKeyboardShortcut(const OleVariant varChild) { WideString r; HRESULT hr = Get_accKeyboardShortcut(varChild, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString accKeyboardShortcut[OleVariant varChild] = {read=_scw_Get_accKeyboardShortcut};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_accFocus() { OleVariant r; HRESULT hr = Get_accFocus(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant accFocus = {read=_scw_Get_accFocus};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_accSelection() { OleVariant r; HRESULT hr = Get_accSelection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant accSelection = {read=_scw_Get_accSelection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_accDefaultAction(const OleVariant varChild) { WideString r; HRESULT hr = Get_accDefaultAction(varChild, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString accDefaultAction[OleVariant varChild] = {read=_scw_Get_accDefaultAction};
};

__dispinterface IAccessibleDisp;
typedef System::DelphiInterface<IAccessibleDisp> _di_IAccessibleDisp;
__dispinterface INTERFACE_UUID("{618736E0-3C3D-11CF-810C-00AA00389B71}") IAccessibleDisp  : public IDispatch 
{
	
};

__interface _IMsoDispObj;
typedef System::DelphiInterface<_IMsoDispObj> _di__IMsoDispObj;
__interface INTERFACE_UUID("{000C0300-0000-0000-C000-000000000046}") _IMsoDispObj  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di_IDispatch &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Creator(int &Get_Creator_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Application() { _di_IDispatch r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Application_ = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Creator() { int r; HRESULT hr = Get_Creator(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Creator = {read=_scw_Get_Creator};
};

__dispinterface _IMsoDispObjDisp;
typedef System::DelphiInterface<_IMsoDispObjDisp> _di__IMsoDispObjDisp;
__dispinterface INTERFACE_UUID("{000C0300-0000-0000-C000-000000000046}") _IMsoDispObjDisp  : public IDispatch 
{
	
};

__interface _IMsoOleAccDispObj;
typedef System::DelphiInterface<_IMsoOleAccDispObj> _di__IMsoOleAccDispObj;
__interface INTERFACE_UUID("{000C0301-0000-0000-C000-000000000046}") _IMsoOleAccDispObj  : public IAccessible 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di_IDispatch &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Creator(int &Get_Creator_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Application() { _di_IDispatch r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Application_ = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Creator() { int r; HRESULT hr = Get_Creator(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Creator = {read=_scw_Get_Creator};
};

__dispinterface _IMsoOleAccDispObjDisp;
typedef System::DelphiInterface<_IMsoOleAccDispObjDisp> _di__IMsoOleAccDispObjDisp;
__dispinterface INTERFACE_UUID("{000C0301-0000-0000-C000-000000000046}") _IMsoOleAccDispObjDisp  : public IDispatch 
{
	
};

__interface CommandBarControl;
typedef System::DelphiInterface<CommandBarControl> _di_CommandBarControl;
__interface CommandBar;
typedef System::DelphiInterface<CommandBar> _di_CommandBar;
__interface CommandBarControls;
typedef System::DelphiInterface<CommandBarControls> _di_CommandBarControls;
__interface INTERFACE_UUID("{000C0302-0000-0000-C000-000000000046}") _CommandBars  : public _IMsoDispObj 
{
	
public:
	_di_CommandBar operator[](OleVariant Index) { return Item[Index]; }
	
public:
	virtual HRESULT __safecall Get_ActionControl(_di_CommandBarControl &Get_ActionControl_result) = 0 ;
	virtual HRESULT __safecall Get_ActiveMenuBar(_di_CommandBar &Get_ActiveMenuBar_result) = 0 ;
	virtual HRESULT __safecall Add(const OleVariant Name, const OleVariant Position, const OleVariant MenuBar, const OleVariant Temporary, _di_CommandBar &Add_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get_DisplayTooltips(Word &Get_DisplayTooltips_result) = 0 ;
	virtual HRESULT __safecall Set_DisplayTooltips(Word pvarfDisplayTooltips) = 0 ;
	virtual HRESULT __safecall Get_DisplayKeysInTooltips(Word &Get_DisplayKeysInTooltips_result) = 0 ;
	virtual HRESULT __safecall Set_DisplayKeysInTooltips(Word pvarfDisplayKeys) = 0 ;
	virtual HRESULT __safecall FindControl(const OleVariant Type_, const OleVariant Id, const OleVariant Tag, const OleVariant Visible, _di_CommandBarControl &FindControl_result) = 0 ;
	virtual HRESULT __safecall Get_Item(const OleVariant Index, _di_CommandBar &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_LargeButtons(Word &Get_LargeButtons_result) = 0 ;
	virtual HRESULT __safecall Set_LargeButtons(Word pvarfLargeButtons) = 0 ;
	virtual HRESULT __safecall Get_MenuAnimationStyle(Activex::TOleEnum &Get_MenuAnimationStyle_result) = 0 ;
	virtual HRESULT __safecall Set_MenuAnimationStyle(Activex::TOleEnum pma) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall ReleaseFocus(void) = 0 ;
	virtual HRESULT __safecall Get_IdsString(int ids, /* out */ WideString &pbstrName, int &Get_IdsString_result) = 0 ;
	virtual HRESULT __safecall Get_TmcGetName(int tmc, /* out */ WideString &pbstrName, int &Get_TmcGetName_result) = 0 ;
	virtual HRESULT __safecall Get_AdaptiveMenus(Word &Get_AdaptiveMenus_result) = 0 ;
	virtual HRESULT __safecall Set_AdaptiveMenus(Word pvarfAdaptiveMenus) = 0 ;
	virtual HRESULT __safecall FindControls(const OleVariant Type_, const OleVariant Id, const OleVariant Tag, const OleVariant Visible, _di_CommandBarControls &FindControls_result) = 0 ;
	virtual HRESULT __safecall AddEx(const OleVariant TbidOrName, const OleVariant Position, const OleVariant MenuBar, const OleVariant Temporary, const OleVariant TbtrProtection, _di_CommandBar &AddEx_result) = 0 ;
	virtual HRESULT __safecall Get_DisplayFonts(Word &Get_DisplayFonts_result) = 0 ;
	virtual HRESULT __safecall Set_DisplayFonts(Word pvarfDisplayFonts) = 0 ;
	virtual HRESULT __safecall Get_DisableCustomize(Word &Get_DisableCustomize_result) = 0 ;
	virtual HRESULT __safecall Set_DisableCustomize(Word pvarfDisableCustomize) = 0 ;
	virtual HRESULT __safecall Get_DisableAskAQuestionDropdown(Word &Get_DisableAskAQuestionDropdown_result) = 0 ;
	virtual HRESULT __safecall Set_DisableAskAQuestionDropdown(Word pvarfDisableAskAQuestionDropdown) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CommandBarControl _scw_Get_ActionControl() { _di_CommandBarControl r; HRESULT hr = Get_ActionControl(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CommandBarControl ActionControl = {read=_scw_Get_ActionControl};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CommandBar _scw_Get_ActiveMenuBar() { _di_CommandBar r; HRESULT hr = Get_ActiveMenuBar(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CommandBar ActiveMenuBar = {read=_scw_Get_ActiveMenuBar};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DisplayTooltips() { Word r; HRESULT hr = Get_DisplayTooltips(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DisplayTooltips = {read=_scw_Get_DisplayTooltips};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DisplayKeysInTooltips() { Word r; HRESULT hr = Get_DisplayKeysInTooltips(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DisplayKeysInTooltips = {read=_scw_Get_DisplayKeysInTooltips};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CommandBar _scw_Get_Item(const OleVariant Index) { _di_CommandBar r; HRESULT hr = Get_Item(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CommandBar Item[OleVariant Index] = {read=_scw_Get_Item/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_LargeButtons() { Word r; HRESULT hr = Get_LargeButtons(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word LargeButtons = {read=_scw_Get_LargeButtons};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MenuAnimationStyle() { Activex::TOleEnum r; HRESULT hr = Get_MenuAnimationStyle(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MenuAnimationStyle = {read=_scw_Get_MenuAnimationStyle};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_IdsString(int ids, /* out */ WideString &pbstrName) { int r; HRESULT hr = Get_IdsString(ids, pbstrName, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int IdsString[int ids][WideString pbstrName] = {read=_scw_Get_IdsString};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_TmcGetName(int tmc, /* out */ WideString &pbstrName) { int r; HRESULT hr = Get_TmcGetName(tmc, pbstrName, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int TmcGetName[int tmc][WideString pbstrName] = {read=_scw_Get_TmcGetName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_AdaptiveMenus() { Word r; HRESULT hr = Get_AdaptiveMenus(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word AdaptiveMenus = {read=_scw_Get_AdaptiveMenus};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DisplayFonts() { Word r; HRESULT hr = Get_DisplayFonts(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DisplayFonts = {read=_scw_Get_DisplayFonts};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DisableCustomize() { Word r; HRESULT hr = Get_DisableCustomize(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DisableCustomize = {read=_scw_Get_DisableCustomize};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DisableAskAQuestionDropdown() { Word r; HRESULT hr = Get_DisableAskAQuestionDropdown(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DisableAskAQuestionDropdown = {read=_scw_Get_DisableAskAQuestionDropdown};
};

__dispinterface _CommandBarsDisp;
typedef System::DelphiInterface<_CommandBarsDisp> _di__CommandBarsDisp;
__dispinterface INTERFACE_UUID("{000C0302-0000-0000-C000-000000000046}") _CommandBarsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C0304-0000-0000-C000-000000000046}") CommandBar  : public _IMsoOleAccDispObj 
{
	
public:
	virtual HRESULT __safecall Get_BuiltIn(Word &Get_BuiltIn_result) = 0 ;
	virtual HRESULT __safecall Get_Context(WideString &Get_Context_result) = 0 ;
	virtual HRESULT __safecall Set_Context(const WideString pbstrContext) = 0 ;
	virtual HRESULT __safecall Get_Controls(_di_CommandBarControls &Get_Controls_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Get_Enabled(Word &Get_Enabled_result) = 0 ;
	virtual HRESULT __safecall Set_Enabled(Word pvarfEnabled) = 0 ;
	virtual HRESULT __safecall FindControl(const OleVariant Type_, const OleVariant Id, const OleVariant Tag, const OleVariant Visible, const OleVariant Recursive, _di_CommandBarControl &FindControl_result) = 0 ;
	virtual HRESULT __safecall Get_Height(int &Get_Height_result) = 0 ;
	virtual HRESULT __safecall Set_Height(int pdy) = 0 ;
	virtual HRESULT __safecall Get_Index(int &Get_Index_result) = 0 ;
	virtual HRESULT __safecall Get_InstanceId(int &Get_InstanceId_result) = 0 ;
	virtual HRESULT __safecall Get_Left(int &Get_Left_result) = 0 ;
	virtual HRESULT __safecall Set_Left(int pxpLeft) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString pbstrName) = 0 ;
	virtual HRESULT __safecall Get_NameLocal(WideString &Get_NameLocal_result) = 0 ;
	virtual HRESULT __safecall Set_NameLocal(const WideString pbstrNameLocal) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Position(Activex::TOleEnum &Get_Position_result) = 0 ;
	virtual HRESULT __safecall Set_Position(Activex::TOleEnum ppos) = 0 ;
	virtual HRESULT __safecall Get_RowIndex(int &Get_RowIndex_result) = 0 ;
	virtual HRESULT __safecall Set_RowIndex(int piRow) = 0 ;
	virtual HRESULT __safecall Get_Protection(Activex::TOleEnum &Get_Protection_result) = 0 ;
	virtual HRESULT __safecall Set_Protection(Activex::TOleEnum pprot) = 0 ;
	virtual HRESULT __safecall Reset(void) = 0 ;
	virtual HRESULT __safecall ShowPopup(const OleVariant x, const OleVariant y) = 0 ;
	virtual HRESULT __safecall Get_Top(int &Get_Top_result) = 0 ;
	virtual HRESULT __safecall Set_Top(int pypTop) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_Visible(Word &Get_Visible_result) = 0 ;
	virtual HRESULT __safecall Set_Visible(Word pvarfVisible) = 0 ;
	virtual HRESULT __safecall Get_Width(int &Get_Width_result) = 0 ;
	virtual HRESULT __safecall Set_Width(int pdx) = 0 ;
	virtual HRESULT __safecall Get_AdaptiveMenu(Word &Get_AdaptiveMenu_result) = 0 ;
	virtual HRESULT __safecall Set_AdaptiveMenu(Word pvarfAdaptiveMenu) = 0 ;
	virtual HRESULT __safecall Get_Id(int &Get_Id_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_BuiltIn() { Word r; HRESULT hr = Get_BuiltIn(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word BuiltIn = {read=_scw_Get_BuiltIn};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Context() { WideString r; HRESULT hr = Get_Context(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Context = {read=_scw_Get_Context};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CommandBarControls _scw_Get_Controls() { _di_CommandBarControls r; HRESULT hr = Get_Controls(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CommandBarControls Controls = {read=_scw_Get_Controls};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Enabled() { Word r; HRESULT hr = Get_Enabled(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Enabled = {read=_scw_Get_Enabled};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Height() { int r; HRESULT hr = Get_Height(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Height = {read=_scw_Get_Height};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Index() { int r; HRESULT hr = Get_Index(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Index = {read=_scw_Get_Index};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_InstanceId() { int r; HRESULT hr = Get_InstanceId(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int InstanceId = {read=_scw_Get_InstanceId};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Left() { int r; HRESULT hr = Get_Left(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Left = {read=_scw_Get_Left};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_NameLocal() { WideString r; HRESULT hr = Get_NameLocal(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString NameLocal = {read=_scw_Get_NameLocal};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Position() { Activex::TOleEnum r; HRESULT hr = Get_Position(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Position = {read=_scw_Get_Position};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_RowIndex() { int r; HRESULT hr = Get_RowIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int RowIndex = {read=_scw_Get_RowIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Protection() { Activex::TOleEnum r; HRESULT hr = Get_Protection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Protection = {read=_scw_Get_Protection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Top() { int r; HRESULT hr = Get_Top(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Top = {read=_scw_Get_Top};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Visible() { Word r; HRESULT hr = Get_Visible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Visible = {read=_scw_Get_Visible};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Width() { int r; HRESULT hr = Get_Width(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Width = {read=_scw_Get_Width};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_AdaptiveMenu() { Word r; HRESULT hr = Get_AdaptiveMenu(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word AdaptiveMenu = {read=_scw_Get_AdaptiveMenu};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Id() { int r; HRESULT hr = Get_Id(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Id = {read=_scw_Get_Id};
};

__dispinterface CommandBarDisp;
typedef System::DelphiInterface<CommandBarDisp> _di_CommandBarDisp;
__dispinterface INTERFACE_UUID("{000C0304-0000-0000-C000-000000000046}") CommandBarDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C0306-0000-0000-C000-000000000046}") CommandBarControls  : public _IMsoDispObj 
{
	
public:
	_di_CommandBarControl operator[](OleVariant Index) { return Item[Index]; }
	
public:
	virtual HRESULT __safecall Add(const OleVariant Type_, const OleVariant Id, const OleVariant Parameter, const OleVariant Before, const OleVariant Temporary, _di_CommandBarControl &Add_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get_Item(const OleVariant Index, _di_CommandBarControl &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_CommandBar &Get_Parent_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CommandBarControl _scw_Get_Item(const OleVariant Index) { _di_CommandBarControl r; HRESULT hr = Get_Item(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CommandBarControl Item[OleVariant Index] = {read=_scw_Get_Item/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CommandBar _scw_Get_Parent() { _di_CommandBar r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CommandBar Parent = {read=_scw_Get_Parent};
};

__dispinterface CommandBarControlsDisp;
typedef System::DelphiInterface<CommandBarControlsDisp> _di_CommandBarControlsDisp;
__dispinterface INTERFACE_UUID("{000C0306-0000-0000-C000-000000000046}") CommandBarControlsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C0308-0000-0000-C000-000000000046}") CommandBarControl  : public _IMsoOleAccDispObj 
{
	
public:
	virtual HRESULT __safecall Get_BeginGroup(Word &Get_BeginGroup_result) = 0 ;
	virtual HRESULT __safecall Set_BeginGroup(Word pvarfBeginGroup) = 0 ;
	virtual HRESULT __safecall Get_BuiltIn(Word &Get_BuiltIn_result) = 0 ;
	virtual HRESULT __safecall Get_Caption(WideString &Get_Caption_result) = 0 ;
	virtual HRESULT __safecall Set_Caption(const WideString pbstrCaption) = 0 ;
	virtual HRESULT __safecall Get_Control(_di_IDispatch &Get_Control_result) = 0 ;
	virtual HRESULT __safecall Copy(const OleVariant Bar, const OleVariant Before, _di_CommandBarControl &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(const OleVariant Temporary) = 0 ;
	virtual HRESULT __safecall Get_DescriptionText(WideString &Get_DescriptionText_result) = 0 ;
	virtual HRESULT __safecall Set_DescriptionText(const WideString pbstrText) = 0 ;
	virtual HRESULT __safecall Get_Enabled(Word &Get_Enabled_result) = 0 ;
	virtual HRESULT __safecall Set_Enabled(Word pvarfEnabled) = 0 ;
	virtual HRESULT __safecall Execute(void) = 0 ;
	virtual HRESULT __safecall Get_Height(int &Get_Height_result) = 0 ;
	virtual HRESULT __safecall Set_Height(int pdy) = 0 ;
	virtual HRESULT __safecall Get_HelpContextId(int &Get_HelpContextId_result) = 0 ;
	virtual HRESULT __safecall Set_HelpContextId(int pid) = 0 ;
	virtual HRESULT __safecall Get_HelpFile(WideString &Get_HelpFile_result) = 0 ;
	virtual HRESULT __safecall Set_HelpFile(const WideString pbstrFilename) = 0 ;
	virtual HRESULT __safecall Get_Id(int &Get_Id_result) = 0 ;
	virtual HRESULT __safecall Get_Index(int &Get_Index_result) = 0 ;
	virtual HRESULT __safecall Get_InstanceId(int &Get_InstanceId_result) = 0 ;
	virtual HRESULT __safecall Move(const OleVariant Bar, const OleVariant Before, _di_CommandBarControl &Move_result) = 0 ;
	virtual HRESULT __safecall Get_Left(int &Get_Left_result) = 0 ;
	virtual HRESULT __safecall Get_OLEUsage(Activex::TOleEnum &Get_OLEUsage_result) = 0 ;
	virtual HRESULT __safecall Set_OLEUsage(Activex::TOleEnum pcou) = 0 ;
	virtual HRESULT __safecall Get_OnAction(WideString &Get_OnAction_result) = 0 ;
	virtual HRESULT __safecall Set_OnAction(const WideString pbstrOnAction) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_CommandBar &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Parameter(WideString &Get_Parameter_result) = 0 ;
	virtual HRESULT __safecall Set_Parameter(const WideString pbstrParam) = 0 ;
	virtual HRESULT __safecall Get_Priority(int &Get_Priority_result) = 0 ;
	virtual HRESULT __safecall Set_Priority(int pnPri) = 0 ;
	virtual HRESULT __safecall Reset(void) = 0 ;
	virtual HRESULT __safecall SetFocus(void) = 0 ;
	virtual HRESULT __safecall Get_Tag(WideString &Get_Tag_result) = 0 ;
	virtual HRESULT __safecall Set_Tag(const WideString pbstrTag) = 0 ;
	virtual HRESULT __safecall Get_TooltipText(WideString &Get_TooltipText_result) = 0 ;
	virtual HRESULT __safecall Set_TooltipText(const WideString pbstrTooltip) = 0 ;
	virtual HRESULT __safecall Get_Top(int &Get_Top_result) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_Visible(Word &Get_Visible_result) = 0 ;
	virtual HRESULT __safecall Set_Visible(Word pvarfVisible) = 0 ;
	virtual HRESULT __safecall Get_Width(int &Get_Width_result) = 0 ;
	virtual HRESULT __safecall Set_Width(int pdx) = 0 ;
	virtual HRESULT __safecall Get_IsPriorityDropped(Word &Get_IsPriorityDropped_result) = 0 ;
	virtual HRESULT __safecall Reserved1(void) = 0 ;
	virtual HRESULT __safecall Reserved2(void) = 0 ;
	virtual HRESULT __safecall Reserved3(void) = 0 ;
	virtual HRESULT __safecall Reserved4(void) = 0 ;
	virtual HRESULT __safecall Reserved5(void) = 0 ;
	virtual HRESULT __safecall Reserved6(void) = 0 ;
	virtual HRESULT __safecall Reserved7(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_BeginGroup() { Word r; HRESULT hr = Get_BeginGroup(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word BeginGroup = {read=_scw_Get_BeginGroup};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_BuiltIn() { Word r; HRESULT hr = Get_BuiltIn(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word BuiltIn = {read=_scw_Get_BuiltIn};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Caption() { WideString r; HRESULT hr = Get_Caption(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Caption = {read=_scw_Get_Caption};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Control() { _di_IDispatch r; HRESULT hr = Get_Control(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Control = {read=_scw_Get_Control};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_DescriptionText() { WideString r; HRESULT hr = Get_DescriptionText(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString DescriptionText = {read=_scw_Get_DescriptionText};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Enabled() { Word r; HRESULT hr = Get_Enabled(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Enabled = {read=_scw_Get_Enabled};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Height() { int r; HRESULT hr = Get_Height(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Height = {read=_scw_Get_Height};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_HelpContextId() { int r; HRESULT hr = Get_HelpContextId(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int HelpContextId = {read=_scw_Get_HelpContextId};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HelpFile() { WideString r; HRESULT hr = Get_HelpFile(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HelpFile = {read=_scw_Get_HelpFile};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Id() { int r; HRESULT hr = Get_Id(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Id = {read=_scw_Get_Id};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Index() { int r; HRESULT hr = Get_Index(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Index = {read=_scw_Get_Index};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_InstanceId() { int r; HRESULT hr = Get_InstanceId(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int InstanceId = {read=_scw_Get_InstanceId};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Left() { int r; HRESULT hr = Get_Left(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Left = {read=_scw_Get_Left};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_OLEUsage() { Activex::TOleEnum r; HRESULT hr = Get_OLEUsage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum OLEUsage = {read=_scw_Get_OLEUsage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OnAction() { WideString r; HRESULT hr = Get_OnAction(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OnAction = {read=_scw_Get_OnAction};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CommandBar _scw_Get_Parent() { _di_CommandBar r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CommandBar Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Parameter() { WideString r; HRESULT hr = Get_Parameter(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Parameter = {read=_scw_Get_Parameter};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Priority() { int r; HRESULT hr = Get_Priority(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Priority = {read=_scw_Get_Priority};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Tag() { WideString r; HRESULT hr = Get_Tag(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Tag = {read=_scw_Get_Tag};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_TooltipText() { WideString r; HRESULT hr = Get_TooltipText(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString TooltipText = {read=_scw_Get_TooltipText};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Top() { int r; HRESULT hr = Get_Top(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Top = {read=_scw_Get_Top};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Visible() { Word r; HRESULT hr = Get_Visible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Visible = {read=_scw_Get_Visible};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Width() { int r; HRESULT hr = Get_Width(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Width = {read=_scw_Get_Width};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsPriorityDropped() { Word r; HRESULT hr = Get_IsPriorityDropped(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsPriorityDropped = {read=_scw_Get_IsPriorityDropped};
};

__dispinterface CommandBarControlDisp;
typedef System::DelphiInterface<CommandBarControlDisp> _di_CommandBarControlDisp;
__dispinterface INTERFACE_UUID("{000C0308-0000-0000-C000-000000000046}") CommandBarControlDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C030E-0000-0000-C000-000000000046}") _CommandBarButton  : public CommandBarControl 
{
	
public:
	virtual HRESULT __safecall Get_BuiltInFace(Word &Get_BuiltInFace_result) = 0 ;
	virtual HRESULT __safecall Set_BuiltInFace(Word pvarfBuiltIn) = 0 ;
	virtual HRESULT __safecall CopyFace(void) = 0 ;
	virtual HRESULT __safecall Get_FaceId(int &Get_FaceId_result) = 0 ;
	virtual HRESULT __safecall Set_FaceId(int pid) = 0 ;
	virtual HRESULT __safecall PasteFace(void) = 0 ;
	virtual HRESULT __safecall Get_ShortcutText(WideString &Get_ShortcutText_result) = 0 ;
	virtual HRESULT __safecall Set_ShortcutText(const WideString pbstrText) = 0 ;
	virtual HRESULT __safecall Get_State(Activex::TOleEnum &Get_State_result) = 0 ;
	virtual HRESULT __safecall Set_State(Activex::TOleEnum pstate) = 0 ;
	virtual HRESULT __safecall Get_Style(Activex::TOleEnum &Get_Style_result) = 0 ;
	virtual HRESULT __safecall Set_Style(Activex::TOleEnum pstyle) = 0 ;
	virtual HRESULT __safecall Get_HyperlinkType(Activex::TOleEnum &Get_HyperlinkType_result) = 0 ;
	virtual HRESULT __safecall Set_HyperlinkType(Activex::TOleEnum phlType) = 0 ;
	virtual HRESULT __safecall Get_Picture(_di_IPictureDisp &Get_Picture_result) = 0 ;
	virtual HRESULT __safecall Set_Picture(const _di_IPictureDisp ppdispPicture) = 0 ;
	virtual HRESULT __safecall Get_Mask(_di_IPictureDisp &Get_Mask_result) = 0 ;
	virtual HRESULT __safecall Set_Mask(const _di_IPictureDisp ppipictdispMask) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_BuiltInFace() { Word r; HRESULT hr = Get_BuiltInFace(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word BuiltInFace = {read=_scw_Get_BuiltInFace};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_FaceId() { int r; HRESULT hr = Get_FaceId(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int FaceId = {read=_scw_Get_FaceId};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ShortcutText() { WideString r; HRESULT hr = Get_ShortcutText(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ShortcutText = {read=_scw_Get_ShortcutText};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_State() { Activex::TOleEnum r; HRESULT hr = Get_State(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum State = {read=_scw_Get_State};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Style() { Activex::TOleEnum r; HRESULT hr = Get_Style(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Style = {read=_scw_Get_Style};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_HyperlinkType() { Activex::TOleEnum r; HRESULT hr = Get_HyperlinkType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum HyperlinkType = {read=_scw_Get_HyperlinkType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IPictureDisp _scw_Get_Picture() { _di_IPictureDisp r; HRESULT hr = Get_Picture(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IPictureDisp Picture = {read=_scw_Get_Picture};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IPictureDisp _scw_Get_Mask() { _di_IPictureDisp r; HRESULT hr = Get_Mask(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IPictureDisp Mask = {read=_scw_Get_Mask};
};

__dispinterface _CommandBarButtonDisp;
typedef System::DelphiInterface<_CommandBarButtonDisp> _di__CommandBarButtonDisp;
__dispinterface INTERFACE_UUID("{000C030E-0000-0000-C000-000000000046}") _CommandBarButtonDisp  : public IDispatch 
{
	
};

__interface CommandBarPopup;
typedef System::DelphiInterface<CommandBarPopup> _di_CommandBarPopup;
__interface INTERFACE_UUID("{000C030A-0000-0000-C000-000000000046}") CommandBarPopup  : public CommandBarControl 
{
	
public:
	virtual HRESULT __safecall Get_CommandBar(_di_CommandBar &Get_CommandBar_result) = 0 ;
	virtual HRESULT __safecall Get_Controls(_di_CommandBarControls &Get_Controls_result) = 0 ;
	virtual HRESULT __safecall Get_OLEMenuGroup(Activex::TOleEnum &Get_OLEMenuGroup_result) = 0 ;
	virtual HRESULT __safecall Set_OLEMenuGroup(Activex::TOleEnum pomg) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CommandBar _scw_Get_CommandBar() { _di_CommandBar r; HRESULT hr = Get_CommandBar(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CommandBar CommandBar = {read=_scw_Get_CommandBar};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CommandBarControls _scw_Get_Controls() { _di_CommandBarControls r; HRESULT hr = Get_Controls(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CommandBarControls Controls = {read=_scw_Get_Controls};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_OLEMenuGroup() { Activex::TOleEnum r; HRESULT hr = Get_OLEMenuGroup(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum OLEMenuGroup = {read=_scw_Get_OLEMenuGroup};
};

__dispinterface CommandBarPopupDisp;
typedef System::DelphiInterface<CommandBarPopupDisp> _di_CommandBarPopupDisp;
__dispinterface INTERFACE_UUID("{000C030A-0000-0000-C000-000000000046}") CommandBarPopupDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C030C-0000-0000-C000-000000000046}") _CommandBarComboBox  : public CommandBarControl 
{
	
public:
	virtual HRESULT __safecall AddItem(const WideString Text, const OleVariant Index) = 0 ;
	virtual HRESULT __safecall Clear(void) = 0 ;
	virtual HRESULT __safecall Get_DropDownLines(int &Get_DropDownLines_result) = 0 ;
	virtual HRESULT __safecall Set_DropDownLines(int pcLines) = 0 ;
	virtual HRESULT __safecall Get_DropDownWidth(int &Get_DropDownWidth_result) = 0 ;
	virtual HRESULT __safecall Set_DropDownWidth(int pdx) = 0 ;
	virtual HRESULT __safecall Get_List(int Index, WideString &Get_List_result) = 0 ;
	virtual HRESULT __safecall Set_List(int Index, const WideString pbstrItem) = 0 ;
	virtual HRESULT __safecall Get_ListCount(int &Get_ListCount_result) = 0 ;
	virtual HRESULT __safecall Get_ListHeaderCount(int &Get_ListHeaderCount_result) = 0 ;
	virtual HRESULT __safecall Set_ListHeaderCount(int pcItems) = 0 ;
	virtual HRESULT __safecall Get_ListIndex(int &Get_ListIndex_result) = 0 ;
	virtual HRESULT __safecall Set_ListIndex(int pi) = 0 ;
	virtual HRESULT __safecall RemoveItem(int Index) = 0 ;
	virtual HRESULT __safecall Get_Style(Activex::TOleEnum &Get_Style_result) = 0 ;
	virtual HRESULT __safecall Set_Style(Activex::TOleEnum pstyle) = 0 ;
	virtual HRESULT __safecall Get_Text(WideString &Get_Text_result) = 0 ;
	virtual HRESULT __safecall Set_Text(const WideString pbstrText) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_DropDownLines() { int r; HRESULT hr = Get_DropDownLines(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int DropDownLines = {read=_scw_Get_DropDownLines};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_DropDownWidth() { int r; HRESULT hr = Get_DropDownWidth(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int DropDownWidth = {read=_scw_Get_DropDownWidth};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_List(int Index) { WideString r; HRESULT hr = Get_List(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString List[int Index] = {read=_scw_Get_List};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ListCount() { int r; HRESULT hr = Get_ListCount(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int ListCount = {read=_scw_Get_ListCount};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ListHeaderCount() { int r; HRESULT hr = Get_ListHeaderCount(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int ListHeaderCount = {read=_scw_Get_ListHeaderCount};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ListIndex() { int r; HRESULT hr = Get_ListIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int ListIndex = {read=_scw_Get_ListIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Style() { Activex::TOleEnum r; HRESULT hr = Get_Style(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Style = {read=_scw_Get_Style};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Text() { WideString r; HRESULT hr = Get_Text(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Text = {read=_scw_Get_Text};
};

__dispinterface _CommandBarComboBoxDisp;
typedef System::DelphiInterface<_CommandBarComboBoxDisp> _di__CommandBarComboBoxDisp;
__dispinterface INTERFACE_UUID("{000C030C-0000-0000-C000-000000000046}") _CommandBarComboBoxDisp  : public IDispatch 
{
	
};

__interface _CommandBarActiveX;
typedef System::DelphiInterface<_CommandBarActiveX> _di__CommandBarActiveX;
__interface INTERFACE_UUID("{000C030D-0000-0000-C000-000000000046}") _CommandBarActiveX  : public CommandBarControl 
{
	
public:
	virtual HRESULT __safecall Get_ControlCLSID(WideString &Get_ControlCLSID_result) = 0 ;
	virtual HRESULT __safecall Set_ControlCLSID(const WideString pbstrClsid) = 0 ;
	virtual HRESULT __safecall Get_QueryControlInterface(const WideString bstrIid, System::_di_IInterface &Get_QueryControlInterface_result) = 0 ;
	virtual HRESULT __safecall SetInnerObjectFactory(const System::_di_IInterface pUnk) = 0 ;
	virtual HRESULT __safecall EnsureControl(void) = 0 ;
	virtual HRESULT __safecall Set_InitWith(const System::_di_IInterface Param1) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ControlCLSID() { WideString r; HRESULT hr = Get_ControlCLSID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ControlCLSID = {read=_scw_Get_ControlCLSID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_QueryControlInterface(const WideString bstrIid) { System::_di_IInterface r; HRESULT hr = Get_QueryControlInterface(bstrIid, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface QueryControlInterface[WideString bstrIid] = {read=_scw_Get_QueryControlInterface};
	__property System::_di_IInterface InitWith = {write=Set_InitWith};
};

__dispinterface _CommandBarActiveXDisp;
typedef System::DelphiInterface<_CommandBarActiveXDisp> _di__CommandBarActiveXDisp;
__dispinterface INTERFACE_UUID("{000C030D-0000-0000-C000-000000000046}") _CommandBarActiveXDisp  : public IDispatch 
{
	
};

__interface Adjustments;
typedef System::DelphiInterface<Adjustments> _di_Adjustments;
__interface INTERFACE_UUID("{000C0310-0000-0000-C000-000000000046}") Adjustments  : public _IMsoDispObj 
{
	
public:
	float operator[](int Index) { return Item[Index]; }
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get_Item(int Index, float &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Set_Item(int Index, float Val) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Item(int Index) { float r; HRESULT hr = Get_Item(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Item[int Index] = {read=_scw_Get_Item, write=Set_Item/*, default*/};
};

__dispinterface AdjustmentsDisp;
typedef System::DelphiInterface<AdjustmentsDisp> _di_AdjustmentsDisp;
__dispinterface INTERFACE_UUID("{000C0310-0000-0000-C000-000000000046}") AdjustmentsDisp  : public IDispatch 
{
	
};

__interface CalloutFormat;
typedef System::DelphiInterface<CalloutFormat> _di_CalloutFormat;
__interface INTERFACE_UUID("{000C0311-0000-0000-C000-000000000046}") CalloutFormat  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall AutomaticLength(void) = 0 ;
	virtual HRESULT __safecall CustomDrop(float Drop) = 0 ;
	virtual HRESULT __safecall CustomLength(float Length) = 0 ;
	virtual HRESULT __safecall PresetDrop(Activex::TOleEnum DropType) = 0 ;
	virtual HRESULT __safecall Get_Accent(Activex::TOleEnum &Get_Accent_result) = 0 ;
	virtual HRESULT __safecall Set_Accent(Activex::TOleEnum Accent) = 0 ;
	virtual HRESULT __safecall Get_Angle(Activex::TOleEnum &Get_Angle_result) = 0 ;
	virtual HRESULT __safecall Set_Angle(Activex::TOleEnum Angle) = 0 ;
	virtual HRESULT __safecall Get_AutoAttach(Activex::TOleEnum &Get_AutoAttach_result) = 0 ;
	virtual HRESULT __safecall Set_AutoAttach(Activex::TOleEnum AutoAttach) = 0 ;
	virtual HRESULT __safecall Get_AutoLength(Activex::TOleEnum &Get_AutoLength_result) = 0 ;
	virtual HRESULT __safecall Get_Border(Activex::TOleEnum &Get_Border_result) = 0 ;
	virtual HRESULT __safecall Set_Border(Activex::TOleEnum Border) = 0 ;
	virtual HRESULT __safecall Get_Drop(float &Get_Drop_result) = 0 ;
	virtual HRESULT __safecall Get_DropType(Activex::TOleEnum &Get_DropType_result) = 0 ;
	virtual HRESULT __safecall Get_Gap(float &Get_Gap_result) = 0 ;
	virtual HRESULT __safecall Set_Gap(float Gap) = 0 ;
	virtual HRESULT __safecall Get_Length(float &Get_Length_result) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Set_Type_(Activex::TOleEnum Type_) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Accent() { Activex::TOleEnum r; HRESULT hr = Get_Accent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Accent = {read=_scw_Get_Accent, write=Set_Accent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Angle() { Activex::TOleEnum r; HRESULT hr = Get_Angle(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Angle = {read=_scw_Get_Angle, write=Set_Angle};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_AutoAttach() { Activex::TOleEnum r; HRESULT hr = Get_AutoAttach(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum AutoAttach = {read=_scw_Get_AutoAttach, write=Set_AutoAttach};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_AutoLength() { Activex::TOleEnum r; HRESULT hr = Get_AutoLength(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum AutoLength = {read=_scw_Get_AutoLength};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Border() { Activex::TOleEnum r; HRESULT hr = Get_Border(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Border = {read=_scw_Get_Border, write=Set_Border};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Drop() { float r; HRESULT hr = Get_Drop(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Drop = {read=_scw_Get_Drop};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DropType() { Activex::TOleEnum r; HRESULT hr = Get_DropType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DropType = {read=_scw_Get_DropType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Gap() { float r; HRESULT hr = Get_Gap(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Gap = {read=_scw_Get_Gap, write=Set_Gap};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Length() { float r; HRESULT hr = Get_Length(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Length = {read=_scw_Get_Length};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_, write=Set_Type_};
};

__dispinterface CalloutFormatDisp;
typedef System::DelphiInterface<CalloutFormatDisp> _di_CalloutFormatDisp;
__dispinterface INTERFACE_UUID("{000C0311-0000-0000-C000-000000000046}") CalloutFormatDisp  : public IDispatch 
{
	
};

__interface ColorFormat;
typedef System::DelphiInterface<ColorFormat> _di_ColorFormat;
__interface INTERFACE_UUID("{000C0312-0000-0000-C000-000000000046}") ColorFormat  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_RGB(int &Get_RGB_result) = 0 ;
	virtual HRESULT __safecall Set_RGB(int RGB) = 0 ;
	virtual HRESULT __safecall Get_SchemeColor(int &Get_SchemeColor_result) = 0 ;
	virtual HRESULT __safecall Set_SchemeColor(int SchemeColor) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_TintAndShade(float &Get_TintAndShade_result) = 0 ;
	virtual HRESULT __safecall Set_TintAndShade(float pValue) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_RGB() { int r; HRESULT hr = Get_RGB(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int RGB = {read=_scw_Get_RGB, write=Set_RGB};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_SchemeColor() { int r; HRESULT hr = Get_SchemeColor(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int SchemeColor = {read=_scw_Get_SchemeColor, write=Set_SchemeColor};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_TintAndShade() { float r; HRESULT hr = Get_TintAndShade(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float TintAndShade = {read=_scw_Get_TintAndShade, write=Set_TintAndShade};
};

__dispinterface ColorFormatDisp;
typedef System::DelphiInterface<ColorFormatDisp> _di_ColorFormatDisp;
__dispinterface INTERFACE_UUID("{000C0312-0000-0000-C000-000000000046}") ColorFormatDisp  : public IDispatch 
{
	
};

__interface ConnectorFormat;
typedef System::DelphiInterface<ConnectorFormat> _di_ConnectorFormat;
__interface Shape;
typedef System::DelphiInterface<Shape> _di_Shape;
__interface INTERFACE_UUID("{000C0313-0000-0000-C000-000000000046}") ConnectorFormat  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall BeginConnect(const _di_Shape ConnectedShape, int ConnectionSite) = 0 ;
	virtual HRESULT __safecall BeginDisconnect(void) = 0 ;
	virtual HRESULT __safecall EndConnect(const _di_Shape ConnectedShape, int ConnectionSite) = 0 ;
	virtual HRESULT __safecall EndDisconnect(void) = 0 ;
	virtual HRESULT __safecall Get_BeginConnected(Activex::TOleEnum &Get_BeginConnected_result) = 0 ;
	virtual HRESULT __safecall Get_BeginConnectedShape(_di_Shape &Get_BeginConnectedShape_result) = 0 ;
	virtual HRESULT __safecall Get_BeginConnectionSite(int &Get_BeginConnectionSite_result) = 0 ;
	virtual HRESULT __safecall Get_EndConnected(Activex::TOleEnum &Get_EndConnected_result) = 0 ;
	virtual HRESULT __safecall Get_EndConnectedShape(_di_Shape &Get_EndConnectedShape_result) = 0 ;
	virtual HRESULT __safecall Get_EndConnectionSite(int &Get_EndConnectionSite_result) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Set_Type_(Activex::TOleEnum Type_) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_BeginConnected() { Activex::TOleEnum r; HRESULT hr = Get_BeginConnected(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum BeginConnected = {read=_scw_Get_BeginConnected};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Shape _scw_Get_BeginConnectedShape() { _di_Shape r; HRESULT hr = Get_BeginConnectedShape(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Shape BeginConnectedShape = {read=_scw_Get_BeginConnectedShape};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_BeginConnectionSite() { int r; HRESULT hr = Get_BeginConnectionSite(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int BeginConnectionSite = {read=_scw_Get_BeginConnectionSite};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_EndConnected() { Activex::TOleEnum r; HRESULT hr = Get_EndConnected(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum EndConnected = {read=_scw_Get_EndConnected};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Shape _scw_Get_EndConnectedShape() { _di_Shape r; HRESULT hr = Get_EndConnectedShape(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Shape EndConnectedShape = {read=_scw_Get_EndConnectedShape};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_EndConnectionSite() { int r; HRESULT hr = Get_EndConnectionSite(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int EndConnectionSite = {read=_scw_Get_EndConnectionSite};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_, write=Set_Type_};
};

__dispinterface ConnectorFormatDisp;
typedef System::DelphiInterface<ConnectorFormatDisp> _di_ConnectorFormatDisp;
__dispinterface INTERFACE_UUID("{000C0313-0000-0000-C000-000000000046}") ConnectorFormatDisp  : public IDispatch 
{
	
};

__interface FillFormat;
typedef System::DelphiInterface<FillFormat> _di_FillFormat;
__interface INTERFACE_UUID("{000C0314-0000-0000-C000-000000000046}") FillFormat  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Background(void) = 0 ;
	virtual HRESULT __safecall OneColorGradient(Activex::TOleEnum Style, int Variant, float Degree) = 0 ;
	virtual HRESULT __safecall Patterned(Activex::TOleEnum Pattern) = 0 ;
	virtual HRESULT __safecall PresetGradient(Activex::TOleEnum Style, int Variant, Activex::TOleEnum PresetGradientType) = 0 ;
	virtual HRESULT __safecall PresetTextured(Activex::TOleEnum PresetTexture) = 0 ;
	virtual HRESULT __safecall Solid(void) = 0 ;
	virtual HRESULT __safecall TwoColorGradient(Activex::TOleEnum Style, int Variant) = 0 ;
	virtual HRESULT __safecall UserPicture(const WideString PictureFile) = 0 ;
	virtual HRESULT __safecall UserTextured(const WideString TextureFile) = 0 ;
	virtual HRESULT __safecall Get_BackColor(_di_ColorFormat &Get_BackColor_result) = 0 ;
	virtual HRESULT __safecall Set_BackColor(const _di_ColorFormat BackColor) = 0 ;
	virtual HRESULT __safecall Get_ForeColor(_di_ColorFormat &Get_ForeColor_result) = 0 ;
	virtual HRESULT __safecall Set_ForeColor(const _di_ColorFormat ForeColor) = 0 ;
	virtual HRESULT __safecall Get_GradientColorType(Activex::TOleEnum &Get_GradientColorType_result) = 0 ;
	virtual HRESULT __safecall Get_GradientDegree(float &Get_GradientDegree_result) = 0 ;
	virtual HRESULT __safecall Get_GradientStyle(Activex::TOleEnum &Get_GradientStyle_result) = 0 ;
	virtual HRESULT __safecall Get_GradientVariant(int &Get_GradientVariant_result) = 0 ;
	virtual HRESULT __safecall Get_Pattern(Activex::TOleEnum &Get_Pattern_result) = 0 ;
	virtual HRESULT __safecall Get_PresetGradientType(Activex::TOleEnum &Get_PresetGradientType_result) = 0 ;
	virtual HRESULT __safecall Get_PresetTexture(Activex::TOleEnum &Get_PresetTexture_result) = 0 ;
	virtual HRESULT __safecall Get_TextureName(WideString &Get_TextureName_result) = 0 ;
	virtual HRESULT __safecall Get_TextureType(Activex::TOleEnum &Get_TextureType_result) = 0 ;
	virtual HRESULT __safecall Get_Transparency(float &Get_Transparency_result) = 0 ;
	virtual HRESULT __safecall Set_Transparency(float Transparency) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_Visible(Activex::TOleEnum &Get_Visible_result) = 0 ;
	virtual HRESULT __safecall Set_Visible(Activex::TOleEnum Visible) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ColorFormat _scw_Get_BackColor() { _di_ColorFormat r; HRESULT hr = Get_BackColor(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ColorFormat BackColor = {read=_scw_Get_BackColor, write=Set_BackColor};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ColorFormat _scw_Get_ForeColor() { _di_ColorFormat r; HRESULT hr = Get_ForeColor(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ColorFormat ForeColor = {read=_scw_Get_ForeColor, write=Set_ForeColor};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_GradientColorType() { Activex::TOleEnum r; HRESULT hr = Get_GradientColorType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum GradientColorType = {read=_scw_Get_GradientColorType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_GradientDegree() { float r; HRESULT hr = Get_GradientDegree(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float GradientDegree = {read=_scw_Get_GradientDegree};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_GradientStyle() { Activex::TOleEnum r; HRESULT hr = Get_GradientStyle(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum GradientStyle = {read=_scw_Get_GradientStyle};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_GradientVariant() { int r; HRESULT hr = Get_GradientVariant(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int GradientVariant = {read=_scw_Get_GradientVariant};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Pattern() { Activex::TOleEnum r; HRESULT hr = Get_Pattern(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Pattern = {read=_scw_Get_Pattern};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_PresetGradientType() { Activex::TOleEnum r; HRESULT hr = Get_PresetGradientType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum PresetGradientType = {read=_scw_Get_PresetGradientType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_PresetTexture() { Activex::TOleEnum r; HRESULT hr = Get_PresetTexture(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum PresetTexture = {read=_scw_Get_PresetTexture};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_TextureName() { WideString r; HRESULT hr = Get_TextureName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString TextureName = {read=_scw_Get_TextureName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_TextureType() { Activex::TOleEnum r; HRESULT hr = Get_TextureType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum TextureType = {read=_scw_Get_TextureType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Transparency() { float r; HRESULT hr = Get_Transparency(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Transparency = {read=_scw_Get_Transparency, write=Set_Transparency};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Visible() { Activex::TOleEnum r; HRESULT hr = Get_Visible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Visible = {read=_scw_Get_Visible, write=Set_Visible};
};

__dispinterface FillFormatDisp;
typedef System::DelphiInterface<FillFormatDisp> _di_FillFormatDisp;
__dispinterface INTERFACE_UUID("{000C0314-0000-0000-C000-000000000046}") FillFormatDisp  : public IDispatch 
{
	
};

__interface FreeformBuilder;
typedef System::DelphiInterface<FreeformBuilder> _di_FreeformBuilder;
__interface INTERFACE_UUID("{000C0315-0000-0000-C000-000000000046}") FreeformBuilder  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall AddNodes(Activex::TOleEnum SegmentType, Activex::TOleEnum EditingType, float X1, float Y1, float X2, float Y2, float X3, float Y3) = 0 ;
	virtual HRESULT __safecall ConvertToShape(_di_Shape &ConvertToShape_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
};

__dispinterface FreeformBuilderDisp;
typedef System::DelphiInterface<FreeformBuilderDisp> _di_FreeformBuilderDisp;
__dispinterface INTERFACE_UUID("{000C0315-0000-0000-C000-000000000046}") FreeformBuilderDisp  : public IDispatch 
{
	
};

__interface GroupShapes;
typedef System::DelphiInterface<GroupShapes> _di_GroupShapes;
__interface ShapeRange;
typedef System::DelphiInterface<ShapeRange> _di_ShapeRange;
__interface INTERFACE_UUID("{000C0316-0000-0000-C000-000000000046}") GroupShapes  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_Shape &Item_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Range(const OleVariant Index, _di_ShapeRange &Range_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface GroupShapesDisp;
typedef System::DelphiInterface<GroupShapesDisp> _di_GroupShapesDisp;
__dispinterface INTERFACE_UUID("{000C0316-0000-0000-C000-000000000046}") GroupShapesDisp  : public IDispatch 
{
	
};

__interface LineFormat;
typedef System::DelphiInterface<LineFormat> _di_LineFormat;
__interface INTERFACE_UUID("{000C0317-0000-0000-C000-000000000046}") LineFormat  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_BackColor(_di_ColorFormat &Get_BackColor_result) = 0 ;
	virtual HRESULT __safecall Set_BackColor(const _di_ColorFormat BackColor) = 0 ;
	virtual HRESULT __safecall Get_BeginArrowheadLength(Activex::TOleEnum &Get_BeginArrowheadLength_result) = 0 ;
	virtual HRESULT __safecall Set_BeginArrowheadLength(Activex::TOleEnum BeginArrowheadLength) = 0 ;
	virtual HRESULT __safecall Get_BeginArrowheadStyle(Activex::TOleEnum &Get_BeginArrowheadStyle_result) = 0 ;
	virtual HRESULT __safecall Set_BeginArrowheadStyle(Activex::TOleEnum BeginArrowheadStyle) = 0 ;
	virtual HRESULT __safecall Get_BeginArrowheadWidth(Activex::TOleEnum &Get_BeginArrowheadWidth_result) = 0 ;
	virtual HRESULT __safecall Set_BeginArrowheadWidth(Activex::TOleEnum BeginArrowheadWidth) = 0 ;
	virtual HRESULT __safecall Get_DashStyle(Activex::TOleEnum &Get_DashStyle_result) = 0 ;
	virtual HRESULT __safecall Set_DashStyle(Activex::TOleEnum DashStyle) = 0 ;
	virtual HRESULT __safecall Get_EndArrowheadLength(Activex::TOleEnum &Get_EndArrowheadLength_result) = 0 ;
	virtual HRESULT __safecall Set_EndArrowheadLength(Activex::TOleEnum EndArrowheadLength) = 0 ;
	virtual HRESULT __safecall Get_EndArrowheadStyle(Activex::TOleEnum &Get_EndArrowheadStyle_result) = 0 ;
	virtual HRESULT __safecall Set_EndArrowheadStyle(Activex::TOleEnum EndArrowheadStyle) = 0 ;
	virtual HRESULT __safecall Get_EndArrowheadWidth(Activex::TOleEnum &Get_EndArrowheadWidth_result) = 0 ;
	virtual HRESULT __safecall Set_EndArrowheadWidth(Activex::TOleEnum EndArrowheadWidth) = 0 ;
	virtual HRESULT __safecall Get_ForeColor(_di_ColorFormat &Get_ForeColor_result) = 0 ;
	virtual HRESULT __safecall Set_ForeColor(const _di_ColorFormat ForeColor) = 0 ;
	virtual HRESULT __safecall Get_Pattern(Activex::TOleEnum &Get_Pattern_result) = 0 ;
	virtual HRESULT __safecall Set_Pattern(Activex::TOleEnum Pattern) = 0 ;
	virtual HRESULT __safecall Get_Style(Activex::TOleEnum &Get_Style_result) = 0 ;
	virtual HRESULT __safecall Set_Style(Activex::TOleEnum Style) = 0 ;
	virtual HRESULT __safecall Get_Transparency(float &Get_Transparency_result) = 0 ;
	virtual HRESULT __safecall Set_Transparency(float Transparency) = 0 ;
	virtual HRESULT __safecall Get_Visible(Activex::TOleEnum &Get_Visible_result) = 0 ;
	virtual HRESULT __safecall Set_Visible(Activex::TOleEnum Visible) = 0 ;
	virtual HRESULT __safecall Get_Weight(float &Get_Weight_result) = 0 ;
	virtual HRESULT __safecall Set_Weight(float Weight) = 0 ;
	virtual HRESULT __safecall Get_InsetPen(Activex::TOleEnum &Get_InsetPen_result) = 0 ;
	virtual HRESULT __safecall Set_InsetPen(Activex::TOleEnum InsetPen) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ColorFormat _scw_Get_BackColor() { _di_ColorFormat r; HRESULT hr = Get_BackColor(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ColorFormat BackColor = {read=_scw_Get_BackColor, write=Set_BackColor};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_BeginArrowheadLength() { Activex::TOleEnum r; HRESULT hr = Get_BeginArrowheadLength(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum BeginArrowheadLength = {read=_scw_Get_BeginArrowheadLength, write=Set_BeginArrowheadLength};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_BeginArrowheadStyle() { Activex::TOleEnum r; HRESULT hr = Get_BeginArrowheadStyle(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum BeginArrowheadStyle = {read=_scw_Get_BeginArrowheadStyle, write=Set_BeginArrowheadStyle};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_BeginArrowheadWidth() { Activex::TOleEnum r; HRESULT hr = Get_BeginArrowheadWidth(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum BeginArrowheadWidth = {read=_scw_Get_BeginArrowheadWidth, write=Set_BeginArrowheadWidth};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DashStyle() { Activex::TOleEnum r; HRESULT hr = Get_DashStyle(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DashStyle = {read=_scw_Get_DashStyle, write=Set_DashStyle};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_EndArrowheadLength() { Activex::TOleEnum r; HRESULT hr = Get_EndArrowheadLength(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum EndArrowheadLength = {read=_scw_Get_EndArrowheadLength, write=Set_EndArrowheadLength};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_EndArrowheadStyle() { Activex::TOleEnum r; HRESULT hr = Get_EndArrowheadStyle(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum EndArrowheadStyle = {read=_scw_Get_EndArrowheadStyle, write=Set_EndArrowheadStyle};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_EndArrowheadWidth() { Activex::TOleEnum r; HRESULT hr = Get_EndArrowheadWidth(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum EndArrowheadWidth = {read=_scw_Get_EndArrowheadWidth, write=Set_EndArrowheadWidth};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ColorFormat _scw_Get_ForeColor() { _di_ColorFormat r; HRESULT hr = Get_ForeColor(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ColorFormat ForeColor = {read=_scw_Get_ForeColor, write=Set_ForeColor};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Pattern() { Activex::TOleEnum r; HRESULT hr = Get_Pattern(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Pattern = {read=_scw_Get_Pattern, write=Set_Pattern};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Style() { Activex::TOleEnum r; HRESULT hr = Get_Style(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Style = {read=_scw_Get_Style, write=Set_Style};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Transparency() { float r; HRESULT hr = Get_Transparency(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Transparency = {read=_scw_Get_Transparency, write=Set_Transparency};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Visible() { Activex::TOleEnum r; HRESULT hr = Get_Visible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Visible = {read=_scw_Get_Visible, write=Set_Visible};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Weight() { float r; HRESULT hr = Get_Weight(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Weight = {read=_scw_Get_Weight, write=Set_Weight};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_InsetPen() { Activex::TOleEnum r; HRESULT hr = Get_InsetPen(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum InsetPen = {read=_scw_Get_InsetPen, write=Set_InsetPen};
};

__dispinterface LineFormatDisp;
typedef System::DelphiInterface<LineFormatDisp> _di_LineFormatDisp;
__dispinterface INTERFACE_UUID("{000C0317-0000-0000-C000-000000000046}") LineFormatDisp  : public IDispatch 
{
	
};

__interface ShapeNode;
typedef System::DelphiInterface<ShapeNode> _di_ShapeNode;
__interface INTERFACE_UUID("{000C0318-0000-0000-C000-000000000046}") ShapeNode  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_EditingType(Activex::TOleEnum &Get_EditingType_result) = 0 ;
	virtual HRESULT __safecall Get_Points(OleVariant &Get_Points_result) = 0 ;
	virtual HRESULT __safecall Get_SegmentType(Activex::TOleEnum &Get_SegmentType_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_EditingType() { Activex::TOleEnum r; HRESULT hr = Get_EditingType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum EditingType = {read=_scw_Get_EditingType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_Points() { OleVariant r; HRESULT hr = Get_Points(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant Points = {read=_scw_Get_Points};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_SegmentType() { Activex::TOleEnum r; HRESULT hr = Get_SegmentType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum SegmentType = {read=_scw_Get_SegmentType};
};

__dispinterface ShapeNodeDisp;
typedef System::DelphiInterface<ShapeNodeDisp> _di_ShapeNodeDisp;
__dispinterface INTERFACE_UUID("{000C0318-0000-0000-C000-000000000046}") ShapeNodeDisp  : public IDispatch 
{
	
};

__interface ShapeNodes;
typedef System::DelphiInterface<ShapeNodes> _di_ShapeNodes;
__interface INTERFACE_UUID("{000C0319-0000-0000-C000-000000000046}") ShapeNodes  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_ShapeNode &Item_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Delete(int Index) = 0 ;
	virtual HRESULT __safecall Insert(int Index, Activex::TOleEnum SegmentType, Activex::TOleEnum EditingType, float X1, float Y1, float X2, float Y2, float X3, float Y3) = 0 ;
	virtual HRESULT __safecall SetEditingType(int Index, Activex::TOleEnum EditingType) = 0 ;
	virtual HRESULT __safecall SetPosition(int Index, float X1, float Y1) = 0 ;
	virtual HRESULT __safecall SetSegmentType(int Index, Activex::TOleEnum SegmentType) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface ShapeNodesDisp;
typedef System::DelphiInterface<ShapeNodesDisp> _di_ShapeNodesDisp;
__dispinterface INTERFACE_UUID("{000C0319-0000-0000-C000-000000000046}") ShapeNodesDisp  : public IDispatch 
{
	
};

__interface PictureFormat;
typedef System::DelphiInterface<PictureFormat> _di_PictureFormat;
__interface INTERFACE_UUID("{000C031A-0000-0000-C000-000000000046}") PictureFormat  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall IncrementBrightness(float Increment) = 0 ;
	virtual HRESULT __safecall IncrementContrast(float Increment) = 0 ;
	virtual HRESULT __safecall Get_Brightness(float &Get_Brightness_result) = 0 ;
	virtual HRESULT __safecall Set_Brightness(float Brightness) = 0 ;
	virtual HRESULT __safecall Get_ColorType(Activex::TOleEnum &Get_ColorType_result) = 0 ;
	virtual HRESULT __safecall Set_ColorType(Activex::TOleEnum ColorType) = 0 ;
	virtual HRESULT __safecall Get_Contrast(float &Get_Contrast_result) = 0 ;
	virtual HRESULT __safecall Set_Contrast(float Contrast) = 0 ;
	virtual HRESULT __safecall Get_CropBottom(float &Get_CropBottom_result) = 0 ;
	virtual HRESULT __safecall Set_CropBottom(float CropBottom) = 0 ;
	virtual HRESULT __safecall Get_CropLeft(float &Get_CropLeft_result) = 0 ;
	virtual HRESULT __safecall Set_CropLeft(float CropLeft) = 0 ;
	virtual HRESULT __safecall Get_CropRight(float &Get_CropRight_result) = 0 ;
	virtual HRESULT __safecall Set_CropRight(float CropRight) = 0 ;
	virtual HRESULT __safecall Get_CropTop(float &Get_CropTop_result) = 0 ;
	virtual HRESULT __safecall Set_CropTop(float CropTop) = 0 ;
	virtual HRESULT __safecall Get_TransparencyColor(int &Get_TransparencyColor_result) = 0 ;
	virtual HRESULT __safecall Set_TransparencyColor(int TransparencyColor) = 0 ;
	virtual HRESULT __safecall Get_TransparentBackground(Activex::TOleEnum &Get_TransparentBackground_result) = 0 ;
	virtual HRESULT __safecall Set_TransparentBackground(Activex::TOleEnum TransparentBackground) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Brightness() { float r; HRESULT hr = Get_Brightness(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Brightness = {read=_scw_Get_Brightness, write=Set_Brightness};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_ColorType() { Activex::TOleEnum r; HRESULT hr = Get_ColorType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum ColorType = {read=_scw_Get_ColorType, write=Set_ColorType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Contrast() { float r; HRESULT hr = Get_Contrast(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Contrast = {read=_scw_Get_Contrast, write=Set_Contrast};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_CropBottom() { float r; HRESULT hr = Get_CropBottom(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float CropBottom = {read=_scw_Get_CropBottom, write=Set_CropBottom};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_CropLeft() { float r; HRESULT hr = Get_CropLeft(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float CropLeft = {read=_scw_Get_CropLeft, write=Set_CropLeft};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_CropRight() { float r; HRESULT hr = Get_CropRight(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float CropRight = {read=_scw_Get_CropRight, write=Set_CropRight};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_CropTop() { float r; HRESULT hr = Get_CropTop(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float CropTop = {read=_scw_Get_CropTop, write=Set_CropTop};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_TransparencyColor() { int r; HRESULT hr = Get_TransparencyColor(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int TransparencyColor = {read=_scw_Get_TransparencyColor, write=Set_TransparencyColor};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_TransparentBackground() { Activex::TOleEnum r; HRESULT hr = Get_TransparentBackground(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum TransparentBackground = {read=_scw_Get_TransparentBackground, write=Set_TransparentBackground};
};

__dispinterface PictureFormatDisp;
typedef System::DelphiInterface<PictureFormatDisp> _di_PictureFormatDisp;
__dispinterface INTERFACE_UUID("{000C031A-0000-0000-C000-000000000046}") PictureFormatDisp  : public IDispatch 
{
	
};

__interface ShadowFormat;
typedef System::DelphiInterface<ShadowFormat> _di_ShadowFormat;
__interface INTERFACE_UUID("{000C031B-0000-0000-C000-000000000046}") ShadowFormat  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall IncrementOffsetX(float Increment) = 0 ;
	virtual HRESULT __safecall IncrementOffsetY(float Increment) = 0 ;
	virtual HRESULT __safecall Get_ForeColor(_di_ColorFormat &Get_ForeColor_result) = 0 ;
	virtual HRESULT __safecall Set_ForeColor(const _di_ColorFormat ForeColor) = 0 ;
	virtual HRESULT __safecall Get_Obscured(Activex::TOleEnum &Get_Obscured_result) = 0 ;
	virtual HRESULT __safecall Set_Obscured(Activex::TOleEnum Obscured) = 0 ;
	virtual HRESULT __safecall Get_OffsetX(float &Get_OffsetX_result) = 0 ;
	virtual HRESULT __safecall Set_OffsetX(float OffsetX) = 0 ;
	virtual HRESULT __safecall Get_OffsetY(float &Get_OffsetY_result) = 0 ;
	virtual HRESULT __safecall Set_OffsetY(float OffsetY) = 0 ;
	virtual HRESULT __safecall Get_Transparency(float &Get_Transparency_result) = 0 ;
	virtual HRESULT __safecall Set_Transparency(float Transparency) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Set_Type_(Activex::TOleEnum Type_) = 0 ;
	virtual HRESULT __safecall Get_Visible(Activex::TOleEnum &Get_Visible_result) = 0 ;
	virtual HRESULT __safecall Set_Visible(Activex::TOleEnum Visible) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ColorFormat _scw_Get_ForeColor() { _di_ColorFormat r; HRESULT hr = Get_ForeColor(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ColorFormat ForeColor = {read=_scw_Get_ForeColor, write=Set_ForeColor};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Obscured() { Activex::TOleEnum r; HRESULT hr = Get_Obscured(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Obscured = {read=_scw_Get_Obscured, write=Set_Obscured};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_OffsetX() { float r; HRESULT hr = Get_OffsetX(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float OffsetX = {read=_scw_Get_OffsetX, write=Set_OffsetX};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_OffsetY() { float r; HRESULT hr = Get_OffsetY(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float OffsetY = {read=_scw_Get_OffsetY, write=Set_OffsetY};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Transparency() { float r; HRESULT hr = Get_Transparency(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Transparency = {read=_scw_Get_Transparency, write=Set_Transparency};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_, write=Set_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Visible() { Activex::TOleEnum r; HRESULT hr = Get_Visible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Visible = {read=_scw_Get_Visible, write=Set_Visible};
};

__dispinterface ShadowFormatDisp;
typedef System::DelphiInterface<ShadowFormatDisp> _di_ShadowFormatDisp;
__dispinterface INTERFACE_UUID("{000C031B-0000-0000-C000-000000000046}") ShadowFormatDisp  : public IDispatch 
{
	
};

__interface Script;
typedef System::DelphiInterface<Script> _di_Script;
__interface INTERFACE_UUID("{000C0341-0000-0000-C000-000000000046}") Script  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Extended(WideString &Get_Extended_result) = 0 ;
	virtual HRESULT __safecall Set_Extended(const WideString Extended) = 0 ;
	virtual HRESULT __safecall Get_Id(WideString &Get_Id_result) = 0 ;
	virtual HRESULT __safecall Set_Id(const WideString Id) = 0 ;
	virtual HRESULT __safecall Get_Language(Activex::TOleEnum &Get_Language_result) = 0 ;
	virtual HRESULT __safecall Set_Language(Activex::TOleEnum Language) = 0 ;
	virtual HRESULT __safecall Get_Location(Activex::TOleEnum &Get_Location_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Get_Shape(_di_IDispatch &Get_Shape_result) = 0 ;
	virtual HRESULT __safecall Get_ScriptText(WideString &Get_ScriptText_result) = 0 ;
	virtual HRESULT __safecall Set_ScriptText(const WideString Script) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Extended() { WideString r; HRESULT hr = Get_Extended(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Extended = {read=_scw_Get_Extended};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Id() { WideString r; HRESULT hr = Get_Id(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Id = {read=_scw_Get_Id};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Language() { Activex::TOleEnum r; HRESULT hr = Get_Language(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Language = {read=_scw_Get_Language};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Location() { Activex::TOleEnum r; HRESULT hr = Get_Location(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Location = {read=_scw_Get_Location};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Shape() { _di_IDispatch r; HRESULT hr = Get_Shape(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Shape = {read=_scw_Get_Shape};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ScriptText() { WideString r; HRESULT hr = Get_ScriptText(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ScriptText = {read=_scw_Get_ScriptText, write=Set_ScriptText};
};

__dispinterface ScriptDisp;
typedef System::DelphiInterface<ScriptDisp> _di_ScriptDisp;
__dispinterface INTERFACE_UUID("{000C0341-0000-0000-C000-000000000046}") ScriptDisp  : public IDispatch 
{
	
};

__interface Scripts;
typedef System::DelphiInterface<Scripts> _di_Scripts;
__interface INTERFACE_UUID("{000C0340-0000-0000-C000-000000000046}") Scripts  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_Script &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const _di_IDispatch Anchor, Activex::TOleEnum Location, Activex::TOleEnum Language, const WideString Id, const WideString Extended, const WideString ScriptText, _di_Script &Add_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface ScriptsDisp;
typedef System::DelphiInterface<ScriptsDisp> _di_ScriptsDisp;
__dispinterface INTERFACE_UUID("{000C0340-0000-0000-C000-000000000046}") ScriptsDisp  : public IDispatch 
{
	
};

__interface TextEffectFormat;
typedef System::DelphiInterface<TextEffectFormat> _di_TextEffectFormat;
__interface TextFrame;
typedef System::DelphiInterface<TextFrame> _di_TextFrame;
__interface ThreeDFormat;
typedef System::DelphiInterface<ThreeDFormat> _di_ThreeDFormat;
__interface IMsoDiagram;
typedef System::DelphiInterface<IMsoDiagram> _di_IMsoDiagram;
__interface DiagramNode;
typedef System::DelphiInterface<DiagramNode> _di_DiagramNode;
__interface CanvasShapes;
typedef System::DelphiInterface<CanvasShapes> _di_CanvasShapes;
__interface INTERFACE_UUID("{000C031C-0000-0000-C000-000000000046}") Shape  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Apply(void) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Duplicate(_di_Shape &Duplicate_result) = 0 ;
	virtual HRESULT __safecall Flip(Activex::TOleEnum FlipCmd) = 0 ;
	virtual HRESULT __safecall IncrementLeft(float Increment) = 0 ;
	virtual HRESULT __safecall IncrementRotation(float Increment) = 0 ;
	virtual HRESULT __safecall IncrementTop(float Increment) = 0 ;
	virtual HRESULT __safecall PickUp(void) = 0 ;
	virtual HRESULT __safecall RerouteConnections(void) = 0 ;
	virtual HRESULT __safecall ScaleHeight(float Factor, Activex::TOleEnum RelativeToOriginalSize, Activex::TOleEnum fScale) = 0 ;
	virtual HRESULT __safecall ScaleWidth(float Factor, Activex::TOleEnum RelativeToOriginalSize, Activex::TOleEnum fScale) = 0 ;
	virtual HRESULT __safecall Select(const OleVariant Replace) = 0 ;
	virtual HRESULT __safecall SetShapesDefaultProperties(void) = 0 ;
	virtual HRESULT __safecall Ungroup(_di_ShapeRange &Ungroup_result) = 0 ;
	virtual HRESULT __safecall ZOrder(Activex::TOleEnum ZOrderCmd) = 0 ;
	virtual HRESULT __safecall Get_Adjustments(_di_Adjustments &Get_Adjustments_result) = 0 ;
	virtual HRESULT __safecall Get_AutoShapeType(Activex::TOleEnum &Get_AutoShapeType_result) = 0 ;
	virtual HRESULT __safecall Set_AutoShapeType(Activex::TOleEnum AutoShapeType) = 0 ;
	virtual HRESULT __safecall Get_BlackWhiteMode(Activex::TOleEnum &Get_BlackWhiteMode_result) = 0 ;
	virtual HRESULT __safecall Set_BlackWhiteMode(Activex::TOleEnum BlackWhiteMode) = 0 ;
	virtual HRESULT __safecall Get_Callout(_di_CalloutFormat &Get_Callout_result) = 0 ;
	virtual HRESULT __safecall Get_ConnectionSiteCount(int &Get_ConnectionSiteCount_result) = 0 ;
	virtual HRESULT __safecall Get_Connector(Activex::TOleEnum &Get_Connector_result) = 0 ;
	virtual HRESULT __safecall Get_ConnectorFormat(_di_ConnectorFormat &Get_ConnectorFormat_result) = 0 ;
	virtual HRESULT __safecall Get_Fill(_di_FillFormat &Get_Fill_result) = 0 ;
	virtual HRESULT __safecall Get_GroupItems(_di_GroupShapes &Get_GroupItems_result) = 0 ;
	virtual HRESULT __safecall Get_Height(float &Get_Height_result) = 0 ;
	virtual HRESULT __safecall Set_Height(float Height) = 0 ;
	virtual HRESULT __safecall Get_HorizontalFlip(Activex::TOleEnum &Get_HorizontalFlip_result) = 0 ;
	virtual HRESULT __safecall Get_Left(float &Get_Left_result) = 0 ;
	virtual HRESULT __safecall Set_Left(float Left) = 0 ;
	virtual HRESULT __safecall Get_Line(_di_LineFormat &Get_Line_result) = 0 ;
	virtual HRESULT __safecall Get_LockAspectRatio(Activex::TOleEnum &Get_LockAspectRatio_result) = 0 ;
	virtual HRESULT __safecall Set_LockAspectRatio(Activex::TOleEnum LockAspectRatio) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString Name) = 0 ;
	virtual HRESULT __safecall Get_Nodes(_di_ShapeNodes &Get_Nodes_result) = 0 ;
	virtual HRESULT __safecall Get_Rotation(float &Get_Rotation_result) = 0 ;
	virtual HRESULT __safecall Set_Rotation(float Rotation) = 0 ;
	virtual HRESULT __safecall Get_PictureFormat(_di_PictureFormat &Get_PictureFormat_result) = 0 ;
	virtual HRESULT __safecall Get_Shadow(_di_ShadowFormat &Get_Shadow_result) = 0 ;
	virtual HRESULT __safecall Get_TextEffect(_di_TextEffectFormat &Get_TextEffect_result) = 0 ;
	virtual HRESULT __safecall Get_TextFrame(_di_TextFrame &Get_TextFrame_result) = 0 ;
	virtual HRESULT __safecall Get_ThreeD(_di_ThreeDFormat &Get_ThreeD_result) = 0 ;
	virtual HRESULT __safecall Get_Top(float &Get_Top_result) = 0 ;
	virtual HRESULT __safecall Set_Top(float Top) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_VerticalFlip(Activex::TOleEnum &Get_VerticalFlip_result) = 0 ;
	virtual HRESULT __safecall Get_Vertices(OleVariant &Get_Vertices_result) = 0 ;
	virtual HRESULT __safecall Get_Visible(Activex::TOleEnum &Get_Visible_result) = 0 ;
	virtual HRESULT __safecall Set_Visible(Activex::TOleEnum Visible) = 0 ;
	virtual HRESULT __safecall Get_Width(float &Get_Width_result) = 0 ;
	virtual HRESULT __safecall Set_Width(float Width) = 0 ;
	virtual HRESULT __safecall Get_ZOrderPosition(int &Get_ZOrderPosition_result) = 0 ;
	virtual HRESULT __safecall Get_Script(_di_Script &Get_Script_result) = 0 ;
	virtual HRESULT __safecall Get_AlternativeText(WideString &Get_AlternativeText_result) = 0 ;
	virtual HRESULT __safecall Set_AlternativeText(const WideString AlternativeText) = 0 ;
	virtual HRESULT __safecall Get_HasDiagram(Activex::TOleEnum &Get_HasDiagram_result) = 0 ;
	virtual HRESULT __safecall Get_Diagram(_di_IMsoDiagram &Get_Diagram_result) = 0 ;
	virtual HRESULT __safecall Get_HasDiagramNode(Activex::TOleEnum &Get_HasDiagramNode_result) = 0 ;
	virtual HRESULT __safecall Get_DiagramNode(_di_DiagramNode &Get_DiagramNode_result) = 0 ;
	virtual HRESULT __safecall Get_Child(Activex::TOleEnum &Get_Child_result) = 0 ;
	virtual HRESULT __safecall Get_ParentGroup(_di_Shape &Get_ParentGroup_result) = 0 ;
	virtual HRESULT __safecall Get_CanvasItems(_di_CanvasShapes &Get_CanvasItems_result) = 0 ;
	virtual HRESULT __safecall Get_Id(int &Get_Id_result) = 0 ;
	virtual HRESULT __safecall CanvasCropLeft(float Increment) = 0 ;
	virtual HRESULT __safecall CanvasCropTop(float Increment) = 0 ;
	virtual HRESULT __safecall CanvasCropRight(float Increment) = 0 ;
	virtual HRESULT __safecall CanvasCropBottom(float Increment) = 0 ;
	virtual HRESULT __safecall Set_RTF(const WideString Param1) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Adjustments _scw_Get_Adjustments() { _di_Adjustments r; HRESULT hr = Get_Adjustments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Adjustments Adjustments = {read=_scw_Get_Adjustments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_AutoShapeType() { Activex::TOleEnum r; HRESULT hr = Get_AutoShapeType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum AutoShapeType = {read=_scw_Get_AutoShapeType, write=Set_AutoShapeType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_BlackWhiteMode() { Activex::TOleEnum r; HRESULT hr = Get_BlackWhiteMode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum BlackWhiteMode = {read=_scw_Get_BlackWhiteMode, write=Set_BlackWhiteMode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CalloutFormat _scw_Get_Callout() { _di_CalloutFormat r; HRESULT hr = Get_Callout(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CalloutFormat Callout = {read=_scw_Get_Callout};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ConnectionSiteCount() { int r; HRESULT hr = Get_ConnectionSiteCount(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int ConnectionSiteCount = {read=_scw_Get_ConnectionSiteCount};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Connector() { Activex::TOleEnum r; HRESULT hr = Get_Connector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Connector = {read=_scw_Get_Connector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ConnectorFormat _scw_Get_ConnectorFormat() { _di_ConnectorFormat r; HRESULT hr = Get_ConnectorFormat(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ConnectorFormat ConnectorFormat = {read=_scw_Get_ConnectorFormat};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FillFormat _scw_Get_Fill() { _di_FillFormat r; HRESULT hr = Get_Fill(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FillFormat Fill = {read=_scw_Get_Fill};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_GroupShapes _scw_Get_GroupItems() { _di_GroupShapes r; HRESULT hr = Get_GroupItems(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_GroupShapes GroupItems = {read=_scw_Get_GroupItems};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Height() { float r; HRESULT hr = Get_Height(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Height = {read=_scw_Get_Height, write=Set_Height};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_HorizontalFlip() { Activex::TOleEnum r; HRESULT hr = Get_HorizontalFlip(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum HorizontalFlip = {read=_scw_Get_HorizontalFlip};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Left() { float r; HRESULT hr = Get_Left(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Left = {read=_scw_Get_Left, write=Set_Left};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_LineFormat _scw_Get_Line() { _di_LineFormat r; HRESULT hr = Get_Line(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_LineFormat Line = {read=_scw_Get_Line};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_LockAspectRatio() { Activex::TOleEnum r; HRESULT hr = Get_LockAspectRatio(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum LockAspectRatio = {read=_scw_Get_LockAspectRatio, write=Set_LockAspectRatio};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ShapeNodes _scw_Get_Nodes() { _di_ShapeNodes r; HRESULT hr = Get_Nodes(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ShapeNodes Nodes = {read=_scw_Get_Nodes};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Rotation() { float r; HRESULT hr = Get_Rotation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Rotation = {read=_scw_Get_Rotation, write=Set_Rotation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_PictureFormat _scw_Get_PictureFormat() { _di_PictureFormat r; HRESULT hr = Get_PictureFormat(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_PictureFormat PictureFormat = {read=_scw_Get_PictureFormat};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ShadowFormat _scw_Get_Shadow() { _di_ShadowFormat r; HRESULT hr = Get_Shadow(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ShadowFormat Shadow = {read=_scw_Get_Shadow};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_TextEffectFormat _scw_Get_TextEffect() { _di_TextEffectFormat r; HRESULT hr = Get_TextEffect(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_TextEffectFormat TextEffect = {read=_scw_Get_TextEffect};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_TextFrame _scw_Get_TextFrame() { _di_TextFrame r; HRESULT hr = Get_TextFrame(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_TextFrame TextFrame = {read=_scw_Get_TextFrame};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ThreeDFormat _scw_Get_ThreeD() { _di_ThreeDFormat r; HRESULT hr = Get_ThreeD(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ThreeDFormat ThreeD = {read=_scw_Get_ThreeD};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Top() { float r; HRESULT hr = Get_Top(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Top = {read=_scw_Get_Top, write=Set_Top};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_VerticalFlip() { Activex::TOleEnum r; HRESULT hr = Get_VerticalFlip(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum VerticalFlip = {read=_scw_Get_VerticalFlip};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_Vertices() { OleVariant r; HRESULT hr = Get_Vertices(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant Vertices = {read=_scw_Get_Vertices};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Visible() { Activex::TOleEnum r; HRESULT hr = Get_Visible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Visible = {read=_scw_Get_Visible, write=Set_Visible};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Width() { float r; HRESULT hr = Get_Width(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Width = {read=_scw_Get_Width, write=Set_Width};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ZOrderPosition() { int r; HRESULT hr = Get_ZOrderPosition(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int ZOrderPosition = {read=_scw_Get_ZOrderPosition};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Script _scw_Get_Script() { _di_Script r; HRESULT hr = Get_Script(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Script Script = {read=_scw_Get_Script};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_AlternativeText() { WideString r; HRESULT hr = Get_AlternativeText(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString AlternativeText = {read=_scw_Get_AlternativeText, write=Set_AlternativeText};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_HasDiagram() { Activex::TOleEnum r; HRESULT hr = Get_HasDiagram(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum HasDiagram = {read=_scw_Get_HasDiagram};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IMsoDiagram _scw_Get_Diagram() { _di_IMsoDiagram r; HRESULT hr = Get_Diagram(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IMsoDiagram Diagram = {read=_scw_Get_Diagram};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_HasDiagramNode() { Activex::TOleEnum r; HRESULT hr = Get_HasDiagramNode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum HasDiagramNode = {read=_scw_Get_HasDiagramNode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_DiagramNode _scw_Get_DiagramNode() { _di_DiagramNode r; HRESULT hr = Get_DiagramNode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_DiagramNode DiagramNode = {read=_scw_Get_DiagramNode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Child() { Activex::TOleEnum r; HRESULT hr = Get_Child(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Child = {read=_scw_Get_Child};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Shape _scw_Get_ParentGroup() { _di_Shape r; HRESULT hr = Get_ParentGroup(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Shape ParentGroup = {read=_scw_Get_ParentGroup};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CanvasShapes _scw_Get_CanvasItems() { _di_CanvasShapes r; HRESULT hr = Get_CanvasItems(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CanvasShapes CanvasItems = {read=_scw_Get_CanvasItems};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Id() { int r; HRESULT hr = Get_Id(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Id = {read=_scw_Get_Id};
	__property WideString RTF = {write=Set_RTF};
};

__dispinterface ShapeDisp;
typedef System::DelphiInterface<ShapeDisp> _di_ShapeDisp;
__dispinterface INTERFACE_UUID("{000C031C-0000-0000-C000-000000000046}") ShapeDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C031D-0000-0000-C000-000000000046}") ShapeRange  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_Shape &Item_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Align(Activex::TOleEnum AlignCmd, Activex::TOleEnum RelativeTo) = 0 ;
	virtual HRESULT __safecall Apply(void) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Distribute(Activex::TOleEnum DistributeCmd, Activex::TOleEnum RelativeTo) = 0 ;
	virtual HRESULT __safecall Duplicate(_di_ShapeRange &Duplicate_result) = 0 ;
	virtual HRESULT __safecall Flip(Activex::TOleEnum FlipCmd) = 0 ;
	virtual HRESULT __safecall IncrementLeft(float Increment) = 0 ;
	virtual HRESULT __safecall IncrementRotation(float Increment) = 0 ;
	virtual HRESULT __safecall IncrementTop(float Increment) = 0 ;
	virtual HRESULT __safecall Group(_di_Shape &Group_result) = 0 ;
	virtual HRESULT __safecall PickUp(void) = 0 ;
	virtual HRESULT __safecall Regroup(_di_Shape &Regroup_result) = 0 ;
	virtual HRESULT __safecall RerouteConnections(void) = 0 ;
	virtual HRESULT __safecall ScaleHeight(float Factor, Activex::TOleEnum RelativeToOriginalSize, Activex::TOleEnum fScale) = 0 ;
	virtual HRESULT __safecall ScaleWidth(float Factor, Activex::TOleEnum RelativeToOriginalSize, Activex::TOleEnum fScale) = 0 ;
	virtual HRESULT __safecall Select(const OleVariant Replace) = 0 ;
	virtual HRESULT __safecall SetShapesDefaultProperties(void) = 0 ;
	virtual HRESULT __safecall Ungroup(_di_ShapeRange &Ungroup_result) = 0 ;
	virtual HRESULT __safecall ZOrder(Activex::TOleEnum ZOrderCmd) = 0 ;
	virtual HRESULT __safecall Get_Adjustments(_di_Adjustments &Get_Adjustments_result) = 0 ;
	virtual HRESULT __safecall Get_AutoShapeType(Activex::TOleEnum &Get_AutoShapeType_result) = 0 ;
	virtual HRESULT __safecall Set_AutoShapeType(Activex::TOleEnum AutoShapeType) = 0 ;
	virtual HRESULT __safecall Get_BlackWhiteMode(Activex::TOleEnum &Get_BlackWhiteMode_result) = 0 ;
	virtual HRESULT __safecall Set_BlackWhiteMode(Activex::TOleEnum BlackWhiteMode) = 0 ;
	virtual HRESULT __safecall Get_Callout(_di_CalloutFormat &Get_Callout_result) = 0 ;
	virtual HRESULT __safecall Get_ConnectionSiteCount(int &Get_ConnectionSiteCount_result) = 0 ;
	virtual HRESULT __safecall Get_Connector(Activex::TOleEnum &Get_Connector_result) = 0 ;
	virtual HRESULT __safecall Get_ConnectorFormat(_di_ConnectorFormat &Get_ConnectorFormat_result) = 0 ;
	virtual HRESULT __safecall Get_Fill(_di_FillFormat &Get_Fill_result) = 0 ;
	virtual HRESULT __safecall Get_GroupItems(_di_GroupShapes &Get_GroupItems_result) = 0 ;
	virtual HRESULT __safecall Get_Height(float &Get_Height_result) = 0 ;
	virtual HRESULT __safecall Set_Height(float Height) = 0 ;
	virtual HRESULT __safecall Get_HorizontalFlip(Activex::TOleEnum &Get_HorizontalFlip_result) = 0 ;
	virtual HRESULT __safecall Get_Left(float &Get_Left_result) = 0 ;
	virtual HRESULT __safecall Set_Left(float Left) = 0 ;
	virtual HRESULT __safecall Get_Line(_di_LineFormat &Get_Line_result) = 0 ;
	virtual HRESULT __safecall Get_LockAspectRatio(Activex::TOleEnum &Get_LockAspectRatio_result) = 0 ;
	virtual HRESULT __safecall Set_LockAspectRatio(Activex::TOleEnum LockAspectRatio) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString Name) = 0 ;
	virtual HRESULT __safecall Get_Nodes(_di_ShapeNodes &Get_Nodes_result) = 0 ;
	virtual HRESULT __safecall Get_Rotation(float &Get_Rotation_result) = 0 ;
	virtual HRESULT __safecall Set_Rotation(float Rotation) = 0 ;
	virtual HRESULT __safecall Get_PictureFormat(_di_PictureFormat &Get_PictureFormat_result) = 0 ;
	virtual HRESULT __safecall Get_Shadow(_di_ShadowFormat &Get_Shadow_result) = 0 ;
	virtual HRESULT __safecall Get_TextEffect(_di_TextEffectFormat &Get_TextEffect_result) = 0 ;
	virtual HRESULT __safecall Get_TextFrame(_di_TextFrame &Get_TextFrame_result) = 0 ;
	virtual HRESULT __safecall Get_ThreeD(_di_ThreeDFormat &Get_ThreeD_result) = 0 ;
	virtual HRESULT __safecall Get_Top(float &Get_Top_result) = 0 ;
	virtual HRESULT __safecall Set_Top(float Top) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_VerticalFlip(Activex::TOleEnum &Get_VerticalFlip_result) = 0 ;
	virtual HRESULT __safecall Get_Vertices(OleVariant &Get_Vertices_result) = 0 ;
	virtual HRESULT __safecall Get_Visible(Activex::TOleEnum &Get_Visible_result) = 0 ;
	virtual HRESULT __safecall Set_Visible(Activex::TOleEnum Visible) = 0 ;
	virtual HRESULT __safecall Get_Width(float &Get_Width_result) = 0 ;
	virtual HRESULT __safecall Set_Width(float Width) = 0 ;
	virtual HRESULT __safecall Get_ZOrderPosition(int &Get_ZOrderPosition_result) = 0 ;
	virtual HRESULT __safecall Get_Script(_di_Script &Get_Script_result) = 0 ;
	virtual HRESULT __safecall Get_AlternativeText(WideString &Get_AlternativeText_result) = 0 ;
	virtual HRESULT __safecall Set_AlternativeText(const WideString AlternativeText) = 0 ;
	virtual HRESULT __safecall Get_HasDiagram(Activex::TOleEnum &Get_HasDiagram_result) = 0 ;
	virtual HRESULT __safecall Get_Diagram(_di_IMsoDiagram &Get_Diagram_result) = 0 ;
	virtual HRESULT __safecall Get_HasDiagramNode(Activex::TOleEnum &Get_HasDiagramNode_result) = 0 ;
	virtual HRESULT __safecall Get_DiagramNode(_di_DiagramNode &Get_DiagramNode_result) = 0 ;
	virtual HRESULT __safecall Get_Child(Activex::TOleEnum &Get_Child_result) = 0 ;
	virtual HRESULT __safecall Get_ParentGroup(_di_Shape &Get_ParentGroup_result) = 0 ;
	virtual HRESULT __safecall Get_CanvasItems(_di_CanvasShapes &Get_CanvasItems_result) = 0 ;
	virtual HRESULT __safecall Get_Id(int &Get_Id_result) = 0 ;
	virtual HRESULT __safecall CanvasCropLeft(float Increment) = 0 ;
	virtual HRESULT __safecall CanvasCropTop(float Increment) = 0 ;
	virtual HRESULT __safecall CanvasCropRight(float Increment) = 0 ;
	virtual HRESULT __safecall CanvasCropBottom(float Increment) = 0 ;
	virtual HRESULT __safecall Set_RTF(const WideString Param1) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Adjustments _scw_Get_Adjustments() { _di_Adjustments r; HRESULT hr = Get_Adjustments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Adjustments Adjustments = {read=_scw_Get_Adjustments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_AutoShapeType() { Activex::TOleEnum r; HRESULT hr = Get_AutoShapeType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum AutoShapeType = {read=_scw_Get_AutoShapeType, write=Set_AutoShapeType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_BlackWhiteMode() { Activex::TOleEnum r; HRESULT hr = Get_BlackWhiteMode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum BlackWhiteMode = {read=_scw_Get_BlackWhiteMode, write=Set_BlackWhiteMode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CalloutFormat _scw_Get_Callout() { _di_CalloutFormat r; HRESULT hr = Get_Callout(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CalloutFormat Callout = {read=_scw_Get_Callout};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ConnectionSiteCount() { int r; HRESULT hr = Get_ConnectionSiteCount(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int ConnectionSiteCount = {read=_scw_Get_ConnectionSiteCount};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Connector() { Activex::TOleEnum r; HRESULT hr = Get_Connector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Connector = {read=_scw_Get_Connector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ConnectorFormat _scw_Get_ConnectorFormat() { _di_ConnectorFormat r; HRESULT hr = Get_ConnectorFormat(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ConnectorFormat ConnectorFormat = {read=_scw_Get_ConnectorFormat};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FillFormat _scw_Get_Fill() { _di_FillFormat r; HRESULT hr = Get_Fill(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FillFormat Fill = {read=_scw_Get_Fill};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_GroupShapes _scw_Get_GroupItems() { _di_GroupShapes r; HRESULT hr = Get_GroupItems(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_GroupShapes GroupItems = {read=_scw_Get_GroupItems};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Height() { float r; HRESULT hr = Get_Height(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Height = {read=_scw_Get_Height, write=Set_Height};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_HorizontalFlip() { Activex::TOleEnum r; HRESULT hr = Get_HorizontalFlip(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum HorizontalFlip = {read=_scw_Get_HorizontalFlip};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Left() { float r; HRESULT hr = Get_Left(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Left = {read=_scw_Get_Left, write=Set_Left};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_LineFormat _scw_Get_Line() { _di_LineFormat r; HRESULT hr = Get_Line(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_LineFormat Line = {read=_scw_Get_Line};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_LockAspectRatio() { Activex::TOleEnum r; HRESULT hr = Get_LockAspectRatio(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum LockAspectRatio = {read=_scw_Get_LockAspectRatio, write=Set_LockAspectRatio};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ShapeNodes _scw_Get_Nodes() { _di_ShapeNodes r; HRESULT hr = Get_Nodes(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ShapeNodes Nodes = {read=_scw_Get_Nodes};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Rotation() { float r; HRESULT hr = Get_Rotation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Rotation = {read=_scw_Get_Rotation, write=Set_Rotation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_PictureFormat _scw_Get_PictureFormat() { _di_PictureFormat r; HRESULT hr = Get_PictureFormat(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_PictureFormat PictureFormat = {read=_scw_Get_PictureFormat};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ShadowFormat _scw_Get_Shadow() { _di_ShadowFormat r; HRESULT hr = Get_Shadow(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ShadowFormat Shadow = {read=_scw_Get_Shadow};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_TextEffectFormat _scw_Get_TextEffect() { _di_TextEffectFormat r; HRESULT hr = Get_TextEffect(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_TextEffectFormat TextEffect = {read=_scw_Get_TextEffect};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_TextFrame _scw_Get_TextFrame() { _di_TextFrame r; HRESULT hr = Get_TextFrame(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_TextFrame TextFrame = {read=_scw_Get_TextFrame};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ThreeDFormat _scw_Get_ThreeD() { _di_ThreeDFormat r; HRESULT hr = Get_ThreeD(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ThreeDFormat ThreeD = {read=_scw_Get_ThreeD};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Top() { float r; HRESULT hr = Get_Top(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Top = {read=_scw_Get_Top, write=Set_Top};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_VerticalFlip() { Activex::TOleEnum r; HRESULT hr = Get_VerticalFlip(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum VerticalFlip = {read=_scw_Get_VerticalFlip};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_Vertices() { OleVariant r; HRESULT hr = Get_Vertices(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant Vertices = {read=_scw_Get_Vertices};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Visible() { Activex::TOleEnum r; HRESULT hr = Get_Visible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Visible = {read=_scw_Get_Visible, write=Set_Visible};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Width() { float r; HRESULT hr = Get_Width(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Width = {read=_scw_Get_Width, write=Set_Width};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ZOrderPosition() { int r; HRESULT hr = Get_ZOrderPosition(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int ZOrderPosition = {read=_scw_Get_ZOrderPosition};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Script _scw_Get_Script() { _di_Script r; HRESULT hr = Get_Script(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Script Script = {read=_scw_Get_Script};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_AlternativeText() { WideString r; HRESULT hr = Get_AlternativeText(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString AlternativeText = {read=_scw_Get_AlternativeText, write=Set_AlternativeText};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_HasDiagram() { Activex::TOleEnum r; HRESULT hr = Get_HasDiagram(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum HasDiagram = {read=_scw_Get_HasDiagram};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IMsoDiagram _scw_Get_Diagram() { _di_IMsoDiagram r; HRESULT hr = Get_Diagram(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IMsoDiagram Diagram = {read=_scw_Get_Diagram};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_HasDiagramNode() { Activex::TOleEnum r; HRESULT hr = Get_HasDiagramNode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum HasDiagramNode = {read=_scw_Get_HasDiagramNode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_DiagramNode _scw_Get_DiagramNode() { _di_DiagramNode r; HRESULT hr = Get_DiagramNode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_DiagramNode DiagramNode = {read=_scw_Get_DiagramNode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Child() { Activex::TOleEnum r; HRESULT hr = Get_Child(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Child = {read=_scw_Get_Child};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Shape _scw_Get_ParentGroup() { _di_Shape r; HRESULT hr = Get_ParentGroup(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Shape ParentGroup = {read=_scw_Get_ParentGroup};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_CanvasShapes _scw_Get_CanvasItems() { _di_CanvasShapes r; HRESULT hr = Get_CanvasItems(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_CanvasShapes CanvasItems = {read=_scw_Get_CanvasItems};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Id() { int r; HRESULT hr = Get_Id(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Id = {read=_scw_Get_Id};
	__property WideString RTF = {write=Set_RTF};
};

__dispinterface ShapeRangeDisp;
typedef System::DelphiInterface<ShapeRangeDisp> _di_ShapeRangeDisp;
__dispinterface INTERFACE_UUID("{000C031D-0000-0000-C000-000000000046}") ShapeRangeDisp  : public IDispatch 
{
	
};

__interface Shapes;
typedef System::DelphiInterface<Shapes> _di_Shapes;
__interface INTERFACE_UUID("{000C031E-0000-0000-C000-000000000046}") Shapes  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_Shape &Item_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall AddCallout(Activex::TOleEnum Type_, float Left, float Top, float Width, float Height, _di_Shape &AddCallout_result) = 0 ;
	virtual HRESULT __safecall AddConnector(Activex::TOleEnum Type_, float BeginX, float BeginY, float EndX, float EndY, _di_Shape &AddConnector_result) = 0 ;
	virtual HRESULT __safecall AddCurve(const OleVariant SafeArrayOfPoints, _di_Shape &AddCurve_result) = 0 ;
	virtual HRESULT __safecall AddLabel(Activex::TOleEnum Orientation, float Left, float Top, float Width, float Height, _di_Shape &AddLabel_result) = 0 ;
	virtual HRESULT __safecall AddLine(float BeginX, float BeginY, float EndX, float EndY, _di_Shape &AddLine_result) = 0 ;
	virtual HRESULT __safecall AddPicture(const WideString FileName, Activex::TOleEnum LinkToFile, Activex::TOleEnum SaveWithDocument, float Left, float Top, float Width, float Height, _di_Shape &AddPicture_result) = 0 ;
	virtual HRESULT __safecall AddPolyline(const OleVariant SafeArrayOfPoints, _di_Shape &AddPolyline_result) = 0 ;
	virtual HRESULT __safecall AddShape(Activex::TOleEnum Type_, float Left, float Top, float Width, float Height, _di_Shape &AddShape_result) = 0 ;
	virtual HRESULT __safecall AddTextEffect(Activex::TOleEnum PresetTextEffect, const WideString Text, const WideString FontName, float FontSize, Activex::TOleEnum FontBold, Activex::TOleEnum FontItalic, float Left, float Top, _di_Shape &AddTextEffect_result) = 0 ;
	virtual HRESULT __safecall AddTextbox(Activex::TOleEnum Orientation, float Left, float Top, float Width, float Height, _di_Shape &AddTextbox_result) = 0 ;
	virtual HRESULT __safecall BuildFreeform(Activex::TOleEnum EditingType, float X1, float Y1, _di_FreeformBuilder &BuildFreeform_result) = 0 ;
	virtual HRESULT __safecall Range(const OleVariant Index, _di_ShapeRange &Range_result) = 0 ;
	virtual HRESULT __safecall SelectAll(void) = 0 ;
	virtual HRESULT __safecall Get_Background(_di_Shape &Get_Background_result) = 0 ;
	virtual HRESULT __safecall Get_Default(_di_Shape &Get_Default_result) = 0 ;
	virtual HRESULT __safecall AddDiagram(Activex::TOleEnum Type_, float Left, float Top, float Width, float Height, _di_Shape &AddDiagram_result) = 0 ;
	virtual HRESULT __safecall AddCanvas(float Left, float Top, float Width, float Height, _di_Shape &AddCanvas_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Shape _scw_Get_Background() { _di_Shape r; HRESULT hr = Get_Background(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Shape Background = {read=_scw_Get_Background};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Shape _scw_Get_Default() { _di_Shape r; HRESULT hr = Get_Default(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Shape Default = {read=_scw_Get_Default};
};

__dispinterface ShapesDisp;
typedef System::DelphiInterface<ShapesDisp> _di_ShapesDisp;
__dispinterface INTERFACE_UUID("{000C031E-0000-0000-C000-000000000046}") ShapesDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C031F-0000-0000-C000-000000000046}") TextEffectFormat  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall ToggleVerticalText(void) = 0 ;
	virtual HRESULT __safecall Get_Alignment(Activex::TOleEnum &Get_Alignment_result) = 0 ;
	virtual HRESULT __safecall Set_Alignment(Activex::TOleEnum Alignment) = 0 ;
	virtual HRESULT __safecall Get_FontBold(Activex::TOleEnum &Get_FontBold_result) = 0 ;
	virtual HRESULT __safecall Set_FontBold(Activex::TOleEnum FontBold) = 0 ;
	virtual HRESULT __safecall Get_FontItalic(Activex::TOleEnum &Get_FontItalic_result) = 0 ;
	virtual HRESULT __safecall Set_FontItalic(Activex::TOleEnum FontItalic) = 0 ;
	virtual HRESULT __safecall Get_FontName(WideString &Get_FontName_result) = 0 ;
	virtual HRESULT __safecall Set_FontName(const WideString FontName) = 0 ;
	virtual HRESULT __safecall Get_FontSize(float &Get_FontSize_result) = 0 ;
	virtual HRESULT __safecall Set_FontSize(float FontSize) = 0 ;
	virtual HRESULT __safecall Get_KernedPairs(Activex::TOleEnum &Get_KernedPairs_result) = 0 ;
	virtual HRESULT __safecall Set_KernedPairs(Activex::TOleEnum KernedPairs) = 0 ;
	virtual HRESULT __safecall Get_NormalizedHeight(Activex::TOleEnum &Get_NormalizedHeight_result) = 0 ;
	virtual HRESULT __safecall Set_NormalizedHeight(Activex::TOleEnum NormalizedHeight) = 0 ;
	virtual HRESULT __safecall Get_PresetShape(Activex::TOleEnum &Get_PresetShape_result) = 0 ;
	virtual HRESULT __safecall Set_PresetShape(Activex::TOleEnum PresetShape) = 0 ;
	virtual HRESULT __safecall Get_PresetTextEffect(Activex::TOleEnum &Get_PresetTextEffect_result) = 0 ;
	virtual HRESULT __safecall Set_PresetTextEffect(Activex::TOleEnum Preset) = 0 ;
	virtual HRESULT __safecall Get_RotatedChars(Activex::TOleEnum &Get_RotatedChars_result) = 0 ;
	virtual HRESULT __safecall Set_RotatedChars(Activex::TOleEnum RotatedChars) = 0 ;
	virtual HRESULT __safecall Get_Text(WideString &Get_Text_result) = 0 ;
	virtual HRESULT __safecall Set_Text(const WideString Text) = 0 ;
	virtual HRESULT __safecall Get_Tracking(float &Get_Tracking_result) = 0 ;
	virtual HRESULT __safecall Set_Tracking(float Tracking) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Alignment() { Activex::TOleEnum r; HRESULT hr = Get_Alignment(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Alignment = {read=_scw_Get_Alignment, write=Set_Alignment};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_FontBold() { Activex::TOleEnum r; HRESULT hr = Get_FontBold(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum FontBold = {read=_scw_Get_FontBold, write=Set_FontBold};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_FontItalic() { Activex::TOleEnum r; HRESULT hr = Get_FontItalic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum FontItalic = {read=_scw_Get_FontItalic, write=Set_FontItalic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FontName() { WideString r; HRESULT hr = Get_FontName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FontName = {read=_scw_Get_FontName, write=Set_FontName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_FontSize() { float r; HRESULT hr = Get_FontSize(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float FontSize = {read=_scw_Get_FontSize, write=Set_FontSize};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_KernedPairs() { Activex::TOleEnum r; HRESULT hr = Get_KernedPairs(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum KernedPairs = {read=_scw_Get_KernedPairs, write=Set_KernedPairs};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_NormalizedHeight() { Activex::TOleEnum r; HRESULT hr = Get_NormalizedHeight(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum NormalizedHeight = {read=_scw_Get_NormalizedHeight, write=Set_NormalizedHeight};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_PresetShape() { Activex::TOleEnum r; HRESULT hr = Get_PresetShape(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum PresetShape = {read=_scw_Get_PresetShape, write=Set_PresetShape};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_PresetTextEffect() { Activex::TOleEnum r; HRESULT hr = Get_PresetTextEffect(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum PresetTextEffect = {read=_scw_Get_PresetTextEffect, write=Set_PresetTextEffect};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_RotatedChars() { Activex::TOleEnum r; HRESULT hr = Get_RotatedChars(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum RotatedChars = {read=_scw_Get_RotatedChars, write=Set_RotatedChars};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Text() { WideString r; HRESULT hr = Get_Text(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Text = {read=_scw_Get_Text, write=Set_Text};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Tracking() { float r; HRESULT hr = Get_Tracking(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Tracking = {read=_scw_Get_Tracking, write=Set_Tracking};
};

__dispinterface TextEffectFormatDisp;
typedef System::DelphiInterface<TextEffectFormatDisp> _di_TextEffectFormatDisp;
__dispinterface INTERFACE_UUID("{000C031F-0000-0000-C000-000000000046}") TextEffectFormatDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C0320-0000-0000-C000-000000000046}") TextFrame  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_MarginBottom(float &Get_MarginBottom_result) = 0 ;
	virtual HRESULT __safecall Set_MarginBottom(float MarginBottom) = 0 ;
	virtual HRESULT __safecall Get_MarginLeft(float &Get_MarginLeft_result) = 0 ;
	virtual HRESULT __safecall Set_MarginLeft(float MarginLeft) = 0 ;
	virtual HRESULT __safecall Get_MarginRight(float &Get_MarginRight_result) = 0 ;
	virtual HRESULT __safecall Set_MarginRight(float MarginRight) = 0 ;
	virtual HRESULT __safecall Get_MarginTop(float &Get_MarginTop_result) = 0 ;
	virtual HRESULT __safecall Set_MarginTop(float MarginTop) = 0 ;
	virtual HRESULT __safecall Get_Orientation(Activex::TOleEnum &Get_Orientation_result) = 0 ;
	virtual HRESULT __safecall Set_Orientation(Activex::TOleEnum Orientation) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_MarginBottom() { float r; HRESULT hr = Get_MarginBottom(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float MarginBottom = {read=_scw_Get_MarginBottom, write=Set_MarginBottom};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_MarginLeft() { float r; HRESULT hr = Get_MarginLeft(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float MarginLeft = {read=_scw_Get_MarginLeft, write=Set_MarginLeft};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_MarginRight() { float r; HRESULT hr = Get_MarginRight(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float MarginRight = {read=_scw_Get_MarginRight, write=Set_MarginRight};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_MarginTop() { float r; HRESULT hr = Get_MarginTop(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float MarginTop = {read=_scw_Get_MarginTop, write=Set_MarginTop};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Orientation() { Activex::TOleEnum r; HRESULT hr = Get_Orientation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Orientation = {read=_scw_Get_Orientation, write=Set_Orientation};
};

__dispinterface TextFrameDisp;
typedef System::DelphiInterface<TextFrameDisp> _di_TextFrameDisp;
__dispinterface INTERFACE_UUID("{000C0320-0000-0000-C000-000000000046}") TextFrameDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C0321-0000-0000-C000-000000000046}") ThreeDFormat  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall IncrementRotationX(float Increment) = 0 ;
	virtual HRESULT __safecall IncrementRotationY(float Increment) = 0 ;
	virtual HRESULT __safecall ResetRotation(void) = 0 ;
	virtual HRESULT __safecall SetThreeDFormat(Activex::TOleEnum PresetThreeDFormat) = 0 ;
	virtual HRESULT __safecall SetExtrusionDirection(Activex::TOleEnum PresetExtrusionDirection) = 0 ;
	virtual HRESULT __safecall Get_Depth(float &Get_Depth_result) = 0 ;
	virtual HRESULT __safecall Set_Depth(float Depth) = 0 ;
	virtual HRESULT __safecall Get_ExtrusionColor(_di_ColorFormat &Get_ExtrusionColor_result) = 0 ;
	virtual HRESULT __safecall Get_ExtrusionColorType(Activex::TOleEnum &Get_ExtrusionColorType_result) = 0 ;
	virtual HRESULT __safecall Set_ExtrusionColorType(Activex::TOleEnum ExtrusionColorType) = 0 ;
	virtual HRESULT __safecall Get_Perspective(Activex::TOleEnum &Get_Perspective_result) = 0 ;
	virtual HRESULT __safecall Set_Perspective(Activex::TOleEnum Perspective) = 0 ;
	virtual HRESULT __safecall Get_PresetExtrusionDirection(Activex::TOleEnum &Get_PresetExtrusionDirection_result) = 0 ;
	virtual HRESULT __safecall Get_PresetLightingDirection(Activex::TOleEnum &Get_PresetLightingDirection_result) = 0 ;
	virtual HRESULT __safecall Set_PresetLightingDirection(Activex::TOleEnum PresetLightingDirection) = 0 ;
	virtual HRESULT __safecall Get_PresetLightingSoftness(Activex::TOleEnum &Get_PresetLightingSoftness_result) = 0 ;
	virtual HRESULT __safecall Set_PresetLightingSoftness(Activex::TOleEnum PresetLightingSoftness) = 0 ;
	virtual HRESULT __safecall Get_PresetMaterial(Activex::TOleEnum &Get_PresetMaterial_result) = 0 ;
	virtual HRESULT __safecall Set_PresetMaterial(Activex::TOleEnum PresetMaterial) = 0 ;
	virtual HRESULT __safecall Get_PresetThreeDFormat(Activex::TOleEnum &Get_PresetThreeDFormat_result) = 0 ;
	virtual HRESULT __safecall Get_RotationX(float &Get_RotationX_result) = 0 ;
	virtual HRESULT __safecall Set_RotationX(float RotationX) = 0 ;
	virtual HRESULT __safecall Get_RotationY(float &Get_RotationY_result) = 0 ;
	virtual HRESULT __safecall Set_RotationY(float RotationY) = 0 ;
	virtual HRESULT __safecall Get_Visible(Activex::TOleEnum &Get_Visible_result) = 0 ;
	virtual HRESULT __safecall Set_Visible(Activex::TOleEnum Visible) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_Depth() { float r; HRESULT hr = Get_Depth(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float Depth = {read=_scw_Get_Depth, write=Set_Depth};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ColorFormat _scw_Get_ExtrusionColor() { _di_ColorFormat r; HRESULT hr = Get_ExtrusionColor(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ColorFormat ExtrusionColor = {read=_scw_Get_ExtrusionColor};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_ExtrusionColorType() { Activex::TOleEnum r; HRESULT hr = Get_ExtrusionColorType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum ExtrusionColorType = {read=_scw_Get_ExtrusionColorType, write=Set_ExtrusionColorType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Perspective() { Activex::TOleEnum r; HRESULT hr = Get_Perspective(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Perspective = {read=_scw_Get_Perspective, write=Set_Perspective};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_PresetExtrusionDirection() { Activex::TOleEnum r; HRESULT hr = Get_PresetExtrusionDirection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum PresetExtrusionDirection = {read=_scw_Get_PresetExtrusionDirection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_PresetLightingDirection() { Activex::TOleEnum r; HRESULT hr = Get_PresetLightingDirection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum PresetLightingDirection = {read=_scw_Get_PresetLightingDirection, write=Set_PresetLightingDirection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_PresetLightingSoftness() { Activex::TOleEnum r; HRESULT hr = Get_PresetLightingSoftness(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum PresetLightingSoftness = {read=_scw_Get_PresetLightingSoftness, write=Set_PresetLightingSoftness};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_PresetMaterial() { Activex::TOleEnum r; HRESULT hr = Get_PresetMaterial(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum PresetMaterial = {read=_scw_Get_PresetMaterial, write=Set_PresetMaterial};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_PresetThreeDFormat() { Activex::TOleEnum r; HRESULT hr = Get_PresetThreeDFormat(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum PresetThreeDFormat = {read=_scw_Get_PresetThreeDFormat};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_RotationX() { float r; HRESULT hr = Get_RotationX(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float RotationX = {read=_scw_Get_RotationX, write=Set_RotationX};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_RotationY() { float r; HRESULT hr = Get_RotationY(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float RotationY = {read=_scw_Get_RotationY, write=Set_RotationY};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Visible() { Activex::TOleEnum r; HRESULT hr = Get_Visible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Visible = {read=_scw_Get_Visible, write=Set_Visible};
};

__dispinterface ThreeDFormatDisp;
typedef System::DelphiInterface<ThreeDFormatDisp> _di_ThreeDFormatDisp;
__dispinterface INTERFACE_UUID("{000C0321-0000-0000-C000-000000000046}") ThreeDFormatDisp  : public IDispatch 
{
	
};

__interface IMsoDispCagNotifySink;
typedef System::DelphiInterface<IMsoDispCagNotifySink> _di_IMsoDispCagNotifySink;
__interface INTERFACE_UUID("{000C0359-0000-0000-C000-000000000046}") IMsoDispCagNotifySink  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall InsertClip(const System::_di_IInterface pClipMoniker, const System::_di_IInterface pItemMoniker) = 0 ;
	virtual HRESULT __safecall WindowIsClosing(void) = 0 ;
};

__dispinterface IMsoDispCagNotifySinkDisp;
typedef System::DelphiInterface<IMsoDispCagNotifySinkDisp> _di_IMsoDispCagNotifySinkDisp;
__dispinterface INTERFACE_UUID("{000C0359-0000-0000-C000-000000000046}") IMsoDispCagNotifySinkDisp  : public IDispatch 
{
	
};

__interface Balloon;
typedef System::DelphiInterface<Balloon> _di_Balloon;
__interface INTERFACE_UUID("{000C0324-0000-0000-C000-000000000046}") Balloon  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Checkboxes(_di_IDispatch &Get_Checkboxes_result) = 0 ;
	virtual HRESULT __safecall Get_Labels(_di_IDispatch &Get_Labels_result) = 0 ;
	virtual HRESULT __safecall Set_BalloonType(Activex::TOleEnum pbty) = 0 ;
	virtual HRESULT __safecall Get_BalloonType(Activex::TOleEnum &Get_BalloonType_result) = 0 ;
	virtual HRESULT __safecall Set_Icon(Activex::TOleEnum picn) = 0 ;
	virtual HRESULT __safecall Get_Icon(Activex::TOleEnum &Get_Icon_result) = 0 ;
	virtual HRESULT __safecall Set_Heading(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Get_Heading(WideString &Get_Heading_result) = 0 ;
	virtual HRESULT __safecall Set_Text(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Get_Text(WideString &Get_Text_result) = 0 ;
	virtual HRESULT __safecall Set_Mode(Activex::TOleEnum pmd) = 0 ;
	virtual HRESULT __safecall Get_Mode(Activex::TOleEnum &Get_Mode_result) = 0 ;
	virtual HRESULT __safecall Set_Animation(Activex::TOleEnum pfca) = 0 ;
	virtual HRESULT __safecall Get_Animation(Activex::TOleEnum &Get_Animation_result) = 0 ;
	virtual HRESULT __safecall Set_Button(Activex::TOleEnum psbs) = 0 ;
	virtual HRESULT __safecall Get_Button(Activex::TOleEnum &Get_Button_result) = 0 ;
	virtual HRESULT __safecall Set_Callback(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Get_Callback(WideString &Get_Callback_result) = 0 ;
	virtual HRESULT __safecall Set_Private_(int plPrivate) = 0 ;
	virtual HRESULT __safecall Get_Private_(int &Get_Private__result) = 0 ;
	virtual HRESULT __safecall SetAvoidRectangle(int Left, int Top, int Right, int Bottom) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Show(Activex::TOleEnum &Show_result) = 0 ;
	virtual HRESULT __safecall Close(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Checkboxes() { _di_IDispatch r; HRESULT hr = Get_Checkboxes(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Checkboxes = {read=_scw_Get_Checkboxes};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Labels() { _di_IDispatch r; HRESULT hr = Get_Labels(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Labels = {read=_scw_Get_Labels};
	__property Activex::TOleEnum BalloonType = {write=Set_BalloonType};
	__property Activex::TOleEnum Icon = {write=Set_Icon};
	__property WideString Heading = {write=Set_Heading};
	__property WideString Text = {write=Set_Text};
	__property Activex::TOleEnum Mode = {write=Set_Mode};
	__property Activex::TOleEnum Animation = {write=Set_Animation};
	__property Activex::TOleEnum Button = {write=Set_Button};
	__property WideString Callback = {write=Set_Callback};
	__property int Private_ = {write=Set_Private_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
};

__dispinterface BalloonDisp;
typedef System::DelphiInterface<BalloonDisp> _di_BalloonDisp;
__dispinterface INTERFACE_UUID("{000C0324-0000-0000-C000-000000000046}") BalloonDisp  : public IDispatch 
{
	
};

__interface BalloonCheckboxes;
typedef System::DelphiInterface<BalloonCheckboxes> _di_BalloonCheckboxes;
__interface INTERFACE_UUID("{000C0326-0000-0000-C000-000000000046}") BalloonCheckboxes  : public _IMsoDispObj 
{
	
public:
	_di_IDispatch operator[](int Index) { return Item[Index]; }
	
public:
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Item(int Index, _di_IDispatch &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Set_Count(int pccbx) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Item(int Index) { _di_IDispatch r; HRESULT hr = Get_Item(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Item[int Index] = {read=_scw_Get_Item/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface BalloonCheckboxesDisp;
typedef System::DelphiInterface<BalloonCheckboxesDisp> _di_BalloonCheckboxesDisp;
__dispinterface INTERFACE_UUID("{000C0326-0000-0000-C000-000000000046}") BalloonCheckboxesDisp  : public IDispatch 
{
	
};

__interface BalloonCheckbox;
typedef System::DelphiInterface<BalloonCheckbox> _di_BalloonCheckbox;
__interface INTERFACE_UUID("{000C0328-0000-0000-C000-000000000046}") BalloonCheckbox  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Item(WideString &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Set_Checked(Word pvarfChecked) = 0 ;
	virtual HRESULT __safecall Get_Checked(Word &Get_Checked_result) = 0 ;
	virtual HRESULT __safecall Set_Text(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Get_Text(WideString &Get_Text_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Item() { WideString r; HRESULT hr = Get_Item(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Item = {read=_scw_Get_Item};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	__property Word Checked = {write=Set_Checked};
	__property WideString Text = {write=Set_Text};
};

__dispinterface BalloonCheckboxDisp;
typedef System::DelphiInterface<BalloonCheckboxDisp> _di_BalloonCheckboxDisp;
__dispinterface INTERFACE_UUID("{000C0328-0000-0000-C000-000000000046}") BalloonCheckboxDisp  : public IDispatch 
{
	
};

__interface BalloonLabels;
typedef System::DelphiInterface<BalloonLabels> _di_BalloonLabels;
__interface INTERFACE_UUID("{000C032E-0000-0000-C000-000000000046}") BalloonLabels  : public _IMsoDispObj 
{
	
public:
	_di_IDispatch operator[](int Index) { return Item[Index]; }
	
public:
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Item(int Index, _di_IDispatch &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Set_Count(int pcwz) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Item(int Index) { _di_IDispatch r; HRESULT hr = Get_Item(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Item[int Index] = {read=_scw_Get_Item/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface BalloonLabelsDisp;
typedef System::DelphiInterface<BalloonLabelsDisp> _di_BalloonLabelsDisp;
__dispinterface INTERFACE_UUID("{000C032E-0000-0000-C000-000000000046}") BalloonLabelsDisp  : public IDispatch 
{
	
};

__interface BalloonLabel;
typedef System::DelphiInterface<BalloonLabel> _di_BalloonLabel;
__interface INTERFACE_UUID("{000C0330-0000-0000-C000-000000000046}") BalloonLabel  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Item(WideString &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Set_Text(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Get_Text(WideString &Get_Text_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Item() { WideString r; HRESULT hr = Get_Item(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Item = {read=_scw_Get_Item};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	__property WideString Text = {write=Set_Text};
};

__dispinterface BalloonLabelDisp;
typedef System::DelphiInterface<BalloonLabelDisp> _di_BalloonLabelDisp;
__dispinterface INTERFACE_UUID("{000C0330-0000-0000-C000-000000000046}") BalloonLabelDisp  : public IDispatch 
{
	
};

__interface AnswerWizardFiles;
typedef System::DelphiInterface<AnswerWizardFiles> _di_AnswerWizardFiles;
__interface INTERFACE_UUID("{000C0361-0000-0000-C000-000000000046}") AnswerWizardFiles  : public _IMsoDispObj 
{
	
public:
	WideString operator[](int Index) { return Item[Index]; }
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Item(int Index, WideString &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Add(const WideString FileName) = 0 ;
	virtual HRESULT __safecall Delete(const WideString FileName) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Item(int Index) { WideString r; HRESULT hr = Get_Item(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Item[int Index] = {read=_scw_Get_Item/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface AnswerWizardFilesDisp;
typedef System::DelphiInterface<AnswerWizardFilesDisp> _di_AnswerWizardFilesDisp;
__dispinterface INTERFACE_UUID("{000C0361-0000-0000-C000-000000000046}") AnswerWizardFilesDisp  : public IDispatch 
{
	
};

__interface AnswerWizard;
typedef System::DelphiInterface<AnswerWizard> _di_AnswerWizard;
__interface INTERFACE_UUID("{000C0360-0000-0000-C000-000000000046}") AnswerWizard  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Files(_di_AnswerWizardFiles &Get_Files_result) = 0 ;
	virtual HRESULT __safecall ClearFileList(void) = 0 ;
	virtual HRESULT __safecall ResetFileList(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_AnswerWizardFiles _scw_Get_Files() { _di_AnswerWizardFiles r; HRESULT hr = Get_Files(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_AnswerWizardFiles Files = {read=_scw_Get_Files};
};

__dispinterface AnswerWizardDisp;
typedef System::DelphiInterface<AnswerWizardDisp> _di_AnswerWizardDisp;
__dispinterface INTERFACE_UUID("{000C0360-0000-0000-C000-000000000046}") AnswerWizardDisp  : public IDispatch 
{
	
};

__interface Assistant;
typedef System::DelphiInterface<Assistant> _di_Assistant;
__interface INTERFACE_UUID("{000C0322-0000-0000-C000-000000000046}") Assistant  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Move(int xLeft, int yTop) = 0 ;
	virtual HRESULT __safecall Set_Top(int pyTop) = 0 ;
	virtual HRESULT __safecall Get_Top(int &Get_Top_result) = 0 ;
	virtual HRESULT __safecall Set_Left(int pxLeft) = 0 ;
	virtual HRESULT __safecall Get_Left(int &Get_Left_result) = 0 ;
	virtual HRESULT __safecall Help(void) = 0 ;
	virtual HRESULT __safecall StartWizard(Word On_, const WideString Callback, int PrivateX, const OleVariant Animation, const OleVariant CustomTeaser, const OleVariant Top, const OleVariant Left, const OleVariant Bottom, const OleVariant Right, int &StartWizard_result) = 0 ;
	virtual HRESULT __safecall EndWizard(int WizardID, Word varfSuccess, const OleVariant Animation) = 0 ;
	virtual HRESULT __safecall ActivateWizard(int WizardID, Activex::TOleEnum act, const OleVariant Animation) = 0 ;
	virtual HRESULT __safecall ResetTips(void) = 0 ;
	virtual HRESULT __safecall Get_NewBalloon(_di_Balloon &Get_NewBalloon_result) = 0 ;
	virtual HRESULT __safecall Get_BalloonError(Activex::TOleEnum &Get_BalloonError_result) = 0 ;
	virtual HRESULT __safecall Get_Visible(Word &Get_Visible_result) = 0 ;
	virtual HRESULT __safecall Set_Visible(Word pvarfVisible) = 0 ;
	virtual HRESULT __safecall Get_Animation(Activex::TOleEnum &Get_Animation_result) = 0 ;
	virtual HRESULT __safecall Set_Animation(Activex::TOleEnum pfca) = 0 ;
	virtual HRESULT __safecall Get_Reduced(Word &Get_Reduced_result) = 0 ;
	virtual HRESULT __safecall Set_Reduced(Word pvarfReduced) = 0 ;
	virtual HRESULT __safecall Set_AssistWithHelp(Word pvarfAssistWithHelp) = 0 ;
	virtual HRESULT __safecall Get_AssistWithHelp(Word &Get_AssistWithHelp_result) = 0 ;
	virtual HRESULT __safecall Set_AssistWithWizards(Word pvarfAssistWithWizards) = 0 ;
	virtual HRESULT __safecall Get_AssistWithWizards(Word &Get_AssistWithWizards_result) = 0 ;
	virtual HRESULT __safecall Set_AssistWithAlerts(Word pvarfAssistWithAlerts) = 0 ;
	virtual HRESULT __safecall Get_AssistWithAlerts(Word &Get_AssistWithAlerts_result) = 0 ;
	virtual HRESULT __safecall Set_MoveWhenInTheWay(Word pvarfMove) = 0 ;
	virtual HRESULT __safecall Get_MoveWhenInTheWay(Word &Get_MoveWhenInTheWay_result) = 0 ;
	virtual HRESULT __safecall Set_Sounds(Word pvarfSounds) = 0 ;
	virtual HRESULT __safecall Get_Sounds(Word &Get_Sounds_result) = 0 ;
	virtual HRESULT __safecall Set_FeatureTips(Word pvarfFeatures) = 0 ;
	virtual HRESULT __safecall Get_FeatureTips(Word &Get_FeatureTips_result) = 0 ;
	virtual HRESULT __safecall Set_MouseTips(Word pvarfMouse) = 0 ;
	virtual HRESULT __safecall Get_MouseTips(Word &Get_MouseTips_result) = 0 ;
	virtual HRESULT __safecall Set_KeyboardShortcutTips(Word pvarfKeyboardShortcuts) = 0 ;
	virtual HRESULT __safecall Get_KeyboardShortcutTips(Word &Get_KeyboardShortcutTips_result) = 0 ;
	virtual HRESULT __safecall Set_HighPriorityTips(Word pvarfHighPriorityTips) = 0 ;
	virtual HRESULT __safecall Get_HighPriorityTips(Word &Get_HighPriorityTips_result) = 0 ;
	virtual HRESULT __safecall Set_TipOfDay(Word pvarfTipOfDay) = 0 ;
	virtual HRESULT __safecall Get_TipOfDay(Word &Get_TipOfDay_result) = 0 ;
	virtual HRESULT __safecall Set_GuessHelp(Word pvarfGuessHelp) = 0 ;
	virtual HRESULT __safecall Get_GuessHelp(Word &Get_GuessHelp_result) = 0 ;
	virtual HRESULT __safecall Set_SearchWhenProgramming(Word pvarfSearchInProgram) = 0 ;
	virtual HRESULT __safecall Get_SearchWhenProgramming(Word &Get_SearchWhenProgramming_result) = 0 ;
	virtual HRESULT __safecall Get_Item(WideString &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_FileName(WideString &Get_FileName_result) = 0 ;
	virtual HRESULT __safecall Set_FileName(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_On_(Word &Get_On__result) = 0 ;
	virtual HRESULT __safecall Set_On_(Word pvarfOn) = 0 ;
	virtual HRESULT __safecall DoAlert(const WideString bstrAlertTitle, const WideString bstrAlertText, Activex::TOleEnum alb, Activex::TOleEnum alc, Activex::TOleEnum ald, Activex::TOleEnum alq, Word varfSysAlert, int &DoAlert_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	__property int Top = {write=Set_Top};
	__property int Left = {write=Set_Left};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Balloon _scw_Get_NewBalloon() { _di_Balloon r; HRESULT hr = Get_NewBalloon(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Balloon NewBalloon = {read=_scw_Get_NewBalloon};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_BalloonError() { Activex::TOleEnum r; HRESULT hr = Get_BalloonError(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum BalloonError = {read=_scw_Get_BalloonError};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Visible() { Word r; HRESULT hr = Get_Visible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Visible = {read=_scw_Get_Visible};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Animation() { Activex::TOleEnum r; HRESULT hr = Get_Animation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Animation = {read=_scw_Get_Animation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Reduced() { Word r; HRESULT hr = Get_Reduced(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Reduced = {read=_scw_Get_Reduced};
	__property Word AssistWithHelp = {write=Set_AssistWithHelp};
	__property Word AssistWithWizards = {write=Set_AssistWithWizards};
	__property Word AssistWithAlerts = {write=Set_AssistWithAlerts};
	__property Word MoveWhenInTheWay = {write=Set_MoveWhenInTheWay};
	__property Word Sounds = {write=Set_Sounds};
	__property Word FeatureTips = {write=Set_FeatureTips};
	__property Word MouseTips = {write=Set_MouseTips};
	__property Word KeyboardShortcutTips = {write=Set_KeyboardShortcutTips};
	__property Word HighPriorityTips = {write=Set_HighPriorityTips};
	__property Word TipOfDay = {write=Set_TipOfDay};
	__property Word GuessHelp = {write=Set_GuessHelp};
	__property Word SearchWhenProgramming = {write=Set_SearchWhenProgramming};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Item() { WideString r; HRESULT hr = Get_Item(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Item = {read=_scw_Get_Item};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FileName() { WideString r; HRESULT hr = Get_FileName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FileName = {read=_scw_Get_FileName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_On_() { Word r; HRESULT hr = Get_On_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word On_ = {read=_scw_Get_On_};
};

__dispinterface AssistantDisp;
typedef System::DelphiInterface<AssistantDisp> _di_AssistantDisp;
__dispinterface INTERFACE_UUID("{000C0322-0000-0000-C000-000000000046}") AssistantDisp  : public IDispatch 
{
	
};

__interface DocumentProperty;
typedef System::DelphiInterface<DocumentProperty> _di_DocumentProperty;
__interface INTERFACE_UUID("{2DF8D04E-5BFA-101B-BDE5-00AA0044DE52}") DocumentProperty  : public IDispatch 
{
	
public:
	virtual _di_IDispatch __stdcall Get_Parent(void) = 0 ;
	virtual HRESULT __stdcall Delete(void) = 0 ;
	virtual HRESULT __stdcall Get_Name(int lcid, /* out */ WideString &pbstrRetVal) = 0 ;
	virtual HRESULT __stdcall Set_Name(int lcid, const WideString pbstrRetVal) = 0 ;
	virtual HRESULT __stdcall Get_Value(int lcid, /* out */ OleVariant &pvargRetVal) = 0 ;
	virtual HRESULT __stdcall Set_Value(int lcid, const OleVariant pvargRetVal) = 0 ;
	virtual HRESULT __stdcall Get_Type_(int lcid, /* out */ Activex::TOleEnum &ptypeRetVal) = 0 ;
	virtual HRESULT __stdcall Set_Type_(int lcid, Activex::TOleEnum ptypeRetVal) = 0 ;
	virtual HRESULT __stdcall Get_LinkToContent(/* out */ Word &pfLinkRetVal) = 0 ;
	virtual HRESULT __stdcall Set_LinkToContent(Word pfLinkRetVal) = 0 ;
	virtual HRESULT __stdcall Get_LinkSource(/* out */ WideString &pbstrSourceRetVal) = 0 ;
	virtual HRESULT __stdcall Set_LinkSource(const WideString pbstrSourceRetVal) = 0 ;
	virtual HRESULT __stdcall Get_Application(/* out */ _di_IDispatch &ppidisp) = 0 ;
	virtual HRESULT __stdcall Get_Creator(/* out */ int &plCreator) = 0 ;
};

__interface DocumentProperties;
typedef System::DelphiInterface<DocumentProperties> _di_DocumentProperties;
__interface INTERFACE_UUID("{2DF8D04D-5BFA-101B-BDE5-00AA0044DE52}") DocumentProperties  : public IDispatch 
{
	
public:
	virtual _di_IDispatch __stdcall Get_Parent(void) = 0 ;
	virtual HRESULT __stdcall Get_Item(const OleVariant Index, int lcid, /* out */ _di_DocumentProperty &ppIDocProp) = 0 ;
	virtual HRESULT __stdcall Get_Count(/* out */ int &pc) = 0 ;
	virtual HRESULT __stdcall Add(const WideString Name, Word LinkToContent, const OleVariant Type_, const OleVariant Value, const OleVariant LinkSource, int lcid, /* out */ _di_DocumentProperty &ppIDocProp) = 0 ;
	virtual HRESULT __stdcall Get__NewEnum(/* out */ System::_di_IInterface &ppunkEnum) = 0 ;
	virtual HRESULT __stdcall Get_Application(/* out */ _di_IDispatch &ppidisp) = 0 ;
	virtual HRESULT __stdcall Get_Creator(/* out */ int &plCreator) = 0 ;
};

__interface IFoundFiles;
typedef System::DelphiInterface<IFoundFiles> _di_IFoundFiles;
__interface INTERFACE_UUID("{000C0338-0000-0000-C000-000000000046}") IFoundFiles  : public IDispatch 
{
	
public:
	WideString operator[](int Index) { return Item[Index]; }
	
public:
	virtual HRESULT __safecall Get_Item(int Index, WideString &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Item(int Index) { WideString r; HRESULT hr = Get_Item(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Item[int Index] = {read=_scw_Get_Item/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface IFoundFilesDisp;
typedef System::DelphiInterface<IFoundFilesDisp> _di_IFoundFilesDisp;
__dispinterface INTERFACE_UUID("{000C0338-0000-0000-C000-000000000046}") IFoundFilesDisp  : public IDispatch 
{
	
};

__interface IFind;
typedef System::DelphiInterface<IFind> _di_IFind;
__interface INTERFACE_UUID("{000C0337-0000-0000-C000-000000000046}") IFind  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_SearchPath(WideString &Get_SearchPath_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_SubDir(Word &Get_SubDir_result) = 0 ;
	virtual HRESULT __safecall Get_Title(WideString &Get_Title_result) = 0 ;
	virtual HRESULT __safecall Get_Author(WideString &Get_Author_result) = 0 ;
	virtual HRESULT __safecall Get_Keywords(WideString &Get_Keywords_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Get_Options(Activex::TOleEnum &Get_Options_result) = 0 ;
	virtual HRESULT __safecall Get_MatchCase(Word &Get_MatchCase_result) = 0 ;
	virtual HRESULT __safecall Get_Text(WideString &Get_Text_result) = 0 ;
	virtual HRESULT __safecall Get_PatternMatch(Word &Get_PatternMatch_result) = 0 ;
	virtual HRESULT __safecall Get_DateSavedFrom(OleVariant &Get_DateSavedFrom_result) = 0 ;
	virtual HRESULT __safecall Get_DateSavedTo(OleVariant &Get_DateSavedTo_result) = 0 ;
	virtual HRESULT __safecall Get_SavedBy(WideString &Get_SavedBy_result) = 0 ;
	virtual HRESULT __safecall Get_DateCreatedFrom(OleVariant &Get_DateCreatedFrom_result) = 0 ;
	virtual HRESULT __safecall Get_DateCreatedTo(OleVariant &Get_DateCreatedTo_result) = 0 ;
	virtual HRESULT __safecall Get_View(Activex::TOleEnum &Get_View_result) = 0 ;
	virtual HRESULT __safecall Get_SortBy(Activex::TOleEnum &Get_SortBy_result) = 0 ;
	virtual HRESULT __safecall Get_ListBy(Activex::TOleEnum &Get_ListBy_result) = 0 ;
	virtual HRESULT __safecall Get_SelectedFile(int &Get_SelectedFile_result) = 0 ;
	virtual HRESULT __safecall Get_Results(_di_IFoundFiles &Get_Results_result) = 0 ;
	virtual HRESULT __safecall Show(int &Show_result) = 0 ;
	virtual HRESULT __safecall Set_SearchPath(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Set_SubDir(Word retval) = 0 ;
	virtual HRESULT __safecall Set_Title(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Set_Author(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Set_Keywords(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Set_Options(Activex::TOleEnum penmOptions) = 0 ;
	virtual HRESULT __safecall Set_MatchCase(Word retval) = 0 ;
	virtual HRESULT __safecall Set_Text(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Set_PatternMatch(Word retval) = 0 ;
	virtual HRESULT __safecall Set_DateSavedFrom(const OleVariant pdatSavedFrom) = 0 ;
	virtual HRESULT __safecall Set_DateSavedTo(const OleVariant pdatSavedTo) = 0 ;
	virtual HRESULT __safecall Set_SavedBy(const WideString pbstr) = 0 ;
	virtual HRESULT __safecall Set_DateCreatedFrom(const OleVariant pdatCreatedFrom) = 0 ;
	virtual HRESULT __safecall Set_DateCreatedTo(const OleVariant pdatCreatedTo) = 0 ;
	virtual HRESULT __safecall Set_View(Activex::TOleEnum penmView) = 0 ;
	virtual HRESULT __safecall Set_SortBy(Activex::TOleEnum penmSortBy) = 0 ;
	virtual HRESULT __safecall Set_ListBy(Activex::TOleEnum penmListBy) = 0 ;
	virtual HRESULT __safecall Set_SelectedFile(int pintSelectedFile) = 0 ;
	virtual HRESULT __safecall Execute(void) = 0 ;
	virtual HRESULT __safecall Load(const WideString bstrQueryName) = 0 ;
	virtual HRESULT __safecall Save(const WideString bstrQueryName) = 0 ;
	virtual HRESULT __safecall Delete(const WideString bstrQueryName) = 0 ;
	virtual HRESULT __safecall Get_FileType(int &Get_FileType_result) = 0 ;
	virtual HRESULT __safecall Set_FileType(int plFileType) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_SearchPath() { WideString r; HRESULT hr = Get_SearchPath(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString SearchPath = {read=_scw_Get_SearchPath, write=Set_SearchPath};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_SubDir() { Word r; HRESULT hr = Get_SubDir(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word SubDir = {read=_scw_Get_SubDir};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Title() { WideString r; HRESULT hr = Get_Title(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Title = {read=_scw_Get_Title};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Author() { WideString r; HRESULT hr = Get_Author(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Author = {read=_scw_Get_Author};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Keywords() { WideString r; HRESULT hr = Get_Keywords(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Keywords = {read=_scw_Get_Keywords};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Options() { Activex::TOleEnum r; HRESULT hr = Get_Options(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Options = {read=_scw_Get_Options};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_MatchCase() { Word r; HRESULT hr = Get_MatchCase(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word MatchCase = {read=_scw_Get_MatchCase};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Text() { WideString r; HRESULT hr = Get_Text(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Text = {read=_scw_Get_Text};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_PatternMatch() { Word r; HRESULT hr = Get_PatternMatch(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word PatternMatch = {read=_scw_Get_PatternMatch};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_DateSavedFrom() { OleVariant r; HRESULT hr = Get_DateSavedFrom(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant DateSavedFrom = {read=_scw_Get_DateSavedFrom};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_DateSavedTo() { OleVariant r; HRESULT hr = Get_DateSavedTo(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant DateSavedTo = {read=_scw_Get_DateSavedTo};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_SavedBy() { WideString r; HRESULT hr = Get_SavedBy(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString SavedBy = {read=_scw_Get_SavedBy};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_DateCreatedFrom() { OleVariant r; HRESULT hr = Get_DateCreatedFrom(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant DateCreatedFrom = {read=_scw_Get_DateCreatedFrom};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_DateCreatedTo() { OleVariant r; HRESULT hr = Get_DateCreatedTo(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant DateCreatedTo = {read=_scw_Get_DateCreatedTo};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_View() { Activex::TOleEnum r; HRESULT hr = Get_View(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum View = {read=_scw_Get_View};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_SortBy() { Activex::TOleEnum r; HRESULT hr = Get_SortBy(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum SortBy = {read=_scw_Get_SortBy};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_ListBy() { Activex::TOleEnum r; HRESULT hr = Get_ListBy(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum ListBy = {read=_scw_Get_ListBy};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_SelectedFile() { int r; HRESULT hr = Get_SelectedFile(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int SelectedFile = {read=_scw_Get_SelectedFile};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IFoundFiles _scw_Get_Results() { _di_IFoundFiles r; HRESULT hr = Get_Results(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IFoundFiles Results = {read=_scw_Get_Results};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_FileType() { int r; HRESULT hr = Get_FileType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int FileType = {read=_scw_Get_FileType};
};

__dispinterface IFindDisp;
typedef System::DelphiInterface<IFindDisp> _di_IFindDisp;
__dispinterface INTERFACE_UUID("{000C0337-0000-0000-C000-000000000046}") IFindDisp  : public IDispatch 
{
	
};

__interface FoundFiles;
typedef System::DelphiInterface<FoundFiles> _di_FoundFiles;
__interface INTERFACE_UUID("{000C0331-0000-0000-C000-000000000046}") FoundFiles  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Item(int Index, int lcid, WideString &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Item(int Index, int lcid) { WideString r; HRESULT hr = Get_Item(Index, lcid, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Item[int Index][int lcid] = {read=_scw_Get_Item};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface FoundFilesDisp;
typedef System::DelphiInterface<FoundFilesDisp> _di_FoundFilesDisp;
__dispinterface INTERFACE_UUID("{000C0331-0000-0000-C000-000000000046}") FoundFilesDisp  : public IDispatch 
{
	
};

__interface PropertyTest;
typedef System::DelphiInterface<PropertyTest> _di_PropertyTest;
__interface INTERFACE_UUID("{000C0333-0000-0000-C000-000000000046}") PropertyTest  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Condition(Activex::TOleEnum &Get_Condition_result) = 0 ;
	virtual HRESULT __safecall Get_Value(OleVariant &Get_Value_result) = 0 ;
	virtual HRESULT __safecall Get_SecondValue(OleVariant &Get_SecondValue_result) = 0 ;
	virtual HRESULT __safecall Get_Connector(Activex::TOleEnum &Get_Connector_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Condition() { Activex::TOleEnum r; HRESULT hr = Get_Condition(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Condition = {read=_scw_Get_Condition};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_Value() { OleVariant r; HRESULT hr = Get_Value(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant Value = {read=_scw_Get_Value};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_SecondValue() { OleVariant r; HRESULT hr = Get_SecondValue(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant SecondValue = {read=_scw_Get_SecondValue};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Connector() { Activex::TOleEnum r; HRESULT hr = Get_Connector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Connector = {read=_scw_Get_Connector};
};

__dispinterface PropertyTestDisp;
typedef System::DelphiInterface<PropertyTestDisp> _di_PropertyTestDisp;
__dispinterface INTERFACE_UUID("{000C0333-0000-0000-C000-000000000046}") PropertyTestDisp  : public IDispatch 
{
	
};

__interface PropertyTests;
typedef System::DelphiInterface<PropertyTests> _di_PropertyTests;
__interface INTERFACE_UUID("{000C0334-0000-0000-C000-000000000046}") PropertyTests  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Item(int Index, int lcid, _di_PropertyTest &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Add(const WideString Name, Activex::TOleEnum Condition, const OleVariant Value, const OleVariant SecondValue, Activex::TOleEnum Connector) = 0 ;
	virtual HRESULT __safecall Remove(int Index) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_PropertyTest _scw_Get_Item(int Index, int lcid) { _di_PropertyTest r; HRESULT hr = Get_Item(Index, lcid, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_PropertyTest Item[int Index][int lcid] = {read=_scw_Get_Item};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface PropertyTestsDisp;
typedef System::DelphiInterface<PropertyTestsDisp> _di_PropertyTestsDisp;
__dispinterface INTERFACE_UUID("{000C0334-0000-0000-C000-000000000046}") PropertyTestsDisp  : public IDispatch 
{
	
};

__interface FileSearch;
typedef System::DelphiInterface<FileSearch> _di_FileSearch;
__interface SearchScopes;
typedef System::DelphiInterface<SearchScopes> _di_SearchScopes;
__interface SearchFolders;
typedef System::DelphiInterface<SearchFolders> _di_SearchFolders;
__interface FileTypes;
typedef System::DelphiInterface<FileTypes> _di_FileTypes;
__interface INTERFACE_UUID("{000C0332-0000-0000-C000-000000000046}") FileSearch  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_SearchSubFolders(Word &Get_SearchSubFolders_result) = 0 ;
	virtual HRESULT __safecall Set_SearchSubFolders(Word SearchSubFoldersRetVal) = 0 ;
	virtual HRESULT __safecall Get_MatchTextExactly(Word &Get_MatchTextExactly_result) = 0 ;
	virtual HRESULT __safecall Set_MatchTextExactly(Word MatchTextRetVal) = 0 ;
	virtual HRESULT __safecall Get_MatchAllWordForms(Word &Get_MatchAllWordForms_result) = 0 ;
	virtual HRESULT __safecall Set_MatchAllWordForms(Word MatchAllWordFormsRetVal) = 0 ;
	virtual HRESULT __safecall Get_FileName(WideString &Get_FileName_result) = 0 ;
	virtual HRESULT __safecall Set_FileName(const WideString FileNameRetVal) = 0 ;
	virtual HRESULT __safecall Get_FileType(Activex::TOleEnum &Get_FileType_result) = 0 ;
	virtual HRESULT __safecall Set_FileType(Activex::TOleEnum FileTypeRetVal) = 0 ;
	virtual HRESULT __safecall Get_LastModified(Activex::TOleEnum &Get_LastModified_result) = 0 ;
	virtual HRESULT __safecall Set_LastModified(Activex::TOleEnum LastModifiedRetVal) = 0 ;
	virtual HRESULT __safecall Get_TextOrProperty(WideString &Get_TextOrProperty_result) = 0 ;
	virtual HRESULT __safecall Set_TextOrProperty(const WideString TextOrProperty) = 0 ;
	virtual HRESULT __safecall Get_LookIn(WideString &Get_LookIn_result) = 0 ;
	virtual HRESULT __safecall Set_LookIn(const WideString LookInRetVal) = 0 ;
	virtual HRESULT __safecall Execute(Activex::TOleEnum SortBy, Activex::TOleEnum SortOrder, Word AlwaysAccurate, int &Execute_result) = 0 ;
	virtual HRESULT __safecall NewSearch(void) = 0 ;
	virtual HRESULT __safecall Get_FoundFiles(_di_FoundFiles &Get_FoundFiles_result) = 0 ;
	virtual HRESULT __safecall Get_PropertyTests(_di_PropertyTests &Get_PropertyTests_result) = 0 ;
	virtual HRESULT __safecall Get_SearchScopes(_di_SearchScopes &Get_SearchScopes_result) = 0 ;
	virtual HRESULT __safecall Get_SearchFolders(_di_SearchFolders &Get_SearchFolders_result) = 0 ;
	virtual HRESULT __safecall Get_FileTypes(_di_FileTypes &Get_FileTypes_result) = 0 ;
	virtual HRESULT __safecall RefreshScopes(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_SearchSubFolders() { Word r; HRESULT hr = Get_SearchSubFolders(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word SearchSubFolders = {read=_scw_Get_SearchSubFolders, write=Set_SearchSubFolders};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_MatchTextExactly() { Word r; HRESULT hr = Get_MatchTextExactly(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word MatchTextExactly = {read=_scw_Get_MatchTextExactly, write=Set_MatchTextExactly};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_MatchAllWordForms() { Word r; HRESULT hr = Get_MatchAllWordForms(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word MatchAllWordForms = {read=_scw_Get_MatchAllWordForms, write=Set_MatchAllWordForms};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FileName() { WideString r; HRESULT hr = Get_FileName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FileName = {read=_scw_Get_FileName, write=Set_FileName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_FileType() { Activex::TOleEnum r; HRESULT hr = Get_FileType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum FileType = {read=_scw_Get_FileType, write=Set_FileType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_LastModified() { Activex::TOleEnum r; HRESULT hr = Get_LastModified(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum LastModified = {read=_scw_Get_LastModified, write=Set_LastModified};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_TextOrProperty() { WideString r; HRESULT hr = Get_TextOrProperty(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString TextOrProperty = {read=_scw_Get_TextOrProperty, write=Set_TextOrProperty};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_LookIn() { WideString r; HRESULT hr = Get_LookIn(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString LookIn = {read=_scw_Get_LookIn, write=Set_LookIn};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FoundFiles _scw_Get_FoundFiles() { _di_FoundFiles r; HRESULT hr = Get_FoundFiles(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FoundFiles FoundFiles = {read=_scw_Get_FoundFiles};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_PropertyTests _scw_Get_PropertyTests() { _di_PropertyTests r; HRESULT hr = Get_PropertyTests(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_PropertyTests PropertyTests = {read=_scw_Get_PropertyTests};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_SearchScopes _scw_Get_SearchScopes() { _di_SearchScopes r; HRESULT hr = Get_SearchScopes(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_SearchScopes SearchScopes = {read=_scw_Get_SearchScopes};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_SearchFolders _scw_Get_SearchFolders() { _di_SearchFolders r; HRESULT hr = Get_SearchFolders(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_SearchFolders SearchFolders = {read=_scw_Get_SearchFolders};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FileTypes _scw_Get_FileTypes() { _di_FileTypes r; HRESULT hr = Get_FileTypes(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FileTypes FileTypes = {read=_scw_Get_FileTypes};
};

__dispinterface FileSearchDisp;
typedef System::DelphiInterface<FileSearchDisp> _di_FileSearchDisp;
__dispinterface INTERFACE_UUID("{000C0332-0000-0000-C000-000000000046}") FileSearchDisp  : public IDispatch 
{
	
};

__interface COMAddIn;
typedef System::DelphiInterface<COMAddIn> _di_COMAddIn;
__interface INTERFACE_UUID("{000C033A-0000-0000-C000-000000000046}") COMAddIn  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Description(WideString &Get_Description_result) = 0 ;
	virtual HRESULT __safecall Set_Description(const WideString RetValue) = 0 ;
	virtual HRESULT __safecall Get_ProgId(WideString &Get_ProgId_result) = 0 ;
	virtual HRESULT __safecall Get_Guid(WideString &Get_Guid_result) = 0 ;
	virtual HRESULT __safecall Get_Connect(Word &Get_Connect_result) = 0 ;
	virtual HRESULT __safecall Set_Connect(Word RetValue) = 0 ;
	virtual HRESULT __safecall Get_Object_(_di_IDispatch &Get_Object__result) = 0 ;
	virtual HRESULT __safecall Set_Object_(const _di_IDispatch RetValue) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Description() { WideString r; HRESULT hr = Get_Description(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Description = {read=_scw_Get_Description, write=Set_Description};
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
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
};

__dispinterface COMAddInDisp;
typedef System::DelphiInterface<COMAddInDisp> _di_COMAddInDisp;
__dispinterface INTERFACE_UUID("{000C033A-0000-0000-C000-000000000046}") COMAddInDisp  : public IDispatch 
{
	
};

__interface COMAddIns;
typedef System::DelphiInterface<COMAddIns> _di_COMAddIns;
__interface INTERFACE_UUID("{000C0339-0000-0000-C000-000000000046}") COMAddIns  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Item(OleVariant &Index, _di_COMAddIn &Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Update(void) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall SetAppModal(Word varfModal) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
};

__dispinterface COMAddInsDisp;
typedef System::DelphiInterface<COMAddInsDisp> _di_COMAddInsDisp;
__dispinterface INTERFACE_UUID("{000C0339-0000-0000-C000-000000000046}") COMAddInsDisp  : public IDispatch 
{
	
};

__interface LanguageSettings;
typedef System::DelphiInterface<LanguageSettings> _di_LanguageSettings;
__interface INTERFACE_UUID("{000C0353-0000-0000-C000-000000000046}") LanguageSettings  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_LanguageID(Activex::TOleEnum Id, int &Get_LanguageID_result) = 0 ;
	virtual HRESULT __safecall Get_LanguagePreferredForEditing(Activex::TOleEnum lid, Word &Get_LanguagePreferredForEditing_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_LanguageID(Activex::TOleEnum Id) { int r; HRESULT hr = Get_LanguageID(Id, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int LanguageID[Activex::TOleEnum Id] = {read=_scw_Get_LanguageID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_LanguagePreferredForEditing(Activex::TOleEnum lid) { Word r; HRESULT hr = Get_LanguagePreferredForEditing(lid, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word LanguagePreferredForEditing[Activex::TOleEnum lid] = {read=_scw_Get_LanguagePreferredForEditing};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
};

__dispinterface LanguageSettingsDisp;
typedef System::DelphiInterface<LanguageSettingsDisp> _di_LanguageSettingsDisp;
__dispinterface INTERFACE_UUID("{000C0353-0000-0000-C000-000000000046}") LanguageSettingsDisp  : public IDispatch 
{
	
};

__interface ICommandBarsEvents;
typedef System::DelphiInterface<ICommandBarsEvents> _di_ICommandBarsEvents;
__interface INTERFACE_UUID("{55F88892-7708-11D1-ACEB-006008961DA5}") ICommandBarsEvents  : public IDispatch 
{
	
public:
	virtual void __stdcall OnUpdate(void) = 0 ;
};

__dispinterface ICommandBarsEventsDisp;
typedef System::DelphiInterface<ICommandBarsEventsDisp> _di_ICommandBarsEventsDisp;
__dispinterface INTERFACE_UUID("{55F88892-7708-11D1-ACEB-006008961DA5}") ICommandBarsEventsDisp  : public IDispatch 
{
	
};

__dispinterface _CommandBarsEvents;
typedef System::DelphiInterface<_CommandBarsEvents> _di__CommandBarsEvents;
__dispinterface INTERFACE_UUID("{000C0352-0000-0000-C000-000000000046}") _CommandBarsEvents  : public IDispatch 
{
	
};

__interface ICommandBarComboBoxEvents;
typedef System::DelphiInterface<ICommandBarComboBoxEvents> _di_ICommandBarComboBoxEvents;
__interface INTERFACE_UUID("{55F88896-7708-11D1-ACEB-006008961DA5}") ICommandBarComboBoxEvents  : public IDispatch 
{
	
public:
	virtual void __stdcall Change(const _di__CommandBarComboBox Ctrl) = 0 ;
};

__dispinterface ICommandBarComboBoxEventsDisp;
typedef System::DelphiInterface<ICommandBarComboBoxEventsDisp> _di_ICommandBarComboBoxEventsDisp;
__dispinterface INTERFACE_UUID("{55F88896-7708-11D1-ACEB-006008961DA5}") ICommandBarComboBoxEventsDisp  : public IDispatch 
{
	
};

__dispinterface _CommandBarComboBoxEvents;
typedef System::DelphiInterface<_CommandBarComboBoxEvents> _di__CommandBarComboBoxEvents;
__dispinterface INTERFACE_UUID("{000C0354-0000-0000-C000-000000000046}") _CommandBarComboBoxEvents  : public IDispatch 
{
	
};

__interface ICommandBarButtonEvents;
typedef System::DelphiInterface<ICommandBarButtonEvents> _di_ICommandBarButtonEvents;
__interface INTERFACE_UUID("{55F88890-7708-11D1-ACEB-006008961DA5}") ICommandBarButtonEvents  : public IDispatch 
{
	
public:
	virtual void __stdcall Click(const _di__CommandBarButton Ctrl, Word &CancelDefault) = 0 ;
};

__dispinterface ICommandBarButtonEventsDisp;
typedef System::DelphiInterface<ICommandBarButtonEventsDisp> _di_ICommandBarButtonEventsDisp;
__dispinterface INTERFACE_UUID("{55F88890-7708-11D1-ACEB-006008961DA5}") ICommandBarButtonEventsDisp  : public IDispatch 
{
	
};

__dispinterface _CommandBarButtonEvents;
typedef System::DelphiInterface<_CommandBarButtonEvents> _di__CommandBarButtonEvents;
__dispinterface INTERFACE_UUID("{000C0351-0000-0000-C000-000000000046}") _CommandBarButtonEvents  : public IDispatch 
{
	
};

__interface WebPageFont;
typedef System::DelphiInterface<WebPageFont> _di_WebPageFont;
__interface INTERFACE_UUID("{000C0913-0000-0000-C000-000000000046}") WebPageFont  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_ProportionalFont(WideString &Get_ProportionalFont_result) = 0 ;
	virtual HRESULT __safecall Set_ProportionalFont(const WideString pstr) = 0 ;
	virtual HRESULT __safecall Get_ProportionalFontSize(float &Get_ProportionalFontSize_result) = 0 ;
	virtual HRESULT __safecall Set_ProportionalFontSize(float pf) = 0 ;
	virtual HRESULT __safecall Get_FixedWidthFont(WideString &Get_FixedWidthFont_result) = 0 ;
	virtual HRESULT __safecall Set_FixedWidthFont(const WideString pstr) = 0 ;
	virtual HRESULT __safecall Get_FixedWidthFontSize(float &Get_FixedWidthFontSize_result) = 0 ;
	virtual HRESULT __safecall Set_FixedWidthFontSize(float pf) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ProportionalFont() { WideString r; HRESULT hr = Get_ProportionalFont(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ProportionalFont = {read=_scw_Get_ProportionalFont, write=Set_ProportionalFont};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_ProportionalFontSize() { float r; HRESULT hr = Get_ProportionalFontSize(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float ProportionalFontSize = {read=_scw_Get_ProportionalFontSize, write=Set_ProportionalFontSize};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FixedWidthFont() { WideString r; HRESULT hr = Get_FixedWidthFont(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FixedWidthFont = {read=_scw_Get_FixedWidthFont, write=Set_FixedWidthFont};
	#pragma option push -w-inl
	/* safecall wrapper */ inline float _scw_Get_FixedWidthFontSize() { float r; HRESULT hr = Get_FixedWidthFontSize(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property float FixedWidthFontSize = {read=_scw_Get_FixedWidthFontSize, write=Set_FixedWidthFontSize};
};

__dispinterface WebPageFontDisp;
typedef System::DelphiInterface<WebPageFontDisp> _di_WebPageFontDisp;
__dispinterface INTERFACE_UUID("{000C0913-0000-0000-C000-000000000046}") WebPageFontDisp  : public IDispatch 
{
	
};

__interface WebPageFonts;
typedef System::DelphiInterface<WebPageFonts> _di_WebPageFonts;
__interface INTERFACE_UUID("{000C0914-0000-0000-C000-000000000046}") WebPageFonts  : public _IMsoDispObj 
{
	
public:
	_di_WebPageFont operator[](Activex::TOleEnum Index) { return Item[Index]; }
	
public:
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get_Item(Activex::TOleEnum Index, _di_WebPageFont &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_WebPageFont _scw_Get_Item(Activex::TOleEnum Index) { _di_WebPageFont r; HRESULT hr = Get_Item(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_WebPageFont Item[Activex::TOleEnum Index] = {read=_scw_Get_Item/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface WebPageFontsDisp;
typedef System::DelphiInterface<WebPageFontsDisp> _di_WebPageFontsDisp;
__dispinterface INTERFACE_UUID("{000C0914-0000-0000-C000-000000000046}") WebPageFontsDisp  : public IDispatch 
{
	
};

__interface HTMLProjectItem;
typedef System::DelphiInterface<HTMLProjectItem> _di_HTMLProjectItem;
__interface INTERFACE_UUID("{000C0358-0000-0000-C000-000000000046}") HTMLProjectItem  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_IsOpen(Word &Get_IsOpen_result) = 0 ;
	virtual HRESULT __safecall LoadFromFile(const WideString FileName) = 0 ;
	virtual HRESULT __safecall Open(Activex::TOleEnum OpenKind) = 0 ;
	virtual HRESULT __safecall SaveCopyAs(const WideString FileName) = 0 ;
	virtual HRESULT __safecall Get_Text(WideString &Get_Text_result) = 0 ;
	virtual HRESULT __safecall Set_Text(const WideString Text) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsOpen() { Word r; HRESULT hr = Get_IsOpen(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsOpen = {read=_scw_Get_IsOpen};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Text() { WideString r; HRESULT hr = Get_Text(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Text = {read=_scw_Get_Text, write=Set_Text};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
};

__dispinterface HTMLProjectItemDisp;
typedef System::DelphiInterface<HTMLProjectItemDisp> _di_HTMLProjectItemDisp;
__dispinterface INTERFACE_UUID("{000C0358-0000-0000-C000-000000000046}") HTMLProjectItemDisp  : public IDispatch 
{
	
};

__interface HTMLProjectItems;
typedef System::DelphiInterface<HTMLProjectItems> _di_HTMLProjectItems;
__interface INTERFACE_UUID("{000C0357-0000-0000-C000-000000000046}") HTMLProjectItems  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Item(OleVariant &Index, _di_HTMLProjectItem &Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
};

__dispinterface HTMLProjectItemsDisp;
typedef System::DelphiInterface<HTMLProjectItemsDisp> _di_HTMLProjectItemsDisp;
__dispinterface INTERFACE_UUID("{000C0357-0000-0000-C000-000000000046}") HTMLProjectItemsDisp  : public IDispatch 
{
	
};

__interface HTMLProject;
typedef System::DelphiInterface<HTMLProject> _di_HTMLProject;
__interface INTERFACE_UUID("{000C0356-0000-0000-C000-000000000046}") HTMLProject  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_State(Activex::TOleEnum &Get_State_result) = 0 ;
	virtual HRESULT __safecall RefreshProject(Word Refresh) = 0 ;
	virtual HRESULT __safecall RefreshDocument(Word Refresh) = 0 ;
	virtual HRESULT __safecall Get_HTMLProjectItems(_di_HTMLProjectItems &Get_HTMLProjectItems_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Open(Activex::TOleEnum OpenKind) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_State() { Activex::TOleEnum r; HRESULT hr = Get_State(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum State = {read=_scw_Get_State};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_HTMLProjectItems _scw_Get_HTMLProjectItems() { _di_HTMLProjectItems r; HRESULT hr = Get_HTMLProjectItems(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_HTMLProjectItems HTMLProjectItems = {read=_scw_Get_HTMLProjectItems};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
};

__dispinterface HTMLProjectDisp;
typedef System::DelphiInterface<HTMLProjectDisp> _di_HTMLProjectDisp;
__dispinterface INTERFACE_UUID("{000C0356-0000-0000-C000-000000000046}") HTMLProjectDisp  : public IDispatch 
{
	
};

__interface MsoDebugOptions;
typedef System::DelphiInterface<MsoDebugOptions> _di_MsoDebugOptions;
__interface INTERFACE_UUID("{000C035A-0000-0000-C000-000000000046}") MsoDebugOptions  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_FeatureReports(int &Get_FeatureReports_result) = 0 ;
	virtual HRESULT __safecall Set_FeatureReports(int puintFeatureReports) = 0 ;
	virtual HRESULT __safecall Get_OutputToDebugger(Word &Get_OutputToDebugger_result) = 0 ;
	virtual HRESULT __safecall Set_OutputToDebugger(Word pvarfOutputToDebugger) = 0 ;
	virtual HRESULT __safecall Get_OutputToFile(Word &Get_OutputToFile_result) = 0 ;
	virtual HRESULT __safecall Set_OutputToFile(Word pvarfOutputToFile) = 0 ;
	virtual HRESULT __safecall Get_OutputToMessageBox(Word &Get_OutputToMessageBox_result) = 0 ;
	virtual HRESULT __safecall Set_OutputToMessageBox(Word pvarfOutputToMessageBox) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_FeatureReports() { int r; HRESULT hr = Get_FeatureReports(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int FeatureReports = {read=_scw_Get_FeatureReports, write=Set_FeatureReports};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_OutputToDebugger() { Word r; HRESULT hr = Get_OutputToDebugger(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word OutputToDebugger = {read=_scw_Get_OutputToDebugger, write=Set_OutputToDebugger};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_OutputToFile() { Word r; HRESULT hr = Get_OutputToFile(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word OutputToFile = {read=_scw_Get_OutputToFile, write=Set_OutputToFile};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_OutputToMessageBox() { Word r; HRESULT hr = Get_OutputToMessageBox(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word OutputToMessageBox = {read=_scw_Get_OutputToMessageBox, write=Set_OutputToMessageBox};
};

__dispinterface MsoDebugOptionsDisp;
typedef System::DelphiInterface<MsoDebugOptionsDisp> _di_MsoDebugOptionsDisp;
__dispinterface INTERFACE_UUID("{000C035A-0000-0000-C000-000000000046}") MsoDebugOptionsDisp  : public IDispatch 
{
	
};

__interface FileDialogSelectedItems;
typedef System::DelphiInterface<FileDialogSelectedItems> _di_FileDialogSelectedItems;
__interface INTERFACE_UUID("{000C0363-0000-0000-C000-000000000046}") FileDialogSelectedItems  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(int Index, WideString &Item_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface FileDialogSelectedItemsDisp;
typedef System::DelphiInterface<FileDialogSelectedItemsDisp> _di_FileDialogSelectedItemsDisp;
__dispinterface INTERFACE_UUID("{000C0363-0000-0000-C000-000000000046}") FileDialogSelectedItemsDisp  : public IDispatch 
{
	
};

__interface FileDialogFilter;
typedef System::DelphiInterface<FileDialogFilter> _di_FileDialogFilter;
__interface INTERFACE_UUID("{000C0364-0000-0000-C000-000000000046}") FileDialogFilter  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Extensions(WideString &Get_Extensions_result) = 0 ;
	virtual HRESULT __safecall Get_Description(WideString &Get_Description_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Extensions() { WideString r; HRESULT hr = Get_Extensions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Extensions = {read=_scw_Get_Extensions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Description() { WideString r; HRESULT hr = Get_Description(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Description = {read=_scw_Get_Description};
};

__dispinterface FileDialogFilterDisp;
typedef System::DelphiInterface<FileDialogFilterDisp> _di_FileDialogFilterDisp;
__dispinterface INTERFACE_UUID("{000C0364-0000-0000-C000-000000000046}") FileDialogFilterDisp  : public IDispatch 
{
	
};

__interface FileDialogFilters;
typedef System::DelphiInterface<FileDialogFilters> _di_FileDialogFilters;
__interface INTERFACE_UUID("{000C0365-0000-0000-C000-000000000046}") FileDialogFilters  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(int Index, _di_FileDialogFilter &Item_result) = 0 ;
	virtual HRESULT __safecall Delete(const OleVariant filter) = 0 ;
	virtual HRESULT __safecall Clear(void) = 0 ;
	virtual HRESULT __safecall Add(const WideString Description, const WideString Extensions, const OleVariant Position, _di_FileDialogFilter &Add_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface FileDialogFiltersDisp;
typedef System::DelphiInterface<FileDialogFiltersDisp> _di_FileDialogFiltersDisp;
__dispinterface INTERFACE_UUID("{000C0365-0000-0000-C000-000000000046}") FileDialogFiltersDisp  : public IDispatch 
{
	
};

__interface FileDialog;
typedef System::DelphiInterface<FileDialog> _di_FileDialog;
__interface INTERFACE_UUID("{000C0362-0000-0000-C000-000000000046}") FileDialog  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Filters(_di_FileDialogFilters &Get_Filters_result) = 0 ;
	virtual HRESULT __safecall Get_FilterIndex(int &Get_FilterIndex_result) = 0 ;
	virtual HRESULT __safecall Set_FilterIndex(int FilterIndex) = 0 ;
	virtual HRESULT __safecall Get_Title(WideString &Get_Title_result) = 0 ;
	virtual HRESULT __safecall Set_Title(const WideString Title) = 0 ;
	virtual HRESULT __safecall Get_ButtonName(WideString &Get_ButtonName_result) = 0 ;
	virtual HRESULT __safecall Set_ButtonName(const WideString ButtonName) = 0 ;
	virtual HRESULT __safecall Get_AllowMultiSelect(Word &Get_AllowMultiSelect_result) = 0 ;
	virtual HRESULT __safecall Set_AllowMultiSelect(Word pvarfAllowMultiSelect) = 0 ;
	virtual HRESULT __safecall Get_InitialView(Activex::TOleEnum &Get_InitialView_result) = 0 ;
	virtual HRESULT __safecall Set_InitialView(Activex::TOleEnum pinitialview) = 0 ;
	virtual HRESULT __safecall Get_InitialFileName(WideString &Get_InitialFileName_result) = 0 ;
	virtual HRESULT __safecall Set_InitialFileName(const WideString InitialFileName) = 0 ;
	virtual HRESULT __safecall Get_SelectedItems(_di_FileDialogSelectedItems &Get_SelectedItems_result) = 0 ;
	virtual HRESULT __safecall Get_DialogType(Activex::TOleEnum &Get_DialogType_result) = 0 ;
	virtual HRESULT __safecall Get_Item(WideString &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Show(int &Show_result) = 0 ;
	virtual HRESULT __safecall Execute(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FileDialogFilters _scw_Get_Filters() { _di_FileDialogFilters r; HRESULT hr = Get_Filters(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FileDialogFilters Filters = {read=_scw_Get_Filters};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_FilterIndex() { int r; HRESULT hr = Get_FilterIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int FilterIndex = {read=_scw_Get_FilterIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Title() { WideString r; HRESULT hr = Get_Title(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Title = {read=_scw_Get_Title};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ButtonName() { WideString r; HRESULT hr = Get_ButtonName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ButtonName = {read=_scw_Get_ButtonName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_AllowMultiSelect() { Word r; HRESULT hr = Get_AllowMultiSelect(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word AllowMultiSelect = {read=_scw_Get_AllowMultiSelect};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_InitialView() { Activex::TOleEnum r; HRESULT hr = Get_InitialView(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum InitialView = {read=_scw_Get_InitialView};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_InitialFileName() { WideString r; HRESULT hr = Get_InitialFileName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString InitialFileName = {read=_scw_Get_InitialFileName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FileDialogSelectedItems _scw_Get_SelectedItems() { _di_FileDialogSelectedItems r; HRESULT hr = Get_SelectedItems(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FileDialogSelectedItems SelectedItems = {read=_scw_Get_SelectedItems};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DialogType() { Activex::TOleEnum r; HRESULT hr = Get_DialogType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DialogType = {read=_scw_Get_DialogType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Item() { WideString r; HRESULT hr = Get_Item(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Item = {read=_scw_Get_Item};
};

__dispinterface FileDialogDisp;
typedef System::DelphiInterface<FileDialogDisp> _di_FileDialogDisp;
__dispinterface INTERFACE_UUID("{000C0362-0000-0000-C000-000000000046}") FileDialogDisp  : public IDispatch 
{
	
};

__interface SignatureSet;
typedef System::DelphiInterface<SignatureSet> _di_SignatureSet;
__interface Signature;
typedef System::DelphiInterface<Signature> _di_Signature;
__interface INTERFACE_UUID("{000C0410-0000-0000-C000-000000000046}") SignatureSet  : public _IMsoDispObj 
{
	
public:
	_di_Signature operator[](int iSig) { return Item[iSig]; }
	
public:
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get_Item(int iSig, _di_Signature &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Add(_di_Signature &Add_result) = 0 ;
	virtual HRESULT __safecall Commit(void) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Signature _scw_Get_Item(int iSig) { _di_Signature r; HRESULT hr = Get_Item(iSig, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Signature Item[int iSig] = {read=_scw_Get_Item/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
};

__dispinterface SignatureSetDisp;
typedef System::DelphiInterface<SignatureSetDisp> _di_SignatureSetDisp;
__dispinterface INTERFACE_UUID("{000C0410-0000-0000-C000-000000000046}") SignatureSetDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C0411-0000-0000-C000-000000000046}") Signature  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Signer(WideString &Get_Signer_result) = 0 ;
	virtual HRESULT __safecall Get_Issuer(WideString &Get_Issuer_result) = 0 ;
	virtual HRESULT __safecall Get_ExpireDate(OleVariant &Get_ExpireDate_result) = 0 ;
	virtual HRESULT __safecall Get_IsValid(Word &Get_IsValid_result) = 0 ;
	virtual HRESULT __safecall Get_AttachCertificate(Word &Get_AttachCertificate_result) = 0 ;
	virtual HRESULT __safecall Set_AttachCertificate(Word pfAttach) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_IsCertificateExpired(Word &Get_IsCertificateExpired_result) = 0 ;
	virtual HRESULT __safecall Get_IsCertificateRevoked(Word &Get_IsCertificateRevoked_result) = 0 ;
	virtual HRESULT __safecall Get_SignDate(OleVariant &Get_SignDate_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Signer() { WideString r; HRESULT hr = Get_Signer(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Signer = {read=_scw_Get_Signer};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Issuer() { WideString r; HRESULT hr = Get_Issuer(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Issuer = {read=_scw_Get_Issuer};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_ExpireDate() { OleVariant r; HRESULT hr = Get_ExpireDate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant ExpireDate = {read=_scw_Get_ExpireDate};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsValid() { Word r; HRESULT hr = Get_IsValid(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsValid = {read=_scw_Get_IsValid};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_AttachCertificate() { Word r; HRESULT hr = Get_AttachCertificate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word AttachCertificate = {read=_scw_Get_AttachCertificate};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsCertificateExpired() { Word r; HRESULT hr = Get_IsCertificateExpired(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsCertificateExpired = {read=_scw_Get_IsCertificateExpired};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsCertificateRevoked() { Word r; HRESULT hr = Get_IsCertificateRevoked(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsCertificateRevoked = {read=_scw_Get_IsCertificateRevoked};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_SignDate() { OleVariant r; HRESULT hr = Get_SignDate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant SignDate = {read=_scw_Get_SignDate};
};

__dispinterface SignatureDisp;
typedef System::DelphiInterface<SignatureDisp> _di_SignatureDisp;
__dispinterface INTERFACE_UUID("{000C0411-0000-0000-C000-000000000046}") SignatureDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000672AC-0000-0000-C000-000000000046}") IMsoEnvelopeVB  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Introduction(WideString &Get_Introduction_result) = 0 ;
	virtual HRESULT __safecall Set_Introduction(const WideString pbstrIntro) = 0 ;
	virtual HRESULT __safecall Get_Item(_di_IDispatch &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_CommandBars(_di_IDispatch &Get_CommandBars_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Introduction() { WideString r; HRESULT hr = Get_Introduction(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Introduction = {read=_scw_Get_Introduction, write=Set_Introduction};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Item() { _di_IDispatch r; HRESULT hr = Get_Item(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Item = {read=_scw_Get_Item};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_CommandBars() { _di_IDispatch r; HRESULT hr = Get_CommandBars(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch CommandBars = {read=_scw_Get_CommandBars};
};

__dispinterface IMsoEnvelopeVBDisp;
typedef System::DelphiInterface<IMsoEnvelopeVBDisp> _di_IMsoEnvelopeVBDisp;
__dispinterface INTERFACE_UUID("{000672AC-0000-0000-C000-000000000046}") IMsoEnvelopeVBDisp  : public IDispatch 
{
	
};

__dispinterface IMsoEnvelopeVBEvents;
typedef System::DelphiInterface<IMsoEnvelopeVBEvents> _di_IMsoEnvelopeVBEvents;
__dispinterface INTERFACE_UUID("{000672AD-0000-0000-C000-000000000046}") IMsoEnvelopeVBEvents  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C036C-0000-0000-C000-000000000046}") FileTypes  : public _IMsoDispObj 
{
	
public:
	Activex::TOleEnum operator[](int Index) { return Item[Index]; }
	
public:
	virtual HRESULT __safecall Get_Item(int Index, Activex::TOleEnum &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Add(Activex::TOleEnum FileType) = 0 ;
	virtual HRESULT __safecall Remove(int Index) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Item(int Index) { Activex::TOleEnum r; HRESULT hr = Get_Item(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Item[int Index] = {read=_scw_Get_Item/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface FileTypesDisp;
typedef System::DelphiInterface<FileTypesDisp> _di_FileTypesDisp;
__dispinterface INTERFACE_UUID("{000C036C-0000-0000-C000-000000000046}") FileTypesDisp  : public IDispatch 
{
	
};

__interface ScopeFolder;
typedef System::DelphiInterface<ScopeFolder> _di_ScopeFolder;
__interface INTERFACE_UUID("{000C036A-0000-0000-C000-000000000046}") SearchFolders  : public _IMsoDispObj 
{
	
public:
	_di_ScopeFolder operator[](int Index) { return Item[Index]; }
	
public:
	virtual HRESULT __safecall Get_Item(int Index, _di_ScopeFolder &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Add(const _di_ScopeFolder ScopeFolder) = 0 ;
	virtual HRESULT __safecall Remove(int Index) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ScopeFolder _scw_Get_Item(int Index) { _di_ScopeFolder r; HRESULT hr = Get_Item(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ScopeFolder Item[int Index] = {read=_scw_Get_Item/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface SearchFoldersDisp;
typedef System::DelphiInterface<SearchFoldersDisp> _di_SearchFoldersDisp;
__dispinterface INTERFACE_UUID("{000C036A-0000-0000-C000-000000000046}") SearchFoldersDisp  : public IDispatch 
{
	
};

__interface ScopeFolders;
typedef System::DelphiInterface<ScopeFolders> _di_ScopeFolders;
__interface INTERFACE_UUID("{000C0369-0000-0000-C000-000000000046}") ScopeFolders  : public _IMsoDispObj 
{
	
public:
	_di_ScopeFolder operator[](int Index) { return Item[Index]; }
	
public:
	virtual HRESULT __safecall Get_Item(int Index, _di_ScopeFolder &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ScopeFolder _scw_Get_Item(int Index) { _di_ScopeFolder r; HRESULT hr = Get_Item(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ScopeFolder Item[int Index] = {read=_scw_Get_Item/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface ScopeFoldersDisp;
typedef System::DelphiInterface<ScopeFoldersDisp> _di_ScopeFoldersDisp;
__dispinterface INTERFACE_UUID("{000C0369-0000-0000-C000-000000000046}") ScopeFoldersDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C0368-0000-0000-C000-000000000046}") ScopeFolder  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Path(WideString &Get_Path_result) = 0 ;
	virtual HRESULT __safecall Get_ScopeFolders(_di_ScopeFolders &Get_ScopeFolders_result) = 0 ;
	virtual HRESULT __safecall AddToSearchFolders(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Path() { WideString r; HRESULT hr = Get_Path(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Path = {read=_scw_Get_Path};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ScopeFolders _scw_Get_ScopeFolders() { _di_ScopeFolders r; HRESULT hr = Get_ScopeFolders(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ScopeFolders ScopeFolders = {read=_scw_Get_ScopeFolders};
};

__dispinterface ScopeFolderDisp;
typedef System::DelphiInterface<ScopeFolderDisp> _di_ScopeFolderDisp;
__dispinterface INTERFACE_UUID("{000C0368-0000-0000-C000-000000000046}") ScopeFolderDisp  : public IDispatch 
{
	
};

__interface SearchScope;
typedef System::DelphiInterface<SearchScope> _di_SearchScope;
__interface INTERFACE_UUID("{000C0367-0000-0000-C000-000000000046}") SearchScope  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_ScopeFolder(_di_ScopeFolder &Get_ScopeFolder_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ScopeFolder _scw_Get_ScopeFolder() { _di_ScopeFolder r; HRESULT hr = Get_ScopeFolder(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ScopeFolder ScopeFolder = {read=_scw_Get_ScopeFolder};
};

__dispinterface SearchScopeDisp;
typedef System::DelphiInterface<SearchScopeDisp> _di_SearchScopeDisp;
__dispinterface INTERFACE_UUID("{000C0367-0000-0000-C000-000000000046}") SearchScopeDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C0366-0000-0000-C000-000000000046}") SearchScopes  : public _IMsoDispObj 
{
	
public:
	_di_SearchScope operator[](int Index) { return Item[Index]; }
	
public:
	virtual HRESULT __safecall Get_Item(int Index, _di_SearchScope &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_SearchScope _scw_Get_Item(int Index) { _di_SearchScope r; HRESULT hr = Get_Item(Index, r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_SearchScope Item[int Index] = {read=_scw_Get_Item/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
};

__dispinterface SearchScopesDisp;
typedef System::DelphiInterface<SearchScopesDisp> _di_SearchScopesDisp;
__dispinterface INTERFACE_UUID("{000C0366-0000-0000-C000-000000000046}") SearchScopesDisp  : public IDispatch 
{
	
};

__interface DiagramNodes;
typedef System::DelphiInterface<DiagramNodes> _di_DiagramNodes;
__interface INTERFACE_UUID("{000C036D-0000-0000-C000-000000000046}") IMsoDiagram  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Nodes(_di_DiagramNodes &Get_Nodes_result) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_AutoLayout(Activex::TOleEnum &Get_AutoLayout_result) = 0 ;
	virtual HRESULT __safecall Set_AutoLayout(Activex::TOleEnum AutoLayout) = 0 ;
	virtual HRESULT __safecall Get_Reverse(Activex::TOleEnum &Get_Reverse_result) = 0 ;
	virtual HRESULT __safecall Set_Reverse(Activex::TOleEnum Reverse) = 0 ;
	virtual HRESULT __safecall Get_AutoFormat(Activex::TOleEnum &Get_AutoFormat_result) = 0 ;
	virtual HRESULT __safecall Set_AutoFormat(Activex::TOleEnum AutoFormat) = 0 ;
	virtual HRESULT __safecall Convert(Activex::TOleEnum Type_) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_DiagramNodes _scw_Get_Nodes() { _di_DiagramNodes r; HRESULT hr = Get_Nodes(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_DiagramNodes Nodes = {read=_scw_Get_Nodes};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_AutoLayout() { Activex::TOleEnum r; HRESULT hr = Get_AutoLayout(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum AutoLayout = {read=_scw_Get_AutoLayout, write=Set_AutoLayout};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Reverse() { Activex::TOleEnum r; HRESULT hr = Get_Reverse(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Reverse = {read=_scw_Get_Reverse, write=Set_Reverse};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_AutoFormat() { Activex::TOleEnum r; HRESULT hr = Get_AutoFormat(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum AutoFormat = {read=_scw_Get_AutoFormat, write=Set_AutoFormat};
};

__dispinterface IMsoDiagramDisp;
typedef System::DelphiInterface<IMsoDiagramDisp> _di_IMsoDiagramDisp;
__dispinterface INTERFACE_UUID("{000C036D-0000-0000-C000-000000000046}") IMsoDiagramDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C036E-0000-0000-C000-000000000046}") DiagramNodes  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_DiagramNode &Item_result) = 0 ;
	virtual HRESULT __safecall SelectAll(void) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface DiagramNodesDisp;
typedef System::DelphiInterface<DiagramNodesDisp> _di_DiagramNodesDisp;
__dispinterface INTERFACE_UUID("{000C036E-0000-0000-C000-000000000046}") DiagramNodesDisp  : public IDispatch 
{
	
};

__interface DiagramNodeChildren;
typedef System::DelphiInterface<DiagramNodeChildren> _di_DiagramNodeChildren;
__interface INTERFACE_UUID("{000C036F-0000-0000-C000-000000000046}") DiagramNodeChildren  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_DiagramNode &Item_result) = 0 ;
	virtual HRESULT __safecall AddNode(const OleVariant Index, Activex::TOleEnum NodeType, _di_DiagramNode &AddNode_result) = 0 ;
	virtual HRESULT __safecall SelectAll(void) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get_FirstChild(_di_DiagramNode &Get_FirstChild_result) = 0 ;
	virtual HRESULT __safecall Get_LastChild(_di_DiagramNode &Get_LastChild_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_DiagramNode _scw_Get_FirstChild() { _di_DiagramNode r; HRESULT hr = Get_FirstChild(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_DiagramNode FirstChild = {read=_scw_Get_FirstChild};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_DiagramNode _scw_Get_LastChild() { _di_DiagramNode r; HRESULT hr = Get_LastChild(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_DiagramNode LastChild = {read=_scw_Get_LastChild};
};

__dispinterface DiagramNodeChildrenDisp;
typedef System::DelphiInterface<DiagramNodeChildrenDisp> _di_DiagramNodeChildrenDisp;
__dispinterface INTERFACE_UUID("{000C036F-0000-0000-C000-000000000046}") DiagramNodeChildrenDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C0370-0000-0000-C000-000000000046}") DiagramNode  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall AddNode(Activex::TOleEnum Pos, Activex::TOleEnum NodeType, _di_DiagramNode &AddNode_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall MoveNode(const _di_DiagramNode TargetNode, Activex::TOleEnum Pos) = 0 ;
	virtual HRESULT __safecall ReplaceNode(const _di_DiagramNode TargetNode) = 0 ;
	virtual HRESULT __safecall SwapNode(const _di_DiagramNode TargetNode, Word SwapChildren) = 0 ;
	virtual HRESULT __safecall CloneNode(Word CopyChildren, const _di_DiagramNode TargetNode, Activex::TOleEnum Pos, _di_DiagramNode &CloneNode_result) = 0 ;
	virtual HRESULT __safecall TransferChildren(const _di_DiagramNode ReceivingNode) = 0 ;
	virtual HRESULT __safecall NextNode(_di_DiagramNode &NextNode_result) = 0 ;
	virtual HRESULT __safecall PrevNode(_di_DiagramNode &PrevNode_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Children(_di_DiagramNodeChildren &Get_Children_result) = 0 ;
	virtual HRESULT __safecall Get_Shape(_di_Shape &Get_Shape_result) = 0 ;
	virtual HRESULT __safecall Get_Root(_di_DiagramNode &Get_Root_result) = 0 ;
	virtual HRESULT __safecall Get_Diagram(_di_IMsoDiagram &Get_Diagram_result) = 0 ;
	virtual HRESULT __safecall Get_Layout(Activex::TOleEnum &Get_Layout_result) = 0 ;
	virtual HRESULT __safecall Set_Layout(Activex::TOleEnum Type_) = 0 ;
	virtual HRESULT __safecall Get_TextShape(_di_Shape &Get_TextShape_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_DiagramNodeChildren _scw_Get_Children() { _di_DiagramNodeChildren r; HRESULT hr = Get_Children(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_DiagramNodeChildren Children = {read=_scw_Get_Children};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Shape _scw_Get_Shape() { _di_Shape r; HRESULT hr = Get_Shape(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Shape Shape = {read=_scw_Get_Shape};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_DiagramNode _scw_Get_Root() { _di_DiagramNode r; HRESULT hr = Get_Root(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_DiagramNode Root = {read=_scw_Get_Root};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IMsoDiagram _scw_Get_Diagram() { _di_IMsoDiagram r; HRESULT hr = Get_Diagram(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IMsoDiagram Diagram = {read=_scw_Get_Diagram};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Layout() { Activex::TOleEnum r; HRESULT hr = Get_Layout(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Layout = {read=_scw_Get_Layout, write=Set_Layout};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Shape _scw_Get_TextShape() { _di_Shape r; HRESULT hr = Get_TextShape(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Shape TextShape = {read=_scw_Get_TextShape};
};

__dispinterface DiagramNodeDisp;
typedef System::DelphiInterface<DiagramNodeDisp> _di_DiagramNodeDisp;
__dispinterface INTERFACE_UUID("{000C0370-0000-0000-C000-000000000046}") DiagramNodeDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000C0371-0000-0000-C000-000000000046}") CanvasShapes  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_Shape &Item_result) = 0 ;
	virtual HRESULT __safecall Get__NewEnum(System::_di_IInterface &Get__NewEnum_result) = 0 ;
	virtual HRESULT __safecall AddCallout(Activex::TOleEnum Type_, float Left, float Top, float Width, float Height, _di_Shape &AddCallout_result) = 0 ;
	virtual HRESULT __safecall AddConnector(Activex::TOleEnum Type_, float BeginX, float BeginY, float EndX, float EndY, _di_Shape &AddConnector_result) = 0 ;
	virtual HRESULT __safecall AddCurve(const OleVariant SafeArrayOfPoints, _di_Shape &AddCurve_result) = 0 ;
	virtual HRESULT __safecall AddLabel(Activex::TOleEnum Orientation, float Left, float Top, float Width, float Height, _di_Shape &AddLabel_result) = 0 ;
	virtual HRESULT __safecall AddLine(float BeginX, float BeginY, float EndX, float EndY, _di_Shape &AddLine_result) = 0 ;
	virtual HRESULT __safecall AddPicture(const WideString FileName, Activex::TOleEnum LinkToFile, Activex::TOleEnum SaveWithDocument, float Left, float Top, float Width, float Height, _di_Shape &AddPicture_result) = 0 ;
	virtual HRESULT __safecall AddPolyline(const OleVariant SafeArrayOfPoints, _di_Shape &AddPolyline_result) = 0 ;
	virtual HRESULT __safecall AddShape(Activex::TOleEnum Type_, float Left, float Top, float Width, float Height, _di_Shape &AddShape_result) = 0 ;
	virtual HRESULT __safecall AddTextEffect(Activex::TOleEnum PresetTextEffect, const WideString Text, const WideString FontName, float FontSize, Activex::TOleEnum FontBold, Activex::TOleEnum FontItalic, float Left, float Top, _di_Shape &AddTextEffect_result) = 0 ;
	virtual HRESULT __safecall AddTextbox(Activex::TOleEnum Orientation, float Left, float Top, float Width, float Height, _di_Shape &AddTextbox_result) = 0 ;
	virtual HRESULT __safecall BuildFreeform(Activex::TOleEnum EditingType, float X1, float Y1, _di_FreeformBuilder &BuildFreeform_result) = 0 ;
	virtual HRESULT __safecall Range(const OleVariant Index, _di_ShapeRange &Range_result) = 0 ;
	virtual HRESULT __safecall SelectAll(void) = 0 ;
	virtual HRESULT __safecall Get_Background(_di_Shape &Get_Background_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get__NewEnum() { System::_di_IInterface r; HRESULT hr = Get__NewEnum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface _NewEnum = {read=_scw_Get__NewEnum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Shape _scw_Get_Background() { _di_Shape r; HRESULT hr = Get_Background(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Shape Background = {read=_scw_Get_Background};
};

__dispinterface CanvasShapesDisp;
typedef System::DelphiInterface<CanvasShapesDisp> _di_CanvasShapesDisp;
__dispinterface INTERFACE_UUID("{000C0371-0000-0000-C000-000000000046}") CanvasShapesDisp  : public IDispatch 
{
	
};

__interface OfficeDataSourceObject;
typedef System::DelphiInterface<OfficeDataSourceObject> _di_OfficeDataSourceObject;
__interface INTERFACE_UUID("{000C1530-0000-0000-C000-000000000046}") OfficeDataSourceObject  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_ConnectString(WideString &Get_ConnectString_result) = 0 ;
	virtual HRESULT __safecall Set_ConnectString(const WideString pbstrConnect) = 0 ;
	virtual HRESULT __safecall Get_Table(WideString &Get_Table_result) = 0 ;
	virtual HRESULT __safecall Set_Table(const WideString pbstrTable) = 0 ;
	virtual HRESULT __safecall Get_DataSource(WideString &Get_DataSource_result) = 0 ;
	virtual HRESULT __safecall Set_DataSource(const WideString pbstrSrc) = 0 ;
	virtual HRESULT __safecall Get_Columns(_di_IDispatch &Get_Columns_result) = 0 ;
	virtual HRESULT __safecall Get_RowCount(int &Get_RowCount_result) = 0 ;
	virtual HRESULT __safecall Get_Filters(_di_IDispatch &Get_Filters_result) = 0 ;
	virtual HRESULT __safecall Move(Activex::TOleEnum MsoMoveRow, int RowNbr, int &Move_result) = 0 ;
	virtual HRESULT __safecall Open(const WideString bstrSrc, const WideString bstrConnect, const WideString bstrTable, int fOpenExclusive, int fNeverPrompt) = 0 ;
	virtual HRESULT __safecall SetSortOrder(const WideString SortField1, Word SortAscending1, const WideString SortField2, Word SortAscending2, const WideString SortField3, Word SortAscending3) = 0 ;
	virtual HRESULT __safecall ApplyFilter(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConnectString() { WideString r; HRESULT hr = Get_ConnectString(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConnectString = {read=_scw_Get_ConnectString, write=Set_ConnectString};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Table() { WideString r; HRESULT hr = Get_Table(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Table = {read=_scw_Get_Table, write=Set_Table};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_DataSource() { WideString r; HRESULT hr = Get_DataSource(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString DataSource = {read=_scw_Get_DataSource, write=Set_DataSource};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Columns() { _di_IDispatch r; HRESULT hr = Get_Columns(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Columns = {read=_scw_Get_Columns};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_RowCount() { int r; HRESULT hr = Get_RowCount(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int RowCount = {read=_scw_Get_RowCount};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Filters() { _di_IDispatch r; HRESULT hr = Get_Filters(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Filters = {read=_scw_Get_Filters};
};

__dispinterface OfficeDataSourceObjectDisp;
typedef System::DelphiInterface<OfficeDataSourceObjectDisp> _di_OfficeDataSourceObjectDisp;
__dispinterface INTERFACE_UUID("{000C1530-0000-0000-C000-000000000046}") OfficeDataSourceObjectDisp  : public IDispatch 
{
	
};

__interface ODSOColumn;
typedef System::DelphiInterface<ODSOColumn> _di_ODSOColumn;
__interface INTERFACE_UUID("{000C1531-0000-0000-C000-000000000046}") ODSOColumn  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Index(int &Get_Index_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Value(WideString &Get_Value_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Index() { int r; HRESULT hr = Get_Index(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Index = {read=_scw_Get_Index};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Value() { WideString r; HRESULT hr = Get_Value(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Value = {read=_scw_Get_Value};
};

__dispinterface ODSOColumnDisp;
typedef System::DelphiInterface<ODSOColumnDisp> _di_ODSOColumnDisp;
__dispinterface INTERFACE_UUID("{000C1531-0000-0000-C000-000000000046}") ODSOColumnDisp  : public IDispatch 
{
	
};

__interface ODSOColumns;
typedef System::DelphiInterface<ODSOColumns> _di_ODSOColumns;
__interface INTERFACE_UUID("{000C1532-0000-0000-C000-000000000046}") ODSOColumns  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant varIndex, _di_IDispatch &Item_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
};

__dispinterface ODSOColumnsDisp;
typedef System::DelphiInterface<ODSOColumnsDisp> _di_ODSOColumnsDisp;
__dispinterface INTERFACE_UUID("{000C1532-0000-0000-C000-000000000046}") ODSOColumnsDisp  : public IDispatch 
{
	
};

__interface ODSOFilter;
typedef System::DelphiInterface<ODSOFilter> _di_ODSOFilter;
__interface INTERFACE_UUID("{000C1533-0000-0000-C000-000000000046}") ODSOFilter  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Index(int &Get_Index_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Column(WideString &Get_Column_result) = 0 ;
	virtual HRESULT __safecall Set_Column(const WideString pbstrCol) = 0 ;
	virtual HRESULT __safecall Get_Comparison(Activex::TOleEnum &Get_Comparison_result) = 0 ;
	virtual HRESULT __safecall Set_Comparison(Activex::TOleEnum pComparison) = 0 ;
	virtual HRESULT __safecall Get_CompareTo(WideString &Get_CompareTo_result) = 0 ;
	virtual HRESULT __safecall Set_CompareTo(const WideString pbstrCompareTo) = 0 ;
	virtual HRESULT __safecall Get_Conjunction(Activex::TOleEnum &Get_Conjunction_result) = 0 ;
	virtual HRESULT __safecall Set_Conjunction(Activex::TOleEnum pConjunction) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Index() { int r; HRESULT hr = Get_Index(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Index = {read=_scw_Get_Index};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Column() { WideString r; HRESULT hr = Get_Column(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Column = {read=_scw_Get_Column, write=Set_Column};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Comparison() { Activex::TOleEnum r; HRESULT hr = Get_Comparison(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Comparison = {read=_scw_Get_Comparison, write=Set_Comparison};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CompareTo() { WideString r; HRESULT hr = Get_CompareTo(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CompareTo = {read=_scw_Get_CompareTo, write=Set_CompareTo};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Conjunction() { Activex::TOleEnum r; HRESULT hr = Get_Conjunction(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Conjunction = {read=_scw_Get_Conjunction, write=Set_Conjunction};
};

__dispinterface ODSOFilterDisp;
typedef System::DelphiInterface<ODSOFilterDisp> _di_ODSOFilterDisp;
__dispinterface INTERFACE_UUID("{000C1533-0000-0000-C000-000000000046}") ODSOFilterDisp  : public IDispatch 
{
	
};

__interface ODSOFilters;
typedef System::DelphiInterface<ODSOFilters> _di_ODSOFilters;
__interface INTERFACE_UUID("{000C1534-0000-0000-C000-000000000046}") ODSOFilters  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Item(int Index, _di_IDispatch &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const WideString Column, Activex::TOleEnum Comparison, Activex::TOleEnum Conjunction, const WideString bstrCompareTo, Word DeferUpdate) = 0 ;
	virtual HRESULT __safecall Delete(int Index, Word DeferUpdate) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
};

__dispinterface ODSOFiltersDisp;
typedef System::DelphiInterface<ODSOFiltersDisp> _di_ODSOFiltersDisp;
__dispinterface INTERFACE_UUID("{000C1534-0000-0000-C000-000000000046}") ODSOFiltersDisp  : public IDispatch 
{
	
};

__interface NewFile;
typedef System::DelphiInterface<NewFile> _di_NewFile;
__interface INTERFACE_UUID("{000C0936-0000-0000-C000-000000000046}") NewFile  : public _IMsoDispObj 
{
	
public:
	virtual HRESULT __safecall Add(const WideString FileName, const OleVariant Section, const OleVariant DisplayName, const OleVariant Action, Word &Add_result) = 0 ;
	virtual HRESULT __safecall Remove(const WideString FileName, const OleVariant Section, const OleVariant DisplayName, const OleVariant Action, Word &Remove_result) = 0 ;
};

__dispinterface NewFileDisp;
typedef System::DelphiInterface<NewFileDisp> _di_NewFileDisp;
__dispinterface INTERFACE_UUID("{000C0936-0000-0000-C000-000000000046}") NewFileDisp  : public IDispatch 
{
	
};

__interface WebComponent;
typedef System::DelphiInterface<WebComponent> _di_WebComponent;
__interface INTERFACE_UUID("{000CD100-0000-0000-C000-000000000046}") WebComponent  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Shape(_di_IDispatch &Get_Shape_result) = 0 ;
	virtual HRESULT __safecall Get_URL(WideString &Get_URL_result) = 0 ;
	virtual HRESULT __safecall Set_URL(const WideString RetValue) = 0 ;
	virtual HRESULT __safecall Get_HTML(WideString &Get_HTML_result) = 0 ;
	virtual HRESULT __safecall Set_HTML(const WideString RetValue) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString RetValue) = 0 ;
	virtual HRESULT __safecall Get_Width(int &Get_Width_result) = 0 ;
	virtual HRESULT __safecall Set_Width(int RetValue) = 0 ;
	virtual HRESULT __safecall Get_Height(int &Get_Height_result) = 0 ;
	virtual HRESULT __safecall Set_Height(int RetValue) = 0 ;
	virtual HRESULT __safecall SetPlaceHolderGraphic(const WideString PlaceHolderGraphic) = 0 ;
	virtual HRESULT __safecall Commit(void) = 0 ;
	virtual HRESULT __safecall Revert(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Shape() { _di_IDispatch r; HRESULT hr = Get_Shape(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Shape = {read=_scw_Get_Shape};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_URL() { WideString r; HRESULT hr = Get_URL(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString URL = {read=_scw_Get_URL, write=Set_URL};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HTML() { WideString r; HRESULT hr = Get_HTML(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HTML = {read=_scw_Get_HTML, write=Set_HTML};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Width() { int r; HRESULT hr = Get_Width(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Width = {read=_scw_Get_Width, write=Set_Width};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Height() { int r; HRESULT hr = Get_Height(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Height = {read=_scw_Get_Height, write=Set_Height};
};

__dispinterface WebComponentDisp;
typedef System::DelphiInterface<WebComponentDisp> _di_WebComponentDisp;
__dispinterface INTERFACE_UUID("{000CD100-0000-0000-C000-000000000046}") WebComponentDisp  : public IDispatch 
{
	
};

__interface WebComponentWindowExternal;
typedef System::DelphiInterface<WebComponentWindowExternal> _di_WebComponentWindowExternal;
__interface INTERFACE_UUID("{000CD101-0000-0000-C000-000000000046}") WebComponentWindowExternal  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_InterfaceVersion(int &Get_InterfaceVersion_result) = 0 ;
	virtual HRESULT __safecall Get_ApplicationName(WideString &Get_ApplicationName_result) = 0 ;
	virtual HRESULT __safecall Get_ApplicationVersion(int &Get_ApplicationVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Application(_di_IDispatch &Get_Application_result) = 0 ;
	virtual HRESULT __safecall CloseWindow(void) = 0 ;
	virtual HRESULT __safecall Get_WebComponent(_di_WebComponent &Get_WebComponent_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_InterfaceVersion() { int r; HRESULT hr = Get_InterfaceVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int InterfaceVersion = {read=_scw_Get_InterfaceVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ApplicationName() { WideString r; HRESULT hr = Get_ApplicationName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Application_Name = {read=_scw_Get_ApplicationName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ApplicationVersion() { int r; HRESULT hr = Get_ApplicationVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Application_Version = {read=_scw_Get_ApplicationVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Application() { _di_IDispatch r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Application_ = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_WebComponent _scw_Get_WebComponent() { _di_WebComponent r; HRESULT hr = Get_WebComponent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_WebComponent WebComponent = {read=_scw_Get_WebComponent};
};

__dispinterface WebComponentWindowExternalDisp;
typedef System::DelphiInterface<WebComponentWindowExternalDisp> _di_WebComponentWindowExternalDisp;
__dispinterface INTERFACE_UUID("{000CD101-0000-0000-C000-000000000046}") WebComponentWindowExternalDisp  : public IDispatch 
{
	
};

__interface WebComponentFormat;
typedef System::DelphiInterface<WebComponentFormat> _di_WebComponentFormat;
__interface INTERFACE_UUID("{000CD102-0000-0000-C000-000000000046}") WebComponentFormat  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di_IDispatch &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_URL(WideString &Get_URL_result) = 0 ;
	virtual HRESULT __safecall Set_URL(const WideString RetValue) = 0 ;
	virtual HRESULT __safecall Get_HTML(WideString &Get_HTML_result) = 0 ;
	virtual HRESULT __safecall Set_HTML(const WideString RetValue) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString RetValue) = 0 ;
	virtual HRESULT __safecall Get_Width(int &Get_Width_result) = 0 ;
	virtual HRESULT __safecall Set_Width(int RetValue) = 0 ;
	virtual HRESULT __safecall Get_Height(int &Get_Height_result) = 0 ;
	virtual HRESULT __safecall Set_Height(int RetValue) = 0 ;
	virtual HRESULT __safecall Get_PreviewGraphic(WideString &Get_PreviewGraphic_result) = 0 ;
	virtual HRESULT __safecall Set_PreviewGraphic(const WideString retval) = 0 ;
	virtual HRESULT __safecall LaunchPropertiesWindow(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Application() { _di_IDispatch r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Application_ = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_URL() { WideString r; HRESULT hr = Get_URL(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString URL = {read=_scw_Get_URL, write=Set_URL};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HTML() { WideString r; HRESULT hr = Get_HTML(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HTML = {read=_scw_Get_HTML, write=Set_HTML};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Width() { int r; HRESULT hr = Get_Width(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Width = {read=_scw_Get_Width, write=Set_Width};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Height() { int r; HRESULT hr = Get_Height(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Height = {read=_scw_Get_Height, write=Set_Height};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_PreviewGraphic() { WideString r; HRESULT hr = Get_PreviewGraphic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString PreviewGraphic = {read=_scw_Get_PreviewGraphic, write=Set_PreviewGraphic};
};

__dispinterface WebComponentFormatDisp;
typedef System::DelphiInterface<WebComponentFormatDisp> _di_WebComponentFormatDisp;
__dispinterface INTERFACE_UUID("{000CD102-0000-0000-C000-000000000046}") WebComponentFormatDisp  : public IDispatch 
{
	
};

__interface ILicWizExternal;
typedef System::DelphiInterface<ILicWizExternal> _di_ILicWizExternal;
__interface INTERFACE_UUID("{4CAC6328-B9B0-11D3-8D59-0050048384E3}") ILicWizExternal  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall PrintHtmlDocument(const System::_di_IInterface punkHtmlDoc) = 0 ;
	virtual HRESULT __safecall InvokeDateTimeApplet(void) = 0 ;
	virtual HRESULT __safecall FormatDate(System::TDateTime date, const WideString pFormat, WideString &FormatDate_result) = 0 ;
	virtual HRESULT __safecall ShowHelp(OleVariant &pvarId) = 0 ;
	virtual HRESULT __safecall Terminate(void) = 0 ;
	virtual HRESULT __safecall DisableVORWReminder(int BPC) = 0 ;
	virtual HRESULT __safecall SaveReceipt(const WideString bstrReceipt, WideString &SaveReceipt_result) = 0 ;
	virtual HRESULT __safecall OpenInDefaultBrowser(const WideString bstrUrl) = 0 ;
	virtual HRESULT __safecall MsoAlert(const WideString bstrText, const WideString bstrButtons, const WideString bstrIcon, int &MsoAlert_result) = 0 ;
	virtual HRESULT __safecall DepositPidKey(const WideString bstrKey, int fMORW, int &DepositPidKey_result) = 0 ;
	virtual HRESULT __safecall WriteLog(const WideString bstrMessage) = 0 ;
	virtual HRESULT __safecall ResignDpc(const WideString bstrProductCode) = 0 ;
	virtual HRESULT __safecall ResetPID(void) = 0 ;
	virtual HRESULT __safecall SetDialogSize(int dx, int dy) = 0 ;
	virtual HRESULT __safecall VerifyClock(int lMode, int &VerifyClock_result) = 0 ;
	virtual HRESULT __safecall SortSelectOptions(const _di_IDispatch pdispSelect) = 0 ;
	virtual HRESULT __safecall InternetDisconnect(void) = 0 ;
	virtual HRESULT __safecall GetConnectedState(int &GetConnectedState_result) = 0 ;
	virtual HRESULT __safecall Get_Context(int &Get_Context_result) = 0 ;
	virtual HRESULT __safecall Get_Validator(_di_IDispatch &Get_Validator_result) = 0 ;
	virtual HRESULT __safecall Get_LicAgent(_di_IDispatch &Get_LicAgent_result) = 0 ;
	virtual HRESULT __safecall Get_CountryInfo(WideString &Get_CountryInfo_result) = 0 ;
	virtual HRESULT __safecall Set_WizardVisible(int Param1) = 0 ;
	virtual HRESULT __safecall Set_WizardTitle(const WideString Param1) = 0 ;
	virtual HRESULT __safecall Get_AnimationEnabled(int &Get_AnimationEnabled_result) = 0 ;
	virtual HRESULT __safecall Set_CurrentHelpId(int Param1) = 0 ;
	virtual HRESULT __safecall Get_OfficeOnTheWebUrl(WideString &Get_OfficeOnTheWebUrl_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Context() { int r; HRESULT hr = Get_Context(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Context = {read=_scw_Get_Context};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Validator() { _di_IDispatch r; HRESULT hr = Get_Validator(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Validator = {read=_scw_Get_Validator};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_LicAgent() { _di_IDispatch r; HRESULT hr = Get_LicAgent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch LicAgent = {read=_scw_Get_LicAgent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CountryInfo() { WideString r; HRESULT hr = Get_CountryInfo(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CountryInfo = {read=_scw_Get_CountryInfo};
	__property int WizardVisible = {write=Set_WizardVisible};
	__property WideString WizardTitle = {write=Set_WizardTitle};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_AnimationEnabled() { int r; HRESULT hr = Get_AnimationEnabled(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int AnimationEnabled = {read=_scw_Get_AnimationEnabled};
	__property int CurrentHelpId = {write=Set_CurrentHelpId};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OfficeOnTheWebUrl() { WideString r; HRESULT hr = Get_OfficeOnTheWebUrl(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OfficeOnTheWebUrl = {read=_scw_Get_OfficeOnTheWebUrl};
};

__dispinterface ILicWizExternalDisp;
typedef System::DelphiInterface<ILicWizExternalDisp> _di_ILicWizExternalDisp;
__dispinterface INTERFACE_UUID("{4CAC6328-B9B0-11D3-8D59-0050048384E3}") ILicWizExternalDisp  : public IDispatch 
{
	
};

__interface ILicValidator;
typedef System::DelphiInterface<ILicValidator> _di_ILicValidator;
__interface INTERFACE_UUID("{919AA22C-B9AD-11D3-8D59-0050048384E3}") ILicValidator  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Products(OleVariant &Get_Products_result) = 0 ;
	virtual HRESULT __safecall Get_Selection(int &Get_Selection_result) = 0 ;
	virtual HRESULT __safecall Set_Selection(int piSel) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_Products() { OleVariant r; HRESULT hr = Get_Products(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant Products = {read=_scw_Get_Products};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Selection() { int r; HRESULT hr = Get_Selection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Selection = {read=_scw_Get_Selection, write=Set_Selection};
};

__dispinterface ILicValidatorDisp;
typedef System::DelphiInterface<ILicValidatorDisp> _di_ILicValidatorDisp;
__dispinterface INTERFACE_UUID("{919AA22C-B9AD-11D3-8D59-0050048384E3}") ILicValidatorDisp  : public IDispatch 
{
	
};

__interface ILicAgent;
typedef System::DelphiInterface<ILicAgent> _di_ILicAgent;
__interface INTERFACE_UUID("{00194002-D9C3-11D3-8D59-0050048384E3}") ILicAgent  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Initialize(unsigned dwBPC, unsigned dwMode, const WideString bstrLicSource, unsigned &Initialize_result) = 0 ;
	virtual HRESULT __safecall GetFirstName(WideString &GetFirstName_result) = 0 ;
	virtual HRESULT __safecall SetFirstName(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetLastName(WideString &GetLastName_result) = 0 ;
	virtual HRESULT __safecall SetLastName(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetOrgName(WideString &GetOrgName_result) = 0 ;
	virtual HRESULT __safecall SetOrgName(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetEmail(WideString &GetEmail_result) = 0 ;
	virtual HRESULT __safecall SetEmail(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetPhone(WideString &GetPhone_result) = 0 ;
	virtual HRESULT __safecall SetPhone(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetAddress1(WideString &GetAddress1_result) = 0 ;
	virtual HRESULT __safecall SetAddress1(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetCity(WideString &GetCity_result) = 0 ;
	virtual HRESULT __safecall SetCity(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetState(WideString &GetState_result) = 0 ;
	virtual HRESULT __safecall SetState(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetCountryCode(WideString &GetCountryCode_result) = 0 ;
	virtual HRESULT __safecall SetCountryCode(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetCountryDesc(WideString &GetCountryDesc_result) = 0 ;
	virtual HRESULT __safecall SetCountryDesc(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetZip(WideString &GetZip_result) = 0 ;
	virtual HRESULT __safecall SetZip(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetIsoLanguage(unsigned &GetIsoLanguage_result) = 0 ;
	virtual HRESULT __safecall SetIsoLanguage(unsigned dwNewVal) = 0 ;
	virtual HRESULT __safecall GetMSUpdate(WideString &GetMSUpdate_result) = 0 ;
	virtual HRESULT __safecall SetMSUpdate(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetMSOffer(WideString &GetMSOffer_result) = 0 ;
	virtual HRESULT __safecall SetMSOffer(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetOtherOffer(WideString &GetOtherOffer_result) = 0 ;
	virtual HRESULT __safecall SetOtherOffer(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetAddress2(WideString &GetAddress2_result) = 0 ;
	virtual HRESULT __safecall SetAddress2(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall CheckSystemClock(unsigned &CheckSystemClock_result) = 0 ;
	virtual HRESULT __safecall GetExistingExpiryDate(System::TDateTime &GetExistingExpiryDate_result) = 0 ;
	virtual HRESULT __safecall GetNewExpiryDate(System::TDateTime &GetNewExpiryDate_result) = 0 ;
	virtual HRESULT __safecall GetBillingFirstName(WideString &GetBillingFirstName_result) = 0 ;
	virtual HRESULT __safecall SetBillingFirstName(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetBillingLastName(WideString &GetBillingLastName_result) = 0 ;
	virtual HRESULT __safecall SetBillingLastName(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetBillingPhone(WideString &GetBillingPhone_result) = 0 ;
	virtual HRESULT __safecall SetBillingPhone(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetBillingAddress1(WideString &GetBillingAddress1_result) = 0 ;
	virtual HRESULT __safecall SetBillingAddress1(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetBillingAddress2(WideString &GetBillingAddress2_result) = 0 ;
	virtual HRESULT __safecall SetBillingAddress2(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetBillingCity(WideString &GetBillingCity_result) = 0 ;
	virtual HRESULT __safecall SetBillingCity(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetBillingState(WideString &GetBillingState_result) = 0 ;
	virtual HRESULT __safecall SetBillingState(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetBillingCountryCode(WideString &GetBillingCountryCode_result) = 0 ;
	virtual HRESULT __safecall SetBillingCountryCode(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall GetBillingZip(WideString &GetBillingZip_result) = 0 ;
	virtual HRESULT __safecall SetBillingZip(const WideString bstrNewVal) = 0 ;
	virtual HRESULT __safecall SaveBillingInfo(int bSave, unsigned &SaveBillingInfo_result) = 0 ;
	virtual HRESULT __safecall IsCCRenewalCountry(const WideString bstrCountryCode, int &IsCCRenewalCountry_result) = 0 ;
	virtual HRESULT __safecall GetVATLabel(const WideString bstrCountryCode, WideString &GetVATLabel_result) = 0 ;
	virtual HRESULT __safecall GetCCRenewalExpiryDate(System::TDateTime &GetCCRenewalExpiryDate_result) = 0 ;
	virtual HRESULT __safecall SetVATNumber(const WideString bstrVATNumber) = 0 ;
	virtual HRESULT __safecall SetCreditCardType(const WideString bstrCCCode) = 0 ;
	virtual HRESULT __safecall SetCreditCardNumber(const WideString bstrCCNumber) = 0 ;
	virtual HRESULT __safecall SetCreditCardExpiryYear(unsigned dwCCYear) = 0 ;
	virtual HRESULT __safecall SetCreditCardExpiryMonth(unsigned dwCCMonth) = 0 ;
	virtual HRESULT __safecall GetCreditCardCount(unsigned &GetCreditCardCount_result) = 0 ;
	virtual HRESULT __safecall GetCreditCardCode(unsigned dwIndex, WideString &GetCreditCardCode_result) = 0 ;
	virtual HRESULT __safecall GetCreditCardName(unsigned dwIndex, WideString &GetCreditCardName_result) = 0 ;
	virtual HRESULT __safecall GetVATNumber(WideString &GetVATNumber_result) = 0 ;
	virtual HRESULT __safecall GetCreditCardType(WideString &GetCreditCardType_result) = 0 ;
	virtual HRESULT __safecall GetCreditCardNumber(WideString &GetCreditCardNumber_result) = 0 ;
	virtual HRESULT __safecall GetCreditCardExpiryYear(unsigned &GetCreditCardExpiryYear_result) = 0 ;
	virtual HRESULT __safecall GetCreditCardExpiryMonth(unsigned &GetCreditCardExpiryMonth_result) = 0 ;
	virtual HRESULT __safecall GetDisconnectOption(int &GetDisconnectOption_result) = 0 ;
	virtual HRESULT __safecall SetDisconnectOption(int bNewVal) = 0 ;
	virtual HRESULT __safecall AsyncProcessHandshakeRequest(int bReviseCustInfo) = 0 ;
	virtual HRESULT __safecall AsyncProcessNewLicenseRequest(void) = 0 ;
	virtual HRESULT __safecall AsyncProcessReissueLicenseRequest(void) = 0 ;
	virtual HRESULT __safecall AsyncProcessRetailRenewalLicenseRequest(void) = 0 ;
	virtual HRESULT __safecall AsyncProcessReviseCustInfoRequest(void) = 0 ;
	virtual HRESULT __safecall AsyncProcessCCRenewalPriceRequest(void) = 0 ;
	virtual HRESULT __safecall AsyncProcessCCRenewalLicenseRequest(void) = 0 ;
	virtual HRESULT __safecall GetAsyncProcessReturnCode(unsigned &GetAsyncProcessReturnCode_result) = 0 ;
	virtual HRESULT __safecall IsUpgradeAvailable(int &IsUpgradeAvailable_result) = 0 ;
	virtual HRESULT __safecall WantUpgrade(int bWantUpgrade) = 0 ;
	virtual HRESULT __safecall AsyncProcessDroppedLicenseRequest(void) = 0 ;
	virtual HRESULT __safecall GenerateInstallationId(WideString &GenerateInstallationId_result) = 0 ;
	virtual HRESULT __safecall DepositConfirmationId(const WideString bstrVal, unsigned &DepositConfirmationId_result) = 0 ;
	virtual HRESULT __safecall VerifyCheckDigits(const WideString bstrCIDIID, int &VerifyCheckDigits_result) = 0 ;
	virtual HRESULT __safecall GetCurrentExpiryDate(System::TDateTime &GetCurrentExpiryDate_result) = 0 ;
	virtual HRESULT __safecall CancelAsyncProcessRequest(int bIsLicenseRequest) = 0 ;
	virtual HRESULT __safecall GetCurrencyDescription(unsigned dwCurrencyIndex, WideString &GetCurrencyDescription_result) = 0 ;
	virtual HRESULT __safecall GetPriceItemCount(unsigned &GetPriceItemCount_result) = 0 ;
	virtual HRESULT __safecall GetPriceItemLabel(unsigned dwIndex, WideString &GetPriceItemLabel_result) = 0 ;
	virtual HRESULT __safecall GetPriceItemValue(unsigned dwCurrencyIndex, unsigned dwIndex, WideString &GetPriceItemValue_result) = 0 ;
	virtual HRESULT __safecall GetInvoiceText(WideString &GetInvoiceText_result) = 0 ;
	virtual HRESULT __safecall GetBackendErrorMsg(WideString &GetBackendErrorMsg_result) = 0 ;
	virtual HRESULT __safecall GetCurrencyOption(unsigned &GetCurrencyOption_result) = 0 ;
	virtual HRESULT __safecall SetCurrencyOption(unsigned dwCurrencyOption) = 0 ;
	virtual HRESULT __safecall GetEndOfLifeHtmlText(WideString &GetEndOfLifeHtmlText_result) = 0 ;
	virtual HRESULT __safecall DisplaySSLCert(unsigned &DisplaySSLCert_result) = 0 ;
};

__dispinterface ILicAgentDisp;
typedef System::DelphiInterface<ILicAgentDisp> _di_ILicAgentDisp;
__dispinterface INTERFACE_UUID("{00194002-D9C3-11D3-8D59-0050048384E3}") ILicAgentDisp  : public IDispatch 
{
	
};

__interface IMsoEServicesDialog;
typedef System::DelphiInterface<IMsoEServicesDialog> _di_IMsoEServicesDialog;
__interface INTERFACE_UUID("{000C0372-0000-0000-C000-000000000046}") IMsoEServicesDialog  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Close(Word ApplyWebComponentChanges) = 0 ;
	virtual HRESULT __safecall AddTrustedDomain(const WideString Domain) = 0 ;
	virtual HRESULT __safecall Get_ApplicationName(WideString &Get_ApplicationName_result) = 0 ;
	virtual HRESULT __safecall Get_Application(_di_IDispatch &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_WebComponent(_di_IDispatch &Get_WebComponent_result) = 0 ;
	virtual HRESULT __safecall Get_ClipArt(_di_IDispatch &Get_ClipArt_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ApplicationName() { WideString r; HRESULT hr = Get_ApplicationName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Application_Name = {read=_scw_Get_ApplicationName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Application() { _di_IDispatch r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Application_ = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_WebComponent() { _di_IDispatch r; HRESULT hr = Get_WebComponent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch WebComponent = {read=_scw_Get_WebComponent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_ClipArt() { _di_IDispatch r; HRESULT hr = Get_ClipArt(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch ClipArt = {read=_scw_Get_ClipArt};
};

__dispinterface IMsoEServicesDialogDisp;
typedef System::DelphiInterface<IMsoEServicesDialogDisp> _di_IMsoEServicesDialogDisp;
__dispinterface INTERFACE_UUID("{000C0372-0000-0000-C000-000000000046}") IMsoEServicesDialogDisp  : public IDispatch 
{
	
};

__interface WebComponentProperties;
typedef System::DelphiInterface<WebComponentProperties> _di_WebComponentProperties;
__interface INTERFACE_UUID("{000C0373-0000-0000-C000-000000000046}") WebComponentProperties  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Shape(_di_IDispatch &Get_Shape_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString RetValue) = 0 ;
	virtual HRESULT __safecall Get_URL(WideString &Get_URL_result) = 0 ;
	virtual HRESULT __safecall Set_URL(const WideString RetValue) = 0 ;
	virtual HRESULT __safecall Get_HTML(WideString &Get_HTML_result) = 0 ;
	virtual HRESULT __safecall Set_HTML(const WideString RetValue) = 0 ;
	virtual HRESULT __safecall Get_PreviewGraphic(WideString &Get_PreviewGraphic_result) = 0 ;
	virtual HRESULT __safecall Set_PreviewGraphic(const WideString RetValue) = 0 ;
	virtual HRESULT __safecall Get_PreviewHTML(WideString &Get_PreviewHTML_result) = 0 ;
	virtual HRESULT __safecall Set_PreviewHTML(const WideString RetValue) = 0 ;
	virtual HRESULT __safecall Get_Width(int &Get_Width_result) = 0 ;
	virtual HRESULT __safecall Set_Width(int RetValue) = 0 ;
	virtual HRESULT __safecall Get_Height(int &Get_Height_result) = 0 ;
	virtual HRESULT __safecall Set_Height(int RetValue) = 0 ;
	virtual HRESULT __safecall Get_Tag(WideString &Get_Tag_result) = 0 ;
	virtual HRESULT __safecall Set_Tag(const WideString RetValue) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Shape() { _di_IDispatch r; HRESULT hr = Get_Shape(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Shape = {read=_scw_Get_Shape};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_URL() { WideString r; HRESULT hr = Get_URL(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString URL = {read=_scw_Get_URL, write=Set_URL};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HTML() { WideString r; HRESULT hr = Get_HTML(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HTML = {read=_scw_Get_HTML, write=Set_HTML};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_PreviewGraphic() { WideString r; HRESULT hr = Get_PreviewGraphic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString PreviewGraphic = {read=_scw_Get_PreviewGraphic, write=Set_PreviewGraphic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_PreviewHTML() { WideString r; HRESULT hr = Get_PreviewHTML(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString PreviewHTML = {read=_scw_Get_PreviewHTML, write=Set_PreviewHTML};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Width() { int r; HRESULT hr = Get_Width(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Width = {read=_scw_Get_Width, write=Set_Width};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Height() { int r; HRESULT hr = Get_Height(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Height = {read=_scw_Get_Height, write=Set_Height};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Tag() { WideString r; HRESULT hr = Get_Tag(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Tag = {read=_scw_Get_Tag, write=Set_Tag};
};

__dispinterface WebComponentPropertiesDisp;
typedef System::DelphiInterface<WebComponentPropertiesDisp> _di_WebComponentPropertiesDisp;
__dispinterface INTERFACE_UUID("{000C0373-0000-0000-C000-000000000046}") WebComponentPropertiesDisp  : public IDispatch 
{
	
};

class DELPHICLASS CoCommandBars;
class PASCALIMPLEMENTATION CoCommandBars : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__CommandBars __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__CommandBars __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoCommandBars(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoCommandBars(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoCommandBarComboBox;
class PASCALIMPLEMENTATION CoCommandBarComboBox : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__CommandBarComboBox __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__CommandBarComboBox __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoCommandBarComboBox(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoCommandBarComboBox(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoCommandBarButton;
class PASCALIMPLEMENTATION CoCommandBarButton : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__CommandBarButton __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__CommandBarButton __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoCommandBarButton(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoCommandBarButton(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoMsoEnvelope;
class PASCALIMPLEMENTATION CoMsoEnvelope : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di_IMsoEnvelopeVB __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di_IMsoEnvelopeVB __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoMsoEnvelope(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoMsoEnvelope(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
static const Shortint OfficeMajorVersion = 0x2;
static const Shortint OfficeMinorVersion = 0x2;
extern PACKAGE GUID LIBID_Office;
extern PACKAGE GUID IID_IAccessible;
extern PACKAGE GUID IID__IMsoDispObj;
extern PACKAGE GUID IID__IMsoOleAccDispObj;
extern PACKAGE GUID IID__CommandBars;
extern PACKAGE GUID IID_CommandBar;
extern PACKAGE GUID IID_CommandBarControls;
extern PACKAGE GUID IID_CommandBarControl;
extern PACKAGE GUID IID__CommandBarButton;
extern PACKAGE GUID IID_CommandBarPopup;
extern PACKAGE GUID IID__CommandBarComboBox;
extern PACKAGE GUID IID__CommandBarActiveX;
extern PACKAGE GUID IID_Adjustments;
extern PACKAGE GUID IID_CalloutFormat;
extern PACKAGE GUID IID_ColorFormat;
extern PACKAGE GUID IID_ConnectorFormat;
extern PACKAGE GUID IID_FillFormat;
extern PACKAGE GUID IID_FreeformBuilder;
extern PACKAGE GUID IID_GroupShapes;
extern PACKAGE GUID IID_LineFormat;
extern PACKAGE GUID IID_ShapeNode;
extern PACKAGE GUID IID_ShapeNodes;
extern PACKAGE GUID IID_PictureFormat;
extern PACKAGE GUID IID_ShadowFormat;
extern PACKAGE GUID IID_Script;
extern PACKAGE GUID IID_Scripts;
extern PACKAGE GUID IID_Shape;
extern PACKAGE GUID IID_ShapeRange;
extern PACKAGE GUID IID_Shapes;
extern PACKAGE GUID IID_TextEffectFormat;
extern PACKAGE GUID IID_TextFrame;
extern PACKAGE GUID IID_ThreeDFormat;
extern PACKAGE GUID IID_IMsoDispCagNotifySink;
extern PACKAGE GUID IID_Balloon;
extern PACKAGE GUID IID_BalloonCheckboxes;
extern PACKAGE GUID IID_BalloonCheckbox;
extern PACKAGE GUID IID_BalloonLabels;
extern PACKAGE GUID IID_BalloonLabel;
extern PACKAGE GUID IID_AnswerWizardFiles;
extern PACKAGE GUID IID_AnswerWizard;
extern PACKAGE GUID IID_Assistant;
extern PACKAGE GUID IID_DocumentProperty;
extern PACKAGE GUID IID_DocumentProperties;
extern PACKAGE GUID IID_IFoundFiles;
extern PACKAGE GUID IID_IFind;
extern PACKAGE GUID IID_FoundFiles;
extern PACKAGE GUID IID_PropertyTest;
extern PACKAGE GUID IID_PropertyTests;
extern PACKAGE GUID IID_FileSearch;
extern PACKAGE GUID IID_COMAddIn;
extern PACKAGE GUID IID_COMAddIns;
extern PACKAGE GUID IID_LanguageSettings;
extern PACKAGE GUID IID_ICommandBarsEvents;
extern PACKAGE GUID DIID__CommandBarsEvents;
extern PACKAGE GUID CLASS_CommandBars;
extern PACKAGE GUID IID_ICommandBarComboBoxEvents;
extern PACKAGE GUID DIID__CommandBarComboBoxEvents;
extern PACKAGE GUID CLASS_CommandBarComboBox;
extern PACKAGE GUID IID_ICommandBarButtonEvents;
extern PACKAGE GUID DIID__CommandBarButtonEvents;
extern PACKAGE GUID CLASS_CommandBarButton;
extern PACKAGE GUID IID_WebPageFont;
extern PACKAGE GUID IID_WebPageFonts;
extern PACKAGE GUID IID_HTMLProjectItem;
extern PACKAGE GUID IID_HTMLProjectItems;
extern PACKAGE GUID IID_HTMLProject;
extern PACKAGE GUID IID_MsoDebugOptions;
extern PACKAGE GUID IID_FileDialogSelectedItems;
extern PACKAGE GUID IID_FileDialogFilter;
extern PACKAGE GUID IID_FileDialogFilters;
extern PACKAGE GUID IID_FileDialog;
extern PACKAGE GUID IID_SignatureSet;
extern PACKAGE GUID IID_Signature;
extern PACKAGE GUID IID_IMsoEnvelopeVB;
extern PACKAGE GUID DIID_IMsoEnvelopeVBEvents;
extern PACKAGE GUID CLASS_MsoEnvelope;
extern PACKAGE GUID IID_FileTypes;
extern PACKAGE GUID IID_SearchFolders;
extern PACKAGE GUID IID_ScopeFolders;
extern PACKAGE GUID IID_ScopeFolder;
extern PACKAGE GUID IID_SearchScope;
extern PACKAGE GUID IID_SearchScopes;
extern PACKAGE GUID IID_IMsoDiagram;
extern PACKAGE GUID IID_DiagramNodes;
extern PACKAGE GUID IID_DiagramNodeChildren;
extern PACKAGE GUID IID_DiagramNode;
extern PACKAGE GUID IID_CanvasShapes;
extern PACKAGE GUID IID_OfficeDataSourceObject;
extern PACKAGE GUID IID_ODSOColumn;
extern PACKAGE GUID IID_ODSOColumns;
extern PACKAGE GUID IID_ODSOFilter;
extern PACKAGE GUID IID_ODSOFilters;
extern PACKAGE GUID IID_NewFile;
extern PACKAGE GUID IID_WebComponent;
extern PACKAGE GUID IID_WebComponentWindowExternal;
extern PACKAGE GUID IID_WebComponentFormat;
extern PACKAGE GUID IID_ILicWizExternal;
extern PACKAGE GUID IID_ILicValidator;
extern PACKAGE GUID IID_ILicAgent;
extern PACKAGE GUID IID_IMsoEServicesDialog;
extern PACKAGE GUID IID_WebComponentProperties;
static const unsigned msoLineDashStyleMixed = 0xfffffffe;
static const Shortint msoLineSolid = 0x1;
static const Shortint msoLineSquareDot = 0x2;
static const Shortint msoLineRoundDot = 0x3;
static const Shortint msoLineDash = 0x4;
static const Shortint msoLineDashDot = 0x5;
static const Shortint msoLineDashDotDot = 0x6;
static const Shortint msoLineLongDash = 0x7;
static const Shortint msoLineLongDashDot = 0x8;
static const unsigned msoLineStyleMixed = 0xfffffffe;
static const Shortint msoLineSingle = 0x1;
static const Shortint msoLineThinThin = 0x2;
static const Shortint msoLineThinThick = 0x3;
static const Shortint msoLineThickThin = 0x4;
static const Shortint msoLineThickBetweenThin = 0x5;
static const unsigned msoArrowheadStyleMixed = 0xfffffffe;
static const Shortint msoArrowheadNone = 0x1;
static const Shortint msoArrowheadTriangle = 0x2;
static const Shortint msoArrowheadOpen = 0x3;
static const Shortint msoArrowheadStealth = 0x4;
static const Shortint msoArrowheadDiamond = 0x5;
static const Shortint msoArrowheadOval = 0x6;
static const unsigned msoArrowheadWidthMixed = 0xfffffffe;
static const Shortint msoArrowheadNarrow = 0x1;
static const Shortint msoArrowheadWidthMedium = 0x2;
static const Shortint msoArrowheadWide = 0x3;
static const unsigned msoArrowheadLengthMixed = 0xfffffffe;
static const Shortint msoArrowheadShort = 0x1;
static const Shortint msoArrowheadLengthMedium = 0x2;
static const Shortint msoArrowheadLong = 0x3;
static const unsigned msoFillMixed = 0xfffffffe;
static const Shortint msoFillSolid = 0x1;
static const Shortint msoFillPatterned = 0x2;
static const Shortint msoFillGradient = 0x3;
static const Shortint msoFillTextured = 0x4;
static const Shortint msoFillBackground = 0x5;
static const Shortint msoFillPicture = 0x6;
static const unsigned msoGradientMixed = 0xfffffffe;
static const Shortint msoGradientHorizontal = 0x1;
static const Shortint msoGradientVertical = 0x2;
static const Shortint msoGradientDiagonalUp = 0x3;
static const Shortint msoGradientDiagonalDown = 0x4;
static const Shortint msoGradientFromCorner = 0x5;
static const Shortint msoGradientFromTitle = 0x6;
static const Shortint msoGradientFromCenter = 0x7;
static const unsigned msoGradientColorMixed = 0xfffffffe;
static const Shortint msoGradientOneColor = 0x1;
static const Shortint msoGradientTwoColors = 0x2;
static const Shortint msoGradientPresetColors = 0x3;
static const unsigned msoTextureTypeMixed = 0xfffffffe;
static const Shortint msoTexturePreset = 0x1;
static const Shortint msoTextureUserDefined = 0x2;
static const unsigned msoPresetTextureMixed = 0xfffffffe;
static const Shortint msoTexturePapyrus = 0x1;
static const Shortint msoTextureCanvas = 0x2;
static const Shortint msoTextureDenim = 0x3;
static const Shortint msoTextureWovenMat = 0x4;
static const Shortint msoTextureWaterDroplets = 0x5;
static const Shortint msoTexturePaperBag = 0x6;
static const Shortint msoTextureFishFossil = 0x7;
static const Shortint msoTextureSand = 0x8;
static const Shortint msoTextureGreenMarble = 0x9;
static const Shortint msoTextureWhiteMarble = 0xa;
static const Shortint msoTextureBrownMarble = 0xb;
static const Shortint msoTextureGranite = 0xc;
static const Shortint msoTextureNewsprint = 0xd;
static const Shortint msoTextureRecycledPaper = 0xe;
static const Shortint msoTextureParchment = 0xf;
static const Shortint msoTextureStationery = 0x10;
static const Shortint msoTextureBlueTissuePaper = 0x11;
static const Shortint msoTexturePinkTissuePaper = 0x12;
static const Shortint msoTexturePurpleMesh = 0x13;
static const Shortint msoTextureBouquet = 0x14;
static const Shortint msoTextureCork = 0x15;
static const Shortint msoTextureWalnut = 0x16;
static const Shortint msoTextureOak = 0x17;
static const Shortint msoTextureMediumWood = 0x18;
static const unsigned msoPatternMixed = 0xfffffffe;
static const Shortint msoPattern5Percent = 0x1;
static const Shortint msoPattern10Percent = 0x2;
static const Shortint msoPattern20Percent = 0x3;
static const Shortint msoPattern25Percent = 0x4;
static const Shortint msoPattern30Percent = 0x5;
static const Shortint msoPattern40Percent = 0x6;
static const Shortint msoPattern50Percent = 0x7;
static const Shortint msoPattern60Percent = 0x8;
static const Shortint msoPattern70Percent = 0x9;
static const Shortint msoPattern75Percent = 0xa;
static const Shortint msoPattern80Percent = 0xb;
static const Shortint msoPattern90Percent = 0xc;
static const Shortint msoPatternDarkHorizontal = 0xd;
static const Shortint msoPatternDarkVertical = 0xe;
static const Shortint msoPatternDarkDownwardDiagonal = 0xf;
static const Shortint msoPatternDarkUpwardDiagonal = 0x10;
static const Shortint msoPatternSmallCheckerBoard = 0x11;
static const Shortint msoPatternTrellis = 0x12;
static const Shortint msoPatternLightHorizontal = 0x13;
static const Shortint msoPatternLightVertical = 0x14;
static const Shortint msoPatternLightDownwardDiagonal = 0x15;
static const Shortint msoPatternLightUpwardDiagonal = 0x16;
static const Shortint msoPatternSmallGrid = 0x17;
static const Shortint msoPatternDottedDiamond = 0x18;
static const Shortint msoPatternWideDownwardDiagonal = 0x19;
static const Shortint msoPatternWideUpwardDiagonal = 0x1a;
static const Shortint msoPatternDashedUpwardDiagonal = 0x1b;
static const Shortint msoPatternDashedDownwardDiagonal = 0x1c;
static const Shortint msoPatternNarrowVertical = 0x1d;
static const Shortint msoPatternNarrowHorizontal = 0x1e;
static const Shortint msoPatternDashedVertical = 0x1f;
static const Shortint msoPatternDashedHorizontal = 0x20;
static const Shortint msoPatternLargeConfetti = 0x21;
static const Shortint msoPatternLargeGrid = 0x22;
static const Shortint msoPatternHorizontalBrick = 0x23;
static const Shortint msoPatternLargeCheckerBoard = 0x24;
static const Shortint msoPatternSmallConfetti = 0x25;
static const Shortint msoPatternZigZag = 0x26;
static const Shortint msoPatternSolidDiamond = 0x27;
static const Shortint msoPatternDiagonalBrick = 0x28;
static const Shortint msoPatternOutlinedDiamond = 0x29;
static const Shortint msoPatternPlaid = 0x2a;
static const Shortint msoPatternSphere = 0x2b;
static const Shortint msoPatternWeave = 0x2c;
static const Shortint msoPatternDottedGrid = 0x2d;
static const Shortint msoPatternDivot = 0x2e;
static const Shortint msoPatternShingle = 0x2f;
static const Shortint msoPatternWave = 0x30;
static const unsigned msoPresetGradientMixed = 0xfffffffe;
static const Shortint msoGradientEarlySunset = 0x1;
static const Shortint msoGradientLateSunset = 0x2;
static const Shortint msoGradientNightfall = 0x3;
static const Shortint msoGradientDaybreak = 0x4;
static const Shortint msoGradientHorizon = 0x5;
static const Shortint msoGradientDesert = 0x6;
static const Shortint msoGradientOcean = 0x7;
static const Shortint msoGradientCalmWater = 0x8;
static const Shortint msoGradientFire = 0x9;
static const Shortint msoGradientFog = 0xa;
static const Shortint msoGradientMoss = 0xb;
static const Shortint msoGradientPeacock = 0xc;
static const Shortint msoGradientWheat = 0xd;
static const Shortint msoGradientParchment = 0xe;
static const Shortint msoGradientMahogany = 0xf;
static const Shortint msoGradientRainbow = 0x10;
static const Shortint msoGradientRainbowII = 0x11;
static const Shortint msoGradientGold = 0x12;
static const Shortint msoGradientGoldII = 0x13;
static const Shortint msoGradientBrass = 0x14;
static const Shortint msoGradientChrome = 0x15;
static const Shortint msoGradientChromeII = 0x16;
static const Shortint msoGradientSilver = 0x17;
static const Shortint msoGradientSapphire = 0x18;
static const unsigned msoShadowMixed = 0xfffffffe;
static const Shortint msoShadow1 = 0x1;
static const Shortint msoShadow2 = 0x2;
static const Shortint msoShadow3 = 0x3;
static const Shortint msoShadow4 = 0x4;
static const Shortint msoShadow5 = 0x5;
static const Shortint msoShadow6 = 0x6;
static const Shortint msoShadow7 = 0x7;
static const Shortint msoShadow8 = 0x8;
static const Shortint msoShadow9 = 0x9;
static const Shortint msoShadow10 = 0xa;
static const Shortint msoShadow11 = 0xb;
static const Shortint msoShadow12 = 0xc;
static const Shortint msoShadow13 = 0xd;
static const Shortint msoShadow14 = 0xe;
static const Shortint msoShadow15 = 0xf;
static const Shortint msoShadow16 = 0x10;
static const Shortint msoShadow17 = 0x11;
static const Shortint msoShadow18 = 0x12;
static const Shortint msoShadow19 = 0x13;
static const Shortint msoShadow20 = 0x14;
static const unsigned msoTextEffectMixed = 0xfffffffe;
static const Shortint msoTextEffect1 = 0x0;
static const Shortint msoTextEffect2 = 0x1;
static const Shortint msoTextEffect3 = 0x2;
static const Shortint msoTextEffect4 = 0x3;
static const Shortint msoTextEffect5 = 0x4;
static const Shortint msoTextEffect6 = 0x5;
static const Shortint msoTextEffect7 = 0x6;
static const Shortint msoTextEffect8 = 0x7;
static const Shortint msoTextEffect9 = 0x8;
static const Shortint msoTextEffect10 = 0x9;
static const Shortint msoTextEffect11 = 0xa;
static const Shortint msoTextEffect12 = 0xb;
static const Shortint msoTextEffect13 = 0xc;
static const Shortint msoTextEffect14 = 0xd;
static const Shortint msoTextEffect15 = 0xe;
static const Shortint msoTextEffect16 = 0xf;
static const Shortint msoTextEffect17 = 0x10;
static const Shortint msoTextEffect18 = 0x11;
static const Shortint msoTextEffect19 = 0x12;
static const Shortint msoTextEffect20 = 0x13;
static const Shortint msoTextEffect21 = 0x14;
static const Shortint msoTextEffect22 = 0x15;
static const Shortint msoTextEffect23 = 0x16;
static const Shortint msoTextEffect24 = 0x17;
static const Shortint msoTextEffect25 = 0x18;
static const Shortint msoTextEffect26 = 0x19;
static const Shortint msoTextEffect27 = 0x1a;
static const Shortint msoTextEffect28 = 0x1b;
static const Shortint msoTextEffect29 = 0x1c;
static const Shortint msoTextEffect30 = 0x1d;
static const unsigned msoTextEffectShapeMixed = 0xfffffffe;
static const Shortint msoTextEffectShapePlainText = 0x1;
static const Shortint msoTextEffectShapeStop = 0x2;
static const Shortint msoTextEffectShapeTriangleUp = 0x3;
static const Shortint msoTextEffectShapeTriangleDown = 0x4;
static const Shortint msoTextEffectShapeChevronUp = 0x5;
static const Shortint msoTextEffectShapeChevronDown = 0x6;
static const Shortint msoTextEffectShapeRingInside = 0x7;
static const Shortint msoTextEffectShapeRingOutside = 0x8;
static const Shortint msoTextEffectShapeArchUpCurve = 0x9;
static const Shortint msoTextEffectShapeArchDownCurve = 0xa;
static const Shortint msoTextEffectShapeCircleCurve = 0xb;
static const Shortint msoTextEffectShapeButtonCurve = 0xc;
static const Shortint msoTextEffectShapeArchUpPour = 0xd;
static const Shortint msoTextEffectShapeArchDownPour = 0xe;
static const Shortint msoTextEffectShapeCirclePour = 0xf;
static const Shortint msoTextEffectShapeButtonPour = 0x10;
static const Shortint msoTextEffectShapeCurveUp = 0x11;
static const Shortint msoTextEffectShapeCurveDown = 0x12;
static const Shortint msoTextEffectShapeCanUp = 0x13;
static const Shortint msoTextEffectShapeCanDown = 0x14;
static const Shortint msoTextEffectShapeWave1 = 0x15;
static const Shortint msoTextEffectShapeWave2 = 0x16;
static const Shortint msoTextEffectShapeDoubleWave1 = 0x17;
static const Shortint msoTextEffectShapeDoubleWave2 = 0x18;
static const Shortint msoTextEffectShapeInflate = 0x19;
static const Shortint msoTextEffectShapeDeflate = 0x1a;
static const Shortint msoTextEffectShapeInflateBottom = 0x1b;
static const Shortint msoTextEffectShapeDeflateBottom = 0x1c;
static const Shortint msoTextEffectShapeInflateTop = 0x1d;
static const Shortint msoTextEffectShapeDeflateTop = 0x1e;
static const Shortint msoTextEffectShapeDeflateInflate = 0x1f;
static const Shortint msoTextEffectShapeDeflateInflateDeflate = 0x20;
static const Shortint msoTextEffectShapeFadeRight = 0x21;
static const Shortint msoTextEffectShapeFadeLeft = 0x22;
static const Shortint msoTextEffectShapeFadeUp = 0x23;
static const Shortint msoTextEffectShapeFadeDown = 0x24;
static const Shortint msoTextEffectShapeSlantUp = 0x25;
static const Shortint msoTextEffectShapeSlantDown = 0x26;
static const Shortint msoTextEffectShapeCascadeUp = 0x27;
static const Shortint msoTextEffectShapeCascadeDown = 0x28;
static const unsigned msoTextEffectAlignmentMixed = 0xfffffffe;
static const Shortint msoTextEffectAlignmentLeft = 0x1;
static const Shortint msoTextEffectAlignmentCentered = 0x2;
static const Shortint msoTextEffectAlignmentRight = 0x3;
static const Shortint msoTextEffectAlignmentLetterJustify = 0x4;
static const Shortint msoTextEffectAlignmentWordJustify = 0x5;
static const Shortint msoTextEffectAlignmentStretchJustify = 0x6;
static const unsigned msoPresetLightingDirectionMixed = 0xfffffffe;
static const Shortint msoLightingTopLeft = 0x1;
static const Shortint msoLightingTop = 0x2;
static const Shortint msoLightingTopRight = 0x3;
static const Shortint msoLightingLeft = 0x4;
static const Shortint msoLightingNone = 0x5;
static const Shortint msoLightingRight = 0x6;
static const Shortint msoLightingBottomLeft = 0x7;
static const Shortint msoLightingBottom = 0x8;
static const Shortint msoLightingBottomRight = 0x9;
static const unsigned msoPresetLightingSoftnessMixed = 0xfffffffe;
static const Shortint msoLightingDim = 0x1;
static const Shortint msoLightingNormal = 0x2;
static const Shortint msoLightingBright = 0x3;
static const unsigned msoPresetMaterialMixed = 0xfffffffe;
static const Shortint msoMaterialMatte = 0x1;
static const Shortint msoMaterialPlastic = 0x2;
static const Shortint msoMaterialMetal = 0x3;
static const Shortint msoMaterialWireFrame = 0x4;
static const unsigned msoPresetExtrusionDirectionMixed = 0xfffffffe;
static const Shortint msoExtrusionBottomRight = 0x1;
static const Shortint msoExtrusionBottom = 0x2;
static const Shortint msoExtrusionBottomLeft = 0x3;
static const Shortint msoExtrusionRight = 0x4;
static const Shortint msoExtrusionNone = 0x5;
static const Shortint msoExtrusionLeft = 0x6;
static const Shortint msoExtrusionTopRight = 0x7;
static const Shortint msoExtrusionTop = 0x8;
static const Shortint msoExtrusionTopLeft = 0x9;
static const unsigned msoPresetThreeDFormatMixed = 0xfffffffe;
static const Shortint msoThreeD1 = 0x1;
static const Shortint msoThreeD2 = 0x2;
static const Shortint msoThreeD3 = 0x3;
static const Shortint msoThreeD4 = 0x4;
static const Shortint msoThreeD5 = 0x5;
static const Shortint msoThreeD6 = 0x6;
static const Shortint msoThreeD7 = 0x7;
static const Shortint msoThreeD8 = 0x8;
static const Shortint msoThreeD9 = 0x9;
static const Shortint msoThreeD10 = 0xa;
static const Shortint msoThreeD11 = 0xb;
static const Shortint msoThreeD12 = 0xc;
static const Shortint msoThreeD13 = 0xd;
static const Shortint msoThreeD14 = 0xe;
static const Shortint msoThreeD15 = 0xf;
static const Shortint msoThreeD16 = 0x10;
static const Shortint msoThreeD17 = 0x11;
static const Shortint msoThreeD18 = 0x12;
static const Shortint msoThreeD19 = 0x13;
static const Shortint msoThreeD20 = 0x14;
static const unsigned msoExtrusionColorTypeMixed = 0xfffffffe;
static const Shortint msoExtrusionColorAutomatic = 0x1;
static const Shortint msoExtrusionColorCustom = 0x2;
static const Shortint msoAlignLefts = 0x0;
static const Shortint msoAlignCenters = 0x1;
static const Shortint msoAlignRights = 0x2;
static const Shortint msoAlignTops = 0x3;
static const Shortint msoAlignMiddles = 0x4;
static const Shortint msoAlignBottoms = 0x5;
static const Shortint msoDistributeHorizontally = 0x0;
static const Shortint msoDistributeVertically = 0x1;
static const unsigned msoConnectorTypeMixed = 0xfffffffe;
static const Shortint msoConnectorStraight = 0x1;
static const Shortint msoConnectorElbow = 0x2;
static const Shortint msoConnectorCurve = 0x3;
static const unsigned msoHorizontalAnchorMixed = 0xfffffffe;
static const Shortint msoAnchorNone = 0x1;
static const Shortint msoAnchorCenter = 0x2;
static const unsigned msoVerticalAnchorMixed = 0xfffffffe;
static const Shortint msoAnchorTop = 0x1;
static const Shortint msoAnchorTopBaseline = 0x2;
static const Shortint msoAnchorMiddle = 0x3;
static const Shortint msoAnchorBottom = 0x4;
static const Shortint msoAnchorBottomBaseLine = 0x5;
static const unsigned msoOrientationMixed = 0xfffffffe;
static const Shortint msoOrientationHorizontal = 0x1;
static const Shortint msoOrientationVertical = 0x2;
static const Shortint msoBringToFront = 0x0;
static const Shortint msoSendToBack = 0x1;
static const Shortint msoBringForward = 0x2;
static const Shortint msoSendBackward = 0x3;
static const Shortint msoBringInFrontOfText = 0x4;
static const Shortint msoSendBehindText = 0x5;
static const Shortint msoSegmentLine = 0x0;
static const Shortint msoSegmentCurve = 0x1;
static const Shortint msoEditingAuto = 0x0;
static const Shortint msoEditingCorner = 0x1;
static const Shortint msoEditingSmooth = 0x2;
static const Shortint msoEditingSymmetric = 0x3;
static const unsigned msoShapeMixed = 0xfffffffe;
static const Shortint msoShapeRectangle = 0x1;
static const Shortint msoShapeParallelogram = 0x2;
static const Shortint msoShapeTrapezoid = 0x3;
static const Shortint msoShapeDiamond = 0x4;
static const Shortint msoShapeRoundedRectangle = 0x5;
static const Shortint msoShapeOctagon = 0x6;
static const Shortint msoShapeIsoscelesTriangle = 0x7;
static const Shortint msoShapeRightTriangle = 0x8;
static const Shortint msoShapeOval = 0x9;
static const Shortint msoShapeHexagon = 0xa;
static const Shortint msoShapeCross = 0xb;
static const Shortint msoShapeRegularPentagon = 0xc;
static const Shortint msoShapeCan = 0xd;
static const Shortint msoShapeCube = 0xe;
static const Shortint msoShapeBevel = 0xf;
static const Shortint msoShapeFoldedCorner = 0x10;
static const Shortint msoShapeSmileyFace = 0x11;
static const Shortint msoShapeDonut = 0x12;
static const Shortint msoShapeNoSymbol = 0x13;
static const Shortint msoShapeBlockArc = 0x14;
static const Shortint msoShapeHeart = 0x15;
static const Shortint msoShapeLightningBolt = 0x16;
static const Shortint msoShapeSun = 0x17;
static const Shortint msoShapeMoon = 0x18;
static const Shortint msoShapeArc = 0x19;
static const Shortint msoShapeDoubleBracket = 0x1a;
static const Shortint msoShapeDoubleBrace = 0x1b;
static const Shortint msoShapePlaque = 0x1c;
static const Shortint msoShapeLeftBracket = 0x1d;
static const Shortint msoShapeRightBracket = 0x1e;
static const Shortint msoShapeLeftBrace = 0x1f;
static const Shortint msoShapeRightBrace = 0x20;
static const Shortint msoShapeRightArrow = 0x21;
static const Shortint msoShapeLeftArrow = 0x22;
static const Shortint msoShapeUpArrow = 0x23;
static const Shortint msoShapeDownArrow = 0x24;
static const Shortint msoShapeLeftRightArrow = 0x25;
static const Shortint msoShapeUpDownArrow = 0x26;
static const Shortint msoShapeQuadArrow = 0x27;
static const Shortint msoShapeLeftRightUpArrow = 0x28;
static const Shortint msoShapeBentArrow = 0x29;
static const Shortint msoShapeUTurnArrow = 0x2a;
static const Shortint msoShapeLeftUpArrow = 0x2b;
static const Shortint msoShapeBentUpArrow = 0x2c;
static const Shortint msoShapeCurvedRightArrow = 0x2d;
static const Shortint msoShapeCurvedLeftArrow = 0x2e;
static const Shortint msoShapeCurvedUpArrow = 0x2f;
static const Shortint msoShapeCurvedDownArrow = 0x30;
static const Shortint msoShapeStripedRightArrow = 0x31;
static const Shortint msoShapeNotchedRightArrow = 0x32;
static const Shortint msoShapePentagon = 0x33;
static const Shortint msoShapeChevron = 0x34;
static const Shortint msoShapeRightArrowCallout = 0x35;
static const Shortint msoShapeLeftArrowCallout = 0x36;
static const Shortint msoShapeUpArrowCallout = 0x37;
static const Shortint msoShapeDownArrowCallout = 0x38;
static const Shortint msoShapeLeftRightArrowCallout = 0x39;
static const Shortint msoShapeUpDownArrowCallout = 0x3a;
static const Shortint msoShapeQuadArrowCallout = 0x3b;
static const Shortint msoShapeCircularArrow = 0x3c;
static const Shortint msoShapeFlowchartProcess = 0x3d;
static const Shortint msoShapeFlowchartAlternateProcess = 0x3e;
static const Shortint msoShapeFlowchartDecision = 0x3f;
static const Shortint msoShapeFlowchartData = 0x40;
static const Shortint msoShapeFlowchartPredefinedProcess = 0x41;
static const Shortint msoShapeFlowchartInternalStorage = 0x42;
static const Shortint msoShapeFlowchartDocument = 0x43;
static const Shortint msoShapeFlowchartMultidocument = 0x44;
static const Shortint msoShapeFlowchartTerminator = 0x45;
static const Shortint msoShapeFlowchartPreparation = 0x46;
static const Shortint msoShapeFlowchartManualInput = 0x47;
static const Shortint msoShapeFlowchartManualOperation = 0x48;
static const Shortint msoShapeFlowchartConnector = 0x49;
static const Shortint msoShapeFlowchartOffpageConnector = 0x4a;
static const Shortint msoShapeFlowchartCard = 0x4b;
static const Shortint msoShapeFlowchartPunchedTape = 0x4c;
static const Shortint msoShapeFlowchartSummingJunction = 0x4d;
static const Shortint msoShapeFlowchartOr = 0x4e;
static const Shortint msoShapeFlowchartCollate = 0x4f;
static const Shortint msoShapeFlowchartSort = 0x50;
static const Shortint msoShapeFlowchartExtract = 0x51;
static const Shortint msoShapeFlowchartMerge = 0x52;
static const Shortint msoShapeFlowchartStoredData = 0x53;
static const Shortint msoShapeFlowchartDelay = 0x54;
static const Shortint msoShapeFlowchartSequentialAccessStorage = 0x55;
static const Shortint msoShapeFlowchartMagneticDisk = 0x56;
static const Shortint msoShapeFlowchartDirectAccessStorage = 0x57;
static const Shortint msoShapeFlowchartDisplay = 0x58;
static const Shortint msoShapeExplosion1 = 0x59;
static const Shortint msoShapeExplosion2 = 0x5a;
static const Shortint msoShape4pointStar = 0x5b;
static const Shortint msoShape5pointStar = 0x5c;
static const Shortint msoShape8pointStar = 0x5d;
static const Shortint msoShape16pointStar = 0x5e;
static const Shortint msoShape24pointStar = 0x5f;
static const Shortint msoShape32pointStar = 0x60;
static const Shortint msoShapeUpRibbon = 0x61;
static const Shortint msoShapeDownRibbon = 0x62;
static const Shortint msoShapeCurvedUpRibbon = 0x63;
static const Shortint msoShapeCurvedDownRibbon = 0x64;
static const Shortint msoShapeVerticalScroll = 0x65;
static const Shortint msoShapeHorizontalScroll = 0x66;
static const Shortint msoShapeWave = 0x67;
static const Shortint msoShapeDoubleWave = 0x68;
static const Shortint msoShapeRectangularCallout = 0x69;
static const Shortint msoShapeRoundedRectangularCallout = 0x6a;
static const Shortint msoShapeOvalCallout = 0x6b;
static const Shortint msoShapeCloudCallout = 0x6c;
static const Shortint msoShapeLineCallout1 = 0x6d;
static const Shortint msoShapeLineCallout2 = 0x6e;
static const Shortint msoShapeLineCallout3 = 0x6f;
static const Shortint msoShapeLineCallout4 = 0x70;
static const Shortint msoShapeLineCallout1AccentBar = 0x71;
static const Shortint msoShapeLineCallout2AccentBar = 0x72;
static const Shortint msoShapeLineCallout3AccentBar = 0x73;
static const Shortint msoShapeLineCallout4AccentBar = 0x74;
static const Shortint msoShapeLineCallout1NoBorder = 0x75;
static const Shortint msoShapeLineCallout2NoBorder = 0x76;
static const Shortint msoShapeLineCallout3NoBorder = 0x77;
static const Shortint msoShapeLineCallout4NoBorder = 0x78;
static const Shortint msoShapeLineCallout1BorderandAccentBar = 0x79;
static const Shortint msoShapeLineCallout2BorderandAccentBar = 0x7a;
static const Shortint msoShapeLineCallout3BorderandAccentBar = 0x7b;
static const Shortint msoShapeLineCallout4BorderandAccentBar = 0x7c;
static const Shortint msoShapeActionButtonCustom = 0x7d;
static const Shortint msoShapeActionButtonHome = 0x7e;
static const Shortint msoShapeActionButtonHelp = 0x7f;
static const Byte msoShapeActionButtonInformation = 0x80;
static const Byte msoShapeActionButtonBackorPrevious = 0x81;
static const Byte msoShapeActionButtonForwardorNext = 0x82;
static const Byte msoShapeActionButtonBeginning = 0x83;
static const Byte msoShapeActionButtonEnd = 0x84;
static const Byte msoShapeActionButtonReturn = 0x85;
static const Byte msoShapeActionButtonDocument = 0x86;
static const Byte msoShapeActionButtonSound = 0x87;
static const Byte msoShapeActionButtonMovie = 0x88;
static const Byte msoShapeBalloon = 0x89;
static const Byte msoShapeNotPrimitive = 0x8a;
static const unsigned msoShapeTypeMixed = 0xfffffffe;
static const Shortint msoAutoShape = 0x1;
static const Shortint msoCallout = 0x2;
static const Shortint msoChart = 0x3;
static const Shortint msoComment = 0x4;
static const Shortint msoFreeform = 0x5;
static const Shortint msoGroup = 0x6;
static const Shortint msoEmbeddedOLEObject = 0x7;
static const Shortint msoFormControl = 0x8;
static const Shortint msoLine = 0x9;
static const Shortint msoLinkedOLEObject = 0xa;
static const Shortint msoLinkedPicture = 0xb;
static const Shortint msoOLEControlObject = 0xc;
static const Shortint msoPicture = 0xd;
static const Shortint msoPlaceholder = 0xe;
static const Shortint msoTextEffect = 0xf;
static const Shortint msoMedia = 0x10;
static const Shortint msoTextBox = 0x11;
static const Shortint msoScriptAnchor = 0x12;
static const Shortint msoTable = 0x13;
static const Shortint msoCanvas = 0x14;
static const Shortint msoDiagram = 0x15;
static const Shortint msoFlipHorizontal = 0x0;
static const Shortint msoFlipVertical = 0x1;
static const unsigned msoTrue = 0xffffffff;
static const Shortint msoFalse = 0x0;
static const Shortint msoCTrue = 0x1;
static const unsigned msoTriStateToggle = 0xfffffffd;
static const unsigned msoTriStateMixed = 0xfffffffe;
static const unsigned msoColorTypeMixed = 0xfffffffe;
static const Shortint msoColorTypeRGB = 0x1;
static const Shortint msoColorTypeScheme = 0x2;
static const Shortint msoColorTypeCMYK = 0x3;
static const Shortint msoColorTypeCMS = 0x4;
static const Shortint msoColorTypeInk = 0x5;
static const unsigned msoPictureMixed = 0xfffffffe;
static const Shortint msoPictureAutomatic = 0x1;
static const Shortint msoPictureGrayscale = 0x2;
static const Shortint msoPictureBlackAndWhite = 0x3;
static const Shortint msoPictureWatermark = 0x4;
static const unsigned msoCalloutAngleMixed = 0xfffffffe;
static const Shortint msoCalloutAngleAutomatic = 0x1;
static const Shortint msoCalloutAngle30 = 0x2;
static const Shortint msoCalloutAngle45 = 0x3;
static const Shortint msoCalloutAngle60 = 0x4;
static const Shortint msoCalloutAngle90 = 0x5;
static const unsigned msoCalloutDropMixed = 0xfffffffe;
static const Shortint msoCalloutDropCustom = 0x1;
static const Shortint msoCalloutDropTop = 0x2;
static const Shortint msoCalloutDropCenter = 0x3;
static const Shortint msoCalloutDropBottom = 0x4;
static const unsigned msoCalloutMixed = 0xfffffffe;
static const Shortint msoCalloutOne = 0x1;
static const Shortint msoCalloutTwo = 0x2;
static const Shortint msoCalloutThree = 0x3;
static const Shortint msoCalloutFour = 0x4;
static const unsigned msoBlackWhiteMixed = 0xfffffffe;
static const Shortint msoBlackWhiteAutomatic = 0x1;
static const Shortint msoBlackWhiteGrayScale = 0x2;
static const Shortint msoBlackWhiteLightGrayScale = 0x3;
static const Shortint msoBlackWhiteInverseGrayScale = 0x4;
static const Shortint msoBlackWhiteGrayOutline = 0x5;
static const Shortint msoBlackWhiteBlackTextAndLine = 0x6;
static const Shortint msoBlackWhiteHighContrast = 0x7;
static const Shortint msoBlackWhiteBlack = 0x8;
static const Shortint msoBlackWhiteWhite = 0x9;
static const Shortint msoBlackWhiteDontShow = 0xa;
static const Word msoIntegerMixed = 0x8000;
static const unsigned msoSingleMixed = 0x80000000;
static const unsigned msoTextOrientationMixed = 0xfffffffe;
static const Shortint msoTextOrientationHorizontal = 0x1;
static const Shortint msoTextOrientationUpward = 0x2;
static const Shortint msoTextOrientationDownward = 0x3;
static const Shortint msoTextOrientationVerticalFarEast = 0x4;
static const Shortint msoTextOrientationVertical = 0x5;
static const Shortint msoTextOrientationHorizontalRotatedFarEast = 0x6;
static const Shortint msoScaleFromTopLeft = 0x0;
static const Shortint msoScaleFromMiddle = 0x1;
static const Shortint msoScaleFromBottomRight = 0x2;
static const Shortint msoBarLeft = 0x0;
static const Shortint msoBarTop = 0x1;
static const Shortint msoBarRight = 0x2;
static const Shortint msoBarBottom = 0x3;
static const Shortint msoBarFloating = 0x4;
static const Shortint msoBarPopup = 0x5;
static const Shortint msoBarMenuBar = 0x6;
static const Shortint msoBarNoProtection = 0x0;
static const Shortint msoBarNoCustomize = 0x1;
static const Shortint msoBarNoResize = 0x2;
static const Shortint msoBarNoMove = 0x4;
static const Shortint msoBarNoChangeVisible = 0x8;
static const Shortint msoBarNoChangeDock = 0x10;
static const Shortint msoBarNoVerticalDock = 0x20;
static const Shortint msoBarNoHorizontalDock = 0x40;
static const Shortint msoBarTypeNormal = 0x0;
static const Shortint msoBarTypeMenuBar = 0x1;
static const Shortint msoBarTypePopup = 0x2;
static const Shortint msoControlCustom = 0x0;
static const Shortint msoControlButton = 0x1;
static const Shortint msoControlEdit = 0x2;
static const Shortint msoControlDropdown = 0x3;
static const Shortint msoControlComboBox = 0x4;
static const Shortint msoControlButtonDropdown = 0x5;
static const Shortint msoControlSplitDropdown = 0x6;
static const Shortint msoControlOCXDropdown = 0x7;
static const Shortint msoControlGenericDropdown = 0x8;
static const Shortint msoControlGraphicDropdown = 0x9;
static const Shortint msoControlPopup = 0xa;
static const Shortint msoControlGraphicPopup = 0xb;
static const Shortint msoControlButtonPopup = 0xc;
static const Shortint msoControlSplitButtonPopup = 0xd;
static const Shortint msoControlSplitButtonMRUPopup = 0xe;
static const Shortint msoControlLabel = 0xf;
static const Shortint msoControlExpandingGrid = 0x10;
static const Shortint msoControlSplitExpandingGrid = 0x11;
static const Shortint msoControlGrid = 0x12;
static const Shortint msoControlGauge = 0x13;
static const Shortint msoControlGraphicCombo = 0x14;
static const Shortint msoControlPane = 0x15;
static const Shortint msoControlActiveX = 0x16;
static const Shortint msoControlSpinner = 0x17;
static const Shortint msoControlLabelEx = 0x18;
static const Shortint msoControlWorkPane = 0x19;
static const Shortint msoControlAutoCompleteCombo = 0x1a;
static const Shortint msoButtonUp = 0x0;
static const unsigned msoButtonDown = 0xffffffff;
static const Shortint msoButtonMixed = 0x2;
static const Shortint msoControlOLEUsageNeither = 0x0;
static const Shortint msoControlOLEUsageServer = 0x1;
static const Shortint msoControlOLEUsageClient = 0x2;
static const Shortint msoControlOLEUsageBoth = 0x3;
static const Shortint msoButtonWrapText = 0x4;
static const Shortint msoButtonTextBelow = 0x8;
static const Shortint msoButtonAutomatic = 0x0;
static const Shortint msoButtonIcon = 0x1;
static const Shortint msoButtonCaption = 0x2;
static const Shortint msoButtonIconAndCaption = 0x3;
static const Shortint msoButtonIconAndWrapCaption = 0x7;
static const Shortint msoButtonIconAndCaptionBelow = 0xb;
static const Shortint msoButtonWrapCaption = 0xe;
static const Shortint msoButtonIconAndWrapCaptionBelow = 0xf;
static const Shortint msoComboNormal = 0x0;
static const Shortint msoComboLabel = 0x1;
static const unsigned msoOLEMenuGroupNone = 0xffffffff;
static const Shortint msoOLEMenuGroupFile = 0x0;
static const Shortint msoOLEMenuGroupEdit = 0x1;
static const Shortint msoOLEMenuGroupContainer = 0x2;
static const Shortint msoOLEMenuGroupObject = 0x3;
static const Shortint msoOLEMenuGroupWindow = 0x4;
static const Shortint msoOLEMenuGroupHelp = 0x5;
static const Shortint msoMenuAnimationNone = 0x0;
static const Shortint msoMenuAnimationRandom = 0x1;
static const Shortint msoMenuAnimationUnfold = 0x2;
static const Shortint msoMenuAnimationSlide = 0x3;
static const Shortint msoBarRowFirst = 0x0;
static const unsigned msoBarRowLast = 0xffffffff;
static const Shortint msoCommandBarButtonHyperlinkNone = 0x0;
static const Shortint msoCommandBarButtonHyperlinkOpen = 0x1;
static const Shortint msoCommandBarButtonHyperlinkInsertPicture = 0x2;
static const Shortint msoHyperlinkRange = 0x0;
static const Shortint msoHyperlinkShape = 0x1;
static const Shortint msoHyperlinkInlineShape = 0x2;
static const Shortint msoMethodGet = 0x0;
static const Shortint msoMethodPost = 0x1;
static const Shortint msoAnimationIdle = 0x1;
static const Shortint msoAnimationGreeting = 0x2;
static const Shortint msoAnimationGoodbye = 0x3;
static const Shortint msoAnimationBeginSpeaking = 0x4;
static const Shortint msoAnimationRestPose = 0x5;
static const Shortint msoAnimationCharacterSuccessMajor = 0x6;
static const Shortint msoAnimationGetAttentionMajor = 0xb;
static const Shortint msoAnimationGetAttentionMinor = 0xc;
static const Shortint msoAnimationSearching = 0xd;
static const Shortint msoAnimationPrinting = 0x12;
static const Shortint msoAnimationGestureRight = 0x13;
static const Shortint msoAnimationWritingNotingSomething = 0x16;
static const Shortint msoAnimationWorkingAtSomething = 0x17;
static const Shortint msoAnimationThinking = 0x18;
static const Shortint msoAnimationSendingMail = 0x19;
static const Shortint msoAnimationListensToComputer = 0x1a;
static const Shortint msoAnimationDisappear = 0x1f;
static const Shortint msoAnimationAppear = 0x20;
static const Shortint msoAnimationGetArtsy = 0x64;
static const Shortint msoAnimationGetTechy = 0x65;
static const Shortint msoAnimationGetWizardy = 0x66;
static const Shortint msoAnimationCheckingSomething = 0x67;
static const Shortint msoAnimationLookDown = 0x68;
static const Shortint msoAnimationLookDownLeft = 0x69;
static const Shortint msoAnimationLookDownRight = 0x6a;
static const Shortint msoAnimationLookLeft = 0x6b;
static const Shortint msoAnimationLookRight = 0x6c;
static const Shortint msoAnimationLookUp = 0x6d;
static const Shortint msoAnimationLookUpLeft = 0x6e;
static const Shortint msoAnimationLookUpRight = 0x6f;
static const Shortint msoAnimationSaving = 0x70;
static const Shortint msoAnimationGestureDown = 0x71;
static const Shortint msoAnimationGestureLeft = 0x72;
static const Shortint msoAnimationGestureUp = 0x73;
static const Shortint msoAnimationEmptyTrash = 0x74;
static const Shortint msoButtonSetNone = 0x0;
static const Shortint msoButtonSetOK = 0x1;
static const Shortint msoButtonSetCancel = 0x2;
static const Shortint msoButtonSetOkCancel = 0x3;
static const Shortint msoButtonSetYesNo = 0x4;
static const Shortint msoButtonSetYesNoCancel = 0x5;
static const Shortint msoButtonSetBackClose = 0x6;
static const Shortint msoButtonSetNextClose = 0x7;
static const Shortint msoButtonSetBackNextClose = 0x8;
static const Shortint msoButtonSetRetryCancel = 0x9;
static const Shortint msoButtonSetAbortRetryIgnore = 0xa;
static const Shortint msoButtonSetSearchClose = 0xb;
static const Shortint msoButtonSetBackNextSnooze = 0xc;
static const Shortint msoButtonSetTipsOptionsClose = 0xd;
static const Shortint msoButtonSetYesAllNoCancel = 0xe;
static const Shortint msoIconNone = 0x0;
static const Shortint msoIconAlert = 0x2;
static const Shortint msoIconTip = 0x3;
static const Shortint msoIconAlertInfo = 0x4;
static const Shortint msoIconAlertWarning = 0x5;
static const Shortint msoIconAlertQuery = 0x6;
static const Shortint msoIconAlertCritical = 0x7;
static const Shortint msoBalloonTypeButtons = 0x0;
static const Shortint msoBalloonTypeBullets = 0x1;
static const Shortint msoBalloonTypeNumbers = 0x2;
static const Shortint msoModeModal = 0x0;
static const Shortint msoModeAutoDown = 0x1;
static const Shortint msoModeModeless = 0x2;
static const Shortint msoBalloonErrorNone = 0x0;
static const Shortint msoBalloonErrorOther = 0x1;
static const Shortint msoBalloonErrorTooBig = 0x2;
static const Shortint msoBalloonErrorOutOfMemory = 0x3;
static const Shortint msoBalloonErrorBadPictureRef = 0x4;
static const Shortint msoBalloonErrorBadReference = 0x5;
static const Shortint msoBalloonErrorButtonlessModal = 0x6;
static const Shortint msoBalloonErrorButtonModeless = 0x7;
static const Shortint msoBalloonErrorBadCharacter = 0x8;
static const Shortint msoBalloonErrorCOMFailure = 0x9;
static const Shortint msoBalloonErrorCharNotTopmostForModal = 0xa;
static const Shortint msoBalloonErrorTooManyControls = 0xb;
static const Shortint msoWizardActInactive = 0x0;
static const Shortint msoWizardActActive = 0x1;
static const Shortint msoWizardActSuspend = 0x2;
static const Shortint msoWizardActResume = 0x3;
static const Shortint msoWizardMsgLocalStateOn = 0x1;
static const Shortint msoWizardMsgLocalStateOff = 0x2;
static const Shortint msoWizardMsgShowHelp = 0x3;
static const Shortint msoWizardMsgSuspending = 0x4;
static const Shortint msoWizardMsgResuming = 0x5;
static const unsigned msoBalloonButtonYesToAll = 0xfffffff1;
static const unsigned msoBalloonButtonOptions = 0xfffffff2;
static const unsigned msoBalloonButtonTips = 0xfffffff3;
static const unsigned msoBalloonButtonClose = 0xfffffff4;
static const unsigned msoBalloonButtonSnooze = 0xfffffff5;
static const unsigned msoBalloonButtonSearch = 0xfffffff6;
static const unsigned msoBalloonButtonIgnore = 0xfffffff7;
static const unsigned msoBalloonButtonAbort = 0xfffffff8;
static const unsigned msoBalloonButtonRetry = 0xfffffff9;
static const unsigned msoBalloonButtonNext = 0xfffffffa;
static const unsigned msoBalloonButtonBack = 0xfffffffb;
static const unsigned msoBalloonButtonNo = 0xfffffffc;
static const unsigned msoBalloonButtonYes = 0xfffffffd;
static const unsigned msoBalloonButtonCancel = 0xfffffffe;
static const unsigned msoBalloonButtonOK = 0xffffffff;
static const Shortint msoBalloonButtonNull = 0x0;
static const Shortint offPropertyTypeNumber = 0x1;
static const Shortint offPropertyTypeBoolean = 0x2;
static const Shortint offPropertyTypeDate = 0x3;
static const Shortint offPropertyTypeString = 0x4;
static const Shortint offPropertyTypeFloat = 0x5;
static const Shortint msoPropertyTypeNumber = 0x1;
static const Shortint msoPropertyTypeBoolean = 0x2;
static const Shortint msoPropertyTypeDate = 0x3;
static const Shortint msoPropertyTypeString = 0x4;
static const Shortint msoPropertyTypeFloat = 0x5;
static const Shortint msoLanguageIDInstall = 0x1;
static const Shortint msoLanguageIDUI = 0x2;
static const Shortint msoLanguageIDHelp = 0x3;
static const Shortint msoLanguageIDExeMode = 0x4;
static const Shortint msoLanguageIDUIPrevious = 0x5;
static const Word MsoFarEastLineBreakLanguageJapanese = 0x411;
static const Word MsoFarEastLineBreakLanguageKorean = 0x412;
static const Word MsoFarEastLineBreakLanguageSimplifiedChinese = 0x804;
static const Word MsoFarEastLineBreakLanguageTraditionalChinese = 0x404;
static const Shortint msoFeatureInstallNone = 0x0;
static const Shortint msoFeatureInstallOnDemand = 0x1;
static const Shortint msoFeatureInstallOnDemandWithUI = 0x2;
static const Shortint msoScriptLanguageJava = 0x1;
static const Shortint msoScriptLanguageVisualBasic = 0x2;
static const Shortint msoScriptLanguageASP = 0x3;
static const Shortint msoScriptLanguageOther = 0x4;
static const Shortint msoScriptLocationInHead = 0x1;
static const Shortint msoScriptLocationInBody = 0x2;
static const Shortint msoOptionsNew = 0x1;
static const Shortint msoOptionsAdd = 0x2;
static const Shortint msoOptionsWithin = 0x3;
static const Shortint msoViewFileInfo = 0x1;
static const Shortint msoViewPreview = 0x2;
static const Shortint msoViewSummaryInfo = 0x3;
static const Shortint msoFileFindSortbyAuthor = 0x1;
static const Shortint msoFileFindSortbyDateCreated = 0x2;
static const Shortint msoFileFindSortbyLastSavedBy = 0x3;
static const Shortint msoFileFindSortbyDateSaved = 0x4;
static const Shortint msoFileFindSortbyFileName = 0x5;
static const Shortint msoFileFindSortbySize = 0x6;
static const Shortint msoFileFindSortbyTitle = 0x7;
static const Shortint msoListbyName = 0x1;
static const Shortint msoListbyTitle = 0x2;
static const Shortint msoLastModifiedYesterday = 0x1;
static const Shortint msoLastModifiedToday = 0x2;
static const Shortint msoLastModifiedLastWeek = 0x3;
static const Shortint msoLastModifiedThisWeek = 0x4;
static const Shortint msoLastModifiedLastMonth = 0x5;
static const Shortint msoLastModifiedThisMonth = 0x6;
static const Shortint msoLastModifiedAnyTime = 0x7;
static const Shortint msoSortByFileName = 0x1;
static const Shortint msoSortBySize = 0x2;
static const Shortint msoSortByFileType = 0x3;
static const Shortint msoSortByLastModified = 0x4;
static const Shortint msoSortByNone = 0x5;
static const Shortint msoSortOrderAscending = 0x1;
static const Shortint msoSortOrderDescending = 0x2;
static const Shortint msoConnectorAnd = 0x1;
static const Shortint msoConnectorOr = 0x2;
static const Shortint msoConditionFileTypeAllFiles = 0x1;
static const Shortint msoConditionFileTypeOfficeFiles = 0x2;
static const Shortint msoConditionFileTypeWordDocuments = 0x3;
static const Shortint msoConditionFileTypeExcelWorkbooks = 0x4;
static const Shortint msoConditionFileTypePowerPointPresentations = 0x5;
static const Shortint msoConditionFileTypeBinders = 0x6;
static const Shortint msoConditionFileTypeDatabases = 0x7;
static const Shortint msoConditionFileTypeTemplates = 0x8;
static const Shortint msoConditionIncludes = 0x9;
static const Shortint msoConditionIncludesPhrase = 0xa;
static const Shortint msoConditionBeginsWith = 0xb;
static const Shortint msoConditionEndsWith = 0xc;
static const Shortint msoConditionIncludesNearEachOther = 0xd;
static const Shortint msoConditionIsExactly = 0xe;
static const Shortint msoConditionIsNot = 0xf;
static const Shortint msoConditionYesterday = 0x10;
static const Shortint msoConditionToday = 0x11;
static const Shortint msoConditionTomorrow = 0x12;
static const Shortint msoConditionLastWeek = 0x13;
static const Shortint msoConditionThisWeek = 0x14;
static const Shortint msoConditionNextWeek = 0x15;
static const Shortint msoConditionLastMonth = 0x16;
static const Shortint msoConditionThisMonth = 0x17;
static const Shortint msoConditionNextMonth = 0x18;
static const Shortint msoConditionAnytime = 0x19;
static const Shortint msoConditionAnytimeBetween = 0x1a;
static const Shortint msoConditionOn = 0x1b;
static const Shortint msoConditionOnOrAfter = 0x1c;
static const Shortint msoConditionOnOrBefore = 0x1d;
static const Shortint msoConditionInTheNext = 0x1e;
static const Shortint msoConditionInTheLast = 0x1f;
static const Shortint msoConditionEquals = 0x20;
static const Shortint msoConditionDoesNotEqual = 0x21;
static const Shortint msoConditionAnyNumberBetween = 0x22;
static const Shortint msoConditionAtMost = 0x23;
static const Shortint msoConditionAtLeast = 0x24;
static const Shortint msoConditionMoreThan = 0x25;
static const Shortint msoConditionLessThan = 0x26;
static const Shortint msoConditionIsYes = 0x27;
static const Shortint msoConditionIsNo = 0x28;
static const Shortint msoConditionIncludesFormsOf = 0x29;
static const Shortint msoConditionFreeText = 0x2a;
static const Shortint msoConditionFileTypeOutlookItems = 0x2b;
static const Shortint msoConditionFileTypeMailItem = 0x2c;
static const Shortint msoConditionFileTypeCalendarItem = 0x2d;
static const Shortint msoConditionFileTypeContactItem = 0x2e;
static const Shortint msoConditionFileTypeNoteItem = 0x2f;
static const Shortint msoConditionFileTypeJournalItem = 0x30;
static const Shortint msoConditionFileTypeTaskItem = 0x31;
static const Shortint msoConditionFileTypePhotoDrawFiles = 0x32;
static const Shortint msoConditionFileTypeDataConnectionFiles = 0x33;
static const Shortint msoConditionFileTypePublisherFiles = 0x34;
static const Shortint msoConditionFileTypeProjectFiles = 0x35;
static const Shortint msoConditionFileTypeDocumentImagingFiles = 0x36;
static const Shortint msoConditionFileTypeVisioFiles = 0x37;
static const Shortint msoConditionFileTypeDesignerFiles = 0x38;
static const Shortint msoConditionFileTypeWebPages = 0x39;
static const Shortint msoConditionEqualsLow = 0x3a;
static const Shortint msoConditionEqualsNormal = 0x3b;
static const Shortint msoConditionEqualsHigh = 0x3c;
static const Shortint msoConditionNotEqualToLow = 0x3d;
static const Shortint msoConditionNotEqualToNormal = 0x3e;
static const Shortint msoConditionNotEqualToHigh = 0x3f;
static const Shortint msoConditionEqualsNotStarted = 0x40;
static const Shortint msoConditionEqualsInProgress = 0x41;
static const Shortint msoConditionEqualsCompleted = 0x42;
static const Shortint msoConditionEqualsWaitingForSomeoneElse = 0x43;
static const Shortint msoConditionEqualsDeferred = 0x44;
static const Shortint msoConditionNotEqualToNotStarted = 0x45;
static const Shortint msoConditionNotEqualToInProgress = 0x46;
static const Shortint msoConditionNotEqualToCompleted = 0x47;
static const Shortint msoConditionNotEqualToWaitingForSomeoneElse = 0x48;
static const Shortint msoConditionNotEqualToDeferred = 0x49;
static const Shortint msoFileTypeAllFiles = 0x1;
static const Shortint msoFileTypeOfficeFiles = 0x2;
static const Shortint msoFileTypeWordDocuments = 0x3;
static const Shortint msoFileTypeExcelWorkbooks = 0x4;
static const Shortint msoFileTypePowerPointPresentations = 0x5;
static const Shortint msoFileTypeBinders = 0x6;
static const Shortint msoFileTypeDatabases = 0x7;
static const Shortint msoFileTypeTemplates = 0x8;
static const Shortint msoFileTypeOutlookItems = 0x9;
static const Shortint msoFileTypeMailItem = 0xa;
static const Shortint msoFileTypeCalendarItem = 0xb;
static const Shortint msoFileTypeContactItem = 0xc;
static const Shortint msoFileTypeNoteItem = 0xd;
static const Shortint msoFileTypeJournalItem = 0xe;
static const Shortint msoFileTypeTaskItem = 0xf;
static const Shortint msoFileTypePhotoDrawFiles = 0x10;
static const Shortint msoFileTypeDataConnectionFiles = 0x11;
static const Shortint msoFileTypePublisherFiles = 0x12;
static const Shortint msoFileTypeProjectFiles = 0x13;
static const Shortint msoFileTypeDocumentImagingFiles = 0x14;
static const Shortint msoFileTypeVisioFiles = 0x15;
static const Shortint msoFileTypeDesignerFiles = 0x16;
static const Shortint msoFileTypeWebPages = 0x17;
static const unsigned msoLanguageIDMixed = 0xfffffffe;
static const Shortint msoLanguageIDNone = 0x0;
static const Word msoLanguageIDNoProofing = 0x400;
static const Word msoLanguageIDAfrikaans = 0x436;
static const Word msoLanguageIDAlbanian = 0x41c;
static const Word msoLanguageIDAmharic = 0x45e;
static const Word msoLanguageIDArabicAlgeria = 0x1401;
static const Word msoLanguageIDArabicBahrain = 0x3c01;
static const Word msoLanguageIDArabicEgypt = 0xc01;
static const Word msoLanguageIDArabicIraq = 0x801;
static const Word msoLanguageIDArabicJordan = 0x2c01;
static const Word msoLanguageIDArabicKuwait = 0x3401;
static const Word msoLanguageIDArabicLebanon = 0x3001;
static const Word msoLanguageIDArabicLibya = 0x1001;
static const Word msoLanguageIDArabicMorocco = 0x1801;
static const Word msoLanguageIDArabicOman = 0x2001;
static const Word msoLanguageIDArabicQatar = 0x4001;
static const Word msoLanguageIDArabic = 0x401;
static const Word msoLanguageIDArabicSyria = 0x2801;
static const Word msoLanguageIDArabicTunisia = 0x1c01;
static const Word msoLanguageIDArabicUAE = 0x3801;
static const Word msoLanguageIDArabicYemen = 0x2401;
static const Word msoLanguageIDArmenian = 0x42b;
static const Word msoLanguageIDAssamese = 0x44d;
static const Word msoLanguageIDAzeriCyrillic = 0x82c;
static const Word msoLanguageIDAzeriLatin = 0x42c;
static const Word msoLanguageIDBasque = 0x42d;
static const Word msoLanguageIDByelorussian = 0x423;
static const Word msoLanguageIDBengali = 0x445;
static const Word msoLanguageIDBulgarian = 0x402;
static const Word msoLanguageIDBurmese = 0x455;
static const Word msoLanguageIDCatalan = 0x403;
static const Word msoLanguageIDChineseHongKongSAR = 0xc04;
static const Word msoLanguageIDChineseMacaoSAR = 0x1404;
static const Word msoLanguageIDSimplifiedChinese = 0x804;
static const Word msoLanguageIDChineseSingapore = 0x1004;
static const Word msoLanguageIDTraditionalChinese = 0x404;
static const Word msoLanguageIDCherokee = 0x45c;
static const Word msoLanguageIDCroatian = 0x41a;
static const Word msoLanguageIDCzech = 0x405;
static const Word msoLanguageIDDanish = 0x406;
static const Word msoLanguageIDDivehi = 0x465;
static const Word msoLanguageIDBelgianDutch = 0x813;
static const Word msoLanguageIDDutch = 0x413;
static const Word msoLanguageIDDzongkhaBhutan = 0x851;
static const Word msoLanguageIDEdo = 0x466;
static const Word msoLanguageIDEnglishAUS = 0xc09;
static const Word msoLanguageIDEnglishBelize = 0x2809;
static const Word msoLanguageIDEnglishCanadian = 0x1009;
static const Word msoLanguageIDEnglishCaribbean = 0x2409;
static const Word msoLanguageIDEnglishIndonesia = 0x3809;
static const Word msoLanguageIDEnglishIreland = 0x1809;
static const Word msoLanguageIDEnglishJamaica = 0x2009;
static const Word msoLanguageIDEnglishNewZealand = 0x1409;
static const Word msoLanguageIDEnglishPhilippines = 0x3409;
static const Word msoLanguageIDEnglishSouthAfrica = 0x1c09;
static const Word msoLanguageIDEnglishTrinidadTobago = 0x2c09;
static const Word msoLanguageIDEnglishUK = 0x809;
static const Word msoLanguageIDEnglishUS = 0x409;
static const Word msoLanguageIDEnglishZimbabwe = 0x3009;
static const Word msoLanguageIDEstonian = 0x425;
static const Word msoLanguageIDFaeroese = 0x438;
static const Word msoLanguageIDFarsi = 0x429;
static const Word msoLanguageIDFilipino = 0x464;
static const Word msoLanguageIDFinnish = 0x40b;
static const Word msoLanguageIDBelgianFrench = 0x80c;
static const Word msoLanguageIDFrenchCameroon = 0x2c0c;
static const Word msoLanguageIDFrenchCanadian = 0xc0c;
static const Word msoLanguageIDFrenchCotedIvoire = 0x300c;
static const Word msoLanguageIDFrench = 0x40c;
static const Word msoLanguageIDFrenchHaiti = 0x3c0c;
static const Word msoLanguageIDFrenchLuxembourg = 0x140c;
static const Word msoLanguageIDFrenchMali = 0x340c;
static const Word msoLanguageIDFrenchMonaco = 0x180c;
static const Word msoLanguageIDFrenchMorocco = 0x380c;
static const Word msoLanguageIDFrenchReunion = 0x200c;
static const Word msoLanguageIDFrenchSenegal = 0x280c;
static const Word msoLanguageIDSwissFrench = 0x100c;
static const Word msoLanguageIDFrenchWestIndies = 0x1c0c;
static const Word msoLanguageIDFrenchZaire = 0x240c;
static const Word msoLanguageIDFrisianNetherlands = 0x462;
static const Word msoLanguageIDFulfulde = 0x467;
static const Word msoLanguageIDGaelicIreland = 0x83c;
static const Word msoLanguageIDGaelicScotland = 0x43c;
static const Word msoLanguageIDGalician = 0x456;
static const Word msoLanguageIDGeorgian = 0x437;
static const Word msoLanguageIDGermanAustria = 0xc07;
static const Word msoLanguageIDGerman = 0x407;
static const Word msoLanguageIDGermanLiechtenstein = 0x1407;
static const Word msoLanguageIDGermanLuxembourg = 0x1007;
static const Word msoLanguageIDSwissGerman = 0x807;
static const Word msoLanguageIDGreek = 0x408;
static const Word msoLanguageIDGuarani = 0x474;
static const Word msoLanguageIDGujarati = 0x447;
static const Word msoLanguageIDHausa = 0x468;
static const Word msoLanguageIDHawaiian = 0x475;
static const Word msoLanguageIDHebrew = 0x40d;
static const Word msoLanguageIDHindi = 0x439;
static const Word msoLanguageIDHungarian = 0x40e;
static const Word msoLanguageIDIbibio = 0x469;
static const Word msoLanguageIDIcelandic = 0x40f;
static const Word msoLanguageIDIgbo = 0x470;
static const Word msoLanguageIDIndonesian = 0x421;
static const Word msoLanguageIDInuktitut = 0x45d;
static const Word msoLanguageIDItalian = 0x410;
static const Word msoLanguageIDSwissItalian = 0x810;
static const Word msoLanguageIDJapanese = 0x411;
static const Word msoLanguageIDKannada = 0x44b;
static const Word msoLanguageIDKanuri = 0x471;
static const Word msoLanguageIDKashmiri = 0x460;
static const Word msoLanguageIDKashmiriDevanagari = 0x860;
static const Word msoLanguageIDKazakh = 0x43f;
static const Word msoLanguageIDKhmer = 0x453;
static const Word msoLanguageIDKirghiz = 0x440;
static const Word msoLanguageIDKonkani = 0x457;
static const Word msoLanguageIDKorean = 0x412;
static const Word msoLanguageIDKyrgyz = 0x440;
static const Word msoLanguageIDLatin = 0x476;
static const Word msoLanguageIDLao = 0x454;
static const Word msoLanguageIDLatvian = 0x426;
static const Word msoLanguageIDLithuanian = 0x427;
static const Word msoLanguageIDMacedonian = 0x42f;
static const Word msoLanguageIDMalaysian = 0x43e;
static const Word msoLanguageIDMalayBruneiDarussalam = 0x83e;
static const Word msoLanguageIDMalayalam = 0x44c;
static const Word msoLanguageIDMaltese = 0x43a;
static const Word msoLanguageIDManipuri = 0x458;
static const Word msoLanguageIDMarathi = 0x44e;
static const Word msoLanguageIDMongolian = 0x450;
static const Word msoLanguageIDNepali = 0x461;
static const Word msoLanguageIDNorwegianBokmol = 0x414;
static const Word msoLanguageIDNorwegianNynorsk = 0x814;
static const Word msoLanguageIDOriya = 0x448;
static const Word msoLanguageIDOromo = 0x472;
static const Word msoLanguageIDPashto = 0x463;
static const Word msoLanguageIDPolish = 0x415;
static const Word msoLanguageIDBrazilianPortuguese = 0x416;
static const Word msoLanguageIDPortuguese = 0x816;
static const Word msoLanguageIDPunjabi = 0x446;
static const Word msoLanguageIDRhaetoRomanic = 0x417;
static const Word msoLanguageIDRomanianMoldova = 0x818;
static const Word msoLanguageIDRomanian = 0x418;
static const Word msoLanguageIDRussianMoldova = 0x819;
static const Word msoLanguageIDRussian = 0x419;
static const Word msoLanguageIDSamiLappish = 0x43b;
static const Word msoLanguageIDSanskrit = 0x44f;
static const Word msoLanguageIDSerbianCyrillic = 0xc1a;
static const Word msoLanguageIDSerbianLatin = 0x81a;
static const Word msoLanguageIDSesotho = 0x430;
static const Word msoLanguageIDSindhi = 0x459;
static const Word msoLanguageIDSindhiPakistan = 0x859;
static const Word msoLanguageIDSinhalese = 0x45b;
static const Word msoLanguageIDSlovak = 0x41b;
static const Word msoLanguageIDSlovenian = 0x424;
static const Word msoLanguageIDSomali = 0x477;
static const Word msoLanguageIDSorbian = 0x42e;
static const Word msoLanguageIDSpanishArgentina = 0x2c0a;
static const Word msoLanguageIDSpanishBolivia = 0x400a;
static const Word msoLanguageIDSpanishChile = 0x340a;
static const Word msoLanguageIDSpanishColombia = 0x240a;
static const Word msoLanguageIDSpanishCostaRica = 0x140a;
static const Word msoLanguageIDSpanishDominicanRepublic = 0x1c0a;
static const Word msoLanguageIDSpanishEcuador = 0x300a;
static const Word msoLanguageIDSpanishElSalvador = 0x440a;
static const Word msoLanguageIDSpanishGuatemala = 0x100a;
static const Word msoLanguageIDSpanishHonduras = 0x480a;
static const Word msoLanguageIDMexicanSpanish = 0x80a;
static const Word msoLanguageIDSpanishNicaragua = 0x4c0a;
static const Word msoLanguageIDSpanishPanama = 0x180a;
static const Word msoLanguageIDSpanishParaguay = 0x3c0a;
static const Word msoLanguageIDSpanishPeru = 0x280a;
static const Word msoLanguageIDSpanishPuertoRico = 0x500a;
static const Word msoLanguageIDSpanishModernSort = 0xc0a;
static const Word msoLanguageIDSpanish = 0x40a;
static const Word msoLanguageIDSpanishUruguay = 0x380a;
static const Word msoLanguageIDSpanishVenezuela = 0x200a;
static const Word msoLanguageIDSutu = 0x430;
static const Word msoLanguageIDSwahili = 0x441;
static const Word msoLanguageIDSwedishFinland = 0x81d;
static const Word msoLanguageIDSwedish = 0x41d;
static const Word msoLanguageIDSyriac = 0x45a;
static const Word msoLanguageIDTajik = 0x428;
static const Word msoLanguageIDTamil = 0x449;
static const Word msoLanguageIDTamazight = 0x45f;
static const Word msoLanguageIDTamazightLatin = 0x85f;
static const Word msoLanguageIDTatar = 0x444;
static const Word msoLanguageIDTelugu = 0x44a;
static const Word msoLanguageIDThai = 0x41e;
static const Word msoLanguageIDTibetan = 0x451;
static const Word msoLanguageIDTigrignaEthiopic = 0x473;
static const Word msoLanguageIDTigrignaEritrea = 0x873;
static const Word msoLanguageIDTsonga = 0x431;
static const Word msoLanguageIDTswana = 0x432;
static const Word msoLanguageIDTurkish = 0x41f;
static const Word msoLanguageIDTurkmen = 0x442;
static const Word msoLanguageIDUkrainian = 0x422;
static const Word msoLanguageIDUrdu = 0x420;
static const Word msoLanguageIDUzbekCyrillic = 0x843;
static const Word msoLanguageIDUzbekLatin = 0x443;
static const Word msoLanguageIDVenda = 0x433;
static const Word msoLanguageIDVietnamese = 0x42a;
static const Word msoLanguageIDWelsh = 0x452;
static const Word msoLanguageIDXhosa = 0x434;
static const Word msoLanguageIDYi = 0x478;
static const Word msoLanguageIDYiddish = 0x43d;
static const Word msoLanguageIDYoruba = 0x46a;
static const Word msoLanguageIDZulu = 0x435;
static const Shortint msoScreenSize544x376 = 0x0;
static const Shortint msoScreenSize640x480 = 0x1;
static const Shortint msoScreenSize720x512 = 0x2;
static const Shortint msoScreenSize800x600 = 0x3;
static const Shortint msoScreenSize1024x768 = 0x4;
static const Shortint msoScreenSize1152x882 = 0x5;
static const Shortint msoScreenSize1152x900 = 0x6;
static const Shortint msoScreenSize1280x1024 = 0x7;
static const Shortint msoScreenSize1600x1200 = 0x8;
static const Shortint msoScreenSize1800x1440 = 0x9;
static const Shortint msoScreenSize1920x1200 = 0xa;
static const Shortint msoCharacterSetArabic = 0x1;
static const Shortint msoCharacterSetCyrillic = 0x2;
static const Shortint msoCharacterSetEnglishWesternEuropeanOtherLatinScript = 0x3;
static const Shortint msoCharacterSetGreek = 0x4;
static const Shortint msoCharacterSetHebrew = 0x5;
static const Shortint msoCharacterSetJapanese = 0x6;
static const Shortint msoCharacterSetKorean = 0x7;
static const Shortint msoCharacterSetMultilingualUnicode = 0x8;
static const Shortint msoCharacterSetSimplifiedChinese = 0x9;
static const Shortint msoCharacterSetThai = 0xa;
static const Shortint msoCharacterSetTraditionalChinese = 0xb;
static const Shortint msoCharacterSetVietnamese = 0xc;
static const Word msoEncodingThai = 0x36a;
static const Word msoEncodingJapaneseShiftJIS = 0x3a4;
static const Word msoEncodingSimplifiedChineseGBK = 0x3a8;
static const Word msoEncodingKorean = 0x3b5;
static const Word msoEncodingTraditionalChineseBig5 = 0x3b6;
static const Word msoEncodingUnicodeLittleEndian = 0x4b0;
static const Word msoEncodingUnicodeBigEndian = 0x4b1;
static const Word msoEncodingCentralEuropean = 0x4e2;
static const Word msoEncodingCyrillic = 0x4e3;
static const Word msoEncodingWestern = 0x4e4;
static const Word msoEncodingGreek = 0x4e5;
static const Word msoEncodingTurkish = 0x4e6;
static const Word msoEncodingHebrew = 0x4e7;
static const Word msoEncodingArabic = 0x4e8;
static const Word msoEncodingBaltic = 0x4e9;
static const Word msoEncodingVietnamese = 0x4ea;
static const Word msoEncodingAutoDetect = 0xc351;
static const Word msoEncodingJapaneseAutoDetect = 0xc6f4;
static const Word msoEncodingSimplifiedChineseAutoDetect = 0xc6f8;
static const Word msoEncodingKoreanAutoDetect = 0xc705;
static const Word msoEncodingTraditionalChineseAutoDetect = 0xc706;
static const Word msoEncodingCyrillicAutoDetect = 0xc833;
static const Word msoEncodingGreekAutoDetect = 0xc835;
static const Word msoEncodingArabicAutoDetect = 0xc838;
static const Word msoEncodingISO88591Latin1 = 0x6faf;
static const Word msoEncodingISO88592CentralEurope = 0x6fb0;
static const Word msoEncodingISO88593Latin3 = 0x6fb1;
static const Word msoEncodingISO88594Baltic = 0x6fb2;
static const Word msoEncodingISO88595Cyrillic = 0x6fb3;
static const Word msoEncodingISO88596Arabic = 0x6fb4;
static const Word msoEncodingISO88597Greek = 0x6fb5;
static const Word msoEncodingISO88598Hebrew = 0x6fb6;
static const Word msoEncodingISO88599Turkish = 0x6fb7;
static const Word msoEncodingISO885915Latin9 = 0x6fbd;
static const Word msoEncodingISO2022JPNoHalfwidthKatakana = 0xc42c;
static const Word msoEncodingISO2022JPJISX02021984 = 0xc42d;
static const Word msoEncodingISO2022JPJISX02011989 = 0xc42e;
static const Word msoEncodingISO2022KR = 0xc431;
static const Word msoEncodingISO2022CNTraditionalChinese = 0xc433;
static const Word msoEncodingISO2022CNSimplifiedChinese = 0xc435;
static const Word msoEncodingMacRoman = 0x2710;
static const Word msoEncodingMacJapanese = 0x2711;
static const Word msoEncodingMacTraditionalChineseBig5 = 0x2712;
static const Word msoEncodingMacKorean = 0x2713;
static const Word msoEncodingMacArabic = 0x2714;
static const Word msoEncodingMacHebrew = 0x2715;
static const Word msoEncodingMacGreek1 = 0x2716;
static const Word msoEncodingMacCyrillic = 0x2717;
static const Word msoEncodingMacSimplifiedChineseGB2312 = 0x2718;
static const Word msoEncodingMacRomania = 0x271a;
static const Word msoEncodingMacUkraine = 0x2721;
static const Word msoEncodingMacLatin2 = 0x272d;
static const Word msoEncodingMacIcelandic = 0x275f;
static const Word msoEncodingMacTurkish = 0x2761;
static const Word msoEncodingMacCroatia = 0x2762;
static const Shortint msoEncodingEBCDICUSCanada = 0x25;
static const Word msoEncodingEBCDICInternational = 0x1f4;
static const Word msoEncodingEBCDICMultilingualROECELatin2 = 0x366;
static const Word msoEncodingEBCDICGreekModern = 0x36b;
static const Word msoEncodingEBCDICTurkishLatin5 = 0x402;
static const Word msoEncodingEBCDICGermany = 0x4f31;
static const Word msoEncodingEBCDICDenmarkNorway = 0x4f35;
static const Word msoEncodingEBCDICFinlandSweden = 0x4f36;
static const Word msoEncodingEBCDICItaly = 0x4f38;
static const Word msoEncodingEBCDICLatinAmericaSpain = 0x4f3c;
static const Word msoEncodingEBCDICUnitedKingdom = 0x4f3d;
static const Word msoEncodingEBCDICJapaneseKatakanaExtended = 0x4f42;
static const Word msoEncodingEBCDICFrance = 0x4f49;
static const Word msoEncodingEBCDICArabic = 0x4fc4;
static const Word msoEncodingEBCDICGreek = 0x4fc7;
static const Word msoEncodingEBCDICHebrew = 0x4fc8;
static const Word msoEncodingEBCDICKoreanExtended = 0x5161;
static const Word msoEncodingEBCDICThai = 0x5166;
static const Word msoEncodingEBCDICIcelandic = 0x5187;
static const Word msoEncodingEBCDICTurkish = 0x51a9;
static const Word msoEncodingEBCDICRussian = 0x5190;
static const Word msoEncodingEBCDICSerbianBulgarian = 0x5221;
static const Word msoEncodingEBCDICJapaneseKatakanaExtendedAndJapanese = 0xc6f2;
static const Word msoEncodingEBCDICUSCanadaAndJapanese = 0xc6f3;
static const Word msoEncodingEBCDICKoreanExtendedAndKorean = 0xc6f5;
static const Word msoEncodingEBCDICSimplifiedChineseExtendedAndSimplifiedChinese = 0xc6f7;
static const Word msoEncodingEBCDICUSCanadaAndTraditionalChinese = 0xc6f9;
static const Word msoEncodingEBCDICJapaneseLatinExtendedAndJapanese = 0xc6fb;
static const Word msoEncodingOEMUnitedStates = 0x1b5;
static const Word msoEncodingOEMGreek437G = 0x2e1;
static const Word msoEncodingOEMBaltic = 0x307;
static const Word msoEncodingOEMMultilingualLatinI = 0x352;
static const Word msoEncodingOEMMultilingualLatinII = 0x354;
static const Word msoEncodingOEMCyrillic = 0x357;
static const Word msoEncodingOEMTurkish = 0x359;
static const Word msoEncodingOEMPortuguese = 0x35c;
static const Word msoEncodingOEMIcelandic = 0x35d;
static const Word msoEncodingOEMHebrew = 0x35e;
static const Word msoEncodingOEMCanadianFrench = 0x35f;
static const Word msoEncodingOEMArabic = 0x360;
static const Word msoEncodingOEMNordic = 0x361;
static const Word msoEncodingOEMCyrillicII = 0x362;
static const Word msoEncodingOEMModernGreek = 0x365;
static const Word msoEncodingEUCJapanese = 0xcadc;
static const Word msoEncodingEUCChineseSimplifiedChinese = 0xcae0;
static const Word msoEncodingEUCKorean = 0xcaed;
static const Word msoEncodingEUCTaiwaneseTraditionalChinese = 0xcaee;
static const Word msoEncodingISCIIDevanagari = 0xdeaa;
static const Word msoEncodingISCIIBengali = 0xdeab;
static const Word msoEncodingISCIITamil = 0xdeac;
static const Word msoEncodingISCIITelugu = 0xdead;
static const Word msoEncodingISCIIAssamese = 0xdeae;
static const Word msoEncodingISCIIOriya = 0xdeaf;
static const Word msoEncodingISCIIKannada = 0xdeb0;
static const Word msoEncodingISCIIMalayalam = 0xdeb1;
static const Word msoEncodingISCIIGujarati = 0xdeb2;
static const Word msoEncodingISCIIPunjabi = 0xdeb3;
static const Word msoEncodingArabicASMO = 0x2c4;
static const Word msoEncodingArabicTransparentASMO = 0x2d0;
static const Word msoEncodingKoreanJohab = 0x551;
static const Word msoEncodingTaiwanCNS = 0x4e20;
static const Word msoEncodingTaiwanTCA = 0x4e21;
static const Word msoEncodingTaiwanEten = 0x4e22;
static const Word msoEncodingTaiwanIBM5550 = 0x4e23;
static const Word msoEncodingTaiwanTeleText = 0x4e24;
static const Word msoEncodingTaiwanWang = 0x4e25;
static const Word msoEncodingIA5IRV = 0x4e89;
static const Word msoEncodingIA5German = 0x4e8a;
static const Word msoEncodingIA5Swedish = 0x4e8b;
static const Word msoEncodingIA5Norwegian = 0x4e8c;
static const Word msoEncodingUSASCII = 0x4e9f;
static const Word msoEncodingT61 = 0x4f25;
static const Word msoEncodingISO6937NonSpacingAccent = 0x4f2d;
static const Word msoEncodingKOI8R = 0x5182;
static const Word msoEncodingExtAlphaLowercase = 0x5223;
static const Word msoEncodingKOI8U = 0x556a;
static const Word msoEncodingEuropa3 = 0x7149;
static const Word msoEncodingHZGBSimplifiedChinese = 0xcec8;
static const Word msoEncodingUTF7 = 0xfde8;
static const Word msoEncodingUTF8 = 0xfde9;
static const Shortint msoHTMLProjectOpenSourceView = 0x1;
static const Shortint msoHTMLProjectOpenTextView = 0x2;
static const Shortint msoHTMLProjectStateDocumentLocked = 0x1;
static const Shortint msoHTMLProjectStateProjectLocked = 0x2;
static const Shortint msoHTMLProjectStateDocumentProjectUnlocked = 0x3;
static const Shortint msoFileDialogOpen = 0x1;
static const Shortint msoFileDialogSaveAs = 0x2;
static const Shortint msoFileDialogFilePicker = 0x3;
static const Shortint msoFileDialogFolderPicker = 0x4;
static const Shortint msoFileDialogViewList = 0x1;
static const Shortint msoFileDialogViewDetails = 0x2;
static const Shortint msoFileDialogViewProperties = 0x3;
static const Shortint msoFileDialogViewPreview = 0x4;
static const Shortint msoFileDialogViewThumbnail = 0x5;
static const Shortint msoFileDialogViewLargeIcons = 0x6;
static const Shortint msoFileDialogViewSmallIcons = 0x7;
static const Shortint msoFileDialogViewWebView = 0x8;
static const Shortint msoAutomationSecurityLow = 0x1;
static const Shortint msoAutomationSecurityByUI = 0x2;
static const Shortint msoAutomationSecurityForceDisable = 0x3;
static const Shortint mfPlainText = 0x1;
static const Shortint mfHTML = 0x2;
static const Shortint mfRTF = 0x3;
static const Shortint msoAlertButtonOK = 0x0;
static const Shortint msoAlertButtonOKCancel = 0x1;
static const Shortint msoAlertButtonAbortRetryIgnore = 0x2;
static const Shortint msoAlertButtonYesNoCancel = 0x3;
static const Shortint msoAlertButtonYesNo = 0x4;
static const Shortint msoAlertButtonRetryCancel = 0x5;
static const Shortint msoAlertButtonYesAllNoCancel = 0x6;
static const Shortint msoAlertIconNoIcon = 0x0;
static const Shortint msoAlertIconCritical = 0x1;
static const Shortint msoAlertIconQuery = 0x2;
static const Shortint msoAlertIconWarning = 0x3;
static const Shortint msoAlertIconInfo = 0x4;
static const Shortint msoAlertDefaultFirst = 0x0;
static const Shortint msoAlertDefaultSecond = 0x1;
static const Shortint msoAlertDefaultThird = 0x2;
static const Shortint msoAlertDefaultFourth = 0x3;
static const Shortint msoAlertDefaultFifth = 0x4;
static const unsigned msoAlertCancelDefault = 0xffffffff;
static const Shortint msoAlertCancelFirst = 0x0;
static const Shortint msoAlertCancelSecond = 0x1;
static const Shortint msoAlertCancelThird = 0x2;
static const Shortint msoAlertCancelFourth = 0x3;
static const Shortint msoAlertCancelFifth = 0x4;
static const Shortint msoSearchInMyComputer = 0x0;
static const Shortint msoSearchInOutlook = 0x1;
static const Shortint msoSearchInMyNetworkPlaces = 0x2;
static const Shortint msoSearchInCustom = 0x3;
static const Shortint msoTargetBrowserV3 = 0x0;
static const Shortint msoTargetBrowserV4 = 0x1;
static const Shortint msoTargetBrowserIE4 = 0x2;
static const Shortint msoTargetBrowserIE5 = 0x3;
static const Shortint msoTargetBrowserIE6 = 0x4;
static const unsigned msoOrgChartOrientationMixed = 0xfffffffe;
static const Shortint msoOrgChartOrientationVertical = 0x1;
static const unsigned msoOrgChartLayoutMixed = 0xfffffffe;
static const Shortint msoOrgChartLayoutStandard = 0x1;
static const Shortint msoOrgChartLayoutBothHanging = 0x2;
static const Shortint msoOrgChartLayoutLeftHanging = 0x3;
static const Shortint msoOrgChartLayoutRightHanging = 0x4;
static const Shortint msoBeforeNode = 0x1;
static const Shortint msoAfterNode = 0x2;
static const Shortint msoBeforeFirstSibling = 0x3;
static const Shortint msoAfterLastSibling = 0x4;
static const unsigned msoDiagramMixed = 0xfffffffe;
static const Shortint msoDiagramOrgChart = 0x1;
static const Shortint msoDiagramCycle = 0x2;
static const Shortint msoDiagramRadial = 0x3;
static const Shortint msoDiagramPyramid = 0x4;
static const Shortint msoDiagramVenn = 0x5;
static const Shortint msoDiagramTarget = 0x6;
static const Shortint msoDiagramNode = 0x1;
static const Shortint msoDiagramAssistant = 0x2;
static const unsigned msoMoveRowFirst = 0xfffffffc;
static const unsigned msoMoveRowPrev = 0xfffffffd;
static const unsigned msoMoveRowNext = 0xfffffffe;
static const unsigned msoMoveRowNbr = 0xffffffff;
static const Shortint msoFilterComparisonEqual = 0x0;
static const Shortint msoFilterComparisonNotEqual = 0x1;
static const Shortint msoFilterComparisonLessThan = 0x2;
static const Shortint msoFilterComparisonGreaterThan = 0x3;
static const Shortint msoFilterComparisonLessThanEqual = 0x4;
static const Shortint msoFilterComparisonGreaterThanEqual = 0x5;
static const Shortint msoFilterComparisonIsBlank = 0x6;
static const Shortint msoFilterComparisonIsNotBlank = 0x7;
static const Shortint msoFilterComparisonContains = 0x8;
static const Shortint msoFilterComparisonNotContains = 0x9;
static const Shortint msoFilterConjunctionAnd = 0x0;
static const Shortint msoFilterConjunctionOr = 0x1;
static const Shortint msoOpenDocument = 0x0;
static const Shortint msoNew = 0x1;
static const Shortint msoNewfromExistingFile = 0x2;
static const Shortint msoNewfromTemplate = 0x3;
static const Shortint msoBottomSection = 0x4;
static const Shortint msoEditFile = 0x0;
static const Shortint msoCreateNewFile = 0x1;
static const Shortint msoOpenFile = 0x2;
static const Word msoLanguageIDChineseHongKong = 0xc04;
static const Word msoLanguageIDChineseMacao = 0x1404;
static const Word msoLanguageIDEnglishTrinidad = 0x2c09;

}	/* namespace Opofcxp */
using namespace Opofcxp;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpOfcXP
