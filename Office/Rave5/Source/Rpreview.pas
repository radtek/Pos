{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit Rpreview;

interface

uses
  Windows, Messages, Graphics, Controls, Forms, Dialogs, StdCtrls, ExtCtrls,
  Classes, SysUtils, RpBase, RpCanvas, RpFPrint, RpDefine, RpDevice;

type
{*************************************************************************}
{                           class TFilePreview                           }
{*************************************************************************}

  TFilePreview = class(TFilePrinter)
  protected
    FScrollBox: TScrollBox; { ScrollBox for print preview }
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

    procedure PageChangeEvent; virtual;
    procedure ZoomChangeEvent; virtual;

    function Destination: TDestination; override; { destPreview }

    procedure AbsorbIntProp(Reader: TReader);
    procedure DefineProperties(Filer: TFiler); override;
    function GetZoomFactor: double;
    procedure SetZoomFactor(Value: double);
    procedure SetZoomInc(Value: integer);
    function GetZoomPageWidthFactor: double;
    function GetZoomPageFactor: double;
    procedure SetMonochrome(Value: boolean);
    procedure SetShadowDepth(Value: integer);
    procedure SetGridPen(Value: TPen);
    procedure Notification(AComponent: TComponent;
                           Operation: TOperation); override;
    procedure SetScrollBox(Value: TScrollBox);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

    procedure PrintPage(PageNum: word); override;
    procedure PrevPage;
    procedure NextPage;
    procedure ZoomIn;
    procedure ZoomOut;
    procedure RedrawPage;
    procedure Clear;

    procedure Start; override;
    procedure Finish; override;
    procedure Execute; override;

    property CurrentPage;
    property ZoomPageWidthFactor: double read GetZoomPageWidthFactor;
    property ZoomPageFactor: double read GetZoomPageFactor;
    property Image: TImage read FImage;
  published
    property ScrollBox: TScrollBox read FScrollBox write SetScrollBox;
    property ZoomInc: integer read FZoomInc write SetZoomInc default 10;
    property ZoomFactor: double read GetZoomFactor write SetZoomFactor;
    property Monochrome: boolean read FMonochrome write SetMonochrome default false;
    property MarginMethod: TMarginMethod read FMarginMethod write FMarginMethod
     default mmFixed;
    property MarginPercent: double read FMarginPercent write FMarginPercent;
    property RulerType: TRulerType read FRulerType write FRulerType default rtNone;
    property GridPen: TPen read FGridPen write SetGridPen;
    property GridHoriz: double read FGridHoriz write FGridHoriz;
    property GridVert: double read FGridVert write FGridVert;
    property ShadowDepth: integer read FShadowDepth write SetShadowDepth;
    property PageInc: integer read FPageInc write FPageInc default 1;

    property OnZoomChange: TNotifyEvent read FOnZoomChange write FOnZoomChange;
    property OnPageChange: TNotifyEvent read FOnPageChange write FOnPageChange;
  end; { TFilePreview }

implementation

{*************************************************************************}
{                           class TFilePreview                           }
{*************************************************************************}

constructor TFilePreview.Create(AOwner: TComponent);
begin { Create }
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
end;  { Create }

destructor TFilePreview.Destroy;
begin { Destroy }
  if Printing then begin
    Finish;
  end; { if }
  FreeAndNil(FGridPen);

  inherited;
end;  { Destroy }

procedure TFilePreview.AbsorbIntProp(Reader: TReader);

begin { AbsorbIntProp }
  Reader.ReadInteger;
end;  { AbsorbIntProp }

procedure TFilePreview.DefineProperties(Filer: TFiler);

begin { DefineProperties }
  inherited DefineProperties(Filer);
  Filer.DefineProperty('PagesHigh',AbsorbIntProp,nil,false);
  Filer.DefineProperty('PagesWide',AbsorbIntProp,nil,false);
end;  { DefineProperties }

procedure TFilePreview.PageChangeEvent;

begin { PageChangeEvent }
  if Assigned(FOnPageChange) then begin
    FOnPageChange(self);
  end; { if }
end;  { PageChangeEvent }

procedure TFilePreview.ZoomChangeEvent;

begin { ZoomChangeEvent }
  if Assigned(FOnZoomChange) then begin
    FOnZoomChange(self);
  end; { if }
end;  { ZoomChangeEvent }

function TFilePreview.GetZoomFactor: double;
begin { GetZoomFactor }
  Result := FZoom;
end;  { GetZoomFactor }

procedure TFilePreview.SetZoomFactor(Value: double);
begin { SetZoomFactor }
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
    if Printing then begin
      RedrawPage;
    end; { if }
  end; { if }
end;  { SetZoomFactor }

procedure TFilePreview.SetZoomInc(Value: integer);

begin { SetZoomInc }
  FZoomInc := Value;
end;  { SetZoomInc }

procedure TFilePreview.SetMonochrome(Value: boolean);

begin { SetMonochrome }
  if (ShadowDepth > 0) and Value then begin { Warn programmer }
    ShowMessage('Monochrome not allowed while shadows are in effect.'#13 +
     'Change ShadowDepth to 0 first');
  end else begin
    FMonochrome := Value;
  end; { else }
end;  { SetMonochrome }

procedure TFilePreview.SetShadowDepth(Value: integer);

begin { SetShadowDepth }
  if (Value > 0) and Monochrome then begin { Warn programmer }
    ShowMessage('Shadows not allowed while monochrome in effect.'#13 +
     'Change Monochrome to false first');
  end else begin
    FShadowDepth := Value;
  end; { else }
end;  { SetShadowDepth }

procedure TFilePreview.SetGridPen(Value: TPen);

begin { SetGridPen }
  FGridPen.Assign(Value);
end;  { SetGridPen }

function TFilePreview.GetZoomPageWidthFactor: double;

var
  F1: double;
  NewZoom: double;
  Adjustment: integer;
  ScrollWidth: integer;
  ScrollHeight: integer;

begin { GetZoomPageWidthFactor }
  CheckPrinting;

{ Calculate new scrollwidth taking into account the shadow depth }
  ScrollWidth := FScrollBox.Width - ShadowDepth - RulerWidth;
  ScrollHeight := FScrollBox.Height - ShadowDepth - RulerHeight;

{ Calculate new zoom factor for no vertical scroll bar }
  if FMarginMethod = mmScaled then begin
    NewZoom := (ScrollWidth - FHorzAdj) / (PaperWidth * FXDPI *
     (1.0 + FMarginPercent / 50.0));
  end else begin { mmFixed }
    Adjustment := Round(PaperWidth * FXDPI * (FMarginPercent / 100.0));
    NewZoom := (ScrollWidth - (FHorzAdj + Adjustment * 2.0)) /
     (PaperWidth * FXDPI);
  end; { else }

{ Calculate adjustment for above zoom factor }
  F1 := PaperWidth * FXDPI * (FMarginPercent / 100.0);
  if FMarginMethod = mmScaled then begin
    F1 := F1 * NewZoom;
  end; { if }
  Adjustment := Round(F1);

{ Determine if vertical scroll bar will still appear }
  if (PaperHeight * FYDPI * NewZoom + 2 * Adjustment) >
   (ScrollHeight - FVertAdj) then begin
  { Subtract scroll bar width from available width and recalc zoom factor }
    if FMarginMethod = mmScaled then begin
      NewZoom := (ScrollWidth - (FHorzAdj + FScrollBarWidth)) /
       (PaperWidth * FXDPI * (1.0 + FMarginPercent / 50.0));
    end else begin { mmFixed }
      NewZoom := (ScrollWidth - (FHorzAdj + Adjustment * 2.0 +
       FScrollBarWidth)) / (PaperWidth * FXDPI);
    end; { else }
  end; { if }

  Result := NewZoom * 100.0;
end;  { GetZoomPageWidthFactor }

function TFilePreview.GetZoomPageFactor: double;

var
  Z1,Z2: double;
  Adjustment: integer;
  ScrollWidth: integer;
  ScrollHeight: integer;

begin { GetZoomPageFactor }
  CheckPrinting;

{ Calculate new scrollwidth taking into account the shadow depth }
  ScrollWidth := FScrollBox.Width - ShadowDepth - RulerWidth;
  ScrollHeight := FScrollBox.Height - ShadowDepth - RulerHeight;

  if FMarginMethod = mmScaled then begin
    Z1 := (ScrollWidth - 4) / (PaperWidth * FXDPI *
     (1.0 + FMarginPercent / 50.0));
    Z2 := (ScrollHeight - 4) / (PaperHeight * FYDPI +
     (PaperWidth * FXDPI * FMarginPercent / 50.0));
  end else begin { mmFixed }
    Adjustment := Round(PaperWidth * FXDPI * (FMarginPercent / 100.0));
    Z1 := (ScrollWidth - 4 - Adjustment * 2.0) / (PaperWidth * FXDPI);
    Z2 := (ScrollHeight - 4 - Adjustment * 2.0) / (PaperHeight * FYDPI);
  end; { else }

  if Z1 <= Z2 then begin
    Result := Z1 * 100.0;
  end else begin
    Result := Z2 * 100.0;
  end; { else }
end;  { GetZoomPageFactor }

procedure TFilePreview.Notification(AComponent: TComponent;
                                     Operation: TOperation);

begin { Notification }
  inherited Notification(AComponent,Operation);
  if (Operation = opRemove) and (AComponent = FScrollBox) then begin
    FScrollBox := nil;
  end; { if }
end;  { Notification }

procedure TFilePreview.SetScrollBox(Value: TScrollBox);

begin { SetScrollBox }
  FScrollBox := Value;
  if Assigned(Value) then begin
    Value.FreeNotification(self);
  end; { if }
end;  { SetScrollBox }

function TFilePreview.Destination: TDestination;

begin { Destination }
  Result := destPreview;
end;  { Destination }

{ Public Methods }
procedure TFilePreview.PrevPage;

begin { PrevPage }
  if PrevPagePos > 0 then begin
    PrintPage(FCurrentPage - FPageInc);
  end; { if }
end;  { PrevPage }

procedure TFilePreview.NextPage;

begin { NextPage }
  if NextPagePos > 0 then begin
    PrintPage(FCurrentPage + FPageInc);
  end; { if }
end;  { NextPage }

procedure TFilePreview.ZoomIn;

begin { ZoomIn }
  ZoomFactor := ZoomFactor + FZoomInc;
end;  { ZoomIn }

procedure TFilePreview.ZoomOut;
begin { ZoomOut }
  if ZoomFactor > FZoomInc then begin
    ZoomFactor := ZoomFactor - FZoomInc;
  end;
end;  { ZoomOut }

procedure TFilePreview.RedrawPage;
begin { RedrawPage }
  PrintPage(FCurrentPage);
end;  { RedrawPage }

procedure TFilePreview.PrintPage(PageNum: word);
var
  F1: double;
  Margin: double;
  SaveHorzPos: integer;
  SaveVertPos: integer;
  Adjustment: integer;
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
  SaveCursor: TCursor;
begin { PrintPage }
  if not Printing then Exit;

  if Monochrome then begin
    ShadowDepth := 0;
  end; { if }

  case RulerType of
    rtNone,rtHorizCm,rtHorizIn: begin
      RulerWidth := 0;
    end;
    else begin
      RulerWidth := 20;
    end;
  end; { case }

  case RulerType of
    rtNone,rtVertCm,rtVertIn: begin
      RulerHeight := 0;
    end;
    else begin
      RulerHeight := 20;
    end;
  end; { case }

  AddX := RulerWidth;
  AddY := RulerHeight;

  RulerColor := RGB(255,255,128);
  SaveCursor := Screen.Cursor;
  Screen.Cursor := crHourGlass;
  try
    GotoPage(PageNum); { Read in page header }

  { Call the OnPageChange event }
    PageChangeEvent;

    with FScrollBox do begin
      SaveHorzPos := HorzScrollBar.Position;
      HorzScrollBar.Position := 0;
      SaveVertPos := VertScrollBar.Position;
      VertScrollBar.Position := 0;
    end; { with }

  { Initialize bitmap and calculate adjustment values for pseudo page }
    Margin := FMarginPercent / 100.0;
    F1 := PaperWidth * FXDPI;
    if FMarginMethod = mmScaled then begin
      Adjustment := Round(F1 * Margin * FZoom / 100.0);
    end else begin
      Adjustment := Round(F1 * Margin);
    end; { else }
    F1 := F1 * FZoom / 100.0;
    FBitmap.Width := Round(F1) + ShadowDepth + AddX;
    FBitmap.Height := Round(PaperHeight * FYDPI * FZoom / 100.0) +
     ShadowDepth + AddY;

  { Draw psuedo-paper }

  { Draw the shadow first }

  { Lower left corner }
    SetPen(FScrollBox.Color,psSolid,1,pmCopy);
    SetBrush(FScrollBox.Color,bsSolid,nil);
    Canvas.Rectangle(0, FBitmap.Height - ShadowDepth,
     ShadowDepth + AddX,FBitmap.Height);

  { Right Shadow }
    SetPen(clBlack,psSolid,1,pmCopy);
    SetBrush(clBlack,bsSolid,nil);
    Canvas.Rectangle(FBitmap.Width - ShadowDepth, ShadowDepth + AddY,
     FBitMap.Width,FBitmap.Height);

  { Upper Right Corner }
    SetPen(FScrollBox.Color,psSolid,1,pmCopy);
    SetBrush(FScrollBox.Color,bsSolid,nil);
    Canvas.Rectangle(FBitmap.Width - ShadowDepth,0, FBitMap.Width,
     ShadowDepth + AddY);

  { Lower Shadow }
    SetPen(clBlack,psSolid,1,pmCopy);
    SetBrush(clBlack,bsSolid,nil);
    Canvas.Rectangle(ShadowDepth + AddX, FBitmap.Height - ShadowDepth,
     FBitmap.Width,FBitmap.Height);

  { Draw the ruler }
  { Upper left corner shadow }
    if (RulerWidth > 0) and (RulerHeight > 0) then begin
      SetPen(clLtGray,psSolid,1,pmCopy);
      SetBrush(clLtGray,bsSolid,nil);
      Canvas.Rectangle(0,0,AddX, AddY);
    end; { if }

  { Upper right ruler shadow }

    if RulerType in [rtHorizIn,rtHorizCm,rtBothIn,rtBothCm] then begin
      SetPen(clBlack,psSolid,1,pmCopy);
      SetBrush(clBlack,bsSolid,nil);
      Canvas.Rectangle(FBitMap.Width - ShadowDepth,ShadowDepth,
      FBitmap.Width,AddY + ShadowDepth);
     end; { if }

  { Lower left ruler shadow }
    if RulerType in [rtVertIn, rtVertCm,rtBothIn,rtBothCm] then begin
      SetPen(clBlack,psSolid,1,pmCopy);
      SetBrush(clBlack,bsSolid,nil);
      Canvas.Rectangle(ShadowDepth,FBitMap.Height - ShadowDepth,
       AddX + ShadowDepth,FBitMap.Height);
     end; { if }

  { Left side ruler }
    if RulerWidth > 0 then begin
      SetPen(clBlack,psSolid,1,pmCopy);
      SetBrush(RulerColor,bsSolid,nil);
      Canvas.Rectangle(0,AddY,AddX + 1,FBitmap.Height - ShadowDepth);
    end; { if }

  { Top Ruler }
    if RulerHeight > 0 then begin
      SetPen(clBlack,psSolid,1,pmCopy);
      SetBrush(RulerColor,bsSolid,nil);
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
    SetPen(clBlack,psSolid,1,pmCopy);
    SetBrush(clWhite,bsSolid,nil);
    Canvas.Rectangle(AddX,AddY,FBitmap.Width - ShadowDepth,
     FBitmap.Height - ShadowDepth);

  { Draw the Grid }
  { Horizontal Lines }
    if GridHoriz > 0 then begin
      SavePen := FBitMap.Canvas.Pen;
      FBitmap.Canvas.Pen := FGridPen;
      GridTarget := FBitmap.Height - ShadowDepth;
      GridMarker := AddY + GridHoriz * FYDPI * FZoom / 100.0;
      while Round(GridMarker) < GridTarget do begin
        FBitMap.Canvas.MoveTo(AddX + 1,Round(GridMarker));
        FBitMap.Canvas.LineTo(FBitmap.Width - (ShadowDepth + 1),
         Round(GridMarker));
        GridMarker := GridMarker + GridHoriz * FYDPI * FZoom / 100.0;
      end; { while }
      FBitmap.Canvas.Pen := SavePen;
    end; { if }

  { Vertical Lines }
    if GridVert > 0 then begin
      SavePen := FBitMap.Canvas.Pen;
      FBitmap.Canvas.Pen := FGridPen;
      GridTarget := FBitmap.Width - ShadowDepth;
      GridMarker := AddX + GridVert * FXDPI * FZoom / 100.0;
      while Round(GridMarker) < GridTarget do begin
        FBitMap.Canvas.MoveTo(Round(GridMarker) ,AddY + 1);
        FBitMap.Canvas.LineTo(Round(GridMarker),FBitmap.Height -
         (ShadowDepth + 1));
        GridMarker := GridMarker + GridVert * FXDPI * FZoom / 100.0;
      end; { while }
      FBitmap.Canvas.Pen := SavePen;
    end; { if }

    inherited PrintPage(PageNum); { Draw page }

  { Initialize Image }
    with FImage do begin
      Top := Adjustment;
      Left := Adjustment;
      Width := FBitmap.Width;
      Height := FBitmap.Height;
      Picture.Bitmap := FBitmap;
    end; { with }

    with FScrollBox do begin
      with HorzScrollBar do begin
        Position := SaveHorzPos;
        Range := FBitmap.Width + 2 * Adjustment;
      end; { with }
      with VertScrollBar do begin
        Position := SaveVertPos;
        Range := FBitmap.Height + 2 * Adjustment;
      end; { with }
    end; { with }
  finally
    Screen.Cursor := SaveCursor;
    ClearFontList;
    ClearLineLists;
  end; { tryf }
end;  { PrintPage }

procedure TFilePreview.Start;

begin { Start }
  if FPrinting then Exit;
  if not Assigned(FScrollBox) then begin
    RaiseError('ScrollBox must be assigned for FilePreview component');
  end; { if }

  FLocalDevice := TRPRenderDevice.Create;
  ReportDateTime := Now;
  ZoomChangeEvent;

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

  FBitmap := TBitmap.Create;
  (FLocalDevice as TRPRenderDevice).Canvas := FBitmap.Canvas;
  FBitmap.Canvas.Handle; { Load valid font, pen and brush }
  FBitmap.Monochrome := Monochrome;
  if Assigned(FImage) then begin
    FScrollBox.Owner.RemoveComponent(FImage);
    FImage.Free;
  end; { if }
  FImage := TImage.Create(FScrollBox.Owner);
  with FImage do begin
    Parent := FScrollBox;
    Align := alNone;
    Top := 0;
    Left := 0;
    Width := 100;
    Height := 100;
  end; { with }

  AddX := 0;
  AddY := 0;
  FImage.Canvas.MoveTo(0,0);
  FXDPI := GetDeviceCaps(FImage.Canvas.Handle,LOGPIXELSX);
  FYDPI := GetDeviceCaps(FImage.Canvas.Handle,LOGPIXELSY);

  JobHeaderRead := false; { force reading of header }
  ReadJobHeader; { Read in job header }
  FCurrentPage := 0;
  PrevPagePos := 0;
  CurrPagePos := 0;
  NextPagePos := FStream.Position;
  FPrinting := true;
  FAborted := false;
  PrintPage(1); { Print first available page }
end;  { Start }

procedure TFilePreview.Finish;

begin { Finish }
  if not FPrinting then Exit;
  FPrinting := false;

{ Unhook from visual component event handlers }
  FScrollBox.Owner.RemoveComponent(FImage);
  FreeAndNil(FImage);
  FreeAndNil(FBitmap);

{ Close input stream }
  if FStreamMode <> smUser then begin
    FreeAndNil(FStream);
  end; { if }
  JobHeaderRead := false;

  FreeAndNil(FLocalDevice);
end;  { Finish }

procedure TFilePreview.Execute;

begin { Execute }
  Start;
end;  { Execute }

procedure TFilePreview.Clear;

begin { Clear } {!!! New Function }
  FreeAndNil(FImage);
end;  { Clear }

end.