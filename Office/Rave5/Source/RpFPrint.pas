{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpFPrint;

interface

{$I RpVer.inc}

uses
  Windows, Messages, Graphics, Controls, Forms, Dialogs, StdCtrls, ExtCtrls,
  Classes, SysUtils, RpDefine, RpBase, RpDevice, RpCanvas;

type
{*************************************************************************}
{                           class TFilePrinter                           }
{*************************************************************************}

  TFilePrinter = class(TCanvasReport)
  protected
    FStream: TStream;
    FStreamMode: TStreamMode;
    FInputFileName: TFileName;
    PrevPagePos: longint;
    CurrPagePos: longint;
    NextPagePos: longint;
    PaperWidth: double;
    PaperHeight: double;
    FileVer: integer;
    ReuseGraphic: boolean;
    FIgnoreFileSettings: boolean;
    NextOrientation: TOrientation;
    VarList: TStringList;
    JobHeaderRead: boolean;
  {$IFDEF SOURCEGEN}
    SourceFile: text;
  {$ENDIF}

    function GetPages: integer;

  { Stream input routines }
    function ReadByte: byte;
    function ReadInteger: smallint;
    function ReadLongint: longint;
    function ReadText: string;
    function ReadFloat: double;
    procedure ReadXY(var X,Y: double);
    procedure LoadGraphic(Value: TGraphic);
    procedure ReadGraphic(Value: TGraphic);
    procedure CreateGraphic(var Value: TGraphic);
    function ReadPoint: TPoint;
    function ReadRect: TRect;
    procedure ReadBuf(var Buf;
                          Len: integer);
    procedure MatchCode(Value: byte);
    procedure ReadBrush;
    procedure ReadFont;
    procedure ReadPen;
    procedure ReadJobHeader;
    procedure ReadPageHeader;
    function ReadPIVar: string;
    procedure ClearVarList;
    function InitInputStream: boolean;
    procedure GotoPage(PageNum: word); virtual;
    procedure SetStream(Value: TStream);
    procedure SetStreamMode(Value: TStreamMode);
  public
    //
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    function GetPIVar(VarName: string): string; override;
    procedure SetPIVar(VarName: string;
                       VarValue: string); override;

    procedure PrintPage(PageNum: word); virtual;

    procedure ExecuteCustom(NewFirstPage: integer;
                            NewLastPage: integer;
                            NewCopies: integer);
    procedure Execute; override;
    procedure Start; override;
    procedure Finish; override;

    property Pages: integer read GetPages;
    property Stream: TStream read FStream write SetStream;
    property IgnoreFileSettings: boolean read FIgnoreFileSettings write
     FIgnoreFileSettings;
  published
    property FileName: TFileName read FInputFileName write FInputFileName;
    property StreamMode: TStreamMode read FStreamMode write SetStreamMode
     default smMemory;
    property OnDecodeImage;
  end; { TFilePrinter }

implementation

{*************************************************************************}
{                           class TFilePrinter                           }
{*************************************************************************}

constructor TFilePrinter.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);

  FStreamMode := smMemory;
  FInputFileName := '';
  FUnitsFactor := 1.0;
  FUnits := unInch;
  NoBufferLine := true;
  VarList := TStringList.Create;
end;  { Create }

destructor TFilePrinter.Destroy;
begin { Destroy }
  ClearVarList;
  FreeAndNil(VarList);
  if FStreamMode <> smUser then begin
    FreeAndNil(FStream);
  end; { if }
  inherited Destroy;
end;  { Destroy }

function TFilePrinter.GetPages: integer;
begin { GetPages }
  ReadJobHeader;
  Result := JobPages;
end;  { GetPages }

{ Stream input routines }

function TFilePrinter.ReadByte: byte;
begin { ReadByte }
  FStream.ReadBuffer(Result,SizeOf(Result));
end;  { ReadByte }

function TFilePrinter.ReadInteger: smallint;
begin { ReadInteger }
  FStream.ReadBuffer(Result,SizeOf(Result));
end;  { ReadInteger }

function TFilePrinter.ReadLongint: longint;
begin { ReadLongint }
  FStream.ReadBuffer(Result,SizeOf(Result));
end;  { ReadLongint }

function TFilePrinter.ReadText: string;
var
  Len: word;
begin { ReadText }
  Len := ReadByte;
  if (FileVer >= 2016) and (Len >= 192) then begin
    Len := (Len * $0100 + ReadByte) - $C000;
  end; { if }
  SetLength(Result,Len);
  if Len = 0 then begin
    Result := '';
  end else begin
    FStream.ReadBuffer(Result[1],Len);
  end; { else }
  ProcessString(Result);
end;  { ReadText }

function TFilePrinter.ReadFloat: double;
var
  SingleVar: single;
begin { ReadFloat }
  FStream.ReadBuffer(SingleVar,SizeOf(SingleVar));
  Result := SingleVar;
end;  { ReadFloat }

procedure TFilePrinter.ReadXY(var X,Y: double);
begin { ReadXY }
  X := ReadFloat;
  Y := ReadFloat;
end;  { ReadXY }

procedure TFilePrinter.LoadGraphic(Value: TGraphic);
var
  TempStream: TMemoryStream;
  NewPos: longint;
  SavePos: longint;
