{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvData;

interface

{$I RpVer.inc}

{.$DEFINE DBG} { Used for data connection thread debugging }

uses
  Windows, Controls, Forms, ComCtrls, Graphics, StdCtrls, Dialogs,
  SysUtils, Classes, RvDefine, RvUtil, RvClass, RpDefine, RvDataField;

type
  TRaveDataFieldInfo = class
    FieldName: string;
    DataType: TRPDataType;
    Width: integer;
    FullName: string;
    Description: string;
  end; { TRaveDataFieldInfo }

  TRaveBaseDataView = class(TRaveDataObject)
  protected
    DataOpened: boolean;
    FEmpty: boolean;
    FieldNameList: TStringList;
    FSaving: boolean;
    AtEOF: boolean;
    DataRow: TRaveDataRow;
    ValueListHead: TRaveDataRow;
    ValueListTail: TRaveDataRow;
    ValueListPtr: TRaveDataRow;
    StateSaveEOF: boolean;
    SaveValueListPtr: TRaveDataRow;
    SaveAtEOF: boolean;
    SaveSaving: boolean;
    //
    function AcceptChild(NewChild: TClass): boolean; override;
    procedure AddValueListItem;
    procedure AfterReport; override;
    procedure GetData(EventType: integer);
    procedure GetRow(EventType: integer); virtual; abstract;
    procedure NextValueListItem;
    procedure SetName(const NewName: TComponentName); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    function ActiveDataRow: TRaveDataRow;
    procedure Close; override;
    function FieldByName(FieldName: string): TRaveDataField;
    procedure First; virtual;
    procedure Next; virtual;
    procedure GetCurrentRow; virtual;
    function Eof: boolean; virtual;
    procedure CreateFields(AFieldList: TList); virtual; abstract;

    procedure SetFilter(FilterList: TStringList); virtual; abstract;
    procedure SetRemoteSort(SortList: TStringList); virtual; abstract;
    function CheckRowsLeft(ReqRows: integer): boolean; virtual;
    procedure SaveState; virtual;
    procedure RestoreState; virtual;
    function CheckBeforeEOF: boolean; virtual;
    procedure Peek(Amount: integer); virtual;
    procedure Unpeek; virtual;
    //
    property Empty: boolean read FEmpty;
    property Saving: boolean read FSaving;
  end; { TRaveBaseDataView }

  TRaveDataBuffer = class
  protected
    RowBuf: PChar;
    RowBufSize: integer;
    RowBufPtr: PChar;
    FieldIdx: integer;
    //
    FAddPad: integer;
    FFieldCount: integer;
    //
    function GetBufferSize: integer;
    function GetBuffer: pointer;
    procedure AddingNative;
    procedure AddingFormatted;
    procedure AllocateSpace(AddSize: longint);
    procedure WriteStr(Value: string);
    procedure WriteInt(Value: longint);
    procedure WriteBool(Value: boolean);
    procedure WriteFloat(Value: extended);
    procedure WriteCurr(Value: currency);
    procedure WriteDateTime(Value: TDateTime);
    procedure WriteBuf(var Buffer;
                           Len: integer);
  public
    constructor Create;
    destructor Destroy; override;
    procedure Init(AFieldCount: integer);
    procedure WriteNullData;
    procedure WriteStrData(FormatData: string;
                           NativeData: string);
    procedure WriteIntData(FormatData: string;
                           NativeData: integer);
    procedure WriteBoolData(FormatData: string;
                            NativeData: boolean);
    procedure WriteFloatData(FormatData: string;
                             NativeData: extended);
    procedure WriteCurrData(FormatData: string;
                            NativeData: currency);
    procedure WriteBCDData(FormatData: string;
                           NativeData: currency);
    procedure WriteDateTimeData(FormatData: string;
                                NativeData: TDateTime);
    procedure WriteBlobData(var Buffer;
                                Len: longint);
    //
    property AddPad: integer read FAddPad write FAddPad;
    property FieldCount: integer read FFieldCount;
    property BufferSize: integer read GetBufferSize;
    property Buffer: pointer read GetBuffer;
  end; { TRaveDataBuffer }

  function ProcessDataStr(Self: TRaveComponent;
                          DefaultDataView: TRaveBaseDataView;
                          Value: string): string;
  function CreateFieldName(DataViewName: string;
                           FieldName: string): string;
  procedure CreateFields(DataView: TRaveBaseDataView;
                         DeletedFields: TStringList;
                         ReplacedFields: TStringList;
                         DoCreate: boolean);
  function PerformLookup(LookupDataView: TRaveBaseDataView;
                         LookupValue: string;
                         LookupValueField: TRaveDataField;
                         LookupField: TRaveFieldName;
                         LookupDisplay: TRaveFieldName;
                         LookupInvalid: string): string;
  procedure DataViewFirst(DataView: TRaveBaseDataView;
                          DetailKey: TRaveFieldName;
                          MasterDataView: TRaveBaseDataView;
                          MasterKey: TRaveFieldName;
                          SortKey: string);

var
  CreateTextControlProc: function(AOwner: TComponent;DataField: TRaveDataField): TRaveControl = nil;
  CreateDataTextControlProc: function(AOwner: TComponent;DataField: TRaveDataField): TRaveControl = nil;
  CreateDataMemoControlProc: function(AOwner: TComponent;DataField: TRaveDataField): TRaveControl = nil;

implementation

uses
  RvProj,
  RpMBCS,
  TypInfo;

procedure ProcessDataStrEx(    Self: TRaveComponent;
                               DefaultDataView: TRaveBaseDataView;
                               Term: char;
                           var InVal: string;
                           var OutVal: string);

const
  ReportVarNum = 16;
  {TransOff}
  ReportVars: array[1..ReportVarNum] of string =
   ('DateShort','DateLong','DateUS','DateInter','TimeShort','TimeLong',
    'TimeAMPM','Time24','FirstPage','LastPage','TotalPages','CurrentPage',
    'RelativePage','PrinterName','DriverName','PortName');
  {TransOn}

var
  I1: integer;
  S1: string;
  S2: string;
  ParentName: string;
  O1: TRaveComponent;
  VarIdx: integer;
  OpCh: char;
  SepCh: char;
  DataValue: string;

  function FieldValue(DataView: TRaveBaseDataView; FieldName: string): string;
  var
    Field: TRaveDataField;
  begin { FieldValue }
    if Assigned(DataView) then begin
      Field := DataView.FieldByName(FieldName);
    end else begin
      Field := nil;
    end; { else }
    if Assigned(Field) then begin
      Result := Field.AsString;
    end else begin
      Result := {Trans-}'(invalid)';
    end; { else }
  end;  { FieldValue }

begin { ProcessDataStrEx }
  S1 := Trim(InVal);
  if (S1 <> '') and (S1[Length(S1)] = ';') then begin
    SetLength(S1,Length(S1)-1);
  end; { if }
  OutVal := '';
  repeat
    DataValue := '';
    if (S1 <> '') and (S1[1] in ['''','"']) then begin { Get string }
      DataValue := ExtractQuotedString(S1,S1[1]);
      if S1 <> '' then begin { Remove OpCh }
        OpCh := S1[1];
        MBCSDelete(S1,1,1);
      end; { if }
    end else begin { Get variable }
      S2 := CutWord(S1,OpCh,{Trans-}'+&()'); // Look for & character
      if S2 <> '' then begin
        ParentName := CutWord(S2,SepCh,'.');
        if S2 <> '' then begin { Process parent }
          S2 := StripJoinChars(S2);
          if AnsiCompareText(ParentName,{Trans-}'REPORT') = 0 then begin
          { Add on report variable }
            VarIdx := 0;
            for I1 := 1 to ReportVarNum do begin
              if AnsiCompareText(S2,ReportVars[I1]) = 0 then begin
                VarIdx := I1;
                Break;
              end; { if }
            end; { for }
            if VarIdx > 0 then begin
              case VarIdx of
                12: begin // Current Page
                  DataValue := IntToStr(TRaveProjectManager(Self.Project).BaseReport.CurrentPage);
                end;
                13: begin // Relative Page
                  DataValue := IntToStr(TRaveProjectManager(Self.Project).BaseReport.RelativePage);
                end;
                else begin
                  DataValue := TRaveProjectManager(Self.Project).BaseReport.Macro(TMacroID(VarIdx - 1));
                end;
              end; { else }
            end else begin
              DataValue := {Trans-}'(invalid)';
            end; { else }
          end else if AnsiCompareText(ParentName,{Trans-}'PIVAR') = 0 then begin
          { Add on data variable }
            DataValue := TRaveProjectManager(Self.Project).BaseReport.PIVar(S2);
          end else if AnsiCompareText(ParentName,{Trans-}'PARAM') = 0 then begin
          { Add on user parameter }
            DataValue := TRaveProjectManager(Self.Project).GetParam(S2);
            if OpCh = '(' then begin // Get default value
              ProcessDataStrEx(self,DefaultDataView,')',S1,S2);
              if DataValue = '' then begin
                DataValue := S2;
              end; { if }
            end; { if }
          end else begin { Look for DataView }
            O1 := TRaveProjectManager(Self.Project).FindRaveComponent(ParentName,
             TRaveProjectManager(Self.Project));
            if Assigned(O1) and (O1 is TRaveBaseDataView) then begin
              DataValue := FieldValue(TRaveBaseDataView(O1),S2);
            end else begin
            { Error, ParentName not found }
              DataValue := {Trans-}'(invalid)';
            end; { else }
          end; { else }
        end else begin { Lookup field in DefaultDataView }
          DataValue := FieldValue(DefaultDataView,ParentName);
        end; { else }
      end; { if }
    end; { else }
    OutVal := OutVal + DataValue;
    if (OpCh = '&') and (DataValue <> '') then begin
      OutVal := OutVal + ' ';
    end; { if }
  until (S1 = '') or (OpCh = Term);
  InVal := S1;
end;  { ProcessDataStrEx }

function ProcessDataStr(Self: TRaveComponent;
                        DefaultDataView: TRaveBaseDataView;
                        Value: string): string;
begin { ProcessDataStr }
  ProcessDataStrEx(Self,DefaultDataView,#0,Value,Result);
end;  { ProcessDataStr }

function CreateFieldName(DataViewName: string;
                         FieldName: string): string;
var
  I1: integer;
begin { CreateFieldName }
  // MBCS-SAFE
  // TODO: Is it?
  Result := StripJoinChars(FieldName);
  I1 := 1;
  while I1 <= Length(Result) do begin
    if Result[I1] in ['0'..'9','_','a'..'z','A'..'Z'] then begin
      Inc(I1);
    end else if Result[I1] = ' ' then begin
      Result[I1] := '_';
      Inc(I1);
    end else begin
      Delete(Result,I1,1);
    end; { else }
  end; { while }
  Result := DataViewName + Result;
end;  { CreateFieldName }

procedure CreateFields(DataView: TRaveBaseDataView;
                       DeletedFields: TStringList;
                       ReplacedFields: TStringList;
                       DoCreate: boolean);
var
  I1: integer;
  I2: integer;
  Field: TRaveDataField;
  NewField: boolean;
  CompName: string;
  OrigFields: TList;
  FieldInfoList: TList;
  FieldInfo: TRaveDataFieldInfo;
begin { CreateFields }
  OrigFields := TList.Create;
  FieldInfoList := TList.Create;

// Copy all fields over to DeletedFields
  OrigFields.Clear;
  for I1 := 0 to DataView.ChildCount - 1 do begin
    OrigFields.Add(DataView.Child[I1]);
  end; { while }

// Get list of fields
  DataView.CreateFields(FieldInfoList);

// Process list of fields
  for I1 := 0 to FieldInfoList.Count - 1 do begin
  // Get new field info item
    FieldInfo := TRaveDataFieldInfo(FieldInfoList[I1]);
    CompName := CreateFieldName(DataView.Name,FieldInfo.FieldName);

  // Figure out if this is a new field of not
    NewField := true;
    Field := nil;
    for I2 := 0 to OrigFields.Count - 1 do begin
      Field := TRaveDataField(OrigFields[I2]);
      if AnsiCompareText(Field.Name,CompName) = 0 then begin
        if Field.DataType = FieldInfo.DataType then begin { Reuse field }
          NewField := false;
          OrigFields.Remove(Field);
        end else begin { Replace field }
          if Assigned(ReplacedFields) then begin
            ReplacedFields.Add(Field.Name);
          end; { if }
          if DoCreate then begin { Replace field }
            DeleteComponent(Field);
            OrigFields.Remove(Field);
            Field.Free;
          end; { if }
        end; { else }
        Break;
      end; { if }
    end; { for }

  // Create/Remove field components
    if DoCreate then begin
      if NewField then begin
        Field := DataFieldTypes[FieldInfo.DataType].Create(DataView);
        Field.Parent := DataView;
        Field.Name := CompName;
      end; { if }
      Field.FieldName := FieldInfo.FieldName;
      Field.Size := FieldInfo.Width;
      if FieldInfo.FullName = '' then begin
        if Field.FullName = '' then begin
          Field.FullName := StripJoinChars(FieldInfo.FieldName);
        end; { if }
      end else begin
        Field.FullName := FieldInfo.FullName;
      end; { else }
      if FieldInfo.Description <> '' then begin
        Field.Description := FieldInfo.Description;
      end; { if }
      if NewField then begin
        AddComponent(Field);
      end; { if }
    end; { if }
  end; { for }

// Process lists
  if Assigned(OrigFields) then begin
    while OrigFields.Count > 0 do begin
      Field := TRaveDataField(OrigFields[0]);
      if Assigned(DeletedFields) then begin
        DeletedFields.Add(Field.Name);
      end; { if }
      if DoCreate then begin // Permanently delete
        DeleteComponent(Field);
        Field.Free;
      end; { if }
      OrigFields.Delete(0);
    end; { while }
    OrigFields.Free;
  end; { if }
end; { CreateFields }

function PerformLookup(LookupDataView: TRaveBaseDataView;
                       LookupValue: string;
                       LookupValueField: TRaveDataField;
                       LookupField: TRaveFieldName;
                       LookupDisplay: TRaveFieldName;
                       LookupInvalid: string): string;
var
  FilterList: TStringList;
begin { PerformLookup }
  FilterList := TStringList.Create;
  try
  { Process lookup }
    FilterList.Add(StripJoinChars(LookupField));
    FilterList.Add('=');
    FilterList.AddObject(LookupValue,LookupValueField);
    FilterList.Add('');
    LookupDataView.SetFilter(FilterList);
    LookupDataView.First;
    if LookupDataView.EOF then begin
      Result := LookupInvalid;
    end else begin
      Result := ProcessDataStr(LookupDataView,LookupDataView,LookupDisplay);
    end; { else }

  { Remove lookup }
    FilterList.Clear;
    FilterList.Add('');
    LookupDataView.SetFilter(FilterList);
  finally
    FilterList.Free;
  end; { tryf }
end;  { PerformLookup }

procedure DataViewFirst(DataView: TRaveBaseDataView;
                        DetailKey: TRaveFieldName;
                        MasterDataView: TRaveBaseDataView;
                        MasterKey: TRaveFieldName;
                        SortKey: string);
var
  S1: string;
  S2: string;
  FilterList: TStringList;
  MasterField: string;
  DetailField: string;
  SortList: TStringList;
  SortField: string;
  Field: TRaveDataField;
  OpCh: char;
  I1: integer;
  DotPos: integer;
begin { DataViewFirst }
  if Assigned(DataView) then begin
  { Set sort }
    if Assigned(DataView) and (SortKey <> '') then begin
      S1 := SortKey;
      SortList := TStringList.Create;
      try
        repeat
          SortField := CutWord(S1,OpCh,'+');
          if SortField <> '' then begin
            SortList.Add(StripJoinChars(SortField));
          end; { if }
        until SortField = '';
        SortList.Add(''); { Add terminating blank field name }
        DataView.SetRemoteSort(SortList);
      finally
        SortList.Free;
      end; { tryf }
    end; { if }

  { Set filter }
    if Assigned(MasterDataView) and Assigned(DataView) and (MasterKey <> '')
     and (DetailKey <> '') then begin
      S1 := DetailKey;
      S2 := MasterKey;
      FilterList := TStringList.Create;
      try
        repeat
          DetailField := CutWord(S1,OpCh,'+');
          MasterField := CutWord(S2,OpCh,'+');
          if DetailField <> '' then begin

          { Remove dataview. from DetailField if any }
          {!!! Warn if not a valid field in DetailDataView }
            DotPos := 0;
            for I1 := 1 to Length(DetailField) do begin
              if DetailField[I1] = '.' then begin
                DotPos := I1;
                Break;
              end else if DetailField[I1] = '<' then begin
                Break;
              end; { else }
            end; { for }
            if DotPos > 0 then begin
              Delete(DetailField,1,DotPos);
            end; { if }

            FilterList.Add(StripJoinChars(DetailField)); { FieldName }
            FilterList.Add('=');
            Field := MasterDataView.FieldByName(MasterField);
            if Assigned(Field) then begin { FieldData }
              FilterList.AddObject(Field.AsString,Field);
            end else begin
              FilterList.AddObject(ProcessDataStr(MasterDataView,MasterDataView,MasterField),nil);
            end; { else }
          end; { if }
        until (DetailField = '') or (MasterField = '');
        FilterList.Add(''); { Add terminating blank field name }
        DataView.SetFilter(FilterList);
      finally
        FilterList.Free;
      end; { tryf }
    end; { if }

  { Goto first record }
    DataView.First;
  end; { if }
end;  { DataViewFirst }

{ class TRaveBaseDataView }

constructor TRaveBaseDataView.Create(AOwner: TComponent);
begin
  inherited;
  DataRow := TRaveDataRow.Create;
  AtEOF := true;
  FieldNameList := TStringList.Create;
end;

destructor TRaveBaseDataView.Destroy;
begin
  inherited;
  FreeAndNil(FieldNameList);
  FreeAndNil(DataRow);
end;

function TRaveBaseDataView.AcceptChild(NewChild: TClass): boolean;
begin { AcceptChild }
  Result := NewChild.InheritsFrom(TRaveDataField);
end;  { AcceptChild }

function TRaveBaseDataView.ActiveDataRow: TRaveDataRow;
begin { ActiveDataRow }
  if Assigned(ValueListPtr) then begin
    Result := ValueListPtr;
  end else if Saving then begin
    Result := ValueListTail;
  end else begin
    Result := DataRow;
  end; { else }
end;  { ActiveDataRow }

procedure TRaveBaseDataView.NextValueListItem;
var
  TempPtr: TRaveDataRow;
begin { NextValueListItem }
  TempPtr := ValueListPtr;
  ValueListPtr := TempPtr.Next;

  if not Saving then begin
    if ValueListHead <> TempPtr then begin
      RaveError(Trans('ValueList mismatch.'));
    end; { if }
    ValueListHead := ValueListPtr;
    TempPtr.Free;
  end; { if }
end;  { NextValueListItem }

procedure TRaveBaseDataView.AddValueListItem;
var
  TempPtr: TRaveDataRow;
begin { AddValueListItem }
  TempPtr := TRaveDataRow.Create;
  TempPtr.DataRowType := rtMiddle;
  TempPtr.Next := nil;
  if Assigned(ValueListHead) then begin { Add to list }
    ValueListTail.Next := TempPtr;
  end else begin { Init ValueList }
    ValueListHead := TempPtr;
  end; { else }
  ValueListTail := TempPtr;
end;  { AddValueListItem }

procedure TRaveBaseDataView.GetData(EventType: integer);
begin { GetData }
  if Assigned(ValueListPtr) then begin
  { Get next ValueList item }
    if EventType = DATAFIRST then begin
      if not Assigned(ValueListPtr.DataPtr) then begin { Empty dataset }
        NextValueListItem;
        AtEOF := true;
        FEmpty := true;
      end; { if }
      Exit; { Quit with current data or EOF }
    end else if EventType = DATANEXT then begin
      AtEOF := ValueListPtr.DataRowType = rtLast;
      NextValueListItem;
      if AtEOF then Exit;
    end else begin { EventType = DATAGETROW }
      AtEOF := ValueListPtr.DataRowType = rtLast;
      FEmpty := not Assigned(ValueListPtr.DataPtr);
      if AtEOF then Exit;
    end; { else }
  end; { if }

  if not Assigned(ValueListPtr) then begin { Get data from data connection }
    Open;
    GetRow(EventType);
  end; { if }
end;  { GetData }

function TRaveBaseDataView.FieldByName(FieldName: string): TRaveDataField;
var
  I1: integer;
  S1: string;
begin { FieldByName }
  if not DataOpened then begin
    Open;
    // Orignially GetRow(current whatever)
    GetCurrentRow;
  end; { if }

{ Remove DataViewName. from beginning if it exists }
  S1 := Name + '.';
  if AnsiCompareText(S1,Copy(FieldName,1,Length(S1))) = 0 then begin
    Delete(FieldName,1,Length(S1));
  end; { if }

{ Find field component for this field name }
  I1 := FieldNameList.IndexOf(StripJoinChars(FieldName));
  if I1 >= 0 then begin
    Result := TRaveDataField(FieldNameList.Objects[I1]);
  end else begin
    Result := nil;
  end; { else }
end;  { FieldByName }

procedure TRaveBaseDataView.SetName(const NewName: TComponentName);
var
  OldName: TComponentName;
  I1: integer;
  Field: TRaveDataField;
begin { SetName }
  OldName := Name;
  inherited SetName(NewName);
{ Go through all data field components and change the name }
  for I1 := 0 to ChildCount - 1 do begin
    Field := TRaveDataField(Child[I1]);
    if AnsiCompareText(Field.Name,CreateFieldName(OldName,Field.FieldName)) = 0 then begin
      Field.Name := CreateFieldName(NewName,Field.FieldName);
    end; { if }
  end; { for }
end;  { SetName }

function TRaveBaseDataView.CheckBeforeEOF: boolean;
begin
  SaveSaving := Saving;
  SaveAtEOF := AtEOF;
  SaveValueListPtr := ValueListPtr;
  if not SaveSaving then begin
    SaveState;
  end; { if }

  Next;
  Result := EOF;

{ Move back to original position }
  AtEOF := SaveAtEOF;
  ValueListPtr := SaveValueListPtr;
  if not SaveSaving then begin
    RestoreState;
  end; { if }
  if Result then begin { Clear saved EOF record }
    DataRow := ValueListPtr;
    ValueListPtr := nil;
  end; { if }
end;

function TRaveBaseDataView.CheckRowsLeft(ReqRows: integer): boolean;
begin
  Peek(ReqRows);
  Result := not EOF;
  UnPeek;
end;

function TRaveBaseDataView.Eof: boolean;
begin
  Result := AtEOF;
end;

procedure TRaveBaseDataView.First;
begin
  FEmpty := false;
  AtEOF := false;
  GetData(DATAFIRST);
end;

procedure TRaveBaseDataView.GetCurrentRow;
begin
  AtEOF := false;
  GetData(DATAGETROW);
end;

procedure TRaveBaseDataView.Peek(Amount: integer);
var
  I1: integer;
begin { Peek }
  SaveSaving := Saving;
  SaveAtEOF := AtEOF;
  SaveValueListPtr := ValueListPtr;
  if not SaveSaving then begin
    SaveState;
  end; { if }

{ Read ahead Count rows and return true if successful }
  for I1 := 1 to Amount do begin
    Next;
    if EOF then Break;
  end; { for }
end;

procedure TRaveBaseDataView.RestoreState;
begin
  if not Saving then begin
    RaveError(Trans('DataView.RestoreState called when not saving.'));
  end; { if }
  FSaving := false;
  ValueListPtr := ValueListHead;
  AtEOF := StateSaveEOF;
end;

procedure TRaveBaseDataView.SaveState;
begin
  if Saving then begin
    RaveError(Trans('DataView.SaveState called while Saving.'));
  end; { if }
  FSaving := true;
{ Save current record }
  StateSaveEOF := AtEOF;
  if not Assigned(ValueListPtr) then begin
    if EOF then begin
      AddValueListItem;
      ValueListTail.DataRowType := rtLast;
    end else begin
      AddValueListItem;
      ValueListTail.DataRowType := rtFirst;
      DataRow.MoveTo(ValueListTail);
    end; { else }
  end; { if }
end;

procedure TRaveBaseDataView.Unpeek;
begin
{ Move back to original position }
  AtEOF := SaveAtEOF;
  ValueListPtr := SaveValueListPtr;
  if not SaveSaving then begin
    RestoreState;
  end; { if }
end;

procedure TRaveBaseDataView.Close;
begin
  inherited;
  if DataOpened then begin
    ValueListPtr := nil;
    ValueListHead := nil;
    ValueListTail := nil;
  end; { if }
end;

procedure TRaveBaseDataView.Next;
begin
  if not EOF then begin
    GetData(DATANEXT);
  end; { if }
end;

procedure TRaveBaseDataView.AfterReport;
begin
  inherited;
  if DataOpened then begin
    Close;
  end; { if }
end;

{ class TRaveDataBuffer }

constructor TRaveDataBuffer.Create;
begin { Create }
  inherited Create;
  FAddPad := 4096;
  RowBufSize := FAddPad;
  GetMem(RowBuf,RowBufSize);
end;  { Create }

destructor TRaveDataBuffer.Destroy;
begin { Destroy }
  if RowBufSize > 0 then begin
    FreeMem(RowBuf,RowBufSize);
  end; { if }
  inherited Destroy;
end;  { Destroy }

procedure TRaveDataBuffer.Init(AFieldCount: integer);
var
  I1: integer;
begin { Init }
  FFieldCount := AFieldCount;
  RowBufPtr := RowBuf;
  for I1 := 1 to FieldCount do begin
    WriteInt(0); { Offset for native data }
    WriteInt(0); { Offset for formatted data }
  end; { for }
  FieldIdx := 0;
end;  { Init }

function TRaveDataBuffer.GetBufferSize: integer;
begin { GetBufferSize }
  Result := RowBufPtr - RowBuf;
end;  { GetBufferSize }

function TRaveDataBuffer.GetBuffer: pointer;
begin { GetBuffer }
  Result := RowBuf;
end;  { GetBuffer }

procedure TRaveDataBuffer.AddingNative;
begin { AddingNative }
  integer(pointer(RowBuf + (FieldIdx * 8))^) := RowBufPtr - RowBuf;
end;  { AddingNative }

procedure TRaveDataBuffer.AddingFormatted;
begin { AddingFormatted }
  integer(pointer(RowBuf + (FieldIdx * 8) + 4)^) := RowBufPtr - RowBuf;
end;  { AddingFormatted }

procedure TRaveDataBuffer.AllocateSpace(AddSize: longint);
var
  NewRowBuf: PChar;
  NewSize: integer;
begin { AllocateSpace }
  if (RowBufPtr - RowBuf + AddSize) > RowBufSize then begin
  // Allocate new buffer
    NewSize := (RowBufPtr - RowBuf) + AddSize + FAddPad;
    GetMem(NewRowBuf,NewSize);

  // Copy data from old buffer to new
    Move(RowBuf^,NewRowBuf^,RowBufPtr - RowBuf);

  // Clean up
    RowBufPtr := NewRowBuf + (RowBufPtr - RowBuf);
    FreeMem(RowBuf,RowBufSize);
    RowBuf := NewRowBuf;
    RowBufSize := NewSize;
  end; { if }
end;  { AllocateSpace }

// MBCS-SAFE
//Length works as expected
procedure TRaveDataBuffer.WriteStr(Value: string);
var
  I1: integer;
begin { WriteStr }
  // MBCS-SAFE
  I1 := Length(Value);
  WriteInt(I1);
  if I1 > 0 then begin
    Move(Value[1],RowBufPtr^,I1);
    Inc(RowBufPtr,I1);
  end; { if }
end;  { WriteStr }

procedure TRaveDataBuffer.WriteInt(Value: longint);
begin { WriteInt }
  longint(pointer(RowBufPtr)^) := Value;
  Inc(RowBufPtr,SizeOf(longint));
end;  { WriteInt }

procedure TRaveDataBuffer.WriteBool(Value: boolean);
begin { WriteBool }
  boolean(pointer(RowBufPtr)^) := Value;
  Inc(RowBufPtr,SizeOf(boolean));
end;  { WriteBool }

procedure TRaveDataBuffer.WriteFloat(Value: extended);
begin { WriteFloat }
  extended(pointer(RowBufPtr)^) := Value;
  Inc(RowBufPtr,SizeOf(extended));
end;  { WriteFloat }

procedure TRaveDataBuffer.WriteCurr(Value: currency);
begin { WriteCurr }
  currency(pointer(RowBufPtr)^) := Value;
  Inc(RowBufPtr,SizeOf(currency));
end;  { WriteCurr }

procedure TRaveDataBuffer.WriteDateTime(Value: TDateTime);
begin { WriteDateTime }
  TDateTime(pointer(RowBufPtr)^) := Value;
  Inc(RowBufPtr,SizeOf(TDateTime));
end;  { WriteDateTime }

procedure TRaveDataBuffer.WriteBuf(var Buffer; Len: integer);
begin { WriteBuf }
 Move(Buffer,RowBufPtr^,Len);
 Inc(RowBufPtr,Len);
end;  { WriteBuf }

procedure TRaveDataBuffer.WriteNullData;
begin { WriteNullData }
  integer(pointer(RowBuf + (FieldIdx * 8))^) := -1;
  Inc(FieldIdx);
end;  { WriteNullData }

procedure TRaveDataBuffer.WriteStrData(FormatData: string;
                                         NativeData: string);
begin { WriteStrData }
  // MBCS-SAFE
  if (NativeData <> FormatData) and (FormatData <> '') then begin
    AllocateSpace(Length(FormatData) + Length(NativeData) + 8);
    AddingFormatted;
    WriteStr(FormatData);
  end else begin
    AllocateSpace(Length(NativeData) + 4);
  end; { else }
  AddingNative;
  WriteStr(NativeData);
  Inc(FieldIdx);
end;  { WriteStrData }

procedure TRaveDataBuffer.WriteIntData(FormatData: string; NativeData: integer);
begin { WriteIntData }
  if (IntToStr(NativeData) <> FormatData) and (FormatData <> '') then begin
    AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
    AddingFormatted;
    WriteStr(FormatData);
  end else begin
    AllocateSpace(SizeOf(NativeData));
  end; { else }
  AddingNative;
  WriteInt(NativeData);
  Inc(FieldIdx);
end;  { WriteIntData }

procedure TRaveDataBuffer.WriteBoolData(FormatData: string; NativeData: boolean);
begin { WriteBoolData }
  if FormatData <> '' then begin
    AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
    AddingFormatted;
    WriteStr(FormatData);
  end else begin
    AllocateSpace(SizeOf(NativeData));
  end; { else }
  AddingNative;
  WriteBool(NativeData);
  Inc(FieldIdx);
end;  { WriteBoolData }

procedure TRaveDataBuffer.WriteFloatData(FormatData: string; NativeData: extended);
begin { WriteFloatData }
  if FormatData <> '' then begin
    AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
    AddingFormatted;
    WriteStr(FormatData);
  end else begin
    AllocateSpace(SizeOf(NativeData));
  end; { else }
  AddingNative;
  WriteFloat(NativeData);
  Inc(FieldIdx);
end;  { WriteFloatData }

procedure TRaveDataBuffer.WriteCurrData(FormatData: string; NativeData: currency);
begin { WriteCurrData }
  if FormatData <> '' then begin
    AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
    AddingFormatted;
    WriteStr(FormatData);
  end else begin
    AllocateSpace(SizeOf(NativeData));
  end; { else }
  AddingNative;
  WriteCurr(NativeData);
  Inc(FieldIdx);
end;  { WriteCurrData }

procedure TRaveDataBuffer.WriteBCDData(FormatData: string; NativeData: currency);
begin { WriteBCDData }
  if FormatData <> '' then begin
    AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
    AddingFormatted;
    WriteStr(FormatData);
  end else begin
    AllocateSpace(SizeOf(NativeData));
  end; { else }
  AddingNative;
  WriteCurr(NativeData);
  Inc(FieldIdx);
end;  { WriteBCDData }

procedure TRaveDataBuffer.WriteDateTimeData(FormatData: string; NativeData: TDateTime);
begin { WriteDateTimeData }
  if FormatData <> '' then begin
    AllocateSpace(Length(FormatData) + 4 + SizeOf(NativeData));
    AddingFormatted;
    WriteStr(FormatData);
  end else begin
    AllocateSpace(SizeOf(NativeData));
  end; { else }
  AddingNative;
  WriteDateTime(NativeData);
  Inc(FieldIdx);
end;  { WriteDateTimeData }

procedure TRaveDataBuffer.WriteBlobData(var Buffer; Len: longint);
begin { WriteBlobData }
  AllocateSpace(Len + 4);
  AddingNative;
  WriteInt(Len);
  if Len > 0 then begin
    WriteBuf(Buffer,Len);
  end; { if }
  Inc(FieldIdx);
end;  { WriteBlobData }

end.
