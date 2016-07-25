// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpFormPreview.pas' rev: 6.00

#ifndef QRpFormPreviewHPP
#define QRpFormPreviewHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpFormSetup.hpp>	// Pascal unit
#include <QRpSystem.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <QRpRenderPreview.hpp>	// Pascal unit
#include <QRpRenderPrinter.hpp>	// Pascal unit
#include <QRpFiler.hpp>	// Pascal unit
#include <QRpBase.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <QComCtrls.hpp>	// Pascal unit
#include <QImgList.hpp>	// Pascal unit
#include <QActnList.hpp>	// Pascal unit
#include <QMenus.hpp>	// Pascal unit
#include <QExtCtrls.hpp>	// Pascal unit
#include <QButtons.hpp>	// Pascal unit
#include <QStdCtrls.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <QTypes.hpp>	// Pascal unit
#include <Qt.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrpformpreview
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRavePreviewForm;
class PASCALIMPLEMENTATION TRavePreviewForm : public Qforms::TForm 
{
	typedef Qforms::TForm inherited;
	
__published:
	Qforms::TScrollBox* ScrollBox1;
	Qactnlist::TActionList* ActionList1;
	Qmenus::TMainMenu* MainMenu1;
	Qmenus::TPopupMenu* PopupMenu1;
	Qactnlist::TAction* actnFile_Print;
	Qactnlist::TAction* actnFile_Save;
	Qactnlist::TAction* actnFile_Exit;
	Qactnlist::TAction* actnFile_Open;
	Qcomctrls::TStatusBar* sbarMain;
	Qactnlist::TAction* actnPage_First;
	Qactnlist::TAction* actnPage_Next;
	Qactnlist::TAction* actnPage_Previous;
	Qactnlist::TAction* actnPage_Last;
	Qactnlist::TAction* actnZoom_In;
	Qactnlist::TAction* actnZoom_Out;
	Qactnlist::TAction* actnZoom_PageWidth;
	Qactnlist::TAction* actnZoom_Page;
	Qmenus::TMenuItem* Page1;
	Qmenus::TMenuItem* Zoom1;
	Qmenus::TMenuItem* First1;
	Qmenus::TMenuItem* Next1;
	Qmenus::TMenuItem* Previous1;
	Qmenus::TMenuItem* Last1;
	Qmenus::TMenuItem* N2;
	Qmenus::TMenuItem* GotoPage1;
	Qmenus::TMenuItem* In1;
	Qmenus::TMenuItem* Out1;
	Qmenus::TMenuItem* N3;
	Qmenus::TMenuItem* Page2;
	Qmenus::TMenuItem* PageWidth1;
	Qmenus::TMenuItem* In2;
	Qmenus::TMenuItem* Out2;
	Qimglist::TImageList* ilstActions;
	Qcomctrls::TToolBar* ToolBar1;
	Qcomctrls::TToolButton* ToolButton1;
	Qcomctrls::TToolButton* ToolButton2;
	Qcomctrls::TToolButton* ToolButton3;
	Qcomctrls::TToolButton* ToolButton4;
	Qcomctrls::TToolButton* ToolButton5;
	Qcomctrls::TToolButton* ToolButton6;
	Qcomctrls::TToolButton* ToolButton7;
	Qcomctrls::TToolButton* ToolButton8;
	Qcomctrls::TToolButton* ToolButton10;
	Qcomctrls::TToolButton* ToolButton11;
	Qcomctrls::TToolButton* ToolButton12;
	Qcomctrls::TToolButton* ToolButton13;
	Qcomctrls::TToolButton* ToolButton14;
	Qdialogs::TOpenDialog* dlogOpen;
	Qdialogs::TSaveDialog* dlogSave;
	Qmenus::TMenuItem* File1;
	Qmenus::TMenuItem* Open1;
	Qmenus::TMenuItem* SaveAs1;
	Qmenus::TMenuItem* Print1;
	Qmenus::TMenuItem* N1;
	Qmenus::TMenuItem* Exit1;
	Qstdctrls::TEdit* ZoomEdit;
	Qstdctrls::TEdit* PageEdit;
	Qextctrls::TPanel* PageLabel;
	Qextctrls::TPanel* Panel1;
	Qextctrls::TPanel* Panel2;
	Qcomctrls::TToolButton* ToolButton9;
	Qcomctrls::TToolButton* ToolButton15;
	Qextctrls::TPanel* Panel3;
	Qextctrls::TTimer* PreviewTimer;
	void __fastcall FormClose(System::TObject* Sender, Qforms::TCloseAction &Action);
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
	void __fastcall SetSaveOptions(Qdialogs::TSaveDialog* SaveDialog);
	void __fastcall NewRenderPage(System::TObject* Sender);
	void __fastcall SetButtonState(void);
	
public:
	Qrpsystem::TRvSystem* ReportSystem;
	bool FormClosed;
	Qrprenderprinter::TRvRenderPrinter* RvRenderPrinter;
	Qrprenderpreview::TRvRenderPreview* RvRenderPreview;
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
	/* TCustomForm.Create */ inline __fastcall virtual TRavePreviewForm(Classes::TComponent* AOwner) : Qforms::TForm(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TRavePreviewForm(Classes::TComponent* AOwner, int Dummy) : Qforms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TRavePreviewForm(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWidgetControl.CreateParented */ inline __fastcall TRavePreviewForm(Qt::QWidgetH* ParentWidget) : Qforms::TForm(ParentWidget) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRavePreviewForm* RavePreviewForm;

}	/* namespace Qrpformpreview */
using namespace Qrpformpreview;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpFormPreview
