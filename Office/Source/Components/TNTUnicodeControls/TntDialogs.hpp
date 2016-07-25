// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntDialogs.pas' rev: 6.00

#ifndef TntDialogsHPP
#define TntDialogsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <TntControls.hpp>	// Pascal unit
#include <TntClasses.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <CommDlg.hpp>	// Pascal unit
#include <Dlgs.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntdialogs
{
//-- type declarations -------------------------------------------------------
typedef void __fastcall (__closure *TIncludeItemEventW)(const _OFNOTIFYEXW &OFN, bool &Include);

class DELPHICLASS TTntOpenDialog;
class PASCALIMPLEMENTATION TTntOpenDialog : public Dialogs::TOpenDialog 
{
	typedef Dialogs::TOpenDialog inherited;
	
private:
	WideString FDefaultExt;
	WideString FFileName;
	WideString FFilter;
	WideString FInitialDir;
	WideString FTitle;
	Tntclasses::TTntStrings* FFiles;
	TIncludeItemEventW FOnIncludeItem;
	WideString __fastcall GetDefaultExt();
	void __fastcall SetInheritedDefaultExt(const AnsiString Value);
	void __fastcall SetDefaultExt(const WideString Value);
	HIDESBASE WideString __fastcall GetFileName();
	void __fastcall SetFileName(const WideString Value);
	WideString __fastcall GetFilter();
	void __fastcall SetInheritedFilter(const AnsiString Value);
	void __fastcall SetFilter(const WideString Value);
	WideString __fastcall GetInitialDir();
	void __fastcall SetInheritedInitialDir(const AnsiString Value);
	HIDESBASE void __fastcall SetInitialDir(const WideString Value);
	WideString __fastcall GetTitle();
	void __fastcall SetInheritedTitle(const AnsiString Value);
	void __fastcall SetTitle(const WideString Value);
	Tntclasses::TTntStrings* __fastcall GetFiles(void);
	#pragma pack(push, 1)
	tagOFNA FProxiedOpenFilenameA;
	#pragma pack(pop)
	
	
protected:
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	bool __fastcall CanCloseW(tagOFNW &OpenFileName);
	void __fastcall GetFileNamesW(tagOFNW &OpenFileName);
	DYNAMIC void __fastcall DoIncludeItem(const Commdlg::TOFNotifyEx &OFN, bool &Include);
	virtual void __fastcall WndProc(Messages::TMessage &Message);
	BOOL __fastcall DoExecuteW(void * Func);
	
public:
	__fastcall virtual TTntOpenDialog(Classes::TComponent* AOwner);
	__fastcall virtual ~TTntOpenDialog(void);
	virtual bool __fastcall Execute(void);
	__property Tntclasses::TTntStrings* Files = {read=GetFiles};
	
__published:
	__property WideString DefaultExt = {read=GetDefaultExt, write=SetDefaultExt};
	__property WideString FileName = {read=GetFileName, write=SetFileName};
	__property WideString Filter = {read=GetFilter, write=SetFilter};
	__property WideString InitialDir = {read=GetInitialDir, write=SetInitialDir};
	__property WideString Title = {read=GetTitle, write=SetTitle};
	__property TIncludeItemEventW OnIncludeItem = {read=FOnIncludeItem, write=FOnIncludeItem};
};


class DELPHICLASS TTntSaveDialog;
class PASCALIMPLEMENTATION TTntSaveDialog : public TTntOpenDialog 
{
	typedef TTntOpenDialog inherited;
	
public:
	virtual bool __fastcall Execute(void);
public:
	#pragma option push -w-inl
	/* TTntOpenDialog.Create */ inline __fastcall virtual TTntSaveDialog(Classes::TComponent* AOwner) : TTntOpenDialog(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TTntOpenDialog.Destroy */ inline __fastcall virtual ~TTntSaveDialog(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool __fastcall WideInputQuery(const WideString ACaption, const WideString APrompt, WideString &Value);
extern PACKAGE bool __fastcall WideSelectDirectory(const WideString Caption, const WideString Root, WideString &Directory);

}	/* namespace Tntdialogs */
using namespace Tntdialogs;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntDialogs
