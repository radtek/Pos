{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvDLBDECfg;

{ Implements configuration form for bDE Data Link driver }

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, ComCtrls, ActnList, RvDLBase;

type
  TDLBDEConfigureForm = class(TDLConfigForm)
    PageControl1: TPageControl;
    TabSheet1: TTabSheet;
    Label3: TLabel;
    Label4: TLabel;
    editUsername: TEdit;
    editPassword: TEdit;
    TabSheet2: TTabSheet;
    Panel1: TPanel;
    butnOk: TButton;
    butnCancel: TButton;
    gboxAlias: TGroupBox;
    gboxParams: TGroupBox;
    radoAlias: TRadioButton;
    radoParams: TRadioButton;
    Label1: TLabel;
    cmboAlias: TComboBox;
    Label2: TLabel;
    cmboDriver: TComboBox;
    Label5: TLabel;
    memoParams: TMemo;
    ActionList1: TActionList;
    actnOk: TAction;
    TabSheet3: TTabSheet;
    Button1: TButton;
    lboxTables: TListBox;
    Label6: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure radoParamsClick(Sender: TObject);
    procedure actnOkUpdate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    procedure EnableGroup(AGroupBox: TGroupBox; const AEnabled: boolean);
  public
    procedure SetData(DataSource, UserName, Password: string; OptionList: TStringList); override;
    procedure GetData(var DataSource, UserName, Password: string; OptionList: TStringList); override;
  end;

implementation

{$R *.DFM}

uses
  DBTables, RvDLCommon, RvDLBDE;

{ TBDEConfigureForm }

procedure TDLBDEConfigureForm.FormCreate(Sender: TObject);
begin
  PageControl1.ActivePageIndex := 0;
  Session.GetAliasNames(cmboAlias.Items);
  Session.GetDriverNames(cmboDriver.Items);
end;

procedure TDLBDEConfigureForm.EnableGroup(AGroupBox: TGroupBox; const AEnabled: boolean);
var
  I1: integer;
begin
  with AGroupBox do begin
    Enabled := AEnabled;
    for I1 := 0 to ControlCount - 1 do begin
      Controls[I1].Enabled := AEnabled;
    end; { for }
  end; { with }
end;

procedure TDLBDEConfigureForm.radoParamsClick(Sender: TObject);
begin
  EnableGroup(gboxAlias,radoAlias.Checked);
  EnableGroup(gboxParams,radoParams.Checked);
end;

procedure TDLBDEConfigureForm.actnOkUpdate(Sender: TObject);
begin
  actnOk.Enabled := (radoAlias.Checked and (cmboAlias.ItemIndex > -1)) or
   (radoParams.Checked and (cmboDriver.ItemIndex > -1));
end;

procedure TDLBDEConfigureForm.SetData(DataSource, UserName, Password: string; OptionList: TStringList);
var
  AliasName: string;
  DriverName: string;
  DriverParams: TStrings;
begin
  DriverParams := TStringList.Create;
  try
  { convert data source into alias name or driver name and parameters }
    DecodeDataSource(DataSource, AliasName, DriverName, DriverParams);
    if AliasName <> '' then begin
    { if alias provided then check radio button and asign combo value }
      radoAlias.Checked := true;
      cmboAlias.ItemIndex := cmboAlias.Items.IndexOf(AliasName);
    end else if DriverName <> '' then begin
    { if driver name given then check radio button and assign combo value }
      radoParams.Checked := true;
      cmboDriver.ItemIndex := cmboDriver.Items.IndexOf(DriverName);
      memoParams.Text := DriverParams.Text;
    end;
  { assign user name and password }
    editUsername.Text := Username;
    editPassword.Text := Password;
  finally
    DriverParams.Free;
  end; { tryf }
end;

procedure TDLBDEConfigureForm.GetData(var DataSource, UserName, Password: string; OptionList: TStringList);
begin
  if radoAlias.Checked then begin
    Datasource := 'Alias=' + cmboAlias.Items[cmboAlias.ItemIndex];
  end else begin
    Datasource := '"Driver=' + cmboDriver.Items[cmboDriver.ItemIndex] + '",' + memoParams.Lines.CommaText;
  end; { else }
{ clear options and return user name and password }
  Username := editUsername.Text;
  Password := editPassword.Text;
end;

procedure TDLBDEConfigureForm.Button1Click(Sender: TObject);
begin
  TestConnection(lboxTables.Items);
end;

initialization
  ConnectionConfigureForm := TDLBDEConfigureForm;
end.
