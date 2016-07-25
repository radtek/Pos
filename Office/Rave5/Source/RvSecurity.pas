{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvSecurity;

interface

uses
  Windows,
  Classes, SysUtils, RvClass, RvData, RvDefine;

type
  TRaveBaseSecurity = class(TRaveDataObject)
  public
    function IsValidUser(AUserName: string;
                         APassword: string): boolean; virtual; abstract;
  end; { TRaveBaseSecurity }

  TRaveSimpleSecurity = class(TRaveBaseSecurity)
  protected
    FUserList: TStrings;
    FCaseMatters: boolean;
    procedure SetUserList(Value: TStrings);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    function IsValidUser(AUserName: string;
                         APassword: string): boolean; override;
  published
    property UserList: TStrings read FUserList write SetUserList;
    property CaseMatters: boolean read FCaseMatters write FCaseMatters default false;
  end; { TRaveSimpleSecurity }

  TRaveLookupSecurity = class(TRaveBaseSecurity)
  protected
    FDataView: TRaveBaseDataView;
    FUserField: TRaveFieldName;
    FPasswordField: TRaveFieldName;
  public
    function IsValidUser(AUserName: string;
                         APassword: string): boolean; override;
  published
    property DataView: TRaveBaseDataView read FDataView write FDataView default nil;
    property UserField: TRaveFieldName read FUserField write FPasswordField;
    property PasswordField: TRaveFieldName read FPasswordField write FPasswordField;
  end; { TRaveLookupSecurity }

  procedure RaveRegister;

implementation

uses
  RpDefine;

procedure RaveRegister;
begin { RaveRegister }
  RegisterRaveComponents('',[TRaveSimpleSecurity,TRaveLookupSecurity]);
  RegisterRaveDataObject(Trans('Simple Security Controller'),TRaveSimpleSecurity);
  RegisterRaveDataObject(Trans('Data Lookup Security Controller'),TRaveLookupSecurity);
  RegisterRaveModuleClasses({Trans-}'RvData',[TRaveSimpleSecurity,TRaveLookupSecurity]);
end;

{ TRaveSimpleSecurity }

constructor TRaveSimpleSecurity.Create(AOwner: TComponent);
begin
  inherited;
  FUserList := TStringList.Create;
end;

destructor TRaveSimpleSecurity.Destroy;
begin
  FreeAndNil(FUserList);
  inherited;
end;

procedure TRaveSimpleSecurity.SetUserList(Value: TStrings);
begin
  FUserList.Assign(Value);
end;

function TRaveSimpleSecurity.IsValidUser(AUserName: string; APassword: string): boolean;
begin
  if APassword = '' then begin
    Result := false;
  end else if CaseMatters then begin
    Result := AnsiCompareStr(UserList.Values[AUserName], APassword) = 0;
  end else begin
    Result := AnsiCompareText(UserList.Values[AUserName], APassword) = 0;
  end;
end;

{ TRaveLookupSecurity }

function TRaveLookupSecurity.IsValidUser(AUserName: string; APassword: string): boolean;
begin
  if (APassword = '') or (AUserName = '') or (UserField = '') or (PasswordField = '') or
   not Assigned(DataView) then begin
    Result := false;
  end else begin
    Result := AnsiCompareStr(PerformLookup(DataView,AUserName,
     DataView.FieldByName(UserField),PasswordField,PasswordField,''),APassword) = 1;
  end; { else }
end;

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.