begin { LoadGraphic }
  TempStream := TMemoryStream.Create; { Create temp stream }
  try
    if ReuseGraphic then begin
      NewPos := ReadLongint;
      SavePos := FStream.Position;
      FStream.Position := NewPos;
    end else begin
      SavePos := 0;
    end; { else }
    TempStream.SetSize(ReadLongint); { Allocate space }
    FStream.ReadBuffer(TempStream.Memory^,TempStream.Size);
    Value.LoadFromStream(TempStream); { Copy stream to icon object }
    if ReuseGraphic then begin
      FStream.Position := SavePos;
      ReuseGraphic := false;
    end; { if }
  finally
    TempStream.Free; { Clean up stream }
  end; { try }
end;  { LoadGraphic }

procedure TFilePrinter.ReadGraphic(Value: TGraphic);
begin { ReadGraphic }
  ReuseGraphic := ReadByte > 128;
  LoadGraphic(Value);
end;  { ReadGraphic }

procedure TFilePrinter.CreateGraphic(var Value: TGraphic);
var
  Kind: byte;
begin { CreateGraphic }
  Kind := ReadByte;
  ReuseGraphic := Kind > 128;
  case (Kind mod 128) of
    1: begin { Bitmap }
      Value := TBitmap.Create;
    end;
    2: begin { Metafile }
      Value := TMetafile.Create;
    end;
    3: begin { Icon }
      Value := TIcon.Create;
    end;
  end; { case }
  LoadGraphic(Value);
end;  { CreateGraphic }

function TFilePrinter.ReadPoint: TPoint;
begin { ReadPoint }
  Result.X := XI2D(ReadFloat);
  Result.Y := YI2D(ReadFloat);
end;  { ReadPoint }

function TFilePrinter.ReadRect: TRect;
begin { ReadRect }
  Result.TopLeft := ReadPoint;
  Result.BottomRight := ReadPoint;
end;  { ReadRect }

procedure TFilePrinter.ReadBuf(var Buf; Len: integer);
begin { ReadBuf }
  FStream.ReadBuffer(Buf,Len);
end;  { ReadBuf }

procedure TFilePrinter.MatchCode(Value: byte);
begin { MatchCode }
  if ReadByte <> Value then begin
    if Printing then begin
      Abort;
    end; { if }
    RaiseError(Trans('Unexpected code found in report file.'));
  end; { if }
end;  { MatchCode }

procedure TFilePrinter.ReadBrush;
begin { ReadBrush }
  with Canvas.Brush do begin
    Color := TColor(ReadLongint);
    Style := TBrushStyle(ReadByte);
    if boolean(ReadByte) then begin
      ReadGraphic(Bitmap);
    end; { if }
  end; { with }
end;  { ReadBrush }

procedure TFilePrinter.ReadFont;
begin { ReadFont }
  with BaseFont do begin
    Name := ReadText;
    Color := TColor(ReadLongint);
    Pitch := TFontPitch(ReadByte);
    if FileVer > 2017 then begin
      Charset := ReadByte;
    end else begin
      Charset := DEFAULT_CHARSET;
    end; { if }
    Size := ReadFloat * 72;
    Height := Size / 72.0;
    Bold := boolean(ReadByte);
    Italic := boolean(ReadByte);
    Underline := boolean(ReadByte);
    Strikeout := boolean(ReadByte);
    if FileVer >= 2000 then begin
      Rotation := ReadInteger;
    end else begin
      Rotation := 0;
    end; { else }
  end; { with }
end;  { ReadFont }

procedure TFilePrinter.ReadPen;
var
  PenWidth: double;
begin { ReadPen }
  with Canvas.Pen do begin
    Color := TColor(ReadLongint);
    Mode := TPenMode(ReadByte);
    Style := TPenStyle(ReadByte);
    PenWidth := ReadFloat;
    Width := Round(PenWidth * FXDPI * FScaleX * FZoom / 10000.0);
  end; { with }
end;  { ReadPen }

procedure TFilePrinter.ReadJobHeader;
var
  I1: integer;
  DefaultSource: integer;
  S1: string;
  B1: byte;
  SavePos: longint;
begin { ReadJobHeader }
  if JobHeaderRead then Exit;

  if not InitInputStream then Exit;

  SavePos := FStream.Position;
  FStream.Position := 0;
  B1 := ReadByte;
  if B1 = rpcJobHeader11 then begin
    FileVer := 1101;
  end else if B1 = rpcJobHeader20i then begin
    FileVer := 2009;
  end else if B1 = rpcJobHeader then begin
    FileVer := ReadInteger;
  end else begin
    RaiseError(Trans('Unexpected header code found in report file.'));
  end; { else }

  if FileVer >= 2010 then begin
    Title := ReadText;
  end; { if }

{ Pages, First & Last Page }
	if FileVer >= 2018 then begin
    FJobPages := ReadLongint;
    { FFirstPage := }ReadLongint;
    { FLastPage := }ReadLongint;
	end else begin
    FJobPages := ReadInteger;
    { FFirstPage := }ReadInteger;
    { FLastPage := }ReadInteger;
	end; { else }

{ Page Width,Height }
  PaperWidth := ReadFloat;
  PaperHeight := ReadFloat;

{ Waste dimensions }
  ReadFloat; { FLeftWaste }
  ReadFloat; { FRightWaste }
  ReadFloat; { FTopWaste }
  ReadFloat; { FBottomLeftWaste }

{ Printer settings }
  Orientation := TOrientation(ReadByte);
  SetOrientationActive;
