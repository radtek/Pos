
{*****************************************************************************}
{                                                                             }
{    Tnt Delphi Unicode Controls                                              }
{      http://home.ccci.org/wolbrink/tnt/delphi_unicode_controls.htm          }
{        Version: 2.0.22                                                      }
{                                                                             }
{    Copyright (c) 2002, 2003 Troy Wolbrink (troy.wolbrink@ccci.org)          }
{                                                                             }
{*****************************************************************************}

unit TntButtons;

{$INCLUDE TntCompilers.inc}

interface

uses
  Windows, Messages, Classes, Controls, Forms, Graphics, StdCtrls,
  ExtCtrls, CommCtrl, Buttons,
  TntForms, TntControls, TntStdCtrls;

type
{TNT-WARN TSpeedButton}
  TTntSpeedButton = class(TSpeedButton{TNT-ALLOW TSpeedButton})
  private
    FPaintInherited: Boolean;
    function GetCaption: TWideCaption;
    procedure SetCaption(const Value: TWideCaption);
    function GetHint: WideString;
    procedure SetHint(const Value: WideString);
    function IsCaptionStored: Boolean;
    function IsHintStored: Boolean;
    procedure CMHintShow(var Message: TMessage); message CM_HINTSHOW;
    procedure CMDialogChar(var Message: TCMDialogChar); message CM_DIALOGCHAR;
    procedure UpdateInternalGlyphList;
  protected
    procedure Paint; override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
  published
    property Caption: TWideCaption read GetCaption write SetCaption stored IsCaptionStored;
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TBitBtn}
  TTntBitBtn = class(TBitBtn{TNT-ALLOW TBitBtn})
  private
    FPaintInherited: Boolean;
    FMouseInControl: Boolean;
    function IsCaptionStored: Boolean;
    function GetCaption: TWideCaption;
    procedure SetCaption(const Value: TWideCaption);
    function IsHintStored: Boolean;
    function GetHint: WideString;
    procedure SetHint(const Value: WideString);
    procedure CMDialogChar(var Message: TCMDialogChar); message CM_DIALOGCHAR;
    procedure CNDrawItem(var Message: TWMDrawItem); message CN_DRAWITEM;
    procedure DrawItem(const DrawItemStruct: TDrawItemStruct);
    procedure UpdateInternalGlyphList;
    procedure CMMouseEnter(var Message: TMessage); message CM_MOUSEENTER;
    procedure CMMouseLeave(var Message: TMessage); message CM_MOUSELEAVE;
  protected
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
  published
    property Caption: TWideCaption read GetCaption write SetCaption stored IsCaptionStored;
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

procedure TButtonGlyph_CalcButtonLayout(Control: TControl; DC: HDC; const Client: TRect;
  const Offset: TPoint; const Caption: WideString; Layout: TButtonLayout;
      Margin, Spacing: Integer; var GlyphPos: TPoint; var TextBounds: TRect; BiDiFlags: Integer);

implementation

uses
  SysUtils, TypInfo, {$IFDEF COMPILER_6_UP} RTLConsts, {$ELSE} Consts, {$ENDIF} TntWindows,
  {$IFDEF COMPILER_7_UP} Themes, {$ENDIF} TntClasses, TntActnList, ActnList;

type
  EAbortPaint = class(EAbort);

// Many routines in this unit are nearly the same as those found in Buttons.pas.  They are
//   included here because the VCL implementation of TButtonGlyph is completetly inaccessible.

type
  THackButtonGlyph_D5_D6_D7 = class
  protected
    FOriginal: TBitmap;
    FGlyphList: TImageList;
    FIndexs: array[TButtonState] of Integer;
    FxxxxTransparentColor: TColor;
    FNumGlyphs: TNumGlyphs;
  end;

  THackBitBtn_D5_D6_D7 = class(TButton{TNT-ALLOW TButton})
  protected
    FCanvas: TCanvas;
    FGlyph: Pointer;
    FxxxxStyle: TButtonStyle;
    FxxxxKind: TBitBtnKind;
    FxxxxLayout: TButtonLayout;
    FxxxxSpacing: Integer;
    FxxxxMargin: Integer;
    IsFocused: Boolean;
  end;

  THackSpeedButton_D5_D6_D7 = class(TGraphicControl)
  protected
    FxxxxGroupIndex: Integer;
    FGlyph: Pointer;
    FxxxxDown: Boolean;
    FDragging: Boolean;
  end;

  {$IFDEF COMPILER_5} // verified against VCL source in Delphi 5 and BCB 5
  THackButtonGlyph = THackButtonGlyph_D5_D6_D7;
  THackBitBtn      = THackBitBtn_D5_D6_D7;
  THackSpeedButton = THackSpeedButton_D5_D6_D7;
  {$ENDIF}
  {$IFDEF COMPILER_6} // verified against VCL source in Delphi 6 and BCB 6
  THackButtonGlyph = THackButtonGlyph_D5_D6_D7;
  THackBitBtn      = THackBitBtn_D5_D6_D7;
  THackSpeedButton = THackSpeedButton_D5_D6_D7;
  {$ENDIF}
  {$IFDEF DELPHI_7}
  THackButtonGlyph = THackButtonGlyph_D5_D6_D7;
  THackBitBtn      = THackBitBtn_D5_D6_D7;
  THackSpeedButton = THackSpeedButton_D5_D6_D7;
  {$ENDIF}

function GetButtonGlyphObject(Control: TControl): THackButtonGlyph;
begin
  if Control is TTntBitBtn then
    Result := THackBitBtn(Control).FGlyph
  else if Control is TTntSpeedButton then
    Result := THackSpeedButton(Control).FGlyph
  else
    raise Exception.Create('TNT Internal Error: wrong button class for GetButtonGlyphObject.');
end;

function TButtonGlyph_CreateButtonGlyph(Control: TControl; State: TButtonState): Integer;
var
  ButtonGlyph: THackButtonGlyph;
  NumGlyphs: Integer;
begin
  ButtonGlyph := GetButtonGlyphObject(Control);
  NumGlyphs := ButtonGlyph.FNumGlyphs;

  if (State = bsDown) and (NumGlyphs < 3) then State := bsUp;
  Result := ButtonGlyph.FIndexs[State];
  if (Result = -1) then begin
    if Control is TTntBitBtn then begin
      TTntBitBtn(Control).UpdateInternalGlyphList;
    end else if Control is TTntSpeedButton then begin
      TTntSpeedButton(Control).UpdateInternalGlyphList;
    end else
      raise Exception.Create('TNT Internal Error: wrong button class for CreateButtonGlyph.');
    Result := ButtonGlyph.FIndexs[State];
  end;
end;

procedure TButtonGlyph_DrawButtonGlyph(Control: TControl; Canvas: TCanvas; const GlyphPos: TPoint;
  State: TButtonState; Transparent: Boolean);
var
  ButtonGlyph: THackButtonGlyph;
  Glyph: TBitmap;
  GlyphList: TImageList;
  Index: Integer;
  {$IFDEF COMPILER_7_UP}
  Details: TThemedElementDetails;
  R: TRect;
  Button: TThemedButton;
  {$ENDIF}
begin
  ButtonGlyph := GetButtonGlyphObject(Control);
  Glyph := ButtonGlyph.FOriginal;
  GlyphList := ButtonGlyph.FGlyphList;
  if Glyph = nil then Exit;
  if (Glyph.Width = 0) or (Glyph.Height = 0) then Exit;
  Index := TButtonGlyph_CreateButtonGlyph(Control, State);
  with GlyphPos do
  {$IFDEF COMPILER_7_UP}
  if ThemeServices.ThemesEnabled then begin
    R.TopLeft := GlyphPos;
    R.Right := R.Left + Glyph.Width div ButtonGlyph.FNumGlyphs;
    R.Bottom := R.Top + Glyph.Height;
    case State of
      bsDisabled:
        Button := tbPushButtonDisabled;
      bsDown,
      bsExclusive:
        Button := tbPushButtonPressed;
    else
      // bsUp
      Button := tbPushButtonNormal;
    end;
    Details := ThemeServices.GetElementDetails(Button);
    ThemeServices.DrawIcon(Canvas.Handle, Details, R, GlyphList.Handle, Index);
  end else
  {$ENDIF}
    if Transparent or (State = bsExclusive) then
      ImageList_DrawEx(GlyphList.Handle, Index, Canvas.Handle, X, Y, 0, 0,
        clNone, clNone, ILD_Transparent)
    else
      ImageList_DrawEx(GlyphList.Handle, Index, Canvas.Handle, X, Y, 0, 0,
        ColorToRGB(clBtnFace), clNone, ILD_Normal);
end;

procedure TButtonGlyph_DrawButtonText(Canvas: TCanvas; const Caption: WideString;
  TextBounds: TRect; State: TButtonState; BiDiFlags: LongInt);
begin
  with Canvas do
  begin
    Brush.Style := bsClear;
    if State = bsDisabled then
    begin
      OffsetRect(TextBounds, 1, 1);
      Font.Color := clBtnHighlight;
      Tnt_DrawTextW(Handle, PWideChar(Caption), Length(Caption), TextBounds,
        DT_CENTER or DT_VCENTER or BiDiFlags);
      OffsetRect(TextBounds, -1, -1);
      Font.Color := clBtnShadow;
      Tnt_DrawTextW(Handle, PWideChar(Caption), Length(Caption), TextBounds,
        DT_CENTER or DT_VCENTER or BiDiFlags);
    end else
      Tnt_DrawTextW(Handle, PWideChar(Caption), Length(Caption), TextBounds,
        DT_CENTER or DT_VCENTER or BiDiFlags);
  end;
end;

procedure TButtonGlyph_CalcButtonLayout(Control: TControl; DC: HDC; const Client: TRect;
  const Offset: TPoint; const Caption: WideString; Layout: TButtonLayout;
      Margin, Spacing: Integer; var GlyphPos: TPoint; var TextBounds: TRect; BiDiFlags: Integer);
var
  TextPos: TPoint;
  ClientSize,
  GlyphSize,
  TextSize: TPoint;
  TotalSize: TPoint;
  Glyph: TBitmap;
  NumGlyphs: Integer;
  ButtonGlyph: THackButtonGlyph;
begin
  ButtonGlyph := GetButtonGlyphObject(Control);
  Glyph := ButtonGlyph.FOriginal;
  NumGlyphs := ButtonGlyph.FNumGlyphs;

  if (BiDiFlags and DT_RIGHT) = DT_RIGHT then
    if Layout = blGlyphLeft then
      Layout := blGlyphRight
    else
      if Layout = blGlyphRight then
        Layout := blGlyphLeft;

  // Calculate the item sizes.
  ClientSize := Point(Client.Right - Client.Left, Client.Bottom - Client.Top);

  if Assigned(Glyph) then
    GlyphSize := Point(Glyph.Width div NumGlyphs, Glyph.Height)
  else
    GlyphSize := Point(0, 0);

  if Length(Caption) > 0 then
  begin
    TextBounds := Rect(0, 0, Client.Right - Client.Left, 0);
    Tnt_DrawTextW(DC, PWideChar(Caption), Length(Caption), TextBounds, DT_CALCRECT or BiDiFlags);
    TextSize := Point(TextBounds.Right - TextBounds.Left, TextBounds.Bottom - TextBounds.Top);
  end
  else
  begin
    TextBounds := Rect(0, 0, 0, 0);
    TextSize := Point(0,0);
  end;

  // If the layout has the glyph on the right or the left, then both the text and the glyph are centered vertically.
  // If the glyph is on the top or the bottom, then both the text and the glyph are centered horizontally.
  if Layout in [blGlyphLeft, blGlyphRight] then
  begin
    GlyphPos.Y := (ClientSize.Y - GlyphSize.Y + 1) div 2;
    TextPos.Y := (ClientSize.Y - TextSize.Y + 1) div 2;
  end
  else
  begin
    GlyphPos.X := (ClientSize.X - GlyphSize.X + 1) div 2;
    TextPos.X := (ClientSize.X - TextSize.X + 1) div 2;
  end;

  // If there is no text or no bitmap, then Spacing is irrelevant.
  if (TextSize.X = 0) or (GlyphSize.X = 0) then
    Spacing := 0;

  // Adjust Margin and Spacing.
  if Margin = -1 then
  begin
    if Spacing = -1 then
    begin
      TotalSize := Point(GlyphSize.X + TextSize.X, GlyphSize.Y + TextSize.Y);
      if Layout in [blGlyphLeft, blGlyphRight] then
        Margin := (ClientSize.X - TotalSize.X) div 3
      else
        Margin := (ClientSize.Y - TotalSize.Y) div 3;
      Spacing := Margin;
    end
    else
    begin
      TotalSize := Point(GlyphSize.X + Spacing + TextSize.X, GlyphSize.Y + Spacing + TextSize.Y);
      if Layout in [blGlyphLeft, blGlyphRight] then
        Margin := (ClientSize.X - TotalSize.X + 1) div 2
      else
        Margin := (ClientSize.Y - TotalSize.Y + 1) div 2;
    end;
  end
  else
  begin
    if Spacing = -1 then
    begin
      TotalSize := Point(ClientSize.X - (Margin + GlyphSize.X), ClientSize.Y - (Margin + GlyphSize.Y));
      if Layout in [blGlyphLeft, blGlyphRight] then
        Spacing := (TotalSize.X - TextSize.X) div 2
      else
        Spacing := (TotalSize.Y - TextSize.Y) div 2;
    end;
  end;
    
  case Layout of
    blGlyphLeft:
      begin
        GlyphPos.X := Margin;
        TextPos.X := GlyphPos.X + GlyphSize.X + Spacing;
      end;
    blGlyphRight:
      begin
        GlyphPos.X := ClientSize.X - Margin - GlyphSize.X;
        TextPos.X := GlyphPos.X - Spacing - TextSize.X;
      end;
    blGlyphTop:
      begin
        GlyphPos.Y := Margin;
        TextPos.Y := GlyphPos.Y + GlyphSize.Y + Spacing;
      end;
    blGlyphBottom:
      begin
        GlyphPos.Y := ClientSize.Y - Margin - GlyphSize.Y;
        TextPos.Y := GlyphPos.Y - Spacing - TextSize.Y;
      end;
  end;

  // Fixup the Result variables.
  with GlyphPos do
  begin
    Inc(X, Client.Left + Offset.X);
    Inc(Y, Client.Top + Offset.Y);
  end;

  {$IFDEF COMPILER_7_UP}
  { Themed text is not shifted, but gets a different color. }
  if ThemeServices.ThemesEnabled then
    OffsetRect(TextBounds, TextPos.X + Client.Left, TextPos.Y + Client.Top)
  else
  {$ENDIF}
    OffsetRect(TextBounds, TextPos.X + Client.Left + Offset.X, TextPos.Y + Client.Top + Offset.Y);
end;

function TButtonGlyph_Draw(Control: TControl; Canvas: TCanvas; const Client: TRect;
  const Offset: TPoint; const Caption: WideString; Layout: TButtonLayout; Margin: Integer;
    Spacing: Integer; State: TButtonState; Transparent: Boolean; BiDiFlags: Longint): TRect;
var
  GlyphPos: TPoint;
begin
  TButtonGlyph_CalcButtonLayout(Control, Canvas.Handle, Client, Offset, Caption, Layout, Margin,
    Spacing, GlyphPos, Result, BiDiFlags);
  TButtonGlyph_DrawButtonGlyph(Control, Canvas, GlyphPos, State, Transparent);
  TButtonGlyph_DrawButtonText(Canvas, Caption, Result, State, BiDiFlags);
end;

{ TTntSpeedButton }

procedure TTntSpeedButton.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

function TTntSpeedButton.IsCaptionStored: Boolean;
begin
  Result := TntControl_IsCaptionStored(Self)
end;

function TTntSpeedButton.GetCaption: TWideCaption;
begin
  Result := TntControl_GetText(Self);
end;

procedure TTntSpeedButton.SetCaption(const Value: TWideCaption);
begin
  TntControl_SetText(Self, Value);
end;

function TTntSpeedButton.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self)
end;

