unit MembersSYS;
{
 Backups - NT to NT is OK
         - NT to 95/98 is not OK unless 95/98 drive is mapped onto NT
         - 95/98 to 95/95 would be maybe
         - 
 
}

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm,  ComCtrls, ImgList, wwSpeedButton, wwDBNavigator,
  wwclearpanel, ExtCtrls, wwrcdpnl, StdCtrls,DB, Grids, Wwdbigrd, Wwdbgrid,
  wwDialog, wwrcdvw, Mask, wwdbedit, Wwdotdot, Wwdbcomb, Buttons, ToolEdit,FileCtrl,
  wwdbdatetimepicker, IBServices, DualList, lmdcctrl, lmdctrl, lmdeditb,
  lmdeditc, lmdbredt, lmdcompo, lmdclass, lmddlgS, DBCtrls,IBTable,
  lmdnonvS, IBEvents, CheckLst, lmdgroup,IBSQL, Menus, ToolWin,
  IBCustomDataSet,IBQuery, ovcbase, ovcdlg, ovccaldg;

  { **************************************************************************** }
// adding, deleting and changing 
type
   TUserTypes = (utAdd,utModify,utDelete);
{ **************************************************************************** }
type
  TfrmMembersSYS = class(TSMStdForm)
    PageControl: TPageControl;
    tsSite: TTabSheet;
    tsUsersSecurity: TTabSheet;
    tsUserLog: TTabSheet;
    dbgLogs: TwwDBGrid;
    dbgUserSecurity: TwwDBGrid;
    dbeName: TwwDBEdit;
    Splitter1: TSplitter;
    Splitter2: TSplitter;
    tsBackup: TTabSheet;
    tsDaily: TTabSheet;
    Panel1: TPanel;
    tsOther: TTabSheet;
    dbgOther: TwwDBGrid;
    rgpOther: TLMDRadioGroup;
    rgpLog: TLMDRadioGroup;
    cbPrinters: TwwDBComboBox;
    popMenu: TPopupMenu;
    Applicationtoall: TMenuItem;
    Applicationto1: TMenuItem;
    Helptoall: TMenuItem;
    HelpTo1: TMenuItem;
    ClearAllUsers1: TMenuItem;
    tsInterface: TTabSheet;
    btnClose: TToolButton;
    menuExit: TMenuItem;
    dbnNavRefresh: TwwDBNavigator;
    dbnRefresh: TwwNavButton;
    Panel10: TPanel;
    GroupBox7: TGroupBox;
    Panel11: TPanel;
    Panel9: TPanel;
    GroupBox5: TGroupBox;
    cbxAllUsers: TCheckBox;
    dbcbProcess: TwwDBComboBox;
    Panel6: TPanel;
    GroupBox6: TGroupBox;
    LogListBox: TLMDRadioGroup;
    gbxInterface: TGroupBox;
    Label4: TLabel;
    GroupBox9: TGroupBox;
    Label3: TLabel;
    btnImport: TButton;
    btnBackup: TToolButton;
    menuOptions: TMenuItem;
    menuManualBackup: TMenuItem;
    btnChangeUser: TToolButton;
    btnAddUser: TToolButton;
    btnDeleteUser: TToolButton;
    break1: TMenuItem;
    menuAddUser: TMenuItem;
    menuChangeUser: TMenuItem;
    menuDeleteUser: TMenuItem;
    Panel8: TPanel;
    GroupBox2: TGroupBox;
    ListBox: TLMDRadioGroup;
    pnlPassword: TPanel;
    Label1: TLabel;
    Label2: TLabel;
    Label7: TLabel;
    edtUserName: TEdit;
    edtConfirmPassword: TEdit;
    edtPassword: TEdit;
    btnOK: TButton;
    btnCancel: TButton;
    GroupBox1: TGroupBox;
    BackupMemo: TMemo;
    GroupBox3: TGroupBox;
    Label15: TLabel;
    cbxManBackupEnabled: TwwDBComboBox;
    dirManBackup: TDirectoryEdit;
    cbxManBackupType: TwwDBComboBox;
    GroupBox4: TGroupBox;
    Label16: TLabel;
    cbxAutoBackupEnabled: TwwDBComboBox;
    cbxAutoBackupType: TwwDBComboBox;
    dtpAutoBackupTime: TwwDBDateTimePicker;
    dtpAutoBackupDate: TwwDBDateTimePicker;
    dirAutoBackup: TDirectoryEdit;
    cbxAutoBackupFileNames: TwwDBComboBox;
    dbeAutoBackupKeep: TwwDBEdit;
    btnAutoBackup: TButton;
    GroupBox8: TGroupBox;
    Label17: TLabel;
    dbcbCaps: TwwDBComboBox;
    dbcbCard: TwwDBComboBox;
    dbcbReceipt: TwwDBComboBox;
    Panel2: TPanel;
    dbgInterface: TwwDBGrid;
    Panel3: TPanel;
    GroupBox10: TGroupBox;
    rgpImport: TLMDRadioGroup;
    pnlSpace: TPanel;
    dbcbNameFields: TwwDBComboBox;
    FieldsDialog: TDualListDialog;
    dbcbServices: TwwDBComboBox;
    dbcbConfirmPayment: TwwDBComboBox;
    dbcbCardEncode: TwwDBComboBox;
    Label5: TLabel;
    Label6: TLabel;
    Label8: TLabel;
    Label14: TLabel;
    Label18: TLabel;
    Label19: TLabel;
    Label20: TLabel;
    Label21: TLabel;
    ibdsMembers: TIBDataSet;
    cbEmulation: TwwDBComboBox;
    cbCommPort: TwwDBComboBox;
    dbcbFirstPayment: TwwDBComboBox;
    ibdsMembersMEMBER: TIntegerField;
    ibdsMembersFIRST_NAME: TIBStringField;
    ibdsMembersSURNAME: TIBStringField;
    ibdsMembersFULL_NAME: TIBStringField;
    ibdsMembersINITIALS: TIBStringField;
    ibdsMembersTITLE: TIBStringField;
    IBDataSet: TIBDataSet;
    IBDataSetAMOUNT: TIBBCDField;
    IBDataSetAMOUNT_PAID: TIBBCDField;
    IBDataSetEDIT_DATE: TDateTimeField;
    IBDataSetINVOICED: TIBStringField;
    IBDataSetMEMBER: TIntegerField;
    IBDataSetPAID: TIBStringField;
    IBDataSetPAY: TIBBCDField;
    IBDataSetPRINT_CARD: TIBStringField;
    IBDataSetPRINT_RECEIPT: TIBStringField;
    IBDataSetRECEIPT: TIntegerField;
    IBDataSetSUBSCRIPTION: TIntegerField;
    btnSave: TButton;
    pcSite: TPageControl;
    tsDefaults: TTabSheet;
    tsRegistry: TTabSheet;
    tsSites: TTabSheet;
    rvpSite: TwwRecordViewPanel;
    pnlRegistry: TPanel;
    rvpMisc: TwwRecordViewPanel;
    tsMenuMate: TTabSheet;
    rvpMenuMate: TwwRecordViewPanel;
    btnSwipeCard: TButton;
    N1: TMenuItem;
    MenuMatePointsLog1: TMenuItem;
    dbcbMMInstalled: TwwDBComboBox;
    dbcdMMTrigger: TwwDBComboBox;
    dbcbOnInsert: TwwDBComboBox;
    Label23: TLabel;
    Label24: TLabel;
    Label25: TLabel;
    Label26: TLabel;
    tsKiosk: TTabSheet;
    rvpKiosk: TwwRecordViewPanel;
    dbcbShowDice: TwwDBComboBox;
    dbcbPointsASDollars: TwwDBComboBox;
    dbcbCheckExpires: TwwDBComboBox;
    Label22: TLabel;
    Label27: TLabel;
    Label28: TLabel;
    Label29: TLabel;
    Label30: TLabel;
    Label31: TLabel;
    GroupBox11: TGroupBox;
    Label10: TLabel;
    edtDataBaseName: TFilenameEdit;
    Label9: TLabel;
    edtServerName: TEdit;
    GroupBox12: TGroupBox;
    Label13: TLabel;
    Label32: TLabel;
    edtMenuMateDataBaseName: TFilenameEdit;
    edtMenuMateServerName: TEdit;
    Label11: TLabel;
    cbxUserLogon: TComboBox;
    Label12: TLabel;
    edtLocalUserName: TEdit;
    Label33: TLabel;
    tsMenuMatePoints: TTabSheet;
    dbgMenuMatePoints: TwwDBGrid;
    GroupBox13: TGroupBox;
    btnMMPoints: TButton;
    CalendarDialog: TOvcCalendarDialog;
    dbcdMMDiscounts: TwwDBComboBox;
    Label34: TLabel;
    Label35: TLabel;
    procedure FormActivate(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
    procedure PageControlChange(Sender: TObject);
    procedure dbgUserSecurityExit(Sender: TObject);
    procedure SetProcessType;
    procedure dbcbProcessChange(Sender: TObject);
    procedure ProcessChange( theQuery : TIBQuery );
    procedure cbxAllUsersClick(Sender: TObject);
    procedure dirManBackupChange(Sender: TObject);
    procedure PopulateListBox;
    procedure btnCancelClick(Sender: TObject);
    procedure btnOKClick(Sender: TObject);
    function LoggedOn( NewUserName : string ) : Boolean ;
    function ChangeUser( UserTypes : TUserTypes ): Boolean;
    procedure edtUserNameKeyPress(Sender: TObject; var Key: Char);
    procedure WriteReg;
    procedure RegSetNames;
    procedure edtServerNameChange(Sender: TObject);
    procedure edtServerNameKeyPress(Sender: TObject; var Key: Char);
    procedure SetManualBackup;
    procedure SetAutoBackup;
    procedure cbxManBackupEnabledCloseUp(Sender: TwwDBComboBox; Select: Boolean);
    procedure cbxManBackupEnabledKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure cbxAutoBackupEnabledCloseUp(Sender: TwwDBComboBox; Select: Boolean);
    procedure cbxAutoBackupEnabledKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure cbxAutoBackupFileNamesCloseUp(Sender: TwwDBComboBox; Select: Boolean);
    procedure dtpAutoBackupTimeExit(Sender: TObject);
    procedure dbgLogsDblClick(Sender: TObject);
    procedure AdjustDate;
    procedure dtpAutoBackupTimeKeyPress(Sender: TObject; var Key: Char);
    procedure btnImportClick(Sender: TObject);
    procedure ImportArevFiles;
    procedure ExportArevFiles;
    procedure LogListBoxChange(Sender: TObject; ButtonIndex: Integer);
    procedure ListBoxChange(Sender: TObject; ButtonIndex: Integer);
    procedure dbgLogsCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure cbxUserLogonChange(Sender: TObject);
    procedure btnSaveClick(Sender: TObject);
    function  CheckImportFile( WriteToFile : Boolean ) : Boolean;
    procedure  LoadIntoImportAREV;
    function  ImportAREVToMembers : Boolean; 
    function  CheckTotalLines : Boolean;
    procedure UpdateMemberPoints;
    function GetLatestFee(const tblSubs,SubsName : shortstring ; SubsInt : Integer) : Extended;
    function UpdateSubsPaid : boolean;
    function GetNewAmounts : Boolean;
    function UpdatePayments: Boolean;
    function UpdateJoinFee : Boolean;
    procedure UpdateMemberCredit;
    procedure UpdateLuckyMember; 
    function UpdateSubsByPayment : Boolean;
   // procedure rgpSiteChange(Sender: TObject; ButtonIndex: Integer);
    procedure cbPrintersCloseUp(Sender: TwwDBComboBox; Select: Boolean);
    procedure dbgOtherExit(Sender: TObject);
    procedure rgpOtherChange(Sender: TObject; ButtonIndex: Integer);
    procedure ApplicationtoallClick(Sender: TObject);
    procedure ClearAllUsers1Click(Sender: TObject);
    procedure Applicationto1Click(Sender: TObject);
    procedure HelpTo1Click(Sender: TObject);
    procedure HelptoallClick(Sender: TObject);
    procedure MakeSendDirectory;
    procedure ShowMsg;
    procedure dbgUserSecurityCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont;  ABrush: TBrush);
    procedure dbgOtherCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure SetButtons( OnOrOff : Boolean);
    procedure btnExportClick(Sender: TObject);
    procedure ExportToFile( ExportDateStr : ShortString );
    function ConvertAmounts : Boolean;
    procedure tsInterfaceExit(Sender: TObject);
    procedure dbgInterfaceCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure dbgInterfaceDblClick(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure menuExitClick(Sender: TObject);
    procedure dbnRefreshClick(Sender: TObject);
    procedure menuManualBackupClick(Sender: TObject);
    procedure menuAddUserClick(Sender: TObject);
    procedure menuChangeUserClick(Sender: TObject);
    procedure menuDeleteUserClick(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure LoadPointsFromFile;
    //function DayMonthYearStr( str : string ) : String;
    //function DayMonthYearFunny( str : string ) : String;
    procedure dbcbNameFieldsClick(Sender: TObject);
    //function GetCommaDelimitedField(FieldNum : shortint ; s : String) : String ;
    procedure LoadGroups;
    procedure SubsUpDate;
    procedure ProcessSubs( i : Integer );
    procedure NoGroups;
    procedure BulkDeleteMembers;
    procedure AddDOB;
    procedure AddJoinFee;
    function UpdateMultipleLinks : Smallint;
    procedure cbEmulationCloseUp(Sender: TwwDBComboBox; Select: Boolean);
    procedure cbCommPortCloseUp(Sender: TwwDBComboBox; Select: Boolean);
    procedure Capitalize;
    procedure DeleteWinFile;
    procedure rgpLogChange(Sender: TObject; ButtonIndex: Integer);
    procedure btnPointsAuditClick(Sender: TObject);
    procedure SaveSite;
    procedure SaveMisc;
    procedure SaveMenuMate;
    procedure SaveKiosk;
    procedure Oamaru;
    function DayMonthYearFunny( str : string ) : String;
    procedure btnMMPointsClick(Sender: TObject);
    function GetPointsFromString(s : String ; var Member : integer; var PointsAvailable,TotalRedeemed ,TotalEarnt,PrePaidCredit,BonusCredit,EarntCredit : extended): Boolean;
    procedure SetTriggerOn;
    procedure pcSiteChange(Sender: TObject);
    procedure btnSwipeCardClick(Sender: TObject);
    procedure DOB;
    procedure GroupsLoadFromSubs;
    //procedure MenuMatePointsLog1Click(Sender: TObject);
    procedure UpdateExpires;
    procedure dbcdMMDiscountsEnter(Sender: TObject);
    procedure dbcdMMTriggerEnter(Sender: TObject);
    
    //procedure updatedates;
  private
    { Private declarations }
    FUserName   : string;
    FToComputer : String;
    FInstallDir : shortstring;
    FUserCode   : Integer;
    FAddUser,FSuccess : Boolean;
    FPassword : string;
    FFlag     : char;
    FPoints,FPointsDollar   : Extended;
    FDescription : String[25];
    FName,FTenderName  : String[25];   
    FMember,FReceiptNumber,FPaidByMember : LongInt;
    FAmountPaid, FAmount,FTender : Extended;
    FHoldPoints, FHoldTender : Extended;
    FHoldSPNT, FHoldERNT : Extended;
    FSubPaid,FSubs  : ShortInt;
    //FPointsAvailable,FTotalRedeemed,FTotalEarnt : Extended;
    FGoodDate,FExpires : TDateTime;
    MemberList : TStringList;
    ImportList : TStringList;
    FpnlWidth : SmallInt;
    FNewMember : String[1];
  public
    { Public declarations }
    procedure wmSysCommand(var Message : TWMSysCommand); message WM_SYSCOMMAND;
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;

var
  frmMembersSYS: TfrmMembersSYS;
const
   
   SC_POINTS   = $00FF;  // Windows SysCommands start at $F000 - Ours must be lessCopyFilesMsg = 'Copying files';
   SC_GROUPS   = $00F0;
   SC_NO_GROUPS   = $00FE;
   SC_SUBSCRIPTION_UPGRADE = $00F1;
   SC_BULK_DELETE_MEMBERS = $00F2;
   SC_ADD_DATES = $00F3;
   SC_ADD_JOINFEE = $00FC;
   SC_CAPITALIZE = $00F4;
   SC_DELETE_FILE = $00FA;
   SC_OAMARU = $00FB;
   SC_TRIGGER = $00FD;
   SC_UPDATE_EXPIRES = $00F5;

   NoHelpMsg = 'No Help files to Send';
   dbgHint = 'Cannot be modified if not logging on';
   UsrSecView = 'USER_CODE = %d';
   POSImport = 'Morning Import';
   POSExport = 'Nightly Export';
   CantSentToSelfMsg = 'Cannot send this file to yourself';
   FExeName = '\Main.exe';
   FHelpFileName = '\*.doc';
   BackSlash = '\';
   SendSuccessMsg = 'Send successful to %s';
   SendUnSuccessMsg = 'Send unsuccessful to %s, check Share/Share Name on %s PC' + #13#10 +
                      'or that they are not using the Exe or Help File';
   
   
implementation

{$R *.DFM}
uses
    Global,    // StockMaster
    Registry,
    Multi,
    MembersCLASSES,
    MembersLookup,
    dmMembers, rPointsAudit;  // StockMaster
var
  PrefixImportTypes : TPrefixImportTypes;
  SubPayTypes       : TSubPayTypes;
  PayTypes          : TPayTypes;
  MemberPayments : TMemberPayments;
  BeforeEditPointsCredit : TBeforeEditPointsCredit;
  
  
{ **************************************************************************** }
class function TfrmMembersSYS.ScreenCode : Integer;
begin Result := Global.scMembersSystem; end;
{ **************************************************************************** }
class function TfrmMembersSYS.ScreenName : String;
begin Result := 'Members System'; end;
{ **************************************************************************** }
class function TfrmMembersSYS.ScreenDesc : String;
begin Result := '  Members System Screen'; end;
{ **************************************************************************** }
procedure TfrmMembersSYS.wmSysCommand(var Message : TWMSysCommand);
//Process the WM_SYSCOMMAND message for what we want and pass others back
begin
     case message.CmdType of
        SC_POINTS : LoadPointsFromFile;
        //SC_GROUPS : DOB;
       // SC_GROUPS : LoadGroups;
        SC_NO_GROUPS : GroupsLoadFromSubs;
       // SC_SUBSCRIPTION_UPGRADE : SubsUpDate;
       // SC_BULK_DELETE_MEMBERS : BulkDeleteMembers;
        SC_ADD_DATES : AddDOB;
        SC_ADD_JOINFEE : AddJoinFee;
       // SC_CAPITALIZE : Capitalize;
        SC_DELETE_FILE : DeleteWinFile;
        SC_UPDATE_EXPIRES : UpdateExpires;
       // SC_TRIGGER : SetTriggerOn;
       // SC_OAMARU : Oamaru;
     else
        //do the default for everything else
        inherited;
     end;
end; 
{ **************************************************************************** }
procedure TfrmMembersSYS.FormResize(Sender: TObject);
begin
     ResizeToolBar( Self ,Self.WindowState,OneButtonWidth,FpnlWidth,pnlSpace);
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.FormActivate(Sender: TObject);
begin
     inherited;
     FUserName := Global.DefaultUserName; //set to - must be in DB 'DEFAULT'
     FUserCode := 0;
     rvpMisc.DataSource := dm.dsrTblMisc;
     rvpSite.DataSource := dm.dsrTblSite;
     rvpMenuMate.DataSource := dm.dsrMenuMate;
     
     // cant change if not logging on - all users have all access
     if not Global.regLogon then begin
        dbgUserSecurity.ReadOnly := True;
        dbgUserSecurity.Hint   := dbgHint;
     end;
     // give access back to ERS user
     if Global.UserName = MasterUserName then begin
        dbgUserSecurity.ReadOnly := False;
     end;
     if not dm.PreCheckLogOn(scSystemTabSite,stSystemTabSite ) then RemoveTab(tsSite);
     if not dm.PreCheckLogOn(scSystemTabOther,stSystemTabOther ) then
        RemoveTab(tsOther)
     else
        TblActiveOrRefresh ( dm.tblVersions );
     if not dm.PreCheckLogOn(scSystemTabUserSecurity,stSystemTabUserSecurity )then begin
        menuDeleteUser.Enabled := False;
        menuAddUser.Enabled := False;
        menuChangeUser.Enabled := False;
        SetButtons(False);
        RemoveTab(tsUsersSecurity)
     end;
     if not dm.PreCheckLogOn(scSystemTabLogs,stSystemTabLogs ) then
        RemoveTab(tsUserlog);
     // only the controller is allowed to see the changes
     if not dm.PreCheckLogOn(scSystemTabInterface,stSystemTabInterface ) or
       (ControllerPCName <> ReadComputerName) then RemoveTab(tsInterface);
     if not dm.PreCheckLogOn(scSystemTabBackup,stSystemTabBackup ) then begin
        btnBackup.Enabled := False;
        RemoveTab(tsBackup);
        menuManualBackup.Enabled := False;
     end else begin
        PageControl.ActivePage := tsBackup;
     end;
     // turn off if no backup is Set Up
     if not Global.ManBackupEnabled then btnBackup.Enabled := False;
     if not dm.PreCheckLogOn(scSystemTabDaily,stSystemTabDaily )   then RemoveTab(tsDaily);
     if not MenuMateInstalled then
        RemoveTab(tsMenuMatePoints)
     else
        tsMenuMatePoints.TabVisible := true;   
     
     if InTestMode then begin
        RemoveTab(tsBackup);
        RemoveTab(tsUsersSecurity);
        RemoveTab(tsDaily);
        menuAddUser.Enabled := False;
        btnAddUser.Enabled := False;
        menuChangeUser.Enabled := False;
        btnChangeUser.Enabled := False;
        menuDeleteUser.Enabled := False;
        btnDeleteUser.Enabled := False;
        menuManualBackup.Enabled := False;
        btnBackup.Enabled := False;
     end;
     PageControlChange(Sender);
     // Logs is the active page on entry
     //rgpLogClick(Sender);
     //LogListBoxChange(Sender,0);
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.FormCreate(Sender: TObject);
var  hSystemMenu : hMenu;
begin
     inherited;
     FpnlWidth := pnlSpace.Width;
     hSystemMenu := GetSystemMenu(handle, false);
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_STRING, SC_POINTS,'&Bulk Points' );
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_STRING, SC_GROUPS,'&Group/Expires Update' );
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_STRING, SC_SUBSCRIPTION_UPGRADE,'Subscriptions Update' );
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_STRING, SC_NO_GROUPS,'No Groups Update' );
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_STRING, SC_BULK_DELETE_MEMBERS,'Bulk Delete Members from Section' );
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_STRING, SC_ADD_DATES,'Add Dates to Members' );
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_STRING, SC_ADD_JOINFEE,'Add Joining Fee to Members' );
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_STRING, SC_CAPITALIZE,'Caps on First/Surname' );
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_STRING, SC_DELETE_FILE,'Delete Win File' );
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_STRING, SC_TRIGGER,'Turn On Trigger' );
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_STRING, SC_UPDATE_EXPIRES,'Update Member Expires Date' );
     // seem to screw up pnlSpace.Width on different screen resolutions
    { if Global.DiffRatio <> 0.00 then begin  // different resolution
        pnlSpace.Width := 10;
        ResizeToolBar( Self ,wsNormal,OneButtonWidth,FpnlWidth,pnlSpace);
        FpnlWidth := pnlSpace.Width; 
     end; }  
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.FormDeactivate(Sender: TObject);
begin
     dm.IBTransactionCommit; // Commit changes on deactivate
      // changes to tblMISc  - backups
     rvpMisc.DataSource := nil;
     rvpSite.DataSource := nil;
     FreeAndNil(ImportList);
     {with dm do begin
        TblCloseIfActiveFiltered(tblPrinters);
        TblCloseIfActiveFiltered(tblUserSecView);
        QryCloseIfActive(qryReport1);
        QryCloseIfActive(qryProcess);
        TblCloseIfActive(tblMisc);
        TblCloseIfActive(tblScreens);
        TblCloseIfActive(tblSite);
        TblCloseIfActive(tblVersions);

     end; }
      if dm.ibdsExport.Active then dm.ibdsExport.Active := False;
     inherited;
end;
{ **************************************************************************** }
{ **************************************************************************** }
procedure TfrmMembersSYS.SetTriggerOn;
begin
     // refiring on Points with the trigger
     if MenuMateTriggerInstalled then begin
        if dm.TriggerState(True) then
           showmessage('Trigger turned on successfully')
        else   
           showmessage('Did not work - call the HelpDesk 0800 657 300');
    end;       
end;
{ ****************************************************************************}
procedure TfrmMembersSYS.tsInterfaceExit(Sender: TObject);
begin
     dm.ExportTimer.Enabled := True;
     if dm.IBExport.InTransaction then begin
        try
           dm.IBExport.Commit;
        except
           dm.IBExport.Rollback;
           sm('Changes have not been accepted');
           dm.IBExport.StartTransAction;
           dm.ibdsExport.Refresh;
        end;
     end;      
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.PageControlChange(Sender: TObject);
var PCName : string[50];
    i : shortint;
    PrinterID : Integer;
    CanView : Boolean;
const SQL1 = 'INSERT into PRINTERS values ( %d,''%s'',null,null,null,''%s'',null )';
      SQL2 = 'SELECT max(PRINTER_ID) from PRINTERS';
begin
     stbStatus.panels[0].Text:= '';
     dbnNavRefresh.DataSource := nil;
     case PageControl.ActivePageIndex of
         0 : begin // Site Registry and Defaults
                TblActiveOrRefresh(dm.tblSite);
                TblActiveOrRefresh(dm.tblMisc);
                TblActiveOrRefresh(dm.tblMenuMate);
                TblActiveOrRefresh(dm.tblKioskSettings);
                pcSite.ActivePage := tsDefaults;
                pcSiteChange(Sender);
                RegSetNames; // registry settings
             end;
         1 : begin // Other
                stbStatus.panels[0].Text:= 'Browse';
                rgpOtherChange(Sender,rgpOther.ItemIndex);
                TblActiveOrRefresh(dm.tblScreens);
                TblActiveOrRefresh(dm.tblPrinters);
                // each PC must have the Printers setup manually
                PCName := ReadComputerName;
                TblSetFiltered(dm.tblPrinters,format('PC_NAME = ''%s''',[PCName]));
                if dm.tblPrinters.RecordCount = 0 then begin
                   // if app has just been installed - no generator on table
                   PrinterID := dm.LastNumberPlusOne(PrintersTableName,'PRINTER_ID');
                   for i := 0 to MaxPrinterTypes do begin
                      dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[PrinterID,PrinterTypes[i],PCName]));
                      inc(PrinterID);
                   end;
                   TblSetFiltered(dm.tblPrinters,format('PC_NAME = ''%s''',[PCName]));
                end;
                // quick fix for adding in CASH DRAWER - was only 3 Printer types
                if dm.tblPrinters.RecordCount = 3 then begin
                   PrinterID := dm.LastNumberPlusOne(PrintersTableName,'PRINTER_ID');
                   dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[PrinterID,PrinterTypes[3],PCName]));
                   TblSetFiltered(dm.tblPrinters,format('PC_NAME = ''%s''',[PCName]));
                end;
                GridLoadPrinters ( cbPrinters );
                dbgOther.SetFocus;
                dbgOther.SetActiveField('PRINTER_NAME');
             end;
         2 : begin // User Security
                dm.qryOpenCheckEOF(dm.qryReport1,'Select * from SCREENS');// needed for Screen Desc
                TblActiveOrRefresh(dm.tblUserSecView);
                dbnNavRefresh.DataSource := dbgUserSecurity.DataSource;
                dm.GetUserNames;
                PopulateListBox;
               ListBoxChange(Sender,0);
             end;
         3 : begin // Logs
                dm.GetUserNames;
                PopulateListBox;
                rgpLogChange(Sender,rgpLog.ItemIndex);
             end;
         4 : begin // Backup
                dbnNavRefresh.DataSource := nil;
                with dm.tblMisc do begin
                   TblActiveOrRefresh(dm.tblMisc);
                   if FieldByName('M_BACKUP_DIR').AsString <> '' then begin
                      dirManBackup.Text := FieldByName('M_BACKUP_DIR').AsString;
                      dirManBackup.InitialDir := FieldByName('M_BACKUP_DIR').AsString;
                   end;
                   if FieldByName('A_BACKUP_DIR').AsString <> '' then begin
                      dirAutoBackup.Text := FieldByName('A_BACKUP_DIR').AsString;
                      dirAutoBackup.InitialDir := FieldByName('A_BACKUP_DIR').AsString;
                   end;
                   cbxAutoBackupEnabled.Modified := False;
                   SetAutoBackup;
                   SetManualBackup;
                end;
             end;
         5 : begin // Daily Jobs
                dbnNavRefresh.DataSource := nil;
                if not assigned(ImportList) then
                   ImportList := TStringList.Create
                else
                   ImportList.Clear;
                ImportArevFiles;
             end;
         6 : begin
                // cannot get to here if not the Controller  (ControllerPCName)
               //dm.PExporting := True;
               CanView := True;
               gbxInterface.Visible := False;
               dbgInterface.Visible := False;
               while dm.PExporting do begin
                  // the Dlg takes care of a few seconds
                  if not IfMsgDlg('Busy Exporting - do you wish to wait for a few seconds?') then begin
                     CanView := False;
                     Break;
                  end;
               end;
               if CanView then begin
                  gbxInterface.Visible := True;
                  dbgInterface.Visible := True;
                  if not dm.IBExport.InTransaction then dm.IBExport.StartTransaction;
                  dm.ibdsExport.ParamByName('EXPORTED').AsString := 'Y';
                  if dm.ibdsExport.Active then
                     dm.ibdsExport.Refresh
                  else
                     dm.ibdsExport.Active := True;
                  dm.ibdsExport.Last;
                  dm.ExportTimer.Enabled := False;
                  dbnNavRefresh.DataSource := dbgInterface.DataSource;
               end else begin
                  sm('Cannot view the Interface data as it is Busy Exporting' + #13#10 +
                     'Click on the Interface Tab to try again');
               end;
             end;
         7 : begin
                QryActiveOrRefresh(dm.qryMenuMatePoints);
                dm.qryMenuMatePoints.First;
             end;
     end;  // case
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.menuDeleteUserClick(Sender: TObject);
const msg1 = 'User is Protected and cannot be deleted';
      msg2 = 'User cannot be deleted as it is in use';
      msg3 = 'Are you sure you want to delete User ';
      msg4 = 'cannot revoke Allusers from ';
      SQL1 = 'REVOKE ALLUSERS FROM %s';
      SQL2 = 'DELETE FROM USER_SEC WHERE USER_CODE = %d';
begin
     if Global.DisableServices then begin
        Sm(DisabledServiceMsg );
        exit;
     end;
     PageControl.ActivePage := tsUsersSecurity;
     PageControlChange(Sender);
     if FUserName = Global.DefaultUserName then begin
        showmessage(msg1);
        exit;
     end;
     if LoggedOn( FUserName ) then begin
        showmessage(msg2);
        exit;
     end;
     if IfMsgDlg( msg3 + FUserName ) then begin
         // Revoke user from the role
         if not dm.sqlExecQuery(dm.qryGeneral,Format(SQL1,[FUserName])) then begin
            sm(msg4 + FUserName);
            exit;
         end;
         if not ChangeUser( utDelete) then begin
           sm('Cannot delete user');                
           exit;
        end;
        // delete from tblUSer_sec
        dm.sqlExecQuery(dm.qryGeneral,Format(SQL2,[FUserCode]));
        SetProcessType;
        dm.LogProcess(dm.qryGeneral, 'User ' + IntToStr(FUserCode) + ' ' + FUserName  + ' Deleted');
        dm.GetUserNames;              
        FUserName := Global.UserName;
        FUserCode := Global.UserCode; 
        PopulateListBox;
        TblSetFiltered(dm.tblUserSecView,format(UsrSecView,[FUserCode]));
        dm.IBCommitRetain;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.menuAddUserClick(Sender: TObject);
begin
     if Global.DisableServices then begin
        Sm(DisabledServiceMsg );
        exit;
     end;
     PageControl.ActivePage := tsUsersSecurity;
     PageControlChange(Sender);
     pnlPassword.Visible := True;
     SetButtons(False);
     edtUsername.ReadOnly := False;
     edtUsername.Setfocus;
     edtUsername.Text := '';
     edtPassword.Text := '';
     edtConfirmPassword.Text := '';
     //edtUsername.Color := clWhite;
     FAddUser := true;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.SetButtons( OnOrOff : Boolean);
begin
     btnAddUser.Enabled    := OnOrOff;
     btnDeleteUser.Enabled := OnOrOff;
     btnChangeUser.Enabled := OnOrOff;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.menuChangeUserClick(Sender: TObject);
const msg1 = 'Cannot edit this User as it is Protected';
begin
     if Global.DisableServices then begin
        Sm(DisabledServiceMsg );
        exit;
     end;
     
     PageControl.ActivePage := tsUsersSecurity;
     PageControlChange(Sender);
     if FUserName = Global.DefaultUsername then begin
        showmessage(msg1);
        exit;
     end;
     pnlPassword.Visible := True;
     SetButtons(False);
     edtUsername.Text := FUserName;
     //edtUsername.Color := clSilver;
     edtPassword.Text := '';
     edtConfirmPassword.Text := '';
     edtUsername.ReadOnly := True;
     edtPassword.Setfocus;
     FAddUser := false;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dbgUserSecurityExit(Sender: TObject);
begin
    if dm.dsrUserSecView.State in [dsEdit,dsInsert] then begin
       with dm.tblUserSecView do begin
          SetProcessType;
          dm.LogProcess(dm.qryGeneral,'User Security - User ' + FieldByName('USER_CODE').AsString +
              ' - Screen Code ' + FieldByName('SCREEN_CODE').AsString + ' Changed to ' +
             FieldByName('HAS_ACCESS').AsString);
       end;
       dm.tblUserSecView.Post;
       dm.IBCommitRetain;
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.SaveSite;
begin
     if dm.dsrTblSite.State = dsEdit then begin
        SetProcessType;
        dm.LogProcess(dm.qryGeneral, 'Site Table Changed ');
        dm.TblSite.Post;
        // refresh local settings
        dm.GetSiteDetails;
        dm.IBCommitRetain;
        MDlgI('Site Settings Saved');
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.pcSiteChange(Sender: TObject);
//procedure TfrmMembersSYS.rgpSiteChange(Sender: TObject; ButtonIndex: Integer);
begin
     rvpMisc.DataSource  := dm.dsrtblMISC;
    // rvpSite.Visible     := False; //Site
    // rvpMisc.Visible     := False; // Misc
    // pnlRegistry.Visible := False; // Reg
     dbnNavRefresh.DataSource := nil;
     case pcSite.ActivePageIndex of
        2 : begin
              // rvpSite.Visible     := True; //Site
               dbnNavRefresh.DataSource := rvpSite.DataSource;
               btnSave.Caption := 'Save Site'; 
               btnSave.Hint    := 'Save Site Settings'; 
            end;   
        1 : begin
              // pnlRegistry.Visible := True; // Registry
               btnSave.Caption := 'Save Registry';
               btnSave.Hint    := 'Save Registry Settings';
            end;   
        0 : begin
              // rvpMisc.Visible     := True; // Misc
               dbnNavRefresh.DataSource := rvpMisc.DataSource;
               btnSave.Caption := 'Save Defaults';
               btnSave.Hint    := 'Save Defaults Settings';
            end;
        3 : begin
               dbnNavRefresh.DataSource := rvpMenuMate.DataSource;
               btnSave.Caption := 'Save MenuMate';
               btnSave.Hint    := 'Save MenuMate Settings';
            end;
        4 : begin
              // rvpMisc.Visible     := True; // Misc
               dbnNavRefresh.DataSource := rvpKiosk.DataSource;
               btnSave.Caption := 'Save Kiosk';
               btnSave.Hint    := 'Save Kiosk Settings';
            end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.SaveMisc;
var ErrorsFound : boolean; 
begin
     if dm.dsrTblMisc.State = dsEdit then begin
        ErrorsFound := false; 
        if dm.TblMisc.FieldByName('CHECK_GROUPS').AsString = 'Y' then begin
           //if dm.TblMisc.FieldByName('MIN_GROUP').AsInteger = 0 then begin
           //   showmessage('Min # of Groups must be greater than zero');
           //   ErrorsFound := true; 
          // end;          
           if (dm.TblMisc.FieldByName('MAX_GROUP').AsInteger < dm.TblMisc.FieldByName('MIN_GROUP').AsInteger) then begin
              showmessage('Max # of Groups must be greater Min # of Groups');
              ErrorsFound := true; 
           end;   
        end;
        if dm.TblMisc.FieldByName('CHECK_SUBS').AsString = 'Y' then begin
           //if dm.TblMisc.FieldByName('MIN_SUB').AsInteger = 0 then begin
           // showmessage('Min # of Subs must be greater than zero');
           //   ErrorsFound := true; 
           //end;          
           if (dm.TblMisc.FieldByName('MAX_SUB').AsInteger < dm.TblMisc.FieldByName('MIN_SUB').AsInteger) then begin
              showmessage('Max # of Subs must be greater Min # of Subs');
              ErrorsFound := true; 
           end;   
        end;
        
        if ErrorsFound then begin
           dm.TblMisc.Cancel;
           showmessage('Defaults Settings NOT Saved');
           exit;
        end;
        SetProcessType;
        dm.LogProcess(dm.qryGeneral, 'Defaults Table Changed ');
        dm.TblMisc.Post;
        // refresh local settings
        dm.GetSiteDetails;
        dm.IBCommitRetain;
        MDlgI('Defaults Settings Saved');
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.SaveMenuMate;
begin
     if dm.dsrMenuMate.State = dsEdit then begin
        SetProcessType;
        dm.LogProcess(dm.qryGeneral, 'MenuMate Table Changed ');
        dm.TblMenuMate.Post;
        dm.IBCommitRetain;
        dm.GetMenuMateDetails;
        MDlgI('MenuMate Settings Saved');
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.SaveKiosk;
begin
     if dm.dsrKioskSettings.State = dsEdit then begin
        SetProcessType;
        dm.LogProcess(dm.qryGeneral, 'Kiosk Table Changed ');
        dm.tblKioskSettings.Post;
        dm.IBCommitRetain;
        MDlgI('Kiosk Settings Saved');
     end;
end;
{ **************************************************************************** }

procedure TfrmMembersSYS.SetProcessType;
begin
     dm.ProcessType := ord(TProcessTypes(ptSystem));
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dbcbProcessChange(Sender: TObject);
begin
     case rgpLog.ItemIndex of
       0 : ProcessChange(dm.qryProcess);
       2 : ProcessChange(dm.qryPointsLog);
     end;  
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.ProcessChange( theQuery : TIBQuery);
const SQL1 = 'order by EDIT_DATE desc';
      SQL2 = 'where PROCESS = %d order by EDIT_DATE desc';
      SQL3 = 'where USER_CODE = %d order by EDIT_DATE desc';
      SQL4 = 'where USER_CODE = %d and PROCESS = %d order by EDIT_DATE desc';
      SQL5 = 'select * FROM %s ';
var PreSQl : ShortString;
// for both Points_log and Prcss_log - qry fields are the same
begin
     PreSQL := SQL5;
     if theQuery.Name = dm.qryProcess.Name then 
        PreSQL := format(PreSQL,['PRCSS_LOG'])
     else
        PreSQL := format(PreSQL,['POINTS_LOG']);
     with theQuery do begin
        DisableControls;
        Close;
        if cbxAllUsers.Checked then begin // All Users
           if dbcbProcess.ItemIndex = 0 then begin // All Processes
              SQL.Text := PreSQL + SQL1;
           end else begin                         // selected Process
              SQL.Text := format( PreSQL + SQL2,[dbcbProcess.ItemIndex]);
           end;
        end else begin  // Selected User
           if dbcbProcess.ItemIndex = 0 then begin  // All Processes
              SQL.Text := format(PreSQL + SQL3,[FUserCode]);
           end else begin                           // selected Process
              SQL.Text := format( PreSQL + SQL4,[FUserCode,dbcbProcess.ItemIndex]);
           end;
        end;                       
        EnableControls;
        Open;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dbgLogsCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
begin
     if not active then exit;
     if rgpLog.ItemIndex = 1 then begin
        if dm.qryBackupLog.FieldByName('SUCCESS').AsString = 'N' then
           AFont.Color := PayColor;
     //   else AFont.Color := clBlack;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dbgInterfaceCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
begin
     if dm.ibdsExport.FieldByName('EXPORTED').AsString = 'N' then AFont.Color := PayColor;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.LogListBoxChange(Sender: TObject;  ButtonIndex: Integer);
var s : string[40];
begin
     s := LogListBox.Items[LogListBox.ItemIndex];
     FUserName := copy(s,POS(' ',s)+ 1,length(s));
     FUserCode := arrayUserCodes[LogListBox.ItemIndex];
     case rgpLog.ItemIndex of 
        0 : ProcessChange(dm.qryProcess);
        2 : ProcessChange(dm.qryPointsLog);
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.ListBoxChange(Sender: TObject; ButtonIndex: Integer);
var s : string[40];
begin
     s := ListBox.Items[ListBox.ItemIndex];
     FUserName := copy(s,POS(' ',s)+ 1,length(s));
     FUserCode := arrayUserCodes[ListBox.ItemIndex];
     TblSetFiltered(dm.tblUserSecView,format(UsrSecView,[FUserCode]));
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.cbxAllUsersClick(Sender: TObject);
begin
     if cbxAllUsers.Checked then 
        LogListBox.Enabled := False
     else
        LogListBox.Enabled := True;
     case rgpLog.ItemIndex of 
        0 : ProcessChange(dm.qryProcess);
        2 : ProcessChange(dm.qryPointsLog);
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.menuManualBackupClick(Sender: TObject);
const
    WrongPCMsg = 'You should run this from the PC it was set up on' + #13#10 + 'Continue?';
    BackupMsg = 'Database Backup';
    msg1 = 'Cannot insert into Backup_LOG ';
    SQL1 = 'Select MAX(BACKUP) as MAX_BACKUP from BACKUP_LOG';
    SQL2 = 'Insert into BACKUP_LOG (BACKUP,NAME,DATE_TIME,SUCCESS,"TYPE",' +
           'MAN_AUTO,PCNAME,NOTIFIED) values (%d,''%s'',''%s'',''N'',0,''%s'',''%s'',''Y'')'; 
var MaxBackup : LongInt;
    d : string[20];
    ExtBackupMsg,BackupFileName : string[40];
    charManualBackup : char;
    dirBackup,strDate : shortstring;
    tblTemp : TIBTable;
    ManualBackup : Boolean;
    v : variant;
begin
     // both Manual and Auto backups look here - Auto backup dates are not altered
     // or delete of old entries
     if Global.DisableServices then begin
        Sm(DisabledServiceMsg );
        exit;
     end;
     if Busy then exit;
     PageControl.ActivePage := tsBackup;
     PageControlChange(Sender);
     
     BackupMemo.Lines.Clear;
     if TDirectoryEdit(Sender).Name = btnBackup.NAme then begin
        dirBackup := dirManBackup.Text;
        ExtBackupMsg := 'Manual ' + BackupMsg;
        ManualBackup := True;
     end else begin
        dirBackup := dirAutoBackup.Text;
        ExtBackupMsg := 'Automatic ' + BackupMsg;
        ManualBackup := False;                
     end;
     if dirBackup[length(dirBackup)] = BackSlash then  // incase c:\
        system.delete(dirBackup,length(dirBackup),1);
     // shouldnt run it from another PC - dir's may be different
     if ManualBackup then begin
        if Global.ManBackupPCName <> Global.ReadComputerName then 
           if not IfMsgDlg(WrongPCMsg) then exit;
     end else begin
        if Global.AutoBackupPCName <> Global.ReadComputerName then 
           if not IfMsgDlg(WrongPCMsg) then exit;
     end;
     //if not directoryexists(dirBackup) then begin
     //   sm(format('%s %s',[DirNotExistMsg,dirBackup]));
    //    exit;
    // end;
     try  // Internal IBBAckup Service
        TooBusy(ExtBackupMsg + PleaseWaitMsg);
        with dm.IBBackupService do begin
             ServerName := global.regServerName;
             Params.Clear;
             Params.Add(Format('USER_NAME=%s',['SYSDBA']));
             // MenuMate password is different - must use SYSDBA
             if MenuMateInstalled then
                Params.Add(Format('PASSWORD=%s' ,['masterkey']))
             else   
                Params.Add(Format('PASSWORD=%s' ,['master']));
             Active := True;
             if IsServiceRunning then begin 
                sm( ExtBackupMsg + 'is already running ' + CantContinueMsg);
                Active := False;
                NotBusy;
                exit;
             end;
             try
               Verbose := True;
               //Options := [NonTransportable, IgnoreLimbo]; set in IBBackupService
               DatabaseName := regDataBaseName;
               // add on backup file name
               if ManualBackup then begin
                  BackupFileName := StockMasterBackupName
               end else begin
                  if cbxAutoBackupFileNames.Text = 'No' then
                     BackupFileName := StockMasterBackupName
                  else    // SMddmmyy.gbk   8.3 for Novel
                     BackupFileName := BackSlash + FilePrefix + formatdatetime('ddmmyy',date) + '.gbk'; 
               end;      
               BackupFile.Clear; // keeps them stored up so clear it
               BackupFile.Add(dirBackup + BackupFileName);
               // Backup_LOG has no generator
               if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL1) then
                  MaxBackup := dm.qryGeneral.FieldByName('MAX_BACKUP').AsInteger;
               inc(MaxBackup);   
               strDate := GetShortDateTime(now);
               if ManualBackup then
                  charManualBackup := 'M'
               else
                  charManualBackup := 'A';
               if not dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[MaxBAckup,dirBackup + BackupFileName,strDate,charManualBackup,Global.ReadComputerName])) then begin  
                  sm( msg1 + CantContinueMsg );
                  exit;
               end;
               ServiceStart;
               While not Eof do begin
                  BackupMemo.Lines.Add(GetNextLine);
                  Application.ProcessMessages;
               end; 
               //copyfiles('Qwerty2\\temp\stkmast.gbk','c:\Temp\stkmast.gbk',false);
                
               // only way to insert log into blob field
               with TIBTable.create(nil) do begin
                  try
                     Database := dm.IBDatabase;
                     Transaction := dm.IBTransaction;
                     Tablename := 'BACKUP_LOG';
                     Open;
                     v := MaxBAckup;
                     if Locate('BACKUP',v,[]) then begin
                        Edit;
                        FieldByName('LOG'    ).AsString   := BackupMemo.Text;
                        FieldByName('SUCCESS').AsString   := 'Y';
                        Post;
                     end;   
                  finally
                     close;               
                     free;
                  end;
               end;     
               if ManualBackup then begin
                  dm.tblMISC.Edit;
                  dm.tblMISC.FieldByName('M_BACKUP_DATE').AsDateTime := date;
                  // incase app doesnt get restarted
                  Global.ManLastBackup := Date;
                  dm.tblMISC.Post;
               end;
               // delete log
               dm.DeleteLog(BackupLogTableName,'DATE_TIME',90); 
               messagedlg(ExtBackupMsg + CompletedMsg,mtInformation,[mbOK],0);
             except
                //dm.IBRollbackRetain;
                messagedlg(ExtBackupMsg + PleaseTryAgainMsg,mtError,[mbOK],0);
            end;
        end;
     finally
         dm.IBCommitRetain;
         dm.IBBackupService.Active := False;
         NotBusy;
     end;
end; 
{ **************************************************************************** }
procedure TfrmMembersSYS.dirManBackupChange(Sender: TObject);
begin
     dm.tblMisc.Edit;
     if TDirectoryEdit(Sender).Name = dirManBackup.Name then begin
        btnBackup.Hint := 'Start Manual Backup to ' + dirManBackup.Text;
        dm.tblMisc.FieldByName('M_BACKUP_DIR').AsString := dirManBackup.Text;
     end else begin
        btnAutoBackup.Hint := 'Start Automatic Backup test to ' + dirAutoBackup.Text;
        dm.tblMisc.FieldByName('A_BACKUP_DIR').AsString := dirAutoBackup.Text;
     end;
     dm.tblMisc.Post;
end;
{ **************************************************************************** }
function TfrmMembersSYS.ChangeUser( UserTypes : TUserTypes ) : Boolean;
var   i : integer;
begin
     Result := False;
     with dm.IBSecurityService do begin
        Active := False;
        ServerName := regServerName;
        Active := True;
        try
            UserName := FUserName;
            case UserTypes of
               utDelete : DeleteUser;
               utAdd    : begin
                             Password := FPassword;
                             UserID   := FUserCode;
                             AddUser;
                          end;
               utModify : begin
                             Password := FPassword;
                             ModifyUser;
                          end;
            end; 
            Result := True;
          finally
             Active := False;
          end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.PopulateListBox;
var i : shortint;
begin
     ListBox.Items.Clear;
     LogListBox.Items.Clear;
     for i := 0 to Global.CountArrayUserNames do begin
        ListBox.Items.Add(intToStr(Global.arrayUserCodes[i]) + '. ' + Global.arrayUserNames[i]);
        LogListBox.Items.Add(intToStr(Global.arrayUserCodes[i]) + '. ' + Global.arrayUserNames[i]);
     end;
     if i > 0 then LogListBox.ItemIndex := 0;
     if i > 0 then ListBox.ItemIndex := 0;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.btnCancelClick(Sender: TObject);
begin
     pnlPassword.Visible := False;
     SetButtons(True);
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.btnOKClick(Sender: TObject);
var i,HoldUser : integer;
const msg1 = 'You must enter a User Name';
      msg2 = 'you must enter a Password';  
      msg3 = 'Passwords are not the same';  
      msg4 = 'Cannot modify user';
      msg5 = 'Cannot add user';
      msg6 = 'Cannot grant Allusers to ';
      SQL1 = 'GRANT ALLUSERS TO %s with ADMIN OPTION';
      SQL2 = 'select * from SCREENS'; 
begin
     // only insert or update of user
     if FAddUser then begin // for add only
        if edtUsername.Text = '' then begin
           sm(msg1);
           edtUserName.SetFocus;
           exit;
        end;
     end;  
     if edtPassword.Text = '' then begin
        sm(msg2);
        edtPassword.SetFocus;
        exit;
     end;
     if edtPassword.Text <> edtConfirmPassword.Text then begin
        sm(msg3);
        edtPassword.SetFocus;
        exit;
     end;
     if FAddUser then begin
        FUserName := edtUserName.Text;
        HoldUser  := FUserCode;
        FUserCode := Global.HighestUserCode + 1;
     end;   
     FPassword := edtPassword.Text;
     SetProcessType;
     if FAddUser then begin
        if not ChangeUser(utAdd) then begin
           sm(msg5);
           exit;
        end;
        // grant role to user
        if not dm.sqlExecQuery(dm.qryGeneral,Format(SQL1,[FUserName])) then begin
            sm(msg6 + FUserName);
            exit;
        end;   
         // add into tblUser_sec
        with dm.qryGeneral do begin 
           if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL2 ) then begin
              while not EOF do begin
                 dm.tblUserSecView.Insert;
                 dm.tblUserSecView.FieldByName('USER_CODE').Asinteger   := FUserCode;
                 dm.tblUserSecView.FieldByName('SCREEN_CODE').Asinteger := FieldByName('SCREEN_CODE').AsInteger;
                 dm.tblUserSecView.FieldByName('HAS_ACCESS').AsString   := FieldByName('DEF_ACCESS').AsString;
                 dm.tblUserSecView.Post;
                 Next;
              end;
           end;
        end;
        TblSetFiltered(dm.tblUserSecView,format(UsrSecView,[HoldUser]));
        dm.LogProcess(dm.qryGeneral, 'User ' + IntToStr(FUserCode) + ' ' + FUserName + ' Added');
        inc(global.CountArrayUserNames);
        Global.HighestUserCode := FUserCode;
        ArrayUserNames[global.CountArrayUserNames] := FUserName;
        ArrayUserCodes[global.CountArrayUserNames] := FUserCode;
        ListBox.Items.Add( IntToStr(FUserCode) + '. ' + FUserName );
        LogListBox.Items.Add( IntToStr(FUserCode) + '. ' + FUserName );
     end else begin    
        if not ChangeUser(utModify) then begin
           sm(msg4);
           exit;
        end;
        dm.LogProcess(dm.qryGeneral, 'User ' + IntToStr(FUserCode) + ' ' + FUserName + ' Changed');
     end;
     dm.IBCommitRetain;   
     pnlPassword.Visible := False;
     SetButtons(True);
end;
{ **************************************************************************** }
function TfrmMembersSYS.LoggedOn( NewUserName : string ) : Boolean ;
var i : integer;
begin
      result := false;
      with dm.IBDatabaseInfo do begin
         for I := 0 to UserNames.Count - 1 do begin
            if UserNames[I] = NewUserName then result := true;
            break;
         end;   
      end;      
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.edtUserNameKeyPress(Sender: TObject;
  var Key: Char);
begin
     if key = #13 then 
       SelectNext(ActiveControl as TWinControl,True,True);
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.WriteReg;
// writes to HKEY_CURRENT_USER Software Membership   - user regedit
//const regSection = '\Software\Membership';
const regSection    = '\SOFTWARE\IQWORKS\MenuMate\Membership';
      msg1 = 'Do you wish to change the Registry settings?';
      msg2  = 'Cannot write to the registry - changes not accepted';
var  regFile : TRegistry;
     LogOn   : String;
    
begin             
     if not IfMsgDlg(msg1) then begin
        RegSetNames;
        exit;
     end;   
     try
        try
           regFile := TRegistry.Create;
           regFile.RootKey  := HKEY_LOCAL_MACHINE;
           //regFile := TRegInifile.Create( 'Software' );
           with regFile do begin
              if OpenKey(regSection, True) then begin
                 WriteString ('ServerName', edtServerName.Text );
                 regServerName := edtServerName.Text;
                 WriteString ('DatabaseName',edtDataBaseName.Text );
                 regDataBaseName := edtDataBaseName.Text;
                 edtDataBaseName.Hint :=  regDataBaseName;
                 case cbxUserLogon.ItemIndex of
                    0 : begin
                           regLogOn := False; //No
                           Logon := '0'; //No
                        end;   
                    1 : begin
                           regLogOn := True; //true
                           Logon := '1'; //Yes
                        end;   
                 end;
                 WriteString ('UserLogon',  Logon );
                 WriteString ('LocalUserName', edtLocalUserName.Text );
                 regLocalUserName := edtLocalUserName.Text;
                 // MenuMate
                 WriteString ('MenuMateServerName', edtMenuMateServerName.Text );
                 regServerName := edtMenuMateServerName.Text;
                 WriteString ('MenuMateDatabaseName',edtMenuMateDataBaseName.Text );
                 regDataBaseName := edtMenuMateDataBaseName.Text;
              end; 
           end;   
        except
           sm(msg2);
        end;   
     finally
        regFile.Free;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.RegSetNames;
begin
     edtServerName.Text    :=  regServerName;
     edtDataBaseName.Text  :=  regDataBaseName;
     edtDataBaseName.Hint :=  regDataBaseName;
     edtMenuMateServerName.Text    :=  regMenuMateServerName;
     edtMenuMateDataBaseName.Text  :=  regMenuMateDataBaseName;
     
     if regLogOn then
        cbxUserLogon.Text     :=  'Yes'
     else
        cbxUserLogon.Text     :=  'No';
     edtLocalUserName.Text :=  regLocalUserName;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.edtServerNameChange(Sender: TObject);
begin
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.cbxUserLogonChange(Sender: TObject);
begin
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.btnSaveClick(Sender: TObject);
begin
     case pcSite.ActivePageIndex of
        2 : SaveSite; // Site
        1 : WriteReg; //Registry
        0 : SaveMisc; // Misc
        3 : SaveMenuMate; // Misc
        4 : SaveKiosk; 
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.edtServerNameKeyPress(Sender: TObject;
  var Key: Char);
begin
     if key = #13 then 
       SelectNext(ActiveControl as TWinControl,True,True);
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.SetAutoBackup;
var b : boolean;
const msg1 = 'The system needs restarting for the changes to take effect';
begin
     if dm.tblMISC.State = dsEdit then begin
        dm.tblMisc.Post;
        dm.IBCommitRetain;
     end;
     b := StrToBoolean(dm.tblMisc.FieldByName('A_BACKUP_ENABLED').AsString);
     btnAutoBackup.Enabled := b;
     dirAutoBackup.Enabled := b;
     cbxAutoBackupFileNames.Enabled := b;
     dbeAutoBackupKeep.Enabled := b;
     cbxAutoBackupType.Enabled := b;
     dtpAutoBackupTime.Enabled := b;
     dtpAutoBackupDate.Enabled := b;
     if cbxAutoBackupEnabled.Modified then begin
        MDlgI(msg1);
        with dm.tblMisc do begin
           Edit;
           FieldByName('A_BACKUP_PCNAME').AsString := ReadComputerName;
           Post;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.SetManualBackup;
var b : boolean;
begin
     if dm.tblMISC.State = dsEdit then begin
        dm.tblMisc.Post;
        dm.IBCommitRetain;
     end;   
     b := StrToBoolean(dm.tblMisc.FieldByName('M_BACKUP_ENABLED').AsString);
     btnBackup.Enabled        := b;
     dirManBackup.Enabled     := b;
     cbxManBackupType.Enabled := b;
     if cbxManBackupEnabled.Modified then begin
        with dm.tblMisc do begin
           Edit;
           FieldByName('M_BACKUP_PCNAME').AsString := ReadComputerName;
           Post;
           ManBackupPCName := ReadComputerName;
        end;                      
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.cbxManBackupEnabledCloseUp(Sender: TwwDBComboBox; Select: Boolean);
begin
     if not active then exit;
     if Global.DisableServices then begin
        Sm(DisabledServiceMsg );
        cbxManBackupEnabled.Text := varNoWord;
     end;
     SetManualBackup;      
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.cbxManBackupEnabledKeyUp(Sender: TObject;
  var Key: Word; Shift: TShiftState);
begin
     if not active then exit;
     if (cbxManBackupEnabled.Text = varNoWord) then begin
        dm.tblMISC.Edit;                                           
        dm.tblMISC.FieldByName('M_BACKUP_ENABLED').AsString := varNo;
        SetManualBackup; 
     end;  
     if (cbxManBackupEnabled.Text = varYesWord) then begin
        dm.tblMISC.Edit;
        dm.tblMISC.FieldByName('M_BACKUP_ENABLED').AsString := varYes;
        SetManualBackup;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.cbxAutoBackupEnabledCloseUp(Sender: TwwDBComboBox;  Select: Boolean);
begin
     if not active then exit;
     if Global.DisableServices then begin
        Sm(DisabledServiceMsg );
        cbxAutoBackupEnabled.Text := varNoWord;
     end;
     SetAutoBackup;      
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.cbxAutoBackupEnabledKeyUp(Sender: TObject;
  var Key: Word; Shift: TShiftState);
begin
     if not active then exit;
     if (cbxAutoBackupEnabled.Text = 'NO') then begin
        dm.tblMISC.Edit;                                           
        dm.tblMISC.FieldByName('A_BACKUP_ENABLED').AsString := 'N';
        SetAutoBackup; 
     end;  
     if (cbxAutoBackupEnabled.Text = 'YES') then begin
        dm.tblMISC.Edit;
        dm.tblMISC.FieldByName('A_BACKUP_ENABLED').AsString := 'Y';
        SetAutoBackup; 
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.cbxAutoBackupFileNamesCloseUp(
  Sender: TwwDBComboBox; Select: Boolean);
begin
     if not active then exit;
     if cbxAutoBackupFileNames.modified then begin
        if cbxAutoBackupFileNames.Text = 'YES' then
           dbeAutoBackupKeep.Visible := True
        else   
           dbeAutoBackupKeep.Visible := False;
     end;      
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.AdjustDate;
begin
     dm.tblMISC.Edit;
     if dtpAutoBackupTime.Time < time then 
        dm.tblMISC.FieldByName('A_BACKUP_DATE').AsDateTime := (date + 1)
     else
        dm.tblMISC.FieldByName('A_BACKUP_DATE').AsDateTime := date;
     dm.tblMISC.Post;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dtpAutoBackupTimeExit(Sender: TObject);
// adjust dates according to time
begin
     AdjustDate;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dbgLogsDblClick(Sender: TObject);
// show log file in  frmPassword
begin  
     if rgpLog.ItemIndex <> 1 then exit; // not the backup log
     with TfrmPasswordDlg.Create( nil ) do begin
        try
           ShowAsMemo := True;
           Memo.Text := dm.qryBackupLog.FieldByName('LOG').AsString;
           ShowModal;
        finally
           Free;
        end;   
     end;     
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dtpAutoBackupTimeKeyPress(Sender: TObject;
  var Key: Char);
begin
     if key = #13 then AdjustDate;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.ExportArevFiles;
//const SQL1 = 'Select Distinct EXPORT_DATE from EXPORT_AREV where EXPORTED is NULL' +
//             ' order by EXPORT_DATE';
begin
 {    rgpExport.Items.Clear;
     ExportList.Clear;
     if not dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL1) then exit;
     while not dm.qryGeneral.EOF do begin
        rgpExport.Items.Add( GetShortMonthDate(dm.qryGeneral.FieldByName('EXPORT_DATE').AsDateTime) );
        ExportList.Add( GetShortDate(dm.qryGeneral.FieldByName('EXPORT_DATE').AsDateTime) );
        dm.qryGeneral.Next;
     end;
     if rgpExport.Items.Count >= 1 then rgpExport.ItemIndex := 0;}
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.ImportArevFiles;
const
    FileExt = '.DAT';
    asterisk = '*';
var    
    SearchRec : TSearchRec;
    ImportFileDate : TDateTime;          
    i : Shortint;
    s : Shortstring;
    LocalList : TStringList;
begin
     rgpImport.Items.Clear;
     ImportList.Clear;
     try
        LocalList := TStringlist.Create;
        If ( FindFirst( Global.ArevImportFilePath + BackSlash + FilePrefix + asterisk + FileExt , faAnyFile, SearchRec ) = 0 ) then begin
           Repeat       // yymmdd
              LocalList.Add( copy(SearchRec.Name,7,2) + copy(SearchRec.Name,5,2) + copy(SearchRec.Name,3,2) );
           Until ( FindNext( SearchRec ) <> 0 );
       end;
       LocalList.Sorted := True;
       // Must load in oldest date first
       for i := 0 to LocalList.Count -1 do begin
          try
             s := LocalList[i];
             ImportFileDate := StrToDateTime(copy(S,5,2) + '/' + copy(S,3,2) + '/' + copy(S,1,2)); 
             rgpImport.Items.Add( GetShortMonthDate(ImportFileDate) );
             ImportList.Add('SM' + (copy(S,5,2) + copy(S,3,2) + copy(S,1,2)) + '.DAT');
          except
             continue; 
          end;
       end;   
       if rgpImport.Items.Count >= 1 then rgpImport.ItemIndex := 0;
    finally
       LocalList.Free;
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.btnExportClick(Sender: TObject);
var i : shortint;
begin
     {if  busy then exit;
     if rgpExport.ItemIndex = -1 then begin
        sm( 'There are no Overnight files to Process');
        exit;
     end;
     if (rgpExport.Items.Count > 1) and (rgpExport.ItemIndex > 0) then begin
        sm( 'You must select the oldest date first');
        exit;
     end;
     FGoodDate := Date;
     if not Global.CheckSystemDate( FGoodDate ) then begin
        sm( IncorrectDateMsg + #13#10 + 'The date must be altered!');
        exit;
     end;
     if MessageDlg(format('Proceed with %s of %s',[POSExport,rgpExport.Items[rgpExport.ItemIndex]]),
        mtConfirmation,[mbYes,mbNo],0) = mrNo then exit;
     try
        SetProcessType;
        if not dm.IBRollover.InTransaction then dm.IBRollover.StartTransAction;
        try
           for i := 0 to rgpExport.Items.Count -1 do begin
              TooBusy(format('Exporting  %s',[rgpExport.Items[0]]));
              ExportToFile(ExportList[i]);
              dm.LogProcess(dm.sqlRollover,POSExport + ' of ' + rgpExport.Items[0] + ' ' + CompletedMsg);
           end;
           if dm.IBRollover.InTransAction then
              dm.IBRollover.Commit;
        except;
           if dm.IBRollover.InTransAction then begin 
              dm.IBRollover.RollBack;
              sm( UsersMakingChangesMsg + #13#10 + 'The ' + POSImport + PleaseTryAgainMsg );
              exit;
           end;   
        end;
        NotBusy;
        MDlgI(POSExport + CompletedMsg );
     finally
        NotBusy;
     end;}   
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.ExportToFile( ExportDateStr : ShortString );
{var s,ExportFileName : String;
    ExportFile : TextFile;
    
const FileExportExt = '.EXP';    
      comma = ',';}
begin
     {with dm.ibdsExport do begin
        Close;
        Params.ByName('EXPORTDATE').AsString := ExportDateStr;
        Open;
        First;
        try
           ExportFileName := Global.ArevImportFilePath + BackSlash + FilePrefix +   FileExportExt;
           if FileExists( ExportFileName ) then begin
              AssignFile( ExportFile,ExportFileName );
              Rewrite( ExportFile,ExportFileName );
           end else begin  
              AssignFile( ExportFile,ExportFileName );
              Reset( ExportFile,ExportFileName );
           end; 
           while not EOF do begin
              s := '';
              s := FieldByName('TYPE').AsString + comma + FieldByName('MEMBER').AsString + comma +
                   FieldByName('KNOWN_AS').AsString + comma + FieldByName('SUB').AsString;
              WriteLn(ExportFile,s);
              Edit;
              FieldByName('EXPORTED').AsString := 'Y';
              Post;    
              next;
           end;
        finally
           closefile( ExportFile );
        end;
     end;}
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.btnImportClick(Sender: TObject);
// Import Arev file from the OverNight
// Use sqlRollover - transaction IBRollover
const BusyFlag = 'B';
      FreeFlag = 'F';
var Renamefilename : ShortString;      
begin
     if busy then exit;
     if rgpImport.ItemIndex = -1 then begin
        sm( 'There are no Overnight files to Process');
        exit;
     end;
     if (rgpImport.Items.Count > 1) and (rgpImport.ItemIndex > 0) then begin
        sm( 'You must select the oldest date first');
        exit;
     end;
     FGoodDate := Date;
     if not Global.CheckSystemDate( FGoodDate ) then begin
        sm( IncorrectDateMsg + #13#10 + 'The date must be altered!');
        exit;
     end;
     if MessageDlg(format('Proceed with %s of %s',[POSImport,rgpImport.Items[rgpImport.ItemIndex]]),
        mtConfirmation,[mbYes,mbNo],0) = mrNo then exit;
     try
        with dm.IBRollover do begin
           TooBusy(format('Loading %s %s',[POSImport,rgpImport.Items[rgpImport.ItemIndex]]));
           try
              if not InTransaction then StartTransAction;
              rgpImport.Enabled := False;
              LoadIntoImportAREV; // load file into 'IMPORT_AREV'
              // load IMPORT_AREV into MEMBERS,M_SUBS,PAYMENTS,PAYMENT_DET,PAYMENT_TEND
              if not ImportAREVtoMembers then begin
                 RollBack;
                 StartTransaction;
                 exit; 
              end;
              SetProcessType;
              dm.LogProcess(dm.sqlRollover,POSImport + ' of ' + rgpImport.Items[rgpImport.ItemIndex] + ' ' + CompletedMsg);
              Renamefilename := Global.ArevImportFilePath + '\' + copy(ImportList[rgpImport.ItemIndex],1,9) + 'BAK';
              // 2 opportunities too rename the file incase 2 rollovers
              // were done in the same day
              if not RenameFile(Global.ArevImportFilePath + '\' + ImportList[rgpImport.ItemIndex],Renamefilename) then begin
                 Renamefilename := Global.ArevImportFilePath + '\' + copy(ImportList[rgpImport.ItemIndex],1,9) + 'BA1';
                 if not RenameFile(Global.ArevImportFilePath + '\' + ImportList[rgpImport.ItemIndex],Renamefilename) then begin
                    sm(format('Cannot Rename %s to %s - %s %s',[ ImportList[rgpImport.ItemIndex],Renamefilename,POSImport,DidntWorkMsg]));
                    RollBack;
                    StartTransaction;
                    exit;
                 end;
              end;
              if InTransaction then Commit;
              NotBusy;
              ImportArevFiles;   
              MDlgI(POSImport +  CompletedMsg );
           except
              if InTransaction then begin
                 RollBack;
                 StartTransaction;
              end;
              NotBusy;
              sm( UsersMakingChangesMsg + #13#10 + 'The ' + POSImport + PleaseTryAgainMsg );
           end;   
        end;
     finally
        //FFlag := FreeFlag;      // Free
        //CheckImportFile(True);  // write to file to say not in use 
        NotBusy;
        rgpImport.Enabled := True;
     end;   
    { try
        frmPointsAudit := TfrmPointsAudit.Create(Self);
        if frmPointsAudit.Extract(ReportPrinterIndex) then
           frmPointsAudit.qr.Preview;
     finally
        frmPointsAudit.Free;
     end;   }
end;
{ **************************************************************************** }
function TfrmMembersSYS.CheckImportFile( WriteToFile : Boolean ) : Boolean;
const FName     = '\AREVOVER.DAT';
      OMsg      = 'The POS Overnight is still running';
      Msg1      = 'Cannot find %s%s %s cancelled';
      Msg2      = 'Error opening %s%s %s cancelled';
var
     ArevFile    : TextFile;
     c : char;
begin
     Result := False;
     try
        TooBusy('Checking ' + POSImport + ' file ...');
        if not FileExists(Global.ArevImportFilePath + FName) then begin
           sm(format(msg1 + HelpDeskMsg + '%s',[Global.ArevImportFilePath,FName,POSImport,HelpDesk])); 
           exit;
        end;
        try 
   	     Assignfile ( ArevFile ,Global.ArevImportFilePath + FName );
           if WriteToFile then
        	     Rewrite ( ArevFile )
           else   
        	     Reset ( ArevFile );
        except
           sm(format(msg2 + HelpDeskMsg + '%s',[Global.ArevImportFilePath,FName,POSImport,HelpDesk])); 
           exit;
        end;
        if WriteToFile then begin          // Write F(Free) or B(Busy) to the file  
           WriteLn( ArevFile,FFlag );
           Result := True;
        end else begin
           ReadLn( ArevFile,c );
           if c = FFlag then                 // free
              Result := True
           else
              sm(OMsg + CantContinueMsg);  // overnight running
        end;
        CloseFile( ArevFile);         
     finally
        NotBusy;
     end;   
end;
{ **************************************************************************** }
function TfrmMembersSYS.ImportAREVtoMembers : Boolean; 
const SQL1 = 'Select Distinct(INVOICE) from IMPORT_AREV where "TYPE" = ''%s'' or "TYPE" = ''%s''';
//const SQL1 = 'Select Distinct(INVOICE) from IMPORT_AREV where "TYPE" <> ''%s'' and "TYPE" <> ''%s'' and "TYPE" <> ''%s''';
      SQL2 = 'Select * from IMPORT_AREV where INVOICE = %s';
      //SQL3 = 'Select * from IMPORT_AREV where "TYPE" = ''%s''';
      //SQL5 = 'Select * from IMPORT_AREV where "TYPE" = ''%s'' or "TYPE" = ''%s''';
      SQL3 = 'Select Distinct(MEMBER) from IMPORT_AREV where "TYPE" = ''%s'' or "TYPE" = ''%s''';
      SQL5 = 'update IMPORT_POINTS set %s = 0 where %s is null';      
      SQL4 = 'Select Count(*) from IMPORT_AREV';
      msg1 = 'Invalid Prefix " %s " in % file';
      msg2 = 'Member,Tender or Points are not Numeric';
      
var
      PA,TR,TE,PPC,BC,EC : Extended;
      MEMBER : LongInt;
      F : TextFile;
      s : String;
{              
 type       SUBS,JOIN 
 Invoice    1 per tranaction
 Member
 Points
 Tender
 Tender name
 Paid By Member
 Subscription

 use qryGeneral to Loop thru IMPORT_AREV      

 }
var
   InvoiceList : TStringList;
   i : LongInt;
   //DelimInput : TIBInputDelimitedFile;
   //MemberPA,ImportPA : Extended;
begin             
     Result := False;
     if not dm.sqlExecQueryCheckEOFCount(dm.sqlRollover,SQL4) then begin
        sm( 'NB there are no Records in the ' + POSImport );
        Result := True;
        exit;
     end;   
     { **************************************************************************** }
     // get the next entries by Invoice - for Subs
     try
        InvoiceList := TStringList.Create;
        MemberList  := TStringList.Create;
        dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL1,['SUBS','JOIN']));
        // get distinct Invoice - one per payment 
        while not dm.sqlRollover.EOF do begin
           InvoiceList.Add(dm.sqlRollover.FieldByName('INVOICE').AsString);
           dm.sqlRollover.Next;
        end;   
        for i := 0 to InvoiceList.Count -1 do begin
           // use qryGeneral for the Select - use sqlRollover for updates / Inserts
           with dm.qryGeneral do begin
              // get tranactions/payments per Invoice
              if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL2,[InvoiceList[i]])) then begin
                 FHoldPoints := 0.00;
                 FHoldTender := 0.00;
                 while not EOF do begin
                    // watch the last line ZZZZZ - totals only
                    PrefixImportTypes := enmPrefixImportTypes(FieldByName('TYPE').AsString);
                    if not ConvertAmounts then exit;
                    case PrefixImportTypes of 
                       piSubsPaid      : begin
                                            // if subs payment is 0.00
                                            if (FTender = 0.00) and (FPointsDollar = 0.00) then begin
                                               next;
                                               continue;
                                            end;
                                            MemberList.Add(IntToStr(FMember));
                                            PointsCreditTypes := pcSubPaidPOS;
                                            if not UpdateSubsPaid then exit;
                                            //UpdateMemberPoints; 
                                         end;   
                       piJoinFeePaid   : begin
                                            // if subs payment is 0.00
                                            if (FTender = 0.00) and (FPointsDollar = 0.00) then begin
                                               next;
                                               continue;
                                            end;   
                                            if not UpdateJoinFee then exit;
                                         end;   
                    end; // end case
                    // accumulate for PAYMENT
                    if (PrefixImportTypes = piSubsPaid) or (PrefixImportTypes = piJoinFeePaid) then begin
                       FHoldPoints := FHoldPoints + FPointsDollar;
                       FHoldTender := FHoldTender + FTender  + FPointsDollar;
                    end;
                    next;
                 end;
                 // have to assume there are only Subs and Joining fee on the same Invoice #
                 if (PrefixImportTypes = piSubsPaid) or (PrefixImportTypes = piJoinFeePaid) then begin
                    if not UpdateSubsByPayment then exit; 
                    if not UpdatePayments then exit; // fill in final details of PAYMENTS
                 end;   
              end; // if
           end; // with
        end; // for
     finally
        MemberList.Free;
        InvoiceList.Free;
     end;
     // Update Card Last Used - select distinct MEMBER
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL3,['SPNT','ERNT'])) then begin
        while not dm.qryGeneral.EOF do begin
           dm.sqlExecQuery(dm.sqlRollover,format('update MEMBERS set CARD_LAST_USED = Current_Date -1 where MEMBER = %d',[dm.qryGeneral.FieldByName('MEMBER').AsInteger]));
           dm.qryGeneral.Next;
         end;  
     end;  
     // there are more files to process - so dont do it until the last file
     if rgpImport.Items.Count > 1 then begin
        // empty table - wont cause the PointsAudit Report to fire as there are more files to process
        dm.sqlExecQuery(dm.sqlRollover,'Delete from IMPORT_POINTS' ); 
        Result := True;
        exit;
     end;   
     if not FileExists(Global.ArevImportFilePath + '\Points.Dat') then begin
        Result := True;
        exit;
     end;   
     try
        // empty the table
       // dm.sqlExecQuery(dm.sqlRollover,'Delete from IMPORT_POINTS' ); 
        {dm.sqlRollover.SQL.TEXT := 'Insert into IMPORT_POINTS values(:MEMBER,:POINTS_AVAILABLE,:TOTAL_REDEEMED,:TOTAL_EARNT,:PRE_PAID_CREDIT,:BONUS_CREDIT,:EARNT_CREDIT)';
        DelimInput := TIBInputDelimitedFile.Create;
        //DelimInput.FileName := 'c:\Points.txt';
        DelimInput.FileName := Global.ArevImportFilePath + '\Points.Dat';
        DelimInput.ColDelimiter := ',';
        DelimInput.ReadBlanksAsNull := True;
        dm.sqlRollover.BatchInput(DelimInput);}
      {  dm.sqlExecQuery(dm.sqlRollover,'Delete from IMPORT_POINTS where MEMBER = ''0''' );
        dm.sqlExecQuery(dm.sqlRollover,format(SQL5,['POINTS_AVAILABLE','POINTS_AVAILABLE']));
        // POINTS_AVAILABLE come in as 444 instead of 4.44
        dm.sqlExecQuery(dm.sqlRollover,'update IMPORT_POINTS set POINTS_AVAILABLE = POINTS_AVAILABLE / 100' );
        dm.sqlExecQuery(dm.sqlRollover,format(SQL5,['TOTAL_REDEEMED','TOTAL_REDEEMED'] ));
        dm.sqlExecQuery(dm.sqlRollover,format(SQL5,['TOTAL_EARNT','TOTAL_EARNT'] ));
        dm.sqlExecQuery(dm.sqlRollover,format(SQL5,['PRE_PAID_CREDIT','PRE_PAID_CREDIT']));
        dm.sqlExecQuery(dm.sqlRollover,format(SQL5,['BONUS_CREDIT','BONUS_CREDIT'] ));
        dm.sqlExecQuery(dm.sqlRollover,format(SQL5,['EARNT_CREDIT','EARNT_CREDIT']));
        dm.sqlExecQueryCheckEOF(dm.sqlRollover,'Select * from IMPORT_POINTS');}

        //GetCommaDelimitedField
        AssignFile( F ,Global.ArevImportFilePath + '\Points.Dat');
        Reset(F);
        while not EOF(F) do begin
           with dm.ibdsMemberPoints do begin
              ReadLn(F,s);
              if not GetPointsFromString(s,Member,PA,TR,TE,PPC,BC,EC) then continue;
              Close;
              paramByName('Member').AsInteger := Member;
              Open;
              if not EOF then begin
                 Edit;
                 FieldByName('TOTAL_EARNT').AsFloat     := TE; 
                 FieldByName('TOTAL_REDEEMED').AsFloat  := TR; 
                 FieldByName('EARNT_CREDIT').AsFloat    := EC; 
                 FieldByName('BONUS_CREDIT').AsFloat    := BC; 
                 FieldByName('PRE_PAID_CREDIT').AsFloat := PPC; 
                 FieldByName('POINTS_AVAILABLE').AsFloat :=  PA;
                // MemberPA := RoundCurrency(FieldByName('POINTS_AVAILABLE').AsFloat) ;
                // ImportPA := RoundCurrency(dm.sqlRollover.FieldByName('POINTS_AVAILABLE').AsFloat);
                 //sm(format('Member %d PA = %g Import = %g',[dm.sqlRollover.FieldByName('MEMBER').AsInteger,MemberPA,ImportPA]));
                 // setCard Last Used to today if Points Available is not correct - for the Audit Report
                 //if MemberPA <> ImportPA then
                 //   FieldByName('CARD_LAST_USED').AsDateTime := Date;
                 post;                                                                
              end;
           end;
        end;
     finally
        //DelimInput.Free;
     end;
     Result := True;
end;
{ **************************************************************************** }
function TfrmMembersSYS.ConvertAmounts : Boolean;
const msg1 = 'Member,Tender or Points are not Numeric';
      //SQL1 = 'Select MEMBER from MEMBERS where MEMBER = %d';
begin
     Result := False;
     with dm.qryGeneral do begin
        try
           if not FieldByName('POINTS').IsNull then begin
              FPoints  := FieldByName('POINTS').AsFloat;
              // -ve works OK ,convert to dollars
              FPointsDollar := FPoints / 100;
           end else begin
              FPointsDollar := 0.00;   
              FPoints := 0.00;   
           end;   
           if not FieldByName('MEMBER').IsNull then begin
              FMember    := FieldByName('MEMBER').AsInteger;
              //if not dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL1,[FMember])) then begin
              //   sm(format ('Member %d is not in the file for %s',[FMember,POSImport]));
              //   exit;
              //end;
              { else begin
                 FPointsAvailable := dm.sqlRollover.FieldByName('POINTS_AVAILABLE').AsFloat; 
                 FTotalRedeemed := dm.sqlRollover.FieldByName('TOTAL_REDEEMED').AsFloat;
                 FTotalEarnt := dm.sqlRollover.FieldByName('TOTAL_EARNT').AsFloat; 
                 //FPointsAvailable := RoundCurrency(dm.sqlRollover.FieldByName('POINTS_AVAILABLE').AsFloat); 
                 //FTotalRedeemed := RoundCurrency(dm.sqlRollover.FieldByName('TOTAL_REDEEMED').AsFloat);
                 //FTotalEarnt := RoundCurrency(dm.sqlRollover.FieldByName('TOTAL_EARNT').AsFloat); 
              end; }
           end else begin
              sm(format ('Member %s is not Numeric for %s',[FieldByName('MEMBER').AsString,POSImport]));
              exit;
           end;
           // NB 123 in the Import File  = $1.23
           if not FieldByName('TENDER').IsNull then begin
              FTender := FieldByName('TENDER').AsFloat;
              // Credit and SubsPaid comes thru as 20.00
              if (PrefixImportTypes <> piSubsPaid) and (PrefixImportTypes <> piCredit) then begin
                 FTender := FTender / 100;
              end;   
           end else FTender := 0.00;   
           if not FieldByName('PAID_BY').IsNull then
              FPaidByMember := FieldByName('PAID_BY').AsInteger
           else FPaidByMember := 0;   
           if FieldByName('SUBS').AsInteger > 0 then begin
              FSubs         := FieldByName('SUBS').AsInteger
           end else begin 
              // Palmerston north has no Subs get the correct one for the Member
              dm.sqlExecQuery(dm.qryGeneral1,format('select SUBSCRIPTION from M_SUBS where MEMBER = %d',[FMember]));
              FSubs := dm.qryGeneral1.FieldByName('SUBSCRIPTION').AsInteger;
           end;

           
           FTenderName   := Trim(FieldByName('TENDER_NAME').AsString);
           // Subs Payment VIP PAY by Points - stored in VALUE
           if (FTenderName = 'VIP PAY') and  (PrefixImportTypes = piSubsPaid) then begin
              FPoints  := FieldByName('VALUE').AsFloat;
              FPointsDollar := FPoints / 100;
              // puts points in the VAlue and the same AMOUNT in the Tender Column 
              // so delete it
              FTender := 0.00;
           end;
           
           Result := True;
        except
           Result := False;
           sm(format ('%s %s for Member %s',[POSImport,msg1,FieldByName('MEMBER').AsString]));
           exit;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.LoadIntoImportAREV; 
const sqldel = 'delete from IMPORT_AREV';
      SQL1 = 'update IMPORT_AREV set INVOICE = 0 where INVOICE IS NULL';
var s,TempStr,s1 : String;
    F : TextFile;      
    TempTable : TIBTable;
//var DelimInput : TIBInputDelimitedFile;
begin
     {try
        // empty the table
        if not dm.sqlExecQuery(dm.sqlRollover,sqldel) then begin
           sm( 'Unable to ' + sqldel); 
           exit;
        end;
        dm.sqlRollover.SQL.TEXT := 'Insert into IMPORT_AREV values(:TYPE,:INVOICE,:MEMBER,:POINTS,:TENDER,:TENDER_NAME,:VALUE,:PAID_BY,:SUBS)';
        DelimInput := TIBInputDelimitedFile.Create;
        DelimInput.FileName := Global.ArevImportFilePath + '\' + ImportList[rgpImport.ItemIndex];
        DelimInput.ColDelimiter := ',';
        DelimInput.ReadBlanksAsNull := True;
        dm.sqlRollover.BatchInput(DelimInput);
        // fill Invoice where blank - on the last line ZZZZ - need if selecting on
        dm.sqlExecQuery(dm.sqlRollover,SQL1);
        dm.IBRollover.CommitRetaining;
     finally
        DelimInput.Free;
     end;}
     try
        // empty the table
        if not dm.sqlExecQuery(dm.sqlRollover,sqldel) then begin
           sm( 'Unable to ' + sqldel); 
           exit;
        end;
        // 
        
        //sm(Global.ArevImportFilePath + '\' + ImportList[rgpImport.ItemIndex]);
        AssignFile(F,Global.ArevImportFilePath + '\' + ImportList[rgpImport.ItemIndex]);
        Reset(F);
        with TIBTable.create(nil) do begin
           try
              Database := dm.IBDatabase;
              Transaction := dm.IBRollover;
              Tablename := 'IMPORT_AREV';
              Open;
              while not System.EOF(F) do begin
                 ReadLn(F,S);
                 Insert;
                 FieldByName('TYPE').AsString        := GetCommaDelimitedField(0,s);
                 FieldByName('INVOICE').AsInteger    := StrToIntDef(GetCommaDelimitedField(1,s),0);
                 FieldByName('MEMBER').AsInteger     := StrToIntDef(GetCommaDelimitedField(2,s),0);
                 TempStr := GetCommaDelimitedField(3,s); 
                 if TempStr <> '' then
                    FieldByName('POINTS').AsFloat    := StrToFloat(TempStr);
                 TempStr := GetCommaDelimitedField(4,s); 
                 if TempStr <> '' then
                    FieldByName('TENDER').AsFloat    := StrToFloat(TempStr);
                 FieldByName('TENDER_NAME').AsString := GetCommaDelimitedField(5,s);
                 TempStr := GetCommaDelimitedField(6,s); 
                 if TempStr <> '' then
                    FieldByName('VALUE').AsFloat    := StrToFloat(TempStr);
                 FieldByName('PAID_BY').AsInteger   := StrToIntDef(GetCommaDelimitedField(7,s),0);
                 // palm North - subs come thru as '----'
                 s1 := GetCommaDelimitedField(8,s);
                // try
                 //   FieldByName('SUBS').AsInteger :=  strToInt(s1);
                 //except
                    FieldByName('SUBS').AsInteger := 0;
                // end;
                 Post;
              end;
           finally
              close;               
              closeFile(f);               
              free;
           end;
        end;     
        {dm.sqlRollover.SQL.TEXT := 'Insert into IMPORT_AREV values(:TYPE,:INVOICE,:MEMBER,:POINTS,:TENDER,:TENDER_NAME,:VALUE,:PAID_BY,:SUBS)';
        DelimInput := TIBInputDelimitedFile.Create;
        DelimInput.FileName := Global.ArevImportFilePath + '\' + ImportList[rgpImport.ItemIndex];
        DelimInput.ColDelimiter := ',';
        DelimInput.ReadBlanksAsNull := True;
        dm.sqlRollover.BatchInput(DelimInput);}
        // fill Invoice where blank - on the last line ZZZZ - need if selecting on
        dm.sqlExecQuery(dm.sqlRollover,SQL1);
        dm.IBRollover.CommitRetaining;
     finally
        //DelimInput.Free;
     end;  
end;
{ **************************************************************************** }
{function TfrmMembersSYS.GetCommaDelimitedField(FieldNum : shortint ; s : String) : String ;
const comma = ',';
var FieldStart,FieldEnd,FieldCount,i : smallint;
// 0 is the first field 
begin         
     result := '';
     FieldStart := 0;
     FieldEnd   := 0;
     FieldCount := 0;
     // move to the FieldNum comma to get StartField
     if FieldNum = 0 then
        FieldStart := 1
     else begin   
        for i := 1 to length(s) do begin
            if s[i] = comma then inc(FieldCount);
            if FieldCount = FieldNum then begin
               FieldStart := i;
               break;
            end;   
         end;   
     end;
     // trim off up to the first comma
     if FieldStart <> 1 then
        system.delete(s,1,FieldStart);
     FieldEnd := pos(comma,s);
     if FieldEnd = 0 then
        // must be the last field - no comma found
        FieldEnd := length(s)
     else 
        // place -1 off the comma
        FieldEnd := FieldEnd -1;
     // not data in between 
     if FieldEnd = 0 then begin
        result := '';
        exit;
     end;   
     Result := copy(s,1,FieldEnd);   
end;
{ **************************************************************************** }
{ **************************************************************************** }
procedure TfrmMembersSYS.UpdateMemberPoints;      
{const SQL1 = 'update MEMBERS set POINTS_AVAILABLE = (POINTS_AVAILABLE ';
      SQL2 = ' - %f), TOTAL_REDEEMED = (TOTAL_REDEEMED + %f)';
      SQL3 = ' + %f), TOTAL_EARNT = (TOTAL_EARNT + %f)';
      SQL4 = ' where MEMBER = %d ';
}      
{const SQL1 = 'update MEMBERS set POINTS_AVAILABLE = %f,TOTAL_REDEEMED = %f,' +
             'TOTAL_EARNT = %f,CARD_LAST_USED = CURRENT_DATE where MEMBER = %d';
{ piPointsSpent - SPNT has always ERNT of the same trans before it
  6 points earnt( 6 cents ) from the sale then 200 ($2.00) points spent
  NB 200 is in the Tender Column
  ERNT,2372913,57000,6,,,57000,
  SPNT,2372913,57000,,200,,57000,
}     
begin {
     FHoldSPNT := FHoldSPNT + FTender;
     FHoldERNT := FHoldERNT + FPointsDollar;
     case PrefixImportTypes of 
        piPointsEarnt : begin 
                           FTotalEarnt :=  FTotalEarnt + FPointsDollar;
                           FPointsAvailable := FPointsAvailable + FPointsDollar;
                           dm.sqlExecQuery(dm.sqlRollover,format(SQL1,[FPointsAvailable,FTotalRedeemed,FTotalEarnt,FMember]));
                        end;  
        piPointsSpent : begin
                           // NB SPNT is in the next column
                           FTotalRedeemed :=  FTotalRedeemed + FTender;
                           FPointsAvailable := FPointsAvailable - FTender;
                           dm.sqlExecQuery(dm.sqlRollover,format(SQL1,[FPointsAvailable,FTotalRedeemed,FTotalEarnt,FMember]));
                        end;   
        piSubsPaid,piJoinFeePaid : 
                        begin
                           FTotalRedeemed :=  FTotalRedeemed + FPointsDollar;
                           FPointsAvailable := FPointsAvailable - FPointsDollar;
                           dm.sqlExecQuery(dm.sqlRollover,format(SQL1,[FPointsAvailable,FTotalRedeemed,FTotalEarnt,FMember]));
                        end;   
     end;}
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.UpdateMemberCredit; 
//const SQL1 = 'update MEMBERS set VALUE_OF_CREDIT = %f where MEMBER = %d';
//const SQL1 = 'update MEMBERS set POINTS_AVAILABLE = %f,TOTAL_REDEEMED = %f,' +
//             'TOTAL_EARNT = %f,VALUE_OF_CREDIT = %f,CARD_LAST_USED = CURRENT_DATE where MEMBER = %d';
// Member Credit adds onto points available + earnt
//var MemberPayments : TMemberPayments;
begin
     //MemberPayments.PointsCreditUpdate(FTender,dm.dsrMemberPoints.DataSet) then begin

     //dm.sqlExecQuery(dm.SQLRollover,format(SQL1,[FTender,FMember]));
     //FTotalEarnt :=  FTotalEarnt + FTender;
     //FPointsAvailable := FPointsAvailable + FTender;
     //dm.sqlExecQuery(dm.sqlRollover,format(SQL1,[FPointsAvailable,FTotalRedeemed,FTotalEarnt,FTender,FMember]));
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.UpdateLuckyMember; 
const SQL1 = 'UPDATE MEMBERS set LUCKY_DATE = ''%s'' where MEMBER = %d';
begin
     dm.sqlExecQuery(dm.SQLRollover,format(SQL1,[GetShortDate(date -1),FMember]));
end;
{ **************************************************************************** }
function TfrmMembersSYS.UpdateSubsPaid : Boolean;
var Paid : char;
    SavePointsCreditTypes : TPointsCreditTypes;
    FYr, FMnth, FDay: Word;
    EYr, EMnth, EDay: Word;
    ThisYearExpiryDate : TDateTime;
const SQL3 = 'UPDATE M_SUBS Set AMOUNT_PAID = %f,AMOUNT = %f,' +
             'PAID = ''%s'',EDIT_DATE = ''%s'',RECEIPT = %d,PAY = %f ' +
             'where MEMBER = %d and SUBSCRIPTION = %d';   
begin
      //VIP PAY    or CASH   ????  
      //  make the decision on points or cash depending on the Name 
      Result := False;
      FReceiptNumber := MemberPayments.GetReceiptNumber(dm.sqlRollover);
      //dm.IBRollover.commit;
      if not GetNewAmounts then exit;
      FAmountPaid := FAmountPaid + FPointsDollar + FTender;
      if CompName = 'Makerewa Country Club' then begin
         // Amount Paid  >  Amount ( 55 Sub + 30 Join Fee) 
         if (FAmountPaid > FAmount) then begin
            FAmount := FAmountPaid;
         end;
      end;
      Paid := 'N';
      SubPayTypes := MemberPayments.PaymentSubPayTypes(FAmount,FAmountPaid);
      if SubPayTypes = spFull then Paid := 'T';
      PayTypes := ptSubs;
      // Payments Detail      
      MemberPayments.PaymentDetailInsert( dm.sqlRollover,FReceiptNumber,FMember,FMember,
                             FSubs,ord(SubPayTypes),ord(PayTypes),FAmount,FAmountPaid,0.00,
                             Date - 1,FDescription,'N',GetShortDate(FExpires),FName,'' );
      // Payments Tend  - FTender is not mapped onto tblTenders
      // if Blank then assume a Cash Payment
      if FTenderName = '' then FTenderName := 'Cash';
      MemberPayments.PaymentTenderInsert( dm.sqlRollover,FReceiptNumber,
                                   FTenderName,'Subs Paid at POS',FTender + FPointsDollar,0.00);
      // update M_SUBS
      dm.sqlExecQuery(dm.sqlRollover,format(SQL3,[FAmountPaid,FAmount,Paid,GetShortDateTime(now),FReceiptNumber,(FTender + FPointsDollar),FMember,FSubs]));
      // update points if applicable
      if FPointsDollar <> 0.00 then begin
         dm.ibdsMemberPoints.ParamByName('MEMBER').AsInteger := FMember;
         dm.ibdsMemberPoints.Open;
         dm.ibdsMemberPoints.Edit;
         MemberPayments.PointsCreditUpdate(FPointsDollar,dm.dsrMemberPoints.DataSet);
         dm.ibdsMemberPoints.Post;
         MemberPayments.LogPointsCredit(dm.IBRollover,format('Subs Paid POS for Member %d',[FMember]),
              BeforeEditPointsCredit,dm.dsrMemberPoints.DataSet,FPointsDollar);
         dm.ibdsMemberPoints.Close;
      end;   
      {if CompName = 'Makerewa Club' then begin
         // new MEMBER is N 
         if (FNewMember = 'N') then begin
            // set SUB_PAYMENT to 'Y' - done in UpdateSubsByPayment
         end else begin
            DecodeDate(now, FYr, FMnth, FDay);
            DecodeDate(MemberExpiry, EYr, EMnth, EDay);
            // work out Expires for this Year - in case they have done a rollover
            ThisYearExpiryDate := EncodeDate(FYr, EMnth, EDay);
            // only if the date is between 1 Oct and 31 Nov 
            if (Date >= (ThisYearExpiryDate - 29)) and (Date <= (ThisYearExpiryDate + 31)) then begin
               SavePointsCreditTypes := PointsCreditTypes;
               PointsCreditTypes := pcPointsEarnt;
               dm.ibdsMemberPoints.ParamByName('MEMBER').AsInteger := FMember;
               dm.ibdsMemberPoints.Open;
               Global.BeforeEditPointsCreditSave(BeforeEditPointsCredit,dm.dsrMemberPoints.DataSet);  
               dm.ibdsMemberPoints.Edit;
              // allocate $10 of points to Members
               MemberPayments.PointsCreditUpdate(10.00,dm.dsrMemberPoints.DataSet);
               dm.ibdsMemberPoints.Post;
               dm.ibdsMemberPoints.Close;
               dm.ibdsMemberPoints.Open; // Refresh the latest points
               MemberPayments.LogPointsCredit(dm.IBRollover,format('$10 Early Payment for Member %d',[FMember]),
                    BeforeEditPointsCredit,dm.dsrMemberPoints.DataSet,FPointsDollar);
               dm.ibdsMemberPoints.Close;
               PointsCreditTypes := SavePointsCreditTypes;
               // turn from dollar to integer
               dm.LogExport(dm.sqlRollover,FMember,1000,False);
            end;
         end;
      end; }
      Result := true;
end;
{ **************************************************************************** }      
function TfrmMembersSYS.UpdateSubsByPayment : Boolean;
const SQL1 = 'SELECT EXPIRES from MEMBERS WHERE MEMBER = %s';
      SQL2 = 'SELECT SUM(AMOUNT) AS AMOUNT,SUM(AMOUNT_PAID) AS AMOUNT_PAID,' +
             ' (SUM(AMOUNT) - SUM(AMOUNT_PAID)) AS DUE,SUM(PAY) as PAY FROM M_SUBS WHERE MEMBER = %s';
      SQL3 = 'UPDATE PAYMENTS_DET SET EXPIRES = ''%s'' where MEMBER = %s and PAYMENT = %d ' +
             ' and PAY_TYPE = %d';
      //SQL4 = 'UPDATE MEMBERS SET SUB_PAID = %d,EDIT_DATE = ''%s'',EXPIRES = ''%s'',IS_ACTIVE = ''Y'',' +
      SQL4 = 'UPDATE MEMBERS SET SUB_PAID = %d,EDIT_DATE = ''%s'',EXPIRES = ''%s'',' +
             'INACTIVE_DATE = null,CARD_LAST_USED = CURRENT_DATE,SUB_PAYMENT = ''Y'' where MEMBER = %s'; 
      SQL5 = 'UPDATE MEMBERS SET SUB_PAID = %d,EDIT_DATE = ''%s'',EXPIRES = ''%s'',' +
             'CARD_LAST_USED = CURRENT_DATE,SUB_PAYMENT = ''Y'' where MEMBER = %s';
      SQL6 = 'SELECT MAX(TERM)as TERM from SUBSCRIPTIONS s, M_SUBS ms where ms.SUBSCRIPTION = ' +
             's.SUBSCRIPTION and ms.MEMBER = %s';
      
var Yrs,SubsTerm : ShortInt;
    Amount,PAY : Extended;
    AmountPaid : Extended;
    MExpires   : TDateTime;
    i : integer;
    CardMembers : TCardMembers;
    //SubPayTypes  : TSubPayTypes;// = (spZero,spPart,spFull);
begin
     Result := False;
     if MemberList.Count = 0 then begin // no subs payments
        Result := True;
        Exit;
     end;   
     for i := 0 to MemberList.Count -1 do begin
        if MemberList[i] = '' then continue;
        with dm.sqlRollover do begin
           if dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL1,[MemberList[i]])) then  
              MExpires := FieldByName('EXPIRES').AsDateTime
           else  // not found
              exit;
           // must make sure MExpires is bought up to date if it is behind
           // may have missed paying a year or AREV to Delphi ChangeOver
           if MExpires < (Global.MemberExpiry - 365) then
              MExpires := Global.MemberExpiry - 365; 
           if dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL2,[MemberList[i]])) then begin 
              // if a + or - payment - then the AMOUNT_PAID would be reflected OK
              Amount     := FieldByName('AMOUNT').AsFloat;
              AmountPaid := FieldByName('AMOUNT_PAID').AsFloat;
              PAY        := FieldByName('PAY').AsFloat;
              SubPayTypes := MemberPayments.PaymentSubPayTypes(Amount,AmountPaid);
              // get the max TERM from Subscripions for the MEMBER
              // if a Member has a 3 month and 12 month sub - they get a 12 month period
              dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL6,[MemberList[i]]));
              SubsTerm := dm.sqlRollover.FieldByName('TERM').AsInteger;
               //Work Out Expires - no skip years from the POS - all existing Members
              MExpires := MemberPayments.PaymentWorkOutExpires(Amount,AmountPaid,Pay,
                                                    SubPayTypes,MExpires,FGoodDAte,0,SubsTerm);
           end;
           // Set Payments_Det EXPIRES to MExpires 
           dm.sqlExecQuery(dm.sqlRollover,format(SQL3,[GetShortDate(MExpires),MemberList[i],FReceiptNumber,ord(TPayTypes(ptSubs))]));
           if SubPayTypes = spFull then begin
              // make Financial
              dm.sqlExecQuery(dm.sqlRollover,format(SQL4,[Ord(SubPayTypes),GetShortDate(now),GetShortDate(MExpires),MemberList[i]]));
              // get a Card if required
              if Global.POSPaymentCardReqd then
                 CardMembers.CardInsertMember( dm.sqlRollover, StrToInt(MemberList[i]),1);
           end else 
              // else let the rollover handle the IS_ACTIVE
              dm.sqlExecQuery(dm.sqlRollover,format(SQL5,[Ord(SubPayTypes),GetShortDate(now),GetShortDate(MExpires),MemberList[i]]));
        end; // with
         Result := True;
        MemberList.Clear;
     end; // for
     
end;
{ **************************************************************************** }
function TfrmMembersSYS.GetNewAmounts : Boolean;
const SQL1 = 'select M.SUB_PAID,M.EXPIRES,M.FULL_NAME,MS.AMOUNT_PAID,MS.AMOUNT, ' +
             'S.DESCRIPTION,M.SUB_PAYMENT from MEMBERS M ,M_SUBS MS , SUBSCRIPTIONS S ' +
             'where M.MEMBER = %d and M.MEMBER = MS.MEMBER and MS.SUBSCRIPTION = S.SUBSCRIPTION ' +
             'and MS.AMOUNT > 0.00 and MS.SUBSCRIPTION = %d';
begin 
      // POS subs dont match
      if dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL1,[FMember,FSubs])) then begin
         Result := True;
         with dm.sqlRollover do begin
            FSubPaid     := FieldByName('SUB_PAID').AsInteger;
            FExpires     := FieldByName('EXPIRES').AsDateTime;
            FName        := FieldByName('FULL_NAME').AsString;
            FDescription := FieldByName('DESCRIPTION').AsString; 
            FNewMember   := FieldByName('SUB_PAYMENT').AsString;
            FAmount := GetLatestFee(SubsTableName,FieldNameArray[0],FSubs);
            FAmountPaid := 0.00;
            //FAmountPaid  := FieldByName('AMOUNT_PAID').AsFloat;
            //FAmount      := FieldByName('AMOUNT').AsFloat; 
            //if (dm.sqlRollover.FieldByName('EXPIRES').AsDateTime <= FGoodDate) then begin
               // paid in advance 
               {if FAmountPaid >= (FAmount * 2) then begin
                  FAmountPaid := FAmountPaid - FAmount;
                  FAmount     := FieldByName('AMOUNT').AsFloat; // keep same price
               end else begin
                  // get latest price Subscriptions
                  FAmount := GetLatestFee(SubsTableName,FieldNameArray[0],FieldByName('SUBSCRIPTION').AsInteger);
                  if FAmount = 0.00 then 
                     FAmount := FieldByName('AMOUNT').AsFloat; // keep same price
                  if FSubPaid = 2 then  // paid in advance - part of a year but past EXPIRES
                     FAmountPaid := FAmountPaid - FAmount
                  else 
                     FAmountPaid := 0.00; // part payments are written off at each payment
               end; }
           // end;
         end;
      end else begin     
         sm(Format('Member %d has the wrong Subscription - on the POS it is %d',[FMember,FSubs]));
         Result := False
      end;
end;
{ **************************************************************************** }
function TfrmMembersSYS.CheckTotalLines : Boolean;
var   SumOfPoints,TablePoints : Extended;
const msg1 = 'Invalid number of Points in the POS Import file';
      SQL1 = 'Select Sum(POINTS_AVAILABLE) AS POINTS,Sum(Total_Earnt) AS Earnt,Sum(Total_redeemed) AS Red from MEMBERS';
      msg2 = 'Sum of Points in the POS Import file is not Numeric';
begin
     Result := False;
     try
        // Points Count is in the last field 1000 = $10.00
        SumOfPoints := StrTOFloat(dm.qryGeneral.FieldByName('INVOICE').AsString);
        SumOfPoints := SumOfPoints / 100; 
     except
        sm(msg2);
        exit;
     end;                     
     //sm('Spent ' + FloatToStr(FHoldSPNT) + '   ' + 'Earnt  ' + FloatToStr(FHoldERNT));
     if dm.sqlExecQueryCheckEOF(dm.sqlRollover,SQL1) then begin
        //sop := 0.00;
        //sop := dm.sqlRollover.FieldByName('POINTS').AsFloat;
        //sop := dm.sqlRollover.FieldByName('Earnt').AsFloat;
        //sop := dm.sqlRollover.FieldByName('red').AsFloat;
        //if Trunc(dm.sqlRollover.FieldByName('POINTS').AsFloat) = SumOfPoints then
        TablePoints :=  dm.sqlRollover.FieldByName('POINTS').AsFloat;
        //sm('TablePoints ' + FloatToStr(TablePoints) + '   ' + 'SumOfPoints  ' + FloatToStr(SumOfPoints));
        if ( SumOfPoints > (TablePoints - 60) ) and ( SumOfPoints < (TablePoints + 60) ) then begin 
        //if dm.sqlRollover.FieldByName('POINTS').AsFloat = SumOfPoints then begin
       //    sm(format('Points Earnt = %f , Points Spent = %f ',[FHoldERNT,FHoldSPNT]));
           Result := True;
        end else                    
           sm(msg1 + HelpDeskMsg + HelpDesk);            
     end;      
end;
{ **************************************************************************** }
function TfrmMembersSYS.GetLatestFee(const tblSubs,SubsName : shortstring ; SubsInt : Integer) : Extended;
const SQL1 = 'SELECT FEE FROM %s where %s = %d';
begin
     Result := 0.00;
     if dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL1,[tblSubs,SubsName,SubsInt])) then 
        result := dm.sqlRollover.FieldByName('FEE').AsFloat;
end;
{ **************************************************************************** }
{function TfrmMembersSYS.GetReceiptNumber : LongInt ;
var Present : TDateTime;
    Hour, Min, Sec, MSec: Word;
const SQL1 = 'INSERT INTO PAYMENTS (EDIT_DATE,PAID_BY) VALUES (''%s'',%d)';    
      SQL2 = 'SELECT PAYMENT FROM PAYMENTS WHERE EDIT_DATE = ''%s'' AND PAID_BY = %d';
begin
     Result := 0;
     Present := Now;// try to make a unique entry
     DecodeTime(Present, Hour, Min, Sec, MSec);
     dm.sqlExecQuery(dm.sqlRollover,format(SQL1,[GetShortDate(Present),MSec + Sec + Min]));
     if dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL2,[GetShortDate(Present),MSec + Sec + Min])) then 
        Result := dm.sqlRollover.FieldByName('PAYMENT').AsInteger;
end;
{ **************************************************************************** }
function TfrmMembersSYS.UpdatePayments : Boolean;
var s : string[15];
    FullName : string[25];
const SQL1 = 'UPDATE PAYMENTS SET PAY_AMOUNT = %f,POINTS_USED = %f,PAID_BY = %d,' +
        'DISCOUNT = 0.00,EDIT_DATE = ''%s'',ROUNDING = %f,PAY_CREDIT = 0.00,FULL_NAME = ' +
         '''%s'' where PAYMENT = %d';
      SQL2 = ' Select FULL_NAME from MEMBERS where MEMBER = %d';   
begin
     // NB the PAYMENT autoinc has already been inserted to get FReceiptNumber prior to this
     Result := False;
     if not dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL2,[FPaidByMember])) then begin
        sm(format('Cant find Paid by Member %s %s',[FPaidByMember,CantContinueMsg]));
        exit; 
     end else
        FullName := dm.sqlRollover.FieldByName('FULL_NAME').AsString;
     dm.sqlExecQuery(dm.sqlRollover,format(SQL1,[FHoldTender,FHoldPoints,FPaidByMember,GetShortDate(Date -1),0.00,FullName,FReceiptNumber]));
     FHoldPoints := 0.00;
     FHoldTender := 0.00;
     Result := True;
end;
{ **************************************************************************** }
function TfrmMembersSYS.UpdateJoinFee : Boolean;
const SQL2 = 'SELECT JOINING_FEE,FULL_NAME,EXPIRES FROM MEMBERS WHERE MEMBER = %d';        
begin
     Result := False;
     if not dm.sqlExecQueryCheckEOF(dm.sqlRollover,format(SQL2,[FMember])) then begin
        sm(format('No such Member %d to Pay the Joining Fee',[FMember]));
        exit;
     end;   
     PayTypes := ptJoinFee;
     SubPayTypes := MemberPayments.PaymentSubPayTypes(dm.sqlRollover.FieldByName('JOINING_FEE').AsFloat,FTender + FPointsDollar);
     MemberPayments.PaymentDetailInsert( dm.sqlRollover,FReceiptNumber,FMember,FMember,
                             FSubs,ord(SubPayTypes),ord(PayTypes),dm.sqlRollover.FieldByName('JOINING_FEE').AsFloat,
                             FTender + FPoints,0.00,
                             Date - 1,'Join Fee','N',GetShortmonthDAte(dm.sqlRollover.FieldByName('EXPIRES').AsDateTime),
                             dm.sqlRollover.FieldByName('FULL_NAME').AsString,'' );
     if FTenderName = '' then FTenderName := 'Cash';
     MemberPayments.PaymentTenderInsert( dm.sqlRollover,FReceiptNumber,
                                   FTenderName,'Join Fee Paid at POS',(FTender + FPointsDollar),0.00);
     //if FPoints <> 0.00 then UpdateMemberPoints; 
     // update points if applicable
     if FPoints <> 0.00 then begin
        MemberPayments.PointsCreditUpdate(FPointsDollar,dm.dsrMemberPoints.DataSet);
        MemberPayments.LogPointsCredit(dm.IBRollover,format('Join Fee Paid POS for Member %d',[FMember]),
             BeforeEditPointsCredit,dm.dsrMemberPoints.DataSet,FPointsDollar);
     end;   
     Result := True; 
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.rgpOtherChange(Sender: TObject; ButtonIndex: Integer);
const Gtab = #9;  
begin
     dbgOther.Hint := '';
     dbgOther.ReadOnly := False;
     dbgOther.PopupMenu := nil;
     if dm.dsrTblPrinters.State = dsEdit then dm.tblPrinters.Post; // Printers
     if dm.dsrScreens.State     = dsEdit then dm.tblScreens.Post;  // Screen Def
     if dm.dsrTblVersions.State = dsEdit then dm.tblVersions.Post; // Versions
     case rgpOther.ItemIndex of
        0 : begin     // Printers  - has a combo box in Printer_name
               with dbgOther do begin
   	           Selected.Clear;
   	           Selected.Add('DESCRIPTION' + GTab + '15' + GTab + 'Description');
   	           Selected.Add('PRINTER_NAME' + GTab + '35' + GTab + 'Printer Name');
      	           Selected.Add('PRINTER_INDEX' + GTab + '6' + GTab + 'Index');
      	           //Selected.Add('PC_NAME' + GTab + '16' + GTab + 'PC Name');
      	           Selected.Add('EMULATION' + GTab + '10' + GTab + 'Emulation');
      	           Selected.Add('COMM_PORT' + GTab + '10' + GTab + 'Comm Port');
      	           ApplySelected;
                   DataSource    := dm.dsrTblPrinters;
                   dbnNavRefresh.DataSource := DataSource;
                   SetFocus;
                   SetActiveField('PRINTER_NAME');
               end;
            end;
        1 : begin // Screen Def
               with dbgOther do begin
   	           Selected.Clear;
   	           Selected.Add('SCREEN_CODE' + GTab + '11'  + GTab + 'Screen Code');
      	        Selected.Add('SCREEN_DESC' + GTab + '60' + GTab + 'Screen Desc');
      	        Selected.Add('DEF_ACCESS'  + GTab + '6'  + GTab + 'Def Access');
      	        ApplySelected;
                 DataSource := dm.dsrScreens;
                 dbnNavRefresh.DataSource := DataSource;
                 if not Global.regLogon then begin
                    Hint := dbgHint; // cant modify
                    ReadOnly := True;
                 end;
               end;
            end;
        2 : begin     // Version Control
               with dbgOther do begin
   	           Selected.Clear;
   	           Selected.Add('PC_NAME' + GTab + '11' + GTab + 'PC Name');
   	           Selected.Add('VERSION' + GTab + '13' + GTab + 'Version');
   	           Selected.Add('INSTALL_DIR' + GTab + '40' + GTab + 'Installed Directory');
   	           Selected.Add('SHARE_NAME' + GTab + '10' + GTab + 'Share Name');
      	        ApplySelected;
                 DataSource := dm.dsrTblVersions;
                 dbnNavRefresh.DataSource := DataSource;
                 SetFocus;
                 SetActiveField('SHARE_NAME');
                 PopupMenu := popMenu;
                 Hint := 'Right Click on the Mouse to see a menu'; // cant modify
               end;
            end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.cbPrintersCloseUp(Sender: TwwDBComboBox; Select: Boolean);
begin
     //if cbPrinters.Modified then begin
        with dm.tblPrinters do begin
           Edit;
           FieldByName('PRINTER_INDEX').AsInteger := cbPrinters.ItemIndex;
           //dbgOther.SetActiveField('EMULATION');
           FieldByName('COMM_PORT').Clear;
           Post;
           //SetProcessType;
           //dm.LogProcess(dm.qryGeneral, 'Printer - "' + FieldByName('DESCRIPTION').AsString + '" Changed to ' + FieldByName('PRINTER_NAME').AsString);
           dm.IBCommitRetain;
        end;
     //end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dbgOtherExit(Sender: TObject);
begin
     case rgpOther.ItemIndex of
        0 : if dm.dsrTblPrinters.State = dsEdit then dm.tblPrinters.Post; // Printers
        1 : if dm.dsrScreens.State = dsEdit then dm.tblScreens.Post; // Screen Def
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.ApplicationtoallClick(Sender: TObject);
begin
     if Busy then 
        exit
     else 
        Busy := True;   
     try
       TooBusy(CopyFilesMsg + PleaseWaitMsg);
       with dm.tblVersions do begin 
          DisableControls;
          First;
          while not EOF do begin
             // dont send to self
             if FieldByName('PC_NAME').AsString = ReadComputerName then begin
                next;
                continue; 
             end;   
             MakeSendDirectory;
             FSuccess := copyfiles(application.exename,FToComputer + FExeName,false);
             ShowMsg;
             next;
          end;
       end;
     finally
       dm.tblVersions.EnableControls;
       NotBusy;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.ClearAllUsers1Click(Sender: TObject);
const SQL1 = 'Delete from VERSIONS';
// clear all entries from the table - and re log on 
begin
     dm.sqlExecQuery(dm.qryGeneral,SQL1);
     SetProcessType;
     dm.LogProcess(dm.qryGeneral, 'VERSIONS - all User logins deleted');
     dm.IBCommitRetain;
     TblActiveOrRefresh(dm.tblVersions);
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.Applicationto1Click(Sender: TObject);
begin
     if Busy then 
        exit
     else 
        Busy := True;   
     if dm.tblVersions.FieldByName('PC_NAME').AsString = ReadComputerName then begin
        Busy := False;   
        sm(CantSentToSelfMsg);
        exit;
     end;
     try
        TooBusy(CopyFilesMsg + PleaseWaitMsg);
        MakeSendDirectory;
        FSuccess := copyfiles(application.exename,FToComputer + FExeName,false);
        ShowMsg;
     finally
        NotBusy;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.MakeSendDirectory;
begin
     with dm.tblVersions do begin
        FInstallDir := Trim(FieldByName('INSTALL_DIR').AsString);
        if length( FInstallDir ) = 3 then // c:\
           FInstallDir := ''
        else   
           FInstallDir := copy(FInstallDir,3,Length(FInstallDir));
        FToComputer := '\\' + FieldByName('PC_NAME').AsString + BackSlash + 
           FieldByName('SHARE_NAME').AsString  + FInstallDir;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.HelpTo1Click(Sender: TObject);
var i,cnt : shortint;
    HelpList : TStringList;
// have to copy each help file - cant use *.doc    
begin   
     if Busy then 
        exit
     else 
        Busy := True;   
     if dm.tblVersions.FieldByName('PC_NAME').AsString = ReadComputerName then begin
        sm(CantSentToSelfMsg);
        Busy := False;   
        exit;
     end;
     try
        TooBusy(CopyFilesMsg + PleaseWaitMsg);
        HelpList := TStringList.Create;
        GetFiles(GetCurrentDir + FHelpFileName,HelpList);
        if HelpList.Count = 0 then begin
           sm(NoHelpMsg);
           exit;
        end;
        MakeSendDirectory;
        cnt := 0;
        for i := 0 to HelpList.Count -1 do begin
           if copyfiles(GetCurrentDir + BackSlash + HelpList[i],FToComputer + BackSlash + HelpList[i],false) then
              inc(cnt);
        end;
        if cnt = HelpList.Count then
           FSuccess := true
        else   
           FSuccess := false;
        NotBusy;
        ShowMsg; 
     finally
        NotBusy;
        HelpList.Free;
     end;
end; 
{ **************************************************************************** }
procedure TfrmMembersSYS.ShowMsg;
begin
     case FSuccess of 
        True : MdlgI(format(SendSuccessMsg,[dm.tblVersions.FieldByName('PC_NAME').AsString]));
        False : MdlgI(format(SendUnSuccessMsg,[dm.tblVersions.FieldByName('PC_NAME').AsString,dm.tblVersions.FieldByName('PC_NAME').AsString]));
     end;
end;

{ **************************************************************************** }
procedure TfrmMembersSYS.HelptoallClick(Sender: TObject);
var i,cnt : shortint;
    HelpList : TStringList;
// have to copy each help file - cant use *.doc    
begin
     if Busy then 
        exit
     else 
        Busy := True;   
     try
       TooBusy(CopyFilesMsg + PleaseWaitMsg);
       HelpList := TStringList.Create;
       GetFiles(GetCurrentDir + FHelpFileName,HelpList);
       if HelpList.Count = 0 then begin
          sm(NoHelpMsg);
          exit;
       end;
       with dm.tblVersions do begin 
          DisableControls;
          First;
          while not EOF do begin
             // dont send to self
             if FieldByName('PC_NAME').AsString = ReadComputerName then begin
                Next;
                continue;
             end;    
             MakeSendDirectory;
             cnt := 0;
             for i := 0 to HelpList.Count -1 do begin
                if copyfiles(GetCurrentDir + BackSlash + HelpList[i],FToComputer + BackSlash + HelpList[i],false) then
                   inc(cnt);
             end;   
             if cnt = HelpList.Count then
                FSuccess := true
             else   
                FSuccess := false;
             ShowMsg;
             next;
          end;
       end;
     finally
        HelpList.Free;
        dm.tblVersions.EnableControls;
        NotBusy;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dbgUserSecurityCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;
  ABrush: TBrush);
begin
     if Field.FieldName = 'HAS_ACCESS' then ABrush.color := fcGridEdit;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dbgOtherCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;
  ABrush: TBrush);
begin
     if (Field.FieldName = 'PRINTER_NAME') or
        (Field.FieldName = 'DEF_ACCESS') or 
        (Field.FieldName = 'INSTALL_DIR') or 
        (Field.FieldName = 'PROGRAM') or 
        (Field.FieldName = 'EMULATION') or
        (Field.FieldName = 'COMM_PORT') or
        (Field.FieldName = 'SHARE_NAME') then
        ABrush.Color := Global.fcGridEdit;                                                                                                                                                                                                                                                   
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
var tabsheet : tTabSheet;  
begin
     inherited;
     case key of   
         VK_ESCAPE : menuExitClick(Sender);
         ///VK_F7 : updatedates;
     end; 
     if ( ssAlt in shift ) then begin
        case key of 
           85,117 : tabsheet := tsUsersSecurity; // U    
           83,115 : tabsheet := tsSite;         // S
           73,105 : tabsheet := tsInterface;    // I
           76,108 : tabsheet := tsUserLog;      // L
           66,98  : tabsheet := tsBackup;       // B
           79,111 : tabsheet := tsOther;        // O
           68,100 : tabsheet := tsDaily;        // D
           88,120 : menuExitClick(Sender);      // x Close 
        end;  
        if tabsheet.TabVisible then PageControl.ActivePage := tabsheet;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.DOB;
var DOB,s,tmpDate,FGoodDateStr : string;
    bGoodDate : boolean;
    dt : TDateTime;
    MEMBER : LongInt;
    century,i : Integer;

begin
     if MessageDlg('Update the DOB from AREV',mtConfirmation,[mbYes,mbNo],0) = mrNo then exit;
     dm.StartTrans(dm.IBRunOnce);
     dm.sqlExecQuery(dm.sqlRunOnce,'select MEMBER,DATE_OF_BIRTH from arev where DATE_OF_BIRTH is not null');
     with dm.sqlRunOnce do begin
        screen.cursor := crHourGlass;
        s := '';
        i := 0;
        try
           while not EOF do begin
              DOB := FieldByName('DATE_OF_BIRTH').AsString;
              MEMBER := FieldByName('MEMBER').AsInteger;
              tmpDate := '';
              if POS('/',DOB) > 0 then begin
                 // dont worry about / in date
              end else begin
                 tmpDate := '';
                 DOB := trim(DOB);
                 if DOB <> '' then begin
                    tmpDate := copy(DOB,1,2) + '/' + copy(DOB,3,2)  + '/19' + copy(DOB,5,2);

                 //end;
                   //Showmessage(tmpDate);
                 end;
              end;
              //
              if tmpDate <> '' then begin
                 try
                    dt := StrToDateTime(tmpDate);
                    tmpDate := GetShortDAte(dt);
                    dm.sqlExecQuery(dm.qryGeneral,format('update MEMBERS set DATE_OF_BIRTH = ''%s'' ' +
                      'where MEMBER = %d',[tmpDate,MEMBER]));
                    dm.sqlExecQuery(dm.qryGeneral,format('update M_EVENTS set DATES = ''%s'' ' +
                      'where MEMBER = %d',[tmpDate,MEMBER]));
                    inc(i);
                 except
                    s := s + FieldByNAme('MEMBER').ASString + '  ' + tmpDate + ',';
                 end;
              end;
              next;
           end;
           try
              dm.IBTransaction.CommitRetaining;
              sm('OK');
           except
              dm.IBTransaction.Rollback;
              sm('no worky');
           end;
        finally
           if s <> '' then
              showmessage(s);
           showmessage(IntToStr(i) + ' Members adjusted');

           screen.cursor := crDefault;
        end;
     end;


end;
{ **************************************************************************** }
{function TfrmMembersSYS.DayMonthYearStr( str : string ) : String;
//  05 DEC 2001
const
   MonthsArray : Array[1..12] of String[3]
             = ( 'JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC');
var s : string[3];
    y : string[8];
    j : shortint;
begin                   
     s := copy(str,4,3);
     for j := 1 to 12 do 
         if s = MonthsArray[j] then break; 
     s := IntToStr(j);
     if Length(s) = 1 then
        s := '0' + s;
     y := copy(str,8,4);
     if Length(y) = 2 then  // 2 digit year
        y := '19' + y;
     Result := copy(str,1,2) + '/' + s + '/' + y;
end;
{ **************************************************************************** }
{function TfrmMembersSYS.DayMonthYearFunny( str : string ) : String;
//  
const
   MonthsArray : Array[1..12] of String[5]
             = ( 'JAN','FEB','MARCH','APRIL','MAY','JUNE','JULY','AUG','SEPT','OCT','NOV','DEC');
   MonthsArray1 : Array[1..12] of String[3]
             = ( 'JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC');
             
var s,FYear : string[8];
    FDays,FMonth : string[4];
    j : shortint;
begin                   
     FDays := copy(str,1,POS(' ',Str) -1);
     if Length(FDays) = 1 then
        FDays := '0' + FDays;
     system.Delete(Str,1,POS(' ',Str));   

     s := copy(str,1,POS(' ',Str) -1);
     for j := 1 to 12 do 
         if s = MonthsArray1[j] then break; 
     // check in the next array
     if j = 13 then 
        for j := 1 to 12 do 
           if s = MonthsArray[j] then break; 

     FMonth :=  IntToStr(j);
     if Length(FMonth) = 1 then
        FMonth := '0' + FMonth;
     
     system.Delete(Str,1,POS(' ',Str)); 
     FYear := Str;   
     if Length(FYear) = 2 then
        FYear := '19' + FYear;
     Result := FDays + '/' + FMonth + '/' + FYear;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dbgInterfaceDblClick(Sender: TObject);
begin
     with dm.ibdsExport do begin
        if (FieldByName('POINTS_USED').AsInteger = 0) and
           (FieldByName('RESET_POINTS').AsString = varNo) then begin
           Edit;
           FieldByName('EXPORTED').AsString := 'N';
           Post;
        end else begin   
           sm('Cannot resend a Points Export - must be manually altered')
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
     // just in case
     if ControllerPCName = ReadComputerName then tsInterfaceExit(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.menuExitClick(Sender: TObject);
begin
     close;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dbnRefreshClick(Sender: TObject);
begin
     case PageControl.ActivePageIndex of
         0 : begin // Site Registry and Defaults
                case pcSite.ActivePageIndex of
                   2 : dm.tblSite.Refresh;
                   0 : dm.tblMisc.Refresh;
                   3 : dm.tblMenuMate.Refresh;
                end;
             end;
         1 : begin // Other
                case rgpOther.ItemIndex of
                   0 : dm.tblPrinters.Refresh;// Printers
                   1 : dm.tblScreens.Refresh; // Screen Def
                   2 : dm.tblVersions.Refresh; // Versions
                end;
             end;
         2 : dm.tblUserSecView.Refresh; // User Security
         3 : begin // Logs
                case rgpLog.ItemIndex of
                   0 : QryReActivate(dm.qryProcess);
                   1 : QryReActivate(dm.qryBackupLog);
                end;
             end;
         6 : dm.ibdsExport.Refresh;
     end;  // case
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.LoadPointsFromFile;
var F : Textfile;
    s,ErrorMsg,TempStr : String;
    Member: LongInt;
    PrePaidCredit,BonusCredit,EarntCredit,TotalRedeemed ,TotalEarnt,PointsAvailable : extended;
const  Bulkimport = 'Bulk Import of Points';                       
{
 Transaction - IBRollover
 file format  1083,.26,1.26,99.99, etc '' for no value
 membership_number,point_available,total.redeemed,total.earnt,Pre_Paid_credit,Bonus_Credit,Earnt_Credit
}       
begin
     if dm.opendialog.execute then begin
        try
           TooBusy('Loading the Points File');
           dm.StartTrans(dm.IBRollover);
           AssignFile( F, dm.opendialog.filename );
           Reset( F );
           ErrorMsg := ''; s := '';
           while not EOF(F) do begin
              Readln(F,S);
              try
                 TempStr       := RemoveSignSpace(GetCommaDelimitedField(0,s));
                 Member        := StrToInt(TempStr);
                 // Field 1 is Points_available 
                 PointsAvailable    := RemoveSignSpaceConvert(GetCommaDelimitedField(1,s));
                 if PointsAvailable  = -9999999.00 then
                    PointsAvailable := 0.00;

                 TotalRedeemed    := RemoveSignSpaceConvert(GetCommaDelimitedField(2,s));
                 if TotalRedeemed  = -9999999.00 then
                    TotalRedeemed := 0.00;
                    
                 TotalEarnt    := RemoveSignSpaceConvert(GetCommaDelimitedField(3,s));
                 if TotalEarnt  = -9999999.00 then
                    TotalEarnt := 0.00;

                 PrePaidCredit    := RemoveSignSpaceConvert(GetCommaDelimitedField(4,s));
                 if PrePaidCredit  = -9999999.00 then
                    PrePaidCredit := 0.00;   
                                 
                 BonusCredit    := RemoveSignSpaceConvert(GetCommaDelimitedField(5,s));
                 if BonusCredit  = -9999999.00 then
                    BonusCredit := 0.00;   
                 
                 EarntCredit    := RemoveSignSpaceConvert(GetCommaDelimitedField(6,s));
                 if EarntCredit  = -9999999.00 then
                    EarntCredit := 0.00;   
                
              except
                 ErrorMsg := ErrorMsg + format('Member %d is not correct',[Member]) + #13#10;
                 Continue;
              end;                  
              with dm.ibdsMemberPoints do begin
                 Close;
                 ParamByName('Member').AsInteger := Member;
                 Open;
                 if RecordCount = 0 then begin                       
                    ErrorMsg := ErrorMsg + format('Member %d is not loaded',[Member]) + #13#10;
                    Continue;
                 end;   
                 edit;
                 FieldByName('POINTS_AVAILABLE').AsFloat := PointsAvailable;
                 FieldByName('TOTAL_REDEEMED').AsFloat   := TotalRedeemed;
                 FieldByName('TOTAL_EARNT').AsFloat      := TotalEarnt;
                 FieldByName('PRE_PAID_CREDIT').AsFloat  := PrePaidCredit;
                 FieldByName('BONUS_CREDIT').AsFloat     := BonusCredit;
                 FieldByName('EARNT_CREDIT').AsFloat     := EarntCredit;
                 Post;
              end;   
           end;                                  
           try
              dm.ProcessType := ord(TProcessTypes(ptSystem));
              dm.LogProcess(dm.sqlRollover, BulkImport + CompletedMsg );
              dm.IBRollover.Commit;
              if ErrorMsg <> '' then sm(ErrorMsg);
              MDlgI(BulkImport + CompletedMsg);
           except
              dm.IBRollover.Rollback;
              sm(BulkImport + DidntWorkMsg);
           end;
        finally
           NotBusy;
           Closefile( F );
           dm.ibdsMemberPoints.Close;
        end;
     end;
end;
{ **************************************************************************** }
function TfrmMembersSYS.GetPointsFromString(s : String ; var Member : integer; var PointsAvailable,TotalRedeemed ,TotalEarnt,PrePaidCredit,BonusCredit,EarntCredit : extended): Boolean;
var TempStr : String;
begin
     Result := False;
     try
         // Member
         Member        := StrToInt(GetCommaDelimitedField(0,s));
         // Field 1 is Points_available 
         TempStr       := Trim(GetCommaDelimitedField(1,s));
         if TempStr <> '' then begin
            PointsAvailable := StrToFloat(TempStr);
            PointsAvailable := PointsAvailable / 100;
            
         end else
           PointsAvailable := 0.00; 
         TempStr       := Trim(GetCommaDelimitedField(2,s));
         if TempStr <> '' then
            TotalRedeemed := StrToFloat(TempStr)
         else
           TotalRedeemed := 0.00; 
         TempStr       := Trim(GetCommaDelimitedField(3,s));
         if TempStr <> '' then
            TotalEarnt := StrToFloat(TempStr)
         else
           TotalEarnt := 0.00; 
         TempStr       := Trim(GetCommaDelimitedField(4,s));
         if TempStr <> '' then
            PrePaidCredit := StrToFloat(TempStr)
         else
           PrePaidCredit := 0.00; 
         TempStr       := Trim(GetCommaDelimitedField(5,s));
         if TempStr <> '' then
            BonusCredit := StrToFloat(TempStr)
         else
           BonusCredit := 0.00; 
         TempStr       := Trim(GetCommaDelimitedField(6,s));
         if TempStr <> '' then
            EarntCredit := StrToFloat(TempStr)
         else
           EarntCredit := 0.00; 
     except
       //
     end;
     Result := True;                  
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.dbcbNameFieldsClick(Sender: TObject);
var i : integer;
    s : string;
begin
     with FieldsDialog do begin
        List1.Clear;
        List2.Clear;
        for i := 0 to Global.MaxNameTypes  do
           List1.add(NameTypes[i]);
        Title := 'FullName Makeup'; 
        Label1Caption := 'Available Fields'; 
        Label2Caption := 'Fullname in order of list'; 
        if Execute then begin
           s := '';
           for i := 0 to List2.Count -1 do s := s + List2[i] + ',';
           delete(s,length(s),1); // remove last comma
           with dm.TblMisc do begin
             Edit;
             FieldByName('NAME_FIELDS').AsString := s;
             Post;
           end; 
        end;
           
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.LoadGroups;
var f : Textfile;
    s : String[10];
    i : Longint;
    j : Smallint;
begin
     SetupOpenDialog('*.*','All Files|*.*','All files');
     if dm.OpenDialog.Execute then begin
        dm.IBRollover.StartTransAction;
        assignfile(f,dm.OpenDialog.FileName);
        reset(f);
        j := 0;
        while not EOF(f) do begin
            Readln(f,s);
            try
               i := StrToInt(s);
               dm.LogExport(dm.sqlRollover,i,0,False);
               inc(j);
            except
               sm(format('Member %s is not numeric',[s]));
               continue;
            end; 
        end;
        
        try
           if dm.IBRollover.InTransaction then
              dm.IBRollover.Commit;
           MDlgI(Format('Update Worked - wait until Export - 15 seconds - %d Records',[j]));
        except
           dm.IBRollover.Rollback;
           sm('Update Failed  - Rolled Back' );
        end;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.NoGroups;
var    j : Integer;
const  SQL1 = 'select m.member,mg.GROUPS from MEMBERS M LEFT JOIN M_GROUPS MG ' +
              'on m.MEMBER = MG.MEMBER';
begin
     if not ifMsgDlg('Are you sure you want to run this') then exit;
     screen.cursor := crHourGlass;   
     dm.IBRollover.StartTransAction;
     j := 0;
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL1) then begin
        while not dm.qryGeneral.EOF do begin
           if dm.qryGeneral.FieldByName('GROUPS').IsNull then begin
              dm.LogExport(dm.sqlRollover,dm.qryGeneral.FieldByName('MEMBER').AsInteger,0,False);
              inc(j);
           end;
           dm.qryGeneral.Next;
        end;   
     end;  
     try
         if dm.IBRollover.InTransaction then
            dm.IBRollover.Commit;
          MDlgI(Format('Update Worked - wait until Export - 15 seconds - %d Records',[j]));
     except
        dm.IBRollover.Rollback;
        sm('Update Failed  - Rolled Back' );
     end;
     screen.cursor := crDefault;   
     
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.GroupsLoadFromSubs;
var s : String;
    i : Integer;
const  SQL1 = 'select m.member,mg.GROUPS from MEMBERS M LEFT JOIN M_GROUPS MG ' +
              'on m.MEMBER = MG.MEMBER';
       SQL2 = 'Select SUBSCRIPTION from M_SUBS where MEMBER = %d';
       SQL3 = 'insert into M_GROUPS values (%d,%d)';
begin
     if not ifMsgDlg('Are you sure you want to run this GroupsLoadFromSubs') then exit;
     try
        screen.cursor := crHourGlass;
        s := '';
        if dm.IBTransaction.InTransaction then
              dm.IBTransaction.CommitRetaining;
        dm.qryReport.Close;
        dm.qryReport.SQL.Text := SQL1;
        dm.qryReport.Open;
        while not dm.qryReport.Eof do begin
           if dm.qryReport.FieldByName('GROUPS').IsNull then begin
           // get the Sub for the Member 
              dm.sqlExecquery(dm.qryGeneral,format(SQL2,[dm.qryReport.FieldByName('MEMBER').AsInteger]));
              if dm.qryGeneral.EOF then begin
                 s := s + dm.qryReport.FieldByName('MEMBER').AsString + ','; 
              end else begin
                 case dm.qryGeneral.FieldByName('SUBSCRIPTION').AsInteger of
                    1 : i := 1;
                    4 : i := 4;
                    7 : i := 3;
                    9 : i := 6;
                    //10 : i := 6;
                 else 
                    i := 1;
                 end;   
                 // put in the Group for the Member 
                 dm.sqlExecquery(dm.qryGeneral,format(SQL3,[dm.qryReport.FieldByName('MEMBER').AsInteger,1]));
              end;
           end;
           dm.qryReport.Next;
        end;
        try
           if dm.IBTransaction.InTransaction then
              dm.IBTransaction.CommitRetaining;
           MDlgI('GroupsLoadFromSubs Worked ' );
           if s <> '' then begin
              MDlgI('Members with no Subs and Groups = ' + s );
              AddToClipBoard(s);
           end;
        except
           dm.IBTransaction.Rollback;
           sm('GroupsLoadFromSubs Failed  - Rolled Back' );
        end;
     finally
        screen.cursor := crDefault;
        dm.qryReport.Close;
     end;
end;
{ ****************************************************************************}
procedure TfrmMembersSYS.SubsUpDate;
var f,outF : TextFile;
    s,MemberStr : ShortString;
    j : Integer;
    Member : LongInt;
    Financial,Active : String[3];
    Logit,HasSub : Boolean;
    v : Variant;
{const SQL1 = 'select m.member,ms.subscription,s.description,mg.groups,g.description ' +
             'from members m,m_Subs ms,m_GROUPS mg ,SUBSCRIPTIONS s,GROUPS g where ' +
             'm.member = ms.member and m.member = mg.member ' +
             'and mg.groups = g.groups and ms.subscription = s.subscription ' +
             'and ms.SUBSCRIPTION = %d and mg.GROUPS  in (%s) order by m.MEMBER';}
{const SQL1 = 'select m.member,ms.subscription,mg.groups ' +
             'from members m,m_Subs ms,m_GROUPS mg where ' +
             'm.member = ms.member and m.member = mg.member ' +
             'and ms.SUBSCRIPTION = %d and mg.GROUPS in (%s)';
 }
begin
     if not ifMsgDlg('Are you sure you want to run this') then exit;
     try
        dm.tblSubs.Open;
        if not dm.IBRunOnce.InTransAction then
           dm.IBRunOnce.StartTransaction;
        ibdsMembers.Open; // selects all Members
        ibdsMembers.First;
        screen.cursor := crHourGlass;
        //dm.sqlExecquery(dm.sqlRunOnce,'Delete from M_Subs where ');
        //dm.sqlExecquery(dm.sqlRunOnce,'Select * from m_groups');
        while not ibdsMembers.Eof do begin
           dm.sqlExecquery(dm.sqlRunOnce,format('Select * from m_groups where MEMBER = %d',[ibdsMembers.FieldByName('MEMBER').AsInteger]));
           // get the Sub for the Member from the Group
           case dm.sqlRunOnce.FieldByName('GROUPS').AsInteger of
              100 : v := 5;
              102 : v := 3;
              104 : v := 6;
              105 : v := 1;
           else 
              v := 5;
           end;   
           dm.TblSubs.Locate('SUBSCRIPTION',v,[]);
           IBDataSet.Close;
           IBDataSet.ParamByName('pMEMBER').AsInteger := ibdsMembers.FieldByName('MEMBER').AsInteger;
           IBDataSet.Open;
           HasSub := False;
           while not IBDataSet.EOF do begin
              if IBDataSet.FieldByName('SUBSCRIPTION').AsInteger = v then begin
                 IBDataSet.Next;
                 HasSub := true;
                 //continue;
              end else begin
                 IBDataSet.Delete;
                 dm.LogExport(dm.sqlRunOnce,ibdsMembers.FieldByName('MEMBER').AsInteger,0,False);
              end;
           end;
           // hasnt got the correct sub
           if not HasSub then begin
              IBDataSet.Insert;
              IBDataSet.FieldByName('MEMBER').AsInteger       := ibdsMembers.FieldByName('MEMBER').AsInteger;
              IBDataSet.FieldByName('SUBSCRIPTION').AsInteger := dm.tblSubs.FieldByName('SUBSCRIPTION').AsInteger;
              IBDataSet.FieldByName('RECEIPT').AsInteger      := 0;
              IBDataSet.FieldByName('AMOUNT_PAID').AsFloat    := dm.tblSubs.FieldByName('FEE').AsFloat; // gets latest price
              IBDataSet.FieldByName('AMOUNT').AsFloat         := dm.tblSubs.FieldByName('FEE').AsFloat;
              IBDataSet.FieldByName('PAID'     ).AsString     := 'N';
              IBDataSet.FieldByName('EDIT_DATE').AsDateTime   := now;
              IBDataSet.Post;
              dm.LogExport(dm.sqlRunOnce,ibdsMembers.FieldByName('MEMBER').AsInteger,0,False);
           end;
           ibdsMembers.Next;
        end;
        try
           if dm.IBRunOnce.InTransaction then
              dm.IBRunOnce.Commit;
           MDlgI('Update Subs Worked ' );
        except
           dm.IBRunOnce.Rollback;
           sm('Update Failed  - Rolled Back' );
        end;
     finally
        dm.tblSubs.Close;
        IBDataSet.Close;
        ibdsMembers.Close;
        screen.cursor := crDefault;
        
     end;
     {if dm.PExporting then begin
        sm('Exporting');
        exit;
     end;
     dm.ExportTimer.Enabled := False;
     SetupOpenDialog('*.*','All Files|*.*','All files');
     if dm.OpenDialog.Execute then begin
        if not dm.IBRunOnce.InTransAction then
           dm.IBRunOnce.StartTransaction;
        screen.cursor := crHourGlass;
        assignfile(f,dm.OpenDialog.FileName);
        assignfile(outf,'c:\Audits.txt');
        reset(f);
        rewrite(outf);
        j := 0;
        //ibdsMembers.oPEN;
        while not EOF(f) do begin
            Readln(f,s);
            try
               MemberStr := copy(s,1,pos('~',s) -1 );
               system.delete(s,1,pos('~',s) );
               Financial :=  copy(s,1,pos('~',s) -1);
               system.delete(s,1,pos('~',s) );
               Active := s;
               Member := StrToInt(MemberStr);
               if ibdsMembers.Locate('MEMBER',Member,[]) then begin
                  Logit := False;
                  if (Financial = '') or  (Active = '') then logit := True;
                  case ibdsMembers.FieldByName('SUB_PAID').AsInteger of
                     0 : if Financial = 'Yes' then Logit := True;
                     1 : if Financial = 'No'  then Logit := True;
                     2 : if Financial = 'No'  then Logit := True;
                  end;
                  if (ibdsMembers.FieldByName('IS_ACTIVE').AsString = 'Y') and
                     (Active = 'No')  then Logit := True;
                  if (ibdsMembers.FieldByName('IS_ACTIVE').AsString = 'N') and
                     (Active = 'Yes') then Logit := True;
                  if Logit then begin
                     dm.LogExport(dm.sqlRunOnce,Member,0,False);
                     Writeln(outf,MemberStr + '  Status');
                     inc(j);
                  end;
                  ibdsMembers.EDIT;
                  ibdsMembers.FieldByName('SPARE_CHAR').AsString := 'Y';
                  ibdsMembers.POST;
               end else begin
                  dm.LogExport(dm.sqlRunOnce,Member,0,False);
                  Writeln(outf,MemberStr + '  not in Membership');
                  inc(j);
               end;
            except
               Writeln(outf,MemberStr + '  is not numeric');
               continue;
            end;
        end;
        closefile(f);
        flush(outf);
        closefile(outf);
        try
           if dm.IBRunOnce.InTransaction then
              dm.IBRunOnce.Commit;
           MDlgI(Format('Update Worked - wait until Export - 15 seconds - %d Records',[j]));
        except
           dm.IBRunOnce.Rollback;
           sm('Update Failed  - Rolled Back' );
        end;
     end;
     screen.cursor := crDefault;
     dm.ExportTimer.Enabled := True;
     ibdsMembers.cLOSE;



     {FMember := 0;
     with dm.sqlRunOnce do begin
        Close;
        SQL.Text := Format(SQL1,[1,'103,104,105,102']);
        execquery;
        ProcessSubs(0);

        Close;
        SQL.Text := Format(SQL1,[2,'100,104,105,102']);
        execquery;
        ProcessSubs(0);

        Close;
        SQL.Text := Format(SQL1,[4,'100,103,105,102']);
        execquery;
        ProcessSubs(0);

        Close;
        SQL.Text := Format(SQL1,[5,'100,103,104,102']);
        execquery;
        ProcessSubs(0);

        Close;
        SQL.Text := Format(SQL1,[6,'100,103,105,104']);
        execquery;
        ProcessSubs(0);
        try
           dm.IBRunOnce.Commit;
           Showmessage (format('Completed CASHMERE Subs Update %d Records Changed',[FMember]));
        except
           dm.IBRunOnce.Rollback;
           Showmessage ('FAILED CASHMERE Subs Update');
        end;
     end;
     screen.cursor := crDefault;   }
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.ProcessSubs( i : Integer );
//const SQL1 = 'select count(*) from m_subs where subscription = %d and Member = %d';
begin
     with dm.sqlRunOnce do begin
        //First;
        while not EOF do begin
           case FieldByName('GROUPS').AsInteger of
              100 : i := 1;
              103 : i := 2;
              104 : i := 4;
              105 : i := 5;
              102 : i := 6;
           else begin
              next;
              continue;
           end;
           end;
           // update the record
           ibdsMembers.Close;
           ibdsMembers.PAramByNAme('Subscription').AsInteger := FieldByName('SUBSCRIPTION').AsInteger;
           ibdsMembers.PAramByNAme('Member'      ).AsInteger := FieldByName('MEMBER').AsInteger;
           ibdsMembers.Open;
           //sm('subs ' + IntToStr(FieldByName('SUBSCRIPTION').AsInteger) + '   Member ' + IntToStr(FieldByName('MEMBER').AsInteger));
           if ibdsMembers.RecordCount = 0 then begin
              next;
              continue;
           end;
           while not ibdsMembers.EOF do begin
           // change the record over
              if dm.tblSubs.Locate('SUBSCRIPTION',i,[]) then begin
                 try
                 ibdsMembers.edit;
                 ibdsMembers.FieldByName('SUBSCRIPTION').AsInteger := i;
                 ibdsMembers.FieldByName('AMOUNT').AsFloat         := dm.tblSubs.FieldByNAme('FEE').AsFloat;
                 ibdsMembers.FieldByName('AMOUNT_PAID').AsFloat    := 0.00;
                 ibdsMembers.FieldByName('PAY').AsFloat            := 0.00;
                 ibdsMembers.post;
                 inc(FMember);
                 except
                     sm('Duplicate subs ' + IntToStr(FieldByName('SUBSCRIPTION').AsInteger) + ' for Member ' + IntToStr(FieldByName('MEMBER').AsInteger));
                     ibdsMembers.Next;
                     continue;
                 end;
              end;   
              ibdsMembers.Next;
           end;
           next;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.BulkDeleteMembers;
var i : smallint;
begin
     if not ifMsgDlg('Are you sure you want to run this') then exit;
     try
        if not dm.IBRunOnce.InTransAction then
           dm.IBRunOnce.StartTransaction;
         frmMembersLookup := TfrmMembersLOOKUP.create(nil);
         frmMembersLookup.Setup( dm.dsrTblSections,dm.tblSections,'SECTION',0);
         if (frmMembersLookup.ShowModal = mrOK) then begin
            i := UpdateMultipleLinks;
         end;
         try
            if dm.IBRunOnce.InTransAction then
              dm.IBRunOnce.Commit;
          except
              dm.IBRunOnce.Rollback;
              sm('Error Bulk Deleting Members');
          end;
     finally
         screen.cursor := crDefault;
         sm( format('There were %d Members deleted',[i]));
         ibdsMembers.Close;
         frmMembersLookup.Free;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.UpdateExpires;
const SQL1 = 'update members set expires = ''%s''';
begin
     if not ifMsgDlg('Do you wish to update Expires on ALL members?') then exit;
     try
        if not dm.IBRunOnce.InTransAction then
           dm.IBRunOnce.StartTransaction;
         // get the expiry date  
        if CalendarDialog.Execute then begin
           try
              dm.sqlExecQuery(dm.SQLRunOnce,format(SQL1,[GetShortDate(CalendarDialog.Calendar.Date)]));
              if dm.IBRunOnce.InTransAction then
                 dm.IBRunOnce.Commit;
            except
               dm.IBRunOnce.Rollback;
               sm('Error updating the Expires ');
            end;
            screen.cursor := crDefault;
            sm('updating Expires completed successfully');
         end;
     finally
         screen.cursor := crDefault;
     end;
end;
{ **************************************************************************** }

function TfrmMembersSYS.UpdateMultipleLinks : Smallint;
var i : shortint;
    s : ShortString;
    Member : LongInt;
const SQL1 = 'DELETE FROM %s WHERE MEMBER = %d';
      SQL2 = 'Select distinct m.MEMBER from MEMBERS m,M_SECTIONS ms where m.MEMBER = ms.MEMBER and ms.SECTION in ( %s )';
begin
     screen.cursor := crHourGlass;
     s := '';
     // get each section
     for i := 0 to frmMembersLookup.MultiList.Count-1 do
         s := s + frmMembersLookup.MultiList[i]  + ',';
     s := deleteLastComma(s);
     //ibdsMembers.ParamByName('Sections').AsString := s;
     ibdsMembers.SelectSQL.Text := format(sql2,[s]);
     //sm(ibdsMembers.SelectSQL.Text);
     ibdsMembers.Open;
     ibdsMembers.First;
     Result := 0;
     while not ibdsMembers.EOF do begin
        Member := ibdsMembers.FieldByName('MEMBER').AsInteger;
        dm.sqlExecQuery(dm.sqlRunOnce,format(SQL1,['M_EVENTS',Member]));
        dm.sqlExecQuery(dm.sqlRunOnce,format(SQL1,['M_SECTIONS',Member]));
        dm.sqlExecQuery(dm.sqlRunOnce,format(SQL1,['M_SUBS',Member]));
        dm.sqlExecQuery(dm.sqlRunOnce,format(SQL1,['M_GROUPS',Member]));
        dm.sqlExecQuery(dm.sqlRunOnce,format(SQL1,['M_INTERESTS',Member]));
        dm.sqlExecQuery(dm.sqlRunOnce,format(SQL1,['LOTS',Member]));
        dm.sqlExecQuery(dm.sqlRunOnce,format(SQL1,['M_PHOTOS',Member]));
        dm.LogExport(dm.sqlRunOnce,Member,0,False);
        dm.LogProcess(dm.sqlRunOnce,Format('Member %d deleted - Bulk Section delete',[Member]));
        ibdsMembers.Delete;
        inc(Result);
      end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.AddDOB;
