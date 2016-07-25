{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvPEDBAuth;

interface

uses
  Windows, Messages,
  SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TformDBAuthEditor = class(TForm)
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    editDatasource: TEdit;
    editUsername: TEdit;
    editPassword: TEdit;
    memoOptions: TMemo;
    butnCancel: TButton;
    butnOk: TButton;
  private
  public
  end;

// Procs
  procedure RaveRegister;

implementation

{$R *.dfm}

uses
  RvDataLink, RvDatabase, RvData, RvDefine, RvUtil, RvClass, RvTool, RpDefine;

type
  TRaveDBAuthPropertyEditor = class(TRavePropertyEditor)
  protected
    function GetOptions: TPropertyOptionsSet; override;
    procedure Edit; override;
    function GetValue: string; override;
  end;

procedure RaveRegister;
begin
{TransOff}
  RegisterRavePropertyEditor(TypeInfo(TRaveDBAuth),TRaveDatabase,'AuthDesign',TRaveDBAuthPropertyEditor);
  RegisterRavePropertyEditor(TypeInfo(TRaveDBAuth),TRaveDatabase,'AuthRun',TRaveDBAuthPropertyEditor);
{TransOn}
end;

{ TRaveDBAuthPropertyEditor }

procedure TRaveDBAuthPropertyEditor.Edit;
var
  DataLink: TRaveDataLink;
  LDatasource: string;
  LOptions: string;
  LPassword: string;
  LUsername: string;
begin
  with TRaveDBAuth(GetOrdValue(0)) do begin
  { Copy properties into local variables }
    LDatasource := Datasource;
    LOptions := Options;
    LPassword := Password;
    LUsername := Username;

  { Attempt to get DataLink currently used by Database }
    DataLink := GDataLinks.FindDataLink(TRaveDatabase(Instance[0]).Linktype);

  { Allow user to change connection configuration parameters }
    if DataLink.ConnectionConfigure(LDataSource, LOptions, LUserName, LPassword) then begin
    { Copy local variables into properties and mark component as modified }
      DataSource := LDataSource;
      Options := LOptions;
      Password := LPassword;
      Username := LUsername;
      Modified;
    end; {if}
  end; {with}
end; { Edit }

function TRaveDBAuthPropertyEditor.GetOptions: TPropertyOptionsSet;
begin
  Result := [poReadOnly, poEditor];
end;

function TRaveDBAuthPropertyEditor.GetValue: string;
begin
  Result := '(' + Trans('Auth') + ')';
end;

initialization
  RegisterProc('RVCL', RaveRegister);
end.