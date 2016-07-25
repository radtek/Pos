// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpFormSetup.pas' rev: 6.00

#ifndef RpFormSetupHPP
#define RpFormSetupHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpRenderPrinter.hpp>	// Pascal unit
#include <RpSystem.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <RpDevice.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <ComCtrls.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Buttons.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rpformsetup
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRPSetupForm;
class PASCALIMPLEMENTATION TRPSetupForm : public Forms::TForm 
{
	typedef Forms::TForm inherited;
	
__published:
	Stdctrls::TGroupBox* DestGB;
	Stdctrls::TRadioButton* PrinterRB;
	Stdctrls::TRadioButton* PreviewRB;
	Stdctrls::TRadioButton* FileRB;
	Stdctrls::TEdit* editFileName;
	Stdctrls::TButton* bbtnOK;
	Stdctrls::TButton* CancelBB;
	Stdctrls::TButton* SetupBB;
	Stdctrls::TGroupBox* RangeGB;
	Buttons::TSpeedButton* FileNameSB;
	Dialogs::TSaveDialog* dlogSave;
	Stdctrls::TRadioButton* AllRB;
	Stdctrls::TRadioButton* SelectionRB;
	Stdctrls::TRadioButton* PagesRB;
	Stdctrls::TLabel* FromLabel;
	Stdctrls::TEdit* FromED;
	Stdctrls::TLabel* ToLabel;
	Stdctrls::TEdit* ToED;
	Stdctrls::TLabel* SelectionLabel;
	Stdctrls::TEdit* SelectionED;
	Stdctrls::TGroupBox* GroupBox1;
	Stdctrls::TLabel* PrinterLabel;
	Stdctrls::TGroupBox* GroupBox2;
	Stdctrls::TEdit* CopiesED;
	Stdctrls::TLabel* CopiesLabel;
	Stdctrls::TCheckBox* CollateCK;
	Stdctrls::TCheckBox* DuplexCK;
	Comctrls::TPageControl* PageControl1;
	Comctrls::TTabSheet* TabSheet1;
	Comctrls::TTabSheet* TabSheet2;
	Stdctrls::TComboBox* cboxFormat;
	Stdctrls::TLabel* Label1;
	void __fastcall SetupBBClick(System::TObject* Sender);
	void __fastcall FileNameSBClick(System::TObject* Sender);
	void __fastcall FormCreate(System::TObject* Sender);
	void __fastcall PagesRBClick(System::TObject* Sender);
	void __fastcall SelectionRBClick(System::TObject* Sender);
	void __fastcall AllRBClick(System::TObject* Sender);
	void __fastcall PrinterRBClick(System::TObject* Sender);
	void __fastcall FileRBClick(System::TObject* Sender);
	void __fastcall FormShow(System::TObject* Sender);
	void __fastcall bbtnOKClick(System::TObject* Sender);
	void __fastcall editFileNameChange(System::TObject* Sender);
	void __fastcall FormKeyPress(System::TObject* Sender, char &Key);
	void __fastcall SelectionEDKeyPress(System::TObject* Sender, char &Key);
	void __fastcall cboxFormatChange(System::TObject* Sender);
	
private:
	void __fastcall UpdateForm(void);
	void __fastcall GetRenderList(void);
	
public:
	Rpsystem::TRvSystem* ReportSystem;
	Rprenderprinter::TRvRenderPrinter* FilePrinter;
	bool PreviewSetup;
public:
	#pragma option push -w-inl
	/* TCustomForm.Create */ inline __fastcall virtual TRPSetupForm(Classes::TComponent* AOwner) : Forms::TForm(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TRPSetupForm(Classes::TComponent* AOwner, int Dummy) : Forms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TRPSetupForm(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TRPSetupForm(HWND ParentWindow) : Forms::TForm(ParentWindow) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRPSetupForm* RPSetupForm;

}	/* namespace Rpformsetup */
using namespace Rpformsetup;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpFormSetup
