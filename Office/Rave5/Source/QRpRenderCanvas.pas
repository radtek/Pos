{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpRenderCanvas;

interface

{$I RpVer.inc}
{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}

uses
  Windows,
  Qt, Types, QGraphics,
  Classes, SysUtils, QRpRender;

type
  TRvRenderCanvas = class(TRpRenderStream)
  protected
    CurrFont: Windows.HFont;

    procedure SetBKColor(Value: TColor); override;
    function GetCanvas: TCanvas; virtual; abstract;
    procedure PrintAlign(Text: string; X1, Y1: double; Flags: integer);
    procedure PrintRightWidth(var X1,Y1: double; Text: string; Width: double); override;
    function CanvasTextWidth(const Text: string): double; virtual;
    procedure FrameRect(const Rect: TRect); override;
    function CreateFont: Windows.HFont;

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
    procedure PrintSpaces(const Text: string; const X1, Y1, AWidth: double); override;
    procedure Rectangle(const X1, Y1, X2, Y2: double); override;
    procedure FillRect(const Rect: TRect); override;
    procedure TextRect(Rect: TRect; X1, Y1: double; S1: string); override;
    procedure RoundRect(const X1, Y1, X2, Y2, X3, Y3: double); override;
    procedure BrushCopy(const Dest: TRect;
                                        Bitmap: TBitmap;
                                  const Source: TRect;
                                        Color: TColor); override;
    procedure CopyRect(const Dest: TRect;
                             Canvas: TCanvas;
                       const Source: TRect); override;
    procedure DrawFocusRect(const Rect: TRect); override;

  // Status Update Methods
    procedure DocBegin; override;
    procedure DocEnd; override;
    procedure PageBegin; override;
    procedure PageEnd; override;

  public
    constructor Create(AOwner: TComponent); override;
    property Canvas: TCanvas read GetCanvas;

  published
  end;

implementation

uses
  QRpDefine, QRpRPTF, QRpMBCS;

{ TRvRenderCanvas }

procedure TRvRenderCanvas.Arc(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double);
begin
  Canvas.Arc(XI2D(X1),YI2D(Y1),XI2D(X2),YI2D(Y2),XI2D(X3),YI2D(Y3),XI2D(X4),YI2D(Y4));
end;

procedure TRvRenderCanvas.BrushChanged(Sender: TObject);
begin
  Canvas.Brush.Assign(Sender as TBrush);
end;

function TRvRenderCanvas.CanvasTextWidth(const Text: string): double;
var
  CanvasFont: Windows.HFont;
  Size: TSize;
begin
  Canvas.Start;
  CanvasFont := SelectObject(QPainter_Handle(Canvas.Handle),CurrFont);
  Windows.GetTextExtentPoint32(QPainter_Handle(Canvas.Handle),
   PAnsiChar(Text),Length(Text),Size);
  Result := Size.cX / XDPI;
  SelectObject(QPainter_Handle(Canvas.Handle),CanvasFont);
  Canvas.Stop;
end;

procedure TRvRenderCanvas.CenterText(const Text: string; const X1, Y1: double);
begin
  PrintAlign(Text,X1,Y1,TA_CENTER);
end;

procedure TRvRenderCanvas.Chord(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double);
begin
  Canvas.Chord(XI2D(X1),YI2D(Y1),XI2D(X2),YI2D(Y2),XI2D(X3),YI2D(Y3),XI2D(X4),YI2D(Y4));
end;

constructor TRvRenderCanvas.Create(AOwner: TComponent);
begin
  inherited;
end;

function TRvRenderCanvas.CreateFont: Windows.HFont;
var
  LogFont: TLogFont;
begin
  with LogFont do begin
    if Converter.FontData.Superscript or Converter.FontData.Subscript then begin
      lfHeight := YI2D(0) - YI2D(Converter.FontData.Height / 1.667);
    end else begin
      lfHeight := YI2D(0) - YI2D(Converter.FontData.Height);
    end; { else }
    lfWidth := 0;
    lfEscapement := Converter.FontData.Rotation * 10;
    lfOrientation := 0;
    if Converter.FontData.Bold then begin
      lfWeight := FW_BOLD;
    end else begin
      lfWeight := FW_NORMAL;
    end; { else }
    lfItalic := byte(Converter.FontData.Italic);
    lfUnderline := byte(Converter.FontData.Underline);
    lfStrikeOut := byte(Converter.FontData.StrikeOut);
    lfCharSet := ConvertCharset(Converter.FontData.Charset);
    StrPCopy(lfFaceName,Converter.FontData.Name);
    lfQuality := DEFAULT_QUALITY;
    lfOutPrecision := OUT_DEFAULT_PRECIS;
    lfClipPrecision := CLIP_DEFAULT_PRECIS;
    case Converter.FontData.Pitch of
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
end;

procedure TRvRenderCanvas.DocBegin;
begin
  inherited;
end;

procedure TRvRenderCanvas.DocEnd;
begin
  inherited;
  if CurrFont <> 0 then begin
    DeleteObject(CurrFont);
    CurrFont := 0;
  end; { if }
end;

procedure TRvRenderCanvas.Draw(const X1, Y1: double; Graphic: TGraphic);
begin
  Canvas.Draw(XI2D(X1),YI2D(Y1),Graphic);
end;

procedure TRvRenderCanvas.Ellipse(const X1, Y1, X2, Y2: double);
var
  PA,PA2: integer;
  LX1,LX2,LY1,LY2: double;
begin
  if X1 > X2 then begin
    LX1 := X2;
    LX2 := X1;
  end else begin
    LX1 := X1;
    LX2 := X2;
  end; { else }
  if Y1 > Y2 then begin
    LY1 := Y2;
    LY2 := Y1;
  end else begin
    LY1 := Y1;
    LY2 := Y2;
  end; { else }
  if Canvas.Pen.Style = psClear then begin
    PA := 0;
    PA2 := 0;
  end else if (FrameMode = fmSplit) then begin
    PA := 0;
    PA2 := 1;
  end else if (FrameMode = fmInside) then begin
    PA := Canvas.Pen.Width div 2;
    PA2 := Canvas.Pen.Width - PA;
  end else begin { FrameMode = fmOutside }
    PA := -Canvas.Pen.Width div 2;
    PA2 := PA - Canvas.Pen.Width;
  end; { else }
  Canvas.Ellipse(XI2D(LX1) + PA,YI2D(LY1) + PA,XI2D(LX2) - PA2 + 2,YI2D(LY2) - PA2 + 2);
end;

procedure TRvRenderCanvas.FillRect(const Rect: TRect);
begin
  Canvas.FillRect(Rect);
end;

procedure TRvRenderCanvas.FontChanged(Sender: TObject);
begin
  DeleteObject(CurrFont);
  CurrFont := CreateFont;
end;

procedure TRvRenderCanvas.LeftText(const Text: string; const X1, Y1: double);
begin
  PrintAlign(Text,X1,Y1,TA_LEFT);
end;

procedure TRvRenderCanvas.LineTo(const X1, Y1: double);
begin
  Canvas.LineTo(XI2D(X1),YI2D(Y1));
end;

procedure TRvRenderCanvas.MoveTo(const X1, Y1: double);
begin
  Canvas.MoveTo(XI2D(X1),YI2D(Y1));
end;

procedure TRvRenderCanvas.PageBegin;
begin
end;

procedure TRvRenderCanvas.PageEnd;
begin
end;

procedure TRvRenderCanvas.PenChanged(Sender: TObject);
begin
  Canvas.Pen.Assign(Sender as TPen);
  Canvas.Pen.Width := (XI2D(PenWidth) - XI2D(0));
end;

procedure TRvRenderCanvas.Pie(const X1, Y1, X2, Y2, X3, Y3, X4, Y4: double);
begin
  Canvas.Pie(XI2D(X1),YI2D(Y1),XI2D(X2),YI2D(Y2),XI2D(X3),YI2D(Y3),XI2D(X4),YI2D(Y4));
end;

procedure TRvRenderCanvas.Polygon(const PolyLineArr: array of TFloatPoint);
var
  I1, Cnt: integer;
  PolyLineArr2: array of TPoint;
begin
  Cnt := Length(PolyLineArr);
  SetLength(PolyLineArr2,Cnt);
  for I1 := 0 to Cnt - 1 do begin
    PolyLineArr2[I1].X := XI2D(PolyLineArr[I1].X);
    PolyLineArr2[I1].Y := YI2D(PolyLineArr[I1].Y);
  end; { for }
  Canvas.Polygon(PolyLineArr2);
end;

procedure TRvRenderCanvas.PolyLine(const PolyLineArr: array of TFloatPoint);
var
  I1, Cnt: integer;
  PolyLineArr2: array of TPoint;
begin
  Cnt := Length(PolyLineArr);
  SetLength(PolyLineArr2,Cnt);
  for I1 := 0 to Cnt - 1 do begin
    PolyLineArr2[I1].X := XI2D(PolyLineArr[I1].X);
    PolyLineArr2[I1].Y := YI2D(PolyLineArr[I1].Y);
  end; { for }
  Canvas.PolyLine(PolyLineArr2);
end;

procedure TRvRenderCanvas.PrintAlign(Text: string;
                                     X1, Y1: double;
                                     Flags: integer);
var
  OutFlags: integer;
//  Format: longint;
  CanvasFont: Windows.HFont;
begin { PrintAlign }
  OutFlags := Flags or TA_NOUPDATECP or TA_BASELINE;
//  Format := DT_LEFT or DT_TOP or DT_SINGLELINE or DT_NOCLIP or DT_NOPREFIX;





(*!!!
  SelectRPFont;
  if BaseFont.Superscript then begin
  {$IFDEF RaveVCL}
    OutFlags := OutFlags or TA_TOP;
  {$ENDIF}
    Y1 := YI2D(YU2I((FontTop + LineTop) / 2.0));
  end else if BaseFont.Subscript then begin
  {$IFDEF RaveVCL}
    OutFlags := OutFlags or TA_BOTTOM;
  {$ENDIF}
    Y1 := YU2D(FontBottom) + 1;
  end else begin
    case BaseFont.Align of
      faTop: begin
      {$IFDEF RaveVCL}
        OutFlags := OutFlags or TA_TOP;
      {$ENDIF}
        Y1 := YI2D(YU2I(FontTop) - BaseFont.InternalLeading);
      end;
      faBaseline: begin
      {$IFDEF RaveVCL}
        OutFlags := OutFlags or TA_BASELINE;
      {$ENDIF}
        Y1 := YI2D(FontBaseline) + 1;
      end;
      else begin { faBottom }
      {$IFDEF RaveVCL}
        OutFlags := OutFlags or TA_BOTTOM;
      {$ENDIF}
        Y1 := YU2D(FontBottom) + 1;
      end;
    end; { case }
  end; { else }
*)

  if Text <> '' then begin
    Canvas.Start;
    CanvasFont := SelectObject(QPainter_Handle(Canvas.Handle),CurrFont);
    SetTextColor(QPainter_Handle(Canvas.Handle),ColorToRGB(Converter.FontData.Color));
    SetTextAlign(QPainter_Handle(Canvas.Handle),OutFlags);
    SetBKMode(QPainter_Handle(Canvas.Handle),Transparent);
    Windows.ExtTextOut(QPainter_Handle(Canvas.Handle),XI2D(X1),YI2D(Y1),0,nil,PAnsiChar(Text),Length(Text),nil);
    SelectObject(QPainter_Handle(Canvas.Handle),CanvasFont);
    Canvas.Stop;
  end; { if }
//SelectCanvasFont;

(*
  case Flags of
    TA_LEFT: begin
      XPos := X1;
      AdjustXY(XPos,YPos,CanvasTextWidth(Text));
    end;
    TA_CENTER: begin
      XPos := X1;
      AdjustXY(XPos,YPos,CanvasTextWidth(Text) / 2.0);
    end;
    TA_RIGHT: begin
      XPos := X1;
    end;
  end; { case }
*)
end;  { PrintAlign }

procedure TRvRenderCanvas.PrintBitmap(const X1, Y1, ScaleX, ScaleY: double; Graphic: TBitmap);
begin
 //!!!
  Canvas.Draw(XI2D(X1),YI2D(Y1),Graphic);
end;

procedure TRvRenderCanvas.PrintBitmapRect(const X1, Y1, X2, Y2: double; Graphic: TBitmap);
begin
 //!!!
  Canvas.StretchDraw(Rect(XI2D(X1),YI2D(Y1),XI2D(X2),YI2D(Y2)),Graphic);
end;

procedure TRvRenderCanvas.PrintRightWidth(var X1, Y1: double; Text: string; Width: double);
begin
{ Print Text right aligned }
  AdjustXY(X1,Y1,Width);
  PrintAlign(Text,X1,Y1,TA_RIGHT);
end;

procedure TRvRenderCanvas.PrintSpaces(const Text: string; const X1, Y1, AWidth: double);
var
  CurrWord: string;
  Spaces: integer;
  SpaceWidth: double;
  SpacesStr: string;
  I1: integer;
  LText: string;
  XPos, YPos: double;

begin { PrintSpaces }
{ Count the number of spaces in Text }
  LText := Text;
  Spaces := CountSpaces(LText);
  if Spaces = 0 then begin { Process each letter as a word }
  { Insert spaces in between each letter pair }
    I1 := 2;
    while I1 <= Length(LText) do begin
      if (LText[I1 - 1] <> ' ') and (LText[I1] <> ' ') then begin { Insert Space }
        Insert(' ',LText,I1);
        Inc(Spaces);
        Inc(I1);
      end; { if }
      Inc(I1);
    end; { while }
  end; { if }
  if Spaces = 0 then begin
    Spaces := 1;
  end; { if }

{ Figure out the width for each internal space }
  SpaceWidth := (AWidth - CanvasTextWidth(LText)) / Spaces;
  SpaceWidth := SpaceWidth + CanvasTextWidth(' ');

{ Print first word left aligned, middle and last words right aligned }
  CurrWord := GetFirstWord(LText);
  XPos := X1;
  YPos := Y1;
  PrintAlign(CurrWord,XPos,YPos,TA_LEFT);
  AdjustXY(XPos,YPos,CanvasTextWidth(CurrWord));
  while LText <> '' do begin
    SpacesStr := GetSpaces(LText);
    Spaces := Length(SpacesStr);
    CurrWord := GetFirstWord(LText);
    // Need to print SpacesStr so underlining works
    PrintRightWidth(XPos,YPos,SpacesStr + CurrWord,CanvasTextWidth(CurrWord) + (SpaceWidth * Spaces));
  end; { while }
end;  { PrintSpaces }

procedure TRvRenderCanvas.Rectangle(const X1, Y1, X2, Y2: double);
var
  PA,PA2: integer;
  LX1,LX2,LY1,LY2: double;
begin
  if X1 > X2 then begin
    LX1 := X2;
    LX2 := X1;
  end else begin
    LX1 := X1;
    LX2 := X2;
  end; { else }
  if Y1 > Y2 then begin
    LY1 := Y2;
    LY2 := Y1;
  end else begin
    LY1 := Y1;
    LY2 := Y2;
  end; { else }
  if Canvas.Pen.Style = psClear then begin
    PA := 0;
    PA2 := 0;
  end else if (FrameMode = fmSplit) then begin
    PA := 0;
    PA2 := 1;
  end else if (FrameMode = fmInside) then begin
    PA := Canvas.Pen.Width div 2;
    PA2 := Canvas.Pen.Width - PA;
  end else begin { FrameMode = fmOutside }
    PA := -Canvas.Pen.Width div 2;
    PA2 := PA - Canvas.Pen.Width;
  end; { else }
  Canvas.Rectangle(XI2D(LX1) + PA,YI2D(LY1) + PA,XI2D(LX2) - PA2 + 2,YI2D(LY2) - PA2 + 2);
end;

procedure TRvRenderCanvas.RightText(const Text: string; const X1, Y1: double);
begin
  PrintAlign(Text,X1,Y1,TA_RIGHT);
end;

procedure TRvRenderCanvas.RoundRect(const X1, Y1, X2, Y2, X3, Y3: double);
begin
  Canvas.RoundRect(XI2D(X1),YI2D(Y1),XI2D(X2),YI2D(Y2),Round(X3 * XDPI),Round(Y3 * YDPI));
end;

procedure TRvRenderCanvas.BrushCopy(const Dest: TRect;
                                        Bitmap: TBitmap;
                                  const Source: TRect;
                                        Color: TColor);

begin { BrushCopy }
end;  { BrushCopy }

procedure TRvRenderCanvas.CopyRect(const Dest: TRect;
                                       Canvas: TCanvas;
                                 const Source: TRect);
begin { CopyRect }
end;  { CopyRect }

procedure TRvRenderCanvas.DrawFocusRect(const Rect: TRect);
begin { DrawFocusRect }
  Canvas.DrawFocusRect(Rect);
end;  { DrawFocusRect }

procedure TRvRenderCanvas.SetBKColor(Value: TColor);
begin
end;

procedure TRvRenderCanvas.FrameRect(const Rect: TRect);

begin { FrameRect }
end;  { FrameRect }


procedure TRvRenderCanvas.StretchDraw(const Rect: TRect; Graphic: TGraphic);
begin
  Canvas.StretchDraw(Rect,Graphic);
end;

procedure TRvRenderCanvas.TextRect(Rect: TRect; X1, Y1: double; S1: string);
begin
//!!PORT!! RPCANVAS01
end;

end.