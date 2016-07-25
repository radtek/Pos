{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRvDataLink;
  {-Implements link between Rave and Data driver libraries}

interface

{$I RpVer.inc}

uses
  Classes, QRpDefine, QRvDLCommon, QRvDLBase;

type
  TRaveDataLink = class;

  TDataConnection = class(TObject)
  protected
    FDataLink: TRaveDataLink;
    FConnectionHandle: TConnectionHandle;
    FDataSource: string;
    FOptions: string;
    FUserName: string;
    FPassword: string;
  public
  {-Create a connection to the database backend}
    constructor Create(ADataLink: TRaveDataLink);
  {-Close connection to database back end}
    destructor Destroy; override;
  {-Open the connection to the database backend}
    procedure Open;
  {-Close the connection to the database backend}
    procedure Close;
  {-Return list of field definitions for requested table}
    procedure GetFieldDefs(const TableName: string; FieldDefs: TStrings);
  {-Return list of table names}
    procedure GetTableNames(TableNames: TStrings);
  {-Return true if user modifies ResultSet query command}
    function ResultSetConfigure(var QueryCommand: string; var AStruct: string): boolean;
    //
    property DataSource: string read FDataSource write FDataSource;
    property Password: string read FPassword write FPassword;
    property Options: string read FOptions write FOptions;
    property UserName: string read FUserName write FUserName;
  end; { TDataConnection }

  TResultSetField = class(TObject)
  public
    FieldName: string;
    DataType: TRpDataType;
    Width: integer;
    FullName: string;
    Description: string;
  end; {TResultSetField}

  TResultSetFieldList = class(TObject)
  protected
    FList: TList;
    function GetCount: integer;
    function GetItem(Index: integer): TResultSetField;
  public
  {-Create a list of result set fields}
    constructor Create;
  {-Free all dynamic structures}
    destructor Destroy; override;
  {-Add a result set field to the end of the list}
    function Add: TResultSetField;
  {-Remove all result set fields from list}
    procedure Clear;
    //
    property Count : integer read GetCount;
    property Items[Index: integer]: TResultSetField read GetItem; default;
  end; {TResultSetFieldList}

  TDataResultSet = class(TObject)
  protected
    FConnection: TDataConnection;
    FCommandText: string;
    FResultSetHandle: TResultSetHandle;
    FFieldList: TResultSetFieldList;
    FTransferList: PDLTransferFieldList;
    function GetFieldCount: integer;
  public
  {-Create a result set returned by a backend database}
    constructor Create(AConnection: TDataConnection);
  {-Close result set}
    destructor Destroy; override;
  {-Close result set returned by connection to backend database}
    procedure Close;
  {-Return true if at end of result set}
    function Eof: boolean;
  {-Move to first row in result set}
    procedure First;
  {-Return data for current row}
    procedure GetRow(var RowBuffer: Pointer; var RowBufSize: longint);
  {-Move to next row in result set}
    procedure Next;
  {-Ask backend database to create/open result set}
    procedure Open;
    //
    property FieldCount: integer read GetFieldCount;
    property Fields: TResultSetFieldList read FFieldList;
    property CommandText: string read FCommandText write FCommandText;
  end; {TDataResultSet}

  TRaveDataLinkProc = procedure(Params : Pointer);

  TRaveDataLink = class(TObject)
  protected
  public
  { Following fields are used for dynamic Data Links}
    FFileName: string;
    FLibHandle: longint;
    FInformationProc: TRaveDataLinkProc;
    FConnectionConfigureProc: TRaveDataLinkproc;
    FConnectionOpenProc: TRaveDataLinkProc;
    FConnectionCloseProc: TRaveDataLinkProc;
    FGetTablesProc: TRaveDataLinkProc;
    FGetFieldsProc: TRaveDataLinkProc;
    FResultSetOpenProc: TRaveDataLinkProc;
    FResultSetCloseProc: TRaveDataLinkProc;
    FResultSetConfigureProc: TRaveDataLinkProc;
    FResultSetFirstProc: TRaveDataLinkProc;
    FResultSetNextProc: TRaveDataLinkProc;
    FResultSetEOFProc: TRaveDataLinkProc;
    FResultSetGetRowProc: TRaveDataLinkProc;
    FResultSetSetFilterProc: TRaveDataLinkProc;
    FResultSetSetSortProc: TRaveDataLinkProc;
    FErrorTextProc: TRaveDataLinkProc;

  { Following fields are used for static Data Links}
    FDriver: TDlBaseDriver;

  { Follwoing fields used by both types of Data Links}
    FInstalled: boolean;
    FInternalName: string;
    FDisplayName: string;
    FVerMajor: integer;
    FVerMinor: integer;
    procedure BindProcs;
    procedure CheckInstalled;
    function FindProc(const Name: string): pointer;
    function GetIsDynamicLink: boolean;
    procedure Information;
    procedure LoadDLL(const FileName: string);
    procedure LogAPIError(const APIName, Message: string);
    procedure LogConnectionError(ConnectionHandle: TConnectionHandle; const APIName: string; const TableName: string = '');
    procedure LogGlobalError(const APIName: string);
    procedure LogResultSetError(ResultSetHandle: TResultSetHandle; const APIName: string);
    procedure UnloadDLL;
  public
  {-Free dynamic structures and unload DLL}
    destructor Destroy; override;
  {-Close connection to Database backend}
    procedure ConnectionClose(ConnectionHandle: TConnectionHandle);
  {-Configure a connection to database backend}
    function ConnectionConfigure(var DataSource, Options, UserName, Password: string): boolean;
  {-Open connection to Database backend}
    procedure ConnectionOpen(const DataSource, Options, UserName, Password: string;
     var ConnectionHandle: TConnectionHandle);
  {-Use Database connection to get list of fields for requested table}
    procedure GetFieldDefs(      ConnectionHandle: TConnectionHandle;
                           const TableName: string;
                                 FieldDefs: TStrings);
  {-Use Database connection to get list of table names}
    procedure GetTableNames(ConnectionHandle: TConnectionHandle;
                            TableNames: TStrings);
  {-Use Database connection to get list of table names}
    procedure InitDynamic(const FileName: string);
  {-Initialize as static Data Link}
    procedure InitStatic(ADriverClass: TDlBaseDriverClass);
  {-Close the result set}
    procedure ResultSetClose(var ResultSetHandle: TResultSetHandle);
  {-Return true if user entered/modified result set query command}
    function ResultSetConfigure(var QueryCommand: string; ConnectionHandle: TConnectionHandle; var AStruct: string): boolean;
  {-Return true if at end of result set}
    function ResultSetEOF(ResultSetHandle: TResultSetHandle): boolean;
  {-Move to the first record in the result set}
    procedure ResultSetFirst(ResultSetHandle: TResultSetHandle);
  {-Return requested fields in current result set row}
    procedure ResultSetGetRow(    ResultSetHandle: TResultSetHandle;
                                  TransferCount: longint;
                                  TransferList: PDLTransferFieldList;
                              var RowBuffer: pointer;
                              var RowBufSize: longint);
  {-Move to the next record in the result set}
    procedure ResultSetNext(ResultSetHandle: TResultSetHandle);
  {-Open result set by executing SQL command}
    procedure ResultSetOpen(      ConnectionHandle: TConnectionHandle;
                            const QueryCommand: string;
                            var   ResultSetHandle: TResultSetHandle;
                                  FieldList: TResultSetFieldList);
    //
    property DisplayName: string read FDisplayName;
    property Installed: boolean read FInstalled;
    property InternalName: string read FInternalName;
    property IsDynamicLink: boolean read GetIsDynamicLink;
    property VerMajor: integer read FVerMajor;
    property VerMinor: integer read FVerMinor;
    property Driver: TDlBaseDriver read FDriver;
  end; { TRaveDataLink }

  TRaveDataLinks = class(TObject)
  protected
    FItems: TList;
    function GetCount: integer;
    function GetItem(Index: integer): TRaveDataLink;
  public
  {-Create empty list of data links}
    constructor Create;
  {-Free list of data links}
    destructor Destroy; override;
  {-Add dynamic Data Link using requested DLL file name}
    procedure AddDynamic(const FileName: string);
  {-Add static Data Link using requested Driver class}
    procedure AddStatic(ADriverClass: TDlBaseDriverClass);
  {-Create data connection for requested type of data link}
    function CreateConnection(const InternalName: string): TDataConnection;
  {-Return Data Link matching requested internal name}
    function FindDataLink(const InternalName: string): TRaveDataLink;
    //
    property Count: integer read GetCount;
    property Items[Index: integer]: TRaveDataLink read GetItem; default;
  end; { TRaveDataLinks }

var
  GDataLinks: TRaveDataLinks = nil;
  LogDataLinkError: procedure(APIName: string; Message: string) = nil;

{-Dynamically load all data links in requested path}
  procedure AddDynamicDataLinks(Path: string);
{-Statically add Data Links registered with RegisterDataDriver}
  procedure AddStaticDataLinks;
{-Find requested data link}
  function FindDataLink(const InternalName: string): TRaveDataLink;

implementation

uses
  Windows,
  SysUtils, QDialogs;

{ Global routines }

procedure AddDynamicDataLinks(Path: string);
var
  Srch: TSearchRec;
  Code: integer;
begin
{ If global list of links is not defined then create it}
  if not Assigned(GDataLinks) then begin
    GDataLinks := TRaveDataLinks.Create;
  end; { if }
{ Be sure path has trailing backslash}
  if Path <> '' then begin
    Path := {$IFDEF LEVEL6}IncludeTrailingPathDelimiter{$ELSE}IncludeTrailingBackSlash{$ENDIF}(Path);
  end; { if }
  Path := ExpandFileName(Path);
{ Loop thru adding all DATA Link DLL files}
  Code := FindFirst(Path + '*.rvd', faAnyFile, Srch);
  while Code = 0 do begin
    GDataLinks.AddDynamic(Path+Srch.Name);
    Code := FindNext(Srch);
  end; {while}
  FindClose(Srch);
end;

procedure AddStaticDataLinks;
var
  Index: integer;
begin
{ Create global list of data links if necessary}
  if not Assigned(GDataLinks) then begin
    GDataLinks := TRaveDataLinks.Create;
  end; { if }
{ Loop thru list of driver classes registered with RegisterDriverClass}
  if Assigned(GDriverClassList) then begin
    for Index := 0 to GDriverClassList.Count-1 do begin
      GDataLinks.AddStatic(GDriverClassList[Index]);
    end; { for }
  end; { if }
end;

function FindDataLink(const InternalName: string): TRaveDataLink;
var
  Index: integer;
begin
  Result := nil;
  if Assigned(GDataLinks) then
    for Index := 0 to GDataLinks.Count - 1 do begin
      if SameText(InternalName, GDataLinks[Index].InternalName) then begin
        Result := GDataLinks[Index];
        Break;
      end; { if }
    end; { for }
end;

{ TDataConnection }

constructor TDataconnection.Create(ADataLink: TRaveDataLink);
begin
  inherited Create;
  FDataLink := ADataLink;
end;

destructor TDataConnection.Destroy;
begin
  Close;
  inherited Destroy;
end;

procedure TDataconnection.Close;
begin
  if Assigned(FConnectionHandle) then begin
    FDataLink.ConnectionClose(FConnectionHandle);
    FConnectionHandle := nil;
  end; { if }
end;

procedure TDataConnection.GetFieldDefs(const TableName: string; FieldDefs: TStrings);
begin
  FDataLink.GetfieldDefs(FConnectionHandle,TableName,FieldDefs);
end;

procedure TDataConnection.GetTableNames(TableNames: TStrings);
begin
  FDataLink.GetTableNames(FConnectionHandle,TableNames);
end;

procedure TDataConnection.Open;
begin
  FDataLink.ConnectionOpen(FDataSource,FOptions,FUserName,FPassword,FConnectionHandle);
end;

function TDataConnection.ResultSetConfigure(var QueryCommand: string; var AStruct: string): boolean;
begin
  Result := FDataLink.ResultSetConfigure(QueryCommand,FConnectionHandle,AStruct);
end;

{ TResultSetFieldList }

constructor TResultSetFieldList.Create;
begin
  inherited Create;
  FList := TList.Create;
end;

destructor TResultSetFieldList.Destroy;
begin
  Clear;
  inherited Destroy;
end;

function TResultSetFieldList.Add: TResultSetField;
begin
  Result := TResultSetField.Create;
  FList.Add(Result);
end; {TResultSetFieldList.Add}

procedure TResultSetFieldList.Clear;
var
  Index: integer;
begin
  for Index := 0 to FList.Count-1 do begin
    TResultSetField(FList[Index]).Free;
  end; { for }
  FList.Clear;
end;

function TResultSetFieldList.GetCount : integer;
begin
  Result := FList.Count;
end;

function TResultSetFieldList.GetItem(Index : integer) : TResultSetField;
begin
  Result := FList[Index];
end;

{ TDataResultSet }

constructor TDataResultSet.Create(AConnection: TDataConnection);
begin
  inherited Create;
  FConnection := AConnection;
  FResultSetHandle := nil;
  FCommandText := '';
  FFieldList := TResultSetFieldList.Create;
end;

destructor TDataResultSet.Destroy;
begin
  Close;
  FFieldList.Free;
  inherited Destroy;
end;

procedure TDataResultSet.Close;
begin
  if Assigned(FResultSetHandle) then begin
  { Ask data connection to close result set}
    FConnection.FDataLink.ResultSetClose(FResultSetHandle);
    FResultSetHandle := nil;
  { Free all fields in list}
    FFieldList.Clear;
  end; { if }
end;

function TDataResultSet.EOF: boolean;
begin
  Result := not Assigned(FResultSetHandle) or FConnection.FDataLink.ResultSetEOF(FResultSetHandle);
end;

procedure TDataResultSet.First;
begin
  if Assigned(FResultSetHandle) then begin
    FConnection.FDataLink.ResultSetFirst(FResultSetHandle);
  end; { if }
end;

function TDataResultSet.GetFieldCount : integer;
begin
  Result := FFieldList.Count;
end;

procedure TDataResultSet.GetRow(var RowBuffer: pointer; var RowBufSize: longint);
begin
  if Assigned(FResultSetHandle) then begin
    FConnection.FDataLink.ResultSetGetRow(FResultSetHandle,FFieldList.Count,FTransferList,RowBuffer,RowBufSize);
  end; { if }
end;

procedure TDataResultSet.Next;
begin
  if Assigned(FResultSetHandle) then begin
    FConnection.FDataLink.ResultSetNext(FResultSetHandle);
  end; { if }
end;

procedure TDataResultSet.Open;
begin
{ Ask the data connection to get the result set handle, record count and list of fields }
  FConnection.FDataLink.ResultSetOpen(FConnection.FConnectionHandle,CommandText,FResultSetHandle,FFieldList);
{ Allocate array of boolean values indicating which fields to transfer }
  GetMem(FTransferList,FFieldList.Count);
  FillChar(FTransferList[0],FieldCount,true); //!!! this causes all fields to be returned
end;

{ TRaveDataLink }

destructor TRaveDataLink.Destroy;
begin
  UnloadDLL;
  FreeAndNil(FDriver);
  inherited;
end;

procedure TRaveDataLink.BindProcs;
begin
  FInformationProc := FindProc('DataInformation');
  FErrorTextProc := FindProc('DataGetErrorText');
  FConnectionConfigureProc := FindProc('DataConnectionConfigure');
  FConnectionOpenProc := FindProc('DataConnectionOpen');
  FConnectionCloseProc := FindProc('DataConnectionClose');
  FGetTablesProc := FindProc('DataGetTables');
  FGetFieldsProc := FindProc('DataGetFields');
  FResultSetOpenProc := FindProc('DataResultSetOpen');
  FResultSetCloseProc := FindProc('DataResultSetClose');
  FResultSetFirstProc := FindProc('DataResultSetFirst');
  FResultSetNextProc := FindProc('DataResultSetNext');
  FResultSetEofProc := FindProc('DataResultSetEOF');
  FResultSetGetRowProc := FindProc('DataResultSetGetRow');
  FResultSetSetFilterProc := FindProc('DataResultSetSetFilter');
  FResultSetSetSortProc := FindProc('DataResultSetSetSort');
  FResultSetConfigureProc := FindProc('DataResultSetConfigure');
end;

procedure TRaveDataLink.checkInstalled;
begin
  if not FInstalled then begin
    raise Exception.Create(FDisplayName + ' is not installed.');
  end; { if }
end;

procedure TRaveDataLink.ConnectionClose(ConnectionHandle: TConnectionHandle);
var
  Params: TDLConnectionClose;
begin
  try
  { Initialize parameter record passed to API call}
    FillChar(Params,SizeOf(Params),0);
    Params.ConnectionHandle := ConnectionHandle;

  { Attempt to close the connection}
    if Assigned(Driver) then begin
      Driver.ConnectionClose(@Params)
    end else begin
      FConnectionCloseProc(@Params);
    end; { else }
    if Params.ResultCode <> 0 then begin
      LogGlobalError('ConnectionClose');
    end; { if }
  except
    on E:Exception do begin
      LogAPIError('DataConnectionOpen',E.Message);
    end;
  end; { tryx }
end;

function TRaveDataLink.ConnectionConfigure(var DataSource, Options, UserName, Password: string): boolean;
var
  Params: TDLConnectionConfigure;
begin
{ Assume no changes to connection configuration}
  Result := false;
  try
  { Initialize parameter record passed to backend driver}
    FillChar(Params,SizeOf(Params),0);
    Params.DataSource := PChar(DataSource);
    Params.Options := PChar(Options);
    Params.UserName := PChar(UserName);
    Params.Password := PChar(Password);

  { If staticly bound then call method}
    if Assigned(Driver) then begin
      Driver.ConnectionConfigure(@Params)
    end else begin
      {if dynamically bound then call DLL procedure}
      FConnectionConfigureProc(@Params);
    end; { else }

    {if result code = 1 then no error and configuration modified}
    if Params.ResultCode = 1 then begin
      Result := true;
      DataSource := StrPas(Params.DataSource);
      Options := StrPas(Params.Options);
      UserName := StrPas(Params.UserName);
      Password := StrPas(Params.Password);
    end else if Params.Resultcode <> 0 then begin { any result accept zero is an error}
      LogGlobalError('DataConnectionConfigure');
    end;

  except
    on E:Exception do
      LogAPIError('DataConnectionConfigure', E.Message);
  end; {try/except}
end; {TRaveDataLink.ConnectionConfigure}

procedure TRaveDataLink.ConnectionOpen(const DataSource       : string;
                                       const Options          : string;
                                       const UserName         : string;
                                       const Password         : string;
                                       var   ConnectionHandle : TConnectionHandle);
  {-Open connection to Database backend}
var
  Params : TDLConnectionOpen;
begin
  {raise exception if database backend is not installed}
  CheckInstalled;

  {assume we will not be able to return connection handle}
  ConnectionHandle := nil;

  try
    {initialize connection parameter record}
    FillChar(Params, SizeOf(Params), 0);
    Params.DataSource := PChar(DataSource);
    Params.Options := PChar(Options);
    Params.userName := PChar(UserName);
    Params.Password := PChar(Password);

    {attempt to open the requested connection}
    if Assigned(Driver) then begin
      Driver.ConnectionOpen(@Params)
    end else begin
      FConnectionOpenProc(@Params);
    end; { else }

    {if no error then return connection handle}
    if Params.Resultcode = 0 then
      ConnectionHandle := Params.ConnectionHandle
  else begin
    {connection handle was created so use it to log error message}
    LogConnectionError(Params.ConnectionHandle, 'DataConnectionOpen');

    {since connection handle was allocated, we need to free it}
  ConnectionClose(Params.ConnectionHandle);
end; {else}

  except
    on E:Exception do
  end; {try/except}
end; {TRaveDataLink.ConnectionOpen}

function TRaveDataLink.FindProc(const Name : string) : pointer;
  {-Attempt to find requested procedure in DLL}
begin
  Result := GetProcAddress(FLibHandle, PChar(Name));
  if not Assigned(Result) then begin
  //Log('"'+Name+'" API procedure not found in '+ExtractFileName(FFileName));
    UnloadDLL;
    Abort;
  end; {if}
end; {TRaveDataLink.FindProc}

procedure TRaveDataLink.GetFieldDefs(     ConnectionHandle : TConnectionHandle;
                                    const TableName        : string;
                                          FieldDefs        : TStrings);
  {-Use Database connection to get list of fields for requested table}
var
  Params : TDLFields;
  Index  : integer;
begin
  {raise exception if database backend is not installed}
  CheckInstalled;

  try
  {initialize get fields parameter record}
    Fillchar(Params, SizeOf(Params), 0);
    Params.ConnectionHandle := ConnectionHandle;
    Params.TableName := PChar(TableName);

    {call dll to get field definitions}
    if Assigned(Driver) then begin
      Driver.GetFields(@Params)
    end else begin
      FGetFieldsProc(@Params);
    end; { else }
    if Params.ResultCode = 0 then begin
      FieldDefs.Clear;
      for Index := 0 to Params.FieldCount-1 do
        FieldDefs.Add(StrPas(Params.FieldList[Index].FieldName)); {!!Mike, this does not use the rest of the field item record}
    end {if}
  else
    LogConnectionError(ConnectionHandle, 'DataGetFields', TableName);

    except
      on E:Exception do
        LogAPIError('DataGetFields', E.Message);
  end; {try/except}
end; {TRaveDataLink.GetFieldDefs}

function TRaveDataLink.GetIsDynamicLink : Boolean;
  {-Return true if this link is dynamically bound}
begin
  Result := not Assigned(Driver);
end; {TRaveDataLink.GetIsDynamicLink}

procedure TRaveDataLink.GetTableNames(ConnectionHandle : TConnectionHandle;
                                      TableNames       : TStrings);
  {-Use Database connection to return list of table names}
var
  Params : TDLTables;
  Index  : integer;
begin
  {raise exception if database backend is not installed}
  CheckInstalled;

  {clear the list of table names}
  TableNames.Clear;

  {initialize table information structure}
  Fillchar(Params, SizeOf(Params), 0);
  Params.ConnectionHandle := ConnectionHandle;

    try
    {call method or API procedure to get table names}
    if Assigned(Driver) then begin
      Driver.GetTables(@Params)
    end else begin
      FGetTablesProc(@Params);
    end; { else }
    if Params.ResultCode = 0 then begin
      for Index := 0 to Params.TableCount-1 do
        TableNames.Add(StrPas(Params.TableList[Index].Name))
    end {if}
  else
    LogConnectionError(ConnectionHandle, 'DataGetTables');

  except
    on E:Exception do
      LogAPIError('DataGetTableNames', E.Message);
  end; {try/except}
end; {TRaveDataLink.GetTableNames}

procedure TRaveDataLink.Information;
  {-Get data link installed state, name, title and version}
var
  Params : TDLInformation;
begin
  try
    {initialize inforamtion parameter record}
    FillChar(Params, SizeOf(Params), 0);

    {call method or API to get driver information}
    if Assigned(Driver) then begin
      Driver.Information(@Params)
    end else begin
      FInformationProc(@Params);
    end; { else }
  except
    on E:Exception do begin
      {log the very unusual but severe error}
      LogAPIError('DataInformation', E.Message);

      {this will cause the data link not to be added to GDataLinks since
       the most basic API call failed}
      Abort;
    end; {on/do}
  end; {try/except}

  {copy installed flag, major version and minor version}
  FInstalled := Params.Installed;
  FVerMajor := Params.VerMajor;
  FVerMinor := Params.VerMinor;

  {copy internal name or log and abort if not assigned}
  FInternalName := StrPas(Params.InternalName);
  if FInternalName = '' then begin
  //Log(ExtractFileName(FFileName)+': Internal name is required.');
    Abort;
  end; {if}

  {copy display name or log and abort if not assigned}
  FDisplayName := StrPas(Params.DisplayName);
  if FDisplayName = '' then begin
  //Log(ExtractFileName(FFileName)+': Display name is required.');
    Abort;
  end; {if}
end; {TRaveDataLink.Information}

procedure TRaveDataLink.LoadDLL(const FileName : string);
  {-Attempt to load DLL, logging any error}
begin
  try
    {keep track of the dll path and file name}
    FFileName := FileName;

    {attempt to load dll logging any error}
    FLibHandle := LoadLibrary(PChar(FileName));
  //if FLibHandle = 0 then
  //  Log('Error #'+IntToStr(GetLastError)+': Unable to load library ('+FileName+')');

  except
    on E:Exception do begin
      {log any exception to file}
    //Log(E.Message+' ('+FileName+')');
      FLibHandle := 0;
    end; {on/do}
  end; {try/except}

  {raise silent exception if unable to load dll}
  if FLibHandle = 0 then begin
    Abort;
  end; { if }
end; {TRaveDataLink.LoadDLL}

procedure TRaveDataLink.LogAPIError(const APIName, Message : string);
  {-Log API related error}
begin
  if Assigned(LogDataLinkError) then begin
    LogDataLinkError(APIName,Message);
  end; { if }
end;

procedure TRaveDataLink.LogConnectionError(      ConnectionHandle : TConnectionHandle;
                                           const APIName          : string;
                                           const TableName        : string = '');
  {-Log Connection related error}
var
  Params : TDLErrorText;
  S      : string;
begin
  try
    {initialize parameter record used to get error text for connection}
    FillChar(Params, SizeOf(Params), 0);
    Params.ConnectionHandle := ConnectionHandle;

    {get pointer to error text}
    if Assigned(Driver) then begin
      Driver.GetErrorText(@Params);
    end else begin
      FErrorTextproc(@Params);
    end; { else }

    {use error text to create message to log to file}
    S := StrPas(Params.ErrorText);
    if TableName <> '' then
      S := '(' + TableName + ') ' + S;

    {write message to log file}
    LogAPIError(APIName, S);

  except
    on E:Exception do
      LogAPIError('LogConnectionError', E.Message);
  end; {try/except}
end; {TRaveDataLink.LogConnectionError}

procedure TRaveDataLink.LogGlobalError(const APIName : string);
  {-Log global error}
var
  Params : TDLErrorText;
begin
  try
    {initialize parameter record used to get global error text}
    FillChar(Params, SizeOf(Params), 0);

    {get pointer to error text}
    if Assigned(Driver) then begin
      Driver.GetErrorText(@Params);
    end else begin
      FErrorTextproc(@Params);
    end; { else }

    {use error text to log message}
    LogAPIError(APIName, StrPas(Params.ErrorText));

  except
    on E:Exception do
      LogAPIError('LogGlobalError', E.Message);
  end; {try/except}
end; {TRaveDataLink.LogGlobalError}

procedure TRaveDataLink.LogResultSetError(      ResultSetHandle : TResultSetHandle;
                                          const APIName         : string);
  {-Log ResultSet related error}
var
  Params : TDLErrorText;
begin
  try
    {initialize parameter record used to get error text for connection}
    FillChar(Params, SizeOf(Params), 0);
    Params.ResultSetHandle := ResultSetHandle;

    {get pointer to error text}
    if Assigned(Driver) then begin
      Driver.GetErrorText(@Params);
    end else begin
      FErrorTextproc(@Params);
    end; { else }

    {write message to log file}
    LogAPIError(APIName, StrPas(Params.ErrorText));

  except
    on E:Exception do
      LogAPIError('LogResultSetError', E.Message);
  end; {try/except}
end; {TRaveDataLink.LogResultSetError}

procedure TRaveDataLink.InitDynamic(const FileName : string);
  {-Initialize as dynamic data link}
begin
  LoadDLL(FileName);
  BindProcs;
  Information;
end; {TRaveDataLink.InitDynamic}

procedure TRaveDataLink.InitStatic(ADriverClass : TDlBaseDriverClass);
  {-Initialize as static Data Link}
begin
  FDriver := ADriverClass.Create;
  Information;
end; {TRaveDataLink.InitStatic}

procedure TRaveDataLink.ResultSetClose(var ResultSetHandle : TResultSetHandle);
  {-Close the result set}
var
  Params : TDLResultSetClose;
begin
  try
  { Initialize parameters record }
    FillChar(Params, SizeOf(Params), 0);
    Params.ResultSetHandle := ResultSetHandle;

  { attempt to close the resultset }
    if Assigned(Driver) then begin
      Driver.ResultSetClose(@Params);
    end else begin
      FResultSetCloseProc(@Params);
    end; { else }

  { if there is an error then log global error text }
    if Params.Resultcode <> 0 then begin
      LogGlobalError('DataResultSetClose');
    end; { if }
  except
    on E: Exception do begin
      LogAPIError('ResultSetClose', E.Message);
    end;
  end; { tryx }

{ Reset the result set handle }
  ResultSetHandle := nil;
end; {TRaveDataLink.ResultSetClose}

function TRaveDataLink.ResultSetConfigure(var QueryCommand     : string;
                                              ConnectionHandle : TConnectionHandle; var AStruct: string) : Boolean;
  {-Return true if user entered/modified result set query command}
var
  Params : TDlResultSetConfigure;
begin
{ Raise exception if database backend is not installed }
  CheckInstalled;
  Result := false;

{ Initialize result set configuration information structure }
  Fillchar(Params, SizeOf(Params), 0);
  Params.ConnectionHandle := ConnectionHandle;
  Params.QueryCommand := PChar(QueryCommand);
  Params.QueryStruct := PChar(AStruct);

  try
  { Call method or API procedure to get table names }
    if Assigned(Driver) then begin
      Driver.ResultSetConfigure(@Params)
    end else begin
      FResultSetConfigureProc(@Params);
    end; { else }

  { If result code is 1 then user successfully modified query command }
    if Params.ResultCode = 1 then begin
      QueryCommand := StrPas(Params.QueryCommand);
      AStruct := StrPas(Params.QueryStruct);
      Result := true;
    end else if Params.Resultcode = 0 then begin
    { if result code is zero then no error and no change to query command }
      Result := false;
    end else begin { any other value is an error }
      Result := false;
      LogConnectionError(ConnectionHandle, 'DataGetTables');
    end; { else }
  except
    on E: Exception do begin
      LogConnectionError(Params.ConnectionHandle, 'DataResultSetConfigure', E.Message);
    end;
  end; { tryx }
end; {TRaveDataLink.ResultSetconfiguration}

function TRaveDataLink.ResultSetEOF(ResultSetHandle : TResultSetHandle) : Boolean;
  {-Return true if on last row in result set}
var
  Params : TDLResultSetEOF;
begin
  {initialize function result}
  Result := false;

  try
    {initialize parameter record}
    FillChar(Params, SizeOf(Params), 0);
    Params.ResultSetHandle := ResultSetHandle;

    {attempt to determine if at end of result set}
    if Assigned(Driver) then begin
      Driver.ResultSetEOF(@Params)
    end else begin
      FResultSetEOFProc (@Params);
    end; { else }

    {if no error then return EOF flag as function result}
    if Params.Resultcode = 0 then
      Result := Params.EOF
  else
    {log any error}
      LogResultSetError(ResultSetHandle, 'DataResultSetEOF');

  except
    on E:Exception do
      LogAPIError('DataResultSetEOF', E.Message);
  end; {try/except}
end; {TRaveDataLink.ResultSetEOF}

procedure TRaveDataLink.ResultSetFirst(ResultSetHandle : TResultSetHandle);
  {-Move to the first record in the result set}
var
  Params : TDLResultSetFirst;
begin
  try
    {initialize parameter record}
    FillChar(Params, SizeOf(Params), 0);
    Params.ResultSetHandle := ResultSetHandle;

    {attempt to position on first row in result set}
    if Assigned(Driver) then begin
      Driver.ResultSetFirst(@Params)
    end else begin
      FResultSetFirstProc(@Params);
    end; { else }

    {log any error}
    if Params.Resultcode <> 0 then
      LogResultSetError(ResultSetHandle, 'DataResultSetFirst');

  except
    on E:Exception do
      LogAPIError('DataResultSetFirst', E.Message);
  end; {try/except}
end; {TRaveDataLink.ResultSetFirst}

procedure TRaveDataLink.ResultSetGetRow(    ResultSetHandle : TResultSetHandle;
                                            TransferCount   : longint;
                                            TransferList    : PDLTransferFieldList;
                                        var RowBuffer       : Pointer;
                                        var RowBufSize      : longint);
  {-Return requested fields in current result set row}
var
  Params : TDLResultSetRow;
begin
  try
    {initialize parameter record}
    FillChar(Params, SizeOf(Params), 0);
    Params.ResultSetHandle := ResultSetHandle;
    Params.TransferCount := TransferCount;
    Params.TransferList := TransferList;

    {attempt to get requested fields for current row}
    if Assigned(Driver) then begin
      Driver.ResultSetGetRow(@Params)
    end else begin
      FResultSetGetRowProc(@Params);
    end; { else }

    {return pointer to row buffer and size of buffer}
    if Params.Resultcode = 0 then begin
      RowBuffer := Params.RowBuffer;
      RowBufSize := Params.RowBufSize;
    end
    else
      {if any error then log it}
      LogResultSetError(ResultSetHandle, 'DataResultSetGetRow');

  except
    on E:Exception do
      LogAPIError('DataResultSetGetRow', E.Message);
  end; {try/except}
end; {TRaveDataLink.ResultSetGetRow}

procedure TRaveDataLink.ResultSetNext(ResultSetHandle : TResultSetHandle);
  {-Move to the next record in the result set}
var
  Params : TDLResultSetNext;
begin
  try
    {initialize parameter record}
    FillChar(Params, SizeOf(Params), 0);
    Params.ResultSetHandle := ResultSetHandle;

    {attempt to position to next row in result set}
    if Assigned(Driver) then begin
      Driver.ResultSetNext(@Params)
    end else begin
      FResultSetNextProc(@Params);
    end; { else }

    {log any error}
    if Params.Resultcode <> 0 then
      LogResultSetError(ResultSetHandle, 'DataResultSetNext');

  except
    on E:Exception do
      LogAPIError('DataResultSetNext', E.Message);
  end; {try/except}
end; {TRaveDataLink.ResultSetNext}

procedure TRaveDataLink.ResultSetOpen(      ConnectionHandle : TConnectionHandle;
                                      const QueryCommand     : string;
                                      var   ResultSetHandle  : TResultSetHandle;
                                         FieldList          : TResultSetFieldList);
  {-Open result set by executing SQL command}
var
  Params : TDLResultSetOpen;
  Index  : integer;
begin
  {initialize result variables}
  ResultSetHandle := nil;

  try
    {initialize parameter record structure}
    FillChar(Params, SizeOf(Params), 0);
    Params.ConnectionHandle := ConnectionHandle;
    Params.QueryCommand := StrNew(PChar(QueryCommand));

    {attempt to open the result set}
    if Assigned(Driver) then begin
      FDriver.ResultSetOpen(@Params)
    end else begin
      FResultSetOpenProc(@Params);
    end; { else }

    {if no error then return handle and list of fields}
    if Params.ResultCode =0 then begin
      ResultSetHandle := Params.ResultSetHandle;

      {loop thru adding all fields passed back to result set field list}
      FieldList.Clear;
      for Index := 0 to Params.FieldCount-1 do
        with FieldList.Add do begin
          FieldName := StrPas(Params.FieldList[Index].FieldName);
          DataType := TRpDataType(Params.FieldList[Index].DataType);
          Width := Params.FieldList[Index].Width;
          if Assigned(Params.FieldList[Index].FullName) then
            FullName := StrPas(Params.FieldList[Index].FullName);
          if Assigned(Params.FieldList[Index].Description) then
            Description := StrPas(Params.FieldList[Index].Description);

        end; {with}
  end {if}
  else begin
    {log error and then close the result set handle}
    LogResultSetError(Params.ResultSetHandle, 'DataResultSetOpen');
    ResultSetClose(Params.ResultSetHandle);
end; {else}

  except
    on E:Exception do
      LogAPIError('DataResultSetOpen', E.Message);
  end; {try/except}
end; {TRaveDataLink.ResultSetOpen}

procedure TRaveDataLink.UnloadDLL;
  {-Unload the DLL}
begin
  if FLibHandle <> 0 then begin
    FreeLibrary(FLibHandle);
    FLibHandle := 0;
  end; {if}
end; {TRaveDataLink.UnloadDLL}

{TRaveDataLinks}

constructor TRaveDataLinks.Create;
  {-Create empty list of Data Link objects}
begin
  inherited Create;
  FItems := TList.Create;
end; {TRaveDataLinks.Create}

destructor TRaveDataLinks.Destroy;
  {-Unload all libraries and deallocate memory}
var
  Index : integer;
begin
  for Index := 0 to FItems.Count-1 do begin
    TRaveDataLink(FItems[Index]).Free;
  end; { for }
  FItems.Free;
  inherited Destroy;
end; {TRaveDataLinks.Destroy}

procedure TRaveDataLinks.AddDynamic(const FileName : string);
  {-Add dynamic Data Link using requested file name}
var
  Index : integer;
  Item  : TRaveDataLink;
begin
  try
    {create data link by loading DLL and performing initialization}
    Item := TRaveDataLink.Create;
    Item.initDynamic(FileName);

    {loop thru ensuring no duplicate internal or display names}
    for Index := 0 to Count-1 do
      if SameText(Item.InternalName, Items[Index].InternalName) then begin
        {if both objects are dynamic then write to log file}
      //if not Assigned(Items[Index].FDriver) then
      //  Log('Duplicate DataLink Internal Name: '+Item.InternalName);
        Item.Free;
        Exit;
    end
    else if SameText(Item.DisplayName, Items[Index].DisplayName) then begin
    //Log('Duplicate DataLink DisplayName: '+Item.DisplayName);
      Item.Free;
      Exit;
  end;

    {add the new data link to the list}
    FItems.Add(Item);

  except
    {Throw exception away as error message is already written to log}
  end; {try/except}
end; {TRaveDataLinks.Add}

procedure TRaveDataLinks.AddStatic(ADriverClass : TDlBaseDriverClass);
  {-Add static Data Link using requested Driver class}
var
  Item : TRaveDataLink;
  I1: integer;
begin
  Item := TRaveDataLink.Create;
  Item.InitStatic(ADriverClass);
  for I1 := 0 to FItems.Count - 1 do begin
    if AnsiSameText(TRaveDataLink(FItems[I1]).InternalName,Item.InternalName) then begin
      Item.Free;
      Exit;
    end; { if }
  end; { for }
  FItems.Add(Item);
end;

function TRaveDataLinks.CreateConnection(const InternalName : string) : TDataConnection;
  {-Create data connection for requested type of data link}
var
  Index : integer;
begin
  {loop thru looking for matching data link internal name}
  for Index := 0 to Count-1 do
    {if match found then create data connection and exit}
    if InternalName = Items[Index].InternalName then begin
      Result := TDataConnection.Create(Items[Index]);
      Exit;
    end;

  {if we get here then we attempted to create a connection to a DATA link
   which is not loaded}
  raise Exception.Create('Unknown DATA link type. (' + InternalName + ')');
end; {TRaveDataLinks.CreateConnection}

function TRaveDataLinks.FindDataLink(const InternalName : string) : TRaveDataLink;
  {-Return Data Link matching internal name}
var
  Index : integer;
begin
  {loop thru looking for matching data link internal name}
  for Index := 0 to Count-1 do
    {if match found then create data connection and exit}
    if InternalName = Items[Index].InternalName then begin
      Result := Items[Index];
      Exit;
    end;

  {if we get here then we attempted to use a data link that is not installed}
  raise Exception.Create('Unknown DATA link type. (' + InternalName + ')');
end; {TRaveDataLinks.FindDataLink}

function TRaveDataLinks.GetCount : integer;
begin
  Result := FItems.Count;
end; {TRaveDataLinks.GetCount}

function TRaveDataLinks.GetItem(Index : integer) : TRaveDataLink;
begin
  Result := TRaveDataLink(FItems[Index]);
end; {TRaveDataLinks.GetItem}

initialization
finalization
  FreeAndNil(GDataLinks);
end.