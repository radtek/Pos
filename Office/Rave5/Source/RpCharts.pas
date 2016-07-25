{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpCharts;

interface

uses
  SysUtils, Classes, Graphics, RpBase;

{

  Pie: Name:Value


  Bar: Name[:Name]:Value

    Name:Value - Simple bars
    Name:Name:Value - 2-D bar array
    Name:Value - Min/Max bars


  Point: Name:Value:Value

    Name:Value:Value - XY points


  Line: Name[:Name]:Value

    Name:Value - Single line
    Name:Name:Value - Group of lines


  Layer: Name[:Name]:Value

}

const
  ChartColors = 20;
  ChartColor: array[1..ChartColors] of TColor = (
   clBlue,
   clRed,
   clLime,
   clFuchsia,
   clAqua,
   clYellow,
   TColor($FF0080), { LightPurple }
   TColor($0080FF), { Orange }
   TColor($FF8000), { GreenBlue }
   TColor($C080FF), { BubbleGum }
   TColor($004080), { Brown }
   TColor($C08080), { Periwinkle }
   TColor($8000FF), { VioletRed }
   clMaroon,
   clGreen,
   clNavy,
   TColor($FF8080), { Lavender }
   clPurple,
   clTeal,
   clOlive);

type
  TRpChartsBase = class;

  TPointData = class
  protected
    FValue: double;
  public
    constructor Create(AValue: double);
    procedure Add(AValue: double);
    function TotalPerc(AChart: TRpChartsBase): double;
    function RangePerc(AChart: TRpChartsBase): double;
    //
    property Value: double read FValue write FValue;
  end; { TPointData }

  TValueFunc = (vfTotal,vfRange,vfAverage);

  TValueList = class(TStringList)
  protected
    FValue1: double;
    FValue2: double;
    FValueFunc: TValueFunc;
  public
    procedure AddValue(AName: string; AValue: double);
    //
    property Value1: double read FValue1 write FValue1;
    property Value2: double read FValue2 write FValue2;
    property ValueFunc: TValueFunc read FValueFunc write FValueFunc;
  end; { TValueList }

  TRpSeries = class
  protected
    FValueList: TStringList;
  public
    constructor Create;
    destructor Destroy; override;
    procedure Add(AName: string;
                  AValue: double);
  end; { TRpSeries }

  TRpChartsBase = class
  protected
    FDraw3D: boolean;
    FBaseReport: TBaseReport;
    FLeft: double;
    FTop: double;
    FWidth: double;
    FHeight: double;
    FFillColor: TColor;
    FPenColor: TColor;
    FPoints: TStringList;
    FPointsTotal: double;
    FPointsMax: double;
    FPointsMin: double;
    FShade: integer;
    FSeries: TRpSeries;
    //
    procedure CalcAngles( X1,Y1,X2,Y2,A1,A2: double;
                         var X3,Y3,X4,Y4: double);
    procedure Rectangle(X1,Y1,X2,Y2: double);
    procedure Arc(X1,Y1,X2,Y2,A1,A2: double);
    procedure Pie(X1,Y1,X2,Y2,A1,A2: double);
    procedure MoveTo(X1,Y1: double);
    procedure LineTo(X1,Y1: double);
    procedure SetMinMax(AValue: double);
  public
    constructor Create(ABaseReport: TBaseReport); virtual;
    destructor Destroy; override;
    procedure Print; virtual; abstract;
    procedure Clear;
    procedure AddPoint(AName: string; AValue: double);
    function Series(AIndex: integer = 0): TRpSeries;
    //
    property Left: double read FLeft write FLeft;
    property Top: double read FTop write FTop;
    property Width: double read FWidth write FWidth;
    property Height: double read FHeight write FHeight;
    property Draw3D: boolean read FDraw3D write FDraw3D;
    property FillColor: TColor read FFillColor write FFillColor;
    property PenColor: TColor read FPenColor write FPenColor;
    property PointsTotal: double read FPointsTotal;
    property PointsMin: double read FPointsMin;
    property PointsMax: double read FPointsMax;
    property Shade: integer read FShade write FShade;
  end; { TRpChartsBase }

  TRpChartsPie = class(TRpChartsBase)
  protected
  public
    procedure Print; override;
  end; { TRpChartsPie }

  TRpChartsBar = class(TRpChartsBase)
  protected
  public
    procedure Print; override;
  end; { TRpChartsBar }

  TRpChartsLine = class(TRpChartsBase)
  protected
  public
    procedure Print; override;
  end; { TRpChartsBar }

implementation

uses
  RpDefine;

{ TPointData }

procedure TPointData.Add(AValue: double);
begin
  FValue := FValue + AValue;
end;

constructor TPointData.Create(AValue: double);
begin
  inherited Create;
  FValue := AValue;
end;

function TPointData.RangePerc(AChart: TRpChartsBase): double;
var
  F1: double;
begin
  F1 := AChart.PointsMax - AChart.PointsMin;
  if F1 = 0 then begin
    Result := 1.0;
  end else begin
    Result := (Value - AChart.PointsMin) / F1;
  end; { else }
end;

function TPointData.TotalPerc(AChart: TRpChartsBase): double;
begin
  Result := Value / AChart.PointsTotal;
end;

{ TValueList }

procedure TValueList.AddValue(AName: string; AValue: double);
begin
  case ValueFunc of
    vfTotal,vfAverage: begin
      Value1 := Value1 + AValue;
      Value2 := Value2 + 1.0;
    end;
    vfRange: begin
      if AValue < Value1 then begin
        Value1 := AValue;
      end; { if }
      if AValue > Value2 then begin
        Value2 := AValue;
      end; { if }
    end;
  end; { case }
end;

{ TRpSeries }

procedure TRpSeries.Add(AName: string; AValue: double);
begin

end;

constructor TRpSeries.Create;
begin
  inherited;
  FValueList := TStringList.Create;
end;

destructor TRpSeries.Destroy;
begin
  FreeAndNil(FValueList);
  inherited;
end;

{ TRpChartsBase }

procedure TRpChartsBase.AddPoint(AName: string; AValue: double);
var
  I1: integer;
begin
  FPointsTotal := FPointsTotal + AValue;
  for I1 := 0 to FPoints.Count - 1 do begin
    if SameText(FPoints[I1],AName) then begin
    // Add to existing point
      TPointData(FPoints.Objects[I1]).Add(AValue);
      SetMinMax(TPointData(FPoints.Objects[I1]).Value);
      Exit;
    end; { if }
  end; { for }

// Add to new point
  FPoints.AddObject(AName,TPointData.Create(AValue));
  SetMinMax(AValue);
end;

procedure TRpChartsBase.Arc(X1, Y1, X2, Y2, A1, A2: double);
var
  X3,Y3: double;
  X4,Y4: double;
begin
  FBaseReport.SetBrush(FillColor,bsSolid,nil);
  FBaseReport.SetPen(PenColor,psSolid,1,pmCopy);
  CalcAngles(X1,Y1,X2,Y2,A1,A2,X3,Y3,X4,Y4);
  FBaseReport.Arc(X1,Y1,X2,Y2,X3,Y3,X4,Y4);
end;

procedure TRpChartsBase.CalcAngles( X1, Y1, X2, Y2, A1, A2: double;
                                   var X3, Y3, X4, Y4: double);
var
  XC,YC: double;
  XR,YR: double;
begin
  XR := Abs(X2 - X1) / 2.0;
  YR := Abs(Y2 - Y1) / 2.0;
  XC := Abs(X1 + X2) / 2.0;
  YC := Abs(Y1 + Y2) / 2.0;
  X3 := XC + XR * Cos(A1 * Pi / 180.0);
  Y3 := YC - YR * Sin(A1 * Pi / 180.0);
  X4 := XC + XR * Cos(A2 * Pi / 180.0);
  Y4 := YC - YR * Sin(A2 * Pi / 180.0);
end;

procedure TRpChartsBase.Clear;
var
  I1: integer;
begin
  FPointsTotal := 0;
  FPointsMax := 5.0E-324;
  FPointsMin := 1.7E308;
  for I1 := 0 to FPoints.Count - 1 do begin
    FPoints.Objects[I1].Free;
    FPoints.Objects[I1] := nil;
  end; { for }
  FPoints.Clear;
end;

constructor TRpChartsBase.Create(ABaseReport: TBaseReport);
begin
  inherited Create;
  FBaseReport := ABaseReport;
  FillColor := clWhite;
  PenColor := clBlack;
  FPoints := TStringList.Create;
  FShade := 100;
end;

destructor TRpChartsBase.Destroy;
begin
  Clear;
  FreeAndNil(FPoints);
  inherited;
end;

procedure TRpChartsBase.LineTo(X1, Y1: double);
begin
  FBaseReport.SetPen(PenColor,psSolid,1,pmCopy);
  FBaseReport.LineTo(X1,Y1);
end;

procedure TRpChartsBase.MoveTo(X1, Y1: double);
begin
  FBaseReport.SetPen(PenColor,psSolid,1,pmCopy);
  FBaseReport.MoveTo(X1,Y1);
end;

procedure TRpChartsBase.Pie(X1, Y1, X2, Y2, A1, A2: double);
var
  X3,Y3: double;
  X4,Y4: double;
  Diff: double;
  Recalc: boolean;
begin
  FBaseReport.SetBrush(FillColor,bsSolid,nil);
  FBaseReport.SetPen(PenColor,psSolid,1,pmCopy);
  if Draw3D then begin
    Diff := (Y2 - Y1) * 0.1;
    CalcAngles(X1,Y1,X2,Y2 - Diff,A1,A2,X3,Y3,X4,Y4);
    FBaseReport.Pie(X1,Y1,X2,Y2 - Diff,X3,Y3,X4,Y4);

    if (A1 < 360) and (A2 > 180) then begin
      Recalc := false;
      if A1 < 180 then begin
        A1 := 180;
        Recalc := true;
      end; { if }
      if A2 > 360 then begin
        A2 := 360;
        Recalc := true;
      end; { if }
      if Recalc then begin
        CalcAngles(X1,Y1,X2,Y2 - Diff,A1,A2,X3,Y3,X4,Y4);
      end; { if }
      FBaseReport.Arc(X1,Y1 + Diff,X2,Y2,X3,Y3 + Diff,X4,Y4 + Diff);
      FBaseReport.MoveTo(X3,Y3);
      FBaseReport.LineTo(X3,Y3 + Diff);
      FBaseReport.MoveTo(X4,Y4);
      FBaseReport.LineTo(X4,Y4 + Diff);
    end; { if }
  end else begin
    CalcAngles(X1,Y1,X2,Y2,A1,A2,X3,Y3,X4,Y4);
    FBaseReport.Pie(X1,Y1,X2,Y2,X3,Y3,X4,Y4);
  end; { if }
end;

procedure TRpChartsBase.Rectangle(X1, Y1, X2, Y2: double);
begin
  FBaseReport.SetBrush(FillColor,bsSolid,nil);
  FBaseReport.SetPen(PenColor,psSolid,1,pmCopy);
  FBaseReport.Rectangle(X1,Y1,X2,Y2);
end;

function TRpChartsBase.Series(AIndex: integer): TRpSeries;
begin
  Result := FSeries;
end;

procedure TRpChartsBase.SetMinMax(AValue: double);
begin
  if AValue > FPointsMax then begin
    FPointsMax := AValue;
  end; { if }
  if AValue < FPointsMin then begin
    FPointsMin := AValue;
  end; { if }
end;

{ TRpChartsPie }

procedure TRpChartsPie.Print;
var
  X1,Y1,X2,Y2: double;
  I1: integer;
  Perc1, Perc2: double;
begin
  X1 := Left;
  Y1 := Top;
  X2 := Left + Width;
  Y2 := Top + Width;
  Perc2 := 0;
  for I1 := 0 to FPoints.Count - 1 do begin
    Perc1 := Perc2;
    Perc2 := Perc1 + TPointData(FPoints.Objects[I1]).TotalPerc(self) * 360;
    FillColor := ShadeToColor(ChartColor[I1 + 1],Shade);
    Pie(X1,Y1,X2,Y2,Perc1,Perc2);
  end; { for }
end;

{ TRpChartsBar }

procedure TRpChartsBar.Print;
var
  X1,Y1,X2,Y2: double;
  XD: double;
  I1: integer;
begin
  X1 := Left;
  Y1 := Top;
  X2 := Left + Width;
  Y2 := Top + Width;
  XD := (X2 - X1) / FPoints.Count;
  for I1 := 0 to FPoints.Count - 1 do begin
    FillColor := ChartColor[I1 + 1];
    Rectangle(X1 + (I1 - 1) * XD,Y2 - TPointData(FPoints.Objects[I1]).RangePerc(self) * (Y2 - Y1),
     X1 + I1 * XD,Y2);
  end; { for }
end;

{ TRpChartsLine }

procedure TRpChartsLine.Print;
var
  X1,Y1,X2,Y2: double;
  XD: double;
  YD: double;
  I1: integer;
begin
  X1 := Left;
  Y1 := Top;
  X2 := Left + Width;
  Y2 := Top + Width;
  XD := (X2 - X1) / (FPoints.Count - 1);

  MoveTo(X1,Y1);
  LineTo(X1,Y2);
  LineTo(X2,Y2);
  for I1 := 0 to FPoints.Count - 1 do begin
    YD := Y2 - TPointData(FPoints.Objects[I1]).RangePerc(self) * (Y2 - Y1);
    if I1 = 0 then begin
      MoveTo(X1,YD);
    end else begin
      LineTo(X1 + I1 * XD,YD);
    end; { else }
  end; { for }
end;

end.
