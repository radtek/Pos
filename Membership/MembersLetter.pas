unit MembersLetter;
{
NB   this form is used for Both Letters and Queries
     - so much for Security
}
interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm, Mask, wwdbedit, Wwdotdot, Wwdbcomb, StdCtrls, ComCtrls,
  wwriched, wwSpeedButton, wwDBNavigator, wwclearpanel, Buttons, ExtCtrls,
  Db, lmdcompo, lmdclass, lmdnonvS, Menus, ToolWin;

type
  TfrmMembersLetter = class(TSMStdForm)
    Panel5: TPanel;
    dbreLetter: TwwDBRichEdit;
    dbnLinks: TwwDBNavigator;
    dbnLinksInsert: TwwNavButton;
    dbnLinksDelete: TwwNavButton;
    dbnLinksEdit: TwwNavButton;
    dbnLinksPost: TwwNavButton;
    dbnLinksCancel: TwwNavButton;
    dbnLinksRefresh: TwwNavButton;
    ToolButton1: TToolButton;
    pnlSpace: TPanel;
    menuExit: TMenuItem;
    Panel1: TPanel;
    dbeNAme: TwwDBEdit;
    dbeComments: TwwDBEdit;
    lblFields: TLabel;
    lblLetter: TLabel;
    Panel3: TPanel;
    dbcbLetters: TwwDBComboBox;
    cbxFields: TComboBox;
    procedure FormCreate(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
    procedure dbcbLettersChange(Sender: TObject);
    procedure UpdateComboBox;
    procedure GetDatafromLetter( idx : smallint );
    procedure dbcbLettersExit(Sender: TObject);
    procedure dbreLetterDragDrop(Sender, Source: TObject; X, Y: Integer);
    procedure dbreLetterDragOver(Sender, Source: TObject; X, Y: Integer;
      State: TDragState; var Accept: Boolean);
    procedure dbreLetterEndDrag(Sender, Target: TObject; X, Y: Integer);
    procedure dbnLinksInsertClick(Sender: TObject);
    procedure dbnLinksDeleteClick(Sender: TObject);
    procedure dbreLetterCloseDialog(Form: TForm);
    procedure dbreLetterInitDialog(Form: TForm);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure menuExitClick(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure cbxFieldsStartDrag(Sender: TObject; var DragObject: TDragObject);
    procedure FormShow(Sender: TObject);
    procedure dbreLetterEnter(Sender: TObject);
  private
    { Private declarations }
    FFieldName : string;
    FpnlWidth : SmallInt;
    FUseAsLetter : Boolean;
  public
    { Public declarations }
    property  SetAsLetter : Boolean write FUseAsLetter;
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;

var
  frmMembersLetter: TfrmMembersLetter;

implementation

{$R *.DFM}

uses
     Global,
     dmMembers, MembersQUERY;
{ **************************************************************************** }
class function TfrmMembersLetter.ScreenCode : Integer;
begin Result := Global.scMembersLetter; end;
{ **************************************************************************** }
class function TfrmMembersLetter.ScreenName : String;
begin Result := 'Members Letter'; end;
{ **************************************************************************** }
class function TfrmMembersLetter.ScreenDesc : String;
begin Result := '  Members Letter Screen'; end;
{ **************************************************************************** }
procedure TfrmMembersLetter.FormCreate(Sender: TObject);
var i : smallint;
begin
     inherited;
     FpnlWidth := pnlSpace.Width;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.menuExitClick(Sender: TObject);
begin
     Close;             
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.FormActivate(Sender: TObject);
var i : smallint;
    FieldsList : TStringList;
const  LetterHint = 'Right click here to Edit the Letter - change fonts, sizes etc';
       QueryHint = 'Right click and Paste the Query';
begin
     inherited;
     case FUseAsLetter of
     True : begin
              //dbcbLetters.Clear;

              lblLetter.Caption := 'Letter';
              lblFields.Caption := 'Fields';
              cbxFields.Visible := True;
              dbeComments.Visible := False;
              dbnLinks.DataSource := dm.dsrLetters;
              dbeNAme.DataSource  := dm.dsrLetters;
              dbreLetter.PlainText := False;
              dbreLetter.DataSource  := dm.dsrLetters;
              dbreLetter.DataField   := 'LETTER';
              dm.ibdsLetters.Open;
              if cbxFields.Items.Count = 0 then begin
                 try
                    dm.qryReport2.Close;
                    dm.qryReport2.SQL.Text := 'Select * from Members';
                    dm.qryReport2.Open;
                    FieldsList := TStringList.Create;
                    for i := 0 to dm.qryReport2.FieldCount - 1 do 
                       FieldsList.add('  {' + dm.qryReport2.Fields[i].DisplayName + '}');
                    FieldsList.Sort;
                    cbxFields.items.addstrings(FieldsList);
                    cbxFields.ItemIndex := 0;
                 finally
                    dm.qryReport2.Close;
                    FieldsList.Free;
                 end;
              end;
              UpdateComboBox;
              dbreLetter.Hint := LetterHint;
              if dbcbLetters.Items.Count > 0 then begin
                 dbcbLetters.ItemIndex := 0; // does a state change
                 //dbcbLetters.Clear;
              end else
                 dbeName.SetFocus;
            end;
     False : begin
                lblLetter.Caption   := 'Query Name';
                lblFields.Caption   := 'Comments';
                cbxFields.Visible   := False;
                dbeComments.Visible := True;
                
                dbnLinks.DataSource    := dm.dsrQueries;
                dbeNAme.DataSource     := dm.dsrQueries;
                dbreLetter.PlainText   := True;
                dbreLetter.DataSource  := dm.dsrQueries;
                dbreLetter.DataField   := 'SQL';
                dm.ibdsQueries.Open;  
                UpdateComboBox;
                dbreLetter.Hint := QueryHint;
             end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.FormDeactivate(Sender: TObject);
begin
     inherited;
     case FUseAsLetter of
     True  : begin
               if dm.ibdsLetters.State = dsEdit then
                  dm.ibdsLetters.Post;
               dm.ibdsLetters.Close;
            end;
     False : begin
               if dm.ibdsQueries.State = dsEdit then
                  dm.ibdsQueries.Post;
               dm.ibdsQueries.Close;
             end; 
      end;       
     dm.IBTransactionCommit;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.dbcbLettersChange(Sender: TObject);
begin
     if not active then exit;
     case FUseAsLetter of
     True  : begin
               if dm.ibdsLetters.State = dsEdit then
                  dm.ibdsLetters.Post;
               GetDatafromLetter( dbcbLetters.ItemIndex );
             end;  
     False : begin
               if dm.ibdsQueries.State = dsEdit then
                  dm.ibdsQueries.Post;
               GetDatafromLetter( dbcbLetters.ItemIndex );
             end;                     
     end;
end;             
{ **************************************************************************** }
procedure TfrmMembersLetter.UpdateComboBox;
const SQL1 = 'SELECT NAME from %s';
var s : string[10];
begin      
     case FUseAsLetter of
        True  : s := LettersTableName;
        False : s := QueriesTableName;
     end;   
     with dm.qryGeneral do begin
        dbcbLetters.Text := '';
        dbcbLetters.Items.Clear;
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral ,format(SQL1,[s])) then begin
           while not EOF do begin
              dbcbLetters.Items.Add(FieldByName('NAME').AsString);
              next;
           end;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.GetDatafromLetter( idx : smallint );
const SQL1 = 'SELECT * from %s where NAME = ''%s'''; 
begin
     case FUseAsLetter of
        True  : dm.ibdsOpenCheckEOF(dm.ibdsLetters,format(SQL1,[LettersTableName,dbcbLetters.Items[idx]]));
        False : dm.ibdsOpenCheckEOF(dm.ibdsQueries,format(SQL1,[QueriesTableName,dbcbLetters.Items[idx]]));
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.dbcbLettersExit(Sender: TObject);
begin
     dbcbLetters.Clear;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.dbreLetterDragDrop(Sender, Source: TObject; X,  Y: Integer);
begin
      dbreLetter.Lines.Append(FFieldName);
      dbreLetter.DragMode := dmManual;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.dbreLetterDragOver(Sender, Source: TObject; X,
  Y: Integer; State: TDragState; var Accept: Boolean);
begin
     if (Source is TComboBox) then
        Accept := True
     else
        Accept := False;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.dbreLetterEndDrag(Sender, Target: TObject; X,  Y: Integer);
begin
    dbreLetter.Update;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.dbnLinksInsertClick(Sender: TObject);
begin
    dbeName.SetFocus;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.dbnLinksDeleteClick(Sender: TObject);
begin
     if FUseAsLetter then begin
        if dm.ibdsLetters.FieldByName('PROTECT').AsString = 'Y' then begin
           sm(ProtectedMsg);
           abort;
        end;   
     end;   
     dbcbLetters.Clear;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.dbreLetterCloseDialog(Form: TForm);
begin
     case FUseAsLetter of
        True  : if dm.ibdsLetters.State = dsEdit then
                   dm.ibdsLetters.Post;
        False  : if dm.ibdsQueries.State = dsEdit then
                   dm.ibdsQueries.Post;
     end;              
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.dbreLetterInitDialog(Form: TForm);
begin
     // maximize edit form
     Form.WindowState := wsMaximized;
     Form.Caption := ' Edit ' + dbeName.Text;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     inherited;
     //case key of   
     //    VK_ESCAPE : menuExitClick(Sender);
     //end; 
     if ( ssCtrl in shift )  then begin
        case key of 
           70,102  : dbcbLetters.SetFocus;
        end;
     end;
     if ( ssAlt in Shift ) then begin
        case key of 
           88,120  : menuExitClick(Sender); // x Close
        end;
     end;                              
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.FormResize(Sender: TObject);
begin
     ResizeToolBar( Self ,Self.WindowState,OneButtonWidth,FpnlWidth,pnlSpace);
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.cbxFieldsStartDrag(Sender: TObject;   var DragObject: TDragObject);
begin
     FFieldName := cbxFields.Text;
     dbreLetter.DragMode := dmAutomatic;
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.FormShow(Sender: TObject);
begin
     if not FUseAsLetter then 
        Self.Caption := 'Members QUERY'
     else
        Self.Caption := ScreenName;   
end;
{ **************************************************************************** }
procedure TfrmMembersLetter.dbreLetterEnter(Sender: TObject);
begin
     // paste from the SQL tab
     if not FUseAsLetter then begin
        if frmMembersQUERY <> nil then
        AddToClipboard(frmMembersQUERY.memoQuery.Text);
     end;   
end;
{ **************************************************************************** }
end.
