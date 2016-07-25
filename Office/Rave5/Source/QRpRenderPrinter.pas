{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpRenderPrinter;

interface

uses
  Windows,
  Qt, Types, QGraphics,
  Classes, SysUtils, QRpRender, QRpRenderCanvas, QRpDefine;

type
  TRvRenderPrinter = class(TRvRenderCanvas)
  protected
    FCopies: integer;
    FCollate: boolean;
    FDuplex: TDuplex;
    FIgnoreFileSettings: boolean;
    FPages: integer;

    function GetCanvas: TCanvas; override;

    // Drawing Methods
  
    procedure Arc(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double); override;
    procedure BrushChanged(Sender: TObject); override;
    procedure Chord(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double); override;
    procedure CenterText(const Text: string; const X1, Y1: double); override;
    procedure Ellipse(const X1, Y1, X2, Y2: double); override;
    procedure FontChanged(Sender: TObject); override;
    procedure LeftText(const Text: string; const X1, Y1: double); override;
    procedure LineTo(const X1, Y1: double); override;
    procedure MoveTo(const X1, Y1: double); override;
    procedure Pie(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double); override;
    procedure PolyLine(const PolyLineArr: array of TFloatPoint); override;
    procedure Polygon(const PolyLineArr: array of TFloatPoint); override;
    procedure RightText(const Text: string; const X1, Y1: double); override;
    procedure PenChanged(Sender: TObject); override;
    procedure PrintBitmapRect(const X1, Y1, X2, Y2: double; Graphic: TBitmap); override;
    procedure PrintBitmap(const X1, Y1, ScaleX, ScaleY: double; Graphic: TBitmap); override;
    procedure StretchDraw(const Rect: TRect; Graphic: TGraphic); override;
    procedure Draw(const X1, Y1: double; Graphic: TGraphic); override;
    procedure PrintSpaces(const Text: string; const X1, Y1, Width: double); override;
    procedure Rectangle(const X1, Y1, X2, Y2: double); override;
    procedure FillRect(const Rect: TRect); override;
    procedure TextRect(Rect: TRect; X1, Y1: double; S1: string); override;
    procedure RoundRect(const X1, Y1, X2, Y2, X3, Y3: double); override;

  // Status Update Methods
    procedure DocBegin; override;
    procedure DocEnd; override;
    procedure PageBegin; override;
    procedure PageEnd; override;
    function GetPages: integer;
    function GetMaxCopies: integer;
    function GetXDPI: integer; override;
    function GetYDPI: integer; override;
    procedure SelectBin(ABin: string); override;
    procedure SelectBin(ABin: integer); override;
    procedure PrintData(Value: string); override;
    procedure PrintDataStream(Stream: TStream;
                              BufSize: longint); override;

  public
    constructor Create(AOwner: TComponent); override;

    property Pages: integer read GetPages write FPages;
    property Copies: integer read FCopies write FCopies;
    property Collate: boolean read FCollate write FCollate;
    property Duplex: TDuplex read FDuplex write FDuplex;
    property IgnoreFileSettings: boolean read FIgnoreFileSettings write FIgnoreFileSettings;
    property MaxCopies: integer read GetMaxCopies;
    function XI2D(Pos: double): integer; override;
    function YI2D(Pos: double): integer; override;
  published
    property Active default false;
    property OutputFileName;
  end;

implementation

uses
  QRpDevice;

{ TRvRenderPrinter }

procedure TRvRenderPrinter.Arc(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double);
begin
  Windows.Arc(RpDev.Handle,XI2D(X1),YI2D(Y1),XI2D(X2),YI2D(Y2),XI2D(X3),YI2D(Y3),XI2D(X4),YI2D(Y4));
end;

procedure TRvRenderPrinter.BrushChanged(Sender: TObject);
var
  Brush: TBrush;
  NewBrush: HBrush;
  NewLogBrush: LOGBRUSH;
begin
  Brush := TBrush(Sender);
  with NewLogBrush do begin
  NewLogBrush.lbColor := ColorToRGB(Brush.Color);
    case Brush.Style of
      bsSolid: begin
        lbStyle := BS_SOLID;
        // lbHatch is ignored with BS_SOLID
      end;
      bsClear: begin
        lbStyle := BS_HOLLOW;
        // lbHatch is ignored with BS_HOLLOW
      end;
      bsHorizontal: begin
        lbStyle := BS_HATCHED;
        lbHatch := HS_HORIZONTAL;
      end;
      bsVertical: begin
        lbStyle := BS_HATCHED;
        lbHatch := HS_VERTICAL;
      end;
      bsFDiagonal: begin
        lbStyle := BS_HATCHED;
        lbHatch := HS_FDIAGONAL;
      end;
      bsBDiagonal: begin
        lbStyle := BS_HATCHED;
        lbHatch := HS_BDIAGONAL;
      end;
      bsCross: begin
        lbStyle := BS_HATCHED;
        lbHatch := HS_CROSS;
      end;
      bsDiagCross: begin
        lbStyle := BS_HATCHED;
        lbHatch := HS_DIAGCROSS;
      end;
    end; { case }
  end; { with }
  NewBrush := CreateBrushIndirect(NewLogBrush);
  DeleteObject(SelectObject(RpDev.Handle,NewBrush));
end;

procedure TRvRenderPrinter.CenterText(const Text: string; const X1, Y1: double);
begin
  Windows.SetTextAlign(RpDev.Handle,Windows.TA_CENTER or Windows.TA_NOUPDATECP or Windows.TA_BASELINE);
  Windows.ExtTextOut(RpDev.Handle,XI2D(X1),YI2D(Y1),0,nil,PChar(Text),Length(Text),nil);
end;

procedure TRvRenderPrinter.Chord(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double);
begin
  Windows.Chord(RpDev.Handle,XI2D(X1),YI2D(Y1),XI2D(X2),YI2D(Y2),XI2D(X3),YI2D(Y3),XI2D(X4),YI2D(Y4));
end;

constructor TRvRenderPrinter.Create(AOwner: TComponent);
begin
  inherited;
  FActive := false;
end;

procedure TRvRenderPrinter.DocBegin;
begin
  RpDev.Title := Converter.Title;
  RpDev.Orientation := Converter.ReportHeader.Orientation;
  RpDev.SetPaperSize(Converter.ReportHeader.DMPaperSize,Converter.ReportHeader.DMPaperLength,
   Converter.ReportHeader.DMPaperWidth);
  if IgnoreFileSettings then begin
    RpDev.Copies := Copies;
    RpDev.Collate := Collate;
    RpDev.Duplex := Duplex;
  end else begin
    RpDev.Copies := Converter.ReportHeader.Copies;
    RpDev.Collate := Converter.ReportHeader.Collate;
    RpDev.Duplex := Converter.ReportHeader.Duplex;
  end; { else }
  RpDev.OutputFile := OutputFileName;
  RpDev.BeginDocSelect;
end;

procedure TRvRenderPrinter.DocEnd;
begin
  DeleteObject(SelectObject(RpDev.Handle,GetStockObject(BLACK_PEN)));
  DeleteObject(SelectObject(RpDev.Handle,GetStockObject(SYSTEM_FONT)));
  DeleteObject(SelectObject(RpDev.Handle,GetStockObject(BLACK_BRUSH)));
  RpDev.EndDoc;
end;

procedure TRvRenderPrinter.Draw(const X1, Y1: double; Graphic: TGraphic);
begin
end;

procedure TRvRenderPrinter.Ellipse(const X1, Y1, X2, Y2: double);
begin
  Windows.Ellipse(RpDev.Handle,XI2D(X1),YI2D(Y1),XI2D(X2),YI2D(Y2));
end;

procedure TRvRenderPrinter.FillRect(const Rect: TRect);
begin
end;

procedure TRvRenderPrinter.FontChanged(Sender: TObject);
begin
  DeleteObject(SelectObject(RpDev.Handle,CreateFont));
  SetTextColor(RpDev.Handle, ColorToRGB(TFont(Sender).Color));
end;

function TRvRenderPrinter.GetCanvas: TCanvas;
begin
  Result := RPDev.Canvas;
end;

procedure TRvRenderPrinter.LeftText(const Text: string; const X1, Y1: double);
begin
  Windows.SetTextAlign(RpDev.Handle,Windows.TA_LEFT or Windows.TA_NOUPDATECP or Windows.TA_BASELINE);
  Windows.ExtTextOut(RpDev.Handle,XI2D(X1),YI2D(Y1),0,nil,PChar(Text),Length(Text),nil);
end;

function TRvRenderPrinter.GetPages: integer;
begin
  if Assigned(Converter) and (Converter.ReportHeader.PageCount > 0) then begin
    Result := Converter.ReportHeader.PageCount;
  end else begin
    Result := FPages;
  end; { else }
end;

procedure TRvRenderPrinter.LineTo(const X1, Y1: double);
begin
  Windows.LineTo(RpDev.Handle,XI2D(X1),YI2D(Y1));
end;

procedure TRvRenderPrinter.MoveTo(const X1, Y1: double);
begin
  Windows.MoveToEx(RpDev.Handle,XI2D(X1),YI2D(Y1),nil);
end;

procedure TRvRenderPrinter.PageBegin;
begin
  RpDev.Orientation := Converter.ReportHeader.Orientation;
  RpDev.DevMode.dmDefaultSource := Converter.ReportHeader.Bin;
  RpDev.NewPageSelect(true);
end;

procedure TRvRenderPrinter.PageEnd;
begin
end;

procedure TRvRenderPrinter.PenChanged(Sender: TObject);
var
  Pen: TPen;
  NewPen: HPen;
begin
  Pen := TPen(Sender);
  Pen.Width := Round(PenWidth * XDPI);
  NewPen := CreatePen(Ord(Pen.Style), Pen.Width, Pen.Color);
  DeleteObject(SelectObject(RpDev.Handle,NewPen));
end;

procedure TRvRenderPrinter.Pie(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double);
begin
  Windows.Pie(RpDev.Handle,XI2D(X1),YI2D(Y1),XI2D(X2),YI2D(Y2),XI2D(X3),YI2D(Y3),XI2D(X4),YI2D(Y4));
end;

procedure TRvRenderPrinter.Polygon(const PolyLineArr: array of TFloatPoint);
begin
end;

procedure TRvRenderPrinter.PolyLine(const PolyLineArr: array of TFloatPoint);
begin
end;

procedure TRvRenderPrinter.PrintBitmap(const X1, Y1, ScaleX, ScaleY: double; Graphic: TBitmap);
begin
end;

procedure TRvRenderPrinter.PrintBitmapRect(const X1, Y1, X2, Y2: double; Graphic: TBitmap);
var
  BM: Windows.TBitmap;
  Info: PBitmapInfo;
  InfoSize: DWORD;
  ImageSize: DWORD;
  Image: pointer;
  ImageHandle: THandle;
  Palette: HPALETTE;
  LastPalette: HPALETTE;
  DC: HDC;
begin
  GetObject(QPixmap_hbm(Graphic.Handle),SizeOf(BM),@BM);
  InfoSize := SizeOf(TBitmapInfoHeader) + (SizeOf(TRGBQuad) shl (BM.bmPlanes * BM.bmBitsPixel));
  GetMem(Info,InfoSize);
  try
    with Info^.bmiHeader do begin
      biSize := SizeOf(TBitmapInfoHeader);
      biWidth := BM.bmWidth;
      biHeight := BM.bmHeight;
      biPlanes := 1;
      biBitCount := BM.bmPlanes * BM.bmBitsPixel;
      biCompression := BI_RGB;
      biSizeImage := (((biWidth * biBitCount) + 31) and not 31) div 8 * biHeight;
      biXPelsPerMeter := 0;
      biYPelsPerMeter := 0;
      biClrUsed := 0;
      biClrImportant := 0;
    end; { with }
    ImageSize := Info^.bmiHeader.biSizeImage;
    if ImageSize < 65535 then begin
      GetMem(Image,ImageSize);
      ImageHandle := 0;
    end else begin
      ImageHandle := GlobalAlloc(GMEM_MOVEABLE,ImageSize);
      Image := GlobalLock(ImageHandle);
    end; { else }
    try
      Palette := GetStockObject(DEFAULT_PALETTE);
      DC := CreateCompatibleDC(0);
      LastPalette := SelectPalette(DC,Palette,false);
      RealizePalette(DC);
      GetDIBits(DC,QPixmap_hbm(Graphic.Handle),0,Info^.bmiHeader.biHeight,Image,Info^,DIB_RGB_COLORS);
      if LastPalette <> 0 then begin
        SelectPalette(DC,LastPalette,false);
      end; { if }
      DeleteDC(DC);
      StretchDIBits(RpDev.Handle,XI2D(X1),YI2D(Y1),XI2D(X2) - XI2D(X1),YI2D(Y2) - YI2D(Y1),0,0,
       Info^.bmiHeader.biWidth,Info^.bmiHeader.biHeight,Image,Info^,DIB_RGB_COLORS,SRCCOPY);
    finally
      if ImageSize < 65535 then begin
        FreeMem(Image,ImageSize);
      end else begin
        GlobalUnlock(ImageHandle);
        GlobalFree(ImageHandle);
      end; { else }
    end; { try }
  finally
    FreeMem(Info,InfoSize);
  end; { try }
end;

procedure TRvRenderPrinter.PrintSpaces(const Text: string; const X1, Y1, Width: double);
begin
  LeftText(Text,X1,Y1);
end;

procedure TRvRenderPrinter.Rectangle(const X1, Y1, X2, Y2: double);
begin
  Windows.Rectangle(RpDev.Handle,XI2D(X1),YI2D(Y1),XI2D(X2),YI2D(Y2));
end;

procedure TRvRenderPrinter.RightText(const Text: string; const X1, Y1: double);
begin
  Windows.SetTextAlign(RpDev.Handle,Windows.TA_RIGHT or Windows.TA_NOUPDATECP or Windows.TA_BASELINE);
  Windows.ExtTextOut(RpDev.Handle,XI2D(X1),YI2D(Y1),0,nil,PChar(Text),Length(Text),nil);
end;

procedure TRvRenderPrinter.RoundRect(const X1, Y1, X2, Y2, X3, Y3: double);
begin
  Windows.RoundRect(RpDev.Handle,XI2D(X1),YI2D(Y1),XI2D(X2),YI2D(Y2),XI2D(X3),YI2D(Y3));
end;

procedure TRvRenderPrinter.StretchDraw(const Rect: TRect; Graphic: TGraphic);
begin
  if Graphic is TMetaFile then begin
    PlayEnhMetaFile(RpDev.Handle,TMetaFile(Graphic).Handle,Rect);
  end; { if }
end;

procedure TRvRenderPrinter.TextRect(Rect: TRect; X1, Y1: double; S1: string);
begin
end;

function TRvRenderPrinter.GetMaxCopies: integer;
begin
  Result := RpDev.MaxCopies;
end;

function TRvRenderPrinter.GetXDPI: integer;
begin
  Result := RpDev.XDPI;
end;

function TRvRenderPrinter.GetYDPI: integer;
begin
  Result := RpDev.YDPI;
end;

procedure TRvRenderPrinter.SelectBin(ABin: string);
begin { SelectBin }
  RpDev.SelectBin(ABin, false);
end;  { SelectBin }

procedure TRvRenderPrinter.SelectBin(ABin: integer);
var
  I1: integer;
begin
  if Assigned(RpDev.Bins) then begin
    for I1 := 0 to RpDev.Bins.Count - 1 do begin
      if integer(RpDev.Bins.Objects[I1]) = ABin then begin
        SelectBin(RpDev.Bins[I1]);
      end;
    end; { for }
  end; { if }
end;

procedure TRvRenderPrinter.PrintData(Value: string);
begin { PrintData }
  if Value = '' then Exit;
  RpDev.RawOut(Value[1],Length(Value));
end;  { PrintData }

function TRvRenderPrinter.XI2D(Pos: double): integer;
begin
  Result := Round((Pos - (RpDev.Waste.Left / XDPI)) * XDPI);
end;

function TRvRenderPrinter.YI2D(Pos: double): integer;
begin
  Result := Round((Pos - (RpDev.Waste.Top / XDPI)) * YDPI);
end;

procedure TRvRenderPrinter.PrintDataStream(Stream: TStream;
                                        BufSize: longint);
const
  BufMax = 4096;
type
  PBuffer = ^TBuffer;
  TBuffer = record
    Size: word;
    Data: array[1..BufMax] of byte;
  end; { TBuffer }
var
  Buffer: PBuffer;
  I1: integer;
  I2: integer;
begin { PrintDataStream }
  if BufSize = 0 then begin
    BufSize := Stream.Size - Stream.Position;
  end; { if }
  New(Buffer);
  try
    I2 := (BufSize div BufMax) + 1;
    for I1 := 1 to I2 do begin
    { Figure out size of this chunk }
      if I1 = I2 then begin { Last piece }
        Buffer^.Size := BufSize mod BufMax;
      end else begin { BufMax sized chunk }
        Buffer^.Size := BufMax;
      end; { else }

      if Buffer^.Size > 0 then begin
      { Move data from stream to buffer and pass to Escape }
        Stream.ReadBuffer(Buffer^.Data,Buffer^.Size);
        RpDev.RawOut(Buffer^.Data,Buffer^.Size);
      end; { if }
    end; { for }
  finally
    Dispose(Buffer);
  end; { tryf }
end;  { PrintDataStream }

end.