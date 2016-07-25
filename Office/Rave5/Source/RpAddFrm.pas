{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpAddFrm;

interface

uses
  SysUtils, WinTypes, WinProcs, Messages, Classes, Graphics, Controls,
  Forms, Dialogs, StdCtrls, Buttons, DB, RpDefine;

type
  TAddColumnForm = class(TForm)
    ListBox1: TListBox;
    Label1: TLabel;
    BitBtn1: TBitBtn;
    BitBtn2: TBitBtn;
    BitBtn3: TBitBtn;
    procedure BitBtn2Click(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
  public
    DataSet: TDataSet;
    FieldList: TStringList;
    AddFields: boolean;
  end;

var
  AddColumnForm: TAddColumnForm;

implementation

{$R *.dfm}

{ TAddColumnForm }

procedure TAddColumnForm.BitBtn2Click(Sender: TObject);
var
  ColumnName: string;
begin
{ Ask for name of column to add }
  ColumnName := '';
  if InputQuery(Trans('Add Column'),Trans('Enter a description for this column'),
   ColumnName) then begin
    FieldList.Add(ColumnName);
    ModalResult := mrOK;
  end; { if }
end;

procedure TAddColumnForm.BitBtn1Click(Sender: TObject);
var
  I1: integer;
begin
{ Copy all selected items to FieldList }
  for I1 := 1 to ListBox1.Items.Count do begin
    if ListBox1.Selected[I1 - 1] then begin
      FieldList.Add(ListBox1.Items[I1 - 1]);
    end; { if }
    AddFields := true;
  end; { for }
end;

procedure TAddColumnForm.FormCreate(Sender: TObject);
begin
  FieldList := TStringList.Create;
  AddFields := false;
end;

procedure TAddColumnForm.FormShow(Sender: TObject);
begin
  if DataSet <> nil then begin
    DataSet.GetFieldNames(ListBox1.Items);
  end; { if }
end;

procedure TAddColumnForm.FormDestroy(Sender: TObject);
begin
  FreeAndNil(FieldList);
end;

end.