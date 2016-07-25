unit MembersEDIT;
{
 idbsMembers
 Insert - once the user reaches the Links tables then it posts it.
          The links tables are all separate and need to have the 
          Member Number confirmed before committing.

 Edit - Cancel of changes does not affect Links changes

 Delete - checks integrity before deletion of Links then finally the Member Number

 Links Inserts are posted automatically.
 Links have to be handled from the separate Navigator;
                
}
interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Menus, ImgList, ComCtrls,  ExtCtrls, stdForm, wwrcdpnl, StdCtrls,
  wwdblook, Buttons, wwSpeedButton, wwDBNavigator, wwclearpanel, Wwdbdlg,
  Mask, wwdbedit, wwrcdvw, DBCtrls, Wwdotdot, Wwdbcomb,db,
  wwriched, wwidlg, wwdbdatetimepicker, Grids, Wwdbigrd, Wwdbgrid,
  Wwfltdlg, DualList, ToolWin,IBCustomDAtaSet,Printers, lmdctrl, lmdstdcS,
  ExtDlgs,Clipbrd,Jpeg, wwDialog,Global;

type
  TfrmMembersEDIT = class(TSMStdForm)
    pnlMember: TPanel;
    btnClose: TToolButton;
    Options1: TMenuItem;
    menuDefaultMember: TMenuItem;
    ToolBar1: TToolBar;
    menuExit: TMenuItem;
    btnFind: TToolButton;
    PopupMenu: TPopupMenu;
    subMenuEvent: TMenuItem;
    submenuGroups: TMenuItem;
    submenuSection: TMenuItem;
    submenuSubscription: TMenuItem;
    btnQuery: TToolButton;
    pnlFind: TPanel;
    dbnQryMembers: TwwDBNavigator;
    dbnQryMembersInsert: TwwNavButton;
    dbnQryMembersEdit: TwwNavButton;
    dbnQryMembersDelete: TwwNavButton;
    dbnQryMembersPost: TwwNavButton;
    dbnQryMembersCancel: TwwNavButton;
    dbnQryMembersRefresh: TwwNavButton;
    dbnQryMembersLook: TwwDBNavigator;
    dbnQryMembersLookFirst: TwwNavButton;
    dbnQryMembersLookPrior: TwwNavButton;
    dbnQryMembersLookNext: TwwNavButton;
    dbnQryMembersLookLast: TwwNavButton;
    menuAddMember: TMenuItem;
    menuDeleteMember: TMenuItem;
    menubb: TMenuItem;
    btnCopyMember: TToolButton;
    menuCopyMember: TMenuItem;
    btnNumbers: TToolButton;
    PopupMenuNumbers: TPopupMenu;
    menuMemberNumber: TMenuItem;
    PageControl: TPageControl;
    tsMaintenance: TTabSheet;
    tsProposer: TTabSheet;
    tsOther: TTabSheet;
    tsGroupsSubs: TTabSheet;
    tsEventsSections: TTabSheet;
    tsDiary: TTabSheet;
    Panel1: TPanel;
    DBRichEdit: TwwDBRichEdit;
    pnlSpace: TPanel;
    dblcSurname: TwwDBLookupCombo;
    pnlOther: TPanel;
    Label18: TLabel;
    Label57: TLabel;
    Label31: TLabel;
    Label32: TLabel;
    Label29: TLabel;
    Label43: TLabel;
    Label38: TLabel;
    Label35: TLabel;
    dbeAnalyse: TwwDBComboBox;
    dbePriceLevel: TwwDBEdit;
    wwDBComboBox6: TwwDBComboBox;
    wwDBComboBox7: TwwDBComboBox;
    wwDBEdit18: TwwDBEdit;
    wwDBEdit23: TwwDBEdit;
    Label19: TLabel;
    Label37: TLabel;
    Label33: TLabel;
    Label40: TLabel;
    Label41: TLabel;
    Label42: TLabel;
    Label30: TLabel;
    Label53: TLabel;
    Label36: TLabel;
    dbcbNewsLetter: TwwDBComboBox;
    wwDBComboBox9: TwwDBComboBox;
    wwDBComboBox10: TwwDBComboBox;
    wwDBComboBox12: TwwDBComboBox;
    wwDBComboBox13: TwwDBComboBox;
    wwDBComboBox5: TwwDBComboBox;
    dbeCardType: TwwDBComboBox;
    dbePosition: TwwDBEdit;
    Label39: TLabel;
    wwDBComboBox8: TwwDBComboBox;
    pnlProposer: TPanel;
    Label21: TLabel;
    Label23: TLabel;
    Label24: TLabel;
    Label26: TLabel;
    Label27: TLabel;
    Label28: TLabel;
    wwDBEdit11: TwwDBEdit;
    dbdtJoined: TwwDBDateTimePicker;
    dbdtApproved: TwwDBDateTimePicker;
    dblcProposer: TwwDBLookupCombo;
    dblcSeconder: TwwDBLookupCombo;
    dbeTransferred: TwwDBEdit;
    dbeProposerName: TwwDBEdit;
    dbeSeconderName: TwwDBEdit;
    pnlMaintenance: TPanel;
    lblAddress: TLabel;
    lblAddress1: TLabel;
    lblDistrict: TLabel;
    lblCity: TLabel;
    lblCountry: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    dbeAddress: TwwDBEdit;
    dbeAddress1: TwwDBEdit;
    dbeDistrict: TwwDBEdit;
    dbeCity: TwwDBEdit;
    dbeCountry: TwwDBEdit;
    dblcPostCode: TwwDBLookupCombo;
    dblcParentCode: TwwDBLookupCombo;
    dbeParentCodeName: TwwDBEdit;
    lblActive: TLabel;
    Label20: TLabel;
    Label13: TLabel;
    Label14: TLabel;
    Label15: TLabel;
    Label8: TLabel;
    dbcbActive: TwwDBComboBox;
    wwDBComboBox1: TwwDBComboBox;
    wwDBEdit6: TwwDBEdit;
    wwDBEdit7: TwwDBEdit;
    dbePhoneCell: TwwDBEdit;
    dbeKnownAs: TwwDBEdit;
    pnlName: TPanel;
    Label1: TLabel;
    dbeMember: TwwDBEdit;
    Label3: TLabel;
    dbcbTitle: TwwDBComboBox;
    Label4: TLabel;
    dbeInitials: TwwDBEdit;
    Label5: TLabel;
    dbeFirstName: TwwDBEdit;
    Label6: TLabel;
    dbeSurname: TwwDBEdit;
    Label2: TLabel;
    dbeFullName: TwwDBEdit;
    Label59: TLabel;
    wwDBEdit9: TwwDBEdit;
    wwDBEdit10: TwwDBEdit;
    Label60: TLabel;
    wwDBComboBox2: TwwDBComboBox;
    Label17: TLabel;
    lblFax: TLabel;
    dbeFax: TwwDBEdit;
    btnAttached: TToolButton;
    menuAttachedMembers: TMenuItem;
    Label64: TLabel;
    wwDBEdit14: TwwDBEdit;
    Label65: TLabel;
    wwDBEdit15: TwwDBEdit;
    N2: TMenuItem;
    menuNextUnUsedNumber: TMenuItem;
    menuAddMemberCard: TMenuItem;
    tsInterests: TTabSheet;
    pnlInterest: TPanel;
    dbgInterests: TwwDBGrid;
    Label51: TLabel;
    wwDBComboBox14: TwwDBComboBox;
    wwDBComboBox3: TwwDBComboBox;
    Label66: TLabel;
    Label44: TLabel;
    dbeEmail: TwwDBEdit;
    Label22: TLabel;
    Label34: TLabel;
    pnlInterests: TPanel;
    dbnInterests: TwwDBNavigator;
    dbnInterestsInsert: TwwNavButton;
    dbnInterestsDelete: TwwNavButton;
    dbnInterestsEdit: TwwNavButton;
    dbnInterestsPost: TwwNavButton;
    dbnInterestsCancel: TwwNavButton;
    dbnInterestsRefresh: TwwNavButton;
    pnlLot: TPanel;
    dbgLots: TwwDBGrid;
    pnlLots: TPanel;
    wwDBNavigator2: TwwDBNavigator;
    wwNavButton1: TwwNavButton;
    wwNavButton2: TwwNavButton;
    wwNavButton3: TwwNavButton;
    wwNavButton4: TwwNavButton;
    wwNavButton5: TwwNavButton;
    wwNavButton6: TwwNavButton;
    Label67: TLabel;
    Label68: TLabel;
    MenuLinks: TPopupMenu;
    dbcbLots: TwwDBComboBox;
    Panel13: TPanel;
    tsCreditPoints: TTabSheet;
    pnlGroup: TPanel;
    pnlGroups: TPanel;
    dbgGroups: TwwDBGrid;
    pnlSub: TPanel;
    pnlSubs: TPanel;
    dbgSubs: TwwDBGrid;
    Label52: TLabel;
    Label54: TLabel;
    dbnGroups: TwwDBNavigator;
    dbnGroupsInsert: TwwNavButton;
    wwNavButton20: TwwNavButton;
    wwNavButton21: TwwNavButton;
    wwNavButton22: TwwNavButton;
    wwNavButton23: TwwNavButton;
    dbnGroupsRefresh: TwwNavButton;
    dbnSubs: TwwDBNavigator;
    dbnSubsInsert: TwwNavButton;
    wwNavButton26: TwwNavButton;
    wwNavButton27: TwwNavButton;
    wwNavButton28: TwwNavButton;
    wwNavButton29: TwwNavButton;
    dbnSubsRefresh: TwwNavButton;
    N1: TMenuItem;
    menuReuseNonActiveMembers: TMenuItem;
    Label16: TLabel;
    Label25: TLabel;
    dbdtExpires: TwwDBDateTimePicker;
    Label62: TLabel;
    Label63: TLabel;
    Label69: TLabel;
    wwDBComboBox4: TwwDBComboBox;
    wwDBComboBox16: TwwDBComboBox;
    wwDBComboBox17: TwwDBComboBox;
    dbdtDOB: TwwDBDateTimePicker;
    pnlEvent: TPanel;
    pnlEvents: TPanel;
    Label10: TLabel;
    dbnEvents: TwwDBNavigator;
    dbnEventsInsert: TwwNavButton;
    wwNavButton8: TwwNavButton;
    wwNavButton9: TwwNavButton;
    wwNavButton10: TwwNavButton;
    wwNavButton11: TwwNavButton;
    dbnEventsRefresh: TwwNavButton;
    dbgEvents: TwwDBGrid;
    pnlSection: TPanel;
    dbgSections: TwwDBGrid;
    pnlSections: TPanel;
    Label9: TLabel;
    dbnSections: TwwDBNavigator;
    dbnSectionsInsert: TwwNavButton;
    wwNavButton14: TwwNavButton;
    wwNavButton15: TwwNavButton;
    wwNavButton16: TwwNavButton;
    wwNavButton17: TwwNavButton;
    dbnSectionsRefresh: TwwNavButton;
    dbdtSectionDate: TwwDBDateTimePicker;
    wwDBEdit1: TwwDBEdit;
    dbeSubsAdd: TwwDBEdit;
    tsPhotograph: TTabSheet;
    Panel2: TPanel;
    Panel3: TPanel;
    OpenPictureDialog: TOpenPictureDialog;
    GroupBox1: TGroupBox;
    btnLoadPhoto: TButton;
    Panel4: TPanel;
    Image: TImage;
    btnDeletePhoto: TButton;
    wwDBComboBox11: TwwDBComboBox;
    dbeHomeClub: TwwDBEdit;
    dblcClubID: TwwDBLookupComboDlg;
    ldlgSections: TwwLookupDialog;
    menuSetupMemberNumbers: TMenuItem;
    menuUseNonFinancialMember: TMenuItem;
    menuNextNumber: TMenuItem;
    N3: TMenuItem;
    menuAdjustPointsCredit: TMenuItem;
    menuResetAdjustPointsCredit: TMenuItem;
    lblPoints: TLabel;
    UsenonFinanc1: TMenuItem;
    menuExportMember: TMenuItem;
    N4: TMenuItem;
    btnPrint: TToolButton;
    menuPrintMemberDetails: TMenuItem;
    menuMemberVisits: TMenuItem;
    N5: TMenuItem;
    menuPHMTransfer: TMenuItem;
    tsPreference: TTabSheet;
    menuClosePreferences: TMenuItem;
    ScrollBox: TScrollBox;
    Panel5: TPanel;
    Label72: TLabel;
    wwDBNavigator1: TwwDBNavigator;
    wwNavButton7: TwwNavButton;
    wwNavButton12: TwwNavButton;
    wwNavButton13: TwwNavButton;
    wwNavButton18: TwwNavButton;
    wwNavButton19: TwwNavButton;
    wwNavButton24: TwwNavButton;
    Bevel1: TBevel;
    rgpPasteOptions: TRadioGroup;
    btnPastePhoto: TButton;
    tsDiscounts: TTabSheet;
    pnlDiscounts: TPanel;
    Label73: TLabel;
    dbnDiscounts: TwwDBNavigator;
    wwNavButton25: TwwNavButton;
    wwNavButton30: TwwNavButton;
    wwNavButton31: TwwNavButton;
    wwNavButton32: TwwNavButton;
    wwNavButton33: TwwNavButton;
    wwNavButton34: TwwNavButton;
    dbgDiscounts: TwwDBGrid;
    Label74: TLabel;
    btnUp: TButton;
    btnDown: TButton;
    ApplyDiscount1: TMenuItem;
    N6: TMenuItem;
    menuDeleteDiscount: TMenuItem;
    pnlDonation: TPanel;
    gbxDonation: TGroupBox;
    Label61: TLabel;
    Label55: TLabel;
    dblcDonation: TwwDBLookupCombo;
    wwDBEdit3: TwwDBEdit;
    dbeDonationName: TwwDBEdit;
    Label56: TLabel;
    Panel6: TPanel;
    pnlPoints: TPanel;
    Label47: TLabel;
    Label48: TLabel;
    Label49: TLabel;
    Label50: TLabel;
    Label7: TLabel;
    Label45: TLabel;
    Label46: TLabel;
    Label58: TLabel;
    Label71: TLabel;
    dbePrepaidCredit: TwwDBEdit;
    dbeBonusCredit: TwwDBEdit;
    dbeEarntCredit: TwwDBEdit;
    wwDBEdit28: TwwDBEdit;
    dbePointsEarnt: TwwDBEdit;
    dbePointsRedeemed: TwwDBEdit;
    wwDBDateTimePicker1: TwwDBDateTimePicker;
    dbePAvailable: TwwDBEdit;
    wwDBEdit2: TwwDBEdit;
    procedure FormCreate(Sender: TObject);
    procedure dbeMemberKeyPress(Sender: TObject; var Key: Char);
    //procedure PageControlLinksChange(Sender: TObject);
    procedure dbcbActiveChange(Sender: TObject);
    procedure dbcbTitleExit(Sender: TObject);
    procedure dblcSurnameDropDown(Sender: TObject);
    procedure dblcSurnameCloseUp(Sender: TObject; LookupTable, FillTable: TDataSet; modified: Boolean);
    procedure dblcSurnameKeyPress(Sender: TObject; var Key: Char);
    procedure dbnQryMembersInsertClick(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure dbnLinksInsertClick(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
    procedure dblcSurnameEnter(Sender: TObject);
    procedure StateCheck;
    procedure dbgSectionsDblClick(Sender: TObject);
    procedure dbgSectionsExit(Sender: TObject);
    //procedure dblcGroupsChange(Sender: TObject);
    procedure dblcProposerKeyPress(Sender: TObject; var Key: Char);
    procedure dblcProposerDropDown(Sender: TObject );
    procedure dblcProposerCloseUp(Sender: TObject; LookupTable,FillTable: TDataSet; modified: Boolean);
    procedure dblcProposerExit(Sender: TObject);
    procedure dblcSeconderExit(Sender: TObject);
    procedure PropSecChange;
    procedure DispMess( MemberName : shortstring );
    procedure dbnQryMembersCancelClick(Sender: TObject);
    procedure dbnQryMembersDeleteClick(Sender: TObject);
    procedure dbnQryMembersLookFilterDialogClick(Sender: TObject);
    procedure dbnQryMembersLookLastRowChanged(Sender: TObject);
    procedure ClearComps;
    procedure dbnQryMembersLookLastClick(Sender: TObject);
    procedure dbnQryMembersLookNextRowChanged(Sender: TObject);
    procedure dbnQryMembersLookNextClick(Sender: TObject);
    procedure dbgSubsCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont;ABrush: TBrush);
    procedure dbgSectionsCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure dbdtApprovedExit(Sender: TObject);
    procedure dbdtApprovedEnter(Sender: TObject);
    procedure PageControlChange(Sender: TObject);
    procedure dbdtDOBExit(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    //procedure dblcGroupsExit(Sender: TObject);
    //procedure dblcGroupsDropDown(Sender: TObject);
    procedure dbnQryMembersRefreshClick(Sender: TObject);
    procedure dbnLinksRefresh1Click(Sender: TObject);
    procedure RefreshLinks;
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure dblcPostCodeDropDown(Sender: TObject);
    procedure UpdateMultipleLinks ( idx : ShortInt);
    //procedure ShowSubsOnInsert;
    procedure btnGlyphClick(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure dbeMemberExit(Sender: TObject);
    procedure menuDefaultMemberClick(Sender: TObject);
    procedure menuExitClick(Sender: TObject);
    procedure subMenuEventClick(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure menuCopyMemberClick(Sender: TObject);
    procedure menuDeleteMemberClick(Sender: TObject);
    procedure popmenuNumbersOnClick(Sender: TObject);
    function GetPopupMenuNumbersTag : LongInt;
    procedure SetUpButtons( b : Boolean);
    procedure FormResize(Sender: TObject);
    procedure menuAttachedMembersClick(Sender: TObject);
    procedure dbdtExpiresEnter(Sender: TObject);
    procedure dbdtExpiresExit(Sender: TObject);
    procedure menuNextUnUsedNumberClick(Sender: TObject);
    procedure menuAddMemberCardClick(Sender: TObject);
    procedure dbdtDOBChange(Sender: TObject);
    procedure MenuLinksPopup(Sender: TObject);
    procedure DeleteLinks( Sender: TObject );
    function GetMenuLinksDesc(theDAtaSet : TIBDAtaSet; i : ShortInt ) : ShortString ;
    procedure CheckIBDSMembersState;
    procedure lblAddressClick(Sender: TObject);
    procedure lblFaxClick(Sender: TObject);
    procedure dbnInterestsInsertClick(Sender: TObject);
    procedure dbeMemberChange(Sender: TObject);
    procedure dbnSubsRefreshClick(Sender: TObject);
    procedure dbnSubsInsertClick(Sender: TObject);
    procedure dbeSubsAddKeyPress(Sender: TObject; var Key: Char);
    procedure btnLoadPhotoClick(Sender: TObject);
    procedure btnPastePhotoClick(Sender: TObject);
    procedure ImageDblClick(Sender: TObject);
    procedure LoadPhotoIntoImage;
    procedure SavePhoto;
    procedure btnDeletePhotoClick(Sender: TObject);
    procedure ConvertToJpg;
    procedure ClubNameChange;
    procedure dbgInterestsDblClick(Sender: TObject);
    procedure dblcClubIDCloseUp(Sender: TObject; LookupTable,FillTable: TDataSet; modified: Boolean);
    procedure opentables;
    procedure menuSetupMemberNumbersClick(Sender: TObject);
    procedure GetNonActiveOrFinancial;
    procedure menuAdjustPointsCreditClick(Sender: TObject);
    procedure SetupPointsCredit( b : boolean);
    procedure menuResetAdjustPointsCreditClick(Sender: TObject);
    procedure menuExportMemberClick(Sender: TObject);
    procedure ExportAllMembers;
    procedure menuPrintMemberDetailsClick(Sender: TObject);
    procedure menuMemberVisitsClick(Sender: TObject);
    procedure menuPHMTransferClick(Sender: TObject);
    procedure menuClosePreferencesClick(Sender: TObject);
    procedure btnClosePrefsClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure btnUpClick(Sender: TObject);
    procedure btnDownClick(Sender: TObject);
    procedure ApplyDiscount1Click(Sender: TObject);
    procedure menuDeleteDiscountClick(Sender: TObject);
    
  private
    { Private declarations }
    FSearchby : ShortInt;
    FFirstLastNavClick : Boolean;
    FNextPriorNavClick : Boolean;
    FApprovedDate : TDateTime;
    FRecordCountText : String[30];
    NameList : TStringList;
    FpnlWidth : Smallint;
    FSaveMembersLook : LongInt;
  public
    { Public declarations }
    SettingMemberDefaults : boolean; // data module needs to see it
    EventCtr,SectionCtr,SubscriptionCtr,GroupCtr : integer;
    procedure wmSysCommand(var Message : TWMSysCommand); message WM_SYSCOMMAND;
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;
                                        
var
  frmMembersEDIT : TfrmMembersEDIT;
  HeightWidth : THeightWidth;
  
const
   SC_EXPORT_MEMBERS   = $00FF;  // Windows SysCommands start at $F000 - Ours must be lessCopyFilesMsg = 'Copying files';
  
implementation

{$R *.DFM}
uses
    dmMembers,  // StockMaster
    dmMenuMate,  // StockMaster
    MembersLOOK,
    MembersSEARCH,
    MembersCLASSES,
    MembersQUERY,
    MembersLOOKUP,
    rIndividualMember,
    ShowHelp,
    MemberMenu, Password, MembersVisits;     // StockMaster
const   SC_DEFAULT        = $00FF;  // Windows SysCommands start at $F000 - Ours must be less
{ **************************************************************************** }
class function TfrmMembersEDIT.ScreenCode : Integer;
begin Result := Global.scMembersEDIT; end;
{ **************************************************************************** }
class function TfrmMembersEDIT.ScreenName : String;
begin Result := 'Membership Maintenance'; end;
{ **************************************************************************** }
class function TfrmMembersEDIT.ScreenDesc : String;
begin Result := '  Membership Maint Screen'; end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.FormCreate(Sender: TObject);
const SQL1 = 'select NUMBER_ID,DESCRIPTION from M_NUMBERS order by NUMBER_ID';
var s : string[45];
    i : integer;
    temp : TComponent;
    MyPopUpItems : TMenuItem;
    hSystemMenu : hMenu;
    NewItem: TMenuItem;
begin
     inherited;
     hSystemMenu := GetSystemMenu(handle, false);
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_STRING, SC_EXPORT_MEMBERS,'Export All Members' );
     HeightWidth.frmHeight := Self.Height;
     HeightWidth.frmWidth  := Self.Width;
     SetupPointsCredit(PointsPassword);
     // save for dm when groups are deleted that affect the home club
     FpnlWidth := pnlSpace.Width;
     FSaveMembersLook       := 0;
     SettingMemberDefaults  := False;
     PageControl.ActivePage := tsMaintenance;
     FFirstLastNavClick     := False;
     FNextPriorNavClick     := False;
     //KeyPreview := True;
     dbeMember.Text := '';
     dm.ibdsMembers.Open;
     NameList := TStringList.create;
     s := Global.NameFields;
     while pos(',',s) > 0 do begin
        NameList.Add(copy (s,1,pos(',',s) -1));
        system.delete(s,1,pos(',',s));
     end;
     if s <> '' then
        NameList.Add(s);// last one
     FSearchby := -1;
     with stbStatus do  begin
       try
          Panels.BeginUpdate;
          Panels.Add;
          Panels[1].Width := 380;
          Panels[2].Width := 80;
       finally
         Panels.EndUpdate;
       end;
     end;
     // populate the POpMenu with Number ID's
     if dm.sqlExecQueryCheckEoF(dm.qryGeneral,SQL1) then begin
        while not dm.qryGeneral.EOF do begin
           MyPopUpItems := TMenuItem.Create(Self);
           MyPopUpItems.Caption := dm.qryGeneral.FieldByName('DESCRIPTION').AsString;
           MyPopUpItems.Name    := 'popmenu' + dm.qryGeneral.FieldByName('DESCRIPTION').AsString;
           MyPopUpItems.Tag     := dm.qryGeneral.FieldByName('NUMBER_ID').AsInteger;
           // set checked on the first item
           if MyPopUpItems.Caption = 'Normal' then MyPopUpItems.Checked := True;
           MyPopUpItems.OnClick :=  popmenuNumbersOnClick;
           PopUpMenuNumbers.Items.Add(MyPopUpItems);
           dm.qryGeneral.Next;
       end;
     end;
     // turn off the Discounts Tab if MenuMate is not installed and MenuMateDiscountsInstalled is not turned on
     if Global.MenuMateInstalled and  global.MenuMateDiscountsInstalled then begin
        NewItem := TMenuItem.Create(Self);
        NewItem.Caption := 'Discounts';
        NewItem.Name    := 'Discounts';;
        NewItem.OnClick := subMenuEventClick;
        NewItem.Tag     := 4;
        PopupMenu.Items.Add(NewItem);
     end else begin
        tsDiscounts.TabVisible := false;
     end; 
    // Capitals is Default see table MISC
    if not EditCapitals then begin
       for I := ComponentCount - 1 downto 0 do begin
          if Components[i].Tag = 99 then begin
             if Components[i] is TwwDBEdit then 
                TwwDBEdit(Components[i]).CharCase := ecNormal;
             if Components[i] is TwwDBComboBox then 
                TwwDBComboBox(Components[i]).CharCase := ecNormal;
             // also the Find for LowerCase and on all forms 
             if Components[i] is TwwDBLookupCombo then 
                TwwDBLookupCombo(Components[i]).CharCase := ecNormal;
          end;
       end;                          
    end;                          
    if MenuMateTriggerInstalled then 
       pnlDonation.Visible := true
    else   
       pnlDonation.Visible := false;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.FormDestroy(Sender: TObject);
begin
     NameList.Free;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.FormShow(Sender: TObject);
var   MenuMatePointsUpdate : TMenuMatePointsUpdate;
begin
     // get all points
     MenuMatePointsUpdate.MembersPointsUpdate('-1');
      
end;
{ ****************************************************************************}
procedure TfrmMembersEDIT.FormActivate(Sender: TObject);
begin
     Inherited;
     opentables;
     ReUsingMember := False;
     dm.qryMembersLook.Locate('MEMBER',FSaveMembersLook,[]);
     // refresh the screen on entry
     if dm.dsrMembers.State in [dsBrowse] then begin
        dblcSurname.Setfocus;
        if dm.ibdsMembers.RecordCount > 0 then dm.ibdsMembers.Refresh;
        RefreshLinks;
     end;
     // need to refresh the new grid selected lengths
     if Global.DiffRatio < 1.00 then dbnQryMembersRefreshClick(Sender); // 640 * 480
     if not dm.PreCheckLogOn(scMembersTabMaintenance,stMembersTabMaintenance) then RemoveTab(tsMaintenance);
     if not dm.PreCheckLogOn(scMembersTabEventsSections,stMembersTabEventsSections ) then RemoveTab(tsEventsSections);
     if not dm.PreCheckLogOn(scMembersTabGroupsSubs,stMembersTabGroupsSubs ) then RemoveTab(tsGroupsSubs);
     if not dm.PreCheckLogOn(scMembersTabInterests,stMembersTabInterests ) then RemoveTab(tsInterests);
     if not dm.PreCheckLogOn(scMembersTabProposer,stMembersTabProposer ) then RemoveTab(tsProposer);
     if not dm.PreCheckLogOn(scMembersTabOther,stMembersTabOther ) then RemoveTab(tsOther);
     if not dm.PreCheckLogOn(scMembersTabDairy,stMembersTabDairy ) then RemoveTab(tsDiary);
     if not dm.PreCheckLogOn(scMembersTabCreditPoints,stMembersTabCreditPoints ) then RemoveTab(tsCreditPoints);
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.FormDeactivate(Sender: TObject);
begin
     Inherited;
     StateCheck;
     // locate it on FormActivate
     FSaveMembersLook := dm.qryMembersLook.FieldByName('MEMBER').AsInteger;
     dm.IBTransaction.Commit;
     dm.IBTransaction.StartTransaction;
     // here if you are doing the delete Members report
     dm.qryReport.Open;
     dm.IBTransactionCommit; // Commit changes on deactivate
     // Discounts are from MenuMate
     if Global.MenuMateInstalled and  Global.MenuMateDiscountsInstalled then begin
        if dmMM.tblDiscounts.Active = True then
           dmMM.tblDiscounts.Transaction.Commit;
     end;      
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.opentables;
begin
     dm.tblGroups.Active       := True;
     dm.tblSubs.Active         := True;
     dm.tblSections.Active     := True;
     dm.tblEvents.Active       := True;
     dm.ibdsMembers.Active     := True;
     dm.ibdsM_Subs.Active      := True;
     dm.ibdsM_Lots.Active      := True;
     dm.ibdsM_Interests.Active := True;
     dm.ibdsM_Groups.Active    := True;
     dm.ibdsM_Sections.Active  := True;
     dm.ibdsM_Events.Active    := True;
     dm.qryMembersLook.Active  := True;
     dm.qryPropSec.Active      := True;
     dm.tblMailCodes.Active    := True;
     dm.ibdsM_Photos.Active    := True;
     dm.tblClubs.Active        := True;
     dm.tblPrefMembers.Active  := True;
     // if MenuMate Installed and MenuMate DataBase is connected 
     if (global.MenuMateInstalled) and (dmMM.MMConnected) and global.MenuMateDiscountsInstalled then begin
        dmMM.tblDiscounts.Active := True;
        // ibdsM_Discounts looks up fields from tblDiscounts
        dm.ibdsM_Discounts.Active   := True;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.menuExitClick(Sender: TObject);
// save or cancel changes on form close
begin
     close;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbeMemberKeyPress(Sender: TObject; var Key: Char);
// if the user presses Enter from a TWinControl
const SQl1 = 'Select Member,FULL_NAME from members where member = %s';
begin
     if key = '''' then key := '`'; // change ' to `  - for sql
     if key = #13 then begin
        if TwwDBEdit(Sender).Name = dbeMember.Name then begin
           if dm.ibdsMembers.State = dsInsert then begin
              if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[dbeMember.Text])) then begin
                 sm(format('Member %s is being already used by %s',[dbeMember.Text,dm.qryGeneral.FieldByName('FULL_NAME').AsString]));
                 dbeMember.Selectall;
                 exit;
              end;
           end;
        end;
        if TwwDBDateTimePicker(Sender).Name = dbeKnownAs.Name then begin
           PageControl.ActivePageIndex := tsProposer.PageIndex;
           dbdtDOB.SetFocus;
           exit;
        end;
        if TwwDBEdit(Sender).Name = dbeSeconderName.Name then begin
           PageControl.ActivePageIndex := tsOther.PageIndex;
           dbeAnalyse.Setfocus;
           exit;
        end;
        if TwwDBEdit(Sender).Name = dbeCardType.Name then begin
           PageControl.ActivePageIndex := tsGroupsSubs.PageIndex;
           exit;
        end;
        {if TwwDBRichEdit(Sender).Name = DBRichEdit.Name then begin
           if dm.dsrMembers.State = dsInsert then begin
              if DBRichEdit.Lines.Count < 2 then begin
                 PageControl.ActivePageIndex := tsLinks.PageIndex;
                 PageControlLinksChange(Sender);
                 exit;
              end;
           end;
        end; }
        {if TwwDBEdit(Sender).Name = dbePAvailable.Name then begin
           PageControl.ActivePageIndex := tsDiary.PageIndex;
           DBRichEdit.SetFocus;
           exit;
        end;       }
        SelectNext(ActiveControl as TWinControl,True,True);
        key := #0;
     end;
end;
{ **************************************************************************** }
{procedure TfrmMembersEDIT.PageControlLinksChange(Sender: TObject);
begin
     if not active then exit;
     if dbeMember.Text <> '' then begin // if there is no editing
        case PageControlLinks.ActivePageIndex of
        0 : begin  // subs
               dbnLinks.DataSource := dm.dsrM_Subs;
               dbnQryMembersRefreshClick(Sender);
            end;
        1 : begin // sections
               dbnLinks.DataSource := dm.dsrM_Sections;
               dbnQryMembersRefreshClick(Sender);
            end;
        2 : begin // events
               dbnLinks.DataSource := dm.dsrM_Events;
               dbnQryMembersRefreshClick(Sender);
               //dbgEvents.SetFocus;
               //dbgEvents.SetActiveField('Dates');
            end;
        end;
     end else begin
        dbnLinks.DataSource := nil;
     end;
end; }
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbcbActiveChange(Sender: TObject);
// Set the color of the label if an Inactive Member
begin
     //if not active then exit;
    // if dbcbActive.Text = 'NO' then
    //    lblActive.Color := PayColor
    // else
    //    lblActive.Color := clBtnFace;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbcbTitleExit(Sender: TObject);
// if inserting - make up the Fullname by the other name fields and KnownAs from FirstName
var s : string; i : integer;
begin
     if SettingMemberDefaults then exit;
     if dm.dsrMembers.State in [dsInsert,dsEdit] then begin
        s := '';  // make up FullName from NAME_FIELDS in tblMISC
        for i := 0 to NameList.Count -1 do begin
           if POS(NameList[i],dbcbTitle.Name) > 0 then 
              if dbcbTitle.Text <> '' then
                 s := s + trim(dbcbTitle.Text) + ' ';
           if POS(NameList[i],dbeInitials.Name) > 0  then 
              if dbeInitials.Text <> '' then
                 s := s + trim(dbeInitials.Text) + ' ';
           if POS(NameList[i],dbeFirstName.Name) > 0 then 
              if dbeFirstName.Text <> '' then
                 s := s + trim(dbeFirstName.Text) + ' ';
           if POS(NameList[i],dbeSurname.Name) > 0   then 
              if dbeSurname.Text <> '' then
                 s := s + trim(dbeSurname.Text) + ' ';
        end;
        // 
        if NameList.Count > 0 then
           dm.ibdsMembers.FieldByName('FULL_NAME').AsString := s;
        //dm.ibdsMembers.FieldByName('FULL_NAME').AsString := dbcbTitle.Text + ' ' + dbeInitials.Text + ' ' + dbeSurname.Text;
        //dbeParentCodeName.Text := dbcbTitle.Text + ' ' + dbeInitials.Text + ' ' + dbeSurname.Text;
        dbeParentCodeName.Text := s;
        if (TwwDBEdit(Sender).Name = dbeFirstName.Name) and KnownAsDefaulted then
           dm.ibdsMembers.FieldByName('KNOWN_AS').AsString := dbeFirstName.Text;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dblcSurnameDropDown(Sender: TObject);
var s : string;// pre drop down selection
const SQL1 = 'select m.SURNAME,m.MEMBER,m.FULL_NAME,m.FIRST_NAME,m.ADDRESS,M.MAILING_LIST_CODE,M.EXPIRES ';
      SQL2 = 'select MAILING_LIST_CODE from members where MEMBER = %s';
begin
     s := dblcSurname.Text;
     if s = '' then exit;
     s := uppercase(s);
     dm.qryMembersLook.Close;
     try
        case FSearchby of
        // set after subMenuEventClick(Sender) - only a one time look up - set to -1 afterwards
        -1 : begin
            if not global.AllDigits( dblcSurname.Text) then begin

              if s[1] = '1' then begin // search by first name
                 delete(s,1,1);
                 dblcSurname.LookupField := 'FIRST_NAME';
                 dm.qryMembersLook.SQL.Text := Format(SQL1 + ' from MEMBERS m' +
                                          ' where upper(m.FIRST_NAME) LIKE ''%s%s'' ORDER BY m.FIRST_NAME,m.FULL_NAME',[s,'%']);
              end else if (s[1] = 'P') and ( global.AllDigits( copy(s,2,Length(s))) ) then begin // search by first name
                 delete(s,1,1);
                 // get the parent code from the member
                 dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[s]));
                 s := dm.qryGeneral.FieldByName('MAILING_LIST_CODE').AsString;
                 dblcSurname.LookupField := 'MAILING_LIST_CODE';
                 dm.qryMembersLook.SQL.Text := Format(SQL1 + ' from MEMBERS m' +
                                          ' where m.MAILING_LIST_CODE = %s ORDER BY m.MAILING_LIST_CODE,m.MEMBER',[s]);

              end else begin
                 dblcSurname.LookupField := 'SURNAME';
                 dm.qryMembersLook.SQL.Text := Format(SQl1 + ' from MEMBERS m' +
                                          ' where upper(m.SURNAME) LIKE ''%s%s'' ORDER BY m.SURNAME,m.FULL_NAME',[s,'%']);
              end;
            end else begin
              dblcSurname.LookupField := 'MEMBER';
              dm.qryMembersLook.SQL.Text := Format(SQL1 + ' from MEMBERS m' +
                                          ' where m.MEMBER = %d ORDER BY m.MEMBER',[StrToIntdef(s,0)]);
            end;
            end;
        0 : begin // Event
            dblcSurname.LookupField := 'MEMBER';
            dm.qryMembersLook.SQL.Text := Format(SQL1 + ',e.EVENT from MEMBERS m,M_EVENTS e ' +
                                          'where m.MEMBER = e.MEMBER ' +
                                          'and e.EVENT = %d ORDER BY M.MEMBER',[StrToIntdef(s,0)]);

            end;
        1 : begin // Group
            dblcSurname.LookupField := 'MEMBER';
            dm.qryMembersLook.SQL.Text := Format(SQl1 + ' from MEMBERS m, M_GROUPS mg ' +
                                          'where m.MEMBER = mg.MEMBER ' +
                                          'and mg.GROUPS = %d ORDER BY m.MEMBER',[StrToIntdef(s,0)]);

            end;
        2 : begin // Section
            dblcSurname.LookupField := 'MEMBER';
            dm.qryMembersLook.SQL.Text := Format(SQL1 + ',s.SECTION from MEMBERS m,M_SECTIONS s ' +
                                          'where m.MEMBER = s.MEMBER ' +
                                          'and s.SECTION = %d ORDER BY M.MEMBER',[StrToIntdef(s,0)]);
            end;
        3 : begin // Subs
            dblcSurname.LookupField := 'MEMBER';
            dm.qryMembersLook.SQL.Text := Format(SQL1 + ',s.SUBSCRIPTION from MEMBERS m,M_SUBS s ' +
                                          'where m.MEMBER = s.MEMBER ' +
                                          'and s.SUBSCRIPTION = %d ORDER BY M.MEMBER',[StrToIntdef(s,0)]);
            end;
        4 : begin // Discounts
            dblcSurname.LookupField := 'MEMBER';
            dm.qryMembersLook.SQL.Text := Format(SQL1 + ',s.DISCOUNT_KEY from MEMBERS m,MEMBERS_DISCOUNTS s ' +
                                          'where m.MEMBER = s.MEMBER ' +
                                          'and s.DISCOUNT_KEY = %d ORDER BY M.MEMBER',[StrToIntdef(s,0)]);
            end;
        end; // CASE
     finally
        dm.qryMembersLook.Open;
        dm.qryMembersLook.FetchAll;              
        stbStatus.Panels[2].Text := Format( '%d Records',[dm.qryMembersLook.RecordCount] );
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dblcSurnameCloseUp(Sender: TObject; LookupTable,
  FillTable: TDataSet; modified: Boolean);
// drop down list choosen
begin
     if modified then begin
        dm.MembersDetailsPopulateGrid(EventCtr,SectionCtr,SubscriptionCtr,GroupCtr);
        PropSecChange;
        dbcbTitle.SetFocus;
        dblcSurname.Clear;
        dbgSubs.Update;
        RefreshLinks;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dblcSurnameKeyPress(Sender: TObject;  var Key: Char);
// enter key pressed - with out the drop down arrow pressed
begin
    if key = '''' then key := '`'; // change ' to `  - for sql
    if key = #13 then begin
       dblcSurnameDropDown(Sender);
       if dm.ibdsMembers.EOF then begin
          clearcomps;
          showmessage(Format('%s Not a valid Member or no Members that match the search',[dblcSurname.Text]));
       end else begin
          dm.MembersDetailsPopulateGrid(EventCtr,SectionCtr,SubscriptionCtr,GroupCtr);
          PropSecChange;
          dbgSubs.Update;
          if dbeMember.ReadOnly then
             dbcbTitle.SetFocus
          else
             dbeMember.SetFocus;
       end;
       FSearchby := -1;
       RefreshLinks;
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dblcSurnameEnter(Sender: TObject);
begin
     StateCheck;
     if not dm.CheckGroupsSubs  then begin
        dbcbTitle.SetFocus;
        abort;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnQryMembersInsertClick(Sender: TObject);
// new record generated in ibdsMembersAfterInsert
begin
     if SettingMemberDefaults then begin
        sm('You cannot add to the Default Member');
        abort;
        exit;
     end;
     if not dm.CheckGroupsSubs  then abort;
     // also used for copy of Members (set to -1 for the Default Member)
     dm.ibdsMembersDef.ParamByName('MEMBER').AsInteger := -1;
     dm.ResetMembersDef;
     FRecordCountText := stbStatus.Panels[2].Text;
     stbStatus.Panels[2].Text := '';
     // the MUNBER_ID is stored in the popupMenu's tag field
     Global.NextMemberIndex := GetPopupMenuNumbersTag;
     PageControl.ActivePage := tsMaintenance;
     CurrentNumberTypes := DefaultNumberTypes;
     // does an edit instead
     if CurrentNumberTypes in [ntNonActive,ntNonFinancial,ntNonFinancialNoPoints] then begin
        GetNonActiveOrFinancial;
        abort;
        exit;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnQryMembersRefreshClick(Sender: TObject);
var MenuMatePointsUpdate : TMenuMatePointsUpdate;
begin         
     if SettingMemberDefaults then begin
        abort;
        exit;
     end;   
     RefreshLinks;
     // dont refresh for new Members 
     // and tries to post the records
     if dm.dsrMembers.State in [dsInsert,dsEdit] then 
        //dm.ibdsMembers.Refresh
     else begin  
        MenuMatePointsUpdate.MembersPointsUpdate(dm.ibdsMembers.FieldByName('MEMBER').AsString);
        dm.ibdsMembers.Refresh;
     end;     
end;     
{ **************************************************************************** }
procedure TfrmMembersEDIT.RefreshLinks;
begin
     dm.ibdsM_Events.Close;
     dm.ibdsM_Events.Open;
     dm.ibdsM_Sections.Close;
     dm.ibdsM_Sections.Open;
     dm.ibdsM_Subs.Close;
     dm.ibdsM_Subs.Open;
     dm.ibdsM_Groups.Close;
     dm.ibdsM_Groups.Open;
     dm.ibdsM_Lots.Close;
     dm.ibdsM_Lots.Open;
     if Global.MenuMateInstalled and  Global.MenuMateDiscountsInstalled then begin
        dm.ibdsM_Discounts.Close;
        dm.ibdsM_Discounts.Open;
     end;   
end;               
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnQryMembersDeleteClick(Sender: TObject);
// delete the links tables first - if not sucessful then abort
var mem : longint;
const ThereAreMsg = 'There are Members who have ';
      SQL1 = 'SELECT COUNT(MEMBER) FROM MEMBERS WHERE %s = %d AND MEMBER <> %d';
      SQL2 = 'SELECT MEMBER FROM MEMBERS WHERE DONATION_MEMBER = %d';
begin
     if SettingMemberDefaults then begin
        sm('You cannot delete from the Default Member');
        abort;
        exit;
     end;
     if MessageDlg('Delete Record?',mtConfirmation,[mbOK,mbCancel],0) = mrCancel then begin
        abort;
        exit;
     end;    
     {if dm.ibdsmembers.FieldByName('IS_ACTIVE').AsString = 'Y' then begin
        if not IfMsgDlg('Member is Financial, do you wish to continue?') then begin
           abort;
           exit;
        end;
     end;}
     mem := dm.ibdsMembers.FieldByName('MEMBER').AsInteger;
     {if dm.sqlExecQueryCheckEOFCount(dm.qryGeneral,format(SQL2,['SECONDER' ,mem ])) then begin
        sm(format('%s %d as their Seconder - remove first',[ThereAreMsg,mem]));
        abort;
     end;
     if dm.sqlExecQueryCheckEOFCount(dm.qryGeneral,format(SQL2,['PROPOSER' ,mem ])) then begin
        sm(format('%s %d as their Proposer - remove first',[ThereAreMsg,mem]));
        abort;
     end; }
     if dm.sqlExecQueryCheckEOFCount(dm.qryGeneral,format(SQL1,['MAILING_LIST_CODE' ,mem,mem ])) then begin
        sm(format('%s %d as their ParentCode - remove first before deletion',[ThereAreMsg,mem]));
        abort;
     end; 
     // Doantion Member
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL2,[mem])) then begin
        sm(format('%s %d as their Donation Member - remove first before deletion',[ThereAreMsg,mem]));
        abort;
     end; 
     try
         dm.DeleteMemberLinks(mem,dm.qryGeneral);
     except
        dm.IBRollBackRetain;
        showmessage('Unable to delete from the link tables, delete not successful');
        abort;
     end;
     if ifMsgDlg(PrintDetailsMsg) then begin
        MemberDeleteReport(dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
     end;  
     PropSecChange; 
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnQryMembersCancelClick(Sender: TObject);
var i : integer;
begin
     // does not cancel Links changes
     stbStatus.Panels[2].Text := FRecordCountText;
     popmenuNumbersOnClick(PopupMenuNumbers.Items[0]);     
     // change the date formats back
     for I := ComponentCount - 1 downto 0 do begin
         if Components[i] is TwwDBDateTimePicker then 
           (Components[i] as TwwDBDateTimePicker).DisplayFormat := 'dd mmm yyyy'; 
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnLinksInsertClick(Sender: TObject);
// Insert, edit and delete  all put the focus into the grid
var i : shortint;
    OK : Boolean;
begin
     {if dbeMember.Text = '' then begin
        abort;
        exit;
     end;
     if dm.ibdsMembers.State = dsInsert then begin
        // post before making changes to Links tables
        dm.ibdsMembers.Post;
        RefreshLinks;
        // dbgSectionsDblClick(Sender) done in dm.ibdsMembers.AfterPost 
        if PageControlLinks.ActivePageIndex = 0 then exit; 
     end;
     case PageControlLinks.ActivePageIndex of
     0 : begin // subs
         //dbgSubs.SetFocus;
         //dbgSubs.SetActiveField('AMOUNT');
         end;
     1 : begin // sections
         //dbgSections.SetFocus;
         //dbgSections.SetActiveField('AMOUNT');
         end;
     2 : begin  // events
         dbgEvents.SetFocus;
         dbgEvents.SetActiveField('DATE');
         end;
     end;
     if TwwNavButton(Sender).Name = dbnLinksInsert.Name then begin
        frmMembersLookup := TfrmMembersLOOKUP.create(nil);
        frmMembersLookup.LookupOnly := False;
        try
           case PageControlLinks.ActivePageIndex of  // before Insert caters for duplicate entries
              0 : frmMembersLookup.Setup( dm.dsrTblSubs,dm.tblSubs,         // subs
                        'SUBSCRIPTION',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
              1 : frmMembersLookup.Setup( dm.dsrTblSections,dm.tblSections, // sections
                        'SECTION',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
              2 : frmMembersLookup.Setup( dm.dsrTblEvents,dm.tblEvents,     // events
                        'EVENT',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
           end;
           OK := False;   
           // all items added into the multilist
           if frmMembersLookup.ShowModal = mrOK then OK := True;
           abort; 
        finally
           if OK and 
              (frmMembersLookup.MultiList.Count > 0) then UpdateMultipleLinks;
           frmMembersLOOKup.Free;
           PageControl.Update; // not repainting after frmMembersLOOK          
           dbgEvents.Update;
           dbgSections.Update;
           dbgSubs.Update;
           Application.ProcessMessages;
        end;
     end;   }
end;
{ **************************************************************************** }
procedure TfrmMembersEdit.UpdateMultipleLinks ( idx : ShortInt);
var i : shortint;
    v : variant;
begin
     for i:= 0 to frmMembersLookup.MultiList.Count-1 do begin
        v := StrToInt(frmMembersLookup.MultiList[i]);
        case idx of  
           0 : begin dm.tblSubs.Locate('SUBSCRIPTION',v,[]); dm.ibdsM_Subs.Insert; end;
           1 : begin dm.tblSections.Locate('SECTION',v,[]); dm.ibdsM_Sections.Insert; end;
           2 : begin dm.tblEvents.Locate('EVENT',v,[]);  dm.ibdsM_Events.Insert; end;
           3 : begin dm.tblGroups.Locate('GROUPS',v,[]);  dm.ibdsM_Groups.Insert; end;
           4 : begin dmMM.tblDiscounts.Locate('DISCOUNT_KEY',v,[]); dm.ibdsM_Discounts.Insert; end;
        end;
     end;        
end;
{ **************************************************************************** }

procedure TfrmMembersEDIT.dbnLinksRefresh1Click(Sender: TObject);
begin
     dbnQryMembersRefreshClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbgSectionsDblClick(Sender: TObject);
// lookup table - returns false if user pressed the cancel button
// post before lookup, checks if there is an entry already
var i: integer;
    idx : shortInt;
begin
     if dbeMember.Text = '' then exit;
     idx := TwwDBGrid(sender).Tag;
     try
        frmMembersLookup := TfrmMembersLOOKUP.create(nil);
        case idx of // need to Post / Insert as not called from Nav
           0 : begin // subs // post before next selection, calls IBCommitRetain;
                  if dm.dsrM_Subs.State in [dsEdit,dsInsert] then dm.ibdsM_Subs.Post;
                  frmMembersLookup.Setup( dm.dsrTblSubs,dm.tblSubs,         // subs
                        'SUBSCRIPTION',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
                  if (frmMembersLookup.ShowModal = mrOK) then UpdateMultipleLinks(idx);
               end;
           1 : begin  // sections // post before next selection, calls IBCommitRetain;
                  if dm.dsrM_Sections.State in [dsEdit,dsInsert] then dm.ibdsM_Sections.Post;
                  frmMembersLookup.Setup( dm.dsrTblSections,dm.tblSections, // sections
                        'SECTION',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
                  if (frmMembersLookup.ShowModal = mrOK) then UpdateMultipleLinks(idx);
               end;
           2 : begin // events  // post before next selection, calls IBCommitRetain;
                  if dm.dsrM_Events.State in [dsEdit,dsInsert] then dm.ibdsM_Events.Post;  
                          frmMembersLookup.Setup( dm.dsrTblEvents,dm.tblEvents,     // events
                        'EVENT',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
                  if (frmMembersLookup.ShowModal = mrOK) then UpdateMultipleLinks(idx);

               end;   
           3 : begin // Groups  // post before next selection, calls IBCommitRetain;
                  if dm.dsrM_Groups.State in [dsEdit,dsInsert] then dm.ibdsM_Groups.Post;
                  frmMembersLookup.Setup( dm.dsrTblGroups,dm.tblGroups,     // grousp
                        'GROUPS',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
                  if (frmMembersLookup.ShowModal = mrOK) then UpdateMultipleLinks(idx);
                  
               end;   
           4 : begin // Discounts  // post before next selection, calls IBCommitRetain;
                  if dm.dsrM_Discounts.State in [dsEdit,dsInsert] then dm.ibdsM_Discounts.Post;
                  frmMembersLookup.Setup( dmMM.dsrDiscounts,dmMM.tblDiscounts,    
                        'DISCOUNT_KEY',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
                  if (frmMembersLookup.ShowModal = mrOK) then UpdateMultipleLinks(idx);
                  
               end;   
            end;
     finally
        frmMembersLookup.Release;
        RefreshLinks;
        PageControl.Update; // not repainting after frmMembersLOOK
        dbgSubs.Update; // not repainting after frmMembersLOOK
        dbgSections.Update; // not repainting after frmMembersLOOK
        dbgEvents.Update; // not repainting after frmMembersLOOK
        dbgDiscounts.Update;
        Application.ProcessMessages;
     end;
end;
{ **************************************************************************** }
{ **************************************************************************** }
procedure TfrmMembersEDIT.StateCheck;
// called before form inactivation and going back into search mode
begin
     // default Member
     if dm.ibdsMembersDef.State = dsEdit then begin
        dm.ibdsMembersDef.Cancel;
     end;
     if dm.dsrMembers.State in [dsEdit,dsInsert] then begin
        if IfMsgdlg('Do you wish to save the changes?') then begin
           dm.ibdsMembers.Post; // does CommitRetain in afterpost event
        end else begin
           if dm.dsrMembers.State = dsInsert then begin
              dm.IBRollBackRetain;
              dm.ResetMemberNumber;
              dm.IBCommitRetain;
           end;
           dm.ibdsMembers.Cancel;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbgSectionsExit(Sender: TObject);
// post on exit from the grid, calls IBCommitRetain;
begin
     if TwwDBGrid(Sender).Name = dbgSections.Name then begin
        if dm.dsrM_Sections.State in [dsEdit,dsInsert] then
           dm.ibdsM_Sections.Post;
     end;
     if TwwDBGrid(Sender).Name = dbgEvents.Name then begin
        if dm.dsrM_Events.State in [dsEdit,dsInsert] then
           dm.ibdsM_Events.Post;
     end;
     if TwwDBGrid(Sender).Name = dbgSubs.Name then begin
        if dm.dsrM_Subs.State in [dsEdit,dsInsert] then
           dm.ibdsM_Subs.Post;
     end;
     if TwwDBGrid(Sender).Name = dbgDiscounts.Name then begin
        if dm.dsrM_Discounts.State in [dsEdit,dsInsert] then
           dm.ibdsM_Discounts.Post;
     end;
     
end;
{ **************************************************************************** }
{procedure TfrmMembersEDIT.dblcGroupsChange(Sender: TObject);
var FVariant : Variant;
// search for the group
begin
     if not active then exit;
     if dblcGroups.Text = '' then begin
        dbeGroupDesc.Text := '';
        exit;
     end;
     FVariant := StrToIntDef(dblcGroups.Text,0);
     if dm.tblGroups.locate('GROUPS',FVariant,[]) then
        dbeGroupDesc.Text := dm.tblGroups.FieldByName('DESCRIPTION').AsString
     else
        dbeGroupDesc.Text := '';
end; }
{ **************************************************************************** }
procedure TfrmMembersEDIT.dblcProposerKeyPress(Sender: TObject;
  var Key: Char);
begin
    if key = #13 then begin
      // - just passing thru
      if dm.dsrMembers.State = dsBrowse then begin
         SelectNext(ActiveControl as TWinControl,True,True);
         exit;
      end;
      if SettingMemberDefaults then exit;
      // if inserting then cannot lookup member to put Parent Code
      if TwwDbLookupCombo(Sender).Name = dblcParentCode.Name then begin
         if dblcParentCode.Text = '' then begin
            SelectNext(ActiveControl as TWinControl,True,True);
            exit;
         end;
         // if insert it cant look up itself - not there yet
         if (dm.dsrMembers.State = dsInsert) and (dbeMember.Text = dblcParentCode.Text) then begin
            dbeParentCodeName.Text := dbeFullName.Text;
         end else if dblcParentCode.Modified then begin
            dblcProposerDropDown(Sender);
            if dm.qryPropSec.EOF then begin
               showmessage(NotAValidMemMsg);
               exit;
            end else begin
               dm.ibdsMembers.FieldByName('MAILING_LIST_CODE').AsInteger := dm.qryPropSec.FieldByName('MEMBER').AsInteger;
               dbeParentCodeName.Text := dm.qryPropSec.FieldByName('FULL_NAME').AsString;
            end;
         end;
      end;
      if TwwDbLookupCombo(Sender).Name = dblcProposer.Name then begin
         if dblcProposer.Text = '' then begin
            SelectNext(ActiveControl as TWinControl,True,True);
            exit;
         end;
         if (dm.dsrMembers.State = dsInsert) or (dblcProposer.Modified) then begin
            if dblcProposer.Text = dbeMember.Text then begin
                DispMess('Current');
                dm.ibdsMembers.FieldByName('PROPOSER').Clear;
                dm.ibdsMembers.FieldByName('PROPOSER_NAME').Clear;
                //dbeProposerName.Text := '';
            end else begin
               dblcProposerDropDown(Sender);
               if dm.qryPropSec.EOF then begin
                  showmessage(NotAValidMemMsg);
                  exit;
               end else begin
                  dm.ibdsMembers.FieldByName('PROPOSER').AsInteger := dm.qryPropSec.FieldByName('MEMBER').AsInteger;
                  dm.ibdsMembers.FieldByName('PROPOSER_NAME').AsString := dm.qryPropSec.FieldByName('FULL_NAME').AsString;
                  //dbeProposerName.Text := dm.qryPropSec.FieldByName('FULL_NAME').AsString;
               end;
            end;
          end;
       end;
       if TwwDbLookupCombo(Sender).Name = dblcSeconder.Name then begin
         if dblcSeconder.Text = '' then begin
            SelectNext(ActiveControl as TWinControl,True,True);
            exit;
         end;
         if (dm.dsrMembers.State = dsInsert) or (dblcSeconder.Modified) then begin
            if dblcSeconder.Text = dbeMember.Text then begin
                DispMess('Current');
                dm.ibdsMembers.FieldByName('SECONDER').Clear;
                dm.ibdsMembers.FieldByName('SECONDER_NAME').Clear;
                //dbeSeconderName.Text := '';
            end else begin
               dblcProposerDropDown(Sender);
               if dm.qryPropSec.EOF then begin
                  showmessage(NotAValidMemMsg);
                  exit;
               end else begin
                  dm.ibdsMembers.FieldByName('SECONDER').AsInteger := dm.qryPropSec.FieldByName('MEMBER').AsInteger;
                  dm.ibdsMembers.FieldByName('SECONDER_NAME').AsString := dm.qryPropSec.FieldByName('FULL_NAME').AsString;
                  //dbeSeconderName.Text := dm.qryPropSec.FieldByName('FULL_NAME').AsString;
               end;
            end;
          end;
       end;
       if TwwDbLookupCombo(Sender).Name = dblcDonation.Name then begin
         if dblcDonation.Text = '' then begin
            SelectNext(ActiveControl as TWinControl,True,True);
            exit;
         end;
         if (dm.dsrMembers.State = dsInsert) or (dblcDonation.Modified) then begin
            if dblcDonation.Text = dbeMember.Text then begin
                DispMess('Current');
                dm.ibdsMembers.FieldByName('DONATION_MEMBER').Clear;
                dbeDonationName.Text := '';
            end else begin
               dblcProposerDropDown(Sender);
               if dm.qryPropSec.EOF then begin
                  showmessage(NotAValidMemMsg);
                  exit;
               end else begin
                  dm.ibdsMembers.FieldByName('DONATION_MEMBER').AsInteger := dm.qryPropSec.FieldByName('MEMBER').AsInteger;
                  dbeDonationName.Text := dm.qryPropSec.FieldByName('FULL_NAME').AsString;
               end;
            end;
          end;
       end;
       if TwwDbLookupCombo(Sender).Name = dblcClubID.Name then begin
          if (dblcClubID.Text = '') then
              dblcClubID.Text := '0';
          if dblcClubID.Text = '0' then
              dbeHomeClub.Text := '';
          ClubNameChange;
       end;
       SelectNext(ActiveControl as TWinControl,True,True);
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dblcProposerDropDown(Sender: TObject);
var s : string;
const SQL1 = 'select SURNAME,MEMBER,FULL_NAME,FIRST_NAME,ADDRESS from MEMBERS ';
begin
     if SettingMemberDefaults then exit;
     if TwwDbLookupCombo(Sender).Name = dblcProposer.Name then s := dblcProposer.Text;
     if TwwDbLookupCombo(Sender).Name = dblcSeconder.Name then s := dblcSeconder.Text;
     if TwwDbLookupCombo(Sender).Name = dblcParentCode.Name then s := dblcParentCode.Text;
     if TwwDbLookupCombo(Sender).Name = dblcDonation.Name then s := dblcDonation.Text;
     
           // dont bother looking up - its already there - Insert mode - is not there
        //if dbeMember.Text = dblcParentCode.Text then begin
        //   dbeParentCodeName.Text := dbeFullName.Text;
        //   exit;
        //end;
     if s = '' then exit;
     dm.qryPropSec.Close;
     if not global.AllDigits( s ) then begin
        if s[1] = '1' then begin // search by first name
           delete(s,1,1);
           //dblcSurname.LookupField := 'FIRST_NAME';
           dm.qryPropSec.SQL.Text := Format(SQL1 + ' where upper(FIRST_NAME) LIKE ''%s%s'' ORDER BY FIRST_NAME,FULL_NAME',[s,'%']);
        end else begin
           //dblcSurname.LookupField := 'SURNAME';
           dm.qryPropSec.SQL.Text := Format(SQL1 + ' where upper(SURNAME) LIKE ''%s%s'' ORDER BY SURNAME,FULL_NAME',[s,'%']);
        end;
     end else begin
        //dblcSurname.LookupField := 'MEMBER';
        dm.qryPropSec.SQL.Text := Format(SQL1 + ' where MEMBER = %d',[StrToIntdef(s,0)]);
     end;
     dm.qryPropSec.Open;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dblcProposerCloseUp(Sender: TObject; LookupTable,
  FillTable: TDataSet; modified: Boolean);
// make sure the Proposer + Seconder names are filled in correctly
begin
     if SettingMemberDefaults then exit;
     if modified then begin
          if TwwDbLookupCombo(Sender).Name = dblcProposer.Name then begin
             if dm.qryPropSec.FieldByName('MEMBER').AsString = dbeMember.Text then begin
                DispMess('Current');
                exit;
             end;
             dm.ibdsMembers.FieldByName('PROPOSER').AsInteger := dm.qryPropSec.FieldByName('MEMBER').AsInteger;
             dm.ibdsMembers.FieldByName('PROPOSER_NAME').AsString := dm.qryPropSec.FieldByName('FULL_NAME').AsString;
             //dbeProposerName.Text := dm.qryPropSec.FieldByName('FULL_NAME').AsString;
          end;
          if TwwDbLookupCombo(Sender).Name = dblcSeconder.Name then begin
             if dm.qryPropSec.FieldByName('MEMBER').AsString = dbeMember.Text then begin
                DispMess('Current');
                exit;
             end;
             dm.ibdsMembers.FieldByName('SECONDER').AsInteger := dm.qryPropSec.FieldByName('MEMBER').AsInteger;
             dm.ibdsMembers.FieldByName('SECONDER_NAME').AsString := dm.qryPropSec.FieldByName('FULL_NAME').AsString;
            // dbeSeconderName.Text := dm.qryPropSec.FieldByName('FULL_NAME').AsString;
          end;
          if TwwDbLookupCombo(Sender).Name = dblcDonation.Name then begin
             if dm.qryPropSec.FieldByName('MEMBER').AsString = dbeMember.Text then begin
                DispMess('Current');
                exit;
             end;
             dm.ibdsMembers.FieldByName('DONATION_MEMBER').AsInteger := dm.qryPropSec.FieldByName('MEMBER').AsInteger;
             dbeDonationName.Text := dm.qryPropSec.FieldByName('FULL_NAME').AsString;
          end;
          if TwwDbLookupCombo(Sender).Name = dblcParentCode.Name then begin
             dm.ibdsMembers.FieldByName('MAILING_LIST_CODE').AsInteger := dm.qryPropSec.FieldByName('MEMBER').AsInteger;
             dbeParentCodeName.Text := dm.qryPropSec.FieldByName('FULL_NAME').AsString;
          end;
          
          SelectNext(ActiveControl as TWinControl,True,True);
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.PropSecChange;
const SQL1 = 'select FULL_NAME from MEMBERS where MEMBER = %s';
// make sure the Proposer + Seconder + ParentCode names are filled in correctly
begin
     dbeParentCodeName.Text := '';
     if (dblcParentCode.Text <> '') and  (global.AllDigits( dblcParentCode.Text)) then begin
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[dblcParentCode.Text])) then
           dbeParentCodeName.Text := dm.qryGeneral.FieldByName('FULL_NAME').AsString;
     end;
     //dbeProposerName.Text := '';
     //if (dblcProposer.Text <> '') and  (global.AllDigits( dblcProposer.Text)) then begin
     //   if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[dblcProposer.Text])) then
    //       dbeProposerName.Text := dm.qryGeneral.FieldByName('FULL_NAME').AsString;
    // end;
    // dbeSeconderName.Text := '';
    // if (dblcSeconder.Text <> '') and  (global.AllDigits( dblcSeconder.Text)) then begin
    //    if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[dblcSeconder.Text])) then
    //       dbeSeconderName.Text := dm.qryGeneral.FieldByName('FULL_NAME').AsString;
    // end;
     dbeDonationName.Text := '';
     if (dblcDonation.Text <> '') and  (global.AllDigits( dblcDonation.Text)) then begin
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[dblcDonation.Text])) then
           dbeDonationName.Text := dm.qryGeneral.FieldByName('FULL_NAME').AsString;
     end;
     ClubNameChange;
     LoadPhotoIntoImage;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.ClubNameChange;
const SQL2 = 'select DESCRIPTION from CLUBS where CLUB_ID = %s';
begin
     dbeHomeClub.Text := '';
     if (dblcClubID.Text <> '') and  (global.AllDigits( dblcClubID.Text)) then begin
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL2,[dblcClubID.Text])) then
           dbeHomeClub.Text := dm.qryGeneral.FieldByName('DESCRIPTION').AsString;
     end else begin
        dbeHomeClub.Text := '';
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dblcProposerExit(Sender: TObject);
// check to see if the member/Proposer are the same and Proposer/Seconder
begin
     if dbeMember.Text = '' then exit;
     if dblcProposer.Text = dbeMember.Text then DispMess('Current');
     if dblcSeconder.Text = '' then exit;
     if dblcSeconder.Text = dblcProposer.Text then DispMess('Seconder');
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dblcSeconderExit(Sender: TObject);
// check to see if the member/Proposer are the same and Proposer/Seconder
begin
     if dbeMember.Text = '' then exit;
     if dblcSeconder.Text = dbeMember.Text then DispMess('Current');
     if dblcProposer.Text = '' then exit;
     if dblcSeconder.Text = dblcProposer.Text then DispMess('Proposer');
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.DispMess( MemberName : shortstring );
begin
     showmessage(Format('Cannot be the same as the %s member',[MemberName]));
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnQryMembersLookFilterDialogClick(Sender: TObject);
var s : string;
const SQL1 = 'select SURNAME,MEMBER,FULL_NAME,FIRST_NAME,ADDRESS,MAILING_LIST_CODE,EXPIRES ';
begin
     if not dm.CheckGroupsSubs  then exit;
     try
        frmMembersQUERY := TfrmMembersQUERY.Create(self);
        frmMembersQUERY.MembersOnly := True;
        if (frmMembersQUERY.ShowModal = mrOK) then begin
           s := dm.qryQuery.SQL.Text;
           if s = '' then exit;
           s := SQL1 + copy(s,POS('*',s) + 1,Length(s));
           with dm.qryMembersLook do begin
              Close;
              SQL.Text := s; 
              Open;
              if RecordCount = 0 then begin
                 ClearComps;
                 showmessage('There are no Records that match the filter');
                 dblcSurname.SetFocus;
                 stbStatus.Panels[2].Text := '0 Records';
                 exit;
              end else begin
                 dm.qryMembersLook.FetchAll;
                 stbStatus.Panels[2].Text := format('%d Records',[dm.qryMembersLook.RecordCount]);
              end;
           end;
        end;
     finally
        frmMembersQUERY.Free;
     end;   
     dm.MembersDetailsPopulateGrid(EventCtr,SectionCtr,SubscriptionCtr,GroupCtr);
     dbeSurname.SetFocus;
     dblcSurname.Clear;
     PropSecChange;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnQryMembersLookNextClick(Sender: TObject);
begin
     if not dm.CheckGroupsSubs  then abort;
     FNextPriorNavClick := True;
     RefreshLinks;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnQryMembersLookNextRowChanged(Sender: TObject);
// next and prior buttons clicked - seems to take 2 clicks on first/last record
begin
     if not FNextPriorNavClick then exit;
     FNextPriorNavClick := False;
     if dm.qryMembersLook.EOF or dm.qryMembersLook.BOF then exit;
     if  dm.qryMembersLook.FieldByName('MEMBER').AsInteger = dm.ibdsMembers.FieldByName('MEMBER').AsInteger then exit;
     dm.MembersDetailsPopulateGrid(EventCtr,SectionCtr,SubscriptionCtr,GroupCtr);
     PropSecChange;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnQryMembersLookLastClick(Sender: TObject);
begin
     if not dm.CheckGroupsSubs  then abort;
     FFirstLastNavClick := True;
     RefreshLinks;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnQryMembersLookLastRowChanged(Sender: TObject);
// first and last buttons clicked - use row change because nav click is pre move
begin
     if not FFirstLastNavClick then exit;
     FFirstLastNavClick := False;
     dm.MembersDetailsPopulateGrid(EventCtr,SectionCtr,SubscriptionCtr,GroupCtr);
     PropSecChange;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.ClearComps;
var
  I: Integer;
  Temp: TComponent;
begin
     for I := ComponentCount - 1 downto 0 do begin
       Temp := Components[I];
       if (Temp is TwwDBEdit) then (Temp as TwwDBEdit).Clear;
       if (Temp is TwwDBDateTimePicker) then (Temp as TwwDBDateTimePicker).Clear;
       if (Temp is TwwDBComboBox) then (Temp as TwwDBComboBox).Clear;
       if (Temp is TwwDBLookupCombo) then (Temp as TwwDBLookupCombo).Clear;
       if (Temp is TImage) then (Temp as TImage).Picture.Assign(nil);
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.subMenuEventClick(Sender: TObject);
var s : shortstring;
    Desc : String[12];
begin
     if not dm.CheckGroupsSubs  then exit;
     FSearchby := TMenuItem(Sender).Tag;
     try
        frmMembersLookup := TfrmMembersLOOKUP.create(nil);
        frmMembersLookup.LookupOnly := True;
        case FSearchby of
        0 : begin    // event
               frmMembersLookup.Setup( dm.dsrTblEvents,dm.tblEvents,     // events
                        'EVENT',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
               if (frmMembersLookup.ShowModal = mrOK) then begin
                  clearcomps;
                  dblcSurname.Text := dm.tblEvents.FieldByName('EVENT').AsString;
                  s := dm.tblEvents.FieldByName('EVENT').AsString;
                  dblcSurnameDropDown(Sender);
                end else exit;
            end;
        1 : begin   // group
               frmMembersLookup.Setup( dm.dsrTblGroups,dm.tblGroups,     // groups
                        'GROUPS',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
               if (frmMembersLookup.ShowModal = mrOK) then begin
                  clearcomps;
                  dblcSurname.Text := dm.tblGroups.FieldByName('GROUPS').AsString;
                  s := dm.tblGroups.FieldByName('GROUPS').AsString;
                  dblcSurnameDropDown(Sender);
               end else exit;
            end;
        2 : begin // section
               frmMembersLookup.Setup( dm.dsrTblSections,dm.tblSections, // sections
                        'SECTION',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
               if (frmMembersLookup.ShowModal = mrOK) then begin
                  clearcomps;
                  dblcSurname.Text := dm.tblSections.FieldByName('SECTION').AsString;
                  s := dm.tblSections.FieldByName('SECTION').AsString;
                  dblcSurnameDropDown(Sender);
              end else exit;
         end;
        3 : begin // Subs
              frmMembersLookup.Setup( dm.dsrTblSubs,dm.tblSubs,         // subs
                        'SUBSCRIPTION',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
              if (frmMembersLookup.ShowModal = mrOK) then begin
                  clearcomps;
                  dblcSurname.Text := dm.tblSubs.FieldByName('SUBSCRIPTION').AsString;
                  s := dm.tblSubs.FieldByName('SUBSCRIPTION').AsString;
                  dblcSurnameDropDown(Sender);
              end else exit;
            end;
        4 : begin // Discounts
              frmMembersLookup.Setup( dmMM.dsrDiscounts,dmMM.tblDiscounts,    
                        'DISCOUNT_KEY',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
              if (frmMembersLookup.ShowModal = mrOK) then begin
                  clearcomps;
                  dblcSurname.Text := dmMM.tblDiscounts.FieldByName('DISCOUNT_KEY').AsString;
                  s := dmMM.tblDiscounts.FieldByName('DESCRIPTION').AsString;
                  dblcSurnameDropDown(Sender);
              end else exit;
            end;
        end; // case
     finally
        frmMembersLOOKup.Free;
     end;
     if dm.ibdsMembers.EOF then begin  // no records
        clearcomps;
        if FSearchby = 4 then 
           Desc := 'Discount'
        else   
            Desc := Global.SearchDesc[FSearchby + 1];
        showmessage(Format('There are no Members with %s = %s',[Desc,s]));
     end else begin
        dm.MembersDetailsPopulateGrid(EventCtr,SectionCtr,SubscriptionCtr,GroupCtr);
        PropSecChange;
        dbeSurname.SetFocus;
        dblcSurname.Clear;
     end;
     FSearchby := -1; //only a one time search
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbgSubsCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;   ABrush: TBrush);
begin
    // if not active then exit;
    /// if (dm.ibdsM_Subs.FieldByName('AMOUNT').AsFloat - dm.ibdsM_Subs.FieldByName('AMOUNT_PAID').AsFloat > 0.00)
    //    then AFont.Color := PayColor;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbgSectionsCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;
  ABrush: TBrush);
begin
     //if not active then exit;
    // if (dm.ibdsM_Sections.FieldByName('AMOUNT').AsFloat - dm.ibdsM_Sections.FieldByName('AMOUNT_PAID').AsFloat > 0.00)
    //    then AFont.Color := PayColor;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbdtApprovedExit(Sender: TObject);
// if a new member (Now - JOINED <= 200 days) and the APPROVED date gets altered
// depending on
begin
     if not active then exit;
     if SettingMemberDefaults then exit;
     if dbdtApproved.Date < dbdtJoined.Date then begin
        dm.ibdsMembers.FieldByName('APPROVED').AsDateTime := dbdtJoined.Date;
        sm('The Approved Date cannot be before the Joined Date');
        exit;
     end;
     dbdtApproved.DisplayFormat := 'dd mmm yyyy';
     
    {if dbdtApproved.modified then begin
        if (now - dm.ibdsMembers.FieldByName('JOINED').AsDateTime) <= 200 then begin
           if dm.ibdsMembers.FieldByName('SUB_PAID').AsInteger = 0 then begin
              dm.ibdsMembers.FieldByName('EXPIRES').AsDateTime := // hasnt paid yet
                 dbdtApproved.Date;
           end;{ else begin
              if not Global.FixedExpirySystem then begin// revolving
                 dm.ibdsMembers.Edit;
                 dm.ibdsMembers.FieldByName('EXPIRES').AsDateTime :=
                    dm.ibdsMembers.FieldByName('EXPIRES').AsDateTime +
                    ( dbdtApproved.Date - FApprovedDate );
              end else begin // fixed date
                 if dbdtApproved.Date >= Global.MemberExpiry then
                    sm('Note - The Approved Date has now gone passed the Rollover Date');
              end;
           end; }
      // end;
    //end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbdtApprovedEnter(Sender: TObject);
begin
     if not active then exit;
     FApprovedDate := dbdtApproved.DAte;
     //dbdtApproved.DisplayFormat := 'ddmmyy';
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.PageControlChange(Sender: TObject);
var  aButton : TButton;
     //NotifyEvent : TNotifyEvent;
     MemberPreferences : TMemberPreferences;
begin
     //pnlMember.BringToFront;
     //PageControl.BringToFront;
     //pnlMember.Align := alTop;
     //PageControl.Align := alClient;
     //scrollBox.SendToBack;
    // btnClosePrefs.Visible := False;
     case PageControl.ActivePageIndex of
        0 : dbeAddress.SetFocus;
        1 : dbdtDOB.SetFocus;
        2 : dbeAnalyse.SetFocus;
        3 : begin
               CheckIBDSMembersState;
               // not visible for the Default member
               if not SettingMemberDefaults then
                  dbeSubsAdd.Setfocus;
               dm.ibdsM_Subs.Close;
               dm.ibdsM_Subs.Open;
               dbgSubs.Update; // not repainting 
            end;
        4,6,10 : CheckIBDSMembersState;  // Links and Interests - Insert
        5 : DBRichEdit.SetFocus;
        7 : dbePrepaidCredit.SetFocus;
        9 : begin
              { PageControl.Align := alNone;
               pnlMember.Align := alNone;
               scrollBox.Visible := True;
               scrollBox.Align := alClient;
               scrollBox.BringToFront;
              // btnClosePrefs.Visible := True;
              //  only load the scroll box once
               if scrollBox.ComponentCount = 0 then begin
                  try
                     MemberPreferences := TMemberPreferences.Create;
                     MemberPreferences.MemberPreferenceLoad(scrollBox,asEdit);
                  finally
                     MemberPreferences.Free;
                  end;   
               end;}     
            end; 
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbdtDOBExit(Sender: TObject);
// keeps the birthday event correct
var DOB : string[12];
const SQL1 = 'select Count(MEMBER) from M_EVENTS where MEMBER = %d and EVENT = 100';
      SQL2 = 'UPDATE M_EVENTS Set DATES = ''%s'' where MEMBER = %d and EVENT = 100';
begin
     if SettingMemberDefaults then exit;
     if (dbdtDOB.Modified) and (dm.dsrMembers.State = dsEdit) then begin
        if dm.ibdsMembers.FieldByName('DATE_OF_BIRTH').AsDateTime >= now then begin
           sm(format('Date of Birth %s is not correct',[GetShortMonthDate(dm.ibdsMembers.FieldByName('DATE_OF_BIRTH').AsDateTime)]));
        end;   
     end;      
     if (dbdtDOB.Modified) and (dm.dsrMembers.State = dsEdit) then begin
        DOB := GetShortDate(dm.ibdsMembers.FieldByName('DATE_OF_BIRTH').AsDateTime);
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[dm.ibdsMembers.FieldByName('MEMBER').AsInteger])) then begin
           if dm.qryGeneral.Fields[0].AsInteger > 0 then begin
              dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[DOB,dm.ibdsMembers.FieldByName('MEMBER').AsInteger]));
              dm.ibdsM_Events.Refresh;
           end;
        end;
     end;
     dbdtDOB.DisplayFormat := 'dd mmm yyyy';
end;
{ **************************************************************************** }
{procedure TfrmMembersEDIT.dblcGroupsExit(Sender: TObject);
var FVariant : Variant;
begin
     if not active then exit;
     if dblcGroups.Text <> '' then begin
        FVariant := StrToIntDef(dblcGroups.Text,0);
        if dm.tblGroups.locate('GROUPS',FVariant,[]) then
           dbeGroupDesc.Text := dm.tblGroups.FieldByName('DESCRIPTION').AsString
        else begin
           sm(format(' %s is not a current Group',[dblcGroups.Text]));
           dbeGroupDesc.Text := '';
           dblcGroups.Text   := '';
        end;   
     end;   
end;}
{ **************************************************************************** }
{procedure TfrmMembersEDIT.dblcGroupsDropDown(Sender: TObject);
begin
     dm.tblGroups.Refresh; // Get latest data
end; }
{ **************************************************************************** }
procedure TfrmMembersEDIT.FormKeyUp(Sender: TObject; var Key: Word;  Shift: TShiftState);
var tabsheet : TTabsheet;
begin
    // sm(TPageControl(sender).Name);
     inherited;
     if key = VK_NEXT then begin	
        case PageControl.ActivePageIndex of
           0 : begin 
               PageControl.ActivePageIndex := 1;
               dbdtDOB.SetFocus;
               end;
           1 : begin 
               PageControl.ActivePageIndex := 2;
               dbeAnalyse.SetFocus;
               end;                       
           2 : begin
               PageControl.ActivePageIndex := 3;
               CheckIBDSMembersState;
               dbeSubsAdd.Setfocus;
               end;
           3 : begin
               PageControl.ActivePageIndex := 4;
               CheckIBDSMembersState;
               end;
           4 : begin
               PageControl.ActivePageIndex := 5;
               DBRichEdit.SetFocus;
               end;
           5 : begin
               PageControl.ActivePageIndex := 6;
               CheckIBDSMembersState;
               end;    
           6 : begin
               PageControl.ActivePageIndex := 7;
               dbePrepaidCredit.Setfocus;
               end;   
           7 : PageControl.ActivePageIndex := 8;
           8 : PageControl.ActivePageIndex := 9;
        end;   
     end;  
     if key = VK_PRIOR then begin	
        case PageControl.ActivePageIndex of 
           1 : begin
               PageControl.ActivePageIndex := 0;
               dbeAddress.Setfocus;
               end;
           2 : begin
               PageControl.ActivePageIndex := 1;
               dbdtDOB.SetFocus;
               end;
           3 : begin
               PageControl.ActivePageIndex := 2;
               dbeAnalyse.SetFocus;
               end;                
           4 : begin
               PageControl.ActivePageIndex := 3;
               CheckIBDSMembersState;
               dbeSubsAdd.Setfocus;
               end;                
           5 : begin
               PageControl.ActivePageIndex := 4;
               CheckIBDSMembersState;
               end;           
           6 : begin
               PageControl.ActivePageIndex := 5;
               DBRichEdit.Setfocus;  
               end;                   
           7 : begin
               PageControl.ActivePageIndex := 6;
               CheckIBDSMembersState;
               end;                   
           8 : begin
               PageControl.ActivePageIndex := 7;
               dbePrepaidCredit.Setfocus;
               end;
           9 : PageControl.ActivePageIndex := 8;
        end;                                   

     end;
     case key of
         VK_ESCAPE : menuExitClick(Sender);
         //VK_F1 : sm(ActiveControl.NAme);
         //VK_F2 : menuAddLinkClick(Sender);
         VK_F3 : begin
                    dbnQryMembersInsertClick(Sender);
                    dm.ibdsMembers.Insert;
                 end;
         VK_F4 : menuDeleteMemberClick(Sender);
         //VK_F5 : menuDeleteLinkClick(Sender);
         VK_F11 : menuAddMemberCardClick(Sender);
         VK_F9 : begin
                    if not SettingMemberDefaults then begin
                       if dm.ibdsMembers.State in [ dsInsert, dsEdit ] then dm.ibdsMembers.Post;
                    end else begin
                       if dm.ibdsMembersDef.State in [ dsEdit ] then dm.ibdsMembersDef.Post;
                    end;    
                  end;       
     end;
     // 2 PageControls on this Form
     if ( ssCtrl in Shift ) then begin
        case key of 
           70,102 : dblcSurname.SetFocus;           // Find 
        end;
     end;   
     if ( ssAlt in Shift ) then begin
        tabsheet := nil;
        case key of
           VK_Right : begin
                         dbnQryMembersLookNextClick(Sender);
                         dm.qryMembersLook.Next;
                         dbnQryMembersLookNextRowChanged(Sender);
                      end;  
           VK_Left : begin
                         dbnQryMembersLookNextClick(Sender);
                         dm.qryMembersLook.Prior;
                         dbnQryMembersLookNextRowChanged(Sender);
                      end;  
           VK_UP   :  begin
                         dbnQryMembersLookLastClick(Sender);
                         dm.qryMembersLook.First;
                         dbnQryMembersLookLastRowChanged(Sender);
                      end; 
           VK_DOWN :  begin
                         dbnQryMembersLookLastClick(Sender);
                         dm.qryMembersLook.LAST;
                         dbnQryMembersLookLastRowChanged(Sender);
                      end;
           88,120 : menuExitClick(Sender);      // x Close
           //85,117 : menuMemberNumberClick(Sender); // u
           67,99  : menuCopyMemberClick(Sender); // C  cant have same as Copy Ctrl C
           65,97  : menuDefaultMemberClick(Sender);  // A  Default Member
        end;                    
        if tabsheet <> nil then
           if tabsheet.TabVisible then PageControl.ActivePage := tabsheet;
     end;   
     key := 0;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dblcPostCodeDropDown(Sender: TObject);
begin
    dm.tblMailCodes.Refresh; // Get latest data
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.wmSysCommand(var Message : TWMSysCommand);
begin
     case message.CmdType of
        SC_EXPORT_MEMBERS : ExportAllMembers;
     else                                         
        inherited;
     end; 
end; 
{ **************************************************************************** }
{procedure TfrmMembersEDIT.ShowSubsOnInsert;
var FSender : TObject;
begin
     if dm.ibdsMembers.State = dsInsert then begin
        if dm.ibdsM_Subs.RecordCount > 0 then exit;
        dbgSectionsDblClick(FSender);
     end;
end;}
{ **************************************************************************** }
procedure TfrmMembersEDIT.btnGlyphClick(Sender: TObject);
begin

end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.FormCloseQuery(Sender: TObject;
  var CanClose: Boolean);
begin
     StateCheck;
     if not dm.CheckGroupsSubs  then 
        CanClose := false
     else
        CanClose := True;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbeMemberExit(Sender: TObject);
begin
     // keep them in sync
     if dm.dsrMembers.State = dsInsert then 
        dm.ibdsMembers.FieldByName('MAILING_LIST_CODE').AsInteger := dm.ibdsMembers.FieldByName('MEMBER').AsInteger;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.menuDefaultMemberClick(Sender: TObject);
// see ResetMemberDefault in dm
begin
     StateCheck;
     if not dm.CheckGroupsSubs  then exit;
     // also used for copy of Members (set to -1 for the Default Member)
     dm.ibdsMembersDef.ParamByName('MEMBER').AsInteger := -1;
     dm.ResetMembersDef;
     SettingMemberDefaults := True;
     SetUpButtons(False);
     dm.dsrMembers.DataSet := dm.ibdsMembersDef;
     pnlFind.Caption := '';
     dm.ibdsMembersDef.Edit;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.SetUpButtons( b : Boolean);
begin
     dblcSurname.Visible   := b;
     btnCopyMember.Enabled := b;
     btnNumbers.Enabled    := b;
     btnFind.Enabled       := b;
     btnQuery.Enabled      := b;
     dbnQryMembersLook.Enabled := b;
     pnlSub.Visible  := b;
     pnlSection.Visible  := b;
     pnlEvent.Visible  := b;
     pnlInterest.Visible  := b;
     pnlLot.Visible  := b;
     pnlGroup.Visible  := b;
     pnlDiscounts.Visible  := b;
     dbgDiscounts.Visible  := b;
     //dbgGroups.Visible  := b;
     //dbgEvents.Visible  := b;
     //dbgSections.Visible  := b;
     //dbgSubs.Visible  := b;
     btnAttached.Enabled      := b;
     dbeSeconderName.Clear;
     dbeProposerName.Clear;
     dbeParentCodeName.Clear;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.Timer1Timer(Sender: TObject);
begin
     //showmessage(IntToStr(ActiveControl.HelpContext));
     
     // PageControl does not get Focus
     
     //if (ActiveControl is TPageControl) then begin

     //if ActiveControl.InheritsFrom(TwwDBCustomEdit) then begin
     //  sm(  (ActiveControl as TPageControl).ActivePage.Caption );
     //end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.menuCopyMemberClick(Sender: TObject);
const SQL1 = 'select NEXT_MEMBER from M_NUMBERS where NUMBER_ID = %d';
      msg1 = 'Member Number %d will be used';
begin
     if SettingMemberDefaults then exit;
     if not dm.CheckGroupsSubs  then exit;
     frmMembersSEARCH := TfrmMembersSEARCH.Create(nil);
     try
        frmMembersSEARCH.SetForSetup := False;
        // the MUNBER_ID is stored in the popupMenu's tag field
        Global.NextMemberIndex := GetPopupMenuNumbersTag;
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[Global.NextMemberIndex])) then  // may return 0 as first member
           frmMembersSEARCH.lblMemberNumber.Caption := format(msg1,[dm.qryGeneral.FieldByName('NEXT_MEMBER').AsInteger])
        else
           frmMembersSEARCH.lblMemberNumber.Caption := '';
        if frmMembersSEARCH.Showmodal = mrOK then begin
           // picks up the default member
           dm.ibdsMembersDef.ParamByName('MEMBER').AsInteger := frmMembersSEARCH.GetCopyMember;
           FRecordCountText := stbStatus.Panels[2].Text;
           stbStatus.Panels[2].Text := '';
           Global.NextMemberIndex := GetPopupMenuNumbersTag;
           PageControl.ActivePage := tsMaintenance;
           dm.ibdsMembers.Insert;
        end;   
     finally
        frmMembersSEARCH.Free;
     end;     
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.menuDeleteMemberClick(Sender: TObject);
begin
     dbnQryMembersDeleteClick(dbnQryMembersDelete);                 
     // have to do the delete as it wont above
     dm.ibdsMembers.Delete;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.popmenuNumbersOnClick(Sender: TObject);
var i : shortint;
begin
     for i := 0 to PopupMenuNumbers.Items.Count -1 do 
        PopupMenuNumbers.Items[i].checked := False;
     for i := 0 to PopupMenuNumbers.Items.Count -1 do
        if TMenuItem(sender).Name = PopupMenuNumbers.Items[i].Name then
           PopupMenuNumbers.Items[i].checked := True;
end;
{ **************************************************************************** }
function TfrmMembersEDIT.GetPopupMenuNumbersTag : LongInt;
var i : shortint;
begin
     Result := 0;
     for i := 0 to PopupMenuNumbers.Items.Count -1 do 
        if PopupMenuNumbers.Items[i].checked then Result := PopupMenuNumbers.Items[i].Tag;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.FormResize(Sender: TObject);
const two = 2;
var x,y,i : integer;
   MemberPreferences : TMemberPreferences;

begin
    
     ResizeToolBar( Self ,Self.WindowState,OneButtonWidth,FpnlWidth,pnlSpace);
     ToolBar1.Width  := ToolBar.Width;
     x := tsMaintenance.Width div two;
     y := tsMaintenance.Height div two;
     if not MenuMateTriggerInstalled then 
        ArrangePanel(pnlPoints,x,y);    // tsCreditPoints
     ArrangePanel(pnlOther,x,y);     // tsOther
     ArrangePanel(pnlProposer,x,y);  // tsProposer
     ArrangePanel(pnlMaintenance,x,y);  // tsMaintanance
     x := pnlMember.Width div two;
     pnlName.Left := x - (pnlName.Width div two);
     if Self.WindowState = wsMaximized then begin
        Self.Height := HeightWidth.frmHeight;
        Self.Width  := HeightWidth.frmWidth;
     end;   
     {try
       MemberPreferences := TMemberPreferences.Create;
       // resize the scroll box and components in it
       MemberPreferences.MemberPreferenceLoad(ScrollBox,asEdit);
     finally
        MemberPreferences.free;
     end;}     
    
     
end;   
{ **************************************************************************** }
procedure TfrmMembersEDIT.menuAttachedMembersClick(Sender: TObject);
begin
     if dm.ibdsMembers.RecordCount = 0 then exit;
     try
       frmMembersLook := TfrmMembersLook.Create(self);
       frmMembersLook.LookupLinks := True;
       frmMembersLook.Member := dm.ibdsMembers.FieldByName('MEMBER').AsInteger;
       frmMembersLook.ShowModal;
     finally
       dm.qryAttachSubs.Close;
       frmMembersLook.Free;
     end;  
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbdtExpiresEnter(Sender: TObject);
begin
     //(Sender as TwwDbDateTimePicker).DisplayFormat := 'ddmmyy';
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbdtExpiresExit(Sender: TObject);
begin
     (Sender as TwwDbDateTimePicker).DisplayFormat := 'dd mmm yyyy';
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.menuNextUnUsedNumberClick(Sender: TObject);
begin
     StateCheck;
     if not dm.CheckGroupsSubs  then exit;
     case TMainMenu(Sender).Tag of
        0 : CurrentNumberTypes := ntNextNumber;
        1 : CurrentNumberTypes := ntBlankNumber;
        2 : CurrentNumberTypes := ntNonActive;
        3 : CurrentNumberTypes := ntNonFinancial;
        4 : CurrentNumberTypes := ntNonFinancialNoPoints;
     end;
     // also used for copy of Members (set to -1 for the Default Member)
     dm.ibdsMembersDef.ParamByName('MEMBER').AsInteger := -1;
     dm.ResetMembersDef;
     FRecordCountText := stbStatus.Panels[2].Text;
     stbStatus.Panels[2].Text := '';
     // the MUNBER_ID is stored in the popupMenu's tag field
     Global.NextMemberIndex := GetPopupMenuNumbersTag;
     PageControl.ActivePage := tsMaintenance;
     // done in ibdsMembers.AfterInsert
     if CurrentNumberTypes in [ntNextNumber,ntBlankNumber] then begin
        dm.ibdsMembers.Insert;
        exit;
     end;
     GetNonActiveOrFinancial;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.GetNonActiveOrFinancial;
const msg1 = 'There are no Members to choose from';
      msg2 = 'Unable to delete from the link tables, delete not successful';
      // Normal Members with at least 1 payment ie not a new Member
      SQL1 = 'select MEMBER from MEMBERS where CARD_TYPE = ''N'' and SUB_PAYMENT = ''Y'' and ';
      SQL2 = 'IS_ACTIVE = ''N'' ORDER BY EXPIRES';
      SQL3 = 'SUB_PAID = 0 ORDER BY EXPIRES';
      SQL4 = 'SUB_PAID = 0 and POINTS_AVAILABLE = 0.00 ORDER BY EXPIRES';
var s: shortString;
begin
     case CurrentNumberTypes of
        ntNonActive            : s := SQl1 + SQL2;
        ntNonFinancial         : s := SQl1 + SQL3;
        ntNonFinancialNoPoints : s := SQl1 + SQL4;
     end;
     if not dm.sqlExecQueryCheckEOF(dm.qryGeneral,s) then begin
        sm(msg1);
        dblcSurname.SetFocus;
        exit;
     end;
     case CurrentNumberTypes of
        ntNonActive : s := 'Non Active Member';
        ntNonFinancial : s := 'Non Financial Member';
        ntNonFinancialNoPoints : s := 'Non Financial Member with no Points or Credit';
     end;
     dblcSurname.Text := dm.qryGeneral.FieldByName('MEMBER').AsString;
     FSearchby := -1;
     dblcSurnameDropDown(dblcSurname);
     dm.MembersDetailsPopulateGrid(EventCtr,SectionCtr,SubscriptionCtr,GroupCtr);
     if ifMsgDlg(format ('Print Details before re-use ( %s )',[s])) then begin
        MemberDeleteReport(dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
        // does a form deactivate and closes the tables
        opentables;
     end;
     try
        dm.DeleteMemberLinks(dm.ibdsMembers.FieldByName('MEMBER').AsInteger,dm.qryGeneral);
     except
        dm.IBRollbackRetain;
        sm(msg2);
        exit;
     end;
     // also used for copy of Members (set to -1 for the Default Member)
     dm.ibdsMembersDef.ParamByName('MEMBER').AsInteger := -1;
     dm.ibdsMembers.Edit;
     // Default Member
     dm.ResetMembersDef;
     dm.SetMembersDefaults; //refreshes fields + sets defaults
     PropSecChange;
     dbeMember.SetFocus;
     ReUsingMember := True;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.menuAddMemberCardClick(Sender: TObject);
var CardMembers : TCardMembers;
begin
     if dm.ibdsMembers.RecordCount = 0 then exit;
     CardMembers.AddMemberInCardMembers(dm.ibdsMembers.FieldByName('MEMBER').AsInteger,1);
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbdtDOBChange(Sender: TObject);
begin
    if TwwDBDateTimePicker(Sender).Modified then
       TwwDBDateTimePicker(Sender).DisplayFormat := 'dd/mm/yyyy';
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.DeleteLinks( Sender : TObject);
var i : shortint;
begin
     i := TwwDBGrid(MenuLinks.PopupComponent).Tag;
     // delete is handled in the before delete event of ibdsM_GROUPS
     // for delete of groups with GOLF and removing the club
     if i <> 3 then
        if not ifMsgDlg('Delete Record?') then exit;
     case i of 
        0 : dm.ibdsM_Subs.Delete;  
        1 : dm.ibdsM_Sections.Delete;  
        2 : dm.ibdsM_Events.Delete;  
        3 : dm.ibdsM_Groups.Delete;
     end;
end;               
{ **************************************************************************** }
procedure TfrmMembersEDIT.MenuLinksPopup(Sender: TObject);
var i : shortint;
    s : string;
    NewItem: TMenuItem;
begin         
     if dbeMember.Text = '' then exit;
     i := TwwDBGrid(MenuLinks.PopupComponent).Tag;
     s := '';
     case i of 
        0 : s := GetMenuLinksDesc(dm.ibdsM_Subs,i);  
        1 : s := GetMenuLinksDesc(dm.ibdsM_Sections,i);
        2 : s := GetMenuLinksDesc(dm.ibdsM_Events,i);
        3 : s := GetMenuLinksDesc(dm.ibdsM_Groups,i);
     end;
     if s = '' then exit;  // no recordCount
     MenuLinks.Items.Clear;
     NewItem := TMenuItem.Create(Self);
     NewItem.OnClick :=  DeleteLinks;
     NewItem.Caption :=  'Delete ' + s;
     MenuLinks.Items.Add(NewItem);
end;
{ **************************************************************************** }
function TfrmMembersEDIT.GetMenuLinksDesc(theDAtaSet : TIBDAtaSet; i : ShortInt ) : ShortString ;
begin
     with theDAtaSet do begin
        if RecordCount = 0 then begin
           Result := '';
           exit;
        end;      
        Result := FieldNameArray[i] + '  '; 
        Result := Result + FieldByName(FieldNameArray[i]).AsString + '  '; 
        Result := Result + FieldByName('DESCRIPTION').AsString; 
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.CheckIBDSMembersState;
begin
      // post before making changes to Links and Interersts tables
     // dbgSectionsDblClick(Sender) done in dm.ibdsMembers.AfterPost 
     if dm.ibdsMembers.State in [dsInsert,dsEdit] then begin
        dm.ibdsMembers.Post;
        RefreshLinks;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.lblAddressClick(Sender: TObject);
const PostalAddress = '(Postal)';
      StreetAddress = '(Street)';
begin
     dbeEmail.Setfocus;
     if POS(StreetAddress,lblAddress.Caption) > 0  then begin
        lblAddress.Caption    :=  copy( lblAddress.Caption,1,POS(' ',lblAddress.Caption )) + PostalAddress;
        lblAddress1.Caption   :=  copy( lblAddress1.Caption,1,POS(' ',lblAddress1.Caption )) + PostalAddress;
        lblDistrict.Caption   :=  copy( lblDistrict.Caption,1,POS(' ',lblDistrict.Caption )) + PostalAddress;
        lblCity.Caption       :=  copy( lblCity.Caption,1,POS(' ',lblCity.Caption )) + PostalAddress;
        lblCountry.Caption    :=  copy( lblCountry.Caption,1,POS(' ',lblCountry.Caption )) + PostalAddress;
        dbeAddress.DataField  := 'ADDRESS';
        dbeAddress1.DataField := 'ADDRESS1';
        dbeDistrict.DataField := 'DISTRICT';
        dbeCity.DataField     := 'CITY';
        dbeCountry.DataField  := 'COUNTRY';
     end else begin   
        lblAddress.Caption    :=  copy( lblAddress.Caption,1,POS(' ',lblAddress.Caption )) + StreetAddress;
        lblAddress1.Caption   :=  copy( lblAddress1.Caption,1,POS(' ',lblAddress1.Caption )) + StreetAddress;
        lblDistrict.Caption   :=  copy( lblDistrict.Caption,1,POS(' ',lblDistrict.Caption )) + StreetAddress;
        lblCity.Caption       :=  copy( lblCity.Caption,1,POS(' ',lblCity.Caption )) + StreetAddress;
        lblCountry.Caption    :=  copy( lblCountry.Caption,1,POS(' ',lblCountry.Caption )) + StreetAddress;
        dbeAddress.DataField  := 'STREET_ADDRESS';
        dbeAddress1.DataField := 'STREET_ADDRESS1';
        dbeDistrict.DataField := 'STREET_DISTRICT';
        dbeCity.DataField     := 'STREET_CITY';
        dbeCountry.DataField  := 'STREET_COUNTRY';
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.lblFaxClick(Sender: TObject);
const WorkFax = '(Work)';
      HomeFax = '(Home)';
begin
     dbePhoneCell.Setfocus;
     if POS(WorkFax,lblFax.Caption) > 0  then begin
        lblFax.Caption    :=  copy( lblFax.Caption,1,POS(' ',lblFax.Caption )) + HomeFax;
        dbeFax.DataField  := 'FAX_NUMBER';
     end else begin
        lblFax.Caption    :=  copy( lblFax.Caption,1,POS(' ',lblFax.Caption )) + WorkFax;
        dbeFax.DataField  := 'FAX_WORK';
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnInterestsInsertClick(Sender: TObject);
begin
    dbgInterestsDblClick(Sender);
    abort;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbeMemberChange(Sender: TObject);
begin
     // if dbeMember.Text <> '' then
    //     LastMemberInMaintenance := dbeMember.Text;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnSubsRefreshClick(Sender: TObject);
var idx : shortInt;
begin
     idx := TwwNavButton(sender).Tag;
     case idx of
        0 : IBDSActiveOrRefresh(dm.ibdsM_Subs);
        1 : IBDSActiveOrRefresh(dm.ibdsM_Sections);
        2 : IBDSActiveOrRefresh(dm.ibdsM_Events);
        3 : IBDSActiveOrRefresh(dm.ibdsM_Groups);
        4 : IBDSActiveOrRefresh(dm.ibdsM_Discounts);
     end; 
     abort;  
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbnSubsInsertClick(Sender: TObject);
var idx : shortInt;
begin
     idx := TwwNavButton(sender).Tag;
     case idx of
        0 : dbgSectionsDblClick(dbgSubs);
        1 : dbgSectionsDblClick(dbgSections); 
        2 : dbgSectionsDblClick(dbgEvents);
        3 : dbgSectionsDblClick(dbgGroups);
        4 : dbgSectionsDblClick(dbgDiscounts);
     end;   
     abort;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbeSubsAddKeyPress(Sender: TObject; var Key: Char);
var v : variant;
    s : String[6];
begin
     if dbeMember.Text = '' then exit;
     if key = #13 then begin
        v := dbeSubsAdd.Text;
        fillchar(s,sizeof(s),' ');
        s := dbeSubsAdd.Text;
        s[0] := chr(6);
        
        if dm.dsrM_Subs.State in [dsEdit,dsInsert] then dm.ibdsM_Subs.Post;
        if AllDigits(dbeSubsAdd.Text) then begin
           if dm.tblSubs.Locate('SUBSCRIPTION',v,[]) then
              dm.ibdsM_Subs.Insert;
        end else begin
           if dm.tblSubs.Locate('CODE',s,[loCaseInsensitive]) then
              dm.ibdsM_Subs.Insert;
        end;
        dbeSubsAdd.SetFocus;
        dbeSubsAdd.SelectAll;
     end; 
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.ConvertToJpg;
// convert the Image to JPG 
const PhotoName = 'Photo.jpg';
var jpg : TJPegImage;
    CurrDir : shortstring;
begin
     try
        jpg := TJPEGImage.Create;
        jpg.Assign( Image.Picture.Bitmap);
        jpg.Compress;
        CurrDir := GetCurrentDir;
        if CurrDir[length(CurrDir)] <> '\' then
           CurrDir := CurrDir + '\';
        jpg.SaveToFile(CurrDir + PhotoName);
        Image.Picture.LoadFromFile(CurrDir + PhotoName);
        DeleteFile(CurrDir + PhotoName);
     finally
        jpg.free;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.btnLoadPhotoClick(Sender: TObject);
begin
     if dbeMember.Text = '' then exit;
     if OpenPictureDialog.Execute then begin
        image.Picture.Assign(nil);
        Image.Picture.LoadFromFile(OpenPictureDialog.Filename);
        if UpperCase(ExtractFileExt(OpenPictureDialog.Filename)) <> UpperCase('.JPG') then
           ConvertToJpg;
        SavePhoto;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.btnPastePhotoClick(Sender: TObject);
var jpg : TJPegImage;
    TempStream : TMemoryStream;
begin
     if dbeMember.Text = '' then exit;
     Image.Picture.Assign(Clipboard);
     if rgpPasteOptions.ItemIndex = 1 then
        ConvertToJpg;
     SavePhoto;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.ImageDblClick(Sender: TObject);
begin
     btnPastePhotoClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.LoadPhotoIntoImage;
var photo : TBlobField;
    AStream : TMemoryStream;
    jpg : TJPegImage;
begin
    if dm.ibdsM_PhotosPHOTO.BlobSize < 1 then begin
       Image.Picture.Assign(nil);
       exit; 
    end;   
    jpg := TJPEGImage.Create;
    AStream := TMemoryStream.Create;
    try
       image.Picture.Assign(nil);
       photo := dm.ibdsM_PhotosPHOTO As TBlobField;
       photo.SaveToStream(AStream);
       AStream.Seek(0, soFromBeginning);
       with jpg do begin
          Pixelformat := jf24bit;
          performance := jpBestSpeed;
          ProgressiveDisplay := True;
          ProgressiveEncoding := True;
          LoadFromStream(AStream);
       end;
       Image.Picture.Assign(jpg);
    finally
        photo := nil;
        AStream.Free;
        jpg.Free;
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.SavePhoto;
var ms : TMemoryStream;
begin
     if dbeMember.Text = '' then exit;
     if Image.Picture.Graphic.Empty then exit;
     ms := TMemoryStream.Create;
     try
        Image.Picture.Graphic.SaveToStream(ms);
        with dm.ibdsM_Photos do begin
           if EOF then 
              Insert
           else
              Edit;
           FieldByName('MEMBER').AsInteger := dm.ibdsMembers.FieldByName('MEMBER').AsInteger;
           FieldByName('DRAWN').AsString := varNo;
           dm.ibdsM_PhotosPHOTO.LoadFromStream(ms);
           Post;          
        end;
     finally
        ms.Free;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.btnDeletePhotoClick(Sender: TObject);
const SQL1 = 'delete from M_PHOTOS where MEMBER = %d';
begin
     if dbeMember.Text = '' then exit;
     dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[dm.ibdsMembers.FieldByName('MEMBER').AsInteger]));
     Image.Picture.Assign(nil);
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dbgInterestsDblClick(Sender: TObject);
begin
     if not ldlgSections.Execute then
        abort
     else
        dm.ibdsM_Interests.Insert;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.dblcClubIDCloseUp(Sender: TObject; LookupTable,
  FillTable: TDataSet; modified: Boolean);
begin
     if Modified then ClubNameChange;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.menuSetupMemberNumbersClick(Sender: TObject);
begin
     frmMembersSEARCH := TfrmMembersSEARCH.Create(nil);
     try
        frmMembersSEARCH.SetForSetup := True;
        frmMembersSEARCH.Showmodal;
     finally
        frmMembersSEARCH.Free;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.menuAdjustPointsCreditClick(Sender: TObject);
begin
     frmPassword := TfrmPassword.Create(nil);
     try
        if frmPassword.ShowModal = mrOk then begin
           //pnlReason.Visible := True;
           lblPoints.Visible := True;
           SetupPointsCredit( False );
           PageControl.ActivePage := tsCreditPoints;
           dbePrepaidCredit.Setfocus;
        end;
     finally
        frmPassword.free;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.SetupPointsCredit( b : boolean);
begin
     dbePrepaidCredit.ReadOnly  := b;
     dbeBonusCredit.ReadOnly    := b;
     dbeEarntCredit.ReadOnly    := b;
     dbePointsEarnt.ReadOnly    := b;
     dbePointsRedeemed.ReadOnly := b;
     dbePAvailable.ReadOnly := b;
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.menuResetAdjustPointsCreditClick(Sender: TObject);
// Has been altering points or Credit - reset it to readonly
begin
     lblPoints.Visible := False;
     SetupPointsCredit(PointsPassword);
end;
{ **************************************************************************** }
procedure TfrmMembersEDIT.menuExportMemberClick(Sender: TObject);
begin
     // Sends this member to Clubmanager
     if dm.ibdsMembers.RecordCount = 0 then exit;
     if dm.ibdsMembers.State in [dsInsert,dsEdit] then 
        dm.ibdsMembers.Post;
     dm.LogExport(dm.qryGeneral,dm.ibdsMembers.FieldByName('MEMBER').AsInteger,0,False);
     MDlgI(format('Exported %s Successfully',[dm.ibdsMembers.FieldByName('MEMBER').AsString]));
     dm.IBCommitRetain;
end;
{ ****************************************************************************}
procedure TfrmMembersEDIT.ExportAllMembers;
const SQL1 = 'insert into EXPORT_AREV select 0,Current_TIMESTAMP, MEMBER,''N'',''N'',0,''N'' from MEMBERS';
begin
     // allows for a system that is stand alone 
     if (ControllerPCName = 'None') or (ControllerPCName = '') then exit;
     if not ifMsgDlg('Are you sure you want to Export All Members to Stockmaster?') then exit;
     try
        TooBusy('Exporting Members to Stockmaster');
        dm.ExportTimer.Enabled := False;
        dm.sqlExecQuery(dm.qryGeneral,SQL1);
        dm.IBCommitRetain;
        MDlgI('Export All Members to Stockmaster worked Successfully');
     finally                                
        NotBusy;
        dm.ExportTimer.Enabled := True;
     end;   
end;
{ ****************************************************************************}
procedure TfrmMembersEDIT.menuPrintMemberDetailsClick(Sender: TObject);
begin
    if dm.ibdsMembers.RecordCount = 0 then exit;
    MemberDeleteReport(dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
end;
{ ****************************************************************************}
procedure TfrmMembersEDIT.menuMemberVisitsClick(Sender: TObject);
begin
    with dm.ibdsMembers do begin
       if RecordCount = 0 then exit;
       if state in [dsEdit,dsInsert] then exit;
       try
          frmMemberVisits := TfrmMemberVisits.Create(Self);
          frmMemberVisits.Showmodal;
       finally
          frmMemberVisits.Free;
       end;   
       //MDlgI(format('Members %d has %d visits, last visit %s',[FieldByName('MEMBER').AsInteger,FieldByName('VISITS').AsInteger,formatDateTime('dd mmm yyyy hh:mm',FieldByName('LAST_VISIT').AsDateTime)]));
    end;   
end;
{ ****************************************************************************}
procedure TfrmMembersEDIT.menuPHMTransferClick(Sender: TObject);
begin
  if dm.PHMExportTimer.Enabled then
     dm.PHMExportTimerTimer(Sender);
end;
{ ****************************************************************************}
procedure TfrmMembersEDIT.menuClosePreferencesClick(Sender: TObject);
begin
     PageControl.ActivePage :=  tsMAintenance;
     PageControlChange(Sender);
end;
{ ****************************************************************************}
procedure TfrmMembersEDIT.btnClosePrefsClick(Sender: TObject);
begin
     menuClosePreferencesClick(Sender);
end;
{ ****************************************************************************}
procedure TfrmMembersEDIT.btnUpClick(Sender: TObject);
var CurrentOrder : Integer;
begin
     with dm.ibdsM_Discounts do begin
        if RecordCount = 0 then exit;
        try                          
           DisableControls;
           CurrentOrder := FieldByName('MEMBER_DISCOUNTS_KEY').AsInteger;
           // cant go up any further
           if CurrentOrder = 1 then exit;
           dbgDiscounts.SetFocus;
           Edit;
           FieldByName('MEMBER_DISCOUNTS_KEY').AsInteger := CurrentOrder -1; 
           Post;
           Prior;
           Edit;
           FieldByName('MEMBER_DISCOUNTS_KEY').AsInteger := CurrentOrder; 
           Post;
           Close;
           // resorts the order
           Open; 
           Locate('MEMBER_DISCOUNTS_KEY',CurrentOrder -1,[]);
        finally
           EnableControls;
        end;   
     end;
end;
{ ****************************************************************************}
procedure TfrmMembersEDIT.btnDownClick(Sender: TObject);
var CurrentOrder : Integer;
begin              
     with dm.ibdsM_Discounts do begin
        if RecordCount = 0 then exit;
        try
           DisableControls;
           CurrentOrder := FieldByName('MEMBER_DISCOUNTS_KEY').AsInteger;
           // cant go down any further
           if CurrentOrder = RecordCount then exit;
           dbgDiscounts.SetFocus;
           Edit;                 
           FieldByName('MEMBER_DISCOUNTS_KEY').AsInteger := CurrentOrder +1; 
           Post;
           Next;
           Edit;
           FieldByName('MEMBER_DISCOUNTS_KEY').AsInteger := CurrentOrder; 
           Post;
           Close;
           // resorts the order
           Open;
           Locate('MEMBER_DISCOUNTS_KEY',CurrentOrder +1,[]);
           
        finally   
           EnableControls;
        end;   
     end;
end;
{ ****************************************************************************}
procedure TfrmMembersEDIT.ApplyDiscount1Click(Sender: TObject);
var SavePlace : TBookMark; 
    i : LongInt;
const SQL1 = 'Select max(MEMBER_DISCOUNTS_KEY) as MaxKey from MEMBERS_DISCOUNTS where MEMBER = %d';
      SQL3 = 'Select DISCOUNT_KEY from MEMBERS_DISCOUNTS where DISCOUNT_KEY = %d and MEMBER = %d';
      SQL2 = 'Insert into MEMBERS_DISCOUNTS values( %d,%d,%d )';
begin
     if not MenuMateDiscountsInstalled then begin
        showmessage('MenuMate Discounts are not installed');
        exit;
     end;
     with dm.qryMembersLook do begin
        if RecordCount = 0 then exit;
        if not IfMsgDlg(format('Apply a Discount to %d Members, do you wish to continue?',[RecordCount])) then exit;
        try
           frmMembersLookup := TfrmMembersLOOKUP.create(nil);
           frmMembersLookup.LookupOnly := True;
           frmMembersLookup.Setup( dmMM.dsrDiscounts,dmMM.tblDiscounts,    
                     'DISCOUNT_KEY',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
           if (frmMembersLookup.ShowModal = mrOK) then begin
              try
                 TooBusy('Applying Discounts ');
                 SavePlace := GetBookmark;
                 DisableControls;
                 First;
                 While not EOF do begin
                    // if not already inserted
                    if not dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL3,[dmMM.tblDiscounts.FieldByName('DISCOUNT_KEY').AsInteger,FieldByName('MEMBER').AsInteger])) then begin
                       if dm.sqlExecQueryCheckEOFCount(dm.qryGeneral,format(SQL1,[FieldByName('MEMBER').AsInteger])) then begin
                          i := dm.qryGeneral.FieldByName('MaxKey').AsInteger + 1;
                       end else begin
                          i := 1;
                       end;
                       dm.sqlExecquery(dm.qryGeneral,format(SQL2,[i,dmMM.tblDiscounts.FieldByName('DISCOUNT_KEY').AsInteger,FieldByName('MEMBER').AsInteger]));
                    end;   
                    Next;
                 end; 
                 dm.IBCommitRetain;
              finally
                 if assigned(SavePlace) then begin
                    GotoBookmark(SavePlace);
                    FreeBookmark(SavePlace);
                 end;
                 NotBusy;
              end;   
           end;   
        finally
           frmMembersLookup.Free;
           EnableControls;
           RefreshLinks;
        end;    
     end;
end;
{ ****************************************************************************}
procedure TfrmMembersEDIT.menuDeleteDiscountClick(Sender: TObject);
var SavePlace : TBookMark; 
const SQL1 = 'Delete from MEMBERS_DISCOUNTS where DISCOUNT_KEY = %d and MEMBER = %d';
begin
     if not MenuMateDiscountsInstalled then begin
        showmessage('MenuMate Discounts are not installed');
        exit;
     end;
     with dm.qryMembersLook do begin
        if RecordCount = 0 then exit;
        if not IfMsgDlg(format('Delete a Discount from %d Members, do you wish to continue?',[RecordCount])) then exit;
        try
           frmMembersLookup := TfrmMembersLOOKUP.create(nil);
           frmMembersLookup.LookupOnly := True;
           frmMembersLookup.Setup( dmMM.dsrDiscounts,dmMM.tblDiscounts,    
                     'DISCOUNT_KEY',dm.ibdsMembers.FieldByName('MEMBER').AsInteger);
           if (frmMembersLookup.ShowModal = mrOK) then begin
              try
                 TooBusy('Deleting Discounts ');
                 SavePlace := GetBookmark;
                 DisableControls;
                 First;
                 While not EOF do begin
                    dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[dmMM.tblDiscounts.FieldByName('DISCOUNT_KEY').AsInteger,FieldByName('MEMBER').AsInteger]));
                    Next;
                 end; 
                 dm.IBCommitRetain;
              finally
                 if assigned(SavePlace) then begin
                    GotoBookmark(SavePlace);
                    FreeBookmark(SavePlace);
                 end;
                 NotBusy;
              end;   
           end;   
        finally
           frmMembersLookup.Free;
           EnableControls;
           RefreshLinks;
        end;    
     end;
end;
{ ****************************************************************************}
end.
