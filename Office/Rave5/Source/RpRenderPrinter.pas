{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpRenderPrinter;

interface

uses
  Windows,
  Graphics,
  Classes, SysUtils, RpRender, RpRenderCanvas, RpDefine;

type
  TRvRenderPrinter = class(TRvRenderCanvas)
  protected
    FCopies: integer;
    FCollate: boolean;
    FDuplex: TDuplex;
    FIgnoreFileSettings: boolean;
    FPages: integer;

    function GetCanvas: TCanvas; override;

    // Drawing Methods

  // Status Update Methods
    procedure DocBegin; override;
    procedure DocEnd; override;
    procedure PageBegin; override;
    procedure PageEnd; override;
    function GetPages: integer;
    function GetMaxCopies: integer;
    function GetXDPI: integer; override;
    function GetYDPI: integer; override;
    procedure SelectBin(ABin: string); override;
    procedure SelectBin(ABin: integer); override;
    procedure PrintData(Value: string); override;
    procedure PrintDataStream(Stream: TStream;
                              BufSize: longint); override;

  public
    constructor Create(AOwner: TComponent); override;

    property Pages: integer read GetPages write FPages;
    property Copies: integer read FCopies write FCopies;
    property Collate: boolean read FCollate write FCollate;
    property Duplex: TDuplex read FDuplex write FDuplex;
    property IgnoreFileSettings: boolean read FIgnoreFileSettings write FIgnoreFileSettings;
    property MaxCopies: integer read GetMaxCopies;
    function XI2D(Pos: double): integer; override;
    function YI2D(Pos: double): integer; override;
  published
    property Active default false;
    property OutputFileName;
  end;

implementation

uses
  RpDevice;

{ TRvRenderPrinter }





constructor TRvRenderPrinter.Create(AOwner: TComponent);
begin
  inherited;
  FActive := false;
end;

procedure TRvRenderPrinter.DocBegin;
begin
  RpDev.Title := Converter.Title;
  RpDev.Orientation := Converter.ReportHeader.Orientation;
  RpDev.SetPaperSize(Converter.ReportHeader.DMPaperSize,Converter.ReportHeader.DMPaperLength,
   Converter.ReportHeader.DMPaperWidth);
  if IgnoreFileSettings then begin
    RpDev.Copies := Copies;
    RpDev.Collate := Collate;
    RpDev.Duplex := Duplex;
  end else begin
    RpDev.Copies := Converter.ReportHeader.Copies;
    RpDev.Collate := Converter.ReportHeader.Collate;
    RpDev.Duplex := Converter.ReportHeader.Duplex;
  end; { else }
  RpDev.OutputFile := OutputFileName;
  RpDev.BeginDocSelect;
end;

procedure TRvRenderPrinter.DocEnd;
begin
  RpDev.EndDoc;
end;





function TRvRenderPrinter.GetCanvas: TCanvas;
begin
  Result := RPDev.Canvas;
end;


function TRvRenderPrinter.GetPages: integer;
begin
  if Assigned(Converter) and (Converter.ReportHeader.PageCount > 0) then begin
    Result := Converter.ReportHeader.PageCount;
  end else begin
    Result := FPages;
  end; { else }
end;



procedure TRvRenderPrinter.PageBegin;
begin
  RpDev.Orientation := Converter.ReportHeader.Orientation;
  RpDev.DevMode.dmDefaultSource := Converter.ReportHeader.Bin;
  RpDev.NewPageSelect(true);
end;

procedure TRvRenderPrinter.PageEnd;
begin
end;













function TRvRenderPrinter.GetMaxCopies: integer;
begin
  Result := RpDev.MaxCopies;
end;

function TRvRenderPrinter.GetXDPI: integer;
begin
  Result := RpDev.XDPI;
end;

function TRvRenderPrinter.GetYDPI: integer;
begin
  Result := RpDev.YDPI;
end;

procedure TRvRenderPrinter.SelectBin(ABin: string);
begin { SelectBin }
  RpDev.SelectBin(ABin, false);
end;  { SelectBin }

procedure TRvRenderPrinter.SelectBin(ABin: integer);
var
  I1: integer;
begin
  if Assigned(RpDev.Bins) then begin
    for I1 := 0 to RpDev.Bins.Count - 1 do begin
      if integer(RpDev.Bins.Objects[I1]) = ABin then begin
        SelectBin(RpDev.Bins[I1]);
      end;
    end; { for }
  end; { if }
end;

procedure TRvRenderPrinter.PrintData(Value: string);
begin { PrintData }
  if Value = '' then Exit;
  RpDev.RawOut(Value[1],Length(Value));
end;  { PrintData }

function TRvRenderPrinter.XI2D(Pos: double): integer;
begin
  Result := Round((Pos - (RpDev.Waste.Left / XDPI)) * XDPI);
end;

function TRvRenderPrinter.YI2D(Pos: double): integer;
begin
  Result := Round((Pos - (RpDev.Waste.Top / XDPI)) * YDPI);
end;

procedure TRvRenderPrinter.PrintDataStream(Stream: TStream;
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
        RpDev.RawOut(Buffer^.Data,Buffer^.Size);
      end; { if }
    end; { for }
  finally
    Dispose(Buffer);
  end; { tryf }
end;  { PrintDataStream }

end.