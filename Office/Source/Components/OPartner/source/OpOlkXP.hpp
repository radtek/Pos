// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpOlkXP.pas' rev: 6.00

#ifndef OpOlkXPHPP
#define OpOlkXPHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <OpShared.hpp>	// Pascal unit
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

namespace Opolkxp
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TOpOutlookBase;
class PASCALIMPLEMENTATION TOpOutlookBase : public Opshared::TOpOfficeComponent 
{
	typedef Opshared::TOpOfficeComponent inherited;
	
public:
	#pragma option push -w-inl
	/* TOpOfficeComponent.Create */ inline __fastcall virtual TOpOutlookBase(Classes::TComponent* AOwner) : Opshared::TOpOfficeComponent(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TOpOfficeComponent.Destroy */ inline __fastcall virtual ~TOpOutlookBase(void) { }
	#pragma option pop
	
};


typedef Activex::TOleEnum OlActionCopyLike;

typedef Activex::TOleEnum OlActionReplyStyle;

typedef Activex::TOleEnum OlActionResponseStyle;

typedef Activex::TOleEnum OlActionShowOn;

typedef Activex::TOleEnum OlAttachmentType;

typedef Activex::TOleEnum OlBusyStatus;

typedef Activex::TOleEnum OlDaysOfWeek;

typedef Activex::TOleEnum OlDefaultFolders;

typedef Activex::TOleEnum OlDisplayType;

typedef Activex::TOleEnum OlEditorType;

typedef Activex::TOleEnum OlFlagStatus;

typedef Activex::TOleEnum OlFolderDisplayMode;

typedef Activex::TOleEnum OlFormRegistry;

typedef Activex::TOleEnum OlGender;

typedef Activex::TOleEnum OlImportance;

typedef Activex::TOleEnum OlInspectorClose;

typedef Activex::TOleEnum OlItemType;

typedef Activex::TOleEnum OlJournalRecipientType;

typedef Activex::TOleEnum OlMailingAddress;

typedef Activex::TOleEnum OlMailRecipientType;

typedef Activex::TOleEnum OlMeetingRecipientType;

typedef Activex::TOleEnum OlMeetingResponse;

typedef Activex::TOleEnum OlMeetingStatus;

typedef Activex::TOleEnum OlNetMeetingType;

typedef Activex::TOleEnum OlNoteColor;

typedef Activex::TOleEnum OlObjectClass;

typedef Activex::TOleEnum OlOutlookBarViewType;

typedef Activex::TOleEnum OlPane;

typedef Activex::TOleEnum OlRecurrenceState;

typedef Activex::TOleEnum OlRecurrenceType;

typedef Activex::TOleEnum OlRemoteStatus;

typedef Activex::TOleEnum OlResponseStatus;

typedef Activex::TOleEnum OlSaveAsType;

typedef Activex::TOleEnum OlSensitivity;

typedef Activex::TOleEnum OlSortOrder;

typedef Activex::TOleEnum OlTaskDelegationState;

typedef Activex::TOleEnum OlTaskOwnership;

typedef Activex::TOleEnum OlTaskRecipientType;

typedef Activex::TOleEnum OlTaskResponse;

typedef Activex::TOleEnum OlTaskStatus;

typedef Activex::TOleEnum OlTrackingStatus;

typedef Activex::TOleEnum OlUserPropertyType;

typedef Activex::TOleEnum OlWindowState;

typedef Activex::TOleEnum OlSyncState;

typedef Activex::TOleEnum OlBodyFormat;

typedef Activex::TOleEnum OlDownloadState;

typedef Activex::TOleEnum OlOfficeDocItemsType;

typedef Activex::TOleEnum OlViewSaveOption;

typedef Activex::TOleEnum OlViewType;

__interface _SyncObject;
typedef System::DelphiInterface<_SyncObject> _di__SyncObject;
typedef _SyncObject SyncObject;
;

__interface _Explorer;
typedef System::DelphiInterface<_Explorer> _di__Explorer;
typedef _Explorer Explorer;
;

__interface _Inspector;
typedef System::DelphiInterface<_Inspector> _di__Inspector;
typedef _Inspector Inspector;
;

__interface _MailItem;
typedef System::DelphiInterface<_MailItem> _di__MailItem;
typedef _MailItem MailItem;
;

__interface _AppointmentItem;
typedef System::DelphiInterface<_AppointmentItem> _di__AppointmentItem;
typedef _AppointmentItem AppointmentItem;
;

__interface _MeetingItem;
typedef System::DelphiInterface<_MeetingItem> _di__MeetingItem;
typedef _MeetingItem MeetingItem;
;

__interface _TaskItem;
typedef System::DelphiInterface<_TaskItem> _di__TaskItem;
typedef _TaskItem TaskItem;
;

__interface _Application;
typedef System::DelphiInterface<_Application> _di__Application;
typedef _Application OutlookApplication;
;

__interface _ContactItem;
typedef System::DelphiInterface<_ContactItem> _di__ContactItem;
typedef _ContactItem ContactItem;
;

__interface _DistListItem;
typedef System::DelphiInterface<_DistListItem> _di__DistListItem;
typedef _DistListItem DistListItem;
;

__interface _DocumentItem;
typedef System::DelphiInterface<_DocumentItem> _di__DocumentItem;
typedef _DocumentItem DocumentItem;
;

__interface _Explorers;
typedef System::DelphiInterface<_Explorers> _di__Explorers;
typedef _Explorers Explorers;
;

__interface _Inspectors;
typedef System::DelphiInterface<_Inspectors> _di__Inspectors;
typedef _Inspectors Inspectors;
;

__interface _Folders;
typedef System::DelphiInterface<_Folders> _di__Folders;
typedef _Folders Folders;
;

__interface _Items;
typedef System::DelphiInterface<_Items> _di__Items;
typedef _Items Items;
;

__interface _JournalItem;
typedef System::DelphiInterface<_JournalItem> _di__JournalItem;
typedef _JournalItem JournalItem;
;

__interface _NameSpace;
typedef System::DelphiInterface<_NameSpace> _di__NameSpace;
typedef _NameSpace NameSpace;
;

__interface _NoteItem;
typedef System::DelphiInterface<_NoteItem> _di__NoteItem;
typedef _NoteItem NoteItem;
;

__interface _OutlookBarGroups;
typedef System::DelphiInterface<_OutlookBarGroups> _di__OutlookBarGroups;
typedef _OutlookBarGroups OutlookBarGroups;
;

__interface _OutlookBarPane;
typedef System::DelphiInterface<_OutlookBarPane> _di__OutlookBarPane;
typedef _OutlookBarPane OutlookBarPane;
;

__interface _OutlookBarShortcuts;
typedef System::DelphiInterface<_OutlookBarShortcuts> _di__OutlookBarShortcuts;
typedef _OutlookBarShortcuts OutlookBarShortcuts;
;

__interface _PostItem;
typedef System::DelphiInterface<_PostItem> _di__PostItem;
typedef _PostItem PostItem;
;

__interface _RemoteItem;
typedef System::DelphiInterface<_RemoteItem> _di__RemoteItem;
typedef _RemoteItem RemoteItem;
;

__interface _ReportItem;
typedef System::DelphiInterface<_ReportItem> _di__ReportItem;
typedef _ReportItem ReportItem;
;

__interface _TaskRequestAcceptItem;
typedef System::DelphiInterface<_TaskRequestAcceptItem> _di__TaskRequestAcceptItem;
typedef _TaskRequestAcceptItem TaskRequestAcceptItem;
;

__interface _TaskRequestDeclineItem;
typedef System::DelphiInterface<_TaskRequestDeclineItem> _di__TaskRequestDeclineItem;
typedef _TaskRequestDeclineItem TaskRequestDeclineItem;
;

__interface _TaskRequestItem;
typedef System::DelphiInterface<_TaskRequestItem> _di__TaskRequestItem;
typedef _TaskRequestItem TaskRequestItem;
;

__interface _TaskRequestUpdateItem;
typedef System::DelphiInterface<_TaskRequestUpdateItem> _di__TaskRequestUpdateItem;
typedef _TaskRequestUpdateItem TaskRequestUpdateItem;
;

__dispinterface _DRecipientControl;
typedef System::DelphiInterface<_DRecipientControl> _di__DRecipientControl;
typedef _DRecipientControl _RecipientControl;
;

__dispinterface _DDocSiteControl;
typedef System::DelphiInterface<_DDocSiteControl> _di__DDocSiteControl;
typedef _DDocSiteControl _DocSiteControl;
;

__interface _IDpxCtrl;
typedef System::DelphiInterface<_IDpxCtrl> _di__IDpxCtrl;
typedef _IDpxCtrl _DpxCtrl;
;

__dispinterface _DPageWrapCtrl;
typedef System::DelphiInterface<_DPageWrapCtrl> _di__DPageWrapCtrl;
typedef _DPageWrapCtrl _PageWrapCtrl;
;

__dispinterface _DInspectorCtrl;
typedef System::DelphiInterface<_DInspectorCtrl> _di__DInspectorCtrl;
typedef _DInspectorCtrl _InspectorCtrl;
;

__dispinterface DDataSourceObject;
typedef System::DelphiInterface<DDataSourceObject> _di_DDataSourceObject;
typedef DDataSourceObject DataSourceObject;
;

__interface _Results;
typedef System::DelphiInterface<_Results> _di__Results;
typedef _Results Results;
;

__interface _Views;
typedef System::DelphiInterface<_Views> _di__Views;
typedef _Views Views;
;

__interface _Reminder;
typedef System::DelphiInterface<_Reminder> _di__Reminder;
typedef _Reminder Reminder;
;

__interface _Reminders;
typedef System::DelphiInterface<_Reminders> _di__Reminders;
typedef _Reminders Reminders;
;

typedef _Application Application_;
;

typedef OleVariant *POleVariant1;

typedef Word *PWordBool1;

typedef GUID *PUserType1;

typedef Shortint *PShortint1;

typedef PShortint1 *PPShortint1;

typedef GUID *PUserType2;

__interface Action;
typedef System::DelphiInterface<Action> _di_Action;
__interface INTERFACE_UUID("{00063043-0000-0000-C000-000000000046}") Action  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_CopyLike(Activex::TOleEnum &Get_CopyLike_result) = 0 ;
	virtual HRESULT __safecall Set_CopyLike(Activex::TOleEnum CopyLike) = 0 ;
	virtual HRESULT __safecall Get_Enabled(Word &Get_Enabled_result) = 0 ;
	virtual HRESULT __safecall Set_Enabled(Word Enabled) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString Name) = 0 ;
	virtual HRESULT __safecall Get_Prefix(WideString &Get_Prefix_result) = 0 ;
	virtual HRESULT __safecall Set_Prefix(const WideString Prefix) = 0 ;
	virtual HRESULT __safecall Get_ReplyStyle(Activex::TOleEnum &Get_ReplyStyle_result) = 0 ;
	virtual HRESULT __safecall Set_ReplyStyle(Activex::TOleEnum ReplyStyle) = 0 ;
	virtual HRESULT __safecall Get_ResponseStyle(Activex::TOleEnum &Get_ResponseStyle_result) = 0 ;
	virtual HRESULT __safecall Set_ResponseStyle(Activex::TOleEnum ResponseStyle) = 0 ;
	virtual HRESULT __safecall Get_ShowOn(Activex::TOleEnum &Get_ShowOn_result) = 0 ;
	virtual HRESULT __safecall Set_ShowOn(Activex::TOleEnum ShowOn) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Execute(_di_IDispatch &Execute_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_CopyLike() { Activex::TOleEnum r; HRESULT hr = Get_CopyLike(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum CopyLike = {read=_scw_Get_CopyLike, write=Set_CopyLike};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Enabled() { Word r; HRESULT hr = Get_Enabled(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Enabled = {read=_scw_Get_Enabled, write=Set_Enabled};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Prefix() { WideString r; HRESULT hr = Get_Prefix(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Prefix = {read=_scw_Get_Prefix, write=Set_Prefix};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_ReplyStyle() { Activex::TOleEnum r; HRESULT hr = Get_ReplyStyle(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum ReplyStyle = {read=_scw_Get_ReplyStyle, write=Set_ReplyStyle};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_ResponseStyle() { Activex::TOleEnum r; HRESULT hr = Get_ResponseStyle(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum ResponseStyle = {read=_scw_Get_ResponseStyle, write=Set_ResponseStyle};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_ShowOn() { Activex::TOleEnum r; HRESULT hr = Get_ShowOn(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum ShowOn = {read=_scw_Get_ShowOn, write=Set_ShowOn};
};

__dispinterface ActionDisp;
typedef System::DelphiInterface<ActionDisp> _di_ActionDisp;
__dispinterface INTERFACE_UUID("{00063043-0000-0000-C000-000000000046}") ActionDisp  : public IDispatch 
{
	
};

__interface Search;
typedef System::DelphiInterface<Search> _di_Search;
__interface INTERFACE_UUID("{00063001-0000-0000-C000-000000000046}") _Application  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Assistant(Opofcxp::_di_Assistant &Get_Assistant_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Version(WideString &Get_Version_result) = 0 ;
	virtual HRESULT __safecall ActiveExplorer(_di__Explorer &ActiveExplorer_result) = 0 ;
	virtual HRESULT __safecall ActiveInspector(_di__Inspector &ActiveInspector_result) = 0 ;
	virtual HRESULT __safecall CreateItem(Activex::TOleEnum ItemType, _di_IDispatch &CreateItem_result) = 0 ;
	virtual HRESULT __safecall CreateItemFromTemplate(const WideString TemplatePath, const OleVariant InFolder, _di_IDispatch &CreateItemFromTemplate_result) = 0 ;
	virtual HRESULT __safecall CreateObject(const WideString ObjectName, _di_IDispatch &CreateObject_result) = 0 ;
	virtual HRESULT __safecall GetNamespace(const WideString Type_, _di__NameSpace &GetNamespace_result) = 0 ;
	virtual HRESULT __safecall Quit(void) = 0 ;
	virtual HRESULT __safecall Get_COMAddIns(Opofcxp::_di_COMAddIns &Get_COMAddIns_result) = 0 ;
	virtual HRESULT __safecall Get_Explorers(_di__Explorers &Get_Explorers_result) = 0 ;
	virtual HRESULT __safecall Get_Inspectors(_di__Inspectors &Get_Inspectors_result) = 0 ;
	virtual HRESULT __safecall Get_LanguageSettings(Opofcxp::_di_LanguageSettings &Get_LanguageSettings_result) = 0 ;
	virtual HRESULT __safecall Get_ProductCode(WideString &Get_ProductCode_result) = 0 ;
	virtual HRESULT __safecall Get_AnswerWizard(Opofcxp::_di_AnswerWizard &Get_AnswerWizard_result) = 0 ;
	virtual HRESULT __safecall Get_FeatureInstall(Activex::TOleEnum &Get_FeatureInstall_result) = 0 ;
	virtual HRESULT __safecall Set_FeatureInstall(Activex::TOleEnum FeatureInstall) = 0 ;
	virtual HRESULT __safecall ActiveWindow(_di_IDispatch &ActiveWindow_result) = 0 ;
	virtual HRESULT __safecall CopyFile(const WideString FilePath, const WideString DestFolderPath, _di_IDispatch &CopyFile_result) = 0 ;
	virtual HRESULT __safecall AdvancedSearch(const WideString Scope, const OleVariant Filter, const OleVariant SearchSubFolders, const OleVariant Tag, _di_Search &AdvancedSearch_result) = 0 ;
	virtual HRESULT __safecall IsSearchSynchronous(const WideString LookInFolders, Word &IsSearchSynchronous_result) = 0 ;
	virtual HRESULT __safecall GetNewNickNames(OleVariant &pvar) = 0 ;
	virtual HRESULT __safecall Get_Reminders(_di__Reminders &Get_Reminders_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opofcxp::_di_Assistant _scw_Get_Assistant() { Opofcxp::_di_Assistant r; HRESULT hr = Get_Assistant(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opofcxp::_di_Assistant Assistant = {read=_scw_Get_Assistant};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Version() { WideString r; HRESULT hr = Get_Version(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Version = {read=_scw_Get_Version};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opofcxp::_di_COMAddIns _scw_Get_COMAddIns() { Opofcxp::_di_COMAddIns r; HRESULT hr = Get_COMAddIns(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opofcxp::_di_COMAddIns COMAddIns = {read=_scw_Get_COMAddIns};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Explorers _scw_Get_Explorers() { _di__Explorers r; HRESULT hr = Get_Explorers(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Explorers Explorers = {read=_scw_Get_Explorers};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspectors _scw_Get_Inspectors() { _di__Inspectors r; HRESULT hr = Get_Inspectors(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspectors Inspectors = {read=_scw_Get_Inspectors};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opofcxp::_di_LanguageSettings _scw_Get_LanguageSettings() { Opofcxp::_di_LanguageSettings r; HRESULT hr = Get_LanguageSettings(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opofcxp::_di_LanguageSettings LanguageSettings = {read=_scw_Get_LanguageSettings};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ProductCode() { WideString r; HRESULT hr = Get_ProductCode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ProductCode = {read=_scw_Get_ProductCode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opofcxp::_di_AnswerWizard _scw_Get_AnswerWizard() { Opofcxp::_di_AnswerWizard r; HRESULT hr = Get_AnswerWizard(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opofcxp::_di_AnswerWizard AnswerWizard = {read=_scw_Get_AnswerWizard};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_FeatureInstall() { Activex::TOleEnum r; HRESULT hr = Get_FeatureInstall(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum FeatureInstall = {read=_scw_Get_FeatureInstall, write=Set_FeatureInstall};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Reminders _scw_Get_Reminders() { _di__Reminders r; HRESULT hr = Get_Reminders(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Reminders Reminders = {read=_scw_Get_Reminders};
};

__dispinterface _ApplicationDisp;
typedef System::DelphiInterface<_ApplicationDisp> _di__ApplicationDisp;
__dispinterface INTERFACE_UUID("{00063001-0000-0000-C000-000000000046}") _ApplicationDisp  : public IDispatch 
{
	
};

__interface Recipient;
typedef System::DelphiInterface<Recipient> _di_Recipient;
__interface AddressLists;
typedef System::DelphiInterface<AddressLists> _di_AddressLists;
__interface MAPIFolder;
typedef System::DelphiInterface<MAPIFolder> _di_MAPIFolder;
__interface SyncObjects;
typedef System::DelphiInterface<SyncObjects> _di_SyncObjects;
__interface INTERFACE_UUID("{00063002-0000-0000-C000-000000000046}") _NameSpace  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_CurrentUser(_di_Recipient &Get_CurrentUser_result) = 0 ;
	virtual HRESULT __safecall Get_Folders(_di__Folders &Get_Folders_result) = 0 ;
	virtual HRESULT __safecall Get_Type_(WideString &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_AddressLists(_di_AddressLists &Get_AddressLists_result) = 0 ;
	virtual HRESULT __safecall CreateRecipient(const WideString RecipientName, _di_Recipient &CreateRecipient_result) = 0 ;
	virtual HRESULT __safecall GetDefaultFolder(Activex::TOleEnum FolderType, _di_MAPIFolder &GetDefaultFolder_result) = 0 ;
	virtual HRESULT __safecall GetFolderFromID(const WideString EntryIDFolder, const OleVariant EntryIDStore, _di_MAPIFolder &GetFolderFromID_result) = 0 ;
	virtual HRESULT __safecall GetItemFromID(const WideString EntryIDItem, const OleVariant EntryIDStore, _di_IDispatch &GetItemFromID_result) = 0 ;
	virtual HRESULT __safecall GetRecipientFromID(const WideString EntryID, _di_Recipient &GetRecipientFromID_result) = 0 ;
	virtual HRESULT __safecall GetSharedDefaultFolder(const _di_Recipient Recipient, Activex::TOleEnum FolderType, _di_MAPIFolder &GetSharedDefaultFolder_result) = 0 ;
	virtual HRESULT __safecall Logoff(void) = 0 ;
	virtual HRESULT __safecall Logon(const OleVariant Profile, const OleVariant Password, const OleVariant ShowDialog, const OleVariant NewSession) = 0 ;
	virtual HRESULT __safecall PickFolder(_di_MAPIFolder &PickFolder_result) = 0 ;
	virtual HRESULT __safecall RefreshRemoteHeaders(void) = 0 ;
	virtual HRESULT __safecall Get_SyncObjects(_di_SyncObjects &Get_SyncObjects_result) = 0 ;
	virtual HRESULT __safecall AddStore(const OleVariant Store) = 0 ;
	virtual HRESULT __safecall RemoveStore(const _di_MAPIFolder Folder) = 0 ;
	virtual HRESULT __safecall Get_Offline(Word &Get_Offline_result) = 0 ;
	virtual HRESULT __safecall Dial(const OleVariant ContactItem) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Recipient _scw_Get_CurrentUser() { _di_Recipient r; HRESULT hr = Get_CurrentUser(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Recipient CurrentUser = {read=_scw_Get_CurrentUser};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Folders _scw_Get_Folders() { _di__Folders r; HRESULT hr = Get_Folders(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Folders Folders = {read=_scw_Get_Folders};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Type_() { WideString r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_AddressLists _scw_Get_AddressLists() { _di_AddressLists r; HRESULT hr = Get_AddressLists(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_AddressLists AddressLists = {read=_scw_Get_AddressLists};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_SyncObjects _scw_Get_SyncObjects() { _di_SyncObjects r; HRESULT hr = Get_SyncObjects(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_SyncObjects SyncObjects = {read=_scw_Get_SyncObjects};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Offline() { Word r; HRESULT hr = Get_Offline(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Offline = {read=_scw_Get_Offline};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
};

__dispinterface _NameSpaceDisp;
typedef System::DelphiInterface<_NameSpaceDisp> _di__NameSpaceDisp;
__dispinterface INTERFACE_UUID("{00063002-0000-0000-C000-000000000046}") _NameSpaceDisp  : public IDispatch 
{
	
};

__interface AddressEntry;
typedef System::DelphiInterface<AddressEntry> _di_AddressEntry;
__interface INTERFACE_UUID("{00063045-0000-0000-C000-000000000046}") Recipient  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Address(WideString &Get_Address_result) = 0 ;
	virtual HRESULT __safecall Get_AddressEntry(_di_AddressEntry &Get_AddressEntry_result) = 0 ;
	virtual HRESULT __safecall Set_AddressEntry(const _di_AddressEntry AddressEntry) = 0 ;
	virtual HRESULT __safecall Get_AutoResponse(WideString &Get_AutoResponse_result) = 0 ;
	virtual HRESULT __safecall Set_AutoResponse(const WideString AutoResponse) = 0 ;
	virtual HRESULT __safecall Get_DisplayType(Activex::TOleEnum &Get_DisplayType_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_Index(int &Get_Index_result) = 0 ;
	virtual HRESULT __safecall Get_MeetingResponseStatus(Activex::TOleEnum &Get_MeetingResponseStatus_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Resolved(Word &Get_Resolved_result) = 0 ;
	virtual HRESULT __safecall Get_TrackingStatus(Activex::TOleEnum &Get_TrackingStatus_result) = 0 ;
	virtual HRESULT __safecall Set_TrackingStatus(Activex::TOleEnum TrackingStatus) = 0 ;
	virtual HRESULT __safecall Get_TrackingStatusTime(System::TDateTime &Get_TrackingStatusTime_result) = 0 ;
	virtual HRESULT __safecall Set_TrackingStatusTime(System::TDateTime TrackingStatusTime) = 0 ;
	virtual HRESULT __safecall Get_Type_(int &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Set_Type_(int Type_) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall FreeBusy(System::TDateTime Start, int MinPerChar, const OleVariant CompleteFormat, WideString &FreeBusy_result) = 0 ;
	virtual HRESULT __safecall Resolve(Word &Resolve_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Address() { WideString r; HRESULT hr = Get_Address(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Address = {read=_scw_Get_Address};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_AddressEntry _scw_Get_AddressEntry() { _di_AddressEntry r; HRESULT hr = Get_AddressEntry(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_AddressEntry AddressEntry = {read=_scw_Get_AddressEntry, write=Set_AddressEntry};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_AutoResponse() { WideString r; HRESULT hr = Get_AutoResponse(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString AutoResponse = {read=_scw_Get_AutoResponse, write=Set_AutoResponse};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DisplayType() { Activex::TOleEnum r; HRESULT hr = Get_DisplayType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DisplayType = {read=_scw_Get_DisplayType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Index() { int r; HRESULT hr = Get_Index(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Index = {read=_scw_Get_Index};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MeetingResponseStatus() { Activex::TOleEnum r; HRESULT hr = Get_MeetingResponseStatus(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MeetingResponseStatus = {read=_scw_Get_MeetingResponseStatus};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Resolved() { Word r; HRESULT hr = Get_Resolved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Resolved = {read=_scw_Get_Resolved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_TrackingStatus() { Activex::TOleEnum r; HRESULT hr = Get_TrackingStatus(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum TrackingStatus = {read=_scw_Get_TrackingStatus, write=Set_TrackingStatus};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_TrackingStatusTime() { System::TDateTime r; HRESULT hr = Get_TrackingStatusTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime TrackingStatusTime = {read=_scw_Get_TrackingStatusTime, write=Set_TrackingStatusTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Type_() { int r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Type_ = {read=_scw_Get_Type_, write=Set_Type_};
};

__dispinterface RecipientDisp;
typedef System::DelphiInterface<RecipientDisp> _di_RecipientDisp;
__dispinterface INTERFACE_UUID("{00063045-0000-0000-C000-000000000046}") RecipientDisp  : public IDispatch 
{
	
};

__interface AddressEntries;
typedef System::DelphiInterface<AddressEntries> _di_AddressEntries;
__interface INTERFACE_UUID("{0006304B-0000-0000-C000-000000000046}") AddressEntry  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Address(WideString &Get_Address_result) = 0 ;
	virtual HRESULT __safecall Set_Address(const WideString Address) = 0 ;
	virtual HRESULT __safecall Get_DisplayType(Activex::TOleEnum &Get_DisplayType_result) = 0 ;
	virtual HRESULT __safecall Get_ID(WideString &Get_ID_result) = 0 ;
	virtual HRESULT __safecall Get_Manager(_di_AddressEntry &Get_Manager_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Set_MAPIOBJECT(const System::_di_IInterface MAPIOBJECT) = 0 ;
	virtual HRESULT __safecall Get_Members(_di_AddressEntries &Get_Members_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString Name) = 0 ;
	virtual HRESULT __safecall Get_Type_(WideString &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Set_Type_(const WideString Type_) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Details(const OleVariant HWnd) = 0 ;
	virtual HRESULT __safecall GetFreeBusy(System::TDateTime Start, int MinPerChar, const OleVariant CompleteFormat, WideString &GetFreeBusy_result) = 0 ;
	virtual HRESULT __safecall Update(const OleVariant MakePermanent, const OleVariant Refresh) = 0 ;
	virtual HRESULT __safecall UpdateFreeBusy(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Address() { WideString r; HRESULT hr = Get_Address(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Address = {read=_scw_Get_Address, write=Set_Address};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DisplayType() { Activex::TOleEnum r; HRESULT hr = Get_DisplayType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DisplayType = {read=_scw_Get_DisplayType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ID() { WideString r; HRESULT hr = Get_ID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ID = {read=_scw_Get_ID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_AddressEntry _scw_Get_Manager() { _di_AddressEntry r; HRESULT hr = Get_Manager(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_AddressEntry Manager = {read=_scw_Get_Manager};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT, write=Set_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_AddressEntries _scw_Get_Members() { _di_AddressEntries r; HRESULT hr = Get_Members(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_AddressEntries Members = {read=_scw_Get_Members};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Type_() { WideString r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Type_ = {read=_scw_Get_Type_, write=Set_Type_};
};

__dispinterface AddressEntryDisp;
typedef System::DelphiInterface<AddressEntryDisp> _di_AddressEntryDisp;
__dispinterface INTERFACE_UUID("{0006304B-0000-0000-C000-000000000046}") AddressEntryDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0006304A-0000-0000-C000-000000000046}") AddressEntries  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_AddressEntry &Item_result) = 0 ;
	virtual HRESULT __safecall Get_RawTable(System::_di_IInterface &Get_RawTable_result) = 0 ;
	virtual HRESULT __safecall Add(const WideString Type_, const OleVariant Name, const OleVariant Address, _di_AddressEntry &Add_result) = 0 ;
	virtual HRESULT __safecall GetFirst(_di_AddressEntry &GetFirst_result) = 0 ;
	virtual HRESULT __safecall GetLast(_di_AddressEntry &GetLast_result) = 0 ;
	virtual HRESULT __safecall GetNext(_di_AddressEntry &GetNext_result) = 0 ;
	virtual HRESULT __safecall GetPrevious(_di_AddressEntry &GetPrevious_result) = 0 ;
	virtual HRESULT __safecall Sort(const OleVariant Property_, const OleVariant Order) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_RawTable() { System::_di_IInterface r; HRESULT hr = Get_RawTable(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface RawTable = {read=_scw_Get_RawTable};
};

__dispinterface AddressEntriesDisp;
typedef System::DelphiInterface<AddressEntriesDisp> _di_AddressEntriesDisp;
__dispinterface INTERFACE_UUID("{0006304A-0000-0000-C000-000000000046}") AddressEntriesDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063040-0000-0000-C000-000000000046}") _Folders  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_MAPIFolder &Item_result) = 0 ;
	virtual HRESULT __safecall Get_RawTable(System::_di_IInterface &Get_RawTable_result) = 0 ;
	virtual HRESULT __safecall Add(const WideString Name, const OleVariant Type_, _di_MAPIFolder &Add_result) = 0 ;
	virtual HRESULT __safecall GetFirst(_di_MAPIFolder &GetFirst_result) = 0 ;
	virtual HRESULT __safecall GetLast(_di_MAPIFolder &GetLast_result) = 0 ;
	virtual HRESULT __safecall GetNext(_di_MAPIFolder &GetNext_result) = 0 ;
	virtual HRESULT __safecall GetPrevious(_di_MAPIFolder &GetPrevious_result) = 0 ;
	virtual HRESULT __safecall Remove(int Index) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_RawTable() { System::_di_IInterface r; HRESULT hr = Get_RawTable(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface RawTable = {read=_scw_Get_RawTable};
};

__dispinterface _FoldersDisp;
typedef System::DelphiInterface<_FoldersDisp> _di__FoldersDisp;
__dispinterface INTERFACE_UUID("{00063040-0000-0000-C000-000000000046}") _FoldersDisp  : public IDispatch 
{
	
};

__interface View;
typedef System::DelphiInterface<View> _di_View;
__interface INTERFACE_UUID("{00063006-0000-0000-C000-000000000046}") MAPIFolder  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_DefaultItemType(Activex::TOleEnum &Get_DefaultItemType_result) = 0 ;
	virtual HRESULT __safecall Get_DefaultMessageClass(WideString &Get_DefaultMessageClass_result) = 0 ;
	virtual HRESULT __safecall Get_Description(WideString &Get_Description_result) = 0 ;
	virtual HRESULT __safecall Set_Description(const WideString Description) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_Folders(_di__Folders &Get_Folders_result) = 0 ;
	virtual HRESULT __safecall Get_Items(_di__Items &Get_Items_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString Name) = 0 ;
	virtual HRESULT __safecall Get_StoreID(WideString &Get_StoreID_result) = 0 ;
	virtual HRESULT __safecall Get_UnReadItemCount(int &Get_UnReadItemCount_result) = 0 ;
	virtual HRESULT __safecall CopyTo(const _di_MAPIFolder DestinationFolder, _di_MAPIFolder &CopyTo_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(void) = 0 ;
	virtual HRESULT __safecall GetExplorer(const OleVariant DisplayMode, _di__Explorer &GetExplorer_result) = 0 ;
	virtual HRESULT __safecall MoveTo(const _di_MAPIFolder DestinationFolder) = 0 ;
	virtual HRESULT __safecall Get_UserPermissions(_di_IDispatch &Get_UserPermissions_result) = 0 ;
	virtual HRESULT __safecall Get_WebViewOn(Word &Get_WebViewOn_result) = 0 ;
	virtual HRESULT __safecall Set_WebViewOn(Word WebViewOn) = 0 ;
	virtual HRESULT __safecall Get_WebViewURL(WideString &Get_WebViewURL_result) = 0 ;
	virtual HRESULT __safecall Set_WebViewURL(const WideString WebViewURL) = 0 ;
	virtual HRESULT __safecall Get_WebViewAllowNavigation(Word &Get_WebViewAllowNavigation_result) = 0 ;
	virtual HRESULT __safecall Set_WebViewAllowNavigation(Word WebViewAllowNavigation) = 0 ;
	virtual HRESULT __safecall AddToPFFavorites(void) = 0 ;
	virtual HRESULT __safecall Get_AddressBookName(WideString &Get_AddressBookName_result) = 0 ;
	virtual HRESULT __safecall Set_AddressBookName(const WideString AddressBookName) = 0 ;
	virtual HRESULT __safecall Get_ShowAsOutlookAB(Word &Get_ShowAsOutlookAB_result) = 0 ;
	virtual HRESULT __safecall Set_ShowAsOutlookAB(Word ShowAsOutlookAB) = 0 ;
	virtual HRESULT __safecall Get_FolderPath(WideString &Get_FolderPath_result) = 0 ;
	virtual HRESULT __safecall AddToFavorites(const OleVariant fNoUI, const OleVariant Name) = 0 ;
	virtual HRESULT __safecall Get_InAppFolderSyncObject(Word &Get_InAppFolderSyncObject_result) = 0 ;
	virtual HRESULT __safecall Set_InAppFolderSyncObject(Word InAppFolderSyncObject) = 0 ;
	virtual HRESULT __safecall Get_CurrentView(_di_View &Get_CurrentView_result) = 0 ;
	virtual HRESULT __safecall Get_CustomViewsOnly(Word &Get_CustomViewsOnly_result) = 0 ;
	virtual HRESULT __safecall Set_CustomViewsOnly(Word CustomViewsOnly) = 0 ;
	virtual HRESULT __safecall Get_Views(_di__Views &Get_Views_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_FullFolderPath(WideString &Get_FullFolderPath_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DefaultItemType() { Activex::TOleEnum r; HRESULT hr = Get_DefaultItemType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DefaultItemType = {read=_scw_Get_DefaultItemType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_DefaultMessageClass() { WideString r; HRESULT hr = Get_DefaultMessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString DefaultMessageClass = {read=_scw_Get_DefaultMessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Description() { WideString r; HRESULT hr = Get_Description(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Description = {read=_scw_Get_Description, write=Set_Description};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Folders _scw_Get_Folders() { _di__Folders r; HRESULT hr = Get_Folders(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Folders Folders = {read=_scw_Get_Folders};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Items _scw_Get_Items() { _di__Items r; HRESULT hr = Get_Items(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Items Items = {read=_scw_Get_Items};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_StoreID() { WideString r; HRESULT hr = Get_StoreID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString StoreID = {read=_scw_Get_StoreID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_UnReadItemCount() { int r; HRESULT hr = Get_UnReadItemCount(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int UnReadItemCount = {read=_scw_Get_UnReadItemCount};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_UserPermissions() { _di_IDispatch r; HRESULT hr = Get_UserPermissions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch UserPermissions = {read=_scw_Get_UserPermissions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_WebViewOn() { Word r; HRESULT hr = Get_WebViewOn(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word WebViewOn = {read=_scw_Get_WebViewOn, write=Set_WebViewOn};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_WebViewURL() { WideString r; HRESULT hr = Get_WebViewURL(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString WebViewURL = {read=_scw_Get_WebViewURL, write=Set_WebViewURL};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_WebViewAllowNavigation() { Word r; HRESULT hr = Get_WebViewAllowNavigation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word WebViewAllowNavigation = {read=_scw_Get_WebViewAllowNavigation, write=Set_WebViewAllowNavigation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_AddressBookName() { WideString r; HRESULT hr = Get_AddressBookName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString AddressBookName = {read=_scw_Get_AddressBookName, write=Set_AddressBookName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ShowAsOutlookAB() { Word r; HRESULT hr = Get_ShowAsOutlookAB(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ShowAsOutlookAB = {read=_scw_Get_ShowAsOutlookAB, write=Set_ShowAsOutlookAB};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FolderPath() { WideString r; HRESULT hr = Get_FolderPath(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FolderPath = {read=_scw_Get_FolderPath};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_InAppFolderSyncObject() { Word r; HRESULT hr = Get_InAppFolderSyncObject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word InAppFolderSyncObject = {read=_scw_Get_InAppFolderSyncObject, write=Set_InAppFolderSyncObject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_View _scw_Get_CurrentView() { _di_View r; HRESULT hr = Get_CurrentView(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_View CurrentView = {read=_scw_Get_CurrentView};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_CustomViewsOnly() { Word r; HRESULT hr = Get_CustomViewsOnly(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word CustomViewsOnly = {read=_scw_Get_CustomViewsOnly, write=Set_CustomViewsOnly};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Views _scw_Get_Views() { _di__Views r; HRESULT hr = Get_Views(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Views Views = {read=_scw_Get_Views};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FullFolderPath() { WideString r; HRESULT hr = Get_FullFolderPath(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FullFolderPath = {read=_scw_Get_FullFolderPath};
};

__dispinterface MAPIFolderDisp;
typedef System::DelphiInterface<MAPIFolderDisp> _di_MAPIFolderDisp;
__dispinterface INTERFACE_UUID("{00063006-0000-0000-C000-000000000046}") MAPIFolderDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063041-0000-0000-C000-000000000046}") _Items  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_IDispatch &Item_result) = 0 ;
	virtual HRESULT __safecall Get_RawTable(System::_di_IInterface &Get_RawTable_result) = 0 ;
	virtual HRESULT __safecall Get_IncludeRecurrences(Word &Get_IncludeRecurrences_result) = 0 ;
	virtual HRESULT __safecall Set_IncludeRecurrences(Word IncludeRecurrences) = 0 ;
	virtual HRESULT __safecall Add(const OleVariant Type_, _di_IDispatch &Add_result) = 0 ;
	virtual HRESULT __safecall Find(const WideString Filter, _di_IDispatch &Find_result) = 0 ;
	virtual HRESULT __safecall FindNext(_di_IDispatch &FindNext_result) = 0 ;
	virtual HRESULT __safecall GetFirst(_di_IDispatch &GetFirst_result) = 0 ;
	virtual HRESULT __safecall GetLast(_di_IDispatch &GetLast_result) = 0 ;
	virtual HRESULT __safecall GetNext(_di_IDispatch &GetNext_result) = 0 ;
	virtual HRESULT __safecall GetPrevious(_di_IDispatch &GetPrevious_result) = 0 ;
	virtual HRESULT __safecall Remove(int Index) = 0 ;
	virtual HRESULT __safecall ResetColumns(void) = 0 ;
	virtual HRESULT __safecall Restrict(const WideString Filter, _di__Items &Restrict_result) = 0 ;
	virtual HRESULT __safecall SetColumns(const WideString Columns) = 0 ;
	virtual HRESULT __safecall Sort(const WideString Property_, const OleVariant Descending) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_RawTable() { System::_di_IInterface r; HRESULT hr = Get_RawTable(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface RawTable = {read=_scw_Get_RawTable};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IncludeRecurrences() { Word r; HRESULT hr = Get_IncludeRecurrences(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IncludeRecurrences = {read=_scw_Get_IncludeRecurrences, write=Set_IncludeRecurrences};
};

__dispinterface _ItemsDisp;
typedef System::DelphiInterface<_ItemsDisp> _di__ItemsDisp;
__dispinterface INTERFACE_UUID("{00063041-0000-0000-C000-000000000046}") _ItemsDisp  : public IDispatch 
{
	
};

__interface Panes;
typedef System::DelphiInterface<Panes> _di_Panes;
__interface Selection;
typedef System::DelphiInterface<Selection> _di_Selection;
__interface INTERFACE_UUID("{00063003-0000-0000-C000-000000000046}") _Explorer  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_CommandBars(Opofcxp::_di__CommandBars &Get_CommandBars_result) = 0 ;
	virtual HRESULT __safecall Get_CurrentFolder(_di_MAPIFolder &Get_CurrentFolder_result) = 0 ;
	virtual HRESULT __safecall Set_CurrentFolder(const _di_MAPIFolder CurrentFolder) = 0 ;
	virtual HRESULT __safecall Close(void) = 0 ;
	virtual HRESULT __safecall Display(void) = 0 ;
	virtual HRESULT __safecall Get_Caption(WideString &Get_Caption_result) = 0 ;
	virtual HRESULT __safecall Get_CurrentView(OleVariant &Get_CurrentView_result) = 0 ;
	virtual HRESULT __safecall Set_CurrentView(const OleVariant CurrentView) = 0 ;
	virtual HRESULT __safecall Get_Height(int &Get_Height_result) = 0 ;
	virtual HRESULT __safecall Set_Height(int Height) = 0 ;
	virtual HRESULT __safecall Get_Left(int &Get_Left_result) = 0 ;
	virtual HRESULT __safecall Set_Left(int Left) = 0 ;
	virtual HRESULT __safecall Get_Panes(_di_Panes &Get_Panes_result) = 0 ;
	virtual HRESULT __safecall Get_Selection(_di_Selection &Get_Selection_result) = 0 ;
	virtual HRESULT __safecall Get_Top(int &Get_Top_result) = 0 ;
	virtual HRESULT __safecall Set_Top(int Top) = 0 ;
	virtual HRESULT __safecall Get_Width(int &Get_Width_result) = 0 ;
	virtual HRESULT __safecall Set_Width(int Width) = 0 ;
	virtual HRESULT __safecall Get_WindowState(Activex::TOleEnum &Get_WindowState_result) = 0 ;
	virtual HRESULT __safecall Set_WindowState(Activex::TOleEnum WindowState) = 0 ;
	virtual HRESULT __safecall Activate(void) = 0 ;
	virtual HRESULT __safecall IsPaneVisible(Activex::TOleEnum Pane, Word &IsPaneVisible_result) = 0 ;
	virtual HRESULT __safecall ShowPane(Activex::TOleEnum Pane, Word Visible) = 0 ;
	virtual HRESULT __safecall Get_Views(_di_IDispatch &Get_Views_result) = 0 ;
	virtual HRESULT __safecall Get_HTMLDocument(_di_IDispatch &Get_HTMLDocument_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opofcxp::_di__CommandBars _scw_Get_CommandBars() { Opofcxp::_di__CommandBars r; HRESULT hr = Get_CommandBars(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opofcxp::_di__CommandBars CommandBars = {read=_scw_Get_CommandBars};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_MAPIFolder _scw_Get_CurrentFolder() { _di_MAPIFolder r; HRESULT hr = Get_CurrentFolder(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_MAPIFolder CurrentFolder = {read=_scw_Get_CurrentFolder, write=Set_CurrentFolder};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Caption() { WideString r; HRESULT hr = Get_Caption(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Caption = {read=_scw_Get_Caption};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_CurrentView() { OleVariant r; HRESULT hr = Get_CurrentView(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant CurrentView = {read=_scw_Get_CurrentView, write=Set_CurrentView};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Height() { int r; HRESULT hr = Get_Height(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Height = {read=_scw_Get_Height, write=Set_Height};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Left() { int r; HRESULT hr = Get_Left(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Left = {read=_scw_Get_Left, write=Set_Left};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Panes _scw_Get_Panes() { _di_Panes r; HRESULT hr = Get_Panes(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Panes Panes = {read=_scw_Get_Panes};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Selection _scw_Get_Selection() { _di_Selection r; HRESULT hr = Get_Selection(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Selection Selection = {read=_scw_Get_Selection};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Top() { int r; HRESULT hr = Get_Top(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Top = {read=_scw_Get_Top, write=Set_Top};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Width() { int r; HRESULT hr = Get_Width(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Width = {read=_scw_Get_Width, write=Set_Width};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_WindowState() { Activex::TOleEnum r; HRESULT hr = Get_WindowState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum WindowState = {read=_scw_Get_WindowState, write=Set_WindowState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Views() { _di_IDispatch r; HRESULT hr = Get_Views(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Views = {read=_scw_Get_Views};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_HTMLDocument() { _di_IDispatch r; HRESULT hr = Get_HTMLDocument(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch HTMLDocument = {read=_scw_Get_HTMLDocument};
};

__dispinterface _ExplorerDisp;
typedef System::DelphiInterface<_ExplorerDisp> _di__ExplorerDisp;
__dispinterface INTERFACE_UUID("{00063003-0000-0000-C000-000000000046}") _ExplorerDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063009-0000-0000-C000-000000000046}") Panes  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_IDispatch &Item_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface PanesDisp;
typedef System::DelphiInterface<PanesDisp> _di_PanesDisp;
__dispinterface INTERFACE_UUID("{00063009-0000-0000-C000-000000000046}") PanesDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063087-0000-0000-C000-000000000046}") Selection  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_IDispatch &Item_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface SelectionDisp;
typedef System::DelphiInterface<SelectionDisp> _di_SelectionDisp;
__dispinterface INTERFACE_UUID("{00063087-0000-0000-C000-000000000046}") SelectionDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063095-0000-0000-C000-000000000046}") View  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Apply(void) = 0 ;
	virtual HRESULT __safecall Copy(const WideString Name, Activex::TOleEnum SaveOption, _di_View &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Reset(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall Get_Language(WideString &Get_Language_result) = 0 ;
	virtual HRESULT __safecall Set_Language(const WideString Language) = 0 ;
	virtual HRESULT __safecall Get_LockUserChanges(Word &Get_LockUserChanges_result) = 0 ;
	virtual HRESULT __safecall Set_LockUserChanges(Word LockUserChanges) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString Name) = 0 ;
	virtual HRESULT __safecall Get_SaveOption(Activex::TOleEnum &Get_SaveOption_result) = 0 ;
	virtual HRESULT __safecall Get_Standard(Word &Get_Standard_result) = 0 ;
	virtual HRESULT __safecall Get_ViewType(Activex::TOleEnum &Get_ViewType_result) = 0 ;
	virtual HRESULT __safecall Get_XML(WideString &Get_XML_result) = 0 ;
	virtual HRESULT __safecall Set_XML(const WideString XML) = 0 ;
	virtual HRESULT __safecall GoToDate(System::TDateTime Date) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Language() { WideString r; HRESULT hr = Get_Language(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Language = {read=_scw_Get_Language, write=Set_Language};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_LockUserChanges() { Word r; HRESULT hr = Get_LockUserChanges(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word LockUserChanges = {read=_scw_Get_LockUserChanges, write=Set_LockUserChanges};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_SaveOption() { Activex::TOleEnum r; HRESULT hr = Get_SaveOption(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum SaveOption = {read=_scw_Get_SaveOption};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Standard() { Word r; HRESULT hr = Get_Standard(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Standard = {read=_scw_Get_Standard};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_ViewType() { Activex::TOleEnum r; HRESULT hr = Get_ViewType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum ViewType = {read=_scw_Get_ViewType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_XML() { WideString r; HRESULT hr = Get_XML(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString XML = {read=_scw_Get_XML, write=Set_XML};
};

__dispinterface ViewDisp;
typedef System::DelphiInterface<ViewDisp> _di_ViewDisp;
__dispinterface INTERFACE_UUID("{00063095-0000-0000-C000-000000000046}") ViewDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0006308D-0000-0000-C000-000000000046}") _Views  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_View &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const WideString Name, Activex::TOleEnum ViewType, Activex::TOleEnum SaveOption, _di_View &Add_result) = 0 ;
	virtual HRESULT __safecall Remove(const OleVariant Index) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface _ViewsDisp;
typedef System::DelphiInterface<_ViewsDisp> _di__ViewsDisp;
__dispinterface INTERFACE_UUID("{0006308D-0000-0000-C000-000000000046}") _ViewsDisp  : public IDispatch 
{
	
};

__interface AddressList;
typedef System::DelphiInterface<AddressList> _di_AddressList;
__interface INTERFACE_UUID("{00063048-0000-0000-C000-000000000046}") AddressLists  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_AddressList &Item_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface AddressListsDisp;
typedef System::DelphiInterface<AddressListsDisp> _di_AddressListsDisp;
__dispinterface INTERFACE_UUID("{00063048-0000-0000-C000-000000000046}") AddressListsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063049-0000-0000-C000-000000000046}") AddressList  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_AddressEntries(_di_AddressEntries &Get_AddressEntries_result) = 0 ;
	virtual HRESULT __safecall Get_ID(WideString &Get_ID_result) = 0 ;
	virtual HRESULT __safecall Get_Index(int &Get_Index_result) = 0 ;
	virtual HRESULT __safecall Get_IsReadOnly(Word &Get_IsReadOnly_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_AddressEntries _scw_Get_AddressEntries() { _di_AddressEntries r; HRESULT hr = Get_AddressEntries(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_AddressEntries AddressEntries = {read=_scw_Get_AddressEntries};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ID() { WideString r; HRESULT hr = Get_ID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ID = {read=_scw_Get_ID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Index() { int r; HRESULT hr = Get_Index(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Index = {read=_scw_Get_Index};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsReadOnly() { Word r; HRESULT hr = Get_IsReadOnly(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsReadOnly = {read=_scw_Get_IsReadOnly};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
};

__dispinterface AddressListDisp;
typedef System::DelphiInterface<AddressListDisp> _di_AddressListDisp;
__dispinterface INTERFACE_UUID("{00063049-0000-0000-C000-000000000046}") AddressListDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063086-0000-0000-C000-000000000046}") SyncObjects  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di__SyncObject &Item_result) = 0 ;
	virtual HRESULT __safecall Get_AppFolders(_di__SyncObject &Get_AppFolders_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__SyncObject _scw_Get_AppFolders() { _di__SyncObject r; HRESULT hr = Get_AppFolders(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__SyncObject AppFolders = {read=_scw_Get_AppFolders};
};

__dispinterface SyncObjectsDisp;
typedef System::DelphiInterface<SyncObjectsDisp> _di_SyncObjectsDisp;
__dispinterface INTERFACE_UUID("{00063086-0000-0000-C000-000000000046}") SyncObjectsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063083-0000-0000-C000-000000000046}") _SyncObject  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Start(void) = 0 ;
	virtual HRESULT __safecall Stop(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
};

__dispinterface _SyncObjectDisp;
typedef System::DelphiInterface<_SyncObjectDisp> _di__SyncObjectDisp;
__dispinterface INTERFACE_UUID("{00063083-0000-0000-C000-000000000046}") _SyncObjectDisp  : public IDispatch 
{
	
};

__dispinterface SyncObjectEvents;
typedef System::DelphiInterface<SyncObjectEvents> _di_SyncObjectEvents;
__dispinterface INTERFACE_UUID("{00063085-0000-0000-C000-000000000046}") SyncObjectEvents  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063005-0000-0000-C000-000000000046}") _Inspector  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_CommandBars(Opofcxp::_di__CommandBars &Get_CommandBars_result) = 0 ;
	virtual HRESULT __safecall Get_CurrentItem(_di_IDispatch &Get_CurrentItem_result) = 0 ;
	virtual HRESULT __safecall Get_EditorType(Activex::TOleEnum &Get_EditorType_result) = 0 ;
	virtual HRESULT __safecall Get_ModifiedFormPages(_di_IDispatch &Get_ModifiedFormPages_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall HideFormPage(const WideString PageName) = 0 ;
	virtual HRESULT __safecall IsWordMail(Word &IsWordMail_result) = 0 ;
	virtual HRESULT __safecall SetCurrentFormPage(const WideString PageName) = 0 ;
	virtual HRESULT __safecall ShowFormPage(const WideString PageName) = 0 ;
	virtual HRESULT __safecall Get_HTMLEditor(_di_IDispatch &Get_HTMLEditor_result) = 0 ;
	virtual HRESULT __safecall Get_WordEditor(_di_IDispatch &Get_WordEditor_result) = 0 ;
	virtual HRESULT __safecall Get_Caption(WideString &Get_Caption_result) = 0 ;
	virtual HRESULT __safecall Get_Height(int &Get_Height_result) = 0 ;
	virtual HRESULT __safecall Set_Height(int Height) = 0 ;
	virtual HRESULT __safecall Get_Left(int &Get_Left_result) = 0 ;
	virtual HRESULT __safecall Set_Left(int Left) = 0 ;
	virtual HRESULT __safecall Get_Top(int &Get_Top_result) = 0 ;
	virtual HRESULT __safecall Set_Top(int Top) = 0 ;
	virtual HRESULT __safecall Get_Width(int &Get_Width_result) = 0 ;
	virtual HRESULT __safecall Set_Width(int Width) = 0 ;
	virtual HRESULT __safecall Get_WindowState(Activex::TOleEnum &Get_WindowState_result) = 0 ;
	virtual HRESULT __safecall Set_WindowState(Activex::TOleEnum WindowState) = 0 ;
	virtual HRESULT __safecall Activate(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Opofcxp::_di__CommandBars _scw_Get_CommandBars() { Opofcxp::_di__CommandBars r; HRESULT hr = Get_CommandBars(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Opofcxp::_di__CommandBars CommandBars = {read=_scw_Get_CommandBars};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_CurrentItem() { _di_IDispatch r; HRESULT hr = Get_CurrentItem(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch CurrentItem = {read=_scw_Get_CurrentItem};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_EditorType() { Activex::TOleEnum r; HRESULT hr = Get_EditorType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum EditorType = {read=_scw_Get_EditorType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_ModifiedFormPages() { _di_IDispatch r; HRESULT hr = Get_ModifiedFormPages(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch ModifiedFormPages = {read=_scw_Get_ModifiedFormPages};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_HTMLEditor() { _di_IDispatch r; HRESULT hr = Get_HTMLEditor(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch HTMLEditor = {read=_scw_Get_HTMLEditor};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_WordEditor() { _di_IDispatch r; HRESULT hr = Get_WordEditor(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch WordEditor = {read=_scw_Get_WordEditor};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Caption() { WideString r; HRESULT hr = Get_Caption(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Caption = {read=_scw_Get_Caption};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Height() { int r; HRESULT hr = Get_Height(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Height = {read=_scw_Get_Height, write=Set_Height};
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
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_WindowState() { Activex::TOleEnum r; HRESULT hr = Get_WindowState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum WindowState = {read=_scw_Get_WindowState, write=Set_WindowState};
};

__dispinterface _InspectorDisp;
typedef System::DelphiInterface<_InspectorDisp> _di__InspectorDisp;
__dispinterface INTERFACE_UUID("{00063005-0000-0000-C000-000000000046}") _InspectorDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0006300A-0000-0000-C000-000000000046}") _Explorers  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di__Explorer &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const OleVariant Folder, Activex::TOleEnum DisplayMode, _di__Explorer &Add_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface _ExplorersDisp;
typedef System::DelphiInterface<_ExplorersDisp> _di__ExplorersDisp;
__dispinterface INTERFACE_UUID("{0006300A-0000-0000-C000-000000000046}") _ExplorersDisp  : public IDispatch 
{
	
};

__dispinterface ExplorerEvents;
typedef System::DelphiInterface<ExplorerEvents> _di_ExplorerEvents;
__dispinterface INTERFACE_UUID("{0006304F-0000-0000-C000-000000000046}") ExplorerEvents  : public IDispatch 
{
	
};

__dispinterface ExplorerEvents_10;
typedef System::DelphiInterface<ExplorerEvents_10> _di_ExplorerEvents_10;
__dispinterface INTERFACE_UUID("{0006300F-0000-0000-C000-000000000046}") ExplorerEvents_10  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063008-0000-0000-C000-000000000046}") _Inspectors  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di__Inspector &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const _di_IDispatch Item, _di__Inspector &Add_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface _InspectorsDisp;
typedef System::DelphiInterface<_InspectorsDisp> _di__InspectorsDisp;
__dispinterface INTERFACE_UUID("{00063008-0000-0000-C000-000000000046}") _InspectorsDisp  : public IDispatch 
{
	
};

__dispinterface InspectorEvents;
typedef System::DelphiInterface<InspectorEvents> _di_InspectorEvents;
__dispinterface INTERFACE_UUID("{0006307D-0000-0000-C000-000000000046}") InspectorEvents  : public IDispatch 
{
	
};

__dispinterface InspectorEvents_10;
typedef System::DelphiInterface<InspectorEvents_10> _di_InspectorEvents_10;
__dispinterface INTERFACE_UUID("{0006302A-0000-0000-C000-000000000046}") InspectorEvents_10  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0006300B-0000-0000-C000-000000000046}") Search  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Filter(WideString &Get_Filter_result) = 0 ;
	virtual HRESULT __safecall Get_IsSynchronous(Word &Get_IsSynchronous_result) = 0 ;
	virtual HRESULT __safecall Get_Results(_di__Results &Get_Results_result) = 0 ;
	virtual HRESULT __safecall Get_SearchSubFolders(Word &Get_SearchSubFolders_result) = 0 ;
	virtual HRESULT __safecall Stop(void) = 0 ;
	virtual HRESULT __safecall Get_Tag(WideString &Get_Tag_result) = 0 ;
	virtual HRESULT __safecall Get_Scope(WideString &Get_Scope_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Filter() { WideString r; HRESULT hr = Get_Filter(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Filter = {read=_scw_Get_Filter};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsSynchronous() { Word r; HRESULT hr = Get_IsSynchronous(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsSynchronous = {read=_scw_Get_IsSynchronous};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Results _scw_Get_Results() { _di__Results r; HRESULT hr = Get_Results(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Results Results = {read=_scw_Get_Results};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_SearchSubFolders() { Word r; HRESULT hr = Get_SearchSubFolders(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word SearchSubFolders = {read=_scw_Get_SearchSubFolders};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Tag() { WideString r; HRESULT hr = Get_Tag(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Tag = {read=_scw_Get_Tag};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Scope() { WideString r; HRESULT hr = Get_Scope(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Scope = {read=_scw_Get_Scope};
};

__dispinterface SearchDisp;
typedef System::DelphiInterface<SearchDisp> _di_SearchDisp;
__dispinterface INTERFACE_UUID("{0006300B-0000-0000-C000-000000000046}") SearchDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0006300C-0000-0000-C000-000000000046}") _Results  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_IDispatch &Item_result) = 0 ;
	virtual HRESULT __safecall Get_RawTable(System::_di_IInterface &Get_RawTable_result) = 0 ;
	virtual HRESULT __safecall GetFirst(_di_IDispatch &GetFirst_result) = 0 ;
	virtual HRESULT __safecall GetLast(_di_IDispatch &GetLast_result) = 0 ;
	virtual HRESULT __safecall GetNext(_di_IDispatch &GetNext_result) = 0 ;
	virtual HRESULT __safecall GetPrevious(_di_IDispatch &GetPrevious_result) = 0 ;
	virtual HRESULT __safecall ResetColumns(void) = 0 ;
	virtual HRESULT __safecall SetColumns(const WideString Columns) = 0 ;
	virtual HRESULT __safecall Sort(const WideString Property_, const OleVariant Descending) = 0 ;
	virtual HRESULT __safecall Get_DefaultItemType(Activex::TOleEnum &Get_DefaultItemType_result) = 0 ;
	virtual HRESULT __safecall Set_DefaultItemType(Activex::TOleEnum DefaultItemType) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_RawTable() { System::_di_IInterface r; HRESULT hr = Get_RawTable(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface RawTable = {read=_scw_Get_RawTable};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DefaultItemType() { Activex::TOleEnum r; HRESULT hr = Get_DefaultItemType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DefaultItemType = {read=_scw_Get_DefaultItemType, write=Set_DefaultItemType};
};

__dispinterface _ResultsDisp;
typedef System::DelphiInterface<_ResultsDisp> _di__ResultsDisp;
__dispinterface INTERFACE_UUID("{0006300C-0000-0000-C000-000000000046}") _ResultsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000630B1-0000-0000-C000-000000000046}") _Reminders  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di__Reminder &Item_result) = 0 ;
	virtual HRESULT __safecall Remove(const OleVariant Index) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface _RemindersDisp;
typedef System::DelphiInterface<_RemindersDisp> _di__RemindersDisp;
__dispinterface INTERFACE_UUID("{000630B1-0000-0000-C000-000000000046}") _RemindersDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000630B0-0000-0000-C000-000000000046}") _Reminder  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Dismiss(void) = 0 ;
	virtual HRESULT __safecall Snooze(const OleVariant SnoozeTime) = 0 ;
	virtual HRESULT __safecall Get_Caption(WideString &Get_Caption_result) = 0 ;
	virtual HRESULT __safecall Get_IsVisible(Word &Get_IsVisible_result) = 0 ;
	virtual HRESULT __safecall Get_Item(_di_IDispatch &Get_Item_result) = 0 ;
	virtual HRESULT __safecall Get_NextReminderDate(System::TDateTime &Get_NextReminderDate_result) = 0 ;
	virtual HRESULT __safecall Get_OriginalReminderDate(System::TDateTime &Get_OriginalReminderDate_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Caption() { WideString r; HRESULT hr = Get_Caption(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Caption = {read=_scw_Get_Caption};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsVisible() { Word r; HRESULT hr = Get_IsVisible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsVisible = {read=_scw_Get_IsVisible};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Item() { _di_IDispatch r; HRESULT hr = Get_Item(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Item = {read=_scw_Get_Item};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_NextReminderDate() { System::TDateTime r; HRESULT hr = Get_NextReminderDate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime NextReminderDate = {read=_scw_Get_NextReminderDate};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_OriginalReminderDate() { System::TDateTime r; HRESULT hr = Get_OriginalReminderDate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime OriginalReminderDate = {read=_scw_Get_OriginalReminderDate};
};

__dispinterface _ReminderDisp;
typedef System::DelphiInterface<_ReminderDisp> _di__ReminderDisp;
__dispinterface INTERFACE_UUID("{000630B0-0000-0000-C000-000000000046}") _ReminderDisp  : public IDispatch 
{
	
};

__interface Actions;
typedef System::DelphiInterface<Actions> _di_Actions;
__interface INTERFACE_UUID("{0006303E-0000-0000-C000-000000000046}") Actions  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_Action &Item_result) = 0 ;
	virtual HRESULT __safecall Add(_di_Action &Add_result) = 0 ;
	virtual HRESULT __safecall Remove(int Index) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface ActionsDisp;
typedef System::DelphiInterface<ActionsDisp> _di_ActionsDisp;
__dispinterface INTERFACE_UUID("{0006303E-0000-0000-C000-000000000046}") ActionsDisp  : public IDispatch 
{
	
};

__dispinterface ApplicationEvents;
typedef System::DelphiInterface<ApplicationEvents> _di_ApplicationEvents;
__interface PropertyPages;
typedef System::DelphiInterface<PropertyPages> _di_PropertyPages;
__dispinterface INTERFACE_UUID("{0006304E-0000-0000-C000-000000000046}") ApplicationEvents  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063080-0000-0000-C000-000000000046}") PropertyPages  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_IDispatch &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const OleVariant Page, const WideString Title) = 0 ;
	virtual HRESULT __safecall Remove(const OleVariant Index) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface PropertyPagesDisp;
typedef System::DelphiInterface<PropertyPagesDisp> _di_PropertyPagesDisp;
__dispinterface INTERFACE_UUID("{00063080-0000-0000-C000-000000000046}") PropertyPagesDisp  : public IDispatch 
{
	
};

__interface Attachments;
typedef System::DelphiInterface<Attachments> _di_Attachments;
__interface FormDescription;
typedef System::DelphiInterface<FormDescription> _di_FormDescription;
__interface UserProperties;
typedef System::DelphiInterface<UserProperties> _di_UserProperties;
__interface Recipients;
typedef System::DelphiInterface<Recipients> _di_Recipients;
__interface RecurrencePattern;
typedef System::DelphiInterface<RecurrencePattern> _di_RecurrencePattern;
__interface Links;
typedef System::DelphiInterface<Links> _di_Links;
__interface ItemProperties;
typedef System::DelphiInterface<ItemProperties> _di_ItemProperties;
__interface INTERFACE_UUID("{00063033-0000-0000-C000-000000000046}") _AppointmentItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall Get_AllDayEvent(Word &Get_AllDayEvent_result) = 0 ;
	virtual HRESULT __safecall Set_AllDayEvent(Word AllDayEvent) = 0 ;
	virtual HRESULT __safecall Get_BusyStatus(Activex::TOleEnum &Get_BusyStatus_result) = 0 ;
	virtual HRESULT __safecall Set_BusyStatus(Activex::TOleEnum BusyStatus) = 0 ;
	virtual HRESULT __safecall Get_Duration(int &Get_Duration_result) = 0 ;
	virtual HRESULT __safecall Set_Duration(int Duration) = 0 ;
	virtual HRESULT __safecall Get_End_(System::TDateTime &Get_End__result) = 0 ;
	virtual HRESULT __safecall Set_End_(System::TDateTime End_) = 0 ;
	virtual HRESULT __safecall Get_IsOnlineMeeting(Word &Get_IsOnlineMeeting_result) = 0 ;
	virtual HRESULT __safecall Set_IsOnlineMeeting(Word IsOnlineMeeting) = 0 ;
	virtual HRESULT __safecall Get_IsRecurring(Word &Get_IsRecurring_result) = 0 ;
	virtual HRESULT __safecall Get_Location(WideString &Get_Location_result) = 0 ;
	virtual HRESULT __safecall Set_Location(const WideString Location) = 0 ;
	virtual HRESULT __safecall Get_MeetingStatus(Activex::TOleEnum &Get_MeetingStatus_result) = 0 ;
	virtual HRESULT __safecall Set_MeetingStatus(Activex::TOleEnum MeetingStatus) = 0 ;
	virtual HRESULT __safecall Get_NetMeetingAutoStart(Word &Get_NetMeetingAutoStart_result) = 0 ;
	virtual HRESULT __safecall Set_NetMeetingAutoStart(Word NetMeetingAutoStart) = 0 ;
	virtual HRESULT __safecall Get_NetMeetingOrganizerAlias(WideString &Get_NetMeetingOrganizerAlias_result) = 0 ;
	virtual HRESULT __safecall Set_NetMeetingOrganizerAlias(const WideString NetMeetingOrganizerAlias) = 0 ;
	virtual HRESULT __safecall Get_NetMeetingServer(WideString &Get_NetMeetingServer_result) = 0 ;
	virtual HRESULT __safecall Set_NetMeetingServer(const WideString NetMeetingServer) = 0 ;
	virtual HRESULT __safecall Get_NetMeetingType(Activex::TOleEnum &Get_NetMeetingType_result) = 0 ;
	virtual HRESULT __safecall Set_NetMeetingType(Activex::TOleEnum NetMeetingType) = 0 ;
	virtual HRESULT __safecall Get_OptionalAttendees(WideString &Get_OptionalAttendees_result) = 0 ;
	virtual HRESULT __safecall Set_OptionalAttendees(const WideString OptionalAttendees) = 0 ;
	virtual HRESULT __safecall Get_Organizer(WideString &Get_Organizer_result) = 0 ;
	virtual HRESULT __safecall Get_Recipients(_di_Recipients &Get_Recipients_result) = 0 ;
	virtual HRESULT __safecall Get_RecurrenceState(Activex::TOleEnum &Get_RecurrenceState_result) = 0 ;
	virtual HRESULT __safecall Get_ReminderMinutesBeforeStart(int &Get_ReminderMinutesBeforeStart_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderMinutesBeforeStart(int ReminderMinutesBeforeStart) = 0 ;
	virtual HRESULT __safecall Get_ReminderOverrideDefault(Word &Get_ReminderOverrideDefault_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderOverrideDefault(Word ReminderOverrideDefault) = 0 ;
	virtual HRESULT __safecall Get_ReminderPlaySound(Word &Get_ReminderPlaySound_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderPlaySound(Word ReminderPlaySound) = 0 ;
	virtual HRESULT __safecall Get_ReminderSet(Word &Get_ReminderSet_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderSet(Word ReminderSet) = 0 ;
	virtual HRESULT __safecall Get_ReminderSoundFile(WideString &Get_ReminderSoundFile_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderSoundFile(const WideString ReminderSoundFile) = 0 ;
	virtual HRESULT __safecall Get_ReplyTime(System::TDateTime &Get_ReplyTime_result) = 0 ;
	virtual HRESULT __safecall Set_ReplyTime(System::TDateTime ReplyTime) = 0 ;
	virtual HRESULT __safecall Get_RequiredAttendees(WideString &Get_RequiredAttendees_result) = 0 ;
	virtual HRESULT __safecall Set_RequiredAttendees(const WideString RequiredAttendees) = 0 ;
	virtual HRESULT __safecall Get_Resources(WideString &Get_Resources_result) = 0 ;
	virtual HRESULT __safecall Set_Resources(const WideString Resources) = 0 ;
	virtual HRESULT __safecall Get_ResponseRequested(Word &Get_ResponseRequested_result) = 0 ;
	virtual HRESULT __safecall Set_ResponseRequested(Word ResponseRequested) = 0 ;
	virtual HRESULT __safecall Get_ResponseStatus(Activex::TOleEnum &Get_ResponseStatus_result) = 0 ;
	virtual HRESULT __safecall Get_Start(System::TDateTime &Get_Start_result) = 0 ;
	virtual HRESULT __safecall Set_Start(System::TDateTime Start) = 0 ;
	virtual HRESULT __safecall ClearRecurrencePattern(void) = 0 ;
	virtual HRESULT __safecall ForwardAsVcal(_di__MailItem &ForwardAsVcal_result) = 0 ;
	virtual HRESULT __safecall GetRecurrencePattern(_di_RecurrencePattern &GetRecurrencePattern_result) = 0 ;
	virtual HRESULT __safecall Respond(Activex::TOleEnum Response, const OleVariant fNoUI, const OleVariant fAdditionalTextDialog, _di__MeetingItem &Respond_result) = 0 ;
	virtual HRESULT __safecall Send(void) = 0 ;
	virtual HRESULT __safecall Get_NetMeetingDocPathName(WideString &Get_NetMeetingDocPathName_result) = 0 ;
	virtual HRESULT __safecall Set_NetMeetingDocPathName(const WideString NetMeetingDocPathName) = 0 ;
	virtual HRESULT __safecall Get_NetShowURL(WideString &Get_NetShowURL_result) = 0 ;
	virtual HRESULT __safecall Set_NetShowURL(const WideString NetShowURL) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_ConferenceServerAllowExternal(Word &Get_ConferenceServerAllowExternal_result) = 0 ;
	virtual HRESULT __safecall Set_ConferenceServerAllowExternal(Word ConferenceServerAllowExternal) = 0 ;
	virtual HRESULT __safecall Get_ConferenceServerPassword(WideString &Get_ConferenceServerPassword_result) = 0 ;
	virtual HRESULT __safecall Set_ConferenceServerPassword(const WideString ConferenceServerPassword) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_InternetCodepage(int &Get_InternetCodepage_result) = 0 ;
	virtual HRESULT __safecall Set_InternetCodepage(int InternetCodepage) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_AllDayEvent() { Word r; HRESULT hr = Get_AllDayEvent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word AllDayEvent = {read=_scw_Get_AllDayEvent, write=Set_AllDayEvent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_BusyStatus() { Activex::TOleEnum r; HRESULT hr = Get_BusyStatus(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum BusyStatus = {read=_scw_Get_BusyStatus, write=Set_BusyStatus};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Duration() { int r; HRESULT hr = Get_Duration(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Duration = {read=_scw_Get_Duration, write=Set_Duration};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_End_() { System::TDateTime r; HRESULT hr = Get_End_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime End_ = {read=_scw_Get_End_, write=Set_End_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsOnlineMeeting() { Word r; HRESULT hr = Get_IsOnlineMeeting(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsOnlineMeeting = {read=_scw_Get_IsOnlineMeeting, write=Set_IsOnlineMeeting};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsRecurring() { Word r; HRESULT hr = Get_IsRecurring(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsRecurring = {read=_scw_Get_IsRecurring};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Location() { WideString r; HRESULT hr = Get_Location(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Location = {read=_scw_Get_Location, write=Set_Location};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MeetingStatus() { Activex::TOleEnum r; HRESULT hr = Get_MeetingStatus(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MeetingStatus = {read=_scw_Get_MeetingStatus, write=Set_MeetingStatus};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NetMeetingAutoStart() { Word r; HRESULT hr = Get_NetMeetingAutoStart(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NetMeetingAutoStart = {read=_scw_Get_NetMeetingAutoStart, write=Set_NetMeetingAutoStart};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_NetMeetingOrganizerAlias() { WideString r; HRESULT hr = Get_NetMeetingOrganizerAlias(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString NetMeetingOrganizerAlias = {read=_scw_Get_NetMeetingOrganizerAlias, write=Set_NetMeetingOrganizerAlias};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_NetMeetingServer() { WideString r; HRESULT hr = Get_NetMeetingServer(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString NetMeetingServer = {read=_scw_Get_NetMeetingServer, write=Set_NetMeetingServer};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_NetMeetingType() { Activex::TOleEnum r; HRESULT hr = Get_NetMeetingType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum NetMeetingType = {read=_scw_Get_NetMeetingType, write=Set_NetMeetingType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OptionalAttendees() { WideString r; HRESULT hr = Get_OptionalAttendees(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OptionalAttendees = {read=_scw_Get_OptionalAttendees, write=Set_OptionalAttendees};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Organizer() { WideString r; HRESULT hr = Get_Organizer(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Organizer = {read=_scw_Get_Organizer};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Recipients _scw_Get_Recipients() { _di_Recipients r; HRESULT hr = Get_Recipients(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Recipients Recipients = {read=_scw_Get_Recipients};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_RecurrenceState() { Activex::TOleEnum r; HRESULT hr = Get_RecurrenceState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum RecurrenceState = {read=_scw_Get_RecurrenceState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ReminderMinutesBeforeStart() { int r; HRESULT hr = Get_ReminderMinutesBeforeStart(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int ReminderMinutesBeforeStart = {read=_scw_Get_ReminderMinutesBeforeStart, write=Set_ReminderMinutesBeforeStart};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ReminderOverrideDefault() { Word r; HRESULT hr = Get_ReminderOverrideDefault(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ReminderOverrideDefault = {read=_scw_Get_ReminderOverrideDefault, write=Set_ReminderOverrideDefault};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ReminderPlaySound() { Word r; HRESULT hr = Get_ReminderPlaySound(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ReminderPlaySound = {read=_scw_Get_ReminderPlaySound, write=Set_ReminderPlaySound};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ReminderSet() { Word r; HRESULT hr = Get_ReminderSet(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ReminderSet = {read=_scw_Get_ReminderSet, write=Set_ReminderSet};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ReminderSoundFile() { WideString r; HRESULT hr = Get_ReminderSoundFile(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ReminderSoundFile = {read=_scw_Get_ReminderSoundFile, write=Set_ReminderSoundFile};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_ReplyTime() { System::TDateTime r; HRESULT hr = Get_ReplyTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime ReplyTime = {read=_scw_Get_ReplyTime, write=Set_ReplyTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_RequiredAttendees() { WideString r; HRESULT hr = Get_RequiredAttendees(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString RequiredAttendees = {read=_scw_Get_RequiredAttendees, write=Set_RequiredAttendees};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Resources() { WideString r; HRESULT hr = Get_Resources(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Resources = {read=_scw_Get_Resources, write=Set_Resources};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ResponseRequested() { Word r; HRESULT hr = Get_ResponseRequested(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ResponseRequested = {read=_scw_Get_ResponseRequested, write=Set_ResponseRequested};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_ResponseStatus() { Activex::TOleEnum r; HRESULT hr = Get_ResponseStatus(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum ResponseStatus = {read=_scw_Get_ResponseStatus};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_Start() { System::TDateTime r; HRESULT hr = Get_Start(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime Start = {read=_scw_Get_Start, write=Set_Start};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_NetMeetingDocPathName() { WideString r; HRESULT hr = Get_NetMeetingDocPathName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString NetMeetingDocPathName = {read=_scw_Get_NetMeetingDocPathName, write=Set_NetMeetingDocPathName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_NetShowURL() { WideString r; HRESULT hr = Get_NetShowURL(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString NetShowURL = {read=_scw_Get_NetShowURL, write=Set_NetShowURL};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ConferenceServerAllowExternal() { Word r; HRESULT hr = Get_ConferenceServerAllowExternal(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ConferenceServerAllowExternal = {read=_scw_Get_ConferenceServerAllowExternal, write=Set_ConferenceServerAllowExternal};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConferenceServerPassword() { WideString r; HRESULT hr = Get_ConferenceServerPassword(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConferenceServerPassword = {read=_scw_Get_ConferenceServerPassword, write=Set_ConferenceServerPassword};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_InternetCodepage() { int r; HRESULT hr = Get_InternetCodepage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int InternetCodepage = {read=_scw_Get_InternetCodepage, write=Set_InternetCodepage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _AppointmentItemDisp;
typedef System::DelphiInterface<_AppointmentItemDisp> _di__AppointmentItemDisp;
__dispinterface INTERFACE_UUID("{00063033-0000-0000-C000-000000000046}") _AppointmentItemDisp  : public IDispatch 
{
	
};

__interface Attachment;
typedef System::DelphiInterface<Attachment> _di_Attachment;
__interface INTERFACE_UUID("{0006303C-0000-0000-C000-000000000046}") Attachments  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_Attachment &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const OleVariant Source, const OleVariant Type_, const OleVariant Position, const OleVariant DisplayName, _di_Attachment &Add_result) = 0 ;
	virtual HRESULT __safecall Remove(int Index) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface AttachmentsDisp;
typedef System::DelphiInterface<AttachmentsDisp> _di_AttachmentsDisp;
__dispinterface INTERFACE_UUID("{0006303C-0000-0000-C000-000000000046}") AttachmentsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063007-0000-0000-C000-000000000046}") Attachment  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_DisplayName(WideString &Get_DisplayName_result) = 0 ;
	virtual HRESULT __safecall Set_DisplayName(const WideString DisplayName) = 0 ;
	virtual HRESULT __safecall Get_FileName(WideString &Get_FileName_result) = 0 ;
	virtual HRESULT __safecall Get_Index(int &Get_Index_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_PathName(WideString &Get_PathName_result) = 0 ;
	virtual HRESULT __safecall Get_Position(int &Get_Position_result) = 0 ;
	virtual HRESULT __safecall Set_Position(int Position) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall SaveAsFile(const WideString Path) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_DisplayName() { WideString r; HRESULT hr = Get_DisplayName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString DisplayName = {read=_scw_Get_DisplayName, write=Set_DisplayName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FileName() { WideString r; HRESULT hr = Get_FileName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FileName = {read=_scw_Get_FileName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Index() { int r; HRESULT hr = Get_Index(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Index = {read=_scw_Get_Index};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_PathName() { WideString r; HRESULT hr = Get_PathName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString PathName = {read=_scw_Get_PathName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Position() { int r; HRESULT hr = Get_Position(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Position = {read=_scw_Get_Position, write=Set_Position};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
};

__dispinterface AttachmentDisp;
typedef System::DelphiInterface<AttachmentDisp> _di_AttachmentDisp;
__dispinterface INTERFACE_UUID("{00063007-0000-0000-C000-000000000046}") AttachmentDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063046-0000-0000-C000-000000000046}") FormDescription  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Category(WideString &Get_Category_result) = 0 ;
	virtual HRESULT __safecall Set_Category(const WideString Category) = 0 ;
	virtual HRESULT __safecall Get_CategorySub(WideString &Get_CategorySub_result) = 0 ;
	virtual HRESULT __safecall Set_CategorySub(const WideString CategorySub) = 0 ;
	virtual HRESULT __safecall Get_Comment(WideString &Get_Comment_result) = 0 ;
	virtual HRESULT __safecall Set_Comment(const WideString Comment) = 0 ;
	virtual HRESULT __safecall Get_ContactName(WideString &Get_ContactName_result) = 0 ;
	virtual HRESULT __safecall Set_ContactName(const WideString ContactName) = 0 ;
	virtual HRESULT __safecall Get_DisplayName(WideString &Get_DisplayName_result) = 0 ;
	virtual HRESULT __safecall Set_DisplayName(const WideString DisplayName) = 0 ;
	virtual HRESULT __safecall Get_Hidden(Word &Get_Hidden_result) = 0 ;
	virtual HRESULT __safecall Set_Hidden(Word Hidden) = 0 ;
	virtual HRESULT __safecall Get_Icon(WideString &Get_Icon_result) = 0 ;
	virtual HRESULT __safecall Set_Icon(const WideString Icon) = 0 ;
	virtual HRESULT __safecall Get_Locked(Word &Get_Locked_result) = 0 ;
	virtual HRESULT __safecall Set_Locked(Word Locked) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Get_MiniIcon(WideString &Get_MiniIcon_result) = 0 ;
	virtual HRESULT __safecall Set_MiniIcon(const WideString MiniIcon) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString Name) = 0 ;
	virtual HRESULT __safecall Get_Number(WideString &Get_Number_result) = 0 ;
	virtual HRESULT __safecall Set_Number(const WideString Number) = 0 ;
	virtual HRESULT __safecall Get_OneOff(Word &Get_OneOff_result) = 0 ;
	virtual HRESULT __safecall Set_OneOff(Word OneOff) = 0 ;
	virtual HRESULT __safecall Get_Password(WideString &Get_Password_result) = 0 ;
	virtual HRESULT __safecall Set_Password(const WideString Password) = 0 ;
	virtual HRESULT __safecall Get_ScriptText(WideString &Get_ScriptText_result) = 0 ;
	virtual HRESULT __safecall Get_Template(WideString &Get_Template_result) = 0 ;
	virtual HRESULT __safecall Set_Template(const WideString Template) = 0 ;
	virtual HRESULT __safecall Get_UseWordMail(Word &Get_UseWordMail_result) = 0 ;
	virtual HRESULT __safecall Set_UseWordMail(Word UseWordMail) = 0 ;
	virtual HRESULT __safecall Get_Version(WideString &Get_Version_result) = 0 ;
	virtual HRESULT __safecall Set_Version(const WideString Version) = 0 ;
	virtual HRESULT __safecall PublishForm(Activex::TOleEnum Registry, const OleVariant Folder) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Category() { WideString r; HRESULT hr = Get_Category(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Category = {read=_scw_Get_Category, write=Set_Category};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CategorySub() { WideString r; HRESULT hr = Get_CategorySub(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CategorySub = {read=_scw_Get_CategorySub, write=Set_CategorySub};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Comment() { WideString r; HRESULT hr = Get_Comment(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Comment = {read=_scw_Get_Comment, write=Set_Comment};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ContactName() { WideString r; HRESULT hr = Get_ContactName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ContactName = {read=_scw_Get_ContactName, write=Set_ContactName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_DisplayName() { WideString r; HRESULT hr = Get_DisplayName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString DisplayName = {read=_scw_Get_DisplayName, write=Set_DisplayName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Hidden() { Word r; HRESULT hr = Get_Hidden(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Hidden = {read=_scw_Get_Hidden, write=Set_Hidden};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Icon() { WideString r; HRESULT hr = Get_Icon(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Icon = {read=_scw_Get_Icon, write=Set_Icon};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Locked() { Word r; HRESULT hr = Get_Locked(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Locked = {read=_scw_Get_Locked, write=Set_Locked};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MiniIcon() { WideString r; HRESULT hr = Get_MiniIcon(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MiniIcon = {read=_scw_Get_MiniIcon, write=Set_MiniIcon};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Number() { WideString r; HRESULT hr = Get_Number(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Number = {read=_scw_Get_Number, write=Set_Number};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_OneOff() { Word r; HRESULT hr = Get_OneOff(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word OneOff = {read=_scw_Get_OneOff, write=Set_OneOff};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Password() { WideString r; HRESULT hr = Get_Password(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Password = {read=_scw_Get_Password, write=Set_Password};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ScriptText() { WideString r; HRESULT hr = Get_ScriptText(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ScriptText = {read=_scw_Get_ScriptText};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Template() { WideString r; HRESULT hr = Get_Template(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Template = {read=_scw_Get_Template, write=Set_Template};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UseWordMail() { Word r; HRESULT hr = Get_UseWordMail(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UseWordMail = {read=_scw_Get_UseWordMail, write=Set_UseWordMail};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Version() { WideString r; HRESULT hr = Get_Version(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Version = {read=_scw_Get_Version, write=Set_Version};
};

__dispinterface FormDescriptionDisp;
typedef System::DelphiInterface<FormDescriptionDisp> _di_FormDescriptionDisp;
__dispinterface INTERFACE_UUID("{00063046-0000-0000-C000-000000000046}") FormDescriptionDisp  : public IDispatch 
{
	
};

__interface UserProperty;
typedef System::DelphiInterface<UserProperty> _di_UserProperty;
__interface INTERFACE_UUID("{0006303D-0000-0000-C000-000000000046}") UserProperties  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_UserProperty &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const WideString Name, Activex::TOleEnum Type_, const OleVariant AddToFolderFields, const OleVariant DisplayFormat, _di_UserProperty &Add_result) = 0 ;
	virtual HRESULT __safecall Find(const WideString Name, const OleVariant Custom, _di_UserProperty &Find_result) = 0 ;
	virtual HRESULT __safecall Remove(int Index) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface UserPropertiesDisp;
typedef System::DelphiInterface<UserPropertiesDisp> _di_UserPropertiesDisp;
__dispinterface INTERFACE_UUID("{0006303D-0000-0000-C000-000000000046}") UserPropertiesDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063042-0000-0000-C000-000000000046}") UserProperty  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Formula(WideString &Get_Formula_result) = 0 ;
	virtual HRESULT __safecall Set_Formula(const WideString Formula) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_ValidationFormula(WideString &Get_ValidationFormula_result) = 0 ;
	virtual HRESULT __safecall Set_ValidationFormula(const WideString ValidationFormula) = 0 ;
	virtual HRESULT __safecall Get_ValidationText(WideString &Get_ValidationText_result) = 0 ;
	virtual HRESULT __safecall Set_ValidationText(const WideString ValidationText) = 0 ;
	virtual HRESULT __safecall Get_Value(OleVariant &Get_Value_result) = 0 ;
	virtual HRESULT __safecall Set_Value(const OleVariant Value) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Get_IsUserProperty(Word &Get_IsUserProperty_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Formula() { WideString r; HRESULT hr = Get_Formula(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Formula = {read=_scw_Get_Formula, write=Set_Formula};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ValidationFormula() { WideString r; HRESULT hr = Get_ValidationFormula(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ValidationFormula = {read=_scw_Get_ValidationFormula, write=Set_ValidationFormula};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ValidationText() { WideString r; HRESULT hr = Get_ValidationText(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ValidationText = {read=_scw_Get_ValidationText, write=Set_ValidationText};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_Value() { OleVariant r; HRESULT hr = Get_Value(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant Value = {read=_scw_Get_Value, write=Set_Value};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsUserProperty() { Word r; HRESULT hr = Get_IsUserProperty(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsUserProperty = {read=_scw_Get_IsUserProperty};
};

__dispinterface UserPropertyDisp;
typedef System::DelphiInterface<UserPropertyDisp> _di_UserPropertyDisp;
__dispinterface INTERFACE_UUID("{00063042-0000-0000-C000-000000000046}") UserPropertyDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0006303B-0000-0000-C000-000000000046}") Recipients  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_Recipient &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const WideString Name, _di_Recipient &Add_result) = 0 ;
	virtual HRESULT __safecall Remove(int Index) = 0 ;
	virtual HRESULT __safecall ResolveAll(Word &ResolveAll_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface RecipientsDisp;
typedef System::DelphiInterface<RecipientsDisp> _di_RecipientsDisp;
__dispinterface INTERFACE_UUID("{0006303B-0000-0000-C000-000000000046}") RecipientsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063034-0000-0000-C000-000000000046}") _MailItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall Get_AlternateRecipientAllowed(Word &Get_AlternateRecipientAllowed_result) = 0 ;
	virtual HRESULT __safecall Set_AlternateRecipientAllowed(Word AlternateRecipientAllowed) = 0 ;
	virtual HRESULT __safecall Get_AutoForwarded(Word &Get_AutoForwarded_result) = 0 ;
	virtual HRESULT __safecall Set_AutoForwarded(Word AutoForwarded) = 0 ;
	virtual HRESULT __safecall Get_BCC(WideString &Get_BCC_result) = 0 ;
	virtual HRESULT __safecall Set_BCC(const WideString BCC) = 0 ;
	virtual HRESULT __safecall Get_CC(WideString &Get_CC_result) = 0 ;
	virtual HRESULT __safecall Set_CC(const WideString CC) = 0 ;
	virtual HRESULT __safecall Get_DeferredDeliveryTime(System::TDateTime &Get_DeferredDeliveryTime_result) = 0 ;
	virtual HRESULT __safecall Set_DeferredDeliveryTime(System::TDateTime DeferredDeliveryTime) = 0 ;
	virtual HRESULT __safecall Get_DeleteAfterSubmit(Word &Get_DeleteAfterSubmit_result) = 0 ;
	virtual HRESULT __safecall Set_DeleteAfterSubmit(Word DeleteAfterSubmit) = 0 ;
	virtual HRESULT __safecall Get_ExpiryTime(System::TDateTime &Get_ExpiryTime_result) = 0 ;
	virtual HRESULT __safecall Set_ExpiryTime(System::TDateTime ExpiryTime) = 0 ;
	virtual HRESULT __safecall Get_FlagDueBy(System::TDateTime &Get_FlagDueBy_result) = 0 ;
	virtual HRESULT __safecall Set_FlagDueBy(System::TDateTime FlagDueBy) = 0 ;
	virtual HRESULT __safecall Get_FlagRequest(WideString &Get_FlagRequest_result) = 0 ;
	virtual HRESULT __safecall Set_FlagRequest(const WideString FlagRequest) = 0 ;
	virtual HRESULT __safecall Get_FlagStatus(Activex::TOleEnum &Get_FlagStatus_result) = 0 ;
	virtual HRESULT __safecall Set_FlagStatus(Activex::TOleEnum FlagStatus) = 0 ;
	virtual HRESULT __safecall Get_HTMLBody(WideString &Get_HTMLBody_result) = 0 ;
	virtual HRESULT __safecall Set_HTMLBody(const WideString HTMLBody) = 0 ;
	virtual HRESULT __safecall Get_OriginatorDeliveryReportRequested(Word &Get_OriginatorDeliveryReportRequested_result) = 0 ;
	virtual HRESULT __safecall Set_OriginatorDeliveryReportRequested(Word OriginatorDeliveryReportRequested) = 0 ;
	virtual HRESULT __safecall Get_ReadReceiptRequested(Word &Get_ReadReceiptRequested_result) = 0 ;
	virtual HRESULT __safecall Set_ReadReceiptRequested(Word ReadReceiptRequested) = 0 ;
	virtual HRESULT __safecall Get_ReceivedByEntryID(WideString &Get_ReceivedByEntryID_result) = 0 ;
	virtual HRESULT __safecall Get_ReceivedByName(WideString &Get_ReceivedByName_result) = 0 ;
	virtual HRESULT __safecall Get_ReceivedOnBehalfOfEntryID(WideString &Get_ReceivedOnBehalfOfEntryID_result) = 0 ;
	virtual HRESULT __safecall Get_ReceivedOnBehalfOfName(WideString &Get_ReceivedOnBehalfOfName_result) = 0 ;
	virtual HRESULT __safecall Get_ReceivedTime(System::TDateTime &Get_ReceivedTime_result) = 0 ;
	virtual HRESULT __safecall Get_RecipientReassignmentProhibited(Word &Get_RecipientReassignmentProhibited_result) = 0 ;
	virtual HRESULT __safecall Set_RecipientReassignmentProhibited(Word RecipientReassignmentProhibited) = 0 ;
	virtual HRESULT __safecall Get_Recipients(_di_Recipients &Get_Recipients_result) = 0 ;
	virtual HRESULT __safecall Get_ReminderOverrideDefault(Word &Get_ReminderOverrideDefault_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderOverrideDefault(Word ReminderOverrideDefault) = 0 ;
	virtual HRESULT __safecall Get_ReminderPlaySound(Word &Get_ReminderPlaySound_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderPlaySound(Word ReminderPlaySound) = 0 ;
	virtual HRESULT __safecall Get_ReminderSet(Word &Get_ReminderSet_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderSet(Word ReminderSet) = 0 ;
	virtual HRESULT __safecall Get_ReminderSoundFile(WideString &Get_ReminderSoundFile_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderSoundFile(const WideString ReminderSoundFile) = 0 ;
	virtual HRESULT __safecall Get_ReminderTime(System::TDateTime &Get_ReminderTime_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderTime(System::TDateTime ReminderTime) = 0 ;
	virtual HRESULT __safecall Get_RemoteStatus(Activex::TOleEnum &Get_RemoteStatus_result) = 0 ;
	virtual HRESULT __safecall Set_RemoteStatus(Activex::TOleEnum RemoteStatus) = 0 ;
	virtual HRESULT __safecall Get_ReplyRecipientNames(WideString &Get_ReplyRecipientNames_result) = 0 ;
	virtual HRESULT __safecall Get_ReplyRecipients(_di_Recipients &Get_ReplyRecipients_result) = 0 ;
	virtual HRESULT __safecall Get_SaveSentMessageFolder(_di_MAPIFolder &Get_SaveSentMessageFolder_result) = 0 ;
	virtual HRESULT __safecall Set_SaveSentMessageFolder(const _di_MAPIFolder SaveSentMessageFolder) = 0 ;
	virtual HRESULT __safecall Get_SenderName(WideString &Get_SenderName_result) = 0 ;
	virtual HRESULT __safecall Get_Sent(Word &Get_Sent_result) = 0 ;
	virtual HRESULT __safecall Get_SentOn(System::TDateTime &Get_SentOn_result) = 0 ;
	virtual HRESULT __safecall Get_SentOnBehalfOfName(WideString &Get_SentOnBehalfOfName_result) = 0 ;
	virtual HRESULT __safecall Set_SentOnBehalfOfName(const WideString SentOnBehalfOfName) = 0 ;
	virtual HRESULT __safecall Get_Submitted(Word &Get_Submitted_result) = 0 ;
	virtual HRESULT __safecall Get_To_(WideString &Get_To__result) = 0 ;
	virtual HRESULT __safecall Set_To_(const WideString To_) = 0 ;
	virtual HRESULT __safecall Get_VotingOptions(WideString &Get_VotingOptions_result) = 0 ;
	virtual HRESULT __safecall Set_VotingOptions(const WideString VotingOptions) = 0 ;
	virtual HRESULT __safecall Get_VotingResponse(WideString &Get_VotingResponse_result) = 0 ;
	virtual HRESULT __safecall Set_VotingResponse(const WideString VotingResponse) = 0 ;
	virtual HRESULT __safecall ClearConversationIndex(void) = 0 ;
	virtual HRESULT __safecall Forward(_di__MailItem &Forward_result) = 0 ;
	virtual HRESULT __safecall Reply(_di__MailItem &Reply_result) = 0 ;
	virtual HRESULT __safecall ReplyAll(_di__MailItem &ReplyAll_result) = 0 ;
	virtual HRESULT __safecall Send(void) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_BodyFormat(Activex::TOleEnum &Get_BodyFormat_result) = 0 ;
	virtual HRESULT __safecall Set_BodyFormat(Activex::TOleEnum BodyFormat) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_InternetCodepage(int &Get_InternetCodepage_result) = 0 ;
	virtual HRESULT __safecall Set_InternetCodepage(int InternetCodepage) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_AlternateRecipientAllowed() { Word r; HRESULT hr = Get_AlternateRecipientAllowed(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word AlternateRecipientAllowed = {read=_scw_Get_AlternateRecipientAllowed, write=Set_AlternateRecipientAllowed};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_AutoForwarded() { Word r; HRESULT hr = Get_AutoForwarded(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word AutoForwarded = {read=_scw_Get_AutoForwarded, write=Set_AutoForwarded};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BCC() { WideString r; HRESULT hr = Get_BCC(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BCC = {read=_scw_Get_BCC, write=Set_BCC};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CC() { WideString r; HRESULT hr = Get_CC(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CC = {read=_scw_Get_CC, write=Set_CC};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_DeferredDeliveryTime() { System::TDateTime r; HRESULT hr = Get_DeferredDeliveryTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime DeferredDeliveryTime = {read=_scw_Get_DeferredDeliveryTime, write=Set_DeferredDeliveryTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DeleteAfterSubmit() { Word r; HRESULT hr = Get_DeleteAfterSubmit(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DeleteAfterSubmit = {read=_scw_Get_DeleteAfterSubmit, write=Set_DeleteAfterSubmit};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_ExpiryTime() { System::TDateTime r; HRESULT hr = Get_ExpiryTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime ExpiryTime = {read=_scw_Get_ExpiryTime, write=Set_ExpiryTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_FlagDueBy() { System::TDateTime r; HRESULT hr = Get_FlagDueBy(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime FlagDueBy = {read=_scw_Get_FlagDueBy, write=Set_FlagDueBy};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FlagRequest() { WideString r; HRESULT hr = Get_FlagRequest(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FlagRequest = {read=_scw_Get_FlagRequest, write=Set_FlagRequest};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_FlagStatus() { Activex::TOleEnum r; HRESULT hr = Get_FlagStatus(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum FlagStatus = {read=_scw_Get_FlagStatus, write=Set_FlagStatus};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HTMLBody() { WideString r; HRESULT hr = Get_HTMLBody(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HTMLBody = {read=_scw_Get_HTMLBody, write=Set_HTMLBody};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_OriginatorDeliveryReportRequested() { Word r; HRESULT hr = Get_OriginatorDeliveryReportRequested(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word OriginatorDeliveryReportRequested = {read=_scw_Get_OriginatorDeliveryReportRequested, write=Set_OriginatorDeliveryReportRequested};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ReadReceiptRequested() { Word r; HRESULT hr = Get_ReadReceiptRequested(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ReadReceiptRequested = {read=_scw_Get_ReadReceiptRequested, write=Set_ReadReceiptRequested};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ReceivedByEntryID() { WideString r; HRESULT hr = Get_ReceivedByEntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ReceivedByEntryID = {read=_scw_Get_ReceivedByEntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ReceivedByName() { WideString r; HRESULT hr = Get_ReceivedByName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ReceivedByName = {read=_scw_Get_ReceivedByName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ReceivedOnBehalfOfEntryID() { WideString r; HRESULT hr = Get_ReceivedOnBehalfOfEntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ReceivedOnBehalfOfEntryID = {read=_scw_Get_ReceivedOnBehalfOfEntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ReceivedOnBehalfOfName() { WideString r; HRESULT hr = Get_ReceivedOnBehalfOfName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ReceivedOnBehalfOfName = {read=_scw_Get_ReceivedOnBehalfOfName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_ReceivedTime() { System::TDateTime r; HRESULT hr = Get_ReceivedTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime ReceivedTime = {read=_scw_Get_ReceivedTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_RecipientReassignmentProhibited() { Word r; HRESULT hr = Get_RecipientReassignmentProhibited(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word RecipientReassignmentProhibited = {read=_scw_Get_RecipientReassignmentProhibited, write=Set_RecipientReassignmentProhibited};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Recipients _scw_Get_Recipients() { _di_Recipients r; HRESULT hr = Get_Recipients(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Recipients Recipients = {read=_scw_Get_Recipients};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ReminderOverrideDefault() { Word r; HRESULT hr = Get_ReminderOverrideDefault(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ReminderOverrideDefault = {read=_scw_Get_ReminderOverrideDefault, write=Set_ReminderOverrideDefault};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ReminderPlaySound() { Word r; HRESULT hr = Get_ReminderPlaySound(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ReminderPlaySound = {read=_scw_Get_ReminderPlaySound, write=Set_ReminderPlaySound};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ReminderSet() { Word r; HRESULT hr = Get_ReminderSet(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ReminderSet = {read=_scw_Get_ReminderSet, write=Set_ReminderSet};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ReminderSoundFile() { WideString r; HRESULT hr = Get_ReminderSoundFile(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ReminderSoundFile = {read=_scw_Get_ReminderSoundFile, write=Set_ReminderSoundFile};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_ReminderTime() { System::TDateTime r; HRESULT hr = Get_ReminderTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime ReminderTime = {read=_scw_Get_ReminderTime, write=Set_ReminderTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_RemoteStatus() { Activex::TOleEnum r; HRESULT hr = Get_RemoteStatus(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum RemoteStatus = {read=_scw_Get_RemoteStatus, write=Set_RemoteStatus};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ReplyRecipientNames() { WideString r; HRESULT hr = Get_ReplyRecipientNames(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ReplyRecipientNames = {read=_scw_Get_ReplyRecipientNames};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Recipients _scw_Get_ReplyRecipients() { _di_Recipients r; HRESULT hr = Get_ReplyRecipients(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Recipients ReplyRecipients = {read=_scw_Get_ReplyRecipients};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_MAPIFolder _scw_Get_SaveSentMessageFolder() { _di_MAPIFolder r; HRESULT hr = Get_SaveSentMessageFolder(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_MAPIFolder SaveSentMessageFolder = {read=_scw_Get_SaveSentMessageFolder, write=Set_SaveSentMessageFolder};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_SenderName() { WideString r; HRESULT hr = Get_SenderName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString SenderName = {read=_scw_Get_SenderName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Sent() { Word r; HRESULT hr = Get_Sent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Sent = {read=_scw_Get_Sent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_SentOn() { System::TDateTime r; HRESULT hr = Get_SentOn(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime SentOn = {read=_scw_Get_SentOn};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_SentOnBehalfOfName() { WideString r; HRESULT hr = Get_SentOnBehalfOfName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString SentOnBehalfOfName = {read=_scw_Get_SentOnBehalfOfName, write=Set_SentOnBehalfOfName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Submitted() { Word r; HRESULT hr = Get_Submitted(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Submitted = {read=_scw_Get_Submitted};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_To_() { WideString r; HRESULT hr = Get_To_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString To_ = {read=_scw_Get_To_, write=Set_To_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_VotingOptions() { WideString r; HRESULT hr = Get_VotingOptions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString VotingOptions = {read=_scw_Get_VotingOptions, write=Set_VotingOptions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_VotingResponse() { WideString r; HRESULT hr = Get_VotingResponse(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString VotingResponse = {read=_scw_Get_VotingResponse, write=Set_VotingResponse};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_BodyFormat() { Activex::TOleEnum r; HRESULT hr = Get_BodyFormat(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum BodyFormat = {read=_scw_Get_BodyFormat, write=Set_BodyFormat};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_InternetCodepage() { int r; HRESULT hr = Get_InternetCodepage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int InternetCodepage = {read=_scw_Get_InternetCodepage, write=Set_InternetCodepage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _MailItemDisp;
typedef System::DelphiInterface<_MailItemDisp> _di__MailItemDisp;
__dispinterface INTERFACE_UUID("{00063034-0000-0000-C000-000000000046}") _MailItemDisp  : public IDispatch 
{
	
};

__dispinterface ItemEvents;
typedef System::DelphiInterface<ItemEvents> _di_ItemEvents;
__dispinterface INTERFACE_UUID("{0006303A-0000-0000-C000-000000000046}") ItemEvents  : public IDispatch 
{
	
};

__interface Link;
typedef System::DelphiInterface<Link> _di_Link;
__interface INTERFACE_UUID("{0006308A-0000-0000-C000-000000000046}") Links  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_Link &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const _di_IDispatch Item, _di_Link &Add_result) = 0 ;
	virtual HRESULT __safecall Remove(const OleVariant Index) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface LinksDisp;
typedef System::DelphiInterface<LinksDisp> _di_LinksDisp;
__dispinterface INTERFACE_UUID("{0006308A-0000-0000-C000-000000000046}") LinksDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063089-0000-0000-C000-000000000046}") Link  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_Item(_di_IDispatch &Get_Item_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Item() { _di_IDispatch r; HRESULT hr = Get_Item(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Item = {read=_scw_Get_Item};
};

__dispinterface LinkDisp;
typedef System::DelphiInterface<LinkDisp> _di_LinkDisp;
__dispinterface INTERFACE_UUID("{00063089-0000-0000-C000-000000000046}") LinkDisp  : public IDispatch 
{
	
};

__interface ItemProperty;
typedef System::DelphiInterface<ItemProperty> _di_ItemProperty;
__interface INTERFACE_UUID("{000630A8-0000-0000-C000-000000000046}") ItemProperties  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_ItemProperty &Item_result) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &_NewEnum_result) = 0 ;
	virtual HRESULT __safecall Add(const WideString Name, Activex::TOleEnum Type_, const OleVariant AddToFolderFields, const OleVariant DisplayFormat, _di_ItemProperty &Add_result) = 0 ;
	virtual HRESULT __safecall Remove(int Index) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface ItemPropertiesDisp;
typedef System::DelphiInterface<ItemPropertiesDisp> _di_ItemPropertiesDisp;
__dispinterface INTERFACE_UUID("{000630A8-0000-0000-C000-000000000046}") ItemPropertiesDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{000630A7-0000-0000-C000-000000000046}") ItemProperty  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Formula(WideString &Get_Formula_result) = 0 ;
	virtual HRESULT __safecall Set_Formula(const WideString Formula) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Type_(Activex::TOleEnum &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Get_ValidationFormula(WideString &Get_ValidationFormula_result) = 0 ;
	virtual HRESULT __safecall Set_ValidationFormula(const WideString ValidationFormula) = 0 ;
	virtual HRESULT __safecall Get_ValidationText(WideString &Get_ValidationText_result) = 0 ;
	virtual HRESULT __safecall Set_ValidationText(const WideString ValidationText) = 0 ;
	virtual HRESULT __safecall Get_Value(OleVariant &Get_Value_result) = 0 ;
	virtual HRESULT __safecall Set_Value(const OleVariant Value) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Get_IsUserProperty(Word &Get_IsUserProperty_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Formula() { WideString r; HRESULT hr = Get_Formula(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Formula = {read=_scw_Get_Formula, write=Set_Formula};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Type_() { Activex::TOleEnum r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Type_ = {read=_scw_Get_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ValidationFormula() { WideString r; HRESULT hr = Get_ValidationFormula(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ValidationFormula = {read=_scw_Get_ValidationFormula, write=Set_ValidationFormula};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ValidationText() { WideString r; HRESULT hr = Get_ValidationText(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ValidationText = {read=_scw_Get_ValidationText, write=Set_ValidationText};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_Value() { OleVariant r; HRESULT hr = Get_Value(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant Value = {read=_scw_Get_Value, write=Set_Value};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsUserProperty() { Word r; HRESULT hr = Get_IsUserProperty(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsUserProperty = {read=_scw_Get_IsUserProperty};
};

__dispinterface ItemPropertyDisp;
typedef System::DelphiInterface<ItemPropertyDisp> _di_ItemPropertyDisp;
__dispinterface INTERFACE_UUID("{000630A7-0000-0000-C000-000000000046}") ItemPropertyDisp  : public IDispatch 
{
	
};

__dispinterface ItemEvents_10;
typedef System::DelphiInterface<ItemEvents_10> _di_ItemEvents_10;
__dispinterface INTERFACE_UUID("{0006302B-0000-0000-C000-000000000046}") ItemEvents_10  : public IDispatch 
{
	
};

__interface Exceptions;
typedef System::DelphiInterface<Exceptions> _di_Exceptions;
__interface INTERFACE_UUID("{00063044-0000-0000-C000-000000000046}") RecurrencePattern  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_DayOfMonth(int &Get_DayOfMonth_result) = 0 ;
	virtual HRESULT __safecall Set_DayOfMonth(int DayOfMonth) = 0 ;
	virtual HRESULT __safecall Get_DayOfWeekMask(Activex::TOleEnum &Get_DayOfWeekMask_result) = 0 ;
	virtual HRESULT __safecall Set_DayOfWeekMask(Activex::TOleEnum DayOfWeekMask) = 0 ;
	virtual HRESULT __safecall Get_Duration(int &Get_Duration_result) = 0 ;
	virtual HRESULT __safecall Set_Duration(int Duration) = 0 ;
	virtual HRESULT __safecall Get_EndTime(System::TDateTime &Get_EndTime_result) = 0 ;
	virtual HRESULT __safecall Set_EndTime(System::TDateTime EndTime) = 0 ;
	virtual HRESULT __safecall Get_Exceptions(_di_Exceptions &Get_Exceptions_result) = 0 ;
	virtual HRESULT __safecall Get_Instance(int &Get_Instance_result) = 0 ;
	virtual HRESULT __safecall Set_Instance(int Instance) = 0 ;
	virtual HRESULT __safecall Get_Interval(int &Get_Interval_result) = 0 ;
	virtual HRESULT __safecall Set_Interval(int Interval) = 0 ;
	virtual HRESULT __safecall Get_MonthOfYear(int &Get_MonthOfYear_result) = 0 ;
	virtual HRESULT __safecall Set_MonthOfYear(int MonthOfYear) = 0 ;
	virtual HRESULT __safecall Get_NoEndDate(Word &Get_NoEndDate_result) = 0 ;
	virtual HRESULT __safecall Set_NoEndDate(Word NoEndDate) = 0 ;
	virtual HRESULT __safecall Get_Occurrences(int &Get_Occurrences_result) = 0 ;
	virtual HRESULT __safecall Set_Occurrences(int Occurrences) = 0 ;
	virtual HRESULT __safecall Get_PatternEndDate(System::TDateTime &Get_PatternEndDate_result) = 0 ;
	virtual HRESULT __safecall Set_PatternEndDate(System::TDateTime PatternEndDate) = 0 ;
	virtual HRESULT __safecall Get_PatternStartDate(System::TDateTime &Get_PatternStartDate_result) = 0 ;
	virtual HRESULT __safecall Set_PatternStartDate(System::TDateTime PatternStartDate) = 0 ;
	virtual HRESULT __safecall Get_RecurrenceType(Activex::TOleEnum &Get_RecurrenceType_result) = 0 ;
	virtual HRESULT __safecall Set_RecurrenceType(Activex::TOleEnum RecurrenceType) = 0 ;
	virtual HRESULT __safecall Get_Regenerate(Word &Get_Regenerate_result) = 0 ;
	virtual HRESULT __safecall Set_Regenerate(Word Regenerate) = 0 ;
	virtual HRESULT __safecall Get_StartTime(System::TDateTime &Get_StartTime_result) = 0 ;
	virtual HRESULT __safecall Set_StartTime(System::TDateTime StartTime) = 0 ;
	virtual HRESULT __safecall GetOccurrence(System::TDateTime StartDate, _di__AppointmentItem &GetOccurrence_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_DayOfMonth() { int r; HRESULT hr = Get_DayOfMonth(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int DayOfMonth = {read=_scw_Get_DayOfMonth, write=Set_DayOfMonth};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DayOfWeekMask() { Activex::TOleEnum r; HRESULT hr = Get_DayOfWeekMask(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DayOfWeekMask = {read=_scw_Get_DayOfWeekMask, write=Set_DayOfWeekMask};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Duration() { int r; HRESULT hr = Get_Duration(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Duration = {read=_scw_Get_Duration, write=Set_Duration};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_EndTime() { System::TDateTime r; HRESULT hr = Get_EndTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime EndTime = {read=_scw_Get_EndTime, write=Set_EndTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Exceptions _scw_Get_Exceptions() { _di_Exceptions r; HRESULT hr = Get_Exceptions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Exceptions Exceptions = {read=_scw_Get_Exceptions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Instance() { int r; HRESULT hr = Get_Instance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Instance = {read=_scw_Get_Instance, write=Set_Instance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Interval() { int r; HRESULT hr = Get_Interval(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Interval = {read=_scw_Get_Interval, write=Set_Interval};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_MonthOfYear() { int r; HRESULT hr = Get_MonthOfYear(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int MonthOfYear = {read=_scw_Get_MonthOfYear, write=Set_MonthOfYear};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoEndDate() { Word r; HRESULT hr = Get_NoEndDate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoEndDate = {read=_scw_Get_NoEndDate, write=Set_NoEndDate};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Occurrences() { int r; HRESULT hr = Get_Occurrences(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Occurrences = {read=_scw_Get_Occurrences, write=Set_Occurrences};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_PatternEndDate() { System::TDateTime r; HRESULT hr = Get_PatternEndDate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime PatternEndDate = {read=_scw_Get_PatternEndDate, write=Set_PatternEndDate};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_PatternStartDate() { System::TDateTime r; HRESULT hr = Get_PatternStartDate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime PatternStartDate = {read=_scw_Get_PatternStartDate, write=Set_PatternStartDate};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_RecurrenceType() { Activex::TOleEnum r; HRESULT hr = Get_RecurrenceType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum RecurrenceType = {read=_scw_Get_RecurrenceType, write=Set_RecurrenceType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Regenerate() { Word r; HRESULT hr = Get_Regenerate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Regenerate = {read=_scw_Get_Regenerate, write=Set_Regenerate};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_StartTime() { System::TDateTime r; HRESULT hr = Get_StartTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime StartTime = {read=_scw_Get_StartTime, write=Set_StartTime};
};

__dispinterface RecurrencePatternDisp;
typedef System::DelphiInterface<RecurrencePatternDisp> _di_RecurrencePatternDisp;
__dispinterface INTERFACE_UUID("{00063044-0000-0000-C000-000000000046}") RecurrencePatternDisp  : public IDispatch 
{
	
};

__interface Exception;
typedef System::DelphiInterface<Exception> _di_Exception;
__interface INTERFACE_UUID("{0006304C-0000-0000-C000-000000000046}") Exceptions  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_Exception &Item_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface ExceptionsDisp;
typedef System::DelphiInterface<ExceptionsDisp> _di_ExceptionsDisp;
__dispinterface INTERFACE_UUID("{0006304C-0000-0000-C000-000000000046}") ExceptionsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0006304D-0000-0000-C000-000000000046}") Exception  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_AppointmentItem(_di__AppointmentItem &Get_AppointmentItem_result) = 0 ;
	virtual HRESULT __safecall Get_Deleted(Word &Get_Deleted_result) = 0 ;
	virtual HRESULT __safecall Get_OriginalDate(System::TDateTime &Get_OriginalDate_result) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__AppointmentItem _scw_Get_AppointmentItem() { _di__AppointmentItem r; HRESULT hr = Get_AppointmentItem(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__AppointmentItem AppointmentItem = {read=_scw_Get_AppointmentItem};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Deleted() { Word r; HRESULT hr = Get_Deleted(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Deleted = {read=_scw_Get_Deleted};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_OriginalDate() { System::TDateTime r; HRESULT hr = Get_OriginalDate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime OriginalDate = {read=_scw_Get_OriginalDate};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
};

__dispinterface ExceptionDisp;
typedef System::DelphiInterface<ExceptionDisp> _di_ExceptionDisp;
__dispinterface INTERFACE_UUID("{0006304D-0000-0000-C000-000000000046}") ExceptionDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063062-0000-0000-C000-000000000046}") _MeetingItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall Get_AutoForwarded(Word &Get_AutoForwarded_result) = 0 ;
	virtual HRESULT __safecall Set_AutoForwarded(Word AutoForwarded) = 0 ;
	virtual HRESULT __safecall Get_DeferredDeliveryTime(System::TDateTime &Get_DeferredDeliveryTime_result) = 0 ;
	virtual HRESULT __safecall Set_DeferredDeliveryTime(System::TDateTime DeferredDeliveryTime) = 0 ;
	virtual HRESULT __safecall Get_DeleteAfterSubmit(Word &Get_DeleteAfterSubmit_result) = 0 ;
	virtual HRESULT __safecall Set_DeleteAfterSubmit(Word DeleteAfterSubmit) = 0 ;
	virtual HRESULT __safecall Get_ExpiryTime(System::TDateTime &Get_ExpiryTime_result) = 0 ;
	virtual HRESULT __safecall Set_ExpiryTime(System::TDateTime ExpiryTime) = 0 ;
	virtual HRESULT __safecall Get_FlagDueBy(System::TDateTime &Get_FlagDueBy_result) = 0 ;
	virtual HRESULT __safecall Set_FlagDueBy(System::TDateTime FlagDueBy) = 0 ;
	virtual HRESULT __safecall Get_FlagRequest(WideString &Get_FlagRequest_result) = 0 ;
	virtual HRESULT __safecall Set_FlagRequest(const WideString FlagRequest) = 0 ;
	virtual HRESULT __safecall Get_FlagStatus(Activex::TOleEnum &Get_FlagStatus_result) = 0 ;
	virtual HRESULT __safecall Set_FlagStatus(Activex::TOleEnum FlagStatus) = 0 ;
	virtual HRESULT __safecall Get_OriginatorDeliveryReportRequested(Word &Get_OriginatorDeliveryReportRequested_result) = 0 ;
	virtual HRESULT __safecall Set_OriginatorDeliveryReportRequested(Word OriginatorDeliveryReportRequested) = 0 ;
	virtual HRESULT __safecall Get_ReceivedTime(System::TDateTime &Get_ReceivedTime_result) = 0 ;
	virtual HRESULT __safecall Set_ReceivedTime(System::TDateTime ReceivedTime) = 0 ;
	virtual HRESULT __safecall Get_Recipients(_di_Recipients &Get_Recipients_result) = 0 ;
	virtual HRESULT __safecall Get_ReminderSet(Word &Get_ReminderSet_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderSet(Word ReminderSet) = 0 ;
	virtual HRESULT __safecall Get_ReminderTime(System::TDateTime &Get_ReminderTime_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderTime(System::TDateTime ReminderTime) = 0 ;
	virtual HRESULT __safecall Get_ReplyRecipients(_di_Recipients &Get_ReplyRecipients_result) = 0 ;
	virtual HRESULT __safecall Get_SaveSentMessageFolder(_di_MAPIFolder &Get_SaveSentMessageFolder_result) = 0 ;
	virtual HRESULT __safecall Set_SaveSentMessageFolder(const _di_MAPIFolder SaveSentMessageFolder) = 0 ;
	virtual HRESULT __safecall Get_SenderName(WideString &Get_SenderName_result) = 0 ;
	virtual HRESULT __safecall Get_Sent(Word &Get_Sent_result) = 0 ;
	virtual HRESULT __safecall Get_SentOn(System::TDateTime &Get_SentOn_result) = 0 ;
	virtual HRESULT __safecall Get_Submitted(Word &Get_Submitted_result) = 0 ;
	virtual HRESULT __safecall Forward(_di__MeetingItem &Forward_result) = 0 ;
	virtual HRESULT __safecall GetAssociatedAppointment(Word AddToCalendar, _di__AppointmentItem &GetAssociatedAppointment_result) = 0 ;
	virtual HRESULT __safecall Reply(_di__MailItem &Reply_result) = 0 ;
	virtual HRESULT __safecall ReplyAll(_di__MailItem &ReplyAll_result) = 0 ;
	virtual HRESULT __safecall Send(void) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_AutoForwarded() { Word r; HRESULT hr = Get_AutoForwarded(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word AutoForwarded = {read=_scw_Get_AutoForwarded, write=Set_AutoForwarded};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_DeferredDeliveryTime() { System::TDateTime r; HRESULT hr = Get_DeferredDeliveryTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime DeferredDeliveryTime = {read=_scw_Get_DeferredDeliveryTime, write=Set_DeferredDeliveryTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DeleteAfterSubmit() { Word r; HRESULT hr = Get_DeleteAfterSubmit(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DeleteAfterSubmit = {read=_scw_Get_DeleteAfterSubmit, write=Set_DeleteAfterSubmit};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_ExpiryTime() { System::TDateTime r; HRESULT hr = Get_ExpiryTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime ExpiryTime = {read=_scw_Get_ExpiryTime, write=Set_ExpiryTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_FlagDueBy() { System::TDateTime r; HRESULT hr = Get_FlagDueBy(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime FlagDueBy = {read=_scw_Get_FlagDueBy, write=Set_FlagDueBy};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FlagRequest() { WideString r; HRESULT hr = Get_FlagRequest(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FlagRequest = {read=_scw_Get_FlagRequest, write=Set_FlagRequest};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_FlagStatus() { Activex::TOleEnum r; HRESULT hr = Get_FlagStatus(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum FlagStatus = {read=_scw_Get_FlagStatus, write=Set_FlagStatus};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_OriginatorDeliveryReportRequested() { Word r; HRESULT hr = Get_OriginatorDeliveryReportRequested(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word OriginatorDeliveryReportRequested = {read=_scw_Get_OriginatorDeliveryReportRequested, write=Set_OriginatorDeliveryReportRequested};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_ReceivedTime() { System::TDateTime r; HRESULT hr = Get_ReceivedTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime ReceivedTime = {read=_scw_Get_ReceivedTime, write=Set_ReceivedTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Recipients _scw_Get_Recipients() { _di_Recipients r; HRESULT hr = Get_Recipients(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Recipients Recipients = {read=_scw_Get_Recipients};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ReminderSet() { Word r; HRESULT hr = Get_ReminderSet(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ReminderSet = {read=_scw_Get_ReminderSet, write=Set_ReminderSet};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_ReminderTime() { System::TDateTime r; HRESULT hr = Get_ReminderTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime ReminderTime = {read=_scw_Get_ReminderTime, write=Set_ReminderTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Recipients _scw_Get_ReplyRecipients() { _di_Recipients r; HRESULT hr = Get_ReplyRecipients(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Recipients ReplyRecipients = {read=_scw_Get_ReplyRecipients};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_MAPIFolder _scw_Get_SaveSentMessageFolder() { _di_MAPIFolder r; HRESULT hr = Get_SaveSentMessageFolder(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_MAPIFolder SaveSentMessageFolder = {read=_scw_Get_SaveSentMessageFolder, write=Set_SaveSentMessageFolder};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_SenderName() { WideString r; HRESULT hr = Get_SenderName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString SenderName = {read=_scw_Get_SenderName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Sent() { Word r; HRESULT hr = Get_Sent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Sent = {read=_scw_Get_Sent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_SentOn() { System::TDateTime r; HRESULT hr = Get_SentOn(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime SentOn = {read=_scw_Get_SentOn};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Submitted() { Word r; HRESULT hr = Get_Submitted(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Submitted = {read=_scw_Get_Submitted};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _MeetingItemDisp;
typedef System::DelphiInterface<_MeetingItemDisp> _di__MeetingItemDisp;
__dispinterface INTERFACE_UUID("{00063062-0000-0000-C000-000000000046}") _MeetingItemDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063021-0000-0000-C000-000000000046}") _ContactItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall Get_Account(WideString &Get_Account_result) = 0 ;
	virtual HRESULT __safecall Set_Account(const WideString Account) = 0 ;
	virtual HRESULT __safecall Get_Anniversary(System::TDateTime &Get_Anniversary_result) = 0 ;
	virtual HRESULT __safecall Set_Anniversary(System::TDateTime Anniversary) = 0 ;
	virtual HRESULT __safecall Get_AssistantName(WideString &Get_AssistantName_result) = 0 ;
	virtual HRESULT __safecall Set_AssistantName(const WideString AssistantName) = 0 ;
	virtual HRESULT __safecall Get_AssistantTelephoneNumber(WideString &Get_AssistantTelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_AssistantTelephoneNumber(const WideString AssistantTelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_Birthday(System::TDateTime &Get_Birthday_result) = 0 ;
	virtual HRESULT __safecall Set_Birthday(System::TDateTime Birthday) = 0 ;
	virtual HRESULT __safecall Get_Business2TelephoneNumber(WideString &Get_Business2TelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_Business2TelephoneNumber(const WideString Business2TelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_BusinessAddress(WideString &Get_BusinessAddress_result) = 0 ;
	virtual HRESULT __safecall Set_BusinessAddress(const WideString BusinessAddress) = 0 ;
	virtual HRESULT __safecall Get_BusinessAddressCity(WideString &Get_BusinessAddressCity_result) = 0 ;
	virtual HRESULT __safecall Set_BusinessAddressCity(const WideString BusinessAddressCity) = 0 ;
	virtual HRESULT __safecall Get_BusinessAddressCountry(WideString &Get_BusinessAddressCountry_result) = 0 ;
	virtual HRESULT __safecall Set_BusinessAddressCountry(const WideString BusinessAddressCountry) = 0 ;
	virtual HRESULT __safecall Get_BusinessAddressPostalCode(WideString &Get_BusinessAddressPostalCode_result) = 0 ;
	virtual HRESULT __safecall Set_BusinessAddressPostalCode(const WideString BusinessAddressPostalCode) = 0 ;
	virtual HRESULT __safecall Get_BusinessAddressPostOfficeBox(WideString &Get_BusinessAddressPostOfficeBox_result) = 0 ;
	virtual HRESULT __safecall Set_BusinessAddressPostOfficeBox(const WideString BusinessAddressPostOfficeBox) = 0 ;
	virtual HRESULT __safecall Get_BusinessAddressState(WideString &Get_BusinessAddressState_result) = 0 ;
	virtual HRESULT __safecall Set_BusinessAddressState(const WideString BusinessAddressState) = 0 ;
	virtual HRESULT __safecall Get_BusinessAddressStreet(WideString &Get_BusinessAddressStreet_result) = 0 ;
	virtual HRESULT __safecall Set_BusinessAddressStreet(const WideString BusinessAddressStreet) = 0 ;
	virtual HRESULT __safecall Get_BusinessFaxNumber(WideString &Get_BusinessFaxNumber_result) = 0 ;
	virtual HRESULT __safecall Set_BusinessFaxNumber(const WideString BusinessFaxNumber) = 0 ;
	virtual HRESULT __safecall Get_BusinessHomePage(WideString &Get_BusinessHomePage_result) = 0 ;
	virtual HRESULT __safecall Set_BusinessHomePage(const WideString BusinessHomePage) = 0 ;
	virtual HRESULT __safecall Get_BusinessTelephoneNumber(WideString &Get_BusinessTelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_BusinessTelephoneNumber(const WideString BusinessTelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_CallbackTelephoneNumber(WideString &Get_CallbackTelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_CallbackTelephoneNumber(const WideString CallbackTelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_CarTelephoneNumber(WideString &Get_CarTelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_CarTelephoneNumber(const WideString CarTelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_Children(WideString &Get_Children_result) = 0 ;
	virtual HRESULT __safecall Set_Children(const WideString Children) = 0 ;
	virtual HRESULT __safecall Get_CompanyAndFullName(WideString &Get_CompanyAndFullName_result) = 0 ;
	virtual HRESULT __safecall Get_CompanyLastFirstNoSpace(WideString &Get_CompanyLastFirstNoSpace_result) = 0 ;
	virtual HRESULT __safecall Get_CompanyLastFirstSpaceOnly(WideString &Get_CompanyLastFirstSpaceOnly_result) = 0 ;
	virtual HRESULT __safecall Get_CompanyMainTelephoneNumber(WideString &Get_CompanyMainTelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_CompanyMainTelephoneNumber(const WideString CompanyMainTelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_CompanyName(WideString &Get_CompanyName_result) = 0 ;
	virtual HRESULT __safecall Set_CompanyName(const WideString CompanyName) = 0 ;
	virtual HRESULT __safecall Get_ComputerNetworkName(WideString &Get_ComputerNetworkName_result) = 0 ;
	virtual HRESULT __safecall Set_ComputerNetworkName(const WideString ComputerNetworkName) = 0 ;
	virtual HRESULT __safecall Get_CustomerID(WideString &Get_CustomerID_result) = 0 ;
	virtual HRESULT __safecall Set_CustomerID(const WideString CustomerID) = 0 ;
	virtual HRESULT __safecall Get_Department(WideString &Get_Department_result) = 0 ;
	virtual HRESULT __safecall Set_Department(const WideString Department) = 0 ;
	virtual HRESULT __safecall Get_Email1Address(WideString &Get_Email1Address_result) = 0 ;
	virtual HRESULT __safecall Set_Email1Address(const WideString Email1Address) = 0 ;
	virtual HRESULT __safecall Get_Email1AddressType(WideString &Get_Email1AddressType_result) = 0 ;
	virtual HRESULT __safecall Set_Email1AddressType(const WideString Email1AddressType) = 0 ;
	virtual HRESULT __safecall Get_Email1DisplayName(WideString &Get_Email1DisplayName_result) = 0 ;
	virtual HRESULT __safecall Get_Email1EntryID(WideString &Get_Email1EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_Email2Address(WideString &Get_Email2Address_result) = 0 ;
	virtual HRESULT __safecall Set_Email2Address(const WideString Email2Address) = 0 ;
	virtual HRESULT __safecall Get_Email2AddressType(WideString &Get_Email2AddressType_result) = 0 ;
	virtual HRESULT __safecall Set_Email2AddressType(const WideString Email2AddressType) = 0 ;
	virtual HRESULT __safecall Get_Email2DisplayName(WideString &Get_Email2DisplayName_result) = 0 ;
	virtual HRESULT __safecall Get_Email2EntryID(WideString &Get_Email2EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_Email3Address(WideString &Get_Email3Address_result) = 0 ;
	virtual HRESULT __safecall Set_Email3Address(const WideString Email3Address) = 0 ;
	virtual HRESULT __safecall Get_Email3AddressType(WideString &Get_Email3AddressType_result) = 0 ;
	virtual HRESULT __safecall Set_Email3AddressType(const WideString Email3AddressType) = 0 ;
	virtual HRESULT __safecall Get_Email3DisplayName(WideString &Get_Email3DisplayName_result) = 0 ;
	virtual HRESULT __safecall Get_Email3EntryID(WideString &Get_Email3EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FileAs(WideString &Get_FileAs_result) = 0 ;
	virtual HRESULT __safecall Set_FileAs(const WideString FileAs) = 0 ;
	virtual HRESULT __safecall Get_FirstName(WideString &Get_FirstName_result) = 0 ;
	virtual HRESULT __safecall Set_FirstName(const WideString FirstName) = 0 ;
	virtual HRESULT __safecall Get_FTPSite(WideString &Get_FTPSite_result) = 0 ;
	virtual HRESULT __safecall Set_FTPSite(const WideString FTPSite) = 0 ;
	virtual HRESULT __safecall Get_FullName(WideString &Get_FullName_result) = 0 ;
	virtual HRESULT __safecall Set_FullName(const WideString FullName) = 0 ;
	virtual HRESULT __safecall Get_FullNameAndCompany(WideString &Get_FullNameAndCompany_result) = 0 ;
	virtual HRESULT __safecall Get_Gender(Activex::TOleEnum &Get_Gender_result) = 0 ;
	virtual HRESULT __safecall Set_Gender(Activex::TOleEnum Gender) = 0 ;
	virtual HRESULT __safecall Get_GovernmentIDNumber(WideString &Get_GovernmentIDNumber_result) = 0 ;
	virtual HRESULT __safecall Set_GovernmentIDNumber(const WideString GovernmentIDNumber) = 0 ;
	virtual HRESULT __safecall Get_Hobby(WideString &Get_Hobby_result) = 0 ;
	virtual HRESULT __safecall Set_Hobby(const WideString Hobby) = 0 ;
	virtual HRESULT __safecall Get_Home2TelephoneNumber(WideString &Get_Home2TelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_Home2TelephoneNumber(const WideString Home2TelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_HomeAddress(WideString &Get_HomeAddress_result) = 0 ;
	virtual HRESULT __safecall Set_HomeAddress(const WideString HomeAddress) = 0 ;
	virtual HRESULT __safecall Get_HomeAddressCity(WideString &Get_HomeAddressCity_result) = 0 ;
	virtual HRESULT __safecall Set_HomeAddressCity(const WideString HomeAddressCity) = 0 ;
	virtual HRESULT __safecall Get_HomeAddressCountry(WideString &Get_HomeAddressCountry_result) = 0 ;
	virtual HRESULT __safecall Set_HomeAddressCountry(const WideString HomeAddressCountry) = 0 ;
	virtual HRESULT __safecall Get_HomeAddressPostalCode(WideString &Get_HomeAddressPostalCode_result) = 0 ;
	virtual HRESULT __safecall Set_HomeAddressPostalCode(const WideString HomeAddressPostalCode) = 0 ;
	virtual HRESULT __safecall Get_HomeAddressPostOfficeBox(WideString &Get_HomeAddressPostOfficeBox_result) = 0 ;
	virtual HRESULT __safecall Set_HomeAddressPostOfficeBox(const WideString HomeAddressPostOfficeBox) = 0 ;
	virtual HRESULT __safecall Get_HomeAddressState(WideString &Get_HomeAddressState_result) = 0 ;
	virtual HRESULT __safecall Set_HomeAddressState(const WideString HomeAddressState) = 0 ;
	virtual HRESULT __safecall Get_HomeAddressStreet(WideString &Get_HomeAddressStreet_result) = 0 ;
	virtual HRESULT __safecall Set_HomeAddressStreet(const WideString HomeAddressStreet) = 0 ;
	virtual HRESULT __safecall Get_HomeFaxNumber(WideString &Get_HomeFaxNumber_result) = 0 ;
	virtual HRESULT __safecall Set_HomeFaxNumber(const WideString HomeFaxNumber) = 0 ;
	virtual HRESULT __safecall Get_HomeTelephoneNumber(WideString &Get_HomeTelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_HomeTelephoneNumber(const WideString HomeTelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_Initials(WideString &Get_Initials_result) = 0 ;
	virtual HRESULT __safecall Set_Initials(const WideString Initials) = 0 ;
	virtual HRESULT __safecall Get_InternetFreeBusyAddress(WideString &Get_InternetFreeBusyAddress_result) = 0 ;
	virtual HRESULT __safecall Set_InternetFreeBusyAddress(const WideString InternetFreeBusyAddress) = 0 ;
	virtual HRESULT __safecall Get_ISDNNumber(WideString &Get_ISDNNumber_result) = 0 ;
	virtual HRESULT __safecall Set_ISDNNumber(const WideString ISDNNumber) = 0 ;
	virtual HRESULT __safecall Get_JobTitle(WideString &Get_JobTitle_result) = 0 ;
	virtual HRESULT __safecall Set_JobTitle(const WideString JobTitle) = 0 ;
	virtual HRESULT __safecall Get_Journal(Word &Get_Journal_result) = 0 ;
	virtual HRESULT __safecall Set_Journal(Word Journal) = 0 ;
	virtual HRESULT __safecall Get_Language(WideString &Get_Language_result) = 0 ;
	virtual HRESULT __safecall Set_Language(const WideString Language) = 0 ;
	virtual HRESULT __safecall Get_LastFirstAndSuffix(WideString &Get_LastFirstAndSuffix_result) = 0 ;
	virtual HRESULT __safecall Get_LastFirstNoSpace(WideString &Get_LastFirstNoSpace_result) = 0 ;
	virtual HRESULT __safecall Get_LastFirstNoSpaceCompany(WideString &Get_LastFirstNoSpaceCompany_result) = 0 ;
	virtual HRESULT __safecall Get_LastFirstSpaceOnly(WideString &Get_LastFirstSpaceOnly_result) = 0 ;
	virtual HRESULT __safecall Get_LastFirstSpaceOnlyCompany(WideString &Get_LastFirstSpaceOnlyCompany_result) = 0 ;
	virtual HRESULT __safecall Get_LastName(WideString &Get_LastName_result) = 0 ;
	virtual HRESULT __safecall Set_LastName(const WideString LastName) = 0 ;
	virtual HRESULT __safecall Get_LastNameAndFirstName(WideString &Get_LastNameAndFirstName_result) = 0 ;
	virtual HRESULT __safecall Get_MailingAddress(WideString &Get_MailingAddress_result) = 0 ;
	virtual HRESULT __safecall Set_MailingAddress(const WideString MailingAddress) = 0 ;
	virtual HRESULT __safecall Get_MailingAddressCity(WideString &Get_MailingAddressCity_result) = 0 ;
	virtual HRESULT __safecall Set_MailingAddressCity(const WideString MailingAddressCity) = 0 ;
	virtual HRESULT __safecall Get_MailingAddressCountry(WideString &Get_MailingAddressCountry_result) = 0 ;
	virtual HRESULT __safecall Set_MailingAddressCountry(const WideString MailingAddressCountry) = 0 ;
	virtual HRESULT __safecall Get_MailingAddressPostalCode(WideString &Get_MailingAddressPostalCode_result) = 0 ;
	virtual HRESULT __safecall Set_MailingAddressPostalCode(const WideString MailingAddressPostalCode) = 0 ;
	virtual HRESULT __safecall Get_MailingAddressPostOfficeBox(WideString &Get_MailingAddressPostOfficeBox_result) = 0 ;
	virtual HRESULT __safecall Set_MailingAddressPostOfficeBox(const WideString MailingAddressPostOfficeBox) = 0 ;
	virtual HRESULT __safecall Get_MailingAddressState(WideString &Get_MailingAddressState_result) = 0 ;
	virtual HRESULT __safecall Set_MailingAddressState(const WideString MailingAddressState) = 0 ;
	virtual HRESULT __safecall Get_MailingAddressStreet(WideString &Get_MailingAddressStreet_result) = 0 ;
	virtual HRESULT __safecall Set_MailingAddressStreet(const WideString MailingAddressStreet) = 0 ;
	virtual HRESULT __safecall Get_ManagerName(WideString &Get_ManagerName_result) = 0 ;
	virtual HRESULT __safecall Set_ManagerName(const WideString ManagerName) = 0 ;
	virtual HRESULT __safecall Get_MiddleName(WideString &Get_MiddleName_result) = 0 ;
	virtual HRESULT __safecall Set_MiddleName(const WideString MiddleName) = 0 ;
	virtual HRESULT __safecall Get_MobileTelephoneNumber(WideString &Get_MobileTelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_MobileTelephoneNumber(const WideString MobileTelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_NetMeetingAlias(WideString &Get_NetMeetingAlias_result) = 0 ;
	virtual HRESULT __safecall Set_NetMeetingAlias(const WideString NetMeetingAlias) = 0 ;
	virtual HRESULT __safecall Get_NetMeetingServer(WideString &Get_NetMeetingServer_result) = 0 ;
	virtual HRESULT __safecall Set_NetMeetingServer(const WideString NetMeetingServer) = 0 ;
	virtual HRESULT __safecall Get_NickName(WideString &Get_NickName_result) = 0 ;
	virtual HRESULT __safecall Set_NickName(const WideString NickName) = 0 ;
	virtual HRESULT __safecall Get_OfficeLocation(WideString &Get_OfficeLocation_result) = 0 ;
	virtual HRESULT __safecall Set_OfficeLocation(const WideString OfficeLocation) = 0 ;
	virtual HRESULT __safecall Get_OrganizationalIDNumber(WideString &Get_OrganizationalIDNumber_result) = 0 ;
	virtual HRESULT __safecall Set_OrganizationalIDNumber(const WideString OrganizationalIDNumber) = 0 ;
	virtual HRESULT __safecall Get_OtherAddress(WideString &Get_OtherAddress_result) = 0 ;
	virtual HRESULT __safecall Set_OtherAddress(const WideString OtherAddress) = 0 ;
	virtual HRESULT __safecall Get_OtherAddressCity(WideString &Get_OtherAddressCity_result) = 0 ;
	virtual HRESULT __safecall Set_OtherAddressCity(const WideString OtherAddressCity) = 0 ;
	virtual HRESULT __safecall Get_OtherAddressCountry(WideString &Get_OtherAddressCountry_result) = 0 ;
	virtual HRESULT __safecall Set_OtherAddressCountry(const WideString OtherAddressCountry) = 0 ;
	virtual HRESULT __safecall Get_OtherAddressPostalCode(WideString &Get_OtherAddressPostalCode_result) = 0 ;
	virtual HRESULT __safecall Set_OtherAddressPostalCode(const WideString OtherAddressPostalCode) = 0 ;
	virtual HRESULT __safecall Get_OtherAddressPostOfficeBox(WideString &Get_OtherAddressPostOfficeBox_result) = 0 ;
	virtual HRESULT __safecall Set_OtherAddressPostOfficeBox(const WideString OtherAddressPostOfficeBox) = 0 ;
	virtual HRESULT __safecall Get_OtherAddressState(WideString &Get_OtherAddressState_result) = 0 ;
	virtual HRESULT __safecall Set_OtherAddressState(const WideString OtherAddressState) = 0 ;
	virtual HRESULT __safecall Get_OtherAddressStreet(WideString &Get_OtherAddressStreet_result) = 0 ;
	virtual HRESULT __safecall Set_OtherAddressStreet(const WideString OtherAddressStreet) = 0 ;
	virtual HRESULT __safecall Get_OtherFaxNumber(WideString &Get_OtherFaxNumber_result) = 0 ;
	virtual HRESULT __safecall Set_OtherFaxNumber(const WideString OtherFaxNumber) = 0 ;
	virtual HRESULT __safecall Get_OtherTelephoneNumber(WideString &Get_OtherTelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_OtherTelephoneNumber(const WideString OtherTelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_PagerNumber(WideString &Get_PagerNumber_result) = 0 ;
	virtual HRESULT __safecall Set_PagerNumber(const WideString PagerNumber) = 0 ;
	virtual HRESULT __safecall Get_PersonalHomePage(WideString &Get_PersonalHomePage_result) = 0 ;
	virtual HRESULT __safecall Set_PersonalHomePage(const WideString PersonalHomePage) = 0 ;
	virtual HRESULT __safecall Get_PrimaryTelephoneNumber(WideString &Get_PrimaryTelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_PrimaryTelephoneNumber(const WideString PrimaryTelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_Profession(WideString &Get_Profession_result) = 0 ;
	virtual HRESULT __safecall Set_Profession(const WideString Profession) = 0 ;
	virtual HRESULT __safecall Get_RadioTelephoneNumber(WideString &Get_RadioTelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_RadioTelephoneNumber(const WideString RadioTelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_ReferredBy(WideString &Get_ReferredBy_result) = 0 ;
	virtual HRESULT __safecall Set_ReferredBy(const WideString ReferredBy) = 0 ;
	virtual HRESULT __safecall Get_SelectedMailingAddress(Activex::TOleEnum &Get_SelectedMailingAddress_result) = 0 ;
	virtual HRESULT __safecall Set_SelectedMailingAddress(Activex::TOleEnum SelectedMailingAddress) = 0 ;
	virtual HRESULT __safecall Get_Spouse(WideString &Get_Spouse_result) = 0 ;
	virtual HRESULT __safecall Set_Spouse(const WideString Spouse) = 0 ;
	virtual HRESULT __safecall Get_Suffix(WideString &Get_Suffix_result) = 0 ;
	virtual HRESULT __safecall Set_Suffix(const WideString Suffix) = 0 ;
	virtual HRESULT __safecall Get_TelexNumber(WideString &Get_TelexNumber_result) = 0 ;
	virtual HRESULT __safecall Set_TelexNumber(const WideString TelexNumber) = 0 ;
	virtual HRESULT __safecall Get_Title(WideString &Get_Title_result) = 0 ;
	virtual HRESULT __safecall Set_Title(const WideString Title) = 0 ;
	virtual HRESULT __safecall Get_TTYTDDTelephoneNumber(WideString &Get_TTYTDDTelephoneNumber_result) = 0 ;
	virtual HRESULT __safecall Set_TTYTDDTelephoneNumber(const WideString TTYTDDTelephoneNumber) = 0 ;
	virtual HRESULT __safecall Get_User1(WideString &Get_User1_result) = 0 ;
	virtual HRESULT __safecall Set_User1(const WideString User1) = 0 ;
	virtual HRESULT __safecall Get_User2(WideString &Get_User2_result) = 0 ;
	virtual HRESULT __safecall Set_User2(const WideString User2) = 0 ;
	virtual HRESULT __safecall Get_User3(WideString &Get_User3_result) = 0 ;
	virtual HRESULT __safecall Set_User3(const WideString User3) = 0 ;
	virtual HRESULT __safecall Get_User4(WideString &Get_User4_result) = 0 ;
	virtual HRESULT __safecall Set_User4(const WideString User4) = 0 ;
	virtual HRESULT __safecall Get_UserCertificate(WideString &Get_UserCertificate_result) = 0 ;
	virtual HRESULT __safecall Set_UserCertificate(const WideString UserCertificate) = 0 ;
	virtual HRESULT __safecall Get_WebPage(WideString &Get_WebPage_result) = 0 ;
	virtual HRESULT __safecall Set_WebPage(const WideString WebPage) = 0 ;
	virtual HRESULT __safecall Get_YomiCompanyName(WideString &Get_YomiCompanyName_result) = 0 ;
	virtual HRESULT __safecall Set_YomiCompanyName(const WideString YomiCompanyName) = 0 ;
	virtual HRESULT __safecall Get_YomiFirstName(WideString &Get_YomiFirstName_result) = 0 ;
	virtual HRESULT __safecall Set_YomiFirstName(const WideString YomiFirstName) = 0 ;
	virtual HRESULT __safecall Get_YomiLastName(WideString &Get_YomiLastName_result) = 0 ;
	virtual HRESULT __safecall Set_YomiLastName(const WideString YomiLastName) = 0 ;
	virtual HRESULT __safecall ForwardAsVcard(_di__MailItem &ForwardAsVcard_result) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_LastFirstNoSpaceAndSuffix(WideString &Get_LastFirstNoSpaceAndSuffix_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_IMAddress(WideString &Get_IMAddress_result) = 0 ;
	virtual HRESULT __safecall Set_IMAddress(const WideString IMAddress) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Set_Email1DisplayName(const WideString Email1DisplayName) = 0 ;
	virtual HRESULT __safecall Set_Email2DisplayName(const WideString Email2DisplayName) = 0 ;
	virtual HRESULT __safecall Set_Email3DisplayName(const WideString Email3DisplayName) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Account() { WideString r; HRESULT hr = Get_Account(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Account = {read=_scw_Get_Account, write=Set_Account};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_Anniversary() { System::TDateTime r; HRESULT hr = Get_Anniversary(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime Anniversary = {read=_scw_Get_Anniversary, write=Set_Anniversary};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_AssistantName() { WideString r; HRESULT hr = Get_AssistantName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString AssistantName = {read=_scw_Get_AssistantName, write=Set_AssistantName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_AssistantTelephoneNumber() { WideString r; HRESULT hr = Get_AssistantTelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString AssistantTelephoneNumber = {read=_scw_Get_AssistantTelephoneNumber, write=Set_AssistantTelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_Birthday() { System::TDateTime r; HRESULT hr = Get_Birthday(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime Birthday = {read=_scw_Get_Birthday, write=Set_Birthday};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Business2TelephoneNumber() { WideString r; HRESULT hr = Get_Business2TelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Business2TelephoneNumber = {read=_scw_Get_Business2TelephoneNumber, write=Set_Business2TelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BusinessAddress() { WideString r; HRESULT hr = Get_BusinessAddress(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BusinessAddress = {read=_scw_Get_BusinessAddress, write=Set_BusinessAddress};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BusinessAddressCity() { WideString r; HRESULT hr = Get_BusinessAddressCity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BusinessAddressCity = {read=_scw_Get_BusinessAddressCity, write=Set_BusinessAddressCity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BusinessAddressCountry() { WideString r; HRESULT hr = Get_BusinessAddressCountry(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BusinessAddressCountry = {read=_scw_Get_BusinessAddressCountry, write=Set_BusinessAddressCountry};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BusinessAddressPostalCode() { WideString r; HRESULT hr = Get_BusinessAddressPostalCode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BusinessAddressPostalCode = {read=_scw_Get_BusinessAddressPostalCode, write=Set_BusinessAddressPostalCode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BusinessAddressPostOfficeBox() { WideString r; HRESULT hr = Get_BusinessAddressPostOfficeBox(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BusinessAddressPostOfficeBox = {read=_scw_Get_BusinessAddressPostOfficeBox, write=Set_BusinessAddressPostOfficeBox};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BusinessAddressState() { WideString r; HRESULT hr = Get_BusinessAddressState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BusinessAddressState = {read=_scw_Get_BusinessAddressState, write=Set_BusinessAddressState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BusinessAddressStreet() { WideString r; HRESULT hr = Get_BusinessAddressStreet(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BusinessAddressStreet = {read=_scw_Get_BusinessAddressStreet, write=Set_BusinessAddressStreet};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BusinessFaxNumber() { WideString r; HRESULT hr = Get_BusinessFaxNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BusinessFaxNumber = {read=_scw_Get_BusinessFaxNumber, write=Set_BusinessFaxNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BusinessHomePage() { WideString r; HRESULT hr = Get_BusinessHomePage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BusinessHomePage = {read=_scw_Get_BusinessHomePage, write=Set_BusinessHomePage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BusinessTelephoneNumber() { WideString r; HRESULT hr = Get_BusinessTelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BusinessTelephoneNumber = {read=_scw_Get_BusinessTelephoneNumber, write=Set_BusinessTelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CallbackTelephoneNumber() { WideString r; HRESULT hr = Get_CallbackTelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CallbackTelephoneNumber = {read=_scw_Get_CallbackTelephoneNumber, write=Set_CallbackTelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CarTelephoneNumber() { WideString r; HRESULT hr = Get_CarTelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CarTelephoneNumber = {read=_scw_Get_CarTelephoneNumber, write=Set_CarTelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Children() { WideString r; HRESULT hr = Get_Children(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Children = {read=_scw_Get_Children, write=Set_Children};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CompanyAndFullName() { WideString r; HRESULT hr = Get_CompanyAndFullName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CompanyAndFullName = {read=_scw_Get_CompanyAndFullName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CompanyLastFirstNoSpace() { WideString r; HRESULT hr = Get_CompanyLastFirstNoSpace(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CompanyLastFirstNoSpace = {read=_scw_Get_CompanyLastFirstNoSpace};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CompanyLastFirstSpaceOnly() { WideString r; HRESULT hr = Get_CompanyLastFirstSpaceOnly(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CompanyLastFirstSpaceOnly = {read=_scw_Get_CompanyLastFirstSpaceOnly};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CompanyMainTelephoneNumber() { WideString r; HRESULT hr = Get_CompanyMainTelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CompanyMainTelephoneNumber = {read=_scw_Get_CompanyMainTelephoneNumber, write=Set_CompanyMainTelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CompanyName() { WideString r; HRESULT hr = Get_CompanyName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CompanyName = {read=_scw_Get_CompanyName, write=Set_CompanyName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ComputerNetworkName() { WideString r; HRESULT hr = Get_ComputerNetworkName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ComputerNetworkName = {read=_scw_Get_ComputerNetworkName, write=Set_ComputerNetworkName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CustomerID() { WideString r; HRESULT hr = Get_CustomerID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CustomerID = {read=_scw_Get_CustomerID, write=Set_CustomerID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Department() { WideString r; HRESULT hr = Get_Department(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Department = {read=_scw_Get_Department, write=Set_Department};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Email1Address() { WideString r; HRESULT hr = Get_Email1Address(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Email1Address = {read=_scw_Get_Email1Address, write=Set_Email1Address};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Email1AddressType() { WideString r; HRESULT hr = Get_Email1AddressType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Email1AddressType = {read=_scw_Get_Email1AddressType, write=Set_Email1AddressType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Email1DisplayName() { WideString r; HRESULT hr = Get_Email1DisplayName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Email1DisplayName = {read=_scw_Get_Email1DisplayName, write=Set_Email1DisplayName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Email1EntryID() { WideString r; HRESULT hr = Get_Email1EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Email1EntryID = {read=_scw_Get_Email1EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Email2Address() { WideString r; HRESULT hr = Get_Email2Address(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Email2Address = {read=_scw_Get_Email2Address, write=Set_Email2Address};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Email2AddressType() { WideString r; HRESULT hr = Get_Email2AddressType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Email2AddressType = {read=_scw_Get_Email2AddressType, write=Set_Email2AddressType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Email2DisplayName() { WideString r; HRESULT hr = Get_Email2DisplayName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Email2DisplayName = {read=_scw_Get_Email2DisplayName, write=Set_Email2DisplayName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Email2EntryID() { WideString r; HRESULT hr = Get_Email2EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Email2EntryID = {read=_scw_Get_Email2EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Email3Address() { WideString r; HRESULT hr = Get_Email3Address(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Email3Address = {read=_scw_Get_Email3Address, write=Set_Email3Address};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Email3AddressType() { WideString r; HRESULT hr = Get_Email3AddressType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Email3AddressType = {read=_scw_Get_Email3AddressType, write=Set_Email3AddressType};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Email3DisplayName() { WideString r; HRESULT hr = Get_Email3DisplayName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Email3DisplayName = {read=_scw_Get_Email3DisplayName, write=Set_Email3DisplayName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Email3EntryID() { WideString r; HRESULT hr = Get_Email3EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Email3EntryID = {read=_scw_Get_Email3EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FileAs() { WideString r; HRESULT hr = Get_FileAs(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FileAs = {read=_scw_Get_FileAs, write=Set_FileAs};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FirstName() { WideString r; HRESULT hr = Get_FirstName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FirstName = {read=_scw_Get_FirstName, write=Set_FirstName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FTPSite() { WideString r; HRESULT hr = Get_FTPSite(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FTPSite = {read=_scw_Get_FTPSite, write=Set_FTPSite};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FullName() { WideString r; HRESULT hr = Get_FullName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FullName = {read=_scw_Get_FullName, write=Set_FullName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_FullNameAndCompany() { WideString r; HRESULT hr = Get_FullNameAndCompany(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString FullNameAndCompany = {read=_scw_Get_FullNameAndCompany};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Gender() { Activex::TOleEnum r; HRESULT hr = Get_Gender(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Gender = {read=_scw_Get_Gender, write=Set_Gender};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_GovernmentIDNumber() { WideString r; HRESULT hr = Get_GovernmentIDNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString GovernmentIDNumber = {read=_scw_Get_GovernmentIDNumber, write=Set_GovernmentIDNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Hobby() { WideString r; HRESULT hr = Get_Hobby(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Hobby = {read=_scw_Get_Hobby, write=Set_Hobby};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Home2TelephoneNumber() { WideString r; HRESULT hr = Get_Home2TelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Home2TelephoneNumber = {read=_scw_Get_Home2TelephoneNumber, write=Set_Home2TelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HomeAddress() { WideString r; HRESULT hr = Get_HomeAddress(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HomeAddress = {read=_scw_Get_HomeAddress, write=Set_HomeAddress};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HomeAddressCity() { WideString r; HRESULT hr = Get_HomeAddressCity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HomeAddressCity = {read=_scw_Get_HomeAddressCity, write=Set_HomeAddressCity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HomeAddressCountry() { WideString r; HRESULT hr = Get_HomeAddressCountry(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HomeAddressCountry = {read=_scw_Get_HomeAddressCountry, write=Set_HomeAddressCountry};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HomeAddressPostalCode() { WideString r; HRESULT hr = Get_HomeAddressPostalCode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HomeAddressPostalCode = {read=_scw_Get_HomeAddressPostalCode, write=Set_HomeAddressPostalCode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HomeAddressPostOfficeBox() { WideString r; HRESULT hr = Get_HomeAddressPostOfficeBox(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HomeAddressPostOfficeBox = {read=_scw_Get_HomeAddressPostOfficeBox, write=Set_HomeAddressPostOfficeBox};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HomeAddressState() { WideString r; HRESULT hr = Get_HomeAddressState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HomeAddressState = {read=_scw_Get_HomeAddressState, write=Set_HomeAddressState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HomeAddressStreet() { WideString r; HRESULT hr = Get_HomeAddressStreet(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HomeAddressStreet = {read=_scw_Get_HomeAddressStreet, write=Set_HomeAddressStreet};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HomeFaxNumber() { WideString r; HRESULT hr = Get_HomeFaxNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HomeFaxNumber = {read=_scw_Get_HomeFaxNumber, write=Set_HomeFaxNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HomeTelephoneNumber() { WideString r; HRESULT hr = Get_HomeTelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HomeTelephoneNumber = {read=_scw_Get_HomeTelephoneNumber, write=Set_HomeTelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Initials() { WideString r; HRESULT hr = Get_Initials(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Initials = {read=_scw_Get_Initials, write=Set_Initials};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_InternetFreeBusyAddress() { WideString r; HRESULT hr = Get_InternetFreeBusyAddress(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString InternetFreeBusyAddress = {read=_scw_Get_InternetFreeBusyAddress, write=Set_InternetFreeBusyAddress};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ISDNNumber() { WideString r; HRESULT hr = Get_ISDNNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ISDNNumber = {read=_scw_Get_ISDNNumber, write=Set_ISDNNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_JobTitle() { WideString r; HRESULT hr = Get_JobTitle(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString JobTitle = {read=_scw_Get_JobTitle, write=Set_JobTitle};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Journal() { Word r; HRESULT hr = Get_Journal(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Journal = {read=_scw_Get_Journal, write=Set_Journal};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Language() { WideString r; HRESULT hr = Get_Language(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Language = {read=_scw_Get_Language, write=Set_Language};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_LastFirstAndSuffix() { WideString r; HRESULT hr = Get_LastFirstAndSuffix(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString LastFirstAndSuffix = {read=_scw_Get_LastFirstAndSuffix};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_LastFirstNoSpace() { WideString r; HRESULT hr = Get_LastFirstNoSpace(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString LastFirstNoSpace = {read=_scw_Get_LastFirstNoSpace};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_LastFirstNoSpaceCompany() { WideString r; HRESULT hr = Get_LastFirstNoSpaceCompany(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString LastFirstNoSpaceCompany = {read=_scw_Get_LastFirstNoSpaceCompany};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_LastFirstSpaceOnly() { WideString r; HRESULT hr = Get_LastFirstSpaceOnly(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString LastFirstSpaceOnly = {read=_scw_Get_LastFirstSpaceOnly};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_LastFirstSpaceOnlyCompany() { WideString r; HRESULT hr = Get_LastFirstSpaceOnlyCompany(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString LastFirstSpaceOnlyCompany = {read=_scw_Get_LastFirstSpaceOnlyCompany};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_LastName() { WideString r; HRESULT hr = Get_LastName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString LastName = {read=_scw_Get_LastName, write=Set_LastName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_LastNameAndFirstName() { WideString r; HRESULT hr = Get_LastNameAndFirstName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString LastNameAndFirstName = {read=_scw_Get_LastNameAndFirstName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MailingAddress() { WideString r; HRESULT hr = Get_MailingAddress(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MailingAddress = {read=_scw_Get_MailingAddress, write=Set_MailingAddress};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MailingAddressCity() { WideString r; HRESULT hr = Get_MailingAddressCity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MailingAddressCity = {read=_scw_Get_MailingAddressCity, write=Set_MailingAddressCity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MailingAddressCountry() { WideString r; HRESULT hr = Get_MailingAddressCountry(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MailingAddressCountry = {read=_scw_Get_MailingAddressCountry, write=Set_MailingAddressCountry};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MailingAddressPostalCode() { WideString r; HRESULT hr = Get_MailingAddressPostalCode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MailingAddressPostalCode = {read=_scw_Get_MailingAddressPostalCode, write=Set_MailingAddressPostalCode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MailingAddressPostOfficeBox() { WideString r; HRESULT hr = Get_MailingAddressPostOfficeBox(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MailingAddressPostOfficeBox = {read=_scw_Get_MailingAddressPostOfficeBox, write=Set_MailingAddressPostOfficeBox};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MailingAddressState() { WideString r; HRESULT hr = Get_MailingAddressState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MailingAddressState = {read=_scw_Get_MailingAddressState, write=Set_MailingAddressState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MailingAddressStreet() { WideString r; HRESULT hr = Get_MailingAddressStreet(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MailingAddressStreet = {read=_scw_Get_MailingAddressStreet, write=Set_MailingAddressStreet};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ManagerName() { WideString r; HRESULT hr = Get_ManagerName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ManagerName = {read=_scw_Get_ManagerName, write=Set_ManagerName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MiddleName() { WideString r; HRESULT hr = Get_MiddleName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MiddleName = {read=_scw_Get_MiddleName, write=Set_MiddleName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MobileTelephoneNumber() { WideString r; HRESULT hr = Get_MobileTelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MobileTelephoneNumber = {read=_scw_Get_MobileTelephoneNumber, write=Set_MobileTelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_NetMeetingAlias() { WideString r; HRESULT hr = Get_NetMeetingAlias(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString NetMeetingAlias = {read=_scw_Get_NetMeetingAlias, write=Set_NetMeetingAlias};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_NetMeetingServer() { WideString r; HRESULT hr = Get_NetMeetingServer(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString NetMeetingServer = {read=_scw_Get_NetMeetingServer, write=Set_NetMeetingServer};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_NickName() { WideString r; HRESULT hr = Get_NickName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString NickName = {read=_scw_Get_NickName, write=Set_NickName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OfficeLocation() { WideString r; HRESULT hr = Get_OfficeLocation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OfficeLocation = {read=_scw_Get_OfficeLocation, write=Set_OfficeLocation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OrganizationalIDNumber() { WideString r; HRESULT hr = Get_OrganizationalIDNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OrganizationalIDNumber = {read=_scw_Get_OrganizationalIDNumber, write=Set_OrganizationalIDNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OtherAddress() { WideString r; HRESULT hr = Get_OtherAddress(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OtherAddress = {read=_scw_Get_OtherAddress, write=Set_OtherAddress};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OtherAddressCity() { WideString r; HRESULT hr = Get_OtherAddressCity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OtherAddressCity = {read=_scw_Get_OtherAddressCity, write=Set_OtherAddressCity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OtherAddressCountry() { WideString r; HRESULT hr = Get_OtherAddressCountry(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OtherAddressCountry = {read=_scw_Get_OtherAddressCountry, write=Set_OtherAddressCountry};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OtherAddressPostalCode() { WideString r; HRESULT hr = Get_OtherAddressPostalCode(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OtherAddressPostalCode = {read=_scw_Get_OtherAddressPostalCode, write=Set_OtherAddressPostalCode};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OtherAddressPostOfficeBox() { WideString r; HRESULT hr = Get_OtherAddressPostOfficeBox(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OtherAddressPostOfficeBox = {read=_scw_Get_OtherAddressPostOfficeBox, write=Set_OtherAddressPostOfficeBox};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OtherAddressState() { WideString r; HRESULT hr = Get_OtherAddressState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OtherAddressState = {read=_scw_Get_OtherAddressState, write=Set_OtherAddressState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OtherAddressStreet() { WideString r; HRESULT hr = Get_OtherAddressStreet(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OtherAddressStreet = {read=_scw_Get_OtherAddressStreet, write=Set_OtherAddressStreet};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OtherFaxNumber() { WideString r; HRESULT hr = Get_OtherFaxNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OtherFaxNumber = {read=_scw_Get_OtherFaxNumber, write=Set_OtherFaxNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OtherTelephoneNumber() { WideString r; HRESULT hr = Get_OtherTelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OtherTelephoneNumber = {read=_scw_Get_OtherTelephoneNumber, write=Set_OtherTelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_PagerNumber() { WideString r; HRESULT hr = Get_PagerNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString PagerNumber = {read=_scw_Get_PagerNumber, write=Set_PagerNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_PersonalHomePage() { WideString r; HRESULT hr = Get_PersonalHomePage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString PersonalHomePage = {read=_scw_Get_PersonalHomePage, write=Set_PersonalHomePage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_PrimaryTelephoneNumber() { WideString r; HRESULT hr = Get_PrimaryTelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString PrimaryTelephoneNumber = {read=_scw_Get_PrimaryTelephoneNumber, write=Set_PrimaryTelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Profession() { WideString r; HRESULT hr = Get_Profession(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Profession = {read=_scw_Get_Profession, write=Set_Profession};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_RadioTelephoneNumber() { WideString r; HRESULT hr = Get_RadioTelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString RadioTelephoneNumber = {read=_scw_Get_RadioTelephoneNumber, write=Set_RadioTelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ReferredBy() { WideString r; HRESULT hr = Get_ReferredBy(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ReferredBy = {read=_scw_Get_ReferredBy, write=Set_ReferredBy};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_SelectedMailingAddress() { Activex::TOleEnum r; HRESULT hr = Get_SelectedMailingAddress(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum SelectedMailingAddress = {read=_scw_Get_SelectedMailingAddress, write=Set_SelectedMailingAddress};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Spouse() { WideString r; HRESULT hr = Get_Spouse(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Spouse = {read=_scw_Get_Spouse, write=Set_Spouse};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Suffix() { WideString r; HRESULT hr = Get_Suffix(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Suffix = {read=_scw_Get_Suffix, write=Set_Suffix};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_TelexNumber() { WideString r; HRESULT hr = Get_TelexNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString TelexNumber = {read=_scw_Get_TelexNumber, write=Set_TelexNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Title() { WideString r; HRESULT hr = Get_Title(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Title = {read=_scw_Get_Title, write=Set_Title};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_TTYTDDTelephoneNumber() { WideString r; HRESULT hr = Get_TTYTDDTelephoneNumber(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString TTYTDDTelephoneNumber = {read=_scw_Get_TTYTDDTelephoneNumber, write=Set_TTYTDDTelephoneNumber};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_User1() { WideString r; HRESULT hr = Get_User1(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString User1 = {read=_scw_Get_User1, write=Set_User1};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_User2() { WideString r; HRESULT hr = Get_User2(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString User2 = {read=_scw_Get_User2, write=Set_User2};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_User3() { WideString r; HRESULT hr = Get_User3(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString User3 = {read=_scw_Get_User3, write=Set_User3};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_User4() { WideString r; HRESULT hr = Get_User4(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString User4 = {read=_scw_Get_User4, write=Set_User4};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_UserCertificate() { WideString r; HRESULT hr = Get_UserCertificate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString UserCertificate = {read=_scw_Get_UserCertificate, write=Set_UserCertificate};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_WebPage() { WideString r; HRESULT hr = Get_WebPage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString WebPage = {read=_scw_Get_WebPage, write=Set_WebPage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_YomiCompanyName() { WideString r; HRESULT hr = Get_YomiCompanyName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString YomiCompanyName = {read=_scw_Get_YomiCompanyName, write=Set_YomiCompanyName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_YomiFirstName() { WideString r; HRESULT hr = Get_YomiFirstName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString YomiFirstName = {read=_scw_Get_YomiFirstName, write=Set_YomiFirstName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_YomiLastName() { WideString r; HRESULT hr = Get_YomiLastName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString YomiLastName = {read=_scw_Get_YomiLastName, write=Set_YomiLastName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_LastFirstNoSpaceAndSuffix() { WideString r; HRESULT hr = Get_LastFirstNoSpaceAndSuffix(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString LastFirstNoSpaceAndSuffix = {read=_scw_Get_LastFirstNoSpaceAndSuffix};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_IMAddress() { WideString r; HRESULT hr = Get_IMAddress(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString IMAddress = {read=_scw_Get_IMAddress, write=Set_IMAddress};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _ContactItemDisp;
typedef System::DelphiInterface<_ContactItemDisp> _di__ContactItemDisp;
__dispinterface INTERFACE_UUID("{00063021-0000-0000-C000-000000000046}") _ContactItemDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063081-0000-0000-C000-000000000046}") _DistListItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall Get_DLName(WideString &Get_DLName_result) = 0 ;
	virtual HRESULT __safecall Set_DLName(const WideString DLName) = 0 ;
	virtual HRESULT __safecall Get_MemberCount(int &Get_MemberCount_result) = 0 ;
	virtual HRESULT __safecall Get_CheckSum(int &Get_CheckSum_result) = 0 ;
	virtual HRESULT __safecall Get_Members(OleVariant &Get_Members_result) = 0 ;
	virtual HRESULT __safecall Set_Members(const OleVariant Members) = 0 ;
	virtual HRESULT __safecall Get_OneOffMembers(OleVariant &Get_OneOffMembers_result) = 0 ;
	virtual HRESULT __safecall Set_OneOffMembers(const OleVariant OneOffMembers) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall AddMembers(const _di_Recipients Recipients) = 0 ;
	virtual HRESULT __safecall RemoveMembers(const _di_Recipients Recipients) = 0 ;
	virtual HRESULT __safecall GetMember(int Index, _di_Recipient &GetMember_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall AddMember(const _di_Recipient Recipient) = 0 ;
	virtual HRESULT __safecall RemoveMember(const _di_Recipient Recipient) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_DLName() { WideString r; HRESULT hr = Get_DLName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString DLName = {read=_scw_Get_DLName, write=Set_DLName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_MemberCount() { int r; HRESULT hr = Get_MemberCount(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int MemberCount = {read=_scw_Get_MemberCount};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_CheckSum() { int r; HRESULT hr = Get_CheckSum(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int CheckSum = {read=_scw_Get_CheckSum};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_Members() { OleVariant r; HRESULT hr = Get_Members(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant Members = {read=_scw_Get_Members, write=Set_Members};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_OneOffMembers() { OleVariant r; HRESULT hr = Get_OneOffMembers(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant OneOffMembers = {read=_scw_Get_OneOffMembers, write=Set_OneOffMembers};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _DistListItemDisp;
typedef System::DelphiInterface<_DistListItemDisp> _di__DistListItemDisp;
__dispinterface INTERFACE_UUID("{00063081-0000-0000-C000-000000000046}") _DistListItemDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063020-0000-0000-C000-000000000046}") _DocumentItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _DocumentItemDisp;
typedef System::DelphiInterface<_DocumentItemDisp> _di__DocumentItemDisp;
__dispinterface INTERFACE_UUID("{00063020-0000-0000-C000-000000000046}") _DocumentItemDisp  : public IDispatch 
{
	
};

__dispinterface ExplorersEvents;
typedef System::DelphiInterface<ExplorersEvents> _di_ExplorersEvents;
__dispinterface INTERFACE_UUID("{00063078-0000-0000-C000-000000000046}") ExplorersEvents  : public IDispatch 
{
	
};

__dispinterface FoldersEvents;
typedef System::DelphiInterface<FoldersEvents> _di_FoldersEvents;
__dispinterface INTERFACE_UUID("{00063076-0000-0000-C000-000000000046}") FoldersEvents  : public IDispatch 
{
	
};

__dispinterface InspectorsEvents;
typedef System::DelphiInterface<InspectorsEvents> _di_InspectorsEvents;
__dispinterface INTERFACE_UUID("{00063079-0000-0000-C000-000000000046}") InspectorsEvents  : public IDispatch 
{
	
};

__dispinterface ItemsEvents;
typedef System::DelphiInterface<ItemsEvents> _di_ItemsEvents;
__dispinterface INTERFACE_UUID("{00063077-0000-0000-C000-000000000046}") ItemsEvents  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063022-0000-0000-C000-000000000046}") _JournalItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall Get_ContactNames(WideString &Get_ContactNames_result) = 0 ;
	virtual HRESULT __safecall Set_ContactNames(const WideString ContactNames) = 0 ;
	virtual HRESULT __safecall Get_DocPosted(Word &Get_DocPosted_result) = 0 ;
	virtual HRESULT __safecall Set_DocPosted(Word DocPosted) = 0 ;
	virtual HRESULT __safecall Get_DocPrinted(Word &Get_DocPrinted_result) = 0 ;
	virtual HRESULT __safecall Set_DocPrinted(Word DocPrinted) = 0 ;
	virtual HRESULT __safecall Get_DocRouted(Word &Get_DocRouted_result) = 0 ;
	virtual HRESULT __safecall Set_DocRouted(Word DocRouted) = 0 ;
	virtual HRESULT __safecall Get_DocSaved(Word &Get_DocSaved_result) = 0 ;
	virtual HRESULT __safecall Set_DocSaved(Word DocSaved) = 0 ;
	virtual HRESULT __safecall Get_Duration(int &Get_Duration_result) = 0 ;
	virtual HRESULT __safecall Set_Duration(int Duration) = 0 ;
	virtual HRESULT __safecall Get_End_(System::TDateTime &Get_End__result) = 0 ;
	virtual HRESULT __safecall Set_End_(System::TDateTime End_) = 0 ;
	virtual HRESULT __safecall Get_Type_(WideString &Get_Type__result) = 0 ;
	virtual HRESULT __safecall Set_Type_(const WideString Type_) = 0 ;
	virtual HRESULT __safecall Get_Recipients(_di_Recipients &Get_Recipients_result) = 0 ;
	virtual HRESULT __safecall Get_Start(System::TDateTime &Get_Start_result) = 0 ;
	virtual HRESULT __safecall Set_Start(System::TDateTime Start) = 0 ;
	virtual HRESULT __safecall Forward(_di__MailItem &Forward_result) = 0 ;
	virtual HRESULT __safecall Reply(_di__MailItem &Reply_result) = 0 ;
	virtual HRESULT __safecall ReplyAll(_di__MailItem &ReplyAll_result) = 0 ;
	virtual HRESULT __safecall StartTimer(void) = 0 ;
	virtual HRESULT __safecall StopTimer(void) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ContactNames() { WideString r; HRESULT hr = Get_ContactNames(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ContactNames = {read=_scw_Get_ContactNames, write=Set_ContactNames};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DocPosted() { Word r; HRESULT hr = Get_DocPosted(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DocPosted = {read=_scw_Get_DocPosted, write=Set_DocPosted};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DocPrinted() { Word r; HRESULT hr = Get_DocPrinted(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DocPrinted = {read=_scw_Get_DocPrinted, write=Set_DocPrinted};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DocRouted() { Word r; HRESULT hr = Get_DocRouted(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DocRouted = {read=_scw_Get_DocRouted, write=Set_DocRouted};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_DocSaved() { Word r; HRESULT hr = Get_DocSaved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word DocSaved = {read=_scw_Get_DocSaved, write=Set_DocSaved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Duration() { int r; HRESULT hr = Get_Duration(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Duration = {read=_scw_Get_Duration, write=Set_Duration};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_End_() { System::TDateTime r; HRESULT hr = Get_End_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime End_ = {read=_scw_Get_End_, write=Set_End_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Type_() { WideString r; HRESULT hr = Get_Type_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Type_ = {read=_scw_Get_Type_, write=Set_Type_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Recipients _scw_Get_Recipients() { _di_Recipients r; HRESULT hr = Get_Recipients(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Recipients Recipients = {read=_scw_Get_Recipients};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_Start() { System::TDateTime r; HRESULT hr = Get_Start(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime Start = {read=_scw_Get_Start, write=Set_Start};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _JournalItemDisp;
typedef System::DelphiInterface<_JournalItemDisp> _di__JournalItemDisp;
__dispinterface INTERFACE_UUID("{00063022-0000-0000-C000-000000000046}") _JournalItemDisp  : public IDispatch 
{
	
};

__dispinterface NameSpaceEvents;
typedef System::DelphiInterface<NameSpaceEvents> _di_NameSpaceEvents;
__dispinterface INTERFACE_UUID("{0006308C-0000-0000-C000-000000000046}") NameSpaceEvents  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063025-0000-0000-C000-000000000046}") _NoteItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Color(Activex::TOleEnum &Get_Color_result) = 0 ;
	virtual HRESULT __safecall Set_Color(Activex::TOleEnum Color) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Height(int &Get_Height_result) = 0 ;
	virtual HRESULT __safecall Set_Height(int Height) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_Left(int &Get_Left_result) = 0 ;
	virtual HRESULT __safecall Set_Left(int Left) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Get_Top(int &Get_Top_result) = 0 ;
	virtual HRESULT __safecall Set_Top(int Top) = 0 ;
	virtual HRESULT __safecall Get_Width(int &Get_Width_result) = 0 ;
	virtual HRESULT __safecall Set_Width(int Width) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Color() { Activex::TOleEnum r; HRESULT hr = Get_Color(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Color = {read=_scw_Get_Color, write=Set_Color};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Height() { int r; HRESULT hr = Get_Height(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Height = {read=_scw_Get_Height, write=Set_Height};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Left() { int r; HRESULT hr = Get_Left(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Left = {read=_scw_Get_Left, write=Set_Left};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Top() { int r; HRESULT hr = Get_Top(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Top = {read=_scw_Get_Top, write=Set_Top};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Width() { int r; HRESULT hr = Get_Width(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Width = {read=_scw_Get_Width, write=Set_Width};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _NoteItemDisp;
typedef System::DelphiInterface<_NoteItemDisp> _di__NoteItemDisp;
__dispinterface INTERFACE_UUID("{00063025-0000-0000-C000-000000000046}") _NoteItemDisp  : public IDispatch 
{
	
};

__interface OutlookBarGroup;
typedef System::DelphiInterface<OutlookBarGroup> _di_OutlookBarGroup;
__interface INTERFACE_UUID("{00063073-0000-0000-C000-000000000046}") OutlookBarGroup  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString Name) = 0 ;
	virtual HRESULT __safecall Get_Shortcuts(_di__OutlookBarShortcuts &Get_Shortcuts_result) = 0 ;
	virtual HRESULT __safecall Get_ViewType(Activex::TOleEnum &Get_ViewType_result) = 0 ;
	virtual HRESULT __safecall Set_ViewType(Activex::TOleEnum ViewType) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__OutlookBarShortcuts _scw_Get_Shortcuts() { _di__OutlookBarShortcuts r; HRESULT hr = Get_Shortcuts(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__OutlookBarShortcuts Shortcuts = {read=_scw_Get_Shortcuts};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_ViewType() { Activex::TOleEnum r; HRESULT hr = Get_ViewType(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum ViewType = {read=_scw_Get_ViewType, write=Set_ViewType};
};

__dispinterface OutlookBarGroupDisp;
typedef System::DelphiInterface<OutlookBarGroupDisp> _di_OutlookBarGroupDisp;
__dispinterface INTERFACE_UUID("{00063073-0000-0000-C000-000000000046}") OutlookBarGroupDisp  : public IDispatch 
{
	
};

__interface OutlookBarShortcut;
typedef System::DelphiInterface<OutlookBarShortcut> _di_OutlookBarShortcut;
__interface INTERFACE_UUID("{00063074-0000-0000-C000-000000000046}") _OutlookBarShortcuts  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_OutlookBarShortcut &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const OleVariant Target, const WideString Name, const OleVariant Index, _di_OutlookBarShortcut &Add_result) = 0 ;
	virtual HRESULT __safecall Remove(const OleVariant Index) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface _OutlookBarShortcutsDisp;
typedef System::DelphiInterface<_OutlookBarShortcutsDisp> _di__OutlookBarShortcutsDisp;
__dispinterface INTERFACE_UUID("{00063074-0000-0000-C000-000000000046}") _OutlookBarShortcutsDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063075-0000-0000-C000-000000000046}") OutlookBarShortcut  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Set_Name(const WideString Name) = 0 ;
	virtual HRESULT __safecall Get_Target(OleVariant &Get_Target_result) = 0 ;
	virtual HRESULT __safecall SetIcon(const OleVariant Icon) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name, write=Set_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline OleVariant _scw_Get_Target() { OleVariant r; HRESULT hr = Get_Target(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property OleVariant Target = {read=_scw_Get_Target};
};

__dispinterface OutlookBarShortcutDisp;
typedef System::DelphiInterface<OutlookBarShortcutDisp> _di_OutlookBarShortcutDisp;
__dispinterface INTERFACE_UUID("{00063075-0000-0000-C000-000000000046}") OutlookBarShortcutDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063072-0000-0000-C000-000000000046}") _OutlookBarGroups  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_OutlookBarGroup &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const WideString Name, const OleVariant Index, _di_OutlookBarGroup &Add_result) = 0 ;
	virtual HRESULT __safecall Remove(const OleVariant Index) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface _OutlookBarGroupsDisp;
typedef System::DelphiInterface<_OutlookBarGroupsDisp> _di__OutlookBarGroupsDisp;
__dispinterface INTERFACE_UUID("{00063072-0000-0000-C000-000000000046}") _OutlookBarGroupsDisp  : public IDispatch 
{
	
};

__dispinterface OutlookBarGroupsEvents;
typedef System::DelphiInterface<OutlookBarGroupsEvents> _di_OutlookBarGroupsEvents;
__dispinterface INTERFACE_UUID("{0006307B-0000-0000-C000-000000000046}") OutlookBarGroupsEvents  : public IDispatch 
{
	
};

__interface OutlookBarStorage;
typedef System::DelphiInterface<OutlookBarStorage> _di_OutlookBarStorage;
__interface INTERFACE_UUID("{00063070-0000-0000-C000-000000000046}") _OutlookBarPane  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Contents(_di_OutlookBarStorage &Get_Contents_result) = 0 ;
	virtual HRESULT __safecall Get_CurrentGroup(_di_OutlookBarGroup &Get_CurrentGroup_result) = 0 ;
	virtual HRESULT __safecall Set_CurrentGroup(const _di_OutlookBarGroup CurrentGroup) = 0 ;
	virtual HRESULT __safecall Get_Name(WideString &Get_Name_result) = 0 ;
	virtual HRESULT __safecall Get_Visible(Word &Get_Visible_result) = 0 ;
	virtual HRESULT __safecall Set_Visible(Word Visible) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_OutlookBarStorage _scw_Get_Contents() { _di_OutlookBarStorage r; HRESULT hr = Get_Contents(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_OutlookBarStorage Contents = {read=_scw_Get_Contents};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_OutlookBarGroup _scw_Get_CurrentGroup() { _di_OutlookBarGroup r; HRESULT hr = Get_CurrentGroup(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_OutlookBarGroup CurrentGroup = {read=_scw_Get_CurrentGroup, write=Set_CurrentGroup};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Name() { WideString r; HRESULT hr = Get_Name(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Name = {read=_scw_Get_Name};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Visible() { Word r; HRESULT hr = Get_Visible(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Visible = {read=_scw_Get_Visible, write=Set_Visible};
};

__dispinterface _OutlookBarPaneDisp;
typedef System::DelphiInterface<_OutlookBarPaneDisp> _di__OutlookBarPaneDisp;
__dispinterface INTERFACE_UUID("{00063070-0000-0000-C000-000000000046}") _OutlookBarPaneDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063071-0000-0000-C000-000000000046}") OutlookBarStorage  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Groups(_di__OutlookBarGroups &Get_Groups_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__OutlookBarGroups _scw_Get_Groups() { _di__OutlookBarGroups r; HRESULT hr = Get_Groups(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__OutlookBarGroups Groups = {read=_scw_Get_Groups};
};

__dispinterface OutlookBarStorageDisp;
typedef System::DelphiInterface<OutlookBarStorageDisp> _di_OutlookBarStorageDisp;
__dispinterface INTERFACE_UUID("{00063071-0000-0000-C000-000000000046}") OutlookBarStorageDisp  : public IDispatch 
{
	
};

__dispinterface OutlookBarPaneEvents;
typedef System::DelphiInterface<OutlookBarPaneEvents> _di_OutlookBarPaneEvents;
__dispinterface INTERFACE_UUID("{0006307A-0000-0000-C000-000000000046}") OutlookBarPaneEvents  : public IDispatch 
{
	
};

__dispinterface OutlookBarShortcutsEvents;
typedef System::DelphiInterface<OutlookBarShortcutsEvents> _di_OutlookBarShortcutsEvents;
__dispinterface INTERFACE_UUID("{0006307C-0000-0000-C000-000000000046}") OutlookBarShortcutsEvents  : public IDispatch 
{
	
};

__interface PropertyPage;
typedef System::DelphiInterface<PropertyPage> _di_PropertyPage;
__interface INTERFACE_UUID("{0006307E-0000-0000-C000-000000000046}") PropertyPage  : public IDispatch 
{
	
public:
	virtual HRESULT __stdcall GetPageInfo(WideString &HelpFile, int &HelpContext) = 0 ;
	virtual HRESULT __stdcall Get_Dirty(/* out */ Word &Dirty) = 0 ;
	virtual HRESULT __stdcall Apply(void) = 0 ;
};

__interface PropertyPageSite;
typedef System::DelphiInterface<PropertyPageSite> _di_PropertyPageSite;
__interface INTERFACE_UUID("{0006307F-0000-0000-C000-000000000046}") PropertyPageSite  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall OnStatusChange(void) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
};

__dispinterface PropertyPageSiteDisp;
typedef System::DelphiInterface<PropertyPageSiteDisp> _di_PropertyPageSiteDisp;
__dispinterface INTERFACE_UUID("{0006307F-0000-0000-C000-000000000046}") PropertyPageSiteDisp  : public IDispatch 
{
	
};

__interface Pages;
typedef System::DelphiInterface<Pages> _di_Pages;
__interface INTERFACE_UUID("{0006303F-0000-0000-C000-000000000046}") Pages  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Count(int &Get_Count_result) = 0 ;
	virtual HRESULT __safecall Item(const OleVariant Index, _di_IDispatch &Item_result) = 0 ;
	virtual HRESULT __safecall Add(const OleVariant Name, _di_IDispatch &Add_result) = 0 ;
	virtual HRESULT __safecall Remove(int Index) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Count() { int r; HRESULT hr = Get_Count(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Count = {read=_scw_Get_Count};
};

__dispinterface PagesDisp;
typedef System::DelphiInterface<PagesDisp> _di_PagesDisp;
__dispinterface INTERFACE_UUID("{0006303F-0000-0000-C000-000000000046}") PagesDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063024-0000-0000-C000-000000000046}") _PostItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall Get_ExpiryTime(System::TDateTime &Get_ExpiryTime_result) = 0 ;
	virtual HRESULT __safecall Set_ExpiryTime(System::TDateTime ExpiryTime) = 0 ;
	virtual HRESULT __safecall Get_HTMLBody(WideString &Get_HTMLBody_result) = 0 ;
	virtual HRESULT __safecall Set_HTMLBody(const WideString HTMLBody) = 0 ;
	virtual HRESULT __safecall Get_ReceivedTime(System::TDateTime &Get_ReceivedTime_result) = 0 ;
	virtual HRESULT __safecall Get_SenderName(WideString &Get_SenderName_result) = 0 ;
	virtual HRESULT __safecall Get_SentOn(System::TDateTime &Get_SentOn_result) = 0 ;
	virtual HRESULT __safecall ClearConversationIndex(void) = 0 ;
	virtual HRESULT __safecall Forward(_di__MailItem &Forward_result) = 0 ;
	virtual HRESULT __safecall Post(void) = 0 ;
	virtual HRESULT __safecall Reply(_di__MailItem &Reply_result) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_BodyFormat(Activex::TOleEnum &Get_BodyFormat_result) = 0 ;
	virtual HRESULT __safecall Set_BodyFormat(Activex::TOleEnum BodyFormat) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_InternetCodepage(int &Get_InternetCodepage_result) = 0 ;
	virtual HRESULT __safecall Set_InternetCodepage(int InternetCodepage) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall SetACLs(Word &SetACLs_result) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_ExpiryTime() { System::TDateTime r; HRESULT hr = Get_ExpiryTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime ExpiryTime = {read=_scw_Get_ExpiryTime, write=Set_ExpiryTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_HTMLBody() { WideString r; HRESULT hr = Get_HTMLBody(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString HTMLBody = {read=_scw_Get_HTMLBody, write=Set_HTMLBody};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_ReceivedTime() { System::TDateTime r; HRESULT hr = Get_ReceivedTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime ReceivedTime = {read=_scw_Get_ReceivedTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_SenderName() { WideString r; HRESULT hr = Get_SenderName(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString SenderName = {read=_scw_Get_SenderName};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_SentOn() { System::TDateTime r; HRESULT hr = Get_SentOn(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime SentOn = {read=_scw_Get_SentOn};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_BodyFormat() { Activex::TOleEnum r; HRESULT hr = Get_BodyFormat(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum BodyFormat = {read=_scw_Get_BodyFormat, write=Set_BodyFormat};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_InternetCodepage() { int r; HRESULT hr = Get_InternetCodepage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int InternetCodepage = {read=_scw_Get_InternetCodepage, write=Set_InternetCodepage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _PostItemDisp;
typedef System::DelphiInterface<_PostItemDisp> _di__PostItemDisp;
__dispinterface INTERFACE_UUID("{00063024-0000-0000-C000-000000000046}") _PostItemDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063023-0000-0000-C000-000000000046}") _RemoteItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall Get_HasAttachment(Word &Get_HasAttachment_result) = 0 ;
	virtual HRESULT __safecall Get_RemoteMessageClass(WideString &Get_RemoteMessageClass_result) = 0 ;
	virtual HRESULT __safecall Get_TransferSize(int &Get_TransferSize_result) = 0 ;
	virtual HRESULT __safecall Get_TransferTime(int &Get_TransferTime_result) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_HasAttachment() { Word r; HRESULT hr = Get_HasAttachment(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word HasAttachment = {read=_scw_Get_HasAttachment};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_RemoteMessageClass() { WideString r; HRESULT hr = Get_RemoteMessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString RemoteMessageClass = {read=_scw_Get_RemoteMessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_TransferSize() { int r; HRESULT hr = Get_TransferSize(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int TransferSize = {read=_scw_Get_TransferSize};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_TransferTime() { int r; HRESULT hr = Get_TransferTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int TransferTime = {read=_scw_Get_TransferTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _RemoteItemDisp;
typedef System::DelphiInterface<_RemoteItemDisp> _di__RemoteItemDisp;
__dispinterface INTERFACE_UUID("{00063023-0000-0000-C000-000000000046}") _RemoteItemDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063026-0000-0000-C000-000000000046}") _ReportItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _ReportItemDisp;
typedef System::DelphiInterface<_ReportItemDisp> _di__ReportItemDisp;
__dispinterface INTERFACE_UUID("{00063026-0000-0000-C000-000000000046}") _ReportItemDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063035-0000-0000-C000-000000000046}") _TaskItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall Get_ActualWork(int &Get_ActualWork_result) = 0 ;
	virtual HRESULT __safecall Set_ActualWork(int ActualWork) = 0 ;
	virtual HRESULT __safecall Get_CardData(WideString &Get_CardData_result) = 0 ;
	virtual HRESULT __safecall Set_CardData(const WideString CardData) = 0 ;
	virtual HRESULT __safecall Get_Complete(Word &Get_Complete_result) = 0 ;
	virtual HRESULT __safecall Set_Complete(Word Complete) = 0 ;
	virtual HRESULT __safecall Get_Contacts(WideString &Get_Contacts_result) = 0 ;
	virtual HRESULT __safecall Set_Contacts(const WideString Contacts) = 0 ;
	virtual HRESULT __safecall Get_ContactNames(WideString &Get_ContactNames_result) = 0 ;
	virtual HRESULT __safecall Set_ContactNames(const WideString ContactNames) = 0 ;
	virtual HRESULT __safecall Get_DateCompleted(System::TDateTime &Get_DateCompleted_result) = 0 ;
	virtual HRESULT __safecall Set_DateCompleted(System::TDateTime DateCompleted) = 0 ;
	virtual HRESULT __safecall Get_DelegationState(Activex::TOleEnum &Get_DelegationState_result) = 0 ;
	virtual HRESULT __safecall Get_Delegator(WideString &Get_Delegator_result) = 0 ;
	virtual HRESULT __safecall Get_DueDate(System::TDateTime &Get_DueDate_result) = 0 ;
	virtual HRESULT __safecall Set_DueDate(System::TDateTime DueDate) = 0 ;
	virtual HRESULT __safecall Get_IsRecurring(Word &Get_IsRecurring_result) = 0 ;
	virtual HRESULT __safecall Get_Ordinal(int &Get_Ordinal_result) = 0 ;
	virtual HRESULT __safecall Set_Ordinal(int Ordinal) = 0 ;
	virtual HRESULT __safecall Get_Owner(WideString &Get_Owner_result) = 0 ;
	virtual HRESULT __safecall Set_Owner(const WideString Owner) = 0 ;
	virtual HRESULT __safecall Get_Ownership(Activex::TOleEnum &Get_Ownership_result) = 0 ;
	virtual HRESULT __safecall Get_PercentComplete(int &Get_PercentComplete_result) = 0 ;
	virtual HRESULT __safecall Set_PercentComplete(int PercentComplete) = 0 ;
	virtual HRESULT __safecall Get_Recipients(_di_Recipients &Get_Recipients_result) = 0 ;
	virtual HRESULT __safecall Get_ReminderTime(System::TDateTime &Get_ReminderTime_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderTime(System::TDateTime ReminderTime) = 0 ;
	virtual HRESULT __safecall Get_ReminderOverrideDefault(Word &Get_ReminderOverrideDefault_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderOverrideDefault(Word ReminderOverrideDefault) = 0 ;
	virtual HRESULT __safecall Get_ReminderPlaySound(Word &Get_ReminderPlaySound_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderPlaySound(Word ReminderPlaySound) = 0 ;
	virtual HRESULT __safecall Get_ReminderSet(Word &Get_ReminderSet_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderSet(Word ReminderSet) = 0 ;
	virtual HRESULT __safecall Get_ReminderSoundFile(WideString &Get_ReminderSoundFile_result) = 0 ;
	virtual HRESULT __safecall Set_ReminderSoundFile(const WideString ReminderSoundFile) = 0 ;
	virtual HRESULT __safecall Get_ResponseState(Activex::TOleEnum &Get_ResponseState_result) = 0 ;
	virtual HRESULT __safecall Get_Role(WideString &Get_Role_result) = 0 ;
	virtual HRESULT __safecall Set_Role(const WideString Role) = 0 ;
	virtual HRESULT __safecall Get_SchedulePlusPriority(WideString &Get_SchedulePlusPriority_result) = 0 ;
	virtual HRESULT __safecall Set_SchedulePlusPriority(const WideString SchedulePlusPriority) = 0 ;
	virtual HRESULT __safecall Get_StartDate(System::TDateTime &Get_StartDate_result) = 0 ;
	virtual HRESULT __safecall Set_StartDate(System::TDateTime StartDate) = 0 ;
	virtual HRESULT __safecall Get_Status(Activex::TOleEnum &Get_Status_result) = 0 ;
	virtual HRESULT __safecall Set_Status(Activex::TOleEnum Status) = 0 ;
	virtual HRESULT __safecall Get_StatusOnCompletionRecipients(WideString &Get_StatusOnCompletionRecipients_result) = 0 ;
	virtual HRESULT __safecall Set_StatusOnCompletionRecipients(const WideString StatusOnCompletionRecipients) = 0 ;
	virtual HRESULT __safecall Get_StatusUpdateRecipients(WideString &Get_StatusUpdateRecipients_result) = 0 ;
	virtual HRESULT __safecall Set_StatusUpdateRecipients(const WideString StatusUpdateRecipients) = 0 ;
	virtual HRESULT __safecall Get_TeamTask(Word &Get_TeamTask_result) = 0 ;
	virtual HRESULT __safecall Set_TeamTask(Word TeamTask) = 0 ;
	virtual HRESULT __safecall Get_TotalWork(int &Get_TotalWork_result) = 0 ;
	virtual HRESULT __safecall Set_TotalWork(int TotalWork) = 0 ;
	virtual HRESULT __safecall Assign(_di__TaskItem &Assign_result) = 0 ;
	virtual HRESULT __safecall CancelResponseState(void) = 0 ;
	virtual HRESULT __safecall ClearRecurrencePattern(void) = 0 ;
	virtual HRESULT __safecall GetRecurrencePattern(_di_RecurrencePattern &GetRecurrencePattern_result) = 0 ;
	virtual HRESULT __safecall MarkComplete(void) = 0 ;
	virtual HRESULT __safecall Respond(Activex::TOleEnum Response, const OleVariant fNoUI, const OleVariant fAdditionalTextDialog, _di__TaskItem &Respond_result) = 0 ;
	virtual HRESULT __safecall Send(void) = 0 ;
	virtual HRESULT __safecall SkipRecurrence(Word &SkipRecurrence_result) = 0 ;
	virtual HRESULT __safecall StatusReport(_di_IDispatch &StatusReport_result) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_InternetCodepage(int &Get_InternetCodepage_result) = 0 ;
	virtual HRESULT __safecall Set_InternetCodepage(int InternetCodepage) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_ActualWork() { int r; HRESULT hr = Get_ActualWork(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int ActualWork = {read=_scw_Get_ActualWork, write=Set_ActualWork};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_CardData() { WideString r; HRESULT hr = Get_CardData(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString CardData = {read=_scw_Get_CardData, write=Set_CardData};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Complete() { Word r; HRESULT hr = Get_Complete(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Complete = {read=_scw_Get_Complete, write=Set_Complete};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Contacts() { WideString r; HRESULT hr = Get_Contacts(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Contacts = {read=_scw_Get_Contacts, write=Set_Contacts};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ContactNames() { WideString r; HRESULT hr = Get_ContactNames(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ContactNames = {read=_scw_Get_ContactNames, write=Set_ContactNames};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_DateCompleted() { System::TDateTime r; HRESULT hr = Get_DateCompleted(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime DateCompleted = {read=_scw_Get_DateCompleted, write=Set_DateCompleted};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DelegationState() { Activex::TOleEnum r; HRESULT hr = Get_DelegationState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DelegationState = {read=_scw_Get_DelegationState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Delegator() { WideString r; HRESULT hr = Get_Delegator(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Delegator = {read=_scw_Get_Delegator};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_DueDate() { System::TDateTime r; HRESULT hr = Get_DueDate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime DueDate = {read=_scw_Get_DueDate, write=Set_DueDate};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsRecurring() { Word r; HRESULT hr = Get_IsRecurring(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsRecurring = {read=_scw_Get_IsRecurring};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Ordinal() { int r; HRESULT hr = Get_Ordinal(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Ordinal = {read=_scw_Get_Ordinal, write=Set_Ordinal};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Owner() { WideString r; HRESULT hr = Get_Owner(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Owner = {read=_scw_Get_Owner, write=Set_Owner};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Ownership() { Activex::TOleEnum r; HRESULT hr = Get_Ownership(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Ownership = {read=_scw_Get_Ownership};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_PercentComplete() { int r; HRESULT hr = Get_PercentComplete(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int PercentComplete = {read=_scw_Get_PercentComplete, write=Set_PercentComplete};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Recipients _scw_Get_Recipients() { _di_Recipients r; HRESULT hr = Get_Recipients(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Recipients Recipients = {read=_scw_Get_Recipients};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_ReminderTime() { System::TDateTime r; HRESULT hr = Get_ReminderTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime ReminderTime = {read=_scw_Get_ReminderTime, write=Set_ReminderTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ReminderOverrideDefault() { Word r; HRESULT hr = Get_ReminderOverrideDefault(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ReminderOverrideDefault = {read=_scw_Get_ReminderOverrideDefault, write=Set_ReminderOverrideDefault};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ReminderPlaySound() { Word r; HRESULT hr = Get_ReminderPlaySound(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ReminderPlaySound = {read=_scw_Get_ReminderPlaySound, write=Set_ReminderPlaySound};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_ReminderSet() { Word r; HRESULT hr = Get_ReminderSet(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word ReminderSet = {read=_scw_Get_ReminderSet, write=Set_ReminderSet};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ReminderSoundFile() { WideString r; HRESULT hr = Get_ReminderSoundFile(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ReminderSoundFile = {read=_scw_Get_ReminderSoundFile, write=Set_ReminderSoundFile};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_ResponseState() { Activex::TOleEnum r; HRESULT hr = Get_ResponseState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum ResponseState = {read=_scw_Get_ResponseState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Role() { WideString r; HRESULT hr = Get_Role(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Role = {read=_scw_Get_Role, write=Set_Role};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_SchedulePlusPriority() { WideString r; HRESULT hr = Get_SchedulePlusPriority(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString SchedulePlusPriority = {read=_scw_Get_SchedulePlusPriority, write=Set_SchedulePlusPriority};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_StartDate() { System::TDateTime r; HRESULT hr = Get_StartDate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime StartDate = {read=_scw_Get_StartDate, write=Set_StartDate};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Status() { Activex::TOleEnum r; HRESULT hr = Get_Status(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Status = {read=_scw_Get_Status, write=Set_Status};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_StatusOnCompletionRecipients() { WideString r; HRESULT hr = Get_StatusOnCompletionRecipients(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString StatusOnCompletionRecipients = {read=_scw_Get_StatusOnCompletionRecipients, write=Set_StatusOnCompletionRecipients};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_StatusUpdateRecipients() { WideString r; HRESULT hr = Get_StatusUpdateRecipients(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString StatusUpdateRecipients = {read=_scw_Get_StatusUpdateRecipients, write=Set_StatusUpdateRecipients};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_TeamTask() { Word r; HRESULT hr = Get_TeamTask(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word TeamTask = {read=_scw_Get_TeamTask, write=Set_TeamTask};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_TotalWork() { int r; HRESULT hr = Get_TotalWork(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int TotalWork = {read=_scw_Get_TotalWork, write=Set_TotalWork};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_InternetCodepage() { int r; HRESULT hr = Get_InternetCodepage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int InternetCodepage = {read=_scw_Get_InternetCodepage, write=Set_InternetCodepage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _TaskItemDisp;
typedef System::DelphiInterface<_TaskItemDisp> _di__TaskItemDisp;
__dispinterface INTERFACE_UUID("{00063035-0000-0000-C000-000000000046}") _TaskItemDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063038-0000-0000-C000-000000000046}") _TaskRequestAcceptItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall GetAssociatedTask(Word AddToTaskList, _di__TaskItem &GetAssociatedTask_result) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _TaskRequestAcceptItemDisp;
typedef System::DelphiInterface<_TaskRequestAcceptItemDisp> _di__TaskRequestAcceptItemDisp;
__dispinterface INTERFACE_UUID("{00063038-0000-0000-C000-000000000046}") _TaskRequestAcceptItemDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063039-0000-0000-C000-000000000046}") _TaskRequestDeclineItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall GetAssociatedTask(Word AddToTaskList, _di__TaskItem &GetAssociatedTask_result) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _TaskRequestDeclineItemDisp;
typedef System::DelphiInterface<_TaskRequestDeclineItemDisp> _di__TaskRequestDeclineItemDisp;
__dispinterface INTERFACE_UUID("{00063039-0000-0000-C000-000000000046}") _TaskRequestDeclineItemDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063036-0000-0000-C000-000000000046}") _TaskRequestItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall GetAssociatedTask(Word AddToTaskList, _di__TaskItem &GetAssociatedTask_result) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _TaskRequestItemDisp;
typedef System::DelphiInterface<_TaskRequestItemDisp> _di__TaskRequestItemDisp;
__dispinterface INTERFACE_UUID("{00063036-0000-0000-C000-000000000046}") _TaskRequestItemDisp  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{00063037-0000-0000-C000-000000000046}") _TaskRequestUpdateItem  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_Application(_di__Application &Get_Application_result) = 0 ;
	virtual HRESULT __safecall Get_Class_(Activex::TOleEnum &Get_Class__result) = 0 ;
	virtual HRESULT __safecall Get_Session(_di__NameSpace &Get_Session_result) = 0 ;
	virtual HRESULT __safecall Get_Parent(_di_IDispatch &Get_Parent_result) = 0 ;
	virtual HRESULT __safecall Get_Actions(_di_Actions &Get_Actions_result) = 0 ;
	virtual HRESULT __safecall Get_Attachments(_di_Attachments &Get_Attachments_result) = 0 ;
	virtual HRESULT __safecall Get_BillingInformation(WideString &Get_BillingInformation_result) = 0 ;
	virtual HRESULT __safecall Set_BillingInformation(const WideString BillingInformation) = 0 ;
	virtual HRESULT __safecall Get_Body(WideString &Get_Body_result) = 0 ;
	virtual HRESULT __safecall Set_Body(const WideString Body) = 0 ;
	virtual HRESULT __safecall Get_Categories(WideString &Get_Categories_result) = 0 ;
	virtual HRESULT __safecall Set_Categories(const WideString Categories) = 0 ;
	virtual HRESULT __safecall Get_Companies(WideString &Get_Companies_result) = 0 ;
	virtual HRESULT __safecall Set_Companies(const WideString Companies) = 0 ;
	virtual HRESULT __safecall Get_ConversationIndex(WideString &Get_ConversationIndex_result) = 0 ;
	virtual HRESULT __safecall Get_ConversationTopic(WideString &Get_ConversationTopic_result) = 0 ;
	virtual HRESULT __safecall Get_CreationTime(System::TDateTime &Get_CreationTime_result) = 0 ;
	virtual HRESULT __safecall Get_EntryID(WideString &Get_EntryID_result) = 0 ;
	virtual HRESULT __safecall Get_FormDescription(_di_FormDescription &Get_FormDescription_result) = 0 ;
	virtual HRESULT __safecall Get_GetInspector(_di__Inspector &Get_GetInspector_result) = 0 ;
	virtual HRESULT __safecall Get_Importance(Activex::TOleEnum &Get_Importance_result) = 0 ;
	virtual HRESULT __safecall Set_Importance(Activex::TOleEnum Importance) = 0 ;
	virtual HRESULT __safecall Get_LastModificationTime(System::TDateTime &Get_LastModificationTime_result) = 0 ;
	virtual HRESULT __safecall Get_MAPIOBJECT(System::_di_IInterface &Get_MAPIOBJECT_result) = 0 ;
	virtual HRESULT __safecall Get_MessageClass(WideString &Get_MessageClass_result) = 0 ;
	virtual HRESULT __safecall Set_MessageClass(const WideString MessageClass) = 0 ;
	virtual HRESULT __safecall Get_Mileage(WideString &Get_Mileage_result) = 0 ;
	virtual HRESULT __safecall Set_Mileage(const WideString Mileage) = 0 ;
	virtual HRESULT __safecall Get_NoAging(Word &Get_NoAging_result) = 0 ;
	virtual HRESULT __safecall Set_NoAging(Word NoAging) = 0 ;
	virtual HRESULT __safecall Get_OutlookInternalVersion(int &Get_OutlookInternalVersion_result) = 0 ;
	virtual HRESULT __safecall Get_OutlookVersion(WideString &Get_OutlookVersion_result) = 0 ;
	virtual HRESULT __safecall Get_Saved(Word &Get_Saved_result) = 0 ;
	virtual HRESULT __safecall Get_Sensitivity(Activex::TOleEnum &Get_Sensitivity_result) = 0 ;
	virtual HRESULT __safecall Set_Sensitivity(Activex::TOleEnum Sensitivity) = 0 ;
	virtual HRESULT __safecall Get_Size(int &Get_Size_result) = 0 ;
	virtual HRESULT __safecall Get_Subject(WideString &Get_Subject_result) = 0 ;
	virtual HRESULT __safecall Set_Subject(const WideString Subject) = 0 ;
	virtual HRESULT __safecall Get_UnRead(Word &Get_UnRead_result) = 0 ;
	virtual HRESULT __safecall Set_UnRead(Word UnRead) = 0 ;
	virtual HRESULT __safecall Get_UserProperties(_di_UserProperties &Get_UserProperties_result) = 0 ;
	virtual HRESULT __safecall Close(Activex::TOleEnum SaveMode) = 0 ;
	virtual HRESULT __safecall Copy(_di_IDispatch &Copy_result) = 0 ;
	virtual HRESULT __safecall Delete(void) = 0 ;
	virtual HRESULT __safecall Display(const OleVariant Modal) = 0 ;
	virtual HRESULT __safecall Move(const _di_MAPIFolder DestFldr, _di_IDispatch &Move_result) = 0 ;
	virtual HRESULT __safecall PrintOut(void) = 0 ;
	virtual HRESULT __safecall Save(void) = 0 ;
	virtual HRESULT __safecall SaveAs(const WideString Path, const OleVariant Type_) = 0 ;
	virtual HRESULT __safecall GetAssociatedTask(Word AddToTaskList, _di__TaskItem &GetAssociatedTask_result) = 0 ;
	virtual HRESULT __safecall Get_Links(_di_Links &Get_Links_result) = 0 ;
	virtual HRESULT __safecall Get_DownloadState(Activex::TOleEnum &Get_DownloadState_result) = 0 ;
	virtual HRESULT __safecall ShowCategoriesDialog(void) = 0 ;
	virtual HRESULT __safecall Get_ItemProperties(_di_ItemProperties &Get_ItemProperties_result) = 0 ;
	virtual HRESULT __safecall Get_MarkForDownload(Activex::TOleEnum &Get_MarkForDownload_result) = 0 ;
	virtual HRESULT __safecall Set_MarkForDownload(Activex::TOleEnum MarkForDownload) = 0 ;
	virtual HRESULT __safecall Get_IsConflict(Word &Get_IsConflict_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Application _scw_Get_Application() { _di__Application r; HRESULT hr = Get_Application(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Application Application = {read=_scw_Get_Application};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Class_() { Activex::TOleEnum r; HRESULT hr = Get_Class_(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Class_ = {read=_scw_Get_Class_};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__NameSpace _scw_Get_Session() { _di__NameSpace r; HRESULT hr = Get_Session(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__NameSpace Session = {read=_scw_Get_Session};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_IDispatch _scw_Get_Parent() { _di_IDispatch r; HRESULT hr = Get_Parent(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_IDispatch Parent = {read=_scw_Get_Parent};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Actions _scw_Get_Actions() { _di_Actions r; HRESULT hr = Get_Actions(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Actions Actions = {read=_scw_Get_Actions};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Attachments _scw_Get_Attachments() { _di_Attachments r; HRESULT hr = Get_Attachments(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Attachments Attachments = {read=_scw_Get_Attachments};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_BillingInformation() { WideString r; HRESULT hr = Get_BillingInformation(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString BillingInformation = {read=_scw_Get_BillingInformation, write=Set_BillingInformation};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Body() { WideString r; HRESULT hr = Get_Body(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Body = {read=_scw_Get_Body, write=Set_Body};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Categories() { WideString r; HRESULT hr = Get_Categories(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Categories = {read=_scw_Get_Categories, write=Set_Categories};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Companies() { WideString r; HRESULT hr = Get_Companies(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Companies = {read=_scw_Get_Companies, write=Set_Companies};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationIndex() { WideString r; HRESULT hr = Get_ConversationIndex(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationIndex = {read=_scw_Get_ConversationIndex};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_ConversationTopic() { WideString r; HRESULT hr = Get_ConversationTopic(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString ConversationTopic = {read=_scw_Get_ConversationTopic};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_CreationTime() { System::TDateTime r; HRESULT hr = Get_CreationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime CreationTime = {read=_scw_Get_CreationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_EntryID() { WideString r; HRESULT hr = Get_EntryID(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString EntryID = {read=_scw_Get_EntryID};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_FormDescription _scw_Get_FormDescription() { _di_FormDescription r; HRESULT hr = Get_FormDescription(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_FormDescription FormDescription = {read=_scw_Get_FormDescription};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di__Inspector _scw_Get_GetInspector() { _di__Inspector r; HRESULT hr = Get_GetInspector(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di__Inspector GetInspector = {read=_scw_Get_GetInspector};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Importance() { Activex::TOleEnum r; HRESULT hr = Get_Importance(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Importance = {read=_scw_Get_Importance, write=Set_Importance};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::TDateTime _scw_Get_LastModificationTime() { System::TDateTime r; HRESULT hr = Get_LastModificationTime(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::TDateTime LastModificationTime = {read=_scw_Get_LastModificationTime};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::_di_IInterface _scw_Get_MAPIOBJECT() { System::_di_IInterface r; HRESULT hr = Get_MAPIOBJECT(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property System::_di_IInterface MAPIOBJECT = {read=_scw_Get_MAPIOBJECT};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_MessageClass() { WideString r; HRESULT hr = Get_MessageClass(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString MessageClass = {read=_scw_Get_MessageClass, write=Set_MessageClass};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Mileage() { WideString r; HRESULT hr = Get_Mileage(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Mileage = {read=_scw_Get_Mileage, write=Set_Mileage};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_NoAging() { Word r; HRESULT hr = Get_NoAging(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word NoAging = {read=_scw_Get_NoAging, write=Set_NoAging};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_OutlookInternalVersion() { int r; HRESULT hr = Get_OutlookInternalVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int OutlookInternalVersion = {read=_scw_Get_OutlookInternalVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_OutlookVersion() { WideString r; HRESULT hr = Get_OutlookVersion(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString OutlookVersion = {read=_scw_Get_OutlookVersion};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_Saved() { Word r; HRESULT hr = Get_Saved(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word Saved = {read=_scw_Get_Saved};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_Sensitivity() { Activex::TOleEnum r; HRESULT hr = Get_Sensitivity(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum Sensitivity = {read=_scw_Get_Sensitivity, write=Set_Sensitivity};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_Size() { int r; HRESULT hr = Get_Size(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int Size = {read=_scw_Get_Size};
	#pragma option push -w-inl
	/* safecall wrapper */ inline WideString _scw_Get_Subject() { WideString r; HRESULT hr = Get_Subject(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property WideString Subject = {read=_scw_Get_Subject, write=Set_Subject};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_UnRead() { Word r; HRESULT hr = Get_UnRead(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word UnRead = {read=_scw_Get_UnRead, write=Set_UnRead};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_UserProperties _scw_Get_UserProperties() { _di_UserProperties r; HRESULT hr = Get_UserProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_UserProperties UserProperties = {read=_scw_Get_UserProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_Links _scw_Get_Links() { _di_Links r; HRESULT hr = Get_Links(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_Links Links = {read=_scw_Get_Links};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_DownloadState() { Activex::TOleEnum r; HRESULT hr = Get_DownloadState(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum DownloadState = {read=_scw_Get_DownloadState};
	#pragma option push -w-inl
	/* safecall wrapper */ inline _di_ItemProperties _scw_Get_ItemProperties() { _di_ItemProperties r; HRESULT hr = Get_ItemProperties(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property _di_ItemProperties ItemProperties = {read=_scw_Get_ItemProperties};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Activex::TOleEnum _scw_Get_MarkForDownload() { Activex::TOleEnum r; HRESULT hr = Get_MarkForDownload(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Activex::TOleEnum MarkForDownload = {read=_scw_Get_MarkForDownload, write=Set_MarkForDownload};
	#pragma option push -w-inl
	/* safecall wrapper */ inline Word _scw_Get_IsConflict() { Word r; HRESULT hr = Get_IsConflict(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property Word IsConflict = {read=_scw_Get_IsConflict};
};

__dispinterface _TaskRequestUpdateItemDisp;
typedef System::DelphiInterface<_TaskRequestUpdateItemDisp> _di__TaskRequestUpdateItemDisp;
__dispinterface INTERFACE_UUID("{00063037-0000-0000-C000-000000000046}") _TaskRequestUpdateItemDisp  : public IDispatch 
{
	
};

__dispinterface ApplicationEvents_10;
typedef System::DelphiInterface<ApplicationEvents_10> _di_ApplicationEvents_10;
__dispinterface INTERFACE_UUID("{0006300E-0000-0000-C000-000000000046}") ApplicationEvents_10  : public IDispatch 
{
	
};

__dispinterface ResultsEvents;
typedef System::DelphiInterface<ResultsEvents> _di_ResultsEvents;
__dispinterface INTERFACE_UUID("{0006300D-0000-0000-C000-000000000046}") ResultsEvents  : public IDispatch 
{
	
};

__dispinterface _ViewsEvents;
typedef System::DelphiInterface<_ViewsEvents> _di__ViewsEvents;
__dispinterface INTERFACE_UUID("{000630A5-0000-0000-C000-000000000046}") _ViewsEvents  : public IDispatch 
{
	
};

__dispinterface ReminderCollectionEvents;
typedef System::DelphiInterface<ReminderCollectionEvents> _di_ReminderCollectionEvents;
__dispinterface INTERFACE_UUID("{000630B2-0000-0000-C000-000000000046}") ReminderCollectionEvents  : public IDispatch 
{
	
};

__interface _IRecipientControl;
typedef System::DelphiInterface<_IRecipientControl> _di__IRecipientControl;
__interface INTERFACE_UUID("{D87E7E16-6897-11CE-A6C0-00AA00608FAA}") _IRecipientControl  : public IDispatch 
{
	
public:
	virtual HRESULT __stdcall Get_Enabled(/* out */ Word &Enabled) = 0 ;
	virtual HRESULT __stdcall Set_Enabled(Word Enabled) = 0 ;
	virtual HRESULT __stdcall Get_BackColor(/* out */ int &BackColor) = 0 ;
	virtual HRESULT __stdcall Set_BackColor(int BackColor) = 0 ;
	virtual HRESULT __stdcall Get_ForeColor(/* out */ int &ForeColor) = 0 ;
	virtual HRESULT __stdcall Set_ForeColor(int ForeColor) = 0 ;
	virtual HRESULT __stdcall Get_ReadOnly(/* out */ Word &ReadOnly) = 0 ;
	virtual HRESULT __stdcall Set_ReadOnly(Word ReadOnly) = 0 ;
	virtual HRESULT __stdcall Get_Font(/* out */ _di_IDispatch &Font) = 0 ;
	virtual HRESULT __stdcall Set_Font(const _di_IDispatch Font) = 0 ;
	virtual HRESULT __stdcall Get_SpecialEffect(/* out */ int &Effect) = 0 ;
	virtual HRESULT __stdcall Set_SpecialEffect(int Effect) = 0 ;
};

__dispinterface INTERFACE_UUID("{0006F025-0000-0000-C000-000000000046}") _DRecipientControl  : public IDispatch 
{
	
};

__dispinterface _DRecipientControlEvents;
typedef System::DelphiInterface<_DRecipientControlEvents> _di__DRecipientControlEvents;
__dispinterface INTERFACE_UUID("{D87E7E17-6897-11CE-A6C0-00AA00608FAA}") _DRecipientControlEvents  : public IDispatch 
{
	
};

__interface _IDocSiteControl;
typedef System::DelphiInterface<_IDocSiteControl> _di__IDocSiteControl;
__interface INTERFACE_UUID("{43507DD0-811D-11CE-B565-00AA00608FAA}") _IDocSiteControl  : public IDispatch 
{
	
public:
	virtual HRESULT __stdcall Get_ReadOnly(/* out */ Word &ReadOnly) = 0 ;
	virtual HRESULT __stdcall Set_ReadOnly(Word ReadOnly) = 0 ;
};

__dispinterface INTERFACE_UUID("{0006F026-0000-0000-C000-000000000046}") _DDocSiteControl  : public IDispatch 
{
	
};

__dispinterface _DDocSiteControlEvents;
typedef System::DelphiInterface<_DDocSiteControlEvents> _di__DDocSiteControlEvents;
__dispinterface INTERFACE_UUID("{50BB9B50-811D-11CE-B565-00AA00608FAA}") _DDocSiteControlEvents  : public IDispatch 
{
	
};

__interface INTERFACE_UUID("{0006F097-0000-0000-C000-000000000046}") _IDpxCtrl  : public IDispatch 
{
	
public:
	virtual HRESULT __safecall Get_StartDate(int &Get_StartDate_result) = 0 ;
	virtual HRESULT __safecall Set_StartDate(int plStartDate) = 0 ;
	virtual HRESULT __safecall Get_EndDate(int &Get_EndDate_result) = 0 ;
	virtual HRESULT __safecall Set_EndDate(int plEndDate) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_StartDate() { int r; HRESULT hr = Get_StartDate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int StartDate = {read=_scw_Get_StartDate, write=Set_StartDate};
	#pragma option push -w-inl
	/* safecall wrapper */ inline int _scw_Get_EndDate() { int r; HRESULT hr = Get_EndDate(r); System::CheckSafecallResult(hr); return r; }
	#pragma option pop
	__property int EndDate = {read=_scw_Get_EndDate, write=Set_EndDate};
};

__dispinterface _IDpxCtrlDisp;
typedef System::DelphiInterface<_IDpxCtrlDisp> _di__IDpxCtrlDisp;
__dispinterface INTERFACE_UUID("{0006F097-0000-0000-C000-000000000046}") _IDpxCtrlDisp  : public IDispatch 
{
	
};

__interface _IPageWrapCtrl;
typedef System::DelphiInterface<_IPageWrapCtrl> _di__IPageWrapCtrl;
__interface INTERFACE_UUID("{494F0970-DD96-11D2-AF70-006008AFF117}") _IPageWrapCtrl  : public IDispatch 
{
	
public:
	virtual HRESULT __stdcall Get_BackColor(/* out */ unsigned &pcolorRef) = 0 ;
	virtual HRESULT __stdcall Set_BackColor(unsigned pcolorRef) = 0 ;
};

__dispinterface INTERFACE_UUID("{0006F096-0000-0000-C000-000000000046}") _DPageWrapCtrl  : public IDispatch 
{
	
};

__dispinterface _DPageWrapCtrlEvents;
typedef System::DelphiInterface<_DPageWrapCtrlEvents> _di__DPageWrapCtrlEvents;
__dispinterface INTERFACE_UUID("{494F0971-DD96-11D2-AF70-006008AFF117}") _DPageWrapCtrlEvents  : public IDispatch 
{
	
};

__interface _IInspectorCtrl;
typedef System::DelphiInterface<_IInspectorCtrl> _di__IInspectorCtrl;
__interface INTERFACE_UUID("{E182A127-EADD-46E1-B878-482C48CD8754}") _IInspectorCtrl  : public IDispatch 
{
	
public:
	virtual HRESULT __stdcall Get_URL(/* out */ WideString &ppwzURL) = 0 ;
	virtual HRESULT __stdcall Set_URL(const WideString ppwzURL) = 0 ;
	virtual HRESULT __stdcall Get_Item(/* out */ _di_IDispatch &ppdisp) = 0 ;
	virtual HRESULT __stdcall OnItemChange(const _di_IDispatch pdispItem) = 0 ;
};

__dispinterface INTERFACE_UUID("{0006F09D-0000-0000-C000-000000000046}") _DInspectorCtrl  : public IDispatch 
{
	
};

__dispinterface _DInspectorEvents;
typedef System::DelphiInterface<_DInspectorEvents> _di__DInspectorEvents;
__dispinterface INTERFACE_UUID("{2D9C6D57-BD3C-4275-BED2-73F0EDC18CCE}") _DInspectorEvents  : public IDispatch 
{
	
};

__dispinterface INTERFACE_UUID("{11858B51-DE06-494E-915A-6CCEF17F7CB6}") DDataSourceObject  : public IDispatch 
{
	
};

class DELPHICLASS CoSyncObject;
class PASCALIMPLEMENTATION CoSyncObject : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__SyncObject __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__SyncObject __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoSyncObject(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoSyncObject(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoExplorer;
class PASCALIMPLEMENTATION CoExplorer : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Explorer __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Explorer __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoExplorer(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoExplorer(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoInspector;
class PASCALIMPLEMENTATION CoInspector : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Inspector __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Inspector __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoInspector(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoInspector(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoMailItem;
class PASCALIMPLEMENTATION CoMailItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__MailItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__MailItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoMailItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoMailItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoAppointmentItem;
class PASCALIMPLEMENTATION CoAppointmentItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__AppointmentItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__AppointmentItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoAppointmentItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoAppointmentItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoMeetingItem;
class PASCALIMPLEMENTATION CoMeetingItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__MeetingItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__MeetingItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoMeetingItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoMeetingItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoTaskItem;
class PASCALIMPLEMENTATION CoTaskItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__TaskItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__TaskItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoTaskItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoTaskItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoOutlookApplication;
class PASCALIMPLEMENTATION CoOutlookApplication : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Application __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Application __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoOutlookApplication(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoOutlookApplication(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoContactItem;
class PASCALIMPLEMENTATION CoContactItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__ContactItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__ContactItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoContactItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoContactItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoDistListItem;
class PASCALIMPLEMENTATION CoDistListItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__DistListItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__DistListItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoDistListItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoDistListItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoDocumentItem;
class PASCALIMPLEMENTATION CoDocumentItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__DocumentItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__DocumentItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoDocumentItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoDocumentItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoExplorers;
class PASCALIMPLEMENTATION CoExplorers : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Explorers __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Explorers __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoExplorers(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoExplorers(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoInspectors;
class PASCALIMPLEMENTATION CoInspectors : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Inspectors __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Inspectors __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoInspectors(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoInspectors(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoFolders;
class PASCALIMPLEMENTATION CoFolders : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Folders __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Folders __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoFolders(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoFolders(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoItems;
class PASCALIMPLEMENTATION CoItems : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Items __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Items __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoItems(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoItems(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoJournalItem;
class PASCALIMPLEMENTATION CoJournalItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__JournalItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__JournalItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoJournalItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoJournalItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoNameSpace;
class PASCALIMPLEMENTATION CoNameSpace : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__NameSpace __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__NameSpace __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoNameSpace(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoNameSpace(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoNoteItem;
class PASCALIMPLEMENTATION CoNoteItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__NoteItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__NoteItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoNoteItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoNoteItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoOutlookBarGroups;
class PASCALIMPLEMENTATION CoOutlookBarGroups : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__OutlookBarGroups __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__OutlookBarGroups __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoOutlookBarGroups(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoOutlookBarGroups(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoOutlookBarPane;
class PASCALIMPLEMENTATION CoOutlookBarPane : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__OutlookBarPane __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__OutlookBarPane __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoOutlookBarPane(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoOutlookBarPane(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoOutlookBarShortcuts;
class PASCALIMPLEMENTATION CoOutlookBarShortcuts : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__OutlookBarShortcuts __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__OutlookBarShortcuts __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoOutlookBarShortcuts(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoOutlookBarShortcuts(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoPostItem;
class PASCALIMPLEMENTATION CoPostItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__PostItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__PostItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoPostItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoPostItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoRemoteItem;
class PASCALIMPLEMENTATION CoRemoteItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__RemoteItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__RemoteItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoRemoteItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoRemoteItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoReportItem;
class PASCALIMPLEMENTATION CoReportItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__ReportItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__ReportItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoReportItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoReportItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoTaskRequestAcceptItem;
class PASCALIMPLEMENTATION CoTaskRequestAcceptItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__TaskRequestAcceptItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__TaskRequestAcceptItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoTaskRequestAcceptItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoTaskRequestAcceptItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoTaskRequestDeclineItem;
class PASCALIMPLEMENTATION CoTaskRequestDeclineItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__TaskRequestDeclineItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__TaskRequestDeclineItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoTaskRequestDeclineItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoTaskRequestDeclineItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoTaskRequestItem;
class PASCALIMPLEMENTATION CoTaskRequestItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__TaskRequestItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__TaskRequestItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoTaskRequestItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoTaskRequestItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoTaskRequestUpdateItem;
class PASCALIMPLEMENTATION CoTaskRequestUpdateItem : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__TaskRequestUpdateItem __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__TaskRequestUpdateItem __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoTaskRequestUpdateItem(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoTaskRequestUpdateItem(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoResults;
class PASCALIMPLEMENTATION CoResults : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Results __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Results __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoResults(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoResults(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoViews;
class PASCALIMPLEMENTATION CoViews : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Views __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Views __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoViews(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoViews(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoReminder;
class PASCALIMPLEMENTATION CoReminder : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Reminder __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Reminder __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoReminder(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoReminder(void) { }
	#pragma option pop
	
};


class DELPHICLASS CoReminders;
class PASCALIMPLEMENTATION CoReminders : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	/*         class method */ static _di__Reminders __fastcall Create(TMetaClass* vmt);
	/*         class method */ static _di__Reminders __fastcall CreateRemote(TMetaClass* vmt, const AnsiString MachineName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall CoReminders(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~CoReminders(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
static const Shortint OutlookMajorVersion = 0x9;
static const Shortint OutlookMinorVersion = 0x1;
extern PACKAGE GUID LIBID_Outlook;
extern PACKAGE GUID IID_Action;
extern PACKAGE GUID IID__Application;
extern PACKAGE GUID IID__NameSpace;
extern PACKAGE GUID IID_Recipient;
extern PACKAGE GUID IID_AddressEntry;
extern PACKAGE GUID IID_AddressEntries;
extern PACKAGE GUID IID__Folders;
extern PACKAGE GUID IID_MAPIFolder;
extern PACKAGE GUID IID__Items;
extern PACKAGE GUID IID__Explorer;
extern PACKAGE GUID IID_Panes;
extern PACKAGE GUID IID_Selection;
extern PACKAGE GUID IID_View;
extern PACKAGE GUID IID__Views;
extern PACKAGE GUID IID_AddressLists;
extern PACKAGE GUID IID_AddressList;
extern PACKAGE GUID IID_SyncObjects;
extern PACKAGE GUID IID__SyncObject;
extern PACKAGE GUID DIID_SyncObjectEvents;
extern PACKAGE GUID CLASS_SyncObject;
extern PACKAGE GUID IID__Inspector;
extern PACKAGE GUID IID__Explorers;
extern PACKAGE GUID DIID_ExplorerEvents;
extern PACKAGE GUID DIID_ExplorerEvents_10;
extern PACKAGE GUID CLASS_Explorer;
extern PACKAGE GUID IID__Inspectors;
extern PACKAGE GUID DIID_InspectorEvents;
extern PACKAGE GUID DIID_InspectorEvents_10;
extern PACKAGE GUID CLASS_Inspector;
extern PACKAGE GUID IID_Search;
extern PACKAGE GUID IID__Results;
extern PACKAGE GUID IID__Reminders;
extern PACKAGE GUID IID__Reminder;
extern PACKAGE GUID IID_Actions;
extern PACKAGE GUID DIID_ApplicationEvents;
extern PACKAGE GUID IID_PropertyPages;
extern PACKAGE GUID IID__AppointmentItem;
extern PACKAGE GUID IID_Attachments;
extern PACKAGE GUID IID_Attachment;
extern PACKAGE GUID IID_FormDescription;
extern PACKAGE GUID IID_UserProperties;
extern PACKAGE GUID IID_UserProperty;
extern PACKAGE GUID IID_Recipients;
extern PACKAGE GUID IID__MailItem;
extern PACKAGE GUID DIID_ItemEvents;
extern PACKAGE GUID IID_Links;
extern PACKAGE GUID IID_Link;
extern PACKAGE GUID IID_ItemProperties;
extern PACKAGE GUID IID_ItemProperty;
extern PACKAGE GUID DIID_ItemEvents_10;
extern PACKAGE GUID CLASS_MailItem;
extern PACKAGE GUID IID_RecurrencePattern;
extern PACKAGE GUID IID_Exceptions;
extern PACKAGE GUID IID_Exception;
extern PACKAGE GUID CLASS_AppointmentItem;
extern PACKAGE GUID IID__MeetingItem;
extern PACKAGE GUID CLASS_MeetingItem;
extern PACKAGE GUID IID__ContactItem;
extern PACKAGE GUID IID__DistListItem;
extern PACKAGE GUID IID__DocumentItem;
extern PACKAGE GUID DIID_ExplorersEvents;
extern PACKAGE GUID DIID_FoldersEvents;
extern PACKAGE GUID DIID_InspectorsEvents;
extern PACKAGE GUID DIID_ItemsEvents;
extern PACKAGE GUID IID__JournalItem;
extern PACKAGE GUID DIID_NameSpaceEvents;
extern PACKAGE GUID IID__NoteItem;
extern PACKAGE GUID IID_OutlookBarGroup;
extern PACKAGE GUID IID__OutlookBarShortcuts;
extern PACKAGE GUID IID_OutlookBarShortcut;
extern PACKAGE GUID IID__OutlookBarGroups;
extern PACKAGE GUID DIID_OutlookBarGroupsEvents;
extern PACKAGE GUID IID__OutlookBarPane;
extern PACKAGE GUID IID_OutlookBarStorage;
extern PACKAGE GUID DIID_OutlookBarPaneEvents;
extern PACKAGE GUID DIID_OutlookBarShortcutsEvents;
extern PACKAGE GUID IID_PropertyPage;
extern PACKAGE GUID IID_PropertyPageSite;
extern PACKAGE GUID IID_Pages;
extern PACKAGE GUID IID__PostItem;
extern PACKAGE GUID IID__RemoteItem;
extern PACKAGE GUID IID__ReportItem;
extern PACKAGE GUID IID__TaskItem;
extern PACKAGE GUID CLASS_TaskItem;
extern PACKAGE GUID IID__TaskRequestAcceptItem;
extern PACKAGE GUID IID__TaskRequestDeclineItem;
extern PACKAGE GUID IID__TaskRequestItem;
extern PACKAGE GUID IID__TaskRequestUpdateItem;
extern PACKAGE GUID DIID_ApplicationEvents_10;
extern PACKAGE GUID DIID_ResultsEvents;
extern PACKAGE GUID DIID__ViewsEvents;
extern PACKAGE GUID DIID_ReminderCollectionEvents;
extern PACKAGE GUID CLASS_OutlookApplication;
extern PACKAGE GUID CLASS_Application_;
extern PACKAGE GUID CLASS_ContactItem;
extern PACKAGE GUID CLASS_DistListItem;
extern PACKAGE GUID CLASS_DocumentItem;
extern PACKAGE GUID CLASS_Explorers;
extern PACKAGE GUID CLASS_Inspectors;
extern PACKAGE GUID CLASS_Folders;
extern PACKAGE GUID CLASS_Items;
extern PACKAGE GUID CLASS_JournalItem;
extern PACKAGE GUID CLASS_NameSpace;
extern PACKAGE GUID CLASS_NoteItem;
extern PACKAGE GUID CLASS_OutlookBarGroups;
extern PACKAGE GUID CLASS_OutlookBarPane;
extern PACKAGE GUID CLASS_OutlookBarShortcuts;
extern PACKAGE GUID CLASS_PostItem;
extern PACKAGE GUID CLASS_RemoteItem;
extern PACKAGE GUID CLASS_ReportItem;
extern PACKAGE GUID CLASS_TaskRequestAcceptItem;
extern PACKAGE GUID CLASS_TaskRequestDeclineItem;
extern PACKAGE GUID CLASS_TaskRequestItem;
extern PACKAGE GUID CLASS_TaskRequestUpdateItem;
extern PACKAGE GUID IID__IRecipientControl;
extern PACKAGE GUID DIID__DRecipientControl;
extern PACKAGE GUID DIID__DRecipientControlEvents;
extern PACKAGE GUID CLASS__RecipientControl;
extern PACKAGE GUID IID__IDocSiteControl;
extern PACKAGE GUID DIID__DDocSiteControl;
extern PACKAGE GUID DIID__DDocSiteControlEvents;
extern PACKAGE GUID CLASS__DocSiteControl;
extern PACKAGE GUID IID__IDpxCtrl;
extern PACKAGE GUID CLASS__DpxCtrl;
extern PACKAGE GUID IID__IPageWrapCtrl;
extern PACKAGE GUID DIID__DPageWrapCtrl;
extern PACKAGE GUID DIID__DPageWrapCtrlEvents;
extern PACKAGE GUID CLASS__PageWrapCtrl;
extern PACKAGE GUID IID__IInspectorCtrl;
extern PACKAGE GUID DIID__DInspectorCtrl;
extern PACKAGE GUID DIID__DInspectorEvents;
extern PACKAGE GUID CLASS__InspectorCtrl;
extern PACKAGE GUID DIID_DDataSourceObject;
extern PACKAGE GUID CLASS_DataSourceObject;
extern PACKAGE GUID CLASS_Results;
extern PACKAGE GUID CLASS_Views;
extern PACKAGE GUID CLASS_Reminder;
extern PACKAGE GUID CLASS_Reminders;
static const Shortint olReply = 0x0;
static const Shortint olReplyAll = 0x1;
static const Shortint olForward = 0x2;
static const Shortint olReplyFolder = 0x3;
static const Shortint olRespond = 0x4;
static const Shortint olOmitOriginalText = 0x0;
static const Shortint olEmbedOriginalItem = 0x1;
static const Shortint olIncludeOriginalText = 0x2;
static const Shortint olIndentOriginalText = 0x3;
static const Shortint olLinkOriginalItem = 0x4;
static const Shortint olUserPreference = 0x5;
static const Word olReplyTickOriginalText = 0x3e8;
static const Shortint olOpen = 0x0;
static const Shortint olSend = 0x1;
static const Shortint olPrompt = 0x2;
static const Shortint olDontShow = 0x0;
static const Shortint olMenu = 0x1;
static const Shortint olMenuAndToolbar = 0x2;
static const Shortint olByValue = 0x1;
static const Shortint olByReference = 0x4;
static const Shortint olEmbeddeditem = 0x5;
static const Shortint olOLE = 0x6;
static const Shortint olFree = 0x0;
static const Shortint olTentative = 0x1;
static const Shortint olBusy = 0x2;
static const Shortint olOutOfOffice = 0x3;
static const Shortint olSunday = 0x1;
static const Shortint olMonday = 0x2;
static const Shortint olTuesday = 0x4;
static const Shortint olWednesday = 0x8;
static const Shortint olThursday = 0x10;
static const Shortint olFriday = 0x20;
static const Shortint olSaturday = 0x40;
static const Shortint olFolderDeletedItems = 0x3;
static const Shortint olFolderOutbox = 0x4;
static const Shortint olFolderSentMail = 0x5;
static const Shortint olFolderInbox = 0x6;
static const Shortint olFolderCalendar = 0x9;
static const Shortint olFolderContacts = 0xa;
static const Shortint olFolderJournal = 0xb;
static const Shortint olFolderNotes = 0xc;
static const Shortint olFolderTasks = 0xd;
static const Shortint olFolderDrafts = 0x10;
static const Shortint olPublicFoldersAllPublicFolders = 0x12;
static const Shortint olUser = 0x0;
static const Shortint olDistList = 0x1;
static const Shortint olForum = 0x2;
static const Shortint olAgent = 0x3;
static const Shortint olOrganization = 0x4;
static const Shortint olPrivateDistList = 0x5;
static const Shortint olRemoteUser = 0x6;
static const Shortint olEditorText = 0x1;
static const Shortint olEditorHTML = 0x2;
static const Shortint olEditorRTF = 0x3;
static const Shortint olEditorWord = 0x4;
static const Shortint olNoFlag = 0x0;
static const Shortint olFlagComplete = 0x1;
static const Shortint olFlagMarked = 0x2;
static const Shortint olFolderDisplayNormal = 0x0;
static const Shortint olFolderDisplayFolderOnly = 0x1;
static const Shortint olFolderDisplayNoNavigation = 0x2;
static const Shortint olDefaultRegistry = 0x0;
static const Shortint olPersonalRegistry = 0x2;
static const Shortint olFolderRegistry = 0x3;
static const Shortint olOrganizationRegistry = 0x4;
static const Shortint olUnspecified = 0x0;
static const Shortint olFemale = 0x1;
static const Shortint olMale = 0x2;
static const Shortint olImportanceLow = 0x0;
static const Shortint olImportanceNormal = 0x1;
static const Shortint olImportanceHigh = 0x2;
static const Shortint olSave = 0x0;
static const Shortint olDiscard = 0x1;
static const Shortint olPromptForSave = 0x2;
static const Shortint olMailItem = 0x0;
static const Shortint olAppointmentItem = 0x1;
static const Shortint olContactItem = 0x2;
static const Shortint olTaskItem = 0x3;
static const Shortint olJournalItem = 0x4;
static const Shortint olNoteItem = 0x5;
static const Shortint olPostItem = 0x6;
static const Shortint olDistributionListItem = 0x7;
static const Shortint olAssociatedContact = 0x1;
static const Shortint olNone = 0x0;
static const Shortint olHome = 0x1;
static const Shortint olBusiness = 0x2;
static const Shortint olOther = 0x3;
static const Shortint olOriginator = 0x0;
static const Shortint olTo = 0x1;
static const Shortint olCC = 0x2;
static const Shortint olBCC = 0x3;
static const Shortint olOrganizer = 0x0;
static const Shortint olRequired = 0x1;
static const Shortint olOptional = 0x2;
static const Shortint olResource = 0x3;
static const Shortint olMeetingTentative = 0x2;
static const Shortint olMeetingAccepted = 0x3;
static const Shortint olMeetingDeclined = 0x4;
static const Shortint olNonMeeting = 0x0;
static const Shortint olMeeting = 0x1;
static const Shortint olMeetingReceived = 0x3;
static const Shortint olMeetingCanceled = 0x5;
static const Shortint olNetMeeting = 0x0;
static const Shortint olNetShow = 0x1;
static const Shortint olExchangeConferencing = 0x2;
static const Shortint olBlue = 0x0;
static const Shortint olGreen = 0x1;
static const Shortint olPink = 0x2;
static const Shortint olYellow = 0x3;
static const Shortint olWhite = 0x4;
static const Shortint olApplication = 0x0;
static const Shortint olNamespace = 0x1;
static const Shortint olFolder = 0x2;
static const Shortint olRecipient = 0x4;
static const Shortint olAttachment = 0x5;
static const Shortint olAddressList = 0x7;
static const Shortint olAddressEntry = 0x8;
static const Shortint olFolders = 0xf;
static const Shortint olItems = 0x10;
static const Shortint olRecipients = 0x11;
static const Shortint olAttachments = 0x12;
static const Shortint olAddressLists = 0x14;
static const Shortint olAddressEntries = 0x15;
static const Shortint olAppointment = 0x1a;
static const Shortint olMeetingRequest = 0x35;
static const Shortint olMeetingCancellation = 0x36;
static const Shortint olMeetingResponseNegative = 0x37;
static const Shortint olMeetingResponsePositive = 0x38;
static const Shortint olMeetingResponseTentative = 0x39;
static const Shortint olRecurrencePattern = 0x1c;
static const Shortint olExceptions = 0x1d;
static const Shortint olException = 0x1e;
static const Shortint olAction = 0x20;
static const Shortint olActions = 0x21;
static const Shortint olExplorer = 0x22;
static const Shortint olInspector = 0x23;
static const Shortint olPages = 0x24;
static const Shortint olFormDescription = 0x25;
static const Shortint olUserProperties = 0x26;
static const Shortint olUserProperty = 0x27;
static const Shortint olContact = 0x28;
static const Shortint olDocument = 0x29;
static const Shortint olJournal = 0x2a;
static const Shortint olMail = 0x2b;
static const Shortint olNote = 0x2c;
static const Shortint olPost = 0x2d;
static const Shortint olReport = 0x2e;
static const Shortint olRemote = 0x2f;
static const Shortint olTask = 0x30;
static const Shortint olTaskRequest = 0x31;
static const Shortint olTaskRequestUpdate = 0x32;
static const Shortint olTaskRequestAccept = 0x33;
static const Shortint olTaskRequestDecline = 0x34;
static const Shortint olExplorers = 0x3c;
static const Shortint olInspectors = 0x3d;
static const Shortint olPanes = 0x3e;
static const Shortint olOutlookBarPane = 0x3f;
static const Shortint olOutlookBarStorage = 0x40;
static const Shortint olOutlookBarGroups = 0x41;
static const Shortint olOutlookBarGroup = 0x42;
static const Shortint olOutlookBarShortcuts = 0x43;
static const Shortint olOutlookBarShortcut = 0x44;
static const Shortint olDistributionList = 0x45;
static const Shortint olPropertyPageSite = 0x46;
static const Shortint olPropertyPages = 0x47;
static const Shortint olSyncObject = 0x48;
static const Shortint olSyncObjects = 0x49;
static const Shortint olSelection = 0x4a;
static const Shortint olLink = 0x4b;
static const Shortint olLinks = 0x4c;
static const Shortint olSearch = 0x4d;
static const Shortint olResults = 0x4e;
static const Shortint olViews = 0x4f;
static const Shortint olView = 0x50;
static const Shortint olItemProperties = 0x62;
static const Shortint olItemProperty = 0x63;
static const Shortint olReminders = 0x64;
static const Shortint olReminder = 0x65;
static const Shortint olLargeIcon = 0x0;
static const Shortint olSmallIcon = 0x1;
static const Shortint olOutlookBar = 0x1;
static const Shortint olFolderList = 0x2;
static const Shortint olPreview = 0x3;
static const Shortint olApptNotRecurring = 0x0;
static const Shortint olApptMaster = 0x1;
static const Shortint olApptOccurrence = 0x2;
static const Shortint olApptException = 0x3;
static const Shortint olRecursDaily = 0x0;
static const Shortint olRecursWeekly = 0x1;
static const Shortint olRecursMonthly = 0x2;
static const Shortint olRecursMonthNth = 0x3;
static const Shortint olRecursYearly = 0x5;
static const Shortint olRecursYearNth = 0x6;
static const Shortint olRemoteStatusNone = 0x0;
static const Shortint olUnMarked = 0x1;
static const Shortint olMarkedForDownload = 0x2;
static const Shortint olMarkedForCopy = 0x3;
static const Shortint olMarkedForDelete = 0x4;
static const Shortint olResponseNone = 0x0;
static const Shortint olResponseOrganized = 0x1;
static const Shortint olResponseTentative = 0x2;
static const Shortint olResponseAccepted = 0x3;
static const Shortint olResponseDeclined = 0x4;
static const Shortint olResponseNotResponded = 0x5;
static const Shortint olTXT = 0x0;
static const Shortint olRTF = 0x1;
static const Shortint olTemplate = 0x2;
static const Shortint olMSG = 0x3;
static const Shortint olDoc = 0x4;
static const Shortint olHTML = 0x5;
static const Shortint olVCard = 0x6;
static const Shortint olVCal = 0x7;
static const Shortint olICal = 0x8;
static const Shortint olNormal = 0x0;
static const Shortint olPersonal = 0x1;
static const Shortint olPrivate = 0x2;
static const Shortint olConfidential = 0x3;
static const Shortint olSortNone = 0x0;
static const Shortint olAscending = 0x1;
static const Shortint olDescending = 0x2;
static const Shortint olTaskNotDelegated = 0x0;
static const Shortint olTaskDelegationUnknown = 0x1;
static const Shortint olTaskDelegationAccepted = 0x2;
static const Shortint olTaskDelegationDeclined = 0x3;
static const Shortint olNewTask = 0x0;
static const Shortint olDelegatedTask = 0x1;
static const Shortint olOwnTask = 0x2;
static const Shortint olUpdate = 0x2;
static const Shortint olFinalStatus = 0x3;
static const Shortint olTaskSimple = 0x0;
static const Shortint olTaskAssign = 0x1;
static const Shortint olTaskAccept = 0x2;
static const Shortint olTaskDecline = 0x3;
static const Shortint olTaskNotStarted = 0x0;
static const Shortint olTaskInProgress = 0x1;
static const Shortint olTaskComplete = 0x2;
static const Shortint olTaskWaiting = 0x3;
static const Shortint olTaskDeferred = 0x4;
static const Shortint olTrackingNone = 0x0;
static const Shortint olTrackingDelivered = 0x1;
static const Shortint olTrackingNotDelivered = 0x2;
static const Shortint olTrackingNotRead = 0x3;
static const Shortint olTrackingRecallFailure = 0x4;
static const Shortint olTrackingRecallSuccess = 0x5;
static const Shortint olTrackingRead = 0x6;
static const Shortint olTrackingReplied = 0x7;
static const Shortint olOutlookInternal = 0x0;
static const Shortint olText = 0x1;
static const Shortint olNumber = 0x3;
static const Shortint olDateTime = 0x5;
static const Shortint olYesNo = 0x6;
static const Shortint olDuration = 0x7;
static const Shortint olKeywords = 0xb;
static const Shortint olPercent = 0xc;
static const Shortint olCurrency = 0xe;
static const Shortint olFormula = 0x12;
static const Shortint olCombination = 0x13;
static const Shortint olMaximized = 0x0;
static const Shortint olMinimized = 0x1;
static const Shortint olNormalWindow = 0x2;
static const Shortint olSyncStopped = 0x0;
static const Shortint olSyncStarted = 0x1;
static const Shortint olFormatUnspecified = 0x0;
static const Shortint olFormatPlain = 0x1;
static const Shortint olFormatHTML = 0x2;
static const Shortint olFormatRichText = 0x3;
static const Shortint olHeaderOnly = 0x0;
static const Shortint olFullItem = 0x1;
static const Shortint olExcelWorkSheetItem = 0x8;
static const Shortint olWordDocumentItem = 0x9;
static const Shortint olPowerPointShowItem = 0xa;
static const Shortint olViewSaveOptionThisFolderEveryone = 0x0;
static const Shortint olViewSaveOptionThisFolderOnlyMe = 0x1;
static const Shortint olViewSaveOptionAllFoldersOfType = 0x2;
static const Shortint olTableView = 0x0;
static const Shortint olCardView = 0x1;
static const Shortint olCalendarView = 0x2;
static const Shortint olIconView = 0x3;
static const Shortint olTimelineView = 0x4;

}	/* namespace Opolkxp */
using namespace Opolkxp;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpOlkXP
