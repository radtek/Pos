{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvCsData;

interface

uses
  Messages, Windows, Controls, Graphics, Dialogs, Buttons, StdCtrls, Forms,
  Classes, SysUtils, TypInfo, RvDefine, RvUtil, RvClass, RvCsStd,
  RvData, RpDefine, RpBase, RvProj, RvCsRpt, RvDataField;

type
  TRaveDataText = class(TRaveText)
  protected
    FDataField: TRaveFieldName;
    FDataView: TRaveBaseDataView;
    FLookupDataView: TRaveBaseDataView;
    FLookupField: TRaveFieldName;
    FLookupDisplay: TRaveFieldName;
    FLookupInvalid: string;
    procedure SetText(Value: string); override;
    function GetText: string; override;
    procedure SetDataField(Value: TRaveFieldName);
    procedure SetLookupDataView(Value: TRaveBaseDataView);
    procedure SetLookupField(Value: TRaveFieldName);
    procedure SetLookupDisplay(Value: TRaveFieldName);
    procedure Changing(OldItem: TRaveComponent; NewItem: TRaveComponent); override;
  public
    constructor Create(AOwner: TComponent); override;
  published
    property Truncate default true;
    property DataField: TRaveFieldName read FDataField write SetDataField;
    property DataView: TRaveBaseDataView read FDataView write FDataView;
    property LookupDataView: TRaveBaseDataView read FLookupDataView write SetLookupDataView default nil;
    property LookupField: TRaveFieldName read FLookupField write SetLookupField;
    property LookupDisplay: TRaveFieldName read FLookupDisplay write SetLookupDisplay;
    property LookupInvalid: string read FLookupInvalid write FLookupInvalid;
  end; { TRaveDataText }

  TRaveDataMemo = class(TRaveMemo)
  protected
    FDataField: TRaveFieldName;
    FDataView: TRaveBaseDataView;

    function GetText: string; override;
    procedure SetDataField(Value: TRaveFieldName);
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
  public
  published
    property ContainsRTF;
    property DataField: TRaveFieldName read FDataField write SetDataField;
    property DataView: TRaveBaseDataView read FDataView write FDataView;
  end; { TRaveDataMemo }

  TRaveDisplayType = (dtNumericFormat,dtDateTimeFormat);

  TRaveCalcFunction = (cfNone,cfTrunc,cfFrac,cfRound,cfNeg,cfAbs,cfSin,cfCos,cfArcTan,
   cfSqr,cfSqrt,cfInc,cfDec,cfRandom,cfRound1,cfRound2,cfRound3,cfRound4,cfRound5,
   cfSecsToTime,cfMinsToTime,cfHoursToTime,cfTimeToSecs,cfTimeToMins,cfTimeToHours,cfPercent);

  TRaveCalcOperator = (coAdd,coSub,coMul,coDiv,coMod,coExp,coGreater,coLesser,coAverage);

  TRaveCalcText = class(TRaveText)
  protected
    Count: integer;
    Total: extended;

    FController: TRaveComponent;
    FInitializer: TRaveComponent;
    FDataView: TRaveBaseDataView;
    FDataField: TRaveFieldName;
    FCalcType: TRaveCalcType;
    FRunningTotal: boolean;
    FDisplayFormat: string;
    FDisplayType: TRaveDisplayType;
    FCountBlanks: boolean;
    FCountNulls: boolean;
    FCountValue: string;
    FOnCalcValue: TRaveFloatEvent;

    procedure BeforeReport; override;
    procedure SetText(Value: string); override;
    function GetText: string; override;
    function GetValue: extended;
    procedure SetDataField(Value: TRaveFieldName);
    procedure SetController(Value: TRaveComponent);
    procedure SetInitializer(Value: TRaveComponent);
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
    procedure CalcNewDataListen(Speaker: TRaveComponent;
                                Msg: TObject);
    procedure CalcInitDataListen(Speaker: TRaveComponent;
                                 Msg: TObject);
    procedure OverrideProperties(Filer: TRaveFiler); override;
    procedure ReadControllerBand(Reader: TRaveReader);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Print(Report: TBaseReport); override;

    property Value: extended read GetValue;
  published
    property CalcType: TRaveCalcType read FCalcType write FCalcType default ctSum;
    property Controller: TRaveComponent read FController write
     SetController default nil;
    property CountBlanks: boolean read FCountBlanks write FCountBlanks default true;
    property CountNulls: boolean read FCountNulls write FCountNulls default false;
    property CountValue: string read FCountValue write FCountValue;
    property DataView: TRaveBaseDataView read FDataView write FDataView default nil;
    property DataField: TRaveFieldName read FDataField write SetDataField;
    property DisplayFormat: string read FDisplayFormat write FDisplayFormat;
    property DisplayType: TRaveDisplayType read FDisplayType write FDisplayType
     default dtNumericFormat;
    property Initializer: TRaveComponent read FInitializer write
     SetInitializer default nil;
    property RunningTotal: boolean read FRunningTotal write FRunningTotal
     default false;
    property OnCalcValue: TRaveFloatEvent read FOnCalcValue write FOnCalcValue;
  end; { TRaveCalcText }

  TRaveCalcVariable = class(TRaveComponent)
  protected
    FDestParam: string;
    FDestPIVar: string;
    FDisplayFormat: string;
    FDisplayType: TRaveDisplayType;
    FOnGetValue: TRaveFloatEvent;

    function GetValue: extended; virtual; abstract;
    function GetIsNull: boolean; virtual;
  public
    procedure Print(Report: TBaseReport); override;

    property Value: extended read GetValue;
    property IsNull: boolean read GetIsNull;
    property DestParam: string read FDestParam write FDestParam;
    property DestPIVar: string read FDestPIVar write FDestPIVar;
    property DisplayFormat: string read FDisplayFormat write FDisplayFormat;
    property DisplayType: TRaveDisplayType read FDisplayType write FDisplayType
     default dtNumericFormat;
    property OnGetValue: TRaveFloatEvent read FOnGetValue write FOnGetValue;
  end; { TRaveCalcVariable }

  TRaveCalcOp = class(TRaveCalcVariable)
  protected
    FSrc1CalcVar: TRaveCalcVariable;
    FSrc1DataField: TRaveFieldName;
    FSrc1DataView: TRaveBaseDataView;
    FSrc1Value: extended;
    FSrc1Function: TRaveCalcFunction;
    FSrc2CalcVar: TRaveCalcVariable;
    FSrc2DataField: TRaveFieldName;
    FSrc2DataView: TRaveBaseDataView;
    FSrc2Value: extended;
    FSrc2Function: TRaveCalcFunction;
    FOperator: TRaveCalcOperator;
    FResultFunction: TRaveCalcFunction;

    function GetIsNull: boolean; override;
    function GetValue: extended; override;
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
  published
    property DestParam;
    property DestPIVar;
    property DisplayFormat;
    property DisplayType;
    property Operator: TRaveCalcOperator read FOperator write FOperator
     default coAdd;
    property ResultFunction: TRaveCalcFunction read FResultFunction write
     FResultFunction default cfNone;
    property Src1CalcVar: TRaveCalcVariable read FSrc1CalcVar write FSrc1CalcVar default nil;
    property Src1DataField: TRaveFieldName read FSrc1DataField write FSrc1DataField;
    property Src1DataView: TRaveBaseDataView read FSrc1DataView write FSrc1DataView default nil;
    property Src1Value: extended read FSrc1Value write FSrc1Value;
    property Src1Function: TRaveCalcFunction read FSrc1Function
     write FSrc1Function default cfNone;
    property Src2CalcVar: TRaveCalcVariable read FSrc2CalcVar write FSrc2CalcVar default nil;
    property Src2DataField: TRaveFieldName read FSrc2DataField write FSrc2DataField;
    property Src2DataView: TRaveBaseDataView read FSrc2DataView write FSrc2DataView default nil;
    property Src2Value: extended read FSrc2Value write FSrc2Value;
    property Src2Function: TRaveCalcFunction read FSrc2Function
     write FSrc2Function default cfNone;
    property OnGetValue;
  end; { TRaveCalcOp }

  TRaveCalcTotal = class(TRaveCalcVariable)
  protected
    Count: integer;
    Total: extended;

    FCalcVar: TRaveCalcVariable;
    FDataView: TRaveBaseDataView;
    FDataField: TRaveFieldName;
    FController: TRaveComponent;
    FInitializer: TRaveComponent;
    FCalcType: TRaveCalcType;
    FCountBlanks: boolean;
    FCountNulls: boolean;
    FCountValue: string;
    FRunningTotal: boolean;
    FOnCalcValue: TRaveFloatEvent;

    procedure BeforeReport; override;
    procedure SetController(Value: TRaveComponent);
    procedure SetInitializer(Value: TRaveComponent);
    function GetValue: extended; override;
    procedure CalcNewDataListen(Speaker: TRaveComponent;
                                Msg: TObject);
    procedure CalcInitDataListen(Speaker: TRaveComponent;
                                 Msg: TObject);
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Print(Report: TBaseReport); override;
  published
    property CalcType: TRaveCalcType read FCalcType write FCalcType default ctSum;
    property CalcVar: TRaveCalcVariable read FCalcVar write FCalcVar default nil;
    property Controller: TRaveComponent read FController write SetController default nil;
    property CountBlanks: boolean read FCountBlanks write FCountBlanks default true;
    property CountNulls: boolean read FCountNulls write FCountNulls default false;
    property CountValue: string read FCountValue write FCountValue;
    property DataView: TRaveBaseDataView read FDataView write FDataView default nil;
    property DataField: TRaveFieldName read FDataField write FDataField;
    property DestParam;
    property DestPIVar;
    property DisplayFormat;
    property DisplayType;
    property Initializer: TRaveComponent read FInitializer write SetInitializer default nil;
    property RunningTotal: boolean read FRunningTotal write FRunningTotal default false;
    property OnCalcValue: TRaveFloatEvent read FOnCalcValue write FOnCalcValue;
    property OnGetValue;
  end; { TRaveCalcTotal }

  TRaveCalcController = class(TRaveComponent)
  protected
    CalcNewDataListenList: TRaveMethodList;
    CalcInitDataListenList: TRaveMethodList;
    FInitCalcVar: TRaveCalcVariable;
    FInitDataField: TRaveFieldName;
    FInitValue: extended;

    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure AddListener(Conversation: string;
                          ListenMethod: TRaveListenEvent); override;
    procedure RemoveListener(Conversation: string;
                             ListenMethod: TRaveListenEvent); override;
    function Habla(Conversation: string): boolean; override;
    procedure Print(Report: TBaseReport); override;
  published
    property InitCalcVar: TRaveCalcVariable read FInitCalcVar write FInitCalcVar;
    property InitDataField: TRaveFieldName read FInitDataField write FInitDataField;
    property InitValue: extended read FInitValue write FInitValue;
  end; { TRaveCalcController }

  TRaveDataMirror = class(TPersistent)
  protected
    FDataValue: string;
    FSectionMirror: TRaveSection;
    FIsDefault: boolean;
  published
    property DataValue: string read FDataValue write FDataValue;
    property IsDefault: boolean read FIsDefault write FIsDefault default false;
    property SectionMirror: TRaveSection read FSectionMirror write FSectionMirror
     default nil;
  end; { TRaveDataMirror }

  TRaveDataMirrorSection = class(TRaveSection)
  protected
    Initialized: boolean;
    DirectReference: boolean;
    FDataField: TRaveFieldName;
    FDataView: TRaveBaseDataView;
    FDataMirrors: TRavePersistentList;
    FCompareCase: boolean;
    FSaveMirror: TRaveControl;

  { State saving variables }
    FSaveAnchor: TRaveAnchor;
    FSaveCompareCase: boolean;
    FSaveDataField: TRaveFieldName;
    FSaveDataMirrors: TRavePersistentList;
    FSaveDataView: TRaveBaseDataView;
    FSaveDisplayOn: TDisplayOn;
    FSaveTag: longint;
    FOnMirrorValue: TRaveStringEvent;

    procedure SetDataMirrors(Value: TRavePersistentList);
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
    procedure InitData; override;
    procedure DoneData; override;
    procedure SaveDesigned; override;
    procedure RestoreDesigned; override;
    procedure RestoreAdjusted; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure PrintAll(Report: TBaseReport); override;
    procedure Paint(Canvas: TCanvas); override;
  published
    property CompareCase: boolean read FCompareCase write FCompareCase default false;
    property DataField: TRaveFieldName read FDataField write FDataField;
    property DataView: TRaveBaseDataView read FDataView write FDataView;
    property DataMirrors: TRavePersistentList read FDataMirrors write SetDataMirrors;
    property OnMirrorValue: TRaveStringEvent read FOnMirrorValue write FOnMirrorValue;
  end; { TRaveDataMirrorSection }

  function CalcFunction(Value: extended;
                        Func: TRaveCalcFunction): extended;
  function CalcVariable(    Self: TRaveComponent;
                            CalcVar: TRaveCalcVariable;
                            DataView: TRaveBaseDataView;
                            DataField: TRaveFieldName;
                            Value: extended;
                            Func: TRaveCalcFunction;
                        var DoCount: boolean): extended;
  function CreateDataTextControl(AOwner: TComponent;
                                 DataField: TRaveDataField): TRaveControl; far;
  function CreateDataMemoControl(AOwner: TComponent;
                                 DataField: TRaveDataField): TRaveControl; far;
  procedure RaveRegister;

implementation

function CreateDataTextControl(AOwner: TComponent;
                               DataField: TRaveDataField): TRaveControl; far;
begin { CreateDataTextControl }
  Result := TRaveDataText.Create(AOwner);
  TRaveDataText(Result).DataView := TRaveBaseDataView(DataField.Parent);
  TRaveDataText(Result).DataField := DataField.FieldName;
end;  { CreateDataTextControl }

function CreateDataMemoControl(AOwner: TComponent;
                               DataField: TRaveDataField): TRaveControl; far;
begin { CreateDataTextControl }
  Result := TRaveDataMemo.Create(AOwner);
  TRaveDataMemo(Result).DataView := TRaveBaseDataView(DataField.Parent);
  TRaveDataMemo(Result).DataField := DataField.FieldName;
end;  { CreateDataTextControl }

procedure RaveRegister;
begin { RaveRegister }
  RegisterRaveGroup({Trans-}'Report',Trans('Report'));
  RegisterRaveComponents({Trans-}'Report',[TRaveDataText,TRaveDataMemo,TRaveCalcText,
   TRaveDataMirrorSection,TRaveRegion,TRaveBand,TRaveDataBand,TRaveDataCycle,
   TRaveCalcOp,TRaveCalcTotal,TRaveCalcController]);
  RegisterRaveModuleClasses({Trans-}'RvCsData',[TRaveDataText,TRaveDataMemo,
   TRaveCalcText,TRaveDataMirrorSection,TRaveCalcOp,TRaveCalcTotal,
   TRaveCalcController]);
  RegisterRaveModuleClasses({Trans-}'RvCsRpt',[TRaveRegion,TRaveBand,TRaveDataBand,
   TRaveDataCycle]);
end;  { RaveRegister }

function CalcFunction(Value: extended;
                      Func: TRaveCalcFunction): extended;
begin { CalcFunction }
  Result := Value;
  try
    case Func of
      cfNone: begin end;
      cfTrunc: Result := Trunc(Result);
      cfFrac: Result := Frac(Result);
      cfRound: Result := Round(Result);
      cfNeg: Result := -Result;
      cfAbs: Result := Abs(Result);
      cfSin: Result := Sin(Result);
      cfCos: Result := Cos(Result);
      cfArcTan: Result := ArcTan(Result);
      cfSqr: Result := Sqr(Result);
      cfSqrt: Result := Sqrt(Result);
      cfInc: Result := Result + 1.0;
      cfDec: Result := Result - 1.0;
      cfRandom: Result := Result * Random;
      cfRound1: Result := Round(Result * 10.0) / 10.0;
      cfRound2: Result := Round(Result * 100.0) / 100.0;
      cfRound3: Result := Round(Result * 1000.0) / 1000.0;
      cfRound4: Result := Round(Result * 10000.0) / 10000.0;
      cfRound5: Result := Round(Result * 100000.0) / 100000.0;
      cfSecsToTime: Result := Result / 86400.0;
      cfMinsToTime: Result := Result / 1440.0;
      cfHoursToTime: Result := Result / 24.0;
      cfTimeToSecs: Result := Result * 86400.0;
      cfTimeToMins: Result := Result * 1440.0;
      cfTimeToHours: Result := Result * 24.0;
      cfPercent: Result := Result * 100.0;
    end; { case }
  except
  end; { tryx }
end;  { CalcFunction }

function CalcVariable(    Self: TRaveComponent;
                          CalcVar: TRaveCalcVariable;
                          DataView: TRaveBaseDataView;
                          DataField: TRaveFieldName;
                          Value: extended;
                          Func: TRaveCalcFunction;
                      var DoCount: boolean): extended;
var
  Field: TRaveDataField;
begin { CalcVariable }
  DoCount := true;
  if Assigned(CalcVar) and not CalcVar.IsNull then begin
    Result := CalcVar.Value;
  end else if DataField <> '' then begin
    if Assigned(DataView) then begin
      Field := DataView.FieldByName(DataField);
    end else begin
      Field := nil;
    end; { else }
    if Assigned(Field) then begin
      Result := Field.AsFloat;
      DoCount := not Field.IsNull;
    end else begin
      Result := StrToFloatDef(StripFloatStr(ProcessDataStr(Self,DataView,
       DataField)),0);
    end; { else }
  end else begin
    Result := Value;
  end; { else }
  Result := CalcFunction(Result,Func);
end;  { CalcVariable }

{ class TRaveDataText }

  constructor TRaveDataText.Create(AOwner: TComponent);
  begin { Create }
    inherited Create(AOwner);
    FTruncate := true;
    if InDesigner then begin
      BracketText := true;
    end; { if }
  end;  { Create }

  procedure TRaveDataText.Changing(OldItem: TRaveComponent;
                                   NewItem: TRaveComponent);
  begin { Changing }
    inherited Changing(OldItem,NewItem);
    if Assigned(DataView) and (DataView = OldItem) then begin
      DataView := NewItem as TRaveBaseDataView;
    end; { if }
    if Assigned(LookupDataView) and (LookupDataView = OldItem) then begin
      LookupDataView := NewItem as TRaveBaseDataView;
    end; { if }
  end;  { Changing }

  procedure TRaveDataText.SetText(Value: string);
  begin { SetText }
  end;  { SetText }

function TRaveDataText.GetText: string;
var
  Field: TRaveDataField;
begin { GetText }
  if TRaveProjectManager(Project).Printing then begin { Get from DataView }
    Result := ProcessDataStr(self,DataView,DataField);
    if Assigned(LookupDataView) and (LookupField <> '') and
     (LookupDisplay <> '') then begin
      if LookupDataView = DataView then begin
        Result := ProcessDataStr(self,DataView,LookupDisplay);
      end else begin
        if Assigned(DataView) then begin
          Field := DataView.FieldByName(DataField);
        end else begin
          Field := nil;
        end; { else }
        Result := PerformLookup(LookupDataView,Result,Field,LookupField,
         LookupDisplay,LookupInvalid);
      end; { else }
    end; { if }
  end else begin
    if DataField = '' then begin
      Result := Name;
    end else if Assigned(LookupDataView) and (LookupField <> '') and
     (LookupDisplay <> '') then begin
      Result := DataField + #187 + LookupDataView.Name + '.' +
       LookupDisplay;
    end else begin
      Result := DataField;
    end; { else }
  end; { else }
end;  { GetText }

procedure TRaveDataText.SetDataField(Value: TRaveFieldName);
begin { SetDataField }
  FDataField := Value;
  Invalidate;
end;  { SetDataField }

procedure TRaveDataText.SetLookupDataView(Value: TRaveBaseDataView);
begin { SetLookupDataView }
  FLookupDataView := Value;
  Invalidate;
end;  { SetLookupDataView }

procedure TRaveDataText.SetLookupField(Value: TRaveFieldName);
begin { SetLookupField }
  FLookupField := Value;
  Invalidate;
end;  { SetLookupField }

procedure TRaveDataText.SetLookupDisplay(Value: TRaveFieldName);
begin { SetLookupDisplay }
  FLookupDisplay := Value;
  Invalidate;
end;  { SetLookupDisplay }

{ class TRaveDataMemo }

procedure TRaveDataMemo.SetDataField(Value: TRaveFieldName);
begin { SetDataField }
  if InDesigner and Assigned(LineList) then begin { Clear out memo lines }
    FreeAndNil(LineList);
  end; { if }
  FDataField := Value;
  Invalidate;
end;  { SetDataField }

procedure TRaveDataMemo.Changing(OldItem: TRaveComponent;
                                 NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(DataView) and (DataView = OldItem) then begin
    DataView := NewItem as TRaveBaseDataView;
  end; { if }
end;  { Changing }

function TRaveDataMemo.GetText: string;
begin { GetText }
  if TRaveProjectManager(Project).Printing then begin { Get from DataView }
    Result := ProcessDataStr(self,DataView,DataField);
  end else begin
    if DataField = '' then begin
      Result := '[' + Name + ']';
    end else begin
      Result := '[' + DataField + ']';
    end; { else }
  end; { else }
end;  { GetText }

{ CalcText and CalcTotal common functions }

function GetCalcValue(Total: extended;
                      Count: integer;
                      CalcType: TRaveCalcType): extended;
begin { GetCalcValue }
  case CalcType of
    ctCount: Result := Count;
    ctSum: Result := Total;
    ctMin,ctMax: begin
      if Count = 0 then begin
        Result := 0.0;
      end else begin
        Result := Total;
      end; { else }
    end;
    ctAverage: begin
      if Count = 0 then begin
        Result := 0.0;
      end else begin
        Result := Total / Count;
      end; { else }
    end;
    else Result := 0.0;
  end; { case }
end;  { GetCalcValue }

procedure CalcNewData(var Total: extended;
                      var Count: integer;
                          CalcType: TRaveCalcType;
                          CountBlanks: boolean;
                          CountValue: string;
                          NewData: extended;
                          NewDataStr: string);
begin { CalcNewData }
  case CalcType of
    ctCount: begin
      if ((CountBlanks or (NewData <> 0.0)) and (CountValue = '')) or
       (NewDataStr = CountValue) then begin
        Inc(Count);
      end; { if }
    end;
    ctSum: begin
      Total := Total + NewData;
    end;
    ctMin: begin
      if (Count = 0) or (NewData < Total) then begin
        Count := 1;
        Total := NewData;
      end; { if }
    end;
    ctMax: begin
      if (Count = 0) or (NewData > Total) then begin
        Count := 1;
        Total := NewData;
      end; { if }
    end;
    ctAverage: begin
      Total := Total + NewData;
      if CountBlanks or (NewData <> 0.0) then begin
        Inc(Count);
      end; { if }
    end;
  end; { case }
end;  { CalcNewData }

procedure CalcInitData(var Total: extended;
                       var Count: integer;
                           CalcType: TRaveCalcType;
                           InitData: extended);
begin { CalcInitData }
  case CalcType of
    ctCount: begin
      Count := Trunc(InitData);
      Total := 0.0;
    end;
    ctSum: begin
      Count := 0;
      Total := InitData;
    end;
    ctMin,ctMax,ctAverage: begin
      Count := 0;
      Total := 0.0;
    end;
  end; { case }
end;  { CalcInitData }

{ class TRaveCalcText }

constructor TRaveCalcText.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FCalcType := ctSum;
  FCountBlanks := true;
  if InDesigner then begin
    BracketText := true;
  end; { if }
end;  { Create }

destructor TRaveCalcText.Destroy;
begin { Destroy }
  inherited Destroy;
end;  { Destroy }

procedure TRaveCalcText.Changing(OldItem: TRaveComponent;
                                 NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(DataView) and (DataView = OldItem) then begin
    DataView := NewItem as TRaveBaseDataView;
  end; { if }
  if Assigned(Controller) and (Controller = OldItem) then begin
    Controller := NewItem;
  end; { if }
  if Assigned(Initializer) and (Initializer = OldItem) then begin
    Initializer := NewItem;
  end; { if }
end;  { Changing }

procedure TRaveCalcText.OverrideProperties(Filer: TRaveFiler);
begin { OverrideProperties }
{ ControllerBand was replaced with Controller in version 3.0E }
  Filer.OverrideProperty({Trans-}'ControllerBand',ReadControllerBand,nil);
end;  { OverrideProperties }

procedure TRaveCalcText.ReadControllerBand(Reader: TRaveReader);
var
  PropInfo: PPropInfo;
begin { ReadControllerBand }
{ Read old ControllerBand value into Controller property }
  PropInfo := GetPropInfo(ClassInfo,{Trans-}'Controller');
  Reader.ReadProperty(self,PropInfo);
end;  { ReadControllerBand }

procedure TRaveCalcText.SetText(Value: string);
begin { SetText }
end;  { SetText }

function TRaveCalcText.GetText: string;
begin { GetText }
  if TRaveProjectManager(Project).Printing then begin
    if (CalcType = ctAverage) and (Count = 0) then begin
      Result := Trans('undefined');
    end else begin
      if DisplayType = dtNumericFormat then begin
        Result := FormatFloat(DisplayFormat,Value);
      end else begin { dtDateTimeFormat }
        Result := FormatDateTime(DisplayFormat,Value);
      end; { else }
    end; { else }
  end else begin
  {TransOff}
    case CalcType of
      ctCount: Result := 'Count(';
      ctSum: Result := 'Sum(';
      ctMin: Result := 'Min(';
      ctMax: Result := 'Max(';
      ctAverage: Result := 'Avg(';
    end; { else }
  {TransOn}
    if DataField = '' then begin
      Result := Result + Name + ')';
    end else begin
      Result := Result + DataField + ')';
    end; { else }
  end; { else }
end;  { GetText }

function TRaveCalcText.GetValue: extended;
begin { GetValue }
  Result := GetCalcValue(Total,Count,CalcType);
end;  { GetValue }

procedure TRaveCalcText.SetDataField(Value: TRaveFieldName);
begin { SetDataField }
  FDataField := Value;
  Invalidate;
end;  { SetDataField }

procedure TRaveCalcText.SetController(Value: TRaveComponent);
begin { SetController }
  if Value = FController then Exit;
  if Assigned(FController) then begin
    FController.RemoveListener({Trans-}'CalcNewData',CalcNewDataListen);
  end; { if }
  FController := Value;
  if Assigned(FController) then begin
    FController.AddListener({Trans-}'CalcNewData',CalcNewDataListen);
  end; { if }
end;  { SetController }

procedure TRaveCalcText.SetInitializer(Value: TRaveComponent);
begin { SetInitializer }
  if Value = FInitializer then Exit;
  if Assigned(FInitializer) then begin
    FInitializer.RemoveListener({Trans-}'CalcInitData',CalcInitDataListen);
  end; { if }
  FInitializer := Value;
  if Assigned(FInitializer) then begin
    FInitializer.AddListener({Trans-}'CalcInitData',CalcInitDataListen);
  end; { if }
end;  { SetInitializer }

procedure TRaveCalcText.BeforeReport;
begin { BeforeReport }
  CalcInitDataListen(self,nil);
  inherited BeforeReport;
end;  { BeforeReport }

procedure TRaveCalcText.CalcNewDataListen(Speaker: TRaveComponent; Msg: TObject);
var
  Value: extended;
  DoCount: boolean;
begin { CalcNewDataListen }
  Value := CalcVariable(Self,nil,DataView,DataField,0.0,cfNone,DoCount);
  if Assigned(FOnCalcValue) then begin
    FOnCalcValue.Value := Value;
    FOnCalcValue.Execute;
    Value := FOnCalcValue.Value;
  end; { if }
  if DoCount or not (CalcType in [ctCount,ctAverage]) then begin
    CalcNewData(Total,Count,CalcType,CountBlanks,CountValue,
     Value,ProcessDataStr(self,DataView,DataField));
  end; { if }
end;  { CalcNewDataListen }

procedure TRaveCalcText.CalcInitDataListen(Speaker: TRaveComponent;
                                           Msg: TObject);
var
  InitData: extended;
begin { CalcInitDataListen }
  if Assigned(Msg) then begin
    InitData := extended(pointer(Msg)^);
  end else begin
    InitData := 0.0;
  end; { else }
  CalcInitData(Total,Count,CalcType,InitData);
end;  { CalcInitDataListen }

procedure TRaveCalcText.Print(Report: TBaseReport);
begin { Print }
  if not AllowPrint then Exit;
  inherited Print(Report);
  if not RunningTotal then begin
    CalcInitDataListen(self,nil);
  end; { if }
end;  { Print }

{ class TRaveCalcVariable }

function TRaveCalcVariable.GetIsNull: boolean;
begin { GetIsNull }
  Result := false;
end;  { GetIsNull }

procedure TRaveCalcVariable.Print(Report: TBaseReport);
var
  S1: string;
begin { Print }
  if (DestParam <> '') or (DestPIVar <> '') then begin
    if DisplayType = dtNumericFormat then begin
      S1 := FormatFloat(DisplayFormat,Value);
    end else begin { dtDateTimeFormat }
      S1 := FormatDateTime(DisplayFormat,Value);
    end; { else }
    if DestParam <> '' then begin
      TRaveProjectManager(Project).SetParam(DestParam,S1);
    end; { if }
    if DestPIVar <> '' then begin
      Report.SetPIVar(DestPIVar,S1);
    end; { if }
  end; { if }
end;  { Print }

{ class TRaveCalcOp }

constructor TRaveCalcOp.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
end;  { Create }

destructor TRaveCalcOp.Destroy;
begin { Destroy }
  inherited Destroy;
end;  { Destroy }

function TRaveCalcOp.GetIsNull: boolean;
begin { GetIsNull }
  Result := (Assigned(Src1CalcVar) and Src1CalcVar.IsNull) or
   (Assigned(Src2CalcVar) and Src2CalcVar.IsNull);
end;  { GetIsNull }

function TRaveCalcOp.GetValue: extended;
var
  Val1: extended;
  Val2: extended;
  DoCount: boolean;
begin { GetValue }
  Val1 := CalcVariable(Self,Src1CalcVar,Src1DataView,Src1DataField,Src1Value,Src1Function,DoCount);
  Val2 := CalcVariable(Self,Src2CalcVar,Src2DataView,Src2DataField,Src2Value,Src2Function,DoCount);
  try
    case Operator of
      coAdd: Result := Val1 + Val2;
      coSub: Result := Val1 - Val2;
      coMul: Result := Val1 * Val2;
      coDiv: begin
        if Val2 = 0.0 then begin
          Result := 0.0;
        end else begin
          Result := Val1 / Val2;
        end; { else }
      end;
      coMod: Result := Trunc(Val1) mod Trunc(Val2);
      coExp: Result := Exp(Val2 * Ln(Val1));
      coGreater: begin
        if Val1 > Val2 then begin
          Result := Val1;
        end else begin
          Result := Val2;
        end; { else }
      end;
      coLesser: begin
        if Val1 < Val2 then begin
          Result := Val1;
        end else begin
          Result := Val2;
        end; { else }
      end;
      coAverage: begin
        Result := (Val1 + Val2) / 2.0;
      end;
      else Result := 0.0;
    end; { case }
    Result := CalcFunction(Result,ResultFunction);
  except
    Result := 0.0;
  end; { tryx }
  if Assigned(FOnGetValue) then begin
    FOnGetValue.Value := Result;
    FOnGetValue.Execute;
    Result := FOnGetValue.Value;
  end; { if }
end;  { GetValue }

procedure TRaveCalcOp.Changing(OldItem: TRaveComponent;
                               NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(Src1CalcVar) and (Src1CalcVar = OldItem) then begin
    Src1CalcVar := NewItem as TRaveCalcVariable;
  end; { if }
  if Assigned(Src1DataView) and (Src1DataView = OldItem) then begin
    Src1DataView := NewItem as TRaveBaseDataView;
  end; { if }
  if Assigned(Src2CalcVar) and (Src2CalcVar = OldItem) then begin
    Src2CalcVar := NewItem as TRaveCalcVariable;
  end; { if }
  if Assigned(Src2DataView) and (Src2DataView = OldItem) then begin
    Src2DataView := NewItem as TRaveBaseDataView;
  end; { if }
end;  { Changing }

{ class TRaveCalcTotal }

constructor TRaveCalcTotal.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FCalcType := ctSum;
  FCountBlanks := true;
end;  { Create }

destructor TRaveCalcTotal.Destroy;
begin { Destroy }
  inherited Destroy;
end;  { Destroy }

function TRaveCalcTotal.GetValue: extended;
begin { GetValue }
  Result := GetCalcValue(Total,Count,CalcType);
  if Assigned(FOnGetValue) then begin
    FOnGetValue.Value := Result;
    FOnGetValue.Execute;
    Result := FOnGetValue.Value;
  end; { if }
end;  { GetValue }

procedure TRaveCalcTotal.BeforeReport;
begin { BeforeReport }
  CalcInitDataListen(self,nil);
  inherited BeforeReport;
end;  { BeforeReport }

procedure TRaveCalcTotal.CalcNewDataListen(Speaker: TRaveComponent; Msg: TObject);
var
  Value: extended;
  DoCount: boolean;
begin { CalcNewDataListen }
  if not (Assigned(CalcVar) and CalcVar.IsNull) then begin
    Value := CalcVariable(Self,CalcVar,DataView,DataField,0.0,cfNone,DoCount);
    if Assigned(FOnCalcValue) then begin
      FOnCalcValue.Value := Value;
      FOnCalcValue.Execute;
      Value := FOnCalcValue.Value;
    end; { if }
    if DoCount or not (CalcType in [ctCount,ctAverage]) then begin
      CalcNewData(Total,Count,CalcType,CountBlanks,CountValue,
       Value,ProcessDataStr(Speaker,DataView,DataField));
    end; { if }
  end; { if }
end;  { CalcNewDataListen }

procedure TRaveCalcTotal.CalcInitDataListen(Speaker: TRaveComponent;
                                            Msg: TObject);
var
  InitData: extended;
begin { CalcInitDataListen }
  if Assigned(Msg) then begin
    InitData := extended(pointer(Msg)^);
  end else begin
    InitData := 0.0;
  end; { else }
  CalcInitData(Total,Count,CalcType,InitData);
end;  { CalcInitDataListen }

procedure TRaveCalcTotal.Changing(OldItem: TRaveComponent;
                                  NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(CalcVar) and (CalcVar = OldItem) then begin
    CalcVar := NewItem as TRaveCalcVariable;
  end; { if }
  if Assigned(DataView) and (DataView = OldItem) then begin
    DataView := NewItem as TRaveBaseDataView;
  end; { if }
  if Assigned(Controller) and (Controller = OldItem) then begin
    Controller := NewItem;
  end; { if }
  if Assigned(Initializer) and (Initializer = OldItem) then begin
    Initializer := NewItem;
  end; { if }
end;  { Changing }

procedure TRaveCalcTotal.SetController(Value: TRaveComponent);
begin { SetController }
  if Value = FController then Exit;
  if Assigned(FController) then begin
    FController.RemoveListener({Trans-}'CalcNewData',CalcNewDataListen);
  end; { if }
  FController := Value;
  if Assigned(FController) then begin
    FController.AddListener({Trans-}'CalcNewData',CalcNewDataListen);
  end; { if }
end;  { SetController }

procedure TRaveCalcTotal.SetInitializer(Value: TRaveComponent);
begin { SetInitializer }
  if Value = FInitializer then Exit;
  if Assigned(FInitializer) then begin
    FInitializer.RemoveListener({Trans-}'CalcInitData',CalcInitDataListen);
  end; { if }
  FInitializer := Value;
  if Assigned(FInitializer) then begin
    FInitializer.AddListener({Trans-}'CalcInitData',CalcInitDataListen);
  end; { if }
end;  { SetInitializer }

procedure TRaveCalcTotal.Print(Report: TBaseReport);
begin { Print }
  inherited Print(Report);
  if not RunningTotal then begin { Reset totals }
    CalcInitDataListen(self,nil);
  end; { if }
end;  { Print }

{ class TRaveCalcController }

constructor TRaveCalcController.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
end;  { Create }

destructor TRaveCalcController.Destroy;
begin { Destroy }
  FreeAndNil(CalcNewDataListenList);
  FreeAndNil(CalcInitDataListenList);
  inherited Destroy;
end;  { Destroy }

procedure TRaveCalcController.AddListener(Conversation: string;
                                          ListenMethod: TRaveListenEvent);
begin { AddListener }
  inherited AddListener(Conversation,ListenMethod);
  if AnsiCompareText(Conversation,{Trans-}'CalcNewData') = 0 then begin
    if not Assigned(CalcNewDataListenList) then begin
      CalcNewDataListenList := TRaveMethodList.Create;
    end; { if }
    CalcNewDataListenList.AddMethod(TMethod(ListenMethod));
  end else if AnsiCompareText(Conversation,{Trans-}'CalcInitData') = 0 then begin
    if not Assigned(CalcInitDataListenList) then begin
      CalcInitDataListenList := TRaveMethodList.Create;
    end; { if }
    CalcInitDataListenList.AddMethod(TMethod(ListenMethod));
  end; { else }
end;  { AddListener }

procedure TRaveCalcController.RemoveListener(Conversation: string;
                                             ListenMethod: TRaveListenEvent);
begin { RemoveListener }
  inherited RemoveListener(Conversation,ListenMethod);
  if (AnsiCompareText(Conversation,{Trans-}'CalcNewData') = 0) and
   Assigned(CalcNewDataListenList) then begin
    CalcNewDataListenList.RemoveMethod(TMethod(ListenMethod));
  end else if (AnsiCompareText(Conversation,{Trans-}'CalcInitData') = 0) and
   Assigned(CalcInitDataListenList) then begin
    CalcInitDataListenList.RemoveMethod(TMethod(ListenMethod));
  end; { else }
end;  { RemoveListener }

function TRaveCalcController.Habla(Conversation: string): boolean;
begin { Habla }
  Result := (AnsiCompareText(Conversation,{Trans-}'CalcNewData') = 0) or
    (AnsiCompareText(Conversation,{Trans-}'CalcInitData') = 0);
end;  { Habla }

procedure TRaveCalcController.Changing(OldItem: TRaveComponent;
                                       NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(InitCalcVar) and (InitCalcVar = OldItem) then begin
    InitCalcVar := NewItem as TRaveCalcVariable;
  end; { if }
  if Assigned(CalcNewDataListenList) and Assigned(OldItem) then begin
    if Assigned(NewItem) then begin
      CalcNewDataListenList.ReplaceObject(OldItem,NewItem);
    end else begin
      CalcNewDataListenList.RemoveObject(OldItem);
    end; { else }
  end; { if }
  if Assigned(CalcInitDataListenList) and Assigned(OldItem) then begin
    if Assigned(NewItem) then begin
      CalcInitDataListenList.ReplaceObject(OldItem,NewItem);
    end else begin
      CalcInitDataListenList.RemoveObject(OldItem);
    end; { else }
  end; { if }
end;  { Changing }

procedure TRaveCalcController.Print(Report: TBaseReport);
var
  Value: extended;
  DoCount: boolean;
begin { Print }
  Value := CalcVariable(Self,InitCalcVar,nil,InitDataField,InitValue,cfNone,DoCount);
  Speak(CalcInitDataListenList,@Value); { Notify controlled objects of initialize }
  Speak(CalcNewDataListenList,nil); { Notify controlled objects of new data }
end;  { Print }

{ class TRaveDataMirrorList }

type
  TRaveDataMirrorList = class(TRavePersistentList)
  public
    function CreateItem(ItemClass: string;
                        RootOwner: TComponent): TPersistent; override;
  end; { TRaveDataMirrorList }

function TRaveDataMirrorList.CreateItem(ItemClass: string;
                                        RootOwner: TComponent): TPersistent;
begin { CreateItem }
  Result := TRaveDataMirror.Create;
end;  { CreateItem }

{ class TRaveDataMirrorSection }

constructor TRaveDataMirrorSection.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FDataMirrors := TRaveDataMirrorList.Create;
end;  { Create }

destructor TRaveDataMirrorSection.Destroy;
begin { Destroy }
  if not DirectReference then begin
    ClearObjectList(FDataMirrors);
    FreeAndNil(FDataMirrors);
  end; { if }
  inherited Destroy;
end;  { Destroy }

procedure TRaveDataMirrorSection.SetDataMirrors(Value: TRavePersistentList);
var
  I1: integer;
  DM1: TRaveDataMirror;
  DM2: TRaveDataMirror;
begin { SetDataMirrors }
  if TRaveProjectManager(Project).Printing then begin
    FDataMirrors := Value;
    DirectReference := true;
  end else begin
    DirectReference := false;
    ClearObjectList(FDataMirrors);
    for I1 := 0 to Value.Count - 1 do begin
      DM1 := TRaveDataMirror(Value[I1]);
      DM2 := TRaveDataMirror.Create;
      DM2.DataValue := DM1.DataValue;
      DM2.SectionMirror := DM1.SectionMirror;
      DM2.IsDefault := DM1.IsDefault;
      FDataMirrors.Add(DM2);
    end; { for }
  end; { else }
end;  { SetDataMirrors }

procedure TRaveDataMirrorSection.InitData;
var
  I1: integer;
  DataText: string;
  CurrControl: TRaveControl;
  NewMirror: TRaveControl;
begin { InitData }
  if Initialized then Exit;
{ Setup Mirror before initializing rest of section }
  FSaveMirror := Mirror;
  CurrControl := self;
  repeat
    NewMirror := nil;
    DataText := ProcessDataStr(self,DataView,DataField);
    if Assigned(FOnMirrorValue) then begin
      FOnMirrorValue.Value := DataText;
      FOnMirrorValue.Execute;
      DataText := FOnMirrorValue.Value;
    end; { if }
    for I1 := 0 to DataMirrors.Count - 1 do begin
      with TRaveDataMirror(DataMirrors[I1]) do begin
        if IsDefault and not Assigned(NewMirror) then begin
          NewMirror := SectionMirror; { Assign default value }
        end; { if }
        if (CompareCase and (AnsiCompareStr(DataText,DataValue) = 0)) or
         (not CompareCase and (AnsiCompareText(DataText,DataValue) = 0)) then begin
          NewMirror := SectionMirror;
          Break;
        end; { if }
      end; { with }
    end; { for }
    CurrControl.Mirror := NewMirror;
    if NewMirror is TRaveDataMirrorSection then begin
      CurrControl := NewMirror;
    end else begin
      CurrControl := nil;
    end; { else }
  until not Assigned(CurrControl);

  inherited InitData;
  if Parent = Page then begin
    SaveAdjusted;
  end; { if }  
  Initialized := true;
end;  { InitData }

procedure TRaveDataMirrorSection.DoneData;
begin { DoneData }
  if not Initialized then Exit;
  inherited DoneData;
  if Mirror <> FSaveMirror then begin
  { Set mirrored item back to designed state.
    Typecast as TRaveDataMirrorSection to get access to protected method }
    if Assigned(Mirror) then begin
      TRaveDataMirrorSection(Mirror).RestoreDesigned;
    end; { if }
    Mirror := FSaveMirror;
  end; { if }
  Initialized := false;
end;  { DoneData }

procedure TRaveDataMirrorSection.SaveDesigned;
begin { SaveDesigned }
  inherited SaveDesigned;
  FSaveAnchor := Anchor;
  FSaveCompareCase := CompareCase;
  FSaveDataField := DataField;
  FSaveDataMirrors := DataMirrors;
  FSaveDataView := DataView;
  FSaveDisplayOn := DisplayOn;
  FSaveTag := Tag;
end;  { SaveDesigned }

procedure TRaveDataMirrorSection.RestoreDesigned;
begin { RestoreDesigned }
  Anchor := FSaveAnchor;
  CompareCase := FSaveCompareCase;
  DataField := FSaveDataField;
  FDataMirrors := FSaveDataMirrors;
  DataView := FSaveDataView;
  DisplayOn := FSaveDisplayOn;
  Tag := FSaveTag;
  DoneData;
  inherited RestoreDesigned;
end;  { RestoreDesigned }

procedure TRaveDataMirrorSection.RestoreAdjusted;
begin { RestoreAdjusted }
  Anchor := FSaveAnchor;
  CompareCase := FSaveCompareCase;
  DataField := FSaveDataField;
  FDataMirrors := FSaveDataMirrors;
  DataView := FSaveDataView;
  DisplayOn := FSaveDisplayOn;
  Tag := FSaveTag;
  inherited RestoreAdjusted;
end;  { RestoreAdjusted }

procedure TRaveDataMirrorSection.Changing(OldItem: TRaveComponent;
                                          NewItem: TRaveComponent);
var
  I1: integer;
  DataMirror: TRaveDataMirror;
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(DataView) and (DataView = OldItem) then begin
    DataView := NewItem as TRaveBaseDataView;
  end; { if }
  if Assigned(FDataMirrors) then begin
    for I1 := 0 to FDataMirrors.Count - 1 do begin
      DataMirror := TRaveDataMirror(FDataMirrors[I1]);
      if Assigned(DataMirror.SectionMirror) and (DataMirror.SectionMirror =
       OldItem) then begin
        DataMirror.SectionMirror := NewItem as TRaveSection;
      end; { if }
    end; { for }
  end; { if }
end;  { Changing }

procedure TRaveDataMirrorSection.PrintAll(Report: TBaseReport);
begin { PrintAll }
  InitData;
  inherited PrintAll(Report);
  DoneData;
end;  { PrintAll }

procedure TRaveDataMirrorSection.Paint(Canvas: TCanvas);
begin { Paint }
  with Canvas do begin
    Pen.Color := clFuchsia;
    Pen.Style := psDot;
    Pen.Mode := pmCopy;
    Pen.Width := 1;
    Brush.Color := clWhite;
    Brush.Style := bsClear;
    Rectangle(XI2D(PageLeft),YI2D(PageTop),XI2D(PageRight) + 1,YI2D(PageBottom) + 1);
  end; { with }
end;  { Paint }

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.
