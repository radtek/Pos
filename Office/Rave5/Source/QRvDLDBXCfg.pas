{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRvDLDBXCfg;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms,
  StdCtrls, ExtCtrls, ComCtrls, ActnList, IniFiles, Grids, QRvDLBase;

type
  TDataSourceObject = class
    DataSource: string;
  end;

  TDLDBXConfigureForm = class(TDLConfigForm)
    Panel1: TPanel;
    butnOk: TButton;
    butnCancel: TButton;
    ActionList1: TActionList;
    actnOk: TAction;
    Panel2: TPanel;
    lboxConnection: TListBox;
    Panel3: TPanel;
    Panel4: TPanel;
    Label1: TLabel;
    Label2: TLabel;
    sgridConnectionSettings: TStringGrid;
    editConnection: TEdit;
    procedure actnOkUpdate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure lboxConnectionClick(Sender: TObject);
    procedure editConnectionChange(Sender: TObject);
    procedure editConnectionKeyPress(Sender: TObject; var Key: Char);
  protected
    FDataSource: string;
    DataSourceList: TStringList;
    FUserName: string;
    FPassword: string;
    FConnectionName: string;
    ConnectionIniFile: TIniFile;
    ConnectionList: TStringList;
    DriverIniFile: TIniFile;
    DriverList: TStringList;
    DriverSection: string;
    ConnectionSection: string;
    PriorItemIndex: integer; //used to tell whether to update string grid
                             // we don't want to draw new blank items
                             // if the new connection name isn't in the
                             // list of defined connections and we haven't
                             // alread draw new default items.

    function BuildDataSource(ConnectionName: string): string;
    procedure ShowDataSource;
    function GetConnectionFileName: string;
    function GetDriverFileName: string;
    procedure GetDataSources;

    property ConnectionFileName: string read GetConnectionFileName;
    property DriverFileName: string read GetDriverFileName;
    property ConnectionName: string read FConnectionName write FConnectionName;
  public
    procedure SetData(DataSource, UserName, Password: string; OptionList: TStringList); override;
    procedure GetData(var DataSource, UserName, Password: string; OptionList: TStringList); override;
    //
    property DataSource: string read FDataSource;
    property UserName: string read FUserName;
    property Password: string read FPassword;
  end;

implementation

uses
  Registry, QRvDLCommon;

{$R *.DFM}

{ TformILConfigBDE }

procedure TDLDBXConfigureForm.actnOkUpdate(Sender: TObject);
begin
  actnOk.Enabled := Trim(editConnection.Text) <> '';
end;

procedure TDLDBXConfigureForm.FormCreate(Sender: TObject);
begin
  ConnectionList := TStringList.Create;
  DriverList := TStringList.Create;
  DataSourceList := TStringList.Create;
end;

procedure TDLDBXConfigureForm.FormDestroy(Sender: TObject);
var
  I1: integer;
begin
  FreeAndNil(ConnectionIniFile);
  with ConnectionList do begin
    for I1 := 0 to Count - 1 do begin
      TDataSourceObject(ConnectionList.Objects[I1]).Free;
    end; { for }
  end; { with }
  FreeAndNil(ConnectionList);
  FreeAndNil(DriverList);
  FreeAndNil(DataSourceList);
end;

procedure TDLDBXConfigureForm.FormShow(Sender: TObject);
var
  Index: integer;
  TempList: TStringList;
  DataSourceObject: TDataSourceObject;
begin
  ConnectionIniFile := TIniFile.Create(ConnectionFileName);
  ConnectionIniFile.ReadSections(ConnectionList);
  DriverIniFile := TIniFile.Create(DriverFileName);
  DriverIniFile.ReadSections(DriverList);
  GetDataSources;
  TempList := TStringList.Create;
  try
    TempList.Text := DataSource;
    Index := lboxConnection.Items.IndexOf(TempList.Values['ConnectionName']);
    if Index >= 0 then begin
      DataSourceObject := TDataSourceObject(lboxConnection.Items.Objects[Index]);
      DataSourceObject.DataSource := DataSource;
    end; { if }
    editConnection.Text := TempList.Values['ConnectionName'];
    if editConnection.Text = '' then begin
      ShowDataSource;
    end; { if }
  finally
    FreeAndNil(TempList);
  end;
end;

function TDLDBXConfigureForm.GetConnectionFileName: string;
var
  Reg: TRegIniFile;
begin
  Reg := TRegIniFile.Create;
  try
    Reg.RootKey := HKey_Current_User;
    Result := Reg.ReadString('\Software\Borland\DBExpress', 'Connection Registry File', '')
  finally
    Reg.Free;
  end;
end;

function TDLDBXConfigureForm.GetDriverFileName: string;
var
  Reg: TRegIniFile;
begin
  Reg := TRegIniFile.Create;
  try
    Reg.RootKey := HKey_Current_User;
    Result := Reg.ReadString('\Software\Borland\DBExpress', 'Driver Registry File', '')
  finally
    Reg.Free;
  end;
end;

function TDLDBXConfigureForm.BuildDataSource(ConnectionName: string): string;
var
  I1 : integer;
  SectionList: TStringList;
  DriverName: string;
begin
  ConnectionSection := ConnectionName;
  SectionList := TStringList.Create;
  try
    { get values from dbxconnections.ini file }
    ConnectionIniFile.ReadSectionValues(ConnectionSection, SectionList);
    Result := 'ConnectionName=' + ConnectionSection + #13;
    for I1 := 0 to SectionList.Count - 1 do begin
      if CompareText(SectionList.Names[I1],'User_Name') = 0 then begin
        FUserName := SectionList.Values['User_Name'];
      end else if CompareText(SectionList.Names[I1],'DriverName') = 0 then begin
        Result := Result + SectionList.Names[I1] + '=' +
                           SectionList.Values[SectionList.Names[I1]] + #13;
        DriverName := SectionList.Values['DriverName'];
      end else if CompareText(SectionList.Names[I1],'Password') = 0 then begin
        FPassword := SectionList.Values['Password'];
      end else begin
        Result := Result + SectionList.Names[I1] + '=' +
                           SectionList.Values[SectionList.Names[I1]] + #13;
      end; { else }
    end; { for }

    { get values from dbxdrivers.ini file }
    DriverIniFile.ReadSectionValues(DriverName, SectionList);
    for I1 := 0 to SectionList.Count - 1 do begin
      if (CompareText(SectionList.Names[I1],'LibraryName') = 0) or
         (CompareText(SectionList.Names[I1],'VendorLib') = 0) or
         (CompareText(SectionList.Names[I1],'GetDriverFunc') = 0) then begin
        Result := Result + SectionList.Names[I1] + '=' +
                           SectionList.Values[SectionList.Names[I1]] + #13;
      end; { else }
    end; { for }

    Result := Result + 'LoadParamsOnConnect=False'+#13;

  finally
    FreeAndNil(SectionList);
  end;
end;

procedure TDLDBXConfigureForm.lboxConnectionClick(Sender: TObject);
var
  I1 : integer;
  ConnectionSettings: TStringList;
begin
  if lboxConnection.ItemIndex >= 0 then begin
    editConnection.Text := lboxConnection.Items[lboxConnection.ItemIndex];

    // Set Header Names
    sgridConnectionSettings.Cells[0,0] := 'Key';
    sgridConnectionSettings.Cells[1,0] := 'Value';

    // Set Key/Value data
    ConnectionSettings := TStringList.Create;
    try
      ConnectionSettings.Text := TDataSourceObject(lboxConnection.Items.Objects[lboxConnection.ItemIndex]).DataSource;
      { get values from dbxconnections.ini file }
      sgridConnectionSettings.RowCount := ConnectionSettings.Count + 1;
      for I1 := 1 to ConnectionSettings.Count do begin
        sgridConnectionSettings.Cells[0,I1] := ConnectionSettings.Names[I1 - 1];
        sgridConnectionSettings.Cells[1,I1] := ConnectionSettings.Values[ConnectionSettings.Names[I1 - 1]];
      end; { for }
    finally
      FreeAndNil(ConnectionSettings);
    end;
  end else begin
    ShowDataSource;
  end; { else }
  with sgridConnectionSettings do begin
    if (RowCount * DefaultRowHeight) > Height then begin
      DefaultColWidth := ((Width - GetSystemMetrics(SM_CXVSCROLL)) div 2) - 2;
    end else begin
      DefaultColWidth := (Width div 2) - 2;
    end; { else }
  end; { with }
end;

procedure TDLDBXConfigureForm.editConnectionChange(Sender: TObject);
begin
  with Sender as TEdit do begin
    if (lboxConnection.Items.IndexOf(Text) >= 0) or
       (PriorItemIndex >= 0) then begin
      lboxConnection.ItemIndex := lboxConnection.Items.IndexOf(Text);
      lboxConnectionClick(lboxConnection);
    end; { if }
  end; { with }
end;

procedure TDLDBXConfigureForm.GetDataSources;
var
  I1: integer;
  ConnectionName: string;
  DataSourceObject: TDataSourceObject;
begin
  lboxConnection.Items.Clear;
  for I1 := 0 to ConnectionList.Count - 1 do begin
    ConnectionName := ConnectionList[I1];
    DataSourceObject := TDataSourceObject.Create;
    DataSourceObject.DataSource := BuildDataSource(ConnectionName);
    lboxConnection.Items.AddObject(ConnectionName, DataSourceObject);
  end; { for }
  ConnectionList.Assign(lboxConnection.Items);
end;

procedure TDLDBXConfigureForm.editConnectionKeyPress(Sender: TObject;
  var Key: AnsiChar);
begin
  PriorItemIndex := lboxConnection.ItemIndex;
end;

procedure TDLDBXConfigureForm.ShowDataSource;
var
  I1: integer;
  ConnectionList: TStringList;
begin
  ConnectionList := TStringList.Create;
  try
    ConnectionList.Text := DataSource;
    ConnectionName := ConnectionList.Values['ConnectionName'];
    with sgridConnectionSettings do begin
      if (ConnectionName = editConnection.Text) and (ConnectionName <> '') then begin
        RowCount := ConnectionList.Count + 1;
        Cells[0,0] := 'Key';
        Cells[1,0] := 'Value';
        for I1 := 1 to ConnectionList.Count do begin
          Cells[0,I1] := ConnectionList.Names[I1-1];
          Cells[1,I1] := ConnectionList.Values[ConnectionList.Names[I1-1]];
        end; { for }

      end else begin
        RowCount := 7;
        Cells[0,0] := 'Key';
        Cells[1,0] := 'Value';
        Cells[0,1] := 'ConnectionName';
        Cells[1,1] := '';
        Cells[0,2] := 'DriverName';
        Cells[1,2] := '';
        Cells[0,3] := 'LibraryName';
        Cells[1,3] := '';
        Cells[0,4] := 'VendorLib';
        Cells[1,4] := '';
        Cells[0,5] := 'GetDriverFunc';
        Cells[1,5] := '';
        Cells[0,6] := 'LoadParamsOnConnect';
        Cells[1,6] := '';
      end; { else }
    end; { with }
  finally
    FreeAndNil(ConnectionList);
  end;
end;

procedure TDLDBXConfigureForm.GetData(var DataSource, UserName, Password: string; OptionList: TStringList);
var
  I1: integer;
begin
  with sgridConnectionSettings do begin
    DataSource := '';
    for I1 := 1 to RowCount - 1 do begin
      DataSource := DataSource + Cells[0,I1] + '=' + Cells[1,I1] + #13;
      if CompareText(Cells[0,I1], 'User_Name') = 0 then begin
        UserName := Cells[1,I1];
      end else if CompareText(Cells[0,I1], 'Password') = 0 then begin
        Password := Cells[1,I1];
      end; { else }
    end; { for }
  end; { with }
end;

procedure TDLDBXConfigureForm.SetData(DataSource, UserName, Password: string; OptionList: TStringList);
begin
  self.FDataSource := DataSource;
  self.FUsername := UserName;
  self.FPassword := Password;
end;

initialization
  RvDLBase.ConnectionConfigureForm := TDLDBXConfigureForm;
end.