(*
  If NoPrinters then begin
    If FileVer >= 2000 then begin
      ReadInteger; { Bin }
      ReadByte; { Collate }
      ReadByte; { Duplex }
    end; { if }
    ReadInteger; { Copies }
    ReadInteger; { PaperSize }
    ReadInteger; { PaperWidth }
    ReadInteger; { PaperLength }
  end else begin
    If IgnoreFileSettings then begin
      If FileVer >= 2000 then begin
        ReadInteger; { Bin }
        ReadByte; { Collate }
        ReadByte; { Duplex }
      end; { if }
      ReadInteger; { Copies }
    end else begin
      If FileVer >= 2000 then begin
        {$IFDEF RaveCLX}
        //!!PORT!! RPBASE02
        ReadInteger;
        {$ELSE}
        DevMode^.dmDefaultSource := ReadInteger;
        {$ENDIF}
        Collate := boolean(ReadByte);
        Duplex := TDuplex(ReadByte);
      end; { if }
      {$IFDEF RaveCLX}
      //!!PORT!! RPBASE02
      ReadInteger;
      {$ELSE}
      DevMode^.dmCopies := ReadInteger;
      {$ENDIF}
    end; { else }
    {$IFDEF RaveCLX}
    //!!PORT!! RPBASE02
    ReadInteger;
    ReadInteger;
    ReadInteger;
    {$ELSE}
    DevMode^.dmPaperSize := ReadInteger;
    DevMode^.dmPaperWidth := ReadInteger;
    DevMode^.dmPaperLength := ReadInteger;
    {$ENDIF}
  end; { else }
*)

  if IgnoreFileSettings then begin
    if FileVer >= 2000 then begin
      ReadInteger; { Bin }
      ReadByte; { Collate }
      ReadByte; { Duplex }
    end; { if }
    ReadInteger; { Copies }
  end else begin
    if FileVer >= 2000 then begin
      DefaultSource := ReadInteger;
      if Assigned(Bins) then begin
        for I1 := 0 to Bins.Count - 1 do begin
          if integer(Bins.Objects[I1]) = DefaultSource then begin
            SelectBin(Bins[I1]);
          end;
        end; { for }
      end; { if }
      Collate := boolean(ReadByte);
      Duplex := TDuplex(ReadByte);
    end; { if }
    Copies := ReadInteger;
  end; { else }
  SetPaperSize(ReadInteger, ReadInteger, ReadInteger);

{ OutputName }
  S1 := ReadText;
  if S1 <> '' then begin
    OutputName := S1;
  end; { if }
  if SavePos > 0 then begin
    FStream.Position := SavePos;
  end; { if }
  JobHeaderRead := true;
end;  { ReadJobHeader }

procedure TFilePrinter.ReadPageHeader;
begin { ReadPageHeader }
  CurrPagePos := FStream.Position;
  MatchCode(rpcNewPage);

{ Read page header }
  PrevPagePos := ReadLongint;
  NextPagePos := ReadLongint;
	if FileVer >= 2018 then begin
    FCurrentPage := ReadLongint;
	end else begin
    FCurrentPage := ReadInteger;
	end; { else }

{ Page Width,Height }
  if FileVer >= 2000 then begin
    PaperWidth := ReadFloat;
    PaperHeight := ReadFloat;
  end else begin
    ReadByte; { Old orientation byte }
  end; { else }
end;  { ReadPageHeader }

function TFilePrinter.ReadPIVar: string;
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

procedure TFilePrinter.ClearVarList;
var
  I1: integer;
begin { ClearVarList }
  for I1 := 0 to (VarList.Count - 1) do begin
    VarList.Objects[I1].Free;
  end; { for }
  VarList.Clear;
end;  { ClearVarList }

function TFilePrinter.GetPIVar(VarName: string): string;
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

procedure TFilePrinter.SetPIVar(VarName: string;
                                VarValue: string);
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

function TFilePrinter.InitInputStream: boolean;
begin { InitInputStream }
  Result := true;
  if Assigned(Stream) then begin
    Stream.Position := 0;
    Exit;
  end; { if }
  try
    case FStreamMode of
      smMemory: begin
        FStream := TMemoryStream.Create;
        with FStream as TMemoryStream do begin
          LoadFromFile(FInputFilename);
        end; { with }
      end;
      smFile,smTempFile: begin
        FStream := TFileStream.Create(FInputFilename,fmOpenRead or fmShareDenyWrite);
      end;
      smUser: begin
        if FStream = nil then begin
          RaiseError(Trans('StreamMode is smUser but Stream is nil'));
        end; { if }
      end;
    end; { case }
  except
    on E: EStreamError do begin
      Result := false;
      if FStreamMode <> smUser then begin
        FreeAndNil(FStream);
      end; { if }
      MessageDlg(Trans(Format({Trans+}'Error initializing input stream "%s"',
       [FInputFilename])),mtError,[mbOk],0);
    end; { on }
  end; { try }
end;  { InitInputStream }

{ Public Methods }

procedure TFilePrinter.GotoPage(PageNum: word);
begin { GotoPage }
  if PageNum = FCurrentPage then begin
    FStream.Position := CurrPagePos;
    ReadPageHeader;
    Exit;
  end; { if }

  while PageNum < FCurrentPage do begin { Go backwards }
    if PrevPagePos > 0 then begin
      FStream.Position := PrevPagePos;
      ReadPageHeader;
    end else begin
      FStream.Position := CurrPagePos;
      ReadPageHeader;
      Exit;
    end; { else }
  end; { while }

  while PageNum > FCurrentPage do begin { Go forwards }
    if NextPagePos > 0 then begin
      FStream.Position := NextPagePos;
      ReadPageHeader;
    end else begin
      FStream.Position := CurrPagePos;
      ReadPageHeader;
      Exit;
    end; { else }
  end; { while }
