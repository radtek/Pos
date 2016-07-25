{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvDLDBX;
{ Implements classes used to link to Db Express }

interface

uses
  SysUtils, Classes, Controls, RvDLCommon, RvDLBase, RvDLDataSet,
  DB, SqlExpr;

type
  TDLDBXDriver = class(TDLDataSetDriver)
  public
    function CreateConnection: TDLBaseConnection; override;
    function CreateResultSet(AConnection: TDLBaseConnection): TDLBaseResultSet; override;
    procedure Information(Params: PDLInformation); override;
  end; { TDLDBXDriver }

  TDLDBXConnection = class(TDLDataSetConnection)
  protected
    FSQLConnection : TSQLConnection;
  public
    procedure Connect(DataSource, UserName, Password: string; OptionList: TStringList); override;
    procedure Disconnect; override;
    procedure GetTableNames(List : TStrings); override;
    //
    property SQLConnection: TSQLConnection read FSQLConnection write FSQLConnection;
  end; { TDLDBXConnection }

  TDLDBXResultSet = class(TDLDataSetResultSet)
  protected
    function OpenDataSet(QueryStr: string): TDataSet; override;
  end; { TDLDBXResultSet }

implementation

uses
  Forms, DbConnAdmin;

const
  csConnectionName = 'ConnectionName';
  csDatabaseName = 'DatabaseName';
  csDriverName = 'DriverName';
  csLibraryName = 'LibraryName';
  csVendorLib = 'VendorLib';
  csGetDriverFunc = 'GetDriverFunc';
  csLoadParamsOnConnect = 'LoadParamsOnConnect';
  csPassword = 'password';
  csUserName = 'User_Name';

type
  TDLDbxConnectionInfo = class(TObject)
  protected
    FConnectionName: string;
    FDriverName: string;
    FLibraryName: string;
    FVendorLib: string;
    FGetDriverFunc: string;
    FLoadParamsOnConnect: Boolean;
    FParams: TStrings;
    FAdmin: TConnectionAdmin;
    function GetDatabaseName: string;
    function GetDataSource: string;
    function GetOptions: string;
    function GetPassword: string;
    function GetUserName: string;
    procedure SetConnectionName(Value: string);
    procedure SetDatabaseName(Value: string);
    procedure SetDataSource(Value: string);
    procedure SetDriverName(Value: string);
    procedure SetOptions(Value: string);
    procedure SetPassword(Value: string);
    procedure SetUserName(Value: string);
  public
  { Create connection information object }
    constructor Create;
  { Free all dynamic structures }
    destructor Destroy; override;

  { Rave related properties }
    property DataSource: string read GetDataSource write SetDataSource;
    property Options: string read GetOptions write SetOptions;
    property Password: string read GetPassword write SetPassword;
    property UserName: string read GetUserName write SetUserName;

  { Db Express related properties }
    property ConnectionName: string read FConnectionName write SetConnectionName;
    property DatabaseName: string read GetDatabaseName write SetDatabaseName;
    property DriverName: string read FDriverName write SetDriverName;
    property GetDriverFunc: string read FGetDriverFunc write FGetDriverFunc;
    property LibraryName: string read FLibraryName write FLibraryName;
    property LoadParamsOnConnect: boolean read FLoadParamsOnConnect write FLoadParamsOnConnect;
    property Params: TStrings read FParams;
    property VendorLib: string read FVendorLib write FVendorLib;
  end; { TDLDbxConnectionInfo }

{ Create an object used to minipulate connection and driver information }

constructor TDLDbxConnectionInfo.Create;
begin
  inherited;
  FParams := TStringList.Create;
  FAdmin := TConnectionAdmin.Create;
end;

{ Free all dynamic structures }

destructor TDLDbxConnectionInfo.Destroy;
begin
  FParams.Free;
  FAdmin.Free;
  inherited;
end;

{ Return name of database }

function TDLDbxConnectionInfo.GetDatabaseName: string;
begin
  Result := FParams.Values[csDatabaseName];
end;

{ Return string stored in Rave report }

function TDLDbxConnectionInfo.GetDataSource: string;
var
  List: TStrings;
begin
{ create temporary string list }
  List := TStringList.Create;
  try
    if FConnectionName <> '' then begin
    { if connection name given }
      List.Values[csConnectionName] := FConnectionName;
    end else begin
    { driver name as given }
      List.Values[csDriverName] := FDriverName;
    end; { else }

  { add remaining parameters to temporary string list }
    if not FLoadParamsOnConnect then begin
      List.Values[csDriverName] := FDriverName;
      List.Values[csLibraryName] := FLibraryName;
      List.VAlues[csVendorLib] := VendorLib;
      List.Values[csGetDriverFunc] := FGetDriverFunc;
      List.AddStrings(FParams);
    end; { if }

  { blank out user name and password they are stored seperately in rave }
    List.Values[csUserName] := '';
    List.Values[csPassword] := '';

  { determine if we want to load parameters on connection }
    List.Values[csLoadParamsOnConnect] := BoolToStr(FLoadParamsOnConnect, true);

  { return the full data source }
    Result := List.Text;

  finally
  { free the temporary string list }
    List.Free;
  end; { tryf }
end;

function TDLDbxConnectionInfo.GetOptions: string;
begin
  Result := '';
end;

{ Return password }

function TDLDbxConnectionInfo.GetPassword : string;
begin
  Result := FParams.Values[csPassword];
end;

{ Return user name }

function TDLDbxConnectionInfo.GetUserName: string;
begin
  Result := Params.Values[csUserName];
end;

{ Assign connection name and initialize related properties }

procedure TDLDbxConnectionInfo.SetConnectionName(Value : string);
var
  List: TStrings;
begin
{ exit if the connection name is the same }
  if CompareText(FConnectionName, Value) = 0 then begin
    Exit;
  end; { if }
{ raise exception if connection name is undefined }
  if not FAdmin.ConnectionConfig.SectionExists(Value) then begin
    raise Exception.Create('Specified named connection is undefined. ('+Value+')');
  end; { if }
{ create string list to hold section values }
  List := TStringList.Create;
  try
  { read connection parameters from connection file }
    FAdmin.ConnectionConfig.ReadSectionValues(Value, List);
  { get driver name }
    DriverName := List.Values[csDriverName];
  finally
  { free temporary string list }
    List.Free;
  end; { tryf }
{ assign the connection name }
  FConnectionName := Value;
end;

{ Assign the database name }

procedure TDLDbxConnectionInfo.SetDatabaseName(Value : string);
begin
  FParams.Values[csDatabaseName] := Value;
end;

{ Parse data source into individual values }

procedure TDLDbxConnectionInfo.SetDataSource(Value: string);
var
  List : TStrings;

{ Extract string value from list }

  function ExtractValue(const Name : string) : string;
  begin
    Result := List.Values[Name];
    List.Values[Name] := '';
  end;

begin
  List := TStringList.Create;
  try
  { break data source into name/value pairs }
    List.Text := Value;

  { extract load parameters on connect flag }
    FLoadParamsOnConnect := StrToBool(ExtractValue(csLoadParamsOnConnect));

  { if loading parameters on connection }
    if FLoadParamsOnConnect then begin
    { this assignment will cause proper values to be initialized }
      SetConnectionName(ExtractValue(csConnectionName));
    end else begin
    { use connection and driver names inside data source string }
      FConnectionName := ExtractValue(csConnectionName);
      FDriverName := ExtractValue(csDriverName);
      FLibraryName := ExtractValue(csLibraryName);
      FVendorLib := ExtractValue(csVendorLib);
      FGetDriverFunc := ExtractValue(csGetDriverFunc);
      FParams.Assign(List);
    end;

  finally
  { free the temporary list }
    List.Free;
  end; { tryf }
end;

{ Set driver name }

procedure TDLDbxConnectionInfo.SetDriverName(Value : string);
var
  List : TStrings;
begin
  if CompareText(FDriverName, Value) = 0 then begin
    if Value = '' then begin
      raise Exception.Create('A driver name is required.');
    end else begin
      Exit;
    end;
  end;

{ create temporary string list }
  List := TStringList.Create;
  try
  { read section values for requested driver }
    FAdmin.DriverConfig.ReadSectionValues(Value, List);

  { get library name, driver name and driver functioon name }
    FLibraryName := List.Values['LibraryName'];
        FVendorLib := List.Values['VendorLib'];
        FGetDriverFunc := List.Values['GetDriverFunc'];

  { blank out library driver and function names }
    List.Values['LibraryName'] := '';
        List.Values['VendorLib'] := '';
        List.Values['GetDriverFunc'] := '';

        { assign remaining values as parameters }
          FParams.Assign(List);

  finally
  { free the temporary list }
          List.Free;
        end; { tryf }

   { keep track of driver name }
     FDriverName := Value;
end;

procedure TDLDbxConnectionInfo.SetOptions(Value: string);
begin
{ no values are stored in options, this is just for compatibility with Rave }
end;

{ Assign login password }

procedure TDLDbxConnectionInfo.SetPassword(Value: string);
begin
  FParams.Values['password'] := Value;
end;

{ Assign user name }

procedure TDLDbxConnectionInfo.SetUserName(Value: string);
begin
  FParams.Values[csUserName] := Value;
end;

{ Create global list of field definitions for requested table }
(*
procedure CreateGlobalFieldList(Connection : TSqlConnection; TableName : string);
var
  Dataset       : TCustomSqlDataset;
  NameField     : TField;
  DataTypeField : TField;
  SizeField     : TField;
  FieldCount    : integer;
  fieldIndex    : integer;
  FieldName     : string;
  DataType      : integer;
  Size          : integer;

{ Get correct capitalization of table name }

  procedure CheckTableName;
  var
    Names : TStringList;
    Index : integer;
  begin
  { create list to hold table names }
    Names := TStringList.Create;
    try
    { make the list unique and case insensitive }
      Names.CaseSensitive := false;
      Names.Duplicates := dupIgnore;

    { get correctly capitalized table names from schema }
      Connection.GetTableNames(Names);

    { raise exception if unable to find table name }
      Index := Names.IndexOf(TableName);
      if Index = -1 then begin
        raise Exception.Create('Unable to get correct capitalization for table name. ('+TableName+')')
      end else begin
        TableName := Names[Index];
      end; { else }

    finally
    { free the list of names }
      Names.Free;
    end; { tryf }
  end;

{ Initialize field object variable }

  function CheckfieldDefined(Name : string) : Tfield;
  begin
    result := Dataset.FieldByName(Name);
    if not Assigned(Result) then begin
      raise Exception.Create('Schema field is not defined. ('+Name+')');
    end; { if }
  end;

begin
{ initialize dynamic variables }
  Dataset := nil;

{ get the properly capitalized table name }
  CheckTableName;

  try
  { create dataset used to retrieve table schema }
    Dataset := TCustomSqlDataset.Create(nil);
    Dataset.SqlConnection := Connection;
    Dataset.SetSchemaInfo(stColumns, TableName, '');
    Dataset.Active := true;

  { determine number of fields defined in rows }
    FieldCount := 0;
    Dataset.First;
    while not Dataset.EOF do begin
      Inc(fieldcount);
      Dataset.Next;
    end; { while }

  { this error should never occur as code above should ensure table exists }
    if FieldCount = 0 then begin
      raise Exception.Create('Table contains no fields! ('+TableName+')');
    end; { if }

  { allocate global list of field item records }
    AllocGlobalFieldList(Fieldcount);

  { move to first record and get pointers to name, data type and size field objects }
    Dataset.First;
    NameField := CheckFieldDefined('COLUMN_NAME');
    DataTypeField := CheckFieldDefined('COLUMN_DATATYPE');
    SizeField := CheckFieldDefined('COLUMN_LENGTH');

  { loop thru all rows in schema table }
    FieldIndex := 0;
    while not Dataset.EOF do begin
    { get field name, data type and size }
      FieldName := NameField.AsString;
      DataType := XlateFieldtype(DataTypeMap[DataTypeField.AsInteger]);
      Size := SizeField.AsInteger;

    { initialize record structure in global field list }
      SetfieldItem(GFieldList, fieldIndex, PChar(FieldName),
        DataType, Size, nil, nil);

    { move to next record which is next field definition }
      Inc(FieldIndex);
      Dataset.Next;
    end; { while }

  finally
  { free all dynamic variables }
    Dataset.Free;
  end; { tryf }
end;
*)
{ TDLDBXDriver }

{ Create BDE connection object }

function TDLDBXDriver.CreateConnection: TDLBaseConnection;
begin
  Result := TDLDBXConnection.Create;
end;

{ Create BDE Result Set object }

function TDLDBXDriver.CreateResultSet(AConnection: TDLBaseConnection): TDLBaseResultSet;
begin
  Result := TDLDBXResultSet.Create(AConnection);
end;

{ Return installed state, version and driver names }

procedure TDLDBXDriver.Information(Params : PDLInformation);
begin
  StrPCopy(Params.InternalName, 'DBX');
  StrPCopy(Params.DisplayName, 'DB Express');
  Params.VerMajor := 2;
  Params.VerMinor := 0;
  Params.Installed := true;
end;

{ Return list of field definitions }

(*
procedure TDLDbxDriver.GetFields(Params : PDLFields);
var
  Connection : TDLDbxConnection;
begin
{ get convenient pointer to connection }
  Connection := Params.ConnectionHandle;
  try
  { use schema for requested table to create list of field definitions }
    CreateGlobalFieldList(Connection.FSQLConnection, Params.TableName);

  { return field list pointer, count and zero result }
    Params.FieldList := GFieldList;
    Params.FieldCount := GFieldCount;
    Params.ResultCode := 0;

  except
  { return any exception as an error }
    on E:Exception do begin
      Params.Resultcode := -1;
      Connection.ErrorText := E.Message;
    end;
  end; { tryx }
end;
*)

{ TDLDBXConnection }

{ Atempt to make connection }

procedure TDLDBXConnection.Connect(DataSource, UserName, Password: string; OptionList: TStringList);
var
  ConnectionInfo: TDLDbxConnectionInfo;
begin
{ create connection information translation object }
  ConnectionInfo := TDLDbxConnectionInfo.Create;
  try
  { assign the data source which is parsed into appropriate parts }
    ConnectionInfo.DataSource := DataSource;

  { create a SQL connection object }
    FSQLConnection := TSQLConnection.Create(nil);

  { prevent login prompt and set load parameters on connect flag }
    FSQLConnection.LoginPrompt := false;
    FSQLConnection.LoadParamsOnConnect := ConnectionInfo.LoadParamsOnConnect;

  { if using connection name and loading from ini file }
    if ConnectionInfo.LoadParamsOnConnect then begin
      FSqlConnection.ConnectionName := ConnectionInfo.ConnectionName;
    end else begin
  { assign values stored in data source string }
      FSqlConnection.DriverName := ConnectionInfo.DriverName;
      FSqlConnection.LibraryName := ConnectionInfo.LibraryName;
      FSqlConnection.VendorLib := ConnectionInfo.VendorLib;
      FSqlConnection.GetDriverFunc := ConnectionInfo.GetDriverFunc;
      FSqlConnection.Params.Assign(ConnectionINfo.Params);
    end; { else }

  { set user name and password }
    FSqlConnection.Params.Values[csUserName] := UserName;
    FSqlConnection.Params.Values[csPassword] := Password;
  finally
  { free the connection info }
    ConnectionInfo.Free;
  end; { tryf }

{ now attempt to connect }
  FSQLConnection.Connected := true;
end;

{ Attempt to disconnect from session and database }

procedure TDLDBXConnection.Disconnect;
begin
  FreeAndNil(FSQLConnection);
end;

{ Use Session object to get list of table names }

procedure TDLDBXConnection.GetTableNames(List : TStrings);
begin
  FSQLConnection.GetTableNames(List);
end;

{ TDLDBXResultSet }

{ Generate TSQLQuery result set }

function TDLDBXResultSet.OpenDataSet(QueryStr: string): TDataSet;
begin
  Result := TSQLQuery.Create(nil);
  try
    with Result as TSQLQuery do begin
      SqlConnection := TDLDBXConnection(self.Connection).FSQLConnection;
      SQL.Text := QueryStr;
      Open;
    end; { with }
  except
    Result.Free;
    raise;
  end; { tryx }
end;

initialization
  RegisterDriverClass(TDLDBXDriver);
end.