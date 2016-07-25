{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRvCsStd;

interface

uses
  Windows,
  Qt, QControls, QGraphics, QDialogs, QTypes,
{$IFDEF LEVEL6}
  Types,
{$ENDIF}
  Classes, SysUtils, QRvDefine, QRvUtil, QRvClass, QRvData, QRpDefine, QRpBase,
  QRpMemo, QRvDataField;

type
  TRaveFontMaster = class(TRaveComponent)
  protected
    FFont: TRaveFont;
    procedure SetFont(Value: TRaveFont);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
  published
    property Font: TRaveFont read FFont write SetFont;
  end; { TRaveFontMaster }

  TRavePageNumInit = class(TRaveComponent)
  protected
    FInitValue: integer;
    FInitDataView: TRaveBaseDataView;
    FInitDataField: TRaveFieldName;
    //
    procedure Changing(OldItem, NewItem: TRaveComponent); override;
  public
    constructor Create(AOwner: TComponent); override;
    procedure Print(AReport: TBaseReport); override;
  published
    property InitValue: integer read FInitValue write FInitValue default 1;
    property InitDataView: TRaveBaseDataView read FInitDataView write FInitDataView default nil;
    property InitDataField: TRaveFieldName read FInitDataField write FInitDataField;
  end; { TRavePageNumInit }

  TRaveCustomText = class(TRaveControl)
  protected
    FFont: TRaveFont;
    FFontJustify: TPrintJustify;
    FText: string;
    FFontMirror: TRaveFontMaster;
    FOnGetText: TRaveStringEvent;
    BracketText: boolean;

    function MakeFont: HFont;
    procedure PaintText(    Handle: THandle;
                            Text: string;
                            PaintLeft: TRaveUnits;
                            PaintRight: TRaveUnits;
                        var PaintTop: TRaveUnits;
                            Truncate: boolean = false);
    procedure SetFontAttr(Index: TFontAttribute; Value: TRaveFont); override;
    function GetFontAttr(Index: TFontAttribute): TRaveFont; override;
    procedure SetText(Value: string); override;
    function GetText: string; override;
    procedure SetFont(Value: TRaveFont);
    procedure SetFontJustify(Value: TPrintJustify); override;
    function GetFontJustify: TPrintJustify; override;
    procedure SetForeColor(Value: TColor); override;
    function GetForeColor: TColor; override;
    procedure SetFontMirror(Value: TRaveFontMaster);
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
    procedure DoGetText(var Value: string);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    property FontMirror: TRaveFontMaster read FFontMirror write SetFontMirror;
    property Color: TColor read GetForeColor write SetForeColor default clBlack;
    property Font: TRaveFont read FFont write SetFont;
  end; { TRaveCustomText }

  TRaveText = class(TRaveCustomText)
  protected
    FTruncate: boolean;
    FRotation: integer;
    function GetHeight: TRaveUnits; override;
  public
    constructor Create(AOwner: TComponent); override;
    procedure CreatePips; override;
    procedure UpdatePips; override;
    procedure Paint(Canvas: TCanvas); override;
    procedure Print(Report: TBaseReport); override;
  published
    property Color;
    property Font;
    property FontJustify;
    property FontMirror;
    property Left;
    property Rotation: integer read FRotation write FRotation default 0;
    property Text;
    property Top;
    property Truncate: boolean read FTruncate write FTruncate default false;
    property Width;
    property OnGetText: TRaveStringEvent read FOnGetText write FOnGetText;
  end; { TRaveText }

  TRavePrintEvent = class(TRaveSimpleEvent)
  protected
    FReport: TBaseReport;
    function SuspendValid: boolean; override;
  public
    procedure DefineParams; override;
    function ParamSource: string; override;
  published
    property Report: TBaseReport read FReport write FReport;
  end; { TRavePrintEvent }

  TRaveSection = class(TRaveContainerControl)
  protected
    FWasteFit: boolean;
    FOnPrint: TRavePrintEvent;
    procedure BeforePrint; override;
    procedure Print(Report: TBaseReport); override;
    function GetDonePrinting: boolean; override;
  public
    constructor Create(AOwner: TComponent); override;
    procedure Paint(Canvas: TCanvas); override;
  published
    property Height;
    property Left;
    property Top;
    property WasteFit: boolean read FWasteFit write FWasteFit default false;
    property Width;
    property OnPrint: TRavePrintEvent read FOnPrint write FOnPrint;
  end; { TRaveSection }

  TMatchSide = (msWidth,msHeight,msBoth,msInside);

  TRaveBaseImage = class(TRaveControl)
  protected
    FMatchSide: TMatchSide;
    FMatchHeight: boolean;
    FFileLink: TFileName;
    FDataField: TRaveFieldName;
    FDataView: TRaveBaseDataView;

    procedure SetFileLink(Value: TFileName);
    procedure SetDataField(Value: TRaveFieldName);
    procedure SetMatchSide(Value: TMatchSide);
    procedure LoadFromFile(FileName: string);
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
    procedure LoadFromStream(Stream: TStream); virtual; abstract;
    function ImageValid: boolean; virtual; abstract;
    procedure UnloadAltImage; virtual; abstract;
    function ImageWidth: integer; virtual; abstract;
    function ImageHeight: integer; virtual; abstract;
    procedure PaintImage(Canvas: TCanvas;
                        Dest: TRect); virtual; abstract;
    procedure PrintImage(Report: TBaseReport;
                        X1,Y1,X2,Y2: TRaveUnits); virtual; abstract;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

    procedure Paint(Canvas: TCanvas); override;
    procedure Print(Report: TBaseReport); override;
    function ImageName: string; virtual;
    function ImageFilter: string; virtual;
  published
    property Height;
    property Left;
    property Top;
    property Width;
    property MatchSide: TMatchSide read FMatchSide write SetMatchSide default msWidth;
    property FileLink: TFileName read FFileLink write SetFileLink;
    property DataField: TRaveFieldName read FDataField write SetDataField;
    property DataView: TRaveBaseDataView read FDataView write FDataView default nil;
  end; { TRaveBaseImage }

  TRaveGraphicImage = class(TRaveBaseImage)
  protected
    FImage: TGraphic;
    FAltImage: TGraphic;

    function CreateGraphic: TGraphic; virtual; abstract;
    procedure SetImage(Value: TGraphic);
    function ActiveImage: TGraphic;
    procedure LoadFromStream(Stream: TStream); override;
    procedure UnloadAltImage; override;
    function ImageWidth: integer; override;
    function ImageHeight: integer; override;
    procedure PaintImage(Canvas: TCanvas;
                        Dest: TRect); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
  published
    property Image: TGraphic read FImage write SetImage;
  end; { TRaveGraphicImage }

  TRaveBitmap = class(TRaveGraphicImage)
  protected
    function CreateGraphic: TGraphic; override;
    function ImageValid: boolean; override;
    procedure PrintImage(Report: TBaseReport;
                        X1,Y1,X2,Y2: TRaveUnits); override;
  public
    function ImageName: string; override;
    function ImageFilter: string; override;
  end; { TRaveBitmap }

  TRaveMetaFile = class(TRaveGraphicImage)
  protected
    function CreateGraphic: TGraphic; override;
    function ImageValid: boolean; override;
    procedure PrintImage(Report: TBaseReport;
                        X1,Y1,X2,Y2: TRaveUnits); override;
  public
    function ImageName: string; override;
    function ImageFilter: string; override;
  end; { TRaveMetaFile }

  TRaveMailMergeItem = class(TPersistent)
  protected
    FSearchToken: string;
    FReplaceString: string;
    FCaseMatters: boolean;
  published
    property CaseMatters: boolean read FCaseMatters write FCaseMatters default false;
    property ReplaceString: string read FReplaceString write FReplaceString;
    property SearchToken: string read FSearchToken write FSearchToken;
  end; { TRaveMailMergeItem }

  TRaveMemo = class(TRaveCustomText)
  protected
    SaveAnchor: TRaveAnchor;
    FExpandParent: boolean;
    FTruncate: boolean;
    Prepared: boolean;
    MemoBuf: TMemoBuf;
    LineList: TStrings;
    FMailMergeItems: TRavePersistentList;
    FContainsRTF: boolean;
    procedure SetMailMergeItems(Value: TRavePersistentList);
    procedure InitMemoBuf;
    procedure FreeMemoBuf;
    procedure SetMemoText(Value: TRaveMemoString);
    function GetMemoText: TRaveMemoString;
    procedure InitData; override;
    function GetDonePrinting: boolean; override;
    procedure BeforeReport; override;
    procedure AfterReport; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Paint(Canvas: TCanvas); override;
    procedure Resize; override;
    procedure SetText(Value: string); override;
    procedure Print(Report: TBaseReport); override;
    function HeightToPrint: TRaveUnits; override;
    property ContainsRTF: boolean read FContainsRTF write FContainsRTF default false;
  published
    property Color;
    property ExpandParent: boolean read FExpandParent write FExpandParent default true;
    property Font;
    property FontJustify;
    property FontMirror;
    property Left;
    property Height;
    property MailMergeItems: TRavePersistentList read FMailMergeItems write
     SetMailMergeItems;
    property Text: TRaveMemoString read GetMemoText write SetMemoText;
    property Top;
    property Truncate: boolean read FTruncate write FTruncate default false;
    property Width;
  end; { TRaveMemo }

  procedure RaveRegister;
  function CreateTextControl(AOwner: TComponent;
                             DataField: TRaveDataField): TRaveControl;

implementation

uses
  QRvProj;

function CreateTextControl(AOwner: TComponent;
                           DataField: TRaveDataField): TRaveControl;
begin { CreateTextControl }
  Result := TRaveText.Create(AOwner);
  TRaveText(Result).Text := DataField.FullName;
end;  { CreateTextControl }

procedure RaveRegister;
begin { RaveRegister }
  RegisterRaveComponents(Trans('Standard'),{Trans-}'Standard',{Trans-}'RvCsStd',[TRaveText,TRaveMemo,
   TRaveSection,TRaveBitmap,TRaveMetaFile,TRaveFontMaster,TRavePageNumInit]);
end;  { RaveRegister }

{ TRaveFontMaster }

constructor TRaveFontMaster.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FFont := TRaveFont.Create;
  FFont.Name := {Trans-}'Arial';
  FFont.Size := 10;
  FFont.Style := [];
  FFont.Color := clBlack;
end;  { Create }

destructor TRaveFontMaster.Destroy;
begin { Destroy }
  FreeAndNil(FFont);
  inherited Destroy;
end;  { Destroy }

procedure TRaveFontMaster.SetFont(Value: TRaveFont);
begin { SetFont }
  FFont.Assign(Value);
  Invalidate;
end;  { SetFont }

{ TRavePageNumInit }

constructor TRavePageNumInit.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FInitValue := 1;
end;  { Create }

procedure TRavePageNumInit.Changing(OldItem: TRaveComponent;
                                    NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(InitDataView) and (InitDataView = OldItem) then begin
    InitDataView := NewItem as TRaveBaseDataView;
  end; { if }
end;  { Changing }

procedure TRavePageNumInit.Print(AReport: TBaseReport);
var
  Value: integer;
begin { Print }
  if InitDataField <> '' then begin
    Value := Trunc(StrToFloatDef(StripFloatStr(ProcessDataStr(self,InitDataView,
     InitDataField)), 1.0));
  end else begin
    Value := InitValue;
  end; { else }
  AReport.PageNumOffset := Value - AReport.CurrentPage;
end;  { Print }

{ TRaveCustomText }

constructor TRaveCustomText.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FFont := TRaveFont.Create;
  FFont.Name := {Trans-}'Arial';
  FFont.Size := 10;
  FFont.Style := [];
  FFont.Color := clBlack;
  FFontJustify := pjLeft;
end;  { Create }

destructor TRaveCustomText.Destroy;
begin { Destroy }
  FreeAndNil(FFont);
  inherited Destroy;
end;  { Destroy }

procedure TRaveCustomText.Changing(OldItem: TRaveComponent;
                                   NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(FontMirror) and (FontMirror = OldItem) then begin
    FontMirror := NewItem as TRaveFontMaster;
  end; { if }
end;  { Changing }

procedure TRaveCustomText.DoGetText(var Value: string);
begin { DoGetText }
  if Assigned(FOnGetText) then begin
    FOnGetText.Value := Value;
    FOnGetText.Execute;
    Value := FOnGetText.Value;
  end; { if }
end;  { DoGetText }

function TRaveCustomText.MakeFont: HFont;

var
  LogFont: TLogFont;
  BaseFont: TRaveFont;

begin { MakeFont }
  if Assigned(FFontMirror) then begin
    BaseFont := FFontMirror.Font;
  end else begin
    BaseFont := FFont;
  end; { else }
  with LogFont do begin
    lfHeight := -YI2P(Abs(BaseFont.Size / 72.0));
    lfWidth := 0;
    lfEscapement := 0; {BaseFont.Rotation * 10;}
    lfOrientation := 0;
    if fsBold in BaseFont.Style then begin
      lfWeight := FW_BOLD;
    end else begin
      lfWeight := FW_NORMAL;
    end; { else }
    lfItalic := byte(fsItalic in BaseFont.Style);
    lfUnderline := byte(fsUnderline in BaseFont.Style);
    lfStrikeOut := byte(fsStrikeout in BaseFont.Style);
    lfCharset := ConvertCharset(CLXToVCLCharSet(BaseFont.Charset));
    StrPCopy(lfFaceName,BaseFont.Name);
    lfQuality := DEFAULT_QUALITY;
    lfOutPrecision := OUT_DEFAULT_PRECIS;
    lfClipPrecision := CLIP_DEFAULT_PRECIS;
    case BaseFont.Pitch of
      fpVariable: begin
        lfPitchAndFamily := VARIABLE_PITCH;
      end;
      fpFixed: begin
        lfPitchAndFamily := FIXED_PITCH;
      end;
      else begin
        lfPitchAndFamily := DEFAULT_PITCH;
      end;
    end; { case }
  end; { with }
  Result := CreateFontIndirect(LogFont);
end;  { MakeFont }

procedure TRaveCustomText.PaintText(    Handle: THandle;
                                        Text: string;
                                        PaintLeft: TRaveUnits;
                                        PaintRight: TRaveUnits;
                                    var PaintTop: TRaveUnits;
                                        Truncate: boolean = false);
const
  Justify: array[TPrintJustify] of integer = (TA_CENTER,TA_LEFT,TA_RIGHT,TA_LEFT);
var
  TempFont: HFont;
  TextMetric: TTextMetric;
  XL,XC,XR: integer;
  Y1: integer;
  SaveTA: integer;
  SaveColor: TColor;
  S1: string;
begin { PaintText }
  TempFont := MakeFont;
  TempFont := SelectObject(Handle,TempFont);
  if Assigned(FFontMirror) then begin
    SaveColor := SetTextColor(Handle,ColorToRGB(ModColor(FFontMirror.Font.Color)));
    PaintTop := PaintTop + Abs(FFontMirror.Font.Size) / 60.0;
  end else begin
    SaveColor := SetTextColor(Handle,ColorToRGB(ModColor(FFont.Color)));
    PaintTop := PaintTop + Abs(FFont.Size) / 60.0;
  end; { else }
  GetTextMetrics(Handle,TextMetric);
  Y1 := YI2D(PaintTop) - TextMetric.tmDescent;
  if Text <> '' then begin
    SetBKMode(Handle,TRANSPARENT);
    SaveTA := SetTextAlign(Handle,Justify[FontJustify] or TA_BASELINE or
     TA_NOUPDATECP);
    if BracketText then begin
      XL := XI2D(PaintLeft);
      XR := XI2D(PaintRight);
      case FontJustify of
        pjCenter: begin
          XC := XI2D((PaintLeft + PaintRight) / 2.0);
          S1 := Text;
          TextOut(Handle,XC,Y1,@S1[1],Length(S1));
          SetTextAlign(Handle,TA_LEFT or TA_BASELINE or TA_NOUPDATECP);
          S1 := '[';
          TextOut(Handle,XL,Y1,@S1[1],Length(S1));
          SetTextAlign(Handle,TA_RIGHT or TA_BASELINE or TA_NOUPDATECP);
          S1 := ']';
          TextOut(Handle,XR,Y1,@S1[1],Length(S1));
        end;
        pjRight: begin
          S1 := Text + ']';
          TextOut(Handle,XR,Y1,@S1[1],Length(S1));
          SetTextAlign(Handle,TA_LEFT or TA_BASELINE or TA_NOUPDATECP);
          S1 := '[';
          TextOut(Handle,XL,Y1,@S1[1],Length(S1));
        end
        else begin
          S1 := '[' + Text;
          TextOut(Handle,XL,Y1,@S1[1],Length(S1));
          SetTextAlign(Handle,TA_RIGHT or TA_BASELINE or TA_NOUPDATECP);
          S1 := ']';
          TextOut(Handle,XR,Y1,@S1[1],Length(S1));
        end;
      end; { case }
    end else begin
      case FontJustify of
        pjCenter: XL := XI2D((PaintLeft + PaintRight) / 2.0);
        pjRight: XL := XI2D(PaintRight);
        else XL := XI2D(PaintLeft);
      end; { case }
      TextOut(Handle,XL,Y1,@Text[1],Length(Text));
    end; { else }
    SetTextAlign(Handle,SaveTA);
    SetTextColor(Handle,SaveColor);
  end; { if }
  TempFont := SelectObject(Handle,TempFont);
  DeleteObject(TempFont);
end;  { PaintText }

procedure TRaveCustomText.SetFontAttr(Index: TFontAttribute;
                                      Value: TRaveFont);
begin { SetFontAttr }
  if Index in [faAll,faName] then begin
    FFont.Name := Value.Name;
  end; { if }
  if Index in [faAll,faSize] then begin
    FFont.Size := Value.Size;
  end; { if }
  if Index in [faAll,faBold] then begin
    if fsBold in Value.Style then begin
      FFont.Style := FFont.Style + [fsBold];
    end else begin
      FFont.Style := FFont.Style - [fsBold];
    end; { else }
  end; { if }
  if Index in [faAll,faItalic] then begin
    if fsItalic in Value.Style then begin
      FFont.Style := FFont.Style + [fsItalic];
    end else begin
      FFont.Style := FFont.Style - [fsItalic];
    end; { else }
  end; { if }
  if Index in [faAll,faUnderline] then begin
    if fsUnderline in Value.Style then begin
      FFont.Style := FFont.Style + [fsUnderline];
    end else begin
      FFont.Style := FFont.Style - [fsUnderline];
    end; { else }
  end; { if }
  Resize;
end;  { SetFontAttr }

function TRaveCustomText.GetFontAttr(Index: TFontAttribute): TRaveFont;
begin { GetFontAttr }
  if Assigned(FontMirror) then begin
    Result := nil;
  end else begin
    Result := FFont;
  end; { if }
end;  { GetFontAttr }

procedure TRaveCustomText.SetText(Value: string);
begin { SetText }
  if FText <> Value then begin
    FText := Value;
    Invalidate;
  end; { if }
end;  { SetText }

function TRaveCustomText.GetText: string;
begin { GetText }
  Result := FText;
end;  { GetText }

procedure TRaveCustomText.SetForeColor(Value: TColor);
begin { SetForeColor }
  FFont.Color := Value;
  Invalidate;
end;  { SetForeColor }

function TRaveCustomText.GetForeColor: TColor;
begin { GetForeColor }
  if Assigned(FontMirror) then begin
    Result := clNone;
  end else begin
    Result := FFont.Color;
  end; { else }
end;  { GetForeColor }

procedure TRaveCustomText.SetFontMirror(Value: TRaveFontMaster);
begin { SetFontMirror }
  FFontMirror := Value;
  Invalidate;
end;  { SetFontMirror }

procedure TRaveCustomText.SetFont(Value: TRaveFont);
begin { SetFont }
  FFont.Assign(Value);
  Invalidate;
// !!! comment out because it fixes the stack overflow bug
// when trying to load projects with mirrors set
//    If Assigned(Page.Designer) then begin
//      Page.Designer.Modified;
//    end; { if }
end;  { SetFont }

procedure TRaveCustomText.SetFontJustify(Value: TPrintJustify);
begin { SetFontJustify }
  if FFontJustify <> Value then begin
    FFontJustify := Value;
    Invalidate;
  end; { if }
end;  { SetFontJustify }

function TRaveCustomText.GetFontJustify: TPrintJustify;
begin { GetFontJustify }
  Result := FFontJustify;
end;  { GetFontJustify }

{ TRaveText }

constructor TRaveText.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FTruncate := false;
  Width := 1.0;
end;  { Create }

function TRaveText.GetHeight: TRaveUnits;
begin { GetHeight }
  if Assigned(FFontMirror) then begin
    Result := Abs(FFontMirror.FFont.Size) / 60.0;
  end else begin
    Result := Abs(FFont.Size) / 60.0;
  end; { else }
end;  { GetHeight }

procedure TRaveText.CreatePips;
var
  X1: TRaveUnits;
  Y1: TRaveUnits;
begin { CreatePips }
  X1 := PageLeft;
  Y1 := PageTop;
  Page.AddPip(3,self,crSizeWE,X1,Y1 + Height / 2.0);
  Page.AddPip(4,self,crSizeWE,X1 + Width,Y1 + Height / 2.0);
end;  { CreatePips }

procedure TRaveText.UpdatePips;
var
  X1: TRaveUnits;
  Y1: TRaveUnits;
begin { UpdatePips }
  X1 := PageLeft;
  Y1 := PageTop;
  Page.UpdatePip(3,self,X1,Y1 + Height / 2.0);
  Page.UpdatePip(4,self,X1 + Width,Y1 + Height / 2.0);
end;  { UpdatePips }

procedure TRaveText.Paint(Canvas: TCanvas);
var
  Y1: TRaveUnits;
  SaveRgn: HRgn;
begin { Paint }
//  Canvas.Start;
  Canvas.Font.Name := Font.Name;
  Canvas.Font.Style := Font.Style;
  Canvas.Font.Height := -YI2P(Abs(Font.Size / 72.0));
  Y1 := PageTop;
  SaveRgn := SetClipped(Canvas);
  if CurrentDesigner.IsSelected(self) then begin
    with Canvas do begin
      Pen.Color := ModColor(clSilver);
      Pen.Style := psSolid;
      Pen.Mode := pmCopy;
      Pen.Width := 1;
      Brush.Color := ModColor(clBlack);
      Brush.Style := bsClear;
      Rectangle(XI2D(PageLeft),YI2D(PageTop),XI2D(PageRight) + 1,YI2D(PageBottom) + 1);
    end; { with }
  end; { if }
  PaintText(QPainter_handle(Canvas.Handle),Text,PageLeft,PageRight,Y1,Truncate);
  SetUnclipped(Canvas,SaveRgn);
//  Canvas.Stop;
end;  { Paint }

procedure TRaveText.Print(Report: TBaseReport);
var
  S1: string;
begin { Print }
  if not AllowPrint then Exit;
  with Report do begin
    S1 := Text;
    DoGetText(S1);
    if Assigned(FontMirror) then begin
      AssignFont(FontMirror.Font);
    end else begin
      AssignFont(Font);
    end; { else }
    FontRotation := Rotation;
    LineTop := PageTop;
    if Truncate then begin { Truncate text }
      S1 := TruncateText(S1,Width);
    end; { if }
    TextFieldWidth := Width;
    PrintJustify(S1,PageLeft,FontJustify,0.0,Width);
  end; { with }
end;  { Print }

{ TRavePrintEvent }

function TRavePrintEvent.SuspendValid: boolean;
begin
  Result := true;
end;

procedure TRavePrintEvent.DefineParams;
begin
  DefineParam(FReport);
end;

function TRavePrintEvent.ParamSource: string;
begin
  Result := 'Report: TBaseReport';
end;

{ TRaveSection }

constructor TRaveSection.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FControlStyle := FControlStyle + [rcsContainer];
  Height := 3.0;
  Width := 3.0;
end;  { Create }

function TRaveSection.GetDonePrinting: boolean;
var
  I1: integer;
begin
  Result := not Assigned(FOnPrint) or not OnPrint.Suspended;
  for I1 := 0 to ChildCount - 1 do begin
    if not Child[I1].DonePrinting then begin
      Result := false;
      Exit;
    end; { if }
  end; { for }
end;

procedure TRaveSection.BeforePrint;
var
  WX1,WY1: TRaveUnits;
  WX2,WY2: TRaveUnits;
  BaseReport: TBaseReport;
begin
{ Make sure section fits within waste area }
  if WasteFit then begin
    BaseReport := TRaveProjectManager(Project).BaseReport;
    WX1 := BaseReport.LeftWaste + 0.01;
    WY1 := BaseReport.TopWaste + 0.01;
    WX2 := BaseReport.PageWidth - (BaseReport.RightWaste + 0.01);
    WY2 := BaseReport.PageHeight - (BaseReport.BottomWaste + 0.01);
    if FLT(Left,WX1) or FLT(Top,WY1) or FGT(Right,WX2) or FGT(Bottom,WY2) then begin
      SetBoundsRect(RaveRect(MaxF(Left,WX1),MaxF(Top,WY1),
       MinF(Right,WX2),MinF(Bottom,WY2)));
    end; { if }
  end; { if }

  inherited;
end;

procedure TRaveSection.Paint(Canvas: TCanvas);
begin { Paint }
  with Canvas do begin
    Pen.Color := ModColor(clGray);
    Pen.Style := psDot;
    Pen.Mode := pmCopy;
    Pen.Width := 1;
    Brush.Color := ModColor(clWhite);
    Brush.Style := bsClear;
    Rectangle(XI2D(PageLeft),YI2D(PageTop),XI2D(PageRight) + 1,YI2D(PageBottom) + 1);
  end; { with }
end;  { Paint }

procedure TRaveSection.Print(Report: TBaseReport);
begin
  if Assigned(FOnPrint) then begin
    Report.OriginX := PageLeft;
    Report.OriginY := PageTop;
    Report.SectionLeft := 0;
    Report.SectionRight := Width;
    Report.SectionTop := 0;
    Report.SectionBottom := Height;
    Report.Home;
    FOnPrint.Report := Report;
    FOnPrint.Execute;
    Report.OriginX := 0;
    Report.OriginY := 0;
  end; { if }
end;

{ TRaveBaseImage }

constructor TRaveBaseImage.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  Height := 3.0;
  Width := 3.0;
end;  { Create }

destructor TRaveBaseImage.Destroy;
begin { Destroy }
  UnloadAltImage;
  inherited Destroy;
end;  { Destroy }

procedure TRaveBaseImage.LoadFromFile(FileName: string);
var
  Stream: TStream;
begin { LoadFromFile }
  if (FileName <> '') and FileExists(FileName) then begin
    Stream := TFileStream.Create(FileName,fmOpenRead);
    try
      LoadFromStream(Stream);
    finally
      Stream.Free;
    end; { tryf }
  end; { if }
end;  { LoadFromFile }

procedure TRaveBaseImage.Changing(OldItem: TRaveComponent;
                                  NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(DataView) and (DataView = OldItem) then begin
    DataView := NewItem as TRaveBaseDataView;
  end; { if }
end;  { Changing }

procedure TRaveBaseImage.SetDataField(Value: TRaveFieldName);
begin { SetDataField }
  FDataField := Value;
  Invalidate;
end;  { SetDataField }

procedure TRaveBaseImage.SetFileLink(Value: TFileName);
begin { SetFileLink }
  Value := Trim(Value);
  if FFileLink <> Value then begin
    FFileLink := Value;
    Invalidate;
  end; { if }
end;  { SetFileLink }

procedure TRaveBaseImage.SetMatchSide(Value: TMatchSide);
begin { SetMatchSide }
  FMatchSide := Value;
  Invalidate;
end;  { SetMatchSide }

procedure TRaveBaseImage.Paint(Canvas: TCanvas);
var
  Dest: TRect;
  W1,H1: integer;
  X1,Y1,X2,Y2: integer;
begin { Paint }
  with Canvas do begin
    Pen.Color := ModColor(clSilver);
    Pen.Mode := pmCopy;
    Pen.Width := 1;
    Brush.Color := ModColor(clBlack);
    Brush.Style := bsClear;
    try
      LoadFromFile(FileLink);
      X1 := XI2D(PageLeft);
      Y1 := YI2D(PageTop);
      X2 := XI2D(PageRight);
      Y2 := YI2D(PageBottom);
      if ImageValid then begin
        W1 := XI2D(Width) - XI2D(0);
        H1 := YI2D(Height) - YI2D(0);
        if CurrentDesigner.IsSelected(self) then begin
          with Canvas do begin
            Pen.Style := psSolid;
            Rectangle(X1,Y1,X2 + 1,Y2 + 1);
          end; { with }
        end; { if }
        Dest := Rect(X1,Y1,X2 + 1,Y2 + 1);
        case MatchSide of
          msWidth: Dest.Bottom := Dest.Top + ((W1 * ImageHeight) div ImageWidth);
          msHeight: Dest.Right := Dest.Left + ((ImageWidth * H1) div ImageHeight);
          msInside: begin
            if ((ImageWidth * Height) /
             (ImageHeight * Width)) > 1.0 then begin { Adjust height }
              Dest.Bottom := Dest.Top + ((W1 * ImageHeight) div ImageWidth);
            end else begin { Adjust width }
              Dest.Right := Dest.Left + ((ImageWidth * H1) div ImageHeight);
            end; { else }
          end;
        end; { case }
        PaintImage(Canvas,Dest);
      end else begin
        Pen.Style := psDash;
        Rectangle(X1,Y1,X2 + 1,Y2 + 1);
        Pen.Style := psSolid;
        MoveTo(X1,Y1);
        LineTo(X2,Y2);
        MoveTo(X1,Y2);
        LineTo(X2,Y1);
      end; { else }
    finally
      UnloadAltImage;
    end; { tryf }
  end; { with }
end;  { Paint }

procedure TRaveBaseImage.Print(Report: TBaseReport);
var
  X1,Y1,X2,Y2: TRaveUnits;
  Stream: TMemoryStream;
  Field: TRaveDataField;
  ImageFileName: string;
begin { Print }
  if not AllowPrint then Exit;
  try
    If Assigned(DataView) and (DataField <> '') then begin
      Field := DataView.FieldByName(DataField);
      if Field.IsNull then Exit;
      if Field.DataType = dtString then begin
        ImageFileName := ExpandFileName(Field.AsString);
        LoadFromFile(ImageFileName);
      end else begin
        Stream := TMemoryStream.Create;
        try
          Stream.Write(Field.AsBlob^,Field.DataSize);
          Stream.Position := 0;
          LoadFromStream(Stream);
        finally
          Stream.Free;
        end; { tryf }
      end; { else }
    end else begin
      LoadFromFile(FileLink);
    end; { else }
    X1 := PageLeft;
    Y1 := PageTop;
    X2 := PageRight;
    Y2 := PageBottom;
    with Report do begin
      if ImageValid then begin
        case MatchSide of
          msWidth: Y2 := Y1 + (ImageHeight * Width / ImageWidth);
          msHeight: X2 := X1 + (ImageWidth * Height / ImageHeight);
          msInside: begin
            if ((ImageWidth * (Y2 - Y1)) /
             (ImageHeight * (X2 - X1))) > 1.0 then begin { Adjust height }
              Y2 := Y1 + (ImageHeight * Width / ImageWidth);
            end else begin { Adjust width }
              X2 := X1 + (ImageWidth * Height / ImageHeight);
            end; { else }
          end;
        end; { case }
        PrintImage(Report,X1,Y1,X2,Y2);
      end else begin
        SetPen(clGray,psSolid,1,pmCopy);
        SetBrush(clBlack,bsClear,nil);
        Rectangle(X1,Y1,X2,Y2);
        MoveTo(X1,Y1);
        LineTo(X2,Y2);
        MoveTo(X1,Y2);
        LineTo(X2,Y1);
      end; { else }
    end; { with }
  finally
    UnloadAltImage;
  end; { tryf }
end;  { Print }

function TRaveBaseImage.ImageName: string;
begin { ImageName }
  Result := Trans('Image');
end;  { ImageName }

function TRaveBaseImage.ImageFilter: string;
begin { ImageFilter }
  Result := Trans('All Files') + {Trans-}' (*.*)|*.*';
end;  { ImageFilter }

{ TRaveGraphicImage }

constructor TRaveGraphicImage.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FImage := CreateGraphic;
end;  { Create }

destructor TRaveGraphicImage.Destroy;
begin { Destroy }
  FreeAndNil(FImage);
  inherited Destroy;
end;  { Destroy }

procedure TRaveGraphicImage.UnloadAltImage;
begin { UnloadAltImage }
  if Assigned(FAltImage) then begin
    FreeAndNil(FAltImage);
  end; { if }
end;  { UnloadAltImage }

procedure TRaveGraphicImage.LoadFromStream(Stream: TStream);
begin { LoadFromStream }
  UnloadAltImage;
  FAltImage := CreateGraphic;
  FAltImage.LoadFromStream(Stream);
  if not ImageValid then begin { Invalid image format }
    UnloadAltImage;
  end; { if }
end;  { LoadFromStream }

function TRaveGraphicImage.ActiveImage: TGraphic;
begin { ActiveImage }
  if Assigned(FAltImage) then begin
    Result := FAltImage;
  end else begin
    Result := FImage;
  end; { else }
end;  { ActiveImage }

function TRaveGraphicImage.ImageWidth: integer;
begin { ImageWidth }
  Result := ActiveImage.Width;
end;  { ImageWidth }

function TRaveGraphicImage.ImageHeight: integer;
begin { ImageHeight }
  Result := ActiveImage.Height;
end;  { ImageHeight }

procedure TRaveGraphicImage.PaintImage(Canvas: TCanvas;
                                       Dest: TRect);
begin { PaintImage }
  Canvas.StretchDraw(Dest,ActiveImage);
end;  { PaintImage }

procedure TRaveGraphicImage.SetImage(Value: TGraphic);
begin { SetImage }
  FImage.Free;
  FImage := CreateGraphic;
  FImage.Assign(Value);
end;  { SetImage }

{ TRaveBitmap }

function TRaveBitmap.CreateGraphic: TGraphic;
begin { TRaveGraphicImage }
  Result := TBitmap.Create;
end;

{ TRaveGraphicImage }

function TRaveBitmap.ImageValid: boolean;
begin { ImageValid }
  Result := (ActiveImage <> nil) and ((ActiveImage as TBitmap).Width > 0);
end;  { ImageValid }

procedure TRaveBitmap.PrintImage(Report: TBaseReport;
                                 X1,Y1,X2,Y2: TRaveUnits);
begin { PrintImage }
  Report.PrintBitmapRect(X1,Y1,X2,Y2,ActiveImage as TBitmap);
end;  { PrintImage }

function TRaveBitmap.ImageName: string;
begin { ImageName }
  Result := Trans('Bitmap');
end;  { ImageName }

function TRaveBitmap.ImageFilter: string;
begin { ImageFilter }
  Result := Trans('Bitmap Files') + {Trans-}' (*.bmp)|*.bmp';
end;  { ImageFilter }

{ TRaveMetaFile }

function TRaveMetaFile.CreateGraphic: TGraphic;
begin { TRaveGraphicImage }
  Result := TMetaFile.Create;
end;  { TRaveGraphicImage }

function TRaveMetaFile.ImageValid: boolean;
begin { ImageValid }
    Result := (ActiveImage as TMetaFile).Handle <> 0;
end;  { ImageValid }

procedure TRaveMetaFile.PrintImage(Report: TBaseReport;
                                   X1,Y1,X2,Y2: TRaveUnits);
var
  Rect: TRect;
begin { PrintImage }
  Rect := Report.CreateRect(X1,Y1,X2,Y2);
  Report.StretchDraw(Rect,ActiveImage);
end;  { PrintImage }

function TRaveMetaFile.ImageName: string;
begin { ImageName }
  Result := Trans('MetaFile');
end;  { ImageName }

function TRaveMetaFile.ImageFilter: string;
begin { ImageFilter }
  Result := Trans('MetaFile Files') + {Trans-}'(*.emf;*.wmf)|*.emf;*.wmf';
end;  { ImageFilter }

{ TRaveMailMergeList }

type
TRaveMailMergeList = class(TRavePersistentList)
public
  function CreateItem(ItemClass: string;
                      RootOwner: TComponent): TPersistent; override;
end; { TRaveMailMergeList }

function TRaveMailMergeList.CreateItem(ItemClass: string;
                                       RootOwner: TComponent): TPersistent;
begin { CreateItem }
  Result := TRaveMailMergeItem.Create;
end;  { CreateItem }

{ TRaveMemo }

constructor TRaveMemo.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  Height := 2.0;
  Width := 3.0;
  FMailMergeItems := TRaveMailMergeList.Create;
  FExpandParent := true;
end;  { Create }

destructor TRaveMemo.Destroy;
begin { Destroy }
  FreeMemoBuf;
  ClearObjectList(FMailMergeItems);
  FreeAndNil(FMailMergeItems);
  if Assigned(LineList) then begin
    FreeAndNil(LineList);
  end; { if }
  inherited Destroy;
end;  { Destroy }

procedure TRaveMemo.SetMailMergeItems(Value: TRavePersistentList);
var
  I1: integer;
  MM1: TRaveMailMergeItem;
  MM2: TRaveMailMergeItem;
begin { SetMailMergeItems }
  ClearObjectList(FMailMergeItems);
  for I1 := 0 to Value.Count - 1 do begin
    MM1 := TRaveMailMergeItem(Value[I1]);
    MM2 := TRaveMailMergeItem.Create;
    MM2.SearchToken := MM1.SearchToken;
    MM2.ReplaceString := MM1.ReplaceString;
    MM2.CaseMatters := MM1.CaseMatters;
    FMailMergeItems.Add(MM2);
  end; { for }
end;  { SetMailMergeItems }

procedure TRaveMemo.Resize;
begin { Resize }
  if Assigned(LineList) then begin
    FreeAndNil(LineList);
  end; { if }
  inherited Resize;
end;  { Resize }

procedure TRaveMemo.SetText(Value: string);
begin { SetText }
  if Assigned(LineList) then begin
    FreeAndNil(LineList);
  end; { if }
  inherited SetText(Value);
end;  { SetText }

procedure TRaveMemo.Paint(Canvas: TCanvas);
var
  EOL: boolean;
  I1: integer;
  X1,X2,Y1: TRaveUnits;
  SaveRgn: HRgn;
  LastPos: integer;
begin { Paint }
  if not Assigned(LineList) then begin
    LineList := TStringList.Create;
    MemoBuf := TMemoBuf.Create;
    try
      if Assigned(FontMirror) then begin
        TRaveProjectManager(Project).BaseReport.AssignFont(FontMirror.Font);
      end else begin
        TRaveProjectManager(Project).BaseReport.AssignFont(Font);
      end; { else }
      MemoBuf.BaseReport := TRaveProjectManager(Project).BaseReport;
      MemoBuf.PrintStart := 0.0;
      MemoBuf.PrintEnd := Width;
      MemoBuf.Text := Text;
      with MemoBuf do begin
        LastPos := -1;
        while (Pos < Size) and (Pos <> LastPos) do begin
          LastPos := Pos;
          LineList.Add(GetNextLine(EOL));
        end; { while }
      end; { with }
    finally
      FreeAndNil(MemoBuf);
    end; { tryf }
  end; { if }

{ Paint LineList }
  X1 := PageLeft;
  X2 := PageRight;
  Y1 := PageTop;
  SaveRgn := SetClipped(Canvas);
  if CurrentDesigner.IsSelected(self) then begin
    with Canvas do begin
      Pen.Color := ModColor(clSilver);
      Pen.Style := psSolid;
      Pen.Mode := pmCopy;
      Pen.Width := 1;
      Brush.Color := ModColor(clBlack);
      Brush.Style := bsClear;
      Rectangle(XI2D(X1),YI2D(Y1),XI2D(X2) + 1,YI2D(PageBottom) + 1);
    end; { with }
  end; { if }
//  Canvas.Start;
  Canvas.Font.Name := Font.Name;
  Canvas.Font.Style := Font.Style;
  Canvas.Font.Height := -YI2P(Abs(Font.Size / 72.0));
  for I1 := 0 to LineList.Count - 1 do begin
    PaintText(QPainter_handle(Canvas.Handle),LineList[I1],X1,X2,Y1);
  end; { for }
  SetUnclipped(Canvas,SaveRgn);
//  Canvas.Stop;
end;  { Paint }

procedure TRaveMemo.InitMemoBuf;
var
  I1: integer;
  MMItem: TRaveMailMergeItem;
begin { InitMemoBuf }
  if not Prepared then begin
    MemoBuf := TMemoBuf.Create;
    MemoBuf.BaseReport := TRaveProjectManager(Project).BaseReport;
    MemoBuf.PrintStart := PageLeft;
    MemoBuf.PrintEnd := PageRight;
    MemoBuf.Justify := FontJustify;
    if ContainsRTF or (AnsiCompareText(Copy(Trim(Text),1, 5),'{\rtf') = 0) then begin
      MemoBuf.RTFText := Text;
    end else begin
      MemoBuf.Text := Text;
    end; { else }
    for I1 := 0 to MailMergeItems.Count - 1 do begin { Do mail merge }
      MMItem :=TRaveMailMergeItem(MailMergeItems[I1]);
      MemoBuf.ReplaceAll(MMItem.SearchToken,ProcessDataStr(self,nil,
       MMItem.ReplaceString),MMItem.CaseMatters);
    end; { for }
    Prepared := true;
  end; { if }
end;  { InitMemoBuf }

procedure TRaveMemo.FreeMemoBuf;
begin { FreeMemoBuf }
  if Prepared then begin
    FreeAndNil(MemoBuf);
    Prepared := false;
  end; { if }
end;  { FreeMemoBuf }

function TRaveMemo.GetDonePrinting: boolean;
begin { GetDonePrinting }
  Result := not Assigned(MemoBuf) or MemoBuf.Empty;
end;  { GetDonePrinting }

procedure TRaveMemo.BeforeReport;
begin { BeforeReport }
  Prepared := false;
  SaveAnchor := Anchor;
  if ExpandParent then begin
    Anchor := TRaveAnchor(Ord(anvStretch) * 16 + Ord(GetHAnchor));
  end; { if }
  inherited BeforeReport;
end;  { BeforeReport }

procedure TRaveMemo.AfterReport;
begin { AfterReport }
  Anchor := SaveAnchor;
  inherited AfterReport;
end;  { AfterReport }

procedure TRaveMemo.SetMemoText(Value: TRaveMemoString);
begin { SetMemoText }
  SetText(Value);
end;  { SetMemoText }

function TRaveMemo.GetMemoText: TRaveMemoString;
begin { GetMemoText }
  Result := GetText;
end;  { GetMemoText }

procedure TRaveMemo.Print(Report: TBaseReport);
begin { Print }
  If not AllowPrint then Exit;
  with Report do begin
    InitMemoBuf;
    if Assigned(FontMirror) then begin
      AssignFont(FontMirror.Font);
    end else begin
      AssignFont(Font);
    end; { else }
    LineTop := PageTop;
    MemoBuf.PrintStart := PageLeft;
    MemoBuf.PrintEnd := PageRight;
    MemoBuf.PrintHeight(Height,false);
    if MemoBuf.Empty or Truncate then begin
      FreeMemoBuf;
    end; { if }
  end; { with }
end;  { Print }

procedure TRaveMemo.InitData;
begin { InitData }
  InitMemoBuf;
end;  { InitData }

function TRaveMemo.HeightToPrint: TRaveUnits;
begin { HeightToPrint }
  if not Assigned(MemoBuf) then begin
    Result := 0.0;
  end else begin
    with TRaveProjectManager(Project).BaseReport do begin
      if Assigned(FontMirror) then begin
        AssignFont(FontMirror.Font);
      end else begin
        AssignFont(Font);
      end; { else }
      ResetLineHeight;
      if ExpandParent then begin
        Result := MemoBuf.MemoHeightLeft;
      end else begin
        Result := AdjHeight;
      end; { else }
    end; { with }
  end; { else }
end;  { HeightToPrint }

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.
