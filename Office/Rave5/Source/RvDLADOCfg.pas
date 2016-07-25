{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvDLADOCfg;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Forms, Dialogs,
  ActnList, Controls, ExtCtrls, StdCtrls, ComCtrls, RVDLBase;

type
  TDLADOConfigureForm = class(TDLConfigForm)
    PageControl1: TPageControl;
    TabSheet2: TTabSheet;
    Panel1: TPanel;
    butnOk: TButton;
    butnCancel: TButton;
    ActionList1: TActionList;
    actnOk: TAction;
    radoDataLinkFile: TRadioButton;
    cmboDataLinkFile: TComboBox;
    radoConnectionString: TRadioButton;
    editConnectionString: TEdit;
    butnBrowseDataLinkFile: TButton;
    butnBuildConnectionString: TButton;
    TabSheet1: TTabSheet;
    Button1: TButton;
    lboxTables: TListBox;
    Label6: TLabel;
    editConnectionTimeout: TEdit;
    Label1: TLabel;
    editCommandTimeout: TEdit;
    Label2: TLabel;
    procedure radoParamsClick(Sender: TObject);
    procedure actnOkUpdate(Sender: TObject);
    procedure butnBrowseDataLinkFileClick(Sender: TObject);
    procedure butnBuildConnectionStringClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
  public
    procedure SetData(DataSource, UserName, Password: string; OptionList: TStringList); override;
    procedure GetData(var DataSource, UserName, Password: string; OptionList: TStringList); override;
  end;

implementation

{$R *.DFM}

uses
  ADODB, RvDLCommon, RVDLADO;

const
  LFileNameIdent = 'FILE NAME=';

{ TDLADOConfigureForm }

procedure TDLADOConfigureForm.radoParamsClick(Sender: TObject);
begin
  cmboDataLinkFile.Enabled := radoDataLinkFile.Checked;
  butnBrowseDataLinkFile.Enabled := radoDataLinkFile.Checked;
  //
  editConnectionString.Enabled := radoConnectionString.Checked;
  butnBuildConnectionString.Enabled := radoConnectionString.Checked;
end;

procedure TDLADOConfigureForm.actnOkUpdate(Sender: TObject);
begin
  actnOk.Enabled := (radoDataLinkFile.Checked and (Length(Trim(cmboDataLinkFile.Text)) > 0))
   or (radoConnectionString.Checked and (Length(Trim(editConnectionString.Text)) > 0))
end;

procedure TDLADOConfigureForm.butnBrowseDataLinkFileClick(Sender: TObject);
var
  S1: string;
begin
  S1 := PromptDataLinkFile(Handle, cmboDataLinkFile.Text);
  if SameText(ExtractFilePath(S1), DataLinkDir + '\') then begin
    cmboDataLinkFile.Text := ExtractFileName(S1)
  end else begin
    cmboDataLinkFile.Text := S1;
  end;
end;

procedure TDLADOConfigureForm.butnBuildConnectionStringClick(Sender: TObject);
begin
  editConnectionString.Text := PromptDataSource(Handle, editConnectionString.Text);
end;

procedure TDLADOConfigureForm.FormCreate(Sender: TObject);
begin
  PageControl1.ActivePageIndex := 0;
  GetDataLinkFiles(cmboDataLinkFile.Items);
end;

procedure TDLADOConfigureForm.FormShow(Sender: TObject);
begin
  radoParamsClick(Sender);
end;

procedure TDLADOConfigureForm.GetData(var DataSource, UserName, Password: string; OptionList: TStringList);
begin
  if radoConnectionString.Checked then begin
    DataSource := Trim(editConnectionString.Text);
  end else if Length(Trim(cmboDataLinkFile.Text)) > 0 then begin
    if Length(ExtractFilePath(Trim((cmboDataLinkFile.Text)))) = 0 then begin
      DataSource := LFileNameIdent + DataLinkDir + '\' + Trim(cmboDataLinkFile.Text);
    end else begin
      DataSource := LFileNameIdent + cmboDataLinkFile.Text;
    end; { else }
  end else begin
    DataSource := '';
  end; { else }
  OptionList.Values['ConnectionTimeout'] := IntToStr(StrToIntDef(editConnectionTimeout.Text,15));
  OptionList.Values['CommandTimeout'] := IntToStr(StrToIntDef(editCommandTimeout.Text,30));
  Username := '';
  Password := '';
end;

procedure TDLADOConfigureForm.SetData(DataSource, UserName, Password: string; OptionList: TStringList);
var
  LFileName: string;
begin
{ Initialize form controls using parameters passed in }
  radoDataLinkFile.Checked := true;
  editConnectionTimeout.Text := OptionList.Values['ConnectionTimeout'];
  if editConnectionTimeout.Text = '' then begin
    editConnectionTimeout.Text := '15';
  end; { if }
  editCommandTimeout.Text := OptionList.Values['CommandTimeout'];
  if editCommandTimeout.Text = '' then begin
    editCommandTimeout.Text := '30';
  end; { if }
  if SameText(Copy(DataSource,1,Length(LFileNameIdent)),LFileNameIdent) then begin
    LFileName := Copy(DataSource,Length(LFileNameIdent) + 1,MaxInt);
    if SameText(ExtractFilePath(LFileName),DataLinkDir + '\') then begin
      cmboDataLinkFile.Text := ExtractFileName(LFileName)
    end else begin
      cmboDataLinkFile.Text := LFileName;
    end; { else }
  end else begin
    editConnectionString.Text := Datasource;
    radoConnectionString.Checked := true;
  end;
end;

procedure TDLADOConfigureForm.Button1Click(Sender: TObject);
begin
  TestConnection(lboxTables.Items);
end;

initialization
  ConnectionConfigureForm := TDLADOConfigureForm;
end.