end;  { GotoPage }

procedure TFilePrinter.PrintPage(PageNum: word);
type
  PPointArr = ^TPointArr;
  TPointArr = array[0..16380] of TPoint;
var
  PageDone: boolean;
  Command: byte;
  Graphic: TGraphic;
  I1,I2: integer;
  L1: integer;
  L2: integer;
  B1: byte;
  F1: double;
  S1: string;
  PointArr: PPointArr;
  X1,X2,X3,X4: double;
  Y1,Y2,Y3,Y4: double;
  Rect1,Rect2: TRect;
  TempStream: TMemoryStream;
begin { PrintPage }
{TransOff}
{$IFDEF SOURCEGEN}
  AssignFile(SourceFile,'RPSOURCE.OUT');
  {$I-} Append(SourceFile); {$I+}
  if IOResult <> 0 then begin
    Rewrite(SourceFile);
  end; { if }
  Writeln(SourceFile,'{**** Printing page ' + IntToStr(PageNum) + ' ***}');
  Writeln(SourceFile,'With Sender as TBaseReport do begin');
{$ENDIF}
  GotoPage(PageNum);
  UpdateStatus;

{ Process commands until end of page marker }
  PageDone := false;
  repeat
    Command := ReadByte;
    case Command of
      rpcArc: begin
        X1 := ReadFloat;
        Y1 := ReadFloat;
        X2 := ReadFloat;
        Y2 := ReadFloat;
        X3 := ReadFloat;
        Y3 := ReadFloat;
        X4 := ReadFloat;
        Y4 := ReadFloat;
        Arc(X1,Y1,X2,Y2,X3,Y3,X4,Y4);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Arc(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',' +
         FloatToStr(X2) + ',' + FloatToStr(Y2) + ',' +
         FloatToStr(X3) + ',' + FloatToStr(Y3) + ',' +
         FloatToStr(X4) + ',' + FloatToStr(Y4) + ');');
      {$ENDIF}
      end;
      rpcBrushCopy: begin
        CreateGraphic(Graphic);
        Rect1 := ReadRect;
        Rect2 := ReadRect;
        L1 := ReadLongint;
      {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
      {$IFDEF LEVEL6}{$WARN SYMBOL_LIBRARY OFF}{$ENDIF}
        BrushCopy(Rect1,Graphic as TBitmap,Rect2,L1);
      {$IFDEF LEVEL6}{$WARN SYMBOL_LIBRARY ON}{$ENDIF}
      {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
        FreeAndNil(Graphic);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  BrushCopy(Rect1,Bitmap,Rect2,TColor(' +
         IntToStr(L1) + '));');
      {$ENDIF}
      end;
      rpcChord: begin
        X1 := ReadFloat;
        Y1 := ReadFloat;
        X2 := ReadFloat;
        Y2 := ReadFloat;
        X3 := ReadFloat;
        Y3 := ReadFloat;
        X4 := ReadFloat;
        Y4 := ReadFloat;
        Chord(X1,Y1,X2,Y2,X3,Y3,X4,Y4);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Chord(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',' +
         FloatToStr(X2) + ',' + FloatToStr(Y2) + ',' +
         FloatToStr(X3) + ',' + FloatToStr(Y3) + ',' +
         FloatToStr(X4) + ',' + FloatToStr(Y4) + ');');
      {$ENDIF}
      end;
      rpcCopyRect: begin
        Rect1 := ReadRect;
        L1 := ReadLongint;
        Rect2 := ReadRect;
      {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
        CopyRect(Rect1,TCanvas(L1),Rect2);
      {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED ON}{$ENDIF}
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  CopyRect(Rect1,Canvas,Rect2);');
      {$ENDIF}
      end;
      rpcDraw: begin
        CreateGraphic(Graphic);
        X1 := ReadFloat;
        Y1 := ReadFloat;
        Draw(X1,Y1,Graphic);
        FreeAndNil(Graphic);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Draw(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',Graphic);');
      {$ENDIF}
      end;
      rpcDrawFocusRect: begin
        Rect1 := ReadRect;
      {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
        DrawFocusRect(Rect1);
      {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED ON}{$ENDIF}
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  DrawFocusRect(Rect1);');
      {$ENDIF}
      end;
      rpcEllipse: begin
        X1 := ReadFloat;
        Y1 := ReadFloat;
        X2 := ReadFloat;
        Y2 := ReadFloat;
        Ellipse(X1,Y1,X2,Y2);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Ellipse(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',' +
         FloatToStr(X2) + ',' + FloatToStr(Y2) + ');');
      {$ENDIF}
      end;
      rpcFillRect: begin
        Rect1 := ReadRect;
      {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
        FillRect(Rect1);
      {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED ON}{$ENDIF}
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  FillRect(Rect1);');
      {$ENDIF}
      end;
      rpcFloodFill: begin
        X1 := ReadFloat;
        Y1 := ReadFloat;
        L1 := ReadLongint;
        L2 := ReadLongint;
      {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
      {$IFDEF LEVEL6}{$WARN SYMBOL_LIBRARY OFF}{$ENDIF}
        FloodFill(X1,Y1,TColor(L1),TFillStyle(L2));
      {$IFDEF LEVEL6}{$WARN SYMBOL_LIBRARY ON}{$ENDIF}
      {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  FloodFill(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',TColor(' +
         IntToStr(L1) + '),TFillStyle(' + IntToStr(L2) + '));');
      {$ENDIF}
      end;
      rpcFrameRect: begin
        Rect1 := ReadRect;
      {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
      {$IFDEF LEVEL6}{$WARN SYMBOL_LIBRARY OFF}{$ENDIF}
        FrameRect(Rect1);
      {$IFDEF LEVEL6}{$WARN SYMBOL_LIBRARY ON}{$ENDIF}
      {$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  FrameRect(Rect1);');
      {$ENDIF}
      end;
      rpcLineTo: begin
        X1 := ReadFloat;
        Y1 := ReadFloat;
        LineTo(X1,Y1);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  LineTo(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ');');
      {$ENDIF}
      end;
      rpcMoveTo: begin
        X1 := ReadFloat;
        Y1 := ReadFloat;
        MoveTo(X1,Y1);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  MoveTo(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ');');
      {$ENDIF}
      end;
      rpcPie: begin
        X1 := ReadFloat;
        Y1 := ReadFloat;
        X2 := ReadFloat;
        Y2 := ReadFloat;
        X3 := ReadFloat;
        Y3 := ReadFloat;
        X4 := ReadFloat;
        Y4 := ReadFloat;
        Pie(X1,Y1,X2,Y2,X3,Y3,X4,Y4);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Pie(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',' +
         FloatToStr(X2) + ',' + FloatToStr(Y2) + ',' +
         FloatToStr(X3) + ',' + FloatToStr(Y3) + ',' +
         FloatToStr(X4) + ',' + FloatToStr(Y4) + ');');
      {$ENDIF}
      end;
      rpcPolygon: begin
        I1 := ReadInteger;
        GetMem(PointArr,(I1 + 1) * SizeOf(TPoint));
        try
          for I2 := 0 to I1 do begin
            PointArr^[I2].X := XU2D(ReadFloat);
            PointArr^[I2].Y := YU2D(ReadFloat);
          end; { for }
        {$IFDEF SOURCEGEN}
          Writeln(SourceFile,'  Polygon(Points);');
        {$ENDIF}

        { Call Polygon(PointArr^) but with only I1+1 items in open array }
          asm
            mov edx,PointArr { Load pointer to PointArr }
            mov ecx,I1 { Load High(PointArr^) }
            mov eax,self { Load self pointer }
            mov ebx,[eax]
            call TFilePrinter.Polygon { call self.Polygon }
          end; { asm }
        finally
          FreeMem(PointArr,(I1 + 1) * SizeOf(TPoint));
        end; { try }
      end;
      rpcPolyline: begin
        I1 := ReadInteger;
        GetMem(PointArr,(I1 + 1) * SizeOf(TPoint));
        try
          for I2 := 0 to I1 do begin
            PointArr^[I2].X := XU2D(ReadFloat);
            PointArr^[I2].Y := YU2D(ReadFloat);
          end; { for }
        {$IFDEF SOURCEGEN}
          Writeln(SourceFile,'  Polyline(Points);');
        {$ENDIF}

        { Call Polyline(PointArr^) but with only I1+1 items in open array }
          asm
            mov edx,PointArr { Load pointer to PointArr }
            mov ecx,I1 { Load High(PointArr^) }
            mov eax,self { Load self pointer }
            mov ebx,[eax]
            call TFilePrinter.Polyline { call self.PolyLine }
          end; { asm }
        finally
          FreeMem(PointArr,(I1 + 1) * SizeOf(TPoint));
        end; { tryf }
      end;
      rpcPrintBitmap: begin
        CreateGraphic(Graphic);
        X1 := ReadFloat;
        Y1 := ReadFloat;
        X2 := ReadFloat;
        Y2 := ReadFloat;
        PrintBitmap(X1,Y1,X2,Y2,Graphic as TBitmap);
        FreeAndNil(Graphic);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  PrintBitmap(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',' +
         FloatToStr(X2) + ',' + FloatToStr(Y2) + ',Bitmap);');
      {$ENDIF}
      end;
      rpcPrintBitmapRect: begin
        CreateGraphic(Graphic);
        X1 := ReadFloat;
        Y1 := ReadFloat;
        X2 := ReadFloat;
        Y2 := ReadFloat;
        PrintBitmapRect(X1,Y1,X2,Y2,Graphic as TBitmap);
        FreeAndNil(Graphic);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  PrintBitmapRect(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',' +
         FloatToStr(X2) + ',' + FloatToStr(Y2) + ',Bitmap);');
      {$ENDIF}
      end;
      rpcRectangle: begin
        X1 := ReadFloat;
        Y1 := ReadFloat;
        X2 := ReadFloat;
        Y2 := ReadFloat;
        Rectangle(X1,Y1,X2,Y2);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Rectangle(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',' +
         FloatToStr(X2) + ',' + FloatToStr(Y2) + ');');
      {$ENDIF}
      end;
      rpcTabRectangle: begin
        X1 := ReadFloat;
        Y1 := ReadFloat;
        X2 := ReadFloat;
        Y2 := ReadFloat;
        TabRectangle(X1,Y1,X2,Y2);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  TabRectangle(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',' +
         FloatToStr(X2) + ',' + FloatToStr(Y2) + ');');
      {$ENDIF}
      end;
      rpcRoundRect: begin
        X1 := ReadFloat;
        Y1 := ReadFloat;
        X2 := ReadFloat;
        Y2 := ReadFloat;
        X3 := ReadFloat;
        Y3 := ReadFloat;
        RoundRect(X1,Y1,X2,Y2,X3,Y3);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  RoundRect(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',' +
         FloatToStr(X2) + ',' + FloatToStr(Y2) + ',' +
         FloatToStr(X3) + ',' + FloatToStr(Y3) + ');');
      {$ENDIF}
      end;
      rpcStretchDraw: begin
        CreateGraphic(Graphic);
        Rect1 := ReadRect;
        StretchDraw(Rect1,Graphic);
        FreeAndNil(Graphic);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  StretchDraw(Rect1,Graphic);');
      {$ENDIF}
      end;
      rpcTextRect: begin
        Rect1 := ReadRect;
        X1 := ReadFloat;
        Y1 := ReadFloat;
        S1 := ReadText;
        TextRect(Rect1,X1,Y1,S1);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  TextRect(Rect1,' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',''' + S1 + ''');');
      {$ENDIF}
      end;
      rpcCenterText: begin
        FCursorYPos := ReadFloat;
        S1 := ReadText;
        X1 := ReadFloat;
        PrintCenterInch(S1,X1);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  PrintCenter(''' + S1 + ''',' + FloatToStr(X1) + ');');
      {$ENDIF}
      end;
      rpcLeftText: begin
        FCursorYPos := ReadFloat;
        S1 := ReadText;
        X1 := ReadFloat;
        PrintLeftInch(S1,X1);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  PrintLeft(''' + S1 + ''',' + FloatToStr(X1) + ');');
      {$ENDIF}
      end;
      rpcRightText: begin
        FCursorYPos := ReadFloat;
        S1 := ReadText;
        X1 := ReadFloat;
        PrintRightInch(S1,X1);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  PrintRight(''' + S1 + ''',' + FloatToStr(X1) + ');');
      {$ENDIF}
      end;
      rpcSetTextBKMode: begin
        TextBKMode := TBKMode(ReadByte);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  TextBKMode := TBKMode(',Ord(TextBKMode),');');
      {$ENDIF}
      end;
      rpcSetBKColor: begin
        BKColor := TColor(ReadLongint);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  BKColor := ',BKColor);
      {$ENDIF}
      end;
      rpcSetScaleX: begin
      { Don't change ScaleX }
        ReadFloat;
      end;
      rpcSetScaleY: begin
      { Don't change ScaleY }
        ReadFloat;
      end;
      rpcPrintData: begin
        S1 := ReadText;
        PrintData(S1);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  PrintData(''' + S1 + ''');');
      {$ENDIF}
      end;
      rpcPrintDataStream: begin
        L1 := ReadLongint;
        PrintDataStream(FStream,L1);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  PrintDataStream(Stream,' + IntToStr(L1) + ');');
      {$ENDIF}
      end;
      rpcPrintSpaces: begin
        FCursorYPos := ReadFloat;
        S1 := ReadText;
        X1 := ReadFloat;
        F1 := ReadFloat;
        PrintBlockInch(S1,X1,F1);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  PrintBlockInch(''' + S1 + ''',' +
         FloatToStr(X1) + ',' + FloatToStr(F1) + ');');
      {$ENDIF}
      end;
      rpcBlockText: begin
        FCursorYPos := ReadFloat;
        S1 := ReadText;
        X1 := ReadFloat;
        X2 := ReadFloat;
        PrintBlockInch(S1,X1,X2);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  PrintBlockInch(''' + S1 + ''',' +
         FloatToStr(X1) + ',' + FloatToStr(X2) + ');');
      {$ENDIF}
      end;
      rpcTransBitmap: begin
        TransparentBitmaps := boolean(ReadByte);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  TransparentBitmaps := ',TransparentBitmaps,';');
      {$ENDIF}
      end;
      rpcSetFrameMode: begin
        FrameMode := TFrameMode(ReadByte);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  FrameMode := TFrameMode(',Ord(FrameMode),');');
      {$ENDIF}
      end;
      rpcPrintImageRect: begin
        TempStream := TMemoryStream.Create; { Create temp stream }
        try
          TempStream.SetSize(ReadLongint); { Allocate space }
          FStream.ReadBuffer(TempStream.Memory^,TempStream.Size);
          TempStream.Position := 0;
          S1 := ReadText; { Read ImageType }
          X1 := ReadFloat;
          Y1 := ReadFloat;
          X2 := ReadFloat;
          Y2 := ReadFloat;
          PrintImageRect(X1,Y1,X2,Y2,TempStream,S1);
        finally
          TempStream.Free; { Clean up stream }
        end; { tryf }
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  PrintImageRect(' +
         FloatToStr(X1) + ',' + FloatToStr(Y1) + ',' +
         FloatToStr(X2) + ',' + FloatToStr(Y2) + ',Image,' + S1 + ');');
      {$ENDIF}
      end;
      rpcVariableData: begin
        S1 := ReadText; { Read variable name }
        SetPIVar(S1,ReadPIVar);
      end;

      rpcSetBrush: begin
        ReadBrush;
      end;
      rpcSetBrushColor: begin
//!!PORT!! under Kylix, Brush.Bitmap is always non-nil - Is this WINCLX as well? RPFPRINT02
        if Canvas.Brush.Bitmap <> nil then begin
          Canvas.Brush.Bitmap.Free;
          Canvas.Brush.Bitmap := nil;
        end; { if }
        Canvas.Brush.Color := TColor(ReadLongint);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Canvas.Brush.Color := ',Canvas.Brush.Color,';');
      {$ENDIF}
      end;
      rpcSetBrushStyle: begin
        Canvas.Brush.Style := TBrushStyle(ReadByte);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Canvas.Brush.Style := TBrushStyle(',
         Ord(Canvas.Brush.Style),');');
      {$ENDIF}
      end;
      rpcSetBrushBitmap: begin
        if boolean(ReadByte) then begin
          if not Assigned(Canvas.Brush.Bitmap) then begin
            Canvas.Brush.Bitmap := TBitmap.Create;
          end; { if }
          ReadGraphic(Canvas.Brush.Bitmap);
        end; { if }
      end;

      rpcSetFont: begin
        ReadFont;
        FontOnChange(self);
      end;
      rpcSetFontName: begin
        FontName := ReadText;
        FontOnChange(self);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  FontName := ''' + FontName + ''';');
      {$ENDIF}
      end;
      rpcSetFontColor: begin
        FontColor := TColor(ReadLongint);
        FontOnChange(self);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  FontColor := ',FontColor,';');
      {$ENDIF}
      end;
      rpcSetFontPitch: begin
        FontPitch := TFontPitch(ReadByte);
        FontOnChange(self);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  FontPitch := TFontPitch(',Ord(FontPitch),');');
      {$ENDIF}
      end;
      rpcSetFontHeight: begin
        X4 := ReadFloat;
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  FontHeight := ' + FloatToStr(X4) + ';');
      {$ENDIF}
        FontHeight := X4;
        FontOnChange(self);
        if FileVer >= 2000 then begin
          X1 := ReadFloat; { Real width of FontString }
          X2 := TextWidth(FontString) * 100.0 / FZoom;
          while X2 > X1 do begin { Get font to match printer size }
            FontHeight := ((FontHeight * FYDPI * FZoom / 100.0) - 1.1) /
             (FYDPI * FZoom / 100.0);
            FontOnChange(self);
            X3 := X2;
            X2 := TextWidth(FontString) * 100.0 / FZoom;
            if X2 >= X3 then begin { Making no progress }
              FontHeight := X4;
              FontOnChange(self);
              Break;
            end; { if }
          end; { while }
        end; { if }
      end;
      rpcSetFontBold: begin
        Bold := boolean(ReadByte);
        FontOnChange(self);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Bold := ',Bold,';');
      {$ENDIF}
      end;
      rpcSetFontUnderline: begin
        Underline := boolean(ReadByte);
        FontOnChange(self);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Underline := ',Underline,';');
      {$ENDIF}
      end;
      rpcSetFontItalic: begin
        Italic := boolean(ReadByte);
        FontOnChange(self);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Italic := ',Italic,';');
      {$ENDIF}
      end;
      rpcSetFontStrikeout: begin
        Strikeout := boolean(ReadByte);
        FontOnChange(self);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Strikeout := ',Strikeout,';');
      {$ENDIF}
      end;
      rpcSetFontSuperscript: begin
        Superscript := boolean(ReadByte);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Superscript := ',Superscript,';');
      {$ENDIF}
      end;
      rpcSetFontSubscript: begin
        Subscript := boolean(ReadByte);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Subscript := ',Subscript,';');
      {$ENDIF}
      end;
      rpcSetFontRotation: begin
        FontRotation := ReadInteger;
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  FontRotation := ',FontRotation,';');
      {$ENDIF}
      end;
      rpcSetFontAlign: begin
        FontAlign := TFontAlign(ReadByte);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  FontAlign := TFontAlign(',Ord(FontAlign),');');
      {$ENDIF}
      end;
      rpcSetFontCharset: begin
        FontCharset := ReadByte;
        FontOnChange(self);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  FontCharset := ',FontCharset,';');
      {$ENDIF}
      end;

      rpcSetPen: begin
        ReadPen;
      end;
      rpcSetPenColor: begin
        Canvas.Pen.Color := TColor(ReadLongint);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Canvas.Pen.Color := ',Canvas.Pen.Color,';');
      {$ENDIF}
      end;
      rpcSetPenMode: begin
        Canvas.Pen.Mode := TPenMode(ReadByte);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Canvas.Pen.Mode := TPenMode(',
         Ord(Canvas.Pen.Mode),');');
      {$ENDIF}
      end;
      rpcSetPenStyle: begin
        Canvas.Pen.Style := TPenStyle(ReadByte);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Canvas.Pen.Style := TPenStyle(',
         Ord(Canvas.Pen.Style),');');
      {$ENDIF}
      end;
      rpcSetPenWidth: begin
        F1 := ReadFloat;
        Canvas.Pen.Width := Round(F1 * FXDPI * FScaleX * FZoom / 10000.0);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Canvas.Pen.Width := ',Canvas.Pen.Width,';');
      {$ENDIF}
      end;

      rpcEndPage: begin
        PageDone := true;
      end;
      rpcResetLineHeight: begin
        FAscentHeight := ReadFloat;
        FDescentHeight := ReadFloat;
        FLineHeight := ReadFloat;
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  ResetLineHeight;');
      {$ENDIF}
      end;
      rpcAllowPreviewOnly: begin
        AllowPreviewOnly;
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  AllowPreviewOnly;');
      {$ENDIF}
      end;
      rpcAllowPrinterOnly: begin
        AllowPrinterOnly;
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  AllowPrinterOnly;');
      {$ENDIF}
      end;
      rpcAllowAll: begin
        AllowAll;
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  AllowAll;');
      {$ENDIF}
      end;
      rpcSetOrientation: begin
        B1 := ReadByte;
        NextOrientation := TOrientation(B1);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  Orientation := TOrientation(',Ord(Orientation),');');
      {$ENDIF}
      end;
      rpcSetBin: begin
        S1 := ReadText;
        SelectBin(S1);
      {$IFDEF SOURCEGEN}
        Writeln(SourceFile,'  SelectBin(''' + S1 + ''');');
      {$ENDIF}
      end;
      else begin
        Abort;
        RaiseError('Unexpected code found in report file.');
      end;
    end; { case }
  until PageDone;

  if NextOrientation <> poDefault then begin
    Orientation := NextOrientation;
    SetOrientationActive;
    NextOrientation := poDefault;
  end; { if }

{$IFDEF SOURCEGEN}
  Writeln(SourceFile,'end; { with }');
  CloseFile(SourceFile);
{$ENDIF}
{TransOn}
end;  { PrintPage }

procedure TFilePrinter.ExecuteCustom(NewFirstPage: integer;
                                     NewLastPage: integer;
                                     NewCopies: integer);

var
  I2: integer;
  I3: integer;
  SaveCursor: TCursor;

  procedure PrintReversed(Reversed: boolean);

  var
    I1: integer;

  begin { PrintReversed }
    if Reversed then begin
      for I1 := I3 downto I2 do begin
        FPageInvalid := not PageInRange(I1) or FAborted;
        LocalDevice.NewPageSelect(not FPageInvalid);
        ProcessMessages;
        PrintPage(I1);
        FCurrentPage := I1; { Reset current page number back to I1 }
        ProcessMessages;
      end; { for }
    end else begin
      for I1 := I2 to I3 do begin
        FPageInvalid := not PageInRange(I1) or FAborted;
        LocalDevice.NewPageSelect(not FPageInvalid);
        ProcessMessages;
        PrintPage(I1);
        FCurrentPage := I1; { Reset current page number back to I1 }
        ProcessMessages;
      end; { for }
    end; { else }
  end;  { PrintReversed }

begin { ExecuteCustom }
  JobHeaderRead := false;
  if NoPrinters then begin
    MessageDlg(Trans('No printers are defined, aborting print job.'),
     mtError,[mbOk],0);
    Exit;
  end; { if }
  CheckNotPrinting;

  FReportDateTime := Now;
  NextOrientation := poDefault;

  SaveCursor := Screen.Cursor;
  Screen.Cursor := crHourglass;
{$IFDEF SOURCEGEN}
  AssignFile(SourceFile,{Trans-}'RPSOURCE.TXT');
  {$I-} Erase(SourceFile); {$I+}
  IOResult;
{$ENDIF}
  try
    if not InitInputStream then Exit;
    try
    { Read job header }
      ReadJobHeader;
      if NewCopies > 0 then begin
        Copies := NewCopies;
      end; { if }
      FCurrentPage := 0;
      PrevPagePos := 0;
      CurrPagePos := 0;
      NextPagePos := FStream.Position;
      LocalDevice.Title := FTitle;

    { Begin printing entire report }
      FPrinting := true;
      FAborted := false;
      LocalDevice.BeginDocSelect;
      Reset;

      if NewFirstPage = 0 then begin
        I2 := 1;
      end else begin
        I2 := NewFirstPage;
      end; { else }

      if (NewLastPage > 0) and (NewLastPage < JobPages) then begin
        I3 := NewLastPage;
      end else begin
        I3 := JobPages;
      end; { else }

      if AnsiPos('R',AnsiUpperCase(Selection)) > 0 then begin
        PrintReversed(true);
      end else begin
        PrintReversed(false);
      end; { else }
    finally
      if LocalDevice.Printing then begin
        LocalDevice.EndDoc;
      end; { if }
      ClearFontList;
      ClearLineLists;
      if FStreamMode <> smUser then begin
        FreeAndNil(FStream);
      end; { if }
      FPrinting := false;
    end; { try }
  finally
    Screen.Cursor := SaveCursor;
  end; { try }
end;  { ExecuteCustom }

procedure TFilePrinter.Execute;
begin { Execute }
  ExecuteCustom(0,0,0);
end;  { Execute }

procedure TFilePrinter.Start;
begin { Start }
  ExecuteCustom(0,0,0);
end;  { Start }

procedure TFilePrinter.Finish;
begin { Finish }
{ Don't do anything }
end;  { Finish }

procedure TFilePrinter.SetStream(Value: TStream);
begin
  CheckNotPrinting;
  if FStreamMode <> smUser then begin
    FreeAndNil(FStream);
    JobHeaderRead := false;
  end; { if }
  FStream := Value;
  if Assigned(FStream) then begin
    FStream.Position := 0;
    JobHeaderRead := false;
  end; { if }
end;

procedure TFilePrinter.SetStreamMode(Value: TStreamMode);
begin
  CheckNotPrinting;
  if FStreamMode = Value then Exit;
  if FStreamMode <> smUser then begin
    FreeAndNil(FStream);
  end else begin
    FStream := nil;
  end; { else }
  FStreamMode := Value;
  JobHeaderRead := false;
end;

end.