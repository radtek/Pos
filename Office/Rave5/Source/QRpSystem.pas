{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpSystem;

interface

uses
  Types, QGraphics, QControls, QForms, QDialogs, QStdCtrls, QExtCtrls, QButtons,
  SysUtils, Classes, QRpDefine, QRpBase, QRpFiler, QRpDevice, QRpRender,
  QRpRenderPrinter, QRpRenderPreview;

type
  TOverrideMode = (omCreate,omShow,omWait,omFree);
  TRvSystem = class;
  TRvSystemEvent = procedure(    ReportSystem: TRvSystem;
                                 OverrideMode: TOverrideMode;
                             var OverrideForm: TForm) of object;

{ class TSystemPrinter }

  TSystemPrinter = class(TPersistent)
  protected
    FCopies: integer;
    FFirstPage: integer;
    FLastPage: integer;
    FLineHeightMethod: TLineHeightMethod;
    FLinesPerInch: integer;
    FMarginBottom: double;
    FMarginLeft: double;
    FMarginRight: double;
    FMarginTop: double;
    FOrientation: TOrientation;
    FScaleX: double;
    FScaleY: double;
    FStatusFormat: TFormatString;
    FStatusText: TStrings;
    FTabShade: integer;
    FTextBKMode: TBKMode;
    FTitle: TTitleString;
    FUnits: TPrintUnits;
    FUnitsFactor: double;
    FCollate: boolean;
    FDuplex: TDuplex;
    //
    procedure SetStatusText(Value: TStrings);
    procedure SetTabShade(Value: integer);
    procedure SetUnits(Value: TPrintUnits);
    procedure SetUnitsFactor(Value: double);
  public
    constructor Create;
    destructor Destroy; override;
    procedure InitPrinter(BaseReport: TBaseReport);
  published
    property Copies: integer read FCopies write FCopies default 1;
    property Collate: boolean read FCollate write FCollate default false;
    property Duplex: TDuplex read FDuplex write FDuplex default dupSimplex;
    property FirstPage: integer read FFirstPage write FFirstPage default 1;
    property LastPage: integer read FLastPage write FLastPage default 9999;
    property LineHeightMethod: TLineHeightMethod read FLineHeightMethod write FLineHeightMethod default lhmFont;
    property LinesPerInch: integer read FLinesPerInch write FLinesPerInch default 6;
    property MarginBottom: double read FMarginBottom write FMarginBottom;
    property MarginLeft: double read FMarginLeft write FMarginLeft;
    property MarginRight: double read FMarginRight write FMarginRight;
    property MarginTop: double read FMarginTop write FMarginTop;
    property Orientation: TOrientation read FOrientation write FOrientation default poPortrait;
    property ScaleX: double read FScaleX write FScaleX;
    property ScaleY: double read FScaleY write FScaleY;
    property StatusFormat: TFormatString read FStatusFormat write FStatusFormat;
    property StatusText: TStrings read FStatusText write SetStatusText;
    property TabShade: integer read FTabShade write SetTabShade default 0;
    property TextBKMode: TBKMode read FTextBKMode write FTextBKMode default bkTransparent;
    property Title: TTitleString read FTitle write FTitle;
    property Units: TPrintUnits read FUnits write SetUnits default unInch;
    property UnitsFactor: double read FUnitsFactor write SetUnitsFactor;
  end; { TSystemPrinter }

{ class TSystemPreview }

  TSystemPreview = class(TPersistent)
  protected
    FGridHoriz: double;
    FGridPen: TPen;
    FGridVert: double;
    FMarginMethod: TMarginMethod;
    FMarginPercent: double;
    FMonochrome: boolean;
    FPagesWide: integer;
    FPagesHigh: integer;
    FPageInc: integer;
    FRulerType: TRulerType;
    FShadowDepth: integer;
    FZoomFactor: double;
    FZoomInc: integer;
    FFormWidth: integer;
    FFormHeight: integer;
    FFormState: TWindowState;
    //
    procedure SetMonochrome(Value: boolean);
    procedure SetShadowDepth(Value: integer);
    procedure SetZoomFactor(Value: double);
  public
    constructor Create;
    destructor Destroy; override;
    procedure InitPreview(RenderPreview: TRvRenderPreview);
  published
    property FormWidth: integer read FFormWidth write FFormWidth default 615;
    property FormHeight: integer read FFormHeight write FFormHeight default 450;
    property FormState: TWindowState read FFormState write FFormState default wsNormal;
    property GridHoriz: double read FGridHoriz write FGridHoriz;
    property GridPen: TPen read FGridPen write FGridPen;
    property GridVert: double read FGridVert write FGridVert;
    property MarginMethod: TMarginMethod read FMarginMethod write FMarginMethod default mmFixed;
    property MarginPercent: double read FMarginPercent write FMarginPercent;
    property Monochrome: boolean read FMonochrome write SetMonochrome default false;
    property PagesWide: integer read FPagesWide write FPagesWide default 1;
    property PagesHigh: integer read FPagesHigh write FPagesHigh default 1;
    property PageInc: integer read FPageInc write FPageInc default 1;
    property RulerType: TRulerType read FRulerType write FRulerType default rtNone;
    property ShadowDepth: integer read FShadowDepth write SetShadowDepth default 0;
    property ZoomFactor: double read FZoomFactor write SetZoomFactor;
    property ZoomInc: integer read FZoomInc write FZoomInc default 10;
  end; { TSystemPreview }

{ class TSystemFiler }

  TSystemFiler = class(TPersistent)
  protected
    FAccuracyMethod: TAccuracyMethod;
    FFileName: TFileName;
    FStatusFormat: TFormatString;
    FStatusText: TStrings;
    FStreamMode: TStreamMode;
    FStream: TStream;
    FIgnoreRPTF: boolean;
    //
    procedure SetStatusText(Value: TStrings);
  public
    constructor Create;
    destructor Destroy; override;
    procedure InitFiler(BaseReport: TBaseReport);
    //
    property Stream: TStream read FStream write FStream;
    property IgnoreRPTF: boolean read FIgnoreRPTF write FIgnoreRPTF;
  published
    property AccuracyMethod: TAccuracyMethod read FAccuracyMethod write FAccuracyMethod default amPositioning;
    property FileName: TFileName read FFileName write FFileName;
    property StatusFormat: TFormatString read FStatusFormat write FStatusFormat;
    property StatusText: TStrings read FStatusText write SetStatusText;
    property StreamMode: TStreamMode read FStreamMode write FStreamMode default smMemory;
  end; { TSystemFiler }

{ class TRvSystem }

  TRvSystem = class(TRPBaseComponent)
  protected
    FDoNativeOutput: boolean;
    FAborted: boolean;
    FBaseReport: TBaseReport;
    SaveLabel: TLabel;
    FDefaultDest: TReportDest;
    FReportDest: TReportDest;
    FOutputFileName: TFileName;
    ForceMultiPrint: boolean;

    FSystemSetups: TSystemSetups;
    FSystemOptions: TSystemOptions;

    FSystemFiler: TSystemFiler;
    FSystemPreview: TSystemPreview;
    FSystemPrinter: TSystemPrinter;
    FTitleSetup: TFormatString;
    FTitleStatus: TFormatString;
    FTitlePreview: TFormatString;
    FStorageMacroData: TStrings;
    FRenderObject: TRPRender;

  { Public Event methods }
    FOnRenderPage: TNotifyEvent;
    FOnRenderData: TNotifyEvent;

  { Published Event methods }
    FOnPrint: TNotifyEvent;
    FOnPrintPage: TPrintPageEvent;
    FOnBeforePrint: TNotifyEvent;
    FOnAfterPrint: TNotifyEvent;
    FOnAfterPreviewPrint: TNotifyEvent;
    FOnNewPage: TNotifyEvent;
    FOnNewColumn: TNotifyEvent;
    FOnPrintHeader: TNotifyEvent;
    FOnPrintFooter: TNotifyEvent;
    FOnEndOfSection: TNotifyEvent;
    FOnDecodeImage: TDecodeImageEvent;
    FOnPreviewSetup: TNotifyEvent;
    FOnPreviewShow: TNotifyEvent;
    FOnOverrideSetup: TRvSystemEvent;
    FOnOverrideStatus: TRvSystemEvent;
    FOnOverridePreview: TRvSystemEvent;

    procedure SetRenderObject(const Value: TRPRender);
    procedure InitEvents(BaseReport: TBaseReport);
    function GetStream: TStream;
    procedure SetStream(Value: TStream);
    procedure OverrideSetupProc(    ReportSystem: TRvSystem;
                                    OverrideMode: TOverrideMode;
                                var OverrideForm: TForm); virtual;
    procedure OverrideStatusProc(    ReportSystem: TRvSystem;
                                     OverrideMode: TOverrideMode;
                                 var OverrideForm: TForm); virtual;
    procedure OverridePreviewProc(    ReportSystem: TRvSystem;
                                      OverrideMode: TOverrideMode;
                                  var OverrideForm: TForm); virtual;
    procedure SetupReport(var SetupForm: TForm);
    procedure GenerateReport(var StatusForm: TForm);
    procedure PrintReport(var StatusForm: TForm);
    procedure PreviewReport(var PreviewForm: TForm);
    procedure SetStorageMacroData(Value: TStrings);
    procedure InitRenderStream(var RenderStream: TStream);
    procedure PrintRender(var StatusForm: TForm);

  public
  { Report Methods - public }
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

    procedure Execute;
    procedure Generate;
    procedure PreviewShowEvent(Sender: TObject);

    property Stream: TStream read GetStream write SetStream;
    property ReportDest: TReportDest read FReportDest write FReportDest;
    property OutputFileName: TFileName read FOutputFileName write FOutputFileName;
    property BaseReport: TBaseReport read FBaseReport write FBaseReport;
    property Aborted: boolean read FAborted write FAborted;
    property StorageMacroData: TStrings read FStorageMacroData write SetStorageMacroData;
    property RenderObject: TRPRender read FRenderObject write SetRenderObject;
    property DoNativeOutput: boolean read FDoNativeOutput write FDoNativeOutput default true;
    property OnRenderPage: TNotifyEvent read FOnRenderPage write FOnRenderPage;
    property OnRenderData: TNotifyEvent read FOnRenderData write FOnRenderData;
  published
    property TitleSetup: TFormatString read FTitleSetup write FTitleSetup;
    property TitleStatus: TFormatString read FTitleStatus write FTitleStatus;
    property TitlePreview: TFormatString read FTitlePreview write FTitlePreview;
    property SystemSetups: TSystemSetups read FSystemSetups write FSystemSetups
     default [ssAllowSetup,ssAllowCopies,ssAllowCollate,ssAllowDuplex,
      ssAllowDestPreview,ssAllowDestPrinter,ssAllowDestFile,ssAllowPrinterSetup,
      ssAllowPreviewSetup];
    property SystemOptions: TSystemOptions read FSystemOptions write FSystemOptions
     default [soShowStatus,soAllowPrintFromPreview,soPreviewModal];
    property DefaultDest: TReportDest read FDefaultDest write FDefaultDest
     default rdPreview;

  { System properties }
    property SystemFiler: TSystemFiler read FSystemFiler write FSystemFiler;
    property SystemPreview: TSystemPreview read FSystemPreview write FSystemPreview;
    property SystemPrinter: TSystemPrinter read FSystemPrinter write FSystemPrinter;

  { Event Properties - redeclare as published }
    property OnPrint: TNotifyEvent read FOnPrint write FOnPrint;
    property OnPrintPage: TPrintPageEvent read FOnPrintPage write FOnPrintPage;
    property OnBeforePrint: TNotifyEvent read FOnBeforePrint write
     FOnBeforePrint;
    property OnAfterPrint: TNotifyEvent read FOnAfterPrint write FOnAfterPrint;
    property OnAfterPreviewPrint: TNotifyEvent read FOnAfterPreviewPrint write FOnAfterPreviewPrint;
    property OnNewPage: TNotifyEvent read FOnNewPage write FOnNewPage;
    property OnNewColumn: TNotifyEvent read FOnNewColumn write FOnNewColumn;
    property OnPrintHeader: TNotifyEvent read FOnPrintHeader write FOnPrintHeader;
    property OnPrintFooter: TNotifyEvent read FOnPrintFooter write FOnPrintFooter;
    property OnEndOfSection: TNotifyEvent read FOnEndOfSection write FOnEndOfSection;
    property OnDecodeImage: TDecodeImageEvent read FOnDecodeImage write FOnDecodeImage;

    property OnPreviewSetup: TNotifyEvent read FOnPreviewSetup write FOnPreviewSetup;
    property OnPreviewShow: TNotifyEvent read FOnPreviewShow write FOnPreviewShow;
    property OverrideSetup: TRvSystemEvent read FOnOverrideSetup write FOnOverrideSetup;
    property OverrideStatus: TRvSystemEvent read FOnOverrideStatus write FOnOverrideStatus;
    property OverridePreview: TRvSystemEvent read FOnOverridePreview write FOnOverridePreview;
  end; { TRvSystem }
  TReportSystem = TRvSystem;

implementation

uses
  QRpFormSetup, QRpFormPreview, QRpFormStatus;

{ class TRvSystem }

constructor TRvSystem.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);

  if (AOwner <> nil) and (csDesigning in ComponentState) and not (csReading in AOwner.ComponentState) then begin
    FTitleSetup := Trans('Output Options');
    FTitleStatus := Trans('Report Status');
    FTitlePreview := Trans('Report Preview');
  end; { if }
  FSystemSetups := [ssAllowSetup,ssAllowCopies,ssAllowCollate,ssAllowDuplex,
   ssAllowDestPreview,ssAllowDestPrinter,ssAllowDestFile,ssAllowPrinterSetup,
   ssAllowPreviewSetup];
  FSystemOptions := [soShowStatus,soAllowPrintFromPreview,soPreviewModal];
  FDefaultDest := rdPreview;
  BaseReport := nil;
  FSystemFiler := TSystemFiler.Create;
  FSystemPreview := TSystemPreview.Create;
  FSystemPrinter := TSystemPrinter.Create;
  FStorageMacroData := TStringList.Create;
  FOutputFileName := '';
  FRenderObject := nil;
  FDoNativeOutput := true;
