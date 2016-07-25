// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpFormStatus.pas' rev: 6.00

#ifndef QRpFormStatusHPP
#define QRpFormStatusHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDefine.hpp>	// Pascal unit
#include <QRpSystem.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <QButtons.hpp>	// Pascal unit
#include <QStdCtrls.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrpformstatus
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRpStatusForm;
class PASCALIMPLEMENTATION TRpStatusForm : public Qforms::TForm 
{
	typedef Qforms::TForm inherited;
	
__published:
	Qstdctrls::TButton* CancelButton;
	Qstdctrls::TLabel* StatusLabel;
	void __fastcall CancelButtonClick(System::TObject* Sender);
	
public:
	Qrpsystem::TRvSystem* ReportSystem;
	bool FormClosed;
public:
	#pragma option push -w-inl
	/* TCustomForm.Create */ inline __fastcall virtual TRpStatusForm(Classes::TComponent* AOwner) : Qforms::TForm(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TRpStatusForm(Classes::TComponent* AOwner, int Dummy) : Qforms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TRpStatusForm(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWidgetControl.CreateParented */ inline __fastcall TRpStatusForm(Qt::QWidgetH* ParentWidget) : Qforms::TForm(ParentWidget) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRpStatusForm* RpStatusForm;

}	/* namespace Qrpformstatus */
using namespace Qrpformstatus;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpFormStatus
