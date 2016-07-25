{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvDLInterbase;

{ Implements classes used to interface with Interbase Express }

interface

uses
  Windows, SysUtils, Forms, RvDLCommon, RvDLBase, RvDLDataSet, Classes,
  DB, IBDatabase, IBCustomDataSet, IBQuery;

type
  TDLInterbaseDriver = class(TDLDataSetDriver)
  public
    function CreateConnection: TDLBaseConnection; override;
    function CreateResultSet(AConnection: TDLBaseConnection): TDLBaseResultSet; override;
    procedure Information(Params: PDLInformation); override;
  end; { TDLInterbaseDriver }

  TDLInterbaseConnection = class(TDLDataSetConnection)
  protected
    FDatabase: TIBDatabase;
    FTransaction: TIBTransaction;
  public
    procedure Connect(DataSource, UserName, Password: string; OptionList: TStringList); override;
    procedure Disconnect; override;
    procedure GetTableNames(List: TStrings); override;
    //
    property Database: TIBDatabase read FDatabase;
    property Transaction: TIBTransaction read FTransaction;
  end; { TDLInterbaseConnection }

  TDLInterbaseResultSet = class(TDLDataSetResultSet)
  public
    function OpenDataSet(QueryStr: string): TDataSet; override;
  end; { TDLInterbaseResultSet }

implementation

{ TDLInterbaseDriver }

function TDLInterbaseDriver.CreateConnection: TDLBaseConnection;
begin
{ Create Interbase connection object }
  Result := TDLInterbaseConnection.Create;
end;

function TDLInterbaseDriver.CreateResultSet(AConnection: TDLBaseConnection): TDLBaseResultSet;
begin
{ Create Interbase Result Set object }
  Result := TDLInterbaseResultSet.Create(AConnection);
end;

procedure TDLInterbaseDriver.Information(Params: PDLInformation);
begin
{ Return installed state, version and driver names }
  Params.Installed := true;
  Params.VerMajor := 2;
  Params.VerMinor := 0;
  StrPCopy(Params.InternalName, 'IBX');
  StrPCopy(Params.DisplayName, 'Interbase Express Database Engine');
end;

{ TDLInterbaseConnection }

procedure TDLInterbaseConnection.Connect(DataSource, UserName, Password : string; OptionList: TStringList);
begin
{ Attempt to connect to the database }
  FDatabase := TIBDatabase.Create(nil);
  with FDatabase do begin
    DatabaseName := Datasource;
    LoginPrompt := False;
    Params.Values['user_name'] := Username;
    Params.Values['password'] := Password;
    if OptionList.Values['SQLDialect'] = '1' then begin
      SQLDialect := 1;
    end else begin
      SQLDialect := 3;
    end; { else }
    Params.Values['sql_role_name'] := OptionList.Values['SQLRole'];
    Connected := True;
  end;
  FTransaction := TIBTransaction.Create(nil);
  with FTransaction do begin
    DefaultDatabase := FDatabase;
    Params.Add('read_committed');
    Params.Add('rec_version');
    Params.Add('nowait');
    Active := True;
  end;
end;

procedure TDLInterbaseConnection.Disconnect;
begin
  FreeAndNil(FTransaction);
  FreeAndNil(FDatabase);
end;

procedure TDLInterbaseConnection.GetTableNames(List : TStrings);
begin
  Database.GetTableNames(List);
end;

{ TDLInterbaseResultSet }

{ Open result set by creating query and executing SQL Command }

function TDLInterbaseResultSet.OpenDataSet(QueryStr: string): TDataSet;
begin
  Result := TIBQuery.Create(Application);
  try
    with TIBQuery(Result) do begin
      Database := TDLInterbaseConnection(self.Connection).Database;
      Database.Name := TDLInterbaseConnection(self.Connection).Database.Name;
      SQL.Text := QueryStr;
      Open;
    end; { with }
  except
    Result.Free;
    raise;
  end; { tryx }
end;

initialization
  RegisterDriverClass(TDLInterbaseDriver);
end.