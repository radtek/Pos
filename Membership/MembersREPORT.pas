unit MembersREPORT;
                                                                                                     
{    
  All Reports use dm.qryReport as the last select - several reports
     use temporary tables prior to this.
  a Client Data set was introduced later.    
               
  TreeView StateImages only allow up to 15
  ReportHoldFree1Click - right click on TreeView allows the report to be held
     NB see ReportHoldFree1Click for clearance of the unique Temptables
        dont clear until the user has released  ReportHoldFree1Click
        otherwise qryReport will be empty;
}
interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm,  ComCtrls, StdCtrls, ImgList, wwSpeedButton,
  wwDBNavigator, wwclearpanel, ExtCtrls, wwdbdatetimepicker, Wwdbdlg,
  wwdblook,Printers,qrprntr, wwmonthcalendar, wwDialog, Wwfltdlg, Mask,
  wwdbedit, Wwdotdot, Wwdbcomb,QuickRpt, QRExport, Buttons, ToolEdit,qrextra,
  DualList,IBTable,OpExcel,OpWord, OpShared,OpWrd2K, OpOlk2k, OpOutlk, Menus, lmdcompo,
  lmdclass, lmdnonvS,FileCtrl, ToolWin,db, OpOlkXP ;

type
  TfrmMembersREPORT = class(TSMStdForm)
    FieldsDialog: TDualListDialog;
    Panel1: TPanel;
    TreeView: TTreeView;
    scbOptions: TScrollBox;
    gbxDates: TGroupBox;
    lblDateFrom: TLabel;
    lblDateTo: TLabel;
    dbdtFrom: TwwDBDateTimePicker;
    dbdtTo: TwwDBDateTimePicker;
    rgpSortBy: TRadioGroup;
    rgpMembers: TRadioGroup;
    gbxEvents: TGroupBox;
    cbxAllEvents: TCheckBox;
    gbxGroups: TGroupBox;
    cbxAllGroups: TCheckBox;
    gbxSubs: TGroupBox;
    cbxAllSubs: TCheckBox;
    rgpProduct: TRadioGroup;
    gbxLocations: TGroupBox;
    gbxMemo: TGroupBox;
    Memo: TMemo;
    rgpMembersOther: TRadioGroup;
    gbxMailJoin: TGroupBox;
    lblMailJoin: TLabel;
    Label6: TLabel;
    dblcMailCode: TwwDBLookupCombo;
    dblcJoiningFee: TwwDBLookupCombo;
    gbxMonths: TGroupBox;
    Label10: TLabel;
    cbxMonths: TwwDBComboBox;
    gbxNoOptions: TGroupBox;
    Label8: TLabel;
    rgpLines: TRadioGroup;
    rgpAlpha: TRadioGroup;
    gbxSections: TGroupBox;
    cbxAllSections: TCheckBox;
    rgpMembersSections: TRadioGroup;
    rgpSortBySections: TRadioGroup;
    rgpSelection: TRadioGroup;
    gbxQuery: TGroupBox;
    SpeedButton1: TSpeedButton;
    gbxExport: TGroupBox;
    dbcbExport: TwwDBComboBox;
    dirEdit: TDirectoryEdit;
    gbxFields: TGroupBox;
    btnFields: TSpeedButton;
    rgpNewMember: TRadioGroup;
    rgpSortLabels: TRadioGroup;
    rgpLinks: TRadioGroup;
    Splitter1: TSplitter;
    Panel3: TPanel;
    Panel4: TPanel;
    btnEvents: TSpeedButton;
    btnGroups: TSpeedButton;
    btnSections: TSpeedButton;
    btnSubs: TSpeedButton;
    OpOutlook: TOpOutlook;
    PopupMenu: TPopupMenu;
    ReportHoldFree1: TMenuItem;
    gbxLetter: TGroupBox;
    dblcLetters: TwwDBLookupCombo;
    Label3: TLabel;
    btnPrint: TToolButton;
    btnEmail: TToolButton;
    btnPreview: TToolButton;
    btnExcel: TToolButton;
    btnLetters: TToolButton;
    btnPrinterSetup: TToolButton;
    menuExit: TMenuItem;
    btnExport: TToolButton;
    menuOptions: TMenuItem;
    menuPrint: TMenuItem;
    menuPreview: TMenuItem;
    menuExportAscii: TMenuItem;
    menuExportExcel: TMenuItem;
    menuEmailReport: TMenuItem;
    menuLetters: TMenuItem;
    menuPrinterSetup: TMenuItem;
    pnlSpace: TPanel;
    btnClose: TToolButton;
    gbxAges: TGroupBox;
    Label4: TLabel;
    Label7: TLabel;
    dbeYearFrom: TwwDBEdit;
    dbeYearTo: TwwDBEdit;
    rpgReceiptSortBy: TRadioGroup;
    rgpExpires: TRadioGroup;
    btnQueries: TToolButton;
    menuQueries: TMenuItem;
    gbxQueries: TGroupBox;
    Label9: TLabel;
    dblcQueries: TwwDBLookupCombo;
    memoQueries: TMemo;
    btnMailingLabels: TToolButton;
    menuMailingLabels: TMenuItem;
    gbxAge: TGroupBox;
    Label11: TLabel;
    Label12: TLabel;
    dbdtThisDAte: TwwDBDateTimePicker;
    dbeAge: TwwDBEdit;
    btnWord: TToolButton;
    menuWordMailingLabelsNew: TMenuItem;
    menuWordMailingLabels: TMenuItem;
    rgpExportOptions: TRadioGroup;
    gbxMemberMulti: TGroupBox;
    cbxAllMembers: TCheckBox;
    cbxFinancial: TCheckBox;
    cbxNonFinancial: TCheckBox;
    cbxActive: TCheckBox;
    cbxNonActive: TCheckBox;
    cbxPartFinancial: TCheckBox;
    gbxClubs: TGroupBox;
    btnClubs: TSpeedButton;
    cbxAllClubs: TCheckBox;
    Label13: TLabel;
    dbcbGender: TwwDBComboBox;
    cbxDOB: TCheckBox;
    btnEmailMembers: TToolButton;
    rgpToday: TRadioGroup;
    menuEmailMembers: TMenuItem;
    rgpMembersEmail: TRadioGroup;
    btnQuery: TToolButton;
    rgpPayment: TRadioGroup;
    cbxAllLocations: TCheckBox;
    btnLocations: TSpeedButton;
    rgpReportByGroup: TRadioGroup;
    gbxMMDates: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    dbdtMMDateFrom: TwwDBDateTimePicker;
    dbdtMMDateTo: TwwDBDateTimePicker;
    rgpMemberOnLabel: TRadioGroup;
    rgpExportParentCode: TRadioGroup;
    gbxTextInput: TGroupBox;
    Label5: TLabel;
    edtTextInput: TEdit;
    gbxDiscounts: TGroupBox;
    btnDiscounts: TSpeedButton;
    cbxAllDiscounts: TCheckBox;
    cbxNoDiscounts: TCheckBox;
    procedure FormCreate(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure SetScrollBoxOptions(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,ab,ac,ad,ae,af,ag,ah,ai,aj,ak : Boolean );
    procedure SetTops(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,ab,ac,ad,ae,af,ag,ah,ai,aj,ak : smallint );
    procedure ResetScrollBoxOptions;
    procedure dblcMailCodeDropDown(Sender: TObject);
    procedure dbdtToCloseUp(Sender: TObject);
    procedure dbdtFromKeyPress(Sender: TObject; var Key: Char);
    function PreReportCheck : Boolean;
    procedure FormShow(Sender: TObject);
    procedure TreeViewChange(Sender: TObject; Node: TTreeNode);
    procedure ResetLookups;
    procedure FormDestroy(Sender: TObject);
    procedure btnQueryClick(Sender: TObject);
    procedure cbxMonthsEnter(Sender: TObject);
    procedure cbxMonthsExit(Sender: TObject);
    procedure SetDefaultOption;
    procedure cbxAllSectionsClick(Sender: TObject);
    procedure cbxAllGroupsClick(Sender: TObject);
    procedure cbxAllEventsClick(Sender: TObject);
    procedure cbxAllSubsClick(Sender: TObject);
    procedure DropTempTables;
    procedure PrintOrPreview ( frmQR : TForm ; qrQR : TQuickRep );
    procedure PrintOrPreviewExport;
    procedure MemoEnter(Sender: TObject);
    procedure MemoExit(Sender: TObject);
    procedure gbxExportClick(Sender: TObject);
    procedure dirEditEnter(Sender: TObject);
    procedure dirEditExit(Sender: TObject);
    procedure CreateList( qrprint : Boolean  ; Title : string  );
    function  ExportEvents( mMonths : smallInt ; eEvent : ShortString ) : Boolean;
    function ExportCardFile(mMembers,mNewMembers,mGroup : string ) : Boolean;
    procedure PreCreateAsciiFile;
    procedure dbcbExportChange(Sender: TObject);
    function ExportSubs( mGroup : shortstring ) : Boolean;
    function ExportSelective(mMembers : string ; mGroup : ShortString) : Boolean;
    procedure btnFieldsClick(Sender: TObject);
    procedure rgpNewMemberClick(Sender: TObject);
    procedure ClearMembersFlag;
    procedure rgpLinksClick(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    function GetDatafromLetter( LetterNAme : string ) : Boolean;
    procedure btnEventsClick(Sender: TObject);
    procedure GetFieldList( aTitle, aCaption : shortstring ; aTable : TIBTable );
    procedure ExportToExcel;
    procedure ExportToWord;
    procedure Email;
    procedure ReportHoldFree1Click(Sender: TObject);
    procedure SetTreeView;
    procedure Button1Click(Sender: TObject);
    function ExportCodeKnownPosition  : Boolean;
    procedure FormKeyUp(Sender: TObject; var Key: Word;  Shift: TShiftState);
    procedure menuExitClick(Sender: TObject);
    procedure menuPrintClick(Sender: TObject);
    procedure menuLettersClick(Sender: TObject);
    procedure menuPrinterSetupClick(Sender: TObject);
    procedure TreeViewDblClick(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure dbdtFromChange(Sender: TObject);
    procedure dbdtFromExit(Sender: TObject);
    procedure PrintFromTable;
    procedure ClearCardMembers;
    procedure rgpExpiresClick(Sender: TObject);
    procedure AddFields( MemberLst : TStrings; s : String  );
    procedure menuQueriesClick(Sender: TObject);
    procedure dblcQueriesCloseUp(Sender: TObject; LookupTable, FillTable: TDataSet; modified: Boolean);
    procedure MailingLabels;
    //procedure menuMailingLabelsClick(Sender: TObject);
    procedure cbxAllClubsClick(Sender: TObject);
    procedure ExportCardsExcel(FName : String);
    procedure ClearInterests;
    procedure cbxDOBClick(Sender: TObject);
    procedure EmailMembers;
    procedure cbxAllLocationsClick(Sender: TObject);
    procedure cbxAllDiscountsClick(Sender: TObject);
    procedure cbxNoDiscountsClick(Sender: TObject);
    //procedure btnEmailMembersClick(Sender: TObject);

  private
    { Private declarations }
    MergeDoc : TOpWordDocument; 
    FpnlWidth : SmallInt;
    FGroupText,FSubText,FSectionText,FTblName,FNewMembers,FMembersEmail  : String;
    FSectionsExtFields,FSectionsFields,FSubsExtFields,FSubsFields,FLocationsFields,FDiscountsFields  : String;
    FGroupsExtFields,FGroupsFields,FEventsExtFields,FEventsFields,FClubsExtFields,FClubsFields,FDiscountsExtFields  : String;
    FSortBy,FSortByText,FSortLabels,FSortBySection,FSortBySectionText,FMembers  : string[40];
    FEventText,FMailJoinText,FMembersSectionsText,FMembersSections,FMembersText,FClubText,FLocationText,FDiscountText : String[50];
    FMembersOtherText,FMembersOther,FProduct,FMailJoin  : string[40];
    FDateTo,FDateFrom,FMMDateTimeTo,FMMDateTimeFrom : TDateTime;
    FReportChoice   : shortint;
    FTreeViewText : String[30];
    FTreeViewHint : string[50];
    FRunOnce,FHoldReport,FbtnQuery  : Boolean;
  public
    { Public declarations }
    FOwnIndex : Shortint;
    ListOne,ListTwo     : TStringList;
    FExtFields,FFields,FExportFields  : string;
    procedure WMSysCommand(var Message : TWMSysCommand); message WM_SYSCOMMAND;
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;

var
  frmMembersREPORT: TfrmMembersREPORT;

const Reportfltr = 'DAT Files|*.DAT|CSV files|*.CSV|All Files|*.*';

const
   
   // NB treeview.items.selectedIndex must be set at design time
   ReportTitles : Array[0..74] of String[40]
                     = {0} ( 'Age Analysis Report',
                       {1}   'Clubs Report',
                       {2}   'Cards',             // Null
                       {3}   'Events',                   // +
                       {4}   'Events Report',
                       {5}   'Monthly Analysis',         // Null
                       {6}   'Export',                   // +                       
                       {7}   'Export Card File',             
                       {8}   'Export Events',                
                       {9}   'Export Subscriptions Due',              
                       {10}  'Export Selective',            
                       {11}  'Free Drink',              // Null                   
                       {12}  'Group Analysis Report',              
                       {13}  'Letters',                 // +                     
                       {14}  'Events Letter',               
                       {15}  'Sections Due Letter',
                       {16}  'Selective Letter',            
                       {17}  'Subs Due Letter',             
                       {18}  'Lucky Member History',               
                       {19}  'Mailing Labels',              
                       {20}  'Members',                 // +
                       {21}  'Member Information Report',
                       {22}  'Member Points Report',               
                       {23}  'Member Query Report',                
                       {24'} 'New Members Information Report',                 
                       {25}  'Top 50 Members Points',             // Null                  
                       {26}  'Sales by Day',            // Null                
                       {27}  'Sections',                // +                   
                       {28}  'Section Analysis Report',
                       {29}  'Section Information Report',         
                       {30}  'Section Points Report',              
                       {31}  'Sponsorship by Section',             // Null                
                       {32}  'Subscriptions',           // +              
                       {33}  'Subscriptions Analysis Report',               
                       {34}  'Subscriptions Invoice',  // null             
                       {35}  'Subscriptions Due Report',                
                       {36}  'Member Voting List',
                       {37}  'Export Code,Known As, Position',                 
                       {38}  'Create Card File ',
                       {39}  'Print Cards',                 
                       {40}  'Dates Letter',
                       {41}  'Receipt Report',
                       {42}  'Subscription Information Report',
                       {43}  'Subs Due Reminder Letter',             
                       {44}  'Subs Due Final Letter',             
                       {45}  'Query',             
                       {46}  'Run Query',             
                       {47}  'Subscriptions Age Report',
                       {48}  'Card Last Used Report',             
                       {49}  'Receipt Letter', // run only from frmPayments             
                       {50}  'Subs By Groups',
                       {51}  'Section Interests Report',
                       {52}  'Points Log Report',
                       {53}  'Member Ages Report',
                       {54}  'New Members by Joined Date Report',
                       {55}  'Member List Report',
                       {56}  'Parent Code Member Report',
                       {57}  'Member Draw Report',
                       {58}  'Member Points Stockmaster Report',               
                       {59}  'Kiosk Report',               
                       {60}  'Member Proposer Seconder Report',               
                       {61}  'MenuMate Points Split Report',               
                       {62}  'PHM Points Log Report',               
                       {63}  'Email Members',               
                       {64}  'Maintenance',               
                       {65}  'Members with No Groups',               
                       {66}  'Members with No Subs',               
                       {67}  'MenuMate Loyalty History Report', // gone  rMenuMatePointsLog            
                       {68}  'Kiosk History Report',               
                       {69}  'Points Audit Report',               
                       {70}  'Members with Many Groups',               
                       {71}  'Members with Many Subs',               
                       {72}  'Top 50 Sales',               
                       {73}  'MenuMate Discounts Report',               
                       {74}  'Sponsorship by Section'
                             );

implementation                            
{$R *.DFM}

uses
    Global,    // StockMaster
    dmMembers,
    rInformation, // StockMaster
    rAnalysis,
    rPointsStockmaster,
    rCurrentPoints,
    rAgeAnalysis,
    rNewMembers,
    rVotingList,                           
    MembersClasses,
    rEventsReport,
    rLuckyHistory,
    rSubAnalysis,
    rSectionInform, 
    rMailingLabels, 
    MembersLetter, 
    MembersEmail, 
    rDueLetter, 
    //SQLEdit,
    MemberMenu, 
    MembersQUERY, rReceiptReport, rInvoiceLandscape, rSubsByGroups,
  rPointsLog, rMemberList, rParentCode, rMemberDraw, rKioskReport,
  rProposerSeconder, rPointsSplit, rPHMPointsLog,
  rKioskHistory, dmMenuMate, rMMPointsAudit, rSponsorshipBySection,
  rTopSales, rMMDiscounts;
const
    F = False;
    T = True;
    k = 999;
    StrMonths : array[0..11] of string[20] = ('January','Feburary','March','April','May','June','July','August','September',
                   'October','November', 'December' );
var
   mMonths : string[20];
{ **************************************************************************** }
class function TfrmMembersREPORT.ScreenCode : Integer;
begin Result := Global.scMembersReport; end;
{ **************************************************************************** }
class function TfrmMembersREPORT.ScreenName : String;
begin Result := 'Members Report'; end;
{ **************************************************************************** }
class function TfrmMembersREPORT.ScreenDesc : String;
begin Result := '  Members Report Screen'; end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.wmSysCommand(var Message : TWMSysCommand);
begin
     case message.CmdType of
        SC_CLOSE : begin
           if TreeView.Enabled = False then 
              SetTreeView;   // x pressed on form
           DefaultHandler(Message);
        end;
     else
        //do the default for everything else
        inherited;
     end;
end;  
{ **************************************************************************** }
procedure TfrmMembersREPORT.FormCreate(Sender: TObject);
var i : shortint;
begin
     inherited;
     FpnlWidth := pnlSpace.Width;
     ListOne := TStringList.Create;
     ListTwo := TStringList.Create;
     dm.qryReport1.Open;
     cbxMonths.ItemIndex := 0;
     FbtnQuery := False;
     //ResetScrollBoxOptions;
     FOwnIndex := 0;
     FRunOnce    := False;
     FHoldReport := False;
     FTblName := '';
     ReadPrinters;
     if Global.ReportPrinterIndex <> -1 then
        Printer.PrinterIndex := Global.ReportPrinterIndex
     else
        Printer.PrinterIndex := -1; // set to default   
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.FormDestroy(Sender: TObject);
begin
     if assigned(MergeDoc) then 
        MergeDoc.Free;
     if dm.OpExcel.Connected = True then
        dm.OpExcel.Connected := false;
     if dm.OpWord.Connected = True then
        dm.OpWord.Connected := false;
     if dm.qryReport1.active then
         dm.qryReport1.active := false;
     if ListOne <> nil then ListOne.Free;    
     if ListTwo <> nil then ListTwo.Free;    
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.FormDeactivate(Sender: TObject);
begin
     inherited;
     ResetLookups;
     dm.IBTransactionCommit; // Commit changes on deactivate
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.FormActivate(Sender: TObject);
begin
     inherited;
     QryActiveOrRefresh ( dm.qryReport1 );  // leave for letters/queries name loookup
     SetupSaveDialog('*.dat',Reportfltr,'Save to','member','');
     if TreeView.Selected = nil then begin // first time only
        TreeView.Selected := TreeView.Items[0];
        scbOptions.Update;
     end;   
     // if gone into frmMembersLetters  then reopen
     //if ReportTitles[FOwnIndex] = 'Selective Letter' then
     //    QryActiveOrRefresh ( dm.qryReport1 );
     //scbOptions.VertScrollBar.Visible := False;
end;        
{ **************************************************************************** }
procedure TfrmMembersREPORT.menuPrinterSetupClick(Sender: TObject);
begin
     dm.PrintDialog.Execute;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.menuExitClick(Sender: TObject);
begin
     // has to disable hold and drop temp tables only if Popup Menu
     // Report Hold/Free is Selected
     //Global.CentreText( 'Hornby WMC' , 40);
     if not TreeView.Enabled then
        ReportHoldFree1Click(Sender); 
     Close;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.TreeViewDblClick(Sender: TObject);
// double click on the tree view
begin
     if FOwnIndex <> TreeView.selected.SelectedIndex then
        TreeViewChange( Sender,TreeView.selected ); 
     menuPreview.OnClick(menuPreview);
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.TreeViewChange(Sender: TObject;  Node: TTreeNode);
var i : smallint;  
begin
   // NB treeview.items.selectedIndex must be set at design time
     ResetScrollBoxOptions; // reset all to false
     FExtFields := ''; FFields := '';     // reset per screen change
     FSectionsExtFields := ''; FSectionsFields := '';
     FSubsExtFields     := ''; FSubsFields     := '';
     FGroupsExtFields   := ''; FGroupsFields   := '';
     FEventsExtFields   := ''; FEventsFields   := '';
     FLocationsFields   := ''; 
     FDiscountsFields   := ''; 
     //scbOptions.VertScrollBar.Visible := False;
     
     lblDateFrom.Caption := 'Date From';
     lblDateTo.Caption := 'Date To';
     
     FOwnIndex := TreeView.selected.SelectedIndex;
     // turn icon off not used 28/06/2001
     for i := 0 to TreeView.Items.Count -1 do
         TreeView.Items[i].StateIndex := -1;
     // must keep 2 images in dm.Imagelist as it wont look at image 0
     TreeView.selected.StateIndex := 1;
     case FOwnIndex of
     0 : SetDefaultOption; // Age Analysis
     1 : begin  // Clubs Report
            SetScrollBoxOptions( F,F,F,F,T,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,T,T,F,F,F );
            SetTops            ( k,k,k,k,150,k,k,50,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,100,0,k,k,k);
         end;
     4 : begin  // 'Events Report'
            SetScrollBoxOptions( F,F,F,T,T,F,F,F,F,T,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,100,150,k,k,k,k,50,k,k,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
         end;
     7 : begin //Export Card File
            SetScrollBoxOptions( F,F,F,T,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,T,T,T,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,50,k,k,k,k,k,0,k,k,k,k,k,k,k,k,k,250,150,200,100,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
         end;
     8 : begin  //Export Events
            SetScrollBoxOptions( F,F,F,F,F,F,F,F,T,F,F,F,F,F,T,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,k,k,k,k,k,50,k,k,k,k,k,0,k,k,k,k,k,100,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
         end;   
     9 : begin  //Export Subs Due
            SetScrollBoxOptions( F,F,F,T,F,T,F,F,F,T,F,F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,T,F,F,T,F,F,F,F,F);
            SetTops            ( k,k,k,200,k,50,k,k,k,0,k,k,k,k,k,k,k,k,k,k,100,k,k,k,k,k,k,k,k,k,k,150,k,k,k,k,k);
            rgpExportParentCode.Top := 250;
            rgpExportParentCode.Visible := true;
            rgpExpiresClick(Sender);
         end;
     10 : begin  //Export Selective
            SetScrollBoxOptions( F,F,F,T,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,T,T,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,0,k,k,k,k,k,50,k,k,k,k,k,k,k,k,k,200,100,150,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
          end;
     12 : SetDefaultOption;  //Group Analysis
     14 : begin  //Events Letter
            SetScrollBoxOptions( F,F,F,T,F,F,F,F,T,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,100,k,k,k,k,50,k,k,k,k,k,0,k,k,k,k,k,k,k,k,k,k,150,k,k,k,k,k,k,k,k,k,k,k);
            QryActiveOrRefresh ( dm.qryReport1 );
          end;  
     40 : begin  //Dates Letter
            SetScrollBoxOptions( F,F,F,T,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,T,T,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,50,k,k,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,100,150,k,k,k,k,k,k,k,k,k,k);
            QryActiveOrRefresh ( dm.qryReport1 );
          end;  
     15 : begin  //Sections Due Letter
            SetScrollBoxOptions( F,F,F,F,F,T,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F);
            SetTops            ( k,k,k,k,k,50,k,k,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
            if dbdtTo.Date   = strToDate(StrToDate1899) then dbdtTo.Date   := now;
            if dbdtFrom.Date = strToDate(StrToDate1899) then dbdtFrom.Date := (now + 30 );
          end;
     16 : begin  //Selective Letter
            SetScrollBoxOptions( F,F,F,T,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,T,F,T,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,150,k,k,k,k,k,100,k,k,k,k,k,k,k,k,k,k,k,k,k,50,k,0,k,k,k,k,k,k,k,k,k,k,k);
            QryActiveOrRefresh ( dm.qryReport1 );
          end;
     17,43,44 : begin  //Subs Due Letter , Subs Due Reminder Letter and Subs Due Final Letter
            SetScrollBoxOptions( F,F,F,F,F,T,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,k,k,50,k,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
            if dbdtTo.Date   = strToDate(StrToDate1899) then dbdtTo.Date   := now + 30;
            if dbdtFrom.Date = strToDate(StrToDate1899) then dbdtFrom.Date := now;
          end;  
     18 : SetScrollBoxOptions( F,F,F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );  //Lucky History
     19 : begin  //Mailing Labels
            SetScrollBoxOptions( T,F,F,T,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,T,T,T,F,F,F,F,F,F,F,F,F,F,T,F);
            SetTops            ( 250,k,k,100,k,k,k,k,k,k,50,k,k,k,k,k,k,k,k,k,k,k,150,0,200,k,k,k,k,k,k,k,k,k,k,300,k);
            rgpMemberOnLabel.Visible := true;
            rgpMemberOnLabel.Top    := 350;
            //scbOptions.VertScrollBar.Visible := True;
            
            rgpLinksClick(Sender);
          end;
     63 : begin  //Email Members
            SetScrollBoxOptions( F,F,F,T,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F);
            SetTops            ( k,k,k,100,k,k,k,k,k,k,50,k,k,k,k,k,k,k,k,k,k,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k);
            rgpLinksClick(Sender);
          end;
     //20 : ;  //Members +
     21 : begin  //Member Information`
            SetScrollBoxOptions( T,F,T,F,T,F,F,T,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,T,F,F,F,F );
            SetTops            ( 250,k,150,k,200,k,k,50,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,100,k,k,k,k);
          end;
     22 : begin  //Member Points
            SetScrollBoxOptions( F,F,F,F,T,F,F,F,F,T,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,k,50,k,k,k,k,0,k,k,100,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
          end;
     58 : rgpReportByGroup.Visible := true; // Member Points Stockmaster
     65,66,70,71 : SetDefaultOption; // maintenance reports
     23 : SetScrollBoxOptions( F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F);  //Member Query
     24 : SetScrollBoxOptions( F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );  //New Members
     25 : begin   //Top Members
            SetScrollBoxOptions( F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,k,k,k,k,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
          end;
     //26 : ;  //Sales by Day
     //27 : ;  //Sections +
     28 : SetDefaultOption;  //Section Analysis
     68 : begin     //Kiosk History Report
            SetScrollBoxOptions( F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
          end;
     72 : begin 
            gbxDates.Visible     := True; 
            gbxTextInput.Visible := True;        
          end;  
     73 : begin 
            gbxDiscounts.Visible := True;        
          end;  
     69 : begin 
            gbxDates.Visible    := True; 
            gbxMMDates.Visible  := True; 
            dbdtMMDateFrom.Date := Date;
            dbdtMMDateFrom.Time := StrToTime('05:00:00');
            dbdtMMDateTo.Date   := Date + 1;
            dbdtMMDateTo.Time   := StrToTime('05:00:00');
            
          end; //Points Audit
     
     29 : begin  //Section Information
            SetScrollBoxOptions( F,F,F,T,T,F,F,F,F,F,T,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,50,150,k,k,k,k,k,0,k,k,k,k,k,k,100,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
          end;
     51 : SetDefaultOption;
            
     52 : SetScrollBoxOptions( F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
       
     53 : SetScrollBoxOptions( F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F );
     54,60 : begin
             SetScrollBoxOptions( F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
             if dbdtTo.Date   = strToDate(StrToDate1899) then dbdtTo.Date   := now;
             if dbdtFrom.Date = strToDate(StrToDate1899) then dbdtFrom.Date := IncMonth(now,-1) ;
          end;  
     42 : begin  //Subs Information
            SetScrollBoxOptions( F,F,F,T,T,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,50,100,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
          end;
     30 : begin  //Section Points
            SetScrollBoxOptions( F,F,F,F,F,F,F,F,F,F,T,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,k,k,k,k,k,k,k,0,k,50,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
          end;
     31 : begin
             gbxLocations.Visible := True; gbxLocations.Top := 0; //Sponsorship
             gbxDates.Visible     := True;
          end;   
     74 : begin
             gbxLocations.Visible := True; gbxLocations.Top := 0; //Sponsorship
             gbxDates.Visible     := True;
          end;   
     //32 : ;  //Subscriptions +
     33 : SetDefaultOption;  //Subs Analysis
     34 : begin   //Subs Invoice
            SetScrollBoxOptions( F,F,F,T,F,T,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,T );
            SetTops            ( k,k,k,50,k,100,k,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,100);
            lblDateFrom.Caption := 'Expiry Date From';
            lblDateTo.Caption   := 'Expiry Date To';
          end; 
     35 : begin  //Subs Monthly
            SetScrollBoxOptions( F,F,F,F,F,T,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,k,k,50,k,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
          end;
     36 :  SetScrollBoxOptions( F,F,F,F,F,F,F,F,F,T,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F ); //Voting List
     37 :  begin
              SetScrollBoxOptions( F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F ); //Export Code , Known As, Position
           end;   
     38 :  SetDefaultOption;//SetScrollBoxOptions( F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F );  // Create Card File
     46 :  begin
             SetScrollBoxOptions( F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F );  // Queries
             //QryActiveOrRefresh ( dm.qryReport1 );
           end;
     56 :  SetDefaultOption;  // Parent Code Member
     39 :  SetDefaultOption;  // Print Cards
     41 : begin  // Receipt Report
            SetScrollBoxOptions( F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F );
            SetTops            ( k,k,k,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,50,k,k,k,k,k,k,k,k,k);
            if dbdtTo.Date = strToDate(StrToDate1899)   then dbdtTo.Date := now ;
            if dbdtFrom.Date = strToDate(StrToDate1899) then dbdtFrom.Date := now;
          end;
     47 : begin  //  'Subscriptions Age Report'
            SetScrollBoxOptions( F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F );
            SetTops            ( k,k,k,k,k,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,50,k,k,k,k,k,k);
            if dbdtFrom.Date = strToDate(StrToDate1899) then dbdtFrom.Date := (now - 30 );
          end;
     48 : begin  //  'Card last Used Report'   
            SetScrollBoxOptions( F,F,F,T,F,T,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F);
            SetTops            ( k,k,k,100,k,50,k,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
          end;
     50 : begin  //  'Subs By Group'
            SetScrollBoxOptions( F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F);
            SetTops            ( k,k,k,k,k,k,k,k,k,0,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k);
          end;
     55 : begin // Member List
            SetScrollBoxOptions( F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F);
          end;
     57 : SetDefaultOption;  //Member Draw Report
     59 : begin  //  'Kiosk Report'
            SetScrollBoxOptions( F,F,F,F,F,F,F,F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,T,F,F);
            SetTops            ( k,k,k,k,k,k,k,k,k,k,k,k,50,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,0,k,k);
          end;
     61 : SetScrollBoxOptions( F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F);  //  'Menu Mate Report'
     62 : SetScrollBoxOptions( F,F,F,F,F,T,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F);  //  'Menu Mate Report'
     end;                    
     //scbOptions.VertScrollBar.Visible := False;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.SetScrollBoxOptions(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,ab,ac,ad,ae,af,ag,ah,ai,aj,ak : Boolean );
// different report selections
begin
     gbxMailJoin.Visible        := a;  // 1
     rgpProduct.Visible         := b;  // 2
     rgpMembersOther.Visible    := c;  // 3
     rgpMembers.Visible         := d;  // 4
     rgpSortBy.Visible          := e;  // 5
     gbxDates.Visible           := f;  // 6
     gbxLocations.Visible       := g;  // 7
     gbxSubs.Visible            := h;  // 8
     gbxEvents.Visible          := i;  // 9
     gbxGroups.Visible          := j;  // 10
     gbxSections.Visible        := k;  // 11
     gbxNoOptions.Visible       := l;  // 12
     rgpLines.Visible           := m;  // 13
     rgpAlpha.Visible           := n;  // 14
     gbxMonths.Visible          := o;  // 15
     gbxQuery.Visible           := p;  // 16
     rgpMembersSections.Visible := q;  // 17
     rgpSortBySections.Visible  := r;  // 18
     rgpSelection.Visible       := s;  // 19
     gbxMemo.Visible            := t;  // 20
     gbxExport.Visible          := u;  // 21
     gbxFields.Visible          := v;  // 22
     rgpNewMember.Visible       := w;  // 23
     rgpLinks.Visible           := x;  // 24
     rgpSortLabels.Visible      := y;  // 25
     gbxLetter.Visible          := z;  // 26
     gbxAges.Visible            := aa; // 27
     rpgReceiptSortBy.Visible   := ab; //28
     rgpExpires.Visible         := ac; //29
     gbxQueries.Visible         := ad; //30
     gbxAge.Visible             := ae; //31
     rgpExportOptions.Visible   := af; //32
     gbxMemberMulti.Visible     := ag; //33
     gbxClubs.Visible           := ah; //34
     rgpToday.Visible           := ai; //35
     rgpMembersEmail.Visible    := aj; //36
     rgpPayment.Visible         := ak; //37
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.SetTops(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,ab,ac,ad,ae,af,ag,ah,ai,aj,ak : smallint );
// preferred options are set to the top
begin
     if gbxMailJoin.Visible then gbxMailJoin.Top               := a; // 1
     if rgpProduct.Visible then rgpProduct.Top                 := b; // 2
     if rgpMembersOther.Visible then rgpMembersOther.Top       := c; // 3
     if rgpMembers.Visible then rgpMembers.Top                 := d; // 4
     if rgpSortBy.Visible then rgpSortBy.Top                   := e; // 5
     if gbxDates.Visible then gbxDates.Top                     := f; // 6
     if gbxLocations.Visible then gbxLocations.Top             := g; // 7
     if gbxSubs.Visible then gbxSubs.Top                       := h; // 8
     if gbxEvents.Visible then gbxEvents.Top                   := i; // 9
     if gbxGroups.Visible then gbxGroups.Top                   := j; // 10
     if gbxSections.Visible then gbxSections.Top               := k; // 11
     if gbxNoOptions.Visible then gbxNoOptions.Top             := l; // 12
     if rgpLines.Visible then rgpLines.Top                     := m; // 13
     if rgpAlpha.Visible then rgpAlpha.Top                     := n; // 14
     if gbxMonths.Visible then gbxMonths.Top                   := o; // 15
     if gbxQuery.Visible then gbxQuery.Top                     := p; // 16
     if rgpMembersSections.Visible then rgpMembersSections.Top := q; // 17
     if rgpSortBySections.Visible then rgpSortBySections.Top   := r; // 18
     if rgpSelection.Visible then rgpSelection.Top             := s; // 19
     if gbxMemo.Visible then gbxMemo.top                       := t; // 20
     if gbxExport.Visible then gbxExport.top                   := u; // 21
     if gbxFields.Visible then  gbxFields.top                  := v; // 22
     if rgpNewMember.Visible then rgpNewMember.Top             := w; // 23
     if rgpLinks.Visible then rgpLinks.Top                     := x; // 24
     if rgpSortLabels.Visible then gbxLetter.Top               := y; // 25
     if gbxLetter.Visible then rgpSortLabels.Top               := z; // 26
     if gbxDAtes.Visible then gbxDAtes.Top                     := aa; // 27
     if rpgReceiptSortBy.Visible then rpgReceiptSortBy.Top     := ab; //28
     if rgpExpires.Visible then rpgReceiptSortBy.Top           := ac; //29
     if gbxQueries.Visible then gbxQueries.Top                 := ad; //30
     if gbxAge.Visible then gbxAge.Top                         := ae; //31
     if rgpExportOptions.Visible then rgpExportOptions.Top     := af; // 32
     if gbxMemberMulti.Visible then gbxMemberMulti.Top         := ag; // 33
     if gbxClubs.Visible then gbxClubs.Top                     := ah; //34
     if rgpToday.Visible then rgpToday.Top                     := ai; //35
     if rgpMembersEmail.Visible then rgpMembersEmail.Top       := aj; //36
     if rgpPayment.Visible then rgpPayment.Top                 := ak; //37

end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.ResetScrollBoxOptions;
var i : integer;
    temp : TControl;
begin
     for i := 0 to ComponentCount -1 do begin
        if Components[i].InheritsFrom(TControl) then begin
           temp := (Components[i] as TControl);
           if temp.Parent = scbOptions then 
              temp.visible := false;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.SetDefaultOption;
begin
     ResetScrollBoxOptions;
     gbxNoOptions.Visible := true;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.PrintOrPreview ( frmQR : TForm ; qrQR : TQuickRep );
// Most reports look here, Email gives no SaveDialog Option
begin
     with frmQR do begin
        Screen.Cursor := crDefault;
        case FReportChoice of
           0 : qrQR.Print;
           1 : qrQR.Preview;
           2 : begin 
                  if dm.SaveDialog.Execute then begin
                  
                    // if FOwnIndex in [57,59] then begin // uses cdsTemp
                        Global.CreateAsciiFile( dm.SaveDialog.FileName,False,dm.qryReport,nil,True,False);
                     
                  end;   
               end;      
           3 : ExportToExcel;
           4 : Email;
           5,6 : ExportToWord;      // Word Mailing Labels New/Existing
           7 : MailingLabels;
           8 : EmailMembers;
           
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.PrintOrPreviewExport;
// see Export Reports { Card file, Events, Subs Due, Selective }
begin
     Screen.Cursor := crDefault;
     case FReportChoice of 
        0 : CreateList(True,ReportTitles[FOwnIndex]);  // print
        1 : CreateList(False,ReportTitles[FOwnIndex]); // preview
        2 : PreCreateAsciiFile;                        // export
        3 : ExportToExcel;                             // excel
        4 : Email;                                     // email
        5,6 : ExportToWord;                            // Word Mailing Labels New/Existing
        7 : MailingLabels;
        8 : EmailMembers;
     end;                     
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.menuPrintClick(Sender: TObject);
type  TNoReports = set of 1..64;
const NoReports   : TNoReports = [2,3,5,6,11,13,20,26,27,32,45,64];
      //PointsReports : TPointsReports = [22,58,69];
      LetterMsg = 'Letter not found or invalid';
      ClearFlagMsg = 'Clear the New Members flag?';
      ClearInterestsMsg = 'Clear All Members from Section Interests';
var s : String;
    i : smallint;
    Years, Months, Days : Word;
    TempDate : TDateTime;
    MenuMatePointsUpdate : TMenuMatePointsUpdate;
//  print , preview , Export , Email , Excel look here
begin
     SetupSaveDialog('*.dat',Reportfltr,'Save to','member','');
     // Reports [57,59] use a TClientDataSet
     // Change ExportToExcel and ExportToWord
     // Email and CreateAsciiFile are not supported as yet

     if FOwnIndex in NoReports then exit;// no reports for these options
     if not PreReportCheck then exit;
     // TToolButton and TMenuItem and FormKeyUp
     if Sender is TToolButton then
        FReportChoice := TToolButton(Sender).Tag
     else if Sender is TMenuItem then 
        FReportChoice := TMenuItem(Sender).Tag
     else 
        FReportChoice := menuPreview.Tag;  //preview

     if FOwnIndex in [22,58,69] then MenuMatePointsUpdate.MembersPointsUpdate('-1');
     
     if not FRunOnce then TooBusy(ExtractRptMsg);
     //if FReportChoice = 1 then CheckToolbar;   // preview - move toolbar down
     case FOwnIndex of
        0  : begin // no options     Age Analysis Rpt
             try
               frmAgeAnalysis := TfrmAgeAnalysis.Create(nil);
               with frmAgeAnalysis do begin
                  lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  if FRunOnce = False then begin
                     FtblName := 'TEMP' + FormatDateTime('sszzz',now); //unique table by user
                     if not Extract(True,Printer.PrinterIndex,FtblName) then begin Free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  PrintOrPreview(frmAgeAnalysis,qr);
               end;
            finally
               //if FRunOnce = False then DropTempTables;
               NotBusy;
               frmAgeAnalysis.Free;
            end;
            end;
        1  : begin // 3 options    Clubs Report
             try
               frmInformation := TfrmInformation.Create(nil);
               with frmInformation do begin
                  if FRunOnce = False then begin
                     if not ClubExtract(Printer.PrinterIndex,dbcbGender.ItemIndex,FMembers,FClubsExtFields,FSortBy,FSubsExtFields ) then begin free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  if FMembersText = '' then FMembersText := 'All Members';
                  case dbcbGender.ItemIndex of
                     0 : s := ' All Members';
                     1 : s := ' Male Members';
                     2 : s := ' Female Members';
                  end;
                  lblReportTitle.Caption := format('%s for %s,%s,%s,%s,%s ',[ReportTitles[FOwnIndex],FMembersText,FSubText,FClubText,s,FSortByText]);
                  PrintOrPreview(frmInformation,qr);
               end;
            finally
               NotBusy;
               frmInformation.Free;
            end;
            end;
        12  : begin // no options    Group Analysis Rpt
             try
               frmAnalysis := TfrmAnalysis.Create(nil);
               with frmAnalysis do begin
                  if FRunOnce = False  then begin 
                     FtblName := 'TEMP' + FormatDateTime('sszzz',now); //unique table by user
                     if not Extract(True,Printer.PrinterIndex,FtblName) then begin Free; Exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := ReportTitles[FOwnIndex] + ' - Financial Members';
                  PrintOrPreview(frmAnalysis,qr);
               end;
            finally
               //if FRunOnce = False then DropTempTables;
               NotBusy;
               frmAnalysis.Free;
            end;
            end;
        28 : begin   // no Options - Section Analysis Rpt
            // both SectionAnalysis and SubAnalysis use frmSubAnalysis
             try
               frmSubAnalysis := TfrmSubAnalysis.Create(nil);
               with frmSubAnalysis do begin
                  if FRunOnce = False then begin
                     if not Extract(True,True,Printer.PrinterIndex) then begin Free; Exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  PrintOrPreview(frmSubAnalysis,qr);
               end;
             finally
                NotBusy;
                frmSubAnalysis.Free;
             end;
             end;
        33 : begin  // no Options - Subs Analysis Rpt
           // both SectionAnalysis and SubAnalysis use frmSubAnalysis
             try
               frmSubAnalysis := TfrmSubAnalysis.Create(nil);
               with frmSubAnalysis do begin
                  if FRunOnce = False then begin
                     if not Extract(True,False,Printer.PrinterIndex) then begin free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  PrintOrPreview(frmSubAnalysis,qr);
               end;
             finally
                NotBusy;
                frmSubAnalysis.Free;
             end;
             end;
        22  : begin // group + line (2) options  Member Points Report
             // Section Points and Member Points use frmCurrentPoints
             try
               frmCurrentPoints := TfrmCurrentPoints.Create(nil);
               with frmCurrentPoints do begin
                  if FRunOnce = False then begin
                     if not Extract(True,Printer.PrinterIndex,rgpLines.ItemIndex,FExtFields,FSortBy) then begin free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s for %s,%s',[ReportTitles[FOwnIndex],FGroupText,FSortByText]);
                  PrintOrPreview(frmCurrentPoints,qr);
               end;
            finally
               NotBusy;
               frmCurrentPoints.Free;
            end;
            end;
        58  : begin //   Member Points StockmasterReport
             try
               frmPointsStockmaster := TfrmPointsStockmaster.Create(nil);
               with frmPointsStockmaster do begin
                  if FRunOnce = False then begin
                     if not Extract(Printer.PrinterIndex,rgpReportByGroup.ItemIndex) then begin free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s',[ReportTitles[FOwnIndex]]);
                  PrintOrPreview(frmPointsStockmaster,qr);
               end;
            finally
               NotBusy;
               frmPointsStockmaster.Free;
            end;
            end;
        25  : begin 
             // top 50 Member Points use frmCurrentPoints
             try
               frmCurrentPoints := TfrmCurrentPoints.Create(nil);
               with frmCurrentPoints do begin
                  if FRunOnce = False then begin 
                     if not ExtractTop50(True,Printer.PrinterIndex,FExtFields) then begin free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := ReportTitles[FOwnIndex] + ' for ' + FGroupText;
                  PrintOrPreview(frmCurrentPoints,qr);
               end;
            finally
               NotBusy;
               frmCurrentPoints.Free;
            end;
            end;
        59  : begin // Kiosk Report 2 Options
             try
               frmKioskReport := TfrmKioskReport.Create(nil);
               with frmKioskReport do begin
                  if FRunOnce = False then begin 
                     if not Extract(Printer.PrinterIndex,rgpLines.ItemIndex,rgpToday.ItemIndex) then begin free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  case rgpToday.ItemIndex of
                     0 : lblReportTitle.Caption := ReportTitles[FOwnIndex] + ' for ' + formatDateTime('dd mmm yyyy',date);
                     1 : lblReportTitle.Caption := ReportTitles[FOwnIndex] + ' for ' + formatDateTime('dd mmm yyyy',date -1);
                  end;
                  PrintOrPreview(frmKioskReport,qr);
               end;
            finally
               NotBusy;
               frmKioskReport.Free;
            end;
            end;
        60  : begin // 1 option   New Member Proposer Seconder
             try
               frmProposerSeconder := TfrmProposerSeconder.Create(nil);
               with frmProposerSeconder do begin
                  lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  if FRunOnce = False then begin
                     FtblName := 'TEMP' + FormatDateTime('sszzz',now); //unique table by user
                     if not Extract(Printer.PrinterIndex,FtblName,FDateFrom,FDateTo) then begin Free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s for Joined Dates from %s to %s',[ReportTitles[FOwnIndex],GetShortMonthDate(FDateFrom),GetShortMonthDate(FDateTo)]); 
                  PrintOrPreview(frmProposerSeconder,qr);
               end;
            finally
               //if FRunOnce = False then DropTempTables;
               NotBusy;
               frmProposerSeconder.Free;
            end;    
            end;    
        31  : begin // 1 option   Sponsorship by Section
             try
               frmSponsorshipBySection := TfrmSponsorshipBySection.Create(nil);
               with frmSponsorshipBySection do begin
                  lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  if FRunOnce = False then begin
                     FtblName := 'TEMP' + FormatDateTime('sszzz',now); //unique table by user
                     if not Extract(Printer.PrinterIndex,FDateFrom,FDateTo,FtblName,FLocationsFields) then begin exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s for Dates from %s to %s for %s',[ReportTitles[FOwnIndex],GetShortMonthDate(FDateFrom),GetShortMonthDate(FDateTo),FLocationText]); 
                  PrintOrPreview(frmSponsorshipBySection,qr);
               end;
            finally
               NotBusy;
               frmSponsorshipBySection.Free;
            end;    
            end;    
        74  : begin // 1 option   Sponsorship by Member
             try
               frmSponsorshipBySection := TfrmSponsorshipBySection.Create(nil);
               with frmSponsorshipBySection do begin
                  lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  if FRunOnce = False then begin
                     FtblName := 'TEMP' + FormatDateTime('sszzz',now); //unique table by user
                     if not ExtractMember(Printer.PrinterIndex,FDateFrom,FDateTo,FtblName,FLocationsFields) then begin exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s for Dates from %s to %s for %s',[ReportTitles[FOwnIndex],GetShortMonthDate(FDateFrom),GetShortMonthDate(FDateTo),FLocationText]); 
                  PrintOrPreview(frmSponsorshipBySection,qr);
               end;
            finally
               NotBusy;
               frmSponsorshipBySection.Free;
            end;    
            end;    
        61  : begin // 1 option   Menu Mate Report
             try
               frmPointsSplit := TfrmPointsSplit.Create(nil);
               with frmPointsSplit do begin
                  lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  if FRunOnce = False then begin
                     if not Extract(Printer.PrinterIndex,FDateFrom,FDateTo) then begin Free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s for Dates from %s to %s',[ReportTitles[FOwnIndex],GetShortMonthDate(FDateFrom),GetShortMonthDate(FDateTo)]); 
                  PrintOrPreview(frmPointsSplit,qr);
               end;
            finally
               NotBusy;
               frmPointsSplit.Free;
            end;    
            end;    
        {67  : begin // dates option  Menu Mate Points Log Report
             try
               frmMenuMatePointsLog := TfrmMenuMatePointsLog.Create(nil);
               with frmMenuMatePointsLog do begin
                  lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  if FRunOnce = False then begin
                     if not Extract(Printer.PrinterIndex,FDateFrom,FDateTo) then begin Free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s for Dates from %s to %s',[ReportTitles[FOwnIndex],GetShortMonthDate(FDateFrom),GetShortMonthDate(FDateTo)]); 
                  PrintOrPreview(frmMenuMatePointsLog,qr);
               end;
            finally
               NotBusy;
               frmMenuMatePointsLog.Free;
            end;    
            end;      }
        68  : begin // dates   Kiosk History Report
             try
               frmKioskHistory := TfrmKioskHistory.Create(nil);
               with frmKioskHistory do begin
                  lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  if FRunOnce = False then begin
                     if not Extract(Printer.PrinterIndex,FDateFrom,FDateTo) then begin Free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s for Dates from %s to %s',[ReportTitles[FOwnIndex],GetShortMonthDate(FDateFrom),GetShortMonthDate(FDateTo)]); 
                  PrintOrPreview(frmKioskHistory,qr);
               end;
            finally
               NotBusy;
               frmKioskHistory.Free;
            end;    
            end;    
        69  : begin // dates    Points Audit Report
             try
               if not dmMM.MMDatabase.Connected then begin
                  showmessage('You must be connected to MenuMate to run this Report');
                  exit;
               end; 
               frmPointsHistory := TfrmPointsHistory.Create(nil);
               with frmPointsHistory do begin
                  lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  if FRunOnce = False then begin
                     FtblName := 'TEMP' + FormatDateTime('sszzz',now); //unique table by user
                     if not Extract(Printer.PrinterIndex,FDateFrom,FDateTo,FMMDateTimeFrom,FMMDateTimeTo,FtblName) then begin Free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s for Dates from %s to %s, MenuMate Dates from %s to %s ',[ReportTitles[FOwnIndex],GetShortMonthDate(FDateFrom),GetShortMonthDate(FDateTo),GetShortMonthDateTime(FMMDateTimeFrom),GetShortMonthDateTime(FMMDateTimeTo)]); 
                  PrintOrPreview(frmPointsHistory,qr);
               end;
            finally
               NotBusy;
               frmPointsHistory.Free;
            end;    
            end;    
        72  : begin // dates    Top 50 Sales
             try
               if not dmMM.MMDatabase.Connected then begin
                  showmessage('You must be connected to MenuMate to run this Report');
                  exit;
               end; 
               dm.qryReport.Close;
               dm.IBCommitRetain;
               // for consistency for Exporting etc
               //dm.qryReport.Database    := dmMM.MMDataBase;
               //dm.qryReport.Transaction := dmMM.MMTransaction;
               frmTopSales := TfrmTopSales.Create(nil);
               with frmTopSales do begin
                  //lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  if FRunOnce = False then begin
                     FtblName := 'TEMP' + FormatDateTime('sszzz',now); //unique table by user
                     if not Extract(Printer.PrinterIndex,StrToInt(edtTextInput.Text),FDateFrom,FDateTo,FtblName) then begin Free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s for Dates from %s to %s',[ReportTitles[FOwnIndex],GetShortMonthDate(FDateFrom),GetShortMonthDate(FDateTo)]); 
                  PrintOrPreview(frmTopSales,qr);
               end;
            finally
               if dmMM.MMTransaction.InTransaction then
                  dmMM.MMTransaction.Commit;
               //dm.qryReport.SQL.Text    := 'Select * from site';
               //dm.qryReport.Database    := dm.IBDataBase;
               //dm.qryReport.Transaction := dm.IBTransaction;
               NotBusy;
               frmTopSales.Free;
            end;                                                                             
            end;    
        73  : begin //     MenuMate Discounts
             try
               if not dmMM.MMDatabase.Connected then begin
                  showmessage('You must be connected to MenuMate to run this Report');
                  exit;
               end; 
               if not MenuMateDiscountsInstalled then begin
                  showmessage('MenuMate Discounts are not installed');
                  exit;
               end;                     
               frmMMDiscounts := TfrmMMDiscounts.Create(nil);
               with frmMMDiscounts do begin
                  if FRunOnce = False then begin
                     if not Extract(Printer.PrinterIndex,FDiscountsExtFields,cbxNoDiscounts.Checked) then begin Free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s for %s',[ReportTitles[FOwnIndex],FDiscountText]); 
                  dmMM.tblDiscounts.Active := true;
                  PrintOrPreview(frmMMDiscounts,qr);
               end;
            finally
               if dmMM.MMTransaction.InTransaction then
                  dmMM.MMTransaction.Commit;
               NotBusy;
               frmMMDiscounts.Free;
            end;                                                                             
            end;    
        62  : begin // 1 option   PHM Points Log Report
             try
               frmPHMPointsLog := TfrmPHMPointsLog.Create(nil);
               with frmPHMPointsLog do begin
                  lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  if FRunOnce = False then begin
                     if not Extract(Printer.PrinterIndex,FDateFrom,FDateTo) then begin Free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s for Dates from %s to %s',[ReportTitles[FOwnIndex],GetShortMonthDate(FDateFrom),GetShortMonthDate(FDateTo)]);
                  PrintOrPreview(frmPHMPointsLog,qr);
               end;
            finally
               NotBusy;
               frmPHMPointsLog.Free;
            end;    
            end;    
            
            
        4  : begin // 4 options   gbxEvents,gbxMonths,gbxMembers,gbxGroups  Events Report
             try
                 frmEventsReport := TfrmEventsReport.Create(nil);
                 with frmEventsReport do begin
                    if FRunOnce = False then begin
                       if not Extract(True,Printer.PrinterIndex,cbxMonths.ItemIndex,FExtFields,FExtFields,FMembers,rgpSortby.ItemIndex) then begin free; exit; end;
                       if FHoldReport then FRunOnce := True;
                    end;
                    //if FMembersText <> '' then s := ',' + FMembersText + ' ';
                    lblReportTitle.Caption := Format('%s for %s for %s',[ReportTitles[FownIndex],FGroupText,mMonths]);
                    //lblReportTitle.Caption := Format('%s for %s%s,%s for %s',[ReportTitles[FownIndex],FEventText,s,FGroupText,mMonths]);
                    PrintOrPreview(frmEventsReport,qr);
                 end;
              finally
                 NotBusy;
                 frmEventsReport.Free;
              end;
              end;
        23  : begin // 1 options   Member Query
              if FbtnQuery = False then begin
                 showmessage('You must select the data first');
                 exit;
              end;
              NotBusy;
              if dm.qryReport.RecordCount = 0 then begin
                 sm(NoRecordsMsg);
                 exit;
              end;
              PrintorPreviewExport;
            { 

             try
                 frmQueryReport := TfrmQueryReport.Create(nil);
                 with frmQueryReport do begin
                    lblReportTitle.Caption := ReportTitles[FownIndex];
                    qr.PrinterSettings.PrinterIndex := Printer.PrinterIndex;
                    PrintOrPreview(frmQueryReport,qr);
                 end;
              finally
                 frmQueryReport.Release;
              end;
             }
              
              end;
        29  : begin // 3 options  Section Information Rpt - Members by Section or vice vera
             try
                 frmSectionInform := TfrmSectionInform.Create(nil);
                 with frmSectionInform do begin
                    // properties
                    case rgpSortBySections.ItemIndex of
                       0 : SortByMember := True;
                       1 : SortByMember := False;
                    end;
                    if FRunOnce = False then begin
                       if not Extract(True,Printer.PrinterIndex,FExtFields,FSortBySection,FMembers) then begin
                          Free;
                          exit;
                       end;
                       if FHoldReport then FRunOnce := True;
                    end;
                    if FMembersText = '' then begin
                       lblReportTitle.Caption :=
                          format('%s for %s,%s',[ReportTitles[FOwnIndex],FSectionText,FSortBySectionText]);
                    end else begin
                       lblReportTitle.Caption :=
                          format('%s for %s,%s,%s',[ReportTitles[FOwnIndex],FSectionText,FMembersText,FSortBySectionText]);
                    end;
                    if rgpSortBySections.ItemIndex = 0 then
                        qr.Page.LeftMargin := 5.1
                    else
                        qr.Page.LeftMargin := 13.0;
                    PrintOrPreview(frmSectionInform,qr);
                 end;
              finally
                 NotBusy;
                 frmSectionInform.Release;
              end;
              end;
        51  : begin // 1 options  Section Interests Rpt
              try
                 frmSectionInform := TfrmSectionInform.Create(nil);
                 with frmSectionInform do begin
                    if FRunOnce = False then begin
                       if not ExtractInterests(Printer.PrinterIndex) then begin
                          Free;
                          exit;
                       end;
                       if FHoldReport then FRunOnce := True;
                    end;
                    lblReportTitle.Caption := ReportTitles[FOwnIndex];
                    PrintOrPreview(frmSectionInform,qr);
                 end;
                 if Global.ClearInterestsFlag then begin
                    // Clear Sections here
                    if dm.qryReport.RecordCount <> 0 then begin
                       if IfMsgdlg(ClearInterestsMsg) then ClearInterests;
                    end;
                 end;   
              finally
                 NotBusy;
                 frmSectionInform.Release;
              end;
              end;
        42  : begin // 3 options  Subs Information Rpt - uses rSectionInform
             try
                 frmSectionInform := TfrmSectionInform.Create(nil);
                 with frmSectionInform do begin
                    if FRunOnce = False then begin 
                       if not ExtractSubs(Printer.PrinterIndex,FExtFields,FSortBy,FMembers) then begin
                          Free;
                          exit;
                       end;
                       if FHoldReport then FRunOnce := True;

                    end;
                    if FMembersText = '' then begin
                       lblReportTitle.Caption :=
                          format('%s for %s,%s',[ReportTitles[FOwnIndex],FSubText,FSortByText]);
                    end else begin
                       lblReportTitle.Caption :=
                          format('%s for %s,%s,%s',[ReportTitles[FOwnIndex],FSubText,FMembersText,FSortByText]);
                    end;
                    //if rgpSortBySections.ItemIndex = 0 then
                    //    qr.Page.LeftMargin := 5.1
                    //else
                    //    qr.Page.LeftMargin := 13.0;
                    PrintOrPreview(frmSectionInform,qr);
                 end;
              finally
                 NotBusy;
                 frmSectionInform.Release;
              end;
              end;
              
        30  : begin // section + line (2) options  Section Points Rpt
             // Section Points and Member Points use frmCurrentPoints
             try
               frmCurrentPoints := TfrmCurrentPoints.Create(nil);
               with frmCurrentPoints do begin
                  if FRunOnce = False then begin 
                     if not ExtractSection(True,Printer.PrinterIndex,rgpLines.ItemIndex,FExtFields) then begin
                        Free;
                        exit;
                     end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := format('%s for %s',[ReportTitles[FOwnIndex],FSectionText]);
                  PrintOrPreview(frmCurrentPoints,qr);
               end;
            finally
               NotBusy;
               frmCurrentPoints.Free;
            end;
            end;
        8  : begin // 3 options  Export Events
             try
                if FRunOnce = False then begin 
                   if not ExportEvents(cbxMonths.ItemIndex,FExtFields) then exit;
                   if FHoldReport then FRunOnce := True;
                end;   
             finally
                NotBusy;
             end;
             if dm.qryReport.RecordCount = 0 then begin
                sm(NoRecordsMsg);
                exit;
             end;
             PrintorPreviewExport;
             end;
        65,66,70,71  : begin // Members with no Subs or Groups
             try
               frmInformation := TfrmInformation.Create(nil);
               with frmInformation do begin
                  if FRunOnce = False then begin
                     if not MaintExtract(Printer.PrinterIndex,FOwnIndex ) then begin free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption :=  ReportTitles[FOwnIndex];
                  PrintOrPreview(frmInformation,qr);
               end;
            finally
               NotBusy;
               frmInformation.Free;
            end;
            end;
             
         37  : begin // 3 options  Export Code Known As Position
             try
                if FRunOnce = False then begin 
                   if not ExportCodeKnownPosition then exit;
                   if FHoldReport then FRunOnce := True;
                end;   
             finally
                NotBusy;
             end;
             PrintorPreviewExport;
             end;     
        9  : begin // 3 options  Export Subs Due
             try
                if FRunOnce = False then begin
                   if not ExportSubs(FExtFields ) then exit;
                   if FHoldReport then FRunOnce := True;
                end;
             finally
                NotBusy;
             end;
             if dm.qryReport.RecordCount = 0 then begin
                sm(NoRecordsMsg);
                exit;
             end;
             PrintorPreviewExport;
             end;
        38  : begin // no options  Create Card File
             try
                dm.tblCardDetail.Open;
                dm.tblCardMember.Open;
                if dm.tblCardMember.RecordCount = 0 then begin
                   sm('There are no Members in the file');
                   exit; 
                end;
                // no other options to preview/ or report
                case FReportChoice of
                   2 : ExportCards(True);                  // export
                   3 : begin
                         ExportCards(False);
                         ExportCardsExcel(LastFileExported);      // excel
                       end;
                   4 : begin
                         ExportCards(False);
                         Email;                            // email
                       end;
                   else
                      exit;
                end;
                // done with IBRollover
                if MessageDlg('Clear the Cards file',mtConfirmation,[mbYes,mbNo],0) = mrYes then 
                   ClearCardMembers;
             finally
                dm.tblCardMember.Close;
                dm.tblCardDetail.Close;
                NotBusy;
             end;
             end;
        39  : begin // no options  Print Cards
              if (Global.CardPrinterIndex = -1) or
                 (Global.CardPrinterName = '') then begin
                 sm(NoCardPrinterMsg);
                 NotBusy;
                 exit;
              end;
             try
                dm.tblCardMember.Open;
                dm.tblCardDetail.Open;
                if dm.tblCardMember.RecordCount = 0 then begin
                   sm('There are no Members in the file');
                   exit; 
                end;
                // no other options except Print
                case FReportChoice of 
                   0 : PrintFromTable;                  // print
                   else
                      exit;     
                end;
                if MessageDlg('Clear the Cards file',mtConfirmation,[mbYes,mbNo],0) = mrYes then
                   ClearCardMembers;
             finally
                dm.tblCardMember.Close;
                dm.tblCardDetail.Close;
                NotBusy;
             end;
             end;
        24  : begin // 1 options   Groups  -  New Member Report
              //New Member, Monthly Subscriptions Due,Member Ages use this report
               try
                  frmNewMember := TfrmNewMember.Create(nil);
                  with frmNewMember do begin
                     if FRunOnce = False then begin
                        if not Extract(True,True,Printer.PrinterIndex,FExtFields,FDateFrom,FDateTo,rtNewMemberRpt,0,0) then begin
                           Free;
                           exit;
                        end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     lblReportTitle.Caption := ReportTitles[FOwnIndex];
                     PrintOrPreview(frmNewMember,qr);
                  end;
               finally
                  NotBusy;
                  frmNewMember.Free;
                  if dm.qryReport.RecordCount <> 0 then begin
                     if IfMsgdlg(ClearFlagMsg) then ClearMembersFlag;
                  end;
               end;
            end;
        34  : begin // 3 options  Groups rgpMembers & Dates - Subscriptions Invoice
               try
                  frmInvoiceLandscape := TfrmInvoiceLandscape.Create(nil);
                  with frmInvoiceLandscape do begin
                     if FRunOnce = False then begin 
                        if not ExtractSubsExpires(Printer.PrinterIndex,rgpPayment.ItemIndex,FExtFields,FMembers,FDateFrom,FDateTo) then begin free; exit; end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     PrintOrPreview(frmInvoiceLandscape,qr);
                  end;
               finally
                  NotBusy;
                  frmInvoiceLandscape.Free;
               end;
               end;     
        35  : begin // 2 options  Groups and Dates - Monthly Subscriptions Due Report
              //New Member ,Monthly Subscriptions Due and Member Ages use this report
               try
                  frmNewMember := TfrmNewMember.Create(nil);
                  with frmNewMember do begin
                     if FRunOnce = False then begin 
                        if not Extract(True,False,Printer.PrinterIndex,FExtFields,FDateFrom,FDateTo,rtMonthlySubsRpt,0,0) then begin free; exit; end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     lblReportTitle.Caption := format('%s for %s from %s to ' +
                        '%s',[ReportTitles[FOwnIndex],FGroupText,FormatDateTime('dd/mm/yyyy',FDateFrom),FormatDateTime('dd/mm/yyyy',FDateTo)]);
                     PrintOrPreview(frmNewMember,qr);
                  end;
               finally
                  NotBusy;
                  frmNewMember.Free;
               end;
            end;
        53  : begin // 1 options  Ages    Member Ages
              //New Member ,Monthly Subscriptions Due and Member Ages, New Members by Joined date
              // use this report
               try
                  frmNewMember := TfrmNewMember.Create(nil);
                  with frmNewMember do begin
                     if FRunOnce = False then begin 
                        if not Extract(True,cbxDOB.Checked,Printer.PrinterIndex,FExtFields,FDateFrom,FDateTo,
                           rtMemberAgesRpt,StrToIntDef(dbeYearFrom.text,0),StrToIntDef(dbeYearTo.Text,0)) then begin free; exit; end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     case cbxDOB.Checked of
                        true  : lblReportTitle.Caption := format('%s for Members with no Date of Birth' ,[ReportTitles[FOwnIndex]]);
                        false : lblReportTitle.Caption := format('%s for Ages from %s to %s',[ReportTitles[FOwnIndex],dbeYearFrom.Text,dbeYearTo.Text]); 
                     end;
                     PrintOrPreview(frmNewMember,qr);
                  end;
               finally
                  NotBusy;
                  frmNewMember.Free;
               end;
            end;
        54  : begin // 1 options  Ages    New Member by Joined Date
              //New Member ,Monthly Subscriptions Due and Member Ages, New Members by Joined date
              // use this report
               try
                  frmNewMember := TfrmNewMember.Create(nil);
                  with frmNewMember do begin
                     if FRunOnce = False then begin 
                        if not Extract(True,True,Printer.PrinterIndex,FExtFields,FDateFrom,FDateTo,rtNewMemberJoinedRpt,0,0) then begin free; exit; end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     lblReportTitle.Caption := format('%s for Joined Dates from %s to %s',[ReportTitles[FOwnIndex],GetShortMonthDate(FDateFrom),GetShortMonthDate(FDateTo)]); 
                     PrintOrPreview(frmNewMember,qr);
                  end;
               finally
                  NotBusy;
                  frmNewMember.Free;
               end;
            end;
        55  : begin // 1 options  Sort By
               try
                  frmMemberList := TfrmMemberList.Create(nil);
                  with frmMemberList do begin
                     if FRunOnce = False then begin 
                        if not Extract(Printer.PrinterIndex,FSortBy) then begin free; exit; end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     lblReportTitle.Caption := format('%s sorted by %s',[ReportTitles[FOwnIndex],FSortByText]); 
                     PrintOrPreview(frmMemberList,qr);
                  end;
               finally
                  NotBusy;
                  frmMemberList.Free;
               end;
            end;
        56  : begin // No options  
               try
                  frmParentCode := TfrmParentCode.Create(nil);
                  with frmParentCode do begin
                     if FRunOnce = False then begin 
                        if not Extract(Printer.PrinterIndex) then begin free; exit; end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     lblReportTitle.Caption := format('%s',[ReportTitles[FOwnIndex]]); 
                     PrintOrPreview(frmParentCode,qr);
                  end;
               finally
                  NotBusy;
                  frmParentCode.Free;
               end;
            end;
        10  : begin // 5 options  Selective Export
             if FExportFields = '' then begin
                sm('Click on "Select Fields" first ');
                exit;
             end;
             try
                if FRunOnce = False then begin 
                   if not ExportSelective(FMembers,FExtFields) then exit;
                   if FHoldReport then FRunOnce := True;
                end;
             finally
                NotBusy;
             end;
             if dm.qryReport.RecordCount = 0 then begin
                sm(NoRecordsMsg);
                exit;
             end;
             PrintOrPreviewExport;
             end;
        18  : begin // 0 options   Lucky History Report
               try
                  frmLuckyHistory := TfrmLuckyHistory.Create(nil);
                  with frmLuckyHistory do begin
                     if FRunOnce = False then begin 
                        if not Extract(True,Printer.PrinterIndex) then begin Free; exit; end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     lblReportTitle.Caption := ReportTitles[FOwnIndex];
                     PrintOrPreview(frmLuckyHistory,qr);
                  end;
               finally
                  NotBusy;
                  frmLuckyHistory.Free;
               end;
            end;
        7  : begin // 6 options  Export CARD File
              if FExportFields = '' then begin
                 sm('Click on "Select Fields" first');
                 exit;
              end;
              try
                if FRunOnce = False then begin
                   if not ExportCardFile(FMembers,FNewMembers,FExtFields) then exit;
                   if FHoldReport then FRunOnce := True;
                end;   
              finally
                NotBusy;
              end;
              if dm.qryReport.RecordCount = 0 then begin
                sm(NoRecordsMsg);
                exit;
              end;
              PrintOrPreviewExport;   
              if rgpNewMember.ItemIndex = 1 then begin // only if 'New' selected
                 if dm.qryReport.RecordCount <> 0 then begin
                    if IfMsgdlg(ClearFlagMsg) then ClearMembersFlag;
                 end;
              end;
              end;
        19  : begin // 6 options  Mailing Labels + Email Members
               try
                  frmMailingLabels := TfrmMailingLabels.Create(nil);
                  with frmMailingLabels do begin
                     if FRunOnce = False then begin 
                        if not Extract(Printer.PrinterIndex,
                                       rgpLinks.ItemIndex,rgpMemberOnLabel.ItemIndex,FExtFields,FMembers,FNewMembers,
                                       FSortLabels,FMailJoin,FMembersEmail ) then begin
                           Free;
                           exit;
                        end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     PrintOrPreview(frmMailingLabels,qr);
                  end;
               finally
                  NotBusy;
                  frmMailingLabels.Free;
               end;
            end;
        63  : begin // Email Members
               try
                  frmMembersEmail := TfrmMembersEmail.Create(nil);
                  with frmMembersEmail do begin
                     if not Extract(rgpLinks.ItemIndex,FExtFields,FMembers ) then begin
                        //Release;
                        exit;
                     end;
                     ESubject  := '';
                     EBody     := '';
                     EfrmTypes := AsMemberEmail;
                     // if btnEmailMembers is pressed
                     if FReportChoice = 8 then
                        ShowModal
                     else   
                        PrintorPreviewExport;
                  end;
               finally
                  NotBusy;
                  frmMembersEmail.Release;
               end;
            end;
        41  : begin // Dates options - Receipt Report
               try
                  frmReceiptReport := TfrmReceiptReport.Create(nil);
                  with frmReceiptReport do begin
                     if FRunOnce = False then begin 
                        if not Extract(Printer.PrinterIndex,rpgReceiptSortBy.ItemIndex,FDateFrom,FDateTo ) then begin
                           Free;
                           exit;
                        end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     lblReportTitle.Caption := format( '%s  from  %s  to  %s',[ReportTitles[FOwnIndex],GetShortMonthDAte(FDateFrom),GetShortMonthDAte( FDateTo)]) ;
                     PrintOrPreview(frmReceiptReport,qr);
                  end;
               finally
                  NotBusy;
                  frmReceiptReport.Free;
               end;
            end;
        36  : begin // alpha or numeric and Groups (2) options  Voting List
             try
               frmVotingList := TfrmVotingList.Create(nil);
               with frmVotingList do begin
                  if FRunOnce = False then begin 
                     FtblName := 'TEMP' + FormatDateTime('sszzz',now); //unique table by user
                     if not Extract(True,Printer.PrinterIndex,FGroupsExtFields,FtblName,rgpAlpha.ItemIndex) then begin free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := ReportTitles[FOwnIndex] + ' for ' + FGroupText;
                  PrintOrPreview(frmVotingList,qr);
               end;
            finally
               //if FRunOnce = False then DropTempTables;
               NotBusy;
               frmVotingList.Free;
            end;
            end;
        21 : begin // Information   Member Information
             try
               frmInformation := TfrmInformation.Create(nil);
               with frmInformation do begin
                  if FRunOnce = False then begin
                     if not Extract(True,Printer.PrinterIndex,FGroupsExtFields,FSubsExtFields,FSortBy,FMembers,FMembersOther,FMailJoin ) then begin free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  s := '';
                  if FMembersText <> '' then s := FMembersText + ',';
                  if FMembersOtherText <> '' then s := s + FMembersOtherText + ',';
                  if FMailJoinText     <> '' then s := s + FMailJoinText + ',';
                  s := deleteLastComma(s);
                  if s = '' then begin
                     lblReportTitle.Caption :=
                        format('%s for %s,%s,%s',[ReportTitles[FOwnIndex],FGroupText,FSubText,FSortByText]);
                  end else begin
                     lblReportTitle.Caption :=
                        format('%s for %s,%s,%s,%s',[ReportTitles[FOwnIndex],FGroupText,FSubText,s,FSortByText]);
                  end;
                  PrintOrPreview(frmInformation,qr);
               end;
            finally
               NotBusy;
               frmInformation.Free;
            end;
            end;
        47 : begin // Subs Age Report -   uses Member Information Report
             try
               frmInformation := TfrmInformation.Create(nil);
               with frmInformation do begin
                  if FRunOnce = False then begin
                     DecodeDate(dbdtThisDate.Date, Years, Months, Days);
                     // dbdtThisDate.date - dbeAge Years
                     Years := Years - StrToInt(dbeAge.Text);
                     TempDate := EncodeDate(Years, Months, Days);
                     if not AgeExtract(Printer.PrinterIndex,FSubsExtFields,TempDate ) then begin free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption :=
                        format('%s for Age Greater or Equal to %s at %s for %s',[ReportTitles[FOwnIndex],dbeAge.Text,GetShortmonthDAte(dbdtThisDate.Date), FSubText]);
                  PrintOrPreview(frmInformation,qr);
               end;
            finally
               NotBusy;
               frmInformation.Free;
            end;
            end;
        48 : begin // Card Last Used Report -   uses Member Information Report
             try
               frmInformation := TfrmInformation.Create(nil);
               with frmInformation do begin
                  if FRunOnce = False then begin
                     if not CardExtract(Printer.PrinterIndex,FGroupsExtFields,FMembers,dbdtFrom.Date,dbdtTo.Date ) then begin free; exit; end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  if FMembersText = '' then FMembersText := 'All Members';
                  lblReportTitle.Caption :=
                        format('%s for %s from %s to %s',[ReportTitles[FOwnIndex],FMembersText,GetShortmonthDAte(dbdtFrom.Date), GetShortmonthDate(dbdtTo.Date)]);
                  PrintOrPreview(frmInformation,qr);
               end;
            finally
               NotBusy;
               frmInformation.Free;
            end;
            end;
        17,43,44,49  : begin // options      Subs Due Letter/ Subs Due Reminder Letter
               try                          // Subs Due Final Letter and Receipt Letter
                  frmDueLetter := TfrmDueLetter.Create(nil);
                  dm.ibdsLetters.Open;
                  with frmDueLetter do begin
                     if FRunOnce = False then begin 
                        if not GetDatafromLetter( ReportTitles[FOwnIndex] ) then begin
                           sm('Subs Due ' + LetterMsg );
                           exit;
                         end;
                         ExportFieldsList    := ListOne;  // properties
                         ExportExtFieldsList := ListTwo;
                         if not Extract(True,0,Printer.PrinterIndex,FExtFields,FDateFrom,FDateTo,
                                          FExportFields,ReportTitles[FOwnIndex],0,0) then begin
                              Free;
                              exit;
                         end;
                         if FHoldReport then FRunOnce := True;
                     end;
                     PrintOrPreview(frmDueLetter,qr);
                  end;
               finally
                  dm.ibdsLetters.Close;
                  NotBusy;
                  frmDueLetter.Free;
               end;
              end; 
        15  : begin // 2 options      Sections Due Letter
               try
                  frmDueLetter := TfrmDueLetter.Create(nil);
                  dm.ibdsLetters.Open;
                  with frmDueLetter do begin
                     if FRunOnce = False then begin 
                        if not GetDatafromLetter( ReportTitles[FOwnIndex] ) then begin
                            sm('Sections Due ' + LetterMsg);
                            exit;
                         end;
                         ExportFieldsList    := ListOne;  // properties
                         ExportExtFieldsList := ListTwo;
                         if not Extract(True,1,Printer.PrinterIndex,FExtFields,FDateFrom,FDateTo,
                                        FExportFields,ReportTitles[FOwnIndex],0,0) then begin
                            Free;
                            exit;
                         end;
                         if FHoldReport then FRunOnce := True;
                     end;
                     PrintOrPreview(frmDueLetter,qr);
                  end;
               finally
                  dm.ibdsLetters.Close;
                  NotBusy;
                  frmDueLetter.Free;
               end;
              end;
            14  : begin // 4 options      Events Letter
               try
                  frmDueLetter := TfrmDueLetter.Create(nil);
                  dm.ibdsLetters.Open;
                  with frmDueLetter do begin
                     if FRunOnce = False then begin 
                        if not GetDatafromLetter( dblcLetters.Text ) then begin
                           sm(LetterMsg);
                           exit;
                        end;
                        ExportFieldsList    := ListOne;  // properties
                        ExportExtFieldsList := ListTwo;
                        RptMonth := cbxMonths.ItemIndex; //properties
                        RptMembers := FMembers;
                        if not Extract(True,2,Printer.PrinterIndex,FExtFields,FDateFrom,FDateTo,
                                       FExportFields,dblcLetters.Text,0,0) then begin
                           Free;
                           exit;
                        end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     PrintOrPreview(frmDueLetter,qr);
                  end;
               finally
                  dm.ibdsLetters.Close;
                  NotBusy;
                  frmDueLetter.Free;
               end;
              end; 
            40  : begin // 4 options      Dates Letter
               try
                  frmDueLetter := TfrmDueLetter.Create(nil);
                  dm.ibdsLetters.Open;
                  with frmDueLetter do begin
                     if FRunOnce = False then begin 
                        if not GetDatafromLetter( dblcLetters.Text ) then begin
                           sm(LetterMsg );
                           exit;
                        end;
                        ExportFieldsList    := ListOne;  // properties
                        ExportExtFieldsList := ListTwo;
                        //RptMonth := cbxMonths.ItemIndex; //properties
                        RptMembers := FMembers;
                        if not Extract(True,4,Printer.PrinterIndex,FExtFields,FDateFrom,FDateTo,
                                       FExportFields,dblcLetters.Text,
                                       StrToIntDef(dbeYearFrom.Text,0),StrToIntDef(dbeYearTo.Text,0)) then begin
                           Free;
                           exit;
                        end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     PrintOrPreview(frmDueLetter,qr);
                  end;
               finally
                  dm.ibdsLetters.Close;
                  NotBusy;
                  frmDueLetter.Free;
               end;
              end;
            16  : begin // 5 options      Selective Letter
               try
                  frmDueLetter := TfrmDueLetter.Create(nil);
                  dm.ibdsLetters.Open;
                  with frmDueLetter do begin
                     if FRunOnce = False then begin 
                        if not GetDatafromLetter( dblcLetters.Text ) then begin
                           sm(LetterMsg );
                           exit;
                        end;
                        ExportFieldsList    := ListOne;  // properties
                        ExportExtFieldsList := ListTwo;
                        RptBy := rgpLinks.ItemIndex; //properties
                        RptMembers := FMembers;
                        if not Extract(True,3,Printer.PrinterIndex,FExtFields,FDateFrom,FDateTo,
                                       FExportFields,dblcLetters.Text,0,0) then begin
                           Free;
                           exit;
                        end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     PrintOrPreview(frmDueLetter,qr);
                  end;
               finally
                  dm.ibdsLetters.Close;
                  NotBusy;
                  frmDueLetter.Free;
               end;
              end; 
            46  : begin // Queries - 1 Option
               try
                  with frmDueLetter do begin
                     if FRunOnce = False then begin 
                        if not dm.qryOpenCheckEOF(dm.qryReport,dm.qryReport1.FieldByName('SQL').AsString ) then begin
                           sm(NoRecordsMsg);
                           exit;
                        end;
                        if FHoldReport then FRunOnce := True;
                     end;
                     PrintOrPreviewExport;
                  end;
               finally
                  NotBusy;
               end;
              end;                     
          50 : begin // 1 options
             try
                 frmSubsByGroups := TfrmSubsByGroups.Create(nil);
                 with frmSubsByGroups do begin
                    if FRunOnce = False then begin 
                       if not Extract(Printer.PrinterIndex,FGroupsExtFields) then begin
                          Free;
                          exit;
                       end;
                       if FHoldReport then FRunOnce := True;
                    end;
                    lblReportTitle.Caption :=
                          format('%s for %s',[ReportTitles[FOwnIndex],FGroupText]);
                    PrintOrPreview(frmSubsByGroups,qr);
                 end;
              finally
                 NotBusy;
                 frmSubsByGroups.Release;
              end;
              end;
          52 : begin // no options
             try
                 frmPointsLog := TfrmPointsLog.Create(nil);
                 with frmPointsLog do begin
                    if FRunOnce = False then begin 
                       if not Extract(Printer.PrinterIndex,FDateFrom,FDateTo) then begin
                          Free;
                          exit;
                       end;
                       if FHoldReport then FRunOnce := True;
                    end;
                    lblReportTitle.Caption :=
                          format('%s for dates from %s to %s',[ReportTitles[FOwnIndex],GetShortMonthDate(FDateFrom),GetShortMonthDate(FDateTo)]);
                    PrintOrPreview(frmPointsLog,qr);
                 end;
              finally
                 NotBusy;
                 frmPointsLog.Release;
              end;
              end;
        57  : begin // Member Draw Report
             try
               frmMemberDraw := TfrmMemberDraw.Create(nil);
               with frmMemberDraw do begin
                  if FRunOnce = False then begin 
                     if not Extract(Printer.PrinterIndex,'a') then begin
                        Free;
                        exit;
                     end;
                     if FHoldReport then FRunOnce := True;
                  end;
                  lblReportTitle.Caption := ReportTitles[FOwnIndex];
                  PrintOrPreview(frmMemberDraw,qr);
               end;
            finally
               NotBusy;
               frmMemberDraw.Free;
            end;
            end;
              
     end; // end case}
     //if ToolbarMoved then 
     //   MoveToolbar(ToolbarSaveTop,ToolbarSaveTop + ToolbarHeight);
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.dblcMailCodeDropDown(Sender: TObject);
// sets up the gbxMailJoin lookup combos - both are linked to here
// one selection clears the other
// also  dblcLetters to look up the letter name
const SQL1 = 'Select %s from %s';
      SQL2 = 'Select DISTINCT MAILING_CODE from MEMBERS';
      SQL3 = 'Select DISTINCT JOINING_FEE from MEMBERS'; 
      NameField = 'NAME';  MailingField = 'MAILING_CODE'; JoiningFee = 'JOINING_FEE';
      CommentsField = ',COMMENTS,SQL';
begin
     if not dm.qryReport1.Active then
        QryActiveOrRefresh ( dm.qryReport1 );  // leave for letters/queries name loookup
     
     ResetLookups;
     if TwwDBLookupCombo(Sender).Name = dblcLetters.Name then begin
        dm.qryOpen(dm.qryReport1 ,format(SQL1,[NameField,LettersTableName]));
        dblcLetters.lookupField := NameField;
        exit;
     end;
     if TwwDBLookupCombo(Sender).Name = dblcQueries.Name then begin
        dm.qryOpen(dm.qryReport1 ,format(SQL1,[NameField + CommentsField,QueriesTableName]));
        dblcQueries.lookupField := NameField;
        exit;
     end;
     if TwwDBLookupCombo(Sender).Name = dblcMailCode.Name then begin
        dblcJoiningFee.Clear;
        dm.qryOpen(dm.qryReport1 ,SQL2);
        dblcMailCode.lookupField := MailingField;
     end;
     if TwwDBLookupCombo(Sender).Name = dblcJoiningFee.Name then begin
        dblcMailCode.Clear;
        dm.qryOpen(dm.qryReport1 ,SQL3);
        dblcJoiningFee.lookupField := JoiningFee;
     end;
end;
{ **************************************************************************** }
function TfrmMembersREPORT.PreReportCheck : Boolean;
var i : integer;
begin
     Result := False;
     if rgpSortLabels.Visible then begin
        case rgpSortLabels.ItemIndex of
           0 : FSortLabels := ' ORDER by MEMBER';
           1 : FSortLabels := ' ORDER by FULL_NAME';
           2 : FSortLabels := ' ORDER by SURNAME';
           // NB mailing labels use M_ROLLOVER as a base - mailing_code is put into HOME_PHONE 
           3 : FSortLabels := ' ORDER by HOME_PHONE,SURNAME';
           4 : FSortLabels := ' ORDER by SURNAME';//,JOINING_FEE';
        end;
     end;
     if rgpSortBy.Visible then begin
        case rgpSortBy.ItemIndex of
           0 : begin
               FSortBy := ' ORDER by m.MEMBER';
               FSortByText := 'Sorted by Member';
               end;
           1 : begin
               FSortBy := ' ORDER by m.FULL_NAME';
               FSortByText := 'Sorted by Full Name';
               end;
           2 : begin
               FSortBy := ' ORDER by m.SURNAME,m.FIRST_NAME';
               FSortByText := 'Sorted by Surname';
               end;
        end;
     end;
     if rgpSortBySections.Visible then begin
        // rgpSortBySections has got to have rgpSortBy included
        case rgpSortBySections.ItemIndex of
           0 : begin
                 case rgpSortBy.ItemIndex of
                    0 : begin
                          FSortBySection     := ' ORDER by m.MEMBER,SECTION';
                          FSortBySectionText := 'Member by Section Sorted by Member';
                        end;
                    1 : begin
                          FSortBySection     := ' ORDER by m.FULL_NAME,SECTION';
                          FSortBySectionText := 'Member by Section Sorted by Full Name';
                        end;
                    2 : begin
                          FSortBySection     := ' ORDER by m.SURNAME,SECTION';
                          FSortBySectionText := 'Member by Section Sorted by Surname';
                        end;
                  end;
               end;
           1 : begin
                 case rgpSortBy.ItemIndex of
                    0 : begin
                          FSortBySection     := ' ORDER by SECTION,m.MEMBER';
                          FSortBySectionText := 'Section by Member Sorted by Member';
                        end;
                    1 : begin
                          FSortBySection     := ' ORDER by SECTION,m.FULL_NAME';
                          FSortBySectionText := 'Section by Member Sorted by Full Name';
                        end;
                    2 : begin
                          FSortBySection     := ' ORDER by SECTION,m.SURNAME';
                          FSortBySectionText := 'Section by Member Sorted by Surname';
                        end;
                 end;
              end;
        end;
     end;
     if rgpProduct.Visible then begin
        case rgpProduct.ItemIndex of
           0 : FProduct := 'PRODUCT';
           1 : FProduct := 'GROUP';
           2 : FProduct := 'CATERGORY';
           3 : FProduct := 'SUPPLIER';
           4 : FProduct := 'TYPE';
        end;
     end;
     if rgpNewMember.Visible then begin
        case rgpNewMember.ItemIndex of
           0 : FNewMembers := ''; // all
           1 : FNewMembers := ' where M.SAVED = ''N'''; // New Member
           2 : FNewMembers := format(' ,PAYMENTS_DET PD where M.MEMBER = PD.MEMBER ' +
                              ' and PD.PAY_DATE BETWEEN ''%s'' and ' +
                              '''%s'' and PD.PAY_TYPE = 0',[GetShortDate(dbdtFrom.Date),GetShortDAte(dbdtTo.Date)]);
        end;
     end;
     if rgpMembers.Visible then begin
        case rgpMembers.ItemIndex of
           0 : begin
               FMembers := '';                        // All
               FMembersText := '';
               end;
           1 : begin
               FMembers := ' and SUB_PAID > 0';       // financial part and full
               FMembersText := 'Financial Members';
               end;
           2 : begin
               FMembers := ' and SUB_PAID = 0';       // non financial
               FMembersText := 'Non Financial Members';
               end;
           3 : begin
               FMembers := ' and IS_ACTIVE = ''Y''';  // Active
               FMembersText := 'Active Members';
               end;
           4 : begin
               FMembers := ' and IS_ACTIVE = ''N''';  // Non Active
               FMembersText := 'Non Active Members';
               end;
           5 : begin
               FMembers := ' and SUB_PAID = 1';       // Part financial
               FMembersText := 'Part Financial Members';
               end;
        end;
     end;
     // in case they want to select multi options
     if gbxMemberMulti.Visible then begin
        FMembers := '';                        // All
        FMembersText := '';
        if cbxAllMembers.Checked then begin
           FMembers := '';                        // All
           FMembersText := '';
        end;
        if cbxFinancial.Checked then begin
           FMembers := ' and SUB_PAID > 0';       // Financial part and full
           FMembersText := 'Financial Members';
        end;
        if cbxNonFinancial.Checked then begin
           FMembers := FMembers + ' and SUB_PAID = 0';            // non financial
           if FMembersText = '' then
              FMembersText := ' Non Financial Members'
           else
              FMembersText := FMembersText + ' and' + ' Non Financial Members';
        end;
        if cbxActive.Checked then begin
           FMembers := FMembers + ' and IS_ACTIVE = ''Y''';       // Active
           if FMembersText = '' then
              FMembersText := ' Active Members'
           else
              FMembersText := FMembersText + ' and' + ' Active Members';
        end;
        if cbxNonActive.Checked then begin
           FMembers := FMembers + ' and IS_ACTIVE = ''N''';       // Non Active
           if FMembersText = '' then
              FMembersText := ' Non Active Members'
           else
              FMembersText := FMembersText + ' and' + ' Non Active Members';
        end;
        if cbxPartFinancial.Checked then begin
           FMembers := FMembers + ' and SUB_PAID = 1';           // Part financial
           if FMembersText = '' then
              FMembersText := ' Part Financial Members'
           else
              FMembersText := FMembersText + ' and' + ' Part Financial Members';
        end;
        if (FMembers = '') and not (cbxAllMembers.Checked) then begin
           sm('You need to tick at least one box');
           exit;
        end;
     end;
     if gbxTextInput.Visible = True then begin
        if edtTextInput.Text = '' then begin
           sm('You need to enter a number in the box');
           exit;
        end;   
        try
           i := StrToInt(edtTextInput.Text);
        except   
           sm('You need to enter a number in the Text box');
           exit;
        end;   
     end;
     if rgpMembersSections.Visible then begin // for Sections
        case rgpMembersSections.ItemIndex of
           0 : begin
               FMembersSections := '';                  // All
               FMembersSectionsText := '';
               end;
           1 : begin
               FMembersSections := ' and PAID = ''Y'''; // financial
               FMembersSectionsText := 'Financial Members';
               end;
           2 : begin
               FMembersSections := ' and PAID = ''N'''; // non financial
               FMembersSectionsText := 'Non Financial Members';
               end;
        end;
     end;
     if rgpMembersOther.Visible then begin
        case rgpMembersOther.ItemIndex of
           0 : begin
               FMembersOther := '';
               FMembersOtherText := '';
               end;
           1 : begin
               FMembersOther := ' and SENIOR = ''Y''';
               FMembersOtherText := 'Senior Members';
               end;
           2 : begin
               FMembersOther := ' and FOUNDATION = ''Y''';
               FMembersOtherText := 'Foundation Members';
               end;
           3 : begin
               FMembersOther := ' and DONATED = ''Y''';
               FMembersOtherText := 'Donated Members';
               end;
           4 : begin
               FMembersOther := ' and REFUNDED = ''Y''';
               FMembersOtherText := 'Refunded Members';
               end;
           5 : begin
               FMembersOther := ' and DONATED = ''Y'' and REFUNDED = ''N''';
               FMembersOtherText := 'Not Refunded Members';
               end;
               
        end;
     end;
     if gbxGroups.Visible then begin
        if not cbxAllGroups.Checked then begin
           if FGroupsFields = '' then begin
              showmessage ('a "Group" must be selected');
              exit;
           end;
           FGroupText := Format('Group/s %s ',[FGroupsFields]);
        end else begin
           FFields := '';
           FGroupsFields := '';
           FGroupText := 'All Groups';
        end;
     end;
     if gbxLocations.Visible then begin
        if not cbxAllLocations.Checked then begin
           if FLocationsFields = '' then begin
              showmessage ('a "Location" must be selected');
              exit;
           end;
           FLocationText := Format('Locations/s %s ',[fLocationsFields]);
        end else begin
           FFields := '';
           FLocationsFields := '';
           FLocationText := 'All Locations';
        end;
     end;
     if gbxDiscounts.Visible then begin
        if cbxNoDiscounts.Checked then begin
           FFields          := '';
           FDiscountsFields := '';
           FDiscountText    := 'No Discounts';
        end else begin
           if not cbxAllDiscounts.Checked then begin
              if FDiscountsFields = '' then begin
                 showmessage ('a "Discount" must be selected');
                 exit;
              end;
              FDiscountText := Format('Discount/s %s ',[fDiscountsFields]);
           end else begin
              FFields          := '';
              FDiscountsFields := '';
              FDiscountText    := 'All Discounts';
           end;
        end;
     end;
     if gbxSections.Visible then begin
        if not cbxAllSections.Checked then begin
           if FSectionsFields = '' then begin
              showmessage ('a "Section" must be selected');
              exit;
           end;
           FSectionText := Format('Section/s %s ',[FSectionsFields]);
        end else begin
           FFields := '';
           FSectionsFields := '';
           FSectionText := 'All Sections';
        end;
     end;
     if gbxSubs.Visible then begin
        if not cbxAllSubs.Checked then begin
           if FSubsFields = '' then begin
              showmessage ('a "Subscription" must be selected');
              exit;
           end;
           FSubText := Format('Sub/s %s ',[FSubsFields]);
        end else begin
           FFields := '';
           FSubsFields := '';
           FSubText := 'All Subs';
        end;
     end;
     if gbxEvents.Visible then begin
        if not cbxAllEvents.Checked then begin
           if FEventsFields = '' then begin
              showmessage ('a "Event" must be selected');
              exit;
           end;
           FEventText := Format('Event/s %s ',[FEventsFields]);
        end else begin
           FEventsFields := '';
           FFields := '';
           FEventText := 'All Events'
        end;
     end;
     if gbxClubs.Visible then begin
        if not cbxAllClubs.Checked then begin
           if FClubsFields = '' then begin
              showmessage ('a "Club" must be selected');
              exit;
           end;
           FClubText := Format('Club/s %s ',[FClubsFields]);
        end else begin
           FFields := '';
           FClubsFields := '';
           FClubText := 'All Clubs'
        end;
     end;

     if gbxMailJoin.Visible then begin // only one selection - optional selection
        FMailJoinText := '';
        FMailJoin := '';
        if (dblcJoiningFee.Text <> '' ) or (Global.AllDigits(dblcJoiningFee.Text)) then begin
           FMailJoin := Format(' and JOINING_FEE = %s',[dblcJoiningFee.Text]);
           FMailJoinText := Format('Joining Fee = %s',[dblcJoiningFee.Text]);
        end;
        if (dblcMailCode.Text <> '' ) or (Global.AllDigits(dblcMailCode.Text)) then begin
           FMailJoin := Format(' and MAILING_CODE = %s',[dblcMailCode.Text]);
           FMailJoinText := Format('MailCode = %s',[dblcMailCode.Text]);
        end;
     end;
     if gbxMonths.Visible then begin
        if (cbxMonths.ItemIndex < 0) or (cbxMonths.ItemIndex > 12 ) then begin
           showmessage ('a "Month" must be selected');
           exit;
        end;
        mMonths := StrMonths[cbxMonths.ItemIndex];
     end;
     if gbxDates.Visible then begin
        if dbdtFrom.Date <> null then
           FDateFrom := dbdtFrom.Date
        else begin
           showmessage ('a "Date From" must be selected');
           exit;
        end;
        if dbdtTo.Date <> null then
           FDateTo := dbdtTo.Date
        else begin
           showmessage ('a "Date To" must be selected');
           exit;
        end;
     end;
     if gbxLetter.Visible then begin 
        if dblcLetters.Text = '' then begin
           showmessage ('You must select a letter to use');
           exit;
        end;
     end;
     if gbxMMDates.Visible then begin
        if dbdtMMDateFrom.Date <> null then
           FMMDateTimeFrom := dbdtMMDateFrom.DateTime
        else begin
           showmessage ('a "Date/Time From" must be selected');
           exit;
        end;
        if dbdtMMDateTo.Date <> null then
           FMMDateTimeTo := dbdtMMDateTo.DateTime
        else begin
           showmessage ('a "Date/Time To" must be selected');
           exit;
        end;
     end;
     if gbxLetter.Visible then begin 
        if dblcLetters.Text = '' then begin
           showmessage ('You must select a letter to use');
           exit;
        end;
     end;
     
     if rgpMembersEmail.Visible then begin
       case rgpMembersEmail.ItemIndex of
          0 : FMembersEmail := ''; // All
          1 : FMembersEmail := ' and m.RECEIVE_EMAIL = ''Y''';  
          2 : FMembersEmail := ' and ( m.RECEIVE_EMAIL = ''N'' or m.RECEIVE_EMAIL is Null)';  
       end;
     end; 
     if gbxAges.Visible then begin
        if not cbxDOB.Checked then begin
           if StrToInt(dbeYearFrom.Text) > StrToInt(dbeYearTo.Text) then begin
              sm('"Years From" must be less than "Years To"');
              exit;
           end;
        end;
     end;
     Result := True;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.dbdtToCloseUp(Sender: TObject);
// check the dates in dbxDates
begin
     if dbdtTo.Date < dbdtFrom.Date then begin
        showmessage( '"Date To" should be greater then "Date From" ,please re-enter');
        dbdtTo.SetFocus;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.dbdtFromKeyPress(Sender: TObject;  var Key: Char);
begin
     if key = #13 then begin
        SelectNext(ActiveControl as TWinControl,True,True);
        key := #0;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.FormShow(Sender: TObject);
//  populate the combobox with the list of printer names
begin
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.ResetLookups;
begin
     dblcJoiningFee.lookupField := '';
     dblcMailCode.lookupField := '';
     dblcLetters.lookupField := '';
     dblcQueries.lookupField := '';     
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.btnQueryClick(Sender: TObject);
var i : integer;
begin
     try
        TooBusy(ExtractRptMsg);
        for i := 0 to TreeView.Items.Count - 1 do begin
           // set it to create query for the export to excel etc
           if TreeView.Items[i].Text = 'Create Query' then begin
              TreeView.Selected := TreeView.Items[i];
              break; 
           end;
        end;
        frmMembersQUERY := TfrmMembersQUERY.Create(self);
        if (frmMembersQUERY.ShowModal = mrOK) then begin
           if dm.qryQuery.SQL.Text <> '' then begin
              with dm.qryReport do begin
                 Close;
                 SQL.Text := dm.qryQuery.SQL.Text; 
                 Open;
                 FbtnQuery := True;
              end;
           end;
        end;
     finally
        frmMembersQUERY.Free;
        NotBusy;
     end;      
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.cbxMonthsEnter(Sender: TObject);
begin
     cbxMonths.color := clWhite;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.cbxMonthsExit(Sender: TObject);
begin
     cbxMonths.color := clSilver;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.cbxAllSectionsClick(Sender: TObject);
begin
     if cbxAllSections.Checked then begin
         FExtFields := '';
         FSectionsExtFields := ''; FSectionsFields := '';
         btnSections.Enabled := False;
     end else
         btnSections.Enabled := True;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.cbxAllGroupsClick(Sender: TObject);
begin
     if cbxAllGroups.Checked then begin
         FExtFields := '';
         btnGroups.Enabled := False;
         FGroupsExtFields   := ''; FGroupsFields   := '';
     end else
         btnGroups.Enabled := True;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.cbxAllEventsClick(Sender: TObject);
begin
     if cbxAllEvents.Checked then begin
         FExtFields := '';
         FEventsExtFields   := ''; FEventsFields   := '';
         btnEvents.Enabled := False;
     end else
         btnEvents.Enabled := True;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.cbxAllSubsClick(Sender: TObject);
begin
     if cbxAllSubs.Checked then begin
         FExtFields := '';
         btnSubs.Enabled := False;
         FSubsExtFields     := ''; FSubsFields     := '';
     end else
         btnSubs.Enabled := True;

end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.cbxAllLocationsClick(Sender: TObject);
begin
     if cbxAllLocations.Checked then begin
         FExtFields := '';
         btnLocations.Enabled := False;
         FLocationsFields   := '';
     end else
         btnLocations.Enabled := True;
end;
{ ****************************************************************************}
procedure TfrmMembersREPORT.cbxAllDiscountsClick(Sender: TObject);
begin
     if cbxAllDiscounts.Checked then begin
         FExtFields := '';
         btnDiscounts.Enabled := False;
         FDiscountsFields   := '';
     end else
         btnDiscounts.Enabled := True;
end;
{ ****************************************************************************}
procedure TfrmMembersREPORT.cbxNoDiscountsClick(Sender: TObject);
begin
     if cbxNoDiscounts.Checked then begin
        btnDiscounts.Enabled    := False;
        cbxAllDiscounts.Enabled := False;
     end else begin
        btnDiscounts.Enabled    := true;
        cbxAllDiscounts.Enabled := true;
     end;
end;
{ ****************************************************************************}
procedure TfrmMembersREPORT.DropTempTables;
// cant have an active transaction, view, trigger or sql pointing at the table
const SQL1 = 'SELECT null AS MNUM FROM GROUPS';
begin
     if FTblName <> '' then begin
        dm.qryOpen(dm.qryReport ,SQL1);
        dm.IBCommitRetain;
        dm.qryOpen(dm.qryReport ,format('DROP TABLE %s',[FTblName]));
        dm.qryReport.Close;
     end else begin
        dm.qryReport.Close;    
     end;      
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.MemoEnter(Sender: TObject);
begin
     Memo.Color := clWhite;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.MemoExit(Sender: TObject);
begin
     Memo.Color := clSilver;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.gbxExportClick(Sender: TObject);
begin

end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.dirEditEnter(Sender: TObject);
begin
     dirEdit.Color := clWhite;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.dirEditExit(Sender: TObject);
begin
     dirEdit.Color := clSilver;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.CreateList( qrprint : Boolean ; Title : string );
// creates the report from TStrings
var
  aReport : TCustomQuickRep;
  SomeFields: TStringList;
  nIdx: integer;
begin
     SomeFields := TStringList.Create;
     // For this example, we will pull the field names from the table
     with dm.qryReport do begin
       for nIdx := 0 to FieldCount - 1 do begin
         SomeFields.Add(Fields[nIdx].FieldName);
         //if Fields[nIdx].DisplayWidth > 10 then // try to adjust to fit preview/print
         //   Fields[nIdx].DisplayWidth := Fields[nIdx].DisplayWidth div 2
         //else
         //   Fields[nIdx].DisplayWidth := Fields[nIdx].DisplayWidth - 3;
       end;
     end;
     // If you didn't create the report, you must set the report object to nil
     // before calling QRCreateList
     try
        //CreateListReport := TQuickRep.Create(self);
        aReport := nil;
       { Build the report }
        // If you change the displaywidth, it will be reflected in the created report
       // with dm.qryReport.Fields[1] do
       //   DisplayWidth := DisplayWidth div 2;
        // use the Query Report Name
        if ReportTitles[FOwnIndex] = 'Run Query' then
           Title := dm.qryReport1.FieldByName('NAME').AsString; 
        // create the report
        QRCreateList(aReport, nil, dm.qryReport, Title , SomeFields);

        aReport.PrinterSettings.PrinterIndex := Printer.PrinterIndex;

        // Make the column header's font use bold attribute
        aReport.Bands.ColumnHeaderBand.Font.Style := [fsBold];

        //aReport.Bands.ColumnHeaderBand.Font.Size := 8;
        aReport.Bands.DetailBand.Font.Size := 8;
        aReport.Bands.DetailBand.Height := 15;
        
        // Now adjust the spacing of the fields.  There isn't any reason to
        // do this, this is just to show how to access the controls on the report.
        for nIdx := 0 to aReport.Bands.ColumnHeaderBand.ControlCount -1 do
          if aReport.Bands.ColumnHeaderBand.Controls[nIdx] is TQRPrintable then
            with TQRPrintable(aReport.Bands.ColumnHeaderBand.Controls[nIdx]) do
              Left := Left - (2 * nIdx);
        for nIdx := 0 to aReport.Bands.DetailBand.ControlCount -1 do
          if aReport.Bands.DetailBand.Controls[nIdx] is TQRPrintable then
            with TQRPrintable(aReport.Bands.DetailBand.Controls[nIdx]) do
              Left := Left - (2 * nIdx);
        { You can change the report objects before calling the report }
        aReport.page.orientation := poLandscape;
        {preview or print the report}
        if qrPrint then
           aReport.print
        else
          aReport.preview;
     finally
        { all done, free the objects }
        aReport.Free;
        SomeFields.Free;
     end;
end;
{ **************************************************************************** }
function TfrmMembersREPORT.ExportEvents( mMonths : smallInt ; eEvent : shortstring ) : Boolean;
const SQL1 = 'SELECT M.MEMBER,M.FULL_NAME,M.KNOWN_AS,M.ADDRESS,M.ADDRESS1,ME.DATES,' +
             'ME.EVENT,E.DESCRIPTION from MEMBERS M,M_EVENTS ME,EVENTS E ' +
             'where M.MEMBER = ME.MEMBER AND ME.EVENT = E.EVENT ';
begin
     result := False;
     mMonths := mMonths + 1; // 0 based array
     with dm.qryReport do begin
         close;
         SQL.Text :=  SQL1;
         SQL.Add( Format( ' and EXTRACT (Month from ME.DATES) = %d',[mMonths]));
         if eEvent <> ''   then SQL.Add( Format( ' AND ME.EVENT in (%s)',[eEvent]));
         SQL.Add( ' ORDER by M.MEMBER');
         open;
     end;
     result := True;
end;
{ **************************************************************************** }
function TfrmMembersREPORT.ExportCodeKnownPosition : Boolean;
const SQL1 = 'SELECT MEMBER,KNOWN_AS,POSITIONS,FULL_NAME from MEMBERS WHERE IS_ACTIVE = ''Y'' and SUB_PAID > 0';
begin
     result := False;
     dm.qryOpenCheckEOF(dm.qryReport ,SQL1);
     result := True;
end;
{ **************************************************************************** }
function TfrmMembersREPORT.ExportSubs( mGroup : shortstring  ) : Boolean;
var  tblTemp : TIBTable;
     ctr,i : shortint;
     TotalFee,TotalPaid : Extended;

     EndSQL,MidSQL : shortstring;
const SQL1 = 'INSERT INTO %s ' +
             'SELECT m.MEMBER,m.MAILING_CODE,m.SURNAME,m.FIRST_NAME,m.FULL_NAME,m.KNOWN_AS, ' +
             'm.ADDRESS,m.DISTRICT,m.ADDRESS1,m.CITY,m.COUNTRY,m.EXPIRES,m.EMAIL,m.HOME_PHONE,' +
             'm.WORK_PHONE,m.CELL_PHONE,'''',0,0,'''',0,0,'''',0,0,'''',0,0,'''',0,0,' +
             'm.AUTO_PAYMENT,m.POINTS_AVAILABLE,0,0,0,m.SUB_PAID,0,m.spare_varchar_50,m.mailing_list_code ' +
             'from MEMBERS m %s where m.DECEASED <> ''Y'' %s ';
             //'from MEMBERS m %s where DECEASED = ''N'' %s'; 
      SQL2 = 'CREATE TABLE %s ( MEMBER Integer,MAILING_CODE integer,' +
             'SURNAME varchar(35),FIRST_NAME varchar(35),FULL_NAME varchar(55),KNOWN_AS  varchar(35),' +
             'ADDRESS varchar(40),DISTRICT varchar(30),ADDRESS1 varchar(40),CITY varchar(35),' +
             'COUNTRY varchar(35),EXPIRES  DATE,EMAIL varchar(70),HOME_PHONE varchar(24),WORK_PHONE varchar(24),CELL_PHONE varchar(24), ' +
             'SUBSCRIPTION1 varchar(60),FEE1 numeric(6,2),AMOUNT_PAID1 numeric(6,2),' +
             'SUBSCRIPTION2 varchar(60),FEE2 numeric(6,2),AMOUNT_PAID2 numeric(6,2),' +
             'SUBSCRIPTION3 varchar(60),FEE3 numeric(6,2),AMOUNT_PAID3 numeric(6,2),' +
             'SUBSCRIPTION4 varchar(60),FEE4 numeric(6,2),AMOUNT_PAID4 numeric(6,2),' +
             'SUBSCRIPTION5 varchar(60),FEE5 numeric(6,2),AMOUNT_PAID5 numeric(6,2),' +
             'AUTO_PAYMENT char(1),POINTS_AVAILABLE Decimal(8,2),TOTAL_FEE numeric(6,2),' +
             'TOTAL_PAID numeric(6,2),GST_CONTENT numeric(6,2),SUB_PAID integer,TOTAL_TO_PAY numeric(6,2),' + 
             'spare_varchar_50 varchar(50),mailing_list_code integer )';
      SQL3 = 'SELECT s.SUBSCRIPTION,s.DESCRIPTION,s.FEE,ms.AMOUNT_PAID from SUBSCRIPTIONS s, M_SUBS ms ' +
             'where ms.SUBSCRIPTION = s.SUBSCRIPTION and ms.MEMBER = %d';
begin
     result := False;
     FtblName := 'TEMP' + FormatDateTime('sszzz',now); //unique table by user
     with dm.qryGeneral do begin
         try
            dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[FtblName]));
            dm.IBCommitRetain;
            EndSQL := '';
            case rgpMembers.ItemIndex of
               0 : EndSQL := '';               // All Members
               1,2,3,4,5 : EndSQL := FMembers; // Part,Non,Active NonActive etc
            end;
            
            case rgpExportOptions.ItemIndex of
               0 : EndSQL := EndSQL + ''; // All
               1 : EndSQL := EndSQL + ' and M.AUTO_PAYMENT = ''Y'' '; // Direct Debit
               2 : EndSQL := EndSQL + ' and M.AUTO_PAYMENT = ''N'' '; // Normal
            end;
            case rgpExpires.ItemIndex of   //0 : Expires   1 : All
              0 : EndSQL := EndSQL + format(' and M.EXPIRES between ''%s'' and ''%s'' ',[GetShortDAte(dbdtFrom.Date),GetShortDAte(dbdtTo.Date)]); // Expires
            end;
            if mGroup = '' then begin
               MidSQL := '';
            end else begin
               MidSQL := ',M_GROUPS mg ';
               EndSQL := EndSQL + format(' and m.MEMBER = mg.MEMBER and mg.GROUPS in (%s) ',[mGroup]);
            end;
            dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[FtblName,MidSQL,EndSQL]));

            case rgpExportParentCode.ItemIndex of
               0 : ;
               1 : EndSQL := EndSQL + ' and M.MAILING_LIST_CODE = m.MEMBER '; // Parent Codes Only
            end;
            
            //if RecordCount = 0 then begin
            //   sm('There are no Members that Expire between the selected dates');
            //   exit;
            //end;
            tblTemp := TIBTable.create(nil);
            tblTemp.Database := dm.IBDatabase;
            tblTemp.Transaction := dm.IBTransaction;
            tblTemp.Tablename := FtblName;
            tblTemp.Open;
            tblTemp.First;
            while not tblTemp.EOF do begin
                // delete out members who are fully paid and passed the expiry date
                {if global.FixedExpirySystem then begin
                   if (tblTemp.FieldByName('EXPIRES').AsDateTime > Global.MemberExpiry) and
                      (tblTemp.FieldByName('SUB_PAID').AsInteger = 2) then begin
                      tblTemp.Delete;
                      tblTemp.Next;
                      Continue;
                   end;
                end;}
                //smi(tblTemp.FieldByName('MEMBER').AsInteger);
                if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL3,[ tblTemp.FieldByName('MEMBER').AsInteger])) then begin
                    ctr := 1;
                    TotalFee := 0.00; TotalPaid := 0.00;
                    //i := tblTemp.FieldByName('MEMBER').AsInteger;
                    while not EOF do begin
                       tblTemp.Edit;
                       tblTemp.FieldByName('SUBSCRIPTION' + intToStr(ctr)).AsString := FieldByName('DESCRIPTION').AsString;
                       tblTemp.FieldByName('FEE'  + intToStr(ctr)).AsFloat          := FieldByName('FEE').AsFloat;
                       TotalFee := TotalFee + FieldByName('FEE').AsFloat;
                       // work out if passed expires - amount_paid is zero
                       // else amount_paid
                       if global.FixedExpirySystem then begin
                          if tblTemp.FieldByName('EXPIRES').AsDateTime <= Date then begin
                             tblTemp.FieldByName('AMOUNT_PAID'  + intToStr(ctr)).AsFloat := 0.00;
                          end else begin
                             tblTemp.FieldByName('AMOUNT_PAID'  + intToStr(ctr)).AsFloat := FieldByName('AMOUNT_PAID').AsFloat;
                             TotalPaid := TotalPaid + FieldByName('AMOUNT_PAID').AsFloat;
                          end;
                       // revolving
                       end else begin
                          tblTemp.FieldByName('AMOUNT_PAID'  + intToStr(ctr)).AsFloat := 0.00;
                       end;
                       inc(ctr);
                       tblTemp.Post;
                       Next;
                    end;
                // no sub
                end else begin
                   tblTemp.Delete;
                //   tblTemp.Next;
                   Continue;
                end;
                // no fee to Pay - so remove the Member
                {if TotalFee - TotalPaid = 0.00 then begin
                   tblTemp.Delete;
                   tblTemp.Next;
                   continue;
                end; }
                tblTemp.Edit;
                tblTemp.FieldByName('TOTAL_FEE').AsFloat    := TotalFee;
                tblTemp.FieldByName('TOTAL_PAID').AsFloat   := TotalPaid;
                tblTemp.FieldByName('TOTAL_TO_PAY').AsFloat := TotalFee - TotalPaid;
                tblTemp.FieldByName('GST_CONTENT').AsFloat  := (TotalFee - TotalPaid) - ((TotalFee - TotalPaid) / 1.125);
                tblTemp.Post;
                tblTemp.Next;
            end;
            with dm.qryReport do begin
               close;
               // same order as Invoices
               SQL.Text := format('Select * from %s order by spare_varchar_50,mailing_list_code,member',[FTblName]);
               open;
            end;
            result := True;
         finally
             tblTemp.Free;
         end;
      end;   
end;
{ **************************************************************************** }
{function TfrmMembersREPORT.ExportSubs( mGroup : shortstring  ) : Boolean;
var  tblTemp : TIBTable;
     ctr : shortint;
     EndSQL,MidSQL : shortstring;
const SQL1 = 'INSERT INTO %s ' +
             'SELECT m.MEMBER,m.MAILING_CODE,m.SURNAME,m.FIRST_NAME,m.FULL_NAME,m.KNOWN_AS, ' +
             'm.ADDRESS,m.DISTRICT,m.ADDRESS1,m.CITY,m.COUNTRY,m.EXPIRES,m.EMAIL,m.HOME_PHONE,m.WORK_PHONE,m.CELL_PHONE,'''',0,'''',0,'''',0,'''',0,'''',0,m.AUTO_PAYMENT,m.POINTS_AVAILABLE ' +
             'from MEMBERS m %s where DECEASED <> ''Y'' %s';
      SQL2 = 'CREATE TABLE %s ( MEMBER Integer,MAILING_CODE integer,' +
             'SURNAME varchar(25),FIRST_NAME varchar(25),FULL_NAME varchar(45),KNOWN_AS  varchar(25),' +
             'ADDRESS varchar(30),DISTRICT varchar(20),ADDRESS1 varchar(30),CITY varchar(25),' +
             'COUNTRY varchar(25),EXPIRES  DATE,EMAIL varchar(60),HOME_PHONE varchar(14),WORK_PHONE varchar(14),CELL_PHONE varchar(14), ' +
             'SUBSCRIPTION1 varchar(60),FEE1 numeric(6,2), ' +
             'SUBSCRIPTION2 varchar(60),FEE2 numeric(6,2),SUBSCRIPTION3 varchar(60),FEE3 numeric(6,2),' +
             'SUBSCRIPTION4 varchar(60),FEE4 numeric(6,2),SUBSCRIPTION5 varchar(60),FEE5 numeric(6,2),AUTO_PAYMENT char(1),POINTS_AVAILABLE Decimal(8,2) )';
      SQL3 = 'SELECT s.SUBSCRIPTION,s.DESCRIPTION,s.FEE from SUBSCRIPTIONS s, M_SUBS ms ' +
             'where ms.SUBSCRIPTION = s.SUBSCRIPTION and ms.MEMBER = %d';
begin
     result := False;
     FtblName := 'TEMP' + FormatDateTime('sszzz',now); //unique table by user
     with dm.qryGeneral do begin
         try
            dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[FtblName]));
            dm.IBCommitRetain;
            EndSQL := '';
            case rgpExportOptions.ItemIndex of
               0 : EndSQL := ''; // All
               1 : EndSQL := ' and AUTO_PAYMENT = ''Y'''; // Direct Debit
               2 : EndSQL := ' and AUTO_PAYMENT = ''N'''; // Normal
            end;
            case rgpExpires.ItemIndex of   //0 : Expires   1 : All
              0 : EndSQL := EndSQL + format('and EXPIRES between ''%s'' and ''%s''',[GetShortDAte(dbdtFrom.Date),GetShortDAte(dbdtTo.Date)]); // Expires
            end;
            if mGroup = '' then begin
               MidSQL := '';
            end else begin
               MidSQL := ',M_GROUPS mg';
               EndSQL := EndSQL + format('and m.MEMBER = mg.MEMBER and mg.GROUPS in (%s)',[mGroup]);
            end;
            dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[FtblName,MidSQL,EndSQL]));
            //if RecordCount = 0 then begin
            //   sm('There are no Members that Expire between the selected dates');
            //   exit;
            //end;
            tblTemp := TIBTable.create(nil);
            tblTemp.Database := dm.IBDatabase;
            tblTemp.Transaction := dm.IBTransaction;
            tblTemp.Tablename := FtblName;
            tblTemp.Open;
            tblTemp.First;
            while not tblTemp.EOF do begin
                if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL3,[ tblTemp.FieldByName('MEMBER').AsInteger])) then begin
                    ctr := 1;
                    while not EOF do begin
                       tblTemp.Edit;
                       tblTemp.FieldByName('SUBSCRIPTION' + intToStr(ctr)).AsString := FieldByName('DESCRIPTION').AsString;
                       tblTemp.FieldByName('FEE'  + intToStr(ctr)).AsFloat          := FieldByName('FEE').AsFloat;
                       inc(ctr);
                       tblTemp.Post;
                       Next;        
                    end;
                end;
                tblTemp.Next;
            end;
            with dm.qryReport do begin
               close;
               SQL.Text := format('Select * from %s',[FTblName]);
               open;
            end;
            result := True;
         finally
             tblTemp.Free;
         end;
      end;
end;}

{
function TfrmMembersREPORT.ExportSubs( mGroup : shortstring  ) : Boolean;
var  tblTemp : TIBTable;
     ctr : shortint;
const SQL1 = 'INSERT INTO %s ' +
             'SELECT MEMBER,MAILING_CODE,SURNAME,FIRST_NAME,FULL_NAME,KNOWN_AS, ' +
             'ADDRESS,DISTRICT,ADDRESS1,CITY,COUNTRY,EXPIRES,'''',0,'''',0,'''',0 ' +
             'from MEMBERS WHERE EXPIRES between ''%s'' and ''%s''';
      SQL2 = 'CREATE TABLE %s ( MEMBER Integer,MAILING_CODE integer,' +
             'SURNAME varchar(25),FIRST_NAME varchar(25),FULL_NAME varchar(45),KNOWN_AS  varchar(25),' +
             'ADDRESS varchar(30),DISTRICT varchar(20),ADDRESS1 varchar(30),CITY varchar(25),' +
             'COUNTRY varchar(25),EXPIRES  DATE,SUBSCRIPTION1 varchar(60),FEE1 numeric(6,2), ' +
             'SUBSCRIPTION2 varchar(60),FEE2 numeric(6,2),SUBSCRIPTION3 varchar(60),FEE3 numeric(6,2) )';
      SQL3 = 'SELECT s.SUBSCRIPTION,s.DESCRIPTION,s.FEE from SUBSCRIPTIONS s, M_SUBS ms ' +
             'where ms.SUBSCRIPTION = s.SUBSCRIPTION and ms.MEMBER = %d';       
begin
     result := False;
     FtblName := 'TEMP' + FormatDateTime('sszzz',now); //unique table by user
     with dm.qryGeneral do begin
         try
            dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[FtblName]));
            dm.IBCommitRetain;
            dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[FtblName,GetShortDAte(dbdtFrom.Date),GetShortDAte(dbdtTo.Date)]));
            //dm.IBCommitRetain;
            //if RecordCount = 0 then begin
            //   sm('There are no Members that Expire between the selected dates');
            //   exit;
            //end;   
            tblTemp := TIBTable.create(nil); 
            tblTemp.Database := dm.IBDatabase;
            tblTemp.Transaction := dm.IBTransaction;
            tblTemp.Tablename := FtblName;
            tblTemp.Open;
            tblTemp.First;
            while not tblTemp.EOF do begin
                if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL3,[ tblTemp.FieldByName('MEMBER').AsInteger])) then begin
                    ctr := 1;
                    while not EOF do begin
                       tblTemp.Edit;
                       tblTemp.FieldByName('SUBSCRIPTION' + intToStr(ctr)).AsString := FieldByName('DESCRIPTION').AsString;
                       tblTemp.FieldByName('FEE'  + intToStr(ctr)).AsFloat           := FieldByName('FEE').AsFloat;
                       inc(ctr);
                       tblTemp.Post;
                       Next;        
                    end;
                end;
                tblTemp.Next;
            end;
            with dm.qryReport do begin
               close;
               SQL.Text := format('Select * from %s',[FTblName]);
               open;
            end;   
            result := True;
         finally
             tblTemp.Free;
         end;
      end;   

end;
{ **************************************************************************** }
function TfrmMembersREPORT.ExportSelective(mMembers : string ; mGroup : shortstring) : Boolean;
var s : string[14];
begin
     result := False;
     with dm.qryReport do begin
         close;
         s := '';
         if mGroup <> '' then
            s := ', M_GROUPS mg';
         SQL.Text :=  format('SELECT m.Member,%s from MEMBERS M %s where m.DECEASED <> ''Y''',[FExportFields,s]);
         if mMembers <> '' then
            SQL.Add( mMembers);
         if mGroup <> '' then
            SQL.Add( Format( ' and M.MEMBER = mg.MEMBER and mg.GROUPS in (%s)',[mGroup]));
         SQL.Add( ' ORDER by M.MEMBER');
         AddToClipBoard(SQL.Text);
         open;
     end;
     result := True;
end;
{ **************************************************************************** }
function TfrmMembersREPORT.ExportCardFile(mMembers,mNewMembers,mGroup : string ) : Boolean;
var WhereUsed : Boolean;
begin
     result := False;
     WhereUsed := False;
     with dm.qryReport do begin
         close;
         SQL.Text :=  format('SELECT Distinct m.Member,%s from MEMBERS M,M_GROUPS mg',[FExportFields]);
         if mNewMembers <> '' then begin
            SQL.Add( Format( '%s',[mNewMembers]));
            WhereUsed := True;
         end;
         if mMembers <> '' then begin
            if WhereUsed then
               SQL.Add( Format( '%s',[mMembers]))
            else begin
               system.delete(mMembers,1,4); // delete ' and'
               SQL.Add( Format( ' where %s',[mMembers]));
               WhereUsed := True;
            end;
         end;
         if mGroup  <> ''  then begin
            if WhereUsed then
               SQL.Add( Format( ' and M.MEMBER = mg.MEMBER and mg.GROUPS in (%s)',[mGroup]))
            else
               SQL.Add( Format( ' where M.MEMBER = mg.MEMBER and mg.GROUPS in (%s)',[mGroup]));
         end;
         SQL.Add( ' ORDER by M.MEMBER');
         open;
     end;
     result := True;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.PreCreateAsciiFile;
var s : string;
const MFileName = 'Member.dat';
begin
      case dbcbExport.ItemIndex of
         0 : begin
             s := dirEdit.Text; // select
             if s[length(s)] <> '\' then
                s := s + '\';
             end;
         1,2 : s := dbcbExport.Text;
         3   : s := dbcbExport.Text + '\'; // c:\my documents
      end;
      if not DirectoryExists(s) then begin
         sm(format('Directory %s does not exist',[s]));
         exit;
      end;
      SetupSaveDialog('*.dat',Reportfltr,'Save to','member',s);
      
      if dm.SaveDialog.Execute then begin
         if FOwnIndex = 37 then begin // export code known as position
               Global.CreateAsciiFile( dm.SaveDialog.FileName,False,dm.qryReport,nil,True,True);
            exit;
         end;
         if gbxMemo.Visible then // create with heading
               Global.CreateAsciiFile( dm.SaveDialog.FileName,False,dm.qryReport,Memo.Lines,True,False)
         else
               Global.CreateAsciiFile( dm.SaveDialog.FileName,False,dm.qryReport,nil,True,False);
      end;         
      // Back to Default
      SetupSaveDialog('*.dat',Reportfltr,'Save to','member','');
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.dbcbExportChange(Sender: TObject);
begin
     case dbcbExport.ItemIndex of
         0 : dirEdit.Enabled := true;      // select
         1,2,3 : dirEdit.Enabled := False; // c: a: c:mydoc 
      end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.btnFieldsClick(Sender: TObject);
var i,j : smallint;
// List1 = dm.qryReport2.displaylabels
// Build up FFields from List 2 - gets dm.qryReport2.FieldName
begin
     with FieldsDialog do begin
        List1.Clear;
        List2.Clear;
        if (ListOne.Count > 0) or (ListTwo.Count > 0) then
           List1.Assign(ListOne)
        else
           for i := 0 to dm.qryReport2.FieldCount - 1 do
               List1.add(dm.qryReport2.Fields[i].DisplayLabel);
        if ListTwo.Count > 0 then  List2.Assign(ListTwo);
        Sorted := true;
        Title := 'Select Member Fields for Export';
        Label1Caption := 'Member Fields';
        Label2Caption := 'Export in order of list';
        if Execute then begin
           ListOne.Assign(List1);  // save
           ListTwo.Assign(List2);
           FExportFields := '';
           for j := 0 to List2.Count -1 do begin
               for i := 0 to dm.qryReport2.FieldCount - 1 do begin
                  if List2[j] = dm.qryReport2.Fields[i].DisplayLabel then begin
                     FExportFields := FExportFields + 'M.' + dm.qryReport2.Fields[i].FieldName + ',';
                     break;
                  end;
               end;
           end;
           delete(FExportFields,length(FExportFields),1); // remove last comma
        end;
        Sorted := False;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.rgpNewMemberClick(Sender: TObject);
begin
     case rgpNewMember.ItemIndex of
        0,1 : gbxDates.Visible := False;
        2   : begin
              gbxDates.Visible := True;
              gbxDates.Top := rgpNewMember.Top + 49;
              dbdtFrom.Date := now - 31;
              dbdtTo.Date   := now;
              end;
      end;
     //scbOptions.VertScrollBar.Visible := False;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.ClearMembersFlag;
// makes SAVED to Y from N - New Members
const SQL1 = 'UPDATE MEMBERS SET SAVED = ''Y'' WHERE SAVED = ''N''';
      msg1 = 'Clear the New Members flag';
begin
     try
        TooBusy( msg1 + PleaseWaitMsg);
        try
           dm.IBCommitRetain;
           dm.sqlExecQuery(dm.qryGeneral,SQL1);
           dm.IBCommitRetain;
        except
           dm.IBRollBackRetain;
           sm(UsersMakingChangesMsg + #13#10 + '"' + msg1 + '"'+ PleaseTryAgainMsg);
        end;
     finally
        NotBusy;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.ClearInterests;
const SQL1 = 'Delete from M_INTERESTS';
      ClearInterestsMsg = 'Clear All Members from Section Interests';
begin
     try
        dm.IBCommitRetain;
        dm.sqlExecQuery(dm.qryGeneral,SQL1);
        dm.IBCommitRetain;
     except
        dm.IBRollBackRetain;
        sm(ClearInterestsMsg + ' has failed');
     end;
end;
{ **************************************************************************** }

procedure TfrmMembersREPORT.rgpLinksClick(Sender: TObject);
begin
     case rgpLinks.ItemIndex of
        0 :  begin
              gbxGroups.Visible := True;
              gbxGroups.Top     := rgpLinks.Top + 49;
              gbxSections.Visible := False;
              gbxSubs.Visible     := False;
            end;
        1 :  begin
              gbxSections.Visible := True;
              gbxSections.Top     := rgpLinks.Top + 49;
              gbxSubs.Visible     := False;
              gbxGroups.Visible   := False;
            end;
        2 : begin
              gbxSubs.Visible := True;
              gbxSubs.Top     := rgpLinks.Top + 49;
              gbxGroups.Visible   := False;
              gbxSections.Visible := False;
            end;
      end;
     //scbOptions.VertScrollBar.Visible := False;
      
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.BitBtn1Click(Sender: TObject);
begin
end;
{ **************************************************************************** }
function TfrmMembersREPORT.GetDatafromLetter( LetterName : string ) : Boolean;
var MemberList : TStringList;
    i : integer;
    s : string;
label Fromhere;      
// Makes up 2 lists and a query string from the Letter 
// ListOne  like FULL_NAME        useage - FieldByName(ListOne[i]).AsString;                 
// Listtwo  the field looks Like  \{Full Name\}  
// FExportFields = M.FULL_NAME,M.FIRST_NAME useage - for the query
begin
     result := False;
     ListOne.Clear;
     ListTwo.Clear;
     with dm.ibdsLetters do begin
         close;
         SelectSQL.Text := format('SELECT * from LETTERS where NAME = ''%s''',[LetterName]);
         open;
         if not EOF then begin
            try
               FExportFields := '';
               s := '';
               // make all fields look like they do in the letter
               MemberList := TStringList.Create;
               // loads in the display name - ie the one in the letter
               for i := 0 to dm.qryReport2.FieldCount - 1 do begin
                   MemberList.add('\{' + dm.qryReport2.Fields[i].DisplayName + '\}');
               end;    
               // sometimes Listtwo  the field looks Like  {Full Name}  
               //if MemberList.Count = 0 then begin
               //   for i := 0 to dm.qryReport2.FieldCount - 1 do 
               //    MemberList.add('{' + dm.qryReport2.Fields[i].DisplayName + '}');
               //end;    
               S := dm.ibdsLettersLETTER.AsString; // gets the whole letter as one string
               AddFields( MemberList,s );
               // sometimes Listtwo  the field looks Like  {Full Name}  
               if FExportFields = '' then begin
                  MemberList.Clear;
                  for i := 0 to dm.qryReport2.FieldCount - 1 do 
                     MemberList.add('{' + dm.qryReport2.Fields[i].DisplayName + '}');
                  AddFields( MemberList,s );
               end;
               result := True;
            finally
               FExportFields := DeleteLastComma(FExportFields);
               // add at least one - see query in rDueLetter 
               if FExportFields = '' then
                  FExportFields := 'M.MEMBER';
               MemberList.Free;
               dm.ibdsLetters.Close;
            end;
         end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.AddFields( MemberLst : TStrings; s : String );
var i : smallint;
begin

     for i := 0 to MemberLst.Count -1 do begin
         if Pos(MemberLst[i],s) > 0 then begin
            FExportFields := FExportFields + 'M.' + dm.qryReport2.Fields[i].FieldName + ',';
            ListOne.Add(dm.qryReport2.Fields[i].FieldName);
            ListTwo.Add(MemberLst[i]); //    \{Full Name\} or {Full Name}
         end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.btnEventsClick(Sender: TObject);
begin
      if (Sender as TSpeedButton).Name = btnEvents.Name then
         GetFieldList( 'Select Events', 'Event',dm.tblEvents );
      if (Sender as TSpeedButton).Name = btnGroups.Name then
         GetFieldList( 'Select Groups', 'Groups',dm.tblGroups );
      if (Sender as TSpeedButton).Name = btnSubs.Name then
         GetFieldList( 'Select Subs', 'Subscription',dm.tblSubs );
      if (Sender as TSpeedButton).Name = btnSections.Name then
         GetFieldList( 'Select Sections', 'Section',dm.tblSections );
      if (Sender as TSpeedButton).Name = btnLocations.Name then begin
         if dmMM.MMConnected then  // locations are in MenuMate
            GetFieldList( 'Select Locations', 'Name',dmMM.tblMMLocations);
      end;      
      if (Sender as TSpeedButton).Name = btnDiscounts.Name then begin
         if not MenuMateDiscountsInstalled then begin
            showmessage('MenuMate Discounts are not installed');
            exit;
         end else begin
            if dmMM.MMConnected then  // locations are in MenuMate
               GetFieldList( 'Select Discounts', 'Discount_Key',dmMM.tblMMDiscounts);
         end;      
      end;    
      if (Sender as TSpeedButton).Name = btnClubs.Name then begin
         GetFieldList( 'Select Clubs', 'CLUB_ID',dm.tblClubs );
     end;   
end;
{ *************************************************************************** }
procedure TfrmMembersREPORT.GetFieldList( aTitle, aCaption : shortstring ; aTable : TIBTable );
var i : integer;
var DescName : ShortString; 
begin
     with FieldsDialog do begin
        List1.Clear;
        List2.Clear;
        Title := aTitle;
        Label1Caption := aCaption;
        Label2Caption := 'Select one or many';
        DescName := 'Description';
        // is Name in Locations table from MenuMate
        if aTable.Name = dmMM.tblMMLocations.Name then begin
           DescName := 'Name';
           dmMM.tblMMLocations.Active := True;
        end;   
        // is Name in Locations table from MenuMate
        if aTable.Name = dmMM.tblMMDiscounts.Name then begin
           List1.add('No Discount');
           dmMM.tblMMDiscounts.Active := True;
        end;   
        with aTable do begin
           Close;
           Open;
           first;
           while not eof do begin
              List1.add(FieldByName(DescName).AsString);
              next;
           end;
        end;
        if Execute then begin
           FFields := '';
           with aTable do begin
              first;
              FExtFields := '';
              while not eof do begin
                 for i := 0 to List2.Count -1 do begin
                     if List2[i] = FieldByName(DescName).AsString then begin
                        FExtFields := FExtFields + FieldByName(aCaption).AsString + ',';
                        break;
                     end;
                 end;
                 next;
              end;
           end;
           // commit MenuMate
           dmMM.MMCommit;
           // is Name in Locations table from MenuMate
           if (aTable.Name = dmMM.tblMMLocations.Name) or (aTable.Name = dmMM.tblMMDiscounts.Name) then begin
             for i := 0 to List2.Count -1 do
                FFields := FFields + '''' + List2[i] +  ''',';
           end else begin
             for i := 0 to List2.Count -1 do
                FFields := FFields + List2[i] + ',';
           end;   
           FFields    := DeleteLastComma(FFields);
           FExtFields := DeleteLastComma(FExtFields);
           {
             most reports still use FFields and FExtFields where there
             is only one option for the report ie subs secions groups or events
             see Members Information report
           }
           if aTable.Name = dm.tblSections.Name then begin
              FSectionsExtFields := FExtFields;
              FSectionsFields    := FFields;
           end;
           if aTable.Name = dmMM.tblMMLocations.Name then begin
              FLocationsFields    := FFields;
           end;
           if aTable.Name = dmMM.tblMMDiscounts.Name then begin
              FDiscountsFields    := FFields;
              FDiscountsExtFields := FExtFields;
           end;                   
           
           if aTable.Name = dm.tblSubs.Name then begin
              FSubsExtFields := FExtFields;
              FSubsFields    := FFields;
           end;
           if aTable.Name = dm.tblGroups.Name then begin
              FGroupsExtFields := FExtFields;
              FGroupsFields    := FFields;
           end;
           if aTable.Name = dm.tblEvents.Name then begin
              FEventsExtFields := FExtFields;
              FEventsFields    := FFields;
           end;
           if aTable.Name = dm.tblClubs.Name then begin
              FClubsExtFields := FExtFields;
              FClubsFields    := FFields;
           end;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.menuLettersClick(Sender: TObject);
begin
    {with dm.IBDataSet1 do begin
       open;
       while not EOF do begin
          edit;
          FieldByName('EMAIL').AsString := 'email' + FieldByName('MEMBER').AsString + '@xtra.co.nz';
          post;
          next;
       end;
       close;
       dm.IBCommitRetain;
    end;
    exit; }
     if frmMembersLetter = nil then
        if sfPreCheckLogOn(TfrmMembersLetter)
           then frmMembersLetter := TfrmMembersLetter.Create(nil)
        else exit;
     ResetLookups;
     frmMembersLetter.SetAsLetter := True;
     SMStdForm.ShowStdForm(frmMembersLetter);
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.ExportToWord;
var WordFileName : ShortString;
begin
     if assigned(MergeDoc) then 
        MergeDoc.Free;
     // look for an existing doc
     if FReportChoice = 5 then begin 
        SetupOpenDialog('*.Doc','Word Docs|*.Doc','Word Merge Documents');
        WordFileName := '';
        if dm.OpenDialog.Execute then 
           WordFileName := dm.OpenDialog.FileName
        else 
           exit;   
     end;
     try
        TooBusy ('Exporting to Word ...');
        with dm.OpWord do begin
          if not Connected  then begin
            Connected := True;
            Visible   := True;
            case FReportChoice of 
               5 : MergeDoc  := OpenDocument(WordFileName);
               6 : MergeDoc  := NewDocument;
            end;   
          end;
          // allow user to open and save the word doc 
          //with dm.QryReport do begin
          //   Close;
          //   SQL.Text := 'Select * from SUBSCRIPTIONS';
          //   Open;
          //end;
          if FOwnIndex in [57,59] then begin // uses cdsTemp
             dm.OpDataSetModel.DataSet := dm.cdsTemp;
          end else
             dm.OpDataSetModel.DataSet := dm.qryReport;
          MergeDoc.MailMerge.OfficeModel := dm.OpDatasetModel;
          MergeDoc.PopulateMailMerge;
          // only if the document is existing
          if FReportChoice = 5 then begin
             MergeDoc.MailMerge.AsMailMerge.Destination := wdSendToNewDocument;
             MergeDoc.ExecuteMailMerge;
          end;
          //Visible := True;                       // show it
        end;
     finally
        dm.OpDataSetModel.DataSet := nil;
        NotBusy;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.ExportToExcel;
var
    //Rng : TOpExcelRange;
    oWorkBook: TOpExcelWorkbook;
    oSheet: TOpExcelWorksheet;
    oRange: TOpExcelRange;
begin

{        Rng := dm.OpExcel.Workbooks[0].Worksheets[FSheet].Ranges.Add;
        inc(Fsheet);
        Rng.Name := 'CustomerRange';
        Rng.Address := 'A1';
        Rng.ClearOnMove := True;
        Rng.OfficeModel := dm.OpDataSetModel;
        dm.OpDataSetModel.Dataset := dm.qryReport;
        RangeByName['ExcelRange'].Populate;
        Workbooks[0].Worksheets[0].Activate;
       // RangeByName['ExcelRange'].AsRange.Columns.AutoFit;
        Rng.AsRange.Columns.AutoFit;
     end; }
     try
        if dm.qryReport.RecordCount = 0 then begin
           showmessage('There are no Records To Export to Excell');
           exit;
        end;
        TooBusy ('Exporting to Excel ...');
        with dm.OpExcel do begin
          if not Connected  then begin
            Connected := True;
            Visible := True;
          end;
          oWorkBook := Workbooks.Add;            // create a workbook
          //oSheet    := oWorkBook.Worksheets.Add; // add a worksheet
          oSheet    := oWorkBook.Worksheets[0]; // add a worksheet
          oRange    := oSheet.Ranges.Add;        // create range for output
          oRange.Address := 'A1';                // locate range

          oRange.OfficeModel := dm.OpDatasetModel; // assign the model to the range
          if FOwnIndex in [57,59] then begin
             dm.OpDataSetModel.DataSet := dm.cdsTemp;
          end else
             dm.OpDataSetModel.DataSet := dm.qryReport;
          oRange.Populate;                       // fill the range from the model

          //oRange.Address := 'Column A';???
          //oRange.AsRange.NumberFormat := 'Dd Mmm Yyyy'; 01 Mar 2005
          

          Visible := True;                       // show it
          Interactive := True                    // Let the user access it
        end;
     finally
        dm.OpDataSetModel.DataSet := nil;
        NotBusy;           
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.Email;
var CurrDirAndFile : string;
// Emails the Whole Report to a person in your address book 
begin
     // dont create the file for exporting of Cards
     if FOwnIndex <> 38 then begin 
        GetDir(0,CurrDirAndFile);  // get for email - needs path ext
        CreateAsciiFile( CurrDirAndFile + ExportFileName,False,dm.qryReport,nil,False,False);
     end;   
     if sfPreCheckLogOn(TfrmMembersEmail) then begin
        try
           frmMembersEmail := TfrmMembersEmail.Create(nil);
           frmMembersEmail.ESubject  := ReportTitles[FOwnIndex];
           frmMembersEmail.EBody     := ReportTitles[FOwnIndex] + ' from ' + global.CompName;
           frmMembersEmail.EfrmTypes := asWholeReport;
           frmMembersEmail.ShowModal;
        finally
           frmMembersEmail.Release;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.SetTreeView;
begin
     FRunOnce := False;
     if TreeView.Enabled then begin
        FTreeViewText := TreeView.Selected.Text;
        FTreeViewHint := TreeView.Hint;
        TreeView.Hint := 'Right click again to release the hold';
        TreeView.Selected.Text := FTreeViewText + ' Report Held';
        TreeView.Enabled := False;
        FHoldReport := True;
     end else begin
        TreeView.Enabled := True;
        TreeView.Selected.Text := FTreeViewText;
        TreeView.Hint := FTreeViewHint;
        FHoldReport := False;
        // DropTempTables; // incase one used
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.ReportHoldFree1Click(Sender: TObject);
begin
     SetTreeView;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.Button1Click(Sender: TObject);
var    strCommand: string;
begin
   {  with dm.qryReport do begin
           Close;
           //strCommand := SQL.Text;
           if SQLEdit.EditSQL(strCommand, dm.IBDatabase.GetTableNames, dm.IBDatabase.GetFieldNames) then
              SQL.Text := strCommand;
           if SQL.Text <> '' then begin
              screen.cursor := crHourGlass;
              Open;
              screen.cursor := crDefault;
           end;   
      end;}
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.FormKeyUp(Sender: TObject; var Key: Word;  Shift: TShiftState);
begin
     inherited;
     case key of   
         VK_ESCAPE : menuExitClick(Sender);
     end;   
    if ( ssAlt in shift ) then begin
        case key of 
          88,120  : menuExitClick(Sender);                       // X  Close 
          69,101  : menuExportAscii.OnClick(menuExportAscii);   // E  Export
          67,99   : menuExportExcel.OnClick(menuExportExcel);  // C  Excel
          77,109  : menuEmailReport.OnClick(menuEmailReport); // M  Email
          76,108  : menuLettersClick(Sender);                // L  Letters
          86,118  : menuPreview.OnClick(menuPreview);       // V  Preview
          80,112  : menuPrinterSetupClick(Sender);         // P  Printer Setup
        end;
     end; 
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.FormResize(Sender: TObject);
begin
     ResizeToolBar( Self ,Self.WindowState,OneButtonWidth,FpnlWidth,pnlSpace);
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.dbdtFromChange(Sender: TObject);
begin
    if TwwDBDateTimePicker(Sender).Modified then
       TwwDBDateTimePicker(Sender).DisplayFormat := 'dd/mm/yyyy';
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.dbdtFromExit(Sender: TObject);
begin
     TwwDBDateTimePicker(Sender).DisplayFormat := 'dd mmm yyyy';
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.PrintFromTable;
begin
     with dm.tblCardMember do begin
        First;
        while not EOF do begin
           // one card if null
           if FieldByName('NUMBER_CARDS').IsNull then begin
              Global.CardPrint(FieldByName('MEMBER').AsString,'',1,dm.IBTransaction);
           end else begin   
              Global.CardPrint(FieldByName('MEMBER').AsString,'',FieldByName('NUMBER_CARDS').AsInteger,dm.IBTransaction);
           end;
           next;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.ClearCardMembers;
var  CardMembers : TCardMembers;
begin
     // was done with IBRunOnce
     try
        CardMembers.CardDeleteMembers(dm.qryGeneral); 
        dm.IBCommitRetain;
     except                      
        dm.IBRollBackRetain;
        sm('Deletion of Cards was unsuccessful');
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.rgpExpiresClick(Sender: TObject);
begin
   case rgpExpires.ItemIndex of 
        0 : gbxDates.Visible := True;
        1 : gbxDates.Visible := False;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.menuQueriesClick(Sender: TObject);
begin
    // Queries use the Letter Form
    if frmMembersLetter = nil then
        if sfPreCheckLogOn(TfrmMembersLetter)
           then frmMembersLetter := TfrmMembersLetter.Create(nil)
        else exit;
     ResetLookups;
     frmMembersLetter.SetAsLetter := False;
     SMStdForm.ShowStdForm(frmMembersLetter);
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.dblcQueriesCloseUp(Sender: TObject; LookupTable, FillTable: TDataSet; modified: Boolean);
begin
     if modified then
        memoQueries.Text := dm.qryReport1.FieldByName('COMMENTS').AsString;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.MailingLabels;
//procedure TfrmMembersREPORT.menuMailingLabelsClick(Sender: TObject);
var i : smallint;
    HasMember : Boolean;
    Response : Word;
begin
     // Card Extract - do before the Card extract as this will possibly delete the file
     if FOwnIndex = 38 then begin 
        dm.qryReport.Close;
        dm.qryReport.SQL.Text := 'Select * from CARD_MEMBERS';
        dm.qryReport.Open;
     end;
     if not dm.qryReport.Active then exit;
     if dm.qryReport.RecordCount = 0 then exit;
     HasMember := False;
     for i := 0 to dm.qryReport.FieldCount - 1 do 
        if dm.qryReport.Fields[i].FieldName = 'MEMBER' then
           HasMember := True;
     if not HasMember then exit; 
     Response := MessageDlg(format('Mailing Labels for %s, Do you wish to put the Member Number on?',[ReportTitles[FOwnIndex]]),mtConfirmation,[mbYes,mbNo, mbCancel],0);
     if Response = mrCancel then Exit;
     
     TooBusy('Creating Mailing Labels ...');
     frmMailingLabels := TfrmMailingLabels.Create(nil);
     try
        with frmMailingLabels do begin
           qr.DataSet := dm.qryReport1;
           if not ExtractFromReport(Printer.PrinterIndex,Response) then begin
              Free;
              exit;
           end;
           NotBusy;
           frmMailingLabels.qr.Preview;
           //PrintOrPreview(frmMailingLabels,qr);
        end;
     finally
        NotBusy;
        dm.qryReport1.Close;
        frmMailingLabels.qr.DataSet := dm.qryReport;
        frmMailingLabels.Free;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.cbxAllClubsClick(Sender: TObject);
begin
     if cbxAllClubs.Checked then begin
         FExtFields := '';
         btnClubs.Enabled := False;
     end else
         btnClubs.Enabled := True;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.ExportCardsExcel(FName : String);
var oWorkBook: TOpExcelWorkbook;
    Rng : TOpExcelRange;

begin
     if ExtractFileExt(FName) <> '.csv' then begin
        sm('Not a CSV file - retry saving it as one');
        exit;
     end;
     try
        TooBusy ('Exporting to Excel ...');
        with dm.OpExcel do begin
          if not Connected  then begin
            Connected := True;
            Visible := True;
          end;
          oWorkBook := Workbooks.Add;            // create a workbook
          {Rng := oWorkBook.Worksheets[0].Ranges.Add;
          Rng.Name := 'CustomerRange';
          // Rng.Address := 'A1';
          Rng.AsRange.Columns.NumberFormat := Text;  //.NumberFormatLocal := 9;}
          oWorkBook.Filename := FName;
          Visible := True;                       // show it
          Interactive := True                    // Let the user access it
        end;
     finally
        NotBusy;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersREPORT.cbxDOBClick(Sender: TObject);
begin
     dbeYearFrom.Enabled := not cbxDOB.Checked;
     dbeYearTo.Enabled   := not cbxDOB.Checked;
end;
{ ****************************************************************************}
//procedure TfrmMembersREPORT.btnEmailMembersClick(Sender: TObject);
procedure TfrmMembersREPORT.EmailMembers;
var HasMember : Boolean; 
    i : smallint;
    //memberStr : string;
    MemberStrings : TStrings;
const SQL1 = 'Select EMAIL from MEMBERS where EMAIL is not null and RECEIVE_EMAIL = ''Y'' and DECEASED <> ''Y'' and MEMBER = %d';     
begin
     HasMember := False;
     i := 0;
     try
     if FOwnIndex = 63 then begin
        // shouldnt get to this
        exit;
     end else if FOwnIndex in [57,59] then begin
        // cdsTemp
        if not dm.cdsTemp.Active then exit;
        if dm.cdsTemp.RecordCount = 0 then exit;
        for i := 0 to dm.cdsTemp.FieldCount - 1 do 
           if dm.cdsTemp.Fields[i].FieldName = 'MEMBER' then
              HasMember := True;
        if Hasmember then begin
            TooBusy('Sending Emails to Members  ...');
            dm.cdsTemp.First;
            MemberStrings := TStringList.Create;
            MemberStrings.Clear;
            i := 0;
            while not dm.cdsTemp.EOF do begin
              if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[dm.cdsTemp.FieldByName('MEMBER').AsInteger])) then begin
                 if pos('@',dm.qryGeneral.FieldByName('EMAIL').AsString) > 0 then begin
                    MemberStrings.Add(dm.qryGeneral.FieldByName('EMAIL').AsString + ';');
                    inc(i);
                 end;   
              end;
              dm.qryReport.Next;
            end;
            //while not dm.cdsTemp.EOF do begin
           //   MemberStr := MemberStr + dm.cdsTemp.FieldByName('MEMBER').AsString + ',';
           //   dm.cdsTemp.Next;
           // end;
           // system.Delete(MemberStr,length(MemberStr),1);
           // dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[MemberStr]));
            //MemberStrings := TStringList.Create;
            //MemberStrings.Clear;
           // i := 0;
           // while not dm.qryGeneral.EOF do begin
           //   if pos('@',dm.qryGeneral.FieldByName('EMAIL').AsString) > 0 then begin
           ///      MemberStrings.Add(dm.qryGeneral.FieldByName('EMAIL').AsString + ';');
           //      inc(i);
           //   end;   
           //   dm.qryGeneral.Next;
           // end;
            NotBusy;
            if i = 0 then begin
               if assigned(MemberStrings) then 
                  MemberStrings.Free;
               sm('There are no Members to Email too');
               exit;
            end; 
        end else begin
           showmessage('No Members to Email to');
           exit;
        end;     
     end else begin   
        if not dm.qryReport.Active then exit;
        if dm.qryReport.RecordCount = 0 then exit;
        // Card Extract - do before the Card extract as this will possibly delete the file
        if FOwnIndex = 38 then begin 
           dm.qryReport.Close;
           dm.qryReport.SQL.Text := 'Select * from CARD_MEMBERS';
           dm.qryReport.Open;
        end;
        for i := 0 to dm.qryReport.FieldCount - 1 do 
           if dm.qryReport.Fields[i].FieldName = 'MEMBER' then
              HasMember := True;
        if Hasmember then begin
            TooBusy('Sending Emails to Members  ...');
            dm.qryReport.First;
            MemberStrings := TStringList.Create;
            MemberStrings.Clear;
            i := 0;
            while not dm.qryReport.EOF do begin
              if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[dm.qryReport.FieldByName('MEMBER').AsInteger])) then begin
                 if pos('@',dm.qryGeneral.FieldByName('EMAIL').AsString) > 0 then begin
                    MemberStrings.Add(dm.qryGeneral.FieldByName('EMAIL').AsString + ';');
                    inc(i);
                 end;   
              end;
              dm.qryReport.Next;
            end;
            //DeleteLastComma(MemberStr);
            //system.Delete(MemberStr,length(MemberStr),1);
            //showmessage(MemberStr);
            //while not dm.qryGeneral.EOF do begin
            //  if pos('@',dm.qryGeneral.FieldByName('EMAIL').AsString) > 0 then begin
            //     MemberStrings.Add(dm.qryGeneral.FieldByName('EMAIL').AsString + ';');
            //     inc(i);
            //  end;   
            //  dm.qryGeneral.Next;
            //end;
            NotBusy;
            if i = 0 then begin
               sm('There are no Members to Email too');
               if assigned(MemberStrings) then 
                  MemberStrings.Free;
               exit;
            end; 
        end else begin
           showmessage('No Members to Email to');
           exit;
        end;     
     end;   
     finally
        NotBusy;
     end;
     if sfPreCheckLogOn(TfrmMembersEmail) then begin
        //if not ifMsgDlg(format('Create Mailing Labels from %s?',[ReportTitles[FOwnIndex]])) then exit;
        TooBusy('Sending Emails to Members  ...');
        try
           frmMembersEmail := TfrmMembersEmail.Create(nil);
           frmMembersEmail.ESubject    := '';
           frmMembersEmail.EBody       := '';
           frmMembersEmail.EBccCount   := i; 
           frmMembersEmail.EfrmTypes   := AsMemberReport;
           frmMembersEmail.EBccStrings := MemberStrings.Text;
           if assigned(MemberStrings) then 
              MemberStrings.Free;
           frmMembersEmail.ShowModal;
        finally
           frmMembersEmail.Release;
        end;
     end;
end;
{ ****************************************************************************}

end.