function TTntSpeedButton.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self)
end;

procedure TTntSpeedButton.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntSpeedButton.CMHintShow(var Message: TMessage);
begin
  ProcessCMHintShowMsg(Message);
  inherited;
end;

procedure TTntSpeedButton.CMDialogChar(var Message: TCMDialogChar);
begin
  with Message do
    if IsWideCharAccel(CharCode, Caption) and Enabled and Visible and
      (Parent <> nil) and Parent.Showing then
    begin
      Click;
      Result := 1;
    end else
      inherited;
end;

procedure TTntSpeedButton.UpdateInternalGlyphList;
begin
  FPaintInherited := True;
  try
    Repaint;
  finally
    FPaintInherited := False;
  end;
  Invalidate;
  raise EAbortPaint.Create('');
end;

procedure TTntSpeedButton.Paint;
const
  DownStyles: array[Boolean] of Integer = (BDR_RAISEDINNER, BDR_SUNKENOUTER);
  FillStyles: array[Boolean] of Integer = (BF_MIDDLE, 0);
var
  PaintRect: TRect;
  DrawFlags: Integer;
  Offset: TPoint;
  {$IFDEF COMPILER_7_UP}
  Button: TThemedButton;
  ToolButton: TThemedToolBar;
  Details: TThemedElementDetails;
  {$ENDIF}
begin
  if FPaintInherited then
    inherited
  else begin
    try
      if not Enabled then
      begin
        FState := bsDisabled;
        THackSpeedButton(Self).FDragging := False;
      end
      else if FState = bsDisabled then
        if Down and (GroupIndex <> 0) then
          FState := bsExclusive
        else
          FState := bsUp;
      Canvas.Font := Self.Font;

      {$IFDEF COMPILER_7_UP}
      if ThemeServices.ThemesEnabled then
      begin
        PerformEraseBackground(Self, Canvas.Handle);
        SelectObject(Canvas.Handle, Canvas.Font.Handle); { For some reason, PerformEraseBackground sometimes messes the font up. }

        if not Enabled then
          Button := tbPushButtonDisabled
        else
          if FState in [bsDown, bsExclusive] then
            Button := tbPushButtonPressed
          else
            if MouseInControl then
              Button := tbPushButtonHot
            else
              Button := tbPushButtonNormal;

        ToolButton := ttbToolbarDontCare;
        if Flat then
        begin
          case Button of
            tbPushButtonDisabled:
              Toolbutton := ttbButtonDisabled;
            tbPushButtonPressed:
              Toolbutton := ttbButtonPressed;
            tbPushButtonHot:
              Toolbutton := ttbButtonHot;
            tbPushButtonNormal:
              Toolbutton := ttbButtonNormal;
          end;
        end;

        PaintRect := ClientRect;
        if ToolButton = ttbToolbarDontCare then
        begin
          Details := ThemeServices.GetElementDetails(Button);
          ThemeServices.DrawElement(Canvas.Handle, Details, PaintRect);
          PaintRect := ThemeServices.ContentRect(Canvas.Handle, Details, PaintRect);
        end
        else
        begin
          Details := ThemeServices.GetElementDetails(ToolButton);
          ThemeServices.DrawElement(Canvas.Handle, Details, PaintRect);
          PaintRect := ThemeServices.ContentRect(Canvas.Handle, Details, PaintRect);
        end;

        if Button = tbPushButtonPressed then
        begin
          // A pressed speed button has a white text. This applies however only to flat buttons.
          if ToolButton <> ttbToolbarDontCare then
            Canvas.Font.Color := clHighlightText;
          Offset := Point(1, 0);
        end
        else
          Offset := Point(0, 0);
        TButtonGlyph_Draw(Self, Canvas, PaintRect, Offset, Caption, Layout, Margin, Spacing, FState,
          Transparent, DrawTextBiDiModeFlags(0));
      end
      else
      {$ENDIF}
      begin
        PaintRect := Rect(0, 0, Width, Height);
        if not Flat then
        begin
          DrawFlags := DFCS_BUTTONPUSH or DFCS_ADJUSTRECT;
          if FState in [bsDown, bsExclusive] then
            DrawFlags := DrawFlags or DFCS_PUSHED;
          DrawFrameControl(Canvas.Handle, PaintRect, DFC_BUTTON, DrawFlags);
        end
        else
        begin
          if (FState in [bsDown, bsExclusive]) or
            (MouseInControl and (FState <> bsDisabled)) or
            (csDesigning in ComponentState) then
            DrawEdge(Canvas.Handle, PaintRect, DownStyles[FState in [bsDown, bsExclusive]],
              FillStyles[Transparent] or BF_RECT)
          else if not Transparent then
          begin
            Canvas.Brush.Color := Color;
            Canvas.FillRect(PaintRect);
          end;
          InflateRect(PaintRect, -1, -1);
        end;
        if FState in [bsDown, bsExclusive] then
        begin
          if (FState = bsExclusive) and (not Flat or not MouseInControl) then
          begin
            Canvas.Brush.Bitmap := AllocPatternBitmap(clBtnFace, clBtnHighlight);
            Canvas.FillRect(PaintRect);
          end;
          Offset.X := 1;
          Offset.Y := 1;
        end
        else
        begin
          Offset.X := 0;
          Offset.Y := 0;
        end;
        TButtonGlyph_Draw(Self, Canvas, PaintRect, Offset, Caption,
          Layout, Margin, Spacing, FState, Transparent, DrawTextBiDiModeFlags(0));
      end;
    except
      on E: EAbortPaint do
        ;
      else
        raise;
    end;
  end;