end;  { Create }

destructor TRvSystem.Destroy;
begin { Destroy }
  FreeAndNil(FSystemPrinter);
  FreeAndNil(FSystemPreview);
  FreeAndNil(FSystemFiler);
  FreeAndNil(FStorageMacroData);

  inherited Destroy;
end;  { Destroy }

procedure TRvSystem.InitEvents(BaseReport: TBaseReport);
begin { InitEvents }
  with BaseReport do begin
    OnPrint := FOnPrint;
    OnPrintPage := FOnPrintPage;
    OnBeforePrint := FOnBeforePrint;
    OnAfterPrint := FOnAfterPrint;
    OnNewPage := FOnNewPage;
    OnNewColumn := FOnNewColumn;
    OnPrintHeader := FOnPrintHeader;
    OnPrintFooter := FOnPrintFooter;
    OnEndOfSection := FOnEndOfSection;
    OnDecodeImage := FOnDecodeImage;
  end; { with }
  if BaseReport is TRvNDRWriter then begin
    TRvNDRWriter(BaseReport).OnRenderPage := FOnRenderPage;
    TRvNDRWriter(BaseReport).OnRenderData := FOnRenderData;
  end; { if }
end;  { InitEvents }

procedure TRvSystem.PreviewShowEvent(Sender: TObject);
begin { PreviewShowEvent }
  if Assigned(FOnPreviewShow) then begin
    FOnPreviewShow(Sender);
  end; { if }
end;  { PreviewShowEvent }

procedure TRvSystem.SetStream(Value: TStream);
begin { SetStream }
  SystemFiler.FStream := Value;
end;  { SetStream }

procedure TRvSystem.SetStorageMacroData(Value: TStrings);
begin { SetStorageMacroData }
  FStorageMacroData.Assign(Value);
end;  { SetStorageMacroData }

function TRvSystem.GetStream: TStream;
begin { GetStream }
  Result := SystemFiler.FStream;
end;  { GetStream }

procedure TRvSystem.OverrideSetupProc(    ReportSystem: TRvSystem;
                                          OverrideMode: TOverrideMode;
                                      var OverrideForm: TForm);
begin { OverrideSetupProc }
  case OverrideMode of
    omCreate: begin
      OverrideForm := TRPSetupForm.Create(self);
      OverrideForm.Caption := FTitleSetup;
      (OverrideForm as TRPSetupForm).ReportSystem := ReportSystem;
    end;
    omShow: begin
      with OverrideForm as TRPSetupForm,ReportSystem do begin
        PreviewSetup := false;
        Aborted := ShowModal = mrCancel;
      end; { with }
    end;
    omWait: begin
    { No need to wait since we always ShowModal }
    end;
    omFree: begin
      FreeAndNil(OverrideForm);
    end;
  end; { case }
end;  { OverrideSetupProc }

procedure TRvSystem.OverrideStatusProc(    ReportSystem: TRvSystem;
                                           OverrideMode: TOverrideMode;
                                       var OverrideForm: TForm);
begin { OverrideStatusProc }
  if not (soShowStatus in ReportSystem.SystemOptions) then Exit;

  case OverrideMode of
    omCreate: begin
      OverrideForm := TRPStatusForm.Create(self);
      OverrideForm.Caption := FTitleStatus;
      (OverrideForm as TRPStatusForm).ReportSystem := ReportSystem;
    end;
    omShow: begin
      with OverrideForm as TRPStatusForm,ReportSystem do begin
        BaseReport.StatusLabel := StatusLabel;
        CancelButton.Caption := Trans('Cancel');
        Show;
      end; { with }
    end;
    omWait: begin
      with OverrideForm as TRPStatusForm,ReportSystem do begin
        if soWaitForOK in SystemOptions then begin
          CancelButton.Caption := Trans('Ok');
          CancelButton.ModalResult := mrOk;
          FormClosed := false;
          repeat
            Application.ProcessMessages;
          until FormClosed;
        end; { if }
      end; { with }
    end;
    omFree: begin
      FreeAndNil(OverrideForm);
    end;
  end; { case }
end;  { OverrideStatusProc }

procedure TRvSystem.OverridePreviewProc(    ReportSystem: TRvSystem;
                                            OverrideMode: TOverrideMode;
                                        var OverrideForm: TForm);
begin { OverridePreviewProc }
  case OverrideMode of
    omCreate: begin
      OverrideForm := TRavePreviewForm.Create(self);
      OverrideForm.Caption := ReportSystem.TitlePreview;
      OverrideForm.Width := ReportSystem.SystemPreview.FormWidth;
      OverrideForm.Height := ReportSystem.SystemPreview.FormHeight;
      OverrideForm.WindowState := ReportSystem.SystemPreview.FormState;
      (OverrideForm as TRavePreviewForm).ReportSystem := ReportSystem;
      (OverrideForm as TRavePreviewForm).OnAfterPreviewPrint := FOnAfterPreviewPrint;
    end;
    omShow: begin
      (OverrideForm as TRavePreviewForm).RvRenderPrinter.MacroData.Assign(StorageMacroData);
      (OverrideForm as TRavePreviewForm).RvRenderPreview.MacroData.Assign(StorageMacroData);
      (OverrideForm as TRavePreviewForm).RvRenderPrinter.OnDecodeImage := FOnDecodeImage;
      (OverrideForm as TRavePreviewForm).RvRenderPreview.OnDecodeImage := FOnDecodeImage;

      ReportSystem.SystemPreview.InitPreview((OverrideForm as TRavePreviewForm).RvRenderPreview);
      if Assigned(FOnPreviewSetup) then begin
        FOnPreviewSetup((OverrideForm as TRavePreviewForm).RvRenderPreview);
      end; { if }

      (OverrideForm as TRavePreviewForm).InputFileName := ReportSystem.SystemFiler.Filename;
      (OverrideForm as TRavePreviewForm).InputStream := ReportSystem.SystemFiler.Stream;
      (OverrideForm as TRavePreviewForm).InitFromRPSystem;
    (* *)
      if soPreviewModal in ReportSystem.SystemOptions then begin
        OverrideForm.ShowModal;
      end else begin
        OverrideForm.Show;
      end; { else }
    end;
    omWait: begin
      if not (soPreviewModal in ReportSystem.SystemOptions) then begin
      // Wait for close
        repeat
          Sleep(250);
          Application.ProcessMessages;
        until not OverrideForm.Visible;
      end; { if }
    end;
    omFree: begin
      if (ReportSystem.SystemFiler.StreamMode in [smTempFile,smFile]) then begin
        (OverrideForm as TRavePreviewForm).RvRenderPreview.NDRStream.Free;
        (OverrideForm as TRavePreviewForm).RvRenderPreview.NDRStream := nil;
      end; { if }
      FreeAndNil(OverrideForm);
    end;
  end; { case }
end;  { OverridePreviewProc }

procedure TRvSystem.SetupReport(var SetupForm: TForm);
begin { SetupReport }
  ReportDest := DefaultDest;
  Aborted := false;
  if ssAllowSetup in SystemSetups then begin
    if not Assigned(FOnOverrideSetup) then begin
      FOnOverrideSetup := OverrideSetupProc;
    end; { if }
    FOnOverrideSetup(self,omCreate,SetupForm);
    try
      FOnOverrideSetup(self,omShow,SetupForm);
      FOnOverrideSetup(self,omWait,SetupForm);
    finally
      FOnOverrideSetup(self,omFree,SetupForm);
    end; { tryf }
  end; { if }
end;  { SetupReport }

procedure TRvSystem.GenerateReport(var StatusForm: TForm);
begin { GenerateReport }
  if soNoGenerate in SystemOptions then Exit;

  Aborted := true;
  if not Assigned(FOnOverrideStatus) then begin
    FOnOverrideStatus := OverrideStatusProc;
  end; { if }
  FOnOverrideStatus(self,omCreate,StatusForm);
  BaseReport := nil;
  try
  { Create and init ReportFiler }
    BaseReport := TRvNDRWriter.Create(Owner);
    SystemPrinter.InitPrinter(BaseReport);
    SystemFiler.InitFiler(BaseReport);
    if (ReportDest = rdFile) and not Assigned(RenderObject) then begin
      if DoNativeOutput then begin
      // Output printer native
        BaseReport.OutputName := OutputFileName;
      end else begin
      // Output NDR
        TRvNDRWriter(BaseReport).StreamMode := smFile;
        TRvNDRWriter(BaseReport).FileName := OutputFileName;
      end; { else }
    end; { if }
    InitEvents(BaseReport);
    FOnOverrideStatus(self,omShow,StatusForm);
    BaseReport.Execute;
    StorageMacroData := BaseReport.MacroData;
    Aborted := BaseReport.Aborted;
  finally
    if BaseReport <> nil then begin
      SaveLabel := BaseReport.StatusLabel;
      SystemFiler.FileName := (BaseReport as TRvNDRWriter).FileName;
      BaseReport.Free;
      BaseReport := nil;
    end; { if }
    FOnOverrideStatus(self,omFree,StatusForm);
  end; { try }
end;  { GenerateReport }

procedure TRvSystem.PrintReport(var StatusForm: TForm);
var
  RvRenderPrinter: TRvRenderPrinter;
begin { PrintReport }
  RvRenderPrinter := TRvRenderPrinter.Create(self);
  try
    RvRenderPrinter.OnDecodeImage := FOnDecodeImage;
    RvRenderPrinter.MacroData.Assign(StorageMacroData);
    RvRenderPrinter.OutputFileName := OutputFileName;
    if SystemFiler.FileName <> '' then begin
      RvRenderPrinter.Render(SystemFiler.FileName);
    end else begin
      RvRenderPrinter.Render(SystemFiler.Stream);
    end;
  finally
    FreeAndNil(RvRenderPrinter);
  end;
end;  { PrintReport }

procedure TRvSystem.Generate;
begin { Generate }
  if soNoGenerate in SystemOptions then Exit;

  Aborted := true;
  BaseReport := nil;
  try
  { Create and init NDRWriter }
    BaseReport := TRvNDRWriter.Create(Owner);
    SystemPrinter.InitPrinter(BaseReport);
    SystemFiler.InitFiler(BaseReport);

    if (ReportDest = rdFile) and not Assigned(RenderObject) then begin
      if DoNativeOutput then begin
      // Output printer native
        BaseReport.OutputName := OutputFileName;
      end else begin
      // Output NDR
        TRvNDRWriter(BaseReport).StreamMode := smFile;
        TRvNDRWriter(BaseReport).FileName := OutputFileName;
      end; { else }
    end; { if }
    InitEvents(BaseReport);

  //!!! Install hooks to call back to render object

    BaseReport.Execute;

    StorageMacroData := BaseReport.MacroData;
    Aborted := BaseReport.Aborted;
  finally
    if BaseReport <> nil then begin
      SaveLabel := BaseReport.StatusLabel;
      SystemFiler.FileName := (BaseReport as TRvNDRWriter).FileName;
      FreeAndNil(FBaseReport);
    end; { if }
  end; { try }
end;  { Generate }

procedure TRvSystem.PreviewReport(var PreviewForm: TForm);
begin { PreviewReport }
  if not Assigned(FOnOverridePreview) then begin
    FOnOverridePreview := OverridePreviewProc;
  end; { if }
  FOnOverridePreview(self,omCreate,PreviewForm);
  try
    FOnOverridePreview(self,omShow,PreviewForm);
    FOnOverridePreview(self,omWait,PreviewForm);
  finally
    FOnOverridePreview(self,omFree,PreviewForm);
  end; { tryf }
end;  { PreviewReport }

procedure TRvSystem.Execute;
var
  ReportStream: TMemoryStream;
  SetupForm: TForm;
  StatusForm: TForm;
  PreviewForm: TForm;
  TempFile: file;

  function CreateNDR: boolean; //!!! temporary
  begin
//    Result := (ReportDest = rdPreview) or (soUseFiler in SystemOptions) or
    Result := (soUseFiler in SystemOptions) or
     ForceMultiPrint or ((ReportDest = rdFile) and not DoNativeOutput
     and not (soNoGenerate in SystemOptions));
  end;

begin { Execute }
  SetupForm := nil;
  StatusForm := nil;
  PreviewForm := nil;

  SetupReport(SetupForm);
  if not Aborted then begin
    ForceMultiPrint := (GlobalDevice.MaxCopies <= 1) and (SystemPrinter.Copies > 1);
(*
    If SystemFiler.StreamMode = smMemory then begin
      ReportStream := TMemoryStream.Create;
      Stream := ReportStream;
    end else begin
      ReportStream := nil;
    end; { else }

    try
      if ReportDest = rdPreview then begin
        PreviewReport(PreviewForm);
      end else begin
      end; { else }
*)
(*
      GenerateReport(StatusForm);

      if not Aborted then begin
        if ReportDest in [rdPrinter,rdFile] then begin
          if (ReportDest = rdPrinter) or DoNativeOutput then begin // Printer or Native output
            PrintReport(StatusForm);
          end else if Assigned(RenderObject) then begin // Rendered output
            PrintRender(StatusForm);
          end else begin // NDR output only
            if Assigned(FOnOverrideStatus) then begin
              FOnOverrideStatus(self,omFree,StatusForm);
            end; { if }
          end; { else }
        end else if ReportDest = rdPreview then begin
          PreviewReport(PreviewForm);
        end; { else }
      end; { if }
*)
(*
    finally
      if ReportStream <> nil then begin
        ReportStream.Free;
      end; { if }
      if ((ReportDest = rdPreview) or (soUseFiler in SystemOptions) or
       ForceMultiPrint) and
       (SystemFiler.StreamMode = smTempFile) then begin { Erase temp file }
        AssignFile(TempFile,SystemFiler.FileName);
        try
          Erase(TempFile);
        except
        end; { tryx }
      end; { if }
    end; { tryf }
*)

    if CreateNDR and (SystemFiler.StreamMode = smMemory) then begin
      ReportStream := TMemoryStream.Create;
      Stream := ReportStream;
    end else begin
      ReportStream := nil;
    end; { else }
    try
      GenerateReport(StatusForm);

      if not Aborted then begin
        if ReportDest in [rdPrinter,rdFile] then begin
          if (ReportDest = rdPrinter) or DoNativeOutput then begin // Printer or Native output
            PrintReport(StatusForm);
          end else if Assigned(RenderObject) then begin // Rendered output
            PrintRender(StatusForm);
          end else begin // NDR output only
            if Assigned(FOnOverrideStatus) then begin
              FOnOverrideStatus(self,omFree,StatusForm);
            end; { if }
          end; { else }
        end else if ReportDest = rdPreview then begin
          PreviewReport(PreviewForm);
        end; { else }
      end; { if }
    finally
      if ReportStream <> nil then begin
        ReportStream.Free;
      end; { if }
      if ((ReportDest = rdPreview) or (soUseFiler in SystemOptions) or
       ForceMultiPrint) and
       (SystemFiler.StreamMode = smTempFile) then begin { Erase temp file }
        AssignFile(TempFile,SystemFiler.FileName);
        try
          Erase(TempFile);
        except
        end; { tryx }
      end; { if }
    end; { tryf }
  end; { if }
end;  { Execute }

procedure TRvSystem.SetRenderObject(const Value: TRPRender);
begin
  FRenderObject := Value;
end;

procedure TRvSystem.InitRenderStream(var RenderStream: TStream);
begin
  case SystemFiler.StreamMode of
    smMemory: begin
      RenderStream := TMemoryStream.Create;
      with RenderStream as TMemoryStream do begin
        SystemFiler.Stream.Position := 0;
        LoadFromStream(SystemFiler.Stream);
      end; { with }
    end;
    smFile,smTempFile: begin
      RenderStream := TFileStream.Create(SystemFiler.Filename,fmOpenRead or
       fmShareDenyWrite);
    end;
    smUser: begin
      if SystemFiler.Stream = nil then begin
        RaiseError(Trans('StreamMode is smUser but Stream is nil'));
      end; { if }
      RenderStream := SystemFiler.Stream;
      RenderStream.Position := 0;
    end;
  end; { case }
  RenderStream.Position := 0;
end;

procedure TRvSystem.PrintRender(var StatusForm: TForm);
var
  NDRStream: TStream;
begin
//!!! Add calls to OnOverrideStatus similar to PrintReport
  if Assigned(FOnOverrideStatus) and Assigned(StatusForm) then begin
    FOnOverrideStatus(self,omFree,StatusForm);
  end; { if }
  NDRStream := nil;
  try
    InitRenderStream(NDRStream);
    RenderObject.OutputFileName := OutputFileName;
    RenderObject.Render(NDRStream);
  finally
    if SystemFiler.StreamMode <> smUser then begin
      FreeAndNil(NDRStream);
    end; { if }
  end;
end;

{ class TSystemPrinter }

constructor TSystemPrinter.Create;
begin { Create }
  inherited Create;

  FCopies := 1;
  FFirstPage := 1;
  FLastPage := 9999;
  FLineHeightMethod := lhmFont;
  FLinesPerInch := 6;
  FMarginBottom := 0.0;
  FMarginLeft := 0.0;
  FMarginRight := 0.0;
  FMarginTop := 0.0;
  FOrientation := poPortrait;
  FScaleX := 100.0;
  FScaleY := 100.0;
  FStatusFormat := Trans('Printing page %p');
  FStatusText := TStringList.Create;
  FTabShade := 0;
  FTextBKMode := bkTransparent;
  FTitle := Trans('ReportPrinter Report');
  FUnits := unInch;
  FUnitsFactor := 1.0;
  FCollate := false;
  FDuplex := dupSimplex;
end;  { Create }

destructor TSystemPrinter.Destroy;
begin { Destroy }
  FreeAndNil(FStatusText);

  inherited Destroy;
end;  { Destroy }

procedure TSystemPrinter.SetStatusText(Value: TStrings);
begin { SetStatusText }
  FStatusText.Assign(Value);
end;  { SetStatusText }

procedure TSystemPrinter.SetTabShade(Value: integer);
begin { SetTabShade }
  if Value >= 100 then begin
    FTabShade := 100;
  end else if Value <= 0 then begin
    FTabShade := 0;
  end else begin
    FTabShade := Value;
  end; { else }
end;  { SetTabShade }

