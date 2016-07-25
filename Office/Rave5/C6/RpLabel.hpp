// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpLabel.pas' rev: 6.00

#ifndef RpLabelHPP
#define RpLabelHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpShell.hpp>	// Pascal unit
#include <RpSect.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rplabel
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TLabelShape { lsRoundRect, lsRect, lsRound };
#pragma option pop

class DELPHICLASS TLabelShell;
typedef void __fastcall (__closure *TLabelEvent)(Rpbase::TBaseReport* ReportPrinter, TLabelShell* LabelShell);

typedef void __fastcall (__closure *TLabelValidEvent)(Rpbase::TBaseReport* ReportPrinter, TLabelShell* LabelShell, bool &Valid);

#pragma option push -b-
enum TLabelBrand { lbAV5167, lbAV5267, lbAV5927, lbAV5160, lbAV5260, lbAV5920, lbAV5161, lbAV5261, lbAV5162, lbAV5262, lbAV5922, lbAV5163, lbAV5263, lbAV5923, lbAV5963, lbAV5164, lbAV5264, lbAV5667, lbAV5660, lbAV5930, lbAV5661, lbAV5662, lbAV5932, lbAV5663, lbAV5664, lbAV5165, lbAV5265, lbAV5266, lbAV5066, lbAV5766, lbAV5866, lbAV5966, lbAV5366, lbAV5293, lbAV5294, lbAV5295, lbAV5196, lbAV5096, lbAV5896, lbAV5197, lbAV5097, lbAV5897, lbAV5198, lbAV5199FACE, lbAV5199SPINE, lbAV5395, lbAV5095, lbAV5895BADGE, lbAV5895SPINE, lbAV5371, lbAV5376, lbAV5377, lbAV5372, lbAV5911, lbAV5383, lbAV5390, lbAV5883, lbAV5384, lbAV5392, lbAV5385, lbAV5386, lbAV5388, lbAV5389, lbAV12_294, lbAV12_295, lbAV12_296, lbAV12_297, lbAV5305, lbAV5309, lbAV5315, lbAV5361, lbAV5362, lbAV5361SINGLE, lbAV5362SINGLE, lbAV5364, lbAV5364S, lbAV8160, lbAV8460, lbAV8920, lbAV8161, lbAV8162, lbAV8462, lbAV8922, lbAV8163, lbAV8463, lbAV8923, lbAV8164, lbAV8165, lbAV8166, lbAV8167, lbAV8927, lbAV8196, lbAV8315, lbAV8316PORTRAIT
	, lbAV8316LANDSCAPE, lbAV8371, lbAV8376, lbAV8377, lbAV8372, lbAV8660, lbAV8930, lbAV8662, lbAV8932, lbAV8663, lbAV6460, lbAV6464, lbAV6466, lbAV6490, lbAV2160, lbAV2162, lbAV2163, lbAV2660, lbAV2662, lbAV2663, lbAV2180, lbAV2181, lbAV2186, lbAV5824, lbAV5836LABEL, lbAV5836TAB, lbAV5837LABEL, lbAV5837TAB, lbAV5838LABEL, lbAV5838TAB, lbAV5848LABEL, lbAV5848TAB, lbAV5849LABEL, lbAV5849TAB, lbAV5855LABEL, lbAV5855TAB, lbAV5839, lbAV5840, lbAV5841, lbAV5842, lbAV5843, lbAV5844, lbAV5845, lbCustom };
#pragma option pop

