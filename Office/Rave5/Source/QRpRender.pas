{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpRender;

interface

{$I RpVer.inc}

uses
  Types, QGraphics, Qt, QTypes, QExtCtrls,
  Classes, SysUtils, QRpDefine, QRpMBCS;

const
  FontDPI = 1248;

type
  TRPRender = class;

  TRPReportHeader = record
    FileVer: word;
    Title: string;
    PageNo: integer;
    PageCount: integer;
    FirstPage: integer; { First valid page }
    LastPage: integer; { Last valid page }
    PaperWidth: double;
    PaperHeight: double;
    LeftWaste: double;
    RightWaste: double;
    TopWaste: double;
    BottomWaste: double;
    Orientation: TOrientation;
    Bin: smallint;
    Collate: boolean;
    Duplex: TDuplex;
    Copies: smallint;
    DMPaperSize: smallint;
    DMPaperLength: smallint;
    DMPaperWidth: smallint;
    OutputName: string; { Output port for Printer }
  end;

  TRPPageHeader = record
    PrevPagePos: longint;
    NextPagePos: longint;
    PageNo: longint;
    PaperWidth : double;
    PaperHeight : double;
    OldOrientation: byte; { Old orientation byte }
  end;

  TFloatPoint = record
    X: double;
    Y: double;
  end;

  TRPConverter = class(TObject)
  protected
    FontBitmap: QGraphics.TBitmap;
    FReportDateTime: TDateTime; { DateTime was started }
    FFirstPage: integer; { First valid page }
    FLastPage: integer; { Last valid page }
    //
    FPageNo, FPageCount: integer;
    FTitle: string;
    FStream: TStream;
    FRenderer: TRPRender;
    FReuseGraphic: boolean;
    FFileVer: word;
    // Begin Page Info
    FCurrPagePos, FPrevPagePos, FNextPagePos: longint;
    // End Page Info
    //
    FXPos: double; // used for last lineto location
    FYPos: double; // used for last lineto location
    VarList: TStringList;

    procedure GetNextRPTFCommand(RPTFStr: string; var Pos: integer; var Command: char; var Param: string);
    function NextPiece(InStr: string; var Pos: integer): string;
    procedure ParseRPTF(InStr: string; var Pos: integer; var TextStr, RPTFStr: string);
    procedure ProcessRPTF(RPTFStr: string);
    procedure CreateGraphic(var Value: TGraphic); virtual;
    procedure LoadGraphic(Value: TGraphic); virtual;
    procedure MatchCode(pyValue: byte);
    procedure ProcessPage;
    function ReadByte: byte;
    function ReadLongint: longint;
    function ReadPoint: TPoint;
    procedure ReadPageHeader;
    procedure ReadReportHeader;
    function ReadRect: TRect;
    function ReadSmallInt: smallint;
    function ReadSingle: single;
    function ReadString: string;
    property ReportDateTime: TDateTime read FReportDateTime write FReportDateTime;
    function GetDeviceName: string; virtual;
    function GetDriverName: string; virtual;
    function GetPort: string; virtual;
    //
    function ConvertMacro(MacroID: TMacroID): string;
    procedure ReadBuf(var Buf; Len: integer);
    function ReadPIVar: string;
    function GetPIVar(VarName: string): string;
    procedure SetPIVar(VarName, VarValue: string);
    procedure ClearVarList;
    //
    procedure ProcessString(var Text: string);
    property DeviceName: string read GetDeviceName;
    property DriverName: string read GetDriverName;
    property Port: string read GetPort;
    procedure PrintRPTF(AX, AY: double; AText: string; AJustify: char);
    procedure PrintRPTFBlock(AX,AY: double; AText: string; AWidth: double);

    procedure ReadGraphic(Value: TGraphic);
  public
    ReportHeader: TRPReportHeader;
    PageHeader: TRPPageHeader;

    CurrX: single;
    CurrY: single;

    Font: TFont;
    FontData: TSaveFont;
    LineHeight: single;
    Brush: TBrush;
    Pen: TPen;
    //
    function RPTFTextWidth(Value: string): double;
    procedure SetBrushColor;
    procedure SetFontColor;
    procedure SetFontName;
    procedure SetFontHeight;
    procedure SetFontBold;
    procedure SetFontUnderline;
    procedure SetFontItalic;
    procedure SetFontStrikeout;
    procedure SetFontSuperscript;
    procedure SetFontSubscript;
    procedure SetFontPitch;
    procedure SetFontAlign;
    procedure SetFontRotation;
    procedure SetFontCharset;
    procedure DoLeftText;
    procedure DoCenterText;
    procedure DoRightText;
    procedure DoPrintSpaces;
    procedure DoRectangle;
    procedure DoArc;
    procedure DoChord;
    procedure DoEllipse;
    procedure DoMoveTo;
    procedure DoLineTo;
    procedure SetPenColor;
    procedure DoBrushCopy;
    procedure DoCopyRect;
    procedure DoDraw;
    procedure DoDrawFocusRect;
    procedure DoFillRect;
    procedure DoFloodFill;
    procedure DoFrameRect;
    procedure DoPie;
    procedure DoPolygon;
    procedure DoPolyline;
    procedure DoPrintBitmap;
    procedure DoPrintBitmapRect;
    procedure DoRoundRect;
    procedure DoStretchDraw;
    procedure DoTextRect;
    procedure SetFont;
    procedure SetTextBkMode;
    procedure SetBkColor;
    procedure SetScaleX;
    procedure SetScaleY;
    procedure DoPrintData;
    procedure DoPrintDataStream;
    procedure DoTransBitmap;
    procedure DoSetFrameMode;
    procedure DoPrintImageRect;
    procedure DoVariableData;
    procedure SetBrush;
    procedure SetBrushStyle;
    procedure SetBrushBitmap;
    procedure SetPen;
    procedure SetPenMode;
    procedure SetPenStyle;
    procedure SetPenWidth;
    procedure DoResetLineHeight;
    procedure DoAllowPreviewOnly;
    procedure DoAllowPrinterOnly;
    procedure DoAllowAll;
    procedure SetOrientation;
    procedure SetBin;
    //
    constructor Create(AStream: TStream; ARenderer: TRPRender); virtual;
    destructor Destroy; override;
    procedure Generate(const APageNo: integer = 0; APageCount: integer = 0); virtual;
    function PageInRange(PageNum: integer): boolean;

    procedure GotoPage(const APageNum: integer);
    //
    property FileVer: word read FFileVer;
    property Renderer: TRPRender read fRenderer;
    property PageNo: integer read FPageNo;
    property PageCount: integer read FPageCount;
    property Title: string read FTitle;
    property ReuseGraphic: boolean read fReuseGraphic;
  end; { TRPConverter }

  TRPRender = class(TRPComponent)
  protected
    ValidDestination: set of TDestination; { Used with OutputInvalid }
    snPaperWidth, snPaperHeight: double;
    snPenWidth: double;
    snFrameMode: TFrameMode;
    FActive: boolean;
    FDisplayName: string;
    FFileExtension: string;
    FMacroData: TStringList;
    FOutputFileName: TFileName;
    FInputFileName: TFileName;
    FRendering: boolean;
    FNDRStream: TStream;
    FIsInternalNDRStream: boolean;
    FSelection: TFormatString; { Page selection string }
    FFirstPage: integer;
    FLastPage: integer;
    FOnDecodeImage: TDecodeImageEvent;
    //
    Converter: TRPConverter;
    FTextBKMode: TBkMode;
    FBKColor: TColor;
    FTransparentBitmaps: boolean; { Controls bitmap draw mode }

    procedure SetDisplayName(const Value: string);
    function StripRPTF(AValue: string): string;
    procedure SetBKColor(Value: TColor); virtual;
    procedure AllowPreviewOnly; virtual;
    procedure AllowPrinterOnly; virtual;
    procedure AllowAll; virtual;
    function GetOutputInvalid: boolean;
    function Destination: TDestination; virtual;

  // Drawing Methods
    procedure Arc(const X1,Y1,X2,Y2,X3,Y3,X4,Y4: double); virtual;
    procedure BrushChanged(Sender: TObject); virtual;
    procedure Chord(const X1,Y1,X2,Y2,X3,Y3,X4,Y4: double); virtual;
    procedure CenterText(const AText: string; const X1, Y1: double); virtual;
    procedure Ellipse(const X1,Y1,X2,Y2: double); virtual;
    procedure FontChanged(Sender: TObject); virtual;
    procedure LeftText(const AText: string; const X1,Y1: double); virtual;
    procedure LineTo(const X1,Y1: double); virtual;
    procedure MoveTo(const X1,Y1: double); virtual;
    procedure Pie(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double); virtual;
    procedure PolyLine(const PolyLineArr: array of TFloatPoint); virtual;
    procedure Polygon(const PolyLineArr: array of TFloatPoint); virtual;
    procedure RightText(const AText: string; const X1,Y1: double); virtual;
    procedure PenChanged(Sender: TObject); virtual;
    procedure PrintImageRect(X1,Y1,X2,Y2: double; ImageStream: TStream; ImageType: string); virtual;
    procedure PrintBitmapRect(const X1,Y1,X2,Y2: double; AGraphic: QGraphics.TBitmap); virtual;
    procedure PrintBitmap(const X1,Y1,ScaleX,ScaleY: double; AGraphic: QGraphics.TBitmap); virtual;
    procedure StretchDraw(const ARect: TRect; AGraphic: TGraphic); virtual;
    procedure Draw(const X1,Y1: double; AGraphic: TGraphic); virtual;
    procedure PrintSpaces(const AText: string; const X1,Y1,AWidth: double); virtual;
    procedure Rectangle(const X1,Y1,X2,Y2: double); virtual;
    procedure FillRect(const ARect: TRect); virtual;
    procedure TextRect(ARect: TRect; X1,Y1: double;S1: string); virtual;
    procedure RoundRect(const X1,Y1,X2,Y2,X3,Y3: double); virtual;
    function CountSpaces(Text: string): integer; virtual;
    function GetFirstWord(var Line: string): string; virtual;
    function GetSpaces(var Line: string): string; virtual;
    procedure AdjustXY(var X1,Y1: double; Width: double); virtual;
    procedure PrintRightWidth(var X1,Y1: double; Text: string; Width: double); virtual;
    procedure SelectBin(ABin: string); overload; virtual;
    procedure SelectBin(ABin: integer); overload; virtual;
    procedure PrintData(Value: string); virtual;
    procedure PrintDataStream(Stream: TStream;
                              BufSize: longint); virtual;
    procedure FrameRect(const Rect: TRect); virtual;
    procedure BrushCopy(const Dest: TRect;
                              Bitmap: TBitmap;
                        const Source: TRect;
                              Color: TColor); virtual;
    procedure CopyRect(const Dest: TRect;
                             Canvas: TCanvas;
                       const Source: TRect); virtual;
    procedure DrawFocusRect(const Rect: TRect); virtual;

  // Status Update Methods
    procedure DocBegin; virtual;
    procedure DocEnd; virtual;
    procedure PageBegin; virtual;
    procedure PageEnd; virtual;
    function GetXDPI: integer; virtual;
    function GetYDPI: integer; virtual;

    property IsInternalNDRStream: boolean read FIsInternalNDRStream write FIsInternalNDRStream;
    property TransparentBitmaps: boolean read FTransparentBitmaps write FTransparentBitmaps;
    property TextBKMode: TBkMode read FTextBKMode write FTextBKMode;
    property BKColor: TColor write SetBKColor;
    property OutputInvalid: boolean read GetOutputInvalid;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Render(ANDRStream: TStream = nil); overload; virtual;
    procedure Render(AInputFileName: TFileName); overload;
    procedure RenderPage(PageNum: integer); virtual;
    procedure PrintRender(ANDRStream: TStream; AOutputFileName: TFileName); virtual; {$IFDEF LEVEL6}deprecated;{$ENDIF}
    function XI2D(Pos: double): integer; virtual;
    function YI2D(Pos: double): integer; virtual;

    property Selection: string read FSelection write FSelection;
    property FirstPage: integer read FFirstPage write FFirstPage;
    property LastPage: integer read FLastPage write FLastPage;
    property FrameMode: TFrameMode read snFrameMode write snFrameMode;
    property PenWidth: double read snPenWidth write snPenWidth;
    property PaperWidth: double read snPaperWidth write snPaperWidth;
    property PaperHeight: double read snPaperHeight write snPaperHeight;
    property MacroData: TStringList read FMacroData write FMacroData;
    property XDPI: integer read GetXDPI;
    property YDPI: integer read GetYDPI;
    property InputFileName: TFileName read FInputFileName write FInputFileName;
    property OutputFileName: TFileName read FOutputFileName write FOutputFileName;
    property Rendering: boolean read FRendering;
    property NDRStream: TStream read FNDRStream write FNDRStream;
    property OnDecodeImage: TDecodeImageEvent read FOnDecodeImage write FOnDecodeImage;
  published
    property Active: boolean read FActive write FActive default true;
    property DisplayName: string read FDisplayName write SetDisplayName;
    property FileExtension: string read FFileExtension write FFileExtension;
  end; { TRPRender }

  TRPRenderClass = class of TRPRender;

  TRPRenderStream = class(TRPRender)
  protected
    FActiveStream: TStream;
    FOutputStream: TStream;
    FOwnedStream: TStream;
    property ActiveStream: TStream read FActiveStream write FActiveStream;
    procedure SetOutputStream(AStream: TStream);
    procedure Print(const AData: string); virtual;
    procedure PrintF(const AData: string; const AArgs: array of const); virtual;
    procedure PrintLn(const AData: string = ''); virtual;
    procedure PrintLnF(const AData: string; const AArgs: array of const); virtual;
    procedure FreeFileStream; virtual;
  public
    destructor Destroy; override;
    procedure InitFileStream(const AFileName: string);

    property OutputStream: TStream read FOutputStream write SetOutputStream;
    property OwnedStream: TStream read FOwnedStream;
  end; { TRPRenderStream }

type
  TGraphicFormat = (gfJPEG, gfBMP, gfMTF);

  TNDGraphic = class(TGraphic)
  protected
    FPixmap: QPixmapH;
    FImage: QImageH;
    FStream: TStream;
    FCompression: integer;
    FFormat: TGraphicFormat;

    procedure SetCompression(AValue: integer);
    function GetFormatString: string;

    procedure Draw(ACanvas: TCanvas; const Rect: TRect); override;
    function GetEmpty: Boolean; override;
    function GetHeight: integer; override;
    function GetWidth: integer; override;
    procedure SetHeight(Value: integer); override;
    procedure SetWidth(Value: integer); override;

    procedure FreePixmap;
  public
    constructor Create; override;
    destructor Destroy; override;

    procedure Assign(Source: TPersistent); override;
    procedure LoadFromMimeSource(MimeSource: TMimeSource); override;
    procedure SaveToMimeSource(MimeSource: TClxMimeSource); override;
    procedure LoadFromStream(Stream: TStream); override;
    procedure SaveToStream(Stream: TStream); override;
    procedure SaveToFile(const Filename: string); override;
    procedure Compress;

    property CompressionQuality: integer read FCompression write SetCompression;
    property Format: TGraphicFormat read FFormat write FFormat;
    property Handle: QPixmapH read FPixmap;
  end;

  procedure RaveRegisterRender(const DisplayName: string; const RenderObject: TRPRender);
  procedure RaveUnRegisterRender(const DisplayName: string; const RenderObject: TRPRender);
  procedure GetRenderList(const AList: TStrings);
  function GetRenderFilter: string;

var
  RenderList: TStringList = nil;

implementation

{ Global }

procedure RaveRegisterRender(const DisplayName: string; const RenderObject: TRPRender);
begin { RaveRegisterRender }
  if not Assigned(RenderList) then begin
    RenderList := TStringList.Create;
  end;
  RenderList.AddObject(DisplayName, RenderObject);
end;  { RaveRegisterRender }

procedure RaveUnRegisterRender(const DisplayName: string; const RenderObject: TRPRender);
var
  Index: integer;
begin { RaveUnRegisterRender }
  if Assigned(RenderList) then begin
    Index := RenderList.IndexOfObject(RenderObject);
    if Index >= 0 then begin
      RenderList.Delete(Index);
    end; { if }
    if RenderList.Count < 1 then begin
      FreeAndNil(RenderList);
    end; { if }
  end; { if }
end;  { RaveUnRegisterRender }

procedure GetRenderList(const AList: TStrings);
var
  RenderObject: TRPRender;
  I1: integer;
begin { GetRenderList }
  AList.Clear;
  if Assigned(RenderList) then begin
    for I1 := 0 to RenderList.Count - 1 do begin
      RenderObject := TRPRender(RenderList.Objects[I1]);
      if RenderObject.Active then begin
        AList.AddObject(RenderObject.DisplayName, RenderObject);
      end; { if }
    end; { for }
  end; { if }
end;  { GetRenderList }

function GetRenderFilter: string;
var
  RenderObject: TRPRender;
  I1: integer;
begin
  Result := '';
  if Assigned(QRPRender.RenderList) then begin
    with QRPRender.RenderList do begin
      for I1 := 0 to Count - 1 do begin
        RenderObject := TRPRender(QRPRender.RenderList.Objects[I1]);
        if RenderObject.Active then begin
          if Result = '' then begin
            Result := RenderObject.DisplayName + '|' + RenderObject.FileExtension;
          end else begin
            Result := Result + '|' + RenderObject.DisplayName + '|' + RenderObject.FileExtension;
          end; { else }
        end; { if }
      end; { for }
    end; { with }
  end; { if }
end;

{ TRPConverter }

procedure TRPConverter.DoArc;
var
  X1, X2, X3, X4, Y1, Y2, Y3, Y4: single;
begin
  inherited;
  X1 := ReadSingle;
  Y1 := ReadSingle;
  X2 := ReadSingle;
  Y2 := ReadSingle;
  X3 := ReadSingle;
  Y3 := ReadSingle;
  X4 := ReadSingle;
  Y4 := ReadSingle;

  if Renderer.OutputInvalid then Exit;

  Renderer.Arc(X1,Y1,X2,Y2,X3,Y3,X4,Y4);
end;

procedure TRPConverter.SetBrushColor;
begin
  Brush.Color := TColor(ReadLongint);
end;

procedure TRPConverter.DoBrushCopy;
var
  Graphic: TGraphic;
  Rect1,Rect2: TRect;
  L1: integer;
begin
  CreateGraphic(Graphic);
  Rect1 := ReadRect;
  Rect2 := ReadRect;
  L1 := ReadLongint;
  if not Renderer.OutputInvalid then begin
    Renderer.BrushCopy(Rect1,Graphic as TBitmap,Rect2,L1);
  end; { if }
  FreeAndNil(Graphic);
end;

procedure TRPConverter.SetFontColor;
begin
  FontData.Color := TColor(ReadLongint);
  Font.Color := FontData.Color;
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.SetFontName;
begin
  FontData.Name := ReadString;
  Font.Name := FontData.Name;
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.SetFontHeight;
begin
  FontData.Height := ReadSingle;
  if FileVer >= 2000 then begin
    FontData.Width := ReadSingle; { Real width of FontString }
  end;
//  Font.Size := Round(FontHeight * Renderer.YDPI);
  Font.Size := Round(FontData.Height * 72);
  FontData.Size := Font.Size;
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.SetFontBold;
begin
  FontData.Bold := boolean(ReadByte);
  if FontData.Bold then begin
    Font.Style := Font.Style + [fsBold];
  end else begin
    Font.Style := Font.Style - [fsBold];
  end;
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.SetFontUnderline;
begin
  FontData.Underline := boolean(ReadByte);
  if FontData.Underline then begin
    Font.Style := Font.Style + [fsUnderline];
  end else begin
    Font.Style := Font.Style - [fsUnderline];
  end;
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.SetFontItalic;
begin
  FontData.Italic := boolean(ReadByte);
  if FontData.Italic then begin
    Font.Style := Font.Style + [fsItalic];
  end else begin
    Font.Style := Font.Style - [fsItalic];
  end;
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.SetFontStrikeOut;
begin
  FontData.StrikeOut := boolean(ReadByte);
  if FontData.StrikeOut then begin
    Font.Style := Font.Style + [fsStrikeOut];
  end else begin
    Font.Style := Font.Style - [fsStrikeOut];
  end;
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.SetFontSuperscript;
begin
  FontData.Superscript := boolean(ReadByte);
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.SetFontSubscript;
begin
  FontData.Subscript := boolean(ReadByte);
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.SetFontPitch;
begin
  Font.Pitch := TFontPitch(ReadByte);
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.SetFontAlign;
begin
  FontData.Align := TFontAlign(ReadByte);
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.SetFontRotation;
begin
  FontData.Rotation := ReadSmallInt;
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.SetFontCharset;
begin
  FontData.CharSet := ReadByte;
  Font.Charset := VCLToCLXCharSet(FontData.Charset);
  Renderer.FontChanged(Font);
end;

procedure TRPConverter.DoCenterText;
var
  Text: string;
  X1: double;
  Y1: double;
begin
  inherited;
  Y1 := ReadSingle;
  Text := ReadString;
  X1 := ReadSingle;
  if Text <> '' then begin
    if Pos(RPTFPrefix,Text) = 0 then begin
      if Renderer.OutputInvalid then Exit;
      Renderer.CenterText(Text,X1,Y1);
    end else begin
      PrintRPTF(X1,Y1,Text,RPTFCenter);
    end; { else }
  end; { if }
end;

procedure TRPConverter.DoChord;
var
  X1, X2, X3, X4, Y1, Y2, Y3, Y4: single;
begin
  inherited;
  X1 := ReadSingle;
  Y1 := ReadSingle;
  X2 := ReadSingle;
  Y2 := ReadSingle;
  X3 := ReadSingle;
  Y3 := ReadSingle;
  X4 := ReadSingle;
  Y4 := ReadSingle;

  if Renderer.OutputInvalid then Exit;

  Renderer.Chord(X1,Y1,X2,Y2,X3,Y3,X4,Y4);
end;

procedure TRPConverter.DoCopyRect;
var
  Rect1,Rect2: TRect;
  L1: integer;
begin
  Rect1 := ReadRect;
  L1 := ReadLongint;
  Rect2 := ReadRect;

  if Renderer.OutputInvalid then Exit;

  Renderer.CopyRect(Rect1,TCanvas(L1),Rect2);
end;

procedure TRPConverter.DoDraw;
var
  LGraphic: TGraphic;
  X1, Y1: double;
begin
  CreateGraphic(LGraphic);
  try
    X1 := ReadSingle;
    Y1 := ReadSingle;

    if Renderer.OutputInvalid then Exit;

    Renderer.Draw(X1, Y1, LGraphic);
  finally
    LGraphic.Free;
  end;
end;

procedure TRPConverter.DoDrawFocusRect;
var
  Rect: TRect;
begin
  Rect := ReadRect;

  if Renderer.OutputInvalid then Exit;

  Renderer.DrawFocusRect(Rect);
end;

procedure TRPConverter.DoEllipse;
var
  X1, X2, Y1, Y2: single;
begin
  inherited;
  X1 := ReadSingle;
  Y1 := ReadSingle;
  X2 := ReadSingle;
  Y2 := ReadSingle;

  if Renderer.OutputInvalid then Exit;

  Renderer.Ellipse(X1,Y1,X2,Y2);
end;

{ Parse the formatted string InStr and return the next piece of unformatted text
  in TextStr and the RPTF formatting string in RPTFStr }

procedure TRPConverter.ParseRPTF(InStr: string; var Pos: longint; var TextStr: string; var RPTFStr: string);
var
  Ch: char;
begin { ParseRPTF }
  TextStr := '';
  RPTFStr := '';
  while Pos <= Length(InStr) do begin
    if InStr[Pos] = RPTFPrefix then begin
      if TextStr <> '' then begin { Return with current string pieces }
        Exit;
      end else begin { Build RPTFStr }
        repeat
          Ch := InStr[Pos];
          RPTFStr := RPTFStr + Ch;
          Inc(Pos);
        until (Pos > Length(InStr)) or (Ch = RPTFSuffix);
      end; { else }
    end else begin
      TextStr := TextStr + InStr[Pos];
      Inc(Pos);
    end; { else }
  end; { while }
end;  { ParseRPTF }

procedure TRPConverter.GetNextRPTFCommand(RPTFStr: string; var Pos: longint; var Command: char; var Param: string);
begin { GetNextRPTFCommand }
  if Pos = 0 then begin
    Pos := 1;
  end; { if }
  repeat
  { Find beginning of next RPTF command }
    while (Pos <= Length(RPTFStr)) and
     not (RPTFStr[Pos] in [RPTFPrefix,RPTFInfix]) do begin
      Inc(Pos);
    end; { while }

    if Pos <= Length(RPTFStr) then begin
    { Skip over framing character }
      Inc(Pos);

    { Read command }
      Command := RPTFStr[Pos];
      Inc(Pos);

    { Read parameter(s) }
      Param := '';
      while Pos <= Length(RPTFStr) do begin
        case RPTFStr[Pos] of
          RPTFInfix: Break;
          RPTFSeparator: begin end; {!!! Start next parameter }
          RPTFSuffix: Break;
          else Param := Param + RPTFStr[Pos];
        end; { case }
        Inc(Pos);
      end; { while }
    end else begin
      Command := RPTFIgnore;
    end; { if }
  until (Pos > Length(RPTFStr)) or (RPTFStr[Pos] in [RPTFInfix,RPTFSuffix]);
end;  { GetNextRPTFCommand }

procedure TRPConverter.ProcessRPTF(RPTFStr: string);
var
  Command: char;
  Param: string;
  Pos: longint;

  procedure SetFontStyle(FontStyle: TFontStyles; AddStyle: boolean);
  begin
    if AddStyle then begin
      Font.Style := Font.Style + FontStyle;
    end else begin
      Font.Style := Font.Style - FontStyle;
    end; { else }
  end;

begin { ProcessRPTF }
{ Process through commands }
  Pos := 0;
  repeat
    GetNextRPTFCommand(RPTFStr,Pos,Command,Param);

  { Process Command/Param combination }
    case Command of
      RPTFIgnore: begin end;
      RPTFBold: begin
        SetFontStyle([fsBold],(Param = RPTFOn));
        FontData.Bold := Param = RPTFOn;
      end;
      RPTFItalic: begin
        SetFontStyle([fsItalic],(Param = RPTFOn));
        FontData.Italic := Param = RPTFOn;
      end;
      RPTFUnderline: begin
        SetFontStyle([fsUnderline],(Param = RPTFOn));
        FontData.Underline := Param = RPTFOn;
      end;
      RPTFStrikeout: begin
        SetFontStyle([fsStrikeout],(Param = RPTFOn));
        FontData.Strikeout := Param = RPTFOn;
      end;

      RPTFSuperscript: FontData.Superscript := (Param = RPTFOn);
      RPTFSubscript: FontData.Subscript := (Param = RPTFOn);

      RPTFName: begin
        Font.Name := Param;
        FontData.Name := Font.Name;
      end;
      RPTFSize: begin
        Font.Size := Round(StrToFloat(Param));
        FontData.Size := Font.Size;
        FontData.Height := Abs(Font.Size) / 72.0;
      end;
      RPTFColor: begin
        Font.Color := StrToInt(Param);
        FontData.Color := Font.Color;
      end;
      RPTFPitch: begin
        case Param[1] of
          'D': Font.Pitch := fpDefault;
          'F': Font.Pitch := fpFixed;
          'V': Font.Pitch := fpVariable;
        end; { case }
        FontData.Pitch := Font.Pitch;
      end;
      RPTFResetParagraph: begin
        Font.Name := 'Arial';
        FontData.Name := Font.Name;
        Font.Size := 12;
        FontData.Size := Font.Size;
        FontData.Height := Abs(Font.Size) / 72;
        SetFontStyle([fsBold],false);
        FontData.Bold := false;
        SetFontStyle([fsUnderline],false);
        FontData.Underline := false;
        SetFontStyle([fsItalic],false);
        FontData.Italic := false;
        SetFontStyle([fsStrikeout],false);
        FontData.Strikeout := false;
        Font.Color := clBlack;
        FontData.Color := Font.Color;
        Font.Pitch := fpDefault;
        FontData.Pitch := Font.Pitch;
        Font.Charset := DEFAULT_CHARSET;
        FontData.CharSet := CLXToVCLCharSet(Font.Charset);
        FontData.Superscript := false;
        FontData.Subscript := false;
        FontData.Rotation := 0;
        FontData.Width := 0.0;
        FontData.Align := faBaseline;
      end;
      RPTFResetStyle: begin
        SetFontStyle([fsBold],false);
        FontData.Bold := false;
        SetFontStyle([fsUnderline],false);
        FontData.Underline := false;
        SetFontStyle([fsItalic],false);
        FontData.Italic := false;
        SetFontStyle([fsStrikeout],false);
        FontData.Strikeout := false;
        Font.Color := clBlack;
        FontData.Color := Font.Color;
        Font.Pitch := fpDefault;
        FontData.Pitch := Font.Pitch;
        Font.Charset := DEFAULT_CHARSET;
        FontData.CharSet := CLXToVCLCharSet(Font.Charset);
        FontData.Superscript := false;
        FontData.Subscript := false;
        FontData.Rotation := 0;
        FontData.Width := 0.0;
        FontData.Align := faBaseline;
      end;
      RPTFJustify: begin
        case Param[1] of
          RPTFLeft: begin end;
          RPTFCenter: begin end;
          RPTFRight: begin end;
          RPTFBlock: begin end;
        end; { case }
      end;
    end; { case }
  until Pos > Length(RPTFStr);
end;  { ProcessRPTF }

function TRPConverter.NextPiece(InStr: string;  var Pos: integer): string;
var
  RPTFStr: string;
begin { NextPiece }
  ParseRPTF(InStr,Pos,Result,RPTFStr);
  if RPTFStr <> '' then begin
    ProcessRPTF(RPTFStr);
  end; { if }
end;  { NextPiece }

function TRPConverter.RPTFTextWidth(Value: string): double;
var
  I1: integer;
  S1: string;
begin { RPTFTextWidth }
  Result := 0;
  I1 := 1;
  while I1 <= Length(Value) do begin
    S1 := NextPiece(Value,I1);
    FontBitmap.Canvas.Font.Assign(Font);
    Result := Result + FontBitmap.Canvas.TextWidth(S1);
  end; { while }
  Result := Result / FontDPI;
end;  { RPTFTextWidth }

procedure TRPConverter.PrintRPTF(AX, AY: double; AText: string; AJustify: char);
var
  I1: integer;
  S1: string;
begin
  if AJustify = RPTFCenter then begin
    AX := AX - RPTFTextWidth(AText) / 2;
  end else if AJustify = RPTFRight then begin
    AX := AX - RPTFTextWidth(AText);
  end; { else }
  I1 := 1;
  while I1 <= Length(AText) do begin
    S1 := NextPiece(AText,I1);
    Renderer.FontChanged(Font);
    if not Renderer.OutputInvalid then begin
      Renderer.LeftText(S1,AX,AY);
    end; { if }
    FontBitmap.Canvas.Font.Assign(Font);
    AX := AX + FontBitmap.Canvas.TextWidth(S1) / FontDPI;
  end; { while }
end;

procedure TRPConverter.PrintRPTFBlock(AX,AY: double; AText: string; AWidth: double);
var
  Spaces: integer;
  SpaceWidth: double;
  S1: string;
  I1: integer;
  CurrWord: string;

  function CountSpaces(AText: string): integer;
  var
    I1, I2: integer;
    S1: string;
  begin
    Result := 0;
    I1 := 1;
    while I1 <= Length(AText) do begin
      S1 := NextPiece(AText, I1);
      for I2 := Length(S1) downto 1 do begin
        if S1[I2] = ' ' then begin
          Inc(Result);
        end; { if }
      end;
    end;
  end;

begin { PrintRPTFBlock }
{ Print Text right aligned }

  Spaces := CountSpaces(AText);
  SpaceWidth := ((AWidth - RPTFTextWidth(AText)) / Spaces);

  I1 := 1;
  while I1 <= Length(AText) do begin
    S1 := NextPiece(AText, I1);
    CurrWord := Renderer.GetFirstWord(S1);
    Renderer.FontChanged(Font);

    if not Renderer.OutputInvalid then begin
      Renderer.LeftText(CurrWord,AX,AY);
    end; { if }
    Renderer.AdjustXY(AX,AY, RPTFTextWidth(CurrWord));
    while Length(S1) > 0 do begin
      CurrWord := Renderer.GetFirstWord(S1);
      if CurrWord <> '' then begin
        Renderer.AdjustXY(AX,AY,RPTFTextWidth(CurrWord) + SpaceWidth);
        if not Renderer.OutputInvalid then begin
          Renderer.RightText(CurrWord,AX,AY);
        end; { if }
      end; { if }
    end; { while }
  end; { while }
end;  { PrintRPTFBlock }

procedure TRPConverter.DoLeftText;
var
  Text: string;
  X1, Y1: double;
begin
  inherited;
  Y1 := ReadSingle;
  Text := ReadString;
  X1 := ReadSingle;
  if Text <> '' then begin
    if Pos(RPTFPrefix,Text) = 0 then begin
      if Renderer.OutputInvalid then Exit;
      Renderer.LeftText(Text,X1,Y1);
    end else begin
      PrintRPTF(X1,Y1,Text,RPTFLeft);
    end; { else }
  end; { if }
end;

procedure TRPConverter.DoLineTo;
var
  nX: single;
  nY: single;
begin
  nX := ReadSingle;
  nY := ReadSingle;
  if not Renderer.OutputInvalid then begin
    Renderer.MoveTo(FXPos,FYPos);
    Renderer.LineTo(nX,nY);
  end; { if }
  FXPos := nX;
  FYPos := nY;
end;

procedure TRPConverter.DoMoveTo;
begin
  inherited;
  CurrX := ReadSingle;
  CurrY := ReadSingle;
//  Called from LineTo, don't call it here
//  Renderer.MoveTo(CurrX, CurrY);
  FXPos := CurrX;
  FYPos := CurrY;
end;

procedure TRPConverter.DoPrintSpaces;
var
  sText: string;
  X1, Y1, LWidth: single;
begin
  inherited;
  Y1 := ReadSingle;
  sText := ReadString;
  X1 := ReadSingle;
  LWidth := ReadSingle;

  if sText <> '' then begin
    if Pos(RPTFPrefix,sText) = 0 then begin
      if Renderer.OutputInvalid then Exit;
      Renderer.PrintSpaces(sText,X1,Y1, LWidth);
    end else begin
      PrintRPTFBlock(X1,Y1,sText,LWidth);
    end; { else }
  end; { if }
end;

procedure TRPConverter.DoRectangle;
var
  X1, X2, Y1, Y2: single;
begin
  inherited;
  X1 := ReadSingle;
  Y1 := ReadSingle;
  X2 := ReadSingle;
  Y2 := ReadSingle;

  if Renderer.OutputInvalid then Exit;

  Renderer.Rectangle(X1, Y1, X2, Y2);
end;

procedure TRPConverter.DoRightText;
var
  Text: string;
  X1: double;
  Y1: double;
begin
  inherited;
  Y1 := ReadSingle;
  Text := ReadString;
  X1 := ReadSingle;
  if Text <> '' then begin
    if Pos(RPTFPrefix,Text) = 0 then begin
      if Renderer.OutputInvalid then Exit;
      Renderer.RightText(Text,X1,Y1);
    end else begin
      PrintRPTF(X1,Y1,Text,RPTFRight);
    end; { else }
  end; { if }
end;

procedure TRPConverter.SetPenColor;
begin
  Pen.Color := TColor(ReadLongint);
end;

function TRPConverter.PageInRange(PageNum: integer): boolean;
var
  LSelection: TFormatString;
  Sel: TFormatString;
  First: integer;
  Last: integer;
  I1: integer;
begin { PageInRange }
  LSelection := Renderer.Selection;
  if LSelection = '' then begin
    Result := (PageNum >= FFirstPage) and (PageNum <= FLastPage);
  end else begin
  { Handle formats like 1-3,5,7,11-,12,even,odd,all or -20 }
    I1 := 0;
    First := -1;
    Sel := '';
    Result := false;
    repeat
      Inc(I1);
      if (I1 > Length(LSelection)) or (LSelection[I1] = ',') then begin
        Sel := UpperCase(Sel);
        if Sel = '' then begin
          if First > -1 then begin
            Result := (PageNum >= First);
          end; { if }
        end else if (Sel = 'E') or (Sel = {Trans-}'EVEN') then begin { Even }
          Result := not Odd(PageNum);
        end else if (Sel = 'R') then begin { Reverse Print }
        end else if (Sel = 'O') or (Sel = {Trans-}'ODD') then begin { Odd }
          Result := Odd(PageNum);
        end else if (Sel = 'A') or (Sel = {Trans-}'ALL') then begin { All }
          Result := true;
        end else begin { Check numeric range }
          Last := StrToInt(Sel);
          if First > -1 then begin
            Result := (PageNum >= First) and (PageNum <= Last);
          end else begin
            Result := (PageNum = Last);
          end; { else }
        end; { else }
        First := -1;
        Sel := '';
      end else if LSelection[I1] = '-' then begin
        if Sel = '' then begin
          First := 1;
        end else begin
          First := StrToInt(Sel);
        end; { else }
        Sel := '';
      end else begin
        Sel := Sel + LSelection[I1];
      end; { else }
    until (I1 > Length(LSelection)) or Result;
  end; { else }
end;  { PageInRange }

procedure TRPConverter.Generate(const APageNo: integer; APageCount: integer);
var
  CurrPage: integer;
begin
  Renderer.Converter := Self;
  FReportDateTime := Now;
  ReadReportHeader;
  try
    if APageNo = 0 then begin
      Renderer.DocBegin;
      if Renderer.Selection <> '' then begin
        CurrPage := PageNo;
        while CurrPage <= PageCount do begin
          if PageInRange(CurrPage) then begin
            GotoPage(CurrPage);
            ProcessPage;
          end; { if }
          Inc(CurrPage);
        end; { while }
      end else begin
        if (Renderer.LastPage <> 9999) and
           (Renderer.LastPage <> 0) and
           ((Renderer.FirstPage <> 1) or
           (Renderer.LastPage <> PageCount)) then begin
          if Renderer.LastPage > PageCount then begin
            Renderer.LastPage := PageCount;
          end; { if }
          GotoPage(Renderer.FirstPage);
          ProcessPage;
          while PageNo < Renderer.LastPage do begin
            ProcessPage;
          end; { while }
        end else begin
          ProcessPage;
          while PageNo < PageCount do begin
            ProcessPage;
          end; { while }
        end; { else }
      end; { else }
    end else begin
      if APageCount <> 0 then begin
        FPageCount := PageCount;
      end; { if }
      Renderer.DocBegin;
      GotoPage(APageNo);
      ProcessPage;
    end; { else }
  finally
    Renderer.DocEnd;
  end; { tryf }
end;

procedure TRPConverter.GotoPage(const APageNum: integer);
begin
  FStream.Position := FCurrPagePos;
  if APageNum = PageNo then begin
    Exit;
  end; { if }

  while APageNum < PageNo do begin { Go backwards }
    ReadPageHeader;
    if (FPrevPagePos > 0) and (APageNum <> PageNo) then begin
      FStream.Position := FPrevPagePos;
    end else begin
      FStream.Position := FCurrPagePos;
      Exit;
    end; { else }
  end; { while }

  while APageNum > PageNo do begin { Go forwards }
    ReadPageHeader;
    if (FNextPagePos > 0) and (APageNum <> PageNo) then begin
      FStream.Position := FNextPagePos;
    end else begin
      FStream.Position := FCurrPagePos;
      Exit;
    end; { else }
  end; { while }
end;

procedure TRPConverter.MatchCode(pyValue: byte);
begin
  if ReadByte <> pyValue then begin
    raise Exception.Create('Unexpected code found in report file.');
  end;
end;

procedure TRPConverter.ProcessPage;
var
  Code: byte;
begin
  ReadPageHeader;
  Renderer.PageBegin;
  repeat
    Code := ReadByte;
    case Code of
    // Graphical commands
      rpcArc: DoArc;
      rpcBrushCopy: DoBrushCopy;
      rpcChord: DoChord;
      rpcCopyRect: DoCopyRect;
      rpcDraw: DoDraw;
      rpcDrawFocusRect: DoDrawFocusRect;
      rpcEllipse: DoEllipse;
      rpcFillRect: DoFillRect;
      rpcFloodFill: DoFloodFill;
      rpcFrameRect: DoFrameRect;
      rpcLineTo: DoLineTo;
      rpcMoveTo: DoMoveTo;
      rpcPie: DoPie;
      rpcPolygon: DoPolygon;
      rpcPolyline: DoPolyline;
      rpcPrintBitmap:  DoPrintBitmap;
      rpcPrintBitmapRect: DoPrintBitmapRect;
      rpcRectangle: DoRectangle;
      rpcTabRectangle: DoRectangle;
      rpcRoundRect: DoRoundRect;
      rpcStretchDraw: DoStretchDraw;
      rpcTextRect: DoTextRect;
      rpcSetTextBKMode: SetTextBKMode;
      rpcCenterText: DoCenterText;
      rpcLeftText: DoLeftText;
      rpcRightText: DoRightText;
      rpcSetBKColor: SetBKColor;
      rpcSetScaleX: SetScaleX;
      rpcSetScaleY: SetScaleY;
      rpcPrintData: DoPrintData;
      rpcPrintDataStream: DoPrintDataStream;
      rpcPrintSpaces: DoPrintSpaces;
      rpcBlockText: DoPrintSpaces; 
      rpcTransBitmap: DoTransBitmap;
      rpcSetFrameMode: DoSetFrameMode;
      rpcPrintImageRect: DoPrintImageRect;
      rpcVariableData: DoVariableData;

    // Brush commands
      rpcSetBrush: SetBrush;
      rpcSetBrushColor: SetBrushColor;
      rpcSetBrushStyle: SetBrushStyle;
      rpcSetBrushBitmap: SetBrushBitmap;

    // Font commands
      rpcSetFont: SetFont;
      rpcSetFontName: SetFontName;
      rpcSetFontColor: SetFontColor;
      rpcSetFontPitch: SetFontPitch;
      rpcSetFontHeight: SetFontHeight;
      rpcSetFontBold: SetFontBold;
      rpcSetFontUnderline: SetFontUnderline;
      rpcSetFontItalic: SetFontItalic;
      rpcSetFontStrikeout: SetFontStrikeout;
      rpcSetFontSuperscript: SetFontSuperscript;
      rpcSetFontSubscript: SetFontSubscript;
      rpcSetFontRotation: SetFontRotation;
      rpcSetFontAlign: SetFontAlign;
      rpcSetFontCharset: SetFontCharset;

    // Pen commands
      rpcSetPen: SetPen;
      rpcSetPenColor: SetPenColor;
      rpcSetPenMode: SetPenMode;
      rpcSetPenStyle: SetPenStyle;
      rpcSetPenWidth: SetPenWidth;

    // Misc commands
      rpcEndPage: Break;
      rpcResetLineHeight: DoResetLineHeight;
      rpcAllowPreviewOnly: DoAllowPreviewOnly;
      rpcAllowPrinterOnly: DoAllowPrinterOnly;
      rpcAllowAll: DoAllowAll;
      rpcSetOrientation: SetOrientation;
      rpcSetBin: SetBin;
      else begin
        RaiseError(Format(Trans('Unexpected code [%d] found in report file.'),[Code]));
      end;
    end; { case }
  until false;
  Renderer.PageEnd;
end;

function TRPConverter.ReadByte: byte;
begin
  FStream.ReadBuffer(Result, SizeOf(Result));
end;

function TRPConverter.ReadLongint: longint;
begin
  FStream.ReadBuffer(Result, SizeOf(Result));
end;

procedure TRPConverter.ReadPageHeader;
begin
  FCurrPagePos := FStream.Position;
  MatchCode(rpcNewPage);

{ Read page header }
  FPrevPagePos := ReadLongint;
  PageHeader.PrevPagePos := FPrevPagePos;
  FNextPagePos := ReadLongint;
  PageHeader.NextPagePos := FNextPagePos;
  if FileVer >= 2018 then begin
    FPageNo := ReadLongint;
  end else begin
    FPageNo := ReadSmallInt;
  end; { else }
  PageHeader.PageNo := FPageNo;

{ Page Width,Height }
  if FileVer >= 2000 then begin
    Renderer.PaperWidth := ReadSingle;
    PageHeader.PaperWidth := Renderer.PaperWidth;
    Renderer.PaperHeight := ReadSingle;
    PageHeader.PaperHeight := Renderer.PaperHeight;
  end else begin
    PageHeader.OldOrientation := ReadByte; { Old orientation byte }
  end;
end;

procedure TRPConverter.ReadReportHeader;
var
  Code: byte;
begin
  FStream.Position := 0;
  Code := ReadByte;
  case Code of
    rpcJobHeader11: FFileVer := 1101;
    rpcJobHeader20i: FFileVer := 2009;
    rpcJobHeader: FFileVer := ReadSmallInt;
    else begin
      RaiseError(Format(Trans('Unexpected header code [%d] found in report file.'),[Code]));
    end;
  end; { case }
  ReportHeader.FileVer := FFileVer;

  if FileVer >= 2010 then begin
    FTitle := ReadString;
    ReportHeader.Title := FTitle;
  end; { if }

  FPageNo := 1;
  ReportHeader.PageNo := FPageNo;
  if FileVer >= 2018 then begin
    FPageCount := ReadLongint;
    FFirstPage := ReadLongint;
    FLastPage := ReadLongint;
  end else begin
    FPageCount := ReadSmallInt;
    FFirstPage := ReadSmallInt;
    FLastPage := ReadSmallInt;
  end; { else }

  ReportHeader.PageCount := FPageCount;
  ReportHeader.FirstPage := FFirstPage;
  ReportHeader.LastPage := FLastPage;

// PaperWidth
  ReportHeader.PaperWidth := ReadSingle;
// PaperHeight
  ReportHeader.PaperHeight := ReadSingle;

// Waste dimensions
  ReportHeader.LeftWaste := ReadSingle; { FLeftWaste }
  ReportHeader.RightWaste := ReadSingle; { FRightWaste }
  ReportHeader.TopWaste := ReadSingle; { FTopWaste }
  ReportHeader.BottomWaste := ReadSingle; { FBottomWaste }

// Printer settings
  ReportHeader.Orientation := TOrientation(ReadByte); { Orientation }
  if FileVer >= 2000 then begin
    ReportHeader.Bin := ReadSmallInt; { Bin }
    Renderer.SelectBin(ReportHeader.Bin);
    ReportHeader.Collate := boolean(ReadByte); { Collate }
    ReportHeader.Duplex := TDuplex(ReadByte); { Duplex }
  end; { if }
  ReportHeader.Copies := ReadSmallInt; { Copies }
  ReportHeader.DMPaperSize := ReadSmallInt; { PaperSize }
  ReportHeader.DMPaperWidth := ReadSmallInt; { PaperWidth }
  ReportHeader.DMPaperLength := ReadSmallInt; { PaperLength }
// OutputName - Output port for Printer
  ReportHeader.OutputName := ReadString;

  FCurrPagePos := FStream.Position;
end;

function TRPConverter.ReadSingle: single;
begin
  FStream.ReadBuffer(Result, SizeOf(Result));
end;

function TRPConverter.ReadSmallInt: smallint;
begin
  FStream.ReadBuffer(Result, SizeOf(Result));
end;

function TRPConverter.ReadString: string;
var
  wLen: word;
begin
  wLen := ReadByte;
  if (FileVer >= 2016) and (wLen >= 192) then begin
    wLen := (wLen * $0100 + ReadByte) - $C000;
  end; { if }
  SetLength(Result, wLen);
  if wLen > 0 then begin
    FStream.ReadBuffer(Result[1], wLen);
    ProcessString(Result);
  end; { if }
end;

procedure TRPConverter.DoFillRect;
var
  Rect: TRect;
begin
  Rect := ReadRect;

  if Renderer.OutputInvalid then Exit;

  Renderer.FillRect(Rect);
end;

procedure TRPConverter.DoFloodFill;
begin
  ReadSingle;
  ReadSingle;
  ReadLongint;
  ReadLongint;
end;

procedure TRPConverter.DoFrameRect;
var
  Rect: TRect;
begin
  Rect := ReadRect;

  if Renderer.OutputInvalid then Exit;

  Renderer.FrameRect(Rect);
end;

procedure TRPConverter.DoPie;
var
  X1, Y1, X2, Y2,
  X3, Y3, X4, Y4: double;
begin
  X1 := ReadSingle;
  Y1 := ReadSingle;
  X2 := ReadSingle;
  Y2 := ReadSingle;
  X3 := ReadSingle;
  Y3 := ReadSingle;
  X4 := ReadSingle;
  Y4 := ReadSingle;

  if Renderer.OutputInvalid then Exit;

  Renderer.Pie(X1,Y1,X2,Y2,X3,Y3,X4,Y4);
end;

procedure TRPConverter.DoPolygon;
var
  I1, I2: integer;
  PointArr: array of TFloatPoint;
begin
  I1 := ReadSmallInt;
  SetLength(PointArr, I1 + 1);
  for I2 := 0 to I1 do begin
    PointArr[I2].X := ReadSingle;
    PointArr[I2].Y := ReadSingle;
    if I2 = 0 then begin
      Renderer.MoveTo(PointArr[I2].X,PointArr[I2].Y);
    end; { if }
  end; { for }

  if Renderer.OutputInvalid then Exit;

  Renderer.Polygon(PointArr);
end;

procedure TRPConverter.DoPolyline;
var
  I1, I2: integer;
  PolyLineArr: array of TFloatPoint;
begin
  I1 := ReadSmallInt;
  SetLength(PolyLineArr, I1 + 1);
  for I2 := 0 to I1 do begin
    PolyLineArr[I2].X := ReadSingle;
    PolyLineArr[I2].Y := ReadSingle;
    if I2 = 0 then begin
      Renderer.MoveTo(PolyLineArr[I2].X,PolyLineArr[I2].Y);
    end;
  end; { for }

  if Renderer.OutputInvalid then Exit;

  Renderer.PolyLine(PolyLineArr);
end;

procedure TRPConverter.DoPrintBitmap;
var
  LGraphic: TGraphic;
  X1,Y1: double;
  ScaleX,ScaleY: double;
begin
  CreateGraphic(LGraphic);
  try
    X1 := ReadSingle;
    Y1 := ReadSingle;
    ScaleX := ReadSingle;
    ScaleY := ReadSingle;

    if Renderer.OutputInvalid then Exit;

    Renderer.PrintBitmap(X1,Y1,ScaleX,ScaleY,LGraphic as
      QGraphics.TBitmap);
  finally
    LGraphic.Free;
  end;
end;

procedure TRPConverter.DoPrintBitmapRect;
var
  LGraphic: TGraphic;
  X1, X2: double;
  Y1, Y2: double;
begin
  CreateGraphic(LGraphic);
  try
    X1 := ReadSingle;
    Y1 := ReadSingle;
    X2 := ReadSingle;
    Y2 := ReadSingle;

    if Renderer.OutputInvalid then Exit;

    Renderer.PrintBitmapRect(X1, Y1, X2, Y2, LGraphic as
      QGraphics.TBitmap);
  finally
    LGraphic.Free;
    LGraphic := nil;
  end;
end;

procedure TRPConverter.DoRoundRect;
var
  X1, X2, X3, Y1, Y2, Y3: single;
begin
  X1 := ReadSingle;
  Y1 := ReadSingle;
  X2 := ReadSingle;
  Y2 := ReadSingle;
  X3 := ReadSingle;
  Y3 := ReadSingle;

  if Renderer.OutputInvalid then Exit;

  Renderer.RoundRect(X1, Y1, X2, Y2, X3, Y3);
end;

procedure TRPConverter.DoStretchDraw;
var
  LGraphic: TGraphic;
  Rect: TRect;
begin
  CreateGraphic(LGraphic);
  try
    Rect := ReadRect;

    if Renderer.OutputInvalid then Exit;

    Renderer.StretchDraw(Rect,LGraphic);
  finally
    LGraphic.Free;
  end;
end;

procedure TRPConverter.DoTextRect;
var
  X1, Y1: double;
  S1: string;
  Rect: TRect;
begin
  Rect := ReadRect;
  X1 := ReadSingle;
  Y1 := ReadSingle;
  S1 := ReadString;

  if Renderer.OutputInvalid then Exit;

  Renderer.TextRect(Rect,X1,Y1,S1);
end;

procedure TRPConverter.SetTextBkMode;
begin
  Renderer.TextBKMode := TBKMode(ReadByte);
end;

procedure TRPConverter.SetBkColor;
begin
  Renderer.SetBKColor(TColor(ReadLongInt));
end;

procedure TRPConverter.SetScaleX;
begin
  { Don't change ScaleX }
  ReadSingle;
end;

procedure TRPConverter.SetScaleY;
begin
  { Don't change ScaleY }
  ReadSingle;
end;

procedure TRPConverter.DoPrintData;
var
  S1: string;
begin
  S1 := ReadString;

  if Renderer.OutputInvalid then Exit;

  Renderer.PrintData(S1);
end;

procedure TRPConverter.DoPrintDataStream;
var
  L1: LongInt;
begin
  L1 := ReadLongint;

  if Renderer.OutputInvalid then Exit;

  Renderer.PrintDataStream(FStream,L1);
end;

procedure TRPConverter.DoTransBitmap;
begin
  Renderer.TransparentBitmaps := boolean(ReadByte);
end;

procedure TRPConverter.DoSetFrameMode;
begin
  Renderer.FrameMode := TFrameMode(ReadByte);
end;

procedure TRPConverter.DoPrintImageRect;
var
  TempStream: TMemoryStream;
  S1: string;
  X1: double;
  Y1: double;
  X2: double;
  Y2: double;
begin
  TempStream := TMemoryStream.Create; { Create temp stream }
  try
    TempStream.SetSize(ReadLongint); { Allocate space }
    FStream.ReadBuffer(TempStream.Memory^,TempStream.Size);
    TempStream.Position := 0;
    S1 := ReadString; { Read ImageType }
    X1 := ReadSingle;
    Y1 := ReadSingle;
    X2 := ReadSingle;
    Y2 := ReadSingle;

    if Renderer.OutputInvalid then Exit;

    Renderer.PrintImageRect(X1,Y1,X2,Y2,TempStream,S1);
  finally
    TempStream.Free; { Clean up stream }
  end; { tryf }
end;

procedure TRPConverter.DoVariableData;
var
  S1: string;
begin
  S1 := ReadString; { Read variable name }
  SetPIVar(S1,ReadPIVar);
end;

function TRPConverter.GetPIVar(VarName: string): string;
var
  I1: integer;
begin { GetPIVar }
  Result := '';
  for I1 := 0 to (VarList.Count - 1) do begin
    if CompareText(VarList[I1],VarName) = 0 then begin
      Result := TRPStringObject(VarList.Objects[I1]).Value;
      Exit;
    end; { if }
  end; { for }
end;  { GetPIVar }

function TRPConverter.ReadPIVar: string;
var
  BufMax: byte;
  BufLen: byte;
  Buf: array[1..32] of longint;
  SavePos: longint;
begin { ReadPIVar }
  BufMax := ReadByte; { Read max buffer size }
  BufLen := ReadByte; { Read buffer length or 255 for undefined }
  ReadBuf(Buf,BufMax);
  if BufLen in [254,255] then begin { not defined, goto previous if any }
    SavePos := FStream.Position;
    FStream.Position := Abs(Buf[1]);
    Result := ReadPIVar;
    FStream.Position := SavePos;
  end else begin
    SetLength(Result,BufLen);
    if BufLen > 0 then begin
      Move(Buf,Result[1],BufLen);
    end; { if }
  end; { else }
end;  { ReadPIVar }

procedure TRPConverter.ReadGraphic(Value: TGraphic);
begin { ReadGraphic }
  FReuseGraphic := ReadByte > 128;
  LoadGraphic(Value);
end;  { ReadGraphic }

procedure TRPConverter.SetBrush;
begin
  with Brush do begin
    Color := TColor(ReadLongint);
    Style := TBrushStyle(ReadByte);
    if boolean(ReadByte) then begin
      ReadGraphic(Bitmap);
    end; { if }
  end; { with }
end;

procedure TRPConverter.SetBrushStyle;
begin
  Brush.Style := TBrushStyle(ReadByte);
end;

procedure TRPConverter.SetBrushBitmap;
begin
  if boolean(ReadByte) then begin
    if not Assigned(Brush.Bitmap) then begin
      Brush.Bitmap := QGraphics.TBitmap.Create;
    end; { if }
    ReadGraphic(Brush.Bitmap);
  end; { if }
end;

procedure TRPConverter.SetFont;
begin
  with Font do begin
    Name := ReadString;
    Color := TColor(ReadLongint);
    Pitch := TFontPitch(ReadByte);
    //!!PORT!! #124 RPRENDER01
      ReadByte;
      CharSet := DEFAULT_CHARSET;
    FontData.Height := ReadSingle;
    Font.Size := Round(FontData.Height * Renderer.XDPI);
    if boolean(ReadByte) then begin
      Style := Style + [fsBold];
    end; { if }
    if boolean(ReadByte) then begin
      Style := Style + [fsItalic];
    end; { if }
    if boolean(ReadByte) then begin
      Style := Style + [fsStrikeout];
    end;
    ReadByte;
    if FileVer >= 2000 then begin
      FontData.Rotation := ReadSmallInt;
    end else begin
      FontData.Rotation := 0;
    end; { else }
  end;
end;

procedure TRPConverter.SetPen;
var
  Pen: TPen;
  PenWidth: double;
begin
  Pen := TPen.Create;
  try
    with Pen do begin
      Color := TColor(ReadLongint);
      Mode := TPenMode(ReadByte);
      Style := TPenStyle(ReadByte);
      PenWidth := ReadSingle;
      Width := Round(PenWidth {* FXDPI * FScaleX * FZoom} / 10000.0); //!!PORT!!
    end; { with }
    Pen.Assign(Pen);
  finally
    Pen.Free;
  end; { tryf }
end;

procedure TRPConverter.SetPenMode;
begin
  Pen.Mode := TPenMode(ReadByte);
end;

procedure TRPConverter.SetPenStyle;
begin
  Pen.Style := TPenStyle(ReadByte);
end;

procedure TRPConverter.SetPenWidth;
begin
  Renderer.PenWidth := ReadSingle;
  Renderer.PenChanged(Pen);
end;

procedure TRPConverter.DoResetLineHeight;
begin
  FontData.Ascent := ReadSingle;
  FontData.Descent := ReadSingle;
  LineHeight := ReadSingle;
end;

procedure TRPConverter.DoAllowPreviewOnly;
begin
  Renderer.AllowPreviewOnly;
end;

procedure TRPConverter.DoAllowPrinterOnly;
begin
  Renderer.AllowPrinterOnly;
end;

procedure TRPConverter.DoAllowAll;
begin
  Renderer.AllowAll;
end;

procedure TRPConverter.SetOrientation;
begin
  ReportHeader.Orientation := TOrientation(ReadByte);
end;

procedure TRPConverter.SetBin;
begin
  Renderer.SelectBin(ReadString);
end;

procedure TRPConverter.ReadBuf(var Buf; Len: integer);
begin { ReadBuf }
  FStream.ReadBuffer(Buf,Len);
end;  { ReadBuf }

constructor TRPConverter.Create(AStream: TStream; ARenderer: TRPRender);
begin
  inherited Create;
  FStream := AStream;
  FRenderer := ARenderer;
  //
  Brush := TBrush.Create;
  Brush.OnChange := ARenderer.BrushChanged; //!!! Put in intermediate function that allows changed call to be paused until all changes are done
  Font := TFont.Create;
  Font.OnChange := ARenderer.FontChanged;
  Pen := TPen.Create;
  Pen.OnChange := ARenderer.PenChanged;

  FontBitmap := QGraphics.TBitmap.Create;
  FontBitmap.Canvas.Font.PixelsPerInch := FontDPI;
  FontBitmap.Width := 10;
  FontBitmap.Height := 10;
  VarList := TStringList.Create;
end;

function TRPConverter.ReadRect: TRect;
begin
  Result.TopLeft := ReadPoint;
  Result.BottomRight := ReadPoint;
end;

procedure TRPConverter.CreateGraphic(var Value: TGraphic);
var
  Kind: byte;
begin { CreateGraphic }
  inherited;
  Kind := ReadByte;
  fReuseGraphic := Kind > 128;
  case (Kind mod 128) of
    1: begin { Bitmap }
      Value := QGraphics.TBitmap.Create;
    end;
    2: begin { Metafile }
      Value := TMetafile.Create;
    end;
    3: begin { Icon }
      Value := TIcon.Create;
    end;
  end; { case }
  LoadGraphic(Value);
end;

procedure TRPConverter.LoadGraphic(Value: TGraphic);
var
  TempStream: TMemoryStream;
  NewPos: longint;
  SavePos: longint;
begin { LoadGraphic }
  TempStream := TMemoryStream.Create; { Create temp stream }
  try
    if fReuseGraphic then begin
      NewPos := ReadLongint;
      SavePos := FStream.Position;
      FStream.Position := NewPos;
    end else begin
      SavePos := 0;
    end; { else }
    TempStream.SetSize(ReadLongint); { Allocate space }
    FStream.ReadBuffer(TempStream.Memory^,TempStream.Size);
    Value.LoadFromStream(TempStream); { Copy stream to icon object }
    if fReuseGraphic then begin
      FStream.Position := SavePos;
      fReuseGraphic := false;
    end; { if }
  finally
    TempStream.Free; { Clean up stream }
  end; { try }
end;

function TRPConverter.ReadPoint: TPoint;
begin
  Result.X := Renderer.XI2D(ReadSingle);
  Result.Y := Renderer.YI2D(ReadSingle);
end;

destructor TRPConverter.Destroy;
begin
  FreeAndNil(Brush);
  FreeAndNil(Font);
  FreeAndNil(Pen);
  FreeAndNil(FontBitmap);
  ClearVarList;
  FreeAndNil(VarList);
  inherited;
end;

procedure TRPConverter.ProcessString(var Text: string);
var
  I1: integer;
  I2: integer;
  I3: integer;
  MacroStr: string;
  MacroID: TMacroID;
  VarName: string;

begin { ProcessString }
{ Look for any macro strings in Text and replace }
  repeat
    I1 := Pos({Trans-}#253'$RPCM',Text);
    if I1 > 0 then begin
      MacroStr := Copy(Text,I1,8);
      MacroID := TMacroID(StrToInt(Copy(MacroStr,7,2)));
      Delete(Text,I1,8);
      Insert(ConvertMacro(MacroID),Text,I1);
    end; { if }
    I2 := Pos({Trans-}#253'$RPCV',Text);
    if I2 > 0 then begin
      I3 := I2 + 6;
      VarName := '';
      if (I3 < Length(Text)) and (Text[I3] = #252) then begin
        Inc(I3);
        while I3 <= Length(Text) do begin
          if Text[I3] = #251 then begin
            Break;
          end else begin
            VarName := VarName + Text[I3];
          end; { else }
          Inc(I3);
        end; { while }
      end; { if }
      if I3 <= Length(Text) then begin
        Delete(Text,I2,I3 - I2 + 1);
      end else begin
        Delete(Text,I2,Length(Text) - I2 + 1);
      end; { else }
      Insert(GetPIVar(VarName),Text,I2);
    end; { if }
  until (I1 = 0) and (I2 = 0);
end;  { ProcessString }

function TRPConverter.ConvertMacro(MacroID: TMacroID): string;
var
  I1: integer;

begin { ConvertMacro }
  case MacroID of
    midCurrDateShort: begin
      Result := FormatDateTime({Trans-}'ddddd',ReportDateTime);
    end;
    midCurrDateLong: begin
      Result := FormatDateTime({Trans-}'dddddd',ReportDateTime);
    end;
    midCurrDateUS: begin
      Result := FormatDateTime({Trans-}'mm/dd/yy',ReportDateTime);
    end;
    midCurrDateInter: begin
      Result := FormatDateTime({Trans-}'dd/mm/yy',ReportDateTime);
    end;
    midCurrTimeShort: begin
      Result := FormatDateTime({Trans-}'t',ReportDateTime);
    end;
    midCurrTimeLong: begin
      Result := FormatDateTime({Trans-}'tt',ReportDateTime);
    end;
    midCurrTimeAMPM: begin
      Result := FormatDateTime({Trans-}'hh:nn ampm',ReportDateTime);
    end;
    midCurrTime24: begin
      Result := FormatDateTime({Trans-}'hh:nn',ReportDateTime);
    end;
    midFirstPage: begin
      Result := IntToStr(FFirstPage);
    end;
    midLastPage: begin
      Result := IntToStr(FLastPage);
    end;
    midTotalPages: begin
      Result := IntToStr(FPageCount);
    end;
    midCurrentPage: begin
      Result := IntToStr(FPageNo);
    end;
    midRelativePage: begin
    // This code should never be called
      Result := '';
    end;
    midPrinterName: begin
      Result := DeviceName;
    end;
    midDriverName: begin
      Result := DriverName;
    end;
    midPortName: begin
      Result := Port;
    end;
    midUser01..midUser20: begin
      I1 := Ord(MacroID) - Ord(midUser01);
      if I1 < Renderer.MacroData.Count then begin
        Result := Renderer.MacroData[I1];
      end else begin
        Result := '';
      end; { else }
    end;
    else begin
      Result := '';
    end;
  end; { case }
end;  { ConvertMacro }

procedure TRPConverter.SetPIVar(VarName, VarValue: string);
var
  I1: integer;
begin { SetPIVar }
  for I1 := 0 to (VarList.Count - 1) do begin
    if CompareText(VarList[I1],VarName) = 0 then begin
    { Modify existing variable item }
      TRPStringObject(VarList.Objects[I1]).Value := VarValue;
      Exit;
    end; { if }
  end; { for }
{ Add a new variable item }
  VarList.AddObject(VarName,TRPStringObject.Create(VarValue));
end;  { SetPIVar }

procedure TRPConverter.ClearVarList;
var
  I1: integer;
begin { ClearVarList }
  for I1 := 0 to (VarList.Count - 1) do begin
    VarList.Objects[I1].Free;
  end; { for }
  VarList.Clear;
end;  { ClearVarList }

function TRPConverter.GetDeviceName: string;
begin
  Result := 'Render';
end;

function TRPConverter.GetDriverName: string;
begin
  Result := 'Render';
end;

function TRPConverter.GetPort: string;
begin
  Result := 'Render';
end;

{ TRPRender }

constructor TRPRender.Create(AOwner: TComponent);
begin
  inherited;
  ValidDestination := [destPrinter,destPreview,destFile,destNone,destCustomFile];
  DisplayName := 'RPRender';
  Active := true;
  FMacroData := TStringList.Create;
end;

destructor TRPRender.Destroy;
begin
  RaveUnRegisterRender(DisplayName, self);
  FreeAndNil(FMacroData);
  inherited;
end;

{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
procedure TRPRender.PrintRender(ANDRStream: TStream; AOutputFileName: TFileName);
begin
  OutputFileName := AOutputFileName;
  Render(ANDRStream);
end;
{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED ON}{$ENDIF}

procedure TRPRender.Render(ANDRStream: TStream);
begin
  if Rendering then Exit;

  if Assigned(ANDRStream) then begin
    FNDRStream := ANDRStream;
  end; { if }
  if Assigned(FNDRStream) then begin
    RenderPage(0);
  end else begin
    IsInternalNDRStream := true;
    FNDRStream := TFileStream.Create(InputFileName,fmOpenRead or fmShareDenyWrite);
    RenderPage(0);
  end; { if }
end;

procedure TRPRender.Render(AInputFileName: TFileName);
begin
  InputFileName := AInputFileName;
  FNDRStream := nil;
  Render;
end;

procedure TRPRender.RenderPage(PageNum: integer);
begin
  if Assigned(NDRStream) then begin
    with TRPConverter.Create(NDRStream,self) do try
      FRendering := true;
      Generate(PageNum);
    finally
      Converter := nil;
      Free;
      FRendering := false;
    end; { with }
  end; { if }
end;

procedure TRPRender.SetDisplayName(const Value: string);
begin
  RaveUnRegisterRender(DisplayName, Self);
  FDisplayName := Value;
  RaveRegisterRender(DisplayName, Self);
end;

function TRPRender.StripRPTF(AValue: string): string;
var
  Ch: char;
  Pos: integer;
  RPTFStr: string;
begin { StripRPTF }
  Result := '';
  RPTFStr := '';
  Pos := 1;
  while Pos <= Length(AValue) do begin
    if AValue[Pos] = RPTFPrefix then begin
      repeat
        Ch := AValue[Pos];
        RPTFStr := RPTFStr + Ch;
        Inc(Pos);
      until (Pos > Length(AValue)) or (Ch = RPTFSuffix);
    end else begin
      Result := Result + AValue[Pos];
      Inc(Pos);
    end; { else }
  end; { while }
end;  { StripRPTF }

procedure TRPRender.Arc(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double);
begin
end;

procedure TRPRender.BrushChanged(Sender: TObject);
begin
end;

procedure TRPRender.CenterText(const AText: string; const X1, Y1: double);
begin
end;

procedure TRPRender.Chord(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double);
begin
end;

procedure TRPRender.DocBegin;
begin
end;

procedure TRPRender.DocEnd;
begin
end;

procedure TRPRender.Ellipse(const X1, Y1, X2, Y2: double);
begin
end;

procedure TRPRender.FillRect(const ARect: TRect);
begin
end;

function TRPRender.GetXDPI: integer;
begin
  Result := 72;
end;

function TRPRender.GetYDPI: integer;
begin
  Result := 72;
end;

procedure TRPRender.TextRect(ARect: TRect; X1,Y1: double;S1: string);
begin
end;

procedure TRPRender.FontChanged(Sender: TObject);
begin
end;

procedure TRPRender.LeftText(const AText: string; const X1, Y1: double);
begin
end;

procedure TRPRender.LineTo(const X1, Y1: double);
begin
end;

procedure TRPRender.MoveTo(const X1, Y1: double);
begin
end;

procedure TRPRender.PageBegin;
begin
end;

procedure TRPRender.PageEnd;
begin
end;

procedure TRPRender.PenChanged(Sender: TObject);
begin
end;

procedure TRPRender.Pie(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double);
begin
end;

procedure TRPRender.PolyLine(const PolyLineArr: array of TFloatPoint);
begin
end;

procedure TRPRender.Polygon(const PolyLineArr: array of TFloatPoint);
begin
end;

procedure TRPRender.PrintBitmap(const X1, Y1, ScaleX, ScaleY: double;
  AGraphic: QGraphics.TBitmap);
begin
end;

procedure TRPRender.PrintBitmapRect(const X1, Y1, X2, Y2: double; AGraphic:
  QGraphics.TBitmap);
begin
end;

procedure TRPRender.PrintImageRect(X1, Y1, X2, Y2: double;
  ImageStream: TStream; ImageType: string);
var
  Bitmap: QGraphics
    .TBitmap;
begin { PrintImageRect }
  if Assigned(FOnDecodeImage) then begin
    Bitmap :=
      QGraphics
      .TBitmap.Create;
    try
      //!!PORT!! RPRENDERPDF01
      PrintBitmapRect(X1,Y1,X2,Y2,Bitmap);
    finally
      Bitmap.Free;
    end; { tryf }
  end; { if }
end;  { PrintImageRect }


procedure TRPRender.PrintSpaces(const AText: string; const X1,Y1,AWidth: double);
begin
end;

procedure TRPRender.StretchDraw(const ARect: TRect; AGraphic: TGraphic);
begin
end;

procedure TRPRender.Draw(const X1, Y1: double; AGraphic: TGraphic);
begin
end;

procedure TRPRender.Rectangle(const X1,Y1,X2,Y2: double);
begin
end;

procedure TRPRender.RightText(const AText: string; const X1, Y1: double);
begin
end;

procedure TRPRender.RoundRect(const X1,Y1,X2,Y2,X3,Y3: double);
begin
end;

function TRPRender.GetFirstWord(var Line: string): string;
var
  LFound: Boolean;
  LSpacePos: integer;
  LNext: PChar;
begin
  Result := '';
  if Line = '' then Exit;
  LSpacePos := 0;
  LFound := false;
  LNext := @Line[1];
  if Assigned(LNext) then begin
    repeat
      if LFound then begin
        if LNext^ = #32 then begin
          LNext := {$IFDEF LEVEL6}StrNextChar(LNext){$ELSE}CharNext(LNext){$ENDIF};
          LSpacePos := LNext - @Line[1];
          Break;
        end; { if }
      end else begin
        LFound := LNext^ <> #32;
      end; { else }
      LNext := {$IFDEF LEVEL6}StrNextChar(LNext){$ELSE}CharNext(LNext){$ENDIF};
    until LNext^ = #0;

    if LSpacePos = 0 then begin
      Result := Line;
      Line := '';
    end else begin
      Result := MBCSCopy(Line, 1, LSpacePos - 1);
      MBCSDelete(Line, 1, LSpacePos - 1);
    end; { else }
  end; { if }
end;  { GetFirstWord }

function TRPRender.CountSpaces(Text: string): integer;
var
  Before: boolean;
  BeforeSpaces: integer;
  Spaces: integer;
  AfterSpaces: integer;
  PNext: PChar;
begin { CountSpaces }
{ Count the number of spaces in Text between first and last word }
  Before := true;
  BeforeSpaces := 0;
  Spaces := 0;
  AfterSpaces := 0;
  PNext := @Text[1];
  if Assigned(PNext) then begin
    repeat
      if PNext^ = ' ' then begin
        if Before then begin
          Inc(BeforeSpaces);
        end; { if }
        Inc(Spaces);
        Inc(AfterSpaces);
      end else begin
        Before := false;
        AfterSpaces := 0;
      end; { else }
      PNext := {$IFDEF LEVEL6}StrNextChar(PNext){$ELSE}CharNext(PNext){$ENDIF};
    until PNext^ = #0;
  end; { if }
  Result := Spaces - (BeforeSpaces + AfterSpaces);
end;  { CountSpaces }

// returns the beginning spaces of a string
// and deletes the spaces from the original string
function TRPRender.GetSpaces(var Line: string): string;
var
  PNext: PChar;
begin
  Result := '';
  PNext := @Line[1];
  if Assigned(PNext) then begin
    repeat
      if PNext^ <> ' ' then begin
        Line := PNext;
        Exit;
      end; { if }
      Result := Result + ' ';
      PNext := {$IFDEF LEVEL6}StrNextChar(PNext){$ELSE}CharNext(PNext){$ENDIF};
    until PNext^ = #0;
  end; { if }
  Line := '';
end;  { GetSpaces }

procedure TRPRender.PrintRightWidth(var X1, Y1: double; Text: string; Width: double);
begin
end;

procedure TRPRender.SelectBin(ABin: string);
begin { SelectBin }
end;  { SelectBin }

procedure TRPRender.SelectBin(ABin: integer);
begin { SelectBin }
end;  { SelectBin }

procedure TRPRender.PrintData(Value: string);
begin { PrintData }
end;  { PrintData }

procedure TRPRender.PrintDataStream(Stream: TStream;
                                    BufSize: longint);
begin { PrintDataStream }
end;  { PrintDataStream }

procedure TRPRender.FrameRect(const Rect: TRect);

begin { FrameRect }
end;  { FrameRect }


procedure TRPRender.BrushCopy(const Dest: TRect;
                                        Bitmap: TBitmap;
                                  const Source: TRect;
                                        Color: TColor);

begin { BrushCopy }
end;  { BrushCopy }

procedure TRPRender.CopyRect(const Dest: TRect;
                                   Canvas: TCanvas;
                             const Source: TRect);
begin { CopyRect }
end;  { CopyRect }

procedure TRPRender.DrawFocusRect(const Rect: TRect);
begin { DrawFocusRect }
end;  { DrawFocusRect }

procedure TRPRender.SetBKColor(Value: TColor);
begin { SetBKColor }
end;  { SetBKColor }

procedure TRPRender.AllowPreviewOnly;

begin { AllowPreviewOnly }
  Exclude(ValidDestination,destPrinter);
  Include(ValidDestination,destPreview);
end;  { AllowPreviewOnly }

procedure TRPRender.AllowPrinterOnly;

begin { AllowPrinterOnly }
  Exclude(ValidDestination,destPreview);
  Include(ValidDestination,destPrinter);
end;  { AllowPrinterOnly }

procedure TRPRender.AllowAll;

begin { AllowAll }
  Include(ValidDestination,destPrinter);
  Include(ValidDestination,destPreview);
end;  { AllowAll }

function TRPRender.GetOutputInvalid: boolean;

begin { GetOutputInvalid }
  Result := not (Destination in ValidDestination);
end;  { GetOutputInvalid }

function TRPRender.Destination: TDestination;

begin { Destination }
  Result := destPrinter;
end;  { Destination }

procedure TRPRender.AdjustXY(var X1,Y1: double;
                                       Width: double);
begin { AdjustXY }
  if Converter.FontData.Rotation = 0 then begin
    X1 := X1 + Width;
  end else begin
    X1 := X1 + Cos(Pi * Converter.FontData.Rotation / 180.0) * Width;
    Y1 := Y1 - Sin(Pi * Converter.FontData.Rotation / 180.0) * Width;
  end; { else }
end;  { AdjustXY }

function TRPRender.XI2D(Pos: double): integer;
begin
  Result := Round(Pos * XDPI);
end;

function TRPRender.YI2D(Pos: double): integer;
begin
  Result := Round(Pos * YDPI);
end;

{ TRPRenderStream }

destructor TRPRenderStream.Destroy;
begin
  FreeAndNil(FOwnedStream);
  if IsInternalNDRStream then begin
    FreeAndNil(FNDRStream);
    IsInternalNDRStream := false;
  end; { if }
  inherited;
end;

procedure TRPRenderStream.InitFileStream(const AFileName: string);
begin
  FOwnedStream := TFileStream.Create(AFilename,fmCreate);
  OutputStream := FOwnedStream;
end;

procedure TRPRenderStream.FreeFileStream;
begin
  FreeAndNil(FOwnedStream);
  OutputStream := nil;
  if IsInternalNDRStream then begin
    NDRStream.Free;
    NDRStream := nil;
  end;
end;

procedure TRPRenderStream.SetOutputStream(AStream: TStream);
begin
  FOutputStream := AStream;
  FActiveStream := AStream;
end;

procedure TRPRenderStream.Print(const AData: string);
begin
  // MBCS-SAFE
  if AData <> '' then begin
    ActiveStream.WriteBuffer(AData[1], Length(AData));
  end; { if }
end;

procedure TRPRenderStream.PrintF(const AData: string;
                                 const AArgs: array of const);
begin
  Print(Format(AData, AArgs));
end;

procedure TRPRenderStream.PrintLn(const AData: string);
begin
  Print(AData + #13#10);
end;

procedure TRPRenderStream.PrintLnF(const AData: string;
                                   const AArgs: array of const);
begin
  PrintLn(Format(AData, AArgs));
end;

{ TNDGraphic }

procedure TNDGraphic.Assign(Source: TPersistent);
begin
  if Source is TNDGraphic then begin
    FreePixmap;
    if Source <> nil then begin
      FPixmap := QPixmap_create(TNDGraphic(Source).FPixmap);
      TNDGraphic(Source).FStream.Position := 0;
      FStream.CopyFrom(TNDGraphic(Source).FStream,TNDGraphic(Source).FStream.Size);
      FCompression := TNDGraphic(Source).FCompression;
      FFormat := TNDGraphic(Source).FFormat;
    end; { if }
  end else if Source is TMetaFile then begin
  //!!PORT!!
  end else if Source is TImage then begin
    FPixmap := TImage(Source).Picture.Bitmap.Handle;
  end else if Source is TPicture then begin
    FPixmap := TPicture(Source).Bitmap.Handle;
  end else if Source is TBitmap then begin
    FreePixmap;
    if Source <> nil then begin
      FPixmap := QPixmap_create(TBitmap(Source).Handle);
      TBitmap(Source).SaveToStream(FStream);
    end; { if }
  end else begin
    inherited;
  end;
end;

procedure TNDGraphic.Compress;
begin
  //
  // This doesn't do anything.
  // It is provided for compatability with the TJpegImage class
  // Compression is done when saving the file
end;

constructor TNDGraphic.Create;
begin
  inherited Create;
  FFormat := gfJPEG;
  FCompression := -1;
  FPixmap := QPixmap_create;
  FStream := TMemoryStream.Create;
end;

destructor TNDGraphic.Destroy;
begin
  FreePixmap;
  FreeAndNil(FStream);
  inherited Destroy;
end;

procedure TNDGraphic.Draw(ACanvas: TCanvas; const Rect: TRect);
begin
//
end;

procedure TNDGraphic.FreePixmap;
begin
  if FPixmap <> nil then begin
    QPixmap_destroy(FPixmap);
    FPixmap := nil;
  end;
end;

function TNDGraphic.GetEmpty: Boolean;
begin
  Result := ((FPixmap = nil) or QPixmap_isNull(FPixmap));
end;

function TNDGraphic.GetFormatString: string;
begin
  if FFormat = gfJPEG then begin
    Result := 'JPEG';
  end else if FFormat = gfBMP then begin
    Result := 'BMP';
  end else if FFormat = gfMTF then begin
    raise Exception.Create('METAFILE NOT SUPPOTED IN NDGraphic');
  end;
end;

function TNDGraphic.GetHeight: integer;
begin
  if FPixmap <> nil then begin
    Result := QPixmap_height(FPixmap);
  end else begin
    Result := 0;
  end; { else }
end;

function TNDGraphic.GetWidth: integer;
begin
  if FPixmap <> nil then begin
    Result := QPixmap_width(FPixmap);
  end else begin
    Result := 0;
  end; { else }
end;

procedure TNDGraphic.LoadFromMimeSource(MimeSource: TMimeSource);
begin
//
end;

procedure TNDGraphic.LoadFromStream(Stream: TStream);
begin
  if Stream.Size > 0 then begin
    Stream.Position := 0;
    FStream.CopyFrom(Stream, Stream.Size);
    if QPixmap_loadFromData(FPixmap, TMemoryStream(FStream).Memory, Stream.Size, PChar(
      GetFormatString), 0) then begin
    end else begin
      raise Exception.Create('Could not load image');
    end;
  end;
end;

procedure TNDGraphic.SaveToFile(const Filename: string);
var
  pWC: PWideChar;
begin
  // MBCS-SAFE
  // Since it's using WideChar which supports UNICODE.
  if FPixmap <> nil then begin
    GetMem(pWC, 2 * (Length(FileName) + 1));
    try
      StringToWideChar(FileName, pWC, 2 * (Length(FileName) + 1));
      if (FCompression < 0) then begin
        QPixmap_save(FPixmap, @pWC, PChar(GetFormatString), -1);
      end else begin
        QPixmap_save(FPixmap, @pWC, PChar(GetFormatString), 100 - FCompression);
      end;
    finally
      FreeMem(pWC);
    end;
  end;
end;

procedure TNDGraphic.SaveToMimeSource(MimeSource: TClxMimeSource);
begin
//
end;

procedure TNDGraphic.SaveToStream(Stream: TStream);
begin
  if FStream <> nil then begin
    FStream.Position := 0;
    Stream.CopyFrom(FStream, FStream.Size);
  end;
end;

procedure TNDGraphic.SetCompression(AValue: integer);
begin
  if FFormat <> gfJPEG then begin
    FCompression := -2;
  end else if (AValue < -1) or (AValue > 100) then begin
    FCompression := -1;
  end else begin
    FCompression := AValue;
  end;
end;

procedure TNDGraphic.SetHeight(Value: integer);
begin
//
end;

procedure TNDGraphic.SetWidth(Value: integer);
begin
//
end;

end.