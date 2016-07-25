unit MembersSEARCH;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Mask, wwdbedit, ExtCtrls;

type
  TfrmMembersSEARCH = class(TForm)
    btnStart: TButton;
    btnCabcel: TButton;
    gbxCopy: TGroupBox;
    Label56: TLabel;
    dbeCopyMember: TwwDBEdit;
    lblMemberNumber: TLabel;
    rgpMemberNumbers: TRadioGroup;
    btnSave: TButton;
    procedure btnStartClick(Sender: TObject);
    procedure dbeMinKeyPress(Sender: TObject; var Key: Char);
    procedure dbeMaxKeyPress(Sender: TObject; var Key: Char);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure FormShow(Sender: TObject);
    procedure btnSaveClick(Sender: TObject);
    //function GetCount ( tbl : shortstring ) : Integer;
  private
    { Private declarations }
    FCopyMember : LongInt;
    FSetup : Boolean;
  public
    { Public declarations }
    property GetCopyMember    : LongInt Read  FCopyMember;
    // form is used for Copy and Search for Next Unused Member
    property SetForSetup : Boolean Write FSetup Default False;
  end;

var
  frmMembersSEARCH: TfrmMembersSEARCH;

implementation

{$R *.DFM}
uses
    Global,dmMembers;

{ **************************************************************************** }
procedure TfrmMembersSEARCH.btnStartClick(Sender: TObject);
const SQl1 = 'Select MEMBER from Members where MEMBER = %s';
begin
     if dbeCopyMember.Text = '' then exit;
     if not dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[dbeCopyMember.Text])) then begin
        sm(NotAValidMemMsg);
        exit;
     end;
     FCopyMember := StrToInt(dbeCopyMember.Text);
     ModalResult   := mrOK;
end;
{ **************************************************************************** }
procedure TfrmMembersSEARCH.dbeMinKeyPress(Sender: TObject; var Key: Char);
begin
      if key = #13 then
         SelectNext(ActiveControl as TWinControl,True,True);
end;
{ **************************************************************************** }
procedure TfrmMembersSEARCH.dbeMaxKeyPress(Sender: TObject; var Key: Char);
begin
     if key = #13 then btnStartClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersSEARCH.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     if key = VK_ESCAPE then modalResult := mrCAncel;
end;
{ **************************************************************************** }
procedure TfrmMembersSEARCH.FormShow(Sender: TObject);
begin
     case FSetup of
        True  : begin
                  rgpMemberNumbers.Visible := True;
                  btnSave.Visible := True;
                  Self.Caption := 'Member Number Setup';
                  rgpMemberNumbers.ItemIndex := Ord(DefaultNumberTypes);
                end;
        False : begin
                  gbxCopy.Visible := True;
                  Self.Caption := 'Copy from a Member';
                  dbeCopyMember.SetFocus;
                end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSEARCH.btnSaveClick(Sender: TObject);
const SQL1 = 'Update MISC set DEFAULT_NUMBER = %d';
begin
     // save into Misc and Global
     SetNumberType(rgpMemberNumbers.ItemIndex);
     dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[rgpMemberNumbers.ItemIndex]));
     Close;
end;
{ ****************************************************************************}
end.
