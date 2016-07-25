// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntStrEdit_Design.pas' rev: 6.00

#ifndef TntStrEdit_DesignHPP
#define TntStrEdit_DesignHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <TntDialogs.hpp>	// Pascal unit
#include <TntClasses.hpp>	// Pascal unit
#include <TntExtCtrls.hpp>	// Pascal unit
#include <TntMenus.hpp>	// Pascal unit
#include <TntForms.hpp>	// Pascal unit
#include <DesignIntf.hpp>	// Pascal unit
#include <DesignEditors.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <TntStdCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Buttons.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntstredit_design
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TTntStrEditDlg;
class PASCALIMPLEMENTATION TTntStrEditDlg : public Tntforms::TTntForm 
{
	typedef Tntforms::TTntForm inherited;
	
__published:
	Tntstdctrls::TTntButton* CodeWndBtn;
	Tntdialogs::TTntOpenDialog* OpenDialog;
	Tntdialogs::TTntSaveDialog* SaveDialog;
	Tntstdctrls::TTntButton* HelpButton;
	Tntstdctrls::TTntButton* OKButton;
	Tntstdctrls::TTntButton* CancelButton;
	Tntmenus::TTntPopupMenu* StringEditorMenu;
	Tntmenus::TTntMenuItem* LoadItem;
	Tntmenus::TTntMenuItem* SaveItem;
	Tntmenus::TTntMenuItem* CodeEditorItem;
	Tntextctrls::TTntBevel* Bevel1;
	Tntstdctrls::TTntMemo* Memo;
	Tntstdctrls::TTntLabel* LineCount;
	Tntstdctrls::TTntLabel* UnicodeEnabledLbl;
	void __fastcall FileOpenClick(System::TObject* Sender);
	void __fastcall FileSaveClick(System::TObject* Sender);
	void __fastcall HelpButtonClick(System::TObject* Sender);
	void __fastcall CodeWndBtnClick(System::TObject* Sender);
	void __fastcall FormCreate(System::TObject* Sender);
	void __fastcall Memo1KeyDown(System::TObject* Sender, Word &Key, Classes::TShiftState Shift);
	void __fastcall UpdateStatus(System::TObject* Sender);
	
private:
	WideString SingleLine;
	WideString MultipleLines;
	
protected:
	bool FModified;
	Tntclasses::TTntStrings* __fastcall GetLines(void);
	void __fastcall SetLines(const Tntclasses::TTntStrings* Value);
	Controls::TWinControl* __fastcall GetLinesControl(void);
	
public:
	__property Tntclasses::TTntStrings* Lines = {read=GetLines, write=SetLines};
	void __fastcall PrepareForWideStringEdit(void);
public:
	#pragma option push -w-inl
	/* TTntForm.Create */ inline __fastcall virtual TTntStrEditDlg(Classes::TComponent* AOwner) : Tntforms::TTntForm(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TTntStrEditDlg(Classes::TComponent* AOwner, int Dummy) : Tntforms::TTntForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TTntStrEditDlg(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TTntStrEditDlg(HWND ParentWindow) : Tntforms::TTntForm(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TWideStringListProperty;
class PASCALIMPLEMENTATION TWideStringListProperty : public Designeditors::TClassProperty 
{
	typedef Designeditors::TClassProperty inherited;
	
protected:
	virtual TTntStrEditDlg* __fastcall EditDialog(void);
	virtual Tntclasses::TTntStrings* __fastcall GetStrings(void);
	virtual void __fastcall SetStrings(const Tntclasses::TTntStrings* Value);
	
public:
	virtual Designintf::TPropertyAttributes __fastcall GetAttributes(void);
	virtual void __fastcall Edit(void);
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TWideStringListProperty(const Designintf::_di_IDesigner ADesigner, int APropCount) : Designeditors::TClassProperty(ADesigner, APropCount) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TWideStringListProperty(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall Register(void);

}	/* namespace Tntstredit_design */
using namespace Tntstredit_design;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntStrEdit_Design
