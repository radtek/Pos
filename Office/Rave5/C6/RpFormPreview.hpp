// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpFormPreview.pas' rev: 6.00

#ifndef RpFormPreviewHPP
#define RpFormPreviewHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpFormSetup.hpp>	// Pascal unit
#include <RpSystem.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <RpRenderPreview.hpp>	// Pascal unit
#include <RpRenderPrinter.hpp>	// Pascal unit
#include <RpFiler.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <ToolWin.hpp>	// Pascal unit
#include <ComCtrls.hpp>	// Pascal unit
#include <ImgList.hpp>	// Pascal unit
#include <ActnList.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
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

namespace Rpformpreview
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRavePreviewForm;
class PASCALIMPLEMENTATION TRavePreviewForm : public Forms::TForm 
{
	typedef Forms::TForm inherited;
	
__published:
	Forms::TScrollBox* ScrollBox1;
	Actnlist::TActionList* ActionList1;
	Menus::TMainMenu* MainMenu1;
	Menus::TPopupMenu* PopupMenu1;
	Actnlist::TAction* actnFile_Print;
	Actnlist::TAction* actnFile_Save;
	Actnlist::TAction* actnFile_Exit;
	Actnlist::TAction* actnFile_Open;
	Comctrls::TStatusBar* sbarMain;
	Actnlist::TAction* actnPage_First;
	Actnlist::TAction* actnPage_Next;
	Actnlist::TAction* actnPage_Previous;
	Actnlist::TAction* actnPage_Last;
	Actnlist::TAction* actnZoom_In;
	Actnlist::TAction* actnZoom_Out;
	Actnlist::TAction* actnZoom_PageWidth;
	Actnlist::TAction* actnZoom_Page;
	Menus::TMenuItem* Page1;
	Menus::TMenuItem* Zoom1;
	Menus::TMenuItem* First1;
	Menus::TMenuItem* Next1;
	Menus::TMenuItem* Previous1;
	Menus::TMenuItem* Last1;
	Menus::TMenuItem* N2;
	Menus::TMenuItem* GotoPage1;
	Menus::TMenuItem* In1;
	Menus::TMenuItem* Out1;
	Menus::TMenuItem* N3;
	Menus::TMenuItem* Page2;
	Menus::TMenuItem* PageWidth1;
	Menus::TMenuItem* In2;
	Menus::TMenuItem* Out2;
	Controls::TImageList* ilstActions;
	Comctrls::TToolBar* ToolBar1;
	Comctrls::TToolButton* ToolButton1;
	Comctrls::TToolButton* ToolButton2;
	Comctrls::TToolButton* ToolButton3;
	Comctrls::TToolButton* ToolButton4;
	Comctrls::TToolButton* ToolButton5;
	Comctrls::TToolButton* ToolButton6;
	Comctrls::TToolButton* ToolButton7;
	Comctrls::TToolButton* ToolButton8;
	Comctrls::TToolButton* ToolButton10;
	Comctrls::TToolButton* ToolButton11;
	Comctrls::TToolButton* ToolButton12;
	Comctrls::TToolButton* ToolButton13;
	Comctrls::TToolButton* ToolButton14;
	Dialogs::TOpenDialog* dlogOpen;
	Dialogs::TSaveDialog* dlogSave;
	Menus::TMenuItem* File1;
	Menus::TMenuItem* Open1;
	Menus::TMenuItem* SaveAs1;
	Menus::TMenuItem* Print1;
	Menus::TMenuItem* N1;
	Menus::TMenuItem* Exit1;
	Stdctrls::TEdit* ZoomEdit;
	Stdctrls::TEdit* PageEdit;
	Extctrls::TPanel* PageLabel;
	Extctrls::TPanel* Panel1;
	Extctrls::TPanel* Panel2;
	Comctrls::TToolButton* ToolButton9;
	Comctrls::TToolButton* ToolButton15;
	Extctrls::TPanel* Panel3;
	Extctrls::TTimer* PreviewTimer;
	void __fastcall FormClose(System::TObject* Sender, Forms::TCloseAction &Action);
	void __fastcall NDRPreviewPageChange(System::TObject* Sender);
	void __fastcall NDRPreviewZoomChange(System::TObject* Sender);
	void __fastcall FormKeyDown(System::TObject* Sender, Word &Key, Classes::TShiftState Shift);
	void __fastcall FormKeyPress(System::TObject* Sender, char &Key);
	void __fastcall actnFile_ExitExecute(System::TObject* Sender);
	void __fastcall actnPage_GotoPageExecute(System::TObject* Sender);
	void __fastcall actnZoom_InExecute(System::TObject* Sender);
	void __fastcall actnZoom_OutExecute(System::TObject* Sender);
	void __fastcall actnPage_PreviousExecute(System::TObject* Sender);
	void __fastcall actnPage_NextExecute(System::TObject* Sender);
	void __fastcall actnZoom_PageWidthExecute(System::TObject* Sender);
	void __fastcall actnZoom_PageExecute(System::TObject* Sender);
	void __fastcall actnFile_PrintExecute(System::TObject* Sender);
	void __fastcall actnPage_FirstExecute(System::TObject* Sender);
	void __fastcall actnPage_LastExecute(System::TObject* Sender);
	void __fastcall actnPage_FirstUpdate(System::TObject* Sender);
	void __fastcall actnPage_PreviousUpdate(System::TObject* Sender);
	void __fastcall actnPage_NextUpdate(System::TObject* Sender);
	void __fastcall actnPage_LastUpdate(System::TObject* Sender);
	void __fastcall FormCreate(System::TObject* Sender);
	void __fastcall actnFile_SaveExecute(System::TObject* Sender);
	void __fastcall actnFile_OpenExecute(System::TObject* Sender);
	void __fastcall actnFile_PrintUpdate(System::TObject* Sender);
	void __fastcall actnFile_SaveUpdate(System::TObject* Sender);
	void __fastcall actnZoom_InUpdate(System::TObject* Sender);
	void __fastcall actnZoom_OutUpdate(System::TObject* Sender);
	void __fastcall actnZoom_PageWidthUpdate(System::TObject* Sender);
	void __fastcall actnZoom_PageUpdate(System::TObject* Sender);
	void __fastcall ZoomEditKeyPress(System::TObject* Sender, char &Key);
	void __fastcall ZoomEditExit(System::TObject* Sender);
	void __fastcall PageEditExit(System::TObject* Sender);
	void __fastcall PageEditKeyPress(System::TObject* Sender, char &Key);
	void __fastcall PreviewTimerTimer(System::TObject* Sender);
	void __fastcall FormDestroy(System::TObject* Sender);
	void __fastcall FormShow(System::TObject* Sender);
	
protected:
	int FPageNum;
	Classes::TNotifyEvent FOnGenerate;
	AnsiString FInputFileName;
	Classes::TStream* FInputStream;
	Classes::TNotifyEvent FOnAfterPreviewPrint;
	virtual void __fastcall AfterPreviewPrintEvent(void);
	void __fastcall SetSaveOptions(Dialogs::TSaveDialog* SaveDialog);
	void __fastcall NewRenderPage(System::TObject* Sender);
	void __fastcall SetButtonState(void);
	
public:
	Rpsystem::TRvSystem* ReportSystem;
	bool FormClosed;
	Rprenderprinter::TRvRenderPrinter* RvRenderPrinter;
	Rprenderpreview::TRvRenderPreview* RvRenderPreview;
	void __fastcall InitFromRPSystem(void);
	void __fastcall OpenFile(const AnsiString AFileName);
	/*         class method */ static void __fastcall PreviewFile(TMetaClass* vmt, const AnsiString AFileName);
	__property int PageNum = {read=FPageNum, write=FPageNum, nodefault};
	__property Classes::TNotifyEvent OnGenerate = {read=FOnGenerate, write=FOnGenerate};
	__property Classes::TNotifyEvent OnAfterPreviewPrint = {read=FOnAfterPreviewPrint, write=FOnAfterPreviewPrint};
	__property AnsiString InputFileName = {read=FInputFileName, write=FInputFileName};
	__property Classes::TStream* InputStream = {read=FInputStream, write=FInputStream};
public:
	#pragma option push -w-inl
	/* TCustomForm.Create */ inline __fastcall virtual TRavePreviewForm(Classes::TComponent* AOwner) : Forms::TForm(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TRavePreviewForm(Classes::TComponent* AOwner, int Dummy) : Forms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TRavePreviewForm(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TRavePreviewForm(HWND ParentWindow) : Forms::TForm(ParentWindow) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRavePreviewForm* RavePreviewForm;

}	/* namespace Rpformpreview */
using namespace Rpformpreview;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpFormPreview
