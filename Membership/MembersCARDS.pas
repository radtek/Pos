unit MembersCARDS;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  wwdblook, StdCtrls, ComCtrls, Mask, wwdbedit, wwdbdatetimepicker,
  Wwdotdot, Wwdbcomb;

type
  TfrmMembersCARDS = class(TForm)
    edtNumberCards: TEdit;
    UpDown1: TUpDown;
    Label11: TLabel;
    Label4: TLabel;
    dblcMember: TwwDBLookupCombo;
    btnOKPrintCard: TButton;
    btnCancelPrintCard: TButton;
    btnSaveCard: TButton;
    dbeFullName: TwwDBEdit;
    Label1: TLabel;
    btnEncodeCard: TButton;
    btnCardEject: TButton;
    btnReadEncoding: TButton;
    dbdtExpires: TwwDBDateTimePicker;
    Label2: TLabel;
    btnResetEncoder: TButton;
    procedure dblcMemberDropDown(Sender: TObject);
    procedure btnCancelPrintCardClick(Sender: TObject);
    procedure btnOKPrintCardClick(Sender: TObject);
    procedure dblcMemberKeyPress(Sender: TObject; var Key: Char);
    procedure edtNumberCardsKeyPress(Sender: TObject; var Key: Char);
    procedure FormShow(Sender: TObject);
    procedure btnSaveCardClick(Sender: TObject);
    procedure btnEncodeCardClick(Sender: TObject);
    procedure btnCardEjectClick(Sender: TObject);
    procedure btnReadEncodingClick(Sender: TObject);
    procedure btnResetEncoderClick(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMembersCARDS: TfrmMembersCARDS;

implementation
{$R *.DFM}
uses
   dmMembers,
   MembersClasses,
   Global;
{ **************************************************************************** }
procedure TfrmMembersCARDS.FormDeactivate(Sender: TObject);
begin
     dm.IBTransactionCommit; // Commit changes on deactivate
end;
{ ****************************************************************************}
procedure TfrmMembersCARDS.dblcMemberDropDown(Sender: TObject);
var s : shortstring;// pre drop down selection
const SQl1 = 'select SURNAME,MEMBER,FULL_NAME,FIRST_NAME,ADDRESS,MAILING_LIST_CODE,EXPIRES,DONATION_MEMBER from MEMBERS ';
begin
     s := dblcMember.Text;
     if s = '' then exit;
     s := uppercase(s);
     dm.qryMembersLook.Close;
     try
         if not global.AllDigits( dblcMember.Text) then begin
           if s[1] = '1' then begin // search by first name
              delete(s,1,1);
              dblcMember.LookupField := 'FIRST_NAME';
              dm.qryMembersLook.SQL.Text := Format( SQL1 + ' where upper(FIRST_NAME) LIKE ''%s%s'' ORDER BY FIRST_NAME,FULL_NAME',[s,'%']);
           end else begin
              dblcMember.LookupField := 'SURNAME';
              dm.qryMembersLook.SQL.Text := Format(SQL1 + ' where upper(SURNAME) LIKE ''%s%s'' ORDER BY SURNAME,FULL_NAME',[s,'%']);
           end;
         end else begin
           dblcMember.LookupField := 'MEMBER';
           dm.qryMembersLook.SQL.Text := Format(SQL1 + ' where MEMBER = %d ORDER BY MEMBER',[StrToIntdef(s,0)]);
         end;
     finally
        dm.qryMembersLook.Open;
        dblcMember.Text := dm.qryMembersLook.FieldByName('MEMBER').AsString;                            
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersCARDS.btnOKPrintCardClick(Sender: TObject);
begin
     try
        dm.tblCardDetail.Open;
        Global.CardPrint(dblcMember.Text,'',StrToInt(edtNumberCards.text),dm.IBTransaction);
     finally
        dm.tblCardDetail.Filtered := False;
        dm.tblCardDetail.Close;
        dblcMember.Setfocus;
        dblcMember.SelectAll;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersCARDS.btnCancelPrintCardClick(Sender: TObject);
begin
     Close;
end;
{ **************************************************************************** }
procedure TfrmMembersCARDS.dblcMemberKeyPress(Sender: TObject;   var Key: Char);
begin
    if key = #13 then begin
       dblcMemberDropDown(Sender);
       if dm.qryMembersLook.EOF then begin
          sm(NotAValidMemMsg);
          dblcMember.SetFocus;
          dblcMember.SelectAll;
       end else begin
          edtNumberCards.Setfocus;
       end;   
    end;   
end;
{ **************************************************************************** }
procedure TfrmMembersCARDS.edtNumberCardsKeyPress(Sender: TObject;  var Key: Char);
begin
     if key = #13 then btnOKPrintCardClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersCARDS.FormShow(Sender: TObject);
begin
     dblcMember.Setfocus;
     if dm.qryMembersLook.RecordCount <> 0 then
        dblcMember.Text := dm.qryMembersLook.FieldByName('MEMBER').AsString;
end;
{ **************************************************************************** }
procedure TfrmMembersCARDS.btnSaveCardClick(Sender: TObject);
var CardMembers : TCardMembers;
begin
     if dm.qryMembersLook.RecordCount = 0 then exit;
     CardMembers.AddMemberInCardMembers(StrToInt(dblcMember.Text),StrToInt(edtNumberCards.Text));
end;
{ **************************************************************************** }
procedure TfrmMembersCARDS.btnEncodeCardClick(Sender: TObject);
begin
     if dblcMember.Text = '' then exit;
     Global.CardEncode( dblcMember.Text,dbdtExpires.Date);
end;
{ **************************************************************************** }
procedure TfrmMembersCARDS.btnCardEjectClick(Sender: TObject);
begin
     CardEject;
end;
{ **************************************************************************** }
procedure TfrmMembersCARDS.btnReadEncodingClick(Sender: TObject);
begin
     CardReadTrack2;
end;
{ **************************************************************************** }
procedure TfrmMembersCARDS.btnResetEncoderClick(Sender: TObject);
begin
     ResetEncoder;
end;
{ **************************************************************************** }

end.
                                           
