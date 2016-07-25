{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRvDatabase;

interface

uses
  Windows,
  Classes,
  QRvDataLink, QRvClass, QRvDefine;

type
  TLinkInfo = class(TObject)
  public
    Link: TDataConnection;
    ThreadID: THandle;
    UseCount: integer;
  end;

  TRaveDBAuth = class(TPersistent)
  protected
    FDatasource: string;
    FOptions: string;
    FPassword: string;
    FUsername: string;
  public
    procedure AssignTo(ADest: TPersistent); override;
    procedure Setup(ADatasource, AOptions, APassword, AUsername: string);
  published
    property Datasource: string read FDatasource write FDatasource;
    property Options: string read FOptions write FOptions;
    property Password: string read FPassword write FPassword;
    property Username: string read FUsername write FUsername;
  end;

  TRaveDatabase = class(TRaveDataObject)
  protected
    FAuth: TRaveDBAuth;
    FAuthDesign: TRaveDBAuth;
    FAuthRun: TRaveDBAuth;
    //TODO: Change TThreadList to use a TReadWriteSynchronizer
    FLinkList: TThreadList; // List of active Links
    FLinkPool: TThreadList; // List of available Links
    FLinkPoolSize: integer;
    FLinkType: string;
    //
    // ConstructLink is used to create new instances of TILLink
    function ConstructLink: TDataConnection;
    procedure SetAuthDesign(AValue: TRaveDBAuth);
    procedure SetAuthRun(AValue: TRaveDBAuth);
    class function UseMaster: boolean; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    function FindLinkForThread: TLinkInfo;
    function FindLink(ALink: TDataConnection): TLinkInfo;
    procedure InitPool;
    // GetLink is called by TRVSQLDataview to obtain a reference for a thread
    function GetLink: TDataConnection;
    procedure ReleaseLink(ALink: TDataConnection);

  published
    property AuthDesign: TRaveDBAuth read FAuthDesign write SetAuthDesign;
    property AuthRun: TRaveDBAuth read FAuthRun write SetAuthRun;
    property LinkPoolSize: integer read FLinkPoolSize write FLinkPoolSize;
    property LinkType: string read FLinkType write FLinkType;
  end;

// Procs
  procedure RaveRegister;

var
  GAuthRunOverride: boolean = false;

implementation

uses
  QRpDefine, SysUtils;

procedure RaveRegister;
begin { RaveRegister }
  RegisterRaveComponents('',[TRaveDatabase]);
  RegisterRaveDataObject(Trans('Database Connection'),TRaveDatabase);
  RegisterRaveModuleClasses({Trans-}'RvData',[TRaveDatabase]);
end;  { RaveRegister }

{ TRaveDatabase }

function TRaveDatabase.ConstructLink: TDataConnection;
begin
{ If global list of data links is allocated and at least one link }
  if Assigned(GDataLinks) and (GDataLinks.Count > 0) then begin
    Result := GDataLinks.CreateConnection(LinkType);
    with FAuth do begin
      Result.Datasource := DataSource;
      Result.Username := Username;
      Result.Password := Password;
      Result.Options := Options;
    end;
    Result.Open;
  end else begin
  { We have not loaded data links or there were no data links when we loaded }
    raise Exception.Create('No DataLink drivers have been loaded.  Use the DataLink' + LinkType +
     '.RVD driver file for the Rave server/IDE or include the unit RvDL' + LinkType + ' for applications.');
  end; { else }
end;

constructor TRaveDatabase.Create(AOwner: TComponent);
begin
  inherited;
  FAuthDesign := TRaveDBAuth.Create;
  FAuthRun := TRaveDBAuth.Create;
  if InDesigner or GAuthRunOverride then begin
    FAuth := AuthDesign;
  end else begin
    FAuth := AuthRun;
  end; { else }
end;

destructor TRaveDatabase.Destroy;
var
  I1: integer;
begin
  if Assigned(FLinkList) then begin
    with FLinkList.LockList do try
      for I1 := Count -1 downto 0 do begin
        ReleaseLink(TDataConnection(Items[I1]));
      end; { for }
    finally
      FLinkList.UnlockList;
    end; { with }
    FreeAndNil(FLinkList);
  end; { if }

  if Assigned(FLinkPool) then begin
    with FLinkPool.LockList do try
      for I1 := 0 to Count - 1 do begin
        TDataConnection(Items[I1]).Free;
      end; { for }
    finally
      FLinkPool.UnlockList;
    end; { with }
    FreeAndNil(FLinkPool);
  end;
  FreeAndNil(FAuthDesign);
  FreeAndNil(FAuthRun);
  inherited;
end;

function TRaveDatabase.FindLink(ALink: TDataConnection): TLinkInfo;
var
  i: integer;
begin
  Result := nil;
  if not Assigned(FLinkList) then Exit;
  with FLinkList.LockList do try
    for i := 0 to Count - 1 do begin
      if TLinkInfo(Items[i]).Link = ALink then begin
        result := TLinkInfo(Items[i]);
      end;
    end;
  finally FLinkList.UnlockList; end;
end;

function TRaveDatabase.FindLinkForThread: TLinkInfo;
var
  i: integer;
begin
  if not Assigned(FLinkList) then begin
    Result := nil;
  end else begin
    with FLinkList.LockList do try
      Result := nil;
      for i := 0 to Count - 1 do begin
        if TLinkInfo(Items[i]).ThreadID = GetCurrentThreadID then begin
          Result := TLinkInfo(Items[i]);
        end;
      end;
    finally FLinkList.UnlockList; end;
  end; { if }
end;

function TRaveDatabase.GetLink: TDataConnection;
var
  LLinkInfo: TLinkInfo;
  FPool: TList;
begin
  LLinkInfo := FindLinkForThread;
  if LLinkInfo = nil then begin
    LLinkInfo := TLinkInfo.Create;
    if FLinkPool <> nil then begin
      FPool := FLinkPool.LockList; try
        if FPool.Count > 0 then begin
          LLinkInfo.Link := TDataConnection(FPool[0]);
          FPool.Delete(0);
        end;
      finally FLinkPool.UnlockList; end;
    end;
    if LLinkInfo.Link = nil then begin
      LLinkInfo.Link := ConstructLink;
    end;
    if not Assigned(FLinkList) then begin
      FLinkList := TThreadList.Create;
    end; { if }
    FLinkList.Add(LLinkInfo);
  end;
  LLinkInfo.UseCount := LLinkInfo.UseCount + 1;
  result := LLinkInfo.Link;
end;

procedure TRaveDatabase.InitPool;
var
  i: integer;
  FPool: TList;
begin
  FLinkPool := TThreadList.Create;
  FPool := FLinkPool.LockList; try
    for i := 1 to LinkPoolSize do begin
      FPool.Add(ConstructLink);
    end;
  finally FLinkPool.UnlockList; end;
end;

procedure TRaveDatabase.ReleaseLink(ALink: TDataConnection);
var
  LLinkInfo: TLinkInfo;
  FPool: TList;
begin
  LLinkInfo := FindLink(ALink);
  if Assigned(LLinkInfo) then begin
    LLinkInfo.UseCount := LLinkInfo.UseCount - 1;
    if LLinkInfo.UseCount = 0 then begin
      if FLinkPool <> nil then begin
        FPool := FLinkPool.LockList; try
          if FPool.Count < LinkPoolSize then begin
            FPool.Add(LLinkInfo.Link);
          end else begin
            FreeAndNil(LLinkInfo.Link);
          end;
        finally FLinkPool.UnlockList; end;
      end else begin
        FreeAndNil(LLinkInfo.Link);
      end;
      FLinkList.Remove(LLinkInfo);
      FreeAndNil(LLinkInfo);
    end;
  end;
end;

procedure TRaveDatabase.SetAuthDesign(AValue: TRaveDBAuth);
begin
  FAuthDesign.Assign(AValue);
end;

procedure TRaveDatabase.SetAuthRun(AValue: TRaveDBAuth);
begin
  FAuthRun.Assign(AValue);
end;

class function TRaveDatabase.UseMaster: boolean;
begin
  Result := true;
end;

{ TRaveDBAuth }

procedure TRaveDBAuth.AssignTo(ADest: TPersistent);
var
  LDest: TRaveDBAuth;
begin
  if ADest is TRaveDBAuth then begin
    LDest := TRaveDBAuth(ADest);
    LDest.Datasource := Datasource;
    LDest.Options := Options;
    LDest.Password := Password;
    LDest.Username := Username;
  end else begin
    inherited;
  end;
end;

procedure TRaveDBAuth.Setup(ADatasource, AOptions, APassword, AUsername: string);
begin
  Datasource := ADatasource;
  Options := AOptions;
  Password := APassword;
  Username := AUsername;
end;

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.