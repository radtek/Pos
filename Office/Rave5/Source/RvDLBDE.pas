{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvDLBDE;

{ Implements classes used to interface with Borland Database Engine }

interface

uses
  Windows, SysUtils, RvDLCommon, RvDLBase, RvDLDataSet, Classes, DB, DbTables;

type
  TDLBDEDriver = class(TDLDataSetDriver)
  public
    function CreateConnection: TDLBaseConnection; override;
    function CreateResultSet(AConnection: TDLBaseConnection): TDLBaseResultSet; override;
    procedure Information(Params: PDLInformation); override;
  end; { TDLBDEDriver }

  TDLBDEConnection = class(TDLDataSetConnection)
  protected
    FSession: TSession;
    FDatabase: TDatabase;
  public
    procedure Connect(DataSource, UserName, Password: string; OptionList: TStringList); override;
    procedure Disconnect; override;
    procedure GetTableNames(List: TStrings); override;
    //
    property Session: TSession read FSession;
    property Database: TDatabase read FDatabase;
  end; { TDLBDEConnection }

  TDLBDEResultSet = class(TDLDataSetResultSet)
  protected
    function OpenDataSet(QueryStr: string): TDataSet; override;
  end; { TDLBDEResultSet }

{ Return encoded DataSource for BDE database }
  function EncodeDataSource(const AliasName, DriverName: string; DriverParams: TStrings) : string;

{ Decode data source into alias or driver name and parameters }
  procedure DecodeDataSource(const DataSource: string; var AliasName, DriverName: string; DriverParams: TStrings);

implementation

uses
  Controls, Forms, Registry;

{ Return encoded DataSource for BDE database }

function EncodeDataSource(const AliasName, DriverName: string; DriverParams: TStrings) : string;
begin
  if AliasName <> '' then begin
    Result := 'Alias=' + AliasName;
  end else begin
    Result := 'Driver=' + DriverName + ',' + DriverParams.CommaText;
  end; { else }
end;

{ Decode data source into alias or driver name and parameters }

procedure DecodeDataSource(const DataSource: string; var AliasName, DriverName: string; DriverParams: TStrings);
var
  Params : TStrings;
begin
{ create temporary string list }
  Params := TStringList.Create;
  try
  { assign data source as comma text }
    Params.CommaText := DataSource;

  { get alias and driver name, one will be filled in and one will be blank }
    AliasName := Params.Values['Alias'];
    DriverName := Params.Values['Driver'];

  { remove alias and driver names from temporary string list }
    Params.Values['Alias'] := '';
    Params.Values['Driver'] := '';

  { any remaining lines are driver parameters }
    DriverParams.Assign(Params);
  finally
  { free the temporary variable }
    Params.Free;
  end; { tryf }
end;

{ Determines if BDE is installed, algorythem from article by Bill Todd }

function BDEInstalled: boolean;
//const
//  cDatabaseEngineKey = '\Software\Borland\Database Engine';
//  cLanguageDriverKey = '\Software\Borland\BLW32';
//  cLanguageDriverTag = 'BLAPIPATH';
//  cConfigPathTag = 'configfile01';
//  cDLLPathTag = 'dllpath';
begin
{ assume the BDE is installed }
  Result := true;
(*
  with TRegistry.Create do try
  { initialize registry object }
    RootKey := HKEY_LOCAL_MACHINE;
    OpenKey(cDatabaseEngineKey, True);

  { See if the Database Engine keys exist. }
    if (ReadString(cConfigPathTag) = '') or (ReadString(cDLLPathTag) = '') then begin
      Result := false;
    end; { if }

  { See if the language driver keys exist. }
    OpenKey(cLanguageDriverKey, True);
    if Result and ReadString(cLanguageDriverTag) = '' then begin
      Result := false;
    end; { if }
  finally
  { free the registery }
    Free;
  end; { tryf }
*)
end;

{ TDLBDEDriver }

function TDLBDEDriver.CreateConnection: TDLBaseConnection;
begin
  Result := TDLBDEConnection.Create;
end;

function TDLBDEDriver.CreateResultSet(AConnection: TDLBaseConnection): TDLBaseResultSet;
begin
  Result := TDLBDEResultSet.Create(AConnection);
end;

procedure TDLBDEDriver.Information(Params: PDLInformation);
begin
  Params.Installed := BDEInstalled;
  Params.VerMajor := 2;
  Params.VerMinor := 0;
  StrPCopy(Params.InternalName,'BDE');
  StrPCopy(Params.DisplayName,'Borland Database Engine');
end;

{ TDLBDEConnection }

procedure TDLBDEConnection.Connect(DataSource, UserName, Password: string; OptionList: TStringList);
var
  AliasName: string;
  DriverName: string;
  DriverParams: TStrings;
begin
{ create temporary string list }
  DriverParams := TStringList.Create;
  try
  { decode data source PChar into seperate connection parameters }
    DecodeDataSource(DataSource,AliasName,DriverName,DriverParams);

  { create the session and setup for automatic unique session name }
    FSession := TSession.Create(nil);
    FSession.Name := 'BDE';
    FSession.AutoSessionName := true;
    FSession.Active := true;

  { create database and assign session and database names }
    FDatabase := TDatabase.Create(nil);
    FDatabase.SessionName := FSession.SessionName;
    FDatabase.DatabaseName := FSession.SessionName + '_Database';
    FDatabase.LoginPrompt := false;

    if AliasName <> '' then begin
    { if alias assigned then use it }
      FDatabase.AliasName := AliasName;
    end else begin
    { otherwise use driver name and parameters }
      FDatabase.DriverName := DriverName;
      FDatabase.Params.Assign(DriverParams);
    end; { else }

  { assign user name and password }
    if UserName <> '' then begin
      FDatabase.Params.Values['User Name'] := UserName;
    end; { if }
    if Password <> '' then begin
      FDatabase.Params.Values['Password'] := Password;
    end; { if }
  finally
  { free temporary string list }
    DriverParams.Free;
  end;

{ activate the session }
  FSession.Active := true;
end;

procedure TDLBDEConnection.Disconnect;
begin
{ attempt to deactivate and destroy database and then session }
  FDatabase.Free;
  FDatabase := nil;
  FSession.Free;
  FSession := nil;
end;

procedure TDLBDEConnection.GetTableNames(List: TStrings);
var
  Driver: string;
begin
  if Database.AliasName = '' then begin
    Driver := Database.DriverName;
  end else begin
    Driver := Session.GetAliasDriverName(Database.AliasName);
  end; { else }
  if UpperCase(Driver) = 'STANDARD' then begin
    Session.GetTableNames(Database.DatabaseName,'',true,false,List);
  end else begin
    Session.GetTableNames(Database.DatabaseName,'',false,false,List);
  end; { else }
end;

{ TDLBDEResultSet }

function TDLBDEResultSet.OpenDataSet(QueryStr: string): TDataSet;
begin
  Result := TQuery.Create(nil);
  try
    with Result as TQuery do begin
      SessionName := TDLBDEConnection(self.Connection).Session.SessionName;
      DatabaseName := TDLBDEConnection(self.Connection).Database.DatabaseName;
      SQL.Text := QueryStr;
      Open;
    end; { with }
  except
    Result.Free;
    raise;
  end; { tryx }
end;

initialization
  RegisterDriverClass(TDLBDEDriver);
end.