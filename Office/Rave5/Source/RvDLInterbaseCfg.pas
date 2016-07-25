{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvDLInterbaseCfg;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, ComCtrls, ActnList, RvDLBase;

type
  TDLInterbaseConfigureForm = class(TDLConfigForm)
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
    ActionList1: TActionList;
    actnOk: TAction;
    Label1: TLabel;
    lablServer: TLabel;
    Label5: TLabel;
    cmboProtocol: TComboBox;
    editServer: TEdit;
    editFile: TEdit;
    butnBrowseFile: TButton;
    odlgDatabase: TOpenDialog;
    Label2: TLabel;
    cboxDialect: TComboBox;
    Label6: TLabel;
    editSQLRole: TEdit;
    TabSheet3: TTabSheet;
    Button1: TButton;
    Label7: TLabel;
    lboxTables: TListBox;
    procedure FormCreate(Sender: TObject);
    procedure actnOkUpdate(Sender: TObject);
    procedure cmboProtocolChange(Sender: TObject);
    procedure butnBrowseFileClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
  public
    procedure SetData(DataSource, UserName, Password: string; OptionList: TStringList); override;
    procedure GetData(var DataSource, UserName, Password: string; OptionList: TStringList); override;
  end;

implementation

uses
  RvDLCommon, RvDLInterbase;

{$R *.DFM}

type
  TIBProtocol = (ipLocal,ipTCPIP,ipNetBios);

procedure TDLInterbaseConfigureForm.FormCreate(Sender: TObject);
begin
  PageControl1.ActivePageIndex := 0;
  with cmboProtocol.Items do begin
    cmboProtocol.ItemIndex := AddObject('Local', TObject(ipLocal));
    AddObject('TCP/IP', TObject(ipTCPIP));
    AddObject('NetBios', TObject(ipNetBios));
  end;
end;

procedure TDLInterbaseConfigureForm.actnOkUpdate(Sender: TObject);
begin
  if TIBProtocol(cmboProtocol.Items.Objects[cmboProtocol.ItemIndex]) = ipLocal then begin
    actnOk.Enabled := Length(Trim(editFile.Text)) > 0;
  end else begin
    actnOk.Enabled := (Length(Trim(editFile.Text)) > 0) and (Length(Trim(editServer.Text)) > 0);
  end;
end;

procedure TDLInterbaseConfigureForm.cmboProtocolChange(Sender: TObject);
begin
  lablServer.Enabled := TIBProtocol(cmboProtocol.Items.Objects[cmboProtocol.ItemIndex]) <> ipLocal;
  editServer.Enabled := lablServer.Enabled;
  butnBrowseFile.Enabled := not lablServer.Enabled;
end;

procedure TDLInterbaseConfigureForm.butnBrowseFileClick(Sender: TObject);
begin
  odlgDatabase.InitialDir := ExtractFilePath(Trim(editFile.Text));
  if odlgDatabase.Execute then begin
    editFile.Text := odlgDatabase.Filename;
  end;
end;

procedure TDLInterbaseConfigureForm.FormShow(Sender: TObject);
begin
  cmboProtocolChange(Sender);
end;

procedure TDLInterbaseConfigureForm.GetData(var DataSource, UserName, Password: string; OptionList: TStringList);
begin
{ Assign new values to DataSource, Options, UserName and Password here... }
  case TIBProtocol(cmboProtocol.Items.Objects[cmboProtocol.ItemIndex]) of
    ipLocal: Datasource := Trim(editFile.Text);
    ipTCPIP: DataSource := Trim(editServer.Text) + ':' + Trim(editFile.Text);
    ipNetBios: DataSource := '\\' + Trim(editServer.Text) + '\' + Trim(editFile.Text);
  end;
  if cboxDialect.ItemIndex = 0 then begin
    OptionList.Values['SQLDialect'] := '1';
  end else begin
    OptionList.Values['SQLDialect'] := '3';
  end; { else }
  OptionList.Values['SQLRole'] := Trim(editSQLRole.Text);
  Username := editUsername.Text;
  Password := editPassword.Text;
end;

procedure TDLInterbaseConfigureForm.SetData(DataSource, UserName, Password: string; OptionList: TStringList);
var
  I1: integer;
  S1: String;
begin
{ Initialize configuration form from DataSource, Options, UserName and Password here... }
  S1 := Datasource;
  I1 := Pos(':', S1);
  if Copy(S1, 1, 2) = '\\' then begin
    Delete(S1, 1, 2);
    I1 := Pos('\', S1);
    if I1 > 0 then begin
      editServer.Text := Copy(S1,1,I1 - 1);
      editFile.Text := Copy(S1,I1 + 1,MaxInt);
      cmboProtocol.ItemIndex := cmboProtocol.Items.IndexOfObject(TObject(ipNetBios));
    end else begin
      editFile.Text := Datasource;
    end;
  end else if (Pos(':',Copy(S1,I1 + 1,MaxInt)) > 0) or  // check for windows path
              (Pos('/',Copy(S1,I1 + 1,MaxInt)) > 0) then begin  // check for linux path
    editServer.Text := Copy(S1,1,I1 - 1);
    editFile.Text := Copy(S1,I1 + 1,MaxInt);
    cmboProtocol.ItemIndex := cmboProtocol.Items.IndexOfObject(TObject(ipTCPIP));
  end else begin
    editFile.Text := Datasource;
  end;
  editUsername.Text := Username;
  editPassword.Text := Password;
  if OptionList.Values['SQLDialect'] = '1' then begin
    cboxDialect.ItemIndex := 0;
  end else begin
    cboxDialect.ItemIndex := 1;
  end; { else }
  editSQLRole.Text := OptionList.Values['SQLRole'];
end;

procedure TDLInterbaseConfigureForm.Button1Click(Sender: TObject);
begin
  TestConnection(lboxTables.Items);
end;

initialization
  RvDLBase.ConnectionConfigureForm := TDLInterbaseConfigureForm;
end.