var F : Textfile;
    s,ErrorMsg,Member: string;
    dt : TDateTime;
    //Year, Month, Day : Word;

const ImpDatesMsg = 'Import of Dates';
     SQL2 = 'UPDATE M_EVENTS SET DATES = ''%s'' where MEMBER = %s';
     SQL1 = 'UPDATE MEMBERS  SET DATE_OF_BIRTH = ''%s'' where MEMBER = %s';

begin
     If not IfMsgDlg('Do you with to import the Dates from an Ascii File?') then exit;
     SetupOpenDialog('*.*','All Files|*.*','All files');
     if dm.opendialog.execute then begin
        try
           TooBusy('Loading the Dates File');
           TwoDigitYearCenturyWindow := 98;	
           dm.StartTrans(dm.IBRunOnce);
           AssignFile( F, dm.opendialog.filename );
           Reset( F );
           ErrorMsg := ''; s := '';
           while not EOF(F) do begin
              Readln(F,S);
              Member := copy(s,1,POS('~',S)-1);
              if not AllDigits(Member) then begin
                 ErrorMsg := ErrorMsg + format('Member %s is not numeric',[Member]) + #13#10;
                 continue;
              end;
              system.delete(s,1, POS('~',S));
              try
                 dt := StrToDateTime(s);
                 //DecodeDate(dt, Year, Month, Day);
                 //if (Year > 5) and (Year < 99) then 
                 //   Year := Year + 1900
                 //else   
                 //   Year := Year + 2000;
                 //dt := EncodeDate(Year, Month, Day);
                 s := GetShortDate( dt);
              except
                 ErrorMsg := ErrorMsg + format('Member %s Date of Birth is incorrect',[Member]) + #13#10;
                 continue;
              end;

              dm.sqlExecQuery(dm.sqlRunOnce,format(SQL1,[s,Member ]));
              dm.sqlExecQuery(dm.sqlRunOnce,format(SQL2,[s,Member ]));
           end;
           try
              dm.ProcessType := ord(TProcessTypes(ptSystem));
              dm.LogProcess(dm.sqlRunOnce, ImpDatesMsg + CompletedMsg );
              dm.IBRunOnce.Commit;
              if ErrorMsg <> '' then sm(ErrorMsg);
              MDlgI(ImpDatesMsg + CompletedMsg);
           except
              dm.IBRunOnce.Rollback;
              sm(ImpDatesMsg + DidntWorkMsg);
           end;
        finally
           NotBusy;
           Closefile( F );
           TwoDigitYearCenturyWindow := 0;	
           
        end;
     end;

end;
{ **************************************************************************** }
procedure TfrmMembersSYS.AddJoinFee;
var F : Textfile;
    Member ,ErrorMsg: string;
const ImpDatesMsg = 'Import of Joining Fees';
      SQL1 = 'UPDATE MEMBERS SET JOINING_FEE = 250 where MEMBER = %s';

begin
     If not IfMsgDlg('Do you with to import the Joining Fees from an Ascii File?') then exit;
     SetupOpenDialog('*.*','All Files|*.*','All files');
     if dm.opendialog.execute then begin
        try
           TooBusy('Loading the Joining Fees File');
           dm.StartTrans(dm.IBRunOnce);
           AssignFile( F, dm.opendialog.filename );
           Reset( F );
           while not EOF(F) do begin
              Readln(F,Member);
              Member := Trim(Member);
              if not AllDigits(Member) then begin
                 ErrorMsg := ErrorMsg + format('Member %s is not numeric',[Member]) + #13#10;
                 continue;
              end;
              dm.sqlExecQuery(dm.sqlRunOnce,format(SQL1,[Member ]));
           end;
           try
              dm.IBRunOnce.Commit;
              if ErrorMsg <> '' then sm(ErrorMsg);
              MDlgI(ImpDatesMsg + CompletedMsg);
           except
              dm.IBRunOnce.Rollback;
              sm(ImpDatesMsg + DidntWorkMsg);
           end;
        finally
           NotBusy;
           Closefile( F );
        end;
     end;

end;
{ **************************************************************************** }

procedure TfrmMembersSYS.cbEmulationCloseUp(Sender: TwwDBComboBox;  Select: Boolean);
begin
     if (cbEmulation.Modified) and (cbEmulation.ItemIndex > 0) then begin
        with dm.tblPrinters do begin
           Edit;
           FieldByName('COMM_PORT').Clear;
           Post;
           dm.IBCommitRetain;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.cbCommPortCloseUp(Sender: TwwDBComboBox; Select: Boolean);
begin
     if (cbCommPort.Modified) and (cbCommPort.ItemIndex > 0) then begin
        with dm.tblPrinters do begin
           Edit;
           FieldByName('PRINTER_INDEX').Clear;
           FieldByName('PRINTER_NAME').Clear;
           FieldByName('EMULATION').Clear;
           Post;
           dm.IBCommitRetain;
        end;
     end;
end;
{******************************************************************************}
procedure TFrmMembersSYS.Capitalize;
begin
     if not ifMsgDlg('Are you sure you want to run this') then exit;
     screen.cursor := crHourGlass;
     dm.StartTrans(dm.IBRunOnce);
     try
        with ibdsMembers do begin
           open;
           first;
           while not EOF do begin
              edit;
              //FieldByName('FIRST_NAME').AsString := global.FirstLetterUpper(FieldByName('FIRST_NAME').AsString);
              //FieldByName('SURNAME').AsString    := global.FirstLetterUpper(FieldByName('SURNAME').AsString);
              //FieldByName('FULL_NAME').AsString  := FieldByName('FIRST_NAME').AsString + ' ' + FieldByName('SURNAME').AsString;
              FieldByName('FULL_NAME').AsString := Trim(FieldByName('TITLE').AsString) + ' ' +
                                                   Trim(FieldByName('INITIALS').AsString) + ' ' +
                                                   Trim(FieldByName('SURNAME').AsString);

              post;
              next;
           end;
        end;
        dm.IBRunOnce.Commit;
        showmessage('Finished OK');
     except
        dm.IBRunOnce.Commit;
        showmessage('Not OK');
     end;
     ibdsMembers.close;
     screen.cursor := crDefault;
end;
{ **************************************************************************** }
procedure TFrmMembersSYS.DeleteWinFile;
begin
     if not ifmsgDlg('Reminder - You must first close Clubmaster on all PCs - continue?') then exit; 
     if not DeleteFile(ArevImportFilePath + '\WIN') then
        sm(format('Cannot delete %s',[ArevImportFilePath + '\WIN']))
     else
        mdlgI('File Deleted Successfully');
end;
{ **************************************************************************** }
procedure TfrmMembersSYS.rgpLogChange(Sender: TObject; ButtonIndex: Integer);
const SQL1 = 'select * FROM %s where USER_CODE = %d order by EDIT_DATE desc';
      msg1 = 'Double Click to see the Backup Log';
      GTab = #9;
var i : shortint;      
begin
     dbgLogs.TitleLines := 1;
     cbxAllUsers.Enabled := True;
     dbcbProcess.Enabled := True;
     dbcbProcess.ItemIndex := 0;
     LogListBox.Enabled := True;
     cbxAllUsers.State := cbChecked;
     LogListBox.Enabled := False;
     dbgLogs.Hint := '';
     case rgpLog.ItemIndex of
     0 :  begin  // Process Log
             dm.qryProcess.UpdateObject := dm.IBUpdateSQL; // for refreshing
             with dbgLogs do begin
   	          Selected.Clear;
     	          Selected.Add('USER_CODE' + GTab + '9' + GTab + 'User Code');
   	          Selected.Add('MSG' + GTab + '55' + GTab + 'Message');
      	       Selected.Add('EDIT_DATE' + GTab + '18' + GTab + 'Date');
      	       ApplySelected;
             end;
             dbcbProcess.Items.Clear;
             for i := 0 to MaxProcessTypes do
                dbcbProcess.Items.Add(ProcessTypes[i]);
             dbgLogs.DataSource := dm.dsrProcess;
             dbnNavRefresh.DataSource := dm.dsrProcess;
             dbcbProcess.ItemIndex := 0; 
             ProcessChange(dm.qryProcess);
          end;
      1 :  begin   // BackupLog
             dm.qryBackupLog.UpdateObject := dm.IBUpdateSQL; // for refreshing
             dbgLogs.DataSource := dm.dsrBackupLog;
             dbnNavRefresh.DataSource := dm.dsrBackupLog;
             with dbgLogs do begin
   	         Selected.Clear;
   	         Selected.Add('NAME' + GTab + '28' + GTab + 'Backup Name');
   	         Selected.Add('DATE_TIME' + GTab + '20' + GTab + 'Date Time');
      	      Selected.Add('LOG' + GTab + '10' + GTab + 'Log' );
      	      Selected.Add('SUCCESS'  + GTab + '1' + GTab + 'Succ');
      	      Selected.Add('MAN_AUTO'  + GTab + '1' + GTab + 'Auto');
      	      Selected.Add('PCNAME'  + GTab + '13' + GTab + 'Backup PC');
      	      ApplySelected;
             end;
             dbgLogs.Hint := Msg1;
             dm.qryBackupLog.Open;
             cbxAllUsers.Enabled := False;
             dbcbProcess.Enabled := False;
             LogListBox.Enabled  := False;
          end;     
      2 :  begin   //
             dm.qryPointsLog.UpdateObject := dm.IBUpdateSQL; // for refreshing
             dbgLogs.DataSource := dm.dsrQryPointsLog;
             dbnNavRefresh.DataSource := dm.dsrQryPointsLog;
             with dbgLogs do begin
               TitleLines := 2;
   	         Selected.Clear;
   	         Selected.Add('MEMBER' + GTab + '8' + GTab + 'Member');
   	         //Selected.Add('FULL_NAME' + GTab + '15' + GTab + 'Full Name');
   	         Selected.Add('EDIT_DATE' + GTab + '15' + GTab + 'Date Time');
     	         Selected.Add('USER_CODE' + GTab + '5' + GTab + 'User~Code');
      	      Selected.Add('BEF_TOTAL_EARNT' + GTab + '8' + GTab + 'Points Earnt~Before' );
      	      Selected.Add('TOTAL_EARNT'  + GTab + '8' + GTab + 'Points Earnt~After');
      	      Selected.Add('BEF_TOTAL_REDEEMED' + GTab + '8' + GTab + 'Points Red~Before' );
      	      Selected.Add('TOTAL_REDEEMED'  + GTab + '8' + GTab + 'Points Red~After');
      	      Selected.Add('BEF_POINTS_AVAILABLE' + GTab + '8' + GTab + 'Points Avail~Before' );
      	      Selected.Add('POINTS_AVAILABLE'  + GTab + '8' + GTab + 'Points Avail~After');
      	      Selected.Add('BEF_EARNT_CREDIT' + GTab + '8' + GTab + 'Earnt Credit~Before' );
      	      Selected.Add('EARNT_CREDIT'  + GTab + '8' + GTab + 'Earnt Credit~After');
      	      Selected.Add('BEF_BONUS_CREDIT' + GTab + '8' + GTab + 'Bonus Credit~Before' );
      	      Selected.Add('BONUS_CREDIT'  + GTab + '8' + GTab + 'Bonus Credit~After');
      	      Selected.Add('BEF_PRE_PAID_CREDIT' + GTab + '8' + GTab + 'Pre Paid Credit~Before' );
      	      Selected.Add('PRE_PAID_CREDIT'  + GTab + '8' + GTab + 'Pre Paid Credit~After');
      	      Selected.Add('BEF_VALUE_OF_CREDIT' + GTab + '8' + GTab + 'Value of Credit~Before' );
      	      Selected.Add('VALUE_OF_CREDIT'  + GTab + '8' + GTab + 'Value of Credit~After');
      	      Selected.Add('POINTS_VALUE'  + GTab + '8' + GTab + 'Points~Adjust');
      	      Selected.Add('REASON'  + GTab + '35' + GTab + 'Reason');
      	      ApplySelected;
             end;
             dbcbProcess.Items.Clear;
             for i := 0 to MaxPointsCreditTypes do
                dbcbProcess.Items.Add(PointsCreditNames[i]);
             dbcbProcess.ItemIndex := 0; 
             ProcessChange(dm.qryPointsLog);
          end;     
     end;
end;
{ ****************************************************************************}
procedure TfrmMembersSYS.btnPointsAuditClick(Sender: TObject);
//var DelimInput : TIBInputDelimitedFile;
begin
    { if not ifMsgDlg('Run a Points Audit - all Rollover Files must be processed first') then exit;
     try
        // empty the table
        dm.StartTrans(dm.IBRollover);
        screen.cursor := crHourGlass;
        dm.sqlExecQuery(dm.sqlRollover,'Delete from IMPORT_POINTS' ); 
        dm.sqlRollover.SQL.TEXT := 'Insert into IMPORT_POINTS values(:MEMBER,:POINTS_AVAILABLE,:TOTAL_REDEEMED,:TOTAL_EARNT,:PRE_PAID_CREDIT,:BONUS_CREDIT,:EARNT_CREDIT)';
        DelimInput := TIBInputDelimitedFile.Create;
        DelimInput.FileName := 'c:\Points.txt';
        //DelimInput.FileName := Global.ArevImportFilePath + '\Points.Dat';
        DelimInput.ColDelimiter := ',';
        DelimInput.ReadBlanksAsNull := True;
        dm.sqlRollover.BatchInput(DelimInput);
       { dm.sqlExecQuery(dm.sqlRollover,'Delete from IMPORT_POINTS where MEMBER = 0' );
        dm.sqlExecQuery(dm.sqlRollover,'update IMPORT_POINTS set POINTS_AVAILABLE = 0 where POINTS_AVAILABLE is null' );
        // POINTS_AVAILABLE come in as 444 instead of 4.44
        dm.sqlExecQuery(dm.sqlRollover,'update IMPORT_POINTS set POINTS_AVAILABLE = POINTS_AVAILABLE / 100' );
        dm.sqlExecQuery(dm.sqlRollover,'update IMPORT_POINTS set TOTAL_REDEEMED = 0 where TOTAL_REDEEMED is null' );
        dm.sqlExecQuery(dm.sqlRollover,'update IMPORT_POINTS set TOTAL_EARNT = 0 where TOTAL_EARNT is null' );
        dm.sqlExecQuery(dm.sqlRollover,'update IMPORT_POINTS set PRE_PAID_CREDIT = 0 where PRE_PAID_CREDIT is null' );
        dm.sqlExecQuery(dm.sqlRollover,'update IMPORT_POINTS set BONUS_CREDIT = 0 where BONUS_CREDIT is null' );
        dm.sqlExecQuery(dm.sqlRollover,'update IMPORT_POINTS set EARNT_CREDIT = 0 where EARNT_CREDIT is null' );
        dm.sqlExecQuery(dm.sqlRollover,'update IMPORT_POINTS set EARNT_CREDIT = 0 where EARNT_CREDIT is null' );}
   {     dm.IBRollover.CommitRetaining;
     finally
        screen.cursor := crDefault;
        DelimInput.Free;
     end; }
end;
{ ****************************************************************************}
procedure TfrmMembersSYS.Oamaru;
var i : integer;
    s : String;
const SQL1 = 'update members set expires = null where member = %d';
      //SQL2 = 'grant all on AREV to allusers';
      SQL3 = 'Select m.member from members m, m_Groups mg where mg.groups in (102,105) ' +
             ' and m.member = mg.member';
begin
     if not ifMsgDlg('Are you sure you want to run this') then exit;
     try
        if not dm.IBRunOnce.InTransAction then
           dm.IBRunOnce.StartTransaction;
      //  dm.sqlExecquery(dm.sqlRunOnce,SQL2);
       // dm.IBRunOnce.Commit;
      //  if not dm.IBRunOnce.InTransAction then
      //     dm.IBRunOnce.StartTransaction;
         
        screen.cursor := crHourGlass;
         
        dm.sqlExecquery(dm.qryGeneral,SQL3);
        i := 1;
        while not dm.qryGeneral.Eof do begin
           inc(i);
           dm.sqlExecquery(dm.sqlRunOnce,format(SQL1,[dm.qryGeneral.FieldByName('MEMBER').AsInteger]));
           dm.LogExport(dm.sqlRunOnce,dm.qryGeneral.FieldByName('MEMBER').AsInteger,0,False);
           dm.qryGeneral.Next;
        end;   
        dm.IBRunOnce.Commit;
        sm('Oamaru update worked ' + inttostr(i));
     finally
        screen.cursor := crDefault;
     end;
end;

function TfrmMembersSYS.DayMonthYearFunny( str : string ) : String;
const
   MonthsArray : Array[1..12] of String[5]
             = ( 'JAN','FEB','MARCH','APRIL','MAY','JUNE','JULY','AUG','SEPT','OCT','NOV','DEC');
   MonthsArray1 : Array[1..12] of String[3]
             = ( 'JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC');
             
var s,FYear : string[8];
    FDays,FMonth : string[4];
    j : shortint;
begin                   
     if str = '' then  begin
        result := '03/31/2003';
        exit;
     end;   
     FDays := copy(str,1,POS(' ',Str) -1);
     if Length(FDays) = 1 then
        FDays := '0' + FDays;
     system.Delete(Str,1,POS(' ',Str));   

     s := copy(str,1,POS(' ',Str) -1);
     for j := 1 to 12 do 
         if s = MonthsArray1[j] then break; 
     // check in the next array
     if j = 13 then 
        for j := 1 to 12 do 
           if s = MonthsArray[j] then break; 
         
     FMonth :=  IntToStr(j);
     if Length(FMonth) = 1 then
        FMonth := '0' + FMonth;
     
     system.Delete(Str,1,POS(' ',Str));
     FYear := Str;   
     if Length(FYear) = 2 then
        FYear := '19' + FYear;
     Result := FMonth + '/' + FDays + '/' + FYear;
end;

{ ****************************************************************************}
procedure TfrmMembersSYS.btnMMPointsClick(Sender: TObject);
var   MenuMatePointsUpdate : TMenuMatePointsUpdate;
begin
     // get all points
     MenuMatePointsUpdate.MembersPointsUpdate('-1');
     QryActiveOrRefresh(dm.qryMenuMatePoints);
     dm.qryMenuMatePoints.First;
end;
{ ****************************************************************************}
procedure TfrmMembersSYS.btnSwipeCardClick(Sender: TObject);
var s : string;
i : integer;
begin
     if not IfMsgDlg('Do you wish to upgrade all Members Swipe Cards?') then exit;
     dm.StartTrans(dm.IBRunOnce);
     try
        with dm.ibdsMenuMate do begin
           TooBusy('Updating Swipe Card Field');
           open;
           while not EOF do begin
              Edit;
              FieldByName('SWIPE_CARD').AsString := dm.InsertMemberFullName(FieldByName('MEMBER').AsString,FieldByName('FULL_NAME').AsString);
              FieldByName('ALLOW_CREDIT').AsString := 'Y';
              FieldByName('EARN_POINTS').AsString  := 'Y';
              Post;
              Next;
           end;
           if dm.IBRunOnce.InTransaction then
              dm.IBRunOnce.Commit; 
          Showmessage('Completed Successfully');    
        end;
     finally
        NotBusy;         
     end;   
end;
{ ****************************************************************************}
{procedure TfrmMembersSYS.MenuMatePointsLog1Click(Sender: TObject);
const
    SQL1 = 'select MENUMATE_POINTS_DATE from MISC';
    SQL2 = 'update MISC set MENUMATE_POINTS_DATE = ''%s''';
begin
    // fires off manually
    if not IfMsgDlg('Do you really want to run the MenuMate points log update?') then exit;
    if not dm.IBRunOnce.InTransAction then
       dm.IBRunOnce.StartTransaction;
    dm.sqlExecquery(dm.sqlRunOnce,SQL1);
    // the date should be
    if not IfMsgDlg(format('MenuMate Points Log Update has been run for %s',[GetShortDate(dm.sqlRunOnce.FieldByName('MENUMATE_POINTS_DATE').AsDateTime)])) then
       exit;
    dm.UpdateMenuMatePoints;
    try
      // MISC Table may be open under IBTransaction and will cause a
      // deadlock when running SQL2
      if dm.IBTransaction.InTransAction then begin
         dm.IBTransaction.Commit;
         dm.IBTransaction.StartTransaction;
      end;
      dm.sqlExecquery(dm.sqlRunOnce,format(SQL2,[GetShortDate(Date )]));
      if dm.IBRunOnce.InTransAction then 
         dm.IBRunOnce.Commit;
    except
      dm.IBRunOnce.Rollback;
      showmessage('proc MenuMatePointsLog1Click - MenuMate Points Update didnt work');
      exit;
    end;     
     Showmessage('Completed MenuMate Points Log');
end;}
{ ****************************************************************************}
procedure TfrmMembersSYS.dbcdMMDiscountsEnter(Sender: TObject);
begin
     showmessage('You must have MenuMate version 4 or above');
end;

procedure TfrmMembersSYS.dbcdMMTriggerEnter(Sender: TObject);
begin
     showmessage('You must add or remove the installed trigger for Donation of Points');
end;

end.