procedure TSystemPrinter.SetUnits(Value: TPrintUnits);
begin { SetUnits }
  FUnits := Value;
  case FUnits of
    unInch: begin
      FUnitsFactor := 1.0;
    end;
    unMM: begin
      FUnitsFactor := 25.4;
    end;
    unCM: begin
      FUnitsFactor := 2.54;
    end;
    unPoint: begin
      FUnitsFactor := 72.0;
    end;
    unUser: begin
    { Don't change FUnitsFactor }
    end;
  end; { case }
end;  { SetUnits }

procedure TSystemPrinter.SetUnitsFactor(Value: double);
var
  R1: array [1..4] of double;
begin { SetUnitsFactor }
  if Value > 0.0 then begin
    FUnitsFactor := Value;
    R1[1] := 1.0;
    R1[2] := 25.4;
    R1[3] := 2.54;
    R1[4] := 72.0;
    if (FUnitsFactor = R1[1]) then begin
      FUnits := unInch;
    end else if (FUnitsFactor = R1[2]) then begin
      FUnits := unMM;
    end else if (FUnitsFactor = R1[3]) then begin
      FUnits := unCM;
    end else if (FUnitsFactor = R1[4]) then begin
      FUnits := unPoint;
    end else begin
      FUnits := unUser;
    end; { else }
  end; { if }
end;  { SetUnitsFactor }

procedure TSystemPrinter.InitPrinter(BaseReport: TBaseReport);
begin { InitPrinter }
  with BaseReport do begin
    Copies := FCopies;
    FirstPage := FFirstPage;
    LastPage := FLastPage;
    LineHeightMethod := FLineHeightMethod;
    LinesPerInch := FLinesPerInch;
    MarginBottom := FMarginBottom;
    MarginLeft := FMarginLeft;
    MarginRight := FMarginRight;
    MarginTop := FMarginTop;
    Orientation := FOrientation;
    ScaleX := FScaleX;
    ScaleY := FScaleY;
    StatusFormat := FStatusFormat;
    StatusText := FStatusText;
    TabShade := FTabShade;
    TextBKMode := FTextBKMode;
    Title := FTitle;
    Units := FUnits;
    if FUnits = unUser then begin
      UnitsFactor := FUnitsFactor;
    end; { if }
    if (RPDev = nil) or not RPDev.InvalidPrinter then begin
      Collate := FCollate;
      Duplex := FDuplex;
    end; { if }
  end; { with }
end;  { InitPrinter }

{ class TSystemPreview }

constructor TSystemPreview.Create;
begin { Create }
  inherited Create;

  FFormWidth := 615;
  FFormHeight := 450;
  FFormState := wsNormal;
  FGridHoriz := 0.0;
  FGridPen := TPen.Create;
  FGridVert := 0.0;
  FMarginMethod := mmFixed;
  FMarginPercent := 0.0;
  FMonochrome := false;
  FRulerType := rtNone;
  FShadowDepth := 0;
  FZoomFactor := 100.0;
  FZoomInc := 10;
  FPagesWide := 1;
  FPagesHigh := 1;
  FPageInc := 1;
end;  { Create }

destructor TSystemPreview.Destroy;
begin { Destroy }
  FreeAndNil(FGridPen);

  inherited Destroy;
end;  { Destroy }

procedure TSystemPreview.SetMonochrome(Value: boolean);
begin { SetMonochrome }
  if (ShadowDepth > 0) and Value then begin { Warn programmer }
    ShowMessage({Trans-}'Monochrome not allowed while shadows are in effect.'#13 +
     {Trans-}'Change ShadowDepth to 0 first');
  end else begin
    FMonochrome := Value;
  end; { else }
end;  { SetMonochrome }

procedure TSystemPreview.SetShadowDepth(Value: integer);
begin { SetShadowDepth }
  if (Value > 0) and Monochrome then begin { Warn programmer }
    ShowMessage({Trans-}'Shadows not allowed while monochrome in effect.'#13 +
     {Trans-}'Change Monochrome to false first');
  end else begin
    FShadowDepth := Value;
  end; { else }
end;  { SetShadowDepth }

procedure TSystemPreview.SetZoomFactor(Value: double);
begin { SetZoomFactor }
  if Value < 10.0 then begin
    FZoomFactor := 10.0;
  end else if Value > 200.0 then begin
    FZoomFactor := 200.0;
  end else begin
    FZoomFactor := Value;
  end; { else }
end;  { SetZoomFactor }

procedure TSystemPreview.InitPreview(RenderPreview: TRvRenderPreview);
begin { InitPreview }
  with RenderPreview as TRvRenderPreview do begin
      GridHoriz := FGridHoriz;
      GridPen := FGridPen;
      GridVert := FGridVert;
      MarginMethod := FMarginMethod;
      MarginPercent := FMarginPercent;
      ShadowDepth := FShadowDepth; // Must be set before Monochrome
      Monochrome := FMonochrome;
      PagesWide := FPagesWide;
      PagesHigh := FPagesHigh;
      PageInc := FPageInc;
      RulerType := FRulerType;
      ZoomFactor := FZoomFactor;
      ZoomInc := FZoomInc;
    end; { with }
end;  { InitPreview }

{ class TSystemFiler }

constructor TSystemFiler.Create;
begin { Create }
  inherited Create;

  FAccuracyMethod := amPositioning;
  FFileName := '';
  FStatusFormat := Trans('Generating page %p');
  FStatusText := TStringList.Create;
  FStreamMode := smMemory;
  FStream := nil;
end;  { Create }

destructor TSystemFiler.Destroy;
begin { Destroy }
  FreeAndNil(FStatusText);
  if FStreamMode = smMemory then begin
    FStream.Free;
  end;
  inherited Destroy;
end;  { Destroy }

procedure TSystemFiler.SetStatusText(Value: TStrings);
begin { SetStatusText }
  FStatusText.Assign(Value);
end;  { SetStatusText }

procedure TSystemFiler.InitFiler(BaseReport: TBaseReport);
begin { InitFiler }
  BaseReport.IgnoreRPTF := IgnoreRPTF;
  if BaseReport is TRvNDRWriter then begin
    with BaseReport as TRvNDRWriter do begin
      AccuracyMethod := FAccuracyMethod;
      FileName := FFileName;
      StatusFormat := FStatusFormat;
      StatusText := FStatusText;
      if FStreamMode = smMemory then begin
        StreamMode := smUser;
        if Assigned(Stream) then begin
          Stream.Free;
          Stream := nil;
        end; { if }
        FStream := TMemoryStream.Create;
      end else begin
        StreamMode := FStreamMode;
      end; { else }
      Stream := FStream;
    end; { with }
(*!!PORT!!
  end else if BaseReport is TRvNDRPrinter then begin
    With BaseReport as TRvNDRPrinter do begin
      FileName := FFileName;
      If FStreamMode = smMemory then begin
        StreamMode := smUser;
      end else begin
        StreamMode := FStreamMode;
      end; { else }
      Stream := FStream;
    end; { with }
*)
  end; { else }
end;  { InitFiler }

end.