end;

function TTntSpeedButton.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

procedure TTntSpeedButton.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

{ TTntButton }

procedure TTntBitBtn.CreateWindowHandle(const Params: TCreateParams);
begin
  CreateUnicodeHandle(Self, Params, 'BUTTON');
end;

procedure TTntBitBtn.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

function TTntBitBtn.IsCaptionStored: Boolean;
var
  PropInfo: PPropInfo;
begin
  // TTntBitBtn has it's own storage specifier, based upon the button kind
  PropInfo := GetPropInfo(TBitBtn{TNT-ALLOW TBitBtn}, 'Caption');
  if PropInfo = nil then
    raise EPropertyError.CreateResFmt(PResStringRec(@SUnknownProperty), ['Caption']);
  Result := IsStoredProp(Self, PropInfo);
end;

function TTntBitBtn.GetCaption: TWideCaption;
begin
  Result := TntControl_GetText(Self)
end;

procedure TTntBitBtn.SetCaption(const Value: TWideCaption);
begin
  TntControl_SetText(Self, Value);
end;

function TTntBitBtn.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self)
end;

function TTntBitBtn.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self)
end;

procedure TTntBitBtn.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntBitBtn.CMDialogChar(var Message: TCMDialogChar);
begin
  TntButton_CMDialogChar(Self, Message);
end;

procedure TTntBitBtn.UpdateInternalGlyphList;
begin
  FPaintInherited := True;
  try
    Repaint;
  finally
    FPaintInherited := False;
  end;
  Invalidate;
  raise EAbortPaint.Create('');
end;

procedure TTntBitBtn.CNDrawItem(var Message: TWMDrawItem);
begin
  if FPaintInherited then
    inherited
  else
    DrawItem(Message.DrawItemStruct^);
end;

procedure TTntBitBtn.DrawItem(const DrawItemStruct: TDrawItemStruct);
var
  IsDown, IsDefault: Boolean;
  State: TButtonState;
  R: TRect;
  Flags: Longint;
  FCanvas: TCanvas;
  IsFocused: Boolean;
  {$IFDEF COMPILER_7_UP}
  Details: TThemedElementDetails;
  Button: TThemedButton;
  Offset: TPoint;
  {$ENDIF}
begin
  try
    FCanvas := THackBitBtn(Self).FCanvas;
    IsFocused := THackBitBtn(Self).IsFocused;
    FCanvas.Handle := DrawItemStruct.hDC;
    R := ClientRect;
    
    with DrawItemStruct do
    begin
      FCanvas.Handle := hDC;
      FCanvas.Font := Self.Font;
      IsDown := itemState and ODS_SELECTED <> 0;
      IsDefault := itemState and ODS_FOCUS <> 0;

      if not Enabled then State := bsDisabled
      else if IsDown then State := bsDown
      else State := bsUp;
    end;

    {$IFDEF COMPILER_7_UP}
    if ThemeServices.ThemesEnabled then
    begin
      if not Enabled then
        Button := tbPushButtonDisabled
      else
        if IsDown then
          Button := tbPushButtonPressed
        else
          if FMouseInControl then
            Button := tbPushButtonHot
          else
            if IsFocused or IsDefault then
              Button := tbPushButtonDefaulted
            else
              Button := tbPushButtonNormal;

      Details := ThemeServices.GetElementDetails(Button);
      // Parent background.
      ThemeServices.DrawParentBackground(Handle, DrawItemStruct.hDC, @Details, True);
      // Button shape.
      ThemeServices.DrawElement(DrawItemStruct.hDC, Details, DrawItemStruct.rcItem);
      R := ThemeServices.ContentRect(FCanvas.Handle, Details, DrawItemStruct.rcItem);

      if Button = tbPushButtonPressed then
        Offset := Point(1, 0)
      else
        Offset := Point(0, 0);
      TButtonGlyph_Draw(Self, FCanvas, R, Offset, Caption, Layout, Margin, Spacing, State, False,
        DrawTextBiDiModeFlags(0));

      if IsFocused and IsDefault then
      begin
        FCanvas.Pen.Color := clWindowFrame;
        FCanvas.Brush.Color := clBtnFace;
        DrawFocusRect(FCanvas.Handle, R);
      end;
    end
    else
    {$ENDIF}
    begin
      R := ClientRect;

      Flags := DFCS_BUTTONPUSH or DFCS_ADJUSTRECT;
      if IsDown then Flags := Flags or DFCS_PUSHED;
      if DrawItemStruct.itemState and ODS_DISABLED <> 0 then
        Flags := Flags or DFCS_INACTIVE;

      { DrawFrameControl doesn't allow for drawing a button as the
          default button, so it must be done here. }
      if IsFocused or IsDefault then
      begin
        FCanvas.Pen.Color := clWindowFrame;
        FCanvas.Pen.Width := 1;
        FCanvas.Brush.Style := bsClear;
        FCanvas.Rectangle(R.Left, R.Top, R.Right, R.Bottom);

        { DrawFrameControl must draw within this border }
        InflateRect(R, -1, -1);
      end;

      { DrawFrameControl does not draw a pressed button correctly }
      if IsDown then
      begin
        FCanvas.Pen.Color := clBtnShadow;
        FCanvas.Pen.Width := 1;
        FCanvas.Brush.Color := clBtnFace;
        FCanvas.Rectangle(R.Left, R.Top, R.Right, R.Bottom);
        InflateRect(R, -1, -1);
      end
      else
        DrawFrameControl(DrawItemStruct.hDC, R, DFC_BUTTON, Flags);

      if IsFocused then
      begin
        R := ClientRect;
        InflateRect(R, -1, -1);
      end;

      FCanvas.Font := Self.Font;
      if IsDown then
        OffsetRect(R, 1, 1);

      TButtonGlyph_Draw(Self, FCanvas, R, Point(0,0), Caption, Layout, Margin, Spacing, State,
        False, DrawTextBiDiModeFlags(0));

      if IsFocused and IsDefault then
      begin
        R := ClientRect;
        InflateRect(R, -4, -4);
        FCanvas.Pen.Color := clWindowFrame;
        FCanvas.Brush.Color := clBtnFace;
        DrawFocusRect(FCanvas.Handle, R);
      end;
    end;
    FCanvas.Handle := 0;
  except
    on E: EAbortPaint do
      ;
    else
      raise;
  end;
end;

procedure TTntBitBtn.CMMouseEnter(var Message: TMessage);
begin
  FMouseInControl := True;
  inherited;
end;

procedure TTntBitBtn.CMMouseLeave(var Message: TMessage);
begin
  FMouseInControl := False;
  inherited;
end;

procedure TTntBitBtn.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntBitBtn.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

end.
