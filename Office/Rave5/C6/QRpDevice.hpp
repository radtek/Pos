// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpDevice.pas' rev: 6.00

#ifndef QRpDeviceHPP
#define QRpDeviceHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDefine.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Qt.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <CommDlg.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <WinSpool.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrpdevice
{
//-- type declarations -------------------------------------------------------
typedef HDC TDCHandle;

#pragma option push -b-
enum TPrintDialogOption { poPrintToFile, poPageNums, poSelection, poWarning, poHelp, poDisablePrintToFile };
#pragma option pop

typedef Set<TPrintDialogOption, poPrintToFile, poDisablePrintToFile>  TPrintDialogOptions;

#pragma option push -b-
enum TPrintRange { prAllPages, prSelection, prPageNums };
#pragma option pop

#pragma pack(push, 4)
struct TPrintDialogRec
{
	bool Collate;
	int Copies;
	int FromPage;
	int ToPage;
	int MinPage;
	int MaxPage;
	TPrintDialogOptions Options;
	bool PrintToFile;
	TPrintRange PrintRange;
} ;
#pragma pack(pop)

#pragma pack(push, 4)
struct TBrushBuf
{
	Qgraphics::TColor Color;
	Qgraphics::TBrushStyle Style;
	Qgraphics::TBitmap* Bitmap;
} ;
#pragma pack(pop)

#pragma pack(push, 4)
struct TFontBuf
{
	AnsiString Name;
	int Size;
	Qgraphics::TColor Color;
	Qgraphics::TFontStyles Style;
} ;
#pragma pack(pop)

#pragma pack(push, 4)
struct TPenBuf
{
	Qgraphics::TPenMode PenMode;
	Qgraphics::TPenStyle Style;
	int Width;
	Qgraphics::TColor Color;
} ;
#pragma pack(pop)

class DELPHICLASS TRPBaseDevice;
class PASCALIMPLEMENTATION TRPBaseDevice : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	AnsiString FTitle;
	bool FPrinting;
	bool FAborted;
	Qrpdefine::TOrientation FOrientation;
	int FCopies;
	Qrpdefine::TDuplex FDuplex;
	bool FCollate;
	int FPageHeight;
	int FPageWidth;
	AnsiString FBin;
	AnsiString FPaper;
	virtual Qgraphics::TCanvas* __fastcall GetCanvas(void) = 0 ;
	virtual int __fastcall GetXDPI(void);
	virtual int __fastcall GetYDPI(void);
	virtual Types::TRect __fastcall GetWaste();
	virtual Classes::TStrings* __fastcall GetBins(void);
	virtual Classes::TStrings* __fastcall GetPapers(void);
	virtual Classes::TStrings* __fastcall GetFonts(void);
	virtual Classes::TStrings* __fastcall GetPrinters(void);
	virtual Qrpdefine::TOrientation __fastcall GetOrientation(void);
	virtual void __fastcall SetOrientation(Qrpdefine::TOrientation Value);
	virtual int __fastcall GetMaxCopies(void);
	virtual void __fastcall SetCopies(int Value);
	virtual int __fastcall GetCopies(void);
	virtual void __fastcall SetDuplex(Qrpdefine::TDuplex Value);
	virtual Qrpdefine::TDuplex __fastcall GetDuplex(void);
	virtual void __fastcall SetCollate(bool Value);
	virtual bool __fastcall GetCollate(void);
	virtual int __fastcall GetPageHeight(void);
	virtual int __fastcall GetPageWidth(void);
	
public:
	__fastcall TRPBaseDevice(void);
	virtual int __fastcall TextWidth(AnsiString Value);
	virtual void __fastcall RawOut(void *ABuffer, Word ALength);
	virtual void __fastcall BeginDoc(void);
	virtual void __fastcall BeginDocSelect(void);
	virtual void __fastcall EndDoc(void);
	virtual void __fastcall Abort(void);
	virtual void __fastcall NewPage(void);
	virtual void __fastcall NewPageSelect(bool PageValid);
	virtual bool __fastcall SelectBin(AnsiString BinName, bool Exact);
	virtual bool __fastcall SelectPaper(AnsiString PaperName, bool Exact);
	virtual void __fastcall GetCustomExtents(Types::TPoint &MinExtent, Types::TPoint &MaxExtent);
	virtual bool __fastcall SetPaperSize(int Size, int Width, int Height);
	virtual bool __fastcall SupportBin(int BinNum);
	virtual bool __fastcall SupportPaper(int PaperNum);
	virtual bool __fastcall SupportOrientation(void);
	virtual bool __fastcall SupportDuplex(void);
	virtual bool __fastcall SupportCollate(void);
	__property AnsiString Title = {read=FTitle, write=FTitle};
	__property bool Printing = {read=FPrinting, nodefault};
	__property bool Aborted = {read=FAborted, nodefault};
	__property Qgraphics::TCanvas* Canvas = {read=GetCanvas};
	__property int XDPI = {read=GetXDPI, nodefault};
	__property int YDPI = {read=GetYDPI, nodefault};
	__property Types::TRect Waste = {read=GetWaste};
	__property Classes::TStrings* Bins = {read=GetBins};
	__property Classes::TStrings* Papers = {read=GetPapers};
	__property Classes::TStrings* Fonts = {read=GetFonts};
	__property Classes::TStrings* Printers = {read=GetPrinters};
	__property Qrpdefine::TOrientation Orientation = {read=GetOrientation, write=SetOrientation, nodefault};
	__property int MaxCopies = {read=GetMaxCopies, nodefault};
	__property int Copies = {read=GetCopies, write=SetCopies, nodefault};
	__property Qrpdefine::TDuplex Duplex = {read=GetDuplex, write=SetDuplex, nodefault};
	__property bool Collate = {read=GetCollate, write=SetCollate, nodefault};
	__property int PageWidth = {read=GetPageWidth, nodefault};
	__property int PageHeight = {read=GetPageHeight, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRPBaseDevice(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRPRenderDevice;
class PASCALIMPLEMENTATION TRPRenderDevice : public TRPBaseDevice 
{
	typedef TRPBaseDevice inherited;
	
protected:
	Qgraphics::TBitmap* FBitmap;
	Qgraphics::TCanvas* FCanvas;
	virtual Qgraphics::TCanvas* __fastcall GetCanvas(void);
	void __fastcall SetCanvas(Qgraphics::TCanvas* Value);
	virtual int __fastcall GetXDPI(void);
	virtual int __fastcall GetYDPI(void);
	virtual int __fastcall GetPageHeight(void);
	virtual int __fastcall GetPageWidth(void);
	
public:
	__fastcall TRPRenderDevice(void);
	__fastcall virtual ~TRPRenderDevice(void);
	virtual int __fastcall TextWidth(AnsiString Value);
	virtual void __fastcall BeginDoc(void);
	virtual void __fastcall BeginDocSelect(void);
	virtual void __fastcall EndDoc(void);
	virtual void __fastcall Abort(void);
	virtual void __fastcall NewPage(void);
	virtual void __fastcall NewPageSelect(bool PageValid);
	__property Qgraphics::TCanvas* Canvas = {read=GetCanvas, write=SetCanvas};
};


class DELPHICLASS TRPPrinterDevice;
class PASCALIMPLEMENTATION TRPPrinterDevice : public TRPBaseDevice 
{
	typedef TRPBaseDevice inherited;
	
protected:
	char *FDeviceName;
	char *FDriverName;
	char *FOutputName;
	HDC DC;
	unsigned FDriverHandle;
	unsigned FDeviceMode;
	_devicemodeA *FDevMode;
	Qrpdefine::TDeviceState FDeviceState;
	Classes::TStrings* FFontList;
	Classes::TStrings* FPrinterList;
	Classes::TStrings* FBinList;
	Classes::TStrings* FPaperList;
	int FDeviceIndex;
	char *FOutputFile;
	Qgraphics::TCanvas* FCanvas;
	Qgraphics::TBitmap* FBitmap;
	bool CanvasActive;
	bool FDevModeChanged;
	TBrushBuf SaveBrush;
	TFontBuf SaveFont;
	TPenBuf SavePen;
	bool OnPage;
	bool KeepDevMode;
	bool FInvalidPrinter;
	int FAbortDelay;
	bool FSkipAbortProc;
	void __fastcall CheckPrinting(void);
	void __fastcall CheckNotPrinting(void);
	char * __fastcall GetWord(char * &Line);
	int __fastcall FindPrinter(char * Device, char * Driver, char * Output);
	AnsiString __fastcall GetOutputFile();
	void __fastcall SetOutputFile(AnsiString Value);
	int __fastcall GetDeviceIndex(void);
	void __fastcall SetDeviceIndex(int Value);
	void __fastcall SetDeviceState(Qrpdefine::TDeviceState Value);
	HDC __fastcall GetHandle(void);
	virtual Qgraphics::TCanvas* __fastcall GetCanvas(void);
	Windows::PDeviceModeA __fastcall GetDevMode(void);
	AnsiString __fastcall GetDriverName();
	AnsiString __fastcall GetOutputName();
	AnsiString __fastcall GetDeviceName();
	void __fastcall OpenDevice(int NewIndex);
	void __fastcall CloseDevice(void);
	void __fastcall ResetDevice(void);
	void __fastcall ReleaseCanvas(void);
	void __fastcall RecoverCanvas(void);
	virtual int __fastcall GetXDPI(void);
	virtual int __fastcall GetYDPI(void);
	virtual Types::TRect __fastcall GetWaste();
	virtual Classes::TStrings* __fastcall GetBins(void);
	virtual Classes::TStrings* __fastcall GetPapers(void);
	virtual Classes::TStrings* __fastcall GetFonts(void);
	virtual Classes::TStrings* __fastcall GetPrinters(void);
	virtual Qrpdefine::TOrientation __fastcall GetOrientation(void);
	virtual void __fastcall SetOrientation(Qrpdefine::TOrientation Value);
	virtual int __fastcall GetMaxCopies(void);
	virtual void __fastcall SetCopies(int Value);
	virtual int __fastcall GetCopies(void);
	virtual void __fastcall SetDuplex(Qrpdefine::TDuplex Value);
	virtual Qrpdefine::TDuplex __fastcall GetDuplex(void);
	virtual void __fastcall SetCollate(bool Value);
	virtual bool __fastcall GetCollate(void);
	virtual int __fastcall GetPageHeight(void);
	virtual int __fastcall GetPageWidth(void);
	
public:
	__fastcall TRPPrinterDevice(void);
	__fastcall virtual ~TRPPrinterDevice(void);
	void __fastcall SaveToPrinter(void);
	void __fastcall LoadFromPrinter(void);
	bool __fastcall PrinterSetupDialog(void);
	bool __fastcall PrintDialog(TPrintDialogRec &PrintDialogRec);
	bool __fastcall SimplePrintDialog(int &FirstPage, int &LastPage);
	void __fastcall ResetHandle(bool Force);
	virtual int __fastcall TextWidth(AnsiString Value);
	virtual void __fastcall RawOut(void *ABuffer, Word ALength);
	virtual void __fastcall BeginDoc(void);
	virtual void __fastcall BeginDocSelect(void);
	virtual void __fastcall EndDoc(void);
	virtual void __fastcall Abort(void);
	virtual void __fastcall NewPage(void);
	virtual void __fastcall NewPageSelect(bool PageValid);
	virtual bool __fastcall SelectBin(AnsiString BinName, bool Exact);
	virtual bool __fastcall SelectPaper(AnsiString PaperName, bool Exact);
	bool __fastcall SelectPrinter(AnsiString PrinterName, bool Exact);
	virtual void __fastcall GetCustomExtents(Types::TPoint &MinExtent, Types::TPoint &MaxExtent);
	virtual bool __fastcall SetPaperSize(int Size, int Width, int Height);
	bool __fastcall InvalidPrinter(void);
	virtual bool __fastcall SupportBin(int BinNum);
	virtual bool __fastcall SupportPaper(int PaperNum);
	virtual bool __fastcall SupportOrientation(void);
	virtual bool __fastcall SupportDuplex(void);
	virtual bool __fastcall SupportCollate(void);
	__property AnsiString OutputFile = {read=GetOutputFile, write=SetOutputFile};
	__property AnsiString Device = {read=GetDeviceName};
	__property AnsiString Driver = {read=GetDriverName};
	__property AnsiString Output = {read=GetOutputName};
	__property Windows::PDeviceModeA DevMode = {read=GetDevMode};
	__property HDC Handle = {read=GetHandle, nodefault};
	__property int DeviceIndex = {read=GetDeviceIndex, write=SetDeviceIndex, nodefault};
	__property Qrpdefine::TDeviceState State = {read=FDeviceState, write=SetDeviceState, nodefault};
	__property bool DevModeChanged = {read=FDevModeChanged, write=FDevModeChanged, nodefault};
	__property int AbortDelay = {read=FAbortDelay, write=FAbortDelay, nodefault};
	__property bool SkipAbortProc = {read=FSkipAbortProc, write=FSkipAbortProc, nodefault};
};


typedef TRPPrinterDevice TRPDevice;
;

//-- var, const, procedure ---------------------------------------------------
extern "C" int __stdcall DeviceCapabilities(char * DeviceName, char * Port, Word Index, char * Output, Windows::PDeviceModeA DevMode);
extern PACKAGE TRPBaseDevice* __fastcall GlobalDevice(void);
extern PACKAGE TRPPrinterDevice* __fastcall RPDev(void);
extern PACKAGE TRPBaseDevice* __fastcall SetNewDevice(TRPBaseDevice* NewDevice);
#pragma option push -w-inl
inline int __stdcall DeviceCapabilities(char * DeviceName, char * Port, Word Index, char * Output, Windows::PDeviceModeA DevMode)
{
	return DeviceCapabilitiesA(DeviceName, Port, Index, Output, DevMode);
}
#pragma option pop



}	/* namespace Qrpdevice */
using namespace Qrpdevice;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpDevice
