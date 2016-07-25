{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvCsBars;

interface

uses
  Classes, SysUtils, Graphics,
  RpDefine, RpBase, RvClass, RpBars, RvDefine,
  RvUtil, RvData;

type
  TRaveBaseBarCode = class(TRaveControl)
  protected
    FOldText: string;
    FBarCode: TRPBarsBase;
    FFont: TRaveFont;
    FCanvas: TCanvas;
    FAutoSize: boolean;
    FDataField: TRaveFieldName;
    FDataView: TRaveBaseDataView;

    procedure SetAutoSize(Value: boolean);
    function GetLeft: TRaveUnits; override;
    procedure SetLeft(Value: TRaveUnits); override;

    function GetRight: TRaveUnits;
    procedure SetRight(Value: TRaveUnits);

    function GetTop: TRaveUnits; override;
    procedure SetTop(Value: TRaveUnits); override;
(*
    function GetBottom: TRaveUnits; override;
    procedure SetBottom(Value: TRaveUnits); override;
*)
    procedure SetHeight(Value: TRaveUnits); override;
    procedure SetWidth(Value: TRaveUnits); override;

    procedure SetFont(Value: TRaveFont);

    procedure SetBarCodeJustify(Value: TPrintJustify);
    function GetBarCodeJustify: TPrintJustify;
    procedure SetTextJustify(Value: TPrintJustify);
    function GetTextJustify: TPrintJustify;
    procedure SetBarWidth(Value: TRaveUnits);
    function GetBarWidth: TRaveUnits;
    procedure SetBarHeight(Value: TRaveUnits);
    function GetBarHeight: TRaveUnits;
    function GetBarTop: TRaveUnits;
    procedure SetBarTop(Value: TRaveUnits);
    function GetCenter: TRaveUnits;
    procedure SetCenter(Value: TRaveUnits);
    procedure SetText(Value: string); override;
    function GetText: string; override;
    procedure SetUseChecksum(Value: boolean);
    function GetUseChecksum: boolean;
    procedure SetWideFactor(Value: TRaveFloat);
    function GetWideFactor: TRaveFloat;
    procedure SetPrintReadable(Value: boolean);
    function GetPrintReadable: boolean;
    procedure SetPrintChecksum(Value: boolean);
    function GetPrintChecksum: boolean;
    procedure SetPrintTop(Value: boolean);
    function GetPrintTop: boolean;
    procedure SetBarCodeRotation(Value: TBarCodeRotation);
    function GetBarCodeRotation: TBarCodeRotation;
    procedure PrintBar(X1,Y1,X2,Y2: double);
    procedure SetDataField(Value: TRaveFieldName);
    procedure BeforeReport; override;
    procedure AfterReport; override;
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
    function IsLeftStored: boolean;
    function IsCenterStored: boolean;
    function IsRightStored: boolean;
    function CreateBarCodeClass: TRPBarsBase; virtual; abstract;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Resize; override;
    procedure Paint(Canvas: TCanvas); override;
    procedure Print(Report: TBaseReport); override;
    property Text: string read GetText write SetText;
  published
    property DataField: TRaveFieldName read FDataField write SetDataField;
    property DataView: TRaveBaseDataView read FDataView write FDataView default nil;
    property AutoSize: boolean read FAutoSize write SetAutoSize default true;
    property BarCodeJustify: TPrintJustify read GetBarCodeJustify
     write SetBarCodeJustify stored false;
    property Top: TRaveUnits read GetTop write SetTop;
    property Bottom: TRaveUnits read GetBottom write SetBottom;
    property Height;
    property Width;
    property Font: TRaveFont read FFont write SetFont;
    property BarWidth: TRaveUnits read GetBarWidth write SetBarWidth;
    property BarHeight: TRaveUnits read GetBarHeight write SetBarHeight;
    property BarTop: TRaveUnits read GetBarTop write SetBarTop;
  { Don't move Left, Right and Center above the Width Property since the
    width needs to be set before Center & Right when loading }
    property Left stored IsLeftStored;
    property Right: TRaveUnits read GetRight write SetRight stored IsRightStored;
    property Center: TRaveUnits read GetCenter write SetCenter stored IsCenterStored;
    property TextJustify: TPrintJustify read GetTextJustify write SetTextJustify
     default pjLeft;
    property UseChecksum: boolean read GetUseChecksum write SetUseChecksum
     default false;
    property WideFactor: TRaveFloat read GetWideFactor write SetWideFactor;
    property PrintReadable: boolean read GetPrintReadable write SetPrintReadable
     default true;
    property PrintChecksum: boolean read GetPrintChecksum write SetPrintChecksum
     default false;
    property PrintTop: boolean read GetPrintTop write SetPrintTop default false;
    property BarCodeRotation: TBarCodeRotation read GetBarCodeRotation
     write SetBarCodeRotation default Rot0;
  end; { TRaveBaseBarCode }

  TRavePostNetBarCode = class(TRaveBaseBarCode)
  protected
    function CreateBarCodeClass: TRPBarsBase; override;
    procedure SetHeight(Value: TRaveUnits); override;
  public
    constructor Create(AOwner: TComponent); override;
  published
    property PrintReadable default false;
    property Height write SetHeight;
    property Text;
  end;

  TRaveI2of5BarCode = class(TRaveBaseBarCode)
  protected
    function CreateBarCodeClass: TRPBarsBase; override;
  public
    constructor Create(AOwner: TComponent); override;
  published
    property Text;
  end;

  TRaveCode39BarCode = class(TRaveBaseBarCode)
  protected
    procedure SetExtended(Value: boolean);
    function GetExtended: boolean;
    function CreateBarCodeClass: TRPBarsBase; override;
  public
    constructor Create(AOwner: TComponent); override;
  published
    property Extended: boolean read GetExtended write SetExtended;
    property Text;
  end;

  TRaveCode128BarCode = class(TRaveBaseBarCode)
  protected
    procedure SetCodePage(Value: TCodePage128);
    function GetCodePage: TCodePage128;
    procedure SetOptimize(Value: boolean);
    function GetOptimize: boolean;
    function CreateBarCodeClass: TRPBarsBase; override;
  public
    constructor Create(AOwner: TComponent); override;
  published
    property CodePage: TCodePage128 read GetCodePage write SetCodePage;
    property Optimize: boolean read GetOptimize write SetOptimize;
    property Text;
  end;

  TRaveUPCBarCode = class(TRaveBaseBarCode)
  protected
    function CreateBarCodeClass: TRPBarsBase; override;
  public
    constructor Create(AOwner: TComponent); override;
  published
    property Text;
  end;

  TRaveEANBarCode = class(TRaveBaseBarCode)
  protected
    function CreateBarCodeClass: TRPBarsBase; override;
  public
    constructor Create(AOwner: TComponent); override;
  published
    property Text;
  end;

  procedure RaveRegister;

implementation

uses
  RvProj;

  procedure RaveRegister;
  begin { RaveRegister }
    RegisterRaveComponents({Trans-}'BarCodes',Trans('Bar Code'),{Trans-}'RVCsBars',
     [TRavePostNetBarCode,TRaveI2of5BarCode,TRaveCode39BarCode,TRaveCode128BarCode,
     TRaveUPCBarCode,TRaveEANBarCode]);
  end;  { RaveRegister }

{ class TRaveBaseBarCode }

constructor TRaveBaseBarCode.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FBarCode := CreateBarCodeClass;
  FFont := TRaveFont.Create;
  FFont.Name := {Trans-}'Arial';
  FAutoSize := true;
  BarCodeJustify := pjLeft;
  TextJustify := pjLeft;
  BarCodeRotation := Rot0;
end;  { Create }

destructor TRaveBaseBarCode.Destroy;
begin { Destroy }
  FreeAndNil(FBarCode);
  FreeAndNil(FFont);
  inherited Destroy;
end;  { Destroy }

procedure TRaveBaseBarCode.SetAutoSize(Value: boolean);
begin { SetAutoSize }
  FAutoSize := Value;
  Height := FBarCode.Height;
  Width := FBarCode.Width;
  Resize;
end;  { SetAutoSize }

procedure TRaveBaseBarCode.Resize;
begin { Resize }
  if Assigned(FBarCode) then begin
    case BarCodeJustify of
      pjLeft,pjBlock: FBarCode.Left := Left;
      pjCenter: FBarCode.Center := (Left + Right) / 2.0;
      pjRight: FBarCode.Right := Right;
    end;
    FBarCode.Top := Top;
    FBarCode.Bottom := Bottom;
    FBarCode.BarHeight := (Bottom - Top) - FBarCode.ReadableHeight;
    if FAutoSize then begin
      if Flt2Str(FBarCode.Width,FloatPropPrecision) <> Flt2Str(Width,FloatPropPrecision) then begin
        Width := FBarCode.Width;
      end; { if }
    end; { if }
  end; { if }
  inherited Resize;
end;  { Resize }

procedure TRaveBaseBarCode.SetLeft(Value: TRaveUnits);
var
  SaveJustify: TPrintJustify;
begin { SetLeft }
  SaveJustify := BarCodeJustify;
  FBarCode.Left := Value;
  FLeft := Value;
  FBarCode.BarCodeJustify := SaveJustify;
  Resize;
end;  { SetLeft }

function TRaveBaseBarCode.GetLeft: TRaveUnits;
begin { GetLeft }
  Result := FLeft;
end;  { GetLeft }

procedure TRaveBaseBarCode.SetRight(Value: TRaveUnits);
begin { SetRight }
  FBarCode.Right := Value;
  FLeft := Value - FWidth;
  Resize;
end;  { SetRight }

function TRaveBaseBarCode.GetRight: TRaveUnits;
begin { GetRight }
  Result := FLeft + FWidth;
end;  { GetRight }

procedure TRaveBaseBarCode.SetCenter(Value: TRaveUnits);
begin { SetCenter }
  FBarCode.Center := Value;
  FLeft := FBarCode.Center - FWidth / 2;
  Resize;
end;  { SetCenter }

function TRaveBaseBarCode.GetCenter: TRaveUnits;
begin { GetCenter }
  Result := FLeft + FWidth / 2;
end;  { GetCenter }

procedure TRaveBaseBarCode.SetTop(Value: TRaveUnits);
begin { SetTop }
  FBarCode.Top := Value;
  FTop := Value;
  Resize;
end;  { SetTop }

function TRaveBaseBarCode.GetTop: TRaveUnits;
begin { GetTop }
  if Assigned(FBarCode) then begin
    Result := FBarCode.Top;
  end else begin
    Result := 0.0;
  end; { else }
end;  { GetTop }

(*
procedure TRaveBaseBarCode.SetBottom(Value: TRaveUnits);
begin { SetBottom }
  FBarCode.Bottom := Value;
  inherited SetBottom(Value);
end;  { SetBottom }

function TRaveBaseBarCode.GetBottom: TRaveUnits;
begin { GetBottom }
  Result := FBarCode.Bottom;
end;  { GetBottom }
*)

procedure TRaveBaseBarCode.SetHeight(Value: TRaveUnits);
begin { SetHeight }
  FBarCode.BarHeight := Value - FBarCode.ReadableHeight;
  inherited SetHeight(FBarCode.Height);
end;  { SetHeight }

procedure TRaveBaseBarCode.SetWidth(Value: TRaveUnits);
begin { SetWidth }
  if FAutoSize then begin
    inherited SetWidth(FBarCode.Width);
  end else begin { else }
    inherited SetWidth(Value);
  end; { else }
end;  { SetWidth }

procedure TRaveBaseBarCode.SetFont(Value: TRaveFont);
begin { SetFont }
  FFont.Assign(Value);
end;  { SetFont }

procedure TRaveBaseBarCode.SetBarCodeJustify(Value: TPrintJustify);
var
  TmpLeft: TRaveFloat;
begin { SetBarCodeJustify }
  TmpLeft := Left;
  FBarCode.BarCodeJustify := Value;

  case Value of
    pjLeft: FBarCode.Left := TmpLeft;
    pjCenter: FBarCode.Center := TmpLeft + (Width / 2);
    pjRight: FBarCode.Right := TmpLeft + Width;
  end;
  Resize;
end;  { SetBarCodeJustify }

function TRaveBaseBarCode.GetBarCodeJustify: TPrintJustify;
begin { GetBarCodeJustify }
  Result := FBarCode.BarCodeJustify;
end;  { GetBarCodeJustify }

procedure TRaveBaseBarCode.SetTextJustify(Value: TPrintJustify);
begin { SetTextJustify }
  FBarCode.TextJustify := Value;
end;  { SetTextJustify }

function TRaveBaseBarCode.GetTextJustify: TPrintJustify;
begin { GetTextJustify }
  Result := FBarCode.TextJustify;
end;  { GetTextJustify }

procedure TRaveBaseBarCode.SetBarWidth(Value: TRaveUnits);
begin { SetBarWidth }
  FBarCode.BarWidth := Value;
  Resize;
end;  { SetBarWidth }

function TRaveBaseBarCode.GetBarWidth: TRaveUnits;
begin { GetBarWidth }
  Result := FBarCode.BarWidth;
end;  { GetBarWidth }

procedure TRaveBaseBarCode.SetBarHeight(Value: TRaveUnits);
begin { SetBarHeight }
  FBarCode.BarHeight := Value;
  Resize;
end;  { SetBarHeight }

function TRaveBaseBarCode.GetBarHeight: TRaveUnits;
begin { GetBarHeight }
  Result := FBarCode.BarHeight;
end;  { GetBarHeight }

procedure TRaveBaseBarCode.SetBarTop(Value: TRaveUnits);
begin { SetBarTop }
  FBarCode.BarTop := Value;
  Top := FBarCode.Top;
end;  { SetBarTop }

function TRaveBaseBarCode.GetBarTop: TRaveUnits;
begin { GetBarTop }
  Result := FBarCode.BarTop;
end;  { GetBarTop }

procedure TRaveBaseBarCode.SetText(Value: string);
begin { SetText }
  FBarCode.Text := Value;
  inherited SetText(FBarCode.Text);
  Invalidate;
  Resize;
end;  { SetText }

function TRaveBaseBarCode.GetText: string;
begin { GetText }
  Result := FBarCode.Text;
end;  { GetText }

procedure TRaveBaseBarCode.SetUseChecksum(Value: boolean);
begin { SetUseChecksum }
  FBarCode.UseChecksum := Value;
  Resize;
end;  { SetUseChecksum }

function TRaveBaseBarCode.GetUseChecksum: boolean;
begin { GetUseChecksum }
  Result := FBarCode.UseChecksum;
end;  { GetUseChecksum }

procedure TRaveBaseBarCode.SetWideFactor(Value: TRaveFloat);
begin { SetWideFactor }
  FBarCode.WideFactor := Value;
  Resize;
end;  { SetWideFactor }

function TRaveBaseBarCode.GetWideFactor: TRaveFloat;
begin { GetWideFactor }
  Result := FBarCode.WideFactor;
end;  { GetWideFactor }

procedure TRaveBaseBarCode.SetPrintReadable(Value: boolean);
begin { SetPrintReadable }
  FBarCode.PrintReadable := Value;
end;  { SetPrintReadable }

function TRaveBaseBarCode.GetPrintReadable: boolean;
begin { GetPrintReadable }
  Result := FBarCode.PrintReadable;
end;  { GetPrintReadable }

procedure TRaveBaseBarCode.SetPrintChecksum(Value: boolean);
begin { SetPrintChecksum }
  FBarCode.PrintChecksum := Value;
end;  { SetPrintChecksum }

function TRaveBaseBarCode.GetPrintChecksum: boolean;
begin { GetPrintChecksum }
  Result := FBarCode.PrintChecksum;
end;  { GetPrintChecksum }

procedure TRaveBaseBarCode.SetPrintTop(Value: boolean);
begin { SetPrintTop }
  FBarCode.PrintTop := Value;
  Invalidate;
end;  { SetPrintTop }

function TRaveBaseBarCode.GetPrintTop: boolean;
begin { GetPrintTop }
  Result := FBarCode.PrintTop;
end;  { GetPrintTop }

procedure TRaveBaseBarCode.SetBarCodeRotation(Value: TBarCodeRotation);
begin { SetBarCodeRotation }
  FBarCode.BarCodeRotation := Value;
end;  { SetBarCodeRotation }

function TRaveBaseBarCode.GetBarCodeRotation: TBarCodeRotation;
begin { GetBarCodeRotation }
  Result := FBarCode.BarCodeRotation;
end;  { GetBarCodeRotation }

procedure TRaveBaseBarCode.PrintBar(X1,Y1,X2,Y2: double);
begin { PrintBar }
  FCanvas.Rectangle(XI2D(PageLeft - Left + X1),
                    YI2D(PageTop - Top + Y1),
                    XI2D(PageLeft - Left + X2),
                    YI2D(PageTop - Top + Y2));
end;  { PrintBar }

procedure TRaveBaseBarCode.Paint(Canvas: TCanvas);
var
  X1, CenterPos, RightPos, Y1: TRaveFloat;
  SavedJustify: TPrintJustify;
begin { Paint }
  FCanvas := Canvas;
  with FCanvas do begin
    Pen.Color := clBlack;
    Pen.Style := psDash;
    Pen.Mode := pmCopy;
    Pen.Width := 1;
    Brush.Color := clBlack;
    Brush.Style := bsClear;
    case BarCodeRotation of
      rot0, rot180: begin
        if PrintReadable then begin
          Rectangle(XI2D(PageLeft),
                    YI2D(PageTop),
                    XI2D(PageRight) + 1,
                    YI2D(PageBottom + Font.Size / 60.0) + 1);
        end else begin
          Rectangle(XI2D(PageLeft),
                    YI2D(PageTop),
                    XI2D(PageRight) + 1,
                    YI2D(PageBottom) + 1);
        end; { else }
      end;
      rot90: begin
        if PrintReadable then begin
          Rectangle(XI2D(PageLeft),
                    YI2D(PageTop + (PageBottom - PageTop)) + 1,
                    XI2D(PageLeft + (PageBottom - PageTop) + Font.Size / 60.0),
                    YI2D(PageBottom - (PageRight - PageLeft)) - 1);
        end else begin
          Rectangle(XI2D(PageLeft),
                    YI2D(PageTop + (PageBottom - PageTop)) + 1,
                    XI2D(PageLeft + (PageBottom - PageTop)),
                    YI2D(PageBottom - (PageRight - PageLeft)) - 1);
        end; { else }
      end;
      rot270: begin
        if PrintReadable then begin
          Rectangle(XI2D(PageLeft),
                    YI2D(PageTop) - 1,
                    XI2D(PageLeft + Height + Font.Size / 60.0),
                    YI2D(PageTop + (PageRight - PageLeft)) + 1);
        end else begin
          Rectangle(XI2D(PageLeft),
                    YI2D(PageTop) - 1,
                    XI2D(PageLeft + Height),
                    YI2D(PageTop + (PageRight - PageLeft)) + 1);
        end; { else }
      end;
    end; { case }
    FBarCode.OverrideBar := PrintBar;
    Pen.Color := clBlack;
    Pen.Style := psSolid;
    Pen.Mode := pmCopy;
    Pen.Width := 1;
    Brush.Color := clBlack;
    Brush.Style := bsSolid;
    X1 := FBarCode.Left;
    CenterPos := FBarCode.Center;
    RightPos := FBarCode.Right;
    Y1 := FBarCode.Top;
    SavedJustify := FBarCode.BarCodeJustify;
    case BarCodeRotation of
      rot0: begin
        if PrintTop then begin
          FBarCode.Top := Y1 + Font.Size / 60.0;
        end; { if }
      end;
      rot90: begin
        case BarCodeJustify of
          pjLeft,pjBlock: begin
            FBarCode.Left := Y1 + FBarCode.Height;
          end;
          pjCenter: begin
            FBarCode.Center := Bottom - Width / 2;
          end;
          pjRight: begin
            FBarCode.Right := Bottom - Width;
          end;
        end; { case }
        if PrintTop then begin
          FBarCode.Top := Left + Font.Size / 60.0;
        end else begin
          FBarCode.Top := Left;
        end; { else }
      end;
      rot180: begin
        case BarCodeJustify of
          pjLeft,pjBlock: begin
            FBarCode.Left := Left + Width;
          end;
          pjCenter: begin
            FBarCode.Center := Left + Width / 2;
          end;
          pjRight: begin
            FBarCode.Right := Left;
          end;
        end;
        if PrintTop then begin
          FBarCode.Top := Y1 + Height;
        end else begin
          FBarCode.Top := Y1 + Height  + Font.Size / 60.0;
        end; { else }
      end;
      rot270: begin
        case BarCodeJustify of
          pjLeft,pjBlock: begin
            FBarCode.Left := Y1;
          end;
          pjCenter: begin
            FBarCode.Center := Top + ((Right - Left) / 2);
          end;
          pjRight: begin
            FBarCode.Right := Top + (Right - Left);
          end;
        end; { case }
        if PrintTop then begin
          FBarCode.Top := Left + Height;
        end else begin
          FBarCode.Top := Left + Height + Font.Size / 60.0;
        end; { else }
      end;
    end; { case }
    FBarCode.Print;
    case SavedJustify of
      pjLeft,pjBlock: begin
        FBarCode.Left := X1;
      end;
      pjCenter: begin
        FBarCode.Center := CenterPos;
      end;
      pjRight: begin
        FBarCode.Right := RightPos;
      end;
    end;
    FBarCode.Top := Y1;
    FBarCode.OverrideBar := nil;
  end; { with }
end;  { Paint }

procedure TRaveBaseBarCode.Print(Report: TBaseReport);
var
  SaveLeft: TRaveFloat;
  SaveTop: TRaveFloat;
  SaveJustify: TPrintJustify;
  T1, L1: TRaveFloat;
begin { Print }
  if not AllowPrint then Exit;

  if DataField <> '' then begin
    FBarCode.Text := ProcessDataStr(self,DataView,DataField);
  end; { if }
  FBarCode.BaseReport := Report;
  FBarCode.BaseReport.AssignFont(FFont);
  FBarCode.OverrideBar := nil;
  SaveTop := FBarCode.Top;
  SaveLeft := FBarCode.Left;
  SaveJustify := FBarCode.BarCodeJustify;

  case BarCodeRotation of
    rot0: begin
      T1 := PageTop + (FBarCode.Top - FTop);
      L1 := PageLeft + (FBarCode.Left - FLeft);
      FBarCode.Left := L1;
      FBarCode.Top := T1;
    end;
    rot90: begin
      T1 := PageTop;
      case BarCodeJustify of
        pjLeft,pjBlock: begin
          FBarCode.Left := T1 + Height;
        end;
        pjCenter: begin
          FBarCode.Center := T1 + Height - (Width /2);
        end;
        pjRight: begin
          FBarCode.Right := T1 + Height - Width;
        end;
      end; { case }
      L1 := PageLeft + (FBarCode.Top - FTop);
      FBarCode.Top := L1;
    end;
    rot180: begin
      case BarCodeJustify of
        pjLeft,pjBlock: begin
          FBarCode.Left := PageLeft + Width;
        end;
        pjCenter: begin
          FBarCode.Center := PageLeft + Width / 2;
        end;
        pjRight: begin
          FBarCode.Right := PageLeft;
        end;
      end; { case }
      FBarCode.Top := PageBottom + Font.Size / 60.0;
    end;
    rot270: begin
      case BarCodeJustify of
        pjLeft,pjBlock: begin
          FBarCode.Left := PageTop;
        end;
        pjCenter: begin
          FBarCode.Center := PageTop + Width /2;
        end;
        pjRight: begin
          FBarCode.Right := PageTop + Width;
        end;
      end; { case }
      if PrintTop then begin
        FBarCode.Top := PageLeft + FBarCode.BarHeight + Font.Size / 60.0;
      end else begin
        FBarCode.Top := PageLeft + FBarCode.BarHeight + Font.Size / 60.0;
      end; { else }
    end;
  end; { case }

  FBarCode.Print;

  FBarCode.Left := SaveLeft;
  FBarCode.Top := SaveTop;
  FBarCode.BarCodeJustify := SaveJustify;
  FBarCode.BaseReport := nil;
end;  { Print }

procedure TRaveBaseBarCode.SetDataField(Value: TRaveFieldName);
begin { SetDataField }
  FDataField := Value;
  Invalidate;
end;  { SetDataField }

procedure TRaveBaseBarCode.BeforeReport;
begin { BeforeReport }
  FOldText := Text;
  inherited BeforeReport;
end;  { BeforeReport }

procedure TRaveBaseBarCode.AfterReport;
begin { AfterReport }
  Text := FOldText;
  inherited AfterReport;
end;  { AfterReport }

procedure TRaveBaseBarCode.Changing(OldItem: TRaveComponent;
                                    NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(DataView) and (DataView = OldItem) then begin
    DataView := NewItem as TRaveBaseDataView;
  end; { if }
end;  { Changing }

function TRaveBaseBarCode.IsLeftStored: boolean;
begin { IsLeftStored }
  Result := BarCodeJustify = pjLeft;
end;  { IsLeftStored }

function TRaveBaseBarCode.IsCenterStored: boolean;
begin { IsCenterStored }
  Result := BarCodeJustify = pjCenter;
end;  { IsCenterStored }

function TRaveBaseBarCode.IsRightStored: boolean;
begin { IsRightStored }
  Result := BarCodeJustify = pjRight;
end;  { IsRightStored }

{ class TRavePostNetBarCode }

constructor TRavePostNetBarCode.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  PrintReadable := false;
  AutoSize := true;
  Height := 0.125;
  Width := 0.5;
end;  { Create }

procedure TRavePostNetBarCode.SetHeight(Value: TRaveUnits);
begin { SetHeight }
  inherited SetHeight(0.125);
end;  { SetHeight }

function TRavePostNetBarCode.CreateBarCodeClass;
begin { CreateBarCodeClass }
  Result := TRPBarsPostNet.Create(nil);
end;  { CreateBarCodeClass }

{ class TRaveI2of5BarCode }

constructor TRaveI2of5BarCode.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  AutoSize := true;
  Height := 0.25;
  Width := 0.5;
end;  { Create }

function TRaveI2of5BarCode.CreateBarCodeClass;
begin { CreateBarCodeClass }
  Result := TRPBars2of5.Create(nil);
end;  { CreateBarCodeClass }

{ class TRaveCode39BarCode }

constructor TRaveCode39BarCode.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  AutoSize := true;
  Height := 0.25;
  Width := 0.5;
end;  { Create }

function TRaveCode39BarCode.CreateBarCodeClass;
begin { CreateBarCodeClass }
  Result := TRPBarsCode39.Create(nil);
end;  { CreateBarCodeClass }

procedure TRaveCode39BarCode.SetExtended(Value: boolean);
begin { SetExtended }
  TRPBarsCode39(FBarcode).Extended := Value;
end;  { SetExtended }

function TRaveCode39BarCode.GetExtended: boolean;
begin { GetExtended }
  Result := TRPBarsCode39(FBarcode).Extended;
end;  { GetExtended }

{ class TRaveCode128BarCode }

constructor TRaveCode128BarCode.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  AutoSize := true;
  Height := 0.25;
  Width := 0.5;
end;  { Create }

function TRaveCode128BarCode.CreateBarCodeClass;
begin { CreateBarCodeClass }
  Result := TRPBarsCode128.Create(nil);
end;  { CreateBarCodeClass }

procedure TRaveCode128BarCode.SetCodePage(Value: TCodePage128);
begin { SetCodePage }
  TRPBarsCode128(FBarcode).CodePage := Value;
end;  { SetCodePage }

function TRaveCode128BarCode.GetCodePage: TCodePage128;
begin { GetCodePage }
  Result := TRPBarsCode128(FBarcode).CodePage;
end;  { GetCodePage }

procedure TRaveCode128BarCode.SetOptimize(Value: boolean);
begin { SetOptimize }
  TRPBarsCode128(FBarcode).Optimize := Value;
end;  { SetOptimize }

function TRaveCode128BarCode.GetOptimize: boolean;
begin { GetOptimize }
  Result := TRPBarsCode128(FBarcode).Optimize;
end;  { GetOptimize }

{ class TRaveUPCBarCode }

constructor TRaveUPCBarCode.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  AutoSize := true;
  Height := 0.5;
  Width := 0.5;
  FFont.Size := 11;
end;  { Create }

function TRaveUPCBarCode.CreateBarCodeClass;
begin { CreateBarCodeClass }
  Result := TRPBarsUPC.Create(nil);
end;  { CreateBarCodeClass }

{ class TRaveEANBarCode }

constructor TRaveEANBarCode.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  AutoSize := true;
  Height := 0.5;
  Width := 0.5;
  FFont.Size := 11;
end;  { Create }

function TRaveEANBarCode.CreateBarCodeClass;
begin { CreateBarCodeClass }
  Result := TRPBarsEAN.Create(nil);
end;  { CreateBarCodeClass }

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.
