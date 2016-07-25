{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpFont;

interface

uses
  Qt, QGraphics,
  Classes,
  QRpDefine,
  SyncObjs, SysUtils;

const
  BaseSize = 600;

type
  TRPBaseFontHandler = class
  public
    constructor Create(Font: TFont); virtual;
    function TextWidth(Value: string;
                       FontSize: double;
                       DPI: integer): integer; virtual; abstract;
  end; { TRPBaseFontHandler }

  TRPTrueTypeFontHandler = class(TRPBaseFontHandler)
  protected
    FFontTable: array[0..255] of integer;
  public
    constructor Create(Font: TFont); override;
    function TextWidth(Value: string;
                       FontSize: double;
                       DPI: integer): integer; override;
  end; { TRPTrueTypeFontHandler }

  TRPNonProportionalFontHandler = class(TRPBaseFontHandler)
  protected
    FFontWidth: integer;
  public
    constructor Create(Font: TFont); override;
    function TextWidth(Value: string;
                       FontSize: double;
                       DPI: integer): integer; override;
  end; { TRPNonProportionalFontHandler }

  TRPFontManager = class
  protected
    FFontList: TStringList;
    FDPI: integer;

    function FontNameOf(Font: TFont): string;
    function GetFontHandler(Font: TFont): TRPBaseFontHandler;
  public
    constructor Create; virtual;
    destructor Destroy; override;
    function TextWidth(Font: TFont;
                       Value: string): integer;
  end; { TRPFontManager }

var
  FontManager: TRPFontManager;

implementation

//uses
//  {Q}RvDefine;

{ class TRPBaseFontHandler }

constructor TRPBaseFontHandler.Create(Font: TFont);
begin { Create }
end;  { Create }

{ class TRPTrueTypeFontHandler }

constructor TRPTrueTypeFontHandler.Create(Font: TFont);
var
  i: integer;
  Bitmap: TBitmap;
begin
  inherited Create(Font);
  Bitmap := TBitmap.Create;
  try
    Bitmap.Width := 200;
    Bitmap.Height := 200;
    Bitmap.Canvas.Font.Assign(Font);
    Bitmap.Canvas.Font.Height := -BaseSize;
    for i := 0 to 255 do begin
      FFontTable[i] := Bitmap.Canvas.TextWidth(Chr(i));
    end;
  finally
    Bitmap.Free;
  end;
end;

function TRPTrueTypeFontHandler.TextWidth(Value: string;
                                          FontSize: double;
                                          DPI: integer): integer;
var
  I1: integer;
begin { TextWidth }
  Result := 0;
  for I1 := 1 to Length(Value) do begin
    Result := Result + FFontTable[Ord(Value[I1])];
  end; { for }
  Result := Round((Result * FontSize * DPI) / (72 * BaseSize));
end;  { TextWidth }

{ class TRPNonProportionalFontHandler }

constructor TRPNonProportionalFontHandler.Create(Font: TFont);
begin
  //!!PORT!! #116 Need to port this RPFONT01
  inherited Create(Font);
  raise Exception.Create('NonProportionalFontHandler.Create not implemented');
end;

function TRPNonProportionalFontHandler.TextWidth(Value: string;
                                                 FontSize: double;
                                                 DPI: integer): integer;
begin { TextWidth }
  Result := Round((Length(Value) * FFontWidth * FontSize * DPI) / (72 * BaseSize));
end;  { TextWidth }

{ class TRPFontManager }

constructor TRPFontManager.Create;
begin { Create }
  inherited Create;
  FFontList := TStringList.Create;
  FFontList.Sorted := true;
end;  { Create }

destructor TRPFontManager.Destroy;
var
  I1: integer;
begin { Destroy }
  for I1 := 0 to FFontList.Count - 1 do begin
    TRPBaseFontHandler(FFontList.Objects[I1]).Free;
  end; { for }
  FreeAndNil(FFontList);
  inherited Destroy;
end;  { Destroy }

function TRPFontManager.FontNameOf(Font: TFont): string;
begin { FontNameOf }
  Result := Font.Name + '|' + IntToStr(Ord(Font.CharSet));
  if fsBold in Font.Style then begin
    Result := Result + '|B';
  end; { if }
  if fsItalic in Font.Style then begin
    Result := Result + '|I';
  end; { if }
  case Font.Pitch of
    fpVariable: Result := Result + '|V';
    fpFixed: Result := Result + '|F';
  end; { case }
end;  { FontNameOf }

var
  CS: TCriticalSection;

function TRPFontManager.GetFontHandler(Font: TFont): TRPBaseFontHandler;
var
  Index: integer;
  QFontI: QFontInfoH;
begin { GetFontHandler }
// Find FontHandler for current font
// Use a critical section since Index may be invalid if a new font handler is
// inserted into the sorted FFontList between the Find() and the [Index] calls
  CS.Enter;
  try
    if FFontList.Find(FontNameOf(Font),Index) then begin // Found
      Result := TRPBaseFontHandler(FFontList.Objects[Index]);
      Exit;
    end; { if }
  finally
    CS.Leave;
  end; { tryf }

// Create font handler since one wasn't found
  
  QFontI := QFontInfo_create(Font.Handle);
  try
    //!!PORT!! #117 what about Type1 fonts? This is only Windows platform RPFONT02
    if QFontInfo_fixedPitch(QFontI) then begin
      Result := TRPNonProportionalFontHandler.Create(Font);
    end else begin
      Result := TRPTrueTypeFontHandler.Create(Font);
    end;
    if Assigned(Result) then begin
      FFontList.AddObject(FontNameOf(Font), Result);
    end;
  finally
    QFontInfo_destroy(QFontI);
  end;
end;  { GetFontHandler }

function TRPFontManager.TextWidth(Font: TFont;
                                  Value: string): integer;
begin { TextWidth }
  if Font.PixelsPerInch = 0 then begin
//    RaveError('Division by 0 would occur. PixelsPerInch is 0');
  end;
  Result := GetFontHandler(Font).TextWidth(Value,
   (72 * Abs(Font.Height)) / Font.PixelsPerInch,Font.PixelsPerInch);
end;  { TextWidth }

initialization
  CS := TCriticalSection.Create;
  FontManager := TRPFontManager.Create;
finalization
  FreeAndNil(FontManager);
  FreeAndNil(CS);
end.
