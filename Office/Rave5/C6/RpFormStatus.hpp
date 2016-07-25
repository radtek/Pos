// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpFormStatus.pas' rev: 6.00

#ifndef RpFormStatusHPP
#define RpFormStatusHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpDefine.hpp>	// Pascal unit
#include <RpSystem.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Buttons.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rpformstatus
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRpStatusForm;
class PASCALIMPLEMENTATION TRpStatusForm : public Forms::TForm 
{
	typedef Forms::TForm inherited;
	
__published:
	Stdctrls::TButton* CancelButton;
	Stdctrls::TLabel* StatusLabel;
	void __fastcall CancelButtonClick(System::TObject* Sender);
	
public:
	Rpsystem::TRvSystem* ReportSystem;
	bool FormClosed;
public:
	#pragma option push -w-inl
	/* TCustomForm.Create */ inline __fastcall virtual TRpStatusForm(Classes::TComponent* AOwner) : Forms::TForm(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TRpStatusForm(Classes::TComponent* AOwner, int Dummy) : Forms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TRpStatusForm(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TRpStatusForm(HWND ParentWindow) : Forms::TForm(ParentWindow) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRpStatusForm* RpStatusForm;

}	/* namespace Rpformstatus */
using namespace Rpformstatus;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpFormStatus
