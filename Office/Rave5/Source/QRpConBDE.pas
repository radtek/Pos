{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpConBDE;

interface

uses
  QForms,
  SysUtils, Classes, DB, DBTables, QRpDefine, QRpCon, QRpConDS;

type
  TRvTableConnection = class(TRvCustomDataSetConnection)
  protected
    FTable: TTable;
    FUseSetRange: boolean;
    SaveMasterSource: TDataSource;
    SaveMasterFields: string;

    procedure Notification(AComponent: TComponent;
                           Operation: TOperation); override;
    procedure SetTable(Value: TTable);
    function GetDataSet: TDataSet; override;
  public
    constructor Create(AOwner: TComponent); override;
    procedure DoSetFilter; override;
    procedure DoGetSorts; override;
    procedure DoSetSort; override;
    procedure DoOpen; override;
    procedure DoRestore; override;
  published
    property LocalFilter default false;
    property Table: TTable read FTable write SetTable default nil;
    property UseSetRange: boolean read FUseSetRange write FUseSetRange default false;
  end; { TRvTableConnection }
  TRPTableConnection = TRvTableConnection; // Rave 4.0 compatibility


  TRvQueryConnection = class(TRvCustomDataSetConnection)
  protected
    FQuery: TQuery;

    procedure Notification(AComponent: TComponent; Operation: TOperation); override;
    procedure SetQuery(Value: TQuery);
    function GetDataSet: TDataSet; override;
  public
    constructor Create(AOwner: TComponent); override;
    procedure DoGetSorts; override;
    procedure DoSetSort; override;
  published
    property LocalFilter default false;
    property Query: TQuery read FQuery write SetQuery default nil;
  end; { TRvQueryConnection }
  TRPQueryConnection = TRvQueryConnection; // Rave 4.0 compatibility


implementation

{ class TRvTableConnection }

  constructor TRvTableConnection.Create(AOwner: TComponent);
  begin { Create }
    inherited Create(AOwner);
    LocalFilter := false;
  end;  { Create }

  procedure TRvTableConnection.Notification(AComponent: TComponent; Operation: TOperation);
  begin { Notification }
    inherited Notification(AComponent,Operation);
    if (Operation = opRemove) and (AComponent = FTable) then begin
      FTable := nil;
    end; { if }
  end;  { Notification }

  procedure TRvTableConnection.SetTable(Value: TTable);
  begin { SetTable }
    FTable := Value;
    if Assigned(Value) then begin
      Value.FreeNotification(self);
    end; { if }
  end;  { SetTable }

  function TRvTableConnection.GetDataSet: TDataSet;
  begin { GetDataSet }
    Result := Table;
  end;  { GetDataSet }

  procedure TRvTableConnection.DoSetFilter;
  var
    FieldName: string;
    FieldOp: string;
    FieldData: string;
  begin { DoSetFilter }
    if UseSetRange then begin
      with Table do begin
        SetRangeStart;
        while ReadFilterItem(FieldName,FieldOp,FieldData) do begin
          FieldByName(FieldName).AsString := FieldData;
        end; { while }
        ApplyRange;
      end; { with }
    end else begin
      inherited DoSetFilter;
    end; { else }
  end;  { DoSetFilter }

  procedure TRvTableConnection.DoGetSorts;
  begin { DoGetSorts }
  end;  { DoGetSorts }

  procedure TRvTableConnection.DoSetSort;
  var
    CalcSort: string;
    SortName: string;
  begin { DoSetSort }
    CalcSort := '';
    repeat
      SortName := ReadStr;
      if SortName <> '' then begin
        if CalcSort <> '' then begin
          CalcSort := CalcSort + ';';
        end; { if }
        CalcSort := CalcSort + SortName;
      end; { if }
    until SortName = '';
    Table.IndexFieldNames := CalcSort;
  end;  { DoSetSort }

  procedure TRvTableConnection.DoOpen;
  begin { DoOpen }
    inherited DoOpen;
		If DisableDataSource then begin
    	with Table do begin
      	SaveMasterSource := MasterSource;
      	SaveMasterFields := MasterFields;
      	if Assigned(MasterSource) and (MasterFields <> '') then begin
        	MasterSource := nil;
        	MasterFields := '';
      	end; { if }
    	end; { with }
		end; { if }
  end;  { DoOpen }

  procedure TRvTableConnection.DoRestore;
  begin { DoRestore }
		if DisableDataSource then begin
    	with Table do begin
      	MasterSource := SaveMasterSource;
      	MasterFields := SaveMasterFields;
    	end; { with }
		end; { if }
    inherited DoRestore;
  end;  { DoRestore }

{ class TRvQueryConnection }

  constructor TRvQueryConnection.Create(AOwner: TComponent);
  begin { Create }
    inherited Create(AOwner);
    LocalFilter := false;
  end;  { Create }

  procedure TRvQueryConnection.Notification(AComponent: TComponent; Operation: TOperation);
  begin { Notification }
    inherited Notification(AComponent,Operation);
    if (Operation = opRemove) and (AComponent = Query) then begin
      Query := nil;
    end; { if }
  end;  { Notification }

  procedure TRvQueryConnection.SetQuery(Value: TQuery);
  begin { SetQuery }
    FQuery := Value;
    if Assigned(Value) then begin
      Value.FreeNotification(self);
    end; { if }
  end;  { SetQuery }

  function TRvQueryConnection.GetDataSet: TDataSet;
  begin { GetDataSet }
    Result := Query;
  end;  { GetDataSet }

  procedure TRvQueryConnection.DoGetSorts;
  begin { DoGetSorts }
  end;  { DoGetSorts }

  procedure TRvQueryConnection.DoSetSort;
  begin { DoSetSort }
  end;  { DoSetSort }

end.