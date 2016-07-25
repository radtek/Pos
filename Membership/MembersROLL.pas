unit MembersROLL;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm, RxGrdCpt, ComCtrls, ExtCtrls, ImgList, wwSpeedButton,
  wwDBNavigator, wwclearpanel, StdCtrls, wwDialog, wwidlg, Mask, wwdbedit,
  Buttons, Grids, Wwdbigrd, Wwdbgrid, Wwkeycb,db,Printers, 
  lmdcompo, lmdclass, lmdnonvS, wwdbdatetimepicker, Menus, ToolWin,
  Wwdotdot, Wwdbcomb, DBGrids;

type
  TfrmMembersROLL = class(TSMStdForm)
    PageControl: TPageControl;
    tsMemberRoll: TTabSheet;
    tsSectionRoll: TTabSheet;
    tsLuckyMember: TTabSheet;
    Label3: TLabel;
    wwDBEdit2: TwwDBEdit;
    Panel2: TPanel;
    cbxAllSections: TCheckBox;
    cbxClearMember: TCheckBox;
    Panel3: TPanel;
    dbgSections: TwwDBGrid;
    btnClose: TToolButton;
    btnRollover: TToolButton;
    pnlSpace: TPanel;
    menuExit: TMenuItem;
    Panel1: TPanel;
    gbxOptions: TGroupBox;
    cbxNewsLetter: TCheckBox;
    cbxAnalyse: TCheckBox;
    Panel4: TPanel;
    dbgRollover: TwwDBGrid;
    rgpView: TRadioGroup;
    Panel5: TPanel;
    lblExpirySystem: TLabel;
    lblRollDate: TLabel;
    dbeLastRoll: TwwDBEdit;
    ToolButton1: TToolButton;
    btnCancelRollover: TToolButton;
    btnAcceptRollover: TToolButton;
    btnPrintRollover: TToolButton;
    ToolButton5: TToolButton;
    Options1: TMenuItem;
    menuStartRollover: TMenuItem;
    N1: TMenuItem;
    menuAcceptRollover: TMenuItem;
    menuCancelRollover: TMenuItem;
    N2: TMenuItem;
    menuPrintRollover: TMenuItem;
    dbeRevolveDays: TwwDBEdit;
    lblRevolveDays: TLabel;
    dbdtNextExpires: TwwDBDateTimePicker;
    lblNextExpires: TLabel;
    btnExportRollover: TToolButton;
    menuExportRollover: TMenuItem;
    dbgSubs: TwwDBGrid;
    Splitter2: TSplitter;
    cbxFinancial: TwwDBComboBox;
    btnLetterRollover: TToolButton;
    menuSubsDueFinalLetter: TMenuItem;
    cbxPrior: TCheckBox;
    N3: TMenuItem;
    menuReceiptRollover: TMenuItem;
    menuPointsCreditRollover: TMenuItem;
    menuNonFinancialMemberDelete: TMenuItem;
    N4: TMenuItem;
    N5: TMenuItem;
    procedure FormCreate(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure btnSectionClick(Sender: TObject);
    procedure btnLuckyClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure cbxAllSectionsClick(Sender: TObject);
    procedure PageControlChange(Sender: TObject);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure menuExitClick(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure dbdtNextExpiresChange(Sender: TObject);
    procedure dbdtNextExpiresExit(Sender: TObject);
    procedure menuStartRolloverClick(Sender: TObject);
    function HasSubKeepExpiresCurrent( Member : LongInt) : Boolean;
    procedure MemberRollover;
    procedure menuCancelRolloverClick(Sender: TObject);
    procedure menuAcceptRolloverClick(Sender: TObject);
    procedure SetButtonsState( b : boolean );
    procedure SetButtons( b : boolean );
    procedure dbgRolloverCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure rgpViewClick(Sender: TObject);
    procedure SelectIBDSRollover( ch : shortstring);
    procedure menuPrintRolloverClick(Sender: TObject);
    procedure menuExportRolloverClick(Sender: TObject);
    procedure menuSubsDueFinalLetterClick(Sender: TObject);
    procedure dbgRolloverRowChanged(Sender: TObject);
    procedure PointsRolloverReset;
    procedure RollbackPointsRollover;
    procedure btnReceiptRolloverClick(Sender: TObject);
    procedure menuPointsCreditRolloverClick(Sender: TObject);
    procedure menuNonFinancialMemberDeleteClick(Sender: TObject);
    
  private
    { Private declarations }
    FGoodDate : TDateTime;
    //FGoodDateFRevolveDays : TDateTime;
    FRevolveDays,FpnlWidth : SmallInt;
  public
    { Public declarations }
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;

var
  frmMembersROLL: TfrmMembersROLL;

const SectRoll = 'Sections Rollover';
      MemRoll  = 'Members RollOver';
      LMRoll   = 'Lucky Member Rollover';
      PointsSQL = 'select MEMBER,TOTAL_REDEEMED,TOTAL_EARNT,EARNT_CREDIT,BONUS_CREDIT,' +
                  'PRE_PAID_CREDIT,POINTS_AVAILABLE,KNOWN_AS,FULL_NAME,PAY,REASON from MEMBERS';

implementation

{$R *.DFM}
uses
    dmMembers,
    rRollover,
    MembersREPORT,
    MembersCLASSES,
    Global, rDueLetter;    // StockMaster

{ **************************************************************************** }
class function TfrmMembersROLL.ScreenCode : Integer;
begin Result := Global.scMembersRoll; end;
{ **************************************************************************** }
class function TfrmMembersROLL.ScreenName : String;
begin Result := MemRoll; end;
{ **************************************************************************** }
class function TfrmMembersROLL.ScreenDesc : String;
begin Result := '  ' + MemRoll + ' Screen'; end;
{ **************************************************************************** }
procedure TfrmMembersROLL.FormCreate(Sender: TObject);
begin
     inherited;
     // ib Rollover is now used for mailing Lables as well - so clear out
     dm.StartTrans(dm.IBRollover);
     // delete from m_rollover
     dm.sqlExecQuery(dm.sqlRollover,'delete from m_rollover');
     dm.IBRollover.Commit;
     PageControlChange(Sender);
     FpnlWidth := pnlSpace.Width;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.FormDeactivate(Sender: TObject);
begin
     inherited;
     dm.IBTransactionCommit; // Commit changes on deactivate
     dm.TblMisc.Close;
     dm.ibdsRollover.Active := False;
     dm.ibdsPoints.Close;
     dm.IBRunOnce.Active  := False;
     dm.qryAttachSubs.Close;
     dm.qryAttachSubs.DataSource := nil;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.FormActivate(Sender: TObject);
begin
     inherited;
     if not dm.TblMisc.Active then
        dm.TblMisc.Open;
     //frmMemberMenu.btnMembersROLL.Down := True;
     // set the date in the Subs Roll tab,
     // Fixed = MEMBER_EXPIRY once per year -  revolving = SUBS_ROLL once per week
     // Revolve Days = days grace for Payment in Revolving System - Fixed the users chooses
     // when to run the rollover
     //if not dm.PreCheckLogOn(scRollTabLucMem,stRollTabLucMem ) then RemoveTab(tsLuckyMember);
     RemoveTab(tsLuckyMember);
     RemoveTab(tsSectionRoll);   
     if not dm.PreCheckLogOn(scRollTabMember,stRollTabMember ) then 
        RemoveTab(tsMemberRoll)
     else
        PageControl.ActivePage := tsMemberRoll;
     if not dm.PreCheckLogOn(scRollTabSection,stRollTabSection ) then RemoveTab(tsSectionRoll);
     PageControlChange(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.FormDestroy(Sender: TObject);
begin
     dm.TblMisc.Close;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.MemberRollover;
var ExpiryDate : String[12];
const Newsletter = 'NEWSLETTER';
      Analyse    = 'ANALYSE';
const SQL1 = 'Delete from M_ROLLOVER';    
      SQL2 = 'INSERT INTO M_ROLLOVER ( ROLL_ID,KEEP_EXPIRES_CURRENT,MEMBER,MAILING_LIST_CODE,' +
             'SURNAME,FIRST_NAME,FULL_NAME,ADDRESS,DISTRICT,ADDRESS1,CITY,COUNTRY,HOME_PHONE,' +
             'EXPIRES,SUB_PAID,IS_ACTIVE,NEWSLETTER,ANALYSE,ACCEPT ) ' +
             'SELECT 0,''N'',MEMBER,MAILING_CODE,SURNAME,FIRST_NAME,FULL_NAME,ADDRESS,DISTRICT,' +
             'ADDRESS1,CITY,COUNTRY,HOME_PHONE,EXPIRES,SUB_PAID,IS_ACTIVE,NEWSLETTER,ANALYSE,''Y'' '+
             //'from MEMBERS WHERE (EXPIRES + %d ) <= ''%s'' and EXPIRES is not Null';
             // Fix Note 1 - removed above line - RPNYC  - added as an option below %s
             'from MEMBERS WHERE (EXPIRES + %d ) <= ''%s'' and EXPIRES is not Null %s';
      SQL3 = 'Update M_ROLLOVER set %s = ''N'' where KEEP_EXPIRES_CURRENT = ''N''';       
begin
     if Global.FixedExpirySystem then begin
        if not cbxPrior.Checked then begin
           if dm.tblMisc.FieldByName('MEMBER_EXPIRY').AsDateTime > FGoodDate then begin
              sm(' You cannot do a Rollover until after the Member Expiry date');
              exit;
           end;
        end;  
     end; 
     if not IfMsgdlg( 'Start ' + MemRoll) then exit;
     TooBusy('Starting ' + MemRoll + PleaseWaitMsg);
     try
        try
           dm.ibdsRollover.DisableControls;
           dm.StartTrans(dm.IBRollover);
           // delete from m_rollover
           dm.sqlExecQuery(dm.sqlRollover,SQL1);
          { // work out the Expiry Date to work off
           if Global.FixedExpirySystem then begin
              // allow user to change the date
              ExpiryDate := GetShortDate(dbdtNextExpires.Date)
           end else begin   
              // todays date
              ExpiryDate := GetShortDate(FGoodDate);
           end;  }
           // insert into M_Rollover   Add Revolve days to Expires 
           // if checked then add all financial members - Pauanui
           if cbxPrior.Checked then 
              dm.sqlExecQuery(dm.sqlRollover,format(SQL2,[FRevolveDays,GetShortDate(dm.tblMisc.FieldByName('MEMBER_EXPIRY').AsDateTime + FRevolveDays),'']))
           else
              dm.sqlExecQuery(dm.sqlRollover,format(SQL2,[FRevolveDays,GetShortDate(FGoodDate),' and SUB_PAID > 0']));
           // attach keep_expires_current to the member
           // member may have many subs - if they have one keep_expires_current then set to N
           with dm.ibdsRollover do begin
               SelectIBDSRollover(varNo);
               First;
               while not EOF do begin
                   if HasSubKeepExpiresCurrent(FieldByName('MEMBER').AsInteger) then begin
                      Edit;
                      FieldByName('KEEP_EXPIRES_CURRENT').AsString := varYes;
                      Post;
                   end;
                   Next;
               end;
           end;
           // update newsletter if Checked where KEEP_EXPIRES_CURRENT = ''N''
           if cbxNewsLetter.Checked then
              dm.sqlExecQuery(dm.sqlRollover,format(SQL3,[Newsletter]));
           // update Analyse  if Checked where KEEP_EXPIRES_CURRENT = ''N''
           if cbxAnalyse.Checked then
              dm.sqlExecQuery(dm.sqlRollover,format(SQL3,[Analyse]));
           SetButtons(False);
           SetButtonsState(True);
           //commit
           if dm.IBRollover.InTransaction then begin
              dm.IBRollover.Commit;
              dm.StartTrans(dm.IBRollover);
              dm.ibdsRollover.Open;
           end; 
           // View
           rgpViewClick(rgpView);
        except
           if dm.IBRollover.InTransaction then begin
              dm.IBRollover.RollBack;
              dm.StartTrans(dm.IBRollover);
           end;
           NotBusy;
           sm(UsersMakingChangesMsg + #13#10 + MemRoll +  PleaseTryAgainMsg);
        end;
     finally
        dm.ibdsRollover.EnableControls;
        NotBusy;
        dm.sqlRollover.Close;
     end;                               
           
     
end;
{ **************************************************************************** }
function TfrmMembersROLL.HasSubKeepExpiresCurrent( Member : LongInt) : Boolean;
const SQL1 = 'Select s.KEEP_EXPIRES_CURRENT from SUBSCRIPTIONS s,M_SUBS ms ' +
             'where ms.MEMBER = %d and ms.SUBSCRIPTION = s.SUBSCRIPTION';
var YesCtr,RecordCtr : Smallint;
begin
     Result := False;
     if dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL1,[Member])) then begin
        with dm.sqlRollover do begin
           YesCtr := 0; RecordCtr := 0;
           while not EOF do begin
              if FieldByName('KEEP_EXPIRES_CURRENT').AsString = varYes then 
                 inc(YesCtr);
              inc(RecordCtr);
              next;
           end;   
           if YesCtr = RecordCtr then 
             // same number of records and Yes's
              Result := True
           else   
              //  set to False if one or more is no 
              Result := False;
        end;
     end;
end;
{ **************************************************************************** }
{procedure TfrmMembersROLL.btnMemberClick(Sender: TObject);
var FormDate : String[15];
    FormDateRevolveDays : string[15]; s : string;
const SQL1 = 'UPDATE MEMBERS M SET EXPIRES = EXPIRES + 365, ' +
             ' SUB_PAID = 2,INACTIVE_DATE = Null,IS_ACTIVE = ''Y'' where EXPIRES < ''%s'' and ' +
             '(select SUM(AMOUNT) from M_SUBS where MEMBER = M.MEMBER) = 0.00';
      //       'and (select Count(member) from m_subs where MEMBER = M.MEMBER) > 0';
      SQL2 = 'UPDATE MEMBERS SET SUB_PAID = 0,IS_ACTIVE = ''N'',' +
             ' INACTIVE_DATE = ''%s'' where EXPIRES < ''%s''';       
//             ' INACTIVE_DATE = ''%s'' where EXPIRES + 365 < ''%s''';       
      SQL3 = 'UPDATE MEMBERS SET IS_ACTIVE = ''N'',INACTIVE_DATE = ''%s'' where SUB_PAID < 2';
      SQL4 = 'UPDATE MEMBERS SET NEWSLETTER = ''N'' where NEWSLETTER = ''Y''';
      SQL6 = 'UPDATE MEMBERS SET SUB_PAID = 0,IS_ACTIVE = ''N'',INACTIVE_DATE = ''%s'' where EXPIRES + 365 < ''%s''';
      SQL7 = 'UPDATE MEMBERS SET IS_ACTIVE = ''N'',INACTIVE_DATE = ''%s'' where SUB_PAID < 2 and EXPIRES < ''%s'' ' +
             ' or SUB_PAID = 1 and EXPIRES + 365 < ''%s''';
      SQL8 = 'update M_SUBS MS set EDIT_DATE = ''%s'', AMOUNT = ' +
             '( select FEE from SUBSCRIPTIONS where MS.SUBSCRIPTION = SUBSCRIPTION ) ' +
             'where AMOUNT_PAID < (AMOUNT * 2 ) and MS.MEMBER = ' +
             '(select MEMBER from MEMBERS where MS.MEMBER = MEMBER and EXPIRES + %d < ''%s'')';
      SQL9 = 'update M_SUBS MS set AMOUNT_PAID = 0.00 where ( select SUB_PAID from ' +
             '  members where MS.MEMBER = MEMBER and EXPIRES + %d < ''%s'') = 0';
      SQL10 = 'UPDATE MISC SET MEMBER_EXPIRY = ''%s''';
      SQL11 = 'UPDATE MISC SET SUBS_ROLL = ''%s''';
             
//  Allows for Fixed and Revolve Exiry Systems
// FRevolve Days from tbl MISC - set to 0 for Fixed
begin
     if Busy then exit;
     if dm.LoggedOnUsers > 1 then begin // allow for self
        sm(UsersLoggedOnMsg + #13#10 + 'They must all be logged off before you can do a ' + MemRoll);
        exit;
     end;
     FGoodDate := Date;
     if not CheckSystemDate(FGoodDate) then begin
        sm(IncorrectDateMsg);
        exit;
     end;
     FormDate := GetShortDate(FGoodDate);
     // make this date for Revolving System - allows for x days grace before
     // making them IS_ACTIVE to 'F' where they do a weekly roll
     FGoodDateFRevolveDays :=  FGoodDate + FRevolveDays;
     FormDateRevolveDAys := GetShortDate(FGoodDateFRevolveDays);
     if Global.FixedExpirySystem then begin
        if Global.AddOneYear(dm.tblMisc.FieldByName('MEMBER_EXPIRY').AsDateTime) > FGoodDate then begin
           sm(' You cannot do a Rollover until 1 year after the Member Expiry date');
           exit;
        end;
     end; 
     if Global.FixedExpirySystem then begin
        if dm.tblMisc.FieldByName('MEMBER_EXPIRY').AsDateTime > FGoodDate then begin
           sm(' You cannot do a Rollover until after the Member Expiry date');
           exit;
        end;
     end;
     if not IfMsgdlg( AreYouSureMsg + MemRoll) then exit;
     TooBusy(MemRoll + PleaseWaitMsg);
     try
        try
           dm.StartTrans(dm.IBRollover);
           if Global.FixedExpirySystem then begin
              // increase the EXPIRES DATE for Life MEMBERS
              // if Amount = 0.00 from M_SUBS,INACTIVE_DATE and IS_ACTIVE are just in case
              // they must have entires in M_SUBS though - count ( Last Line )
              dm.sqlExecQuery(dm.sqlRollover,format(SQL1,[GetShortDate(FGoodDate)]));
              // demote any member to 0 where EXPIRES + 1 Year  < now
              // for SUB_PAID = 0,1 or 2
              dm.sqlExecQuery(dm.sqlRollover,format(SQL2,[FormDate,FormDate]));
              // set any member to N where they are not fully paid (= 2)
              dm.sqlExecQuery(dm.sqlRollover,format(SQL3,[FormDate]));
              if cbxNewsLetter.Checked then 
                 // clear for all MEMBERS
                 dm.sqlExecQuery(dm.sqlRollover,SQL4);
           end else begin // revolving System
              // increase the EXPIRES DATE for Life MEMBERS
              // if Amount = 0.00 from M_SUBS,INACTIVE_DATE and IS_ACTIVE are just in case
              // they must have entires in M_SUBS though - count ( Last Line )
              dm.sqlExecQuery(dm.sqlRollover,format(SQL1,[GetShortDate(FGoodDate)]));
              // demote any member to 0  where EXPIRES + 1 Year  < now + Revolve days
              // for SUB_PAID = 0,1 or 2
              dm.sqlExecQuery(dm.sqlRollover,format(SQL6,[FormDate,FormDateRevolveDAys]));
              // set any member to N where they are not fully paid (= 2)
              // and in the last year they are have been a 0,1 or 2
              // or they are part paid 1 and the EXPIRES + 1 year + RevolveDays  < now
              dm.sqlExecQuery(dm.sqlRollover,format(SQL7,[FormDate,FormDateRevolveDays,FormDateRevolveDays]));
           end;
          // if cbxApplySubPrices.Checked then begin
          //    TooBusy('Updating Prices' +  PleaseWaitMsg);
              // apply prices from SUBSCRIPTIONS only where AMOUNT_PAID < (AMOUNT * 2 )
              // ie they havnt Paid in advance, AMOUNT_PAID remains until next roll or Payment
           //   dm.sqlExecQuery(dm.sqlRollover,format(SQL8,[GetShortDateTime(now),FRevolveDays,FormDate]));
          // end;
           TooBusy('Updating Amount Paid' + PleaseWaitMsg);
           // clear AMOUNT_PAID in M_SUBS  if SUB_PAID in MEMBERS = 0
           // by this stage the 0,1 and 2 have been sorted out
           dm.sqlExecQuery(dm.sqlRollover,format(SQL9,[RevolveDays,FormDate]));
           if Global.FixedExpirySystem then begin
              // only update MEMBER_EXPIRY for Fixed System to + 1 year
              dm.sqlExecQuery(dm.sqlRollover,format(SQL10,[Global.AddOneYear(dm.tblMisc.FieldByName('MEMBER_EXPIRY').AsDateTime)]));
              Global.MemberExpiry := Global.AddOneYear(dm.tblMisc.FieldByName('MEMBER_EXPIRY').AsDateTime);
           end else begin
              // only update SUBS_ROLL for Revolving System to today
              dm.sqlExecQuery(dm.sqlRollover,format(SQL11,[FormDate]));
           end;
           if dm.IBRollover.InTransaction then
              dm.IBRollover.Commit;
           NotBusy;
           MDlgI(MemRoll + CompletedMsg );
           dm.tblMisc.Close;// refresh  - shows date
           dm.tblMisc.Open;
           dm.ProcessType := ord(TProcessTypes(ptRollover));
           dm.LogProcess(dm.sqlRollover,MemRoll + CompletedMsg );
        except
           if dm.IBRollover.InTransaction then begin
              dm.IBRollover.RollBack;
              dm.StartTrans(dm.IBRollover);
           end;
           NotBusy;
           sm(UsersMakingChangesMsg + #13#10 + MemRoll +  PleaseTryAgainMsg);
        end;
     finally
        NotBusy;
        dm.sqlRollover.Close;
     end;                               
end;}
{ **************************************************************************** }
procedure TfrmMembersROLL.btnSectionClick(Sender: TObject);
var s : string[20];
const SQL1 = 'update M_SECTIONS MS set AMOUNT = ( select FEE from SECTIONS where MS.SECTION = SECTION ),' +
             ' MS.AMOUNT_PAID = 0.00 where MS.EXPIRES <= ''%s''';
      SQL2 = 'DELETE from M_SECTIONS MS where MS.AMOUNT_PAID = 0.00';       
begin
     if Busy then exit;
     FGoodDate := Date;
     if not CheckSystemDate(FGoodDate) then begin
        sm(IncorrectDateMsg);
        exit;
     end;
     if dm.tblSections.FieldByName('EXPIRES').AsDateTime > FGoodDate then begin
        sm(' You cannot do a ' + SectRoll + ' until after the Section Expires date');
        exit;
     end;
     if dm.LoggedOnUsers > 1 then begin // allow for self
        sm(UsersLoggedOnMsg + #13#10 + 'They must all be logged off before you can do a ' + SectRoll);
        exit;
     end;

     if cbxAllSections.Checked then begin
        if not IfMsgDlg(AreYouSureMsg + SectRoll + ' for ALL Sections') then exit;
     end else begin
        if not IfMsgDlg(Format(AreYouSureMsg + SectRoll + ' for ' +
           '%s',[dm.tblSections.FieldByName('DESCRIPTION').AsString])) then exit;
     end;
     TooBusy(SectRoll + PleaseWaitMsg);
     try
        try
           dm.StartTrans(dm.IBRollover);
           case cbxAllSections.Checked of
              true  : s := '';
              false : s := format(' and MS.SECTION = %d',[dm.tblSections.FieldByName('SECTION').AsInteger]);
           end;
           // makes AMOUNT_PAID = 0.00 where <= Expires
           // updates latest price
           with dm.sqlRollover do begin
              Close;
              sql.text := format(SQL1,[GetShortDate(FGoodDate)]);
              if s <> '' then
               sql.Add(format('%s',[s]));
              ExecQuery;
           end;   
           // takes out all Members where Amount_Paid = 0.00
           if cbxClearMember.Checked then begin
              with dm.sqlRollover do begin
                 Close;
                 sql.text := SQL2;
                 if s <> '' then 
                  sql.Add(format(' %s',[s]));
                 ExecQuery;
              end;   
           end;   
           if dm.IBRollover.InTransaction then dm.IBRollover.Commit;
           with dm.TblSections do begin
              Edit;
              FieldByName('LAST_ROLL').AsDateTime := FGoodDate;
              FieldByName('EXPIRES'  ).AsDateTime :=
                 AlterExpireDateAbove(FieldByName('EXPIRES').AsDateTime, FGoodDate );
              Post;  // does commit here
              Refresh;
           end;
           NotBusy;
           dm.ProcessType := ord(TProcessTypes(ptRollover));
           dm.LogProcess(dm.sqlRollover, SectRoll + CompletedMsg );
           MDlgI(SectRoll + CompletedMsg );
        except
           if dm.IBRollover.InTransaction then begin
              dm.IBRollover.RollBack;
              dm.StartTrans(dm.IBRollover);
           end;
           NotBusy;
           sm(UsersMakingChangesMsg + #13#10 + 'The ' + SectRoll + PleaseTryAgainMsg);
        end;
     finally
        NotBusy;
        dm.sqlRollover.Close;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.btnLuckyClick(Sender: TObject);
const SQL1 = 'UPDATE MEMBERS SET LUCKY_DATE = null where LUCKY_DATE is not null';
      SQL2 = 'UPDATE MISC SET LUCKY_ROLL = ''%s''';
begin
     if Busy then exit;
     if dm.LoggedOnUsers > 1 then  // allow for self
        sm(UsersLoggedOnMsg);
     if not IfMsgDlg(AreYouSureMsg + LMRoll) then exit;
     FGoodDate := Date;
     if not CheckSystemDate(FGoodDate) then begin
        sm(IncorrectDateMsg);
        exit;
     end;
     TooBusy(LMRoll + PleaseWaitMsg);
     try
        with dm.IBRollover do begin
           try
              if not InTransaction then StartTransAction;
              dm.sqlExecQuery(dm.sqlRollover,SQL1);    
              dm.sqlExecQuery(dm.sqlRollover,format(SQL2,[GetShortDate(FGoodDate)]));    
              if InTransaction then Commit;
              NotBusy;
              dm.ProcessType := ord(TProcessTypes(ptRollover));
              dm.LogProcess(dm.sqlRollover, LMRoll + CompletedMsg);
              MDlgI(LMRoll + CompletedMsg);
              dm.tblMisc.Close;// refresh - no primary key - shows date
              dm.tblMisc.Open;
           except
              if InTransaction then begin
                 RollBack;
                 StartTransaction;
              end;
              NotBusy;
              sm(UsersMakingChangesMsg + #13#10 + 'The ' + LMRoll+ PleaseTryAgainMsg);
           end;
        end;
     finally
        NotBusy;
        dm.sqlRollover.Close;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.menuExitClick(Sender: TObject);
begin
     if Busy then exit;
     if dm.IBRollover.InTransaction then // incase
        dm.IBRollover.RollBack;
     Close;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.cbxAllSectionsClick(Sender: TObject);
begin
    if cbxAllSections.Checked then 
       dbgSections.Visible := False
    else 
       dbgSections.Visible := True;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.PageControlChange(Sender: TObject);
const FixedExpSystem = 'Fixed Expiry System';
      RevolveExpSystem = 'Revolving Expiry System';
begin
     btnRollover.Enabled := True;
     
     case PageControl.ActivePageIndex of
     0 : begin
            if Global.FixedExpirySystem then begin
               dbeRevolveDays.Visible := False;
               lblRevolveDays.Visible := False;
               lblExpirySystem.Caption := FixedExpSystem;
               //dbeLastRoll.DataField := 'MEMBER_EXPIRY';
               dbdtNextExpires.Date  := AddOneYear(MemberExpiry);
               //lblRollDate.Caption := 'Member Expiry';
               cbxPrior.Enabled := True;
               FRevolveDays := 0;        
            end else begin
               lblNextExpires.Visible  := False;
               dbdtNextExpires.Visible := False;
               lblExpirySystem.Caption := RevolveExpSystem;
               dbeLastRoll.DataField := 'SUBS_ROLL';
               lblRollDate.Caption := 'Last Rollover';
               FRevolveDays := Global.RevolveDays;
               cbxPrior.Enabled := False;
            end;
            btnRollover.Hint := 'Select expired Members for a ' + MemRoll;
            dm.qryAttachSubs.DataSource := dm.dsrRollover;
            dm.qryAttachSubs.Open;
            if not dm.ibdsRollover.Active then begin
               dm.StartTrans(dm.IBRollOver);
               dm.ibdsRollover.Active := True;
               rgpViewClick(Sender);
               if dm.ibdsRollover.RecordCount > 0 then begin
                  SetButtons(False);
                  SetButtonsState(True);
               end else begin   
                  SetButtons(True);
                  SetButtonsState(False);
               end;
            end;   
         end;   
     1 : begin
         btnRollover.Hint := 'Do a ' + SectRoll;
         end;
     2 : btnRollover.Hint := 'Do a ' + LMRoll;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
var tabsheet : TTabSheet;
begin
     inherited;
     case key of   
         VK_ESCAPE : menuExitClick(Sender);
     end;   
     if ( ssCtrl in shift )  or ( ssAlt in shift ) then begin
        case key of 
           77,109 : tabsheet := tsMemberRoll;  // M    
           83,115 : tabsheet := tsSectionRoll; // S
           76,108 : tabsheet := tsLuckyMember; // L
           88,120 : menuExitClick(Sender); // X Close 
        end;  
        if tabsheet.TabVisible then PageControl.ActivePage := tabsheet;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.FormResize(Sender: TObject);
begin
     // dont call inherited
     ResizeToolBar( Self ,Self.WindowState,OneButtonWidth,FpnlWidth,pnlSpace);
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.dbdtNextExpiresChange(Sender: TObject);
begin
    if TwwDBDateTimePicker(Sender).Modified then
       TwwDBDateTimePicker(Sender).DisplayFormat := 'dd/mm/yyyy';
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.dbdtNextExpiresExit(Sender: TObject);
begin
     TwwDBDateTimePicker(Sender).DisplayFormat := 'dd/mm/yyyy';
end;

{ **************************************************************************** }
procedure TfrmMembersROLL.menuStartRolloverClick(Sender: TObject);
begin
     if Busy then exit;
     if dm.LoggedOnUsers > 1 then  // allow for self
        sm(UsersLoggedOnMsg);
     //if dm.LoggedOnUsers > 1 then begin // allow for self
     //   sm(UsersLoggedOnMsg + #13#10 + 'They must all be logged off before you can do a ' + MemRoll);
     //   exit;
     //end;
     FGoodDate := Date;
     if not CheckSystemDate(FGoodDate) then begin
        sm(IncorrectDateMsg);
        exit;
     end;
     case PageControl.ActivePageIndex of
        0 : MemberRollover;               // Members
        1 : btnSectionClick(Sender);      // Sections
        2 : btnLuckyClick(Sender);        // lucky
        //3 : PointsRollover;               // Points Credit Rollover to zero
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.menuCancelRolloverClick(Sender: TObject);
const SQL1 = 'Delete from M_ROLLOVER';    
begin
     if not IfMsgdlg( 'Cancel ' + MemRoll) then exit;
     SetButtons(True);
     SetButtonsState(False);
     dm.sqlExecQuery(dm.sqlRollover,SQL1);
     if dm.IBRollover.InTransaction then begin
        dm.IBRollover.Commit;
        dm.StartTrans(dm.IBRollover);
     end;
     dm.ibdsMembers.Open;
     rgpViewClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.menuAcceptRolloverClick(Sender: TObject);
var s,Member : String[15];
    msg : shortstring;
const RollMsg = 'Member %d %s became Non Financial (Rollover)';
      KECMsg  = 'Member %d %s Expires to %s (Rollover)';
      SQL1 = 'Update MEMBERS set ANALYSE = ''%s'',NEWSLETTER = ''%s'' where MEMBER = %d';
      SQL2 = 'Update MEMBERS set EXPIRES = ''%s'' where MEMBER = %d';
      SQL3 = 'Update M_SUBS set AMOUNT = %f,AMOUNT_PAID = 0.00,PAID = ''N'' where MEMBER = %d';
      SQL4 = 'Update MEMBERS set SUB_PAID = 0 where MEMBER = %d';
      SQL5 = 'Delete from M_ROLLOVER';    
      SQL10 = 'UPDATE MISC SET MEMBER_EXPIRY = ''%s''';
      SQL11 = 'UPDATE MISC SET SUBS_ROLL = ''%s''';
      SQL12 = 'select ms.SUBSCRIPTION,s.FEE from SUBSCRIPTIONS s,M_SUBS ms ' +
              'where ms.SUBSCRIPTION = s.SUBSCRIPTION and ms.MEMBER = %d';
      EndMsg =  ' failed for Member ';
      NewsAnalyseMsg = 'Update NEWSLETTER,ANALYSE';
      AmountPaidMsg  = 'Update AMOUNT_PAID,PAID';
      SubPaidMsg  = 'Update SUB_PAID';
      LogProcessMsg = 'Insert Process Log';
      LogExportMsg  = 'Insert Export Log';
      ExpiresMsg = 'Update Expires';
      MemberExpiryMsg = 'Update MEMBER_EXPIRY';
      SubsRollMsg = 'Update SUBS_ROLL';
      DeleteMsg = 'Delete from M_ROLLOVER';
      
begin
     if dm.ibdsRollover.recordCount = 0 then begin
       sm('There are no Members to Rollover');
       exit;
     end;          
     if FixedExpirySystem then begin
        if dbdtNextExpires.Date <= FGoodDate then begin
           Sm( 'The Next Expires date cannot be before the Current Expires date');
           exit;
        end;
     end;
     if not IfMsgdlg( 'Accept ' + MemRoll + #13#10 + 'NB Printing or Exporting must be done before this' ) then exit;
     if dm.ibdsRollover.State = dsEdit then dm.ibdsRollover.Post;
     dm.ProcessType := ord(TProcessTypes(ptRollover));
     try
        with dm.ibdsRollover do begin
           TooBusy(MemRoll + PleaseWaitMsg);
           DisableControls;
           First;
           try
              // Select with KEEP_EXPIRES_CURRENT = No - Param 
              SelectIBDSRollover(varNo);
              while not EOF do begin
                 if FieldByName('ACCEPT').AsString = varYes then begin
                    // Just in case
                    if FieldByName('KEEP_EXPIRES_CURRENT').AsString = varNo then begin
                       // update newsletter, analyse
                       Member := FieldByName('MEMBER').AsString;
                       msg := NewsAnalyseMsg;
                       dm.sqlExecQuery(dm.sqlRollover,format(SQL1,[ FieldByName('ANALYSE').AsString,FieldByName('NEWSLETTER').AsString,FieldByName('MEMBER').AsInteger])); 
                       msg := AmountPaidMsg;
                       // Get all Subs for this Member use qryGeneral
                       dm.sqlExecQuery(dm.qryGeneral,format(SQL12,[FieldByName('MEMBER').AsInteger]));
                       while not dm.qryGeneral.EOF do begin
                        // update M_SUBS AMOUNT_PAID  to 0.00 and the latest fee
                          dm.sqlExecQuery(dm.sqlRollover,format(SQL3,[dm.qryGeneral.FieldByName('FEE').AsFloat, FieldByName('MEMBER').AsInteger]));
                          dm.qryGeneral.Next;
                       end;
                       // make non Financial
                       msg := SubPaidMsg;
                       dm.sqlExecQuery(dm.sqlRollover,format(SQL4,[FieldByName('MEMBER').AsInteger]));
                       // Log the non Financial fact
                       msg := LogProcessMsg;
                       dm.LogProcess( dm.sqlRollover, format(RollMsg,[FieldByName('MEMBER').AsInteger,FieldByName('FULL_NAME').AsString] ));
                       // Export to AREV
                       msg := LogExportMsg;
                       dm.LogExport ( dm.sqlRollover, FieldByName('MEMBER').AsInteger,0,False);
                    end;
                 end;
                 next;
              end; 
              // Select with KEEP_EXPIRES_CURRENT = Yes - Param 
              SelectIBDSRollover(varYes);
              while not EOF do begin
                 if FieldByName('ACCEPT').AsString = varYes then begin
                    // Just in case
                    if FieldByName('KEEP_EXPIRES_CURRENT').AsString = varYes then begin
                       Member := FieldByName('MEMBER').AsString;
                       // update Keep Expires Current to add one year
                       if FixedExpirySystem then begin
                          // use dbdtNextExpires.Date
                          s := GetShortDate(dbdtNextExpires.Date);
                       end else begin   
                          // add one year onto Members Expires
                          s := GetShortDate(AddOneYear(FieldByName('EXPIRES').AsDateTime));
                       end;
                       msg := ExpiresMsg;
                       dm.sqlExecQuery(dm.sqlRollover,format(SQL2,[s,FieldByName('MEMBER').AsInteger]));
                       // Log the non Financial fact
                       msg := LogProcessMsg;
                       dm.LogProcess( dm.sqlRollover, format(KECMsg,[FieldByName('MEMBER').AsInteger,FieldByName('FULL_NAME').AsString,s] ));
                       // Export to AREV - keep expires up to date in Clubmaster
                       msg := LogExportMsg;
                       dm.LogExport ( dm.sqlRollover, FieldByName('MEMBER').AsInteger,0,False);
                    end;                 
                 end;
                 next;
              end;    
              Member := '';
              if Global.FixedExpirySystem then begin
                 // only update MEMBER_EXPIRY for Fixed System to dbdtNextExpires
                 msg := MemberExpiryMsg;
                 dm.sqlExecQuery(dm.sqlRollover,format(SQL10,[GetShortDate(dbdtNextExpires.Date)]));
                 Global.MemberExpiry := dbdtNextExpires.Date;
              end else begin
                 // only update SUBS_ROLL for Revolving System to today
                 msg := SubsRollMsg;
                 dm.sqlExecQuery(dm.sqlRollover,format(SQL11,[GetShortDate(FGoodDate)]));
              end;
              dbeLastRoll.Update;
              //dm.ProcessType := ord(TProcessTypes(ptRollover));
              msg := LogProcessMsg;
              dm.LogProcess(dm.sqlRollover,MemRoll + CompletedMsg );
              // delete from the rollover table
              msg := DeleteMsg;
              dm.sqlExecQuery(dm.sqlRollover,SQL5);
              if dm.IBRollover.InTransaction then begin
                 dm.IBRollover.Commit;
                 dm.IBRollover.StartTransaction;
              end;
              dm.tblMisc.Close;// refresh  - shows date
              dm.tblMisc.Open;
              SetButtons(True);
              SetButtonsState(False);
              rgpViewClick(Sender);
              NotBusy;
              MDlgI(MemRoll + CompletedMsg );
           except
              if dm.IBRollover.InTransaction then begin
                 dm.IBRollover.RollBack;
                 dm.IBRollover.StartTransaction;
              end;
              if Member = '' then
                 sm( MemRoll + FailedMsg + #13#10 + Msg )
              else
                 sm( MemRoll + FailedMsg + #13#10 + Msg + EndMsg + Member);
              rgpViewClick(Sender);
           end;
        end;
     finally
       NotBusy;
       dm.ibdsRollover.EnableControls;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.SetButtons( b : boolean );
begin
     gbxOptions.Visible := b;
     menuStartRollover.Enabled := b;
     btnRollover.Enabled := b;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.SetButtonsState( b : boolean );
begin
     rgpView.Visible := b;
     menuAcceptRollover.Enabled := b;
     btnAcceptRollover.Enabled  := b;
     menuCancelRollover.Enabled := b;
     btnCancelRollover.Enabled  := b;
     btnPrintRollover.Enabled   := b;
     menuPrintRollover.Enabled  := b;
     btnExportRollover.Enabled  := b;
     menuExportRollover.Enabled  := b;
     btnLetterRollover.Enabled  := b;
     menuSubsDueFinalLetter.Enabled  := b;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.dbgRolloverCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;  ABrush: TBrush);
begin
  if (Field.FieldName = 'ACCEPT') or
     (Field.FieldName = 'NEWSLETTER') or
     (Field.FieldName = 'ANALYSE') then
        ABrush.Color := Global.fcGridEdit;    
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.rgpViewClick(Sender: TObject);
begin
     if not active then exit;
     if dm.ibdsRollover.State = dsEdit then dm.ibdsRollover.Post;
     with dm.ibdsRollover do begin
        case rgpView.ItemIndex of
            0 : SelectIBDSRollover(varNo);// Rollover
            1 : SelectIBDSRollover(varYes);// Keep Expires Current
        end;
     end;     
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.SelectIBDSRollover( ch : shortstring);
begin
     with dm.ibdsRollover do begin
        Close;
        ParamByName('KeepExpiresCurrent').AsString := ch;
        Open;
     end;
end;     
{ **************************************************************************** }
procedure TfrmMembersROLL.menuPrintRolloverClick(Sender: TObject);
begin
     frmRollover := TfrmRollover.Create(nil);
     try
        //frmRollover.qr.Print;
        // use the same transaction - changes are not commited
        if dm.ibdsRollover.State = dsEdit then dm.ibdsRollover.Post;
        dm.qryReport.Transaction := dm.IBRollover;
        frmRollover.qr.Preview;
     finally
        frmRollover.Free;
        dm.qryReport.Close;
        dm.qryReport.Transaction := dm.IBTransaction;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.menuExportRolloverClick(Sender: TObject);
const SQL1 = 'Select * from M_ROLLOVER WHERE ACCEPT = ''Y'' Order BY KEEP_EXPIRES_CURRENT,ACCEPT,MEMBER';
var CurrDirAndFile : ShortString;
begin
     try
        // use the same tranasction - changes are not commited
        if dm.ibdsRollover.State = dsEdit then dm.ibdsRollover.Post;
        dm.qryReport.Transaction := dm.IBRollover;
        with dm.qryReport do begin
            Close;
            SQL.Text := SQL1;
            Open;
        end;
        GetDir(0,CurrDirAndFile);
        CreateAsciiFile( CurrDirAndFile + ExportFileName,False,dm.qryReport,nil,True,False);
     finally
        dm.qryReport.Close;
        dm.qryReport.Transaction := dm.IBTransaction;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.menuSubsDueFinalLetterClick(Sender: TObject);
begin
     if frmMembersReport = nil then
        frmMembersReport := TfrmMembersReport.Create(Self);
     with frmMembersReport do begin
         FOwnIndex := 44;          
         frmDueLetter := TfrmDueLetter.Create(nil);
         dm.qryReport.Transaction := dm.IBTransPAy;
         dm.ibdsLetters.Open;
         try 
            with frmDueLetter do begin
              if not GetDatafromLetter( ReportTitles[FOwnIndex] ) then begin
                  sm('Subs Due Letter not found or invalid');
                  exit;
                end;
                ExportFieldsList    := ListOne;  // properties
                ExportExtFieldsList := ListTwo;
                if not Extract(True,5,Printer.PrinterIndex,FExtFields,date,date,
                                    FExportFields,ReportTitles[FOwnIndex],0,0) then begin
                     Free;
                     exit;
                end;
               qr.Preview;
            end;
         finally
            dm.qryReport.Close;
            dm.qryReport.Transaction := dm.IBTransaction;
            frmMembersReport.Free;
            dm.ibdsLetters.Close;
            frmDueLetter.Free;
         end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.dbgRolloverRowChanged(Sender: TObject);
begin
     dm.qryAttachSubs.Close;
     dm.qryAttachSubs.Open;
end;
{ **************************************************************************** }
procedure TfrmMembersROLL.RollbackPointsRollover;
begin
     if dm.IBRunOnce.InTransaction then begin
        dm.IBRunOnce.Rollback;
        dm.IBRunOnce.StartTransAction;
     end;
     PointsRolloverReset;
end;
{ ****************************************************************************}
procedure TfrmMembersROLL.PointsRolloverReset;
const SQL1 = ' MEMBER = -1';
begin
     if not dm.IBRunOnce.InTransaction then
        dm.IBRunOnce.StartTransaction;
     with dm.ibdsPoints do begin
        Close;
        SelectSQL.Text := PointsSQl + SQL1;
        Open;
     end;
end;
{ ****************************************************************************}
procedure TfrmMembersROLL.btnReceiptRolloverClick(Sender: TObject);
begin
     if not IfMsgdlg( 'Do you want to Clear all the Receipt Numbers from Members?' ) then exit;
     if not dm.IBRunOnce.InTransaction then
        dm.IBRunOnce.StartTransaction;
     with dm.sqlRunOnce do begin
        close;
        SQL.Text := 'Update M_SUBS set RECEIPT = 0';
        Execquery;
     end;
     try
        dm.IBRunOnce.Commit;
        MdlgI('Receipt Rollover Completed');
     except
        MdlgI('Receipt Rollover Failed');
     end;   
end;
{ ****************************************************************************}
procedure TfrmMembersROLL.menuPointsCreditRolloverClick(Sender: TObject);
//const SQL1 = ' TOTAL_EARNT + TOTAL_REDEEMED <> 0.00 or EARNT_CREDIT + BONUS_CREDIT + PRE_PAID_CREDIT <> 0.00';
var MemberPayments : TMemberPayments;
    BeforeEditPointsCredit : TBeforeEditPointsCredit;
    MenuMatePointsUpdate   : TMenuMatePointsUpdate;
    
begin
     if not ifMsgDlg('This will reset all Members Points and Credits to zero,' + #13#10 +
                     'Are you sure you want to run a Points Rollover?') then exit;
     if not ifMsgDlg('******************************************************' + #13#10 +
                     'This will reset all Members Points and Credits to zero,' + #13#10 +
                     'Are you sure you want to run a Points Rollover?'  + #13#10 +
                     '******************************************************') then exit;
     try
        // disable the export timer to allow for processing time
        dm.exportTimer.Enabled := False;
        // get the latest points for these members
        MenuMatePointsUpdate.MembersPointsUpdate('-1');
        
        screen.cursor := crHourGlass;
        with dm.ibdsPoints do begin
           Close;
           SelectSQL.Text := PointsSQL;
           Open;
           PointsCreditTypes := pcReset;
           while not EOF do begin
              // save current points values
              Global.BeforeEditPointsCreditSave(BeforeEditPointsCredit,dm.dsrPoints.DataSet);
              edit;
              if not MemberPayments.PointsCreditUpdate( 0.00,dm.dsrPoints.DataSet ) then begin
                 RollbackPointsRollover;
                 sm('Error updating Points/Credits, Points Rollover has not worked');  
                 exit;
              end;
              post;
              // ResetPoints  
              dm.LogExport(dm.sqlRunOnce,FieldByName('MEMBER').AsInteger,0,True);
              // Log it
              if not MemberPayments.LogPointsCredit(dm.IBRunOnce,'Points Rollover',
                                                    BeforeEditPointsCredit,
                                                    dm.dsrPoints.DataSet,
                                                    FieldByName('PAY').AsFloat)
                                                    then begin
                 RollbackPointsRollover;
                 sm('Error Logging Points/Credits, Points Rollover has not worked');  
                 exit;
              end;   
              next;
           end;
           dm.ProcessType := ord(TProcessTypes(ptRollover));
           dm.LogProcess(dm.sqlRunOnce,'Points Rollover Completed');
           try
             if dm.IBRunOnce.InTransaction then begin
                dm.IBRunOnce.Commit;
                dm.IBRunOnce.StartTransAction;
             end;
             //PointsRolloverReset;
             mdlgi('Points Rollover was successful');
           except
             on E: Exception do begin
                sm('The Points Rollover has not worked' + E.Message);
                RollbackPointsRollover;
             end;   
           end;
        end;
     finally
        dm.exportTimer.Enabled := True;
        screen.cursor := crDefault;
     end;                            
end;
{ ****************************************************************************}
procedure TfrmMembersROLL.menuNonFinancialMemberDeleteClick(Sender: TObject);
var EnableTimer : Boolean;
    s : String[8];
    MemberPayments : TMemberPayments;
    BeforeEditPointsCredit : TBeforeEditPointsCredit;
    MenuMatePointsUpdate   : TMenuMatePointsUpdate;
    
const SQL1 = 'Select Count(Member) from Members where SUB_PAID = 0 and CARD_TYPE = ''N'' or SUB_PAID = 0 and CARD_TYPE is NULL';
begin
     dm.StartTrans(dm.IBRunOnce);
     dm.sqlRunOnce.SQL.Text := SQL1;
     dm.sqlRunOnce.Execquery;
     s := dm.sqlRunOnce.Fields[0].AsString;
     if not ifMsgDlg('This will DELETE all NON Financial MEMBERS,' + #13#10 +
                     'with a Card Type of Normal, or a Card Type of nothing = ' + s + ' Members,' + #13#10 +
                     'Any Points will deleted with the Member,' + #13#10 +
                     'Are you sure you want to run this?') then exit;
     try
        // disable the export timer to allow processing time
        EnableTimer := False;
        TooBusy('Deleting Non Financial Members');
        if dm.exportTimer.Enabled then begin
           dm.exportTimer.Enabled := False;
           EnableTimer := True;
        end;   
        screen.cursor := crHourGlass;
        // get the latest points for these members 
        MenuMatePointsUpdate.MembersPointsUpdate('-1');
        with dm.ibdsMemberDelete do begin
           //close;
           //SQL.Text := format(SQL1,['MEMBER']);
           //ExecQuery;
           open;
           PointsCreditTypes := pcDeletedMember;
           First;
           while not EOF do begin
              dm.DeleteMemberLinks(FieldByName('MEMBER').AsInteger,dm.sqlRunOnce );
              dm.LogExport ( dm.sqlRunOnce, FieldByName('MEMBER').AsInteger,0,False);
              Global.BeforeEditPointsCreditSave(BeforeEditPointsCredit,dm.ibdsMemberDelete);
              MemberPayments.LogPointsCredit(dm.IBRunOnce,'Non Financial Member Delete',
                                                    BeforeEditPointsCredit,dm.ibdsMemberDelete,0.00);
              Delete;
           end;
           dm.ProcessType := ord(TProcessTypes(ptRollover));
           dm.LogProcess(dm.sqlRunOnce,'Non Financial Member Delete - Completed');
           try
              dm.IBRunOnce.Commit;
              MDlgI('Non Financial Member Delete - Completed');
           except;
              dm.IBRunOnce.Rollback;
              sm( 'Non Financial Member Delete - did not work');
           end;   
        end;   
     finally 
        if EnableTimer then
           dm.exportTimer.Enabled := True;
        NotBusy;
        screen.cursor := crDefault;
     end;
end;
{ ****************************************************************************}
end.
