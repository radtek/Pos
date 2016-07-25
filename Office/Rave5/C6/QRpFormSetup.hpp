// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpFormSetup.pas' rev: 6.00

#ifndef QRpFormSetupHPP
#define QRpFormSetupHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpRenderPrinter.hpp>	// Pascal unit
#include <QRpSystem.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <QRpDevice.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <QComCtrls.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QButtons.hpp>	// Pascal unit
#include <QStdCtrls.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrpformsetup
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRPSetupForm;
class PASCALIMPLEMENTATION TRPSetupForm : public Qforms::TForm 
{
	typedef Qforms::TForm inherited;
	
__published:
	Qstdctrls::TGroupBox* DestGB;
	Qstdctrls::TRadioButton* PrinterRB;
	Qstdctrls::TRadioButton* PreviewRB;
	Qstdctrls::TRadioButton* FileRB;
	Qstdctrls::TEdit* editFileName;
	Qstdctrls::TButton* bbtnOK;
	Qstdctrls::TButton* CancelBB;
	Qstdctrls::TButton* SetupBB;
	Qstdctrls::TGroupBox* RangeGB;
	Qbuttons::TSpeedButton* FileNameSB;
	Qdialogs::TSaveDialog* dlogSave;
	Qstdctrls::TRadioButton* AllRB;
	Qstdctrls::TRadioButton* SelectionRB;
	Qstdctrls::TRadioButton* PagesRB;
	Qstdctrls::TLabel* FromLabel;
	Qstdctrls::TEdit* FromED;
	Qstdctrls::TLabel* ToLabel;
	Qstdctrls::TEdit* ToED;
	Qstdctrls::TLabel* SelectionLabel;
	Qstdctrls::TEdit* SelectionED;
	Qstdctrls::TGroupBox* GroupBox1;
	Qstdctrls::TLabel* PrinterLabel;
	Qstdctrls::TGroupBox* GroupBox2;
	Qstdctrls::TEdit* CopiesED;
	Qstdctrls::TLabel* CopiesLabel;
	Qstdctrls::TCheckBox* CollateCK;
	Qstdctrls::TCheckBox* DuplexCK;
	Qcomctrls::TPageControl* PageControl1;
	Qcomctrls::TTabSheet* TabSheet1;
	Qcomctrls::TTabSheet* TabSheet2;
	Qstdctrls::TComboBox* cboxFormat;
	Qstdctrls::TLabel* Label1;
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
	Qrpsystem::TRvSystem* ReportSystem;
	Qrprenderprinter::TRvRenderPrinter* FilePrinter;
	bool PreviewSetup;
public:
	#pragma option push -w-inl
	/* TCustomForm.Create */ inline __fastcall virtual TRPSetupForm(Classes::TComponent* AOwner) : Qforms::TForm(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TRPSetupForm(Classes::TComponent* AOwner, int Dummy) : Qforms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TRPSetupForm(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWidgetControl.CreateParented */ inline __fastcall TRPSetupForm(Qt::QWidgetH* ParentWidget) : Qforms::TForm(ParentWidget) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRPSetupForm* RPSetupForm;

}	/* namespace Qrpformsetup */
using namespace Qrpformsetup;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpFormSetup
