// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntExtDlgs.pas' rev: 6.00

#ifndef TntExtDlgsHPP
#define TntExtDlgsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Dialogs.hpp>	// Pascal unit
#include <TntButtons.hpp>	// Pascal unit
#include <TntStdCtrls.hpp>	// Pascal unit
#include <TntExtCtrls.hpp>	// Pascal unit
#include <TntDialogs.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntextdlgs
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TTntOpenPictureDialog;
class PASCALIMPLEMENTATION TTntOpenPictureDialog : public Tntdialogs::TTntOpenDialog 
{
	typedef Tntdialogs::TTntOpenDialog inherited;
	
private:
	Tntextctrls::TTntPanel* FPicturePanel;
	Tntstdctrls::TTntLabel* FPictureLabel;
	Tntbuttons::TTntSpeedButton* FPreviewButton;
	Tntextctrls::TTntPanel* FPaintPanel;
	Tntextctrls::TTntImage* FImageCtrl;
	WideString FSavedFilename;
	bool __fastcall IsFilterStored(void);
	void __fastcall PreviewKeyPress(System::TObject* Sender, char &Key);
	
protected:
	virtual void __fastcall PreviewClick(System::TObject* Sender);
	DYNAMIC void __fastcall DoClose(void);
	DYNAMIC void __fastcall DoSelectionChange(void);
	DYNAMIC void __fastcall DoShow(void);
	__property Tntextctrls::TTntImage* ImageCtrl = {read=FImageCtrl};
	__property Tntstdctrls::TTntLabel* PictureLabel = {read=FPictureLabel};
	
__published:
	__property Filter  = {stored=IsFilterStored};
	
public:
	__fastcall virtual TTntOpenPictureDialog(Classes::TComponent* AOwner);
	virtual bool __fastcall Execute(void);
public:
	#pragma option push -w-inl
	/* TTntOpenDialog.Destroy */ inline __fastcall virtual ~TTntOpenPictureDialog(void) { }
	#pragma option pop
	
};


class DELPHICLASS TTntSavePictureDialog;
class PASCALIMPLEMENTATION TTntSavePictureDialog : public TTntOpenPictureDialog 
{
	typedef TTntOpenPictureDialog inherited;
	
public:
	virtual bool __fastcall Execute(void);
public:
	#pragma option push -w-inl
	/* TTntOpenPictureDialog.Create */ inline __fastcall virtual TTntSavePictureDialog(Classes::TComponent* AOwner) : TTntOpenPictureDialog(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TTntOpenDialog.Destroy */ inline __fastcall virtual ~TTntSavePictureDialog(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Tntextdlgs */
using namespace Tntextdlgs;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntExtDlgs
