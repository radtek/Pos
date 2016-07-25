{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRvDirectDataView;

interface

{.$DEFINE DBG} { Used for data connection thread debugging }

uses
  Windows,
  Qt,
  Classes, QRpCon, QRvData, QRvDataField;

type
  TRaveDataSystem = class;
  TRaveDataResult = (drContinue,drAbort,drPause);
  TTimeoutEvent = procedure(    DataSystem: TRaveDataSystem;
                                Counter: integer;
                                Timeout: integer;
                                EventType: integer;
                                Connection: string;
                                First: boolean;
                            var DataResult: TRaveDataResult) of object;
  TRaveDataAction = (daOpen,daClose);
  TDataActionEvent = procedure(DataSystem: TRaveDataSystem;
                               DataAction: TRaveDataAction) of object;

  TRaveDataConnection = class
  protected
    FTag: integer;
    FName: string;
    FRuntime: boolean;
    FAppHandle: QApplicationH;
    FConnection: TRvCustomConnection;
    FDataEvent: THandle;
    FVersion: longint;
    FDataView: TRaveBaseDataView;
  public
    procedure Assign(Value: TRaveDataConnection);

    property AppHandle: QApplicationH read FAppHandle write FAppHandle;
    property Connection: TRvCustomConnection read FConnection write FConnection;
    property DataEvent: THandle read FDataEvent write FDataEvent;
    property DataView: TRaveBaseDataView read FDataView write FDataView;
    property Name: string read FName write FName;
    property Runtime: boolean read FRuntime write FRuntime;
    property Tag: integer read FTag write FTag;
    property Version: longint read FVersion write FVersion;
  end; { TRaveDataConnnection }

  PDataSystemEventData = ^TDataSystemEventData;
  TDataSystemEventData = record
    DataCon: TRaveDataConnection;
    EventType: integer;
    Handled: boolean;
  end; { TDataSystemEventData }

  TRaveBufferInfo = record
    Size: longint;
    FileMap: byte;
    FileBuf: PChar;
  end; { TRaveBufferInfo }

  TRaveDataSystem = class
  protected
    ControlCount: integer;
    NormFileMap: THandle;
    NormFileBuf: PChar;
    AltFileMap: THandle;
    AltFileBuf: PChar;
    FileBuf: PChar;
    FilePtr: PChar;
    CurrentFileMap: byte; { 0=FileBuf; 1=AltFileBuf[1]; 2=AltFileBuf[2] }
    CurrentDataCon: TRaveDataConnection;
    ControllerMutex: THandle;
    CompletedEvent: THandle;
    ErrorEvent: THandle;
    FOnSmallTimeout: TTimeoutEvent;
    FOnLargeTimeout: TTimeoutEvent;
    FOnDataAction: TDataActionEvent;
    FRTConnectList: TStringList;
    FDTConnectList: TStringList;
    FAutoUpdate: boolean;

  {$IFDEF DBG}
    DebugMap: THandle;
    DebugBuf: PChar;
    procedure TDbg(Value: string);
    procedure DumpDebug(FileName: string;
                        DebugBuf: PChar);
  {$ENDIF}
  public
    constructor Create;
    destructor Destroy; override;
  {$IFDEF DBG}
    procedure Dbg(Value: string);
  {$ENDIF}
    function GainControl: boolean;
    procedure ReleaseControl;
    function IsUnique(Name: string): boolean;
    procedure UpdateConnections;

    function ReadStr: string;
    function ReadInt: integer;
    function ReadBool: boolean;
    function ReadFloat: extended;
    function ReadCurr: currency;
    function ReadDateTime: TDateTime;
    procedure ReadBuf(var Buffer;
                          Len: integer);
    function ReadPtr(Len: integer): pointer;
    procedure WriteStr(Value: string);
    procedure WriteInt(Value: integer);
    procedure WriteInt64(Value: int64);
    procedure WriteBool(Value: boolean);
    procedure WriteFloat(Value: extended);
    procedure WriteCurr(Value: currency);
    procedure WriteDateTime(Value: TDateTime);
    procedure WriteBuf(var Buffer;
                           Len: integer);
    procedure ClearBuffer;
    procedure GetBufferInfo(var BufferInfo: TRaveBufferInfo);

    function OpenDataEvent(AName: string;
                           DataCon: TRaveDataConnection): boolean;
    procedure CloseDataEvent(DataCon: TRaveDataConnection);
    function CallEvent(EventType: integer;
                       DataCon: TRaveDataConnection): boolean;
    procedure PrepareEvent(EventType: integer);
    procedure OpenFileMap(DataCon: TRaveDataConnection);
    procedure CloseFileMap;

    property AutoUpdate: boolean read FAutoUpdate write FAutoUpdate;
    property RTConnectList: TStringList read FRTConnectList;
    property DTConnectList: TStringList read FDTConnectList;
    property OnSmallTimeout: TTimeoutEvent read FOnSmallTimeout write FOnSmallTimeout;
    property OnLargeTimeout: TTimeoutEvent read FOnLargeTimeout write FOnLargeTimeout;
    property OnDataAction: TDataActionEvent read FOnDataAction write FOnDataAction;
  end; { TRaveDataSystem }

  TRaveDataView = class(TRaveBaseDataView)
  protected
    FConnectionName: string;
    FDataCon: TRaveDataConnection;
    //
    procedure AssignTo(Dest: TPersistent); override;
    procedure SetDataCon(Value: TRaveDataConnection);
    procedure GetRow(EventType: integer); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Open; override;
    procedure Close; override;
    procedure AfterReport; override;
    procedure SetFilter(FilterList: TStringList); override;
    procedure SetRemoteSort(SortList: TStringList); override;
    procedure CreateFields(AFieldList: TList); override;
    //
    property DataCon: TRaveDataConnection read FDataCon write SetDataCon;
  published
    property ConnectionName: string read FConnectionName write FConnectionName;
  end; { TRaveDataView }

var
  RaveDataSystem: TRaveDataSystem = nil;

  function CreateDataCon(RPConnection: TRvCustomConnection): TRaveDataConnection;
  function CreateDataView(DataCon: TRaveDataConnection): TRaveDataView;
  procedure RaveRegister;

implementation

uses
  QControls, QDialogs, QForms,
  QRpDefine, QRvClass, QRvDefine, QRvProj,
  QRvUtil, SysUtils, TypInfo;

function CreateDataCon(RPConnection: TRvCustomConnection): TRaveDataConnection;
begin { CreateDataCon }
  Result := TRaveDataConnection.Create;
  with Result do begin
    Connection := RPConnection;
    Name := RPConnection.Name;
    AppHandle := Application.Handle;
    Runtime := true;
    Version := DataConVersion;
  end; { with }
end;  { CreateDataCon }

function CreateDataView(DataCon: TRaveDataConnection): TRaveDataView;
begin { CreateDataView }
  Result := ProjectManager.NewDataObject(TRaveDataView) as TRaveDataView;
  Result.ConnectionName := DataCon.Name;
  Result.DataCon := DataCon;
  CreateFields(Result,nil,nil,true);
end;  { CreateDataView }

procedure RaveRegister;
begin { RaveRegister }
  RegisterRaveComponents('',[TRaveDataView]);
  RegisterRaveModuleClasses({Trans-}'RvData',[TRaveDataView]);
  RegisterRaveDataObject(Trans('Direct Data View'),TRaveDataView);
end;  { RaveRegister }

{ class TRaveDataConnection }

procedure TRaveDataConnection.Assign(Value: TRaveDataConnection);
begin { Assign }
  Name := Value.Name;
  Runtime := Value.Runtime;
  AppHandle := Value.AppHandle;
  Connection := Value.Connection;
  DataEvent := Value.DataEvent;
  Version := Value.Version;
end;  { Assign }

{ class TRaveDataSystem }

constructor TRaveDataSystem.Create;
begin { Create }
  inherited Create;
  NormFileMap := InitFileMap(FileMapName,FileMapSize);
  NormFileBuf := InitFileBuf(NormFileMap);
  FileBuf := NormFileBuf;
  ControllerMutex := InitMutex(ControllerMutexName);
  CompletedEvent := InitEvent(CompletedEventName);
  ErrorEvent := InitEvent(ErrorEventName);
{$IFDEF DBG}
  DebugMap := InitDebugMap;
  DebugBuf := InitFileBuf(DebugMap);
  Dbg({Trans-}'Session Active');
{$ENDIF}
end;  { Create }

destructor TRaveDataSystem.Destroy;
begin { Destroy }
{$IFDEF DBG}
  DumpDebug({Trans-}'C:\DEBUGCON.TXT',DebugBuf);
  UnmapViewOfFile(DebugBuf);
  CloseHandle(DebugMap);
{$ENDIF}

  if Assigned(FRTConnectList) then begin
    while FRTConnectList.Count > 0 do begin
      FRTConnectList.Objects[0].Free;
      FRTConnectList.Delete(0);
    end; { while }
    FreeAndNil(FRTConnectList);
  end; { if }
  if Assigned(FDTConnectList) then begin
    while FDTConnectList.Count > 0 do begin
      FDTConnectList.Objects[0].Free;
      FDTConnectList.Delete(0);
    end; { while }
    FreeAndNil(FDTConnectList);
  end; { if }
  if CurrentFileMap > 0 then begin
    QRpDefine.CloseFileMap(AltFileMap,AltFileBuf);
  end; { if }
  QRpDefine.CloseFileMap(NormFileMap,NormFileBuf);
  CloseMutex(ControllerMutex);
  CloseEvent(CompletedEvent);
  CloseEvent(ErrorEvent);
  inherited Destroy;
  RaveDataSystem := nil;
end;  { Destroy }

procedure TRaveDataSystem.OpenFileMap(DataCon: TRaveDataConnection);
begin { OpenFileMap }
  FilePtr := FileBuf;
  ReadInt;
  CurrentFileMap := ReadInt;
  CurrentDataCon := DataCon;
  if CurrentFileMap > 0 then begin
    AltFileMap := InitFileMap(AltFileMapName + IntToStr(CurrentFileMap),0);
    AltFileBuf := InitFileBuf(AltFileMap);
    FilePtr := AltFileBuf + (FilePtr - FileBuf);
    FileBuf := AltFileBuf;
  end; { if }
end;  { OpenFileMap }

procedure TRaveDataSystem.CloseFileMap;
begin { CloseFileMap }
  if CurrentFileMap > 0 then begin
    QRpDefine.CloseFileMap(AltFileMap,AltFileBuf);
    FileBuf := NormFileBuf;
    FilePtr := FileBuf;
    CurrentFileMap := 0;
    CurrentDataCon := nil;
  end; { if }
end;  { CloseFileMap }

{$IFDEF DBG}
procedure TRaveDataSystem.Dbg(Value: string);
begin { Dbg }
  WriteDebug(Value,DebugBuf);
end;  { Dbg }

procedure TRaveDataSystem.TDbg(Value: string);
begin { TDbg }
  WriteDebug({Trans-}'  SysThread: ' + Value,DebugBuf);
end;  { TDbg }

procedure TRaveDataSystem.DumpDebug(FileName: string;
                                    DebugBuf: PChar);
var
  OutFile: text;
  DebugPtr: PChar;
  S1: string;
begin { DumpDebug }
{ Write DebugBuf out to file }
  Dbg({Trans-}'End of session');
  AssignFile(OutFile,FileName);
  Rewrite(OutFile);
  DebugPtr := DebugBuf + 4;
  repeat
    S1 := DebugPtr;
    Writeln(OutFile,S1);
    DebugPtr := StrEnd(DebugPtr) + 1;
  until S1 = {Trans-}'End of session';
  CloseFile(OutFile);
end;  { DumpDebug }
{$ENDIF}

function TRaveDataSystem.GainControl: boolean;
var
  WaitResult: longword;
begin { GainControl }
  if ControlCount > 0 then begin
    Inc(ControlCount);
    Result := true;
    Exit;
  end; { if }
{ Try to gain control of Rave Data Communication }
  repeat
    {$IFDEF DBG}Dbg({Trans-}'Attempt to Gain Control');{$ENDIF}
    WaitResult := WaitForSingleObject(ControllerMutex,5000);
    if WaitResult <> WAIT_OBJECT_0 then begin
      if MessageDlg(Trans('Unable to gain control of RAVE Data Communication System.'),
       mtError,[mbRetry,mbCancel],0) <> mrRetry then begin
        Result := false;
        Exit;
      end; { if }
    end; { if }
  until WaitResult = WAIT_OBJECT_0;
  {$IFDEF DBG}Dbg({Trans-}'Control Gained');{$ENDIF}
  Result := true;
  Inc(ControlCount);

{!!!  If InDesigner then begin
    DataEventHandler.DataSystemOpen(self);
  end; { if }
end;  { GainControl }

procedure TRaveDataSystem.ReleaseControl;
begin { ReleaseControl }
  Dec(ControlCount);
  if ControlCount < 0 then begin
    RaiseError(Trans('Control count underflow on ReleaseControl'));
    ControlCount := 0;
  end; { if }
  if ControlCount = 0 then begin
    {$IFDEF DBG}Dbg({Trans-}'Control Released');{$ENDIF}
    ReleaseMutex(ControllerMutex);
{!!!If InDesigner then begin
      DataEventHandler.DataSystemClose(self);
    end; { if }
  end; { if }
end;  { ReleaseControl }

function TRaveDataSystem.IsUnique(Name: string): boolean;
var
  I1: integer;
begin { IsUnique }
  if not Assigned(RTConnectList) or not Assigned(DTConnectList) then begin
    Result := true;
    Exit;
  end; { if }
  I1 := RTConnectList.IndexOf(Name);
  if I1 >= 0 then begin
    Result := ((I1 + 1) = RTConnectList.Count) or
      (AnsiCompareText(RTConnectList[I1],RTConnectList[I1 + 1]) <> 0);
  end else begin
    I1 := DTConnectList.IndexOf(Name);
    Result := (I1 < 0) or ((I1 + 1) = DTConnectList.Count) or
      (AnsiCompareText(DTConnectList[I1],DTConnectList[I1 + 1]) <> 0);
  end; { else }
end;  { IsUnique }

procedure TRaveDataSystem.UpdateConnections;
var
  WaitResult: longword;
  ConnectEvent: THandle;
  DisconnectEvent: THandle;
  ConnectName: string;
  Finished: boolean;
  RaveDataConnection: TRaveDataConnection;
  EventList: array[1..2] of THandle;
  RTVisible: TRuntimeVisibility;
begin { UpdateConnections }
  {$IFDEF DBG}Dbg({Trans-}'  Connect: Begin');{$ENDIF}

{ Free current connections }
  if Assigned(FRTConnectList) then begin
    ClearStringList(FRTConnectList);
  end else begin
    FRTConnectList := TStringList.Create;
    FRTConnectList.Sorted := true;
    FRTConnectList.Duplicates := dupAccept;
  end; { else }
  if Assigned(FDTConnectList) then begin
    ClearStringList(FDTConnectList);
  end else begin
    FDTConnectList := TStringList.Create;
    FDTConnectList.Sorted := true;
    FDTConnectList.Duplicates := dupAccept;
  end; { else }

  ConnectEvent := InitEvent(ConnectEventName);
  if GainControl then try
  { Find all inactive connections }
    Finished := false;
    repeat
    { Signal Connect Request }
      {$IFDEF DBG}Dbg({Trans-}'  Connect: Signal Connect Request');{$ENDIF}
      SetEvent(ConnectEvent);
    { Wait for Completed Event }
      {$IFDEF DBG}Dbg({Trans-}'  Connect: Wait for Completed');{$ENDIF}
      EventList[1] := CompletedEvent;
      EventList[2] := ErrorEvent;
      WaitResult := WaitForMultipleObjects(2,@EventList,false,250);
      {$IFDEF DBG}Dbg({Trans-}'  Connect: Received Completed (' + IntToStr(WaitResult) + ')');{$ENDIF}
      if WaitResult = WAIT_OBJECT_0 then begin
        try
          ClearBuffer;
          ConnectName := ReadStr;
          RaveDataConnection := TRaveDataConnection.Create;
          RaveDataConnection.Name := ConnectName;
          RaveDataConnection.Runtime := ReadBool;
          RaveDataConnection.AppHandle := QApplicationH(ReadInt);
          RaveDataConnection.Connection := TRvCustomConnection(ReadInt);
          RaveDataConnection.Version := ReadInt;
          if RaveDataConnection.Version >= 30003 then begin { Read visibility }
            RTVisible := TRuntimeVisibility(ReadInt);
          end else begin
            RTVisible := rtEndUser;
          end; { else }

          if DeveloperRave or (RTVisible = rtEndUser) then begin
            {$IFDEF DBG}Dbg({Trans-}'  Connect: ConnectName = ' + ConnectName);{$ENDIF}

            if RaveDataConnection.Runtime then begin
              RTConnectList.AddObject(ConnectName,RaveDataConnection);
            end else begin
              DTConnectList.AddObject(ConnectName,RaveDataConnection);
            end; { else }
          end; { if }
        except
        end; { tryx }
      end else if WaitResult = (WAIT_OBJECT_0 + 1) then begin
      //!!! write out error
      end else begin
        {$IFDEF DBG}Dbg({Trans-}'  Connect: Release Connect Request');{$ENDIF}
        ResetEvent(ConnectEvent);
        Finished := true;
        {$IFDEF DBG}Dbg({Trans-}'  Connect: No more connections');{$ENDIF}
      end; { if }
    until Finished;
  finally
    DisconnectEvent := InitManualEvent(DisconnectEventName);
    try
      {$IFDEF DBG}Dbg({Trans-}'  Connect: Signal Global Disconnect');{$ENDIF}
      PulseEvent(DisconnectEvent);
      Sleep(0);
    finally
      CloseEvent(DisconnectEvent);
    end; { tryf }
    CloseEvent(ConnectEvent);
    ReleaseControl;
  end; { if }

  {$IFDEF DBG}Dbg({Trans-}'  Connect: End');{$ENDIF}
end;  { UpdateConnections }

function TRaveDataSystem.ReadStr: string;
var
  I1: integer;
begin { ReadStr }
  I1 := ReadInt;
  SetLength(Result,I1);
  if I1 > 0 then begin
    Move(FilePtr^,Result[1],I1);
    Inc(FilePtr,I1);
  end; { if }
end;  { ReadStr }

function TRaveDataSystem.ReadInt: integer;
begin { ReadInt }
  Result := integer(pointer(FilePtr)^);
  Inc(FilePtr,SizeOf(integer));
end;  { ReadInt }

function TRaveDataSystem.ReadBool: boolean;
begin { ReadBool }
  Result := boolean(pointer(FilePtr)^);
  Inc(FilePtr,SizeOf(boolean));
end;  { ReadBool }

function TRaveDataSystem.ReadFloat: extended;
begin { ReadFloat }
  Result := extended(pointer(FilePtr)^);
  Inc(FilePtr,SizeOf(extended));
end;  { ReadFloat }

function TRaveDataSystem.ReadCurr: currency;
begin { ReadCurr }
  Result := currency(pointer(FilePtr)^);
  Inc(FilePtr,SizeOf(currency));
end;  { ReadCurr }

function TRaveDataSystem.ReadDateTime: TDateTime;
begin { ReadDateTime }
  Result := TDateTime(pointer(FilePtr)^);
  Inc(FilePtr,SizeOf(TDateTime));
end;  { ReadDateTime }

procedure TRaveDataSystem.ReadBuf(var Buffer;
                                      Len: integer);
begin { ReadBuf }
  Move(FilePtr^,Buffer,Len);
  Inc(FilePtr,Len);
end;  { ReadBuf }

function TRaveDataSystem.ReadPtr(Len: integer): pointer;
begin { ReadPtr }
  Result := FilePtr;
  Inc(FilePtr,Len);
end;  { ReadPtr }

procedure TRaveDataSystem.WriteStr(Value: string);
var
  I1: integer;
begin { WriteStr }
  I1 := Length(Value);
  WriteInt(I1);
  if I1 > 0 then begin
    Move(Value[1],FilePtr^,I1);
    Inc(FilePtr,I1);
  end; { if }
end;  { WriteStr }

procedure TRaveDataSystem.WriteInt(Value: integer);
begin { WriteInt }
  integer(pointer(FilePtr)^) := Value;
  Inc(FilePtr,SizeOf(integer));
end;  { WriteInt }

procedure TRaveDataSystem.WriteInt64(Value: int64);
begin { WriteInt64 }
  int64(pointer(FilePtr)^) := Value;
  Inc(FilePtr,SizeOf(int64));
end;  { WriteInt64 }

procedure TRaveDataSystem.WriteBool(Value: boolean);
begin { WriteBool }
  boolean(pointer(FilePtr)^) := Value;
  Inc(FilePtr,SizeOf(boolean));
end;  { WriteBool }

procedure TRaveDataSystem.WriteFloat(Value: extended);
begin { WriteFloat }
  extended(pointer(FilePtr)^) := Value;
  Inc(FilePtr,SizeOf(extended));
end;  { WriteFloat }

procedure TRaveDataSystem.WriteCurr(Value: currency);
begin { WriteCurr }
  currency(pointer(FilePtr)^) := Value;
  Inc(FilePtr,SizeOf(currency));
end;  { WriteCurr }

procedure TRaveDataSystem.WriteDateTime(Value: TDateTime);
begin { WriteDateTime }
  TDateTime(pointer(FilePtr)^) := Value;
  Inc(FilePtr,SizeOf(TDateTime));
end;  { WriteFloat }

procedure TRaveDataSystem.WriteBuf(var Buffer;
                                       Len: integer);
begin { WriteBuf }
  Move(Buffer,FilePtr^,Len);
  Inc(FilePtr,Len);
end;  { WriteBuf }

procedure TRaveDataSystem.ClearBuffer;
begin { ClearBuffer }
  CloseFileMap; { Close alternate filemap if any }
  FilePtr := FileBuf + 2 * SizeOf(longint); { save room for bufsize & mapidx }
end;  { ClearBuffer }

procedure TRaveDataSystem.GetBufferInfo(var BufferInfo: TRaveBufferInfo);
var
  SavePtr: PChar;
begin { GetBufferInfo }
{ Code used by RaveDataRecorder for access to data buffer }
  SavePtr := FilePtr;
  FilePtr := FileBuf;
  BufferInfo.Size := ReadInt;
  BufferInfo.FileMap := ReadInt;
  BufferInfo.FileBuf := FileBuf;
  FilePtr := SavePtr;
end;  { GetBufferInfo }

function TRaveDataSystem.OpenDataEvent(AName: string;
                                       DataCon: TRaveDataConnection): boolean;
var
  RTDataEvent: THandle;
  DTDataEvent: THandle;
  RTAckEvent: THandle;
  DTAckEvent: THandle;
  WaitResult: longword;
  EventList: array[1..3] of THandle;
  I1: integer;
  EventType: integer;
  DidSmall: boolean;
  DidLarge: boolean;
  DataResult: TRaveDataResult;

  procedure ConnectRemote(DataEvent: THandle);
  begin { ConnectRemote }
    ClearBuffer;
    DataCon.DataEvent := DataEvent;
    DataCon.Name := ReadStr;
    DataCon.Runtime := ReadBool;
    DataCon.AppHandle := QApplicationH(ReadInt);
    DataCon.Version := ReadInt;
    DataCon.Connection := nil;
    Result := true;
{!!!    If InDesigner then begin
      DataEventHandler.DataConConnect(DataCon);
    end; { if }
  end;  { ConnectRemote }

  procedure ConnectLocal;
  begin { ConnectLocal }
    if Assigned(FOnDataAction) then begin
      FOnDataAction(self,daOpen);
    end; { if }
    DataCon.DataEvent := 0;
    DataCon.Name := AName;
    DataCon.Runtime := true;
    DataCon.AppHandle := Application.Handle;
    DataCon.Version := DataConVersion;
    ClearBuffer;
    Result := true;
{!!!    If InDesigner then begin
      DataEventHandler.DataConConnect(DataCon);
    end; { if }
  end;  { ConnectLocal }

begin { OpenDataEvent }
  Result := false;
  EventType := DATAACKNOWLEDGE;
  {$IFDEF DBG}Dbg({Trans-}'  OpenDataEvent: ' + AName);{$ENDIF}

{ Process peDataConOpen }
  DataCon.Connection := nil;
{!!!  If InDesigner then begin
    DataEventHandler.DataConOpen(DataCon);
  end; { if }
  if Assigned(DataCon.Connection) then begin { Open override }
    ConnectLocal;
    Exit;
  end; { if }

{ Look in local DataConnectManager }
  DataCon.Connection := DataConnectManager.FindConnection(AName);
  if Assigned(DataCon.Connection) then begin
    ConnectLocal;
    Exit;
  end; { if }

  if AutoUpdate then begin
  { Update connections for IsUnique if data connection was not found locally }
    UpdateConnections;
    AutoUpdate := false;
  end; { if }

{ Try runtime connection }
  RTDataEvent := InitEvent(DataRTEventName + AName);
  RTAckEvent := InitEvent(AcknowledgeRTEventName + AName);
  ClearBuffer;
  WriteInt(EventType);
  SetEvent(RTDataEvent);
  Sleep(0);
  Application.ProcessMessages;
  EventList[1] := RTAckEvent;
  EventList[2] := ErrorEvent;
  WaitResult := WaitForMultipleObjects(2,@EventList,false,100);
  if WaitResult = WAIT_OBJECT_0 then begin
    {$IFDEF DBG}Dbg({Trans-}'  Connected to RTEvent(1): ' + AName);{$ENDIF}
    if Assigned(FOnDataAction) then begin
      FOnDataAction(self,daOpen);
    end; { if }
    ConnectRemote(RTDataEvent);
    CloseEvent(RTAckEvent);
    ClearBuffer;
    Exit;
  end else if WaitResult = (WAIT_OBJECT_0 + 1) then begin
  //!!! handle error event
  end; { else }
{ Try both connections }
  DTDataEvent := InitEvent(DataDTEventName + AName);
  DTAckEvent := InitEvent(AcknowledgeDTEventName + AName);
  SetEvent(DTDataEvent);
  Sleep(0);
  DidSmall := false;
  DidLarge := false;
  for I1 := 1 to EventDelays[EventType] do begin
    Application.ProcessMessages;
    EventList[1] := RTAckEvent;
    EventList[2] := DTAckEvent;
    EventList[3] := ErrorEvent;
    WaitResult := WaitForMultipleObjects(3,@EventList,false,100);
    if WaitResult = WAIT_OBJECT_0 then begin { RT Connection }
      {$IFDEF DBG}Dbg({Trans-}'  Connected to RTEvent(2): ' + AName);{$ENDIF}
      if Assigned(FOnDataAction) then begin
        FOnDataAction(self,daOpen);
      end; { if }
      ConnectRemote(RTDataEvent);
      CloseEvent(DTDataEvent);
      Break;
    end else if WaitResult = (WAIT_OBJECT_0 + 1) then begin { DT Connection }
      {$IFDEF DBG}Dbg({Trans-}'  Connected to DTEvent(2): ' + AName);{$ENDIF}
      if Assigned(FOnDataAction) then begin
        FOnDataAction(self,daOpen);
      end; { if }
      ConnectRemote(DTDataEvent);
      CloseEvent(RTDataEvent);
      Break;
    end else if WaitResult = (WAIT_OBJECT_0 + 2) then begin { Error }
    //!!! handle error
    end; { else }
    DataResult := drContinue;
    if Assigned(FOnSmallTimeout) then begin
      repeat
        DataResult := drContinue;
        FOnSmallTimeout(self,I1,EventDelays[EventType],EventType,AName,I1 = 1,DataResult);
        if DataResult = drPause then begin
          Application.ProcessMessages;
        end; { if }
      until DataResult in [drContinue,drAbort];
      DidSmall := true;
    end; { if }
    if (I1 >= LargeTimeouts[EventType]) and Assigned(FOnLargeTimeout) then begin
      repeat
        DataResult := drContinue;
        FOnLargeTimeout(self,I1,EventDelays[EventType],EventType,AName,
          I1 = LargeTimeouts[EventType],DataResult);
        if DataResult = drPause then begin
          Application.ProcessMessages;
        end; { if }
      until DataResult in [drContinue,drAbort];
      DidLarge := true;
    end; { if }
    if DataResult = drAbort then begin
      Break;
    end; { if }
  end; { for }
  if not Result then begin
    {$IFDEF DBG}Dbg({Trans-}'  Failed to open data event: ' + AName);{$ENDIF}
    CloseEvent(RTDataEvent);
    CloseEvent(DTDataEvent);
  end; { if }
  CloseEvent(RTAckEvent);
  CloseEvent(DTAckEvent);
  ClearBuffer;
  if Result then begin { Success }
    I1 := 0;
  end else begin { Failure }
    I1 := -1;
  end; { else }
  if DidSmall and Assigned(FOnSmallTimeout) then begin
    DataResult := drContinue;
    FOnSmallTimeout(self,I1,0,EventType,AName,false,DataResult);
  end; { if }
  if DidLarge and Assigned(FOnLargeTimeout) then begin
    DataResult := drContinue;
    FOnLargeTimeout(self,I1,0,EventType,AName,false,DataResult);
  end; { if }
end;  { OpenDataEvent }

procedure TRaveDataSystem.CloseDataEvent(DataCon: TRaveDataConnection);
begin { CloseDataEvent }
  CloseFileMap; { Close previous filemap if any }

  if Assigned(FOnDataAction) then begin
    FOnDataAction(self,daClose);
  end; { if }

{ Process deDataConClose }
{!!!  If InDesigner then begin
    DataEventHandler.DataConClose(DataCon);
  end; { if }

{ Free DataCon.DataEvent }
  if DataCon.DataEvent <> 0 then begin
    CloseEvent(DataCon.DataEvent);
    DataCon.DataEvent := 0;
  end; { if }
end;  { CloseDataEvent }

function TRaveDataSystem.CallEvent(EventType: integer;
                                   DataCon: TRaveDataConnection): boolean;
var
  WaitResult: longword;
  I1: integer;
  DidSmall: boolean;
  DidLarge: boolean;
  DataResult: TRaveDataResult;
  EventData: TDataSystemEventData;
  SavePtr: PChar;
  EventList: array[1..2] of THandle;
begin { CallEvent }
  CloseFileMap; { Close alternate filemap if any }

{ Initialize buffer header for write to data connection }
  SavePtr := FilePtr;
  FilePtr := FileBuf;
  WriteInt(SavePtr - FileBuf);
  WriteInt(0);
  FilePtr := SavePtr;

  ClearBuffer;
  if EventType = EVENTPREPARED then begin
    EventType := ReadInt; { Get EventType }
  end else begin
    WriteInt(EventType);
  end; { else }
  {$IFDEF DBG}Dbg({Trans-}'  CallEvent: Call Event #' + IntToStr(EventType));{$ENDIF}
  {$IFDEF DBG}Dbg({Trans-}'  CallEvent: Signal DataRequest');{$ENDIF}

{ Process peDataSystemCallEvent }
  EventData.DataCon := DataCon;
  EventData.EventType := EventType;
  EventData.Handled := false;
{!!!  If InDesigner then begin
    DataEventHandler.DataSystemCallEvent(self,EventData);
  end; { if }
  if EventData.Handled then begin
    Result := true;
  end else if Assigned(DataCon.Connection) then begin { Direct access }
    Result := true;
    DataCon.Connection.ClearBuffer;
    EventType := DataCon.Connection.ReadInt;
    case EventType of
      DATAFIRST: DataCon.Connection.ExecFirst;
      DATANEXT: DataCon.Connection.ExecNext;
      DATAEOF: DataCon.Connection.ExecEOF;
      DATAGETCOLS: DataCon.Connection.ExecGetCols;
      DATAGETROW: DataCon.Connection.ExecGetRow;
      DATASETFILTER: DataCon.Connection.ExecSetFilter;
      DATAGETSORTS: DataCon.Connection.ExecGetSorts;
      DATASETSORT: DataCon.Connection.ExecSetSort;
      DATAOPEN: DataCon.Connection.ExecOpen;
      DATARESTORE: DataCon.Connection.ExecRestore;
      else Result := false;
    end; { case }
    if Result then begin
      OpenFileMap(DataCon);
    end; { if }
  end else begin { Go through event }
    SetEvent(DataCon.DataEvent);
    Sleep(0);
    WaitResult := WAIT_OBJECT_0 + 1;
    DidSmall := false;
    DidLarge := false;
    for I1 := 1 to EventDelays[EventType] do begin { 60 seconds }
      Application.ProcessMessages;
      EventList[1] := CompletedEvent;
      EventList[2] := ErrorEvent;
      WaitResult := WaitForMultipleObjects(2,@EventList,false,100);
      if WaitResult = WAIT_OBJECT_0 then begin
        Break;
      end else if WaitResult = (WAIT_OBJECT_0 + 1) then begin { Error }
//!!!
      end; { else }
      DataResult := drContinue;
      if Assigned(FOnSmallTimeout) then begin
        repeat
          DataResult := drContinue;
          FOnSmallTimeout(self,I1,EventDelays[EventType],EventType,
            DataCon.Name,I1 = 1,DataResult);
          if DataResult = drPause then begin
            Application.ProcessMessages;
          end; { if }
        until DataResult in [drContinue,drAbort];
        DidSmall := true;
      end; { if }
      if (I1 >= LargeTimeouts[EventType]) and Assigned(FOnLargeTimeout) then begin
        repeat
          DataResult := drContinue;
          FOnLargeTimeout(self,I1,EventDelays[EventType],EventType,
            DataCon.Name,I1 = LargeTimeouts[EventType],DataResult);
          if DataResult = drPause then begin
            Application.ProcessMessages;
          end; { if }
        until DataResult in [drContinue,drAbort];
        DidLarge := true;
      end; { if }
      if DataResult = drAbort then begin
        WaitResult := WAIT_OBJECT_0 + 1;
        Break;
      end; { if }
    end; { for }
    {$IFDEF DBG}Dbg({Trans-}'  CallEvent: Received Completed (' + IntToStr(WaitResult) + ')');{$ENDIF}
    Result := (WaitResult = WAIT_OBJECT_0);
    if Result then begin { Success }
      OpenFileMap(DataCon);
      I1 := 0;
    end else begin { Failure }
      I1 := -1;
    end; { else }
    if DidSmall and Assigned(FOnSmallTimeout) then begin
      DataResult := drContinue;
      FOnSmallTimeout(self,I1,0,EventType,DataCon.Name,false,DataResult);
    end; { if }
    if DidLarge and Assigned(FOnLargeTimeout) then begin
      DataResult := drContinue;
      FOnLargeTimeout(self,I1,0,EventType,DataCon.Name,false,DataResult);
    end; { if }
  end; { else }
//ClearBuffer;
  FilePtr := FileBuf + 2 * SizeOf(longint); { save room for bufsize & mapidx }

{ Process peDataSystemEventCalled }
  EventData.DataCon := DataCon;
  EventData.EventType := EventType;
  EventData.Handled := false;
{!!!  If InDesigner then begin
    DataEventHandler.DataSystemEventCalled(self,EventData);
  end; { if }
end;  { CallEvent }

procedure TRaveDataSystem.PrepareEvent(EventType: integer);
begin { PrepareEvent }
  {$IFDEF DBG}Dbg({Trans-}'  PrepareEvent: Prepare Event #' + IntToStr(EventType));{$ENDIF}
  ClearBuffer;
  WriteInt(EventType);
end;  { PrepareEvent }

{ class TRaveDataView }

constructor TRaveDataView.Create(AOwner: TComponent);
begin { Create }
  inherited;
  FDataCon := TRaveDataConnection.Create;
  FDataCon.DataView := self;
end; { Create }

destructor TRaveDataView.Destroy;
begin { Destroy }
  FreeAndNil(FDataCon);
  inherited Destroy;
end;  { Destroy }

procedure TRaveDataView.AssignTo(Dest: TPersistent);
var
  P1: TRaveDataView;
  S1: string;
begin { AssignTo }
  if Dest is TRaveDataView then begin
    P1 := TRaveDataView(Dest);
    S1 := Name;
    Name := '';
    P1.Name := S1;
    P1.Tag := Tag;
  end else begin
    inherited AssignTo(Dest);
  end; { else }
end;  { AssignTo }

procedure TRaveDataView.SetDataCon(Value: TRaveDataConnection);
begin { SetDataCon }
  FDataCon.Assign(Value);
end;  { SetDataCon }

procedure TRaveDataView.Open;
var
  I1: integer;
  I2: integer;
  I3: integer;
  S1: string;
  FieldName: string;
  Field: TRaveDataField;
  DataType: TRPDataType;
begin { Open }
  if not DataOpened then begin
    if not RaveDataSystem.IsUnique(ConnectionName) then begin
      MessageDlg(Trans(Format(
        {Trans+}'Error!  Duplicate data connections found with name "%s".  Aborting report.',
        [ConnectionName])),mtError,[mbOK],0);
      AbortReport(self);
    end; { if }

    if DataCon.DataEvent = 0 then begin
      if not RaveDataSystem.OpenDataEvent(ConnectionName,DataCon) then begin
        MessageDlg(Trans(Format({Trans+}
          'Error!  Could not open data connection "%s".  Aborting report.',
          [ConnectionName])),mtError,[mbOK],0);
        AbortReport(self);
      end; { if }
    end; { if }
    DataOpened := RaveDataSystem.CallEvent(DATAOPEN,DataCon);
    FActive := DataOpened;
    if DataOpened then begin
      with RaveDataSystem do begin
        if RaveDataSystem.CallEvent(DATAGETCOLS,DataCon) then begin
          DataRow.Clear;
          FieldNameList.Clear;
          try
            I2 := ReadInt;
            for I1 := 0 to (I2 - 1) do begin { Read in all defined columns }
              DataType := dtString; { Default DataType value }
              S1 := ReadStr;
              while S1 <> '' do begin
                case S1[1] of
                  'N': begin { Name }
                    FieldName := AddJoinChars(ReadStr);
                  end;
                  'T': begin { Type }
                    DataType := TRPDataType(GetEnumValue(TypeInfo(TRPDataType),
                      ReadStr));
                  end;
                  else begin { Ignore other values }
                    ReadStr;
                  end;
                end; { case }
                S1 := ReadStr;
              end; { while }

            { Add a field definition - Find TRaveDataField component }
              Field := nil;
              for I3 := 0 to ChildCount - 1 do begin
                if AnsiCompareText(TRaveDataField(Child[I3]).FieldName,FieldName) = 0 then begin
                  Field := TRaveDataField(Child[I3]);
                  if Field.DataType <> DataType then begin
                    RaveError(Trans(Format(
                    {Trans+}'Field %0:s:%1:s.  Datatype expected: %2:s  Datatype found: %3:s',
                    [Name,Field.FieldName,
                      GetEnumName(TypeInfo(TRPDataType),Ord(Field.DataType)),
                      GetEnumName(TypeInfo(TRPDataType),Ord(DataType))])));
                    Field := nil;
                  end; { if }
                  Break;
                end; { if }
              end; { for }
              if Assigned(Field) then begin
                Field.DataIndex := I1;
              end; { if }
              FieldNameList.AddObject(StripJoinChars(FieldName),Field);
            end; { for }
          except
            FieldNameList.Clear;
          end; { tryx }
        end else begin
          AbortReport(self);
        end; { else }
      end; { with }
    end else begin
      AbortReport(self);
    end; { else }
  end; { if }
end;  { Open }

procedure TRaveDataView.Close;
begin { Close }
  inherited;
  if DataOpened then begin
    DataOpened := not RaveDataSystem.CallEvent(DATARESTORE,DataCon);
    FActive := DataOpened;
    if DataOpened then begin
      AbortReport(self);
    end; { if }
  end; { if }
end;  { Close }

procedure TRaveDataView.AfterReport;
begin { AfterReport }
  if DataOpened then begin
    Close;
    RaveDataSystem.CloseDataEvent(DataCon);
    DataCon.DataEvent := 0;
  end; { if }
  inherited;
end;  { AfterReport }

procedure TRaveDataView.GetRow(EventType: integer);
var
  I1: integer;
  DR: TRaveDataRow;
begin { GetRow }
  with RaveDataSystem do begin
    if DataOpened and CallEvent(EventType,DataCon) then begin
      AtEOF := ReadBool;
      if AtEOF then begin
        if Saving then begin { Mark last item as EOF }
          if EventType = DATAFIRST then begin { Add dummy record }
            FEmpty := true;
            AddValueListItem;
          end; { if }
          ValueListTail.DataRowType := rtLast;
        end; { if }
      end else begin { Read data row in }
        if Saving then begin { Create value list item }
          AddValueListItem;
          if EventType = DATAFIRST then begin
            ValueListTail.DataRowType := rtFirst;
          end; { if }
          DR := ValueListTail;
        end else begin { Use DataView DataRow }
          DR := DataRow;
        end; { else }

      { Get Row Data }
        I1 := ReadInt; { Size of all data }
        DR.Init(I1);
        ReadBuf(DR.DataPtr^,I1);
      end; { else }
    end else begin
      AtEOF := true;
      AbortReport(self);
    end; { else }
  end; { with }
end;  { GetRow }

procedure TRaveDataView.SetFilter(FilterList: TStringList);
var
  I1: integer;
  Field: TRaveDataField;
begin { SetFilter }
  if Assigned(ValueListPtr) then Exit;

  Open;
  with RaveDataSystem do begin
    ClearBuffer;
    WriteInt(DATASETFILTER);
    for I1 := 0 to FilterList.Count - 1 do begin
      WriteStr(FilterList[I1]);
      if (I1 mod 3) = 2 then begin { On a FieldData item }
        Field := TRaveDataField(FilterList.Objects[I1]);
        if Assigned(Field) then begin
          WriteInt(Ord(Field.DataType));
          case Field.DataType of
            dtString: begin end;
            dtInteger: WriteInt(Field.AsInteger);
            dtInt64: WriteInt64(Field.AsInt64);
            dtBoolean: WriteBool(Field.AsBoolean);
            dtFloat: WriteFloat(Field.AsFloat);
            dtCurrency,dtBCD: WriteCurr(Field.AsCurrency);
            dtDate,dtTime,dtDateTime: WriteDateTime(Field.AsDateTime);
          { dtBlob,dtMemo,dtGraphic }
          end; { case }
        end else begin
          WriteInt(Ord(dtString));
        end; { else }
      end; { if }
    end; { for }
    if not CallEvent(EVENTPREPARED,DataCon) then begin
      AbortReport(self);
    end; { if }
  end; { with }
end;  { SetFilter }

procedure TRaveDataView.SetRemoteSort(SortList: TStringList);
var
  I1: integer;
begin { SetRemoteSort }
  if Assigned(ValueListPtr) then Exit;

  Open;
  with RaveDataSystem do begin
    ClearBuffer;
    WriteInt(DATASETSORT);
    for I1 := 0 to SortList.Count - 1 do begin
      WriteStr(SortList[I1]);
    end; { for }
    if not CallEvent(EVENTPREPARED,DataCon) then begin
      AbortReport(self);
    end; { if }
  end; { with }
end;  { SetRemoteSort }

procedure TRaveDataView.CreateFields(AFieldList: TList);
var
  I2: integer;
  I3: integer;
  S1: string;
  DataType: TRPDataType;
  FieldName: string;
  CompName: string;
  Width: integer;
  FullName: string;
  Description: string;
  DoRestore: boolean;
  DataFieldInfo: TRaveDataFieldInfo;
begin { CreateFields }
  DoRestore := false;
  with RaveDataSystem do begin
    if GainControl then try
      if not OpenDataEvent(self.ConnectionName,self.DataCon) then Exit;
      if not CallEvent(DATAOPEN,DataCon) then begin
        AbortReport(self);
        Exit;
      end; { if }
      DoRestore := true;
      if CallEvent(DATAGETCOLS,self.DataCon) then begin
        I3 := ReadInt; // Read in number of columns
      { Read in field names }
        for I2 := 0 to I3 - 1 do begin
          S1 := ReadStr;
          DataType := dtString;
          Width := 0;
          FullName := '';
          Description := '';
          FieldName := '';
          CompName := '';
          while S1 <> '' do begin
            case S1[1] of
              'N': begin { Name }
                FieldName := ReadStr;
                CompName := CreateFieldName(self.Name,FieldName);
              end;
              'T': begin { Type }
                DataType := TRPDataType(GetEnumValue(TypeInfo(TRPDataType),
                  ReadStr));
              end;
              'W': begin { Width }
                Width := StrToInt(ReadStr);
              end;
              'L': begin { Full Name }
                FullName := ReadStr;
              end;
              'D': begin { Description }
                Description := ReadStr;
              end;
            end; { case }
            S1 := ReadStr;
          end; { while }

        // Create field info object and add it to the list
          DataFieldInfo := TRaveDataFieldInfo.Create;
          DataFieldInfo.FieldName := FieldName;
          DataFieldInfo.DataType := DataType;
          DataFieldInfo.Width := Width;
          DataFieldInfo.FullName := FullName;
          DataFieldInfo.Description := Description;
          AFieldList.Add(DataFieldInfo);
        end; { for }
      end else begin
        AbortReport(self);
      end; { else }
    finally
      try
        try
          if DoRestore then begin
            if not CallEvent(DATARESTORE,self.DataCon) then begin
              AbortReport(self);
            end; { if }
          end; { if }
        finally
          CloseDataEvent(self.DataCon);
        end; { tryf }
      finally
        ReleaseControl;
      end; { tryf }
    end; { if }
  end; { with }
end;  { CreateFields }

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
finalization
  if Assigned(RaveDataSystem) then begin
    FreeAndNil(RaveDataSystem);
  end; { if }
end.
