{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpRave;

interface

uses
  QStdCtrls, QDialogs, QForms, QControls, Qt,
  Windows,
  Classes, SysUtils, QRpDefine, QRvDefine, QRvClass, QRvProj,
  QRvUtil, QRvCmHuff, QRvData, QRvDirectDataView,
  QRvCsStd, QRvCsRpt, QRvCsDraw, QRvCsData, QRvDatabase, QRvDriverDataView,
  QRvLEModule, QRvLESystem, QRvDataLink;

type
  TRvProject = class(TRPComponent)
  protected
    FActive: boolean;
    FProjectFile: string;
    FEngine: TRPComponent;
    FDLLFile: string;
    FLoadDesigner: boolean;
    FStoreRAV: boolean;
    RaveBlob: TMemoryStream;
    FRaveBlobDateTime: TDateTime;
    FProjMan: TRaveProjectManager;
    ProjOwner: TRaveContainerControl;

    FOnCreate: TNotifyEvent;
    FOnDestroy: TNotifyEvent;
    FOnDesignerSave: TNotifyEvent;
    FOnDesignerSaveAs: TNotifyEvent;
    FOnDesignerNew: TNotifyEvent;
    FOnDesignerOpen: TNotifyEvent;
    FOnDesignerShow: TNotifyEvent;
    FOnBeforeOpen: TNotifyEvent;
    FOnAfterOpen: TNotifyEvent;
    FOnBeforeClose: TNotifyEvent;
    FOnAfterClose: TNotifyEvent;

    procedure CreateEvent; virtual;
    procedure DestroyEvent; virtual;
    procedure DesignerShowEvent; virtual;
    procedure BeforeOpenEvent; virtual;
    procedure AfterOpenEvent; virtual;
    procedure BeforeCloseEvent; virtual;
    procedure AfterCloseEvent; virtual;

    procedure Loaded; override;
    function GetReportDesc: string;
    function GetReportName: string;
    function GetReportFullName: string;
    procedure SetActive(Value: boolean);
    procedure Notification(AComponent: TComponent; Operation: TOperation); override;
    procedure SetEngine(Value: TRPComponent);
    procedure ReadRaveBlobOld(Reader: TReader);
    procedure ReadRaveBlob(Stream: TStream);
    procedure WriteRaveBlob(Stream: TStream);
    procedure DefineProperties(Filer: TFiler); override;
    function GetProjMan: TRaveProjectManager;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure New;
    class procedure RegisterRaveUnits;
    procedure Open;
    procedure Close;
    procedure Save;
    procedure Execute;
    procedure ExecuteReport(ReportName: string);
    function Design: boolean;
    function DesignReport(ReportName: string): boolean;
    procedure GetReportList(ReportList: TStrings; FullNames: boolean);
    procedure GetReportCategoryList(ReportList: TStrings;
                                    Categories: string;
                                    FullNames: boolean);
    function SelectReport(ReportName: string; FullName: boolean): boolean;
    procedure ReportDescToMemo(Memo: TCustomMemo);
    procedure SetParam(Param, Value: string);
    function GetParam(Param: string): string;
    procedure ClearParams;
    procedure LoadFromFile(FileName: string);
    procedure LoadFromStream(Stream: TStream);
    procedure SaveToFile(FileName: string);
    procedure SaveToStream(Stream: TStream);
    procedure LoadRaveBlob(Stream: TStream);
    procedure SaveRaveBlob(Stream: TStream);
    procedure ClearRaveBlob;
    procedure SetProjectFile(Value: string);

    property Active: boolean read FActive write SetActive;
    property ProjMan: TRaveProjectManager read GetProjMan;
    property RaveBlobDateTime: TDateTime read FRaveBlobDateTime;
    property ReportDesc: string read GetReportDesc;
    property ReportName: string read GetReportName;
    property ReportFullName: string read GetReportFullName;
  published
    property DLLFile: string read FDLLFile write FDLLFile;
    property Engine: TRPComponent read FEngine write SetEngine default nil;
    property LoadDesigner: boolean read FLoadDesigner write FLoadDesigner default false;
    property ProjectFile: string read FProjectFile write SetProjectFile;
    property StoreRAV: boolean read FStoreRAV write FStoreRAV stored false;

    property OnCreate: TNotifyEvent read FOnCreate write FOnCreate;
    property OnDestroy: TNotifyEvent read FOnDestroy write FOnDestroy;
    property OnDesignerSave: TNotifyEvent read FOnDesignerSave write FOnDesignerSave;
    property OnDesignerSaveAs: TNotifyEvent read FOnDesignerSaveAs write FOnDesignerSaveAs;
    property OnDesignerShow: TNotifyEvent read FOnDesignerShow write FOnDesignerShow;
    property OnBeforeOpen: TNotifyEvent read FOnBeforeOpen write FOnBeforeOpen;
    property OnAfterOpen: TNotifyEvent read FOnAfterOpen write FOnAfterOpen;
    property OnBeforeClose: TNotifyEvent read FOnBeforeClose write FOnBeforeClose;
    property OnAfterClose: TNotifyEvent read FOnAfterClose write FOnAfterClose;
  end; { TRvProject }

  TRaveCreateProc = procedure(RVInfo: pointer;
                              AppHandle: THandle); stdcall;
  TRaveProc = procedure;
  TRaveDesignerCreateProc = procedure(Data: pointer;
                                      SaveCallback: TRaveSaveCallbackProc;
                                      SaveAsCallback: TRaveSaveCallbackProc); stdcall;
  TRaveDesignerLoadProc = procedure(Buf: pointer;
                                    Size: longint); stdcall;
  TRaveDesignerSaveProc = function(var Buf: pointer;
                                   var Size: longint): boolean; stdcall;

var
  RaveCreate: TRaveCreateProc;
  DesignerCreate: TRaveDesignerCreateProc;
  DesignerLoad: TRaveDesignerLoadProc;
  DesignerShow: TRaveProc;
  DesignerSave: TRaveDesignerSaveProc;
  DesignerFree: TRaveProc;
  RaveFree: TRaveProc;

  function LoadRaveDLL(DLLName: string): THandle;
  procedure CloseRaveDLL;

implementation

{$L RpInfo.dat}

// Procs
  procedure RVInfo; external;

type
  TRaveMemoryStream = class(TCustomMemoryStream)
  public
    procedure SetBuf(Buf: pointer;
                     Size: longint);
    function Write(const Buffer;
                         Count: longint): longint; override;
  end; { TRaveMemoryStream }

var
  RaveModule: THandle = 0;

procedure RaveSaveCallback(Buf: pointer; Size: longint; Data: pointer); stdcall;
var
  Stream: TRaveMemoryStream;
begin { RaveSaveCallback }
  with TObject(Data) as TRvProject do begin
    Stream := TRaveMemoryStream.Create;
    try
      Stream.SetBuf(Buf,Size);
      LoadFromStream(Stream);
    finally
      Stream.Free;
    end; { tryf }
    if Assigned(FOnDesignerSave) then begin
      FOnDesignerSave(TObject(Data));
    end; { if }
  end; { with }
end;  { RaveSaveCallback }

procedure RaveSaveAsCallback(Buf: pointer; Size: longint; Data: pointer); stdcall;
var
  Stream: TRaveMemoryStream;
begin { RaveSaveAsCallback }
  with TObject(Data) as TRvProject do begin
    Stream := TRaveMemoryStream.Create;
    try
      Stream.SetBuf(Buf,Size);
      LoadFromStream(Stream);
    finally
      Stream.Free;
    end; { tryf }
    if Assigned(FOnDesignerSaveAs) then begin
      FOnDesignerSaveAs(TObject(Data));
    end; { if }
  end; { with }
end;  { RaveSaveAsCallback }

function LoadRaveDLL(DLLName: string): THandle;
var
  S1: string;
begin { LoadRaveDLL }
  if RaveModule = 0 then begin
    if DLLName = '' then begin
      S1 := 'RAVE.DLL';
    end else begin
      S1 := DLLName;
    end; { else }
    RaveModule := LoadLibrary(@S1[1]);
    if RaveModule < HINSTANCE_ERROR then begin
      RaveModule := 0;
      RaiseError(Trans('Error loading') + ' ' + S1);
    end else begin
      @RaveCreate := GetProcAddress(RaveModule,'RaveCreate');
      @DesignerCreate := GetProcAddress(RaveModule,'DesignerCreate');
      @DesignerLoad := GetProcAddress(RaveModule,'DesignerLoad');
      @DesignerShow := GetProcAddress(RaveModule,'DesignerShow');
      @DesignerSave := GetProcAddress(RaveModule,'DesignerSave');
      @DesignerFree := GetProcAddress(RaveModule,'DesignerFree');
      @RaveFree := GetProcAddress(RaveModule,'RaveFree');
      //!!PORT!! RPRAVE03
    end; { else }
  end; { if }
  Result := RaveModule;
end;  { LoadRaveDLL }

procedure CloseRaveDLL;
begin { CloseRaveDLL }
  if RaveModule <> 0 then begin
    RaveFree; { Free Rave System }
    try
      FreeLibrary(RaveModule);
    finally
      RaveModule := 0;
    end; { tryf }
  end; { if }
end;  { CloseRaveDLL }

{ class TRaveMemoryStream }

procedure TRaveMemoryStream.SetBuf(Buf: pointer; Size: longint);
begin { SetBuf }
  SetPointer(Buf,Size);
  Position := 0;
end;  { SetBuf }

function TRaveMemoryStream.Write(const Buffer;
                                       Count: longint): longint;
begin { Write }
  Result := 0;
end;  { Write }

{ class TRvProject }

constructor TRvProject.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  ProjOwner := TRaveContainerControl.Create(nil);
  FProjMan := TRaveProjectManager.Create(ProjOwner);
  FProjMan.Name := 'RaveProject';
  if not (csDesigning in ComponentState) and not RaveInitialized then begin
    if not Assigned(RaveDataSystem) then begin
      RaveDataSystem := TRaveDataSystem.Create;
    end; { if }
    RegisterRaveUnits;
    AddStaticDataLinks;
    RaveInitialized := true;
  end; { if }
end;  { Create }

destructor TRvProject.Destroy;
begin { Destroy }
  Close;
  DestroyEvent;
  ClearRaveBlob;
  FreeAndNil(FProjMan);
  FreeAndNil(ProjOwner);
  inherited Destroy;
end;  { Destroy }

procedure TRvProject.Loaded;
begin { Loaded }
  CreateEvent;
end;  { Loaded }

procedure TRvProject.CreateEvent;
begin { CreateEvent }
  if Assigned(FOnCreate) then begin
    FOnCreate(self);
  end; { if }
end;  { CreateEvent }

procedure TRvProject.DestroyEvent;
begin { DestroyEvent }
  if Assigned(FOnDestroy) then begin
    FOnDestroy(self);
  end; { if }
end;  { DestroyEvent }

procedure TRvProject.DesignerShowEvent;
begin { DesignerShowEvent }
  if Assigned(FOnDesignerShow) then begin
    FOnDesignerShow(self);
  end; { if }
end;  { DesignerShowEvent }

procedure TRvProject.BeforeOpenEvent;
begin { BeforeOpenEvent }
  if Assigned(FOnBeforeOpen) then begin
    FOnBeforeOpen(self);
  end; { if }
end;  { BeforeOpenEvent }

procedure TRvProject.AfterOpenEvent;
begin { AfterOpenEvent }
  if Assigned(FOnAfterOpen) then begin
    FOnAfterOpen(self);
  end; { if }
end;  { AfterOpenEvent }

procedure TRvProject.BeforeCloseEvent;
begin { BeforeCloseEvent }
  if Assigned(FOnBeforeClose) then begin
    FOnBeforeClose(self);
  end; { if }
end;  { BeforeCloseEvent }

procedure TRvProject.AfterCloseEvent;
begin { AfterCloseEvent }
  if Assigned(FOnAfterClose) then begin
    FOnAfterClose(self);
  end; { if }
end;  { AfterCloseEvent }

procedure TRvProject.Notification(AComponent: TComponent; Operation: TOperation);
begin { Notification }
  inherited Notification(AComponent,Operation);
  if (Operation = opRemove) and (AComponent = FEngine) then begin
    FEngine := nil;
  end; { if }
end;  { Notification }

procedure TRvProject.SetEngine(Value: TRPComponent);
begin { SetEngine }
  FEngine := Value;
  if Assigned(Value) then begin
    Value.FreeNotification(self);
  end; { if }
end;  { SetEngine }

procedure TRvProject.ReadRaveBlobOld(Reader: TReader);
var
  BlobSize: integer;
begin { ReadRaveBlobOld }
  if not Assigned(RaveBlob) then begin
    RaveBlob := TMemoryStream.Create;
  end; { if }
  BlobSize := Reader.ReadInteger;
  FRaveBlobDateTime := Reader.ReadFloat;
  RaveBlob.SetSize(BlobSize);
  Reader.Read(RaveBlob.Memory^,BlobSize);
end;  { ReadRaveBlobOld }

procedure TRvProject.ReadRaveBlob(Stream: TStream);
var
  BlobSize: integer;
begin { ReadRaveBlob }
  if not Assigned(RaveBlob) then begin
    RaveBlob := TMemoryStream.Create;
  end else begin
    RaveBlob.Clear;
  end; { else }
  Stream.Read(BlobSize,SizeOf(BlobSize));
  Stream.Read(FRaveBlobDateTime,SizeOf(FRaveBlobDateTime));
  RaveBlob.CopyFrom(Stream,BlobSize);
end;  { ReadRaveBlob }

procedure TRvProject.WriteRaveBlob(Stream: TStream);
var
  I1: integer;
begin { WriteRaveBlob }
  I1 := RaveBlob.Size;
  Stream.Write(I1,SizeOf(I1));
  Stream.Write(FRaveBlobDateTime,SizeOf(FRaveBlobDateTime));
  RaveBlob.Position := 0;
  Stream.CopyFrom(RaveBlob,RaveBlob.Size);
end;  { WriteRaveBlob }

procedure TRvProject.DefineProperties(Filer: TFiler);
begin { DefineProperties }
  inherited DefineProperties(Filer);
  Filer.DefineProperty('RaveBlob',ReadRaveBlobOld,nil,false);
  Filer.DefineBinaryProperty('RaveBlobNew',ReadRaveBlob,WriteRaveBlob,Assigned(RaveBlob));
end;  { DefineProperties }

procedure TRvProject.LoadRaveBlob(Stream: TStream);
begin { LoadRaveBlob }
  if not Assigned(RaveBlob) then begin
    RaveBlob := TMemoryStream.Create;
  end; { if }
  RaveBlob.Clear;
  RaveBlob.CopyFrom(Stream,0);
  FRaveBlobDateTime := Now;
end;  { LoadRaveBlob }

procedure TRvProject.SaveRaveBlob(Stream: TStream);
begin { SaveRaveBlob }
  if Assigned(RaveBlob) then begin
    Stream.CopyFrom(RaveBlob,0);
  end; { if }
end;  { SaveRaveBlob }

procedure TRvProject.ClearRaveBlob;
begin { CreateRaveBlob }
  if Assigned(RaveBlob) then begin
    RaveBlob.Free;
    RaveBlob := nil;
    FRaveBlobDateTime := 0.0;
  end; { if }
end;  { CreateRaveBlob }

procedure TRvProject.SetProjectFile(Value: string);
begin { SetProjectFile }
  if (Pos('.',Value) = 0) and (Value <> '') { no file type specified } then begin
    FProjectFile := Value + '.rav';
  end else begin
    FProjectFile := Value;
  end; { else }
end;  { SetProjectFile }

procedure TRvProject.SetActive(Value: boolean);
begin { SetActive }
  if Value = Active then Exit;
  GetProjMan;
  if Value then begin
    Open;
  end else begin
    Close;
  end; { else }
end;  { SetActive }

function TRvProject.GetProjMan: TRaveProjectManager;
begin { GetProjMan }
  if Assigned(FProjMan) and Assigned(FProjMan.ActiveReport) and Assigned(FProjMan.ActiveReport.ExecProject) then begin
    Result := FProjMan.ActiveReport.ExecProject;
  end else begin
    Result := FProjMan;
  end; { else }
end;  { GetProjMan }

procedure TRvProject.New;
begin { New }
  GetProjMan;
  if Active then begin
    ProjMan.Unload;
  end else begin
    BeforeOpenEvent;
  end; { else }
  ProjMan.New;
  if not Active then begin
    FActive := true;
    AfterOpenEvent;
  end; { if }
end;  { New }

procedure TRvProject.Open;
var
  FileDate: TDateTime;
  LoadFile: boolean;
  DiskDate: integer;
begin { Open }
  if Active then Exit;
  BeforeOpenEvent;
  GetProjMan;

  DiskDate := FileAge(ProjectFile);
  if DiskDate >= 0 then begin
    FileDate := FileDateToDateTime(DiskDate);
  end else begin
    FileDate := 0;
  end; { else }
  LoadFile := not Assigned(RaveBlob);
  if not LoadFile and (FileDate > RaveBlobDateTime) then begin
    LoadFile := MessageDlg(
     'The report project stored in the executable is older than the disk file'#13 +
     '"' + ExpandFileName(ProjectFile) + '"'#13 + 'Do you want to use the file on disk?',
     mtWarning,[mbYes,mbNo],0) = mrYes;
  end; { if }
  if not LoadFile then begin
    RaveBlob.Position := 0;
    ProjMan.LoadFromStream(RaveBlob);
  end else if (ProjectFile <> '') and FileExists(ExpandFileName(ProjectFile)) then begin
    ProjMan.FileName := ExpandFileName(ProjectFile);
    ProjMan.Load;
  end else begin
    RaveError('The following reporting project does not exist:'#13 +
     '"' + ExpandFileName(ProjectFile) + '"');
  end; { else }
  FActive := true;
  AfterOpenEvent;
end;  { Open }

procedure TRvProject.Close;
begin { Close }
  if not Active then Exit;
  BeforeCloseEvent;
  GetProjMan;
  ProjMan.Unload;
  FActive := false;
  AfterCloseEvent;
end;  { Close }

procedure TRvProject.Save;
begin { Save }
  if not Active then Exit;
  GetProjMan;
  ProjMan.FileName := ProjectFile;
  ProjMan.Save;
end;  { Save }

procedure TRvProject.Execute;
begin { Execute }
  Open;
  ProjMan.ActiveReport.Show;
  ProjMan.ActiveReport.Execute(Engine);
  ProjMan.ActiveReport.Hide;
end;  { Execute }

procedure TRvProject.ExecuteReport(ReportName: string);
begin { ExecuteReport }
  Open;
  if SelectReport(ReportName,false) then begin
    ProjMan.ActiveReport.Show;
    ProjMan.ActiveReport.Execute(Engine);
    ProjMan.ActiveReport.Hide;
  end; { if }
end;  { ExecuteReport }

function TRvProject.Design: boolean;
var
  StreamOut: TMemoryStream;
  StreamIn: TRaveMemoryStream;
  Buf: pointer;
  Size: longint;
  SaveProc: TRaveDesignerSaveProc;
  SaveAsProc: TRaveDesignerSaveProc;
begin { Design }
  LoadRaveDLL(DLLFile);
  try
    Open;
    if RaveModule = 0 then begin
      ShowMessage(Trans('The Rave System is not currently loaded'));
      Result := false;
      Exit;
    end; { if }

  { Create visual designer }
    if Assigned(FOnDesignerSave) then begin
      SaveProc := @RaveSaveCallback;
    end else begin
      SaveProc := nil;
    end; { else }
    if Assigned(FOnDesignerSaveAs) then begin
      SaveAsProc := @RaveSaveAsCallback;
    end else begin
      SaveAsProc := nil;
    end; { else }
    if Assigned(SaveProc) or Assigned(SaveAsProc) then begin
      DesignerCreate(self,@SaveProc,@SaveAsProc);
    end else begin
      DesignerCreate(nil,nil,nil);
    end; { else }

  { Stream project out to visual designer }
    StreamOut := TMemoryStream.Create;
    try
      ProjMan.StreamParamValues := true;
      ProjMan.SaveToStream(StreamOut);
      ProjMan.StreamParamValues := false;
      DesignerLoad(StreamOut.Memory,StreamOut.Size);
    finally
      StreamOut.Free;
    end; { tryf }

  { Show visual designer }
    DesignerShowEvent;
    DesignerShow;

  { Check if project was modified }
    Result := DesignerSave(Buf,Size);
    if Result then begin
    { Stream project into ProjMan }
      ProjMan.Unload;
      StreamIn := TRaveMemoryStream.Create;
      try
        StreamIn.SetBuf(Buf,Size);
        ProjMan.LoadFromStream(StreamIn);
      finally
        StreamIn.Free;
      end; { tryf }
    end; { if }

  { Free visual designer }
    DesignerFree;
  finally
    CloseRaveDLL;
  end;
end;  { Design }

function TRvProject.DesignReport(ReportName: string): boolean;
begin { DesignReport }
  SelectReport(ReportName,false);
  Result := Design;
end;  { DesignReport }

procedure TRvProject.LoadFromFile(FileName: string);
var
  Stream: TMemoryStream;
begin { LoadFromFile }
  Stream := TMemoryStream.Create;
  try
    Stream.LoadFromFile(FileName);
    LoadFromStream(Stream);
  finally
    Stream.Free;
  end; { tryf }
end;  { LoadFromFile }

procedure TRvProject.LoadFromStream(Stream: TStream);
begin { LoadFromStream }
  ProjMan.Unload;
  ProjMan.LoadFromStream(Stream);
end;  { LoadFromStream }

procedure TRvProject.SaveToFile(FileName: string);
var
  Stream: TMemoryStream;
begin { SaveToFile }
  Stream := TMemoryStream.Create;
  try
    SaveToStream(Stream);
    Stream.SaveToFile(FileName);
  finally
    Stream.Free;
  end; { tryf }
end;  { SaveToFile }

procedure TRvProject.SaveToStream(Stream: TStream);
begin { SaveToStream }
  ProjMan.SaveToStream(Stream);
end;  { SaveToStream }

procedure TRvProject.GetReportList(ReportList: TStrings; FullNames: boolean);
var
  I1: integer;
  Report: TRaveReport;
begin { GetReportList }
  ReportList.Clear;
  with ProjMan.ReportList do begin
    for I1 := 0 to Count - 1 do begin
      Report := TRaveReport(Items[I1]);
      if FullNames then begin
        ReportList.AddObject(Report.FullName,Report);
      end else begin
        ReportList.AddObject(Report.Name,Report);
      end; { else }
    end; { for }
  end; { with }
end;  { GetReportList }

procedure TRvProject.GetReportCategoryList(ReportList: TStrings; Categories: string; FullNames: boolean);
var
  I1: integer;
  ValidCats: string;
  Report: TRaveReport;
begin { GetReportCategoryList }
  ValidCats := AnsiUpperCase(';' + Categories + ';');
  ReportList.Clear;
  with ProjMan.ReportList do begin
    for I1 := 0 to Count - 1 do begin
      Report := TRaveReport(Items[I1]);
      if AnsiPos(AnsiUpperCase(Report.Category),ValidCats) > 0 then begin
        if FullNames then begin
          ReportList.AddObject(Report.FullName,Report);
        end else begin
          ReportList.AddObject(Report.Name,Report);
        end; { else }
      end; { if }
    end; { for }
  end; { with }
end;  { GetReportCategoryList }

function TRvProject.SelectReport(ReportName: string; FullName: boolean): boolean;
var
  LReport: TRaveReport;
begin
  Open;
  LReport := ProjMan.FindReport(ReportName, FullName);
  result := LReport <> nil;
  if result then begin
    ProjMan.ActivateReport(LReport);
  end;
end;

procedure TRvProject.ReportDescToMemo(Memo: TCustomMemo);
begin { ReportDescToMemo }
  If Assigned(ProjMan) and Assigned(ProjMan.ActiveReport) then begin
    Memo.Text := ProjMan.ActiveReport.Description;
  end; { if }
end;  { ReportDescToMemo }

function TRvProject.GetReportDesc: string;
begin { GetReportDesc }
  If Assigned(ProjMan) and Assigned(ProjMan.ActiveReport) then begin
    Result := ProjMan.ActiveReport.Description;
  end else begin
    Result := '';
  end; { else }
end;  { GetReportDesc }

function TRvProject.GetReportName: string;
begin { GetReportName }
  If Assigned(ProjMan) and Assigned(ProjMan.ActiveReport) then begin
    Result := ProjMan.ActiveReport.Name;
  end else begin
    Result := '';
  end; { else }
end;  { GetReportName }

function TRvProject.GetReportFullName: string;
begin { GetReportFullName }
  If Assigned(ProjMan) and Assigned(ProjMan.ActiveReport) then begin
    Result := ProjMan.ActiveReport.FullName;
  end else begin
    Result := '';
  end; { else }
end;  { GetReportFullName }

procedure TRvProject.SetParam(Param, Value: string);
begin { SetParam }
  ProjMan.SetParam(Param,Value);
end;  { SetParam }

function TRvProject.GetParam(Param: string): string;
begin { GetParam }
  Result := ProjMan.GetParam(Param);
end;  { GetParam }

procedure TRvProject.ClearParams;

begin { ClearParams }
  ProjMan.ClearParams;
end;  { ClearParams }

class procedure TRvProject.RegisterRaveUnits;
begin
  CallRegisters('*');
end;

end.