{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvPEData;

interface

  procedure RaveRegister;

implementation

uses
  Classes, SysUtils, TypInfo, Dialogs, Forms, Controls, RvDefine, RvClass,
  RvProj, RvTool, RvData, RvDatabase, RvPEDTxt, RpDefine,
  RvDataField;

type
  TRaveDatabasePropertyEditor = class(TRaveComponentPropertyEditor)
  protected
    procedure SetValue(Value: string); override;
  public
    procedure GetValues(ValueList: TStrings); override;
  end; { TRaveDatabasePropertyEditor }

  TRaveDataViewPropertyEditor = class(TRaveComponentPropertyEditor)
  protected
    procedure SetValue(Value: string); override;
  public
    procedure GetValues(ValueList: TStrings); override;
  end; { TRaveDataViewPropertyEditor }

  TRaveDataFieldPropertyEditor = class(TRaveStringPropertyEditor)
  protected
    function GetOptions: TPropertyOptionsSet; override;
  public
    procedure GetValues(ValueList: TStrings); override;
    procedure Edit; override;
  end; { TRaveDataFieldPropertyEditor }

  procedure RaveRegister;

  begin { RaveRegister }
  {TransOff}
    RegisterRavePropertyEditor(TypeInfo(TRaveDatabase),nil,'',TRaveDatabasePropertyEditor);
    RegisterRavePropertyEditor(TypeInfo(TRaveBaseDataView),nil,'',TRaveDataViewPropertyEditor);
    RegisterRavePropertyEditor(TypeInfo(TRaveFieldName),nil,'',TRaveDataFieldPropertyEditor);
  {TransOn}
  end;  { RaveRegister }

(*****************************************************************************}
( class TRaveDatabasePropertyEditor
(*****************************************************************************)

  procedure TRaveDatabasePropertyEditor.GetValues(ValueList: TStrings);

  var
    I1: integer;

  begin { GetValues }
    ValueList.Clear;
  { Get list of databases in project }
    with ProjectManager do begin
      for I1 := 0 to DataObjectList.Count - 1 do begin
        if TObject(DataObjectList[I1]) is TRaveDatabase then begin
          ValueList.Add(TRaveDatabase(DataObjectList[I1]).Name);
        end; { if }
      end; { for }
    end; { with }
  end;  { GetValues }

  procedure TRaveDatabasePropertyEditor.SetValue(Value: string);

  var
    Value1: TRaveComponent;
    I1: integer;

  begin { SetValue }
    Value1 := nil;
    if Value <> '' then begin
      with ProjectManager do begin
        for I1 := 0 to DataObjectList.Count - 1 do begin
          if TObject(DataObjectList[I1]) is TRaveDatabase then begin
            if (CompareText(TRaveDatabase(DataObjectList[I1]).Name,Value) = 0) then begin
              Value1 := TRaveComponent(DataObjectList[I1]);
            end; { if }
          end; { if }
        end; { for }
      end; { with }
      if not Assigned(Value1) then begin
        MessageDlg(Trans(Format({Trans+}'"%s" is not a valid entry.',[Value])),mtError,[mbOk],0);
        Exit;
      end; { else }
    end; { if }
    SetOrdValue(longint(Value1));
  end;  { SetValue }

(*****************************************************************************}
( class TRaveDataViewPropertyEditor
(*****************************************************************************)

  procedure TRaveDataViewPropertyEditor.GetValues(ValueList: TStrings);

  var
    I1: integer;

  begin { GetValues }
    ValueList.Clear;
  { Get list of dataviews in project }
    with ProjectManager do begin
      for I1 := 0 to DataObjectList.Count - 1 do begin
        if TObject(DataObjectList[I1]) is TRaveBaseDataView then begin
          ValueList.Add(TRaveBaseDataView(DataObjectList[I1]).Name);
        end; { if }
      end; { for }
    end; { with }
  end;  { GetValues }

  procedure TRaveDataViewPropertyEditor.SetValue(Value: string);

  var
    Value1: TRaveComponent;
    I1: integer;

  begin { SetValue }
    Value1 := nil;
    if Value <> '' then begin
      with ProjectManager do begin
        for I1 := 0 to DataObjectList.Count - 1 do begin
          if TObject(DataObjectList[I1]) is TRaveBaseDataView then begin
            if (CompareText(TRaveBaseDataView(DataObjectList[I1]).Name,Value) = 0) then begin
              Value1 := TRaveComponent(DataObjectList[I1]);
            end; { if }
          end; { if }
        end; { for }
      end; { with }
      if not Assigned(Value1) then begin
        MessageDlg(Trans(Format({Trans+}'"%s" is not a valid entry.',[Value])),mtError,[mbOk],0);
        Exit;
      end; { else }
    end; { if }
    SetOrdValue(longint(Value1));
  end;  { SetValue }

(*****************************************************************************}
( class TRaveDataFieldPropertyEditor
(*****************************************************************************)

  function TRaveDataFieldPropertyEditor.GetOptions: TPropertyOptionsSet;

  begin { GetOptions }
    if (UserLevel = ulBeginner){or (Copy(Name,Length(Name) - 2,3) = 'Key')}then begin
      Result := [poListing];
    end else begin
      Result := [poListing,poEditor];
    end; { else }
  end;  { GetOptions }

  procedure TRaveDataFieldPropertyEditor.GetValues(ValueList: TStrings);

  var
    I1: integer;
    S1: string;
    PropInfo: PPropInfo;
    DataView: TRaveBaseDataView;
    DataViewName: string;

  begin { GetValues }
    ValueList.Clear;
  { Get list of data fields for the DataView property of Instance[0] }
    with ProjectManager do begin
    { Find value of DataView property, if it exists }
      S1 := self.Name;
    {TransOff}
      if Copy(S1,Length(S1) - 8,9) = 'DataField' then begin
        DataViewName := Copy(S1,1,Length(S1) - 9) + 'DataView';
      end else if Copy(S1,Length(S1) - 4,5) = 'Field' then begin
        DataViewName := Copy(S1,1,Length(S1) - 5) + 'DataView';
      end else if Copy(S1,Length(S1) - 2,3) = 'Key' then begin
        DataViewName := Copy(S1,1,Length(S1) - 3) + 'DataView';
      end else if Copy(S1,Length(S1) - 6,7) = 'Display' then begin
        DataViewName := Copy(S1,1,Length(S1) - 7) + 'DataView';
      end else begin
        DataViewName := 'DataView';
      end; { else }
    {TransOn}

      PropInfo := TypInfo.GetPropInfo(Instance[0].ClassInfo,DataViewName);
      if not Assigned(PropInfo) then begin
        PropInfo := TypInfo.GetPropInfo(Instance[0].ClassInfo,{Trans-}'DataView');
      end; { if }
      if Assigned(PropInfo) then begin { Has DataView property }
        I1 := GetOrdProp(Instance[0],PropInfo);
        if TObject(I1) is TRaveBaseDataView then begin
          DataView := TRaveBaseDataView(I1);
          for I1 := 0 to DataView.ChildCount - 1 do begin
            if DataView.Child[I1] is TRaveDataField then begin
              ValueList.Add(TRaveDataField(DataView.Child[I1]).FieldName);
            end; { if }
          end; { for }
        end; { if }
      end; { if }
    end; { with }
  end;  { GetValues }

  procedure TRaveDataFieldPropertyEditor.Edit;

  var
    I1: integer;
    PropInfo: PPropInfo;
    DataViewName: string;

  begin { Edit }
    with TDataTextEditor.Create(Application) do try
      DataInst := Instance[0];
    {TransOff}
      if self.Name = 'MasterKey' then begin
        DataViewName := 'MasterDataView';
      end else if self.Name = 'GroupKey' then begin
        DataViewName := 'GroupDataView';
      end else begin
        DataViewName := 'DataView';
      end; { else }
    {TransOn}
      PropInfo := TypInfo.GetPropInfo(Instance[0].ClassInfo,DataViewName);
      if Assigned(PropInfo) then begin { Has DataView property }
        I1 := GetOrdProp(Instance[0],PropInfo);
        if TObject(I1) is TRaveBaseDataView then begin
          DataView := TRaveBaseDataView(I1);
        end; { if }
      end; { if }
      DataText := GetStrValue(0);
      if ShowModal = mrOK then begin
        SetStrValue(DataText);
      end; { if }
    finally
      Free;
    end; { with }
  end;  { Edit }

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.