class PASCALIMPLEMENTATION TLabelShell : public Rpshell::TBaseShell 
{
	typedef Rpshell::TBaseShell inherited;
	
protected:
	int FNumAcross;
	int FNumDown;
	double FLabelWidth;
	double FLabelHeight;
	double FSpacingLeft;
	double FSpacingTop;
	double FSpacingWidth;
	double FSpacingHeight;
	bool FDrawExtents;
	Graphics::TPen* FDrawPen;
	bool FDrawPreviewOnly;
	bool FPrintByRow;
	TLabelBrand FLabelBrand;
	double FBorder;
	TLabelShape FLabelShape;
	int FRow;
	int FCol;
	int FStartRow;
	int FStartCol;
	int FSkipNum;
	TLabelEvent FOnReportBefore;
	TLabelEvent FOnPageBefore;
	TLabelValidEvent FOnLabelBefore;
	TLabelValidEvent FOnLabelPrint;
	TLabelValidEvent FOnLabelAfter;
	TLabelEvent FOnPageAfter;
	TLabelEvent FOnReportAfter;
	virtual void __fastcall ReportBeforeEvent(void);
	virtual void __fastcall PageBeforeEvent(void);
	virtual void __fastcall LabelBeforeEvent(bool &Valid);
	virtual void __fastcall LabelPrintEvent(bool &Valid);
	virtual void __fastcall LabelAfterEvent(bool &Valid);
	virtual void __fastcall PageAfterEvent(void);
	virtual void __fastcall ReportAfterEvent(void);
	void __fastcall SetDrawPen(Graphics::TPen* Value);
	void __fastcall SetLabelBrand(TLabelBrand Value);
	void __fastcall SetLabelDims(double MT, double ML, double SH, double SW, double LH, double LW, int LA, int LD);
	void __fastcall SetNumAcross(int Value);
	void __fastcall SetNumDown(int Value);
	void __fastcall SetLabelWidth(double Value);
	void __fastcall SetLabelHeight(double Value);
	void __fastcall SetSpacingLeft(double Value);
	void __fastcall SetSpacingTop(double Value);
	void __fastcall SetSpacingWidth(double Value);
	void __fastcall SetSpacingHeight(double Value);
	void __fastcall SetLabelShape(TLabelShape Value);
	void __fastcall BeginPage(System::TObject* Sender);
	void __fastcall EndPage(System::TObject* Sender);
	bool __fastcall PrintLabel(int Row, int Col);
	void __fastcall PrintReport(void);
	
public:
	__fastcall virtual TLabelShell(Classes::TComponent* AOwner);
	__fastcall virtual ~TLabelShell(void);
	virtual void __fastcall Execute(Rpbase::TBaseReport* Report);
	__property int Row = {read=FRow, nodefault};
	__property int Col = {read=FCol, nodefault};
	__property int StartRow = {read=FStartRow, write=FStartRow, nodefault};
	__property int StartCol = {read=FStartCol, write=FStartCol, nodefault};
	__property int SkipNum = {read=FSkipNum, write=FSkipNum, nodefault};
	
__published:
	__property int NumAcross = {read=FNumAcross, write=SetNumAcross, default=3};
	__property int NumDown = {read=FNumDown, write=SetNumDown, default=10};
	__property double LabelWidth = {read=FLabelWidth, write=SetLabelWidth};
	__property double LabelHeight = {read=FLabelHeight, write=SetLabelHeight};
	__property double SpacingLeft = {read=FSpacingLeft, write=SetSpacingLeft};
	__property double SpacingTop = {read=FSpacingTop, write=SetSpacingTop};
	__property double SpacingWidth = {read=FSpacingWidth, write=SetSpacingWidth};
	__property double SpacingHeight = {read=FSpacingHeight, write=SetSpacingHeight};
	__property bool DrawExtents = {read=FDrawExtents, write=FDrawExtents, default=0};
	__property Graphics::TPen* DrawPen = {read=FDrawPen, write=SetDrawPen};
	__property bool DrawPreviewOnly = {read=FDrawPreviewOnly, write=FDrawPreviewOnly, default=1};
	__property bool PrintByRow = {read=FPrintByRow, write=FPrintByRow, default=1};
	__property TLabelBrand LabelBrand = {read=FLabelBrand, write=SetLabelBrand, default=137};
	__property double Border = {read=FBorder, write=FBorder};
	__property TLabelShape LabelShape = {read=FLabelShape, write=SetLabelShape, default=0};
	__property TLabelEvent OnReportBefore = {read=FOnReportBefore, write=FOnReportBefore};
	__property TLabelEvent OnPageBefore = {read=FOnPageBefore, write=FOnPageBefore};
	__property TLabelValidEvent OnLabelBefore = {read=FOnLabelBefore, write=FOnLabelBefore};
	__property TLabelValidEvent OnLabelPrint = {read=FOnLabelPrint, write=FOnLabelPrint};
	__property TLabelValidEvent OnLabelAfter = {read=FOnLabelAfter, write=FOnLabelAfter};
	__property TLabelEvent OnPageAfter = {read=FOnPageAfter, write=FOnPageAfter};
	__property TLabelEvent OnReportAfter = {read=FOnReportAfter, write=FOnReportAfter};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rplabel */
using namespace Rplabel;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpLabel
