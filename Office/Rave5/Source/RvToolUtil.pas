{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvToolUtil;

interface

uses
  Classes, SysUtils, Forms, StdCtrls, Buttons, IniFiles, TypInfo, Graphics;

type
  TRaveSettings = class
  protected
    FSectionList: TStringList;
    FSection: string;
    FIniFile: TMemIniFile;
    FChanged: boolean;
    FCacheSaves: boolean;
    //
    function GetSection: string;
  public
    constructor Create(FileName: string); virtual;
    destructor Destroy; override;

    function ReadInteger(AIdent: string; ADefault: integer): integer;
    function ReadBool(AIdent: string; ADefault: boolean): boolean;
    function ReadString(AIdent: string; ADefault: string): string;
    function ReadFloat(AIdent: string; ADefault: extended): extended;

    procedure WriteInteger(AIdent: string; AValue: integer);
    procedure WriteBool(AIdent: string; AValue: boolean);
    procedure WriteString(AIdent: string; AValue: string);
    procedure WriteFloat(AIdent: string; AValue: extended);

    function LockSection(ASection: string): TRaveSettings;
    procedure UnlockSection;
    procedure EraseSection;
    procedure ReadSectionValues(const ASection: string; AStrings: TStrings);
    procedure DeleteKey(AIdent: string);
    procedure Save;
    //
    property CacheSaves: boolean read FCacheSaves write FCacheSaves;
    property Changed: boolean read FChanged;
    property Section: string read GetSection;
  end; { TRaveSettings }

var
  RavePath: string; { File path for Rave IDE }
  TransCharset: {$IFDEF Level6}TFontCharset{$ELSE}byte{$ENDIF};

  function RaveSettings: TRaveSettings;
  procedure SaveToolWin(ToolWin: TForm);
  procedure PrepareToolWin(ToolWin: TForm);
  procedure RestoreToolWin(ToolWin: TForm);
  procedure FixupButtons(Form: TForm);
  procedure FormTrans(Form: TForm);

implementation

uses
  RpDefine,
  RvUtil;

var
  GlobalSettings: TRaveSettings = nil;

function RaveSettings: TRaveSettings;
begin { RaveSettings }
  if not Assigned(GlobalSettings) then begin
    GlobalSettings := TRaveSettings.Create(RavePath + 'Rave.ini');
  end; { if }
  Result := GlobalSettings;
end;  { RaveSettings }

{ TRaveSettings }

constructor TRaveSettings.Create(FileName: string);
begin
  inherited Create;
  FIniFile := TMemIniFile.Create(FileName);
  FSectionList := TStringList.Create;
end;

procedure TRaveSettings.DeleteKey(AIdent: string);
begin
  FIniFile.DeleteKey(Section,AIdent);
  FChanged := true;
end;

destructor TRaveSettings.Destroy;
begin
  Save;
  FreeAndNil(FIniFile);
  FreeAndNil(FSectionList);
  inherited;
end;

procedure TRaveSettings.EraseSection;
begin
  FIniFile.EraseSection(Section);
  FChanged := true;
end;

function TRaveSettings.GetSection: string;
begin
  Result := FSectionList[0];
end;

function TRaveSettings.LockSection(ASection: string): TRaveSettings;
begin
  Result := self;
  FSectionList.Insert(0,ASection);
end;

function TRaveSettings.ReadBool(AIdent: string; ADefault: boolean): boolean;
begin
  Result := FIniFile.ReadBool(Section,AIdent,ADefault);
end;

function TRaveSettings.ReadFloat(AIdent: string; ADefault: extended): extended;
begin
  Result := FIniFile.ReadFloat(Section,AIdent,ADefault);
end;

function TRaveSettings.ReadInteger(AIdent: string;
  ADefault: integer): integer;
begin
  Result := FIniFile.ReadInteger(Section,AIdent,ADefault);
end;

procedure TRaveSettings.ReadSectionValues(const ASection: string;
  AStrings: TStrings);
begin
  FIniFile.ReadSectionValues(ASection,AStrings);
end;

function TRaveSettings.ReadString(AIdent, ADefault: string): string;
begin
  Result := FIniFile.ReadString(Section,AIdent,ADefault);
end;

procedure TRaveSettings.Save;
begin
  FIniFile.UpdateFile;
  FChanged := false;
end;

procedure TRaveSettings.UnlockSection;
begin
  FSectionList.Delete(0);
  if Changed and not CacheSaves then begin
    Save;
  end; { if }
end;

procedure TRaveSettings.WriteBool(AIdent: string; AValue: boolean);
begin
  FIniFile.WriteBool(Section,AIdent,AValue);
  FChanged := true;
end;

procedure TRaveSettings.WriteFloat(AIdent: string; AValue: extended);
begin
  FIniFile.WriteFloat(Section,AIdent,AValue);
  FChanged := true;
end;

procedure TRaveSettings.WriteInteger(AIdent: string; AValue: integer);
begin
  FIniFile.WriteInteger(Section,AIdent,AValue);
  FChanged := true;
end;

procedure TRaveSettings.WriteString(AIdent, AValue: string);
begin
  FIniFile.WriteString(Section,AIdent,AValue);
  FChanged := true;
end;

{ Call this proc in a form's OnDestroy event to save the current state }

procedure SaveToolWin(ToolWin: TForm);

begin { SaveToolWin }
  with RaveSettings.LockSection({Trans-}'Dialogs_' + ToolWin.Name) do try
    WriteInteger({Trans-}'Left',ToolWin.Left);
    WriteInteger({Trans-}'Top',ToolWin.Top);
    if (ToolWin.BorderStyle = bsSizeToolWin) or (ToolWin.BorderStyle = bsSizeable) then begin
      WriteInteger({Trans-}'Height',ToolWin.Height);
      WriteInteger({Trans-}'Width',ToolWin.Width);
    end; { if }
  finally
    UnlockSection;
  end; { with }
end;  { SaveToolWin }

{ Call this proc in a form's OnCreate event to prepare the Position property }

procedure PrepareToolWin(ToolWin: TForm);

begin { PrepareToolWin }
  with RaveSettings.LockSection({Trans-}'Dialogs_' + ToolWin.Name) do try
    if ReadInteger({Trans-}'Left',-1) = -1 then begin // No settings yet
      ToolWin.Position := poScreenCenter;
    end else begin // Key already exists
      ToolWin.Position := poDesigned;
    end; { else }
  finally
    UnlockSection;
  end; { with }
end;  { PrepareToolWin }

{ Call this proc in a form's OnShow event to restore the forms state }

procedure RestoreToolWin(ToolWin: TForm);

begin { RestoreToolWin }
  with RaveSettings.LockSection({Trans-}'Dialogs_' + ToolWin.Name) do try
    ToolWin.Left := MinI(ReadInteger({Trans-}'Left',ToolWin.Left),Screen.Width - 32);
    ToolWin.Top := MinI(ReadInteger({Trans-}'Top',ToolWin.Top),Screen.Height - 32);
    if (ToolWin.BorderStyle = bsSizeToolWin) or (ToolWin.BorderStyle = bsSizeable) then begin
      ToolWin.Width := ReadInteger({Trans-}'Width',ToolWin.Width);
      ToolWin.Height := ReadInteger({Trans-}'Height',ToolWin.Height);
    end; { if }
  finally
    UnlockSection;
  end; { with }
end;  { RestoreToolWin }

procedure FixupButtons(Form: TForm);
var
  I1: integer;
begin { FixupButtons }
  for I1 := 0 to Form.ComponentCount - 1 do begin
    if Form.Components[I1] is TSpeedButton then begin
      with TSpeedButton(Form.Components[I1]) do begin
        if Assigned(Glyph) then begin
          Caption := ''; // Remove caption if a glyph is assigned
        end; { if }
      end; { with }
    end; { if }
  end; { for }
end;  { FixupButtons }

procedure FormTrans(Form: TForm);

var
  I1: integer;
  S1: string;
  S2: string;
  C1: TComponent;
  StringsType: PTypeInfo;

  procedure TransStrProp(Name: string);
  var
    PropInfo: PPropInfo;
  begin { TransStrProp }
    PropInfo := GetPropInfo(C1.ClassInfo,Name);
    if Assigned(PropInfo) and (PropInfo.PropType^.Kind = tkLString) then begin
      S1 := GetStrProp(C1,PropInfo);
      if S1 <> '' then begin
        S2 := Trans(S1);
        if S1 <> S2 then begin
          SetStrProp(C1,PropInfo,S2);
        end; { if }
      end; { if }
    end; { if }
  end;  { TransStrProp }

  procedure TransStringsProp(Name: string);
  var
    PropInfo: PPropInfo;
    StringList: TStringList;
    I1: integer;
  begin { TransStringsProp }
    PropInfo := GetPropInfo(C1.ClassInfo,Name);
    if Assigned(PropInfo) and (PropInfo.PropType^ = StringsType) then begin
      StringList := TStringList.Create;
      try
        StringList.Assign(TPersistent(GetOrdProp(C1,PropInfo)));
        for I1 := 0 to StringList.Count - 1 do begin
          StringList[I1] := Trans(StringList[I1]);
        end; { for }
        SetOrdProp(C1,PropInfo,integer(StringList));
      finally
        StringList.Free;
      end; { tryf }
    end; { if }
  end;  { TransStringsProp }

  procedure SetCharset;
  var
    PropInfo: PPropInfo;
  begin { SetCharset }
    PropInfo := GetPropInfo(C1.ClassInfo,'Font');
    if Assigned(PropInfo) and (PropInfo.PropType^ = TypeInfo(TFont)) then begin
      TFont(GetOrdProp(C1,PropInfo)).Charset := TransCharset;
    end; { if }
  end;  { SetCharset }

begin { FormTrans }
  Form.Font.Charset := TransCharset;
  if Addr(Trans) = Addr(DefaultTrans) then Exit;
  StringsType := TStrings.ClassInfo;
  C1 := Form;
  TransStrProp('Caption');
  for I1 := 0 to Form.ComponentCount - 1 do begin
    C1 := Form.Components[I1];
    SetCharset;
    TransStrProp('Caption');
    TransStrProp('Hint');
    TransStringsProp('Items');
  end; { for }
end;  { FormTrans }

initialization
  RavePath := ExtractFilePath(ParamStr(0));
finalization
  FreeAndNil(GlobalSettings);
end.