{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvPEQuery;

interface

uses
  Windows, Messages,
  SysUtils, Classes, Graphics, Controls, Forms, Dialogs, StdCtrls, ExtCtrls;

// Procs
  procedure RaveRegister;

implementation

uses
  RvData, RvDefine, RvUtil, RvClass, RvTool, RpDefine, RvDatabase,
  RvDriverDataView, RvDataLink;

type
  TRaveQueryPropertyEditor = class(TRavePropertyEditor)
  protected
    function GetOptions: TPropertyOptionsSet; override;
    procedure Edit; override;
    function GetValue: string; override;
  end;

  TRaveLinkTypePropertyEditor = class(TRaveStringPropertyEditor)
  protected
    function GetOptions: TPropertyOptionsSet; override;
    procedure GetValues(ValueList: TStrings); override;
  end;

procedure RaveRegister;
begin
{TransOff}
  RegisterRavePropertyEditor(TypeInfo(string), TRaveDriverDataView, 'Query', TRaveQueryPropertyEditor);
  RegisterRavePropertyEditor(TypeInfo(string), TRaveDatabase, 'LinkType', TRaveLinkTypePropertyEditor);
{TransOn}
end;

{ TRaveSQLPropertyEditor }

procedure TRaveQueryPropertyEditor.Edit;
var
  DataLink: TDataConnection;
  QueryCommand: string;
  LQueryStruct: string;
begin
  DataLink := TRaveDriverDataView(Instance[0]).Database.GetLink;
  try
    QueryCommand := TRaveDriverDataView(Instance[0]).Query;
    LQueryStruct := TRaveDriverDataview(Instance[0]).QueryStruct;
    DataLink.Open;
    if DataLink.ResultSetConfigure(QueryCommand, LQueryStruct) then begin
      TRaveDriverDataView(Instance[0]).Query := QueryCommand;
      TRaveDriverDataView(Instance[0]).QueryStruct := LQueryStruct;
      Modified;
      Screen.Cursor := crHourGlass;
      try
        CreateFields(TRaveDriverDataView(Instance[0]), nil, nil, true);
      finally
        Screen.Cursor := crDefault;
      end;
    end; { if }
  finally
    TRaveDriverDataView(Instance[0]).Database.ReleaseLink(DataLink);
  end; { with }
end;

function TRaveQueryPropertyEditor.GetOptions: TPropertyOptionsSet;
begin
  Result := [poReadOnly, poEditor];
end;

function TRaveQueryPropertyEditor.GetValue: string;
begin
  if GetStrValue(0) = '' then begin
    Result := '';
  end else begin
    Result := '(' + Trans('Query') + ')';
  end;
end;

{ TRaveLinkTypePropertyEditor }

procedure TRaveLinkTypePropertyEditor.GetValues(ValueList: TStrings);
var
  I1: integer;
begin
  ValueList.Clear;
  for I1 := 0 to GDataLinks.Count - 1 do begin
    ValueList.Add(GDataLinks[I1].InternalName);
  end; { for }
end;

function TRaveLinkTypePropertyEditor.GetOptions: TPropertyOptionsSet;
begin
  Result := [poReadOnly, poListing];
end;

initialization
  RegisterProc({Trans-}'RVCL', RaveRegister);
end.