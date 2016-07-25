{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRvDLDataSet;

{ Contains declarations common to all TDataSet based database backends }

interface

{$I RpVer.inc}

uses
  Windows,
  SysUtils, Classes, DB, QForms, QControls, QRvDLCommon, QRvDLBase;

type
  TDLDataSetConnection = class;
  TDLDataSetResultSet = class;

  TDLDataSetDriver = class(TDLBaseDriver)
  public
    procedure ResultSetGetRow(Params: PDLResultSetRow); override;
  end;

  TDLDataSetConnection = class(TDLBaseConnection)
  end;

  TDLDataSetResultSet = class(TDLBaseResultSet)
  private
    FDataSet: TDataSet;
    FFirstEOFCheck: boolean;
  protected
    function OpenDataSet(QueryStr: string): TDataSet; virtual; abstract;
  public
    procedure First; override;
    procedure Next; override;
    function Eof: boolean; override;
    procedure GetFields(AFields: TDLFieldListManager); override;
    procedure Open(QueryStr: string); override;
    procedure Close; override;
    //
    property DataSet: TDataSet read FDataSet;
  end;

implementation

const
  DataTypeMap : array[TFieldType] of byte =
    (dt_String,         {ftUnknown}
     dt_String,         {ftString}
     dt_Integer,        {ftSmallInt}
     dt_Integer,        {ftInteger}
     dt_Integer,        {ftWord}
     dt_Boolean,        {ftBoolean}
     dt_Float,          {ftFloat}
     dt_Currency,       {ftCurrency}
     dt_BCD,            {ftBCD}
     dt_Date,           {ftDate}
     dt_Time,           {ftTime}
     dt_DateTime,       {ftDateTime}
     dt_Blob,           {ftBytes}
     dt_Blob,           {ftVarBytes}
     dt_Integer,        {ftAutoInc}
     dt_Blob,           {ftBlob}
     dt_Memo,           {ftMemo}
     dt_Graphic,        {ftGraphic}
     dt_Blob,           {ftFmtMemo}
     dt_Blob,           {ftParadoxOle}
     dt_Blob,           {ftdBaseOle}
     dt_Blob,           {ftTypedBinary}
     dt_Blob,           {ftCursor}
     dt_String,         {ftFixedChar}
     dt_String,         {ftWideString}
     dt_Int64,          {ftLargeInt}
     dt_Blob,           {ftADT}
     dt_Blob,           {ftArray}
     dt_Blob,           {ftReference}
     dt_Blob{$IFDEF LEVEL5}, {ftDataSet}
     dt_Blob,           {ftOraBlob}
     dt_Blob,           {ftOraClob}
     dt_Blob,           {ftVariant}
     dt_Blob,           {ftInterface}
     dt_Blob,           {ftIDispatch}
     dt_String{$IFDEF LEVEL6}, {ftGUID}
     dt_DateTime,       {ftTimeStamp}
     dt_BCD{$ENDIF}{$ENDIF}); {ftFMTBcd}

{ TDLDataSetDriver }

procedure TDLDataSetDriver.ResultSetGetRow(Params : PDLResultSetRow);
type
  TGraphicHeader = record
    Count: word; { Fixed at 1 }
    HType: word; { Fixed at $0100 }
    Size: longint; { Size not including header }
  end; { TGraphicHeader }
var
  ResultSet: TDLDataSetResultSet;
  Index: longint;
  Stream: TMemoryStream;
  BlobBuf: PChar;
  BlobLen: longint;
  Header: TGraphicHeader;
begin
  ResultSet := nil;
  try
  { copy result set handle into convenient variable }
    ResultSet := Params.ResultSetHandle;

  { initialize buffer to all null values }
    ResultSet.FBuffer.Init(Params.TransferCount);

  { loop thru list of fields transfering those with flag set to True }
    for Index := 0 to Params.TransferCount-1 do begin
      if Params.TransferList[Index] then begin
        with ResultSet.DataSet.Fields[Index] do begin
          if IsNull then
            ResultSet.FBuffer.WriteNullData
          else case DatatypeMap[DataType] of
            dt_String: ResultSet.FBuffer.WriteStrData(DisplayText,AsString);
            dt_Integer: ResultSet.FBuffer.WriteLongData(DisplayText,AsInteger);
            dt_Boolean: ResultSet.FBuffer.WriteBoolData(DisplayText,AsBoolean);
            dt_Float: ResultSet.FBuffer.WriteFloatData(DisplayText,AsFloat);
            dt_Currency: ResultSet.FBuffer.WriteCurrData(DisplayText,AsCurrency);
            dt_BCD: ResultSet.FBuffer.WriteBCDData(DisplayText,AsCurrency);
            dt_Date, dt_Time, dt_DateTime: ResultSet.FBuffer.WriteDateTimeData(DisplayText,AsDateTime);
            dt_Blob, dt_Memo, dt_Graphic: begin
              Stream := TMemoryStream.Create;
              try
                if not ResultSet.FDataSet.Fields[Index].IsBLOB then begin
                  Stream.SetSize(ResultSet.FDataSet.Fields[Index].DataSize);
                  ResultSet.FDataSet.Fields[Index].GetData(Stream.Memory);
                end else begin
                  (ResultSet.FDataSet.Fields[Index] as TBlobField).SaveToStream(Stream);
                end; { else }
                BlobBuf := Stream.Memory;
                BlobLen := Stream.Size;
                if ((DataTypeMap[DataType] = dt_Graphic) or (DataTypeMap[DataType] = dt_Blob)) and
                 (BlobLen >= 8) then begin
                  Move(BlobBuf^,Header,SizeOf(Header));
                  if (Header.Count = 1) and (Header.HType = $0100) and
                   (Header.Size = (BlobLen - SizeOf(Header))) then begin
                    BlobBuf := pointer(PChar(BlobBuf) + 8);
                    BlobLen := BlobLen - 8;
                  end; { if }
                end; { if }
                ResultSet.FBuffer.WriteBlobData(BlobBuf^, BlobLen);
              finally
                Stream.Free;
              end; { tryf }
            end;
            dt_Int64: begin
              if ResultSet.DataSet.Fields[Index] is TLargeIntField then begin
                ResultSet.FBuffer.WriteInt64Data(DisplayText,TLargeIntField(ResultSet.DataSet.Fields[Index]).AsLargeInt);
              end else begin
                ResultSet.FBuffer.WriteInt64Data(DisplayText,AsInteger);
              end; { else }
            end;
          end; { case }
        end; { with }
      end; { if }
    end; { for }

  { if we get here then return pointer to buffer and size of buffer }
    Params.RowBuffer := ResultSet.FBuffer.Buffer;
    Params.RowBufSize := ResultSet.FBuffer.BufferSize;

  except
    on E: Exception do begin
    { if any exception then return error code and save error message }
      Params.ResultCode := -1;
      ResultSet.ErrorText := E.Message;
    end;
  end; { tryx }
end;

{ TDLDataSetResultSet }

procedure TDLDataSetResultSet.GetFields(AFields: TDLFieldListManager);
var
  Index: integer;
begin
  if Assigned(DataSet) then begin
  // Call this once to initialize the FieldList to the proper number of fields
    AFields.AllocFieldList(DataSet.FieldCount);
    for Index := 0 to DataSet.FieldCount - 1 do begin
      with DataSet.Fields[Index] do begin
      // Call this method once for each field in the FieldList
        AFields.SetFieldItem(Index,FieldName,DataTypeMap[DataType],DataSize,DisplayName,'');
      end; { with }
    end; { for }
  end else begin
    AFields.AllocFieldList(0);
  end; { else }
end;

function TDLDataSetResultSet.Eof: boolean;
begin
  if FFirstEOFCheck then begin
    Result := DataSet.Bof and DataSet.Eof;
    FFirstEOFCheck := false;
  end else begin
    Result := DataSet.Eof;
  end; { else }
end;

procedure TDLDataSetResultSet.First;
begin
  DataSet.First;
end;

procedure TDLDataSetResultSet.Next;
begin
  DataSet.Next;
end;

procedure TDLDataSetResultSet.Close;
begin
  if Assigned(FDataSet) then begin
  // Prevents a TDataSet exception.  Error message indicates we are at BOF or EOF of record was deleted
    FDataSet.First;

  // Close and destroy the data set
    FDataSet.Free;
    FDataSet := nil;
  end; { if }
end;

procedure TDLDataSetResultSet.Open(QueryStr: string);
begin
  Close; // Make sure existing DataSet is closed
  FDataSet := OpenDataSet(QueryStr);
end;

end.