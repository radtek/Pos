{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpCanvas;

interface

{$I RpVer.inc}

uses
  Windows, Messages, Graphics, Controls, Forms, Dialogs, StdCtrls, ExtCtrls,
  SysUtils, Classes, RpDefine, RpBase, RpDevice, RpRPTF;

type
{*************************************************************************}
{                            class TCanvasReport                          }
{*************************************************************************}

  TCanvasReport = class(TBaseReport)
  protected
    procedure PrintAlign(Text: string;
                         Pos: double;
                         Flags: integer);
    procedure PrintRightWidth(Text: string;
                              Width: double);

  { Abstract Output Methods - protected }
    procedure PrintCenterInch(Text: string;
                              Pos: double); override;
    procedure PrintLeftInch(Text: string;
                            Pos: double); override;
    procedure PrintRightInch(Text: string;
                             Pos: double); override;
    procedure PrintBlockInch(Text: string;
                             Pos: double;
                             Width: double); override;
  public
  { Deprecated output methods }
  {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
  {$IFDEF LEVEL6}{$WARN SYMBOL_LIBRARY OFF}{$ENDIF}
    procedure BrushCopy(const Dest: TRect;
                              Bitmap: TBitmap;
                        const Source: TRect;
                              Color: TColor); override; {$IFDEF LEVEL6}deprecated; library;{$ENDIF}
    procedure FloodFill(X,Y: double;
                        Color: TColor;
                        FillStyle: TFillStyle); override; {$IFDEF LEVEL6}deprecated; library;{$ENDIF}
    procedure FrameRect(const Rect: TRect); override; {$IFDEF LEVEL6}deprecated; library;{$ENDIF}
  {$IFDEF LEVEL6}{$WARN SYMBOL_LIBRARY ON}{$ENDIF}
    procedure CopyRect(const Dest: TRect;
                             Canvas: TCanvas;
                       const Source: TRect); override; {$IFDEF LEVEL6}deprecated;{$ENDIF}
    procedure DrawFocusRect(const Rect: TRect); override; {$IFDEF LEVEL6}deprecated;{$ENDIF}
    procedure FillRect(const Rect: TRect); override; {$IFDEF LEVEL6}deprecated;{$ENDIF}
  {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED ON}{$ENDIF}

  { Output Methods - override as public }
    procedure Arc(X1,Y1,X2,Y2,X3,Y3,X4,Y4: double); override;
    procedure Chord(X1,Y1,X2,Y2,X3,Y3,X4,Y4: double); override;
    procedure Draw(X,Y: double;
                    Graphic: TGraphic); override;
    procedure Ellipse(X1,Y1,X2,Y2: double); override;
    procedure LineTo(X,Y: double); override;
    procedure MoveTo(X,Y: double); override;
    procedure Pie(X1,Y1,X2,Y2,X3,Y3,X4,Y4: double); override;
    procedure Polygon(const Points: array of TPoint); override;
    procedure Polyline(const Points: array of TPoint); override;
    procedure PrintBitmap(X,Y: double;
                          ScaleX,ScaleY: double;
                          Bitmap: TBitmap); override;
    procedure PrintBitmapRect(X1,Y1,X2,Y2: double;
                              Bitmap: TBitmap); override;
    procedure PrintImageRect(X1,Y1,X2,Y2: double;
                             ImageStream: TStream;
                             ImageType: string); override;
    procedure Rectangle(X1,Y1,X2,Y2: double); override;
    procedure RoundRect(X1,Y1,X2,Y2,X3,Y3: double); override;
    procedure StretchDraw(const Rect: TRect;
                                Graphic: TGraphic); override;
    procedure TabRectangle(X1,Y1,X2,Y2: double); override;
    procedure TextRect(      Rect: TRect;
                             X,Y: double;
                       const Text: string); override;
    procedure PrintData(Value: string); override;
    procedure PrintDataStream(Stream: TStream;
                              BufSize: longint); override;
    procedure NewPage; override;
  end; { TCanvasReport }

implementation

uses
  RpMBCS;

{ Misc functions }

function GetFirstWord(var Line: string): string;
var
  LFound: Boolean;
  LSpacePos: integer;
  LNext: PChar;
begin
  Result := '';
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

function GetSpaces(var Line: string): string;
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

function CountSpaces(Text: string): integer;
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

{*************************************************************************}
{                            class TCanvasReport                          }
{*************************************************************************}

{ Output routines }


procedure TCanvasReport.PrintAlign(Text: string;
                                   Pos: double;
                                   Flags: integer);
var
  Y1: longint;
  R1: TRect;
  OutFlags: integer;
  BkMode: integer;
  Format: longint;
begin { PrintAlign }
  BKMode := SetBkMode(Canvas.Handle,Ord(FTextBKMode) + 1);
  OutFlags := Flags or TA_NOUPDATECP;
  Format := DT_LEFT or DT_TOP or DT_SINGLELINE or DT_NOCLIP or DT_NOPREFIX;
  SelectRPFont;
  if BaseFont.Superscript then begin
    OutFlags := OutFlags or TA_TOP;
    Y1 := YI2D(YU2I((FontTop + LineTop) / 2.0));
  end else if BaseFont.Subscript then begin
    OutFlags := OutFlags or TA_BOTTOM;
    Y1 := YU2D(FontBottom) + 1;
  end else begin
    case BaseFont.Align of
      faTop: begin
        OutFlags := OutFlags or TA_TOP;
        Y1 := YI2D(YU2I(FontTop) - BaseFont.InternalLeading);
      end;
      faBaseline: begin
        OutFlags := OutFlags or TA_BASELINE;
        Y1 := YU2D(FontBaseline) + 1;
      end;
      else begin { faBottom }
        OutFlags := OutFlags or TA_BOTTOM;
        Y1 := YU2D(FontBottom) + 1;
      end;
    end; { case }
  end; { else }

  if Text <> '' then begin
    SetTextAlign(Canvas.Handle,OutFlags);
    if UseDrawText then begin
      R1.Left := XI2D(Pos);
      R1.Top := Y1;
      R1.Right := R1.Left + 10000;
      R1.Bottom := R1.Top + 10000;
    // !!! Temporary. This disables the wide string support.
      Windows.DrawText(Canvas.Handle,PChar(string(Text)),Length(Text),R1,Format);
//      Windows.DrawText(Canvas.Handle,@Text[1],Length(Text),R1,Format);
    end else begin
    // !!! Temporary. This disables the wide string support.
      Windows.TextOut(Canvas.Handle,XI2D(Pos),Y1,PChar(string(Text)),Length(Text));
//      Windows.TextOut(Canvas.Handle,XI2D(Pos),Y1,@Text[1],Length(Text));
    end; { else }
  end; { if }
  SelectCanvasFont;
  SetBkMode(Canvas.Handle,BkMode);

  case Flags of
    TA_LEFT: begin
      FCursorXPos := Pos;
      AdjustXY(TextWidth(Text));
    end;
    TA_CENTER: begin
      FCursorXPos := Pos;
      AdjustXY(TextWidth(Text) / 2.0);
    end;
    TA_RIGHT: begin
      FCursorXPos := Pos;
    end;
  end; { case }
end;  { PrintAlign }

procedure TCanvasReport.PrintCenterInch(Text: string;
                                        Pos: double);
begin { PrintCenterInch }
  CheckPrinting;
  if OutputInvalid then Exit;

  if IgnoreRPTF or (System.Pos(RPTFPrefix,Text) = 0) then begin
    PrintAlign(Text,Pos,TA_CENTER);
  end else begin
    PrintLeftInch(Text,Pos - (RPTFTextWidth(Self,Text) / ((FZoom / 100.0) * XDPI * 2.0)));
  end; { else }
end;  { PrintCenterInch }

procedure TCanvasReport.PrintLeftInch(Text: string;
                                      Pos: double);
var
  I1: longint;
  S1: string;
begin { PrintLeftInch }
  CheckPrinting;
  if OutputInvalid then Exit;
  if IgnoreRPTF or (System.Pos(RPTFPrefix,Text) = 0) then begin
    PrintAlign(Text,Pos,TA_LEFT);
  end else begin
    I1 := 1;
    while I1 <= Length(Text) do begin
      S1 := NextPiece(Self,Text,I1);
      PrintAlign(S1,Pos,TA_LEFT);
      Pos := Pos + XU2I(TextWidth(S1)) * (100.0 / FZoom);
    end; { while }
  end; { else }
end;  { PrintLeftInch }

procedure TCanvasReport.PrintRightInch(Text: string;
                                       Pos: double);
begin { PrintRightInch }
  CheckPrinting;
  if OutputInvalid then Exit;
  if IgnoreRPTF or (System.Pos(RPTFPrefix,Text) = 0) then begin
    PrintAlign(Text,Pos,TA_RIGHT);
  end else begin
    PrintLeftInch(Text,Pos - (RPTFTextWidth(Self,Text) / XDPI) * (100.0 / FZoom));
  end; { else }
end;  { PrintRightInch }

procedure TCanvasReport.PrintRightWidth(Text: string; Width: double);
var
  LTextWidth: double;
  Spaces: integer;
  SpaceStr: string;
  SpaceWidth: double;
begin { PrintRightWidth }
{ Print a block of spaces to fill the front space width }
  PushPos;
  SpaceWidth := Width - TextWidth(Text);
  LTextWidth := TextWidth(' ');
  Spaces := 0;
  if LTextWidth > 0 then begin
    Spaces := Trunc(SpaceWidth / LTextWidth);
  end; { if }
  if Spaces > 0 then begin
    SetLength(SpaceStr,Spaces);
    Spaces := Length(SpaceStr);
    FillChar(SpaceStr[1],Spaces,' ');
    PushPos;
    PrintAlign(SpaceStr,FCursorXPos,TA_LEFT);
    PopPos;
  end; { if }
  AdjustXY(SpaceWidth);
  PrintAlign(' ',FCursorXPos,TA_RIGHT);
  PopPos;

{ Print Text right aligned }
  AdjustXY(Width);
  PrintAlign(Text,FCursorXPos,TA_RIGHT);
end;  { PrintRightWidth }

procedure TCanvasReport.PrintBlockInch(Text: string;
                                       Pos: double;
                                       Width: double);
var
  CurrWord: string;
  Spaces: integer;
  SpaceWidth: double;
  I1: integer;
begin { PrintBlockInch }
  CheckPrinting;
  if OutputInvalid then Exit;

{ Count the number of spaces in Text }
  Spaces := CountSpaces(Text);
  if Spaces = 0 then begin { Process each letter as a word }
  { Insert spaces in between each letter pair }
    I1 := 2;
    while I1 <= Length(Text) do begin
      if (Text[I1 - 1] <> ' ') and (Text[I1] <> ' ') then begin { Insert Space }
        Insert(' ',Text,I1);
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
  SpaceWidth := TextWidth(' ') + ((((XI2U(Width) * FScaleX * FZoom) / 10000.0) -
   TextWidth(Text)) / Spaces);

{ Print first word left aligned, middle and last words right aligned }
  CurrWord := GetFirstWord(Text);
  PrintAlign(CurrWord,Pos,TA_LEFT);
  while Text <> '' do begin
    Spaces := Length(GetSpaces(Text));
    CurrWord := GetFirstWord(Text);
    PrintRightWidth(CurrWord,TextWidth(CurrWord) + (Spaces * SpaceWidth));
  end; { while }
end;  { PrintBlockInch }

procedure TCanvasReport.PrintData(Value: string);
begin { PrintData }
  CheckPrinting;
  if OutputInvalid or (Value = '') then Exit;
  LocalDevice.RawOut(Value[1],Length(Value));
end;  { PrintData }

procedure TCanvasReport.PrintDataStream(Stream: TStream;
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
  CheckPrinting;
  if OutputInvalid then Exit;
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
        LocalDevice.RawOut(Buffer^.Data,Buffer^.Size);
      end; { if }
    end; { for }
  finally
    Dispose(Buffer);
  end; { tryf }
end;  { PrintDataStream }

{ Deprecated Output Commands }

{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
{$IFDEF LEVEL6}{$WARN SYMBOL_LIBRARY OFF}{$ENDIF}
procedure TCanvasReport.BrushCopy(const Dest: TRect;
                                        Bitmap: TBitmap;
                                  const Source: TRect;
                                        Color: TColor);

begin { BrushCopy }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.BrushCopy(Dest,BitMap,Source,Color);
end;  { BrushCopy }

procedure TCanvasReport.FloodFill(X,Y: double;
                                  Color: TColor;
                                  FillStyle: TFillStyle);

begin { FloodFill }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.FloodFill(XU2D(X),YU2D(Y),Color,FillStyle);
end;  { FloodFill }

procedure TCanvasReport.FrameRect(const Rect: TRect);

begin { FrameRect }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.FrameRect(Rect);
end;  { FrameRect }
{$IFDEF LEVEL6}{$WARN SYMBOL_LIBRARY ON}{$ENDIF}

procedure TCanvasReport.CopyRect(const Dest: TRect;
                                       Canvas: TCanvas;
                                 const Source: TRect);
begin { CopyRect }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.CopyRect(Dest,Canvas,Source);
end;  { CopyRect }

procedure TCanvasReport.DrawFocusRect(const Rect: TRect);
begin { DrawFocusRect }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.DrawFocusRect(Rect);
end;  { DrawFocusRect }

procedure TCanvasReport.FillRect(const Rect: TRect);
begin { FillRect }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.FillRect(Rect);
end;  { FillRect }
{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED ON}{$ENDIF}

{ Output methods }

procedure TCanvasReport.Arc(X1,Y1,X2,Y2,X3,Y3,X4,Y4: double);
begin { Arc }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.Arc(XU2D(X1),YU2D(Y1),XU2D(X2),YU2D(Y2),XU2D(X3),YU2D(Y3),XU2D(X4),YU2D(Y4));
end;  { Arc }

procedure TCanvasReport.Chord(X1,Y1,X2,Y2,X3,Y3,X4,Y4: double);
begin { Chord }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.Chord(XU2D(X1),YU2D(Y1),XU2D(X2),YU2D(Y2),XU2D(X3),YU2D(Y3),XU2D(X4),YU2D(Y4));
end;  { Chord }

procedure TCanvasReport.Draw(X,Y: double;
                             Graphic: TGraphic);
begin { Draw }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.Draw(XU2D(X),YU2D(Y),Graphic);
end;  { Draw }

procedure TCanvasReport.Ellipse(X1,Y1,X2,Y2: double);
var
  F1: double;
  PA,PA2: integer;
begin { Ellipse }
  CheckPrinting;
  if OutputInvalid then Exit;
  if X1 > X2 then begin
    F1 := X1;
    X1 := X2;
    X2 := F1;
  end; { if }
  if Y1 > Y2 then begin
    F1 := Y1;
    Y1 := Y2;
    Y2 := F1;
  end; { if }
  if Canvas.Pen.Style = psClear then begin
    PA := 0;
    PA2 := 0;
  end else if (Canvas.Pen.Style = psInsideFrame) or (FrameMode = fmSplit) then begin
    PA := 0;
    PA2 := 1;
  end else if (FrameMode = fmInside) then begin
    PA := Canvas.Pen.Width div 2;
    PA2 := Canvas.Pen.Width - PA;
  end else begin { FrameMode = fmOutside }
    PA := -Canvas.Pen.Width div 2;
    PA2 := PA - Canvas.Pen.Width;
  end; { else }
  Canvas.Ellipse(XU2D(X1) + PA,YU2D(Y1) + PA,XU2D(X2) - PA2 + 2,YU2D(Y2) - PA2 + 2);
end;  { Ellipse }

procedure TCanvasReport.LineTo(X,Y: double);
var
  SaveX, SaveY: double;
begin { LineTo }
  CheckPrinting;
  if OutputInvalid then Exit;
  SaveX := LastLineX;
  SaveY := LastLineY;
  if NoBufferLine then begin
    Canvas.LineTo(XU2D(X),YU2D(Y));
  end else if not BufferLine(X,Y) then begin
    Canvas.MoveTo(XU2D(SaveX - OriginX),YU2D(SaveY - OriginY));
    Canvas.LineTo(XU2D(X),YU2D(Y));
  end; { else }
end;  { LineTo }

procedure TCanvasReport.MoveTo(X,Y: double);
begin { MoveTo }
  CheckPrinting;
  if OutputInvalid then Exit;
  if NoBufferLine then begin
    Canvas.MoveTo(XU2D(X),YU2D(Y));
  end; { if }
  LastLineX := X + OriginX;
  LastLineY := Y + OriginY;
end;  { MoveTo }

procedure TCanvasReport.Pie(X1,Y1,X2,Y2,X3,Y3,X4,Y4: double);
begin { Pie }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.Pie(XU2D(X1),YU2D(Y1),XU2D(X2),YU2D(Y2),XU2D(X3),YU2D(Y3),XU2D(X4),
   YU2D(Y4));
end;  { Pie }

procedure TCanvasReport.Polygon(const Points: array of TPoint);
begin { Polygon }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.Polygon(Points);
end;  { Polygon }

procedure TCanvasReport.Polyline(const Points: array of TPoint);
begin { Polyline }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.Polyline(Points);
end;  { Polyline }

procedure TCanvasReport.PrintBitmap(X,Y: double;
                                    ScaleX,ScaleY: double;
                                    Bitmap: TBitmap);
var
  Info: PBitmapInfo;
  InfoSize: DWORD;
  ImageSize: DWORD;
  Image: pointer;
  ImageHandle: THandle;
  DrawMode: longint;
begin { PrintBitmap }
  CheckPrinting;
  if OutputInvalid then Exit;
  with Bitmap do begin
    GetDIBSizes(Handle,InfoSize,ImageSize);
    GetMem(Info,InfoSize);
    try
      if ImageSize < 65535 then begin
        GetMem(Image,ImageSize);
        ImageHandle := 0;
      end else begin
        ImageHandle := GlobalAlloc(GMEM_MOVEABLE,ImageSize);
        Image := GlobalLock(ImageHandle);
      end; // else
      try
        GetDIB(Handle,Palette,Info^,Image^);
        with Info^.bmiHeader do begin
          if FTransparentBitmaps then begin
            DrawMode := SRCAND;
          end else begin
            DrawMode := SRCCOPY;
          end; { else }
          StretchDIBits(self.Canvas.Handle,XU2D(X),YU2D(Y),
           Round(Width * ScaleX),Round(Height * ScaleY),
           0,0,biWidth,biHeight,Image,Info^,DIB_RGB_COLORS,DrawMode);
        end; { with }
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
  end; { with }
end;  { PrintBitmap }

procedure TCanvasReport.PrintBitmapRect(X1,Y1,X2,Y2: double;
                                        Bitmap: TBitmap);
var
  Info: PBitmapInfo;
  InfoSize: DWORD;
  ImageSize: DWORD;
  Image: pointer;
  ImageHandle: THandle;
  DrawMode: longint;
begin { PrintBitmapRect }
  CheckPrinting;
  if OutputInvalid then Exit;
  with Bitmap do begin
    GetDIBSizes(Handle,InfoSize,ImageSize);
    GetMem(Info,InfoSize);
    try
      if ImageSize < 65535 then begin
        GetMem(Image,ImageSize);
        ImageHandle := 0;
      end else begin
        ImageHandle := GlobalAlloc(GMEM_MOVEABLE,ImageSize);
        Image := GlobalLock(ImageHandle);
      end; { else }
      try
        GetDIB(Handle,Palette,Info^,Image^);
        with Info^.bmiHeader do begin
          if FTransparentBitmaps then begin
            DrawMode := SRCAND;
          end else begin
            DrawMode := SRCCOPY;
          end; { else }
          StretchDIBits(self.Canvas.Handle,XU2D(X1),YU2D(Y1),
           XU2D(X2) - XU2D(X1),YU2D(Y2) - YU2D(Y1),0,0,
           biWidth,biHeight,Image,Info^,DIB_RGB_COLORS,DrawMode);
        end; { with }
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
  end; { with }
end;  { PrintBitmapRect }

procedure TCanvasReport.PrintImageRect(X1,Y1,X2,Y2: double;
                                       ImageStream: TStream;
                                       ImageType: string);
var
  Bitmap: TBitmap;
begin { PrintImageRect }
  if Assigned(FOnDecodeImage) then begin
    Bitmap := TBitmap.Create;
    try
      FOnDecodeImage(self,ImageStream,ImageType,Bitmap);
      PrintBitmapRect(X1,Y1,X2,Y2,Bitmap);
    finally
      Bitmap.Free;
    end; { tryf }
  end; { if }
end;  { PrintImageRect }

procedure TCanvasReport.Rectangle(X1,Y1,X2,Y2: double);
var
  F1: double;
  PA,PA2: integer;
begin { Rectangle }
  CheckPrinting;
  if OutputInvalid then Exit;
  if X1 > X2 then begin
    F1 := X1;
    X1 := X2;
    X2 := F1;
  end; { if }
  if Y1 > Y2 then begin
    F1 := Y1;
    Y1 := Y2;
    Y2 := F1;
  end; { if }
  if Canvas.Pen.Style = psClear then begin
    PA := 0;
    PA2 := 0;
  end else if (Canvas.Pen.Style = psInsideFrame) or (FrameMode = fmSplit) then begin
    PA := 0;
    PA2 := 1;
  end else if (FrameMode = fmInside) then begin
    PA := Canvas.Pen.Width div 2;
    PA2 := Canvas.Pen.Width - PA;
  end else begin { FrameMode = fmOutside }
    PA := -Canvas.Pen.Width div 2;
    PA2 := PA - Canvas.Pen.Width;
  end; { else }
  Canvas.Rectangle(XU2D(X1) + PA,YU2D(Y1) + PA,XU2D(X2) - PA2 + 2,YU2D(Y2) - PA2 + 2);
end;  { Rectangle }

procedure TCanvasReport.TabRectangle(X1,Y1,X2,Y2: double);
begin { TabRectangle }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.Rectangle(XU2D(X1),YU2D(Y1),XU2D(X2) + 1,YU2D(Y2) + 1);
end;  { TabRectangle }

procedure TCanvasReport.RoundRect(X1,Y1,X2,Y2,X3,Y3: double);
begin { RoundRect }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.RoundRect(XU2D(X1),YU2D(Y1),XU2D(X2),YU2D(Y2),Round(XU2I(X3) *
   (FScaleX * FZoom / 10000.0) * XDPI),Round(YU2I(Y3) * (FScaleY * FZoom /
   10000.0) * YDPI));
end;  { RoundRect }

procedure TCanvasReport.StretchDraw(const Rect: TRect;
                                          Graphic: TGraphic);
begin { StretchDraw }
  CheckPrinting;
  if OutputInvalid then Exit;
  Canvas.StretchDraw(Rect,Graphic);
end;  { StretchDraw }

procedure TCanvasReport.TextRect(      Rect: TRect;
                                       X,Y: double;
                                 const Text: string);
var
  Options: word;
begin { TextRect }
  CheckPrinting;
  if OutputInvalid or (Text = '') then Exit;
  with Canvas do begin
    SelectRPFont;
    SetTextAlign(Handle,TA_LEFT or TA_BASELINE);
    Options := ETO_CLIPPED;
    if Brush.Style <> bsClear then begin
      Options := Options or ETO_OPAQUE;
    end; { if }
    Windows.ExtTextOut(Handle,XU2D(X),YU2D(Y),Options,@Rect,@Text[1],
     Length(Text),nil);
    SelectCanvasFont;
  end; { with }
end;  { TextRect }

procedure TCanvasReport.NewPage;
begin { NewPage }
  inherited NewPage;
  LocalDevice.NewPageSelect(not FPageInvalid);
  InitPrinter;
end;  { NewPage }

end.