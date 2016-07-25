{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpRenderPS;

interface

uses
  QGraphics, Types,
  Classes,
  SysUtils,
  QRpRender,
  QRpDefine;

const
  DPI_MULTIPLIER = 72;

type
  TEllipsePts = record
    XA,XB,XC,XD,XE,
    YA,YB,YC,YD,YE: double
  end;
  TSmallArcRecord = record
    X1, X2, X3, X4: double;
    Y1, Y2, Y3, Y4: double;
  end;
  TSmallArcBuffer = array[1..4] of TSmallArcRecord;

  TRvRenderPS = class(TRPRenderStream)
  private
    FPageHeight: double;
    FPageWidth: double;
    FPenColor: TColor;
    FLastPenColor: TColor;
    FLastPenColorDefined: boolean;
    FLastPenWidth: double;
    FLastPenWidthDefined: boolean;
    FLastTextColor: TColor;
    FLastTextColorDefined: boolean;
    FBrushColor: TColor;
    FLastPenStyle: TPenStyle;
    FLastPenStyleDefined: boolean;
    SmallArcData: TSmallArcBuffer;

    function GetPageHeight: double;
    function GetPageWidth: double;
    procedure SetPageHeight(const AValue: double);
    procedure SetPageWidth(const AValue: double);
    function GetPenColor: TColor;
    procedure SetPenColor(const AValue: TColor);
    function GetBrushColor: TColor;
    procedure SetBrushColor(const AValue: TColor);
    function GetDoFill: boolean;
    function GetDoStroke: boolean;
  protected
    FDataStream: TMemoryStream;
    FOutlineList: TStringList;
    FPageList: TStringList;
    FFontList: TStringList;
    FXRefList: TStringList;
    FontBitmap: TBitmap;

    XRefPos: longint;
    FOpenMode: string;
    FFontSize: integer;
    FFontRotation: double;
    FUnderlineFont: boolean;
    FStrikeoutFont: boolean;
    FFrameMode : TFrameMode;
    FUseCompression: boolean;

    FirstNewPage: boolean;
    //
    FOnDecodeImage: TDecodeImageEvent;
    FMetafileDPI: integer;
    //
    function GetEllipsePts(const AX1, AY1, AX2, AY2: double): TEllipsePts;
    //
    procedure PrintPenColor;
    procedure PrintBrushColor;
    procedure NewPage;
    function TrimAll(const AValue: string): string;
    function FormatPSText(const AValue: string): string;
    function InchesToUnitsX(const AValue: double): double;
    function InchesToUnitsY(const AValue: double): double;
    procedure SetGraphicColor(AStrokeColor: TColor; ABrushColor: TColor);
    function FormatEx(const AData: string; const AArgs: array of const): string;
    procedure PrintF(const AData: string; const AArgs: array of const); override;
    procedure PrintLnF(const AData: string; const AArgs: array of const); override;
    procedure SetPenAndBrush;

    property PageWidth: double read GetPageWidth write SetPageWidth;
    property PageHeight: double read GetPageHeight write SetPageHeight;
    property FontRotation: double read FFontRotation write FFontRotation;
    property PenColor: TColor read GetPenColor write SetPenColor;
    property BrushColor: TColor read GetBrushColor write SetBrushColor;
    property DataStream: TMemoryStream read FDataStream;
    property UnderlineFont: boolean read FUnderlineFont write FUnderlineFont;
    property StrikeoutFont: boolean read FStrikeoutFont write FStrikeoutFont;
    property FontSize: integer read FFontSize write FFontSize;
    property PageList: TStringList read FPageList;
    property FontList: TStringList read FFontList;
    property LastTextColor: TColor read FLastTextColor write FLastTextColor;
    property LastPenColorDefined: boolean read FLastPenColorDefined write FLastPenColorDefined;
    property LastTextColorDefined: boolean read FLastTextColorDefined write FLastTextColorDefined;
    property LastPenWidthDefined: boolean read FLastPenWidthDefined write FLastPenWidthDefined;
    procedure RotateXYCoordinate(AX1, AY1, AAngle: double; var xRot, yRot: double);
    function DegreesToRadians(ADegrees: double): double;
    procedure RadiusArc(const AX1, AY1, AR1, sAngle, eAngle: double);
    procedure SmallArc(AX1, AY1, AR1, MidTheta, HalfAngle: double; ccwcw: double; AIndex: integer);
    procedure PSArc(const AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4: double; ADoClosePath, AAllowFill: boolean);
    procedure PSLineTo(const pfX1, pfY1: double; AClosePath: boolean);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

    function GetPSColor(Color: TColor): string;
    procedure PrintTextColor;
    procedure PrintLeft(const AX, AY: double; AText: string);
    procedure PrintCenter(const AX, AY: double; AText: string);
    procedure PrintRight(const AX, AY: double; AText: string);

    procedure InitData;
    procedure FreeData;
    procedure ClosePath;

    procedure FontChanged(Sender: TObject); override;
    procedure LeftText(const psText: string; const pfX, pfY: double); override;
    procedure CenterText(const psText: string; const pfX, pfY: double); override;
    procedure RightText(const psText: string; const pfX, pfY: double); override;
    procedure TextRect(Rect: TRect; X1,Y1: double;S1: string); override;
    procedure PrintSpaces(const psText: string; const pfX, pfY, pfWidth: double); override;
    procedure Arc(const AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4: double); override;
    procedure Chord(const AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4: double); override;
    procedure Rectangle(const X1, Y1, X2, Y2: double); override;
    procedure FillRect(const pRect: TRect); override;
    procedure RoundRect(const pfX1, pfY1, pfX2, pfY2, pfX3, pfY3: double); override;
    procedure Ellipse(const AX1, AY1, AX2, AY2: double); override;
    procedure MoveTo(const pfX1, pfY1: double); override;
    procedure LineTo(const pfX1, pfY1: double); override;
    procedure CurveTo(const AX1, AY1, AX2, AY2, AX3, AY3: double);
    procedure PrintBitmapRect(const X1, Y1, X2, Y2: double; AGraphic:
      Graphics.TBitmap); override;
    procedure PrintImageRect(X1,Y1,X2,Y2: double;ImageStream: TStream;ImageType: string); override;
    procedure PrintBitmap(const X1,Y1,ScaleX,ScaleY: double; AGraphic:
      Graphics.TBitmap); override;
    procedure StretchDraw(const Rect: TRect; AGraphic: TGraphic); override;
    procedure Draw(const pfX1, pfY1: double; AGraphic: TGraphic); override;
    procedure PrintJPG(const AX1, AY1, AX2, AY2: double; AName: string);
    procedure Pie(const AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4: double); override;
    //
    procedure DocBegin; override;
    procedure DocEnd; override;
    procedure PageBegin; override;
    procedure PageEnd; override;
    //
    property DoStroke: boolean read GetDoStroke;
    property DoFill: boolean read GetDoFill;
    property LastPenColor: TColor read FLastPenColor write FLastPenColor;
    property LastPenStyle: TPenStyle read FLastPenStyle write FLastPenStyle;
    property LastPenStyleDefined: boolean read FLastPenStyleDefined
                                          write FLastPenStyleDefined;
    property LastPenWidth: double read FLastPenWidth write FLastPenWidth;
  published
    property UseCompression: boolean read fUseCompression write fUseCompression default false;
    property OnDecodeImage: TDecodeImageEvent read FOnDecodeImage write
     FOnDecodeImage;
  end;

implementation

{ TRvRenderPS }

constructor TRvRenderPS.Create(AOwner: TComponent);
begin
  inherited;
  DisplayName := 'Adobe PostScript (PS)';
  FileExtension := '*.ps';
  FontBitmap := TBitmap.Create;
  FontBitmap.Width := 50;
  FontBitmap.Height := 50;
  FontBitmap.Canvas.Font.PixelsPerInch := 96;
end;

destructor TRvRenderPS.Destroy;
begin
  inherited;
  FreeAndNil(FontBitmap);
end;

procedure TRvRenderPS.PageBegin;
begin
  if FirstNewPage then begin
    FirstNewPage := false;
  end else begin
    NewPage;
  end;
  Print('%%');
  PrintLnF('Page: %d %d',[Converter.PageNo,Converter.PageNo]);
  PrintLn('%%BeginPageSetup');
  PrintLn('/pgsave save def');
  Print('%%');
  PrintLnF('BoundingBox: 0 0 %.0f %.0f',[PageWidth * DPI_MULTIPLIER,
                                          PageHeight * DPI_MULTIPLIER]);
  PrintLn('%%EndPageSetup');
end;

procedure TRvRenderPS.PageEnd;
begin
  PrintLn('pgsave restore');
  PrintLn('showpage');
// this is   Print('%%');
// optional  PrintLn('PageTrailer');
end;

function TRvRenderPS.InchesToUnitsX(const AValue: double): double;
begin
  Result := AValue * DPI_MULTIPLIER;
end;

function TRvRenderPS.InchesToUnitsY(const AValue: double): double;
begin
  Result := (PageHeight - AValue) * DPI_MULTIPLIER;
end;

procedure TRvRenderPS.LeftText(const psText: string; const pfX, pfY: double);
begin
  PrintLeft(pfX, pfY, psText);
end;

procedure TRvRenderPS.CenterText(const psText: string; const pfX, pfY: double);
begin
  PrintCenter(pfX, pfY, psText);
end;

procedure TRvRenderPS.RightText(const psText: string; const pfX, pfY: double);
begin
  PrintRight(pfX, pfY, psText);
end;

procedure TRvRenderPS.PrintSpaces(const psText: string; const pfX, pfY, pfWidth: double);
begin
  {TODO This is temp}
  PrintLeft(pfX, pfY, psText);
end;

procedure TRvRenderPS.TextRect(Rect: TRect; X1,Y1: double; S1: string);
begin
end;

procedure TRvRenderPS.Rectangle(const X1, Y1, X2, Y2: double);
//var
//  RectType: string;
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;

  SetPenAndBrush;
  MoveTo(X1, Y1);
  PSLineTo(X2, Y1, false);
  PSLineTo(X2, Y2, false);
  PSLineTo(X1, Y2, false);
  if not DoFill then begin
    PSLineTo(X1, Y1, false);
  end; { if }
  ClosePath;
end;

procedure TRvRenderPS.MoveTo(const pfX1, pfY1: double);
begin
  PrintLnF('%.0f %.0f m',[InchesToUnitsX(pfX1),InchesToUnitsY(pfY1)]);
end;

procedure TRvRenderPS.LineTo(const pfX1, pfY1: double);
var
  BrushStyle: TBrushStyle;
  BrushColor: TColor;
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  BrushStyle := Converter.Brush.Style;
  Converter.Brush.Style := bsClear;
  SetPenAndBrush;
  PrintLnF('%.0f %.0f l sk',[InchesToUnitsX(pfX1),InchesToUnitsY(pfY1)]);

  Converter.Brush.Style := BrushStyle;
  Converter.Brush.Color := BrushColor;
end;

procedure TRvRenderPS.PSLineTo(const pfX1, pfY1: double; AClosePath: boolean);
begin
  PrintF('%.0f %.0f l ',[InchesToUnitsX(pfX1),InchesToUnitsY(pfY1)]);
  if AClosePath then begin
    ClosePath;
  end; { if }
end;

procedure TRvRenderPS.CurveTo(const AX1, AY1, AX2, AY2, AX3, AY3: double);
begin
  PrintLnF('%.4f %.4f %.4f %.4f %.4f %.4f c',[
          InchesToUnitsX(AX1), InchesToUnitsY(AY1),
          InchesToUnitsX(AX2), InchesToUnitsY(AY2),
          InchesToUnitsX(AX3), InchesToUnitsY(AY3)]);
end;

procedure TRvRenderPS.DocEnd;
begin { DocEnd }
  PrintLn('%%Trailer');
  PrintLn('%%EndDocument');
  PrintLn('%%EOF');
  FreeFileStream;
end;  { DocEnd }

procedure TRvRenderPS.DocBegin;
begin
  PrintLn('%!PS-Adobe-1.0');
  Print('%%');
  PrintLnF('Pages: %d',[Converter.PageCount]);
  PrintLn('%%BeginDocument');
  PrintLn('/s {show} bind def');
  PrintLn('/l {lineto} bind def');
  PrintLn('/lw {setlinewidth} bind def');
  PrintLn('/m {newpath moveto} bind def');
  PrintLn('/c {curveto} bind def');
  PrintLn('/r {rotate} bind def');
  PrintLn('/sd {setdash} bind def');
  PrintLn('/sr {setrgbcolor} bind def');
  PrintLn('/sk {stroke} bind def');
  PrintLn('/cp {closepath} bind def');
  PrintLn('/gs {gsave} bind def');
  PrintLn('/gr {grestore} bind def');
  PrintLn('/f {fill} bind def');
end; { DocBegin }

procedure TRvRenderPS.SetGraphicColor(AStrokeColor: TColor;
                                      ABrushColor: TColor);
begin
  PenColor := AStrokeColor;
  BrushColor := ABrushColor;
end;

procedure TRvRenderPS.Ellipse(const AX1, AY1, AX2, AY2: double);
var
  EllipsePts: TEllipsePts;
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;

  SetPenAndBrush;

  EllipsePts := GetEllipsePts(AX1, AY1, AX2, AY2);

  with EllipsePts do begin
    MoveTo(XC,YA);
    CurveTo(XD,YA,XE,YB,XE,YC);
    CurveTo(XE,YD,XD,YE,XC,YE);
    CurveTo(XB,YE,XA,YD,XA,YC);
    CurveTo(XA,YB,XB,YA,XC,YA);
  end; { with }
  ClosePath;
end; { Ellipse }

procedure TRvRenderPS.PrintBitmapRect(const X1, Y1, X2, Y2: double;
  AGraphic: Graphics.TBitmap);
var
  ImageStream: TMemoryStream;
  HexStr: string;
  I1: integer;
  I2: byte;
  DataOffset: byte;
  PicData: string;
begin
  PicData := '';
  ImageStream := TMemoryStream.Create;
  try
    AGraphic.SaveToStream(ImageStream);
    DataOffset := 0;
    ImageStream.Position := DataOffset;
    for I1 := 0 to ImageStream.Size - DataOffset - 1 do begin
      ImageStream.Read(I2,1);
      HexStr := IntToHex(I2,2);
      PicData := PicData + HexStr;
    end;
    // write image data

    PrintLn('gs');
    PrintLn('/DeviceRGB setcolorspace');
    PrintLnF('%.0f %.0f translate',[InchesToUnitsX(X1),InchesToUnitsY(Y1)]);
    PrintLnF('%.0f %.0f scale',[Abs(X1 - X2) * DPI_MULTIPLIER,Abs(Y1 - Y2) * DPI_MULTIPLIER]);
    PrintLn('<<');
    PrintLn('  /ImageType 1');
    PrintLnF('  /Width %d',[AGraphic.Width]);
    PrintLnF('  /Height %d',[AGraphic.Height]);
    PrintLn('  /BitsPerComponent 8');
    PrintLn('  /Decode[0 1 0 1 0 1]');
    PrintLnF('  /ImageMatrix[%d 0 0 %d 0 %d]',[AGraphic.Width,
                                              AGraphic.Height,
                                              AGraphic.Height]);
    PrintLn('  /DataSource currentfile');
    PrintLn('  /ASCIIHexDecode filter');
    PrintLn('>>');
    PrintLn('image');
    while Length(PicData) > 0 do begin
      PrintLn(Copy(PicData, 1, 64));
      delete(PicData, 1, 64);
    end; { while }
    PrintLn('> gr');

//    PrintLn('gs');
(*
    PrintLnF('/picstr %d string def',[AGraphic.Width]);
    PrintLnF('%.0f %.0f translate',[InchesToUnitsX(X1),InchesToUnitsY(Y1)]);
    PrintLnF('%.0f %.0f scale',[Abs(X1 - X2) * DPI_MULTIPLIER,Abs(Y1 - Y2) * DPI_MULTIPLIER]);
    PrintLnF('gs %d %d 4',[AGraphic.Width, AGraphic.Height]);
    PrintLnF('[%d 0 0 %d 0 %d]',[AGraphic.Width, AGraphic.Height, AGraphic.Height]);
    PrintLn('{ currentfile picstr readhexstring pop}');
    PrintLn('image gr');
    while Length(PicData) > 0 do begin
      PrintLn(Copy(PicData, 1, 64));
      delete(PicData, 1, 64);
    end; { while }
*)
  finally
    FreeAndNil(ImageStream);
  end; { tryf }
end;

procedure TRvRenderPS.PrintBitmap(const X1,Y1,ScaleX,ScaleY: double;
  AGraphic: Graphics.TBitmap);
begin
end;

procedure TRvRenderPS.FontChanged(Sender: TObject);
begin
  FontRotation := Converter.FontData.Rotation;
end;

function TRvRenderPS.GetEllipsePts(const AX1, AY1, AX2, AY2: double): TEllipsePts;
var
  RvRenderPS: TRvRenderPS;
  Factor: double;
  Offset: double;
begin
  RvRenderPS := TRvRenderPS(Owner);
  with Result do begin
    case RvRenderPS.FrameMode of
      fmInside: Offset := RvRenderPS.PenWidth / 2;
      fmSplit: Offset := 0;
      fmOutside: Offset := -(RvRenderPS.PenWidth / 2);
      else Offset := 0;
    end; { case }
    Factor := (4.0 / 3.0) * (Sqrt(2) - 1.0);
    //Factor := (Sqrt(2) - 1.0) / 3 * 4;
    if (AX1 + Offset) < (AX2 - Offset) then begin
      XA := AX1 + Offset;
    end else begin
      XA := AX2 - Offset;
    end; { else }
    XC := ((AX1 + Offset) + (AX2 - Offset)) / 2.0;
    if (AX1 + Offset) > (AX2 - Offset) then begin
      XE := AX1 + Offset;
    end else begin
      XE := AX2 - Offset;
    end; { else }
    XB := XC - Factor * (XC - XA);
    XD := XC + Factor * (XC - XA);

    if (AY1 + Offset) < (AY2 - Offset) then begin
      YA := AY1 + Offset;
    end else begin
      YA := AY2 - Offset;
    end; { else }
    YC := ((AY1 + Offset) + (AY2 - Offset)) / 2.0;
    if (AY1 + Offset) > (AY2 - Offset) then begin
      YE := AY1 + Offset;
    end else begin
      YE := AY2 - Offset;
    end; { else }
    YB := YC - Factor * (YC - YA);
    YD := YC + Factor * (YC - YA);
  end; { with }
end;

procedure TRvRenderPS.NewPage;
begin
end;

procedure TRvRenderPS.PrintPenColor;
begin
  PrintLnF('%s sr',[GetPSColor(PenColor)]);
end;

procedure TRvRenderPS.PrintBrushColor;
begin
  PrintLnF('%s sr',[GetPSColor(BrushColor)]);
end;

function TRvRenderPS.GetPageHeight: double;
begin
  Result := PaperHeight;
end;

function TRvRenderPS.GetPageWidth: double;
begin
  Result := PaperWidth;
end;

procedure TRvRenderPS.SetPageHeight(const AValue: double);
begin
  FPageHeight := AValue;
end;

procedure TRvRenderPS.SetPageWidth(const AValue: double);
begin
  FPageWidth := AValue;
end;

function TRvRenderPS.FormatPSText(const AValue: string): string;
var
  s1: string;
  i1: integer;
begin
  s1 := AValue;
  for i1 := Length(s1) downto 1 do begin
    if (s1[i1] = '(') or (s1[i1] = ')') or (s1[i1] = '\') then begin
      Insert('\', s1, i1);
    end; { if }
  end; { for }
  Result := s1;
end;

function TRvRenderPS.TrimAll(const AValue: string): string;
var
  I1: integer;
begin
  I1 := Length(AValue);
  Result := AValue;
  while I1 > 1 do begin
    if Result[I1] = ' ' then begin
      Delete(Result,I1, 1);
    end;
    Dec(I1);
  end;
end;

function TRvRenderPS.GetPSColor(Color: TColor): string;
var
  sColor: string;
  dRed: double;
  dGreen: double;
  dBlue: double;
begin
  sColor := IntToHex(Color, 8);
  dBlue := StrToInt('$' + Copy(sColor, 3,2)) / 255;
  dGreen := StrToInt('$' + Copy(sColor, 5,2)) / 255;
  dRed := StrToInt('$' + Copy(sColor, 7, 2)) / 255;
  sColor := FormatEx('%.3g %.3g %.3g', [dRed, dGreen, dBlue]);
  Result := sColor;
end;

procedure TRvRenderPS.PrintTextColor;
begin
  PrintLnF('%s sr',[GetPSColor(Converter.Font.Color)]);
end;

procedure TRvRenderPS.PrintLeft(const AX, AY: double; AText: string);
var
  FontName: string;
  Font: TFont;
begin
  Font := Converter.Font;
  FontName := Font.Name;
  if (fsBold in Font.Style) and (fsItalic in Font.Style) then begin
    FontName := FontName + '-BoldOblique';
  end else if (fsBold in Font.Style) then begin
    FontName := FontName + 'Bold'
  end else if (fsItalic in Font.Style) then begin
    FontName := FontName + '-Oblique'
  end;
  PrintLnF('/%s findfont',[TrimAll(FontName)]);
  PrintLnF('%d scalefont setfont',[Converter.Font.Size]);

  if Converter.Font.Color <> LastTextColor then begin
    PrintTextColor;
    LastTextColor := Converter.Font.Color;
  end; { if }

  if FontRotation <> 0 then begin
    PrintLn('gs');
  end; { if }
  PrintLnF('%.0f %.0f m',[InchesToUnitsX(AX), InchesToUnitsY(AY)]);

  PrintLnF('%.0f r',[FontRotation]);

  PrintLnF('(%s) show',[FormatPSText(AText)]);
  if FontRotation <> 0 then begin
    PrintLn('gr');
  end; { if }
end;

procedure TRvRenderPS.PrintCenter(const AX, AY: double; AText: string);
var
  FontName: string;
  StrWidth: double;
begin
  FontBitmap.Canvas.Font := Converter.Font;
  StrWidth := FontBitmap.Canvas.TextWidth(AText) / 100;
  FontName := TrimAll(Converter.Font.Name);

  PrintLeft(AX - StrWidth / 2, AY, AText);
end;

procedure TRvRenderPS.PrintRight(const AX, AY: double; AText: string);
var
  FontName: string;
  StrWidth: double;
begin
  FontBitmap.Canvas.Font := Converter.Font;
  StrWidth := FontBitmap.Canvas.TextWidth(AText) / 100;
  FontName := TrimAll(Converter.Font.Name);

  PrintLeft(AX - StrWidth, AY, AText);
end;

function TRvRenderPS.GetPenColor: TColor;
begin
  Result := FPenColor;
end;

procedure TRvRenderPS.SetPenColor(const AValue: TColor);
begin
  FPenColor := AValue;
end;

function TRvRenderPS.GetBrushColor: TColor;
begin
  Result := FBrushColor;
end;

procedure TRvRenderPS.SetBrushColor(const AValue: TColor);
begin
  FBrushColor := AValue;
end;

procedure TRvRenderPS.PrintJPG(const AX1, AY1, AX2, AY2: double; AName: string);
begin
end;

function TRvRenderPS.FormatEx(const AData: string;
                              const AArgs: array of const): string;
var
  OrigSeparator: char;
begin
  OrigSeparator := DecimalSeparator;
  DecimalSeparator := '.';
  Result := Format(AData, AArgs);
  DecimalSeparator := OrigSeparator;
end;

procedure TRvRenderPS.PrintF(const AData: string;
                             const AArgs: array of const);
begin
  Print(FormatEx(AData, AArgs));
end;

procedure TRvRenderPS.PrintLnF(const AData: string;
                               const AArgs: array of const);
begin
  PrintLn(FormatEx(AData, AArgs));
end;

procedure TRvRenderPS.SetPenAndBrush;
begin
  if not LastPenStyleDefined or (Converter.Pen.Style <> LastPenStyle) then begin
    case Converter.Pen.Style of
      psDash: PrintLn('[13 6] 0 sd');
      psDot: PrintLn('[2] 0 sd');
      psDashDot: PrintLn('[8 4 1 4] 0 sd');
      psDashDotDot: PrintLn('[10 3 2 2 2 3] 0 sd');
      else begin
        PrintLn('[] 0 sd');
      end;
    end; { case }
    LastPenStyle := Converter.Pen.Style;
    LastPenStyleDefined := true;
  end; { if }
  if not LastPenColorDefined or (PenColor <> LastPenColor) then begin
    PrintPenColor;
    LastPenColor := PenColor;
    LastPenColorDefined := true;
    LastTextColorDefined := false;
  end; { if }
  if not LastPenWidthDefined or (PenWidth <> LastPenWidth) then begin
    PrintLnF('%.4f lw ', [PenWidth * DPI_MULTIPLIER]); // %Set linewidth
    LastPenWidth := PenWidth;
    LastPenWidthDefined := true;
  end;
end;

procedure TRvRenderPS.RoundRect(const pfX1, pfY1, pfX2, pfY2, pfX3, pfY3: double);
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
end;

procedure TRvRenderPS.SmallArc(AX1, AY1, AR1, MidTheta, HalfAngle, ccwcw: double; AIndex: integer);
var
  vCos, vSin: double;
  x0, y0, x1, y1, x2, y2, x3, y3: double;
  HalfTheta: double;
begin
  HalfTheta := DegreesToRadians(Abs(halfAngle));
  vCos := Cos(HalfTheta);
  vSin := Sin(HalfTheta);

  x0 := AR1 * vCos;
  y0 := -ccwcw * AR1 * vSin;
  x1 := AR1 * (4.0 - vCos) / 3.0;
  x2 := x1;
  y1 := AR1 * ccwcw * (1.0 - vCos) * (vCos - 3.0) / (3.0 * vSin);
  y2 := -y1;
  x3 := AR1 * vCos;
  y3 := ccwcw * AR1 * vSin;

  RotateXYCoordinate(x0,y0,MidTheta,x0,y0);
  RotateXYCoordinate(x1,y1,MidTheta,x1,y1);
  RotateXYCoordinate(x2,y2,MidTheta,x2,y2);
  RotateXYCoordinate(x3,y3,MidTheta,x3,y3);

  SmallArcData[AIndex].X1 := AX1 + x0;
  SmallArcData[AIndex].Y1 := AY1 - y0;
  SmallArcData[AIndex].X2 := AX1 + x1;
  SmallArcData[AIndex].Y2 := AY1 - y1;
  SmallArcData[AIndex].X3 := AX1 + x2;
  SmallArcData[AIndex].Y3 := AY1 - y2;
  SmallArcData[AIndex].X4 := AX1 + x3;
  SmallArcData[AIndex].Y4 := AY1 - y3;

//MoveTo(AX1+x0,AY1-y0);
//CurveTo(AX1+x1,AY1-y1,AX1+x2,AY1-y2,AX1+x3,AY1-y3);
end;

procedure TRvRenderPS.RotateXYCoordinate(AX1, AY1, AAngle: double; var xRot, yRot: double);
var
  Theta, rCos, rSin: double; //cos, sine of theta
begin
  Theta := DegreesToRadians(AAngle);
  rCos := Cos(Theta);
  rSin := Sin(Theta);
  xRot := (rCos * AX1) - (rSin * AY1);
  yRot := (rSin * AX1) + (rCos * AY1);
end;

function TRvRenderPS.DegreesToRadians(ADegrees: double): double;
begin
  Result := ADegrees * System.Pi / 180;
end;

procedure TRvRenderPS.RadiusArc(const AX1, AY1, AR1, sAngle, eAngle: double);
var
  i1: integer;
  numArcs: integer;
  ccwcw: double;
  angleBump: double;
  halfBump: double;
  aSpan: double;
  curAngle: double;
begin
  if sAngle <> eAngle then begin
    numArcs := 1;
    ccwcw := 1.0;

    aSpan := eAngle - sAngle;
    if aSpan < 0 then begin
      aSpan := 360 + aSpan;
    end;

    if eAngle < sAngle then begin
      ccwcw := -1.0;
    end; { if }
    while Abs(aSpan) / numArcs > 90.0 do begin
      Inc(numArcs);
    end; { while }
    angleBump := aSpan / numArcs;
    halfBump := 0.5 * angleBump;
    curAngle := sAngle + halfBump;
    for i1 := 1 to numArcs do begin
      SmallArc(AX1,AY1,AR1,curAngle,halfBump, ccwcw, i1);
      curAngle := curAngle + angleBump;
    end;
    if (eAngle - sAngle) < 0 then begin
      moveto(SmallArcData[numArcs].X1, SmallArcData[numArcs].Y1);
      for i1 := numArcs downto 1 do begin
        with SmallArcData[i1] do begin
          CurveTo(X2, Y2, X3, Y3, X4, Y4);
        end; { with }
      end; { for }
    end else begin
      moveto(SmallArcData[1].X1, SmallArcData[1].Y1);
      for i1 := 1 to numArcs do begin
        with SmallArcData[i1] do begin
          CurveTo(X2, Y2, X3, Y3, X4, Y4);
        end; { with }
      end; { for }
    end;
  end;
end;

procedure TRvRenderPS.PSArc(const AX1, AY1, AX2, AY2, AX3, AY3,
 AX4, AY4: double; ADoClosePath, AAllowFill: boolean);
var
  X1, Y1: double;
  XC, YC: double;
  A1, A2: double;
  R1: double;

  function VectorAngle(XC,YC: double;
                       XT,YT: double): double;
  begin { VectorAngle }
    if XT > XC then begin
      Result := 360.0 - (ArcTan((YT - YC) / (XT - XC)) * 180.0 / Pi);
      if Result > 360.0 then begin
        Result := Result - 360.0;
      end; { if }
    end else if XT < XC then begin
      Result := 180.0 - (ArcTan((YT - YC) / (XT - XC)) * 180.0 / Pi);
    end else begin
      if YT < YC then begin
        Result := 90.0;
      end else begin
        Result := 270.0;
      end; { else }
    end; { else }
  end;  { VectorAngle }

begin
  SetPenAndBrush;
//  PrintLn('% arc code');
  X1 := (AX1 + AX2)/ 2;
  Y1 := (AY1 + AY2) / 2;
  R1 := (AX2 - AX1) / 2;
  XC := (AX1 + AX2) / 2;
  YC := (AY1 + AY2) / 2;
  A1 := VectorAngle(XC,YC,AX3,AY3);
  A2 := VectorAngle(XC,YC,AX4,AY4);
  // TODO: Doesn't currently support elliptical arcs
  RadiusArc(X1, Y1, R1, A1, A2);
//    RadiusArc(X1, Y1, R1, 150, 45);
  if ADoClosePath then begin
    if AAllowFill then begin
      ClosePath;
    end else begin
      PrintLn('sk');
    end; { else }
  end; { if }
end;

procedure TRvRenderPS.Arc(const AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4: double);
var
  BrushStyle: TBrushStyle;
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  BrushStyle := Converter.Brush.Style;
  Converter.Brush.Style := bsClear;
  PSArc(AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4, true, false);
  Converter.Brush.Style := BrushStyle;
end;

procedure TRvRenderPS.Chord(const AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4: double);
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  PSArc(AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4, true, true);
end;

function TRvRenderPS.GetDoFill: boolean;
begin
  Result := bsClear <> Converter.Brush.Style;
end;

function TRvRenderPS.GetDoStroke: boolean;
begin
  Result := psClear <> Converter.Pen.Style;
end;

procedure TRvRenderPS.Pie(const AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4: double);
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  SetPenAndBrush;
  PSArc(AX1,AY1,AX2,AY2,AX3,AY3,AX4,AY4,false,true);
  PrintLnF('%.0f %.0f l ',[InchesToUnitsX((AX1 + AX2) / 2.0),InchesToUnitsY((AY1 + AY2) / 2.0)]);
  ClosePath;
end;

procedure TRvRenderPS.FreeData;
begin
  FOwnedStream.Free;
  FOwnedStream := nil;
end;

procedure TRvRenderPS.InitData;
begin
  FirstNewPage := true;
end;

procedure TRvRenderPS.ClosePath;
begin
  if DoStroke and DoFill then begin
    Print('cp gs ');
    PrintBrushColor;
    PrintLn('f gr sk');
  end else if DoFill then begin
    Print('cp gs ');
    PrintBrushColor;
    PrintLn('f gr');
  end else if DoStroke then begin
    PrintLn('sk');
  end; { else }
end;

procedure TRvRenderPS.FillRect(const pRect: TRect);
var
  PenStyle: TPenStyle;
begin
  PenColor := Converter.Pen.Color;
  PenStyle := Converter.Pen.Style;
  Converter.Pen.Style := psClear;
  BrushColor := Converter.Brush.Color;
//
  Converter.Pen.Style := PenStyle;
end;

procedure TRvRenderPS.StretchDraw(const Rect: TRect; AGraphic: TGraphic);
begin
end;

procedure TRvRenderPS.Draw(const pfX1, pfY1: double; AGraphic: TGraphic);
begin
end; { Draw }

procedure TRvRenderPS.PrintImageRect(X1, Y1, X2, Y2: double;
  ImageStream: TStream; ImageType: string);
begin { PrintImageRect }
end;  { PrintImageRect }

end.
