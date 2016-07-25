{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvDLADO;

interface

{$I RpVer.inc}

uses
  Windows, SysUtils, RvDLCommon, RvDLBase, RvDLDataSet, Classes,
  DB, ADODB;

type
  TDLADODriver = class(TDLDataSetDriver)
  public
    function CreateConnection: TDLBaseConnection; override;
    function CreateResultSet(AConnection: TDLBaseConnection): TDLBaseResultSet; override;
    procedure Information(Params: PDLInformation); override;
  end; { TDLADODriver}

  TDLAdoConnection = class(TDLDataSetConnection)
  protected
    FADOConnection: TADOConnection;
  public
    procedure Connect(DataSource, UserName, Password: string; OptionList: TStringList); override;
    procedure Disconnect; override;
    procedure GetTableNames(List : TStrings); override;
    property ADOConnection: TADOConnection read FADOConnection write FADOConnection;
  end; { TDLADOConnection }

  TDLADOResultSet = class(TDLDataSetResultSet)
  public
    function OpenDataSet(QueryStr: string): TDataSet; override;
  end; { TDLADOResultSet}

implementation

uses
  Controls, Forms;

{TDLADODriver}

{ Create connection object }

function TDLADODriver.CreateConnection: TDLBaseConnection;
begin
  Result := TDLADOConnection.Create;
end;

{ Create Result Set object }

function TDLADODriver.CreateResultSet(AConnection: TDLBaseConnection): TDLBaseResultSet;
begin
  Result := TDLADOResultSet.Create(AConnection);
end;

{ Return installed state, version and driver names }

procedure TDLADODriver.Information(Params: PDLInformation);
begin
  StrPCopy(Params.InternalName,'ADO');
  StrPCopy(Params.DisplayName,'Active Data Objects');
  Params.VerMajor := 2;
  Params.VerMinor := 0;
  Params.Installed := true;
end;

{ TDLADOConnection }

{ Attempt to connect to session and database }

procedure TDLADOConnection.Connect(DataSource, UserName, Password: string; OptionList: TStringList);
begin
  FADOConnection := TADOConnection.Create(nil);
  FADOConnection.LoginPrompt := false;
  FADOConnection.ConnectionString := DataSource;
  FADOConnection.ConnectionTimeout := StrToIntDef(OptionList.Values['ConnectionTimeout'],15);
  FADOConnection.CommandTimeout := StrToIntDef(OptionList.Values['CommandTimeout'],30);
  FADOConnection.Connected := true;
end;

{ Attempt to disconnect from session and database }

procedure TDLADOConnection.Disconnect;
begin
  FreeAndNil(FADOConnection);
end;

{ Use Session object to get list of table names }

procedure TDLADOConnection.GetTableNames(List: TStrings);
begin
  FADOConnection.GetTableNames(List);
end;

{ TDLADOResultSet }

function TDLADOResultSet.OpenDataSet(QueryStr: string): TDataSet;
begin
{ Create query object and assign it to Ancestor's DataSet property }
  Result := TADOQuery.Create(nil);
  try
    with Result as TADOQuery do begin
      Connection := TDLADOConnection(self.Connection).ADOConnection;
		{$IFDEF LEVEL6}
      CommandTimeout := Connection.CommandTimeout;
		{$ENDIF}
      SQL.Text := QueryStr;
      Open;
    end; { with }
  except
    Result.Free;
    raise;
  end; { tryx }
end;

initialization
  RegisterDriverClass(TDLADODriver);
end.