{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpTChart;

interface

uses
  Windows, Graphics,
  Classes, Chart, RpBase, RpCon;

  procedure WriteChartData(Connection: TRvCustomConnection; Chart: TCustomChart);
  procedure PrintTeeChart(Report: TBaseReport; X1, Y1, X2, Y2: double; Chart: TCustomChart);
  function CalcTeeChartWidth(Height: double; Chart: TCustomChart): double;
  function CalcTeeChartHeight(Width: double; Chart: TCustomChart): double;

implementation

procedure WriteChartData(Connection: TRvCustomConnection; Chart: TCustomChart);
var
  MetaFile: TMetaFile;
  Stream: TMemoryStream;
begin { WriteChartData }
  MetaFile := Chart.TeeCreateMetafile(false,Chart.GetRectangle);
  Stream := TMemoryStream.Create;
  try
    MetaFile.SaveToStream(Stream);
    Connection.WriteBlobData(Stream.Memory^,Stream.Size);
  finally
    Stream.Free;
    MetaFile.Free;
  end; { tryf }
end;  { WriteChartData }

procedure PrintTeeChart(Report: TBaseReport; X1, Y1, X2, Y2: double; Chart: TCustomChart);
var
  MetaFile: TMetaFile;
begin { PrintTeeChart }
  MetaFile := Chart.TeeCreateMetafile(false,Chart.GetRectangle);
  try
    Report.StretchDraw(Report.CreateRect(X1,Y1,X2,Y2),MetaFile);
  finally
    MetaFile.Free;
  end; { tryf }
end;  { PrintTeeChart }

function CalcTeeChartWidth(Height: double; Chart: TCustomChart): double;
begin { CalcTeeChartWidth }
  Result := Chart.Width * Height / Chart.Height;
end;  { CalcTeeChartWidth }

function CalcTeeChartHeight(Width: double; Chart: TCustomChart): double;
begin { CalcTeeChartHeight }
  Result := Chart.Height * Width / Chart.Width;
end;  { CalcTeeChartHeight }

end.
