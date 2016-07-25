{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvCsRpt;

interface

uses
  Messages, Windows, Controls, Graphics, Dialogs, Buttons, StdCtrls, Forms,
  Classes, SysUtils, RvDefine, RvUtil, RvClass, RvCsStd,
  RvData, RpDefine, RpBase, RvProj, RvDataField;

const
  BandColors = 20;
  BandColor: array[1..BandColors] of TColor = (
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
  TRaveBand = class;
  TRaveIterateBand = class;

  TBandPositionMode = (pmOffset,pmOverlay,pmAbsolute);
  TBandPrintLoc = (plBodyHeader,plGroupHeader,plRowHeader,plMaster,plDetail,
   plRowFooter,plGroupFooter,plBodyFooter);
  TBandPrintLocSet = set of TBandPrintLoc;
  TBandPrintOcc = (poFirst,poNewPage,poNewColumn);
  TBandPrintOccSet = set of TBandPrintOcc;
  PBandDetail = ^TBandDetail;
  TBandDetail = record
    Band: TRaveBand; { Band object }
    PointerDown: boolean; { Is ControllerBand pointer up or down? }
    PointerColor: integer; { Color index if this band is controlled }
    MasterColor: integer; { Color index if this band is a controller }
    Level: byte; { Level of first graphical object }
    MasterLevel: byte; { Level of controller }
    IsController: boolean; { Is this band a controller? }
    ControllerDetail: PBandDetail; { TBandDetail record of controller }
  end; { TBandDetail }

  PBandSearchRec = ^TBandSearchRec;
  TBandSearchRec = record
    Index: integer;
    ControllerBand: TRaveIterateBand;
    PrintLoc: TBandPrintLoc;
  end; { TBandSearchRec }

  TRaveBandStyle = class(TPersistent)
  protected
    FPrintLoc: TBandPrintLocSet;
    FPrintOcc: TBandPrintOccSet;
    procedure AssignTo(Dest: TPersistent); override;
  public
    constructor Create;
  published
    property PrintLoc: TBandPrintLocSet read FPrintLoc write FPrintLoc default [];
    property PrintOcc: TBandPrintOccSet read FPrintOcc write FPrintOcc default [poFirst];
  end; { TRaveBandStyle }

  TRaveRegion = class(TRaveContainerControl)
  protected
    FReplaceBand: TRaveBand;
    FReplaceBandStyle: TRaveBandStyle;
    PrintTop: TRaveUnits;
    SavePrintTop: TRaveUnits; { Saves PrintTop value }
    BandSearchRec: TBandSearchRec;
    BandSearchSave: TRaveStackList;

    RestartBand: TRaveBand; { Which band will restart }
    RestartLoc: TBandPrintLoc; { Which PrintLoc for RestartBand }
    FRestarting: boolean; { Are we restarting? }
    ForceRestart: boolean; { Force a restart for this region }
    FTopOfRegion: boolean; { Are we starting at the top of the region? }

    PrintLoc: TBandPrintLoc; { Current region PrintLoc }
    PrintOcc: TBandPrintOcc; { Current region PrintOcc }

    FCurrentColumn: integer;
    FColumns: integer;
    FColumnSpacing: TRaveUnits;

    function AcceptChild(NewChild: TClass): boolean; override;
    function GetDonePrinting: boolean; override;
    procedure BeforeReport; override;
    function GetColumnStart: TRaveUnits;
    function GetBandWidth(RegionWidth: TRaveUnits): TRaveUnits;
    function GetColumnWidth: TRaveUnits;
    procedure SetColumns(Value: integer);
    procedure SetColumnSpacing(Value: TRaveUnits);
    procedure PushBandSearchRec;
    procedure PopBandSearchRec;
    function HeightAvail: TRaveUnits;
    procedure InitData; override;
    procedure DoneData; override;
    procedure SetName(const NewName: TComponentName); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure SaveState;
    procedure RestoreState;
    procedure Restart(Band: TRaveBand);
    procedure UndoRestart;
    procedure PostponeRestart;
    procedure Paint(Canvas: TCanvas); override;
    procedure PaintAll(Canvas: TCanvas); override;
    procedure PrintBands(Report: TBaseReport;
                         ControllerBand: TRaveIterateBand;
                         APrintLoc: TBandPrintLoc);

    procedure PrintAll(Report: TBaseReport); override;
    function FirstSubBand(var BandSearchRec: TBandSearchRec;
                              ControllerBand: TRaveIterateBand;
                              PrintLoc: TBandPrintLoc): TRaveBand;
    function NextSubBand(var BandSearchRec: TBandSearchRec): TRaveBand;

    property ReplaceBand: TRaveBand read FReplaceBand write FReplaceBand;
    property ReplaceBandStyle: TRaveBandStyle read FReplaceBandStyle write
     FReplaceBandStyle;
    property Restarting: boolean read FRestarting;
    property CurrentColumn: integer read FCurrentColumn;
    property ColumnStart: TRaveUnits read GetColumnStart;
    property ColumnWidth: TRaveUnits read GetColumnWidth;
    property TopOfRegion: boolean read FTopOfRegion;
  published
    property Height;
    property Left;
    property Top;
    property Width;
    property Columns: integer read FColumns write SetColumns default 1;
    property ColumnSpacing: TRaveUnits read FColumnSpacing write SetColumnSpacing;
  end; { TRaveRegion }

  TRaveBand = class(TRaveContainerControl)
  protected
    BandHeaderTop: TRaveUnits;
    BandHeaderBottom: TRaveUnits;
    LastGroupValue: string; { Value of previous group }
    OnCurrPage: boolean; { Has band been printed on current page? }
    FControllerBand: TRaveIterateBand;
    FPositionMode: TBandPositionMode;
    FPositionValue: TRaveUnits;
    FBandStyle: TRaveBandStyle;
    FGroupDataView: TRaveBaseDataView;
    FGroupKey: TRaveFieldName;
    FReprintLocs: TBandPrintLocSet;
    FSaving: boolean; { Is the state of this band currently being saved? }
    FStartNewPage: boolean; { Start new page when this band prints? }
    FFinishNewPage: boolean; { Goto new page when this band finishes printing? }
    FDesignerHide: boolean; { Should band be hidden in visual designer? }
    FAllowSplit: boolean; { Can this band split across pages/columns? }
    FMinHeightLeft: TRaveUnits; { Minimum height left to print }
    FDoSplit: boolean;
    FOnGetGroup: TRaveStringEvent;

    class function AcceptParent(NewParent: TClass): boolean; override;
    procedure SetParent(Value: TRaveComponent); override;
    procedure SetControllerBand(Value: TRaveIterateBand);
    procedure SetBandStyle(Value: TRaveBandStyle);
    function GetRegion: TRaveRegion;
    function GetMinHeight: TRaveUnits;
    function GetLeft: TRaveUnits; override;
    function GetWidth: TRaveUnits; override;
    procedure AnchorAdjust(WidthChanging: boolean;
                           OldWidth,NewWidth: TRaveUnits;
                           HeightChanging: boolean;
                           OldHeight,NewHeight: TRaveUnits); override;
    procedure BeforeReport; override;
    function GetCurrGroupValue: string;
    function GetNextGroupValue: string;
    procedure SaveState; virtual;
    procedure RestoreState; virtual;
    function GetSaving: boolean; virtual;
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
    procedure SetDesignerHide(Value: boolean);
    procedure SetName(const NewName: TComponentName); override;
    procedure Preprint; virtual;
    procedure ResetState; virtual;
    procedure SetBandTop; virtual;
    procedure DoGetGroup(var Value: string);

    property DoSplit: boolean read FDoSplit write FDoSplit;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Paint(Canvas: TCanvas); override;
    procedure CreatePips; override;
    procedure UpdatePips; override;
    function CanSelect(Item: TRaveComponent): boolean; override;
    function IsSelectPoint(Point: TRavePoint): boolean; override;
    procedure PrintAll(Report: TBaseReport); override;
    procedure Resize; override;

    property MinHeight: TRaveUnits read GetMinHeight;
    property Region: TRaveRegion read GetRegion;
    property Saving: boolean read GetSaving;
  published
    property AllowSplit: boolean read FAllowSplit write FAllowSplit default false;
    property BandStyle: TRaveBandStyle read FBandStyle write SetBandStyle;
    property ControllerBand: TRaveIterateBand read FControllerBand
     write SetControllerBand;
    property DesignerHide: boolean read FDesignerHide write SetDesignerHide
     default false;
    property FinishNewPage: boolean read FFinishNewPage write FFinishNewPage
     default false;
    property GroupDataView: TRaveBaseDataView read FGroupDataView
     write FGroupDataView default nil;
    property GroupKey: TRaveFieldName read FGroupKey write FGroupKey;
    property Height;
    property MinHeightLeft: TRaveUnits read FMinHeightLeft write FMinHeightLeft;
    property PositionMode: TBandPositionMode read FPositionMode
     write FPositionMode default pmOffset;
    property PositionValue: TRaveUnits read FPositionValue write FPositionValue;
    property ReprintLocs: TBandPrintLocSet read FReprintLocs write FReprintLocs
     default [plBodyHeader,plGroupHeader,plRowHeader,plMaster,plDetail,
     plRowFooter,plGroupFooter,plBodyFooter];
    property StartNewPage: boolean read FStartNewPage write FStartNewPage
     default false;
    property OnGetGroup: TRaveStringEvent read FOnGetGroup write FOnGetGroup;
  end; { TRaveBand }

  TRaveIterateBand = class(TRaveBand)
  protected
    NewData: boolean; { Band has new data to print }
    FControlledList: TList; { List of bands that being controlled by self }
    FKeepBodyTogether: boolean;
    FKeepRowTogether: boolean;
    FOrphanRows: integer;
    FWidowRows: integer;
    FLocOnRestart: TBandPrintLoc; { What was the PrintLoc when restarted }
    FMaxRows: integer; { Maximum # of rows per page/column allowed }
    FInitToFirst: boolean; { Initialize to first record? }
    Active: boolean;
    CalcNewDataListenList: TRaveMethodList;
    FColumns: integer;
    FColumnSpacing: TRaveUnits;
    FCurrentColumn: integer;
    SaveLastTop: TRaveUnits;
    SavePriorTop: TRaveUnits;
    SaveLastBottom: TRaveUnits;
    SaveRegionColumn: integer;
    FDetailKey: TRaveFieldName;

    function GetColumnStart: TRaveUnits;
    function GetColumnWidth: TRaveUnits;
    procedure SetColumns(Value: integer);
    procedure SetColumnSpacing(Value: TRaveUnits);
    function GetLeft: TRaveUnits; override;
    function GetWidth: TRaveUnits; override;
    procedure First; virtual; abstract;
    procedure Next; virtual; abstract;
    procedure GetCurrent; virtual; abstract;
    function Eof: boolean; virtual; abstract;
    function CheckRowsLeft(ReqRows: integer): boolean; virtual; abstract;
    procedure AddControlled(Value: TRaveBand);
    procedure RemoveControlled(Value: TRaveBand);
    procedure SaveState; override;
    procedure RestoreState; override;
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
    procedure ResetControlled;
    procedure Preprint; override;
    procedure SetBandTop; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Paint(Canvas: TCanvas); override;
    procedure PrintAll(Report: TBaseReport); override;
    procedure AddListener(Conversation: string;
                          ListenMethod: TRaveListenEvent); override;
    procedure RemoveListener(Conversation: string;
                             ListenMethod: TRaveListenEvent); override;
    function Habla(Conversation: string): boolean; override;

    property LocOnRestart: TBandPrintLoc read FLocOnRestart write FLocOnRestart;
    property CurrentColumn: integer read FCurrentColumn;
    property ColumnStart: TRaveUnits read GetColumnStart;
    property ColumnWidth: TRaveUnits read GetColumnWidth;
  published
    property Columns: integer read FColumns write SetColumns default 1;
    property ColumnSpacing: TRaveUnits read FColumnSpacing write SetColumnSpacing;
    property DetailKey: TRaveFieldName read FDetailKey write FDetailKey;
    property KeepBodyTogether: boolean read FKeepBodyTogether write FKeepBodyTogether
     default false;
    property KeepRowTogether: boolean read FKeepRowTogether write FKeepRowTogether
     default false;
    property MaxRows: integer read FMaxRows write FMaxRows default 0;
    property InitToFirst: boolean read FInitToFirst write FInitToFirst default true;
    property OrphanRows: integer read FOrphanRows write FOrphanRows default 0;
    property WidowRows: integer read FWidowRows write FWidowRows default 0;
  end; { TRaveIterateBand }

  TRaveDataBand = class(TRaveIterateBand)
  protected
    FDataView: TRaveBaseDataView;
    FMasterDataView: TRaveBaseDataView;
    FMasterKey: TRaveFieldName;
    FSortKey: TRaveFieldName;

    function GetSaving: boolean; override;
    procedure SaveState; override;
    procedure RestoreState; override;
    procedure First; override;
    procedure Next; override;
    procedure GetCurrent; override;
    function Eof: boolean; override;
    function CheckRowsLeft(ReqRows: integer): boolean; override;
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
  public
  published
    property DataView: TRaveBaseDataView read FDataView write FDataView default nil;
    property MasterDataView: TRaveBaseDataView read FMasterDataView
     write FMasterDataView default nil;
    property MasterKey: TRaveFieldName read FMasterKey write FMasterKey;
    property SortKey: TRaveFieldName read FSortKey write FSortKey;
  end; { TRaveDataBand }

  TRaveDataCycle = class(TRaveComponent)
  protected
    FDataView: TRaveBaseDataView;
    FDetailKey: TRaveFieldName;
    FInitToFirst: boolean; { Initialize to first record? }
    FMasterDataView: TRaveBaseDataView;
    FMasterKey: TRaveFieldName;
    FMaxRows: integer; { Maximum # of rows per page/column allowed }
    FSortKey: TRaveFieldName;
    RowCount: integer;
    Active: boolean;

    function GetDonePrinting: boolean; override;
    procedure BeforeReport; override;
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
  public
    constructor Create(AOwner: TComponent); override;
    procedure Print(Report: TBaseReport); override;
  published
    property DataView: TRaveBaseDataView read FDataView write FDataView default nil;
    property DetailKey: TRaveFieldName read FDetailKey write FDetailKey;
    property InitToFirst: boolean read FInitToFirst write FInitToFirst default true;
    property MasterDataView: TRaveBaseDataView read FMasterDataView
     write FMasterDataView default nil;
    property MasterKey: TRaveFieldName read FMasterKey write FMasterKey;
    property MaxRows: integer read FMaxRows write FMaxRows default 0;
    property SortKey: TRaveFieldName read FSortKey write FSortKey;
  end; { TRaveDataCycle }

  function CreateBandList(Region: TRaveRegion): TList;
  procedure FreeBandList(BandList: TList);

implementation

{ BandList utilities }

function CreateBandList(Region: TRaveRegion): TList;
var
  I1: integer;
  I2: integer;
  BandDetail: PBandDetail;
  BandDetail2: PBandDetail;
  ColorIdx: integer;
  Final: boolean;

  procedure FindControllerLevel(BandDetail: PBandDetail);
  begin { FindControllerLevel }
    with BandDetail^ do begin
      if Assigned(ControllerDetail) and (ControllerDetail.MasterLevel = 0) then begin
        FindControllerLevel(ControllerDetail);
      end; { if }
      if Assigned(ControllerDetail) then begin
        Level := ControllerDetail.MasterLevel;
      end; { if }
      if IsController then begin
        MasterLevel := Level + 1;
      end; { if }
    end; { if }
  end;  { FindControllerLevel }

begin { CreateBandList }
  with Region do begin
    Result := TList.Create;

  { Initialize BandList }
    ColorIdx := 1;
    for I1 := 0 to (ChildCount - 1) do begin
      Final := (I1 = (ChildCount - 1));

    { Create new BandDetail record }
      New(BandDetail);
      with BandDetail^ do begin
        Band := TRaveBand(Child[I1]);
        PointerColor := 0;
        ControllerDetail := nil;
        IsController := (Band is TRaveIterateBand){ and
         Assigned(TRaveIterateBand(Band).FControlledList) and
         (TRaveIterateBand(Band).FControlledList.Count > 0)};

        if not Assigned(Band.ControllerBand) then begin { Level 1 Controller }
          Level := 1;
          MasterLevel := 1;
          IsController := true;
        end else begin
          Level := 0;
          MasterLevel := 0;
        end; { else }

        if IsController then begin
          MasterColor := ColorIdx;
          ColorIdx := (ColorIdx mod BandColors) + 1;
        end else begin
          MasterColor := 0;
        end; { else }
      end; { with }

    { Look for any bands already registered that are controlled by this band }
      for I2 := 0 to (Result.Count - 1) do begin
        BandDetail2 := Result[I2];
        if BandDetail2.Band.ControllerBand = BandDetail.Band then begin
        { BandDetail2 is controlled by BandDetail }
          with BandDetail2^ do begin
            PointerDown := true;
            ControllerDetail := BandDetail;
            PointerColor := BandDetail.MasterColor;
            if BandDetail.MasterLevel > 0 then begin
              Level := BandDetail.MasterLevel;
              if IsController then begin
                MasterLevel := BandDetail.MasterLevel + 1;
              end; { if }
            end; { if }
          end; { with }
        end else if BandDetail2.Band = BandDetail.Band.ControllerBand then begin
         { BandDetail2 is the controller for BandDetail }
           with BandDetail^ do begin
             PointerDown := false;
             ControllerDetail := BandDetail2;
             PointerColor := BandDetail2.MasterColor;
             if BandDetail2.MasterLevel > 0 then begin
               Level := BandDetail2.MasterLevel;
               if IsController then begin
                 MasterLevel := BandDetail2.MasterLevel + 1;
               end; { if }
             end; { if }
           end; { with }
        end; { else }

        if Final and (BandDetail2.Level = 0) then begin { Update Level }
          FindControllerLevel(BandDetail2);
        end; { if }
      end; { for }

    { Add BandDetail to BandList }
      Result.Add(BandDetail);
    end; { for }
  end; { with }
end;  { CreateBandList }

procedure FreeBandList(BandList: TList);
var
  I1: integer;
begin { FreeBandList }
  for I1 := 0 to BandList.Count - 1 do begin
    Dispose(PBandDetail(BandList[I1]));
  end; { for }
  FreeAndNil(BandList);
end;  { FreeBandList }

{ class TRaveRegion }

constructor TRaveRegion.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FControlStyle := FControlStyle + [rcsContainer];
  Width := 3.0;
  Height := 3.0;
  FColumns := 1;
  FColumnSpacing := 0.0;
  FCurrentColumn := 1;
end;  { Create }

destructor TRaveRegion.Destroy;
begin { Destroy }
  if Assigned(BandSearchSave) then begin
    FreeAndNil(BandSearchSave);
  end; { if }
  inherited Destroy;
end;  { Destroy }

function TRaveRegion.AcceptChild(NewChild: TClass): boolean;
begin { AcceptChild }
  Result := NewChild.InheritsFrom(TRaveBand);
end;  { AcceptChild }

function TRaveRegion.GetDonePrinting: boolean;
begin { GetDonePrinting }
  Result := not Assigned(RestartBand) and inherited GetDonePrinting;
end;  { GetDonePrinting }

procedure TRaveRegion.BeforeReport;
begin { BeforeReport }
  PrintOcc := poFirst;
  RestartBand := nil;
  ForceRestart := false;
  FRestarting := false;
  inherited BeforeReport;
end;  { BeforeReport }

function TRaveRegion.GetColumnStart: TRaveUnits;
begin { GetColumnStart }
  Result := (ColumnWidth + ColumnSpacing) * (CurrentColumn - 1);
end;  { GetColumnStart }

function TRaveRegion.GetBandWidth(RegionWidth: TRaveUnits): TRaveUnits;
begin { GetBandWidth }
  Result := (RegionWidth - ((Columns - 1) * ColumnSpacing)) / Columns;
end;  { GetBandWidth }

function TRaveRegion.GetColumnWidth: TRaveUnits;
begin { GetColumnWidth }
  Result := GetBandWidth(Width);
end;  { GetColumnWidth }

procedure TRaveRegion.SetColumns(Value: integer);
begin { SetColumns }
  if (FColumns <> Value) and (Value > 0) then begin
    FColumns := Value;
    ResizeAll;
  end; { if }
end;  { SetColumns }

procedure TRaveRegion.SetColumnSpacing(Value: TRaveUnits);
begin { SetColumnSpacing }
  if FColumnSpacing <> Value then begin
    FColumnSpacing := Value;
    ResizeAll;
  end; { if }
end;  { SetColumnSpacing }

procedure TRaveRegion.SetName(const NewName: TComponentName);
begin { SetName }
  inherited SetName(NewName);
  Invalidate;
end;  { SetName }

procedure TRaveRegion.PushBandSearchRec;
var
  BandSearchPtr: PBandSearchRec;
begin { PushBandSearchRec }
  if not Assigned(BandSearchSave) then begin
    BandSearchSave := TRaveStackList.Create;
  end; { if }
  New(BandSearchPtr);
  BandSearchPtr^ := BandSearchRec;
  BandSearchSave.Push(BandSearchPtr);
end;  { PushBandSearchRec }

procedure TRaveRegion.PopBandSearchRec;
var
  BandSearchPtr: PBandSearchRec;
begin { PopBandSearchRec }
  if Assigned(BandSearchSave) then begin
    BandSearchPtr := PBandSearchRec(BandSearchSave.Pop);
    BandSearchRec := BandSearchPtr^;
    Dispose(BandSearchPtr);
  end; { if }
end;  { PopBandSearchRec }

function TRaveRegion.HeightAvail: TRaveUnits;
begin { HeightAvail }
  Result := Height - PrintTop;
end;  { HeightAvail }

procedure TRaveRegion.InitData;
begin { InitData }
end;  { InitData }

procedure TRaveRegion.DoneData;
begin { DoneData }
end;  { DoneData }

function TRaveRegion.FirstSubBand(var BandSearchRec: TBandSearchRec;
                                      ControllerBand: TRaveIterateBand;
                                      PrintLoc: TBandPrintLoc): TRaveBand;
begin { FirstSubBand }
  BandSearchRec.PrintLoc := PrintLoc;
  BandSearchRec.ControllerBand := ControllerBand;
  BandSearchRec.Index := -1;
  Result := NextSubBand(BandSearchRec);
end;  { FirstSubBand }

function TRaveRegion.NextSubBand(var BandSearchRec: TBandSearchRec): TRaveBand;
var
  I1: integer;
  Band: TRaveBand;

  function ValidBand(ABand: TRaveBand;
                     ABandStyle: TRaveBandStyle): boolean;
  begin { ValidBand }
    Result := (BandSearchRec.ControllerBand = ABand.ControllerBand) and
     (not Assigned(ABand.ControllerBand) or
     ((BandSearchRec.PrintLoc in ABandStyle.PrintLoc)));
  end;  { ValidBand }

begin { NextSubBand }
  for I1 := BandSearchRec.Index + 1 to ChildCount - 1 do begin
    Band := TRaveBand(Child[I1]);
    if InDesigner then begin
      if ((ReplaceBand = Band) and ValidBand(Band,ReplaceBandStyle)) or
      ((ReplaceBand <> Band) and ValidBand(Band,Band.BandStyle)) then begin
        Result := Band;
        BandSearchRec.Index := I1;
        Exit;
      end; { if }
    end else begin
      if ValidBand(Band,Band.BandStyle) then begin
        Result := Band;
        BandSearchRec.Index := I1;
        Exit;
      end; { if }
    end; { else }
  end; { for }
  Result := nil;
  BandSearchRec.Index := ChildCount;
end;  { NextSubBand }

procedure TRaveRegion.Restart(Band: TRaveBand);
begin { Restart }
  RestartBand := Band;
  FRestarting := true;
  ForceRestart := false;
end;  { Restart }

procedure TRaveRegion.UndoRestart;
begin { UndoRestart }
  RestartBand := nil;
  FRestarting := false;
end;  { UndoRestart }

procedure TRaveRegion.PostponeRestart;
begin { PostponeRestart }
  UndoRestart;
  ForceRestart := true;
end;  { PostponeRestart }

procedure TRaveRegion.SaveState;
begin { SaveState }
  SavePrintTop := PrintTop;
end;  { SaveState }

procedure TRaveRegion.RestoreState;
begin { RestoreState }
  PrintTop := SavePrintTop;
end;  { RestoreState }

procedure TRaveRegion.PaintAll;
var
  I1: integer;
  I2: integer;
  I3: integer;
  SaveRgn: HRgn;
  Rect: TRect;
  BandComp: TRaveBand;
  F1: TRaveFloat;
  S1: string;
  BandList: TList;
  BandDetail: PBandDetail;
  RegionSelected: boolean;
  BandSelected: boolean;
  FirstBandHeader: boolean;
  BandHeaderAdjust: TRaveUnits;

  procedure PrintItem(    Text: string;
                          Active: boolean;
                      var X: integer;
                          Y: integer;
                          XLimit: integer);
  begin { PrintItem }
    with Canvas do begin
      X := X - (TextWidth(Text) + 2);
      if X < XLimit then Exit;
      Brush.Style := bsClear;
      if Active then begin
        Font.Color := ModColor(clBlack);
        Font.Style := [fsBold];
      end else begin
        Font.Color := ModColor(clWhite);
        TextOut(X + 1,Y + 1,Text);
        Font.Color := ModColor(clGray);
      end; { else }
      TextOut(X,Y,Text);
      Font.Style := [];
    end; { with }
  end;  { PrintItem }

begin { PaintAll }
  if not Visible then Exit;

  Paint(Canvas);
  PaintMirror(Canvas,self,self);

{ Paint all children }
  if ChildCount > 0 then begin
  { Build list of bands }
    BandList := CreateBandList(self);
    RegionSelected := AlwaysShowHeaders or CurrentDesigner.IsSelected(self);

    SaveRgn := SetClipped(Canvas);
    Rect.Left := XI2D(PageLeft);
    Rect.Right := XI2D(PageRight) + 1;
    Rect.Top := YI2D(PageTop);
    FirstBandHeader := true;
    BandHeaderAdjust := PageTop - Top;

    for I1 := 0 to (ChildCount - 1) do begin
      BandComp := Child[I1] as TRaveBand;
      BandDetail := BandList[I1];

    { Draw banner for child }
      BandSelected := CurrentDesigner.IsSelected(BandComp);

      if FirstBandHeader and (BandSelected or RegionSelected or
       not BandComp.DesignerHide) then begin
      { Draw initial top gray line }
        FirstBandHeader := false;
        with Canvas do begin
          Pen.Width := 1;
          if BandSelected or RegionSelected then begin { Band header is showing }
            Pen.Style := psSolid;
          end else begin
            Pen.Style := psDot;
          end; { else }
          Pen.Color := ModColor(clSilver);
          Pen.Mode := pmCopy;
          Brush.Style := bsSolid;
          Brush.Color := ModColor(clWhite);
          MoveTo(Rect.Left,Rect.Top);
          LineTo(Rect.Right,Rect.Top);
        end; { with }
      end; { with }

      if BandSelected or RegionSelected then begin { Draw Band Header }
        with Canvas do begin
          Font.Name := {Trans-}'Arial';
          Font.Size := 8;
          Font.Color := ModColor(clBlack);
          Font.Style := [];
          Inc(Rect.Top);
          Inc(Rect.Left);
          Rect.Bottom := Rect.Top + Abs(Font.Height) + 7;
          DrawButtonFace(Canvas,Rect,1,bsNew,false,false,false);
          Dec(Rect.Left);
          Dec(Rect.Top);
          BandComp.BandHeaderTop := YD2I(Rect.Top) + BandHeaderAdjust;
          BandComp.BandHeaderBottom := YD2I(Rect.Bottom) + BandHeaderAdjust;

        { Draw gray line below header bar}
          if BandSelected or not BandComp.DesignerHide then begin
            with Canvas do begin
              Pen.Width := 1;
              Pen.Style := psDot;
              Pen.Color := ModColor(clSilver);
              Pen.Mode := pmCopy;
              Brush.Style := bsSolid;
              Brush.Color := ModColor(clWhite);
              MoveTo(Rect.Left,Rect.Bottom);
              LineTo(Rect.Right,Rect.Bottom);
            end; { with }
          end; { if }

        { Draw Band Glyphs and Funtion Text }
          with BandDetail^ do begin
            I2 := Rect.Left + 5 + ((Level - 1) * 16);
            Brush.Style := bsSolid;
            Pen.Width := 1;
            Pen.Style := psSolid;
            Pen.Mode := pmCopy;
            if Assigned(ControllerDetail) then begin { Print arrow }
              Brush.Color := ModColor(BandColor[PointerColor]);
              Pen.Color := ModColor(clBlack);
              if PointerDown then begin
                Polygon([Point(I2 + 2,Rect.Top + 3),
                 Point(I2 + 10,Rect.Top + 3),
                 Point(I2 + 6,Rect.Top + 15)]);
              end else begin
                Polygon([Point(I2 + 6,Rect.Top + 3),
                 Point(I2 + 2,Rect.Top + 15),
                 Point(I2 + 10,Rect.Top + 15)]);
              end; { else }
              I2 := I2 + 16;
            end; { if }
            if IsController then begin { Print Diamond }
              Brush.Color := ModColor(BandColor[MasterColor]);
              Pen.Color := ModColor(clBlack);
              Polygon([Point(I2 + 6,Rect.Top + 3),
               Point(I2 + 12,Rect.Top + 9),
               Point(I2 + 6,Rect.Top + 15),
               Point(I2,Rect.Top + 9)]);
              I2 := I2 + 16;
            end; { if }

          { Draw RegionName: BandName }
            Brush.Style := bsClear;
            Brush.Color := ModColor(clBtnFace);
            Font.Color := ModColor(clWhite);
            Font.Color := ModColor(clBlack);
            S1 := Name + {Trans-}': ' + BandComp.Name;
            TextOut(I2,Rect.Top + 2,S1);
            I2 := I2 + TextWidth(S1) + 5;

            I3 := Rect.Right - 5;
            with Band.BandStyle do begin
              PrintItem(')',true,I3,Rect.Top + 2,I2);
              PrintItem('C',poNewColumn in PrintOcc,I3,Rect.Top + 2,I2);
              PrintItem('P',poNewPage in PrintOcc,I3,Rect.Top + 2,I2);
              PrintItem('1',poFirst in PrintOcc,I3,Rect.Top + 2,I2);
              PrintItem({Trans-}'  ',true,I3,Rect.Top + 2,I2);
              if not Assigned(Band.ControllerBand) then begin
                PrintItem({Trans-}'Master ',true,I3,Rect.Top + 2,I2);
              end else begin
                PrintItem('b',plBodyFooter in PrintLoc,I3,Rect.Top + 2,I2);
                PrintItem('g',plGroupFooter in PrintLoc,I3,Rect.Top + 2,I2);
                PrintItem('r',plRowFooter in PrintLoc,I3,Rect.Top + 2,I2);
                PrintItem('D',plDetail in PrintLoc,I3,Rect.Top + 2,I2);
                PrintItem('R',plRowHeader in PrintLoc,I3,Rect.Top + 2,I2);
                PrintItem('G',plGroupHeader in PrintLoc,I3,Rect.Top + 2,I2);
                PrintItem('B',plBodyHeader in PrintLoc,I3,Rect.Top + 2,I2);
              end; { else }
              PrintItem('(',true,I3,Rect.Top + 2,I2);
            end; { with }
          end; { with }
        end; { with }
      end else begin
        Rect.Bottom := Rect.Top;
        BandComp.BandHeaderTop := -1;
        BandComp.BandHeaderBottom := -1;
      end; { else }

      if BandSelected or not BandComp.DesignerHide then begin
      { Set Top, Left and Width properties for child }
        F1 := YD2I(Rect.Bottom) - Top;
        if BandComp.Top <> F1 then begin
          BandComp.Top := F1;
        end; { if }

      { Reset Rect.Top for Height of child }
        Rect.Top := YI2D(BandComp.PageBottom){ - 1};

      { Draw sides and bottom gray divider line }
        with Canvas do begin
          Pen.Width := 1;
          Pen.Style := psDot;
          Pen.Color := ModColor(clSilver);
          Pen.Mode := pmCopy;
          Brush.Style := bsSolid;
          Brush.Color := ModColor(clWhite);

        { Bottom and top are actually switched here }
          MoveTo(Rect.Left,Rect.Bottom + 1);
          LineTo(Rect.Left,Rect.Top);
          MoveTo(Rect.Right - 1,Rect.Bottom + 1);
          LineTo(Rect.Right - 1,Rect.Top);
          MoveTo(Rect.Left,Rect.Top);
          LineTo(Rect.Right,Rect.Top);
        end; { with }

      { Paint child }
        BandComp.PaintAll(Canvas);
      end else begin { Band hidden }
        Rect.Top := Rect.Bottom;
      end; { else }
    end; { for }
    SetUnclipped(Canvas,SaveRgn);

    FreeBandList(BandList);
  end; { if }
end;  { PaintAll }

procedure TRaveRegion.Paint(Canvas: TCanvas);
var
  I1: integer;
  I2: integer;
  X1: TRaveUnits;
begin { Paint }
  with Canvas do begin
    Pen.Color := ModColor(clBlack);
    Pen.Style := psSolid;
    Pen.Mode := pmCopy;
    Pen.Width := 1;
    Brush.Color := ModColor(clGray);
    Brush.Style := bsSolid;
    Rectangle(XI2D(PageLeft),YI2D(PageTop),XI2D(PageRight) + 1,YI2D(PageBottom) + 1);
    Font.Name := {Trans-}'Arial';
    Font.Size := 8;
    Font.Color := ModColor(clWhite);
    Font.Style := [fsBold];
    TextOut(XI2D(PageRight) - (TextWidth(Name) + 3),YI2D(PageBottom) -
     (Abs(Font.Height) + 5),Name);

  { Draw column spacings }
    if Columns > 1 then begin
      Brush.Style := bsDiagCross;
      Brush.Color := ModColor(clBlack);
      for I1 := 1 to (Columns - 1) do begin
        if ColumnSpacing = 0.0 then begin { Just draw line }
          I2 := XI2D(PageLeft + (ColumnWidth * I1));
          MoveTo(I2,YI2D(PageTop));
          LineTo(I2,YI2D(PageBottom));
        end else begin { Draw rectangle }
          X1 := PageLeft + ((ColumnWidth + ColumnSpacing) * I1);
          Rectangle(XI2D(X1 - ColumnSpacing),YI2D(PageTop),XI2D(X1),YI2D(PageBottom));
        end; { else }
      end; { for }
    end; { if }
  end; { with }
end;  { Paint }

procedure TRaveRegion.PrintBands(Report: TBaseReport;
                                 ControllerBand: TRaveIterateBand;
                                 APrintLoc: TBandPrintLoc);
var
  SubBand: TRaveBand;
begin { PrintBands }
  PrintLoc := APrintLoc;
  SubBand := FirstSubBand(BandSearchRec,ControllerBand,PrintLoc);
  while Assigned(SubBand) do begin
    if not Assigned(SubBand.ControllerBand) then begin
      PrintLoc := plDetail;
    end; { if }
    if SubBand is TRaveIterateBand then begin
      PushBandSearchRec;
      SubBand.PrintAll(Report);
      PopBandSearchRec;
    end else begin
      SubBand.PrintAll(Report);
    end; { else }
    FTopOfRegion := false;
    if Restarting then Break;
    SubBand := NextSubBand(BandSearchRec);
  end; { while }
end;  { PrintBands }

procedure TRaveRegion.PrintAll(Report: TBaseReport);
var
  I1: integer;
begin { PrintAll }
  BeforePrint;
  if not AllowPrint then Exit;
  FTopOfRegion := true;

{ Clear OnCurrPage values for all bands }
  for I1 := 0 to ChildCount - 1 do begin
    TRaveBand(Child[I1]).OnCurrPage := false;
    if Child[I1] is TRaveIterateBand then begin
      TRaveIterateBand(Child[I1]).SaveLastBottom := 0;
    end; { if }
  end; { for }

  for I1 := 1 to Columns do begin
    FCurrentColumn := I1;
    FRestarting := false;
    PrintTop := 0.0; { Start at top of region }
//      SavePrintTop := 0;
    PrintBands(Report,nil,plDetail); { Print all master bands }
    if not Restarting then Break; { No more to print }
    if I1 < Columns then begin { New Column }
      PrintOcc := poNewColumn;
    end else begin { New Page }
      PrintOcc := poNewPage;
      Break;
    end; { else }
  end; { for }
  FCurrentColumn := 1;
  AfterPrint;
end;  { PrintAll }

{ class TRaveBandStyle }

constructor TRaveBandStyle.Create;
begin { Create }
  inherited Create;
  FPrintLoc := [];
  FPrintOcc := [poFirst];
end;  { Create }

procedure TRaveBandStyle.AssignTo(Dest: TPersistent);
begin { AssignTo }
  if Dest is TRaveBandStyle then begin
    TRaveBandStyle(Dest).FPrintLoc := FPrintLoc;
    TRaveBandStyle(Dest).FPrintOcc := FPrintOcc;
  end else begin
    inherited AssignTo(Dest);
  end; { else }
end;  { AssignTo }

{ class TRaveBand }

constructor TRaveBand.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FControlStyle := FControlStyle + [rcsContainer];
  FAnchor := Ord(anvTop) * 16 + Ord(anhStretch);
  Height := 0.2;
  FBandStyle := TRaveBandStyle.Create;
  FReprintLocs := [plBodyHeader,plGroupHeader,plRowHeader,plMaster,plDetail,
   plRowFooter,plGroupFooter,plBodyFooter];
{ FAnchor := (Ord(anvTop) * 16) + Ord(anhStretch); }
end;  { Create }

destructor TRaveBand.Destroy;
begin { Destroy }
  FreeAndNil(FBandStyle);
  inherited Destroy;
end;  { Destroy }

procedure TRaveBand.Changing(OldItem: TRaveComponent;
                             NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(ControllerBand) and (ControllerBand = OldItem) then begin
    ControllerBand := NewItem as TRaveIterateBand;
  end; { if }
  if Assigned(GroupDataView) and (GroupDataView = OldItem) then begin
    GroupDataView := NewItem as TRaveBaseDataView;
  end; { if }
end;  { Changing }

procedure TRaveBand.SetDesignerHide(Value: boolean);
begin { SetDesignerHide }
  if Value <> FDesignerHide then begin
    FDesignerHide := Value;
    Invalidate;
  end; { if }
end;  { SetDesignerHide }

class function TRaveBand.AcceptParent(NewParent: TClass): boolean;
begin { AcceptParent }
  Result := NewParent.InheritsFrom(TRaveRegion);
end;  { AcceptParent }

procedure TRaveBand.SetParent(Value: TRaveComponent);
begin { SetParent }
  inherited SetParent(Value);
end;  { SetParent }

procedure TRaveBand.SetName(const NewName: TComponentName);
begin { SetName }
  inherited SetName(NewName);
  invalidate;
end;  { SetName }

procedure TRaveBand.SetControllerBand(Value: TRaveIterateBand);
begin { SetControllerBand }
  if Assigned(Value) and (Value.Parent <> Parent) then Exit;
  if Assigned(FControllerBand) then begin
    ControllerBand.RemoveControlled(self);
  end; { if }
  FControllerBand := Value;
  if Assigned(FControllerBand) then begin
    ControllerBand.AddControlled(self);
  end; { if }
  Invalidate;
end;  { SetControllerBand }

procedure TRaveBand.SetBandStyle(Value: TRaveBandStyle);
begin { SetBandStyle }
  FBandStyle.Assign(Value);
  Invalidate;
end;  { SetBandStyle }

function TRaveBand.GetRegion: TRaveRegion;
begin { GetRegion }
  Result := TRaveRegion(Parent);
end;  { GetRegion }

function TRaveBand.GetMinHeight: TRaveUnits;
begin { GetMinHeight }
  Result := Height;
end;  { GetMinHeight }

function TRaveBand.GetLeft: TRaveUnits;
begin { GetLeft }
  if Assigned(Region) then begin
    Result := Region.ColumnStart;
  end else begin
    Result := 0.0;
  end; { else }
end;  { GetLeft }

function TRaveBand.GetWidth: TRaveUnits;
begin { GetWidth }
  if Assigned(Region) then begin
    Result := Region.ColumnWidth;
  end else begin
    Result := 0.0;
  end; { else }
end;  { GetWidth }

procedure TRaveBand.AnchorAdjust(WidthChanging: boolean;
                                 OldWidth,NewWidth: TRaveUnits;
                                 HeightChanging: boolean;
                                 OldHeight,NewHeight: TRaveUnits);
begin { AnchorAdjust }
  if WidthChanging then begin
    AnchorAdjustAll(WidthChanging,Region.GetBandWidth(OldWidth),
     Region.GetBandWidth(NewWidth),false,Height,Height);
  end; { if }
end;  { AnchorAdjust }

procedure TRaveBand.SaveState;
begin { SaveState }
  if Saving then begin
    RaveError(Trans('SaveState called while already saving.'));
  end; { if }
  FSaving := true;
end;  { SaveState }

procedure TRaveBand.RestoreState;
begin { RestoreState }
  if not Saving then begin
    RaveError(Trans('RestoreState called while not saving.'));
  end; { if }
  FSaving := false;
end;  { RestoreState }

function TRaveBand.GetSaving: boolean;
begin { GetSaving }
  Result := FSaving;
  if not Result and Assigned(ControllerBand) then begin
    Result := ControllerBand.Saving;
  end; { if }
end;  { GetSaving }

procedure TRaveBand.Paint(Canvas: TCanvas);
begin { Paint }
  with Canvas do begin
    Pen.Color := ModColor(clWhite);
    Pen.Style := psSolid;
    Pen.Mode := pmCopy;
    Pen.Width := 1;
    Brush.Color := ModColor(clWhite);
    Brush.Style := bsSolid;
  { Draw rectangle one pixel in on each side to account for gray boarder }
    Rectangle(XI2D(PageLeft) + 1,YI2D(PageTop) + 1,
     XI2D(PageRight),YI2D(PageBottom));
  end; { with }
end;  { Paint }

procedure TRaveBand.CreatePips;
begin { CreatePips }
  Page.AddPip(6,self,crSizeNS,PageLeft + Width / 2.0,PageBottom);
end;  { CreatePips }

procedure TRaveBand.UpdatePips;
begin { UpdatePips }
  Page.UpdatePip(6,self,PageLeft + Width / 2.0,PageBottom);
end;  { UpdatePips }

function TRaveBand.CanSelect(Item: TRaveComponent): boolean;
begin { CanSelect }
  Result := not DesignerHide and inherited Parent.CanSelect(Item);
end;  { CanSelect }

function TRaveBand.IsSelectPoint(Point: TRavePoint): boolean;
begin { IsSelectPoint }
  Result := ((Point.Y > BandHeaderTop) and (Point.Y < BandHeaderBottom)) or
   (inherited IsSelectPoint(Point) and (not DesignerHide or
   CurrentDesigner.IsSelected(self)));
end;  { IsSelectPoint }

procedure TRaveBand.Resize;
begin { Resize }
{ Resize according to parent }
  inherited Resize;
  if Left <> 0.0 then begin
    Left := 0.0;
    Invalidate;
  end; { if }
end;  { Resize }

procedure TRaveBand.BeforeReport;
begin { BeforeReport }
  LastGroupValue := '';
  inherited BeforeReport;
end;  { BeforeReport }

function TRaveBand.GetCurrGroupValue: string;
var
  S1: string;
  S2: string;
  S3: string;
  O1: TRaveComponent;
  DataView: TRaveBaseDataView;
  Field: TRaveDataField;
  OpCh: char;
  SepCh: char;
begin { GetCurrGroupValue }
  Result := '';
  if GroupKey <> '' then begin
    S1 := GroupKey;
    repeat
      S2 := CutWord(S1,OpCh,'+');
      if S2 <> '' then begin
        S3 := CutWord(S2,SepCh,'.');
        if S2 <> '' then begin
          O1 := TRaveProjectManager(Project).FindRaveComponent(S3, TRaveProjectManager(Project));
          if Assigned(O1) and (O1 is TRaveBaseDataView) then begin
            DataView := TRaveBaseDataView(O1);
          end else begin
            DataView := nil;
          end; { else }
        end else begin
          DataView := GroupDataView;
          S2 := S3; { Copy fieldname back to S2 }
        end; { else }

        if Assigned(DataView) then begin
          if DataView.EOF then begin
            Result := Result + '|'#26 + {Trans-}'EOF'#26;
          end else begin
            Field := DataView.FieldByName(S2);
            if Assigned(Field) then begin
              Result := Result + '|' + Field.AsString;
            end else begin
              Result := Result + {Trans-}'|(invalid)';
            end; { else }
          end; { else }
        end; { if }
      end; { if }
    until S1 = '';
    Result := Result + '|';
  end; { else }
  DoGetGroup(Result);
end;  { GetCurrGroupValue }

function TRaveBand.GetNextGroupValue: string;
begin { GetNextGroupValue }
  if Assigned(GroupDataView) then begin
    GroupDataView.Peek(1);
  end; { if }
  Result := GetCurrGroupValue;
  if Assigned(GroupDataView) then begin
    GroupDataView.Unpeek;
  end; { if }
end;  { GetNextGroupValue }

procedure TRaveBand.Preprint;
begin { Preprint }
end;  { Preprint }

procedure TRaveBand.ResetState;
begin { ResetState }
  LastGroupValue := '';
end;  { ResetState }

procedure TRaveBand.SetBandTop;
begin { SetBandTop }
  case PositionMode of
    pmOffset,pmOverlay: begin
      Top := Region.PrintTop + PositionValue;
    end;
    else begin { pmAbsolute }
      Top := PositionValue;
    end;
  end; { case }
end;  { SetBandTop }

procedure TRaveBand.DoGetGroup(var Value: string);
begin { DoGetGroup }
  if Assigned(FOnGetGroup) then begin
  // Remove framing chars - '|'
    Delete(Value,1,1);
    Delete(Value,Length(Value),1);

    FOnGetGroup.Value := Value;
    FOnGetGroup.Execute;

  // Add framing chars - '|'
    if FOnGetGroup.Value = #255 then begin
      Value := '';
    end else begin
    Value := '|' + FOnGetGroup.Value + '|';
    end; { else }
  end; { if }
end;  { DoGetGroup }

procedure TRaveBand.PrintAll(Report: TBaseReport);
var
  GroupValue: string;
  BandFits: boolean;
  MaxHeight: TRaveUnits;
begin { PrintAll }
  BeforePrint;
  if not AllowPrint then Exit;
  with Region do begin
  { Check to see if this band is where we should restart }
    if not Assigned(RestartBand) then begin
      RestartLoc := PrintLoc;
    end else if (RestartBand = self) and (RestartLoc = PrintLoc) then begin
      PrintOcc := poFirst;
      RestartBand := nil;
      FRestarting := false;
    end; { else }

  { Check to see if this is a group header or group footer on a new group }
    if (PrintLoc in [plGroupHeader,plGroupFooter]) and (PrintOcc = poFirst) then begin
      if PrintLoc = plGroupHeader then begin
        GroupValue := GetCurrGroupValue;
      end else begin { plGroupFooter }
        if LastGroupValue = '' then begin
          LastGroupValue := GetCurrGroupValue;
        end; { if }
        GroupValue := GetNextGroupValue;
      end; { else }
      if (GroupValue = LastGroupValue) then Exit;
      if (LastGroupValue = '') and (PrintLoc = plGroupFooter) then begin
        LastGroupValue := GroupValue;
        Exit;
      end; { if }
    end else begin
      GroupValue := '';
    end; { else }

  { See if this band is supposed to print on this occurrence }
    if not (PrintOcc in BandStyle.PrintOcc) then Exit;

  { See if this band has valid ReprintLocs for a reprint }
    if Assigned(RestartBand) then begin
      if self is TRaveIterateBand then begin
        if not (TRaveIterateBand(self).LocOnRestart in ReprintLocs) then begin
          Exit;
        end; { if }
      end else if not Assigned(ControllerBand) or (not (ControllerBand.LocOnRestart in ReprintLocs)) then begin
        Exit;
      end; { else }
    end; { if }
  end; { with }

  if Region.ForceRestart or (StartNewPage and OnCurrPage) then begin
    Region.Restart(self);
    Exit;
  end; { if }

{ Prepare band for printing }
  RestoreAdjusted;
  if not DoSplit then begin
    InitData;
  end; { if }
  MaxHeight := HeightToPrint;

  SetBandTop;
  case PositionMode of
    pmOffset,pmOverlay: begin
      BandFits := FLE(MaxHeight + PositionValue,Region.HeightAvail);
    end;
    else begin { pmAbsolute }
      BandFits := true;
    end;
  end; { case }

  if FGT(MinHeightLeft,Region.HeightAvail) then begin
    Region.Restart(self);
    Exit;
  end; { if }

  if DoSplit then begin
    ContinuationComponent := self;
  end else begin
    ContinuationComponent := nil;
  end; { else }
  DoSplit := false;
  if not BandFits then begin { Doesn't fit on this page }
    if (not AllowSplit and OnCurrPage) or FGT(AdjHeight,Region.HeightAvail) then begin { Goto next page }
      Region.Restart(self);
      Exit;
    end else begin { Split }
      DoSplit := true;
      MaxHeight := Region.HeightAvail;
    end; { else }
  end; { else }

{ Resize band for MaxHeight }
  Height := MaxHeight;

  OnCurrPage := true;
  if not Saving then begin
    Preprint;
    inherited PrintAll(Report);
    if FGT(Height,0.0) then begin
      Report.NewLine; { Force a new line for RTF/HTML output formats }
    end; { if }
    ContinuationComponent := nil;
  end; { if }

  if PositionMode = pmOffset then begin
    Region.PrintTop := Top + MaxHeight;
  end; { if }

  if (Region.PrintLoc in [plGroupHeader,plGroupFooter]) and
   (GroupValue <> '') then begin
    LastGroupValue := GroupValue;
  end; { if }

  if DoSplit and not DonePrinting then begin { Continue printing this band on the next page }
    Region.Restart(self);
  end else begin
    DoSplit := false;
    DoneData;
    if FinishNewPage then begin
      Region.PostponeRestart;
    end; { if }
  end; { else }
  AfterPrint;
end;  { PrintAll }

{ class TRaveIterateBand }

constructor TRaveIterateBand.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FInitToFirst := true;
  FColumns := 1;
end;  { Create }

destructor TRaveIterateBand.Destroy;
begin { Destroy }
  FreeAndNil(FControlledList);
  FreeAndNil(CalcNewDataListenList);
  inherited Destroy;
end;  { Destroy }

procedure TRaveIterateBand.AddListener(Conversation: string;
                                       ListenMethod: TRaveListenEvent);
begin { AddListener }
  inherited AddListener(Conversation,ListenMethod);
  if AnsiCompareText(Conversation,{Trans-}'CalcNewData') = 0 then begin
    if not Assigned(CalcNewDataListenList) then begin
      CalcNewDataListenList := TRaveMethodList.Create;
    end; { if }
    CalcNewDataListenList.AddMethod(TMethod(ListenMethod));
  end; { if }
end;  { AddListener }

procedure TRaveIterateBand.RemoveListener(Conversation: string;
                                          ListenMethod: TRaveListenEvent);
begin { RemoveListener }
  inherited RemoveListener(Conversation,ListenMethod);
  if (AnsiCompareText(Conversation,{Trans-}'CalcNewData') = 0) and
   Assigned(CalcNewDataListenList) then begin
    CalcNewDataListenList.RemoveMethod(TMethod(ListenMethod));
  end; { if }
end;  { RemoveListener }

function TRaveIterateBand.Habla(Conversation: string): boolean;
begin { Habla }
  Result := AnsiCompareText(Conversation,{Trans-}'CalcNewData') = 0;
end;  { Habla }

procedure TRaveIterateBand.Changing(OldItem: TRaveComponent;
                                    NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(OldItem) and not Assigned(NewItem) and Assigned(FControlledList) and
   (OldItem is TRaveBand) then begin
    FControlledList.Remove(OldItem);
  end; { if }

  if Assigned(CalcNewDataListenList) and Assigned(OldItem) then begin
    if Assigned(NewItem) then begin
      CalcNewDataListenList.ReplaceObject(OldItem,NewItem);
    end else begin
      CalcNewDataListenList.RemoveObject(OldItem);
    end; { else }
  end; { if }
end;  { Changing }

procedure TRaveIterateBand.AddControlled(Value: TRaveBand);
begin { AddControlled }
  if not Assigned(FControlledList) then begin
    FControlledList := TList.Create;
  end; { if }
  FControlledList.Add(Value);
end;  { AddControlled }

procedure TRaveIterateBand.RemoveControlled(Value: TRaveBand);
begin { RemoveControlled }
  if Assigned(FControlledList) then begin
    FControlledList.Remove(Value);
  end; { if }
end;  { RemoveControlled }

procedure TRaveIterateBand.SaveState;
//  var
//    I1: integer;
begin { SaveState }
  inherited SaveState;
//    If Assigned(FControlledList) then begin
//      For I1 := 0 to FControlledList.Count - 1 do begin
//        TRaveBand(FControlledList[I1]).SaveState;
//      end; { for }
//    end; { if }
end;  { SaveState }

procedure TRaveIterateBand.RestoreState;
//  var
//    I1: integer;
begin { RestoreState }
  inherited RestoreState;
//    If Assigned(FControlledList) then begin
//      For I1 := 0 to FControlledList.Count - 1 do begin
//        TRaveBand(FControlledList[I1]).RestoreState;
//      end; { for }
//    end; { if }
end;  { RestoreState }

function TRaveIterateBand.GetColumnStart: TRaveUnits;
begin { GetColumnStart }
  if TRaveProjectManager(Project).Printing and (Columns > 1) then begin
    Result := (ColumnWidth + ColumnSpacing) * (CurrentColumn - 1);
  end else begin
    Result := 0.0;
  end; { else }
end;  { GetColumnStart }

function TRaveIterateBand.GetColumnWidth: TRaveUnits;
begin { GetColumnWidth }
  if Assigned(Region) then begin
    Result := (Region.ColumnWidth - ((Columns - 1) * ColumnSpacing)) / Columns;
  end else begin
    Result := 0.0;
  end; { else }
end;  { GetColumnWidth }

procedure TRaveIterateBand.SetColumns(Value: integer);
begin { SetColumns }
  if (FColumns <> Value) and (Value > 0) then begin
    FColumns := Value;
    ResizeAll;
  end; { if }
end;  { SetColumns }

procedure TRaveIterateBand.SetColumnSpacing(Value: TRaveUnits);
begin { SetColumnSpacing }
  if FColumnSpacing <> Value then begin
    FColumnSpacing := Value;
    ResizeAll;
  end; { if }
end;  { SetColumnSpacing }

function TRaveIterateBand.GetLeft: TRaveUnits;
begin { GetLeft }
  if Assigned(Region) then begin
    Result := Region.ColumnStart + GetColumnStart;
  end else begin
    Result := 0.0;
  end; { else }
end;  { GetLeft }

function TRaveIterateBand.GetWidth: TRaveUnits;
begin { GetWidth }
  Result := ColumnWidth;
end;  { GetWidth }

procedure TRaveIterateBand.Paint(Canvas: TCanvas);
var
  I1: integer;
  I2: integer;
  X1: TRaveUnits;
begin { Paint }
  with Canvas do begin
    Pen.Color := ModColor(clWhite);
    Pen.Style := psSolid;
    Pen.Mode := pmCopy;
    Pen.Width := 1;
    Brush.Style := bsSolid;
    if Columns > 1 then begin
    { Fill whole bar with gray }
      Brush.Color := ModColor(clGray);
      Rectangle(XI2D(PageLeft) + 1,YI2D(PageTop) + 1,
       XI2D(PageRight),YI2D(PageBottom));

    { Draw primary band area }
      Brush.Color := ModColor(clWhite);
      Rectangle(XI2D(PageLeft) + 1,YI2D(PageTop) + 1,
       XI2D(PageLeft + ColumnWidth),YI2D(PageBottom));

    { Draw column separators }
      Brush.Style := bsDiagCross;
      Brush.Color := ModColor(clBlack);
      for I1 := 1 to (Columns - 1) do begin
        if ColumnSpacing = 0.0 then begin { Just draw line }
          I2 := XI2D(PageLeft + (ColumnWidth * I1));
          MoveTo(I2,YI2D(PageTop));
          LineTo(I2,YI2D(PageBottom));
        end else begin { Draw rectangle }
          X1 := PageLeft + ((ColumnWidth + ColumnSpacing) * I1);
          Rectangle(XI2D(X1 - ColumnSpacing),YI2D(PageTop),XI2D(X1),YI2D(PageBottom));
        end; { else }
      end; { for }
    end else begin
    { Draw rectangle one pixel in on each side to account for gray boarder }
      Brush.Color := ModColor(clWhite);
      Rectangle(XI2D(PageLeft) + 1,YI2D(PageTop) + 1,
       XI2D(PageRight),YI2D(PageBottom));
    end; { else }
  end; { with }
end;  { Paint }

procedure TRaveIterateBand.Preprint;
begin { Preprint }
  if NewData then begin
    NewData := false;
    Speak(CalcNewDataListenList,nil); { Notify controlled objects of new data }
  end; { if }
end;  { Preprint }

procedure TRaveIterateBand.SetBandTop;
begin { SetBandTop }
  inherited SetBandTop;
  if Columns > 1 then begin
    FCurrentColumn := FCurrentColumn mod Columns + 1;
    if (CurrentColumn = 1) or (Region.CurrentColumn <> SaveRegionColumn) or
     FLT(SavePriorTop,Region.PrintTop) then begin // Print at current location, column 1
      FCurrentColumn := 1;
      SaveLastTop := Region.PrintTop;
      SaveRegionColumn := Region.CurrentColumn;
      SaveLastBottom := 0;
    end else begin // Restore to last top position
      if SaveLastBottom < Region.PrintTop then begin
        SaveLastBottom := Region.PrintTop;
      end; { if }
      Region.PrintTop := SaveLastTop;
      Top := SaveLastTop;
    end; { else }
  end else begin
    SaveLastBottom := 0;
  end; { if }
end;  { SetBandTop }

procedure TRaveIterateBand.ResetControlled;
var
  I1: integer;
begin { ResetControlled }
  if Assigned(FControlledList) then begin
    for I1 := 0 to FControlledList.Count - 1 do begin
      TRaveBand(FControlledList[I1]).ResetState;
    end; { for }
  end; { if }
end;  { ResetControlled }

procedure TRaveIterateBand.PrintAll(Report: TBaseReport);

  function BandsPrint(PrintLoc: TBandPrintLoc): boolean;
  begin { BandsPrint }
    if PrintLoc = plMaster then begin
      Region.PrintLoc := plMaster;
      inherited PrintAll(Report);
      SavePriorTop := Region.PrintTop;
      if Region.PrintTop < SaveLastBottom then begin
        Region.PrintTop := SaveLastBottom;
      end; { if }
    end else begin
      Region.PrintBands(Report,self,PrintLoc);
    end; { else }
    Result := not Region.Restarting;
    if not Result then begin
      LocOnRestart := PrintLoc;
    end; { if }
  end;  { BandsPrint }

  function IsRowValid: boolean;
  begin { IsRowValid }
  { Need to look at EmptyCheck and figure out if this row will be printing
    based on it's detail sections.  The state variable for this should be
    saved globally so that if two things check it, it doesn't need to
    be recalculated.  The state variable would be reset by a call to First
    or Next. }
    Result := true; //!!!
  end;  { IsRowValid }

  function PrintRowInner: boolean;
  begin { PrintRowInner }
    Result := false;
    if IsRowValid then begin
      if not BandsPrint(plRowHeader) then Exit;
      if not BandsPrint(plMaster) then Exit;
      if not BandsPrint(plDetail) then Exit;
      if not BandsPrint(plRowFooter) then Exit;
    end; { if }
    Result := true;
  end;  { PrintRowInner }

  function PrintRow: boolean;
  var
    Printed: boolean;
//      RowCalc: integer;
  begin { PrintRow }
    Result := false;
    if KeepRowTogether and not Saving and not Region.TopOfRegion then begin
      Region.SaveState;
      SaveState;
      try
        Printed := PrintRowInner;
      finally
        RestoreState;
        Region.RestoreState;
      end; { tryf }
      if not Printed then begin
        Region.PostponeRestart;
      end; { if }
(*
    end else if (WidowRows > 0) and not Region.TopOfRegion then begin
    { FitRows = How many rows will fit on current page
      RowsLeft = After FitRows, how many printing rows remain until EOF

      Calculate FitRows.  If (FitRows > WidowRows) or EOF before the end
      of the page, then the current row can be printed on this page.

      If (FitRows <= WidowRows) and ((FitRows + RowsLeft) > WidowRows) then
      the current row can be printed on this page.

      Otherwise, restart.
    }

      Region.SaveState;
      SaveState;
      RowCalc := 0;
      Printed := true;
      Repeat
        If not EOF then begin
          If Printed then begin
            Printed := PrintRowInner; {!!! Optimize by saving results for next cycle }
          end; { if }
          If IsRowValid then begin {!!! Optimize by saving results for next cycle }
            Inc(RowCalc);
          end; { if }
        end; { if }
        Next;
      until (RowCalc > WidowRows) or EOF;
      RestoreState;
      Region.RestoreState;
      If (RowCalc <= WidowRows) or not Printed then begin { Row can't print }
        Region.PostponeRestart;
      end; { if }
*)
    end; { else }
    if not PrintRowInner then Exit;
    Result := true;
  end;  { PrintRow }

  function PrintBodyInner(MinRows: integer;
                          DoSave: boolean): boolean;
  var
    MinRowCount: integer;
    MaxRowCount: integer;
  begin { PrintBodyInner }
    MinRowCount := MinRows;
    MaxRowCount := MaxRows;
    Result := false;

    if Region.PrintOcc = poNewPage then begin
      FCurrentColumn := 0;
    end; { if }

  { Activate this band section }
    if Region.PrintOcc = poFirst then begin
    { Start at the first column }
      FCurrentColumn := 0;

    { Reset all controlled bands }
      ResetControlled;

    { Initialize data }
      if InitToFirst then begin
        First;
      end else begin
        GetCurrent;
      end; { else }
      if not EOF then begin
        NewData := true;
      end; { if }
    end; { if }

    if DoSave then begin
      Region.SaveState;
      SaveState;
    end; { if }
    try
      if not EOF or Assigned(Region.RestartBand) then begin { Skip body if no records }
        if not BandsPrint(plBodyHeader) then Exit;
        while not EOF do begin
          if not BandsPrint(plGroupHeader) then Exit;
          if not PrintRow then Exit;
          if MinRows > 0 then begin
            Dec(MinRowCount);
            if MinRowCount = 0 then begin { Printed MinRows ok, exit }
              Result := true;
              Exit;
            end; { if }
          end; { if }
          if not BandsPrint(plGroupFooter) then Exit;
          if MaxRows > 0 then begin
            Dec(MaxRowCount);
            if MaxRowCount = 0 then begin { Printed MaxRows, break }
              Result := true;
              Break;
            end; { if }
          end; { if }
          Next;
          if not EOF then begin
            NewData := true;
          end; { if }
        end; { while }
        if not BandsPrint(plBodyFooter) then Exit;
      end; { if }
    finally
      if DoSave then begin
        RestoreState;
        Region.RestoreState;
      end; { if }
    end; { tryf }
    Result := true;
  end;  { PrintBodyInner }

  procedure PrintBody;
  var
    Printed: boolean;
  begin { PrintBody }
    if KeepBodyTogether and not Saving and not Region.TopOfRegion then begin
      Printed := PrintBodyInner(0,true);
      if not Printed then begin
        Region.PostponeRestart;
      end; { if }
    end else if (OrphanRows > 0) and not Saving and not Region.TopOfRegion then begin
    { Check to make sure we can print OrphanRows before continuing }
      Printed := PrintBodyInner(OrphanRows,true);
      if not Printed then begin
        Region.PostponeRestart;
      end; { if }
    end; { else }
    PrintBodyInner(0,false);
  end;  { PrintBody }

begin { PrintAll }
  BeforePrint;
  if not AllowPrint then Exit;

{ Check to skip inactive band section when restarting }
  if Assigned(Region.RestartBand) and not Active then Exit;

{ Print the body of this iterate band }
  Active := true;
  PrintBody;

{ Deactivate band section if not restarting }
  if not Region.Restarting then begin
    Active := false;
  end; { if }
  AfterPrint;
end;  { PrintAll }

{ class TRaveDataBand }

procedure TRaveDataBand.Changing(OldItem: TRaveComponent;
                                 NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(DataView) and (DataView = OldItem) then begin
    DataView := NewItem as TRaveBaseDataView;
  end; { if }
  if Assigned(MasterDataView) and (MasterDataView = OldItem) then begin
    MasterDataView := NewItem as TRaveBaseDataView;
  end; { if }
end;  { Changing }

function TRaveDataBand.GetSaving: boolean;
begin { GetSaving }
  Result := inherited GetSaving;
  if not Result and Assigned(DataView) then begin
    Result := DataView.Saving;
  end; { if }
end;  { GetSaving }

procedure TRaveDataBand.SaveState;
begin { SaveState }
  inherited SaveState;
  if Assigned(DataView) then begin
    DataView.SaveState;
  end; { if }
end;  { SaveState }

procedure TRaveDataBand.RestoreState;
begin { RestoreState }
  inherited RestoreState;
  if Assigned(DataView) then begin
    DataView.RestoreState;
  end; { if }
end;  { RestoreState }

procedure TRaveDataBand.First;
begin { First }
  DataViewFirst(DataView,DetailKey,MasterDataView,MasterKey,SortKey);
end;  { First }

procedure TRaveDataBand.Next;
begin { Next }
  if Assigned(DataView) then begin
    DataView.Next;
  end; { if }
end;  { Next }

procedure TRaveDataBand.GetCurrent;
begin { GetCurrent }
  if Assigned(DataView) then begin
    DataView.GetCurrentRow;
  end; { if }
end;  { GetCurrent }

function TRaveDataBand.Eof: boolean;
begin { Eof }
  if Assigned(DataView) then begin
    Result := DataView.EOF;
  end else begin
    Result := true;
  end; { else }
end;  { Eof }

function TRaveDataBand.CheckRowsLeft(ReqRows: integer): boolean;
begin { CheckRowsLeft }
  if Assigned(DataView) then begin
    Result := DataView.CheckRowsLeft(ReqRows);
  end else begin
    Result := true;
  end; { else }
end;  { CheckRowsLeft }

{ class TRaveDataCycle }

constructor TRaveDataCycle.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FInitToFirst := true;
end;  { Create }

function TRaveDataCycle.GetDonePrinting: boolean;
begin { GetDonePrinting }
  Result := not Active or (Assigned(DataView) and DataView.EOF);
end;  { GetDonePrinting }

procedure TRaveDataCycle.BeforeReport;
begin { BeforeReport }
  RowCount := 0;
  Active := Assigned(DataView);
  inherited BeforeReport;
end;  { BeforeReport }

procedure TRaveDataCycle.Changing(OldItem: TRaveComponent;
                                  NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(DataView) and (DataView = OldItem) then begin
    DataView := NewItem as TRaveBaseDataView;
  end; { if }
  if Assigned(MasterDataView) and (MasterDataView = OldItem) then begin
    MasterDataView := NewItem as TRaveBaseDataView;
  end; { if }
end;  { Changing }

procedure TRaveDataCycle.Print(Report: TBaseReport);
begin { Print }
  if not (AllowPrint and Active) then Exit;
  if RowCount = 0 then begin
    if InitToFirst then begin
      DataViewFirst(DataView,DetailKey,MasterDataView,MasterKey,SortKey);
    end else begin
      DataView.GetCurrentRow;
    end; { else }
  end else begin
    DataView.Next;
  end; { else }
  TRaveDataCycle(Page).InitData; // Override access to protected method
  Inc(RowCount);
  Active := not DataView.CheckBeforeEOF or ((MaxRows > 0) and (RowCount < MaxRows));
end;  { Print }

end.
