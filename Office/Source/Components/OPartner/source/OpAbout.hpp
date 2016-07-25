// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpAbout.pas' rev: 6.00

#ifndef OpAboutHPP
#define OpAboutHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ShellAPI.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <DesignIntf.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opabout
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TFrmAbout;
class PASCALIMPLEMENTATION TFrmAbout : public Forms::TForm 
{
	typedef Forms::TForm inherited;
	
__published:
	Extctrls::TPanel* Panel1;
	Extctrls::TImage* Image1;
	Stdctrls::TLabel* lblVersion;
	Stdctrls::TButton* btnOK;
	Stdctrls::TLabel* WebLbl;
	Extctrls::TPanel* Panel2;
	Stdctrls::TLabel* NewsLbl;
	Stdctrls::TLabel* SupportLbl;
	void __fastcall btnOKClick(System::TObject* Sender);
	void __fastcall FormCreate(System::TObject* Sender);
	void __fastcall WebLblMouseMove(System::TObject* Sender, Classes::TShiftState Shift, int X, int Y);
	void __fastcall NewsLblMouseMove(System::TObject* Sender, Classes::TShiftState Shift, int X, int Y);
	void __fastcall Panel2MouseMove(System::TObject* Sender, Classes::TShiftState Shift, int X, int Y);
	void __fastcall FormMouseMove(System::TObject* Sender, Classes::TShiftState Shift, int X, int Y);
	void __fastcall WebLblClick(System::TObject* Sender);
	void __fastcall NewsLblClick(System::TObject* Sender);
public:
	#pragma option push -w-inl
	/* TCustomForm.Create */ inline __fastcall virtual TFrmAbout(Classes::TComponent* AOwner) : Forms::TForm(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TFrmAbout(Classes::TComponent* AOwner, int Dummy) : Forms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TFrmAbout(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TFrmAbout(HWND ParentWindow) : Forms::TForm(ParentWindow) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Opabout */
using namespace Opabout;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpAbout
