{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRvDLBase;

interface

uses
  Windows,
  QForms, SysUtils, Classes, QRvDLCommon;

{ Global Table and Field lists used in Rave Designer }

{ Use string list to create global list of table names }
  procedure CreateGlobalTableList(TableNames: TStrings);
{ Deallocate global list of table item records }
  procedure FreeGlobalTableList;
{ Allocate empty global list of table item records }
  procedure AllocGlobalTableList(Count: longint);
{ Assign values to the requested global table item }
  procedure SetGlobalTableItem(Index: longint; TableName: string);
{ Make dynamic copy of error text in global variable }
  procedure SetGlobalErrorText(ErrorText: string);

type
  TDLBaseConnection = class;
  TDLBaseResultSet = class;

  TDLFieldListManager = class
  protected
    FFieldCount: longint;
    FFieldList: PDLFieldList;
  public
    destructor Destroy; override;
    procedure AllocFieldList(Count: longint);
    procedure SetFieldItem(Index: longint; FieldName: string; DataType: byte; Width: longint;
     FullName: string; Description: string);

    property FieldCount: longint read FFieldCount;
    property FieldList: PDLFieldList read FFieldList;
  end; { TDLFieldList }

  TDLConfigForm = class(TForm)
  protected
    procedure TestConnection(TableList: TStrings);
  public
    procedure SetData(DataSource, UserName, Password: string; OptionList: TStringList); virtual; abstract;
    procedure GetData(var DataSource, UserName, Password: string; OptionList: TStringList); virtual; abstract;
  end;
  TDLConfigFormClass = class of TDLConfigForm;

  TDLBaseDriver = class(TObject)
  protected
  { following fields used in connection configuration API call }
    FDataSource: string;
    FUserName: string;
    FPassword: string;
    FOptionList: TStringList;
    FOptionText: string; // Used for storage of FOptionList.CommaText for DLL calls
    FFields: TDLFieldListManager;
  public
    constructor Create; virtual;
    destructor Destroy; override;

  { Descendant classes must replace all of the following methods }
    procedure ConnectionClose(Params: PDLConnectionClose); virtual;
    procedure ConnectionConfigure(Params: PDLConnectionConfigure); virtual;
    procedure ConnectionOpen(Params: PDLConnectionOpen); virtual;
    procedure GetErrorText(Params: PDLErrorText); virtual;
    procedure GetFields(Params: PDLFields); virtual;
    procedure GetTables(Params: PDLTables); virtual;
    procedure Information(Params: PDLInformation); virtual; abstract;
    procedure ResultSetClose(Params: PDLResultSetClose); virtual;
    procedure ResultSetConfigure(Params: PDLResultSetConfigure); virtual;
    procedure ResultSetEOF(Params: PDLResultSetEOF); virtual;
    procedure ResultSetFirst(Params: PDLResultSetFirst); virtual;
    procedure ResultSetGetRow(Params: PDLResultSetRow); virtual; abstract;
    procedure ResultSetNext(Params: PDLResultSetNext); virtual;
    procedure ResultSetOpen(Params: PDLResultSetOpen); virtual;
    procedure ResultSetSetFilter(Params: PDLResultSetFilter); virtual;
    procedure ResultSetSetSort(Params: PDLResultSetSort); virtual;

  { Overridden by descendants to create an appropriate connection object }
    function CreateConnection: TDLBaseConnection; virtual; abstract;
  { Overridden by descendants to create an appropriate result set object }
    function CreateResultSet(AConnection: TDLBaseConnection): TDLBaseResultSet; virtual; abstract;

    property DataSource: string read FDataSource;
    property UserName: string read FUserName;
    property Password: string read FPassword;
    property OptionList: TStringList read FOptionList;
    property Fields: TDLFieldListManager read FFields;
  end; { TDLBaseDriver }

  TDLBaseDriverClass = class of TDLBaseDriver;

  TDLBaseConnection = class(TObject)
  protected
    FDriver: TDLBaseDriver;
    FErrorText: string;
  public
    constructor Create; virtual;

    procedure Connect(DataSource, UserName, Password: string; OptionList: TStringList); virtual; abstract;
    procedure Disconnect; virtual; abstract;
    procedure GetTableNames(List: TStrings); virtual; abstract;
    procedure GetFields(TableName: string); virtual;

    property Driver: TDLBaseDriver read FDriver;
    property ErrorText: string read FErrorText write FErrorText;
  end; { TDLBaseConnection }

  TDLBaseResultSet = class(TObject)
  protected
    FConnection: TDLBaseConnection;
    FErrorText: string;
    FBuffer: TDLBuffer;
    FFields: TDLFieldListManager;
  public
    constructor Create(AConnection: TDLBaseConnection); virtual;
    destructor Destroy; override;
    procedure Open(QueryStr: string); virtual; abstract;
    procedure GetFields(AFields: TDLFieldListManager); virtual; abstract;
    procedure Close; virtual; abstract;
    procedure First; virtual; abstract;
    procedure Next; virtual; abstract;
    function Eof: boolean; virtual; abstract;
    //
    property Connection: TDLBaseConnection read FConnection;
    property ErrorText: string read FErrorText write FErrorText;
    property Fields: TDLFieldListManager read FFields;
  end; { TDLBaseResultSet }

{ Register class with list of Data Driver classes }
  procedure RegisterDriverClass(ADriverClass: TDLBaseDriverClass);

{ Data Link API Routines to Export }

{ Close connection to Database backend }
  procedure DataConnectionClose(Params: PDLConnectionClose);
{ Configure paramaters for connection to backend database }
  procedure DataConnectionConfigure(Params: PDLConnectionConfigure);
{ Open connection to Database backend }
  procedure DataConnectionOpen(Params: PDLConnectionOpen);
{ Return pointer to last error text }
  procedure DataGetErrorText(Params: PDLErrorText);
{ Return list of field definitions for requested table }
  procedure DataGetFields(Params: PDLFields);
{ Return count/list of tables for requested database }
  procedure DataGetTables(Params: PDLTables);
{ Return information about Data Driver }
  procedure DataInformation(Params: PDLInformation);
{ Close the result set }
  procedure DataResultSetClose(Params: PDLResultSetClose);
{ Return true if at end of result set }
  procedure DataResultSetEOF(Params: PDLResultSetEOF);
{ Move to the first record in the result set }
  procedure DataResultSetFirst(Params: PDLResultSetFirst);
{ Return data for current row in result set }
  procedure DataResultSetGetRow(Params: PDLResultSetRow);
{ Move to the next record in the result set }
  procedure DataResultSetNext(Params: PDLResultSetNext);
{ Open result set }
  procedure DataResultSetOpen(Params: PDLResultSetOpen);
{ Set Filter on Result Set }
  procedure DataResultSetSetFilter(Params: PDLResultSetFilter);
{ Set Sort on Result Set }
  procedure DataResultSetSetSort(Params: PDLResultSetSort);
{ Configure result set }
  procedure DataResultSetConfigure(Params: PDLResultSetConfigure);

var
  ConnectionConfigureForm: TDLConfigFormClass;
  SQLEditorName: string = 'SQLEditor.dll';
  GDriverClassList: TList = nil;

implementation

uses
  QDialogs, QControls;

var
  GTableCount: longint = 0;
  GTableList: PDLTableList = nil;
  GErrorText: PChar = nil;

var
  ConfigureQueryProc: function(AParams: PDLQueryEditor): boolean;
  QueryEditorLib: HMODULE = 0;


{ this variable is used by the Data Link API procedures }
var
  GDriver: TDLBaseDriver = nil;

{ Use string list to create global list of table names }

procedure CreateGlobalTableList(TableNames: TStrings);
var
  Index: integer;
begin
  AllocGlobalTableList(TableNames.Count);
  for Index := 0 to TableNames.Count - 1 do begin
    SetGlobalTableItem(Index,TableNames[Index]);
  end; { for }
end;

{ Deallocate global list of table names }

procedure FreeGlobalTableList;
var
  Index: integer;
begin
{ loop thru deallocating table names }
  for Index := 0 to GTableCount-1 do begin
    StrDispose(GTableList[Index].Name);
  end; { for }
  FreeMem(GTableList, GTableCount * SizeOf(TDLTableItem));
  GTableCount := 0;
  GTableList := nil;
end;

{ Allocate empty global list of table item records }

procedure AllocGlobalTableList(Count: longint);
begin
{ free any existing list of tables }
  FreeGlobalTableList;

{ assign table limit and allocate array of table records }
  GTableCount := Count;
  GetMem(GTableList, GTableCount * SizeOf(TDLTableItem));
end;

{ Assign values to requested global table item record }

procedure SetGlobalTableItem(Index: longint; TableName: string);
begin
{ if DLL written property index should always be within bounds 0 - GTableCount-1 }
  if Index < GTableCount then begin
    GTableList[Index].Name := StrNew(@TableName[1]);
  end; { if }
end;

{ Make dynamic copy of error text in global variable }

procedure SetGlobalErrorText(ErrorText: string);
begin
  StrDispose(GErrorText);
  if ErrorText <> '' then begin
    GErrorText := StrNew(@ErrorText[1]);
  end; { if }
end;

{ TDLFieldListManager }

procedure TDLFieldListManager.AllocFieldList(Count: Integer);
var
  Index: integer;
begin
  if Assigned(FieldList) then begin
    for Index := 0 to FieldCount - 1 do begin
      with FieldList[Index] do begin
        StrDispose(FieldName);
        StrDispose(FullName);
        StrDispose(Description);
      end; { with }
    end; { for }
    FreeMem(FieldList,FieldCount * SizeOf(TDLFieldItem));
    FFieldList := nil;
  end; { if }
  FFieldCount := Count;
  if FieldCount > 0 then begin
    GetMem(FFieldList,FieldCount * SizeOf(TDLFieldItem));
  end; { if }
end;

destructor TDLFieldListManager.Destroy;
begin
  AllocFieldList(0);
  inherited;
end;

procedure TDLFieldListManager.SetFieldItem(Index: Integer; FieldName: string; DataType: byte; Width: Integer;
 FullName, Description: string);
begin
{ fill the field definition record with zeros }
  FillChar(FieldList^[Index], SizeOf(FieldList^[Index]), 0);

{ initialize field name, data type and width }
  FieldList[Index].FieldName := StrNew(@FieldName[1]);
  FieldList[Index].Datatype := Datatype;
  FieldList[Index].Width := Width;

{ if field name and full name are different then copy full name }
  if CompareText(FieldName,FullName) <> 0 then begin
    FieldList[Index].FullName := StrNew(@FullName[1]);
  end; { if }

{ copy field description }
  FieldList[Index].Description := StrNew(@Description[1]);
end;

{ TDLConfigForm }

procedure TDLConfigForm.TestConnection(TableList: TStrings);
var
  Connection: TDLBaseConnection;
  OptionList: TStringList;
  DataSource, UserName, Password: string;
begin
  OptionList := TStringList.Create;
  TableList.Clear;
  try
    Connection := GDriver.CreateConnection;
    Screen.Cursor := crSQLWait;
    try
      GetData(DataSource,UserName,Password,OptionList);
      Connection.Connect(DataSource,UserName,Password,OptionList);
      Connection.GetTableNames(TableList);
      Connection.Disconnect;
    finally
      Screen.Cursor := crDefault;
      OptionList.Free;
      Connection.Free;
    end; { tryf }
    ShowMessage('Connection successful!');
  except
    on E: Exception do begin
      ShowMessage('Connection error: ' + E.ClassName + ' - ' + E.Message);
    end;
  end; { tryx }
end;

{ TDLBaseDriver }

{ Close connection to database backend }

procedure TDLBaseDriver.ConnectionClose(Params: PDLConnectionClose);
begin
{ Close connection to DataSet based database back end }
  try
  { disconnect from the database backend and then free all memory }
    TDLBaseConnection(Params.ConnectionHandle).Disconnect;
    TDLBaseConnection(Params.ConnectionHandle).Free;

  { set connection handle to nil and return zero result code }
    Params.ConnectionHandle := nil;
    Params.ResultCode := 0;
  except
    on E: Exception do begin
      Params.Resultcode := -1;
      SetGlobalErrorText(E.Message); {TODO: this is not thread safe }
    end;
  end; { tryx }
end;

{ Configure connection parameters to backend database }

procedure TDLBaseDriver.ConnectionConfigure(Params: PDLConnectionConfigure);
begin
  try
  { copy configuration parameters into string variables }
    FDataSource := StrPas(Params.DataSource);
    OptionList.CommaText := StrPas(Params.Options);
    FUserName := StrPas(Params.UserName);
    FPassword := StrPas(Params.Password);

  { if changes made, result code = 1 and assign parameters }

    if Assigned(ConnectionConfigureForm) then begin
      with ConnectionConfigureForm.Create(Application) do try
        SetData(DataSource,UserName,Password,OptionList);
        if ShowModal = mrOK then begin
          GetData(FDataSource,FUserName,FPassword,OptionList);

          Params.ResultCode := 1;
          Params.DataSource := @FDataSource[1];
          FOptionText := OptionList.CommaText;
          Params.Options := @FOptionText[1];
          Params.UserName := @FUserName[1];
          Params.Password := @FPassword[1];
        end else begin
          Params.ResultCode := 0;
        end; { if }
      finally
        Free;
      end; { with }
    end else begin
      Params.ResultCode := -1;
      SetGlobalErrorText('No configuration available.');
    end; { else }
  except
    on E: Exception do begin
    { signal error and set global error text }
      Params.Resultcode := -1;
      SetGlobalErrorText(E.Message);
    end;
  end; { tryx }
end;

{ Open connection to database backend }

procedure TDLBaseDriver.ConnectionOpen(Params: PDLConnectionOpen);
var
  Connection: TDLBaseConnection;
begin
  Connection := nil;
  try
  { use virtual method to create appropriate connection to backend database }
    Connection := CreateConnection as TDLBaseConnection;

  { always return handle to connection object }
    Params.ConnectionHandle := Connection;

  { attempt to connect to the database backend }
    OptionList.CommaText := Params.Options;
    Connection.Connect(Params.DataSource,Params.UserName,Params.Password,OptionList);

  { if no error then return zero result code }
    Params.ResultCode := 0;
  except
    on E: Exception do begin
    { return error code and set error text in connection handle }
      Params.Resultcode := -1;
      Connection.ErrorText := E.Message;
    end;
  end; { tryx }
end;

{ Return driver, connection or result set error text }

constructor TDLBaseDriver.Create;
begin
  inherited;
  FOptionList := TStringList.Create;
  FFields := TDLFieldListManager.Create;
end;

destructor TDLBaseDriver.Destroy;
begin
  FFields.Free;
  FOptionList.Free;
  inherited;
end;

procedure TDLBaseDriver.GetErrorText(Params: PDLErrorText);
begin
  with Params^ do try
  { if connection handle given then return error text inside it }
    if Assigned(ConnectionHandle) then begin
      ErrorText := @TDLBaseconnection(ConnectionHandle).ErrorText[1];
  { if result set handle given then return error text inside it }
    end else if Assigned(ResultSetHandle) then begin
      ErrorText := @TDLBaseResultSet(ResultSethandle).ErrorText[1];
    end else begin
    { return global error text }
      ErrorText := GErrorText;
    end; { else }
  except
  { if any error getting error text then set global error and return it }
    on E:Exception do begin
      SetGlobalErrorText(E.Message);
      ErrorText := GErrorText;
    end;
  end; { with }
end;

{ Return list of tables for requested database }

procedure TDLBaseDriver.GetFields(Params: PDLFields);
begin
  try
    TDLBaseConnection(Params.ConnectionHandle).GetFields(Params.TableName);
    Params.FieldCount := Fields.FieldCount;
    Params.FieldList := Fields.FieldList;
  except
    on E: Exception do begin
      Params.ResultCode := -1;
      TDLBaseConnection(Params.ConnectionHandle).ErrorText := E.Message;
    end;
  end; { tryx }
end;

procedure TDLBaseDriver.GetTables(Params: PDLTables);
var
  Connection: TDLBaseConnection;
  TableNames: TStrings;
begin
  Connection := nil;
  try
  { create list for table names }
    TableNames := TStringList.Create;
    try
    { copy connection handle into convenient object variable }
      Connection := Params.ConnectionHandle;

    { attempt to get list of table names }
      Connection.GetTableNames(TableNames);

    { copy table information into global variables }
      CreateGlobalTableList(TableNames);

    { return pointer and count to global table item list }
      Params.TableCount := GTableCount;
      Params.TableList := GTableList;
    finally
    { free the table name list }
      TableNames.Free;
    end; { tryf }
  except
  { if any error then return error code and set error text }
    on E: Exception do begin
      Params.ResultCode := -1;
      Connection.ErrorText := E.Message;
    end;
  end; { tryx }
end;

{ Close the requested result set }

procedure TDLBaseDriver.ResultSetClose(Params: PDLResultSetClose);
var
  ResultSet: TDLBaseResultSet;
begin
  try
  { copy ResultSet handle into convenient object variable }
    ResultSet := Params.ResultSetHandle;

  { attempt to close and free the result set }
    ResultSet.Close;
    ResultSet.Free;
  except
  { return error code and set error text inside connection object }
    on E: Exception do begin
      Params.ResultCode := -1;
      SetGlobalErrorText(E.Message);
    end;
  end; { tryx }
end;

{ Configure the command text used to create result set }

procedure TDLBaseDriver.ResultSetConfigure(Params: PDLResultSetConfigure);
var
  QueryParams: TDLQueryEditor;
  S1: string;
begin
  try
    if not Assigned(ConfigureQueryProc) and (QueryEditorLib = 0) then begin
      S1 := ExtractFilePath(ParamStr(0)) + 'DataLinks\' + SQLEditorName;
      QueryEditorLib := LoadLibrary(@S1[1]);
      ConfigureQueryProc := GetProcAddress(QueryEditorLib,'ConfigureQuery');
    end; { if }

    FillChar(QueryParams,SizeOf(QueryParams),0);
    QueryParams.ConnectionHandle := Params.ConnectionHandle;
    QueryParams.QueryCommand := Params.QueryCommand;
    QueryParams.QueryStruct := Params.QueryStruct;
    QueryParams.GetTablesProc := DataGetTables;
    QueryParams.GetFieldsProc := DataGetFields;
    QueryParams.ConnectionCloseProc := DataConnectionClose;
    QueryParams.ConnectionConfigureProc := DataConnectionConfigure;
    QueryParams.ConnectionOpenProc := DataConnectionOpen;
    QueryParams.GetErrorTextProc := DataGetErrorText;
    QueryParams.InformationProc := DataInformation;
    QueryParams.ResultSetCloseProc := DataResultSetClose;
    QueryParams.ResultSetEOFProc := DataResultSetEOF;
    QueryParams.ResultSetFirstProc := DataResultSetFirst;
    QueryParams.ResultSetGetRowProc := DataResultSetGetRow;
    QueryParams.ResultSetNextProc := DataResultSetNext;
    QueryParams.ResultSetOpenProc := DataResultSetOpen;
    QueryParams.ResultSetSetFilterProc := DataResultSetSetFilter;
    QueryParams.ResultSetSetSortProc := DataResultSetSetSort;

    if Assigned(ConfigureQueryProc) and ConfigureQueryProc(@QueryParams) then begin
      Params.QueryCommand := QueryParams.QueryCommand;
      Params.QueryStruct := QueryParams.QueryStruct;
      Params.ResultCode := 1;
    end else begin
      Params.ResultCode := 0;
    end;
  except
    on E: Exception do begin
      Params.ResultCode := -1;
      SetGlobalErrorText(E.Message);
    end;
  end;
end;

{ Return true if at end of result set }

procedure TDLBaseDriver.ResultSetEOF(Params: PDLResultSetEOF);
begin
  try
    Params.Eof := TDLBaseResultSet(Params.ResultSetHandle).Eof;
  except
    on E: Exception do begin
      Params.Resultcode := -1;
      TDLBaseResultSet(Params.ResultSetHandle).ErrorText := E.Message;
    end;
  end; { tryx }
end;

{ Move to first row in result set }

procedure TDLBaseDriver.ResultSetFirst(Params: PDLResultSetFirst);
begin
  try
    TDLBaseResultSet(Params.ResultSetHandle).First;
  except
    on E: Exception do begin
      Params.Resultcode := -1;
      TDLBaseResultSet(Params.ResultSetHandle).ErrorText := E.Message;
    end;
  end; { tryx }
end;

{ Move to next row in reuslt set }

procedure TDLBaseDriver.ResultSetNext(Params: PDLResultSetNext);
begin
  try
    TDLBaseResultSet(Params.ResultSetHandle).Next;
  except
    on E: Exception do begin
      Params.Resultcode := -1;
      TDLBaseResultSet(Params.ResultSetHandle).ErrorText := E.Message;
    end;
  end; { tryx }
end;

procedure TDLBaseDriver.ResultSetOpen(Params: PDLResultSetOpen);
var
  ResultSet: TDLBaseResultSet;
begin
  ResultSet := nil;
  try
    ResultSet := CreateResultSet(Params.ConnectionHandle);
    Params.ResultSetHandle := ResultSet;
    ResultSet.Open(Params.QueryCommand);
    ResultSet.GetFields(ResultSet.Fields);
    Params.FieldList := ResultSet.Fields.FieldList;
    Params.FieldCount := ResultSet.Fields.FieldCount;
  except
    on E: Exception do begin
      Params.Resultcode := -1;
      ResultSet.ErrorText := E.Message;
    end;
  end; { tryx }
end;

procedure TDLBaseDriver.ResultSetSetFilter(Params: PDLResultSetFilter);
begin
end;

procedure TDLBaseDriver.ResultSetSetSort(Params: PDLResultSetSort);
begin
end;

{ TDLBaseConnection }

constructor TDLBaseConnection.Create;
begin
  FDriver := GDriver;
end;

procedure TDLBaseConnection.GetFields(TableName: string);
begin
  with Driver.CreateResultSet(self) do try
  // Create an empty result set to get field info only
    Open('select * from ' + TableName + ' where 0=1');
    GetFields(Driver.Fields); // Global Driver FieldList
    Close;
  finally
    Free;
  end; { with }
end;

{ TDLBaseResultSet }

constructor TDLBaseResultSet.Create(AConnection: TDLBaseConnection);
begin
  inherited Create;
  FBuffer := TDLBuffer.Create;
  FConnection := AConnection;
  FFields := TDLFieldListManager.Create;
end;

destructor TDLBaseResultSet.Destroy;
begin
  FFields.Free;
  FBuffer.Free;
  inherited Destroy;
end;

{ Register class with list of Data Driver classes }

procedure RegisterDriverClass(ADriverClass: TDLBaseDriverClass);
var
  Index: integer;
begin
{ if first call then create list for holding classes }
  if not assigned(GDriverClassList) then begin
    GDriverClassList := TList.Create;
  end; { if }

{ exit if the class has already been registered }
  for Index := 0 to GDriverClassList.Count-1 do begin
    if GDriverClassList[Index] = ADriverClass then begin
      Exit;
    end; { if }
  end; { for }

{ add the driver class to the list }
  GDriverClassList.Add(ADriverClass);

{ the GDriver variable is used by the procedures exported in DLLs }
  GDriver.Free;
  GDriver := ADriverClass.Create;
end;

{ Data Link API Calls }

{ Close connection to Database backend }

procedure DataConnectionClose(Params: PDLConnectionClose);
begin
  GDriver.ConnectionClose(Params);
end;

{ Configure connection parameters to database backend }

procedure DataConnectionConfigure(Params: PDLConnectionConfigure);
begin
  GDriver.ConnectionConfigure(Params);
end;

{ Open connection to Database backend }

procedure DataConnectionOpen(Params: PDLConnectionOpen);
begin
  GDriver.ConnectionOpen(Params);
end;

{ Return pointer to last error text }

procedure DataGetErrorText(Params: PDLErrorText);
begin
  GDriver.GetErrorText(Params);
end;

{ Return list of field definitions for requested table }

procedure DataGetFields(Params: PDLFields);
begin
  GDriver.GetFields(Params);
end;

{ Return count/list of tables for requested database }

procedure DataGetTables(Params: PDLTables);
begin
  GDriver.GetTables(Params);
end;

{ Return information about Data Driver }

procedure DataInformation(Params: PDLInformation);
begin
  if Assigned(GDriver) then begin
  { if a Data Driver was registered }
    GDriver.Information(Params);
  end else begin
  { if no driver assigned then RegisterDataDriver was not called }
    Params.Resultcode := -1;
    SetGlobalErrorText('No Data Driver class registered.');
  end; { else }
end;

{ Close the result set }

procedure DataResultSetClose(Params: PDLResultSetClose);
begin
  GDriver.ResultSetClose(Params);
end;

{ Return true if at end of result set }

procedure DataResultSetEOF(Params: PDLResultSetEOF);
begin
  GDriver.ResultSetEOF(Params);
end;

{ Move to the first record in the result set }

procedure DataResultSetFirst(Params: PDLResultSetFirst);
begin
  GDriver.ResultSetfirst(Params);
end;

{ Return data for current row in result set }

procedure DataResultSetGetRow(Params: PDLResultSetRow);
begin
  GDriver.ResultSetGetRow(Params);
end;

{ Move to the next record in the result set }

procedure DataResultSetNext(Params: PDLResultSetNext);
begin
  GDriver.ResultSetNext(Params);
end;

{ Open result set }

procedure DataResultSetOpen(Params: PDLResultSetOpen);
begin
  GDriver.ResultSetOpen(Params);
end;

{ Set filter on Result Set }

procedure DataResultSetSetFilter(Params: PDLResultSetFilter);
begin
  GDriver.ResultSetSetFilter(Params);
end;

{ Set Sort on Result Set }

procedure DataResultSetSetSort(Params: PDLResultSetSort);
begin
  GDriver.ResultSetSetSort(Params);
end;

{ Get Result Set }

procedure DataResultSetConfigure(Params: PDLResultSetConfigure);
begin
  GDriver.ResultSetConfigure(Params);
end;

initialization
finalization
{ free all memory used in global variables }
  if Assigned(GDriver) then begin
    GDriver.Free;
    GDriver := nil;
  end; { if }
  FreeGlobalTableList;
  SetGlobalErrorText('');
end.