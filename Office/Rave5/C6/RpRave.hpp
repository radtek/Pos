// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpRave.pas' rev: 6.00

#ifndef RpRaveHPP
#define RpRaveHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvDataLink.hpp>	// Pascal unit
#include <RvLESystem.hpp>	// Pascal unit
#include <RvLEModule.hpp>	// Pascal unit
#include <RvDriverDataView.hpp>	// Pascal unit
#include <RvDatabase.hpp>	// Pascal unit
#include <RvCsData.hpp>	// Pascal unit
#include <RvCsDraw.hpp>	// Pascal unit
#include <RvCsRpt.hpp>	// Pascal unit
#include <RvCsStd.hpp>	// Pascal unit
#include <RvDirectDataView.hpp>	// Pascal unit
#include <RvData.hpp>	// Pascal unit
#include <RvCmHuff.hpp>	// Pascal unit
#include <RvUtil.hpp>	// Pascal unit
#include <RvProj.hpp>	// Pascal unit
#include <RvClass.hpp>	// Pascal unit
#include <RvDefine.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rprave
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRvProject;
class PASCALIMPLEMENTATION TRvProject : public Rpdefine::TRpComponent 
{
	typedef Rpdefine::TRpComponent inherited;
	
protected:
	bool FActive;
	AnsiString FProjectFile;
	Rpdefine::TRpComponent* FEngine;
	AnsiString FDLLFile;
	bool FLoadDesigner;
	bool FStoreRAV;
	Classes::TMemoryStream* RaveBlob;
	System::TDateTime FRaveBlobDateTime;
	Rvproj::TRaveProjectManager* FProjMan;
	Rvclass::TRaveContainerControl* ProjOwner;
	Classes::TNotifyEvent FOnCreate;
	Classes::TNotifyEvent FOnDestroy;
	Classes::TNotifyEvent FOnDesignerSave;
	Classes::TNotifyEvent FOnDesignerSaveAs;
	Classes::TNotifyEvent FOnDesignerNew;
	Classes::TNotifyEvent FOnDesignerOpen;
	Classes::TNotifyEvent FOnDesignerShow;
	Classes::TNotifyEvent FOnBeforeOpen;
	Classes::TNotifyEvent FOnAfterOpen;
	Classes::TNotifyEvent FOnBeforeClose;
	Classes::TNotifyEvent FOnAfterClose;
	virtual void __fastcall CreateEvent(void);
	virtual void __fastcall DestroyEvent(void);
	virtual void __fastcall DesignerShowEvent(void);
	virtual void __fastcall BeforeOpenEvent(void);
	virtual void __fastcall AfterOpenEvent(void);
	virtual void __fastcall BeforeCloseEvent(void);
	virtual void __fastcall AfterCloseEvent(void);
	virtual void __fastcall Loaded(void);
	AnsiString __fastcall GetReportDesc();
	AnsiString __fastcall GetReportName();
	AnsiString __fastcall GetReportFullName();
	void __fastcall SetActive(bool Value);
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	void __fastcall SetEngine(Rpdefine::TRpComponent* Value);
	void __fastcall ReadRaveBlobOld(Classes::TReader* Reader);
	void __fastcall ReadRaveBlob(Classes::TStream* Stream);
	void __fastcall WriteRaveBlob(Classes::TStream* Stream);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	Rvproj::TRaveProjectManager* __fastcall GetProjMan(void);
	
public:
	__fastcall virtual TRvProject(Classes::TComponent* AOwner);
	__fastcall virtual ~TRvProject(void);
	void __fastcall New(void);
	/*         class method */ static void __fastcall RegisterRaveUnits(TMetaClass* vmt);
	void __fastcall Open(void);
	void __fastcall Close(void);
	void __fastcall Save(void);
	void __fastcall Execute(void);
	void __fastcall ExecuteReport(AnsiString ReportName);
	bool __fastcall Design(void);
	bool __fastcall DesignReport(AnsiString ReportName);
	void __fastcall GetReportList(Classes::TStrings* ReportList, bool FullNames);
	void __fastcall GetReportCategoryList(Classes::TStrings* ReportList, AnsiString Categories, bool FullNames);
	bool __fastcall SelectReport(AnsiString ReportName, bool FullName);
	void __fastcall ReportDescToMemo(Stdctrls::TCustomMemo* Memo);
	void __fastcall SetParam(AnsiString Param, AnsiString Value);
	AnsiString __fastcall GetParam(AnsiString Param);
	void __fastcall ClearParams(void);
	void __fastcall LoadFromFile(AnsiString FileName);
	void __fastcall LoadFromStream(Classes::TStream* Stream);
	void __fastcall SaveToFile(AnsiString FileName);
	void __fastcall SaveToStream(Classes::TStream* Stream);
	void __fastcall LoadRaveBlob(Classes::TStream* Stream);
	void __fastcall SaveRaveBlob(Classes::TStream* Stream);
	void __fastcall ClearRaveBlob(void);
	void __fastcall SetProjectFile(AnsiString Value);
	__property bool Active = {read=FActive, write=SetActive, nodefault};
	__property Rvproj::TRaveProjectManager* ProjMan = {read=GetProjMan};
	__property System::TDateTime RaveBlobDateTime = {read=FRaveBlobDateTime};
	__property AnsiString ReportDesc = {read=GetReportDesc};
	__property AnsiString ReportName = {read=GetReportName};
	__property AnsiString ReportFullName = {read=GetReportFullName};
	
__published:
	__property AnsiString DLLFile = {read=FDLLFile, write=FDLLFile};
	__property Rpdefine::TRpComponent* Engine = {read=FEngine, write=SetEngine, default=0};
	__property bool LoadDesigner = {read=FLoadDesigner, write=FLoadDesigner, default=0};
	__property AnsiString ProjectFile = {read=FProjectFile, write=SetProjectFile};
	__property bool StoreRAV = {read=FStoreRAV, write=FStoreRAV, stored=false, nodefault};
	__property Classes::TNotifyEvent OnCreate = {read=FOnCreate, write=FOnCreate};
	__property Classes::TNotifyEvent OnDestroy = {read=FOnDestroy, write=FOnDestroy};
	__property Classes::TNotifyEvent OnDesignerSave = {read=FOnDesignerSave, write=FOnDesignerSave};
	__property Classes::TNotifyEvent OnDesignerSaveAs = {read=FOnDesignerSaveAs, write=FOnDesignerSaveAs};
	__property Classes::TNotifyEvent OnDesignerShow = {read=FOnDesignerShow, write=FOnDesignerShow};
	__property Classes::TNotifyEvent OnBeforeOpen = {read=FOnBeforeOpen, write=FOnBeforeOpen};
	__property Classes::TNotifyEvent OnAfterOpen = {read=FOnAfterOpen, write=FOnAfterOpen};
	__property Classes::TNotifyEvent OnBeforeClose = {read=FOnBeforeClose, write=FOnBeforeClose};
	__property Classes::TNotifyEvent OnAfterClose = {read=FOnAfterClose, write=FOnAfterClose};
};


typedef void __stdcall (*TRaveCreateProc)(void * RVInfo, unsigned AppHandle);

typedef void __fastcall (*TRaveProc)(void);

typedef void __stdcall (*TRaveDesignerCreateProc)(void * Data, Rvdefine::TRaveSaveCallbackProc SaveCallback, Rvdefine::TRaveSaveCallbackProc SaveAsCallback);

typedef void __stdcall (*TRaveDesignerLoadProc)(void * Buf, int Size);

typedef bool __stdcall (*TRaveDesignerSaveProc)(void * &Buf, int &Size);

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRaveCreateProc RaveCreate;
extern PACKAGE TRaveDesignerCreateProc DesignerCreate;
extern PACKAGE TRaveDesignerLoadProc DesignerLoad;
extern PACKAGE TRaveProc DesignerShow;
extern PACKAGE TRaveDesignerSaveProc DesignerSave;
extern PACKAGE TRaveProc DesignerFree;
extern PACKAGE TRaveProc RaveFree;
extern PACKAGE unsigned __fastcall LoadRaveDLL(AnsiString DLLName);
extern PACKAGE void __fastcall CloseRaveDLL(void);

}	/* namespace Rprave */
using namespace Rprave;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpRave
