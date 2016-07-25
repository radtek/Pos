{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRvDriverDataView;

interface

{$I RpVer.inc}

uses
  Classes, QRvDataLink, QRvClass, QRvData, QRvDatabase, QRvDefine, QRvDataField;

type

  TRaveDriverDataView = class(TRaveBaseDataView)
  protected
    FActive: boolean;
    FDatabase: TRaveDatabase;
    FConnection: TDataConnection;
    FResultSet: TDataResultSet;
    FQuery: string;
    FQueryStruct: string;
    FQueryParams: TStrings;
    function FindRaveField(FieldName : string) : TRaveDataField;
    procedure GetRow(EventType: integer); override;
    procedure SetActive(const AValue: boolean);
    procedure Changing(OldItem: TRaveComponent; NewItem: TRaveComponent); override;
    procedure OverrideProperties(Filer: TRaveFiler); override;
    procedure ReadSQLProperty(Reader: TRaveReader);
    procedure ReadParamsProperty(Reader: TRaveReader);
    procedure SetQueryParams(Value: TStrings);
    function PrepareQuery(QueryStr: string): string;
  public
    procedure CreateFields(AFieldList: TList); override;
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Open; override;
    procedure Close; override;
    procedure SetFilter(FilterList: TStringList); override;
    procedure SetRemoteSort(SortList: TStringList); override;
    property Active: boolean read FActive write SetActive;
  published
    property Database: TRaveDatabase read FDatabase write FDatabase;
    property Query: string read FQuery write FQuery;
    property QueryParams: TStrings read FQueryParams write SetQueryParams;
    property QueryStruct: string read FQueryStruct write FQueryStruct;
  end; { TRaveDriverDataView }

  procedure RaveRegister;

implementation

uses
  QRpDefine, QRvUtil, SysUtils, TypInfo;

procedure RaveRegister;
  {-Register the TRaveDriverDataView}
begin
  RegisterClassAlias(TRaveDriverDataView,'TRaveSQLDataView');
  RegisterRaveComponents('',[TRaveDriverDataView]);
  RegisterRaveModuleClasses({Trans-}'RvData',[TRaveDriverDataView]);
  RegisterRaveDataObject(Trans('Driver Data View'),TRaveDriverDataView);
end;

{ TRaveDriverDataView }

constructor TRaveDriverDataView.Create(AOwner: TComponent);
begin
  inherited;
  FQueryParams := TStringList.Create;
end;

destructor TRaveDriverDataView.Destroy;
  {-Close result set and free dynamic structures}
begin
  Close;
  FreeAndNil(FQueryParams);
  inherited;
end; {TRaveDriverDataView.Destroy}

procedure TRaveDriverDataView.Close;
  {-Close result set and release link to Data connection}
begin
  if Active then begin
    FResultSet.Free;
    if Assigned(FDatabase) then begin
      FDatabase.ReleaseLink(FConnection);
      FDatabase.Close;
    end; { if }
    FResultSet := nil;
    FConnection := nil;
    DataOpened := false;
    FActive := false;
  end; {if}
end; {TRaveDriverDataView.Close}

procedure TRaveDriverDataView.CreateFields(AFieldList: TList);
  {-Create list of RAve data fields}
var
  FieldInfo : TRaveDataFieldInfo;
  Index     : integer;
begin
  {open the result set}
  Open;
  try
    {loop thru all fields in result set}
    for Index := 0 to FResultSet.FieldCount - 1 do begin
      {create and initialize rave data field object}
      FieldInfo := TRaveDataFieldInfo.Create;
      FieldInfo.FieldName := FResultSet.Fields[index].FieldName;
      FieldInfo.DataType := FResultSet.Fields[Index].DataType;
      FieldInfo.Width := FResultSet.Fields[Index].Width;
      FieldInfo.FullName := FResultSet.Fields[index].FullName;
      FieldInfo.Description := FResultSet.Fields[Index].Description;

      {add new rave data field to list}
      AFieldList.Add(FieldInfo);
    end; {for}

  finally
    {close the result set}
    Close;
  end; {try/finally}
end; {TRaveDriverDataView.CreateFields}

procedure TRaveDriverDataView.Changing(OldItem: TRaveComponent;
                                    NewItem: TRaveComponent);
  {-Handle database component being changed}
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(Database) and (OldItem = Database) then
    Database := NewItem as TRaveDatabase;
end;  {TRaveDriverDataView.Changing }

procedure TRaveDriverDataView.GetRow(EventType: integer);
var
  RowBuffer  : Pointer;
  RowBufSize : longint;
  DR         : TRaveDataRow;
begin
  {if the result set is currently open}
  if DataOpened then begin
    {process event type}
    case EventType of
      DataFirst  : FResultSet.First;
      DataNext   : FResultSet.Next;
      DataGetRow : ;
    end;
  {now see if we are at end of file}
    AtEOF := FResultSet.EOF;
  {if now at end of result set}
    if AtEOF then begin
    {if saving row buffer}
      if Saving then begin
      {add dummy record}
        if EventType = DataFirst then begin
          FEmpty := true;
          AddValueListItem;
        end; { if }

      {make this row as the last one}
        ValueListTail.DataRowType := rtLast;
      end; { if }
    end else begin
    {if not at end of result set}
      {ask result set to return pointer to row buffer and size of buffer}
      FResultSet.GetRow(RowBuffer, RowBufSize);

      {if saving row buffer}
      if Saving then begin
        AddValueListItem;
        if EventType = DATAFIRST then begin
          ValueListTail.DataRowType := rtFirst;
        end; { if }

        {keep track of current row object}
        DR := ValueListTail;
      end else begin
      {if not saving the use data view row buffer}
        DR := DataRow;
      end; { else }

      {transfer result set buffer into row buffer}
      DR.Init(RowBufSize);
      Move(RowBuffer^, DR.DataPtr^, RowBufSize);
    end; { else }
  end else begin
  {if result set not open then abort the report}
    AtEOF := true;
    AbortReport(self);
  end; { else }
end; {TRaveDriverDataView.GetRow}

function TRaveDriverDataView.FindRaveField(FieldName : string) : TRaveDataField;
  {-Attempt to find matching rave data field}
var
  Index : integer;
begin
  {loop thru all child components looking for matching rave fields}
  for Index := 0 to ChildCount-1 do begin
    Result := TRaveDataField(Child[Index]);
    if CompareText(Result.FieldName, FieldName) = 0then
      Exit;
  end; {if}

  {if we get here then field could not be found}
  Result := nil;
end; {TRaveDriverDataView.FindRaveField}

procedure TRaveDriverDataView.Open;
  {-Open data view by creating result set}
var
  ResultSetField : TResultSetField;
  RaveField      : TRaveDataField;
  Index          : integer;
begin
  {exit if the data view is already active}
  if Active then
    Exit;

  {if no database assigned then raise exception}
  if FDatabase = nil then begin
    raise exception.Create('Database is nil.');
  end; { if }

  {ensure database is open and get data connection}
  FDatabase.Open;
  FConnection := FDatabase.GetLink;

  {create and open the result set}
  FResultSet := TDataResultSet.Create(FConnection);
  FResultSet.CommandText := PrepareQuery(Query);
  FResultSet.Open;

  {build field name/rave field object list}
  FieldNameList.Clear;
  for Index := 0 to FResultSet.FieldCount - 1 do begin
    {look for matching rave data field component}
    ResultSetField := FResultSet.Fields[Index];
    RaveField := FindRaveField(AddJoinChars(ResultSetField.FieldName));

    if Assigned(RaveField) then begin
    { if data types still match then assign data array index }
      if RaveField.DataType = ResultSetField.DataType then begin
        RaveField.DataIndex := Index;
      end else begin
        {if data types don't match then report error}
        RaveError(Trans(Format({Trans+}'Field %0:s:%1:s.  Datatype expected: %2:s  Datatype found: %3:s',
         [Name,RaveField.FieldName,GetEnumName(TypeInfo(TRPDataType),Ord(RaveField.DataType)),
         GetEnumName(TypeInfo(TRPDataType),Ord(ResultSetField.DataType))])));
       end; { else }
       FieldNameList.AddObject(StripJoinChars(ResultSetField.FieldName),RaveField);
     end; { if }
  end; {for}

  {indicate the data view is currently open}
  DataOpened := true;
  FActive := true;
end; {TRaveDriverDataView.Open}

procedure TRaveDriverDataView.SetActive(const AValue: boolean);
  {-Open or close the data view}
begin
  if AValue then
    Open
  else
    Close;
end; {TRaveDriverDataView.SetActive}

procedure TRaveDriverDataView.SetFilter(FilterList: TStringList);
begin
  Close;
  Open;
end;

procedure TRaveDriverDataView.SetRemoteSort(SortList: TStringList);
begin
  raise Exception.Create('SetRemoteSort not supported.');
end;

procedure TRaveDriverDataView.OverrideProperties(Filer: TRaveFiler);
begin
  inherited;
  Filer.OverrideProperty('SQL',ReadSQLProperty,nil);
  Filer.OverrideProperty('Params',ReadParamsProperty,nil);
end;

procedure TRaveDriverDataView.ReadSQLProperty(Reader: TRaveReader);
var
  PropInfo: PPropInfo;
begin
// Read old SQL value into Query property
  PropInfo := GetPropInfo(ClassInfo,{Trans-}'Query');
  Reader.ReadProperty(self,PropInfo);
end;

procedure TRaveDriverDataView.ReadParamsProperty(Reader: TRaveReader);
var
  PropInfo: PPropInfo;
begin
  PropInfo := GetPropInfo(ClassInfo,{Trans-}'QueryParams');
  Reader.ReadProperty(self,PropInfo);
(*
  With Reader do begin
    ValueKind := TValueKind(StreamHelper.ReadByte);
    Case ValueKind of
      vkPropList: begin { Read a TStrings object }
        TempStrings := TStrings.Create;
        try
          ReadProperties(TempStrings);
          Description := TempStrings.Text;
        finally
          TempStrings.Free;
        end; { tryf }
      vkString: begin { Read a normal string }
        Description := StreamHelper.ReadString;
      end;
    end; { case }
  end; { with }
*)
end;

procedure TRaveDriverDataView.SetQueryParams(Value: TStrings);
begin
  FQueryParams.Assign(Value);
end;

function TRaveDriverDataView.PrepareQuery(QueryStr: string): string;

  function ReplaceAllParam(AString: string; const AParam, AValue: string): string;
  var
    I1, I2: Integer;
    S1: string;
  begin
    repeat
      I1 := Pos(':' + UpperCase(AParam),UpperCase(AString));
      if I1 > 0 then begin
        I2 := I1 + Length(AParam) + 1;
        while AString[I2] in ['a'..'z','A'..'Z','0'..'9','_'] do begin
          Inc(I2);
        end;
        S1 := S1 + Copy(AString,1,I1 - 1) + AValue;
        Delete(AString,1,I2 - 1);
      end else begin
        S1 := S1 + AString;
      end;
    until I1 = 0;
    Result := S1;
  end;

var
  I1: integer;
begin
// Replace any query params
  Result := QueryStr;
  for I1 := 0 to QueryParams.Count - 1 do begin
    Result := ReplaceAllParam(Result,Trim(QueryParams.Names[I1]),
     ProcessDataStr(self,nil,QueryParams.Values[QueryParams.Names[I1]]));
  end; { for }
end;

{ TRaveQueryParamsList }


initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.