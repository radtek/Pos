{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpDevice;

{$T-}

interface

uses
  WinSpool, Windows, CommDlg, Messages,
  Graphics, Dialogs, Forms, Printers,
  SysUtils, Classes, RpDefine;

{ Special notes on using RPDev: TRPBaseDevice directly:
  * When modifying RPDev.DevMode directly you must call ResetHandle for
    the changes to take effect.
  * Use RPDev.PrinterSetupDialog to show the user a printer setup dialog
  * Use RPDev.SaveToPrinter to copy the printer settings from RPDev to
    Delphi's Printer object
  * Use RPDev.LoadFromPrinter to copy the printer settings from Delphi's
    Printer object to RPDev
}

type
  TDCHandle = Windows.HDC;

  TPrintDialogRec = record
    Collate: boolean;
    Copies: integer;
    FromPage: integer;
    ToPage: integer;
    MinPage: integer;
    MaxPage: integer;
    Options: TPrintDialogOptions;
    PrintToFile: boolean;
    PrintRange: TPrintRange;
  end; { TPrintDialogRec }

  TBrushBuf = record
    Color: TColor;
    Style: TBrushStyle;
    Bitmap: TBitmap;
  end; { TBrushBuf }

  TFontBuf = record
    Name: TFontName;
    Size: integer;
    Color: TColor;
    Style: TFontStyles;
  end; { TFontBuf }

  TPenBuf = record
    PenMode: TPenMode;
    Style: TPenStyle;
    Width: integer;
    Color: TColor;
  end; { TPenBuf }

  TRPBaseDevice = class
  protected
    FTitle: TTitleString; { Title of report for print manager }
    FPrinting: boolean; { Is this device printing? }
    FAborted: boolean; { Has the print job been aborted? }
    FOrientation: TOrientation;
    FCopies: integer;
    FDuplex: TDuplex;
    FCollate: boolean;
    FPageHeight: integer;
    FPageWidth: integer;
    FBin: string;
    FPaper: string;

    function GetCanvas: TCanvas; virtual; abstract;
    function GetXDPI: integer; virtual;
    function GetYDPI: integer; virtual;
    function GetWaste: TRect; virtual;

  { Capability methods }
    function GetBins: TStrings; virtual;
    function GetPapers: TStrings; virtual;
    function GetFonts: TStrings; virtual;
    function GetPrinters: TStrings; virtual;
    function GetOrientation: TOrientation; virtual;
    procedure SetOrientation(Value: TOrientation); virtual;
    function GetMaxCopies: longint; virtual;
    procedure SetCopies(Value: integer); virtual;
    function GetCopies: integer; virtual;
    procedure SetDuplex(Value: TDuplex); virtual;
    function GetDuplex: TDuplex; virtual;
    procedure SetCollate(Value: boolean); virtual;
    function GetCollate: boolean; virtual;
    function GetPageHeight: integer; virtual;
    function GetPageWidth: integer; virtual;
  public
    constructor Create;

  { Output methods }
    function TextWidth(Value: string): integer; virtual;
    procedure RawOut(var ABuffer;
                         ALength: word); virtual;

  { Job methods }
    procedure BeginDoc; virtual;
    procedure BeginDocSelect; virtual;
    procedure EndDoc; virtual;
    procedure Abort; virtual;
    procedure NewPage; virtual;
    procedure NewPageSelect(PageValid: boolean); virtual;

  { Capability methods }
    function SelectBin(BinName: string; Exact: boolean): boolean; virtual;
    function SelectPaper(PaperName: string; Exact: boolean): boolean; virtual;
    procedure GetCustomExtents(var MinExtent: TPoint;
                               var MaxExtent: TPoint); virtual;
    function SetPaperSize(Size, Width, Height: integer): boolean; virtual;

  { Support methods }
    function SupportBin(BinNum: integer): boolean; virtual;
    function SupportPaper(PaperNum: integer): boolean; virtual;
    function SupportOrientation: boolean; virtual;
    function SupportDuplex: boolean; virtual;
    function SupportCollate: boolean; virtual;

  { Job properties }
    property Title: TTitleString read FTitle write FTitle;
    property Printing: boolean read FPrinting;
    property Aborted: boolean read FAborted;

  { Device properties }
    property Canvas: TCanvas read GetCanvas;
    property XDPI: integer read GetXDPI;
    property YDPI: integer read GetYDPI;
    property Waste: TRect read GetWaste;

  { Capability properties }
    property Bins: TStrings read GetBins;
    property Papers: TStrings read GetPapers;
    property Fonts: TStrings read GetFonts;
    property Printers: TStrings read GetPrinters;
    property Orientation: TOrientation read GetOrientation write SetOrientation;
    property MaxCopies: longint read GetMaxCopies;
    property Copies: integer read GetCopies write SetCopies;
    property Duplex: TDuplex read GetDuplex write SetDuplex;
    property Collate: boolean read GetCollate write SetCollate;
    property PageWidth: integer read GetPageWidth;
    property PageHeight: integer read GetPageHeight;
  end; { TRPBaseDevice }

  TRPRenderDevice = class(TRPBaseDevice)
  protected
    FBitmap: Graphics.TBitmap;
    FCanvas: TCanvas;

    function GetCanvas: TCanvas; override;
    procedure SetCanvas(Value: TCanvas);
    function GetXDPI: integer; override;
    function GetYDPI: integer; override;
    function GetPageHeight: integer; override;
    function GetPageWidth: integer; override;
  public
    constructor Create;
    destructor Destroy; override;

  { Output methods }
    function TextWidth(Value: string): integer; override;

  { Job methods }
    procedure BeginDoc; override;
    procedure BeginDocSelect; override;
    procedure EndDoc; override;
    procedure Abort; override;
    procedure NewPage; override;
    procedure NewPageSelect(PageValid: boolean); override;

  { Capability methods }
(*
    function SelectBin(BinName: string;
                       Exact: boolean): boolean; virtual;
    function SelectPaper(PaperName: string;
                         Exact: boolean): boolean; virtual;
    procedure GetCustomExtents(var MinExtent: TPoint;
                               var MaxExtent: TPoint); virtual;
    function SetPaperSize(Size: integer;
                          Width: integer;
                          Height: integer): boolean; virtual;
*)
    property Canvas: TCanvas read GetCanvas write SetCanvas;
  end; { TRPRenderDevice }

  TRPPrinterDevice = class(TRPBaseDevice)
  protected
    FDeviceName: PChar; { Current device name }
    FDriverName: PChar; { Current driver name }
    FOutputName: PChar; { Current output port name }
    DC: TDCHandle; { Handle of device DC }
    FDriverHandle: THandle; { Stores handle of printer driver }
    FDeviceMode: THandle; { Handle to TDevMode memory }
    FDevMode: Windows.PDevMode; { Pointer to TDevMode memory }
    FDeviceState: TDeviceState; { Current state of printer device }
    FFontList: TStrings; { List of available fonts }
    FPrinterList: TStrings; { List of installed printers }
    FBinList: TStrings; { List of available bins }
    FPaperList: TStrings; { List of available paper sizes }
    FDeviceIndex: integer; { Current driver index, -2 for none }
    FOutputFile: PChar; { Alternate output file }
    FCanvas: TCanvas; { Canvas attached to this device }
    FBitmap: TBitmap; { Alternate TCanvas for CLX support }
    CanvasActive: boolean; { Is the canvas active }
    FDevModeChanged: boolean; { Has DevMode changed since last ResetDC? }
    SaveBrush: TBrushBuf; { Used by ReleaseCanvas and RecoverCanvas }
    SaveFont: TFontBuf; { Used by ReleaseCanvas and RecoverCanvas }
    SavePen: TPenBuf; { Used by ReleaseCanvas and RecoverCanvas }
    OnPage: boolean; { Are we currently printing a page }
    KeepDevMode: boolean; { Keep current TDevMode settings for new printer }
    FInvalidPrinter: boolean; { Current printer is invalid or no printers }
    FAbortDelay: integer; { Setting to get around certain driver problems }
    FSkipAbortProc: boolean; { Setting to get around certain driver problems }

  { Internal methods }
    procedure CheckPrinting;
    procedure CheckNotPrinting;
    function GetWord(var Line: PChar): PChar;
    function FindPrinter(Device, Driver, Output: PChar): integer;

  { Job methods }
    function GetOutputFile: string;
    procedure SetOutputFile(Value: string);

  { Device methods }
    function GetDeviceIndex: integer;
    procedure SetDeviceIndex(Value: integer);
    procedure SetDeviceState(Value: TDeviceState);
    function GetHandle: TDCHandle;
    function GetCanvas: TCanvas; override;
    function GetDevMode: Windows.PDevMode;
    function GetDriverName: string;
    function GetOutputName: string;
    function GetDeviceName: string;
    procedure OpenDevice(NewIndex: integer);
    procedure CloseDevice;
    procedure ResetDevice;
    procedure ReleaseCanvas;
    procedure RecoverCanvas;
    function GetXDPI: integer; override;
    function GetYDPI: integer; override;
    function GetWaste: TRect; override;

  { Capability methods }
    function GetBins: TStrings; override;
    function GetPapers: TStrings; override;
    function GetFonts: TStrings; override;
    function GetPrinters: TStrings; override;
    function GetOrientation: TOrientation; override;
    procedure SetOrientation(Value: TOrientation); override;
    function GetMaxCopies: longint; override;
    procedure SetCopies(Value: integer); override;
    function GetCopies: integer; override;
    procedure SetDuplex(Value: TDuplex); override;
    function GetDuplex: TDuplex; override;
    procedure SetCollate(Value: boolean); override;
    function GetCollate: boolean; override;
    function GetPageHeight: integer; override;
    function GetPageWidth: integer; override;
  public
    constructor Create;
    destructor Destroy; override;
    procedure SaveToPrinter;
    procedure LoadFromPrinter;
    function PrinterSetupDialog: boolean;
    function PrintDialog(var PrintDialogRec: TPrintDialogRec): boolean;
    function SimplePrintDialog(var FirstPage: integer;
                               var LastPage: integer): boolean;
    procedure ResetHandle(Force: boolean);

  { Output methods }
    function TextWidth(Value: string): integer; override;
    procedure RawOut(var ABuffer; ALength: word); override;

  { Job methods }
    procedure BeginDoc; override;
    procedure BeginDocSelect; override;
    procedure EndDoc; override;
    procedure Abort; override;
    procedure NewPage; override;
    procedure NewPageSelect(PageValid: boolean); override;

  { Capability methods }
    function SelectBin(BinName: string; Exact: boolean): boolean; override;
    function SelectPaper(PaperName: string; Exact: boolean): boolean; override;
    function SelectPrinter(PrinterName: string; Exact: boolean): boolean;
    procedure GetCustomExtents(var MinExtent: TPoint;
                               var MaxExtent: TPoint); override;
    function SetPaperSize(Size, Width, Height: integer): boolean; override;
    function InvalidPrinter: boolean;

  { Support methods }
    function SupportBin(BinNum: integer): boolean; override;
    function SupportPaper(PaperNum: integer): boolean; override;
    function SupportOrientation: boolean; override;
    function SupportDuplex: boolean; override;
    function SupportCollate: boolean; override;

  { Job properties }
    property OutputFile: string read GetOutputFile write SetOutputFile;

  { Device properties }
    property Device: string read GetDeviceName;
    property Driver: string read GetDriverName;
    property Output: string read GetOutputName;
    property DevMode: Windows.PDevMode read GetDevMode;
    property Handle: TDCHandle read GetHandle;
    property DeviceIndex: integer read GetDeviceIndex write SetDeviceIndex;
    property State: TDeviceState read FDeviceState write SetDeviceState;
    property DevModeChanged: boolean read FDevModeChanged write FDevModeChanged;
    property AbortDelay: integer read FAbortDelay write FAbortDelay;
    property SkipAbortProc: boolean read FSkipAbortProc write FSkipAbortProc;
  end; { TRPPrinterDevice }
  TRPDevice = TRPPrinterDevice; // Compatibility typecast

  TRPPrinterCanvas = class(TCanvas)
    RPDevice: TRPPrinterDevice;
    constructor Create(Device: TRPPrinterDevice);
    procedure CreateHandle; override;
  end; { TRPCanvas }

  function DeviceCapabilities(DeviceName: PChar;
                              Port: PChar;
                              Index: word;
                              Output: PChar;
                              DevMode: PDeviceMode): integer; stdcall;
  function GlobalDevice: TRPBaseDevice;
  function RPDev: TRPPrinterDevice;
  function SetNewDevice(NewDevice: TRPBaseDevice): TRPBaseDevice;

implementation

uses
  RpFont;

{ class TRPBaseDevice }

constructor TRPBaseDevice.Create;
begin
  inherited Create;
  FCopies := 1;
  FOrientation := RpDefine.poLandscape;
  FPageWidth := Round(8.5 * XDPI);
  FPageHeight := Round(11.0 * YDPI);
end;

function TRPBaseDevice.GetBins: TStrings;
begin
  Result := nil;
end;

function TRPBaseDevice.GetPapers: TStrings;
begin
  Result := nil;
end;

function TRPBaseDevice.GetFonts: TStrings;
begin
  Result := nil;
end;

function TRPBaseDevice.GetPrinters: TStrings;
begin
  Result := nil;
end;

function TRPBaseDevice.GetOrientation: TOrientation;
begin
  Result := FOrientation;
end;

procedure TRPBaseDevice.SetOrientation(Value: TOrientation);
begin
  FOrientation := Value;
end;

function TRPBaseDevice.GetMaxCopies: longint;
begin
  Result := 1;
end;

procedure TRPBaseDevice.SetCopies(Value: integer);
begin
  FCopies := Value;
end;

function TRPBaseDevice.GetCopies: integer;
begin
  Result := FCopies;
end;

procedure TRPBaseDevice.SetDuplex(Value: TDuplex);
begin
  FDuplex := Value;
end;

function TRPBaseDevice.GetDuplex: TDuplex;
begin
  Result := FDuplex;
end;

procedure TRPBaseDevice.SetCollate(Value: boolean);
begin
  FCollate := Value;
end;

function TRPBaseDevice.GetCollate: boolean;
begin
  Result := FCollate;
end;

function TRPBaseDevice.GetPageHeight: integer;
begin
  Result := FPageHeight;
end;

function TRPBaseDevice.GetPageWidth: integer;
begin
  Result := FPageWidth;
end;

function TRPBaseDevice.TextWidth(Value: string): integer;
begin
  Result := 0;
end;

procedure TRPBaseDevice.RawOut(var ABuffer; ALength: word);
begin
end;

procedure TRPBaseDevice.BeginDoc;
begin
end;

procedure TRPBaseDevice.BeginDocSelect;
begin
end;

procedure TRPBaseDevice.EndDoc;
begin
end;

procedure TRPBaseDevice.Abort;
begin
end;

procedure TRPBaseDevice.NewPage;
begin
end;

procedure TRPBaseDevice.NewPageSelect(PageValid: boolean);
begin
end;

function TRPBaseDevice.SelectBin(BinName: string; Exact: boolean): boolean;
begin
  FBin := BinName;
  Result := true;
end;

function TRPBaseDevice.SelectPaper(PaperName: string; Exact: boolean): boolean;
begin
  FPaper := PaperName;
  Result := true;
end;

procedure TRPBaseDevice.GetCustomExtents(var MinExtent: TPoint;
                                         var MaxExtent: TPoint);
begin
end;

function TRPBaseDevice.SetPaperSize(Size, Width, Height: integer): boolean;
var
  I1: integer;
  Found: boolean;
begin
  Result := true;
  if (Width <> 0) and (Height <> 0) then begin
    FPageWidth := Round((Width / 254) * XDPI);
    FPageHeight := Round((Height / 254) * YDPI);
  end else begin
    Found := false;
    for I1 := 1 to PaperSizeNameCnt do begin
      if PaperSizeName[I1].PaperSize = Size then begin
        FPageWidth := Round(PaperSizeName[I1].PaperWidth * XDPI);
        FPageHeight := Round(PaperSizeName[I1].PaperHeight * YDPI);
        Found := true;
        Break;
      end; { if }
    end; { for }
    if not Found then begin
    FPageWidth := Round(8.5 * XDPI);
    FPageHeight := Round(11.0 * YDPI);
    end; { if }
  end; { else }
end;

function TRPBaseDevice.SupportBin(BinNum: integer): boolean;
begin
  Result := true;
end;

function TRPBaseDevice.SupportPaper(PaperNum: integer): boolean;
begin
  Result := true;
end;

function TRPBaseDevice.SupportOrientation: boolean;
begin
  Result := true;
end;

function TRPBaseDevice.SupportDuplex: boolean;
begin
  Result := true;
end;

function TRPBaseDevice.SupportCollate: boolean;
begin
  Result := true;
end;

function TRPBaseDevice.GetXDPI: integer;
begin { GetXDPI }
  Result := 96;
end;  { GetXDPI }

function TRPBaseDevice.GetYDPI: integer;
begin { GetYDPI }
  Result := 96;
end;  { GetYDPI }

function TRPBaseDevice.GetWaste: TRect;
begin { GetWaste }
  FillChar(Result,SizeOf(Result),0);
end;  { GetWaste }


{ TRPRenderDevice }

constructor TRPRenderDevice.Create;
begin
  inherited;
  FPrinting := true;
  FBitmap := TBitmap.Create;
  FBitmap.Canvas.Font.PixelsPerInch := YDPI;
  FBitmap.Width := 10;
  FBitmap.Height := 10;
  FCanvas := FBitmap.Canvas;
end;

destructor TRPRenderDevice.Destroy;
begin
  FreeAndNil(FBitmap);
  inherited;
end;

function TRPRenderDevice.GetCanvas: TCanvas;
begin
  Result := FCanvas;
end;

procedure TRPRenderDevice.SetCanvas(Value: TCanvas);
begin
  if not Assigned(Value) then begin
    if not Assigned(FBitmap) then begin
      FBitmap := TBitmap.Create;
      FBitmap.Canvas.Font.PixelsPerInch := YDPI;
      FCanvas := FBitmap.Canvas;
    end; { if }
  end else begin
    if Assigned(FBitmap) then begin
      FreeAndNil(FBitmap);
    end; { if }
    FCanvas := Value;
  end; { else }
end;

function TRPRenderDevice.GetXDPI: integer;
begin
  Result := RpFont.BaseSize;
end;

function TRPRenderDevice.GetYDPI: integer;
begin
  Result := RpFont.BaseSize;
end;

function TRPRenderDevice.GetPageHeight: integer;
begin
  if FOrientation = poLandScape then begin
    Result := FPageWidth;
  end else begin
    Result := FPageHeight;
  end; { else }
end;

function TRPRenderDevice.GetPageWidth: integer;
begin
  if FOrientation = poLandscape then begin
    Result := FPageHeight;
  end else begin
    Result := FPageWidth;
  end; { else }
end;

function TRPRenderDevice.TextWidth(Value: string): integer;
begin
  Result := FontManager.TextWidth(Canvas.Font,Value);
end;

procedure TRPRenderDevice.BeginDoc;
begin
end;

procedure TRPRenderDevice.BeginDocSelect;
begin
end;

procedure TRPRenderDevice.EndDoc;
begin
end;

procedure TRPRenderDevice.Abort;
begin
end;

procedure TRPRenderDevice.NewPage;
begin
end;

procedure TRPRenderDevice.NewPageSelect(PageValid: boolean);
begin
end;

{ Printer device items }

function DeviceCapabilities; external winspl name {Trans-}'DeviceCapabilitiesA';


{ TRPPrinterCanvas }

constructor TRPPrinterCanvas.Create(Device: TRPPrinterDevice);
begin { Create }
  inherited Create;
  RPDevice := Device;
  Font.PixelsPerInch := GetDeviceCaps(RPDevice.Handle,LOGPIXELSY);//!!!
end;  { Create }

procedure TRPPrinterCanvas.CreateHandle;
begin { CreateHandle }
  Handle := RPDevice.Handle; { Pass handle for RPDevice object }//!!!
end;  { CreateHandle }

{ class TRPDeviceItem }

{ This class is used to hold the printer information from FPrinterList }

type
TRPDeviceItem = class
  FDeviceName: PChar;
  FDriverName: PChar;
  FOutputName: PChar;

  constructor Create(DeviceName: PChar;
                     DriverName: PChar;
                     OutputName: PChar);
  destructor Destroy; override;
end; { TRPDeviceItem }

constructor TRPDeviceItem.Create(DeviceName: PChar;
                                 DriverName: PChar;
                                 OutputName: PChar);
begin { Create }
  inherited Create;
  FDeviceName := StrNew(DeviceName);
  FDriverName := StrNew(DriverName);
  FOutputName := StrNew(OutputName);
end;  { Create }

destructor TRPDeviceItem.Destroy;
begin { Destroy }
  StrDispose(FOutputName);
  StrDispose(FDriverName);
  StrDispose(FDeviceName);
  inherited Destroy;
end;  { Destroy }

{ class TRPPrinterDevice }

constructor TRPPrinterDevice.Create;
begin { Create }
  FDeviceIndex := -2;
  inherited Create;
end;  { Create }

destructor TRPPrinterDevice.Destroy;
var
  I1: integer;
begin { Destroy }
  State := dsNone;
  CloseDevice;
  if Assigned(FPrinterList) then begin
    for I1 := 1 to FPrinterList.Count do begin
      FPrinterList.Objects[I1 - 1].Free;
    end; { for }
    FreeAndNil(FPrinterList);
  end; { if }
  StrDispose(FOutputFile);
  FreeAndNil(FCanvas);

  inherited Destroy;
end;  { Destroy }

{ Internal Methods }

procedure TRPPrinterDevice.CheckPrinting;
begin { CheckPrinting }
  if not FPrinting then begin
    RaiseError(Trans('Invalid method call.  Device is not printing.'));
  end; { if }
end;  { CheckPrinting }

procedure TRPPrinterDevice.CheckNotPrinting;
begin { CheckNotPrinting }
  if FPrinting then begin
    RaiseError(Trans('Invalid method call.  Device already printing.'));
  end; { if }
end;  { CheckNotPrinting }

function TRPPrinterDevice.GetWord(var Line: PChar): PChar;
begin { GetWord }
  if not Assigned(Line) then begin
    Result := nil;
    Exit;
  end; { if }
  while Line^ = ' ' do begin
    Inc(Line);
  end; { while }
  Result := Line;
  Line := StrPos(Line,',');
  if Assigned(Line) then begin
    Line^ := #0;
    Inc(Line);
  end; { if }
end;  { GetWord }

function TRPPrinterDevice.FindPrinter(Device, Driver, Output: PChar): integer;
var
  I1: integer;
begin { FindPrinter }
  Result := -1;
  with FPrinterList do begin
    if Count = 0 then Exit;
    for I1 := 0 to Count - 1 do begin
      with Objects[I1] as TRPDeviceItem do begin
        if Win32Platform = VER_PLATFORM_WIN32_NT then begin
          if AnsiStrIComp(Device,FDeviceName) = 0 then begin
            Result := I1;
            Exit;
          end; { if }
        end else begin
          if (AnsiStrIComp(Device,FDeviceName) = 0) and
           (AnsiStrIComp(Output,FOutputName) = 0) then begin
            Result := I1;
            Exit;
          end; { if }
        end; { else }
      end; { with }
    end; { for }
  end; { with }
end;  { FindPrinter }

{ Output methods }

function TRPPrinterDevice.TextWidth(Value: string): integer;
var
  TextExtent: TSize;
begin
  if Value = '' then begin
    Result := 1;
  end else begin
    //!!PORT!! RPDEVICE07 #45 Canvas.Handle probably isn't correct since it's not the printer Canvas
//  GetTextExtentPoint32({$IFDEF RaveCLX}QPainter_handle{$ENDIF}(Canvas.Handle),@Value[1],Length(Value),TextExtent);
    GetTextExtentPoint32(Handle,@Value[1],Length(Value),TextExtent);
    if TextExtent.cX = 0 then begin
      TextExtent.cX := 1;
    end; { if }
    Result := TextExtent.cX;
  end; { else }
end;

procedure TRPPrinterDevice.RawOut(var ABuffer; ALength: word);
var
  Buffer: PChar;
begin
  GetMem(Buffer,ALength + SizeOf(word));
  try
    Move(ALength,Buffer^,SizeOf(ALength));
    Move(ABuffer,(Buffer+SizeOf(ALength))^,ALength);
    Escape(Handle,PASSTHROUGH,0,Buffer,nil);
  finally
    FreeMem(Buffer,ALength + SizeOf(word));
  end; { tryf }
end;

{ Device methods }

procedure TRPPrinterDevice.LoadFromPrinter;
var
  NewDevice: PChar;
  NewDriver: PChar;
  NewPort: PChar;
  NewDeviceMode: THandle;
  NewDevMode: Windows.PDevMode;
begin { LoadFromPrinter }
  GetMem(NewDevice,256);
  GetMem(NewDriver,256);
  GetMem(NewPort,256);
  try
    Printer.GetPrinter(NewDevice,NewDriver,NewPort,NewDeviceMode);
    SelectPrinter(NewDevice,true);
    NewDevMode := GlobalLock(NewDeviceMode);
    try
      Move(NewDevMode^,DevMode^,SizeOf(Windows.TDevMode));
    finally
      GlobalUnlock(NewDeviceMode);
    end; { tryf }
    DevModeChanged := true;
  finally
    FreeMem(NewPort,256);
    FreeMem(NewDriver,256);
    FreeMem(NewDevice,256);
  end; { tryf }
end;  { LoadFromPrinter }

procedure TRPPrinterDevice.SaveToPrinter;
begin { SaveToPrinter }
  if FDeviceName = nil then Exit;
  Printer.SetPrinter(FDeviceName,FDriverName,FOutputName,FDeviceMode);
  CloseDevice;
end;  { SaveToPrinter }

function DialogHook(Wnd: HWnd;
                    Msg: UINT;
                    WParam: WPARAM;
                    LParam: LPARAM): UINT; stdcall;
var
  Width: integer;
  Rect: TRect;
begin { DialogHook }
  Result := 0;
  try
    case Msg of
      WM_INITDIALOG: begin
        GetWindowRect(Wnd,Rect);
        Width := Rect.Right - Rect.Left;
        SetWindowPos(Wnd,0,(GetSystemMetrics(SM_CXSCREEN) - Width) div 2,
         (GetSystemMetrics(SM_CYSCREEN) - Rect.Bottom + Rect.Top) div 3,
         0,0,SWP_NOACTIVATE + SWP_NOSIZE + SWP_NOZORDER);
        Result := 1;
      end;
    end; { case }
  except
    Application.HandleException(nil);
  end; { tryx }
end;  { DialogHook }

function TRPPrinterDevice.PrinterSetupDialog: boolean;
var
  PrintDlgRec: TPrintDlg;
  DevNames: PDevNames;
  DeviceNames: THandle;
  S1: PChar;
  S2: string;
begin { PrinterSetupDialog }
  State := dsIC;

{ Allocate DevNames structure }
  Result := false;
  DeviceNames := GlobalAlloc(GHND,SizeOf(TDevNames) + Length(Driver) +
   Length(Device) + Length(Output) + 5);
  try
    DevNames := GlobalLock(DeviceNames);
    try
      S1 := PChar(DevNames) + SizeOf(TDevNames);
      with DevNames^ do begin
        wDriverOffset := longint(S1) - longint(DevNames);
        S1 := StrECopy(S1,PChar(Driver)) + 1;
        wDeviceOffset := longint(S1) - longint(DevNames);
        S1 := StrECopy(S1,PChar(Device)) + 1;
        wOutputOffset := longint(S1) - longint(DevNames);
        StrCopy(S1,PChar(Output));
      end; { with }
    finally
      GlobalUnlock(DeviceNames);
    end; { tryf }

    FillChar(PrintDlgRec,SizeOf(PrintDlgRec),0);
    PrintDlgRec.hInstance := HInstance;
    with PrintDlgRec do begin
      lStructSize := Sizeof(PrintDlgRec);
      hWndOwner := GetFocus;
      hDevMode := FDeviceMode;
      hDevNames := DeviceNames;
      Flags := PD_ENABLESETUPHOOK or PD_PRINTSETUP;
      lpfnSetupHook := DialogHook;
      GlobalUnlock(FDeviceMode);
      if PrintDlg(PrintDlgRec) then begin { Successful }
        FDeviceMode := hDevMode;
        if FDeviceMode = 0 then begin
          FDevMode := nil;
        end else begin
          FDevMode := GlobalLock(FDeviceMode);
        end; { else }
        DevNames := GlobalLock(hDevNames);
        try
//!!PORT!! Added StrPas back in to get it to compile.  MBCS issue
          if Win32Platform = VER_PLATFORM_WIN32_NT then begin
            S2 := StrPas(PChar(DevNames) + DevNames^.wDeviceOffset);
          end else begin
            S2 := StrPas(PChar(DevNames) + DevNames^.wDeviceOffset) +
             {Trans-}' on ' + StrPas(PChar(DevNames) + DevNames^.wOutputOffset);
          end; { else }

          KeepDevMode := true;
          SelectPrinter(S2,true);
          Result := true;
        finally
          GlobalUnlock(hDevNames);
        end; { tryf }
      end; { if }
    end; { with }
  finally
    GlobalUnlock(DeviceNames);
    GlobalFree(PrintDlgRec.hDevNames);
    DevModeChanged := true;
  end; { tryf }
end;  { PrinterSetupDialog }

function TRPPrinterDevice.PrintDialog(var PrintDialogRec: TPrintDialogRec): boolean;
const
  APrintRange: array[TPrintRange] of integer =
   (PD_ALLPAGES, PD_SELECTION, PD_PAGENUMS);
var
  PrintDlgRec: TPrintDlg;
  DevNames: PDevNames;
  DeviceNames: THandle;
  S1: PChar;
  S2: string;
begin { PrintDialog }

  State := dsIC;

{ Allocate DevNames structure }
  Result := false;
  DeviceNames := GlobalAlloc(GHND,SizeOf(TDevNames) + Length(Driver) +
   Length(Device) + Length(Output) + 5);
  try
    DevNames := GlobalLock(DeviceNames);
    try
      S1 := PChar(DevNames) + SizeOf(TDevNames);
      with DevNames^ do begin
        wDriverOffset := longint(S1) - longint(DevNames);
        S1 := StrECopy(S1,PChar(Driver)) + 1;
        wDeviceOffset := longint(S1) - longint(DevNames);
        S1 := StrECopy(S1,PChar(Device)) + 1;
        wOutputOffset := longint(S1) - longint(DevNames);
        StrCopy(S1,PChar(Output));
      end; { with }
    finally
      GlobalUnlock(DeviceNames);
    end; { tryf }

    FillChar(PrintDlgRec,SizeOf(PrintDlgRec),0);
    PrintDlgRec.hInstance := HInstance;
    with PrintDlgRec do begin
      lStructSize := Sizeof(PrintDlgRec);
      hWndOwner := GetFocus;
      hDevMode := FDeviceMode;
      hDevNames := DeviceNames;
      Flags := PD_ENABLESETUPHOOK or PD_ENABLEPRINTHOOK or
       PD_USEDEVMODECOPIES;
      with PrintDialogRec do begin
        case PrintRange of
          prAllPages: Flags := Flags or PD_ALLPAGES;
          prSelection: Flags := Flags or PD_SELECTION;
          prPageNums: Flags := Flags or PD_PAGENUMS;
        end; { case }
        if Collate then begin
          Flags := Flags or PD_COLLATE;
        end; { if }
        if not (poPrintToFile in Options) then begin
          Flags := Flags or PD_HIDEPRINTTOFILE;
        end; { if }
        if not (poPageNums in Options) then begin
          Flags := Flags or PD_NOPAGENUMS;
        end; { if }
        if not (poSelection in Options) then begin
          Flags := Flags or PD_NOSELECTION;
        end; { if }
        if (poDisablePrintToFile in Options) then begin
          Flags := Flags or PD_DISABLEPRINTTOFILE;
        end; { if }
        if poHelp in Options then begin
          Flags := Flags or PD_SHOWHELP;
        end; { if }
        if not (poWarning in Options) then begin
          Flags := Flags or PD_NOWARNING;
        end; { if }
        if PrintToFile then begin
          Flags := Flags or PD_PRINTTOFILE;
        end; { if }
        nFromPage := FromPage;
        nToPage := ToPage;
        nMinPage := MinPage;
        nMaxPage := MaxPage;
        if Assigned(FDevMode) then begin
          nCopies := FDevMode^.dmCopies;
        end else begin
          nCopies := 1;
        end; { else }
      end; { with }
      lpfnPrintHook := DialogHook;
      lpfnSetupHook := DialogHook;
      GlobalUnlock(FDeviceMode);
      if PrintDlg(PrintDlgRec) then begin { Successful }
        FDeviceMode := hDevMode;
        if FDeviceMode = 0 then begin
          FDevMode := nil;
        end else begin
          FDevMode := GlobalLock(FDeviceMode);
        end; { else }
        DevNames := GlobalLock(hDevNames);
        try
//!!PORT!! Added StrPas back in to get it to compile.  MBCS issue
          if Win32Platform = VER_PLATFORM_WIN32_NT then begin
            S2 := StrPas(PChar(DevNames) + DevNames^.wDeviceOffset);
          end else begin
            S2 := StrPas(PChar(DevNames) + DevNames^.wDeviceOffset) +
             {Trans-}' on ' + StrPas(PChar(DevNames) + DevNames^.wOutputOffset);
          end; { else }
          KeepDevMode := true;
          SelectPrinter(S2,true);
          Result := true;
        finally
          GlobalUnlock(hDevNames);
        end; { tryf }
        with PrintDialogRec do begin
          Collate := (Flags and PD_COLLATE) <> 0;
          PrintToFile := (Flags and PD_PRINTTOFILE) <> 0;
          if (Flags and PD_SELECTION) <> 0 then begin
            PrintRange := prSelection;
          end else if (Flags and PD_PAGENUMS) <> 0 then begin
            PrintRange := prPageNums;
          end else begin
            PrintRange := prAllPages;
          end; { else }
          FromPage := nFromPage;
          ToPage := nToPage;
          if Assigned(FDevMode) then begin
            Copies := FDevMode^.dmCopies;
          end else begin
            Copies := 1;
          end; { else }
        end; { with }
      end; { if }
    end; { with }
  finally
    GlobalUnlock(DeviceNames);
    GlobalFree(PrintDlgRec.hDevNames);
    DevModeChanged := true;
  end; { tryf }
end;  { PrintDialog }

function TRPPrinterDevice.SimplePrintDialog(var FirstPage: integer;
                                            var LastPage: integer): boolean;
var
  PrintDialogRec: TPrintDialogRec;
begin { SimplePrintDialog }
  FillChar(PrintDialogRec,SizeOf(PrintDialogRec),0);
  with PrintDialogRec do begin
    Collate := self.Collate;
    Copies := self.Copies;
    FromPage := FirstPage;
    ToPage := LastPage;
    MinPage := 1;
    MaxPage := LastPage;
    Options := [poWarning];
    PrintToFile := false;
    PrintRange := prAllPages;
  end; { with }
  Result := self.PrintDialog(PrintDialogRec);
  with PrintDialogRec do begin
    self.Collate := Collate;
    SetCopies(Copies);
    if PrintRange = prPageNums then begin
      FirstPage := FromPage;
      LastPage := ToPage;
    end else if PrintRange = prAllPages then begin
      FirstPage := 1;
      LastPage := 9999;
    end; { else }
  end; { with }
end;  { SimplePrintDialog }

function TRPPrinterDevice.GetDeviceIndex: integer;
begin { GetDeviceIndex }
  if FDeviceIndex < 0 then begin
    FDeviceIndex := -2;
    SetDeviceIndex(-1);
  end; { if }
  Result := FDeviceIndex;
end;  { GetDeviceIndex }

procedure TRPPrinterDevice.SetDeviceIndex(Value: integer);
const
  DeviceBufSize = 256;
var
  DeviceBuf: PChar;
  DevicePtr: PChar;
  DeviceName: PChar;
  DriverName: PChar;
  OutputName: PChar;
  NewIndex: integer;
begin { SetDeviceIndex }
  CheckNotPrinting;
  FInvalidPrinter := false;
  if (Value <> FDeviceIndex) and (Value < Printers.Count) then begin
    if Value < 0 then begin { Set to default printer }
      GetMem(DeviceBuf,DeviceBufSize);
      try
        GetProfileString({Trans-}'WINDOWS',{Trans-}'DEVICE','',DeviceBuf,
         DeviceBufSize);
        if DeviceBuf^ = #0 then begin
          NewIndex := FPrinterList.Count - 1;
        end else begin
          DevicePtr := DeviceBuf;
          DeviceName := GetWord(DevicePtr);
          DriverName := GetWord(DevicePtr);
          OutputName := GetWord(DevicePtr);
          NewIndex := FindPrinter(DeviceName,DriverName,OutputName);
          if NewIndex < 0 then begin
            NewIndex := FPrinterList.Count - 1;
          end; { if }
        end; { else }
      finally
        FreeMem(DeviceBuf,DeviceBufSize);
      end; { tryf }
    end else begin { Get printer from FPrinterList }
      NewIndex := Value;
    end; { else }
    if NewIndex >= 0 then begin
      OpenDevice(NewIndex);
    end; { if }
  end; { if }
end;  { SetDeviceIndex }

procedure TRPPrinterDevice.SetDeviceState(Value: TDeviceState);
var
  RPDeviceItem: TRPDeviceItem;
begin { SetDeviceState }
  if not ((FDeviceState = dsDC) and (Value = dsIC)) and
   (Value <> FDeviceState) then begin
    if DC <> 0 then begin
      ReleaseCanvas;
      DeleteDC(DC);
      DC := 0;
    end; { if }

    if (Value <> dsNone) and not InvalidPrinter then begin
      GetDeviceIndex;
      RPDeviceItem := TRPDeviceItem(Printers.Objects[FDeviceIndex]);
      with RPDeviceItem do begin
        if Value = dsIC then begin
          DC := CreateIC(FDriverName,FDeviceName,FOutputName,FDevMode);
        end else begin
          DC := CreateDC(FDriverName,FDeviceName,FOutputName,FDevMode);
        end; { else }
        if DC = 0 then begin
          FInvalidPrinter := true;
        end; { if }
        if FCanvas <> nil then begin
          FCanvas.Handle := DC;
        end; { if }
      end; { with }
    end; { if }
    FDeviceState := Value;
  end; { if }
end;  { SetDeviceState }

function TRPPrinterDevice.GetHandle: TDCHandle;
begin { GetHandle }
  State := dsIC;
  Result := DC;
end;  { GetHandle }

function TRPPrinterDevice.GetCanvas: TCanvas;
begin { GetCanvas }
  if FCanvas = nil then begin
    FCanvas := TRPPrinterCanvas.Create(self);
  end; { if }
  Result := FCanvas;
end;  { GetCanvas }

function TRPPrinterDevice.GetDevMode: Windows.PDevMode;
begin { GetDevMode }
  if InvalidPrinter then begin
    Result := nil;
  end else begin
    GetDeviceIndex;
    Result := FDevMode;
  end; { else }
end;  { GetDevMode }

function TRPPrinterDevice.GetDriverName: string;
begin { GetDriverName }
  if Assigned(FDriverName) then begin
    Result := FDriverName;
  end else begin
    Result := '';
  end; { else }
end;  { GetDriverName }

function TRPPrinterDevice.GetOutputName: string;
begin { GetOutputName }
  if Assigned(FOutputName) then begin
    Result := FOutputName;
  end else begin
    Result := '';
  end; { else }
end;  { GetOutputName }

function TRPPrinterDevice.GetDeviceName: string;
begin { GetDeviceName }
  if Assigned(FDeviceName) then begin
    Result := FDeviceName;
  end else begin
    Result := '';
  end; { else }
end;  { GetDeviceName }

procedure TRPPrinterDevice.CloseDevice;
begin { CloseDevice }
{ Close out old device first }
  State := dsNone;
  if FDeviceMode <> 0 then begin
    if not KeepDevMode then begin
      GlobalUnlock(FDeviceMode);
      GlobalFree(FDeviceMode);
      FDeviceMode := 0;
      FDevMode := nil;
      DevModeChanged := false;
    end; { if }
    FDeviceIndex := -2;
  end; { if }
  if FDriverHandle <> 0 then begin
    ClosePrinter(FDriverHandle);
    FDriverHandle := 0;
  end; { if }
  FreeAndNil(FFontList);
  FreeAndNil(FBinList);
  FreeAndNil(FPaperList);

  StrDispose(FDeviceName);
  FDeviceName := nil;
  StrDispose(FDriverName);
  FDriverName := nil;
  StrDispose(FOutputName);
  FOutputName := nil;
end;  { CloseDevice }

procedure TRPPrinterDevice.OpenDevice(NewIndex: integer);
var
  TempDevMode: Windows.PDevMode;
  DevSize: integer;
begin { OpenDevice }
  if NewIndex = FDeviceIndex then Exit;

  CloseDevice;

{ Get the selected RPDeviceItem }
  FDeviceIndex := NewIndex;
  FDeviceName := StrNew(TRPDeviceItem(FPrinterList.Objects[FDeviceIndex]).
   FDeviceName);
  FDriverName := StrNew(TRPDeviceItem(FPrinterList.Objects[FDeviceIndex]).
   FDriverName);
  FOutputName := StrNew(TRPDeviceItem(FPrinterList.Objects[FDeviceIndex]).
   FOutputName);

{ Get printer driver handle }
  if not OpenPrinter(FDeviceName,FDriverHandle,nil) then begin
    FDriverHandle := 0;
  end; { if }

  if (FDriverHandle > 0) and not KeepDevMode then begin
  { Figure out DevSize }
    GetMem(TempDevMode,SizeOf(TDevMode));
    try
      DevSize := DocumentProperties(0,FDriverHandle,FDeviceName,TempDevMode^,
       TempDevMode^,0);
    finally
      FreeMem(TempDevMode,SizeOf(TDevMode));
    end; { tryf }

    if DevSize > 0 then begin
    { Allocate space for FDeviceMode }
      FDeviceMode := GlobalAlloc(GMEM_MOVEABLE or GMEM_ZEROINIT,DevSize);
      if FDeviceMode <> 0 then begin
        FDevMode := GlobalLock(FDeviceMode);
        DevModeChanged := false;

      { Load up FDevMode with TDevMode information }
        if DocumentProperties(0,FDriverHandle,FDeviceName,FDevMode^,
         FDevMode^,DM_OUT_BUFFER) < 0 then begin { Error getting data }
          CloseDevice;
        end; { if }
      end; { if }
    end; { if }
  end; { if }
  KeepDevMode := false;
end;  { OpenDevice }

procedure TRPPrinterDevice.ResetDevice;
begin { ResetDevice }
  State := dsNone;
  if FDriverHandle <> 0 then begin
    ClosePrinter(FDriverHandle);
    FDriverHandle := 0;
  end; { if }
  FreeAndNil(FFontList);
  FreeAndNil(FBinList);
  FreeAndNil(FPaperList);

{ Get printer driver handle }
  if not OpenPrinter(FDeviceName,FDriverHandle,nil) then begin
    FDriverHandle := 0;
  end; { if }
  if FDriverHandle = 0 then begin
    CloseDevice;
  end; { if }
end;  { ResetDevice }

procedure TRPPrinterDevice.ReleaseCanvas;
begin { ReleaseCanvas }
  CanvasActive := (FCanvas <> nil);
  if not CanvasActive then Exit;

{ Save off Font, Pen and Brush }
  with Canvas do begin
    with SaveBrush do begin
      Color := Brush.Color;
      Style := Brush.Style;
      if (Brush.Bitmap = nil) or Brush.Bitmap.Empty then begin
        Bitmap := nil;
      end else begin
        Bitmap := TBitmap.Create;
        Bitmap.Assign(Brush.Bitmap);
      end; { if }
    end; { with }
    with SavePen do begin
      Color := Pen.Color;
      PenMode := Pen.Mode;
      Style := Pen.Style;
      Width := Pen.Width;
    end; { with }
    with SaveFont do begin
      Name := Font.Name;
      Size := Font.Size;
      Color := Font.Color;
      Style := Font.Style;
    end; { with }
  end; { with }

{ Release canvas }
  FCanvas.Handle := 0;
  FCanvas.Pen.Handle := GetStockObject(BLACK_PEN);
  FCanvas.Font.Handle := GetStockObject(SYSTEM_FONT);
  FCanvas.Brush.Handle := GetStockObject(BLACK_BRUSH);
end;  { ReleaseCanvas }

procedure TRPPrinterDevice.RecoverCanvas;
begin { RecoverCanvas }
  if not CanvasActive then Exit;

{ Recover canvas }
  FCanvas.Handle := DC;
  FCanvas.Font.PixelsPerInch := GetDeviceCaps(DC,LOGPIXELSY);


{ Recover Font, Pen and Brush }
  with Canvas do begin
    with SaveBrush do begin
      Brush.Color := Color;
      Brush.Style := Style;
      if Bitmap <> nil then begin
        Brush.Bitmap.Assign(Bitmap);
        FreeAndNil(Bitmap);
      end; { if }
    end; { with }
    with SavePen do begin
      Pen.Color := Color;
      Pen.Mode := PenMode;
      Pen.Style := Style;
      Pen.Width := Width;
    end; { with }
    with SaveFont do begin
      Font.Name := Name;
      Font.Size := Size;
      Font.Color := Color;
      Font.Style := Style;
    end; { with }
  end; { with }
end;  { RecoverCanvas }

{ Job Methods }

function TRPPrinterDevice.GetOutputFile: string;
begin { GetOutputFile }
  Result := FOutputFile;
end;  { GetOutputFile }

procedure TRPPrinterDevice.SetOutputFile(Value: string);
begin { SetOutputFile }
  CheckNotPrinting;

  StrDispose(FOutputFile);
  if Value = '' then begin
    FOutputFile := nil;
  end else begin
    FOutputFile := StrPCopy(StrAlloc(Length(Value) + 1),Value);
  end; { else }
end;  { SetOutputFile }

procedure TRPPrinterDevice.BeginDoc;
begin { BeginDoc }
  BeginDocSelect;
  StartPage(DC);
  OnPage := true;
end;  { BeginDoc }

procedure TRPPrinterDevice.BeginDocSelect;
var
  TempTitle: string;
  DocInfo: TDocInfo;
begin { BeginDocSelect }
  CheckNotPrinting;
  ResetHandle(false);
  State := dsDC;
  FPrinting := true;
  FAborted := false;
  Canvas.Refresh;
  TempTitle := Title + #0;
  with DocInfo do begin
    cbSize := SizeOf(DocInfo);
    lpszDocName := @TempTitle[1];
    lpszOutput := FOutputFile;
    lpszDataType := nil;
    fwType := 0;
  end; { with }
  StartDoc(DC,DocInfo);
  OnPage := false;
end;  { BeginDocSelect }

procedure TRPPrinterDevice.EndDoc;
begin { EndDoc }
  CheckPrinting;
  if OnPage then begin
    EndPage(DC);
  end; { if }
  if not Aborted then begin
    Windows.EndDoc(DC);
  end; { if }
  State := dsNone;
  FPrinting := false;
  FAborted := false;
	OutputFile := '';
end;  { EndDoc }

procedure TRPPrinterDevice.Abort;
begin { Abort }
  CheckPrinting;
  AbortDoc(DC);
  FAborted := true;
  EndDoc;
end;  { Abort }

procedure TRPPrinterDevice.ResetHandle(Force: boolean);
begin { ResetHandle }
  if ((State = dsIC) and DevModeChanged) or Force then begin
    DevModeChanged := false;
    ReleaseCanvas;
    if Assigned(FDevMode) then begin
      ResetDC(DC,FDevMode^);
    end; { if }
    RecoverCanvas;
  end; { if }
end;  { ResetHandle }

procedure TRPPrinterDevice.NewPage;
begin { NewPage }
  CheckPrinting;
  EndPage(DC);
  if DevModeChanged then begin
    ResetHandle(true);
  end; { if }
  StartPage(DC);
end;  { NewPage }

procedure TRPPrinterDevice.NewPageSelect(PageValid: boolean);
begin { NewPageSelect }
  CheckPrinting;
  if OnPage then begin
    EndPage(DC);
  end; { if }
  if DevModeChanged then begin
    ResetHandle(true);
  end else begin
    FCanvas.Refresh;
  end; { else }
  if PageValid then begin
    StartPage(DC);
  end; { if }
  OnPage := PageValid;
end;  { NewPageSelect }

{ Capability Methods }

{ Paper Bin Methods }

function TRPPrinterDevice.GetBins: TStrings;
type
  TBinName = array[1..24] of char;
  TBinBuf = array[1..512] of TBinName;
  TBinNumBuf = array[1..512] of word;
var
  Bins: integer;
  Bin: integer;
  BinNum: longint;
  BinBuf: ^TBinBuf;
  BinNumBuf: ^TBinNumBuf;
begin { GetBins }
  if FBinList = nil then begin
    State := dsIC;
    Bins := DeviceCapabilities(FDeviceName,FOutputName,DC_BINNAMES,nil,nil);
    FBinList := TStringList.Create;
    GetMem(BinBuf,Bins * SizeOf(TBinName));
    GetMem(BinNumBuf,Bins * SizeOf(word));
    try
      try
        if DeviceCapabilities(FDeviceName,FOutputName,DC_BINNAMES,
         PChar(BinBuf),nil) >= 0 then begin
          DeviceCapabilities(FDeviceName,FOutputName,DC_BINS,
           PChar(BinNumBuf),nil);
          for Bin := 1 to Bins do begin
            BinNum := BinNumBuf^[Bin];
//!!PORT!! Added StrPas back in to get it to compile.  MBCS issue
            FBinList.AddObject(StrPas(@BinBuf^[Bin,1]),pointer(BinNum));
          end; { for }
        end; { if }
      finally
        FreeMem(BinNumBuf,Bins * SizeOf(word));
        FreeMem(BinBuf,Bins * SizeOf(TBinName));
      end; { tryf }
    except
      FreeAndNil(FBinList);
      Raise;
    end; { tryx }
  end; { if }
  Result := FBinList;
end;  { GetBins }

function TRPPrinterDevice.SelectBin(BinName: string; Exact: boolean): boolean;
var
  Bin: integer;
  Found: boolean;
  BinNum: longint;
begin { SelectBin }
{ Look for BinName in FBinList }
  GetBins;
  Found := false;
  for Bin := 1 to FBinList.Count do begin
    if Exact then begin
      Found := (FBinList[Bin - 1] = BinName);
    end else begin
      Found := (AnsiPos(AnsiUpperCase(BinName),AnsiUpperCase(FBinList[Bin - 1])) > 0);
    end; { else }
    if Found then begin
      BinNum := longint(FBinList.Objects[Bin - 1]);
      if DevMode <> nil then begin
        if FDevMode^.dmDefaultSource <> BinNum then begin
          FDevMode^.dmDefaultSource := BinNum;
          FDevMode^.dmFields := FDevMode^.dmFields or DM_DEFAULTSOURCE;
          DevModeChanged := true;
        end; { if }
      end else begin
        Found := false;
      end; { else }
      Break;
    end; { if }
  end; { for }
  Result := Found;
end;  { SelectBin }

function TRPPrinterDevice.SupportBin(BinNum: integer): boolean;
var
  Bin: integer;
  Found: boolean;
begin { SupportBin }
  Result := (DevMode <> nil) and ((DevMode^.dmFields and DM_DEFAULTSOURCE) > 0);
  if Result then begin
  { Look for BinNum in FBinList }
    GetBins;
    Found := false;
    for Bin := 1 to FBinList.Count do begin
      Found := (longint(FBinList.Objects[Bin - 1]) = BinNum);
      if Found then begin
        Break;
      end; { if }
    end; { for }
    Result := Found;
  end; { if }
end;  { SupportBin }

{ Paper Size Methods }

function TRPPrinterDevice.GetPapers: TStrings;
type
  TPaperName = array[1..64] of char;
  TPaperBuf = array[1..512] of TPaperName;
  TPaperNumBuf = array[1..512] of word;
var
  Papers: integer;
  Paper: integer;
  PaperNum: longint;
  PaperBuf: ^TPaperBuf;
  PaperNumBuf: ^TPaperNumBuf;
begin { GetPapers }
  if FPaperList = nil then begin
    State := dsIC;
    Papers := DeviceCapabilities(FDeviceName,FOutputName,DC_PAPERNAMES,nil,nil);
    FPaperList := TStringList.Create;
    GetMem(PaperBuf,Papers * SizeOf(TPaperName));
    GetMem(PaperNumBuf,Papers * SizeOf(word));
    try
      try
        if DeviceCapabilities(FDeviceName,FOutputName,DC_PAPERNAMES,
         PChar(PaperBuf),nil) >= 0 then begin
          DeviceCapabilities(FDeviceName,FOutputName,DC_PAPERS,
           PChar(PaperNumBuf),nil);
          for Paper := 1 to Papers do begin
            PaperNum := PaperNumBuf^[Paper];
//!!PORT!! Added StrPas back in to get it to compile.  MBCS issue
            FPaperList.AddObject(StrPas(@PaperBuf^[Paper,1]),pointer(PaperNum));
          end; { for }
        end; { if }
      finally
        FreeMem(PaperNumBuf,Papers * SizeOf(word));
        FreeMem(PaperBuf,Papers * SizeOf(TPaperName));
      end; { tryf }
    except
      FreeAndNil(FPaperList);
      Raise;
    end; { tryx }
  end; { if }
  Result := FPaperList;
end;  { GetPapers }

function TRPPrinterDevice.SelectPaper(PaperName: string; Exact: boolean): boolean;
var
  Paper: integer;
  Found: boolean;
  PaperNum: longint;
begin { SelectPaper }
  CheckNotPrinting;

{ Look for PaperName in FPaperList }
  GetPapers;
  Found := false;
  for Paper := 1 to FPaperList.Count do begin
    if Exact then begin
      Found := (FPaperList[Paper - 1] = PaperName);
    end else begin
      Found := (AnsiPos(AnsiUpperCase(PaperName),AnsiUpperCase(FPaperList[Paper - 1])) > 0);
    end; { else }
    if Found then begin
      PaperNum := longint(FPaperList.Objects[Paper - 1]);
      if DevMode <> nil then begin
        if FDevMode^.dmPaperSize <> PaperNum then begin
          FDevMode^.dmPaperSize := PaperNum;
          FDevMode^.dmFields := FDevMode^.dmFields or DM_PAPERSIZE;
          DevModeChanged := true;
        end; { if }
      end else begin
        Found := false;
      end; { else }
      Break;
    end; { if }
  end; { for }
  Result := Found;
end;  { SelectPaper }

function TRPPrinterDevice.SupportPaper(PaperNum: integer): boolean;
var
  Paper: integer;
  Found: boolean;
begin { SupportPaper }
  Result := (DevMode <> nil) and ((DevMode^.dmFields and DM_PAPERSIZE) > 0);
  if Result then begin
  { Look for PaperNum in FPaperList }
    GetPapers;
    Found := false;
    for Paper := 1 to FPaperList.Count do begin
      Found := (longint(FPaperList.Objects[Paper - 1]) = PaperNum);
      if Found then begin
        Break;
      end; { if }
    end; { for }
    Result := Found;
  end; { if }
end;  { SupportPaper }

{ Misc capability methods }

function EnumFontsProc(var LogFont: TLogFont;
                       var TextMetric: TTextMetric;
                           FontType: integer;
                           Data: pointer): integer; stdcall;
begin { EnumFontsProc }
  TStrings(Data).Add(LogFont.lfFaceName);
  Result := 1;
end;  { EnumFontsProc }

function TRPPrinterDevice.GetFonts: TStrings;
var
  EnumProc: TFarProc;
begin { GetFonts }
  if FFontList = nil then begin
    State := dsIC;
    FFontList := TStringList.Create;
    try
      EnumProc := @EnumFontsProc;
      EnumFonts(DC,nil,EnumProc,pointer(FFontList));
    except
      FreeAndNil(FFontList);
      Raise;
    end; { tryx }
  end; { if }
  Result := FFontList;
end;  { GetFonts }

function TRPPrinterDevice.GetPrinters: TStrings;
const
  DeviceBufInc = 1024;
  DriverBufSize = 256;
var
  Needed: DWORD;
  PrinterInfos: PChar;
  Flags: DWORD;
  Level: DWORD;
  Returned: DWORD;
  PrinterInfo: PChar;
  Port: PChar;
  Ports: PChar;
  I1: integer;
begin { GetPrinters }
  if FPrinterList = nil then begin { Init FPrinterList }
    FPrinterList := TStringList.Create;
    Result := FPrinterList;
    try
      if Win32Platform = VER_PLATFORM_WIN32_NT then begin
        Level := 4;
        Flags := PRINTER_ENUM_LOCAL or PRINTER_ENUM_CONNECTIONS;
      end else begin
        Level := 5;
        Flags := PRINTER_ENUM_LOCAL;
      end; { else }
      Needed := 0;
      EnumPrinters(Flags,nil,Level,nil,0,Needed,Returned);
      if Needed = 0 then Exit;

      GetMem(PrinterInfos,Needed);
      try
        if not EnumPrinters(Flags,nil,Level,PrinterInfos,Needed,Needed,Returned) then begin
          Exit;
        end; { if }
        PrinterInfo := PrinterInfos;
        for I1 := 0 to Returned - 1 do begin
          if Win32Platform = VER_PLATFORM_WIN32_NT then begin
            with PPrinterInfo4(PrinterInfo)^ do begin
              FPrinterList.AddObject(pPrinterName,
               TRPDeviceItem.Create(pPrinterName,nil,nil));
              Inc(PrinterInfo,SizeOf(TPrinterInfo4));
            end; { with }
          end else begin
            with PPrinterInfo5(PrinterInfo)^ do begin
              Ports := pPortName;
              Port := GetWord(Ports);
              while Assigned(Port) do begin
                FPrinterList.AddObject(Format({Trans-}'%s on %s',[pPrinterName,Port]),
                 TRPDeviceItem.Create(pPrinterName,nil,Port));
                Port := GetWord(Ports);
              end; { while }
              Inc(PrinterInfo,SizeOf(TPrinterInfo5));
            end; { with }
          end; { else }
        end; { for }
      finally
        FreeMem(PrinterInfos,Needed);
      end; { tryf }
    except
      FreeAndNil(FPrinterList);
      Raise;
    end; { tryx }
  end; { if }
  Result := FPrinterList;
end;  { GetPrinters }

function TRPPrinterDevice.SelectPrinter(PrinterName: string; Exact: boolean): boolean;
var
  Printer: integer;
  Found: boolean;
begin { SelectPrinter }
  CheckNotPrinting;

{ Look for PrinterName in FPrinterList }
  GetPrinters;
  Found := false;
  for Printer := 1 to FPrinterList.Count do begin
    with TRPDeviceItem(FPrinterList.Objects[Printer - 1]) do begin
      if Exact then begin
        if Pos({Trans-}' on ',PrinterName) > 0 then begin
          Found := (Pos(AnsiUpperCase(PrinterName),
           AnsiUpperCase(StrPas(FDeviceName) + {Trans-}' on ' + StrPas(FOutputName))) > 0);
        end else begin
          Found := (FDeviceName = PrinterName);
        end; { else }
      end else begin
        Found := (AnsiPos(AnsiUpperCase(PrinterName),
         AnsiUpperCase(FDeviceName + {Trans-}' on ' + FOutputName)) > 0);
      end; { else }
    end; { with }
    if Found then begin
      SetDeviceIndex(Printer - 1);
      Break;
    end; { if }
  end; { for }
  Result := Found;
end;  { SelectPrinter }

{ Orientation methods }

function TRPPrinterDevice.GetOrientation: TOrientation;
begin { GetOrientation }
  State := dsIC;
  if DevMode <> nil then begin
    Result := TOrientation(FDevMode^.dmOrientation - 1);
  end else begin
    Result := TOrientation(poPortrait);
  end; { else }
end;  { GetOrientation }

procedure TRPPrinterDevice.SetOrientation(Value: TOrientation);
begin { SetOrientation }
  if (DevMode <> nil) and (Value <> poDefault) and
   (FDevMode^.dmOrientation <> (Ord(Value) + 1)) then begin
    FDevMode^.dmOrientation := Ord(Value) + 1;
    FDevMode^.dmFields := FDevMode^.dmFields or DM_ORIENTATION;
    DevModeChanged := true;
  end; { if }
end;  { SetOrientation }

function TRPPrinterDevice.SupportOrientation: boolean;
begin { SupportOrientation }
  Result := (DevMode <> nil) and ((DevMode^.dmFields and DM_ORIENTATION) > 0);
end;  { SupportOrientation }

{ Copies methods }

function TRPPrinterDevice.GetMaxCopies: longint;
begin { GetMaxCopies }
  if InvalidPrinter then begin
    Result := 1;
    Exit;
  end; { if }
  State := dsIC;
  Result := DeviceCapabilities(FDeviceName,FOutputName,DC_COPIES,nil,nil);
end;  { GetMaxCopies }

procedure TRPPrinterDevice.SetCopies(Value: integer);
begin { SetCopies }
  CheckNotPrinting;
  if (DevMode <> nil) and (Value > 0) and (Value < MaxCopies) and
   (Value <> FDevMode^.dmCopies) then begin
    FDevMode^.dmCopies := Value;
    FDevMode^.dmFields := FDevMode^.dmFields or DM_COPIES;
    DevModeChanged := true;
  end; { if }
end;  { SetCopies }

function TRPPrinterDevice.GetCopies: integer;
begin { GetCopies }
  if DevMode <> nil then begin
    Result := FDevMode^.dmCopies;
    if Result = 0 then begin { Some printers return 0 ?!? }
      Result := 1;
    end; { if }
  end else begin
    Result := 1;
  end; { else }
end;  { GetCopies }

{ Duplex methods }

function TRPPrinterDevice.GetDuplex: TDuplex;
begin { GetDuplex }
  if (DevMode <> nil) and (FDevMode^.dmDuplex in [1..3]) then begin
    Result := TDuplex(FDevMode^.dmDuplex - 1);
  end else begin
    Result := dupSimplex;
  end; { else }
end;  { GetDuplex }

procedure TRPPrinterDevice.SetDuplex(Value: TDuplex);
begin { SetDuplex }
  CheckNotPrinting;
  if (DevMode <> nil) and SupportDuplex and
   (FDevMode^.dmDuplex <> (Ord(Value) + 1)) then begin
    FDevMode^.dmDuplex := Ord(Value) + 1;
    FDevMode^.dmFields := FDevMode^.dmFields or DM_DUPLEX;
    DevModeChanged := true;
  end; { if }
end;  { SetDuplex }

function TRPPrinterDevice.SupportDuplex: boolean;
begin { SupportDuplex }
  Result := (DevMode <> nil) and ((DevMode^.dmFields and DM_DUPLEX) > 0);
end;  { SupportDuplex }

{ Collate methods }

function TRPPrinterDevice.GetCollate: boolean;
begin { GetCollate }
  if DevMode <> nil then begin
    Result := FDevMode^.dmCollate = DMCOLLATE_TRUE;
  end else begin
    Result := false;
  end; { else }
end;  { GetCollate }

procedure TRPPrinterDevice.SetCollate(Value: boolean);
begin { SetCollate }
  CheckNotPrinting;
  if (DevMode <> nil) and SupportCollate then begin
    if Value <> (FDevMode^.dmCollate = DMCOLLATE_TRUE) then begin
      if Value then begin
        FDevMode^.dmCollate := DMCOLLATE_TRUE;
      end else begin
        FDevMode^.dmCollate := DMCOLLATE_FALSE;
      end; { else }
      FDevMode^.dmFields := FDevMode^.dmFields or DM_COLLATE;
      DevModeChanged := true;
    end; { if }
  end; { if }
end;  { SetCollate }

function TRPPrinterDevice.SupportCollate: boolean;
begin { SupportCollate }
  Result := (DevMode <> nil) and ((DevMode^.dmFields and DM_COLLATE) > 0);
end;  { SupportCollate }

{ Page size methods }

function TRPPrinterDevice.GetPageHeight: integer;
begin { GetPageHeight }
  State := dsIC;
  ResetHandle(false);
  Result := GetDeviceCaps(DC,VertRes);
end;  { GetPageHeight }

function TRPPrinterDevice.GetPageWidth: integer;
begin { GetPageWidth }
  State := dsIC;
  ResetHandle(false);
  Result := GetDeviceCaps(DC,HorzRes);
end;  { GetPageWidth }

procedure TRPPrinterDevice.GetCustomExtents(var MinExtent: TPoint;
                                            var MaxExtent: TPoint);
var
  L1: longint;
begin { GetCustomExtents }
  L1 := DeviceCapabilities(FDeviceName,FOutputName,DC_MINEXTENT,nil,nil);
  if L1 = -1 then begin
    MinExtent.X := 1;
    MinExtent.Y := 1;
  end else begin
    MinExtent.X := LoWord(L1);
    MinExtent.Y := HiWord(L1);
  end; { else }
  if MinExtent.Y = 0 then begin
    MinExtent.Y := MinExtent.X;
  end; { if }
  L1 := DeviceCapabilities(FDeviceName,FOutputName,DC_MAXEXTENT,nil,nil);
  if L1 = -1 then begin
    MaxExtent.X := 32767;
    MaxExtent.Y := 32767;
  end else begin
    MaxExtent.X := LoWord(L1);
    MaxExtent.Y := HiWord(L1);
  end; { else }
  if MaxExtent.Y = 0 then begin
    MaxExtent.Y := MaxExtent.X;
  end; { if }
end;  { GetCustomExtents }

function TRPPrinterDevice.SetPaperSize(Size, Width, Height: integer): boolean;
var
  MinExtent: TPoint;
  MaxExtent: TPoint;
begin { SetPaperSize }
  CheckNotPrinting;

  Result := false;
  if DevMode = nil then Exit;
  if (Width <> 0) and (Height <> 0) then begin
    if (FDevMode^.dmPaperSize <> DMPAPER_USER) or
     (FDevMode^.dmPaperWidth <> Width) or
     (FDevMode^.dmPaperLength <> Height) then begin
    { See if values are within min and max extents }
      GetCustomExtents(MinExtent,MaxExtent);
      if (Width >= MinExtent.X) and (Width <= MaxExtent.X) and
       (Height >= MinExtent.Y) and (Height <= MaxExtent.Y) then begin
        FDevMode^.dmPaperSize := DMPAPER_USER;
        FDevMode^.dmPaperWidth := Width;
        FDevMode^.dmPaperLength := Height;
        FDevMode^.dmFields := FDevMode^.dmFields or DM_PAPERSIZE or
         DM_PAPERWIDTH or DM_PAPERLENGTH;
        DevModeChanged := true;
        Result := true;
      end; { if }
    end else begin
      Result := true;
    end; { else }
  end else if SupportPaper(Size) then begin
    if ((FDevMode^.dmPaperSize <> Size) or (FDevMode^.dmPaperWidth <> 0) or (FDevMode^.dmPaperLength <> 0)) then begin
      FDevMode^.dmPaperSize := Size;
      FDevMode^.dmPaperWidth := 0;
      FDevMode^.dmPaperLength := 0;
      FDevMode^.dmFields := FDevMode^.dmFields or DM_PAPERSIZE or
       DM_PAPERWIDTH or DM_PAPERLENGTH;
      DevModeChanged := true;
    end; { if }
    Result := true;
  end; { else }
end;  { SetPaperSize }

function TRPPrinterDevice.InvalidPrinter: boolean;
begin { InvalidPrinter }
  Result := FInvalidPrinter or (Printers.Count = 0) or (DeviceIndex < 0);
  FInvalidPrinter := Result;
end;  { InvalidPrinter }

function TRPPrinterDevice.GetXDPI: integer;
begin { GetXDPI }
  State := dsIC;
  Result := GetDeviceCaps(Handle,LOGPIXELSX);
if Result = 0 then Result := 100; //!!!
end;  { GetXDPI }

function TRPPrinterDevice.GetYDPI: integer;
begin { GetYDPI }
  State := dsIC;
  Result := GetDeviceCaps(Handle,LOGPIXELSY);
if Result = 0 then Result := 100; //!!!
end;  { GetYDPI }

function TRPPrinterDevice.GetWaste: TRect;
var
  PrintOffset: TPoint;
  PageSize: TPoint;
begin { GetWaste }
  State := dsIC;
  PrintOffset.X := GetDeviceCaps(Handle,PHYSICALOFFSETX);
  PrintOffset.Y := GetDeviceCaps(Handle,PHYSICALOFFSETY);
  PageSize.X := GetDeviceCaps(Handle,PHYSICALWIDTH);
  PageSize.Y := GetDeviceCaps(Handle,PHYSICALHEIGHT);
  Result.Left := PrintOffset.X;
  Result.Right := (PageSize.X - (GetDeviceCaps(Handle,HORZRES) + PrintOffset.X));
  Result.Top := PrintOffset.Y;
  Result.Bottom := (PageSize.Y - (GetDeviceCaps(Handle,VERTRES) + PrintOffset.Y));
end;  { GetWaste }

{ Persistent device object management routines }

var
  FRPDev: TRPBaseDevice;

function GlobalDevice: TRPBaseDevice;
begin { GlobalDevice }
  if FRPDev = nil then begin
    FRPDev := TRPPrinterDevice.Create;
  end; { if }
  Result := FRPDev;
end;  { GlobalDevice }

function RPDev: TRPPrinterDevice;
begin { RPDev }
  GlobalDevice;
  if FRPDev is TRPPrinterDevice then begin
    Result := TRPPrinterDevice(FRPDev);
  end else begin
    Result := nil;
  end; { else }
end;  { RPDev }

function SetNewDevice(NewDevice: TRPBaseDevice): TRPBaseDevice;
begin { SetNewDevice }
  Result := FRPDev;
  FRPDev := NewDevice;
end;  { SetNewDevice }

initialization
  FRPDev := nil;
finalization
  if FRPDev <> nil then begin
    FreeAndNil(FRPDev);
  end; { if }
end.