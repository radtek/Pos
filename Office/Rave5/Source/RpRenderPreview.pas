{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpRenderPreview;

interface

uses
  Windows,
  Graphics, Controls, Forms, Dialogs, ExtCtrls,
  Classes, SysUtils, RpDefine, RpRender, RpRenderCanvas;

type
  TRvRenderPreview = class(TRvRenderCanvas)
  protected
    FSaveHorzPos: integer;
    FSaveVertPos: integer;
    FSaveCursor: TCursor;
    FScrollBox: TScrollBox; { ScrollBox for print preview output }
    FImage: TImage; { Image that contains FBitmap }
    FBitmap: TBitmap; { Bitmap that page is drawn in }
    FZoomInc: integer; { Value that the Zoom inc/dec ZoomFactor }
    FMonochrome: boolean; { Do bitmap in monochrome? }
    FMarginMethod: TMarginMethod; { Keep margin scaled or fixed }
    FMarginPercent: double; { Percent of margin around page }
    FScrollBarWidth: integer; { Width of ScrollBox vertical scroll bar }
    FScrollBarHeight: integer; { Height of ScrollBox horizontal scroll bar }
    FHorzAdj: integer; { Diffence between Width and ClientWidth }
    FVertAdj: integer; { Diffence between Height and ClientHeight }
    FGridPen: TPen;  { Pen to draw the grid with }
    FGridHoriz: double;  { Spacing between horizontal grid lines }
    FGridVert: double;   { Spacing between vertical grid line }
    FShadowDepth: integer;  { Depth for the page shadow }
    FRulerType: TRulerType;  { Characteristics of the ruler to draw }
    RulerWidth: integer;  { Width of the horizontal ruler }
    RulerHeight: integer;  { Width of the vertical ruler }
    FPageInc: integer; { How far does NextPage/PrevPage inc/dec }
    FOnPageChange: TNotifyEvent; { Called when CurrentPage changes }
    FOnZoomChange: TNotifyEvent; { Called when ZoomFactor changes }
    FCurrentPage: integer;
    FPages: integer;

    FZoom: double; { Internal zoom factor -  100% norm, 10% small, 200% large } //!!! Move to base render
    AddX: longint; { Amount to add to all measurements for preview } //!!! Move to base render
    AddY: longint; { Amount to add to all measurements for preview } //!!! Move to base render

    function GetCanvas: TCanvas; override;
    function GetZoomFactor: double;
    procedure SetGridPen(const Value: TPen);
    procedure SetMonochrome(const Value: boolean);
    procedure SetScrollBox(const Value: TScrollBox);
    procedure SetShadowDepth(const Value: integer);
    procedure SetZoomFactor(const Value: double);
    procedure SetZoomInc(const Value: integer);
    procedure Notification(AComponent: TComponent; Operation: TOperation); override;
    procedure PageChangeEvent; virtual;
    procedure ZoomChangeEvent; virtual;
    function GetZoomPageFactor: double;
    function GetZoomPageWidthFactor: double;
    procedure DocBegin; override;
    procedure DocEnd; override;
    procedure PageBegin; override;
    procedure PageEnd; override;
    function GetXDPI: integer; override;
    function GetYDPI: integer; override;
    function GetPages: integer;
    function CanvasTextWidth(const AText: string): double; override;
    procedure PrintBitmapRect(const X1, Y1, X2, Y2: double; Graphic: TBitmap); override;
    function Destination: TDestination; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure ZoomIn;
    procedure ZoomOut;
    procedure Clear;
    procedure RedrawPage;
    procedure PrevPage;
    procedure NextPage;
    procedure RenderPage(PageNum: integer); override;
    function XI2D(Pos: double): integer; override;
    function YI2D(Pos: double): integer; override;
    //
    property ZoomPageWidthFactor: double read GetZoomPageWidthFactor;
    property ZoomPageFactor: double read GetZoomPageFactor;
    property Image: TImage read FImage;
    property CurrentPage: integer read FCurrentPage write FCurrentPage;
    property Pages: integer read GetPages write FPages;
  published
    property Active default false;
    property ScrollBox: TScrollBox read FScrollBox write SetScrollBox;
    property ZoomInc: integer read FZoomInc write SetZoomInc default 10;
    property ZoomFactor: double read GetZoomFactor write SetZoomFactor;
    property Monochrome: boolean read FMonochrome write SetMonochrome default false;
    property MarginMethod: TMarginMethod read FMarginMethod write FMarginMethod default mmFixed;
    property MarginPercent: double read FMarginPercent write FMarginPercent;
    property RulerType: TRulerType read FRulerType write FRulerType default rtNone;
    property GridPen: TPen read FGridPen write SetGridPen;
    property GridHoriz: double read FGridHoriz write FGridHoriz;
    property GridVert: double read FGridVert write FGridVert;
    property ShadowDepth: integer read FShadowDepth write SetShadowDepth;
    property PageInc: integer read FPageInc write FPageInc default 1;

    property OnZoomChange: TNotifyEvent read FOnZoomChange write FOnZoomChange;
    property OnPageChange: TNotifyEvent read FOnPageChange write FOnPageChange;
  end;

implementation

{ TRvRenderPreview }

procedure TRvRenderPreview.Clear;
begin
  FScrollBox.Owner.RemoveComponent(FImage);
  FreeAndNil(FImage);
end;

constructor TRvRenderPreview.Create(AOwner: TComponent);
begin
  inherited;
  FScrollBox := nil;
  FZoomInc := 10;
  FZoom := 100.0;
  FOnPageChange := nil;
  FOnZoomChange := nil;
  FMonochrome := false;
  FMarginPercent := 0.0;
  FMarginMethod := mmFixed;
  FShadowDepth := 0;
  FRulerType := rtNone;
  FGridPen := TPen.Create;
  FGridHoriz := 0;
  FGridVert := 0;
  FPageInc := 1;
  RulerWidth := 0;
  RulerHeight := 0;
  FBitmap := TBitmap.Create;
  FActive := false;
  FBitmap.Canvas.Handle; { Load valid font, pen and brush }
end; { Create }

destructor TRvRenderPreview.Destroy;
begin
  FreeAndNil(FGridPen);
  inherited;
end;

procedure TRvRenderPreview.DocBegin;
begin
  inherited; 
end;

procedure TRvRenderPreview.DocEnd;
begin
  inherited;
end;

function TRvRenderPreview.GetCanvas: TCanvas;
begin
  Result := FBitmap.Canvas;
end;

function TRvRenderPreview.GetZoomFactor: double;
begin
  Result := FZoom;
end;

function TRvRenderPreview.GetZoomPageFactor: double;
var
  Z1,Z2: double;
  Adjustment: integer;
  ScrollWidth: integer;
  ScrollHeight: integer;
begin
//!!!CheckPrinting;

{ Calculate new scrollwidth taking into account the shadow depth }
  ScrollWidth := FScrollBox.Width - ShadowDepth - RulerWidth;
  ScrollHeight := FScrollBox.Height - ShadowDepth - RulerHeight;

  if FMarginMethod = mmScaled then begin
    Z1 := (ScrollWidth - 4) / (PaperWidth * XDPI * (1.0 + FMarginPercent / 50.0));
    Z2 := (ScrollHeight - 4) / (PaperHeight * YDPI + (PaperWidth * XDPI * FMarginPercent / 50.0));
  end else begin { mmFixed }
    Adjustment := Round(PaperWidth * XDPI * (FMarginPercent / 100.0));
    Z1 := (ScrollWidth - 4 - Adjustment * 2.0) / (PaperWidth * XDPI);
    Z2 := (ScrollHeight - 4 - Adjustment * 2.0) / (PaperHeight * YDPI);
  end; { else }

  if Z1 <= Z2 then begin
    Result := Z1 * 100.0;
  end else begin
    Result := Z2 * 100.0;
  end; { else }
end;

function TRvRenderPreview.GetZoomPageWidthFactor: double;
var
  F1: double;
  NewZoom: double;
  Adjustment: integer;
  ScrollWidth: integer;
  ScrollHeight: integer;
begin
//!!!  CheckPrinting;

{ Calculate new scrollwidth taking into account the shadow depth }
  ScrollWidth := FScrollBox.Width - ShadowDepth - RulerWidth;
  ScrollHeight := FScrollBox.Height - ShadowDepth - RulerHeight;

{ Calculate new zoom factor for no vertical scroll bar }
  if FMarginMethod = mmScaled then begin
    NewZoom := (ScrollWidth - FHorzAdj) / (PaperWidth * XDPI * (1.0 + FMarginPercent / 50.0));
  end else begin { mmFixed }
    Adjustment := Round(PaperWidth * XDPI * (FMarginPercent / 100.0));
    NewZoom := (ScrollWidth - (FHorzAdj + Adjustment * 2.0)) / (PaperWidth * XDPI);
  end; { else }

{ Calculate adjustment for above zoom factor }
  F1 := PaperWidth * XDPI * (FMarginPercent / 100.0);
  if FMarginMethod = mmScaled then begin
    F1 := F1 * NewZoom;
  end; { if }
  Adjustment := Round(F1);

{ Determine if vertical scroll bar will still appear }
  if (PaperHeight * YDPI * NewZoom + 2 * Adjustment) >
   (ScrollHeight - FVertAdj) then begin
  { Subtract scroll bar width from available width and recalc zoom factor }
    if FMarginMethod = mmScaled then begin
      NewZoom := (ScrollWidth - (FHorzAdj + FScrollBarWidth)) /
       (PaperWidth * XDPI * (1.0 + FMarginPercent / 50.0));
    end else begin { mmFixed }
      NewZoom := (ScrollWidth - (FHorzAdj + Adjustment * 2.0 +
       FScrollBarWidth)) / (PaperWidth * XDPI);
    end; { else }
  end; { if }

  Result := NewZoom * 100.0;
end;

procedure TRvRenderPreview.Notification(AComponent: TComponent; Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) and (AComponent = FScrollBox) then begin
    FScrollBox := nil;
  end; { if }
end;

procedure TRvRenderPreview.PageBegin;
var
  RulerColor: TColor;
  TickLength: integer;
  TickHeight: double;
  TickWidth: double;
  Ticks: integer;
  TickNum: integer;
  TickStr: string[3];
  TickMarker: double;
  TickTarget: integer;
  SavePen: TPen;
  GridTarget: integer;
  GridMarker: double;
  Adjustment: integer;
  F1: double;
  Margin: double;
begin
  inherited;
  if not Assigned(FScrollBox) then begin
    RaiseError(Trans('ScrollBox must be assigned for RenderPreview component'));
  end; { if }

  FPages := Converter.ReportHeader.PageCount;
//FLocalDevice := TRPRenderDevice.Create;
//ReportDateTime := Now;
//!!PORT!!  ZoomChangeEvent;

  with FScrollBox do begin
  { Calculate scroll bar height }
    HorzScrollBar.Range := ClientWidth * 2;
    FScrollBarHeight := ClientHeight;
    HorzScrollBar.Range := ClientWidth;
    FScrollBarHeight := ClientHeight - FScrollBarHeight;

  { Calculate scroll bar width }
    VertScrollBar.Range := ClientHeight * 2;
    FScrollBarWidth := ClientWidth;
    VertScrollBar.Range := ClientHeight;
    FScrollBarWidth := ClientWidth - FScrollBarWidth;

  { Calculate difference between dimension and client dimension }
    FHorzAdj := Width - ClientWidth;
    FVertAdj := Height - ClientHeight;
  end; { with }

// Look for Image object in FScrollBox
  if not Assigned(FImage) or (FImage.Parent <> FScrollBox) then begin
    FImage := TImage.Create(FScrollBox.Owner);
    with FImage do begin
      Parent := FScrollBox;
      Align := alNone;
      AutoSize := true;
    end; { with }
    if Assigned(FBitmap) then begin
      FreeAndNil(FBitmap);
    end; { if }
    FBitmap := FImage.Picture.Bitmap;
    FBitmap.Monochrome := Monochrome;
  end; { if }

  AddX := 0;
  AddY := 0;
  FImage.Canvas.MoveTo(0,0);

  if Monochrome then begin
    ShadowDepth := 0;
  end; { if }

  if RulerType in [rtNone,rtHorizCm,rtHorizIn] then begin
    RulerWidth := 0;
  end else begin
    RulerWidth := 20;
  end; { else }

  if RulerType in [rtNone,rtVertCm,rtVertIn] then begin
    RulerHeight := 0;
  end else begin
    RulerHeight := 20;
  end; { else }

  AddX := RulerWidth;
  AddY := RulerHeight;

  RulerColor := RGB(255,255,128);
  FSaveCursor := Screen.Cursor;
  Screen.Cursor := crHourGlass;

{ Call the OnPageChange event }
  PageChangeEvent;

  with FScrollBox do begin
    FSaveHorzPos := HorzScrollBar.Position;
    HorzScrollBar.Position := 0;
    FSaveVertPos := VertScrollBar.Position;
    VertScrollBar.Position := 0;
  end; { with }

{ Initialize bitmap and calculate adjustment values for pseudo page }
  FBitmap.Width := Round(PaperWidth * XDPI * FZoom / 100.0) + ShadowDepth + AddX;
  FBitmap.Height := Round(PaperHeight * YDPI * FZoom / 100.0) + ShadowDepth + AddY;

{ Draw psuedo-paper }

{ Draw the shadow first }

{ Lower left corner }
  Canvas.Pen.Color := FScrollBox.Color;
  Canvas.Pen.Style := psSolid;
  Canvas.Pen.Width := 1;
  Canvas.Pen.Mode := pmCopy;
  Canvas.Brush.Color := FScrollBox.Color;
  Canvas.Brush.Style := bsSolid;
  Canvas.Rectangle(0, FBitmap.Height - ShadowDepth,ShadowDepth + AddX,FBitmap.Height);

{ Right Shadow }
  Canvas.Pen.Color := clBlack;
  Canvas.Pen.Style := psSolid;
  Canvas.Pen.Width := 1;
  Canvas.Pen.Mode := pmCopy;
  Canvas.Brush.Color := clBlack;
  Canvas.Brush.Style := bsSolid;
  Canvas.Rectangle(FBitmap.Width - ShadowDepth, ShadowDepth + AddY,
   FBitMap.Width,FBitmap.Height);

{ Upper Right Corner }
  Canvas.Pen.Color := FScrollBox.Color;
  Canvas.Pen.Style := psSolid;
  Canvas.Pen.Width := 1;
  Canvas.Pen.Mode := pmCopy;
  Canvas.Brush.Color := FScrollBox.Color;
  Canvas.Brush.Style := bsSolid;
  Canvas.Rectangle(FBitmap.Width - ShadowDepth,0, FBitMap.Width,
   ShadowDepth + AddY);

{ Lower Shadow }
  Canvas.Pen.Color := clBlack;
  Canvas.Pen.Style := psSolid;
  Canvas.Pen.Width := 1;
  Canvas.Pen.Mode := pmCopy;
  Canvas.Brush.Color := clBlack;
  Canvas.Brush.Style := bsSolid;
  Canvas.Rectangle(ShadowDepth + AddX,FBitmap.Height - ShadowDepth,
   FBitmap.Width,FBitmap.Height);

{ Draw the ruler }
{ Upper left corner shadow }
  if (RulerWidth > 0) and (RulerHeight > 0) then begin
    Canvas.Pen.Color := clLtGray;
    Canvas.Pen.Style := psSolid;
    Canvas.Pen.Width := 1;
    Canvas.Pen.Mode := pmCopy;
    Canvas.Brush.Color := clLtGray;
    Canvas.Brush.Style := bsSolid;
    Canvas.Rectangle(0,0,AddX,AddY);
  end; { if }

{ Upper right ruler shadow }

  if RulerType in [rtHorizIn,rtHorizCm,rtBothIn,rtBothCm] then begin
    Canvas.Pen.Color := clBlack;
    Canvas.Pen.Style := psSolid;
    Canvas.Pen.Width := 1;
    Canvas.Pen.Mode := pmCopy;
    Canvas.Brush.Color := clBlack;
    Canvas.Brush.Style := bsSolid;
    Canvas.Rectangle(FBitmap.Width - ShadowDepth,ShadowDepth,
     FBitmap.Width,AddY + ShadowDepth);
   end; { if }

{ Lower left ruler shadow }
  if RulerType in [rtVertIn, rtVertCm,rtBothIn,rtBothCm] then begin
    Canvas.Pen.Color := clBlack;
    Canvas.Pen.Style := psSolid;
    Canvas.Pen.Width := 1;
    Canvas.Pen.Mode := pmCopy;
    Canvas.Brush.Color := clBlack;
    Canvas.Brush.Style := bsSolid;
    Canvas.Rectangle(ShadowDepth,FBitmap.Height - ShadowDepth,
     AddX + ShadowDepth,FBitMap.Height);
   end; { if }

{ Left side ruler }
  if RulerWidth > 0 then begin
    Canvas.Pen.Color := clBlack;
    Canvas.Pen.Style := psSolid;
    Canvas.Pen.Width := 1;
    Canvas.Pen.Mode := pmCopy;
    Canvas.Brush.Color := RulerColor;
    Canvas.Brush.Style := bsSolid;
    Canvas.Rectangle(0,AddY,AddX + 1,FBitmap.Height - ShadowDepth);
  end; { if }

{ Top Ruler }
  if RulerHeight > 0 then begin
    Canvas.Pen.Color := clBlack;
    Canvas.Pen.Style := psSolid;
    Canvas.Pen.Width := 1;
    Canvas.Pen.Mode := pmCopy;
    Canvas.Brush.Color := RulerColor;
    Canvas.Brush.Style := bsSolid;
    Canvas.Rectangle(AddX,0,FBitmap.Width - ShadowDepth,AddY + 1);
  end; { if }

{ Draw ticks and numbers }
  if RulerType <> rtNone then begin
    TickWidth := (FBitmap.Width - ShadowDepth - AddX) / PaperWidth;
    TickHeight := (FBitmap.Height - ShadowDepth - AddY) / PaperHeight;
    TickStr := '';

    with FBitmap.Canvas do begin
      with Font do begin
        Name := 'Arial';
        Size := 7;
        Color := clBlack;
        Style := [];
      end; { with }
      SetBKMode(Handle,TRANSPARENT);
    end; { with }

    case RulerType of
      rtHorizIn,rtVertIn,rtBothIn: begin
        TickWidth := TickWidth / 8;
        TickHeight := TickHeight / 8;
        Ticks := 8;
      end;
      else begin { rtHorizCm,rtVertCm,rtBothCm }
        TickWidth := TickWidth / 25.4;
        TickHeight := TickHeight / 25.4;
        Ticks := 10;
      end;
    end; { case }

  { Draw horizontal ruler tick marks }
    if RulerType in [rtHorizIn,rtBothIn,rtHorizCm,rtBothCm] then begin
      TickMarker := AddX + TickWidth;
      TickTarget := FBitmap.Width - (ShadowDepth + 1);
      TickNum := 0;

      while Round(TickMarker) <= TickTarget do begin
        Inc(TickNum);

      { Determine which type of tick mark we are on }
        case RulerType of
          rtHorizIn,rtBothIn: begin
            case TickNum mod Ticks of
              0: begin { 18 pixels }
                TickLength := 18;
                TickStr := IntToStr(TickNum div Ticks);
              end;
              2,6: begin { 10 pixels }
                TickLength := 10;
              end;
              4: begin { 15 pixels }
                TickLength := 15;
              end;
              else begin { 1,3,5,7 - 5 pixels }
                TickLength := 5;
              end;
            end; { case }
          end;
          else begin { rtHorizCm,rtBothCm }
            case TickNum mod Ticks of
              0: begin { 18 pixels }
                TickLength := 18;
                TickStr := IntToStr(TickNum div Ticks);
              end;
              5: begin { 15 pixels }
                TickLength := 15;
              end;
              else begin { 1,2,3,4,6,7,8,9 - 5 pixels }
                TickLength := 5;
              end;
            end; { case }
          end;
        end; { case }

      { Draw the tick line }
        FBitmap.Canvas.MoveTo(Round(TickMarker),AddY);
        FBitmap.Canvas.LineTo(Round(TickMarker),AddY - TickLength + 1);

      { Draw the tick string if any }
        if TickStr <> '' then begin
          with FBitmap.Canvas do begin
            Textout(Round(TickMarker) - (TextWidth(TickStr) + 1),0,TickStr);
            TickStr := '';
          end; { with }
        end; { if }

        TickMarker := TickMarker + TickWidth;
      end; { while }
    end; { if }

  { Draw vertical ruler tick marks }
    if RulerType in [rtVertIn,rtBothIn,rtVertCm,rtBothCm] then begin
      TickMarker := AddY + TickHeight;
      TickTarget := FBitmap.Height - (ShadowDepth + 1);
      TickNum := 0;

      while Round(TickMarker) <= TickTarget do begin
        Inc(TickNum);

      { Determine which type of tick mark we are on }
        case RulerType of
          rtVertIn,rtBothIn: begin
            case TickNum mod Ticks of
              0: begin { 18 pixels }
                TickLength := 18;
                TickStr := IntToStr(TickNum div Ticks);
              end;
              2,6: begin { 10 pixels }
                TickLength := 10;
              end;
              4: begin { 15 pixels }
                TickLength := 15;
              end;
              else begin { 1,3,5,7 - 5 pixels }
                TickLength := 5;
              end;
            end; { case }
          end;
          else begin { rtVertCm,rtBothCm }
            case TickNum mod Ticks of
              0: begin { 18 pixels }
                TickLength := 18;
                TickStr := IntToStr(TickNum div Ticks);
              end;
              5: begin { 15 pixels }
                TickLength := 15;
              end;
              else begin { 1,2,3,4,6,7,8,9 - 5 pixels }
                TickLength := 5;
              end;
            end; { case }
          end;
        end; { case }

      { Draw the tick line }
        FBitmap.Canvas.MoveTo(AddX,Round(TickMarker));
        FBitmap.Canvas.LineTo(AddX - TickLength + 1,Round(TickMarker));

      { Draw the tick string if any }
        if TickStr <> '' then begin
          with FBitmap.Canvas do begin
            Textout(2,Round(TickMarker) - (TextHeight(TickStr) + 1),TickStr);
            TickStr := '';
          end; { with }
        end; { if }

        TickMarker := TickMarker + TickHeight;
      end; { while }
    end; { if }
  end; { if }

{ Draw the page }
  Canvas.Pen.Color := clBlack;
  Canvas.Pen.Style := psSolid;
  Canvas.Pen.Width := 1;
  Canvas.Pen.Mode := pmCopy;
  Canvas.Brush.Color := clWhite;
  Canvas.Brush.Style := bsSolid;
  Canvas.Rectangle(AddX,AddY,FBitmap.Width - ShadowDepth,FBitmap.Height - ShadowDepth);

{ Draw the Grid }
{ Horizontal Lines }
  if GridHoriz > 0 then begin
    SavePen := FBitMap.Canvas.Pen;
    FBitmap.Canvas.Pen := FGridPen;
    GridTarget := FBitmap.Height - ShadowDepth;
    GridMarker := AddY + GridHoriz * YDPI * FZoom / 100.0;
    while Round(GridMarker) < GridTarget do begin
      FBitMap.Canvas.MoveTo(AddX + 1,Round(GridMarker));
      FBitMap.Canvas.LineTo(FBitmap.Width - (ShadowDepth + 1),Round(GridMarker));
      GridMarker := GridMarker + GridHoriz * YDPI * FZoom / 100.0;
    end; { while }
    FBitmap.Canvas.Pen := SavePen;
  end; { if }

{ Vertical Lines }
  if GridVert > 0 then begin
    SavePen := FBitMap.Canvas.Pen;
    FBitmap.Canvas.Pen := FGridPen;
    GridTarget := FBitmap.Width - ShadowDepth;
    GridMarker := AddX + GridVert * XDPI * FZoom / 100.0;
    while Round(GridMarker) < GridTarget do begin
      FBitMap.Canvas.MoveTo(Round(GridMarker) ,AddY + 1);
      FBitMap.Canvas.LineTo(Round(GridMarker),FBitmap.Height -
       (ShadowDepth + 1));
      GridMarker := GridMarker + GridVert * XDPI * FZoom / 100.0;
    end; { while }
    FBitmap.Canvas.Pen := SavePen;
  end; { if }

{ Initialize Image }
  Margin := FMarginPercent / 100.0;
  F1 := PaperWidth * XDPI;
  if FMarginMethod = mmScaled then begin
    Adjustment := Round(F1 * Margin * FZoom / 100.0);
  end else begin
    Adjustment := Round(F1 * Margin);
  end; { else }

  with FImage do begin
    Top := Adjustment;
    Left := Adjustment;
  end; { with }

  with FScrollBox do begin
    with HorzScrollBar do begin
      Position := FSaveHorzPos;
      Range := FBitmap.Width + 2 * Adjustment;
    end; { with }
    with VertScrollBar do begin
      Position := FSaveVertPos;
      Range := FBitmap.Height + 2 * Adjustment;
    end; { with }
  end; { with }
end;

procedure TRvRenderPreview.PageChangeEvent;
begin
  if Assigned(FOnPageChange) then begin
    FOnPageChange(self);
  end; { if }
end;

procedure TRvRenderPreview.PageEnd;
begin
  inherited;
  Screen.Cursor := FSaveCursor;
end;

procedure TRvRenderPreview.SetGridPen(const Value: TPen);
begin
  FGridPen.Assign(Value);
end;

procedure TRvRenderPreview.SetMonochrome(const Value: boolean);
begin
  if Value then begin
    FShadowDepth := 0;
  end; { if }
  FMonochrome := Value;
end;

procedure TRvRenderPreview.SetScrollBox(const Value: TScrollBox);
begin
  FScrollBox := Value;
  if Assigned(Value) then begin
    Value.FreeNotification(self);
  end; { if }
end;

procedure TRvRenderPreview.SetShadowDepth(const Value: integer);
begin
  if Value > 0 then begin
    FMonochrome := false;
  end; { if }
  FShadowDepth := Value;
end;

procedure TRvRenderPreview.SetZoomFactor(const Value: double);
begin
  if Value <= -1.9 then begin { -2 for ZoomPageWidthFactor } {!!!}
    FZoom := ZoomPageWidthFactor;
  end else if Value < 0.0 then begin { -1 for ZoomPageFactor }
    FZoom := ZoomPageFactor;
  end else if Value < MINPREVIEWZOOM then begin
    FZoom := MINPREVIEWZOOM;
  end else if Value > MAXPREVIEWZOOM then begin
    FZoom := MAXPREVIEWZOOM;
  end else begin
    FZoom := Value;
  end; { else }

{ Call the OnZoomChange event }
  ZoomChangeEvent;
  if not Assigned(FOnZoomChange) then begin
    RedrawPage;
  end; { if }
end;

procedure TRvRenderPreview.SetZoomInc(const Value: integer);
begin
  FZoomInc := Value;
end;

procedure TRvRenderPreview.ZoomChangeEvent;
begin
  if Assigned(FOnZoomChange) then begin
    FOnZoomChange(self);
  end; { if }
end;

procedure TRvRenderPreview.ZoomIn;
begin
  ZoomFactor := ZoomFactor + FZoomInc;
end;

procedure TRvRenderPreview.ZoomOut;
begin
  if ZoomFactor > FZoomInc then begin
    ZoomFactor := ZoomFactor - FZoomInc;
  end; { if }
end;

function TRvRenderPreview.GetXDPI: integer;
begin
  Result := Screen.PixelsPerInch;
end;

function TRvRenderPreview.GetYDPI: integer;
begin
  Result := Screen.PixelsPerInch;
end;

function TRvRenderPreview.GetPages: integer;
begin
  if Assigned(Converter) and (Converter.ReportHeader.PageCount > 0) then begin
    Result := Converter.ReportHeader.PageCount;
  end else begin
    Result := FPages;
  end; { else }
end;

procedure TRvRenderPreview.RedrawPage;
begin
  RenderPage(CurrentPage);
end;

procedure TRvRenderPreview.NextPage;
begin
  if CurrentPage + PageInc <= Pages then begin
    RenderPage(CurrentPage + PageInc);
  end else if CurrentPage > 1 then begin
    RenderPage(1);
  end; { else }
end;

procedure TRvRenderPreview.PrevPage;
begin
  if CurrentPage - PageInc > 0 then begin
    RenderPage(CurrentPage - PageInc);
  end else if CurrentPage <> Pages then begin
    RenderPage(Pages);
  end; { else }
end;

procedure TRvRenderPreview.RenderPage(PageNum: integer);
begin
  FCurrentPage := PageNum;
  if CurrentPage < 1 then begin
    CurrentPage := 1;
  end; { if }
  if (CurrentPage > Pages) and (Pages > 0) then begin
    CurrentPage := Pages;
  end; { if }
  inherited RenderPage(CurrentPage);
end;

function TRvRenderPreview.XI2D(Pos: double): integer;
begin
  Result := Round(Pos * XDPI * ZoomFactor / 100.0) + AddX;
end;

function TRvRenderPreview.YI2D(Pos: double): integer;
begin
  Result := Round(Pos * YDPI * ZoomFactor / 100.0) + AddY;
end;

function TRvRenderPreview.CanvasTextWidth(const AText: string): double;
begin
  Result := inherited CanvasTextWidth(AText);
  Result := Result * 100.0 / ZoomFactor;
end;

procedure TRvRenderPreview.PrintBitmapRect(const X1, Y1, X2, Y2: double;
  Graphic: TBitmap);
begin
  Canvas.StretchDraw(Rect(XI2D(X1),YI2D(Y1),XI2D(X2),YI2D(Y2)),Graphic);
end;

function TRvRenderPreview.Destination: TDestination;

begin { Destination }
  Result := destPreview;
end;  { Destination }

end.