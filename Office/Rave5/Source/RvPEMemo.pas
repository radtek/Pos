{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvPEMemo;

interface

uses
  Windows,Messages,
  SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, RvDefine, RvUtil, RvClass, RvTool, RpDefine, RvToolUtil;

type
  TMemoEditor = class(TForm)
    Memo1: TMemo;
    Panel1: TPanel;
    Button1: TButton;
    Button2: TButton;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure Memo1KeyPress(Sender: TObject; var Key: Char);
  private
    procedure SetMemoText(Value: string);
    function GetMemoText: string;
  public
    property MemoText: string read GetMemoText write SetMemoText;
  end;

// Procs
  procedure RaveRegister;

implementation

{$R *.dfm}

type
  TRaveMemoPropertyEditor = class(TRavePropertyEditor)
  protected
    function GetOptions: TPropertyOptionsSet; override;
    procedure Edit; override;
    function GetValue: string; override;
  end; { TRaveMemoPropertyEditor }

procedure RaveRegister;
begin { RaveRegister }
{TransOff}
  RegisterRavePropertyEditor(TypeInfo(TRaveMemoString), nil, '', TRaveMemoPropertyEditor);
{TransOn}
end;  { RaveRegister }

(*****************************************************************************}
( class TRaveMemoPropertyEditor
(*****************************************************************************)

function TRaveMemoPropertyEditor.GetOptions: TPropertyOptionsSet;
begin { GetOptions }
  Result := [poReadOnly,poEditor,poMultiSelect];
end;  { GetOptions }

function TRaveMemoPropertyEditor.GetValue: string;
begin { GetValue }
  if GetStrValue(0) = '' then begin
    Result := '';
  end else begin
    Result := '(' + Trans('Memo') + ')';
  end; { else }
end;  { GetValue }

procedure TRaveMemoPropertyEditor.Edit;
begin { Edit }
  with TMemoEditor.Create(Application) do try
    MemoText := GetStrValue(0);
    if ShowModal = mrOK then begin
      SetStrValue(MemoText);
    end; { if }
  finally
    Free;
  end; { with }
end;  { Edit }

(*****************************************************************************}
( class TMemoEditor
(*****************************************************************************)

procedure TMemoEditor.FormCreate(Sender: TObject);
begin
  FormTrans(self);
  PrepareToolWin(self);
end;

procedure TMemoEditor.FormDestroy(Sender: TObject);
begin
  SaveToolWin(self);
end;

procedure TMemoEditor.FormShow(Sender: TObject);
begin
  RestoreToolWin(self);
end;

procedure TMemoEditor.SetMemoText(Value: string);
begin { SetMemoText }
  Memo1.Text := Value;
end;  { SetMemoText }

function TMemoEditor.GetMemoText: string;
begin { GetMemoText }
  Result := Memo1.Text;
end;  { GetMemoText }

procedure TMemoEditor.Memo1KeyPress(Sender: TObject; var Key: Char);
begin
  if Key = #27 then begin { Escape }
    ModalResult := mrCancel;
  end; { if }
end;

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.