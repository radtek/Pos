{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvPEMail;

interface

uses
  Windows, Messages,
  SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, RvDefine, RvUtil, RvClass, RvCsStd, RvTool, RvPEDTxt, RpDefine,
  RvToolUtil;

type
  TMailMergeEditor = class(TForm)
    GroupBox1: TGroupBox;
    MailMergeLB: TListBox;
    AddBtn: TButton;
    DeleteBtn: TButton;
    gboxSettings: TGroupBox;
    SearchLabel: TLabel;
    SearchEdit: TEdit;
    ReplaceLabel: TLabel;
    ReplaceMemo: TMemo;
    EditBtn: TButton;
    OKBtn: TButton;
    CancelBtn: TButton;
    CaseCB: TCheckBox;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure MailMergeLBClick(Sender: TObject);
    procedure AddBtnClick(Sender: TObject);
    procedure DeleteBtnClick(Sender: TObject);
    procedure OKBtnClick(Sender: TObject);
    procedure CancelBtnClick(Sender: TObject);
    procedure MailMergeChange(Sender: TObject);
    procedure EditBtnClick(Sender: TObject);
  private
    FRaveMemo: TRaveMemo;
    FMailMergeItems: TRavePersistentList;
    SuspendChanging: boolean;
    procedure SetMailMergeItems(Value: TRavePersistentList);

    function MailMergeStr(MailMergeItem: TRaveMailMergeItem): string;
    procedure AddItem(MailMergeItem: TRaveMailMergeItem);
  public
    property MailMergeItems: TRavePersistentList read FMailMergeItems write SetMailMergeItems;
    property RaveMemo: TRaveMemo read FRaveMemo write FRaveMemo;
  end;

  procedure RaveRegister;

var
  MailMergeEditor: TMailMergeEditor;

implementation

{$R *.dfm}

type
  TRaveMailMergePropertyEditor = class(TRavePropertyEditor)
  protected
    function GetOptions: TPropertyOptionsSet; override;
    procedure Edit; override;
    function GetValue: string; override;
  end; { TRaveMailMergePropertyEditor }

procedure RaveRegister;

begin { RaveRegister }
{TransOff}
  RegisterRavePropertyEditor(TypeInfo(TRavePersistentList),
   TRaveMemo,'MailMergeItems',TRaveMailMergePropertyEditor);
{TransOn}
end;  { RaveRegister }

(*****************************************************************************}
( class TRaveMailMergePropertyEditor
(*****************************************************************************)

function TRaveMailMergePropertyEditor.GetOptions: TPropertyOptionsSet;

begin { GetOptions }
  Result := [poReadOnly,poEditor];
end;  { GetOptions }

function TRaveMailMergePropertyEditor.GetValue: string;

begin { GetValue }
  if TRavePersistentList(GetOrdValue(0)).Count > 0 then begin
    Result := '(' + Trans('Mail Merge Items') + ')';
  end else begin
    Result := '';
  end; { else }
end;  { GetValue }

procedure TRaveMailMergePropertyEditor.Edit;

begin { Edit }
  with TMailMergeEditor.Create(Application) do try
    RaveMemo := Instance[0] as TRaveMemo;
    MailMergeItems := TObject(GetOrdValue(0)) as TRavePersistentList;
    if ShowModal = mrOK then begin
      RaveMemo.MailMergeItems := MailMergeItems;
      Modified;
    end; { if }
  finally
    Free;
  end; { with }
end;  { Edit }

(*****************************************************************************}
( class TMailMergeEditor
(*****************************************************************************)

procedure TMailMergeEditor.SetMailMergeItems(Value: TRavePersistentList);

var
  I1: integer;
  MM1: TRaveMailMergeItem;
  MM2: TRaveMailMergeItem;

begin { SetMailMergeItems }
  ClearObjectList(FMailMergeItems);
  for I1 := 0 to Value.Count - 1 do begin
    MM1 := TRaveMailMergeItem(Value[I1]);
    MM2 := TRaveMailMergeItem.Create;
    MM2.SearchToken := MM1.SearchToken;
    MM2.ReplaceString := MM1.ReplaceString;
    MM2.CaseMatters := MM1.CaseMatters;
    FMailMergeItems.Add(MM2);
  end; { for }
end;  { SetMainMergeItems }

function TMailMergeEditor.MailMergeStr(MailMergeItem: TRaveMailMergeItem): string;

begin { MailMergeStr }
  Result := MailMergeItem.SearchToken + {Trans-}' (' + MailMergeItem.ReplaceString + ')'
end;  { MailMergeStr }

procedure TMailMergeEditor.AddItem(MailMergeItem: TRaveMailMergeItem);

begin { AddItem }
  if not MailMergeLB.Enabled then begin
    SetControlsEnabled(true,[MailMergeLB,DeleteBtn,gboxSettings]);
  end; { if }
  MailMergeLB.Items.AddObject(MailMergeStr(MailMergeItem),MailMergeItem);
end;  { AddItem }

procedure TMailMergeEditor.FormCreate(Sender: TObject);

begin
  FormTrans(self);
  PrepareToolWin(self);
  FMailMergeItems := TRavePersistentList.Create;
end;

procedure TMailMergeEditor.FormDestroy(Sender: TObject);

begin
  SaveToolWin(self);
  ClearObjectList(FMailMergeItems);
  FreeAndNil(FMailMergeItems);
end;

procedure TMailMergeEditor.FormShow(Sender: TObject);

var
  I1: integer;
  EnableFields: boolean;

begin
//!!! If Form.Scaled is false, then buttons don't show up in the correct location
  RestoreToolWin(self);

{ Copy MailMergeItems to MailMergeLB }
  for I1 := 0 to MailMergeItems.Count - 1 do begin
    AddItem(TRaveMailMergeItem(MailMergeItems[I1]));
  end; { for }
  EnableFields := MailMergeLB.Items.Count > 0;
  if EnableFields then begin
    MailMergeLB.ItemIndex := 0;
    MailMergeLBClick(MailMergeLB);
  end; { if }
  MailMergeItems.Clear;
  SetControlsEnabled(EnableFields,[MailMergeLB,DeleteBtn,gboxSettings]);
end;

procedure TMailMergeEditor.MailMergeLBClick(Sender: TObject);

begin
  if SuspendChanging then Exit;
  if MailMergeLB.ItemIndex >= 0 then begin
    with TRaveMailMergeItem(MailMergeLB.Items.Objects[MailMergeLB.ItemIndex]) do begin
      SuspendChanging := true;
      SearchEdit.Text := SearchToken;
      CaseCB.Checked := CaseMatters;
      ReplaceMemo.Text := ReplaceString;
      SuspendChanging := false;
    end; { with }
  end; { if }
end;

procedure TMailMergeEditor.AddBtnClick(Sender: TObject);

begin
  MailMergeLB.Sorted := false;
  AddItem(TRaveMailMergeItem.Create);
  MailMergeLB.ItemIndex := MailMergeLB.Items.Count - 1;
  MailMergeLBClick(MailMergeLB);
  SetControlsEnabled(true,[gboxSettings]);
  SearchEdit.SetFocus;
end;

procedure TMailMergeEditor.DeleteBtnClick(Sender: TObject);

var
  Idx: integer;

begin
  Idx := MailMergeLB.ItemIndex;
  if Idx >= 0 then begin
    SuspendChanging := true;
    TRaveMailMergeItem(MailMergeLB.Items.Objects[Idx]).Free;
    MailMergeLB.Items.Delete(Idx);
    if MailMergeLB.Items.Count = 0 then begin
      SetControlsEnabled(false,[MailMergeLB,DeleteBtn,gboxSettings]);
      SearchEdit.Text := '';
      ReplaceMemo.Text := '';
      SuspendChanging := false;
    end else begin
      if Idx > 0 then begin
        MailMergeLB.ItemIndex := Idx - 1;
      end else begin
        MailMergeLB.ItemIndex := 0;
      end; { else }
      SuspendChanging := false;
      MailMergeLBClick(MailMergeLB);
    end; { else }
  end; { if }
end;

procedure TMailMergeEditor.OKBtnClick(Sender: TObject);

var
  I1: integer;

begin
{ Copy MailMergeLB to MailMergeItems }
  for I1 := 0 to MailMergeLB.Items.Count - 1 do begin
    MailMergeItems.Add(MailMergeLB.Items.Objects[I1]);
  end; { for }
end;

procedure TMailMergeEditor.CancelBtnClick(Sender: TObject);

var
  I1: integer;

begin
{ Delete MailMergeItem objects in MailMergeLB }
  for I1 := 0 to MailMergeLB.Items.Count - 1 do begin
    TRaveMailMergeItem(MailMergeLB.Items.Objects[I1]).Free;
    MailMergeLB.Items.Objects[I1] := nil;
  end; { for }
end;

procedure TMailMergeEditor.MailMergeChange(Sender: TObject);

var
  MM1: TRaveMailMergeItem;
  I1: integer;

begin
  if SuspendChanging then Exit;
  MM1 := TObject(MailMergeLB.Items.Objects[MailMergeLB.ItemIndex]) as
   TRaveMailMergeItem;
  MM1.SearchToken := SearchEdit.Text;
  MM1.ReplaceString := ReplaceMemo.Text;
  MM1.CaseMatters := CaseCB.Checked;
  MailMergeLB.Sorted := false;
  MailMergeLB.Items[MailMergeLB.ItemIndex] := MailMergeStr(MM1);
  MailMergeLB.Sorted := true; { Resort MailMergeLB }
{ Find new ItemIndex }
  for I1 := 0 to MailMergeLB.Items.Count - 1 do begin
    if MM1 = TRaveMailMergeItem(MailMergeLB.Items.Objects[I1]) then begin
      if MailMergeLB.ItemIndex <> I1 then begin
        MailMergeLB.ItemIndex := I1;
      end; { if }
      Break;
    end; { if }
  end; { for }
end;

procedure TMailMergeEditor.EditBtnClick(Sender: TObject);

begin
  with TDataTextEditor.Create(Application) do try
    DataInst := RaveMemo;
    DataText := ReplaceMemo.Text;
    DataView := nil;
    if ShowModal = mrOK then begin
      ReplaceMemo.Text := DataText;
    end; { if }
  finally
    Free;
  end; { with }
end;

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.