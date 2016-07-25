// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntDBLogDlg.pas' rev: 6.00

#ifndef TntDBLogDlgHPP
#define TntDBLogDlgHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Forms.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <TntExtCtrls.hpp>	// Pascal unit
#include <TntStdCtrls.hpp>	// Pascal unit
#include <TntForms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntdblogdlg
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TTntLoginDialog;
class PASCALIMPLEMENTATION TTntLoginDialog : public Tntforms::TTntForm 
{
	typedef Tntforms::TTntForm inherited;
	
__published:
	Tntextctrls::TTntPanel* Panel;
	Tntextctrls::TTntBevel* Bevel;
	Tntstdctrls::TTntLabel* DatabaseName;
	Tntstdctrls::TTntButton* OKButton;
	Tntstdctrls::TTntButton* CancelButton;
	Tntextctrls::TTntPanel* Panel1;
	Tntstdctrls::TTntLabel* Label1;
	Tntstdctrls::TTntLabel* Label2;
	Tntstdctrls::TTntLabel* Label3;
	Tntstdctrls::TTntEdit* Password;
	Tntstdctrls::TTntEdit* UserName;
	void __fastcall FormShow(System::TObject* Sender);
public:
	#pragma option push -w-inl
	/* TTntForm.Create */ inline __fastcall virtual TTntLoginDialog(Classes::TComponent* AOwner) : Tntforms::TTntForm(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TTntLoginDialog(Classes::TComponent* AOwner, int Dummy) : Tntforms::TTntForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TTntLoginDialog(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TTntLoginDialog(HWND ParentWindow) : Tntforms::TTntForm(ParentWindow) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool __fastcall TntLoginDialog(const WideString ADatabaseName, WideString &AUserName, WideString &APassword);
extern PACKAGE bool __fastcall TntLoginDialogEx(const WideString ADatabaseName, WideString &AUserName, WideString &APassword, bool NameReadOnly);
extern PACKAGE bool __fastcall TntRemoteLoginDialog(WideString &AUserName, WideString &APassword);

}	/* namespace Tntdblogdlg */
using namespace Tntdblogdlg;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntDBLogDlg
