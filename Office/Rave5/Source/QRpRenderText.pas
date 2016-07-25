{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpRenderText;

interface

{$I RpVer.inc}
{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}

uses
  Windows,
  Qt, Types, QGraphics,
  Classes, SysUtils, QRpRender, QRpDefine;

type
  PTextItem = ^TTextItem;
  TTextItem = packed record
    Col: integer;
    Line: integer;
    Data: string;
  end; { TTextItem }

  TRvRenderText = class(TRPRenderStream)
  protected
    FCPI: double; { Characters per inch }
    FLPI: double; { Lines per inch }
    PageList: TList; { List of LineLists which are lists of TTextItems }
    FLeftBorder: double; { Spacing on left of page }
    FTopBorder: double; { Spacing on top of page }
    FFormFeed: boolean; { Should FormFeedStr be printed? }
    FFormFeedStr: string; { String to print for form feed }

    function TextWidth(Text: string): double;
    function ColAt(Pos: double): integer;
    function LineAt(Pos: double): integer;
    function MakeSpaces(Count: byte): string;
    procedure ClearPageList;
    procedure AddTextItem(TextItem: PTextItem; Line: integer);
    procedure WriteStr(Value: string);
    procedure PageEnd; override;

    procedure LeftText(const AText: string; const AX, AY: double); override;
    procedure CenterText(const AText: string; const AX, AY: double); override;
    procedure RightText(const AText: string; const AX,AY: double); override;
    procedure PrintSpaces(const AText: string; const AX, AY, AWidth: double); override;
    procedure PrintRightWidth(var AX,AY: double; AText: string; Width: double); override;

  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    function GetTextExtent(Value: string): TSize;
    procedure PrintRender(NDRStream: TStream;
                          OutputFileName: TFileName); override;

    procedure DocBegin; override;
    procedure DocEnd; override;
  { Unsupported drawing functions } (*
    procedure Arc(X1,Y1,X2,Y2,X3,Y3,X4,Y4: double); override;
    procedure BrushCopy(const Dest: TRect;
                              Bitmap: TBitmap;
                        const Source: TRect;
                              Color: TColor); override;
    procedure Chord(X1,Y1,X2,Y2,X3,Y3,X4,Y4: double); override;
    procedure CopyRect(const Dest: TRect;
                             Canvas: TCanvas;
                       const Source: TRect); override;
    procedure Draw(X,Y: double;
                    Graphic: TGraphic); override;
    procedure DrawFocusRect(const Rect: TRect); override;
    procedure Ellipse(X1,Y1,X2,Y2: double); override;
    procedure FillRect(const Rect: TRect); override;
    procedure FloodFill(X,Y: double;
                        Color: TColor;
                        FillStyle: TFillStyle); override;
    procedure FrameRect(const Rect: TRect); override;
    procedure LineTo(X,Y: double); override;
    procedure MoveTo(X,Y: double); override;
    procedure Pie(X1,Y1,X2,Y2,X3,Y3,X4,Y4: double); override;
    procedure Polygon(const Points: array of TFloatPoint); override;
    procedure Polyline(const Points: array of TFloatPoint); override;
    procedure PrintBitmap(X,Y: double;
                          ScaleX,ScaleY: double;
                          Bitmap: TBitmap); override;
    procedure PrintBitmapRect(X1,Y1,X2,Y2: double;
                              Bitmap: TBitmap); override;
    procedure PrintImageRect(X1,Y1,X2,Y2: double;
                             ImageStream: TStream;
                             ImageType: string); override;
    procedure PrintDataStream(Stream: TStream;
                              BufSize: longint); override;
    procedure Rectangle(X1,Y1,X2,Y2: double); override;
    procedure RoundRect(X1,Y1,X2,Y2,X3,Y3: double); override;
    procedure StretchDraw(const Rect: TRect;
                                Graphic: TGraphic); override;
    procedure TabRectangle(X1,Y1,X2,Y2: double); override;
    procedure TextRect(      Rect: TRect;
                             X,Y: double;
                       const Text: string); override;
    *)
  { Public properties }
    property FormFeedStr: string read FFormFeedStr write FFormFeedStr;
  published
    property TopBorder: double read FTopBorder write FTopBorder;
    property LeftBorder: double read FLeftBorder write FLeftBorder;
    property CPI: double read FCPI write FCPI;
    property LPI: double read FLPI write FLPI;
    property FormFeed: boolean read FFormFeed write FFormFeed default true;
  end;


implementation

{ TRvRenderText }

procedure TRvRenderText.AddTextItem(TextItem: PTextItem; Line: integer);
var
  I1: integer;
begin { AddTextItem }
  while PageList.Count < Line do begin { Add LineLists }
    PageList.Add(TList.Create);
  end; { while }
  with TList(PageList[Line - 1]) do begin { Find insert spot }
    for I1 := 0 to (Count - 1) do begin
      if PTextItem(Items[I1])^.Col > TextItem^.Col then begin
        Insert(I1,TextItem);
        Exit;
      end; { if }
    end; { for }
    Add(TextItem);
  end; { with }
end;  { AddTextItem }

procedure TRvRenderText.ClearPageList;
begin
  with PageList do begin
    while Count > 0 do begin
      with TList(Items[Count - 1]) do begin
        while Count > 0 do begin { Delete all TextItems }
          Dispose(PTextItem(Items[Count - 1]));
          Delete(Count - 1);
        end; { while }
        Free; { Free LineList }
      end; { with }
      Delete(Count - 1); { Delete LineList from PageList }
    end; { while }
  end; { with }
end;

function TRvRenderText.ColAt(Pos: double): integer;
begin { ColAt }
  Result := Round((Pos - FLeftBorder) * CPI + 0.4999);
  if Result < 1 then begin
    Result := 1;
  end; { if }
end;  { ColAt }

constructor TRvRenderText.Create(AOwner: TComponent);
begin
  inherited;
  DisplayName := 'Plain Text (TXT)';
  FileExtension := '*.txt';
  PageList := TList.Create;
  FFormFeed := true;
  FFormFeedStr := #12;
  FCPI := 10;
  FLPI := 6;
end;

destructor TRvRenderText.Destroy;
begin
  ClearPageList;
  FreeAndNil(PageList);
  inherited Destroy;
end;

function TRvRenderText.GetTextExtent(Value: string): TSize;
begin

end;

function TRvRenderText.LineAt(Pos: double): integer;
begin { LineAt }
  Result := Round((Pos - FTopBorder) * LPI + 0.4999);
  if Result < 1 then begin
    Result := 1;
  end; { if }
end;  { LineAt }

function TRvRenderText.MakeSpaces(Count: byte): string;
begin { MakeSpaces }
  SetLength(Result,Count);
  if Count > 0 then begin
    FillChar(Result[1],SizeOf(Result[1]) * Count,' ');
  end; { if }
end;  { MakeSpaces }

procedure TRvRenderText.CenterText(const AText: string; const AX, AY: double);
var
  Text: string;
begin
  if AText = '' then Exit;
  Text := StripRPTF(AText);
  LeftText(Text,AX - (TextWidth(Text) / 2.0),AY);
end;

procedure TRvRenderText.LeftText(const AText: string; const AX,AY: double);
var
  TextItem: PTextItem;
begin { LeftText }
{ Create TTextItem }
  New(TextItem);
  with TextItem^ do begin
    Col := ColAt(AX);
    Data := AText;
  end; { with }
  AddTextItem(TextItem,LineAt(AY));
end;  { LeftText }

procedure TRvRenderText.RightText(const AText: string; const AX,AY: double);
var
  Text: string;
begin
  if AText = '' then Exit;
  Text := StripRPTF(AText);
  LeftText(Text,AX - TextWidth(Text), AY);
end;

procedure TRvRenderText.PageEnd;
var
  I1: integer;
  Line: integer;
  LineList: TList;
  TextItem: PTextItem;
  CurrentPos: integer;
begin { PageEnd }
{ Process through PageList and output TextItems }
  for Line := 1 to PageList.Count do begin
    LineList := PageList[Line - 1];
    CurrentPos := 1;
    for I1 := 0 to LineList.Count - 1 do begin
      TextItem := PTextItem(LineList[I1]);
      with TextItem^ do begin
        if Col > CurrentPos then begin
          WriteStr(MakeSpaces(Col - CurrentPos));
        end else if Col < CurrentPos then begin
          WriteStr(#13 + MakeSpaces(Col - 1));
        end; { else }
        WriteStr(Data);
        CurrentPos := Col + Length(Data); { Need to strip out escape chars }
      end; { with }
    end; { for }
    WriteStr(#13#10);
  end; { for }

  if FormFeed then begin
    WriteStr(FormFeedStr);
  end; { if }
  ClearPageList;
end;  { PageEnd }

procedure TRvRenderText.WriteStr(Value: string);
begin { WriteStr }
  Print(Value);
end;  { WriteStr }

function TRvRenderText.TextWidth(Text: string): double;
begin { TextWidth }
  Result := Length(Text) / CPI;
end;  { TextWidth }

{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
procedure TRvRenderText.PrintRender(NDRStream: TStream;
  OutputFileName: TFileName);
var
  LExtension: string;
begin
  if Pos('.',OutputFileName) < 1 then begin
    LExtension := Copy(FileExtension, Pos('.',FileExtension),Length(FileExtension));
    if Pos(';',LExtension) > 0 then begin
      LExtension := Copy(LExtension, 0, Pos(';',LExtension) - 1);
    end; { if }

    OutputFileName := OutputFileName + LExtension;
  end; { if }
  InitFileStream(OutputFileName);
  with TRPConverter.Create(NDRStream, self) do try
    Generate;
  finally
    Free;
  end; { with }
end;
{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED ON}{$ENDIF}

procedure TRvRenderText.PrintSpaces(const AText: string; const AX, AY,
  AWidth: double);
var
  CurrWord: string;
  Spaces: integer;
  SpaceWidth: double;
  I1: integer;
  LText: string;
  XPos, YPos: double;
begin
{ Count the number of spaces in Text }
  LText := AText;
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
  SpaceWidth := TextWidth(' ');

  SpaceWidth := SpaceWidth + ((AWidth - TextWidth(LText)) / Spaces);

{ Print first word left aligned, middle and last words right aligned }
  CurrWord := GetFirstWord(LText);
  XPos := AX;
  YPos := AY;
  LeftText(CurrWord, XPos, YPos);
  AdjustXY(XPos,YPos,TextWidth(CurrWord));
  while LText <> '' do begin
    Spaces := Length(GetSpaces(LText));
    CurrWord := GetFirstWord(LText);
    PrintRightWidth(XPos,YPos,CurrWord,TextWidth(CurrWord) + (Spaces * SpaceWidth));
  end; { while }
end;

procedure TRvRenderText.DocBegin;
begin
  if FOutputStream = nil then begin
    InitFileStream(OutputFileName);
  end; { if }
end;

procedure TRvRenderText.DocEnd;
begin
  FreeFileStream;
end;

procedure TRvRenderText.PrintRightWidth(var AX, AY: double; AText: string;
  Width: double);
var
  LTextWidth: double;
  Spaces: integer;
  SpaceStr: string;
  SpaceWidth: double;
begin
{ Print a block of spaces to fill the front space width }
  SpaceWidth := Width - TextWidth(AText);
  LTextWidth := TextWidth(' ');
  Spaces := 0;
  if LTextWidth > 0 then begin
    Spaces := Round(SpaceWidth / LTextWidth);
  end; { if }
  if Spaces > 0 then begin
    SetLength(SpaceStr,Spaces);
    Spaces := Length(SpaceStr);
    FillChar(SpaceStr[1],Spaces,' ');
    LeftText(SpaceStr,AX,AY);
    AdjustXY(AX,AY,TextWidth(SpaceStr));
  end; { if }
//  AdjustXY(AX,AY,SpaceWidth);
//  RightText(' ',AX,AY);

{ Print Text right aligned }
  AdjustXY(AX,AY,TextWidth(AText));
  RightText(AText,AX,AY);
end;

end.