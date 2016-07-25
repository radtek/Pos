unit MembersMAINT;

interface

uses          
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Menus, ImgList, ComCtrls, ToolWin, ExtCtrls,StdForm, StdCtrls, Buttons,
  wwrcdpnl, wwSpeedButton, wwDBNavigator, wwclearpanel, Grids, Wwdbigrd,
  Wwdbgrid, wwdbdatetimepicker,IB, DB, Wwkeycb, CheckLst,IBTable, Mask,
  wwdbedit, wwdblook, lmdcctrl, lmdctrl, lmdgroup,  Wwdotdot,
  Wwdbcomb,IBCustomDataSet,IBDataBase, DBCtrls, wwriched;

type
  TfrmMembersMAINT = class(TSMStdForm)
    PageControl: TPageControl;
    tsLinks: TTabSheet;
    tsLuckyMember: TTabSheet;
    tsBulkSection: TTabSheet;
    tsOther: TTabSheet;
    dbgOther: TwwDBGrid;
    tsSetupPoints: TTabSheet;
    rgpOther: TLMDRadioGroup;
    rgpLinks: TLMDRadioGroup;
    dbgLinks: TwwDBGrid;
    btnClose: TToolButton;
    dbnLinks: TwwDBNavigator;
    dbnLinksInsert: TwwNavButton;
    dbnLinksDelete: TwwNavButton;
    dbnLinksEdit: TwwNavButton;
    dbnLinksPost: TwwNavButton;
    dbnLinksCancel: TwwNavButton;
    dbnLinksRefresh: TwwNavButton;
    menuExit: TMenuItem;
    tsCards: TTabSheet;
    Panel1: TPanel;
    GroupBox1: TGroupBox;
    edtCardUsed: TEdit;
    Label4: TLabel;
    rgpCards: TLMDRadioGroup;
    Label5: TLabel;
    wwDBEdit1: TwwDBEdit;
    tsCardSetup: TTabSheet;
    pnlSpace: TPanel;
    Panel2: TPanel;
    dbgCards: TwwDBGrid;
    dbcbStyle: TwwDBComboBox;
    dbcbGraphicMode: TwwDBComboBox;
    dbcbFieldNames: TwwDBComboBox;
    dbcbRotation: TwwDBComboBox;
    Panel3: TPanel;
    GroupBox2: TGroupBox;
    Label8: TLabel;
    Label9: TLabel;
    edtHorizontal: TEdit;
    edtVertical: TEdit;
    btnCalculate: TButton;
    Panel4: TPanel;
    GroupBox3: TGroupBox;
    pnlCard: TPanel;
    lblPosition: TLabel;
    Panel5: TPanel;
    Panel6: TPanel;
    btnDelete: TButton;
    dbgSectionBulk: TwwDBGrid;
    Panel7: TPanel;
    Panel8: TPanel;
    Button1: TButton;
    Memo: TMemo;
    dbcbTextPos: TwwDBComboBox;
    tsManualCard: TTabSheet;
    Panel9: TPanel;
    dbgManualCard: TwwDBGrid;
    Panel10: TPanel;
    Panel11: TPanel;
    dbgPoints: TwwDBGrid;
    GroupBox4: TGroupBox;
    dblcMember: TwwDBLookupCombo;
    btnCards: TButton;
    dblcGroups: TwwDBLookupCombo;
    Panel13: TPanel;
    btnExportCrds: TButton;
    dbcbOnInvoice: TwwDBComboBox;
    btnAllMembers: TButton;
    Panel14: TPanel;
    GroupBox6: TGroupBox;
    edtMemberTime: TwwDBEdit;
    Label1: TLabel;
    Label2: TLabel;
    UpDown1: TUpDown;
    btnStartPhoto: TButton;
    Panel15: TPanel;
    dbdtStartTime: TwwDBDateTimePicker;
    Label3: TLabel;
    dblcRptGroups: TwwDBLookupCombo;
    dbdtEndTime: TwwDBDateTimePicker;
    Label7: TLabel;
    edtPhotoSize: TwwDBEdit;
    UpDown2: TUpDown;
    pnlFind: TPanel;
    IncSearch: TwwIncrementalSearch;
    Label10: TLabel;
    DBText1: TDBText;
    edtFullNAme: TwwDBEdit;
    dbdtExpires: TwwDBDateTimePicker;
    menuOptions: TMenuItem;
    menuLogonBonusPoints: TMenuItem;
    menuLogoffBonusPoints: TMenuItem;
    lblPoints: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    GroupBox5: TGroupBox;
    btnCancel: TButton;
    btnSave: TButton;
    rgpBonusPoints: TLMDRadioGroup;
    tsMailingLabels: TTabSheet;
    GroupBox8: TGroupBox;
    Label15: TLabel;
    wwDBComboBox1: TwwDBComboBox;
    wwDBEdit7: TwwDBEdit;
    Label14: TLabel;
    Label13: TLabel;
    wwDBEdit8: TwwDBEdit;
    GroupBox9: TGroupBox;
    Label19: TLabel;
    wwDBEdit2: TwwDBEdit;
    Label16: TLabel;
    wwDBEdit4: TwwDBEdit;
    Label18: TLabel;
    wwDBEdit5: TwwDBEdit;
    Label17: TLabel;
    wwDBEdit6: TwwDBEdit;
    wwDBEdit3: TwwDBEdit;
    Label20: TLabel;
    btnPaidMembers: TButton;
    tsKiosk: TTabSheet;
    wwDBGrid1: TwwDBGrid;
    tsKioskMsg: TTabSheet;
    btnGroupMembers: TButton;
    tsPointsAllocation: TTabSheet;
    Panel12: TPanel;
    dbgPointsAllocation: TwwDBGrid;
    btnStartPointsAllocation: TButton;
    GroupBox11: TGroupBox;
    DBCheckBox2: TDBCheckBox;
    DBCheckBox3: TDBCheckBox;
    DBCheckBox4: TDBCheckBox;
    DBCheckBox5: TDBCheckBox;
    DBCheckBox6: TDBCheckBox;
    DBCheckBox7: TDBCheckBox;
    DBCheckBox9: TDBCheckBox;
    wwDBEdit9: TwwDBEdit;
    Panel16: TPanel;
    Panel17: TPanel;
    GroupBox10: TGroupBox;
    Panel18: TPanel;
    GroupBox12: TGroupBox;
    DBCheckBox1: TDBCheckBox;
    wwDBEdit10: TwwDBEdit;
    dbdtMonday: TwwDBDateTimePicker;
    dbdtTuesday: TwwDBDateTimePicker;
    dbdtWednesday: TwwDBDateTimePicker;
    dbdtThursday: TwwDBDateTimePicker;
    dbdtFriday: TwwDBDateTimePicker;
    dbdtSaturday: TwwDBDateTimePicker;
    dbdtSunday: TwwDBDateTimePicker;
    GroupBox13: TGroupBox;
    edtLuckyDrawMsg: TwwDBEdit;
    wwDBEdit12: TwwDBEdit;
    wwDBEdit13: TwwDBEdit;
    Label21: TLabel;
    Label22: TLabel;
    Label23: TLabel;
    Panel19: TPanel;
    Panel20: TPanel;
    GroupBox7: TGroupBox;
    Label6: TLabel;
    GroupBox14: TGroupBox;
    Panel21: TPanel;
    btnLuckyMember: TButton;
    dbgKioskDraw: TwwDBGrid;
    edtKioskDraws: TwwDBEdit;
    Label24: TLabel;
    wwDBDateTimePicker1: TwwDBDateTimePicker;
    Label25: TLabel;
    tsPreferences: TTabSheet;
    Panel22: TPanel;
    Panel23: TPanel;
    Panel24: TPanel;
    Panel25: TPanel;
    GroupBox15: TGroupBox;
    dbgPrefHeading: TwwDBGrid;
    GroupBox16: TGroupBox;
    GroupBox17: TGroupBox;
    Panel26: TPanel;
    Panel27: TPanel;
    Button2: TButton;
    Panel28: TPanel;
    wwDBGrid2: TwwDBGrid;
    wwDBGrid3: TwwDBGrid;
    cbxDispHeading: TwwDBComboBox;
    cbxDetailType: TwwDBComboBox;
    cbxDisplayDetail: TwwDBComboBox;
    btnKiosk: TButton;
    procedure FormCreate(Sender: TObject);
    procedure PageControlChange(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure dbnLinksInsertClick(Sender: TObject);
    procedure btnLuckyMemberClick(Sender: TObject);
    procedure dbnLinksLoadClick(Sender: TObject);
    procedure dbgOtherCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure OpenCloseLast( theTable : TIBTable );
    procedure RefreshLast( theTable : TIBTable );
    procedure dblcMemberCloseUp(Sender: TObject; LookupTable,FillTable: TDataSet; modified: Boolean);
    procedure dblcMemberDropDown(Sender: TObject);
    procedure dblcMemberKeyPress(Sender: TObject; var Key: Char);
    procedure PopulatePoints(Cardtype : Boolean);
    procedure rgpOtherChange(Sender: TObject; ButtonIndex: Integer);
    procedure rgpLinksChange(Sender: TObject; ButtonIndex: Integer);
    procedure dbgOtherExit(Sender: TObject);
    procedure TopPanelVisible;
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure btnDeleteClick(Sender: TObject);
    procedure menuExitClick(Sender: TObject);
    procedure rgpCardsChange(Sender: TObject; ButtonIndex: Integer);
    procedure dbgCardsDblClick(Sender: TObject);
    procedure edtCardUsedDblClick(Sender: TObject);
    procedure pnlCardMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer);
    procedure btnCalculateClick(Sender: TObject);
    procedure edtHorizontalKeyPress(Sender: TObject; var Key: Char);
    procedure edtVerticalKeyPress(Sender: TObject; var Key: Char);
    procedure SetCardID ( ToClear : Boolean);
    procedure dbgCardsCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure FormResize(Sender: TObject);
    procedure dbgManualCardCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure dbgPointsCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure btnCardsClick(Sender: TObject);
    procedure btnCancelClick(Sender: TObject);
    procedure ResetPointsGrid;
    procedure btnSaveClick(Sender: TObject);
    procedure dbgPointsDblClick(Sender: TObject);
    procedure dbgPointsKeyPress(Sender: TObject; var Key: Char);
    procedure btnExportCrdsClick(Sender: TObject);
    procedure btnAllMembersClick(Sender: TObject);
    procedure btnStartPhotoClick(Sender: TObject);
    function SetupLuckyMember( OneLuckyMember : Boolean; var MaxId : LongInt ) : Boolean;
    procedure dbgManualCardTopRowChanged(Sender: TObject);
    procedure menuLogoffBonusPointsClick(Sender: TObject);
    procedure menuLogonBonusPointsClick(Sender: TObject);
    procedure RollbackBonusPoints;
    procedure dbgPointsAllocationCalcCellColors(Sender: TObject;
      Field: TField; State: TGridDrawState; Highlight: Boolean;
      AFont: TFont; ABrush: TBrush);
    procedure btnStartPointsAllocationClick(Sender: TObject);
    procedure edtLuckyDrawMsgEnter(Sender: TObject);
    procedure wwDBGrid1Enter(Sender: TObject);
    procedure edtKioskDrawsEnter(Sender: TObject);
    procedure dbgKioskDrawEnter(Sender: TObject);
    procedure dbgPrefHeadingEnter(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure CheckPrefFields;
    procedure dbgPrefHeadingExit(Sender: TObject);
    procedure btnKioskClick(Sender: TObject);
    
    
  private
    { Private declarations }
     FFunctionCArd : boolean;
     FpnlWidth : SmallInt;
  public
    { Public declarations }
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;

var
  frmMembersMAINT: TfrmMembersMAINT;
const SQL_POINTS = 'SELECT MEMBER,KNOWN_AS,POINTS_AVAILABLE,FULL_NAME,TOTAL_EARNT,TOTAL_REDEEMED,PAY,' +
                   'EARNT_CREDIT,BONUS_CREDIT,PRE_PAID_CREDIT,REASON from MEMBERS where';

implementation

{$R *.DFM}
uses
    Global,    // StockMaster
    MembersCLASSES,    // StockMaster
    MembersPHOTO,    // StockMaster
    dmMembers, Password, MembersREPORT, KioskPas; // StockMaster
    
{ **************************************************************************** }
class function TfrmMembersMAINT.ScreenCode : Integer;
begin Result := Global.scMembersMaintenance; end;
{ **************************************************************************** }
class function TfrmMembersMAINT.ScreenName : String;
begin Result := 'Members Settings'; end;
{ **************************************************************************** }
class function TfrmMembersMAINT.ScreenDesc : String;
begin Result := '  Members Settings Screen'; end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.FormCreate(Sender: TObject);
begin
     inherited;
     PageControl.ActivePage := tsLinks;
     FpnlWidth := pnlSpace.Width;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.FormActivate(Sender: TObject);
begin
     inherited;
     if Global.DiffRatio < 1.00 then begin // 600 * 480
        // need to refresh the new grid selected lengths
       OpenCloseLast(dm.tblSubs);
       OpenCloseLast(dm.tblSections);
       OpenCloseLast(dm.tblEvents);
       OpenCloseLast(dm.tblGroups);  
       OpenCloseLast(dm.tblMailCodes); // always first
     end else begin
       if dm.tblSubs.Active then
          RefreshLast(dm.tblSubs)
       else   
          OpenCloseLast(dm.tblSubs);
       RefreshLast(dm.tblSections);
       RefreshLast(dm.tblEvents);
       RefreshLast(dm.tblGroups);  
       RefreshLast(dm.tblMailCodes);  
     end;
     if not dm.PreCheckLogOn(scMaintenanceTabLinks,stMaintenanceTabLinks ) then RemoveTab(tsLinks);
     if not dm.PreCheckLogOn(scMaintenanceTabLucMem,stMaintenanceTabLucMem ) then RemoveTab(tsLuckyMember); 
     if not dm.PreCheckLogOn(scMaintenanceTabBulkSection,stMaintenanceTabBulkSection ) then RemoveTab(tsBulkSection);
     if not dm.PreCheckLogOn(scMaintenanceTabOther,stMaintenanceTabOther ) then RemoveTab(tsOther);
     if not dm.PreCheckLogOn(scMaintenanceTabSetupPoints,stMaintenanceTabSetupPoints ) then RemoveTab(tsSetupPoints);
     if not dm.PreCheckLogOn(scMaintenanceTabCards,stMaintenanceTabCards ) then RemoveTab(tsCards);
     if not dm.PreCheckLogOn(scMaintenanceTabCardSetup,stMaintenanceTabCardSetup ) then RemoveTab(tsCardSetup);
     if not dm.PreCheckLogOn(scMaintenanceTabManualCard,stMaintenanceTabManualCard ) then RemoveTab(tsManualCard);
     PageControlChange(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.FormDeactivate(Sender: TObject);
begin
     inherited;
     dm.IBTransactionCommit; // Commit changes on deactivate
     dbnLinks.datasource := dm.dsrMembers;
     dm.tblTenders.Close;
     dm.tblNumbers.Close;
     dm.tblMisc.Close;
     dm.tblKioskDraws.Close;
     dm.tblCardMember.Close;
     dm.qryReport2.Close;
     if dm.ibdsPrefDetail.Active then begin
        // Check Fields are in PREF_MEMBERS
        CheckPrefFields;
     end;
     dm.ibdsPrefDetail.Close;  
     dm.ibdsPrefSubHeading.Close;
     dm.ibdsPrefHeading.close;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.menuExitClick(Sender: TObject);
begin
     if Busy then exit;
     close;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.OpenCloseLast( theTable : TIBTable );
begin
      with theTable do begin
         Close;
         Open;
         Last;
      end;   
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.RefreshLast( theTable : TIBTable );
begin
      with theTable do begin
         if Active then begin
            Refresh;
            Last;
         end;   
      end;   
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.PageControlChange(Sender: TObject);
var v : Variant;
    x : Extended;
    Hour,Min,Sec,MSec : Word;
    Present : TTime;
const OneMinute = 0.0006944444;
      OneHour   = 0.0416666666;
begin
     stbStatus.panels[0].Text := '';
     dbnLinks.Enabled := False;
     pnlFind.Visible := True;
     incSearch.Clear;
     incSearch.Hint := 'Search by Description';
     incSearch.SearchField := 'DESCRIPTION';
     pnlFind.Visible := False;
     case PageControl.ActivePageIndex of
     0 : begin
            TopPanelVisible;
            dbnLinks.DataSource := dm.dsrTblSubs;
            incSearch.DataSource := dbnLinks.DataSource;
            rgpLinksChange(Sender,rgpLinks.ItemIndex);
            dbnLinksInsertClick(Sender);
            pnlFind.Visible := True;
         end;
     1 : begin
            if edtMemberTime.Text = '0' then
               edtMemberTime.Text := '1';
            if dbdtStartTime.Time = 0 then
               dbdtStartTime.Time := Time + (OneMinute * 5);
            if dbdtEndTime.Time = 0 then
               dbdtEndTime.Time := Time + OneHour + (OneMinute * 5);
            dbnLinks.Enabled := True;
            dbnLinks.DataSource := dm.dsrKioskDraws;
            TblActiveOrRefresh (dm.tblKioskDraws);
            TblActiveOrRefresh (dm.tblMisc);
         end;
     2 : begin // Bulk Section
            pnlFind.Visible := True;
            incSearch.DataSource := dbgSectionBulk.DataSource;
         end;
     3 : begin  // Other  Mail Codes, Tenders and Member Numbers
            TopPanelVisible;
            TblActiveOrRefresh (dm.tblTenders);
            TblActiveOrRefresh (dm.tblNumbers);
            TblActiveOrRefresh (dm.tblClubs);
            rgpOtherChange(Sender,rgpOther.ItemIndex);
         end;
     4 : begin
            dm.qryMembersLook.Open;
            if not lblPoints.Visible then begin
               if PointsPassword then 
                  dbgPoints.ReadOnly := True
               else   
                  dbgPoints.ReadOnly := False;
            end;      
            dblcMember.SetFocus;  // points
            dm.StartTrans(dm.IBRunOnce);
            IBDSActiveOrRefresh(dm.ibdsPoints);
         end;
     5 : begin // Cards
            TopPanelVisible;
            TblActiveOrRefresh (dm.tblCardMaster);
            TblActiveOrRefresh (dm.tblCardDetail);
            if DefaultCardID <> -1 then begin
               v := DefaultCardID;
               dm.tblCardMaster.Locate('CARD_ID',v,[]);
               edtCardUsed.Text := dm.tblCardMaster.FieldByName('CARD_NAME').AsString;
            end;
            rgpCardsChange(Sender,rgpCards.ItemIndex);
         end;
     7 : begin
            TopPanelVisible;
            TblActiveOrRefresh (dm.tblCardMember);
            dbnLinks.DataSource := dm.dsrCardMember;
            dbgManualCardTopRowChanged(Sender);
            pnlFind.Visible := True;
            incSearch.DataSource := dbgManualCard.DataSource;
            incSearch.SearchField := 'MEMBER';
            incSearch.Hint := 'Search by Member';
         end;
     8 : begin
            TopPanelVisible;
            IBDSActiveOrRefresh (dm.ibdsMailingLabels);
            dbnLinks.DataSource := dm.dsrMailingLabels;
            pnlFind.Visible := False;
         end;
     9 : begin
            TopPanelVisible;
            // has 2 tables but only one navigator
            TblActiveOrRefresh (dm.tblKiosk);
            TblActiveOrRefresh (dm.tblMisc);
            dbnLinks.DataSource := dm.dsrTblMisc;
           // dbnLinks.DataSource := dm.dsrKiosk;
            edtLuckyDrawMsg.Setfocus;
            pnlFind.Visible := False;
         end;
     10 : begin
            TopPanelVisible;
            TblActiveOrRefresh (dm.tblMisc);
            dbnLinks.DataSource := dm.dsrTblMisc;
            pnlFind.Visible := False;
         end;    
     11 : begin
            TopPanelVisible;
            IBDSActiveOrRefresh (dm.ibdsPointsAllocation);
            dbnLinks.DataSource   := dm.dsrPointsAllocation;
            incSearch.DataSource  := dbnLinks.DataSource;
            incSearch.SearchField := 'MEMBER';
            pnlFind.Visible := True;
         end;
     12 : begin // customer preferences
            TopPanelVisible;
            dbnLinks.DataSource := dm.dsrPrefHeading;
            dbgPrefHeading.SetFocus;
            IBDSActiveOrRefresh (dm.ibdsPrefDetail);
            IBDSActiveOrRefresh (dm.ibdsPrefSubHeading);
            IBDSActiveOrRefresh (dm.ibdsPrefHeading);
            //incSearch.DataSource := dbnLinks.DataSource;
            //pnlFind.Visible := True;
         end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.TopPanelVisible;
begin
     dbnLinks.Enabled := True;
     stbStatus.panels[0].Text:= 'Browse';
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dbnLinksInsertClick(Sender: TObject);
begin
     case PageControl.ActivePageIndex of 
     0 : begin
            dbgLinks.SetFocus;
            dbgLinks.SetActiveField('DESCRIPTION');
         end;
    { 4 : begin //Points  dont wont 
            if (Sender as TwwNavButton).Name = dbnLinksPost.Name then begin
               btnSaveClick(Sender);
               abort;
            end else if (Sender as TwwNavButton).Name = dbnLinksCancel.Name then begin
               btnCancelClick(Sender);
               abort;
            end else if (Sender as TwwNavButton).Name = dbnLinksInsert.Name then begin
               abort;
            end;                        
         end;  }
     7 : begin
            dbgManualCard.SetFocus;
            dbgManualCard.SetActiveField('MEMBER');
         end;
     3 : begin   // Other
           dbgOther.SetFocus;
           case rgpOther.ItemIndex of 
              1 : begin // Tenders
                     if (TwwDBNavigator(Sender).Name = dbnLinksInsert.Name) or
                        (TwwDBNavigator(Sender).Name = dbnLinksDelete.Name) then begin
                        sm('Cannot Insert or Delete from Tenders');
                        abort;
                     end;
                  end;
              2 : begin // member numbers
                     if (TwwDBNavigator(Sender).Name = dbnLinksDelete.Name) then begin
                        if dm.tblNumbers.FieldByName('NUMBER_ID').AsInteger = 0 then begin
                           sm('Cannot Delete this entry');
                           abort;
                        end;
                     end;  
                  end;
           end;
         end;
     5 : begin // Cards
            if rgpCards.ItemIndex = 0 then begin
               if (TwwDBNavigator(Sender).Name = dbnLinksDelete.Name) then begin
                  if dm.tblCardMaster.FieldByName('CARD_ID').AsInteger = DefaultCardID then begin
                     MDlgI('The Default Card needs to be setup');
                     SetCardID(True);
                  end;   
                end;   
            end;   
            dbgCards.SetFocus;
            case rgpCards.ItemIndex of 
               0 : dbgCards.SetActiveField('CARD_NAME');
               1 : dbgCards.SetActiveField('FIELD_NAME');
            end;
         end;      
     10 : begin
              // post
              if dm.tblMisc.State in [dsInsert,dsEdit] then begin
                 dm.tblMisc.Post;
                 dm.IBCommitRetain;
                 dm.tblMisc.Edit;
                 
              end;
          end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.btnLuckyMemberClick(Sender: TObject);
var
   MaxID,LuckyID,LuckyMember : LongInt;
   s : string[50];
const LuckyMemberMsg = 'Lucky Member';  
      SQL1 = 'Select MEMBER from LUCKY_MEMBER where LUCKY_ID = %d';
      SQL2 = 'Select FULL_NAME from MEMBERS where MEMBER = %d';
      SQL3 = 'UPDATE MEMBERS set LUCKY_DATE = ''%s'' where MEMBER = %d';
label ReSelect;
// puts member into LUCKY_MEMBER DB with LUCKY_ID as autoinc
// randomize picks the LUCKY_ID and the MEMBER beside it, LUCKY_ID will be sequential 1,2 .. 3000
// done because of big gaps in the file
begin
     if Busy then exit;
     TooBusy('Selecting a ' + LuckyMemberMsg + PleaseWaitMsg);
     try
        if not SetupLuckyMember( True, MaxId ) then exit;
        if not dm.IBRunOnce.InTransaction then
            dm.IBRunOnce.StartTransAction;
        try
            Randomize;
            ReSelect:;
            LuckyID := Random(MaxID);
            if MaxID < 1 then goto ReSelect;
           // Member will always be Financial and have not won before
           if dm.sqlExecQueryCheckEOF(dm.sqlRunOnce,Format(SQL1,[LuckyID])) then begin
               LuckyMember := dm.sqlRunOnce.FieldByName('Member').AsInteger;
               // get the full name
               if dm.sqlExecQueryCheckEOF(dm.sqlRunOnce,Format(SQL2,[LuckyMember])) then
                  s := dm.sqlRunOnce.FieldByName('FULL_NAME').AsString
               else
                  goto ReSelect;
           end else
              goto ReSelect;
           dm.sqlExecQuery(dm.sqlRunOnce,Format(SQL3,[GetShortDate(Date),LuckyMember]));
           dm.ProcessType := ord(TProcessTypes(ptMaintain));
           dm.LogProcess( dm.sqlRunOnce,Format(LuckyMemberMsg + ' Picked %d %s',[LuckyMember,s]));
           if dm.IBRunOnce.InTransaction then // Commit the last Update
              dm.IBRunOnce.Commit;
           NotBusy;
           MdlgI( Format(LuckyMemberMsg + ' = %d  %s',[LuckyMember,s]));
           //btnLuckyMember.Enabled := False;
        except
           if dm.IBRunOnce.InTransaction then dm.IBRunOnce.RollBack;
           sm(LuckyMemberMsg + PleaseTryAgainMsg);
        end;  
     finally
        NotBusy;
     end;
end;
{ **************************************************************************** }
function TfrmMembersMAINT.SetupLuckyMember( OneLuckyMember : Boolean; var MaxId : LongInt ) : Boolean;
const SQL1 = 'Delete from LUCKY_MEMBER';
      SQL2 = 'SET GENERATOR LUCKY_GEN TO 0';
      SQL3 = 'INSERT INTO LUCKY_MEMBER SELECT m.MEMBER,m.MEMBER FROM MEMBERS m,M_PHOTOS mp ' +
             'WHERE m.SUB_PAID > 0 and m.MEMBER = mp.MEMBER';
      SQL5 = 'INSERT INTO LUCKY_MEMBER SELECT MEMBER,MEMBER FROM MEMBERS ' +
             'WHERE SUB_PAID > 0 and LUCKY_DATE is null';
      SQL4 = 'Select Max(LUCKY_ID) from LUCKY_MEMBER';
      msg1 = 'No Lucky Members to choose from';
begin
     Result := False;
     TooBusy('Setting up Lucky Member ' + PleaseWaitMsg);
     if not dm.IBRunOnce.InTransaction then
        dm.IBRunOnce.StartTransAction;
     try
        dm.sqlExecQuery(dm.sqlRunOnce,SQL1);
        dm.sqlExecQuery(dm.sqlRunOnce,SQL2);
        // One Lucky Member or Photo Draw
        case OneLuckyMember of
           true : dm.sqlExecQuery(dm.sqlRunOnce,SQL5);
           false : dm.sqlExecQuery(dm.sqlRunOnce,SQL3);
        end;
        MaxID := -1;
        if dm.sqlExecQueryCheckEOFCount(dm.sqlRunOnce,SQL4) then
           MaxID := dm.sqlRunOnce.Fields[0].AsInteger;
        if MaxID < 1 then begin
           sm(msg1);
           dm.IBRunOnce.RollBack;
           exit;
        end;
        dm.IBRunOnce.Commit;
        NotBusy;
     except
        dm.IBRunOnce.RollBack;
        sm('Error setting up Lucky Member');
     end;
     Result := True;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dbnLinksLoadClick(Sender: TObject);
const bSection = 'Bulk Section Load';
      SQL1 = 'SELECT MEMBER from MEMBERS where MEMBER = %s';
      SQL2 = 'SELECT MEMBER from M_SECTIONS where MEMBER = %s and SECTION = %d';
      SQL3 = 'INSERT INTO M_SECTIONS (MEMBER,SECTION,RECEIPT,EXPIRES,PAID,SPONSORSHIP,' +
             'EDIT_DATE,AMOUNT,AMOUNT_PAID) VALUES (%s,%d,0,''%s'',''N'',''N'',''%s'',%f,0.00)';
var i : integer;
    StopError : Boolean;
    SectionDesc : string[30];
    Section : Integer;
    Fee     : extended;
    EDate   : string[20];
    Exp     : string[12];
    s : string;
begin
     if Busy then exit;
     if Memo.Lines.Count = 0 then begin
        sm('You must load some Members first');
        exit;
     end;
     dbnLinks.DataSource := dm.dsrTblSubs;
     //if dm.LoggedOnUsers > 1 then begin // allow for self
     //   sm(UsersLoggedOnMsg);
     //end;
     SectionDesc := dm.tblSections.FieldByName('DESCRIPTION').AsString;
     Section     := dm.tblSections.FieldByName('SECTION').AsInteger;
     if not IfMsgdlg(Format(AreYouSureMsg + bSection + ' of Members into Section %s',[SectionDesc])) then exit;
     TooBusy(bSection + PleaseWaitMsg);
     try
        try
           if not dm.IBRollover.InTransaction then
               dm.IBRollover.StartTransAction;
           // check for valid members in DB
           StopError := False;
           s := 'Members which are not Numeric' + #13#10;
           for i := 0 to Memo.lines.Count -1 do begin
              if not Global.AllDigits(Memo.Lines[i]) then begin
                 //Memo.lines.Delete(i);
                  s := s + Memo.Lines[i] + #13#10;
                  StopError := True;
              end;
           end;
           if StopError then begin
              ShowMessage(s + #13#10 + ' These must be fixed before continuing');
              exit;
           end;
           s := 'Not valid Members' + #13#10;
           for i := 0 to Memo.lines.Count -1 do begin
              if not dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL1,[Memo.lines[i]])) then begin
                 //Memo.lines.Delete(i);
                 s := s + Memo.Lines[i] + #13#10;
                 StopError := True;
              end;
           end;
           if StopError then begin
              ShowMessage(s + #13#10 + ' These must be fixed before continuing');
              exit;
           end;
           // members already in the Section
           s := format('Members already in Section %s' + #13#10,[SectionDesc]);
           for i := 0 to Memo.lines.Count -1 do begin
              if dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL2,[Memo.lines[i],Section])) then begin
                 s := s + Memo.lines[i] + #13#10;
                 StopError := True;
              end;
           end;
           if StopError then begin
              ShowMessage(s + #13#10 + ' These must be fixed before continuing');
              exit;
           end;
           Fee    := dm.tblSections.FieldByName('FEE').AsFloat;
           EDate  := GetShortDateTime(now);
           Exp    := GetShortDate(dm.tblSections.FieldByName('EXPIRES').AsDateTime);
           for i := 0 to Memo.lines.Count -1 do begin
              dm.sqlExecQuery(dm.sqlRollover,format(SQL3,[Memo.lines[i],Section,Exp,EDate,Fee]));
           end;
           dm.ProcessType := ord(TProcessTypes(ptMaintain));
           dm.LogProcess(dm.sqlRollover,bSection + ' of Members into Section ' + dm.tblSections.FieldByName('SECTION').AsString);
           if dm.IBRollover.InTransaction then
              dm.IBRollover.Commit;
           NotBusy;
           MDlgI(bSection + CompletedMsg);
           memo.clear;
        except
           if dm.IBRollover.InTransaction then begin
              dm.IBRollover.RollBack;
              dm.IBRollover.StartTransaction;
           end;
           NotBusy;
           sm(UsersMakingChangesMsg + #13#10 + 'The ' + bSection + PleaseTryAgainMsg );
        end;
     finally
        NotBusy;
        dm.sqlRollover.Close;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dbgOtherCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;
  ABrush: TBrush);
begin
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dblcMemberCloseUp(Sender: TObject; LookupTable,
  FillTable: TDataSet; modified: Boolean);
begin
     if modified then PopulatePoints(False);
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dblcMemberDropDown(Sender: TObject);
var s : shortstring;// pre drop down selection
const SQl1 = 'select SURNAME,MEMBER,FULL_NAME,FIRST_NAME,ADDRESS,MAILING_LIST_CODE,EXPIRES from MEMBERS ';
begin
     s := dblcMember.Text;
     if s = '' then exit;
     s := uppercase(s);
     ResetPointsGrid;
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
procedure TfrmMembersMAINT.dblcMemberKeyPress(Sender: TObject;
  var Key: Char);
begin
    if key = '''' then key := '`'; // change ' to `  - for sql
    if key = #13 then begin
       dblcMemberDropDown(Sender);
       if not dm.qryMembersLook.EOF then 
          PopulatePoints(False);
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.PopulatePoints(Cardtype : Boolean);
const SQL3 = ' MEMBER = %s' ;
      SQL2 = ' CARD_TYPE = ''F'' order by Member';
var   MenuMatePointsUpdate : TMenuMatePointsUpdate;
begin
     if not CardType and (dblcMember.Text = '') then exit;
     // update the points before changing them
     MenuMatePointsUpdate.MembersPointsUpdate(dm.qryMembersLook.FieldByName('MEMBER').AsString);
     with dm.ibdsPoints do begin
        Close;
        case CardType of
           True  : begin 
                      SelectSQL.Text := SQL_POINTS + SQL2; 
                      FFunctionCArd := True; 
                   end;
           False : begin 
                      SelectSQL.Text := format(SQL_POINTS + SQL3,[dblcMember.Text]); 
                      FFunctionCArd := False; 
                   end;
        end; 
        Open;  
        try
           disablecontrols;
           first;
           while not eof do begin
              edit;
              FieldByName('PAY').AsFloat := 0.00;
              post;
              next;
           end;
           dbgPoints.Setfocus;
           dbgPoints.SetActiveField('PAY');
        finally
           Enablecontrols;
        end; 
     end;
     dbgPoints.Setfocus;
     dbgPoints.SetActiveField('PAY');
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.btnSaveClick(Sender: TObject);
var pay : Extended;
    MemberPayments : TMemberPayments;
    BeforeEditPointsCredit : TBeforeEditPointsCredit;
    ResetPoints : Boolean;
    s,TmpMsg : string[40];
const msg1 = 'Reset of Credit/Points - %s';    
      msg2 = 'Bonus Points - %s';
begin
     if not ifmsgdlg(format('Save "%s" for selected Members',[rgpBonusPoints.Items[rgpBonusPoints.ItemIndex]])) then exit;
     case rgpBonusPoints.ItemIndex of 
        1 : begin
              PointsCreditTypes := pcReset;
              ResetPoints := True;
              TmpMsg := msg1; 
            end;
        0 : begin    
              PointsCreditTypes := pcSetup;
              ResetPoints := False;
              TmpMsg := msg2;
            end;  
     end;   
     with dm.ibdsPoints do begin
        try
           if State = dsEdit then 
              Post;
           Disablecontrols;
           // Refresh to latest data
           Close;
           Open;
           first;
           while not eof do begin
              // no alteration
              if (PointsCreditTypes = pcSetup) and (FieldByName('PAY').AsFloat = 0.00) then begin
                 next;
                 continue;
              end;
              {if ((FieldByName('TOTAL_EARNT').AsFloat - FieldByName('TOTAL_REDEEMED').AsFloat) +
                 FieldByName('PAY').AsFloat  < 0.00) and not FFunctionCard then begin
                 if not ifMsgDlg('Points Available will become a negative value - continue?') then begin
                    RollbackBonusPoints;
                    exit;
                 end;
              end;}
              // save current points values
              Global.BeforeEditPointsCreditSave(BeforeEditPointsCredit,dm.dsrPoints.DataSet);  
              edit;
              Pay := FieldByName('PAY').AsFloat;
              // PointsCreditTypes are set - see above
              if not MemberPayments.PointsCreditUpdate(FieldByName('PAY').AsFloat,dm.dsrPoints.DataSet) then begin
                 sm('There has been an error when updating Credit/Points');
                 RollbackBonusPoints;
                 exit;
              end;
              post;
              // ResetPoints - see above  true if all credits/points are to be reset
              dm.LogExport(dm.sqlRunOnce,FieldByName('MEMBER').AsInteger,Round( Pay * 100 ),ResetPoints);
              // known As or Full_name alteration - send anyway
              dm.LogExport(dm.sqlRunOnce,FieldByName('MEMBER').AsInteger,0,False);
              // Points Log
              if FieldByName('REASON').IsNull then 
                 s := format(TmpMsg,[ScreenName])
              else begin  
                 s := FieldByName('REASON').AsString;
                 edit;
                 // dont save the string - dont want it to reappear in Maintenance
                 FieldByName('REASON').AsString := '';
                 post;
              end;   
              if not MemberPayments.LogPointsCredit(dm.IBRunOnce,s,
                                                    BeforeEditPointsCredit,
                                                    dm.dsrPoints.DataSet,
                                                    FieldByName('PAY').AsFloat)
                                                    then RollbackBonusPoints;
              next;
           end;
           try
             if dm.IBRunOnce.InTransaction then begin
                dm.IBRunOnce.Commit;
                dm.IBRunOnce.StartTransAction;
             end;
             //PopulatePoints(FFunctionCard);
             ResetPointsGrid;
             dblcMember.Setfocus;
             dblcMember.SelectAll;
           except
             on E: Exception do begin
                sm('The changes to Points have NOT been accepted' + E.Message);
                RollbackBonusPoints;
             end;   
           end;
        finally
           Enablecontrols;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.RollbackBonusPoints;
begin
     if dm.IBRunOnce.InTransaction then begin
        dm.IBRunOnce.Rollback;
        dm.IBRunOnce.StartTransAction;
     end;
     PopulatePoints(FFunctionCard);
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.rgpOtherChange(Sender: TObject;  ButtonIndex: Integer);
const Gtab = #9;  
begin
     if dm.dsrTblMailCodes.State in [dsEdit,dsInsert] then dm.TblMailCodes.Post;
     if dm.dsrTblTenders.State   in [dsEdit,dsInsert] then dm.tblTenders.Post;   
     if dm.dsrTblNumbers.State   in [dsEdit,dsInsert] then dm.tblNumbers.Post;
     if dm.dsrClubs.State        in [dsEdit,dsInsert] then dm.tblClubs.Post;
     pnlFind.Visible := False;
     case rgpOther.ItemIndex of
        0 : begin // Mail Codes
               with dbgOther do begin
   	            Selected.Clear;
      	         Selected.Add('MAIL_CODE' + Gtab + '12' + Gtab + 'Mail Code');
      	         Selected.Add('DESCRIPTION' + Gtab + '50' + Gtab + 'Description');
         	      ApplySelected;
                  DataSource := dm.dsrTblMailCodes;
                  dbnLinks.DataSource := dm.dsrTblMailCodes;
                end;
            end;
        1 : begin // Tenders
               with dbgOther do begin
   	            Selected.Clear;
      	         Selected.Add('TENDER_TYPE' + Gtab + '48' + Gtab + 'Tender Name');
      	         Selected.Add('EX_RATE' + Gtab + '16' + Gtab + 'Exch Rate');
         	      ApplySelected;
                  DataSource := dm.dsrTblTenders;
                  dbnLinks.DataSource := dm.dsrTblTenders;
                end;
            end;
        2 : begin // Member Numbers
               with dbgOther do begin
   	            Selected.Clear;
      	         Selected.Add('NUMBER_ID' + Gtab + '12' + Gtab + 'NumberID');
      	         Selected.Add('DESCRIPTION' + Gtab + '35' + Gtab + 'Description');
      	         Selected.Add('NEXT_MEMBER' + Gtab + '18' + Gtab + 'Next Member');
         	      ApplySelected;
                  DataSource := dm.dsrTblNumbers;
                  dbnLinks.DataSource := dm.dsrTblNumbers;
                end;
            end;
        3 : begin // Clubs
               with dbgOther do begin
   	            Selected.Clear;
      	            Selected.Add('CLUB_ID' + Gtab + '12' + Gtab + 'Club ID');
      	            Selected.Add('DESCRIPTION' + Gtab + '50' + Gtab + 'Description');
         	    ApplySelected;
                    DataSource := dm.dsrClubs;
                    dbnLinks.DataSource := dm.dsrClubs;
                end;
                pnlFind.Visible := True;
            end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.rgpLinksChange(Sender: TObject; ButtonIndex: Integer);
const Gtab = #9;
//FieldNameArray : ( 'SUBSCRIPTION','SECTION','EVENT','GROUPS');
begin
     case rgpLinks.ItemIndex of
        0 : begin // Subs
               with dbgLinks do begin
   	            Selected.Clear;
      	         Selected.Add(FieldNameArray[0] + Gtab + '6' + Gtab + 'Subscription');
      	         Selected.Add('CODE' + Gtab + '6' + Gtab + 'Code');
      	         Selected.Add('DESCRIPTION' + Gtab + '21' + Gtab + 'Description');
      	         Selected.Add('FEE' + Gtab + '13' + Gtab + 'Fee');
      	         Selected.Add('KEEP_EXPIRES_CURRENT' + Gtab + '12' + Gtab + 'Expires Current');
      	         Selected.Add('Term' + Gtab + '7' + Gtab + 'Term');
      	         Selected.Add('DISCOUNTABLE' + Gtab + '7' + Gtab + 'Discount');
      	         Selected.Add('PRO_RATA' + Gtab + '7' + Gtab + 'Pro Rata');
      	         Selected.Add('PRINT_ON_CARD' + Gtab + '8' + Gtab + 'On Card');
      	         Selected.Add('CARD_DESC' + Gtab + '21' + Gtab + 'Card Description');
      	         Selected.Add('SUB_GROUPS' + Gtab + '10' + Gtab + 'Group');
      	         Selected.Add('GROUP_DESC' + Gtab + '20' + Gtab + 'Group Desc');
      	         Selected.Add('AP_PREMIUM' + Gtab + '7' + Gtab + 'AP Premium');
      	         Selected.Add('ON_INVOICE' + Gtab + '7' + Gtab + 'On Invoice');
                  // reuse  as LAte Payment Sub
      	         Selected.Add('SPARE_CHAR' + Gtab + '12' + Gtab + 'Late Payment');
                  // reuse  as Card Ranking
      	         Selected.Add('SPARE_SMALLINT' + Gtab + '10' + Gtab + 'Card Ranking');
                  // reuse  as Rpt Group
      	         Selected.Add('POINTS_SPENT' + Gtab + '12' + Gtab + 'Rpt Group');
      	         Selected.Add('EXPIRES_CARD' + Gtab + '19' + Gtab + 'Expires not on Card');
      	         Selected.Add('EXPIRES_DESC' + Gtab + '19' + Gtab + 'Expires Desc');
         	      ApplySelected;
                  DataSource := dm.dsrTblSubs;
                  dbnLinks.DataSource := dm.dsrTblSubs;
                  tblActiveorRefresh (dm.TblSubs);
                end;
            end;    
        1 : begin // Sections
               with dbgLinks do begin
   	            Selected.Clear;
      	         Selected.Add(FieldNameArray[1] + Gtab + '7' + Gtab + 'Section');
      	         Selected.Add('DESCRIPTION' + Gtab + '24' + Gtab + 'Description');
      	         Selected.Add('EXPIRES' + Gtab + '12' + Gtab + 'Expires');
      	         Selected.Add('FEE' + Gtab + '9' + Gtab + 'Fee');
      	         Selected.Add('MIN_FEE' + Gtab + '9' + Gtab + 'Min Fee');
      	         Selected.Add('LAST_ROLL' + Gtab + '12' + Gtab + 'Last Roll');
      	         Selected.Add('PRINT_ON_CARD' + Gtab + '7' + Gtab + 'On Card');
      	         Selected.Add('CARD_DESC' + Gtab + '35' + Gtab + 'Card Description');
      	         Selected.Add('DONATION' + Gtab + '14' + Gtab + 'Donation %');
         	      ApplySelected;
                  DataSource := dm.dsrTblSections;
                  dbnLinks.DataSource := dm.dsrTblSections;
                  tblActiveorRefresh (dm.TblSections);
                end;
            end;    
        2 : begin // Events
               with dbgLinks do begin  
   	            Selected.Clear;
      	         Selected.Add(FieldNameArray[2] + Gtab + '10' + Gtab + 'Event');
      	         Selected.Add('DESCRIPTION' + Gtab + '36' + Gtab + 'Description');
      	         Selected.Add('BONUS_POINTS' + Gtab + '17' + Gtab + 'Bonus Points');
         	      ApplySelected;
                  DataSource := dm.dsrTblEvents;
                  dbnLinks.DataSource := dm.dsrTblEvents;
                  tblActiveorRefresh (dm.TblEvents);
                end;
            end;    
        3 : begin // Groups
               with dbgLinks do begin  
   	            Selected.Clear;
      	         Selected.Add(FieldNameArray[3] + Gtab + '10' + Gtab + 'Group');
      	         Selected.Add('DESCRIPTION' + Gtab + '35' + Gtab + 'Description');
      	         Selected.Add('PRINT_ON_CARD' + Gtab + '7' + Gtab + 'On Card');
      	         Selected.Add('CARD_DESC' + Gtab + '35' + Gtab + 'Card Description');
      	         Selected.Add('IN_REPORT' + Gtab + '12' + Gtab + 'In Rec Rpt');
         	      ApplySelected;
                  DataSource := dm.dsrTblGroups;                                 
                  dbnLinks.DataSource := dm.dsrTblGroups;
                  tblActiveorRefresh (dm.TblGroups);
                end;
            end;    
     end;
     incSearch.DataSource := dbnLinks.DataSource;
     incSearch.Clear;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dbgOtherExit(Sender: TObject);
begin
     if TwwDBGrid(Sender).Name = dbgLinks.Name then begin
        case rgpLinks.ItemIndex of                 
           0 : if dm.dsrTblSubs.State in [dsEdit,dsInsert] then dm.TblSubs.Post;
           1 : if dm.dsrTblSections.State in [dsEdit,dsInsert] then dm.TblSections.Post;
           2 : if dm.dsrTblEvents.State in [dsEdit,dsInsert] then dm.TblEvents.Post;
           3 : if dm.dsrTblGroups.State in [dsEdit,dsInsert] then dm.TblGroups.Post;
        end;
     end;
     if TwwDBGrid(Sender).Name = dbgOther.Name then begin
        case rgpOther.ItemIndex of                 
           0 : if dm.dsrTblMailCodes.State in [dsEdit,dsInsert] then dm.TblMailCodes.Post;   
           1 : if dm.dsrTblTenders.State   in [dsEdit,dsInsert] then dm.tblTenders.Post;   
           2 : if dm.dsrTblNumbers.State   in [dsEdit,dsInsert] then dm.tblNumbers.Post;   
           3 : if dm.dsrClubs.State        in [dsEdit,dsInsert] then dm.tblClubs.Post;   
        end;   
     end; 
     if TwwDBGrid(Sender).Name = dbgCards.Name then begin
        case rgpCards.ItemIndex of                 
           0 : if dm.dsrCardMaster.State in [dsEdit,dsInsert] then dm.tblCardMaster.Post;   
           1 : if dm.dsrCardDetail.State in [dsEdit,dsInsert] then dm.tblCardDetail.Post;   
        end;   
     end; 
     if TwwDBGrid(Sender).Name = dbgManualCard.Name then 
        if dm.dsrCardMember.State in [dsEdit,dsInsert] then dm.tblCardMember.Post;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
var tabsheet : TTabSheet;
begin
     inherited;
     case key of   
         VK_ESCAPE : menuExitClick(Sender);
         VK_F9     : if PAgeControl.ActivePAge = tsSetupPoints then btnSaveClick(Sender); // Points
     end;   
     if ( ssAlt in shift ) then begin
        tabsheet := nil;
        case key of 
           84,117 : tabsheet := tsLuckyMember; // U    
           83,115 : tabsheet := tsSetupPoints; // S
           76,108 : tabsheet := tsLinks;       // L
           66,98  : tabsheet := tsBulkSection; // B
           79,111 : tabsheet := tsOther;       // O
           67,99  :  btnCalculateClick(Sender);       // C
           //80,112 : btnAdjustPointsClick(Sender); // P
           88,120 : menuExitClick(Sender); // x Close 
        end;  
        if tabsheet <> nil then
           if tabsheet.TabVisible then PageControl.ActivePage := tabsheet;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.btnDeleteClick(Sender: TObject);
const bSection = 'Bulk Section Delete';
      SQL1 = 'DELETE from M_SECTIONS where SECTION = %d';
begin

     if Busy then exit;
     dbnLinks.DataSource := dm.dsrTblSubs;
     //if dm.LoggedOnUsers > 1 then begin // allow for self
     //   sm(UsersLoggedOnMsg);
     //end;
     if not IfMsgdlg(Format('Are you sure you want delete All Members from the ' +
        '"%s" Section',[dm.tblSections.FieldByName('DESCRIPTION').AsString])) then exit;
     TooBusy( bSection + PleaseWaitMsg);
     try
        try
           if not dm.IBRollover.InTransaction then
               dm.IBRollover.StartTransAction;
           // removes all Members from section
           dm.sqlExecQuery(dm.sqlRollover,format(SQL1,[dm.tblSections.FieldByName('SECTION').AsInteger]));
           dm.ProcessType := ord(TProcessTypes(ptMaintain));
           dm.LogProcess(dm.sqlRollover, bSection + 'of Members from Section ' + dm.tblSections.FieldByName('SECTION').AsString);
           if dm.IBRollover.InTransaction then
              dm.IBRollover.Commit;
           NotBusy;
           MDlgI(bSection + CompletedMsg);
        except
           if dm.IBRollover.InTransaction then begin
              dm.IBRollover.RollBack;
              dm.IBRollover.StartTransaction;
           end;
           sm(UsersMakingChangesMsg + #13#10 + 'The ' + bSection + PleaseTryAgainMsg);
        end;
     finally
        dm.sqlRollover.Close;
        NotBusy;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.rgpCardsChange(Sender: TObject; ButtonIndex: Integer);
begin
     dbgOtherExit(dbgCards); // Post before changing
     case rgpCards.ItemIndex of 
        0 : begin // Card Master
               with dbgCards do begin
   	            Selected.Clear;
      	         //Selected.Add('CARD_ID'#9'15'#9'Card ID');
      	         Selected.Add('CARD_NAME'#9'80'#9'Card Name');
         	      ApplySelected;
                  DataSource := dm.dsrCardMaster;
                  dbnLinks.DataSource := dm.dsrCardMaster;
                  dbgCards.Hint := 'Double Click to Setup up the Default Card'; 
                end;
            end;    
        1 : begin // Card Detail
               if  dm.tblCardMaster.RecordCount = 0 then begin
                  //dbgCards.DataSource := nil;
                  //dbnLinks.DataSource := nil;
                  sm('You must Create a Card first');
                  rgpCards.ItemIndex := 0;
               end else begin
                  with dbgCards do begin
      	            Selected.Clear;
                     Selected.Add('FIELD_NAME'#9'15'#9'Field Name'#9'F');
                     Selected.Add('TEXT_POSITION'#9'11'#9'Text Position'#9'F');
                     Selected.Add('CARD_TEXT'#9'27'#9'Card Text'#9'F');
                     Selected.Add('HORIZONTAL'#9'11'#9'Horizontal'#9'F');
                     Selected.Add('VERTICAL'#9'10'#9'Vertical'#9'F');
                     Selected.Add('ROTATION'#9'21'#9'Rotation'#9'F');
                     Selected.Add('STYLE'#9'9'#9'Style'#9'F');
                     Selected.Add('PROPORTION'#9'12'#9'Proportion'#9'F');
                     Selected.Add('SIZE'#9'6'#9'Size'#9'F');
                     Selected.Add('GRAPHIC_MODE'#9'18'#9'Graphic Mode'#9'F');
                     Selected.Add('TEXT_LENGTH'#9'18'#9'Length before Reduction'#9'F');
                     Selected.Add('SIZE_REDUCE'#9'18'#9'Size Reduction'#9'F');
                     //Selected.Add('MULTI_GAP'#9'12'#9'Multi Gap'#9'F');
                     //Selected.Add('EXPORT_LINES'#9'12'#9'Export Lines'#9'F');
            	      ApplySelected;
                     DataSource := dm.dsrCardDetail;
                     dbnLinks.DataSource := dm.dsrCardDetail;
                     dbgCards.Hint := '';
                     // filter out records
                     dm.tblCardDetail.Filter := format('CARD_ID = %d',[dm.tblCardMaster.FieldByName('CARD_ID').AsInteger]);
                     dm.tblCardDetail.Filtered := True;
                   end;
                end;
            end;    
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dbgCardsDblClick(Sender: TObject);
begin
     // cant do it if not in looking at CardMaster
     if rgpCards.ItemIndex <> 0 then exit;
     SetCardID( False )
end;
{ **************************************************************************** }     
procedure TfrmMembersMAINT.SetCardID ( ToClear : Boolean); 
begin   
     try                                             
        with dm.TblMisc do begin
           open;
           edit;
           if ToClear then begin
              FieldByName('DEFAULT_CARDID').Clear;
              DefaultCardID    := -1;
              edtCardUsed.Text := '';
           end else begin
              FieldByName('DEFAULT_CARDID').AsInteger := dm.tblCardMaster.FieldByName('CARD_ID').AsInteger; 
              DefaultCardID    := dm.tblCardMaster.FieldByName('CARD_ID').AsInteger;
              edtCardUsed.Text := dm.tblCardMaster.FieldByName('CARD_NAME').AsString;
           end;
           post;
        end;
     finally
       dm.IBCommitRetain;
       dm.TblMisc.Close;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.edtCardUsedDblClick(Sender: TObject);
begin
     dbgCardsDblClick(Sender)
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.pnlCardMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer);
const hor = 'Horizontal = ';
      ver = 'Vertical = ';
// Card is 3 times bigger than normal ( H 55 , W 85 )
// Calculate Point POsition * 12
// so just * by 4
begin
     pnlCard.Hint := hor + IntToStr(x * 4) + '  ' + ver + intToStr(y * 4);
     lblPosition.Left := x;
     lblPosition.Top  := y;
     Application.ProcessMessages;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.btnCalculateClick(Sender: TObject);
var Shift: TShiftState;
begin
     if (StrToInt(edtHorizontal.Text) < 0) or 
        (StrToInt(edtHorizontal.Text) < 0) then begin
        sm( 'No negative numbers' );
        exit;
     end;   
     if (StrToInt(edtHorizontal.Text) Div 4) > pnlCard.Width then begin
        sm( 'To Higher Number');
        edtHorizontal.SetFocus;
        exit;
     end; 
     if (StrToInt(edtVertical.Text) Div 4) > pnlCard.Height then begin
        sm( 'To Higher Number');
        edtVertical.SetFocus;
        exit;
     end; 
     pnlCardMouseMove(Sender, Shift,StrToInt(edtHorizontal.Text) Div 4, StrToInt(edtVertical.Text ) Div 4);
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.edtHorizontalKeyPress(Sender: TObject;  var Key: Char);
begin
     if key = #13 then begin
        SelectNext(ActiveControl as TWinControl,True,True);
        key := #0;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.edtVerticalKeyPress(Sender: TObject;  var Key: Char);
begin
     if key = #13 then  btnCalculateClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dbgCardsCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;   ABrush: TBrush);
begin
      if (Field.FieldName = 'CARD_NAME') then ABrush.Color := Global.fcGridEdit;    
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.FormResize(Sender: TObject);
var x,y : Smallint;
begin
     ResizeToolBar( Self ,Self.WindowState,OneButtonWidth,FpnlWidth,pnlSpace);
     x := tsSetupPoints.Width div 2;
     y := tsSetupPoints.Height div 2;
     //ArrangePanel(pnlPoints,x,y);    
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dbgManualCardCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
begin
     if (Field.FieldName = 'MEMBER') or (Field.FieldName = 'NUMBER_CARDS') then
        ABrush.Color := Global.fcGridEdit; 
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dbgPointsCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;  ABrush: TBrush);
begin
     if (Field.FieldName = 'KNOWN_AS') or
        (Field.FieldName = 'FULL_NAME') or 
        (Field.FieldName = 'PAY') then  
        ABrush.Color := Global.fcGridEdit;    
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.btnCardsClick(Sender: TObject);
begin
      dblcMember.Clear;
      PopulatePoints(True);
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.btnCancelClick(Sender: TObject);
begin
     if MessageDlg('Cancel and Clear Changes?',mtConfirmation,[mbYes,mbNo],0) = mrNo then exit;
     if dm.IBRunOnce.InTransaction then begin
        dm.IBRunOnce.Rollback;
        dm.IBRunOnce.StartTransaction;
        ResetPointsGrid;
        dblcMember.Setfocus;
        dblcMember.SelectAll;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.ResetPointsGrid;
begin
     dm.ibdsPoints.SelectSQL.TEXT := SQL_POINTS + ' MEMBER = -1';
     dm.ibdsPoints.Open;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dbgPointsDblClick(Sender: TObject);
begin
     if not FFunctionCard then exit;
     with dm.ibdsPoints do begin
        edit;
        FieldByName('PAY').AsFloat := FieldByName('POINTS_AVAILABLE').AsFloat
            - (FieldByName('POINTS_AVAILABLE').AsFloat * 2);
        post;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dbgPointsKeyPress(Sender: TObject;  var Key: Char);
begin
     // turns Enter to tab
     if (key = #09) and not FFunctionCard  then
         btnSaveClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.btnExportCrdsClick(Sender: TObject);
var ExportReceipt : TextFile;
    s,tempFieldName : shortstring;
    AddressList : TStringList;
    i : smallint;
const SQL1 = 'SELECT MEMBER,EXPIRES,FIRST_NAME,SURNAME,ADDRESS,ADDRESS1,DISTRICT,CITY,' + 
             'COUNTRY FROM MEMBERS WHERE MEMBER = %d';
begin
       if dm.tblCardMember.State in [dsEdit,dsInsert] then dm.tblCardMember.Post;
       if dm.SaveDialog.Execute then begin
        try
           AssignFile(ExportReceipt,dm.SaveDialog.FileName);
           Rewrite( ExportReceipt );
           AddressList := TStringList.Create;
           s := 'Receipt,Date,Member#,FName,LName,Address1,Address2,Address3,Address4,Address5,' +
                'Paid,PaidBy,SendReceipt,GSTContent';
           Writeln(ExportReceipt,s);
           with dm.qryGeneral do begin
              dm.tblCardMember.DisableControls;
              dm.tblCardMember.First;
              while not dm.tblCardMember.EOF do begin
                 if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[dm.tblCardMember.FieldByName('MEMBER').AsInteger])) then begin
                    s := '';
                    s := ',';
                    s := s + GetShortMonthDate(FieldByName('EXPIRES').AsDAteTime) + ',';
                    s := s + FieldByName('MEMBER').AsString + ',';  
                    s := s + FieldByName('FIRST_NAME').AsString + ',';  
                    s := s + FieldByName('SURNAME').AsString + ',';  
                    AddressList.Clear;
                    if not FieldByName('ADDRESS').IsNull then AddressList.Add(FieldByName('ADDRESS').AsString);
                    if not FieldByName('ADDRESS1').IsNull then AddressList.Add(FieldByName('ADDRESS1').AsString);
                    if not FieldByName('DISTRICT').IsNull then AddressList.Add(FieldByName('DISTRICT').asString);
                    if not FieldByName('CITY').IsNull then AddressList.Add(FieldByName('CITY').asString);
                    if not FieldByName('COUNTRY').IsNull then AddressList.Add(FieldByName('COUNTRY').asString);
                    for i := 0 to 4 do begin
                       if i <= AddressList.Count -1 then                                    
                          s := s + AddressList[i] + ',' 
                       else
                          s := s + ','
                    end;       
                    s := s + ',';
                    s := s + ',';
                    s := s + 'Yes' + ',';
                    s := s + ',';
                    Writeln(ExportReceipt,s);
                 end;
                 dm.tblCardMember.next;
              end;
           end;
           MDlgI(CreatedFileMsg + dm.SaveDialog.Filename);
        finally
           dm.tblCardMember.EnableControls;
           CloseFile( ExportReceipt );
           AddressList.Free;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.btnAllMembersClick(Sender: TObject);
var CardMembers : TCardMembers;
begin
     if (Sender as TButton).Name = btnPaidMembers.Name then begin
        CardMembers.AllFinancialMembers(0,'');
     end else if (Sender as TButton).Name = btnAllMembers.Name then begin  
        CardMembers.AllFinancialMembers(1,'');
     end else begin   // Groups
        if frmMembersREPORT = nil then
        if sfPreCheckLogOn(TfrmMembersREPORT)
           then frmMembersREPORT := TfrmMembersREPORT.Create(self)
        else exit;
        frmMembersREPORT.GetFieldList( 'Select Groups', 'Groups',dm.tblGroups );
        CardMembers.AllFinancialMembers(2,frmMembersREPORT.FExtFields);
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.btnStartPhotoClick(Sender: TObject);
var MaxId : LongInt;
begin
     if edtMemberTime.Text = '0' then exit;
     if dbdtEndTime.Time < dbdtStartTime.time then begin
        sm('The End Time cannot be before the Start Time');
        exit;
     end;
     if not SetupLuckyMember( False, MaxId ) then begin
        NotBusy;
        exit;
     end;
     frmMembersPHOTO := TfrmMembersPHOTO.Create(nil);
     try
        with frmMembersPHOTO do begin
           MaxMember  := MaxID;
           StartTime  := dbdtStartTime.Time;
           if edtPhotoSize.Text = '0' then
              IncreasePhotoSize := 0
           else
              IncreasePhotoSize  := StrToInt(edtPhotoSize.Text) / 100;
           MemberTime := StrToInt(edtMemberTime.Text);
           EndTime    := dbdtEndTime.Time;
           Showmodal;
        end;
     finally
        frmMembersPHOTO.Release;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dbgManualCardTopRowChanged(Sender: TObject);
const SQL1 =  'Select FULL_NAME,EXPIRES from MEMBERS where MEMBER = %d';
begin
     edtFullName.Clear;
     dbdtExpires.Clear;
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,Format(SQL1,[dm.tblCardMember.FieldByName('MEMBER').AsInteger])) then begin
        edtFullName.Text   := dm.qryGeneral.FieldByName('FULL_NAME').AsString;
        dbdtExpires.Date   :=   dm.qryGeneral.FieldByName('EXPIRES').AsDateTime;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.menuLogoffBonusPointsClick(Sender: TObject);
begin
     dbgPoints.ReadOnly := True;
     lblPoints.Visible := False;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.menuLogonBonusPointsClick(Sender: TObject);
begin
     frmPassword := TfrmPassword.Create(nil);
     try
        if frmPassword.ShowModal = mrOk then begin
           dbgPoints.ReadOnly := False;
           lblPoints.Visible := True;
        end;   
     finally
        frmPassword.free;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersMAINT.dbgPointsAllocationCalcCellColors(
  Sender: TObject; Field: TField; State: TGridDrawState;
  Highlight: Boolean; AFont: TFont; ABrush: TBrush);
begin
    if (Field.FieldName = 'POINTS') or
        (Field.FieldName = 'DONATION_POINTS') then
        ABrush.Color := Global.fcGridEdit;
end;
{ ****************************************************************************}
procedure TfrmMembersMAINT.btnStartPointsAllocationClick(Sender: TObject);
begin
     with dm.ibdsPointsAllocation do begin
        if EOF then exit;
        if not ifMsgDlg('Continue with Points Allocation') then exit;
        dm.IBCommitRetain;
        try
           screen.Cursor := crHourGlass;
           DisableControls;
           First;
           try
              while not EOF do begin
                 if FieldByName('DONATION_MEMBER').AsInteger = 0 then begin
                    edit;
                    FieldByName('UPDATED').AsString := 'Y';
                    post;
                    next;
                    continue;
                 end;   
                 dm.ibdsPointsAlloc.Close;
                 dm.ibdsPointsAlloc.ParamByName('MEMBER').AsInteger := FieldByName('DONATION_MEMBER').AsInteger;
                 dm.ibdsPointsAlloc.Open;
                 if dm.ibdsPointsAlloc.Eof then begin
                    showmessage( format('Member %d is no longer available',[FieldByName('DONATION_MEMBER').AsInteger]));
                 end else begin
                    dm.ibdsPointsAlloc.Edit;
                    dm.ibdsPointsAlloc.FieldByName('POINTS_AVAILABLE').AsFloat := dm.ibdsPointsAlloc.FieldByName('POINTS_AVAILABLE').AsFloat +
                        FieldByName('DONATION_POINTS').AsFloat;
                    dm.ibdsPointsAlloc.FieldByName('BONUS_CREDIT').AsFloat := dm.ibdsPointsAlloc.FieldByName('BONUS_CREDIT').AsFloat +
                        FieldByName('DONATION_POINTS').AsFloat;
                    dm.ibdsPointsAlloc.FieldByName('TOTAL_EARNT').AsFloat := dm.ibdsPointsAlloc.FieldByName('TOTAL_EARNT').AsFloat +
                        FieldByName('DONATION_POINTS').AsFloat;
                    dm.ibdsPointsAlloc.Post;
                    edit;
                    FieldByName('UPDATED').AsString := 'Y';
                    post;
                 end;   
                 next;
              end;
              dm.IBCommitRetain;
              showmessage('Points Allocation Completed');
          except
             showmessage('The Points Allocation did not work');
             dm.IBRollBackRetain;
          end;    
        finally
           dm.ibdsPointsAlloc.Close;
           EnableControls;
           Close;
           Open;
           screen.Cursor := crDefault;
        end;   
     end;
end;
{ ****************************************************************************}
procedure TfrmMembersMAINT.edtLuckyDrawMsgEnter(Sender: TObject);
begin
     dbnLinks.DataSource := dm.dsrTblMisc;
end;
{ ****************************************************************************}
procedure TfrmMembersMAINT.wwDBGrid1Enter(Sender: TObject);
begin
     dbnLinks.DataSource := dm.dsrKiosk;
end;
{ ****************************************************************************}
procedure TfrmMembersMAINT.edtKioskDrawsEnter(Sender: TObject);
begin
     dbnLinks.DataSource := dm.dsrTblMisc;
end;
{ ****************************************************************************}
procedure TfrmMembersMAINT.dbgKioskDrawEnter(Sender: TObject);
begin
     dbnLinks.DataSource := dm.dsrKioskDraws;
end;
{ ****************************************************************************}
procedure TfrmMembersMAINT.dbgPrefHeadingEnter(Sender: TObject);
begin
   // 3 grids look here
   dbnLinks.DataSource := (Sender as TwwDBgrid).Datasource;
end;
{ ****************************************************************************}
procedure TfrmMembersMAINT.Button2Click(Sender: TObject);
var MemberPreferences : TMemberPreferences;
begin
     try
        MemberPreferences := TMemberPreferences.Create;
       // MemberPreferences.MemberPreferenceLoad(scrollBox,asReport);
     finally
        MemberPreferences.Free;
     end;  
end;
{ ****************************************************************************}
procedure TfrmMembersMAINT.CheckPrefFields;
type  TCharSet = set of Char;
const Digits   : TCharSet = ['A'..'Z','a'..'z','0'..'9'];
var aFieldList : TStringList;
    i : smallint;
    Index : Integer;   
    AlterTableStr,s : String;
    MakeAlterations : Boolean;
    NotifyEvent     :  TDataSetNotifyEvent;
label fromhere;    
begin
     // havnt done drop fieldname yet
     with dm.ibdsPrefDetail do begin
        // Turn off IBCommitRetain in AfterPost
        NotifyEvent := dm.ibdsPrefDetail.AfterPost;
        dm.ibdsPrefDetail.AfterPost := nil;
        try
           // looks at PrefHeading for MasterSource
           aFieldList := TStringList.Create;
           // get the fields from tblPrefMembers
           dm.tblPrefMembers.Open;
           for i := 0 to dm.tblPrefMembers.FieldCount - 1 do begin
               aFieldList.Add(dm.tblPrefMembers.Fields[i].FieldName);
           end;
           aFieldList.Sort;
           First;
           AlterTableStr := 'Alter table "PREF_MEMBERS" ';
           MakeAlterations := False;
           dm.ibdsPrefHeading.First;
           while not dm.ibdsPrefHeading.EOF do begin
              while not EOF do begin
                 // has no FIELD description made up
                 //sm( FieldByName('FIELD_DESC').AsString);
                 if FieldByName('FIELD_DESC').AsString = '' then begin
                    s := FieldByName('DETAIL_DESC').AsString;
                    for i := 1 to length(s) do begin
                       // space to underscore
                       if s[i] = ' ' then begin
                          s[i] := '_';
                          continue;
                       end;   
                       // illegal char to underscore
                       if not (s[i] in Digits) then begin
                          s[i] := '_';
                       end;   
                    end;
                    i := 1;
                    fromhere:;
                    if aFieldList.Find(s, Index) then begin
                       // try to make unique
                       s := s + '_' + IntToStr(i);
                       inc(i);
                       goto fromhere;
                    end;
                    aFieldList.Add(s);
                    aFieldList.Sort;
                    edit;
                    FieldByName('FIELD_DESC').AsString := s;
                    post;
                    // add FIELD_DESC Integer,
                    MakeAlterations := True;
                    if FieldByName('DETAIL_TYPE').AsString = 'Y' then begin
                       AlterTableStr := AlterTableStr + ' add "' + s + '" char(3),'
                    end else if FieldByName('DETAIL_TYPE').AsString = 'C' then begin  
                       AlterTableStr := AlterTableStr + ' add "' + s + '" char(1),'
                    end else if FieldByName('DETAIL_TYPE').AsString = 'T' then begin  
                       AlterTableStr := AlterTableStr + ' add "' + s + '" varchar(40),'
                    end else if FieldByName('DETAIL_TYPE').AsString = 'N' then begin  
                       AlterTableStr := AlterTableStr + ' add "' + s + '" Integer,';
                    end;   
                 end;
                 next;
              end;
              dm.ibdsPrefHeading.Next;   
           end;
           // make changes to prefMembers - prefMembers is created with the DEFAULT user
           // in Splash form so it can be changed here
           if MakeAlterations then begin
              // change last comma to a ;
              System.Delete( AlterTableStr,LastDelimiter(',',AlterTableStr),1);
              AlterTableStr := AlterTableStr + ';';
              AddToClipBoard(AlterTableStr);
              dm.sqlExecquery(dm.qryGeneral,AlterTableStr);
           end;   
           try
              dm.IBCommitRetain;
           except
              dm.IBRollbackRetain;
              sm('Proc CheckPrefFields failed - cant create the FIELD NAMES'); 
           end;
        finally
           dm.ibdsPrefDetail.AfterPost := NotifyEvent;
           if assigned( aFieldList) then
              aFieldList.Free;
           dm.tblPrefMembers.Close;
        end;   
     end;
end;
{ ****************************************************************************}
procedure TfrmMembersMAINT.dbgPrefHeadingExit(Sender: TObject);
begin
     if dm.ibdsPrefDetail.State in [dsEdit,dsInsert] then
        dm.ibdsPrefDetail.Post;
     if dm.ibdsPrefSubHeading.State in [dsEdit,dsInsert] then
        dm.ibdsPrefSubHeading.Post;
     if dm.ibdsPrefHeading.State in [dsEdit,dsInsert] then
        dm.ibdsPrefHeading.Post;
end;  
{ ****************************************************************************}      
procedure TfrmMembersMAINT.btnKioskClick(Sender: TObject);
begin
     try
        frmKiosk := TfrmKiosk.Create(nil);
        frmKiosk.ShowModal;
     finally
        frmKiosk.Free;
     end;
end;
{ ****************************************************************************}
end.

