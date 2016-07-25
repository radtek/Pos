{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpConDS;

interface

{$I RpVer.inc}

uses
  Types, QForms,
  SysUtils, Classes, DB, QRpDefine, QRpCon;

type
  TRvCustomDataSetConnection = class(TRvCustomConnection)
  protected
    StateSaved: boolean;
    InitActive: boolean;
    SaveFilter: string;
    SaveFiltered: boolean;
    SaveBookmark: TBookmark;
    FirstEOFCheck: boolean;
		FDisableDataSource: boolean;
		FRestoreDataSet: boolean;
    function GetDataSet: TDataSet; virtual; abstract;
  public
		constructor Create(AOwner: TComponent); override;
    procedure DoFirst; override;
    procedure DoNext; override;
    function DoEOF: boolean; override;
    procedure DoGetCols; override;
    procedure DoGetRow; override;
    procedure DoSetFilter; override;
    procedure DoOpen; override;
    procedure DoRestore; override;
	published
		property DisableDataSource: boolean read FDisableDataSource write FDisableDataSource default true;
		property RestoreDataSet: boolean read FRestoreDataSet write FRestoreDataSet default true;
  end; { TRvCustomDataSetConnection }

  TRvDataSetConnection = class(TRvCustomDataSetConnection)
  protected
    FDataSet: TDataSet;
    procedure Notification(AComponent: TComponent; Operation: TOperation); override;
    procedure SetDataSet(Value: TDataSet);
    function GetDataSet: TDataSet; override;
  published
    property DataSet: TDataSet read FDataSet write SetDataSet default nil;
  end; { TRvDataSetConnection }
  TRPDataSetConnection = TRvDataSetConnection; // Rave 4.0 compatibility


implementation

{ class TRvCustomDataSetConnection }

  constructor TRvCustomDataSetConnection.Create(AOwner: TComponent);
	begin
		inherited;
		FRestoreDataSet := true;
		FDisableDataSource := true;
	end;

  procedure TRvCustomDataSetConnection.DoFirst;
  begin { DoFirst }
    GetDataSet.First;
  end;  { DoFirst }

  procedure TRvCustomDataSetConnection.DoNext;
  begin { DoNext }
    GetDataSet.Next;
  end;  { DoNext }

  function TRvCustomDataSetConnection.DoEOF: boolean;
  begin { DoEOF }
    if FirstEOFCheck then begin
      Result := GetDataSet.EOF and GetDataSet.BOF;
      FirstEOFCheck := false;
    end else begin
      Result := GetDataSet.EOF;
    end; { else }
  end;  { DoEOF }

  procedure TRvCustomDataSetConnection.DoGetCols;
  var
    I1: integer;
    RPDataType: TRPDataType;
  begin { DoGetCols }
    with GetDataSet do begin
      for I1 := 0 to FieldCount - 1 do begin
        with Fields[I1] do begin
          RPDataType := TRPDataSetType[DataType];
          if DisplayName = FieldName then begin
            WriteField(FieldName,RPDataType,DataSize,'','');
          end else begin
            WriteField(FieldName,RPDataType,DataSize,DisplayName,'');
          end; { else }
        end; { with }
      end; { for }
    end; { with }
  end;  { DoGetCols }

  procedure TRvCustomDataSetConnection.DoGetRow;
  type
    TGraphicHeader = record
      Count: word; { Fixed at 1 }
      HType: word; { Fixed at $0100 }
      Size: longint; { Size not including header }
    end; { TGraphicHeader }
  var
    I1: integer;
    I2: integer;
    Stream: TMemoryStream;
    P1: pointer;
    Header: TGraphicHeader;
  begin { DoGetRow }
    with GetDataSet do begin
      for I1 := 0 to FieldCount - 1 do begin
        with Fields[I1] do begin
          if not CurrentFieldDef.Included or IsNull then begin
            WriteNullData;
          end else begin
            case TRPDataSetType[DataType] of
               dtString: begin
                WriteStrData(DisplayText,AsString);
              end;
              dtInteger: begin
                WriteIntData(DisplayText,AsInteger);
              end;
              dtBoolean: begin
                WriteBoolData(DisplayText,AsBoolean);
              end;
              dtFloat: begin
                WriteFloatData(DisplayText,AsFloat);
              end;
              dtCurrency: begin
                WriteCurrData(DisplayText,AsCurrency);
              end;
              dtBCD: begin
                WriteBCDData(DisplayText,AsCurrency);
              end;
              dtDate, dtTime, dtDateTime: begin
                WriteDateTimeData(DisplayText,AsDateTime);
              end;
              dtBlob, dtMemo, dtGraphic: begin
                Stream := TMemoryStream.Create;
                try
                  if not Fields[I1].IsBLOB then begin
                    Stream.SetSize(Fields[I1].DataSize);
                    Fields[I1].GetData(Stream.Memory);
                  end else begin
                    (Fields[I1] as TBlobField).SaveToStream(Stream);
                  end; { else }
                  P1 := Stream.Memory;
                  I2 := Stream.Size;
                  if ((TRPDataSetType[DataType] = dtGraphic) or (TRPDataSetType[DataType] = dtBlob)) and
                   (I2 >= 8) then begin
                    Move(P1^,Header,SizeOf(Header));
                    if (Header.Count = 1) and (Header.HType = $0100) and
                     (Header.Size = (I2 - SizeOf(Header))) then begin
                      P1 := pointer(PChar(P1) + 8);
                      I2 := I2 - 8;
                    end; { if }
                  end; { if }
                  WriteBlobData(P1^,I2);
                finally
                  Stream.Free;
                end; { tryf }
              end;
              dtInt64: begin
                WriteInt64Data(DisplayText,(Fields[I1] as TLargeIntField).AsLargeInt);
              end;
            end; { case }
          end; { else }
        end; { with }
      end; { for }
    end; { with }
  end;  { DoGetRow }

  procedure TRvCustomDataSetConnection.DoSetFilter;
  var
    CalcFilter: string;
  begin { DoSetFilter }
    CalcFilter := '';
    while GetFilterItem do begin
      if CalcFilter <> '' then begin
        CalcFilter := CalcFilter + ' and ';
      end; { if }
      CalcFilter := CalcFilter + '[' + FilterFieldName + '] ' + FilterFieldOp +
       ' ' + QuotedStr(FilterFieldData);
    end; { while }
    with GetDataSet do begin
      Filter := CalcFilter;
      FilterOptions := [foNoPartialCompare];
      Filtered := (Filter <> '');
    end; { with }
  end;  { DoSetFilter }

  procedure TRvCustomDataSetConnection.DoOpen;
  begin { DoOpen }
    FirstEOFCheck := true;
    with GetDataSet do begin
      InitActive := Active;
      if Active and RestoreDataSet then begin
        SaveBookmark := GetBookmark;
      end; { if }
 			if DisableDataSource then begin
        DisableControls;
			end; { if }
			if RestoreDataSet then begin
        SaveFilter := Filter;
        SaveFiltered := Filtered;
			end; { if }
      if not Active then begin
        Active := true;
        First;
      end; { if }
    end; { with }
    StateSaved := true;
  end;  { DoOpen }

  procedure TRvCustomDataSetConnection.DoRestore;
  begin { DoRestore }
    if StateSaved then begin
      with GetDataSet do begin
        Active := InitActive;
				if DisableDataSource then begin
          EnableControls;
				end; { if }
				if RestoreDataSet then begin
        	Filter := SaveFilter;
        	Filtered := SaveFiltered;
        	if InitActive then begin
          	GotoBookmark(SaveBookmark);
          	FreeBookmark(SaveBookmark);
        	end; { if }
				end; { if }
      end; { with }
      StateSaved := false;
    end; { if }
  end;  { DoRestore }

{ class TRvDataSetConnection }

  procedure TRvDataSetConnection.Notification(AComponent: TComponent; Operation: TOperation);
  begin { Notification }
    inherited Notification(AComponent,Operation);
    if (Operation = opRemove) and (AComponent = FDataSet) then begin
      FDataSet := nil;
    end; { if }
  end;  { Notification }

  procedure TRvDataSetConnection.SetDataSet(Value: TDataSet);
  begin { SetDataSet }
    FDataSet := Value;
    if Assigned(Value) then begin
      Value.FreeNotification(self);
    end; { if }
  end;  { SetDataSet }

  function TRvDataSetConnection.GetDataSet: TDataSet;
  begin { GetDataSet }
    Result := FDataSet;
  end;  { GetDataSet }

end.