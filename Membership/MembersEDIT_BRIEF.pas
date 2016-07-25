unit MembersEDIT_BRIEF;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm, Menus, ExtCtrls, ToolWin, ComCtrls, StdCtrls, Mask, wwdbedit,
  wwSpeedButton, wwDBNavigator, wwclearpanel, wwdbdatetimepicker, Wwdotdot,
  Wwdbcomb,DB;

type
  TfrmMembersEditBrief = class(TSMStdForm)
    pnlSpace: TPanel;
    btnClose: TToolButton;
    pnlName: TPanel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    dbeFirstName: TwwDBEdit;
    dbeSurname: TwwDBEdit;
    dbeFullName: TwwDBEdit;
    Label2: TLabel;
    dbeMember: TwwDBEdit;
    Label16: TLabel;
    Label1: TLabel;
    dbdtDOB: TwwDBDateTimePicker;
    dbdtJoined: TwwDBDateTimePicker;
    Label23: TLabel;
    lblAddress: TLabel;
    dbeAddress: TwwDBEdit;
    lblAddress1: TLabel;
    dbeAddress1: TwwDBEdit;
    lblDistrict: TLabel;
    dbeDistrict: TwwDBEdit;
    lblCity: TLabel;
    dbeCity: TwwDBEdit;
    lblCountry: TLabel;
    dbeCountry: TwwDBEdit;
    Label44: TLabel;
    dbeEmail: TwwDBEdit;
    lblActive: TLabel;
    dbcbActive: TwwDBComboBox;
    Label20: TLabel;
    wwDBComboBox1: TwwDBComboBox;
    Label25: TLabel;
    Label13: TLabel;
    wwDBEdit6: TwwDBEdit;
    Label14: TLabel;
    wwDBEdit7: TwwDBEdit;
    dbdtExpires: TwwDBDateTimePicker;
    FULL_NAME: TCheckBox;
    ADDRESS: TCheckBox;
    SURNAME: TCheckBox;
    DISTRICT: TCheckBox;
    CITY: TCheckBox;
    COUNTRY: TCheckBox;
    ADDRESS1: TCheckBox;
    EMAIL: TCheckBox;
    FIRST_NAME: TCheckBox;
    IS_ACTIVE: TCheckBox;
    SUB_PAID: TCheckBox;
    EXPIRES: TCheckBox;
    DATE_OF_BIRTH: TCheckBox;
    JOINED: TCheckBox;
    HOME_PHONE: TCheckBox;
    WORK_PHONE: TCheckBox;
    ToolBar1: TToolBar;
    dbnQryMembers: TwwDBNavigator;
    dbnQryMembersEdit: TwwNavButton;
    dbnQryMembersPost: TwwNavButton;
    dbnQryMembersCancel: TwwNavButton;
    dbnQryMembersRefresh: TwwNavButton;
    btnSaveFields: TToolButton;
    btnEditFields: TToolButton;
    menuOptions: TMenuItem;
    menuEditFieldWatch: TMenuItem;
    menuSaveFieldWatch: TMenuItem;
    procedure btnCloseClick(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure dbeSurnameKeyPress(Sender: TObject; var Key: Char);
    procedure dbdtExpiresChange(Sender: TObject);
    procedure dbdtExpiresExit(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure menuSaveFieldWatchClick(Sender: TObject);
    procedure menuEditFieldWatchClick(Sender: TObject);
    procedure SetCheckBox( theState : Boolean );
    procedure Setup( ChangedFieldList : TStrings );
    procedure FormDestroy(Sender: TObject);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure FormDeactivate(Sender: TObject);
  private
    { Private declarations }
     FpnlWidth : SmallInt;
     FNeedsFormActivate,FChangeCheckedFields : Boolean;
     FChangedFieldList : TStringList;
  public
    { Public declarations }
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
    property NeedsFormActivate   : Boolean read FNeedsFormActivate;
    property ChangeCheckedFields : Boolean read FChangeCheckedFields;
  end;

var
  frmMembersEditBrief: TfrmMembersEditBrief;

implementation

{$R *.DFM}
uses
    Global,    // StockMaster
    dmMembers; // StockMaster
{ **************************************************************************** }
class function TfrmMembersEditBrief.ScreenCode : Integer;
begin Result := Global.scMembersEDIT_BRIEF; end;
{ **************************************************************************** }
class function TfrmMembersEditBrief.ScreenName : String;
begin Result := 'Membership Edit Brief'; end;
{ **************************************************************************** }
class function TfrmMembersEditBrief.ScreenDesc : String;
begin Result := '  Membership Edit Brief Screen'; end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.btnCloseClick(Sender: TObject);
begin
     if dbdtExpires.modified then
        FNeedsFormActivate := True;
     if dm.ibdsMembersBrief.State = dsEdit then begin
        dm.ibdsMembersBrief.Post;
     end;
     if btnSaveFields.Enabled then
        menuSaveFieldWatchClick(Sender);
     close;
end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.FormDeactivate(Sender: TObject);
begin
     inherited;
     dm.IBTransactionCommit; // Commit changes on deactivate
end;
{ ****************************************************************************}
procedure TfrmMembersEditBrief.FormResize(Sender: TObject);
begin
     ResizeToolBar( Self ,Self.WindowState,OneButtonWidth,FpnlWidth,pnlSpace);
end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.FormCreate(Sender: TObject);
var i : shortint;
begin
     inherited;
     if not dm.ibdsMembersBrief.Active then
        dm.ibdsMembersBrief.Open;
     FChangeCheckedFields := False;
     FpnlWidth := pnlSpace.Width;
     FNeedsFormActivate := False;
     stbStatus.Panels[0].Text := StateNames[1];
     SetLength(CheckedFields,dm.ibdsMembersBrief.FieldCount -1 );
     // load check box
     SetCheckBox(True);
     for i := ComponentCount -1 downto 0 do begin
        if (Components[i] is TCheckBox) then begin
           if CheckedFields[(Components[i] as TCheckBox).Tag] = varYes then
              (Components[i] as TCheckBox).Checked := True
           else
              (Components[i] as TCheckBox).Checked := False;
        end;
     end;
     SetCheckBox(False);
      // Capitals is Default see table MISC
    if not EditCapitals then begin
       for I := ComponentCount - 1 downto 0 do begin
          if Components[i].Tag = 99 then begin
             if Components[i] is TwwDBEdit then 
                TwwDBEdit(Components[i]).CharCase := ecNormal;
             //if Components[i] is TwwDBComboBox then 
             //   TwwDBComboBox(Components[i]).CharCase := ecNormal;
             // also the Find for LowerCase and on all forms 
            // if Components[i] is TwwDBLookupCombo then 
            //    TwwDBLookupCombo(Components[i]).CharCase := ecNormal;
          end;
       end;                          
    end;      
end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.dbeSurnameKeyPress(Sender: TObject;  var Key: Char);
begin
     if key = '''' then key := '`'; // change ' to `  - for sql
     if key = #13 then begin
        SelectNext(ActiveControl as TWinControl,True,True);
        key := #0;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.dbdtExpiresChange(Sender: TObject);
begin
     if TwwDBDateTimePicker(Sender).Modified then begin
        TwwDBDateTimePicker(Sender).DisplayFormat := 'dd/mm/yyyy';
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.dbdtExpiresExit(Sender: TObject);
begin
     (Sender as TwwDbDateTimePicker).DisplayFormat := 'dd mmm yyyy';
end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.FormShow(Sender: TObject);
var i,j : shortint;
    k : Integer;
begin
     FChangedFieldList.Sort;
     // dont need member has index of 0
     for i := 1 to dm.ibdsMembersBrief.FieldCount - 1 do begin
        if FChangedFieldList.Find(dm.ibdsMembersBrief.Fields[i].FieldName,k) then begin
           if dm.ibdsMembersBrief.FieldByName(FChangedFieldList[k]).IsNull then begin
              for j := ComponentCount -1 downto 0 do begin
                 //sm( (Components[j] as TwwDbCustomEdit).DataField  + ' ' + ChangedFieldList[k]);
                 if(Components[j] is TwwDbDateTimePicker) then begin
                    if(Components[j] as TwwDbDateTimePicker).DataField = FChangedFieldList[k] then begin
                       (Components[j] as TwwDbDateTimePicker).Color := clYellow;
                       if not dbeMember.Focused then
                          (Components[j] as TwwDbDateTimePicker).Setfocus;
                    end;
                 end else if Components[j].InheritsFrom(TCustomEdit) then begin
                    if(Components[j] as TwwDbCustomEdit).DataField = FChangedFieldList[k] then begin
                       (Components[j] as TwwDbCustomEdit).Color := clYellow;
                       (Components[j] as TwwDbCustomEdit).Setfocus;
                       if not dbeMember.Focused then
                          (Components[j] as TCustomEdit).Setfocus;
                    end;
                 end;
              end;
           end;
        end;
     end;
     Self.Update;
end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.menuSaveFieldWatchClick(Sender: TObject);
var i : ShortInt;
begin
     btnSaveFields.Enabled := False;
     btnEditFields.Enabled := True;
     for i := ComponentCount -1 downto 0 do begin
        if (Components[i] is TCheckBox) then begin
           if (Components[i] as TCheckBox).Checked then
              CheckedFields[(Components[i] as TCheckBox).Tag] := 'Y'
           else
              CheckedFields[(Components[i] as TCheckBox).Tag] := 'N';
        end;
     end;
     dm.StartTrans(dm.IBRunOnce);
     dm.sqlExecquery(dm.sqlRunOnce,format('Update MISC set CHECKED_FIELDS = ''%s''',[CheckedFields]));
     dm.IBRunOnce.Commit;
     FChangeCheckedFields := True;
     SetCheckBox(False);
end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.menuEditFieldWatchClick(Sender: TObject);
begin
     SetCheckBox(True);
     btnSaveFields.Enabled := true;
     btnEditFields.Enabled := False;
end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.SetCheckBox( theState : Boolean );
var i : integer;
begin
     for i := ComponentCount -1 downto 0 do begin
        if (Components[i] is TCheckBox) then begin
           (Components[i] as TCheckBox).Visible := theState;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.Setup( ChangedFieldList : TStrings);
begin
     FChangedFieldList := TStringList.Create;
     FChangedFieldList.AddStrings(ChangedFieldList);
end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.FormDestroy(Sender: TObject);
begin
     FChangedFieldList.Free;
end;
{ **************************************************************************** }
procedure TfrmMembersEditBrief.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     inherited;
     case key of
         VK_F9     : if dm.ibdsMembersBrief.State = dsEdit then dm.ibdsMembersBrief.Post;
         VK_ESCAPE : btnCloseClick(Sender);
     end;
end;
{ **************************************************************************** }

end.
