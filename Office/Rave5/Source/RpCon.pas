{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpCon;

{.$DEFINE DBG} { Used for data connection thread debugging }

interface

uses
  Forms,
  Windows,
  SysUtils, Classes, RpDefine;

const
  DataConVersion: longint = 30003; { 3.0003 }

type
  TRvCustomConnection = class;
  TRPCustomConnection = TRvCustomConnection; // Rave 4.0 compatibility

  TRPConnectorEvent = procedure(Connection: TRvCustomConnection) of object;
  TRPEOFEvent = procedure(    Connection: TRvCustomConnection;
                          var Eof: boolean) of object;
  TRPValidateRowEvent = procedure(    Connection: TRvCustomConnection;
                                  var ValidRow: boolean) of object;
  TRuntimeVisibility = (rtNone,rtDeveloper,rtEndUser);

  TDataConnectManager = class
  protected
    FConnectList: TList;
  public
    constructor Create;
    destructor Destroy; override;
    procedure AddConnection(Connection: TRvCustomConnection);
    procedure RemoveConnection(Connection: TRvCustomConnection);
    function FindConnection(ConnectName: string): TRvCustomConnection;
  end; { TDataConnectManager }

  TDataConnectThread = class(TThread)
  protected
    SyncMethod: TThreadMethod;
    SyncRan: boolean;
    FDataCon: TRvCustomConnection;
    procedure SyncExecute;
    procedure DoEvent(AMethod: TThreadMethod);
  public
    procedure Execute; override;
    property DataCon: TRvCustomConnection read FDataCon write FDataCon;
  end; { TDataConnectThread }

  TDataConnectFieldDef = class
  protected
    FDataType: TRPDataType;
    FFieldName: string;
    FIncluded: boolean;
    FFieldIndex: integer;
    StrData: string;
    BinData: pointer;
    DataSize: integer;
  public
    constructor Create(ADataType: TRPDataType;
                       AFieldName: string;
                       AIncluded: boolean);
    destructor Destroy; override;
    procedure Clear;
    procedure SetFilter(var Value);
    function FilterValid(var Value): boolean;

    property DataType: TRPDataType read FDataType;
    property FieldIndex: integer read FFieldIndex write FFieldIndex;
    property FieldName: string read FFieldName;
    property Included: boolean read FIncluded;
  end; { TDataConnectFieldDef }

  TRvCustomConnection = class(TRPComponent)
  protected
  {$IFDEF DBG}
    DebugMap: THandle; { Handle to MMF }
    DebugBuf: PChar; { Pointer to MMF }
  {$ENDIF}
    NormFileMap: THandle; { Handle to normal MMF }
    NormFileBuf: PChar; { Pointer to normal MMF }
    AltFileMap: THandle; { Handle to alternate MMF }
    AltFileBuf: PChar; { Pointer to alternate MMF }
    CurrentMapSize: longint; { Size of current MMF }
    CurrentFileMap: byte; { 0=FileBuf; 1=AltFileBuf[1]; 2=AltFileBuf[2] }
    FileBuf: PChar; { Pointer to current MMF }
    FilePtr: PChar; { Utility pointer for accessing FileBuf }
    RowHeaderPtr: PChar; { Where the row header in FileBuf is stored }
    ErrorResult: boolean; { Is an error result being sent back? }

    Thread: TDataConnectThread;
    Runtime: boolean; { Are we in runtime or design-time? }
    ModeStr: string; { Runtime mode string DT or RT }
    ThreadDone: boolean;
    FieldDefList: TList; { List of TDataConnectFieldDef }
    FieldIdx: integer; { Which field is being written, starts at 0 }
    IncludedFieldIdx: integer; { FieldIdx if non-included fields are removed }
    FDataIndex: integer; { Utility index for custom connections }
    FDataRows: integer; { Utility row count for custom connections }
    FFilterFieldName: string; { Filter field name from GetFilterItem }
    FFilterFieldOp: string; { Filter field operator from GetFilterItem }
    FFilterFieldData: string; { Filter field data from GetFilterItem }

    FLocalFilter: boolean; { Use local filtering? }
    LocalFilterValid: boolean; { Does this record pass local filtering rules? }
    FRTVisible: TRuntimeVisibility; { Visibility of data at runtime }
    FFieldAliasList: TStrings; { List of Field Aliases }

    FOnFirst: TRPConnectorEvent;
    FOnNext: TRPConnectorEvent;
    FOnEOF: TRPEOFEvent;
    FOnGetCols: TRPConnectorEvent;
    FOnGetRow: TRPConnectorEvent;
    FOnSetFilter: TRPConnectorEvent;
    FOnGetSorts: TRPConnectorEvent;
    FOnSetSort: TRPConnectorEvent;
    FOnOpen: TRPConnectorEvent;
    FOnRestore: TRPConnectorEvent;
    FOnValidateRow: TRPValidateRowEvent;

  {$IFDEF DBG}
    procedure Dbg(Value: string);
    procedure TDbg(Value: string);
  {$ENDIF}
    procedure ClearFieldDefs;
    procedure ClearFilterData;
    procedure CheckDataType(ValidDataTypes: TRPDataTypeSet);
    procedure AddingNative;
    procedure AddingFormatted;
    procedure AllocateSpace(AddSize: longint);
    function LocalFilterAbort(var FilterData): boolean;
    procedure SetRTVisible(Value: TRuntimeVisibility);
    procedure ReturnError(Msg: string;
                          ErrorType: TRaveConnectionError);
    procedure ReadVisible(Reader: TReader);
    procedure DefineProperties(Filer: TFiler); override;
    procedure SetFieldAliasList(Value: TStrings);
    function GetFieldAlias(FieldName: string): string;
    procedure NextField;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

  { Utility methods }
    procedure WriteStr(Value: string);
    procedure WriteInt(Value: longint);
    procedure WriteInt64(Value: int64);
    procedure WriteBool(Value: boolean);
    procedure WriteFloat(Value: extended);
    procedure WriteCurr(Value: currency);
    procedure WriteDateTime(Value: TDateTime);
    procedure WriteBuf(var Buffer; Len: integer);
    procedure WriteField(Name: string;
                         DataType: TRPDataType;
                         Width: integer;
                         FullName: string;
                         Description: string);
    procedure WriteNullData;
    procedure WriteStrData(FormatData: string; NativeData: string);
    procedure WriteIntData(FormatData: string; NativeData: integer);
    procedure WriteInt64Data(FormatData: string; NativeData: int64);
    procedure WriteBoolData(FormatData: string; NativeData: boolean);
    procedure WriteFloatData(FormatData: string; NativeData: extended);
    procedure WriteCurrData(FormatData: string; NativeData: currency);
    procedure WriteBCDData(FormatData: string; NativeData: currency);
    procedure WriteDateTimeData(FormatData: string; NativeData: TDateTime);
    procedure WriteBlobData(var Buffer; Len: longint);
    function ReadStr: string;
    function ReadInt: integer;
    function ReadInt64: int64;
    function ReadBool: boolean;
    function ReadFloat: extended;
    function ReadCurr: currency;
    function ReadDateTime: TDateTime;
    procedure ReadBuf(var Buffer; Len: integer);
    function CurrentFieldDef: TDataConnectFieldDef;
    function FindFieldDef(FieldName: string): TDataConnectFieldDef;
    function ReadFilterItem(var FieldName: string;
                            var FieldOp: string;
                            var FieldData: string): boolean;
    function GetFilterItem: boolean;
    procedure ClearBuffer;
    procedure FixupBufferHeader;
    procedure ClearBufferHeader;

    procedure ExecFirst;
    procedure ExecNext;
    procedure ExecEOF;
    function GetEOF: boolean;
    procedure ExecGetCols;
    procedure ExecGetRow;
    procedure ExecSetFilter;
    procedure ExecGetSorts;
    procedure ExecSetSort;
    procedure ExecOpen;
    procedure ExecRestore;
    function ExecValidateRow: boolean;

  { Thread Event methods }
    procedure DoFirst; virtual;
    procedure DoNext; virtual;
    function DoEOF: boolean; virtual;
    procedure DoGetCols; virtual;
    procedure DoGetRow; virtual;
    procedure DoSetFilter; virtual;
    procedure DoGetSorts; virtual;
    procedure DoSetSort; virtual;
    procedure DoOpen; virtual;
    procedure DoRestore; virtual;
    function DoValidateRow: boolean; virtual;

    property DataIndex: integer read FDataIndex write FDataIndex;
    property DataRows: integer read FDataRows write FDataRows;
    property FilterFieldName: string read FFilterFieldName;
    property FilterFieldOp: string read FFilterFieldOp;
    property FilterFieldData: string read FFilterFieldData;
  published
    property FieldAliasList: TStrings read FFieldAliasList write SetFieldAliasList;
    property LocalFilter: boolean read FLocalFilter write FLocalFilter default true;
    property RuntimeVisibility: TRuntimeVisibility read FRTVisible write SetRTVisible
     stored true;
    property OnFirst: TRPConnectorEvent read FOnFirst write FOnFirst;
    property OnNext: TRPConnectorEvent read FOnNext write FOnNext;
    property OnEOF: TRPEOFEvent read FOnEOF write FOnEOF;
    property OnGetCols: TRPConnectorEvent read FOnGetCols write FOnGetCols;
    property OnGetRow: TRPConnectorEvent read FOnGetRow write FOnGetRow;
    property OnSetFilter: TRPConnectorEvent read FOnSetFilter write FOnSetFilter;
    property OnGetSorts: TRPConnectorEvent read FOnGetSorts write FOnGetSorts;
    property OnSetSort: TRPConnectorEvent read FOnSetSort write FOnSetSort;
    property OnOpen: TRPConnectorEvent read FOnOpen write FOnOpen;
    property OnRestore: TRPConnectorEvent read FOnRestore write FOnRestore;
    property OnValidateRow: TRPValidateRowEvent read FOnValidateRow write FOnValidateRow;
  end; { TRvCustomConnection }

var
  DataConnectManager: TDataConnectManager;
  ManualSync: boolean = false;

  procedure ExecSyncMethod;

implementation

uses
  TypInfo;

{ class TDataConnectManager }

  constructor TDataConnectManager.Create;

  begin { Create }
    inherited Create;
    FConnectList := TList.Create;
  end;  { Create }

  destructor TDataConnectManager.Destroy;

  begin { Destroy }
    FreeAndNil(FConnectList);
    inherited Destroy;
  end;  { Destroy }

  procedure TDataConnectManager.AddConnection(Connection: TRvCustomConnection);

  begin { AddConnection }
    FConnectList.Add(Connection);
  end;  { AddConnection }

  procedure TDataConnectManager.RemoveConnection(Connection: TRvCustomConnection);

  begin { RemoveConnection }
    FConnectList.Remove(Connection);
  end;  { RemoveConnection }

  function TDataConnectManager.FindConnection(ConnectName: string): TRvCustomConnection;

  var
    I1: integer;

  begin { FindConnection }
    for I1 := 0 to FConnectList.Count - 1 do begin
      Result := TRvCustomConnection(FConnectList[I1]);
      if AnsiCompareText(Result.Name,ConnectName) = 0 then begin
        Exit;
      end; { if }
    end; { for }
    Result := nil;
  end;  { FindConnection }

{ class TDataConnectThread }

var
  DataConMethod: TThreadMethod = nil;

  procedure ExecSyncMethod;

  begin { ExecSyncMethod }
    if Assigned(DataConMethod) then begin
      try
        DataConMethod;
      finally
        DataConMethod := nil;
      end; { tryf }
    end; { if }
  end;  { ExecSyncMethod }

  procedure TDataConnectThread.SyncExecute;

  begin { SyncExecute }
    try
      SyncMethod;
    except
    {!!! Log error message and report back to controller }
    end; { tryx }
    SyncRan := true;
  end;  { SyncExecute }

  procedure TDataConnectThread.DoEvent(AMethod: TThreadMethod);

  begin { DoEvent }
    SyncMethod := AMethod;
    SyncRan := false;
    if ManualSync then begin
      DataConMethod := SyncExecute;
    end else begin
      Synchronize(SyncExecute);
    end; { else }
    while not SyncRan do begin
      Sleep(0);
    end; { while }
  end;  { DoEvent }

  procedure TDataConnectThread.Execute;
  var
    I1: integer;
    WaitResult: DWORD;
    EventList: array[1..2] of THandle;
    CompletedEvent: THandle;
    CompletedXTEvent: THandle;
    ConnectEvent: THandle;
    DisconnectEvent: THandle;
    DataEvent: THandle;
    Connected: boolean;
    OldDataConName: string;

  begin { Execute }
    Connected := false;
    with DataCon do begin
      {$IFDEF DBG}TDbg({Trans-}'Begin');{$ENDIF}
    { Create Events }
      CompletedEvent := InitEvent(CompletedEventName);
      DisconnectEvent := InitManualEvent(DisconnectEventName);
      ConnectEvent := InitEvent(ConnectEventName);
      OldDataConName := '!';
      DataEvent := 0;
      CompletedXTEvent := 0;
      try
        repeat
          if OldDataConName <> Name then begin { Recreate named events }
            if DataEvent <> 0 then begin
              CloseEvent(DataEvent);
            end; { if }
            if CompletedXTEvent <> 0 then begin
              CloseEvent(CompletedXTEvent);
            end; { if }
            if Runtime then begin
              DataEvent := InitEvent(DataRTEventName + Name);
              CompletedXTEvent := InitEvent(AcknowledgeRTEventName + Name);
            end else begin
              DataEvent := InitEvent(DataDTEventName + Name);
              CompletedXTEvent := InitEvent(AcknowledgeDTEventName + Name);
            end; { else }
            OldDataConName := Name;
          end; { if }
          EventList[1] := DataEvent;
          if Connected then begin
            EventList[2] := DisconnectEvent;
          end else begin
            EventList[2] := ConnectEvent;
          end; { else }
          WaitResult := WaitForMultipleObjects(2,@EventList,false,1000);
          if not Terminated then begin
            if WaitResult = WAIT_OBJECT_0 then begin { Data Request }
              ClearBuffer;
              I1 := ReadInt;
              {$IFDEF DBG}TDbg({Trans-}'Received Data Request #' + IntToStr(I1));{$ENDIF}
              case I1 of
                DATAFIRST: begin
                  DoEvent(ExecFirst);
                end;
                DATANEXT: begin
                  DoEvent(ExecNext);
                end;
                DATAEOF: begin
                  DoEvent(ExecEOF);
                end;
                DATAGETCOLS: begin
                  DoEvent(ExecGetCols);
                end;
                DATAGETROW: begin
                  DoEvent(ExecGetRow);
                end;
                DATASETFILTER: begin
                  DoEvent(ExecSetFilter);
                end;
                DATAGETSORTS: begin
                  DoEvent(ExecGetSorts);
                end;
                DATASETSORT: begin
                  DoEvent(ExecSetSort);
                end;
                DATAOPEN: begin
                  DoEvent(ExecOpen);
                end;
                DATARESTORE: begin
                  DoEvent(ExecRestore);
                end;
                DATAACKNOWLEDGE: begin
                  ClearBuffer;
                  WriteStr(Name);
                  WriteBool(Runtime);
                  WriteInt(Application.Handle); { Application Handle }
                  WriteInt(integer(FDataCon)); { Connection Handle }
                  WriteInt(DataConVersion); { Version }
                  WriteInt(integer(DataCon.RuntimeVisibility)); { Runtime visibility }
                end;
                DATAFREEALTBUF: begin
                  if CurrentFileMap > 0 then begin
                    CloseFileMap(AltFileMap,AltFileBuf);
                    CurrentFileMap := 0;
                    CurrentMapSize := FileMapSize;
                    FileBuf := NormFileBuf;
                    FilePtr := FileBuf;
                  end; { if }
                end;
              end; { case }
              if I1 = DATAACKNOWLEDGE then begin
                {$IFDEF DBG} if Runtime then begin
                  TDbg({Trans-}'Signal AckRTCompleted');
                end else begin
                  TDbg({Trans-}'Signal AckDTCompleted');
                end; {$ENDIF}
                SetEvent(CompletedXTEvent);
              end else begin
                if not ErrorResult then begin
                  {$IFDEF DBG}TDbg({Trans-}'Signal Completed');{$ENDIF}
                  SetEvent(CompletedEvent); { Signal Completed }
                end; { if }
              end; { else }
            end else if WaitResult = (WAIT_OBJECT_0 + 1) then begin { Connect Request }
              Connected := not Connected;
              if Connected then begin
                {$IFDEF DBG}TDbg({Trans-}'Received Connect Request');{$ENDIF}
                ClearBuffer; { Copy data connection name to buffer }
                WriteStr(Name);
                WriteBool(Runtime);
                WriteInt(Application.Handle); { Application Handle }
                WriteInt(integer(FDataCon));
                WriteInt(DataConVersion);
                WriteInt(integer(DataCon.RuntimeVisibility));
                {$IFDEF DBG}TDbg({Trans-}'Signal Completed');{$ENDIF}
                SetEvent(CompletedEvent); { Signal Completed }
              end else begin
                {$IFDEF DBG}TDbg({Trans-}'Received Disconnect Request');{$ENDIF}
              end; { else }
            end; { else }
          end else begin
            {$IFDEF DBG}TDbg({Trans-}'Received Terminate Request (' + IntToStr(WaitResult) + ')');{$ENDIF}
          end; { else }
          ErrorResult := false;
        until Terminated;
      finally
        CloseEvent(CompletedXTEvent);
        CloseEvent(CompletedEvent);
        CloseEvent(ConnectEvent);
        CloseEvent(DisconnectEvent);
        CloseEvent(DataEvent);
        ThreadDone := true;
      end; { tryf }
      {$IFDEF DBG}TDbg({Trans-}'End');{$ENDIF}
    end; { with }
  end;  { Execute }

{ class TDataConnectFieldDef }

const
  DataTypeSize: array[TRPDataType] of byte = (SizeOf(string),SizeOf(integer),
   SizeOf(boolean),SizeOf(extended),SizeOf(currency),SizeOf(currency),
   SizeOf(TDateTime),SizeOf(TDateTime),SizeOf(TDateTime),0,0,0,SizeOf(int64));

  constructor TDataConnectFieldDef.Create(ADataType: TRPDataType;
                                          AFieldName: string;
                                          AIncluded: boolean);
  begin { Create }
    inherited Create;
    FDataType := ADataType;
    FFieldName := AFieldName;
    FIncluded := AIncluded;
  end;  { Create }

  destructor TDataConnectFieldDef.Destroy;

  begin { Destroy }
    Clear;
    inherited Destroy;
  end;  { Destroy }

  procedure TDataConnectFieldDef.Clear;

  begin { Clear }
    if DataType = dtString then begin
      StrData := '';
    end else begin
      if DataSize > 0 then begin
        FreeMem(BinData,DataSize);
        BinData := nil;
      end; { if }
    end; { else }
    DataSize := 0;
  end;  { Clear }

  procedure TDataConnectFieldDef.SetFilter(var Value);
  begin { SetFilter }
    Clear;
    DataSize := DataTypeSize[DataType];
    if DataType = dtString then begin
      StrData := string(Value);
    end else begin
      if DataSize > 0 then begin
        GetMem(BinData,DataSize);
        case DataType of
          dtInteger: integer(BinData^) := integer(Value);
          dtInt64: int64(BinData^) := int64(Value);
          dtBoolean: boolean(BinData^) := boolean(Value);
          dtFloat: extended(BinData^) := extended(Value);
          dtCurrency,dtBCD: currency(BinData^) := currency(Value);
          dtDate,dtTime,dtDateTime: TDateTime(BinData^) := TDateTime(Value);
        { dtBlob,dtMemo,dtGraphic }
        end; { case }
      end; { if }
    end; { else }
  end;  { SetFilter }

  function TDataConnectFieldDef.FilterValid(var Value): boolean;
  begin { FilterValid }
    Result := true;
    if DataSize > 0 then begin
      case DataType of
        dtString: Result := AnsiCompareStr(StrData,string(Value)) = 0;
        dtInteger: Result := integer(BinData^) = integer(Value);
        dtInt64: Result := int64(BinData^) = int64(Value);
        dtBoolean: Result := boolean(BinData^) = boolean(Value);
        dtFloat: Result := extended(BinData^) = extended(Value);
        dtCurrency,dtBCD: Result := currency(BinData^) = currency(Value);
        dtDate,dtTime,dtDateTime: Result := TDateTime(BinData^) = TDateTime(Value);
      { dtBlob,dtMemo,dtGraphic }
      end; { case }
    end; { if }
  end;  { FilterValid }

{ class TRvCustomConnection }

  constructor TRvCustomConnection.Create(AOwner: TComponent);
  begin { Create }
    inherited Create(AOwner);
    FieldDefList := TList.Create;
    DataConnectManager.AddConnection(self);
    Runtime := not (csDesigning in ComponentState);
    if Runtime then begin
      ModeStr := {Trans-}'RT';
    end else begin
      ModeStr := {Trans-}'DT';
    end; { else }
    CurrentMapSize := FileMapSize;
    CurrentFileMap := 0;
    NormFileMap := InitFileMap(FileMapName,CurrentMapSize);
    NormFileBuf := InitFileBuf(NormFileMap);
    FileBuf := NormFileBuf;
  {$IFDEF DBG}
    DebugMap := InitDebugMap;
    DebugBuf := InitFileBuf(DebugMap);
  {$ENDIF}
    if not Runtime then begin { Always create thread in design-time }
      ThreadDone := false;
      Thread := TDataConnectThread.Create(true);
      Thread.FreeOnTerminate := true;
      Thread.DataCon := self;
      Thread.Resume;
    end else begin
      Thread := nil;
    end; { else }
    FLocalFilter := true;
    FRTVisible := rtDeveloper;
    FFieldAliasList := TStringList.Create;
  end;  { Create }

  destructor TRvCustomConnection.Destroy;
  var
    DataEvent: THandle;
    ControllerMutex: THandle;
    WaitResult: DWORD;
  begin { Destroy }
    DataConnectManager.RemoveConnection(self);
    if Assigned(Thread) then begin
      Thread.Terminate;

    { Try to gain control of RAVE Data Communication System }
      ControllerMutex := InitMutex(ControllerMutexName);
      try
      { Try safe terminate, otherwise just allow thread to terminate on its own }
        {$IFDEF DBG}Dbg({Trans-}'Attempt to Gain Control');{$ENDIF}
        WaitResult := WaitForSingleObject(ControllerMutex,1000);
        {$IFDEF DBG}Dbg({Trans-}'Received Control (' + IntToStr(WaitResult) + ')');{$ENDIF}
        if WaitResult = WAIT_OBJECT_0 then begin
          if Runtime then begin
            DataEvent := InitEvent(DataRTEventName + Name);
          end else begin
            DataEvent := InitEvent(DataDTEventName + Name);
          end; { else }
          try
            {$IFDEF DBG}Dbg({Trans-}'Signal Terminate (DataRequest)');{$ENDIF}
            SetEvent(DataEvent); { Signal Data Event with Terminated = true }
            repeat
              Sleep(0);
            until ThreadDone;
          finally
            CloseEvent(DataEvent);
            {$IFDEF DBG}Dbg({Trans-}'Release Control');{$ENDIF}
            ReleaseMutex(ControllerMutex);
          end; { tryf }
        end; { if }
      finally
        CloseMutex(ControllerMutex);
      end; { tryf }
    end; { if }

  { Free resources }
    if CurrentFileMap > 0 then begin
      CloseFileMap(AltFileMap,AltFileBuf);
    end; { if }
    CloseFileMap(NormFileMap,NormFileBuf);
  {$IFDEF DBG}
    CloseFileMap(DebugMap,DebugBuf);
  {$ENDIF}
    ClearFieldDefs;
    FreeAndNil(FieldDefList);
    FreeAndNil(FFieldAliasList);

    inherited Destroy;
  end;  { Destroy }

  procedure TRvCustomConnection.ReturnError(Msg: string; ErrorType: TRaveConnectionError);
  var
    ErrorEvent: THandle;
  begin { ReturnError }
    ClearBuffer;
    WriteInt(Ord(ErrorType));
    WriteStr(Msg);
    ErrorResult := true;
    ErrorEvent := InitEvent(ErrorEventName);
    SetEvent(ErrorEvent);
    CloseEvent(ErrorEvent);
  end;  { ReturnError }

  procedure TRvCustomConnection.ReadVisible(Reader: TReader);
  begin { ReadVisible }
    if Reader.ReadBoolean then begin
      SetRTVisible(rtEndUser);
    end; { if }
  end;  { ReadVisible }

  procedure TRvCustomConnection.DefineProperties(Filer: TFiler);
  begin { DefineProperties }
    inherited DefineProperties(Filer);
    Filer.DefineProperty({Trans-}'Visible',ReadVisible,nil,false);
  end;  { DefineProperties }

  procedure TRvCustomConnection.SetRTVisible(Value: TRuntimeVisibility);
  begin { SetRTVisible }
    FRTVisible := Value;
    if not Assigned(Thread) and
     (FRTVisible in [rtDeveloper,rtEndUser]) then begin { Create thread }
      ThreadDone := false;
      Thread := TDataConnectThread.Create(true);
      Thread.FreeOnTerminate := true;
      Thread.DataCon := self;
      Thread.Resume;
    end; { if }
  end;  { SetRTVisible }

  procedure TRvCustomConnection.SetFieldAliasList(Value: TStrings);
  begin { SetFieldAliasList }
    FFieldAliasList.Assign(Value);
  end;  { SetFieldAliasList }

  function TRvCustomConnection.GetFieldAlias(FieldName: string): string;
  var
    I1: integer;
    I2: integer;
    S1: string;

  begin { GetFieldAlias }
    Result := FieldName;
    for I1 := 0 to FFieldAliasList.Count - 1 do begin
      S1 := FFieldAliasList[I1];
      I2 := Pos('=',S1);
      if (I2 > 0) and (AnsiCompareText(FieldName,Trim(Copy(S1,1,I2 - 1))) = 0) then begin
        Result := Trim(Copy(S1,I2 + 1,Length(S1) - I2));
        Exit;
      end; { if }
    end; { for }
  end;  { GetFieldAlias }

  procedure TRvCustomConnection.WriteStr(Value: string);
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

  procedure TRvCustomConnection.WriteInt(Value: longint);
  begin { WriteInt }
    longint(pointer(FilePtr)^) := Value;
    Inc(FilePtr,SizeOf(longint));
  end;  { WriteInt }

  procedure TRvCustomConnection.WriteInt64(Value: int64);
  begin { WriteInt64 }
    int64(pointer(FilePtr)^) := Value;
    Inc(FilePtr,SizeOf(int64));
  end;  { WriteInt64 }

  procedure TRvCustomConnection.WriteBool(Value: boolean);
  begin { WriteBool }
    boolean(pointer(FilePtr)^) := Value;
    Inc(FilePtr,SizeOf(boolean));
  end;  { WriteBool }

  procedure TRvCustomConnection.WriteFloat(Value: extended);
  begin { WriteFloat }
    extended(pointer(FilePtr)^) := Value;
    Inc(FilePtr,SizeOf(extended));
  end;  { WriteFloat }

  procedure TRvCustomConnection.WriteCurr(Value: currency);
  begin { WriteCurr }
    currency(pointer(FilePtr)^) := Value;
    Inc(FilePtr,SizeOf(currency));
  end;  { WriteCurr }

  procedure TRvCustomConnection.WriteDateTime(Value: TDateTime);
  begin { WriteDateTime }
    TDateTime(pointer(FilePtr)^) := Value;
    Inc(FilePtr,SizeOf(TDateTime));
  end;  { WriteDateTime }

  procedure TRvCustomConnection.WriteBuf(var Buffer; Len: integer);
  begin { WriteBuf }
   Move(Buffer,FilePtr^,Len);
   Inc(FilePtr,Len);
  end;  { WriteBuf }

  procedure TRvCustomConnection.ClearFieldDefs;
  var
    I1: integer;
  begin { ClearFieldDefs }
    for I1 := 0 to (FieldDefList.Count - 1) do begin
      if Assigned(FieldDefList[I1]) then begin
        TDataConnectFieldDef(FieldDefList[I1]).Free;
      end; { if }
    end; { for }
    FieldDefList.Clear;
  end;  { ClearFieldDefs }

  procedure TRvCustomConnection.ClearFilterData;
  var
    I1: integer;
  begin { ClearFilterData }
    for I1 := 0 to (FieldDefList.Count - 1) do begin
      if Assigned(FieldDefList[I1]) then begin
        TDataConnectFieldDef(FieldDefList[I1]).Clear;
      end; { if }
    end; { for }
  end;  { ClearFilterData }

  function TRvCustomConnection.CurrentFieldDef: TDataConnectFieldDef;
  begin { CurrentFieldDef }
    if FieldIdx >= FieldDefList.Count then begin
      Result := nil;
    end else begin
      Result := TDataConnectFieldDef(FieldDefList[FieldIdx]);
    end; { else }
  end;  { CurrentFieldDef }

  procedure TRvCustomConnection.CheckDataType(ValidDataTypes: TRPDataTypeSet);
  begin { CheckDataType }
    if FieldIdx >= FieldDefList.Count then begin
      RaiseError(Trans('Attempt to write more fields than defined.'));
    end else begin
      if not (TDataConnectFieldDef(FieldDefList[FieldIdx]).DataType in ValidDataTypes) then begin
        RaiseError(Trans(Format({Trans+}
         'Attempt to write different data type than defined (Column %0:s/%1:s).',
         [IntToStr(FieldIdx + 1),IntToStr(FieldDefList.Count)])));
      end; { if }
    end; { else }
  end;  { CheckDataType }

  procedure TRvCustomConnection.WriteField(Name: string;
                                           DataType: TRPDataType;
                                           Width: integer;
                                           FullName: string;
                                           Description: string);
  var
    FieldDef: TDataConnectFieldDef;
    PrevFieldIndex: integer;
    FieldAlias: string;
  begin { WriteField }
    if FieldDefList.Count = 0 then begin
      PrevFieldIndex := -1;
    end else begin
      PrevFieldIndex := TDataConnectFieldDef(FieldDefList[FieldDefList.Count - 1]).FieldIndex;
    end; { else }

    FieldAlias := GetFieldAlias(Name);
    FieldDef := TDataConnectFieldDef.Create(DataType,Name,FieldAlias <> '');
    if FieldDef.Included then begin
      FieldDef.FieldIndex := PrevFieldIndex + 1;
      WriteStr('N');
      WriteStr(FieldAlias);
      if DataType <> dtString then begin
        WriteStr('T');
        WriteStr(GetEnumName(TypeInfo(TRPDataType),Ord(DataType)));
      end; { if }
      if Width > 0 then begin
        WriteStr('W');
        WriteStr(IntToStr(Width));
      end; { if }
      if FullName <> '' then begin
        WriteStr('L');
        WriteStr(FullName);
      end; { if }
      if Description <> '' then begin
        WriteStr('D');
        WriteStr(Description);
      end; { if }
      WriteStr('');
    end else begin
      FieldDef.FieldIndex := PrevFieldIndex;
    end; { else }
    FieldDefList.Add(FieldDef);
  end;  { WriteField }

  procedure TRvCustomConnection.AddingNative;
  begin { AddingNative }
    integer(pointer(RowHeaderPtr + (IncludedFieldIdx * 8))^) := FilePtr -
     RowHeaderPtr;
  end;  { AddingNative }

  procedure TRvCustomConnection.AddingFormatted;
  begin { AddingFormatted }
    integer(pointer(RowHeaderPtr + (IncludedFieldIdx * 8) + 4)^) := FilePtr -
     RowHeaderPtr;
  end;  { AddingFormatted }

  procedure TRvCustomConnection.AllocateSpace(AddSize: longint);
  var
    NewFileMapIdx: byte;
    NewFileMap: THandle;
    NewFileBuf: PChar;
    NewSize: longint;
    SavePtr: PChar;
  begin { AllocateSpace }
    if (FilePtr - FileBuf + AddSize) > CurrentMapSize then begin
      NewFileMapIdx := CurrentFileMap mod 2 + 1;
      NewSize := (FilePtr - FileBuf) + AddSize + 65536;

    { Create new file map }
      NewFileMap := InitFileMap(AltFileMapName + IntToStr(NewFileMapIdx),NewSize);
      NewFileBuf := InitFileBuf(NewFileMap);

    { Copy data from old buffer to new }
      Move(FileBuf^,NewFileBuf^,FilePtr - FileBuf);

    { Reassign utility pointers }
      FilePtr := NewFileBuf + (FilePtr - FileBuf);
      RowHeaderPtr := NewFileBuf + (RowHeaderPtr - FileBuf);

    { Free old alternate file map if necessary }
      if CurrentFileMap > 0 then begin
        CloseFileMap(AltFileMap,AltFileBuf);
      end; { if }

    { Write out NewFileMapIdx to normal buffer for redirect }
      SavePtr := FilePtr;
      FilePtr := NormFileBuf;
      WriteInt(0);
      WriteInt(NewFileMapIdx);
      FilePtr := SavePtr;

    { Assign new file map to Alt vars and status vars }
      AltFileMap := NewFileMap;
      AltFileBuf := NewFileBuf;
      CurrentMapSize := NewSize;
      CurrentFileMap := NewFileMapIdx;
      FileBuf := AltFileBuf;
    end; { if }
  end;  { AllocateSpace }

  function TRvCustomConnection.LocalFilterAbort(var FilterData): boolean;
  begin { LocalFilterAbort }
    if LocalFilter and LocalFilterValid then begin
      LocalFilterValid := CurrentFieldDef.FilterValid(FilterData);
    end; { if }
    Result := LocalFilter and not LocalFilterValid;
  end;  { LocalFilterAbort }

  procedure TRvCustomConnection.NextField;
  begin { NextField }
    if CurrentFieldDef.Included then begin
      Inc(IncludedFieldIdx);
    end; { if }
    Inc(FieldIdx);
  end;  { NextField }

  procedure TRvCustomConnection.WriteNullData;
  begin { WriteNullData }
    if LocalFilter and (CurrentFieldDef.DataSize <> 0) then begin
      LocalFilterValid := false;
    end; { if }
    if LocalFilterValid and CurrentFieldDef.Included then begin
      CheckDataType([Low(TRPDataType)..High(TRPDataType)]);
      integer(pointer(RowHeaderPtr + (IncludedFieldIdx * 8))^) := -1;
    end; { if }
    NextField;
  end;  { WriteNullData }

  procedure TRvCustomConnection.WriteStrData(FormatData, NativeData: string);
  begin { WriteStrData }
    if CurrentFieldDef.Included and not LocalFilterAbort(NativeData) then begin
      CheckDataType([dtString]);
      if (NativeData <> FormatData) and (FormatData <> '') then begin
        AllocateSpace(Length(FormatData) + Length(NativeData) + 8);
        AddingFormatted;
        WriteStr(FormatData);
      end else begin
        AllocateSpace(Length(NativeData) + 4);
      end; { else }
      AddingNative;
      WriteStr(NativeData);
    end; { if }
    NextField;
  end;  { WriteStrData }

  procedure TRvCustomConnection.WriteIntData(FormatData: string; NativeData: integer);
  begin { WriteIntData }
    if CurrentFieldDef.Included and not LocalFilterAbort(NativeData) then begin
      CheckDataType([dtInteger]);
      if (IntToStr(NativeData) <> FormatData) and (FormatData <> '') then begin
        AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
        AddingFormatted;
        WriteStr(FormatData);
      end else begin
        AllocateSpace(SizeOf(NativeData));
      end; { else }
      AddingNative;
      WriteInt(NativeData);
    end; { if }
    NextField;
  end;  { WriteIntData }

  procedure TRvCustomConnection.WriteInt64Data(FormatData: string; NativeData: int64);
  begin { WriteInt64Data }
    if CurrentFieldDef.Included and not LocalFilterAbort(NativeData) then begin
      CheckDataType([dtInt64]);
      if (IntToStr(NativeData) <> FormatData) and (FormatData <> '') then begin
        AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
        AddingFormatted;
        WriteStr(FormatData);
      end else begin
        AllocateSpace(SizeOf(NativeData));
      end; { else }
      AddingNative;
      WriteInt64(NativeData);
    end; { if }
    NextField;
  end;  { WriteInt64Data }

  procedure TRvCustomConnection.WriteBoolData(FormatData: string; NativeData: boolean);
  begin { WriteBoolData }
    if CurrentFieldDef.Included and not LocalFilterAbort(NativeData) then begin
      CheckDataType([dtBoolean]);
      if FormatData <> '' then begin
        AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
        AddingFormatted;
        WriteStr(FormatData);
      end else begin
        AllocateSpace(SizeOf(NativeData));
      end; { else }
      AddingNative;
      WriteBool(NativeData);
    end; { if }
    NextField;
  end;  { WriteBoolData }

  procedure TRvCustomConnection.WriteFloatData(FormatData: string; NativeData: extended);
  begin { WriteFloatData }
    if CurrentFieldDef.Included and not LocalFilterAbort(NativeData) then begin
      CheckDataType([dtFloat]);
      if FormatData <> '' then begin
        AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
        AddingFormatted;
        WriteStr(FormatData);
      end else begin
        AllocateSpace(SizeOf(NativeData));
      end; { else }
      AddingNative;
      WriteFloat(NativeData);
    end; { if }
    NextField;
  end;  { WriteFloatData }

  procedure TRvCustomConnection.WriteCurrData(FormatData: string; NativeData: currency);
  begin { WriteCurrData }
    if CurrentFieldDef.Included and not LocalFilterAbort(NativeData) then begin
      CheckDataType([dtCurrency]);
      if FormatData <> '' then begin
        AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
        AddingFormatted;
        WriteStr(FormatData);
      end else begin
        AllocateSpace(SizeOf(NativeData));
      end; { else }
      AddingNative;
      WriteCurr(NativeData);
    end; { if }
    NextField;
  end;  { WriteCurrData }

  procedure TRvCustomConnection.WriteBCDData(FormatData: string; NativeData: currency);
  begin { WriteBCDData }
    if CurrentFieldDef.Included and not LocalFilterAbort(NativeData) then begin
      CheckDataType([dtBCD]);
      if FormatData <> '' then begin
        AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
        AddingFormatted;
        WriteStr(FormatData);
      end else begin
        AllocateSpace(SizeOf(NativeData));
      end; { else }
      AddingNative;
      WriteCurr(NativeData);
    end; { if }
    NextField;
  end;  { WriteBCDData }

  procedure TRvCustomConnection.WriteDateTimeData(FormatData: string; NativeData: TDateTime);
  begin { WriteDateTimeData }
    if CurrentFieldDef.Included and not LocalFilterAbort(NativeData) then begin
      CheckDataType([dtDateTime,dtDate,dtTime]);
      if FormatData <> '' then begin
        AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
        AddingFormatted;
        WriteStr(FormatData);
      end else begin
        AllocateSpace(SizeOf(NativeData));
      end; { else }
      AddingNative;
      WriteDateTime(NativeData);
    end; { if }
    NextField;
  end;  { WriteDateTimeData }

  procedure TRvCustomConnection.WriteBlobData(var Buffer; Len: longint);
  begin { WriteBlobData }
    if LocalFilterValid and CurrentFieldDef.Included then begin
      CheckDataType([dtBlob,dtMemo,dtGraphic]);
      AllocateSpace(Len + 4);
      AddingNative;
      WriteInt(Len);
      if Len > 0 then begin
        WriteBuf(Buffer,Len);
      end; { if }
    end; { if }
    NextField;
  end;  { WriteBlobData }

  function TRvCustomConnection.ReadStr: string;
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

  function TRvCustomConnection.ReadInt: integer;
  begin { ReadInt }
    Result := integer(pointer(FilePtr)^);
    Inc(FilePtr,SizeOf(integer));
  end;  { ReadInt }

  function TRvCustomConnection.ReadInt64: int64;
  begin { ReadInt64 }
    Result := int64(pointer(FilePtr)^);
    Inc(FilePtr,SizeOf(int64));
  end;  { ReadInt64 }

  function TRvCustomConnection.ReadBool: boolean;
  begin { ReadBool }
    Result := boolean(pointer(FilePtr)^);
    Inc(FilePtr,SizeOf(boolean));
  end;  { ReadBool }

  function TRvCustomConnection.ReadFloat: extended;
  begin { ReadFloat }
    Result := extended(pointer(FilePtr)^);
    Inc(FilePtr,SizeOf(extended));
  end;  { ReadFloat }

  function TRvCustomConnection.ReadCurr: currency;
  begin { ReadCurr }
    Result := currency(pointer(FilePtr)^);
    Inc(FilePtr,SizeOf(currency));
  end;  { ReadCurr }

  function TRvCustomConnection.ReadDateTime: TDateTime;
  begin { ReadDateTime }
    Result := TDateTime(pointer(FilePtr)^);
    Inc(FilePtr,SizeOf(TDateTime));
  end;  { ReadDateTime }

  procedure TRvCustomConnection.ReadBuf(var Buffer; Len: integer);
  begin { ReadBuf }
   Move(FilePtr^,Buffer,Len);
   Inc(FilePtr,Len);
  end;  { ReadBuf }

  function TRvCustomConnection.FindFieldDef(FieldName: string): TDataConnectFieldDef;
  var
    I1: integer;
  begin { FindFieldDef }
    for I1 := 0 to FieldDefList.Count - 1 do begin
      Result := TDataConnectFieldDef(FieldDefList[I1]);
      if CompareText(Result.FieldName,FieldName) = 0 then Exit;
    end; { for }
    Result := nil;
  end;  { FindFieldDef }

  function TRvCustomConnection.ReadFilterItem(var FieldName: string;
                                              var FieldOp: string;
                                              var FieldData: string): boolean;
  var
    DataType: TRPDataType;
    FieldDef: TDataConnectFieldDef;
    DataInt: integer;
    DataInt64: int64;
    DataBool: boolean;
    DataFloat: extended;
    DataCurr: currency;
    DataDateTime: TDateTime;
  begin { ReadFilterItem }
    FieldName := ReadStr;
    Result := (FieldName <> '');
    if Result then begin
      FieldOp := ReadStr;
      FieldData := ReadStr;
      DataType := TRPDataType(ReadInt);
      case DataType of
        dtString: begin end; { Nothing to read, use FieldData }
        dtInteger: DataInt := ReadInt;
        dtInt64: DataInt64 := ReadInt64;
        dtBoolean: DataBool := ReadBool;
        dtFloat: DataFloat := ReadFloat;
        dtCurrency,dtBCD: DataCurr := ReadCurr;
        dtDate,dtTime,dtDateTime: DataDateTime := ReadDateTime;
      { dtBlob,dtMemo,dtGraphic }
      end; { case }

    { Assign filter data to FieldDef }
      FieldDef := FindFieldDef(FieldName);
      if Assigned(FieldDef) and (DataType = FieldDef.DataType) then begin
        case DataType of
          dtString: FieldDef.SetFilter(FieldData);
          dtInteger: FieldDef.SetFilter(DataInt);
          dtInt64: FieldDef.SetFilter(DataInt64);
          dtBoolean: FieldDef.SetFilter(DataBool);
          dtFloat: FieldDef.SetFilter(DataFloat);
          dtCurrency,dtBCD: FieldDef.SetFilter(DataCurr);
          dtDate,dtTime,dtDateTime: FieldDef.SetFilter(DataDateTime);
        { dtBlob,dtMemo,dtGraphic }
        end; { case }
      end; { if }
    end; { if }
  end;  { ReadFilterItem }

  function TRvCustomConnection.GetFilterItem: boolean;
  begin { GetFilterItem }
    Result := ReadFilterItem(FFilterFieldName,FFilterFieldOp,FFilterFieldData);
  end;  { GetFilterItem }

  procedure TRvCustomConnection.ClearBuffer;
  begin { ClearBuffer }
  { Free AltFileBuf }
    if CurrentFileMap > 0 then begin
      CloseFileMap(AltFileMap,AltFileBuf);
      FileBuf := NormFileBuf;
      CurrentFileMap := 0;
      CurrentMapSize := FileMapSize;
    end; { if }
    FilePtr := FileBuf + 2 * SizeOf(longint); { Save space for bufsize & mapidx }
  end;  { ClearBuffer }

  procedure TRvCustomConnection.FixupBufferHeader;
  var
    SavePtr: PChar;
  begin { FixupBufferHeader }
  { Write out buffer size and CurrentFileMap }
    SavePtr := FilePtr;
    FilePtr := FileBuf;
    WriteInt(SavePtr - FileBuf); { Buffer size }
    WriteInt(CurrentFileMap); { File Map # }
    FilePtr := SavePtr;
  end;  { FixupBufferHeader }

  procedure TRvCustomConnection.ClearBufferHeader;
  var
    SavePtr: PChar;
  begin { ClearBufferHeader }
  { Clear buffer header values }
    SavePtr := FilePtr;
    FilePtr := FileBuf;
    WriteInt(0); { Buffer size }
    WriteInt(0); { File Map # }
    FilePtr := SavePtr;
  end;  { ClearBufferHeader }

{$IFDEF DBG}
  procedure TRvCustomConnection.Dbg(Value: string);
  begin { Dbg }
    WriteDebug({Trans-}'    DataCon(' + Name + '_' + ModeStr + {Trans-}'): ' +
     Value,DebugBuf);
  end;  { Dbg }

  procedure TRvCustomConnection.TDbg(Value: string);
  begin { TDbg }
    WriteDebug({Trans-}'    Thread(' + Name + '_' + ModeStr + {Trans-}'): ' +
     Value,DebugBuf);
  end;  { TDbg }
{$ENDIF}

  procedure TRvCustomConnection.ExecFirst;
  begin { ExecFirst }
    if Assigned(FOnFirst) then begin
      FOnFirst(self);
    end else begin
      DoFirst;
    end; { else }
    if LocalFilter then begin { Preread row for local filtering }
      ExecGetRow;
    end; { if }
    if not ExecValidateRow then begin
      ExecNext;
    end else begin
      if not LocalFilter then begin { Row was already read if LocalFilter }
        ExecGetRow;
      end; { if }
    end; { else }
  end;  { ExecFirst }

  procedure TRvCustomConnection.ExecNext;
  begin { ExecNext }
    repeat
      if Assigned(FOnNext) then begin
        FOnNext(self);
      end else begin
        DoNext;
      end; { else }
      if LocalFilter then begin { Preread row for local filtering }
        ExecGetRow;
      end; { if }
    until ExecValidateRow or GetEOF;
    if not LocalFilter then begin { Row was already read if LocalFilter }
      ExecGetRow;
    end; { if }
  end;  { ExecNext }

  procedure TRvCustomConnection.ExecEOF;
  var
    AtEOF: boolean;
  begin { ExecEOF }
    if Assigned(FOnEOF) then begin
      FOnEOF(self,AtEOF);
    end else begin
      AtEOF := DoEOF;
    end; { else }
    ClearBuffer;
    WriteBool(AtEOF);
    FixupBufferHeader;
  end;  { ExecEOF }

  function TRvCustomConnection.GetEOF: boolean;
  begin { GetEOF }
    if Assigned(FOnEOF) then begin
      FOnEOF(self,Result);
    end else begin
      Result := DoEOF;
    end; { else }
  end;  { GetEOF }

  procedure TRvCustomConnection.ExecGetRow;
  var
    I1: integer;
  {$IFDEF DBG}
    TempPtr: PChar;
    S1: string;
  {$ENDIF}
  begin { ExecGetRow }
    ClearBuffer;
    if GetEOF then begin
      WriteBool(true);
      Exit; { Don't write out row data }
    end else begin
      WriteBool(false);
    end; { else }

  { Init row processing variables and allocate space for row header }
    FieldIdx := 0;
    IncludedFieldIdx := 0;
    WriteInt(0); { Place holder for data size }
    RowHeaderPtr := FilePtr; { Save current file position }
    for I1 := 0 to FieldDefList.Count - 1 do begin
      if TDataConnectFieldDef(FieldDefList[I1]).Included then begin
        WriteInt(0); { Offset for native data }
        WriteInt(0); { Offset for formatted data }
      end; { if }
    end; { for }

  { Add all row data }
    LocalFilterValid := true;
    if Assigned(FOnGetRow) then begin
      FOnGetRow(self);
    end else begin
      DoGetRow;
    end; { else }

  { Update data size }
    I1 := FilePtr - RowHeaderPtr;
    FilePtr := RowHeaderPtr - 4;
    WriteInt(I1);
    FilePtr := RowHeaderPtr + I1;
    FixupBufferHeader;

  {$IFDEF DBG}
    TempPtr := FileBuf + 2 * SizeOf(longint); { Save space for bufsize & mapidx }
    S1 := '';
    while TempPtr <> FilePtr do begin
      S1 := S1 + ' ' + IntToHex(byte(TempPtr^),2);
      Inc(TempPtr);
    end; { while }
    TDbg({Trans-}'GetRow Data:' + S1);
  {$ENDIF}
  end;  { ExecGetRow }

  procedure TRvCustomConnection.ExecGetCols;
  var
    SavePtr: PChar;
    I1: integer;
    FieldCount: integer;
  begin { ExecGetCols }
  { Init field processing allocate space for field count }
    ClearBuffer;
    ClearFieldDefs;
    WriteInt(0); { Place holder for field count }

  { Add all field data }
    if Assigned(FOnGetCols) then begin
      FOnGetCols(self);
    end else begin
      DoGetCols;
    end; { else }

  { Update field count }
    SavePtr := FilePtr;
    ClearBuffer;
  { Figure out how many included fields for FieldCount }
    FieldCount := 0;
    for I1 := 0 to FieldDefList.Count - 1 do begin
      if TDataConnectFieldDef(FieldDefList[I1]).Included then begin
        Inc(FieldCount);
      end; { if }
    end; { for }
    WriteInt(FieldCount); { Write field count }
    FilePtr := SavePtr;
    FixupBufferHeader;
  end;  { ExecGetCols }

  procedure TRvCustomConnection.ExecSetFilter;
  var
    FieldName: string;
    FieldOp: string;
    FieldData: string;
  begin { ExecSetFilter }
    ClearFilterData;
    if LocalFilter then begin { Read in native filter data using ReadFilterItem }
      while ReadFilterItem(FieldName,FieldOp,FieldData) do begin
      end; { while }
    end else if Assigned(FOnSetFilter) then begin
      FOnSetFilter(self);
    end else begin
      DoSetFilter;
    end; { else }
    ClearBufferHeader;
  end;  { ExecSetFilter }

  procedure TRvCustomConnection.ExecGetSorts;
  begin { ExecGetSorts }
    ClearBuffer;
    if Assigned(FOnGetSorts) then begin
      FOnGetSorts(self);
    end else begin
      DoGetSorts;
    end; { else }
    FixupBufferHeader;
  end;  { ExecGetSorts }

  procedure TRvCustomConnection.ExecSetSort;
  begin { ExecSetSort }
    if Assigned(FOnSetSort) then begin
      FOnSetSort(self);
    end else begin
      DoSetSort;
    end; { if }
    ClearBufferHeader;
  end;  { ExecSetSort }

  procedure TRvCustomConnection.ExecOpen;
  begin { ExecOpen }
    ClearBuffer;
    if Assigned(FOnOpen) then begin
      FOnOpen(self);
    end else begin
      DoOpen;
    end; { else }
    ClearBufferHeader;
  end;  { ExecOpen }

  procedure TRvCustomConnection.ExecRestore;
  begin { ExecRestore }
    ClearBuffer;
    if Assigned(FOnRestore) then begin
      FOnRestore(self);
    end else begin
      DoRestore;
    end; { if }
    ClearBufferHeader;
  end;  { ExecRestore }

  function TRvCustomConnection.ExecValidateRow: boolean;
  begin { ExecValidateRow }
    if LocalFilter then begin
      Result := LocalFilterValid;
    end else begin
      Result := true;
    end; { else }
    if Result then begin
      if Assigned(FOnValidateRow) then begin
        FOnValidateRow(self,Result);
      end else begin
        Result := DoValidateRow;
      end; { else }
    end; { if }
  end;  { ExecValidateRow }

  procedure TRvCustomConnection.DoFirst;
  begin { DoFirst }
    FDataIndex := 0;
  end;  { DoFirst }

  procedure TRvCustomConnection.DoNext;
  begin { DoNext }
    Inc(FDataIndex);
  end;  { DoNext }

  function TRvCustomConnection.DoEOF: boolean;
  begin { DoEOF }
    Result := DataIndex >= DataRows;
  end;  { DoEOF }

  procedure TRvCustomConnection.DoGetRow;
  begin { DoGetRow }
    WriteStr('[' + Trans('Inactive at design-time') + ']');
  end;  { DoGetRow }

  procedure TRvCustomConnection.DoGetCols;
  begin { DoGetCols }
    WriteField({Trans-}'FieldName',dtString,30,{Trans-}'FieldName','');
  end;  { DoGetCols }

  procedure TRvCustomConnection.DoSetFilter;
  begin { DoSetFilter }
  end;  { DoSetFilter }

  procedure TRvCustomConnection.DoGetSorts;
  begin { DoGetSorts }
    WriteStr('');
  end;  { DoGetSorts }

  procedure TRvCustomConnection.DoSetSort;
  begin { DoSetSort }
  end;  { DoSetSort }

  procedure TRvCustomConnection.DoOpen;
  begin { DoOpen }
    DataRows := 1;
    DataIndex := 0;
  end;  { DoOpen }

  procedure TRvCustomConnection.DoRestore;
  begin { DoRestore }
  end;  { DoRestore }

  function TRvCustomConnection.DoValidateRow: boolean;
  begin { DoValidateRow }
    Result := true;
  end;  { DoValidateRow }

initialization
  DataConnectManager := TDataConnectManager.Create;
finalization
  FreeAndNil(DataConnectManager);
end.