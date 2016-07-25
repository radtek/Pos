// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpFiler.pas' rev: 6.00

#ifndef QRpFilerHPP
#define QRpFilerHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDevice.hpp>	// Pascal unit
#include <QRpBase.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <QExtCtrls.hpp>	// Pascal unit
#include <QStdCtrls.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrpfiler
{
//-- type declarations -------------------------------------------------------
typedef Byte TOutBuf[16384];

typedef Byte *POutBuf;

class DELPHICLASS TRvNDRWriter;
class PASCALIMPLEMENTATION TRvNDRWriter : public Qrpbase::TBaseReport 
{
	typedef Qrpbase::TBaseReport inherited;
	
protected:
	Qrpdefine::TSaveBrush SaveBrush;
	Qrpdefine::TSaveFont SaveFont;
	Qrpdefine::TSavePen SavePen;
	bool WriteAll;
	Byte *OutBuf;
	int OutPos;
	Byte AllowMode;
	Classes::TNotifyEvent BrushSaveOnChange;
	Classes::TNotifyEvent PenSaveOnChange;
	int GraphicPos[10];
	int LastGraphicPos;
	int ReuseGraphicPos;
	Classes::TStream* OutputStream;
	Classes::TStream* FStream;
	AnsiString FOutputFileName;
	Qrpdefine::TStreamMode FStreamMode;
	Qrpdefine::TAccuracyMethod FAccuracyMethod;
	bool FHoldOutput;
	int PrevPagePos;
	int NextPagePos;
	int SaveJobPos;
	int AbsolutePage;
	Classes::TStringList* VarList;
	bool LocalDeviceOwned;
	Classes::TNotifyEvent FOnRenderPage;
	Classes::TNotifyEvent FOnRenderData;
	int __fastcall FindVarName(AnsiString VarName);
	void __fastcall CreateVarBuf(AnsiString VarName, AnsiString VarData, bool Defined, int PrevPos);
	void __fastcall WriteJobHeader(void);
	void __fastcall FinishJob(void);
	void __fastcall WritePageHeader(void);
	void __fastcall FinishPage(bool LastPage);
	void __fastcall BrushOnChange(System::TObject* Sender);
	virtual void __fastcall FontOnChange(System::TObject* Sender);
	void __fastcall PenOnChange(System::TObject* Sender);
	void __fastcall WriteByte(Byte Value);
	void __fastcall WriteInteger(short Value);
	void __fastcall WriteLongint(int Value);
	void __fastcall WriteText(AnsiString Value);
	void __fastcall WriteFloat(double Value);
	void __fastcall WriteXY(double X, double Y);
	void __fastcall WriteGraphic(Qgraphics::TGraphic* Value);
	void __fastcall WritePoint(const Types::TPoint &Value);
	void __fastcall WriteRect(const Types::TRect &Value);
	void __fastcall Flush(void);
	void __fastcall WriteBuf(void *Buf, int Len);
	void __fastcall WriteBrush(void);
	void __fastcall WriteFont(void);
	void __fastcall WritePen(void);
	virtual void __fastcall SetBKColor(Qgraphics::TColor Value);
	virtual void __fastcall SetScaleX(double Value);
	virtual void __fastcall SetScaleY(double Value);
	virtual void __fastcall SetTextBKMode(Qrpdefine::TBKMode Value);
	virtual void __fastcall SetOrientation(Qrpdefine::TOrientation Value);
	virtual void __fastcall SetFrameMode(Qrpdefine::TFrameMode Value);
	virtual void __fastcall PrintCenterInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintLeftInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintRightInch(AnsiString Text, double Pos);
	virtual void __fastcall PrintBlockInch(AnsiString Text, double Pos, double Width);
	virtual Qrpdefine::TDestination __fastcall Destination(void);
	virtual Qgraphics::TCanvas* __fastcall GetCanvas(void);
	
public:
	__fastcall virtual TRvNDRWriter(Classes::TComponent* AOwner);
	__fastcall virtual ~TRvNDRWriter(void);
	virtual void __fastcall Start(void);
	virtual void __fastcall Finish(void);
	virtual void __fastcall NewPage(void);
	virtual void __fastcall AbortPage(void);
	virtual void __fastcall ResetLineHeight(void);
	virtual bool __fastcall SelectBin(AnsiString BinName);
	virtual void __fastcall CopyRect(const Types::TRect &Dest, Qgraphics::TCanvas* Canvas, const Types::TRect &Source);
	virtual void __fastcall DrawFocusRect(const Types::TRect &Rect);
	virtual void __fastcall FillRect(const Types::TRect &Rect);
	virtual void __fastcall Arc(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	virtual void __fastcall Chord(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	virtual void __fastcall Draw(double X, double Y, Qgraphics::TGraphic* Graphic);
	virtual void __fastcall Ellipse(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall LineTo(double X, double Y);
	virtual void __fastcall MoveTo(double X, double Y);
	virtual void __fastcall Pie(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	virtual void __fastcall Polygon(const Types::TPoint * Points, const int Points_Size);
	virtual void __fastcall Polyline(const Types::TPoint * Points, const int Points_Size);
	virtual void __fastcall PrintBitmap(double X, double Y, double ScaleX, double ScaleY, Qgraphics::TBitmap* Bitmap);
	virtual void __fastcall PrintBitmapRect(double X1, double Y1, double X2, double Y2, Qgraphics::TBitmap* Bitmap);
	virtual void __fastcall PrintImageRect(double X1, double Y1, double X2, double Y2, Classes::TStream* ImageStream, AnsiString ImageType);
	virtual void __fastcall Rectangle(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall RoundRect(double X1, double Y1, double X2, double Y2, double X3, double Y3);
	virtual void __fastcall StretchDraw(const Types::TRect &Rect, Qgraphics::TGraphic* Graphic);
	virtual void __fastcall TabRectangle(double X1, double Y1, double X2, double Y2);
	virtual void __fastcall TextRect(const Types::TRect &Rect, double X, double Y, const AnsiString Text);
	virtual void __fastcall PrintData(AnsiString Value);
	virtual void __fastcall PrintDataStream(Classes::TStream* Stream, int BufSize);
	virtual void __fastcall AllowPreviewOnly(void);
	virtual void __fastcall AllowPrinterOnly(void);
	virtual void __fastcall AllowAll(void);
	virtual void __fastcall UnregisterGraphic(int Index);
	virtual void __fastcall RegisterGraphic(int Index);
	virtual void __fastcall ReuseGraphic(int Index);
	virtual AnsiString __fastcall PIVar(AnsiString VarName);
	virtual void __fastcall SetPIVar(AnsiString VarName, AnsiString VarValue);
	__property Classes::TStream* Stream = {read=FStream, write=FStream};
	__property bool HoldOutput = {read=FHoldOutput, write=FHoldOutput, nodefault};
	__property int JobPages = {read=FJobPages, write=FJobPages, nodefault};
	__property Classes::TNotifyEvent OnRenderPage = {read=FOnRenderPage, write=FOnRenderPage};
	__property Classes::TNotifyEvent OnRenderData = {read=FOnRenderData, write=FOnRenderData};
	
__published:
	__property FirstPage  = {default=1};
	__property LastPage  = {default=9999};
	__property StatusLabel ;
	__property StatusFormat ;
	__property StatusText ;
	__property LineHeightMethod  = {default=1};
	__property LinesPerInch  = {default=6};
	__property Units  = {default=0};
	__property UnitsFactor ;
	__property MarginLeft ;
	__property MarginRight ;
	__property MarginTop ;
	__property MarginBottom ;
	__property TabShade  = {default=0};
	__property Title ;
	__property Copies  = {default=1};
	__property Orientation ;
	__property TextBKMode  = {default=0};
	__property ScaleX ;
	__property ScaleY ;
	__property Qrpdefine::TAccuracyMethod AccuracyMethod = {read=FAccuracyMethod, write=FAccuracyMethod, default=1};
	__property AnsiString FileName = {read=FOutputFileName, write=FOutputFileName};
	__property Qrpdefine::TStreamMode StreamMode = {read=FStreamMode, write=FStreamMode, default=0};
	__property OnPrint ;
	__property OnPrintPage ;
	__property OnBeforePrint ;
	__property OnAfterPrint ;
	__property OnNewPage ;
	__property OnNewColumn ;
	__property OnPrintHeader ;
	__property OnPrintFooter ;
	__property OnEndOfSection ;
};


typedef TRvNDRWriter TReportFiler;
;

class DELPHICLASS TReportRender;
class PASCALIMPLEMENTATION TReportRender : public TRvNDRWriter 
{
	typedef TRvNDRWriter inherited;
	
public:
	__fastcall virtual TReportRender(Classes::TComponent* AOwner);
	__fastcall virtual ~TReportRender(void);
};


//-- var, const, procedure ---------------------------------------------------
static const Word OUTBUFSIZE = 0x4000;
static const Shortint MAXGRAPHICINDEX = 0xa;

}	/* namespace Qrpfiler */
using namespace Qrpfiler;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpFiler
