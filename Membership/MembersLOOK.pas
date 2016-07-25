unit MembersLOOK;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm, Grids, wwDataInspector, ComCtrls, DBCGrids, Wwdbigrd,
  Wwdbgrid, Buttons, ExtCtrls, lmdcompo, lmdclass, lmdnonvS,
  Menus, ToolWin, StdCtrls;

type
  TfrmMembersLook = class(TSMStdForm)
    pnlArea: TPanel;
    dbgSubs: TwwDBGrid;
    dbgSections: TwwDBGrid;
    dbgEvents: TwwDBGrid;
    ToolButton1: TToolButton;
    menuExit: TMenuItem;
    strGrid: TStringGrid;
    procedure FormCreate(Sender: TObject);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure menuExitClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormResize(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    LookupLinks : Boolean;
    Member : LongInt;
    PayExpires : TDateTime;
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;

var
  frmMembersLook: TfrmMembersLook;

implementation
{$R *.DFM}

uses
   Global,
   dmMembers;

{ **************************************************************************** }
class function TfrmMembersLook.ScreenCode : Integer;
begin Result := Global.scMembersLook; end;
{ **************************************************************************** }
class function TfrmMembersLook.ScreenName : String;
begin Result := 'Members Lookup'; end;
{ **************************************************************************** }
class function TfrmMembersLook.ScreenDesc : String;
begin Result := 'Members Lookup Screen'; end;
{ **************************************************************************** }
procedure TfrmMembersLook.FormCreate(Sender: TObject);
begin
    inherited;
    if LookupLinks then Self.Caption := 'Members Attachments';
end;
{ **************************************************************************** }
procedure TfrmMembersLook.FormKeyUp(Sender: TObject; var Key: Word;  Shift: TShiftState);
begin
     inherited;
     case key of   
         VK_ESCAPE : menuExitClick(Sender);
     end; 
     if ( ssAlt in Shift ) then begin
        case key of 
           88,120  : menuExitClick(Sender); // x Close
        end;
     end; 
end;
{ **************************************************************************** }
procedure TfrmMembersLook.menuExitClick(Sender: TObject);
begin
     Close;
end;
{ **************************************************************************** }
procedure TfrmMembersLook.FormShow(Sender: TObject);
const SQL1 = 'select MEMBER,FULL_NAME,EXPIRES from MEMBERS where MAILING_LIST_CODE = %d';
      SQL2 = 'select MAILING_LIST_CODE from MEMBERS where MEMBER = %d';
      SQL3 = 'select MEMBER,PAY_DATE,AMOUNT_PAID from PAYMENTS_DET where MEMBER = %d ' +
             ' and PAY_DATE >= ''%s''';
      forMember = ' for Member ';
      m = 0;
      n = 1;
      o = 2;
var s : string; i : smallint;     
    tot : Extended;
begin
     if LookupLinks then begin
        strGrid.Visible    := True;
        dbgSections.Visible   := False;
        dbgEvents.Visible     := False;
        dbgSubs.Visible       := True;
        strGrid.Height        := pnlArea.Height div 2;
        dbgSubs.DataSource    := dm.dsrAttachSubs;
     end else begin   
        strGrid.Visible       := True;
        strGrid.Align         := alClient;
        
        dbgSections.Visible   := False;
        dbgEvents.Visible     := False;
        dbgSubs.Visible       := False;
        //dbgSubs.DataSource    := dm.dsrM_Subs;
        //dbgSubs.Hint := FieldNamesArray[0] + ForMember + dm.qryPayLook.FieldByName('MEMBER').AsString;
        //dbgSections.Hint := FieldNamesArray[1] + ForMember + dm.qryPayLook.FieldByName('MEMBER').AsString;
        //dbgEvents.Hint := FieldNamesArray[2] + ForMember + dm.qryPayLook.FieldByName('MEMBER').AsString;
     end;
     if LookupLinks then begin
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL2,[Member])) then
           Member := dm.qryGeneral.FieldByName('MAILING_LIST_CODE').AsInteger
        else
           exit;
        dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[Member]));
        s := '';
        strGrid.Cells[m,0] := 'Member'; strGrid.Cells[n,0] := 'Full Name'; strGrid.Cells[o,0] := 'Expires'; 
        i := 1;
        while not dm.qryGeneral.EOF do begin
           with strGrid do begin
              Cells[m,i] := dm.qryGeneral.FieldByName('MEMBER').AsString; 
              Cells[n,i] := dm.qryGeneral.FieldByName('FULL_NAME').AsString; 
              Cells[o,i] := GetShortMonthDate(dm.qryGeneral.FieldByName('EXPIRES').AsDateTime); 
              inc(i);
           end;
           s := s + dm.qryGeneral.FieldByName('MEMBER').AsString + ',';                       
           dm.qryGeneral.Next;
        end;
        system.delete(s,length(s),1); // remove last comma
        dm.qryAttachSubs.Close;
        dm.qryAttachSubs.SQL.Text := format('SELECT MEMBER ,SUBSCRIPTION, AMOUNT_PAID,' +
          'AMOUNT,PAID,EDIT_DATE,RECEIPT FROM M_SUBS WHERE MEMBER in ' +
          ' ( %s )',[s]); 
        dm.qryAttachSubs.Open;
     end else begin
        tot := 0.00;
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL3,[Member,GetShortDate(PayExpires)])) then begin
           strGrid.Cells[m,0] := 'Member'; strGrid.Cells[n,0] := 'Pay Date'; strGrid.Cells[o,0] := 'Amount Paid'; 
           i := 1;
           while not dm.qryGeneral.EOF do begin
              with strGrid do begin
                 Cells[m,i] := dm.qryGeneral.FieldByName('MEMBER').AsString; 
                 Cells[n,i] := formatDateTime('dd mmm yyyy',dm.qryGeneral.FieldByName('PAY_DATE').AsDate); 
                 Cells[o,i] := FormatFloat( '$##0.00',dm.qryGeneral.FieldByName('AMOUNT_PAID').AsFloat); 
                 tot := tot + dm.qryGeneral.FieldByName('AMOUNT_PAID').AsFloat;
                 inc(i);
              end;
              dm.qryGeneral.Next;
           end;
           strGrid.Cells[o,i] := FormatFloat( '$##0.00',tot);
        end;
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersLook.FormResize(Sender: TObject);
begin
     inherited;
     if LookupLinks then
        strGrid.Height := pnlArea.Height div 2;
end;

end.
