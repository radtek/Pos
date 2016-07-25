unit MembersPAY;
{ NB all Tranactions in this Unit use IBTransPay to isolate the payments from
  general updates.

Points   - Earnt by purchasing products - each product has a points value 
           eg $3.00 purchase may only be 2 points
Redeemed - 1 cent per point - stored in the database as a dollar amount
           Points_avaliable, Total_Redeemed, Total_earnt
           eg 100 points = $1.00  ( / 100)
           Subs can be paid by points        



 Sections 
 Paid   Expires   AmountPaid
 F      Below     0.00
 F      Above     < Amount
 T      Above     >= Amount



 Brief desc of how the payments work
 1Trans could involve many payments of Subs Sections and Joining Fee
 tables - PAYMENTS 1ToMany PAYMENTS_DET 1ToMany PAYMENTS_TEND
          GetReceiptNumber gets Payment ID for each table
 SUBSCRIPTIONS/SECTIONS hold the fee for each
 Members SUB_PAID is TSubPayTypes = (spZero,spPart,spFull) 0,1 or 2 and is a
   total of all Subs 
 Members EXPIRES  is calculated on PAY / FEE = ? yrs ahead 
   from the fixed expires or revolving expires ( from when they join - date )
   NB is based only on Subscriptions ( not on sections )

 If the Date is past the Member Expires then AmountPaid is 0.00 Amount = (latest fee)
   and PAY = Amount
 else the AmountPaid is left intact, Amount is the current Amount and PAY usually is 0.00.
  
 
                Expires <= Date  Insert   Update     also        then                 then  
                Work Out                             Update      Update               Update
                 
1 -> Subs       Fee,AmountPaid     P      P _+ P _    M_Subs      Members              P
T                                  a      a d  a t                (SubPaid, Expires)   a
R                                  y      y e  y e                (total of all Subs)  y
A                                  m      m t  m n                                     m
N -> Sections   Fee,AmountPaid     e      e    e d    M_Sections                       e
S                                  n      n    n      (Expires)                        n
                                   t      t    t                                       t
  -> Join Fee                      s      s    s                  Members (JoinFee)    s
                                                           
Subscription structures
usually linked by the Parent Code (MAILING_LIST_CODE)

0 charge for some of the members
rules are only one Parent Code per payment
Corporate = 5000               each member = 0.00
family    = 165                each member = 0.00

A charge for each member
family    = 25 each  up to 2 ,additional members = 10
Member    = 25

 }
interface


uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm, ComCtrls, wwSpeedButton, wwDBNavigator, wwclearpanel,
  ExtCtrls, StdCtrls, wwdblook, Grids, Wwdbigrd, Wwdbgrid,
  wwdbdatetimepicker, Mask, wwdbedit,DB, wwDialog, wwrcdvw, Buttons,
  ImgList, ToolWin,Printers, wwidlg, Wwkeycb, Wwdotdot, Wwdbcomb,IBCustomDataSet,
  lmdcompo, lmdclass, lmdnonvS, IBTable, Menus,OPWord, OpWrd2K,
  OpModels, OpDbOfc, OpShared, lmdcctrl, lmdctrl, lmdgroup, DBCtrls;

type
  TfrmMembersPAY = class(TSMStdForm)
    PageControl: TPageControl;
    tsSubsPay: TTabSheet;
    tsSectionsPay: TTabSheet;
    tsReprint: TTabSheet;
    lblMemberExpiry: TLabel;
    btnCash: TToolButton;
    btnSaveSection: TToolButton;
    btnTender: TToolButton;
    menuSubs: TMenuItem;
    menuPaybyCash: TMenuItem;
    menuPaybyTender: TMenuItem;
    menuSections: TMenuItem;
    menuSavetheSection: TMenuItem;
    menuTenderSection: TMenuItem;
    menuBreak: TMenuItem;
    menuClearSection: TMenuItem;
    menub: TMenuItem;
    menubb: TMenuItem;
    menuClearSubs: TMenuItem;
    menuChangeSubs: TMenuItem;
    btnClear: TToolButton;
    btnChangeSubs: TToolButton;
    btnTenderSection: TToolButton;
    menuExit: TMenuItem;
    btnPrint: TToolButton;
    btnPreview: TToolButton;
    Options1: TMenuItem;
    menuPrint: TMenuItem;
    menuPrinterSetup: TMenuItem;
    menuPreview: TMenuItem;
    btnPrintInvoice: TToolButton;
    menubbbb: TMenuItem;
    menuPrintInvoice: TMenuItem;
    pnlSpace: TPanel;
    N1: TMenuItem;
    ToolButton6: TToolButton;
    btnLateFee: TToolButton;
    menuPrintTodaysTotals: TMenuItem;
    btnPrinterSetup: TToolButton;
    Panel2: TPanel;
    pnlReprint: TPanel;
    Label16: TLabel;
    Label6: TLabel;
    Label8: TLabel;
    dtPicker: TwwDBDateTimePicker;
    dblcSearch: TwwDBLookupCombo;
    dbeReceipt: TwwDBEdit;
    Panel1: TPanel;
    dbgPayments_Det: TwwDBGrid;
    dbgPayments_DetIButton: TwwIButton;
    Panel3: TPanel;
    dbgPayments: TwwDBGrid;
    dbgPaymentsIButton: TwwIButton;
    Panel4: TPanel;
    dblcSections: TwwDBLookupCombo;
    dbcbOptions: TwwDBComboBox;
    Label15: TLabel;
    incSearch: TwwIncrementalSearch;
    btnMembers: TRadioButton;
    btnSurname: TRadioButton;
    Panel5: TPanel;
    Label13: TLabel;
    dblcSectionPaidBy: TwwDBLookupCombo;
    Label12: TLabel;
    dbeSectionsDue: TwwDBEdit;
    cbxUsePointsS: TCheckBox;
    dbePointsS: TwwDBEdit;
    Label14: TLabel;
    dbeTotalSection: TwwDBEdit;
    Panel6: TPanel;
    dbgSectionsPay: TwwDBGrid;
    dbgSectionsPayIButton: TwwIButton;
    Panel7: TPanel;
    dblcSurname: TwwDBLookupCombo;
    Label10: TLabel;
    dblcPaidBy: TwwDBLookupCombo;
    Label7: TLabel;
    dbeFullName: TwwDBEdit;
    pnlSubs: TPanel;
    dbgSubPay: TwwDBGrid;
    dbgSubPayButton: TwwIButton;
    pnlSections: TPanel;
    dbgPaySecMem: TwwDBGrid;
    btndbgPayMemSec: TwwIButton;
    pnlJoinFee: TPanel;
    dbgJoinPay: TwwDBGrid;
    Panel11: TPanel;
    Label3: TLabel;
    dbeJoiningFeeDue: TwwDBEdit;
    Label5: TLabel;
    Label2: TLabel;
    lblToPay: TLabel;
    dbeSubsDue: TwwDBEdit;
    dbeSectionDue: TwwDBEdit;
    dbeTotal: TwwDBEdit;
    Splitter2: TSplitter;
    tsBulkCardReceipt: TTabSheet;
    Panel12: TPanel;
    Panel13: TPanel;
    dbgBulk: TwwDBGrid;
    N2: TMenuItem;
    menuAddMemberCard: TMenuItem;
    tsAutoPay: TTabSheet;
    Panel15: TPanel;
    Panel16: TPanel;
    dbgAutoPay: TwwDBGrid;
    GroupBox2: TGroupBox;
    rgpBulk: TLMDRadioGroup;
    GroupBox3: TGroupBox;
    Label9: TLabel;
    dtpDateFrom: TwwDBDateTimePicker;
    Label17: TLabel;
    dtpDateTo: TwwDBDateTimePicker;
    Panel17: TPanel;
    GroupBox4: TGroupBox;
    dbgAutoPayIButton: TwwIButton;
    Panel19: TPanel;
    GroupBox6: TGroupBox;
    btnExportCards: TButton;
    btnExportReceipts: TButton;
    btnPrintCards: TButton;
    Panel14: TPanel;
    dbdtAutoPay: TwwDBDateTimePicker;
    menuLastPaid: TPopupMenu;
    menuLastPayment: TMenuItem;
    Label1: TLabel;
    dbdtPayDate: TwwDBDateTimePicker;
    Label18: TLabel;
    dbeDiscount: TwwDBEdit;
    Label19: TLabel;
    Label20: TLabel;
    Panel20: TPanel;
    GroupBox7: TGroupBox;
    btnStart: TToolButton;
    menuBulkCardReceipt: TMenuItem;
    menuStartCR: TMenuItem;
    menuCancelCR: TMenuItem;
    menuSaveCR: TMenuItem;
    menuAutoPayments: TMenuItem;
    menuStartAP: TMenuItem;
    menuCancelAP: TMenuItem;
    menuSaveAP: TMenuItem;
    btnExportCrds: TButton;
    Label21: TLabel;
    dbeProRata: TwwDBComboBox;
    Label22: TLabel;
    dbePartPay: TwwDBComboBox;
    N3: TMenuItem;
    menuUndoInvoice: TMenuItem;
    btnSaveReceipts: TButton;
    dbdtSetPayDay: TwwDBDateTimePicker;
    Label23: TLabel;
    RichEdit: TRichEdit;
    ToolButton2: TToolButton;
    ToolButton1: TToolButton;
    menuMemberBrief: TMenuItem;
    incAutoPayments: TwwIncrementalSearch;
    rpgAutoPay: TLMDRadioGroup;
    Label4: TLabel;
    DBText1: TDBText;
    dbePoints: TwwDBEdit;
    menuAddaLateSub: TMenuItem;
    btnChangeSection: TToolButton;
    menuChangeSection: TMenuItem;
    menuDeleteSection: TMenuItem;
    procedure dblcSurnameCloseUp(Sender: TObject; LookupTable, FillTable: TDataSet; modified: Boolean);
    procedure dblcSurnameDropDown(Sender: TObject);
    procedure dblcSurnameKeyPress(Sender: TObject; var Key: Char);
    procedure dbgSubPayButtonClick(Sender: TObject);
    procedure PopulatePayGrid;
    procedure FormCreate(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure dbgSubPayCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure CalcTotal;
    procedure dblcPaidByKeyPress(Sender: TObject; var Key: Char);
    procedure dbgJoinPayCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure FillGrids;
    procedure dblcPaidByCloseUp(Sender: TObject; LookupTable, FillTable: TDataSet; modified: Boolean);
    procedure CalcJoinPay;
    procedure CalcSubPay;
    procedure CalcSubPaySetup;
    procedure cbUsePointsClick(Sender: TObject);
    procedure dbgSubPayFieldChanged(Sender: TObject; Field: TField);
    procedure dbeFullNameKeyPress(Sender: TObject; var Key: Char);
    procedure FormDestroy(Sender: TObject);
    procedure dblcPaidByKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    function UpdatePayJoin : Boolean ;
    procedure EndTransaction ( Msg : String );
    procedure ClearPayFields;
    procedure UpdatePayments;
    Function UpdateSubPaid : Boolean;
    procedure PageControlChange(Sender: TObject);
    procedure dblcSearchKeyPress(Sender: TObject; var Key: Char);
    procedure dblcSearchCloseUp(Sender: TObject; LookupTable, FillTable: TDataSet; modified: Boolean);
    procedure PageControlChanging(Sender: TObject; var AllowChange: Boolean);
    procedure dbeReceiptKeyPress(Sender: TObject; var Key: Char);
    procedure GetPayments;
    procedure SelectPayments;
    procedure dbgPaymentsCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure dbgPayments_DetCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure dbgPayments_DetUpdateFooter(Sender: TObject);
    procedure OpenTables;
    procedure PrintFromSubs(FromList : TStringList);
    procedure dbgPaymentsDblClick(Sender: TObject);
    procedure GetLinkedMembers( MemberCode : shortstring );
    function  GetLatestFee(const tblSubs,SubsName,AutoPayment : shortstring ; SubsInt : Integer) : Extended;
    procedure  UpdateSubsSections(  PayType : shortint; theDataSet : TIBDataSet);
    procedure UpdatePoints;
    procedure CommitAndOpenTables;
    procedure SubsPayButtonSetup;
    procedure SectionsPayButtonSetup;
    procedure ReprintButtonSetup;
    procedure PrintReceiptLetter( ReceiptLetterStr : string );
    procedure PopulateSectionsGrid;
    procedure dblcSectionsKeyPress(Sender: TObject; var Key: Char);
    procedure dblcSectionsCloseUp(Sender: TObject; LookupTable, FillTable: TDataSet; modified: Boolean);
    procedure incSearchExit(Sender: TObject);
    procedure btnMembersClick(Sender: TObject);
    procedure ClearSectionFields;
    procedure FillGridsSections;
    procedure CalcSection;
    procedure cbxUsePointsSClick(Sender: TObject);
    function SectionCheckDate : Boolean ;
    procedure dbgSectionsPayRowChanged(Sender: TObject);
    procedure CalcSectionTotal;
    procedure UpdateSection;
    procedure PopulateSectionsSorted;
    procedure dbcbOptionsChange(Sender: TObject);
    procedure dbgSectionsPayCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure dbgSectionsPayFieldChanged(Sender: TObject; Field: TField);
    procedure incSearchKeyPress(Sender: TObject; var Key: Char);
    procedure dbgSectionsPayDblClick(Sender: TObject);
    procedure dblcSectionPaidByKeyPress(Sender: TObject; var Key: Char);
    procedure dblcSectionPaidByCloseUp(Sender: TObject; LookupTable, FillTable: TDataSet; modified: Boolean);
    procedure dtPickerKeyPress(Sender: TObject; var Key: Char);
    procedure dbgSubPayExit(Sender: TObject);
    procedure dbgPaySecMemCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
    procedure dbgPaySecMemFieldChanged(Sender: TObject; Field: TField);
    procedure dbgSubPayEnter(Sender: TObject);
    procedure dbgSubPayKeyPress(Sender: TObject; var Key: Char);
    procedure SetDataSource( theDataSource : TDataSource; theDataSet : TIBDataSet);
    procedure UpdateFooter;
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure menuSavetheSectionClick(Sender: TObject);
    procedure menuPaybyCashClick(Sender: TObject);
    procedure menuChangeSubsClick(Sender: TObject);
    procedure menuClearSubsClick(Sender: TObject);
    procedure menuExitClick(Sender: TObject);
    procedure menuTenderSectionClick(Sender: TObject);
    procedure menuPrintClick(Sender: TObject);
    procedure menuPrinterSetupClick(Sender: TObject);
    //procedure menuPrintCardClick(Sender: TObject);
    procedure menuPrintTodaysTotalsClick(Sender: TObject);
    procedure DisableButtons;
    procedure FormResize(Sender: TObject);
   function UpdateParentCodePays : Boolean;
    procedure dtpDateFromExit(Sender: TObject);
    procedure dtpDateFromKeyPress(Sender: TObject; var Key: Char);
    procedure dtpDateToKeyPress(Sender: TObject; var Key: Char);
    procedure btnPrintCardsClick(Sender: TObject);
    procedure dbgBulkUpdateFooter(Sender: TObject);
    procedure btnExportReceiptsClick(Sender: TObject);
    procedure menuAddMemberCardClick(Sender: TObject);
    procedure btnExportCardsClick(Sender: TObject);
    procedure dtpDateFromChange(Sender: TObject);
    procedure GetAutoPayMembers( theChar : char);
    procedure dbgAutoPayCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont;      ABrush: TBrush);
    procedure dbgAutoPayIButtonClick(Sender: TObject);
    procedure ResetBulkGrid;
    procedure ShowExpiresLastPAyment;
    procedure dbeDiscountExit(Sender: TObject);
    procedure menuPrintInvoiceClick(Sender: TObject);
    procedure rpgAutoPayChange(Sender: TObject; ButtonIndex: Integer);
    procedure incAutoPaymentsKeyPress(Sender: TObject; var Key: Char);
    procedure menuStartCRClick(Sender: TObject);
    procedure menuSaveCRClick(Sender: TObject);
    procedure menuCancelCRClick(Sender: TObject);
    procedure menuStartAPClick(Sender: TObject);
    procedure menuSaveAPClick(Sender: TObject);
    procedure menuCancelAPClick(Sender: TObject);
    procedure dbeDayToKeyPress(Sender: TObject; var Key: Char);
    procedure btnSaveSectionClick(Sender: TObject);
    procedure btnClearClick(Sender: TObject);
    procedure btnStartClick(Sender: TObject);
    procedure AutoPayButtonSetup;
    procedure menuClearSectionClick(Sender: TObject);
    procedure CloseOpenWithSQL( theDAtaSet : TIBDataSet; SQL1 : string );
    procedure dbeProRataCloseUp(Sender: TwwDBComboBox; Select: Boolean);
    procedure menuViewSubscriptionsGridClick(Sender: TObject);
    procedure menuUndoInvoiceClick(Sender: TObject);
    procedure btnSaveReceiptsClick(Sender: TObject);
    procedure PrintCardsAndReceipts;
    procedure dbgAutoPayUpdateFooter(Sender: TObject);
    procedure dbePointsEnter(Sender: TObject);
    procedure dbePointsDblClick(Sender: TObject);
    procedure dbePointsExit(Sender: TObject);
    procedure PopulateFile( var F : Textfile; Receipt : ShortString );
    procedure AddRemoveLateFeeSub;
    procedure menuMemberBriefClick(Sender: TObject);
    procedure dbgSubPayDblClick(Sender: TObject);
    procedure BuildCheckedFieldsList;
    procedure ExportSubsToFile;
    function GetTenders : Boolean ;
    procedure dbgSubPayRowChanged(Sender: TObject);
    procedure CalcPoints;
    procedure DBText1DblClick(Sender: TObject);
    procedure btnLateFeeClick(Sender: TObject);
    procedure btnChangeSectionClick(Sender: TObject);
    procedure dbgPaySecMemDblClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    //function WorkOutDiscount( DiscountMember : Extended ; DiscountableCount : ShortInt ): Extended;
  private
    { Private declarations }
     FSubsDue : Extended;
     
     InSaveMode,FInvoiced, FPartPayUserModified,FMemberPreExpiry,FParentCodePays,FDoneMembersEditBrief,FLateSubAdded : boolean;
     TempMember : string[10];
     PaymentStr,Memberstr,FFileName : string;
     FGoodDate : TDateTime;
     FRoundValue,FPoints,FAmountPaid,FJoiningFeeDue,FTotalLessPoints{,FDiscountMEmber} : extended;
     FPointsUsed,FDiscount,FHoldTotalLessPoints,FSectionDue,FTotalToPayBalance : Extended;
     FReceiptNumber,FKeepMailingListCode,FLateSub : LongInt;

{ **************************************************************************** }
// section privates
     FSection  : Integer;
     FAddOrder,FFilterStr : string;
     FLoadingSections : Boolean;
     FpnlWidth : SmallInt;
  public
    { Public declarations }
    //Printer : TPrinter;
    //PrinterIndex: integer;
    // passed into dm for Sections Payments
    SectionReceiptList : TStringList;
    ParentCodeList : TStringList;
    
    SectionGoodDate   : TDateTime;
    SectionReceipt    : LongInt;
    SectionName       : string;
    PointsUsedSection : Extended;
    RoundValue : Extended;
    Blocked : Boolean;
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;
  
var
  frmMembersPAY: TfrmMembersPAY;
  MergeDoc: TOpWordDocument;
const
  NoTotalPaymentMsg = 'There is no Total Payment';
  SubsSQL = 'select M.MEMBER,M.FULL_NAME,M.JOINING_FEE,s.SUB_GROUPS,' +
     'M.AUTO_PAYMENT,m.PAYMENT_PERIOD,m.MAILING_LIST_CODE,s.PRO_RATA,s.DISCOUNTABLE,m.SUB_PAYMENT,'+
     'M.DISCOUNT,M.EXPIRES,MS.SUBSCRIPTION,MS.AMOUNT_PAID,MS.AMOUNT,(MS.AMOUNT - MS.AMOUNT_PAID) AS AMOUNT_DUE,M.SUB_PAID,' +
     'S.DESCRIPTION,MS.EDIT_DATE,MS.RECEIPT,MS.PAID,MS.PAY,ms.PRINT_CARD,ms.PRINT_RECEIPT,ms.INVOICED, ' +
     'M.DATE_OF_BIRTH,M.ADDRESS,M.ADDRESS1,m.DISTRICT,m.CITY,m.COUNTRY,m.JOINED,s.SPARE_CHAR ' + 
     'from MEMBERS M, SUBSCRIPTIONS S,M_SUBS MS ';
   SectionsSQL = 'Select M.MEMBER,M.FULL_NAME, S.DESCRIPTION,MS.AMOUNT_PAID,MS.AMOUNT,S.MIN_FEE,' +
     'MS.PAY,MS.EXPIRES,MS.PAID,MS.SECTION,MS.EDIT_DATE,MS.RECEIPT,S.EXPIRES as SECTION_EXPIRES from M_SECTIONS MS, SECTIONS S, MEMBERS M '; 
   DropDownSQL = 'select SURNAME,MEMBER,FULL_NAME,FIRST_NAME,ADDRESS,MAILING_LIST_CODE from MEMBERS ';   
  
implementation

{$R *.DFM}
uses
    Global,    // StockMaster
    dmMembers,
    rPayReprint,
    rPayTotals,
    Tenders,
    MembersLOOK,
    MembersEDIT_BRIEF,
    MembersCLASSES,
    MembersLOOKUP, MemberMenu, rInvoiceLandscape, MembersREPORT, rDueLetter,
    rGenericReceipt,
  rDirectDebit; // StockMaster
var
   ReceiptList,MemberList,CheckedFieldsList : TStringList;
   HeightWidth : THeightWidth;
   BeforeEditPointsCredit : TBeforeEditPointsCredit;
{ **************************************************************************** }
class function TfrmMembersPAY.ScreenCode : Integer;
begin Result := Global.scMembersPayments; end;
{ **************************************************************************** }
class function TfrmMembersPAY.ScreenName : String;
begin Result := 'Members Payments       ' ; end;
{ **************************************************************************** }
class function TfrmMembersPAY.ScreenDesc : String;
begin Result := '  Members Payments Screen'; end;
{ **************************************************************************** }
procedure TfrmMembersPAY.FormCreate(Sender: TObject);
var i,j : shortint;
    s : string;
begin
     inherited;
     HeightWidth.frmHeight := Self.Height;
     HeightWidth.frmWidth  := Self.Width;
     FpnlWidth := pnlSpace.Width;
     dbePoints.Text := '$0.00';
     FLateSubAdded := False;
     OpenTables;
     FDoneMembersEditBrief := False;

     InSaveMode := False;
     FLoadingSections := False;
     FAddOrder := 'MEMBER';
     SectionReceipt := 0;
     PointsUsedSection := 0.00;
     Blocked := False;
     incSearch.SearchField := 'MEMBER';
     MemberList := TStringList.create;
     ReceiptList := TStringList.create;
     SectionReceiptList := TStringList.create;
     ParentCodeList := TStringList.create;
     CheckedFieldsList := TStringList.create;
     BuildCheckedFieldsList;

     FReceiptNumber := -1;
     FillGrids;
     PageControl.ActivePage := tsSubsPay;
     dbeProRata.Text := '12';
     ReadPrinters;

     {if Global.CommPortsTypes <> cpNone then begin
        dm.ComPort.Port := Ord(CommPortsTypes);
        dm.ComPort.Open := True;
     end;}
     if Global.ReceiptPrinterIndex <> -1 then begin
        Printer.PrinterIndex := Global.ReceiptPrinterIndex;
     end else
        Printer.PrinterIndex := -1; // set to default
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.FormDeactivate(Sender: TObject);
begin
     inherited;
     dm.IBTransactionCommit; // Commit changes on deactivate
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.FormShow(Sender: TObject);
var   MenuMatePointsUpdate : TMenuMatePointsUpdate;
begin
     MenuMatePointsUpdate.MembersPointsUpdate('-1');
end;
{ ****************************************************************************}
procedure TfrmMembersPAY.FormActivate(Sender: TObject);
begin
     // have gone to quick report processing an invoice/Letter or receipt
     if InSaveMode then exit;
     inherited; // sets up the Windows Menu
     // resize controls
     if (Screen.Height = 480) and not (Self.WindowState = wsMaximized) then begin
        dbeDiscount.Width := 42;
        dbeTotal.Width := 42;
        dbgSubPay.SetActiveField('DESCRIPTION');
        dbgSubPay.SelectedField.DisplayWidth := 19;
     end;
     FGoodDate := Date;
     if not Global.CheckSystemDate( FGoodDate ) then begin
        ClearPayFields; // does a rollback here
        sm( IncorrectDateMsg + #13#10 + 'The Date must be altered! No payments will be accepted');
        close;
     end;
     dbdtPayDate.Date      := FGoodDate;
     FMemberPreExpiry      := False;
     PageControlChange(Sender);

     // Some Clubs when getting new Members 30-60 days before the Expires
     // then they automatically get this year and next
     // saves re-issueing cards in a month
     if Global.FixedExpirySystem then begin
        if FGoodDate < MemberExpiry  then begin // watch after rollover
           if ((FGoodDate + MemberPreExpiry) >= MemberExpiry ) then begin
               lblMemberexpiry.Caption := format('New Members Expires Date = %s',[FormatDateTime('dd mmm yyyy',AddOneYear(MemberExpiry))]);
               FMemberPreExpiry := True;
               lblMemberexpiry.Font.Color := clBlack;
           end;
        end;
     end;

     // refresh the details on startup
     if (dm.ibdsPayDet.RecordCount > 0) or (dm.ibdsPayJoin.RecordCount > 0) or
        (dm.ibdsPayMemSec.RecordCount > 0) then begin
        dm.ibdsPayMembers.Close;
        dm.ibdsPayMembers.Open;
        PopulatePayGrid;
        dbgSubPay.Update;
     end else begin
        // pass value from Members Edit Screen if no entries
        //if dm.qryMembersLook.RecordCount > 0 then begin
           dblcSurname.Text :=  LastMemberInMaintenance;
           dblcSurname.SelectAll
        //end;
     end;
     // security
     if not dm.PreCheckLogOn(scPaymentsTabSubs,stPaymentsTabSubs ) then begin
        RemoveTab(tsSubsPay);
     end else begin
        // String of YYY or YNN
        if PaymentGrids[1] = varNo then begin// Subs
           dbgSubPay.Visible := False;
           pnlSubs.Visible := False;
        end;
        if PaymentGrids[2] = varNo then begin// Sections
           pnlSections.Visible := False;
           dbgPaySecMem.Visible := False;
        end;
        if PaymentGrids[3] = varNo then begin// Join Fee
           pnlJoinFee.Visible := False;
           dbgJoinPay.Visible := False;
        end;   
     end;     
     
     if not dm.PreCheckLogOn(scPaymentsTabSections,stPaymentsTabSections ) then RemoveTab(tsSectionsPay); 
     if not dm.PreCheckLogOn(scPaymentsTabReprint,stPaymentsTabReprint ) then RemoveTab(tsReprint);
     if not dm.PreCheckLogOn(scPaymentsTabBulkCardReceipt,stPaymentsTabBulkCardReceipt ) then RemoveTab(tsBulkCardReceipt); 
     if not dm.PreCheckLogOn(scPaymentsTabAutoPay,stPaymentsTabAutoPAy ) then RemoveTab(tsAutoPay);
     // no body uses it and there is bugs with it
     RemoveTab(tsSectionsPay);
     RemoveTab(tsBulkCardReceipt);
     
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.FormDestroy(Sender: TObject);
// work committed in the save - any other work is rolled back that isnt saved
begin
     if dm.IBTransPay.InTransaction then
       dm.IBTransPay.RollBack;
     MemberList.Free;
     ParentCodeList.Free;
     ReceiptList.Free;
     SectionReceiptList.Free;
     CheckedFieldsList.Free;
     dm.ibdsPayMembers.Close;
     dm.ibdsPayJoin.Close;
     dm.ibdsPayDet.Close;
     //dm.ibdsPaySections.Close;
     //dm.ibdsPaySecMem.Close;
     //dm.ibdsPayMemSec.Close;
     //dm.ibdsBulkCardRec.Close;
     dm.ibdsAutoPay.Close;
     dm.ibdsPayments.Close;
     dm.ibdsPay_Det.Close;
     dm.ibdsMembersBrief.Close;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.OpenTables;
begin
     dm.ibdsPayJoin.Open;
     dm.ibdsPayDet.Open;
     //dm.ibdsPaySections.Open;
     //dm.ibdsPaySecMem.Open;
     //dm.ibdsPayMemSec.Open;   
     dm.ibdsPayMembers.Open;
     dm.ibdsPayments.Open;
     dm.ibdsPay_Det.Open;
     //dm.ibdsBulkCardRec.Open;   
     dm.ibdsAutoPAy.Open;   
     dm.ibdsMembersBrief.Open;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dblcSurnameKeyPress(Sender: TObject;
  var Key: Char);
var i : smallint;
begin
    if key = #13 then begin
        TempMember := dblcSurname.Text;
        if TempMember = '' then exit;
        if global.AllDigits( TempMember ) then begin
           i := MemberList.IndexOf(TempMember);
           if i <> -1 then begin
              sm(Format('Member %s already selected',[TempMember]));
              dblcSurname.Clear;
              exit;
           end;
       end;
       dblcSurnameDropDown(Sender);
       if dm.qryMembersPAY.EOF then begin
          showmessage(Format('%s %s',[dblcSurname.Text,NotAValidMemMsg]));
          dblcSurname.Setfocus;
          exit;
       end else begin
          TempMember := dm.qryMembersPAY.FieldByName('MEMBER').AsString;
          MemberList.Add(TempMember);
          if MemberList.Count = 1 then begin  // 1ST time thru
             dm.ibdsPayMembers.Close;
             dm.ibdsPayMembers.Params.ByName('MEMBER').AsInteger := StrToIntDef(TempMember,0);
             dm.ibdsPayMembers.Open;
          end;
          GetLinkedMembers(dm.qryMembersPAY.FieldByName('MAILING_LIST_CODE').AsString);
          PopulatePayGrid;
       end;
       //dblcPaidBy.Text := dm.ibdsPayMembers.FieldByName('MEMBER').AsString;
       //SelectNext(ActiveControl as TWinControl,True,True);
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dblcSurnameCloseUp(Sender: TObject; LookupTable,
  FillTable: TDataSet; modified: Boolean);
var i : smallint;
begin
     if modified then begin
        TempMember := dm.qryMembersPay.FieldByName('MEMBER').AsString;
        if TempMember = '' then exit;
        i := MemberList.IndexOf(TempMember);
        if i = -1 then begin
           MemberList.Add(TempMember);
        end else begin
           sm(Format('Member %s already selected',[TempMember]));
           dblcSurname.Setfocus;
           exit;
        end;
        if MemberList.Count = 1 then begin // 1ST time thru
           dm.ibdsPayMembers.Close;
           dm.ibdsPayMembers.Params.ByName('MEMBER').AsInteger := StrToIntDef(TempMember,0);
           dm.ibdsPayMembers.Open;
        end;
        GetLinkedMembers(dm.qryMembersPAY.FieldByName('MAILING_LIST_CODE').AsString);
        PopulatePayGrid;
        //dbeTotal.SetFocus;
        //SelectNext(ActiveControl as TWinControl,True,True);

     end;
     dblcPaidBy.Text := dm.ibdsPayMembers.FieldByName('MEMBER').AsString;
     dblcSurname.Clear;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dblcSurnameDropDown(Sender: TObject);
var s : string;
    i : shortint;
//const SQL1 = 'select SURNAME,MEMBER,FULL_NAME,FIRST_NAME,ADDRESS,MAILING_LIST_CODE from MEMBERS ';
// pre drop down selection - tsReprint also uses this and Sections
begin
     if TwwDBLookupCombo(Sender).Name = dblcSurname.Name then s := dblcSurname.Text;
     if TwwDBLookupCombo(Sender).Name = dblcPaidBy.Name  then s := dblcPaidBy.Text;
     if TwwDBLookupCombo(Sender).Name = dblcSearch.Name  then s := dblcSearch.Text; // Reprint
     if TwwDBLookupCombo(Sender).Name = dblcSectionPaidBy.Name then s := dblcSectionPaidBy.Text; // Sections
     if s = '' then exit;
     s := uppercase(s);
     dm.qryMembersPAY.Close;
     try
         if not global.AllDigits( s ) then begin
           if s[1] = '1' then begin // search by first name
              delete(s,1,1);
              dm.qryMembersPAY.SQL.Text := Format(DropDownSQL + ' where upper(FIRST_NAME) LIKE ''%s%s''',[s,'%']);
           end else begin
              dm.qryMembersPAY.SQL.Text := Format(DropDownSQL + ' where upper(SURNAME) LIKE ''%s%s''',[s,'%']);
           end;
        end else begin
           dm.qryMembersPAY.SQL.Text := Format(DropDownSQL + ' where MEMBER = %d',[StrToIntdef(s,0)]);
        end;
     finally
        dm.qryMembersPAY.Open;

     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.GetLinkedMembers( MemberCode : shortstring );
// adds all of the linked members to the list - MAILING_LIST_CODE
var i,j : smallint;
const SQL1 = //'select SURNAME,MEMBER,FULL_NAME,FIRST_NAME,ADDRESS,MAILING_LIST_CODE from MEMBERS ' +
             'where MAILING_LIST_CODE = %s';
      SQL2 = 'select sum(AMOUNT) as AMOUNT from M_SUBS where MEMBER = %d';
begin
     FKeepMailingListCode := -1;
     FParentCodePays := False;
     if MemberCode = '' then exit;
     j := 0;
     if dm.qryOpenCheckEOF(dm.qryMembersPAY,format(DropDownSQL + SQL1,[MemberCode])) then begin
        with dm.qryMembersPAY do begin
           First;
           while not EOF do begin
              // get the Mailing list code for the Paying Member
              // only one FKeepMailingListCode per trans if structure is
              // corporate 5000 and each member is free
              // may have keyed in any on of the members
              if FieldByName('MEMBER').AsInteger = FieldByName('MAILING_LIST_CODE').AsInteger then
                 FKeepMailingListCode := FieldByName('MEMBER').AsInteger;
              if dm.sqlExecQueryCheckEOF(dm.sqlTransPay,format(SQL2,[FieldByName('MEMBER').AsInteger])) then begin
                 if dm.sqlTransPay.FieldByName('AMOUNT').AsFloat > 0.00 then inc(j);
              end;
              i := MemberList.IndexOf(FieldByName('MEMBER').AsString);
              if i = -1 then // not found in the list
                 MemberList.Add(FieldByName('MEMBER').AsString);
              next;
           end;
           // if only one Member has an Amount to Pay and more than one member selected
           if (j = 1) and (dm.qryMembersPAY.RecordCount <> 1 ) then 
              FParentCodePays := True;
        end;       
     end;
end;
{ ******************************** ******************************************** }
procedure TfrmMembersPAY.dbgSubPayButtonClick(Sender: TObject);
// link the query qryPayLook.DataSource to each datasource - frmMembersLook
var Member : LongInt;
begin
     //dm.qryPayLook.Close;
     case TwwIButton(sender).Tag of
     0 : begin
         if dm.ibdsPayDet.Recordcount = 0 then exit;
         Member := dm.ibdsPayDet.FieldByName('MEMBER').AsInteger;
         //dm.qryPayLook.DataSource := dm.dsrPayDet;
         end;
     1 : begin
         if dm.ibdsPayMemSec.Recordcount = 0 then exit;
         Member := dm.ibdsPayMemSec.FieldByName('MEMBER').AsInteger;
         //dm.qryPayLook.DataSource := dm.dsrPayMemSec;
         end;
     2 : begin
         if dm.ibdsPaySections.Recordcount = 0 then exit;
         Member := dm.ibdsPaySections.FieldByName('MEMBER').AsInteger;
         //dm.qryPayLook.DataSource := dm.dsrPaySections;
         end;
     3 : begin
         if dm.ibdsPay_Det.Recordcount = 0 then exit;
         Member := dm.ibdsPay_Det.FieldByName('MEMBER').AsInteger;
         //dm.qryPayLook.DataSource := dm.dsrPay_Det;
         end;
     end;        
     try
       frmMembersLook := TfrmMembersLook.Create(self);
       frmMembersLook.LookupLinks := True;
       frmMembersLook.Member := Member;
       frmMembersLook.ShowModal;
     finally
       dm.qryAttachSubs.Close;
       frmMembersLook.Free;
     end;  
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.SetDataSource( theDataSource : TDataSource; theDataSet : TIBDataSet);
begin
      with theDataSet do begin
         close;
         DataSource := theDataSource;
         open;
      end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.PopulatePayGrid;
var  i : smallint;
     // subs and Sections - do not get the Member if the expires date is NULL
//const SQL1 = 'select M.MEMBER,M.FULL_NAME,M.POINTS_AVAILABLE,M.JOINING_FEE,' +
{const SQL1 = 'select M.MEMBER,M.FULL_NAME,M.JOINING_FEE,' +
          'M.AUTO_PAYMENT,m.PAYMENT_PERIOD,m.MAILING_LIST_CODE,s.PRO_RATA,s.DISCOUNTABLE,m.SUB_PAYMENT,s.SUB_GROUPS,'+
          'M.DISCOUNT,M.EXPIRES,MS.SUBSCRIPTION,MS.AMOUNT_PAID,MS.AMOUNT,(MS.AMOUNT - MS.AMOUNT_PAID) AS AMOUNT_DUE,M.SUB_PAID,' +
          'S.DESCRIPTION,MS.EDIT_DATE,MS.RECEIPT,MS.PAID,MS.PAY,M.MAILING_LIST_CODE,ms.PRINT_CARD,ms.PRINT_RECEIPT,ms.INVOICED ' +
          'from MEMBERS M, SUBSCRIPTIONS S,M_SUBS MS ' +
          'where M.MEMBER in ( %s )  AND S.SUBSCRIPTION = MS.SUBSCRIPTION and M.MEMBER = MS.MEMBER ' +
          ' and M.EXPIRES is not NULL order by M.MEMBER,s.SUB_GROUPS';
       SQL2 = 'select MEMBER,FULL_NAME,EXPIRES,JOINING_FEE,RECEIPT,PAY ' +
          ' from MEMBERS where MEMBER in ( %s ) and JOINING_FEE > 0.00 order by MEMBER';
       SQL3 = 'Select M.MEMBER,M.FULL_NAME, S.DESCRIPTION,MS.AMOUNT_PAID,MS.AMOUNT,MS.PAY,MS.EXPIRES,' +
          'MS.PAID,MS.SECTION,MS.EDIT_DATE,MS.RECEIPT from M_SECTIONS MS, SECTIONS S, MEMBERS M ' +
          'WHERE M.MEMBER in ( %s ) AND M.MEMBER = MS.MEMBER AND MS.SECTION = S.SECTION' +
          ' and MS.EXPIRES is not NULL order by M.MEMBER';
     // subs and Sections - do not get the Member if the expires date is NULL}
const SQL1 = 'where M.MEMBER in ( %s ) AND S.SUBSCRIPTION = MS.SUBSCRIPTION and M.MEMBER ' +
             '= MS.MEMBER and M.EXPIRES is not NULL order by M.MEMBER,s.SUB_GROUPS';
      SQL2 = 'select MEMBER,FULL_NAME,EXPIRES,JOINING_FEE,RECEIPT,PAY ' +
             'from MEMBERS where MEMBER in ( %s ) and JOINING_FEE > %f order by MEMBER';
      SQL3 = 'WHERE M.MEMBER in ( %s ) AND M.MEMBER = MS.MEMBER AND MS.SECTION = ' +
             'S.SECTION and MS.EXPIRES is not NULL order by M.MEMBER';
begin
     // set to one payment
     dbePartPay.Text := '1';
     FPartPayUserModified := False;
     Memberstr := '';
     for i := 0 to MemberList.Count -1 do begin
        Memberstr := Memberstr + MemberList[i] + ',';
     end;
     delete(Memberstr,length(Memberstr),1); // remove last comma
     if MemberStr = '' then exit;
     // get the latest points for these members
     CloseOpenWithSQL( dm.ibdsPayDet,format(SubsSQL + SQL1,[Memberstr]));
     // Howick Quick Fix , dont want joining fees coming thru into payments but want it left as a record
     // in Maintanance
     if (POS('Howick',CompName) > 0) then begin
        CloseOpenWithSQL( dm.ibdsPayJoin,format(SQL2,[Memberstr,99999.00]));
     end else begin
        CloseOpenWithSQL( dm.ibdsPayJoin,format(SQL2,[Memberstr,0.00]));
     end;
     

     CloseOpenWithSQL( dm.ibdsPayMemSec,format(SectionsSQL + SQL3,[Memberstr]));
     CalcSubPaySetup;
     CalcTotal;
     // only 1 record - may not work for many subs - watch refiring
      if not FDoneMembersEditBrief then begin
         for i := 0 to CheckedFieldsList.Count -1 do begin
           if dm.ibdsMembersBrief.FieldByName(CheckedFieldsList[i]).IsNull then begin
              menuMemberBriefClick(menuMemberBrief);
              break;
           end;
         end;
      end;
      FDoneMembersEditBrief := False;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuClearSubsClick(Sender: TObject);
begin
     ClearPayFields;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgJoinPayCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;  ABrush: TBrush);
begin
     if not active then exit;
     if (dm.ibdsPayJoin.FieldByName('PAY').AsFloat > 0.00) or
        (dm.ibdsPayJoin.FieldByName('PAY').AsFloat < 0.00) then AFont.Color := PayColor;
     if (Field.FieldName = 'PAY') or
        (Field.FieldName = 'AMOUNT') then
        ABrush.Color := Global.fcGridEdit;    
     //else AFont.Color := clBlack;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgPaySecMemCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
begin
     if not active then exit;
     if (dm.ibdsPayMemSec.FieldByName('PAY').AsFloat > 0.00 ) or
        (dm.ibdsPayMemSec.FieldByName('PAY').AsFloat < 0.00 ) then AFont.Color := PayColor;
     if (Field.FieldName = 'PAY') or
        (Field.FieldName = 'AMOUNT') then 
        ABrush.Color := Global.fcGridEdit;
     //else AFont.Color := clBlack;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgSubPayCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
// if amount_due > 0
begin
     if not active then exit;
     if (dm.ibdsPayDet.FieldByName('PAY').AsFloat > 0.00 ) or
        (dm.ibdsPayDet.FieldByName('PAY').AsFloat < 0.00 ) then AFont.Color := PayColor;
     if (Field.FieldName = 'PAY') or
        (Field.FieldName = 'AMOUNT') or
        (Field.FieldName = 'PRINT_CARD') or
        (Field.FieldName = 'PRINT_RECEIPT') then 
        ABrush.Color := Global.fcGridEdit;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.CalcTotal;
var TempTotal : extended;                
    Value : Real;
    i : shortint;
//const SQL1 = 'select PAYMENT_DISC from members where MAILING_LIST_CODE = %d';
{    TempTotal := FJoiningFeeDue + FSubsDue;
               -  total is truncated 73.175 = 73.17
               - PointsAvailable if checked -
                   >= TempTotal - then TempTotal is 0.00,points are worked out
                   < TempTotal - points are worked out
     Got to pass thru this Proc at least once 
     
 NB Points are stored as a dollar value 73.17 dollars are 7317 points
 
     }
begin
     TempTotal := 0.00;
     Value     := 0.00;
     FPoints   := 0.00; 
     FPointsUsed := 0.00;
     dbePoints.Hint := '';
     dbeTotal.Hint  := '';
     CalcJoinPay;
     CalcSubPay;
     // worked out at CalcSubPaySetup
      // part Pay up to 3 months - stored in FKeepMailingListCode (PAYMENT_DAY)
     i := StrToInt(dbePartPay.Text);
     if FInvoiced then begin
        FDiscount := RemoveSignSpaceConvert(dbeDiscount.Text);
     end else begin   
        if FDiscount <> 0.00 then
           FDiscount := FDiscount / i;
        if FSubsDue <> 0.00 then begin
           i := trunc(FAmountPaid / FSubsDue);
           if (FDiscount <> 0.00) and (i <> 0) then
              FDiscount := FDiscount / i;
        end;
     end;
     dbeJoiningFeeDue.Text := FloatToStrF( FJoiningFeeDue,ffcurrency,7,2);
     dbeSectionDue.Text    := FloatToStrF( FSectionDue,ffcurrency,7,2);
     dbeSubsDue.Text       := FloatToStrF( FSubsDue,ffcurrency,7,2);
     dbeTotal.Text         := FloatToStrF( 0.00,ffcurrency,7,2);
     dbeDiscount.Text      := FloatToStrF( FDiscount,ffcurrency,7,2);
     // only if a value to work from
     if (FJoiningFeeDue <> 0.00) or (FSubsDue <> 0.00) or (FSectionDue <> 0.00) then begin
         TempTotal := FJoiningFeeDue + FSubsDue + FSectionDue - FDiscount;
         dbePoints.Hint := Format('Points dollar value = $%f',[dm.ibdsPayMembers.FieldByName('POINTS_AVAILABLE').AsFloat]);
         FPoints := RemoveSignSpaceConvert(dbePoints.Text);
         if FPoints = -9999999.00 then
            raise Exception.Create('Points entered are not correct');
         FPointsUsed := 0;
         if FPoints >= TempTotal then begin
            FPointsUsed := TempTotal;
            TempTotal   := 0.00;
         end else begin
            FPointsUsed := FPoints;
            TempTotal   := TempTotal - FPoints;
         end;
         dbeTotal.Hint := Format('Points Used = %f',[FPointsUsed]);
         dbeTotal.Text   := FloatToStrF( TempTotal ,ffcurrency,7,2); // 73.17
         // hold for comparison between total payment and what the user keys in
         FHoldTotalLessPoints := TempTotal;
         dbeTotal.Modified := False;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dblcPaidByKeyPress(Sender: TObject;
  var Key: Char);
begin
     if key = #13 then begin
       dblcSurnameDropDown(Sender);
       if dm.qryMembersPAY.EOF then begin
          dblcPaidBy.Clear;
          dbePoints.Clear;
          dbeFullName.Clear;
          showmessage(Format('%s %s',[dblcSurname.Text,NotAValidMemMsg]));
       end else begin
          dm.ibdsPayMembers.Close;
          dm.ibdsPayMembers.Params.ByName('MEMBER').AsInteger := dm.qryMembersPAY.FieldByName('MEMBER').AsInteger;
          dm.ibdsPayMembers.Open;
          dblcPaidBy.Text := dm.ibdsPAYMembers.FieldByName('MEMBER').AsString;
          SelectNext(ActiveControl as TWinControl,True,True);
          key := #0;
       end;
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dblcPaidByCloseUp(Sender: TObject; LookupTable,
  FillTable: TDataSet; modified: Boolean);
begin
     if dblcPaidBy.modified then begin
        dm.ibdsPayMembers.Close;
        dm.ibdsPayMembers.Params.ByName('MEMBER').AsInteger := dm.qryMembersPAY.FieldByName('MEMBER').AsInteger;
        dm.ibdsPayMembers.Open;
        dblcPaidBy.Text := dm.ibdsPAYMembers.FieldByName('MEMBER').AsString;
     end;
     SelectNext(ActiveControl as TWinControl,True,True);
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.FillGrids;
//const SQL1 = 'select M.MEMBER,M.FULL_NAME,M.POINTS_AVAILABLE,M.JOINING_FEE,s.SUB_GROUPS,' +
{const SQL1 = 'select M.MEMBER,M.FULL_NAME,M.JOINING_FEE,s.SUB_GROUPS,' +
          'M.AUTO_PAYMENT,m.PAYMENT_PERIOD,m.MAILING_LIST_CODE,s.PRO_RATA,s.DISCOUNTABLE,m.SUB_PAYMENT,'+
          'M.DISCOUNT,M.EXPIRES,MS.SUBSCRIPTION,MS.AMOUNT_PAID,MS.AMOUNT,M.SUB_PAID,' +
          'ms.PRINT_CARD,ms.PRINT_RECEIPT,ms.INVOICED,S.DESCRIPTION,MS.EDIT_DATE,' +
          'MS.RECEIPT,MS.PAID,MS.PAY from MEMBERS M, SUBSCRIPTIONS S,M_SUBS MS ' +
          'where M.MEMBER = -1';
       SQL2 = 'select MEMBER,FULL_NAME,EXPIRES,JOINING_FEE,RECEIPT,PAY from MEMBERS where MEMBER = -1';
       SQL3 = 'Select M.MEMBER,M.FULL_NAME,S.DESCRIPTION,MS.AMOUNT_PAID,MS.AMOUNT,MS.PAY,MS.EXPIRES,' +
              'MS.PAID,MS.SECTION,MS.EDIT_DATE,MS.RECEIPT from M_SECTIONS MS, SECTIONS S, MEMBERS M WHERE M.MEMBER = -1';
       SQL4 = 'select SURNAME,MEMBER,FULL_NAME,FIRST_NAME,ADDRESS,MAILING_LIST_CODE from MEMBERS where MEMBER = -1';
begin;
     // set to one payment
     dbePartPay.Text := '1';
     FPartPayUserModified := False;
     CloseOpenWithSQL( dm.ibdsPayDet,SQL1);
     CloseOpenWithSQL( dm.ibdsPayJoin,SQL2);
     CloseOpenWithSQL( dm.ibdsPayMemSec,SQL3);
     with dm.qryMembersPay do begin
        Close;
        SQL.Text := SQL4;
        Open;
     end;
     dblcPaidBy.Clear;
     dblcPaidBy.Text := '';}
const  SQL1 = 'where MEMBER = -1';
       SQL2 = 'select MEMBER,FULL_NAME,EXPIRES,JOINING_FEE,RECEIPT,PAY from MEMBERS where MEMBER = -1';
begin;
     // set to one payment
     dbePartPay.Text := '1';
     FPartPayUserModified := False;
     CloseOpenWithSQL( dm.ibdsPayDet,SubsSQL + SQL1);
     CloseOpenWithSQL( dm.ibdsPayJoin,SQL2);
     CloseOpenWithSQL( dm.ibdsPayMemSec,SectionsSQL + SQL1);
     with dm.qryMembersPay do begin
        Close;
        SQL.Text := DropDownSQL + SQL1;
        Open;
     end;
     dblcPaidBy.Clear;
     dblcPaidBy.Text := '';
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.CloseOpenWithSQL( theDAtaSet : TIBDataSet; SQL1 : string );
begin
     with theDAtaSet do begin              
        close;
        SelectSQL.Text := SQL1;
        Open;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.CalcSubPaySetup;
const
   SubsName = 'SUBSCRIPTION';
   SectionsName = 'SECTION';
   SQL1 = 'select PAYMENT_DISC,PAYMENT_DAY from members where MEMBER = %d';
var i,SubPaid,PartPay : shortint;
    Amount,ProRataAmount,AmountPaid,PAY : extended;
    varTemp : string[1];
    Member  : LongInt;
    PaidThisYear : Boolean;
begin
     Amount := 0.00;
     AmountPaid := 0.00;
     PAY := 0.00;
     with dm.ibdsPayJoin do begin
         DisableControls;
         first;
         while not EOF do begin
            edit;
            if FieldByName('JOINING_FEE').AsFloat > 0.00 then
               FieldByName('PAY').AsFloat := FieldByName('JOINING_FEE').AsFloat
            else
               FieldByName('PAY').AsFloat := 0.00;
            post;
            next;
         end;
         EnableControls;
     end;
     with dm.ibdsPayDet do begin                        
         DisableControls;
         first;
         Member := FieldByName('MEMBER').AsInteger;
         FInvoiced := False;
         try
            InSaveMode := True;
            dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[FKeepMailingListCode]));
            if FPartPayUserModified then begin
               PartPay := StrToIntDef(dbePartPay.Text,1);
            end else begin   
               PartPay := dm.sqlTransPay.FieldByName('PAYMENT_DAY').AsInteger;
               dbePartPay.Text := IntToStr(PartPay);
               if (dbePartPay.Text = '0') or  (dbePartPay.Text = '') then
                  dbePartPay.Text := '1';
            end;      
            while not EOF do begin
               // if they want to print a card or receipt
               edit;
               // if it a late fee Subscription then dont add a Card
               if FieldByName('SPARE_CHAR').AsString = varYes then
                  FieldByName('PRINT_CARD').AsString     := varNo
               else
                  FieldByName('PRINT_CARD').AsString  := CardDefault;
               FieldByName('PRINT_RECEIPT').AsString  := ReceiptDefault;
               post;
               Amount     := FieldByName('AMOUNT').AsFloat;
               AmountPaid := FieldByName('AMOUNT_PAID').AsFloat;
               // try to work out if they have paid this year
               if Global.FixedExpirySystem and (FieldByName('EXPIRES').AsDateTime > Global.MemberExpiry) then 
                  PaidThisYear := True
               else   
                  PaidThisYear := False;
               { **************************************************************************** }
               // Invoiced - leave totals intact
               if FieldByName('INVOICED').AsString = varYes then begin
                  FInvoiced := True;
                  // get the Discount Field
                  if dm.sqlTransPay.FieldByName('PAYMENT_DISC').AsFloat > 0.00 then begin
                     dbeDiscount.Text := FloatToStrF( dm.sqlTransPay.FieldByName('PAYMENT_DISC').AsFloat,ffCurrency,7,2);
                  end;
                  next;
                  continue;
               { **************************************************************************** }
               // after the expires date - restart the payment
               // some clubs pay alot before the expiry date
               end else if (FieldByName('EXPIRES').AsDateTime <= FGoodDate) or PayBeforeExpires then begin
                   // PayBeforeExpires and Paid this year
                   if PaidThisYear then begin
                      Pay  := Amount - AmountPaid;
                   end else begin   
                      Amount     := GetLatestFee(SubsTableName,SubsName,FieldByName('AUTO_PAYMENT').AsString,FieldByName('SUBSCRIPTION').AsInteger);
                      AmountPaid := 0.00;
                      Pay        := Amount;
                   end;   
               // Before the expires date - they could be part or fully paid
               end else begin  
                    Pay  := Amount - AmountPaid;
               end;
               // Pro Rata
               if (FieldByName('PRO_RATA').AsString = varYes ) then begin
                  ProRataAmount  := Pay / 12;
                  Pay := StrToInt(dbeProRata.Text) * ProRataAmount;
                  Amount := Pay;
               end;          
               // Part Pay - only should apply to Members who are renewing their subs
               if PartPay > 1 then begin
                  Pay := Amount / PartPay;
               end;
               // Auto Payment   
               if (FieldByName('AUTO_PAYMENT').AsString = varYes) and
                  ( FieldByName('PAYMENT_PERIOD').AsInteger > 0 ) then begin
                  Pay := Amount / FieldByName('PAYMENT_PERIOD').AsInteger;
               end;                                                                  
               edit;
               FieldByName('PAY').AsFloat     := Pay;            // To Pay
               FieldByName('AMOUNT').AsFloat  := Amount;         // Fee
               FieldByName('AMOUNT_PAID').AsFloat  := AmountPaid;// Total Paid so far
               post;
               Next;
            end;   
            EnableControls;
               
               
                                                                              
              { end else if (FieldByName('EXPIRES').AsDateTime <= FGoodDate) or
                   PayBeforeExpires then begin
                   // after the expires date - restart the payment
                   Amount := GetLatestFee(SubsTableName,SubsName,FieldByName('AUTO_PAYMENT').AsString,FieldByName('SUBSCRIPTION').AsInteger);
                 // ProRata Payments  
                  // Amount and pay will be the same ie will be financial OK
                  //if (FieldByName('PRO_RATA').AsString = 'Y') and (FieldByName('SUB_PAYMENT').AsString = 'N') then begin
                  if (FieldByName('PRO_RATA').AsString = varYes ) then begin
                     ProRataAmount  := Amount / 12;
                     Amount := StrToInt(dbeProRata.Text) * ProRataAmount;
                  end;
                  edit;
                  FieldByName('AMOUNT').AsFloat      := Amount;
                  FieldByName('AMOUNT_PAID').AsFloat := 0.00;
                  // part Pay up to 3 months  -
                  // get the parent code PAYMENT_DAY
                  if PartPay > 1 then begin
                     Amount := Amount / PartPay;
                  end;   
                  FieldByName('PAY').AsFloat         := Amount;
                  // Auto Payments
                  if (FieldByName('AUTO_PAYMENT').AsString = varYes) and
                     ( FieldByName('PAYMENT_PERIOD').AsInteger > 0 ) then begin
                     //FieldByName('PAY').AsFloat      :=  FieldByName('PAYMENT_PERIOD').AsInteger / Amount;
                     FieldByName('PAY').AsFloat      :=  Amount / FieldByName('PAYMENT_PERIOD').AsInteger;
                  end; 
                  post;
               end else begin  
      { **************************************************************************** }         
      { **************************************************************************** }         
            {   // not past the Expires date
                  Amount     := FieldByName('AMOUNT').AsFloat;
                  AmountPaid := FieldByName('AMOUNT_PAID').AsFloat;
                  edit;
                  // new payments - year is ahead but they have a balance to pay
                  if AmountPaid = 0.00  then begin
                     // Get the latest fee if a new sub or in case of Pauanui then
                     // they have rolled before the Expires Date
                     Amount := GetLatestFee(SubsTableName,SubsName,FieldByName('AUTO_PAYMENT').AsString,FieldByName('SUBSCRIPTION').AsInteger);
                     FieldByName('AMOUNT').AsFloat := Amount;
                     FieldByName('PAY').AsFloat    := Amount;
                     // ProRata Payments  
                     // Amount and pay will be the same ie will be financial OK
                     if (FieldByName('PRO_RATA').AsString = 'Y') then begin
                        // need latest fee other wise each time thru the loop
                        // it wont work - ie amount will become less and less
                        //Amount := GetLatestFee(SubsTableName,SubsName,FieldByName('AUTO_PAYMENT').AsString,FieldByName('SUBSCRIPTION').AsInteger);
                        // can work once only thru the loop
                        ProRataAmount  := Amount / 12;
                        FieldByName('AMOUNT').AsFloat := StrToInt(dbeProRata.Text) * ProRataAmount;
                        FieldByName('PAY').AsFloat    := FieldByName('AMOUNT').AsFloat;
                     end; 
                  //end else if AmountPaid < Amount then    change here
                  // part payments - year is ahead but they have a balance to pay
                  end else if (AmountPaid > 0.00) and (AmountPaid < Amount) then begin
                     //  and   (PartPay > 1) 
                     FieldByName('PAY').AsFloat  := Amount - AmountPaid;
                     // part Payment 2nd or 3rd months
                     // get the parent code PAYMENT_DAY
                     dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[FKeepMailingListCode]));
                     if PartPay > 1 then begin
                        Amount := Amount / PartPay;
                        FieldByName('PAY').AsFloat := Amount;
                     end;   
                  end else
                     FieldByName('PAY').AsFloat  := 0.00;
                  post; }
            // get the last one after EOF
            //FDiscountMember := WorkOutDiscount(FDiscountMember,DiscountableCount);
         except
            InSaveMode := False;
            ClearPayFields; // does rollback in here
            sm( 'There are users that are using these records' + #13#10 + RolledBackPaymentMsg);
         end;
     end;
     with dm.ibdsPayMemSec do begin
         DisableControls;
         first;
         // if EXPIRES <= today (either 0,1, or 2 ) then alter the
         // 0 = year behind 1 = year ahead  2 = Year/Years ahead
         // AMOUNT_PAID/AMOUNT_DUE for the next payment
         // part paid =1 - EXPIRES is put forward 1 year
         try
            InSaveMode := True;
            while not EOF do begin
               // try to work out if they have paid this year
               //if Global.FixedExpirySystem and (FieldByName('EXPIRES').AsDateTime > Global.MemberExpiry) then 
               ///if FieldByName('EXPIRES').AsDateTime > FGoodDate then 
               //   PaidThisYear := True
               //else   
               //   PaidThisYear := False;

                  
               // passed the expires date - restart the Payment
               if FieldByName('EXPIRES').AsDateTime <= FGoodDate  then begin
                  Amount := GetLatestFee(SectionsTableName,SectionsName,'N',FieldByName('SECTION').AsInteger);
                  edit;
                  FieldByName('AMOUNT_PAID').AsFloat := 0.00;
                  FieldByName('AMOUNT').AsFloat      := Amount;
                  FieldByName('PAY').AsFloat         := Amount;
                  if FieldByName('MIN_FEE').AsFloat <> 0.00 then  
                     FieldByName('PAY').AsFloat  := FieldByName('MIN_FEE').AsFloat;
                  post;
               // not past the Expires date - could be fully or part paid
               end else begin  
                  Amount     := FieldByName('AMOUNT').AsFloat;
                  AmountPaid := FieldByName('AMOUNT_PAID').AsFloat;
                  edit;
                  // part payments - year is ahead but they have a balance to pay
                  if (AmountPaid > 0.00) and (AmountPaid < Amount) then begin 
                     FieldByName('PAY').AsFloat  := Amount - AmountPaid;
                     //assume mininmum fee for part payments
                     if FieldByName('MIN_FEE').AsFloat <> 0.00 then begin 
                        FieldByName('PAY').AsFloat  := FieldByName('MIN_FEE').AsFloat;
                     end;   
                  end else begin
                     FieldByName('PAY').AsFloat  := 0.00;
                  end;   
                  post;
               end;
               // Pay Before Expires is left on and Expires not equals the Section Expires date
               // assume they have not paid or the have altered the Expires date near to payment time
               if PayBeforeExpires and (FieldByName('EXPIRES').AsDateTime <> FieldByName('SECTION_EXPIRES').AsDateTime) then begin
                  Amount := GetLatestFee(SectionsTableName,SectionsName,'N',FieldByName('SECTION').AsInteger);
                  edit;
                  FieldByName('AMOUNT_PAID').AsFloat := 0.00;
                  FieldByName('AMOUNT').AsFloat      := Amount;
                  FieldByName('PAY').AsFloat         := Amount;
                  if FieldByName('MIN_FEE').AsFloat <> 0.00 then  
                     FieldByName('PAY').AsFloat  := FieldByName('MIN_FEE').AsFloat;
                  post;
               end;
               
               // passed the expires date 
               {if (FieldByName('EXPIRES').AsDateTime <= FGoodDate) or PayBeforeExpires then begin
                  if PaidThisYear then begin
                     Amount     := FieldByName('AMOUNT').AsFloat;
                     AmountPaid := FieldByName('AMOUNT_PAID').AsFloat;
                     edit;
                     // part payments - year is ahead but they have a balance to pay
                     if (AmountPaid > 0.00) and (AmountPaid < Amount) then
                        FieldByName('PAY').AsFloat  := Amount - AmountPaid
                     else 
                        FieldByName('PAY').AsFloat  := 0.00;
                     if FieldByName('MIN_FEE').AsFloat <> 0.00 then  
                        FieldByName('PAY').AsFloat  := FieldByName('MIN_FEE').AsFloat;
                     post;
                  end else begin
                     //  passed the expires date - restart the payment  
                     Amount := GetLatestFee(SectionsTableName,SectionsName,'N',FieldByName('SECTION').AsInteger);
                     edit;
                     FieldByName('AMOUNT_PAID').AsFloat := 0.00;
                     FieldByName('AMOUNT').AsFloat      := Amount;
                     FieldByName('PAY').AsFloat         := Amount;
                     if FieldByName('MIN_FEE').AsFloat <> 0.00 then  
                        FieldByName('PAY').AsFloat  := FieldByName('MIN_FEE').AsFloat;
                     post;
                  end;   
               end else begin  // not past the Expires date
                  Amount     := FieldByName('AMOUNT').AsFloat;
                  AmountPaid := FieldByName('AMOUNT_PAID').AsFloat;
                  edit;
                  // part payments - year is ahead but they have a balance to pay
                  if (AmountPaid > 0.00) and (AmountPaid < Amount) then begin 
                     FieldByName('PAY').AsFloat  := Amount - AmountPaid;
                     //assume mininmum fee for part payments
                     if FieldByName('MIN_FEE').AsFloat <> 0.00 then begin 
                        FieldByName('PAY').AsFloat  := FieldByName('MIN_FEE').AsFloat;
                     end;   
                  end else begin
                     FieldByName('PAY').AsFloat  := 0.00;
                  end;   
                  post;
               end;}
               
               Next;
            end;
            InSaveMode := False;
            EnableControls;
         except
            InSaveMode := False;
            ClearPayFields; // does rollback in here
            sm( 'There are users that are using these records' + #13#10 + RolledBackPaymentMsg);
         end;
     end;
end;
{ **************************************************************************** }
{function TfrmMembersPAY.WorkOutDiscount( DiscountMember : Extended ;DiscountableCount : ShortInt ): Extended;
begin
     case DiscountableCount of
        2 :     Result := DiscountMember + Global.Discount2;
        3..10 : Result := DiscountMember + Global.Discount3;
     else
        Result := DiscountMember;
     end;   
end;
{ **************************************************************************** }
function TfrmMembersPAY.GetLatestFee(const tblSubs,SubsName,AutoPayment : shortstring ; SubsInt : Integer) : Extended;
const SQL1 = 'SELECT FEE,AP_PREMIUM FROM %s where %s = %d';
      SQL2 = 'SELECT FEE from %s where %s = %d';
// if the Member is Auto Payment = Y then add on the Premium
begin                 
     Result := 0.00;
     // Subscriptions - AP_PREMIUM is not in SECTIONS
     if tblSubs = SubsTableName then begin
        if dm.sqlExecQueryCheckEOF(dm.sqlTransPay,format(SQL1,[tblSubs,SubsName,SubsInt])) then begin
           if AutoPayment = 'Y' then
              result := dm.sqlTransPay.FieldByName('FEE').AsFloat + dm.sqlTransPay.FieldByName('AP_PREMIUM').AsFloat
           else   
              result := dm.sqlTransPay.FieldByName('FEE').AsFloat;
        end;   
     end else begin   
        if dm.sqlExecQueryCheckEOF(dm.sqlTransPay,format(SQL2,[tblSubs,SubsName,SubsInt])) then 
           result := dm.sqlTransPay.FieldByName('FEE').AsFloat;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.CalcSubPay;
var v : variant;
    PAY : extended;
label DoSections;    
begin
     if not active then exit;
     FSubsDue := 0.00; FAmountPaid := 0.00;
     if dm.ibdsPayDet.RecordCount = 0 then Goto DoSections;
     with dm.ibdsPayDet do begin
         v :=  FieldByName('MEMBER').AsInteger;
         DisableControls;
         first;
         while not EOF do begin
             PAY := FieldByName('PAY').AsFloat;
             if PAY < 0.00 then begin // credit
                PAY := abs(PAY);
                if PAY > FieldByName('AMOUNT_PAID').AsFloat then begin
                   if not IfMsgDlg('Credit Payment is greater than Amount Paid, Continue? ') then begin
                      Locate('MEMBER',v,[]);  // reposition
                      EnableControls;
                      exit;
                   end;   
                end;
             end;
             if not FieldByName('PAY').IsNull then
                FSubsDue := FSubsDue + FieldByName('PAY').AsFloat;
             FAmountPaid := FAmountPaid + FieldByName('AMOUNT_PAID').AsFloat;
             next;
         end;
         Locate('MEMBER',v,[]);  // reposition
         EnableControls;
     end;
     DoSections:;
     FSectionDue := 0.00;
     if dm.ibdsPayMemSec.RecordCount = 0 then exit;
     with dm.ibdsPayMemSec do begin
         v :=  FieldByName('MEMBER').AsInteger;
         DisableControls;
         first;
         while not EOF do begin
             PAY := FieldByName('PAY').AsFloat;
             if PAY < 0.00 then begin // credit
                PAY := abs(PAY);
                if PAY > FieldByName('AMOUNT_PAID').AsFloat then begin
                   if not IfMsgDlg('Credit Payment is greater than Amount Paid, Continue?') then begin
                      Locate('MEMBER',v,[]);  // reposition
                      EnableControls;
                      exit;
                   end;   
                end;
             end;
             FSectionDue := FSectionDue + FieldByName('PAY').AsFloat;
             next;
         end;
         Locate('MEMBER',v,[]);  // reposition
         EnableControls;
     end;
end;     
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgSubPayKeyPress(Sender: TObject; var Key: Char);
begin
     if key = #13 then begin
        dbgSubPayExit(Sender); // posts
        CalcTotal;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.CalcJoinPay;
var v : variant;
begin
     if not active then exit;
     FJoiningFeeDue  := 0.00;
     if dm.ibdsPayJoin.RecordCount = 0 then exit;
     with dm.ibdsPayJoin do begin
         v :=  FieldByName('MEMBER').AsInteger;
         DisableControls;
         first;
         while not EOF do begin
            FJoiningFeeDue := FJoiningFeeDue + FieldByName('PAY').AsFloat;
            Next;
         end;
         Locate('MEMBER',v,[]);  // reposition
         EnableControls;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.cbUsePointsClick(Sender: TObject);
begin               
     {if RemoveSignSpaceConvert(dbeTotal.Text) = 0.00 then begin
        cbUsePoints.State := cbUnchecked; // no points to use
        exit;
     end;
     if dm.ibdsPayMembers.FieldByName('POINTS_AVAILABLE').AsFloat = 0.00 then begin
        cbUsePoints.State := cbUnchecked; // no points to use
        exit;
     end;
     if (FSubsDue + FJoiningFeeDue + FSectionDue) < 0.00 then begin
        cbUsePoints.State := cbUnchecked; // no points to use
        sm('Cannot use points on a Credit Payment');
        exit;
     end;
     CalcTotal;                   }
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgPaySecMemFieldChanged(Sender: TObject;
  Field: TField);
begin
     if InSaveMode then exit;
     CalcTotal;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgSubPayFieldChanged(Sender: TObject; Field: TField);
// 3  grids look here Sub/Section and Joining Fee  
begin
     if InSaveMode then exit;
     CalcTotal;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgSubPayExit(Sender: TObject);
begin
     if dm.ibdsPayDet.State = dsEdit    then dm.ibdsPayDet.Post;
     if dm.ibdsPayMemSec.State = dsEdit then dm.ibdsPayMemSec.Post;
     if dm.ibdsPayJoin.State = dsEdit   then dm.ibdsPayJoin.Post;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgSubPayEnter(Sender: TObject);
begin
      if not Active then exit;
      if (( Sender as TwwDBGrid).Name = dbgSubPay.Name) and 
         (dm.ibdsPayDet.RecordCount > 0 ) then
         dbgSubPay.SetActiveField('PAY');
      if (( Sender as TwwDBGrid).Name = dbgPaySecMem.Name) and
         (dm.ibdsPaySecMem.RecordCount > 0 ) then
         dbgPaySecMem.SetActiveField('PAY');
      if (( Sender as TwwDBGrid).Name = dbgJoinPay.Name) and
         (dm.ibdsPayJoin.RecordCount > 0 ) then
         dbgJoinPay.SetActiveField('PAY');   
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbeFullNameKeyPress(Sender: TObject;  var Key: Char);
begin
     if key = #13 then begin
        if TwwDBEdit(Sender).Name = dbeDiscount.Name then begin
           dbeDiscountExit(Sender);
           key := #0;
        end;
        if (TwwDBEdit(Sender).NAme = dbeProRata.Name) or
           (TwwDBEdit(Sender).NAme = dbePartPay.Name) then begin
           // recalc the pro rata or part pay payment
           FPartPayUserModified := True;
           CalcSubPaySetup;
           CalcTotal;
        end;
        if TwwDBEdit(Sender).Name = dbePoints.Name then begin
           SelectNext(ActiveControl as TWinControl,True,True);
           CalcPoints;
           //exit;
           //dbePointsExit(Sender);
        end;
     end;
     if key = #13 then begin
        SelectNext(ActiveControl as TWinControl,True,True);
        key := #0;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dblcPaidByKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     if MemberList.Count = 0 then begin
        dblcPAidBy.Clear;
        sm('Must select a Member first');
        dblcSurname.SetFocus;
     end;
end;
{ **************************************************************************** }
function TfrmMembersPAY.GetTenders : Boolean ;
var  AmountExt : extended;
begin
     try
        result := false;
        frmTenders := TfrmTenders.Create(nil);
        // unrounded with no points
        frmTenders.CashAmount := FTotalLessPoints;
        if (frmTenders.ShowModal = mrOK) then begin
           // Post the Payments into PAYMENT_TEND if mrOK
           with dm.ibdsTenders do begin
              First;
              while not EOF do begin
                 AmountExt  := Global.RemoveSignSpaceConvert(FieldByName('AMOUNT').AsString);
                 if AmountExt = -9999999.00 then begin
                    ClearPayFields; // does a rollback here
                    InSaveMode := False;
                    sm( 'There has been an error converting the tender amount' + #13#10 + RolledBackPaymentMsg);
                    exit;
                 end;
                 if AmountExt <> 0.00 then
                    dm.sqlExecQuery(dm.sqlTransPay,format(InsertPaymentsTend,[FReceiptNumber,FieldByName('TENDER_TYPE').AsString,AmountExt,FieldByName('DETAILS').AsString, FieldByName('EX_RATE').AsFloat]));
                 next;
              end;
           end;
        end else begin
           // cancelled
           exit;
        end;
        result := true;
     finally
        dm.ibdsTenders.Close;
        FRoundValue := FRoundValue + frmTenders.RndValue;
        frmTenders.Release;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuPaybyCashClick(Sender: TObject);
var AmountOK,PayByCash,NewMember : Boolean;
    CardListCtr : Integer;
    i : smallint;
    MemberPayments : TMemberPayments;
const  SQL1 = 'Delete from m_SUBS where MEMBER = %s and SUBSCRIPTION = %d';
       SQL2 = 'update MEMBERS set DISCOUNT = null where MAILING_LIST_CODE = %d';
       msg1 = 'There has been a conflict in updating the Payment';
begin
     if ConfirmPayment then
        if not ifMsgDlg(format('Confirm Payment for %s',[GetShortMonthDate(dbdtPayDate.DAte)])) then exit;
     // if Payment is by cash or other tenders
     if Sender is TToolButton then begin
       if TToolButton(Sender).Name = btnCash.Name  then
          PayByCash := True
       else   
          PayByCash := False;
     end else begin     
       if TMenuItem(Sender).Name = menuPaybyCash.Name then
          PayByCash := True
       else   
          PayByCash := False;
     end;  
     // if records are not posted 
     if dm.ibdsPayDet.State = dsEdit then dm.ibdsPayDet.Post;
     if dm.ibdsPayJoin.State = dsEdit then dm.ibdsPayJoin.Post;
     if dm.ibdsPayMemSec.State = dsEdit then dm.ibdsPayMemSec.Post;
     FTotalLessPoints := RemoveSignSpaceConvert(dbeTotal.Text);
     // a different Total Payment                     
     //FRoundValue :=  ( FHoldTotalLessPoints - FTotalLessPoints);
     FTotalToPayBalance := FSubsDue + FJoiningFeeDue + FSectionDue;
     // - could be a credit payment
     if FTotalToPayBalance = 0.00 then begin
        sm(NoTotalPaymentMsg);
        exit;
     end;
     if dblcPaidBy.Text = '' then begin
        sm('there is no Paid By member - please select one');
        exit;
     end;
     FReceiptNumber := MemberPayments.GetReceiptNumber(dm.sqlTransPAy);
     if FReceiptNumber = 0 then begin
        sm('Cannot get a Receipt Number');
        exit;
     end;
     InSaveMode := True;
     FDiscount      := RemoveSignSpaceConvert(dbeDiscount.Text);
     // only show if there is something to pay
     if FTotalLessPoints <> 0.00 then begin
        // Pay by Cash
        if PayByCash then begin
           dm.sqlExecQuery(dm.sqlTransPay,format(InsertPaymentsTend,[FReceiptNumber,'Cash',FTotalLessPoints,'',0.00]));
        end else begin  
           if sfPreCheckLogOn(TfrmTenders) then begin
              // paying by another tender - check for Cancellation
              if not GetTenders then begin
                 InSaveMode := False;
                 exit;
              end;
           end else begin     
              // if they cannot go into the payments screen
              ClearPayFields;
              InSaveMode := False;
              exit;
           end;
        end;   
     end else begin
        // if a points only payment and nothing to tender 
        if not IfMsgdlg('Save this Payment') then exit;
     end;
     { **************************************************************************** }
     try
        // Joining Fee in Members  
        UpdatePayJoin;
        // Subscriptions
        UpdateSubsSections( 0,dm.ibdsPayDet    ); // Subs
        // Sections
        UpdateSubsSections( 1,dm.ibdsPayMemSec ); // Sections
        // Paid By Member
        UpdatePoints;
        NewMember := False;
        // N for New Member
        if dm.ibdsPayDet.FieldByName('SUB_PAYMENT').AsString = 'N' then
           NewMember := True;
        // work out if sub paid  and update members records + expires date
        // not for Sections
        if not UpdateSubPaid then begin
           EndTransaction ( msg1 + ' - UpdateSubPaid');  // rollback
           InSaveMode := False;
           exit;
        end;
        // If Parent is $500 and other Members are 0.00 - apply Expires etc to other
        // Members  eg Corporates
        if ParentCodeList.Count > 0 then begin
           if not UpdateParentCodePays then begin
              EndTransaction ( msg1 + ' - UpdateParentCodePays');  // rollback
              InSaveMode := False;
              exit;
           end;
        end;
        // update the Payments DB with details
        UpdatePayments;
         // commit the work done - leave here for RPNYC
        CommitAndOpenTables;
         // checks to see if one is connected - from ReadPrinters
        Global.OpenCashDrawer;  
        // Remove any late payments
        if FLateSubAdded then begin
           for i := 0 to MemberList.Count -1 do 
               dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[MemberList[i],FLateSub]));
        end;
        // can fire off formactivate in this form to/from quick report  
        PrintCardsAndReceipts;  
        // dont do this if connected to the test DB
        if not inTestMode then
           if ExportSubs then ExportSubsToFile;
        // Print Members Details  - see Hornby
        if PrintMemberPayment and NewMember then   
           MemberDeleteReport(dm.ibdsPayDet.FieldByName('MEMBER').AsInteger);
        // commit the work done   
        CommitAndOpenTables; 
     except
        on E : Exception do begin
           EndTransaction ( Msg1 + #13#10 + E.Message);  // rollback
           InSaveMode := False;
           exit;
        end;   
     end;
     // clear all fields
     menuClearSubsClick(Sender);
     // menu to show expires etc 
     ShowExpiresLastPAyment;    
     Self.Caption := ScreenName;
     InSaveMode := False;
     stbStatus.Panels[0].Text := Format('Last Receipt = %d',[FReceiptNumber]);
end;
{ **************************************************************************** }
procedure TFrmMembersPAY.ExportSubsToFile;
var s : string;
    F : TextFile;
    AmtPaid,PointsUsed : extended;
    FirstRecord : Boolean;
const WinSubFile = '\WinSub.DAT';
      SQL1 = 'Select pd.PAY_TYPE,pd.CODE,pd.AMOUNT_PAID,p.POINTS_USED,pd.MEMBER ' +
             'from PAYMENTS_DET pd, PAYMENTS p where p.PAYMENT = %d and p.PAYMENT = pd.PAYMENT';
      SQL2 = 'Select pd.PAY_TYPE,pd.CODE,pd.AMOUNT_PAID,p.POINTS_USED,pd.MEMBER ' +
             'from PAYMENTS_DET pd, PAYMENTS p where p.PAYMENT = %d and p.PAYMENT = pd.PAYMENT';
             
begin
     if not CheckCreateFile( F,ArevImportFilePath + WinSubFile,'' ) then exit;
     try
        if dm.sqlExecQueryCheckEOF(dm.sqlTransPay,format(SQL1,[FReceiptNumber])) then begin
           with dm.sqlTransPay do begin
              while not EOF do begin
                 if FirstRecord then begin
                    PointsUsed := FieldByName('POINTS_USED').AsFloat;
                    FirstRecord := False;
                 end;
                 s := '';
                 case FieldByName('PAY_TYPE').AsInteger of
                    0 : s := 'SUB';
                    1 : s := 'SEC';
                    2 : s := 'JOI';
                 end;
                 // Credit Payment Points
                 AmtPaid := FieldByName('AMOUNT_PAID').AsFloat;
                 if PointsUsed >= AmtPaid then begin
                    PointsUsed := PointsUsed - AmtPaid;
                    AmtPaid := 0.00;
                 end else if PointsUsed > 0.00 then begin
                    AmtPaid := AmtPaid - PointsUsed;
                    PointsUsed := 0.00;
                 end;
                 s := s + ',' + FieldByName('CODE').AsString + ',' +
                          FormatFloat( '###.00',AmtPaid) + ',' +
                          IntToStr(FReceiptNumber) + ',' + FieldByName('MEMBER').AsString;
                 writeln(F,s);
                 next;
              end;
           end;
        end else begin
           sm(format('Receipt # %d has not been Exported to Clubmaster %s %s',[FReceiptNumber,HelpDeskMsg,HelpDesk]));
        end;
     finally
        closefile(F);
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.PrintCardsAndReceipts;
var ReceiptMemberStr,CardMemberStr,ReceiptLetterStr : String;
    Printed : Boolean;
    CardMembers : TCardMembers;
const SQL3 = 'update MEMBERS set %s = ''Y'' where MEMBER in %s';
      Print_Card    = 'PRINT_CARD';
      Print_Receipt = 'PRINT_RECEIPT';
      comma = ',';
begin
     with dm.ibdsPayDet do begin
        DisableControls;
        first;
        ReceiptMemberStr := '';
        ReceiptLetterStr := '';
        CardMemberStr := '';
        dm.tblCardDetail.Open;
        Printed := False;
        while not eof do begin
           if FieldByName('PRINT_CARD').AsString = varYes then begin
              case TPaymentCardTypes of
                 pcPrint : Global.CardPrint(FieldByName('MEMBER').AsString,'',1,dm.IBTransPay);
                 pcSave  : CardMembers.CardInsertMember( dm.sqlTransPay,FieldByName('MEMBER').AsInteger,1);
                 pcBulk  : CardMemberStr := CardMemberStr + FieldByName('MEMBER').AsString + comma;
              end;
           end;   
           if FieldByName('PRINT_RECEIPT').AsString = varYes then begin
              case TPaymentReceiptTypes of
                 prPrint : begin
                              // only print the receipt once
                              if not Printed then begin
                                 ReceiptList.Add(IntToStr(FReceiptNumber));
                                 Printer.PrinterIndex := Global.ReceiptPrinterIndex;
                                 menuPrintClick(btnPrint); // prints straight away
                                 Printer.PrinterIndex := Global.ReportPrinterIndex;
                                 Printed := True;
                              end;
                           end;
                 prSave  : if ReceiptList.IndexOf(IntToStr(FReceiptNumber)) <> -1 then
                              ReceiptList.Add(IntToStr(FReceiptNumber)); // cleared after printing
                 prBulk   : ReceiptMemberStr := ReceiptMemberStr + FieldByName('MEMBER').AsString + comma;
                 prLetter : ReceiptLetterStr := ReceiptLetterStr + FieldByName('MEMBER').AsString + comma;
              end;   
           end;
           next;
        end; 
        dm.tblCardDetail.Close;
        EnableControls;
     end;
     if CardMemberStr <> '' then begin
        CardMemberStr    := DeleteLastComma(CardMemberStr);
        dm.sqlExecQuery(dm.sqlTransPay,format(SQL3,[Print_Card,CardMemberStr]));
     end;
     if ReceiptMemberStr <> '' then begin
        ReceiptMemberStr := DeleteLastComma(ReceiptMemberStr);
        dm.sqlExecQuery(dm.sqlTransPay,format(SQL3,[Print_Receipt,ReceiptMemberStr]));
     end;
     if ReceiptLetterStr <> '' then begin
        ReceiptLetterStr := DeleteLastComma(ReceiptLetterStr);
        PrintReceiptLetter( ReceiptLetterStr );
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.PrintReceiptLetter( ReceiptLetterStr : string );
begin
     if frmMembersReport = nil then
        frmMembersReport := TfrmMembersReport.Create(Self);
     with frmMembersReport do begin
         FOwnIndex := 49; // Receipt Letter          
         frmDueLetter := TfrmDueLetter.Create(self);
         dm.qryReport.Close;
         //dm.qryReport.Transaction := dm.IBTransPAy;
         dm.ibdsLetters.Open;
         try
            with frmDueLetter do begin
              if not GetDatafromLetter( ReportTitles[FOwnIndex] ) then begin
                  sm('Receipt Letter not found or invalid');
                  exit;
                end;
                ExportFieldsList    := ListOne;  // properties
                ExportExtFieldsList := ListTwo;
                // change the printer to Receipt Printer A4 
                if not Extract(True,6,Global.ReportPrinterIndex,ReceiptLetterStr,date,date,
                                    FExportFields,ReportTitles[FOwnIndex],0,0) then begin
                     Free;
                     exit;                                   
                end;
               dm.qryReport.Open;
               qr.Print;
            end;
         finally
            dm.qryReport.Close;
            //dm.qryReport.Transaction := dm.IBTransaction;
            dm.ibdsLetters.Close;
            frmDueLetter.Free;
            //frmMembersReport.Free;
         end;
     end;
end;     
{ **************************************************************************** }
procedure TfrmMembersPAY.ShowExpiresLastPAyment;
const SQL1 = 'Select MEMBER,FULL_NAME,DESCRIPTION,EXPIRES from Payments_DET ' +
             ' where PAYMENT = %d order by MEMBER';
var  NewItem: TMenuItem;
     i  : integer;
     Member : String[9];
     Desc,FName : String[30];
     Exp : String[14];
begin
     dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[FReceiptNumber]));
     menuLastPaid.Items.Clear;
     with dm.sqlTransPay do begin
        NewItem := TMenuItem.Create(Self);
        NewItem.Caption := 'Member    ' +
                           'Receipt   ' +
                           'Expires         ' +
                           'Subscription       ';

        menuLastPaid.Items.Add(NewItem);
        while not EOF do begin
           Member := FieldByName('MEMBER').AsString;
           //FName   := FieldByName('FULL_NAME').AsString;
           Exp    := GetShortMonthDAte(FieldByName('EXPIRES').AsDateTime);
           Desc   := FieldByName('DESCRIPTION').AsString;
           NewItem := TMenuItem.Create(Self);
           NewItem.Caption := Member + '           ' + IntToStr(FReceiptNumber) + '        ' + Exp + '     ' + Desc;
           menuLastPaid.Items.Add(NewItem);
           next;
        end;
     end;
     menuLastPaid.Popup(Self.Left + 250 , Self.Top + 250);
end;
{ **************************************************************************** }
function TfrmMembersPAY.UpdatePayJoin : Boolean ;
var JoinFee ,HoldJoinFee,PAY : extended;
    SubPayTypes  : TSubPayTypes;// = (spZero,spPart,spFull);
    MemberPayments : TMemberPayments;
begin
     Result := False;
     if dm.ibdsPayJoin.RecordCount = 0 then begin
        Result := True;
        exit;
     end;
     JoinFee := 0.00;
     with dm.ibdsPayJoin do begin
        DisableControls;
        first;
        while not EOF do begin
           // could be a credit payment
           if FieldByName('PAY').AsFloat = 0.00 then begin
              next;
              continue; // no payment
           end;
           HoldJoinFee := FieldByName('JOINING_FEE').AsFloat;
           JoinFee     := FieldByName('JOINING_FEE').AsFloat;
           PAY         := FieldByName('PAY').AsFloat;
           edit;
           if PAY < 0.00 then // credit
              FieldByName('JOINING_FEE').AsFloat := JoinFee - PAY
           else
              FieldByName('JOINING_FEE').AsFloat := PAY - JoinFee;
           FieldByName('RECEIPT').AsInteger := FReceiptNumber;
           Post;
           //  fill in PAYMENT_DET
           JoinFee := FieldByName('JOINING_FEE').AsFloat; // get again
           // hard to match back - as it may have only been a part payment last time
           if JoinFee >= HoldJoinFee then
              SubPayTypes := spZero  // not paid
           else if JoinFee = 0.00
              then SubPayTypes := spFull
           else SubPayTypes := spPart;
           // - not the correct expires - hasnt yet been updated
           // see UpdateSubsPAid proc
           MemberPayments.PaymentDetailInsert( dm.sqlTransPay,FReceiptNumber,
                             dm.ibdsPayMembers.FieldByName('MEMBER').AsInteger,
                             FieldByName('MEMBER').AsInteger,0,ord(SubPayTypes),ord(TPayTypes(ptJoinFee)) ,
                             HoldJoinFee,PAY,0.00,dbdtPayDate.Date,
                             'Joining Fee','N',GetShortDate(FieldByName('EXPIRES').AsDateTime),
                             FieldByName('FULL_NAME').AsString,'' );
           next;
        end; // while
        EnableControls;
     end; // with
     Result := True;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.UpdateSubsSections( PayType : shortint; theDataSet : TIBDataSet);
var PAY,AmountPaid,Amount : extended;
    StrExpires : string[15];
    PayFull,Yrs : shortint;
    Code : Integer;
    MExpires   : TDateTime;
    PayFullText : char;
    SubPayTypes  : TSubPayTypes;// = (spZero,spPart,spFull);
    MemberPayments : TMemberPayments;
begin
     if theDataSet.RecordCount = 0 then begin
        exit;
     end;
     AmountPaid := 0.00;
     with theDataSet do begin
        DisableControls;
        first;
        while not EOF do begin
           // turn off invoiced after making a payment - only M_SUBS
           if theDataSet.Name = dm.ibdsPayDet.Name then begin
              edit;
              FieldByName('INVOICED').AsString := varNo;
              post;
           end;
           // got to make an entry into Payments_det
           if not FParentCodePays and (FieldByName('PAY').AsFloat = 0.00) then begin
              next;
              continue; // no payment
           end;   
           AmountPaid     := FieldByName('AMOUNT_PAID').AsFloat;
           Amount         := FieldByName('AMOUNT').AsFloat;
           PAY            := FieldByName('PAY').AsFloat;
           Edit;
           // credits are OK (45 + -45)
           FieldByName('AMOUNT_PAID').AsFloat  := AmountPaid + PAY;
           FieldByName('EDIT_DATE').AsDateTime := now;
           FieldByName('RECEIPT'  ).AsInteger  := FReceiptNumber;
           Post;
           //  fill in PAYMENT_DET
           AmountPaid := FieldByName('AMOUNT_PAID').AsFloat; // get again
           // NB subs done in Proc UpdateSubPaid as it is a combination of all subs
           // Sections is on a record basis
           if PayType = 1 then begin  //Sections
              MExpires := FieldByName('EXPIRES').AsDateTime;
              // must make sure MExpires is bought up to date if it is behind
              //MemberPayments.PaymentUpdateExpires(MExpires,FGoodDate);
              
              //PayFullText := 'N'; // 1 part paid gets Expires + 1 year but PAID = 'N' - see top
              SubPayTypes := MemberPayments.PaymentSubPayTypes(FieldByName('AMOUNT').AsFloat,
                                                       FieldByName('AMOUNT_PAID').AsFloat);
              
              //MExpires := MemberPayments.PaymentWorkOutExpires(Amount,AmountPaid,Pay,
              //                                   SubPayTypes,MExpires,FGoodDate,0,12);
              
              // the Section date must be manually set in the settings and put ahead
              // to the relevant year
              case SubPayTypes of
                 spFull : begin
                             PayFullText := 'Y';
                             MExpires := FieldByName('SECTION_EXPIRES').AsDateTime;
                          end;
                 spPart : begin 
                             PayFullText := 'P';
                             if MExpires < FieldByName('SECTION_EXPIRES').AsDateTime then
                                MExpires := FieldByName('SECTION_EXPIRES').AsDateTime;
                          end;  
                 spZero : PayFullText := 'N';
              end; 
               
              edit;
              strExpires := GetShortDate(MExpires);
              FieldByName('EXPIRES').AsDateTime  := MExpires;
              FieldByName('PAID').AsString       := PayFullText;
              post;
              Code := FieldByName('SECTION').AsInteger;
           end else begin
              // - for Subs - not the correct expires - updated in Proc UpdateSubPaid
              SubPayTypes := MemberPayments.PaymentSubPayTypes(FieldByName('AMOUNT').AsFloat,
                                                       FieldByName('AMOUNT_PAID').AsFloat);
              StrExpires := GetShortDate(FieldByName('EXPIRES').AsDateTime);
              Code :=  FieldByName('SUBSCRIPTION').AsInteger;
           end;
           MemberPayments.PaymentDetailInsert( dm.sqlTransPay,FReceiptNumber,
                             dm.ibdsPayMembers.FieldByName('MEMBER').AsInteger,
                             FieldByName('MEMBER').AsInteger,Code,ord(SubPayTypes),PayType,
                             Amount,PAY,0.00,dbdtPayDate.Date,
                             FieldByName('DESCRIPTION').AsString,'N',StrExpires,
                             FieldByName('FULL_NAME').AsString,'' );
           next;
        end; // while
        EnableControls;
     end; // with
end;
{ **************************************************************************** }
function TfrmMembersPAY.UpdateSubPaid : Boolean;
const SQL1 = 'SELECT EXPIRES,SUB_PAYMENT,SUB_PAID from MEMBERS WHERE MEMBER = %s';
      SQL2 = 'SELECT SUM(AMOUNT) AS AMOUNT,SUM(AMOUNT_PAID) AS AMOUNT_PAID,' +
             ' (SUM(AMOUNT) - SUM(AMOUNT_PAID)) AS DUE,SUM(PAY) as PAY FROM M_SUBS WHERE MEMBER = %s';
      SQL3 = 'SELECT MAX(TERM)as TERM from SUBSCRIPTIONS s, M_SUBS ms where ms.SUBSCRIPTION = ' +
             's.SUBSCRIPTION and ms.MEMBER = %s';
var i,SubPaid,SkipYear,SubsTerm : smallint;
    SubPayment : String[1];
    v : VAriant;
    SubPayTypes  : TSubPayTypes;// = (spZero,spPart,spFull);
    MemberPayments : TMemberPayments;
    MExpires : TDateTime;
// do for all members in MemberList and update their Sub_Paid 0 = no subs or no AMOUNT_PAID
// 1 = Part 2 = Full
// Expires date is worked out depending on how much they pay
// if SubPaid = 2 then  IS_ACTIVE is set to 'Y' straight away else
// the Rollover will Pick then up in Fixed or Revolving Sytem ( EXPIRES < now by x days)
// the Member has the Grace of Payment between Expires Date and Rollover
// before they become IS_ACTIVE = 'N' if SUB_PAID 0 or 1
begin
     Result := True;
     for i := 0 to MemberList.Count -1 do begin
        if MemberList[i] = '' then continue;
        // both Sections and subs in memberlist 
        // may be life member with Sections ie has no Subs  
        V := MemberList[i];
        with dm.ibdsPayDet do begin
           if not dm.ibdsPayDet.Locate('MEMBER', V, [loCaseInsensitive]) then begin
              // section payment
              continue;
           end else if FParentCodePays and (FKeepMailingListCode <> FieldByName('MEMBER').AsInteger) then begin
              // add in the Parent Code members whos amount is 0.00
              // the Member with the Amount > 0.00 goes thru
              ParentCodeList.Add(MemberList[i]);
              continue;
           end;
        end;          
        with dm.sqlTransPay do begin
           if dm.sqlExecQueryCheckEOF(dm.sqlTransPay,format(SQL1,[MemberList[i]])) then begin
              SubPaid       := FieldByName('SUB_PAID').AsInteger;
              SubPayment    := FieldByName('SUB_PAYMENT').AsString;
              MExpires      := FieldByName('EXPIRES').AsDateTime;
              // must make sure MExpires is bought up to date if it is behind
              MemberPayments.PaymentUpdateExpires(MExpires,FGoodDate);
           end else begin // not found
              Result := False;
              exit;
           end;   
           // get the max TERM from Subscripions for the MEMBER
           // if a Member has a 3 month and 12 month sub - they get a 12 month period
           dm.sqlExecQueryCheckEOF(dm.sqlTransPay,format(SQL3,[MemberList[i]]));
           SubsTerm := dm.sqlTransPay.FieldByName('TERM').AsInteger;
           // if the field is not filled in then default it to 12 months
           if SubsTerm = 0 then SubsTerm := 12;
           if dm.sqlExecQueryCheckEOF(dm.sqlTransPay,format(SQL2,[MemberList[i]])) then begin 
              // only for members who have paid in the transaction
              // maybe other parent codes with nothing to pay
              if FieldByName('PAY').AsFloat > 0.00 then begin
                 SubPayTypes := MemberPayments.PaymentSubPayTypes(FieldByName('AMOUNT').AsFloat,
                                                       FieldByName('AMOUNT_PAID').AsFloat);
                //New Member and 30-60 days before Expires - setting in MISC
                 if (SubPayment = 'N' ) and FMemberPreExpiry then 
                     SkipYear := SubsTerm
                     //SkipYear := 1  should be now SkipMonth
                 else
                     SkipYear := 0;
                 //Work Out Expires
                 MExpires := MemberPayments.PaymentWorkOutExpires(FieldByName('AMOUNT').AsFloat,
                                                    FieldByName('AMOUNT_PAID').AsFloat,
                                                    FieldByName('PAY').AsFloat,
                                                    SubPayTypes,MExpires,FGoodDAte,SkipYear,SubsTerm);
                 if not MemberPayments.PaymentMemberState(dm.sqlTransPay,MExpires,MemberList[i],SubPayment,
                                              FReceiptNumber,SubPaid,SubPayTypes) then exit;

              end;
           end else begin
              // no records in M_SUBS - forced to have at least one
              SubPayTypes := spZero;
              MExpires := Global.AlterExpireDateBelow(MExpires,FGoodDate);
              if not MemberPayments.PaymentMemberState(dm.sqlTransPay,MExpires,MemberList[i],SubPayment,
                                              FReceiptNumber,SubPaid,SubPayTypes) then exit;                                
           end;
           // keep Member for a Card
           //if SubPayTypes = spFull then CardList.Add(MemberList[i]);
           // watch lots of Part Payments
           //if (SubPayTypes = spPart) and (SubPaid <> ord(TSubPayTypes(spPart))) then CardList.Add(MemberList[i]);

        end;
        //stbStatus.Panels[2].Text := Format('Last Sub Expires = %s',[formatDateTime('dd mmm yyyy',MExpires)]);
        Result := True;
     end; // for
end;
{ **************************************************************************** }
function TfrmMembersPAY.UpdateParentCodePays : Boolean ;
const SQL1 = 'SELECT EXPIRES,SUB_PAID,IS_ACTIVE,INACTIVE_DATE from MEMBERS where MEMBER = %d';
      SQL2 = 'UPDATE PAYMENTS_DET SET EXPIRES = ''%s'',PAY_FULL = %d where MEMBER = %s and PAYMENT = %d';
      SQL3 = 'UPDATE MEMBERS SET SUB_PAID = %d, EXPIRES = ''%s'',IS_ACTIVE = ''%s'',' +
             'INACTIVE_DATE = ''%s'',SUB_PAYMENT = ''Y'' where MEMBER = %s'; 
var Expires,IsActive,InactiveDate : String[12];
    SubPaid : shortint;
    i : smallint;
begin      
     // get Members from the ParentCode Member
     Result := False;
     if dm.sqlExecQueryCheckEOF(dm.sqlTransPAY,format(SQL1,[FKeepMailingListCode])) then begin
        Expires      := GetShortDate(dm.sqlTransPAY.FieldByName('EXPIRES').AsDateTime);
        InactiveDate := GetShortDate(dm.sqlTransPAY.FieldByName('INACTIVE_DATE').AsDateTime);
        SubPaid      := dm.sqlTransPAY.FieldByName('SUB_PAID').AsInteger;
        IsActive     := dm.sqlTransPAY.FieldByName('IS_ACTIVE').AsString;
     end else begin
        exit;
     end;
     for i := 0 to ParentCodeList.Count -1 do begin
        // update Payments det
        dm.sqlExecQuery(dm.sqlTransPAY,format(SQL2,[Expires,SubPaid,ParentCodeList[i],FReceiptNumber]));
        // update members
        //if InactiveDate = Date1899 then 
        dm.sqlExecQuery(dm.sqlTransPAY,format(SQL3,[SubPaid,Expires,IsActive,InactiveDate,ParentCodeList[i]]));
     end;
     Result := True;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.UpdatePoints;
var PointsUsed : Integer;
    MemberPayments : TMemberPayments;
const SQL1 = 'update MEMBERS set PAYMENT_DISC = %f,PAYMENT_DAY = %s where MAILING_LIST_CODE = %d';    
begin
     // set the parent code with the Part pay Information - stored in Part Pay
     dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[RemoveSignSpaceConvert(dbeDiscount.Text),dbePartPay.Text,FKeepMailingListCode]));
     // once per PaidBy Member - update the Points
     with dm.ibdsPayMembers do begin
        if FPointsUsed <> 0.00 then begin
           PointsCreditTypes := pcSubPaidOffice;
           Global.BeforeEditPointsCreditSave(BeforeEditPointsCredit,dm.dsrPayMembers.DataSet);  
           edit;
           MemberPayments.PointsCreditUpdate(FPointsUsed,dm.dsrPayMembers.DataSet);
           post;
           // turn from dollar to integer
           PointsUsed := trunc(FPointsUsed * 100);
           // turn into negative
           PointsUsed := PointsUsed - (PointsUsed * 2);
           dm.LogExport(dm.sqlTransPay,FieldByName('MEMBER').AsInteger,PointsUsed,False);
           // Points Log
           MemberPayments.LogPointsCredit(dm.IBTransPay,format('Points Payment by Member %d',[FieldByName('MEMBER').AsInteger]),
                                          BeforeEditPointsCredit,
                                          dm.dsrPayMembers.DataSet,
                                          FPointsUsed);
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.UpdatePayments;
var s : string[15];
const SQL1 = 'UPDATE PAYMENTS SET PAY_AMOUNT = %f,POINTS_USED = %f,PAID_BY = %s,' +
        'DISCOUNT = %f,EDIT_DATE = ''%s'',ROUNDING = %f,PAY_CREDIT = 0.00,FULL_NAME = ' +
         '''%s'' where PAYMENT = %d';
begin   
     // FPointsUsed are from CalcTotal proc , FRoundValue is received back from frmTenders
     // NB the PAYMENT autoinc has already been inserted to get FReceiptNumber prior to this                                                 // fullname here is 25
     dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[FTotalLessPoints,FPointsUsed,dblcPaidBy.Text,FDiscount,GetShortDate(dbdtPayDate.Date),FRoundValue,copy(dbeFullName.Text,1,25),FReceiptNumber]));
end;
{ **************************************************************************** }
{ **************************************************************************** }
procedure TfrmMembersPAY.EndTransaction ( Msg : String );
begin
     dm.IBTransPay.RollBack;
     dm.IBTransPay.StartTransaction;
     sm( Msg + #13#10 + RolledBackPaymentMsg + #13#10 +
        'Please check the details and retry saving the Payment');
     // refresh the details
     Application.ProcessMessages;
     dbgSubPay.Update;
     dbgPaySecMem.Update;
     PopulatePayGrid;
     dm.ibdsPayMembers.Close; // paidBy
     dm.ibdsPayMembers.Open;
     InSaveMode := False;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.ClearPayFields;
begin
     dbePoints.Text := '0.00';
     dbeSectionDue.Clear;
     dbeJoiningFeeDue.Clear;
     ParentCodeList.Clear;
     MemberList.Free;
     MemberList := TStringList.create;
     dbeDiscount.Clear;
     FDiscount := 0.00;
     dbeSubsDue.Clear;
     dbeTotal.Clear;
     dbeProRata.Text := '12';
     dm.ibdsPayMembers.Close;
     if visible then
       dblcSurname.SetFocus;
     if dm.IBTransPay.InTransaction then begin
        dm.IBTransPay.RollBack;
        dm.IBTransPay.StartTransaction;
     end;
     FillGrids;
     //FillGridsSections;
     //cbUsePoints.State := cbUnchecked;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.PageControlChange(Sender: TObject);
const msg1 = 'You must Save or Clear Payments first';
begin
     // cannot start a new payment if there are some unsaved
     // commit on IBTransPay
     case PageControl.ActivePageIndex of
        0 : begin
               if dm.ibdsPAySections.RecordCount > 0 then begin 
                  sm(msg1);
                  PageControl.ActivePage := tsSectionsPay;
                  SectionsPayButtonSetup;
                  exit;
               end;   
               if dm.ibdsBulkCardRec.RecordCount > 0 then begin 
                  sm(msg1);
                  PageControl.ActivePage := tsBulkCardReceipt;
                  ReprintButtonSetup;
                  exit;
               end;   
               if dm.ibdsAutoPay.RecordCount > 0 then begin 
                  sm(msg1);
                  PageControl.ActivePage := tsAutoPay;
                  DisableButtons;
                  btnClear.Enabled := FAlse;
                  exit;
               end;                  
               SubsPayButtonSetup;
            end;
        1 : begin // Sections
               if (dm.ibdsPayDet.RecordCount > 0) or 
                  (dm.ibdsPayJoin.RecordCount > 0) or 
                  (dm.ibdsPayMemSec.RecordCount > 0) then begin
                  sm(msg1);
                  PageControl.ActivePage := tsSubsPay;
                  SubsPayButtonSetup;
                  exit;
               end;   
               if dm.ibdsBulkCardRec.RecordCount > 0 then begin 
                  sm(msg1);
                  PageControl.ActivePage := tsBulkCardReceipt;
                  ReprintButtonSetup;
                  exit;
               end;   
               if dm.ibdsAutoPay.RecordCount > 0 then begin
                  sm(msg1);
                  PageControl.ActivePage := tsAutoPay;
                  DisableButtons;
                  btnClear.Enabled := FAlse;
                  exit;
               end;
               SectionsPayButtonSetup;
            end;
        2 : begin
               dbeReceipt.SetFocus; //tsReprint
               if not dm.ibdsPay_Det.Active then
                  dm.ibdsPay_Det.Active := True;
               if not dm.ibdsPayments.Active then
                  dm.ibdsPayments.Active := True;

               ReprintButtonSetup;
            end;
        3 : begin
               if (dm.ibdsPayDet.RecordCount > 0) or 
                  (dm.ibdsPayJoin.RecordCount > 0) or 
                  (dm.ibdsPayMemSec.RecordCount > 0) then begin
                  sm(msg1);
                  PageControl.ActivePage := tsSubsPay;
                  SubsPayButtonSetup;
                  exit;
               end;   
               if dm.ibdsPAySections.RecordCount > 0 then begin 
                  sm(msg1);
                  PageControl.ActivePage := tsSectionsPay;
                  SectionsPayButtonSetup;
                  exit;
               end;
               if dm.ibdsAutoPay.RecordCount > 0 then begin 
                  sm(msg1);
                  PageControl.ActivePage := tsAutoPay;
                  DisableButtons;
                  btnClear.Enabled := FAlse;
                  exit;
               end;                  
               dm.ibdsBulkCardRec.Open;
               AutoPayButtonSetup;
               menuBulkCardReceipt.Enabled  := True;
               dtpDateFrom.Setfocus;
            end;
        4 : begin          
               if (dm.ibdsPayDet.RecordCount > 0) or 
                  (dm.ibdsPayJoin.RecordCount > 0) or
                  (dm.ibdsPayMemSec.RecordCount > 0) then begin
                  sm(msg1);
                  PageControl.ActivePage := tsSubsPay;
                  SubsPayButtonSetup;
                  exit;
               end;   
               if dm.ibdsPAySections.RecordCount > 0 then begin 
                  sm(msg1);
                  PageControl.ActivePage := tsSectionsPay;
                  SectionsPayButtonSetup;
                  exit;
               end;   
               if dm.ibdsBulkCardRec.RecordCount > 0 then begin 
                  sm(msg1);
                  PageControl.ActivePage := tsBulkCardReceipt;
                  ReprintButtonSetup;
                  exit;
               end;   
               AutoPayButtonSetup;
               menuAutoPayments.Enabled  := True;
               // opens here
               dbdtSetPayDay.Date := Date;
               if not dm.ibdsAutoPay.Active then
                  GetAutoPayMembers('Z');
               rpgAutoPayChange(Sender,0);
            end;
        end;        
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.ReprintButtonSetup;
begin
     DisableButtons;
     btnClear.Enabled   := False;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.SubsPayButtonSetup;
begin
     dblcSurname.SetFocus;  //tsSubsPay
     DisableButtons;
     //Subs
     btnClear.Enabled  := True;
     menuSubs.Enabled  := True;
     btnCash.Enabled   := True;
     btnTender.Enabled := True;
     btnChangeSubs.Enabled := True;
     btnPrintInvoice.Enabled := True;
end;

{ **************************************************************************** }
procedure TfrmMembersPAY.SectionsPayButtonSetup;
begin
     DisableButtons;
     // sections
     btnClear.Enabled         := True;
     btnTenderSection.Enabled := True;
     btnSaveSection.Enabled   := True;
     menuSections.Enabled     := True;
     if dm.ibdsPaySections.RecordCount = 0 then
        FillGridsSections;
     if dblcSections.enabled then
        dblcSections.SetFocus;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.AutoPayButtonSetup;
begin
     DisableButtons;
     //Auto Payments
     btnClear.Enabled  := True;
     btnStart.Enabled  := True;
     btnSaveSection.Enabled  := True;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.DisableButtons;
begin
     btnTenderSection.Enabled := False;
     btnSaveSection.Enabled   := False;
     menuSections.Enabled     := False;
     btnCash.Enabled          := False;
     btnTender.Enabled        := False;
     btnStart.Enabled         := False;
     btnChangeSubs.Enabled    := False;
     menuSections.Enabled     := False;
     menuSubs.Enabled         := False;
     btnPrintInvoice.Enabled  := False;
     menuBulkCardReceipt.Enabled  := False;
     menuAutoPayments.Enabled     := False;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.PageControlChanging(Sender: TObject; var AllowChange: Boolean);
begin
end;
{ **************************************************************************** }
{ **************************************************************************** }
// Reprints Methods below
procedure TfrmMembersPAY.dblcSearchKeyPress(Sender: TObject;
  var Key: Char);
begin
     if key = #13 then begin
        if dblcSearch.Text = '' then exit;
        dblcSurnameDropDown(Sender);
        if dm.qryMembersPAY.EOF then begin
           showmessage(Format('%s %s',[dblcSearch.Text,NotAValidMemMsg]));
        end else begin
           dblcSearch.Clear;
           dbgPayments.SetFocus;
           SelectPayments;
           GetPayments;
           UpdateFooter;
        end;
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dblcSearchCloseUp(Sender: TObject; LookupTable,
  FillTable: TDataSet; modified: Boolean);
begin
     if modified then begin
        if dm.qryMembersPay.FieldByName('MEMBER').AsString = '' then exit;
        dblcSearch.Clear;
        dbgPayments.SetFocus;
        SelectPayments;
        GetPayments;
        UpdateFooter;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbeReceiptKeyPress(Sender: TObject; var Key: Char);
const SQL1 = 'select PAYMENT from PAYMENTS WHERE PAYMENT = %s';
      msg1 = '%s is Not a valid Receipt, please re-enter';
begin
     if key = #13 then begin
        if (dbeReceipt.Text = '') or (dbeReceipt.Text = '0') then exit;
        if dm.sqlExecQueryCheckEOF(dm.sqlTransPay,format(SQL1,[dbeReceipt.Text])) then begin
           if (dm.sqlTransPay.Fields[0].AsString <> dbeReceipt.Text) then begin
              sm(Format(msg1,[dbeReceipt.Text]));
              exit;
           end;
        end else begin   // EOF
           sm(Format(msg1,[dbeReceipt.Text]));
           exit;
        end;
        PaymentStr := dbeReceipt.Text;
        dbeReceipt.Clear;
        dbgPayments.SetFocus;
        GetPayments;
        UpDateFooter;
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.SelectPayments;
// select from Payments(member or paid_by) as paid_by may not have paid for himself
const SQL1 = 'select distinct PAYMENT from PAYMENTS_DET WHERE MEMBER = %d OR PAID_BY = %d';
begin
     if dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[dm.qryMembersPay.FieldByName('MEMBER').AsInteger,dm.qryMembersPay.FieldByName('MEMBER').AsInteger])) then begin
        PaymentStr := '';
        while not dm.sqlTransPay.EOF do begin
           PaymentStr := PaymentStr + dm.sqlTransPay.FieldByName('PAYMENT').AsString + ',';
           dm.sqlTransPay.next;
        end;
        delete(PaymentStr,length(PaymentStr),1); // remove last comma
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.GetPayments;
const SQL1 = 'select * from PAYMENTS WHERE PAYMENT in ( %s ) ORDER BY PAYMENT desc';
begin
     if PaymentStr = '' then begin
        MDlgI(format('No Receipts for Member %s ' + 
           '%s',[dm.qryMembersPay.FieldByName('MEMBER').AsString,dm.qryMembersPay.FieldByName('FULL_NAME').AsString]));
        exit;   
     end; 
     dm.ibdsPayments.Close;
     dm.ibdsPayments.SelectSQL.Text:= format(SQL1,[PaymentStr]);
     dm.ibdsPayments.Open;
     with dm.ibdsPay_Det do begin // datasource is linked to qryPayments
        Close;
        Open;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgPaymentsCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
begin
     if not active then exit;
     if dm.ibdsPaymentsPAY_AMOUNT.AsFloat < 0.00 then
        AFont.Color := PayColor;
     //else
     //   AFont.Color := clBlack;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgPayments_DetCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;
  ABrush: TBrush);
begin
     if not active then exit;
     if dm.ibdsPay_DetAMOUNT_PAID.AsFloat < 0.00 then
        AFont.Color := PayColor;
    // else
     //   AFont.Color := clBlack;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgPayments_DetUpdateFooter(Sender: TObject);
const SQL1 = 'select sum(AMOUNT_PAID) as AMOUNT_PAID from PAYMENTS_DET WHERE PAYMENT = %d';
begin
     if dm.ibdsPayments.RecordCount = 0 then exit;
     with dm.sqlTransPay do begin
        dm.sqlExecQueryCheckEOF(dm.sqlTransPay,format(SQL1,[dm.ibdsPayments.FieldByName('PAYMENT').AsInteger]));
        dbgPayments_Det.ColumnByName('AMOUNT_PAID').FooterValue :=
           FloatToStrF(FieldByName('AMOUNT_PAID').AsFloat,ffcurrency,7,2);
        if FieldByName('AMOUNT_PAID').AsFloat >= 0.00 then
           dbgPayments_Det.Font.Color := clBlack
        else
           dbgPayments_Det.Font.Color := PayColor;
        close;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuPrintClick(Sender: TObject);
var qrPrint : Boolean;
    ReprintList : TStringList;
    s : shortstring;
{ if tsSubsPay and print - print all Receipts in  ReceiptList
               and preview - preview last receipt = FReceiptNumber
  if tsReprint and print - s := dm.ibdsPayments.FieldByName('PAYMENT').AsString;
               and preview - s := dm.qryPayments.FieldByName('PAYMENT').AsString;
}

{
 Posiflex - use QuickReport with a Generic Text Driver - with the actual driver it is to slow with graphics
 Samsumg  - use drivers
 Dot Matrix - use Generic
}
begin
     try
        if ReceiptType = 'P' then begin  
           frmGenericReceipt := TfrmGenericReceipt.Create(nil);
           frmGenericReceipt.lblReportTitle.Caption := 'Payment Receipt';
           if PageControl.ActivePageIndex = 2 then
              frmGenericReceipt.lblReportTitle.Caption := '** Reprint Payment Receipt **';
           frmGenericReceipt.qr.PrinterSettings.PrinterIndex := ReceiptPrinterIndex;
        end else begin                                 
           frmPayReprint := TfrmPayReprint.Create(nil);
           frmPayReprint.lblReportTitle.Caption := 'Payment Receipt';
           if PageControl.ActivePageIndex = 2 then
              frmPayReprint.lblReportTitle.Caption := '** Reprint Payment Receipt **';
           frmPayReprint.qr.PrinterSettings.PrinterIndex := ReceiptPrinterIndex;
        end;                 
        if TToolbutton(Sender).Name = btnPreview.Name then
           qrPrint := False
        else
           qrPrint := True;
        s := '';
        try
           if qrPrint then
              TooBusy(ExtractRptMsg);
           case PageControl.ActivePageIndex of
           0 : begin  // subs
                  if qrPrint then begin
                     {PrintFromSubs(Receiptlist); // can be multiple receipts in Receiptlist
                     if ReceiptList.Count > 1 then begin
                       if IfMsgdlg('Have all Receipts printed OK?') then
                     end else begin
                        ReceiptList.Clear;
                     end;
                     exit; }
                     PrintFromSubs(Receiptlist); // can be multiple receipts in Receiptlist
                     ReceiptList.Clear;  // clear after Printing

                  end else begin
                     s := IntToStr(FReceiptNumber);
                  end;
               end;
           1 : begin // sections
                  if qrPrint then begin
                     PrintFromSubs(SectionReceiptlist); // can be multiple receipts in Receiptlist
                     if SectionReceiptlist.Count > 1 then begin
                       if IfMsgdlg('Have all Section Receipts printed OK?') then
                          SectionReceiptlist.Clear;  // clear after Printing
                     end else begin
                        SectionReceiptlist.Clear;
                     end;
                  end else begin
                     // preview only the last Section
                     s := SectionReceiptlist[0]
                  end;
               end;
           2 : begin // reprint
                  if qrPrint then begin
                     ReprintList := TStringList.Create;
                     ReprintList.Add(dm.ibdsPayments.FieldByName('PAYMENT').AsString);
                     PrintFromSubs(ReprintList); 
                     ReprintList.Free;
                  end else begin
                     // Previews on frmPayReprint
                  s := dm.ibdsPayments.FieldByName('PAYMENT').AsString;
               end;
           end;
           end;
        finally
           NotBusy;
        end;
        // Preview
        if s <> '' then begin
           if ReceiptType = 'P' then begin
              if frmGenericReceipt.Extract(s)then
                 frmGenericReceipt.qr.Preview;
           end else begin  
              if frmPayReprint.Extract(s)then
                 frmPayReprint.qr.Preview;
           end;
        end;
     finally
        if ReceiptType = 'P' then   
           frmGenericReceipt.free
        else 
           frmPayReprint.free;
        dm.qryReport1.Close;
        dm.qryReport.Close;
     end;
     
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.PrintFromSubs( FromList : TStringList);
var i,j : smallint;
    FileName : ShortString;
    F : TextFile;
begin
      if FromList.Count = 0 then begin
         sm('No Receipts to print');
         exit; // none to print
      end;
      for i := 0 to FromList.Count -1 do begin
         // Thermal - Samsungs at the Moment
         if ReceiptType = 'T' then begin  
            if not frmPayReprint.Extract(FromList[i]) then continue;
            frmPayReprint.qr.Print;
         // Posiflex Thermal
         end else if ReceiptType = 'P' then begin  
            if not frmGenericReceipt.Extract(FromList[i]) then continue;
            frmGenericReceipt.qr.Print;          
         end else if ReceiptType = 'D' then begin // Dot Matrix  
         FileName := GetCurrentDir + ReceiptDosFileName;
            if not CreateFileLeaveOpen(F,FileName) then begin
               sm('Cannot Create the Receipt File ' + FileName);
               exit;
            end;
            PopulateFile(F,FromList[i]);
            CloseFile(F);
            // separate call to print blank lines at the Top
            PrintBlankLines(TopLineFeed);
            RichEdit.Lines.Clear;
            RichEdit.Lines.LoadFromFile(FileName);
            RichEdit.Print('Receipt');
            // separate call to print blank lines at the bottom
            PrintBlankLines(BottomLineFeed);
         end;
      end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.PopulateFile( var F : TextFile ; Receipt : ShortString );
const BlankLine = '';
var i : Shortint;
    s,Member,Subs,Paid : shortstring;
    Total : extended;
const SQL1 = 'select p.PAYMENT, p.POINTS_USED, p.DISCOUNT,pd.DESCRIPTION,' +
             'pd.MEMBER,pd.FULL_NAME,pd.AMOUNT, pd.AMOUNT_PAID,' +
             'POINTS_AVAILABLE from PAYMENTS p, PAYMENTS_DET pd ,MEMBERS m where p.PAYMENT = pd.PAYMENT ' +
             'and pd.MEMBER = m.MEMBER AND p.PAYMENT = %s ORDER BY p.PAYMENT';
begin
     WriteLn(F,CentreText(CompName,ReceiptLineLength));
     WriteLn(F,CentreText('Subscriptions Receipt',ReceiptLineLength));
     WriteLn(F,BlankLine);
     WriteLn(F,CentreText('Tax Invoice GST # ' + GSTNo,ReceiptLineLength));
     WriteLn(F,CentreText(formatDateTime ('dd mmm yyyy hh:mm:ss',now),ReceiptLineLength));
     WriteLn(F,BlankLine);

     with dm.qryGeneral do begin
        s := '';
        Total := 0.00;
        if dm.sqlExecqueryCheckEOF(dm.qryGeneral,format(SQL1,[Receipt])) then begin
           WriteLn(F,CentreText(format('Receipt Number %d',[FieldByName('PAYMENT').AsInteger]),ReceiptLineLength));
           WriteLn(F,CentreText(FieldByName('FULL_NAME').AsString,ReceiptLineLength));
           WriteLn(F,BlankLine);
           WriteLn(F,'Member  Subscription              Paid ');
           WriteLn(F,BlankLine);
           while not EOF do begin
              Member := LeftAdjustRightSpaces(FieldByName('MEMBER').AsString,6);
              Subs := LeftAdjustRightSpaces(FieldByName('DESCRIPTION').AsString,24);
              Paid := RightAdjustLeftSpaces(FormatFloat('$###.00',FieldByName('AMOUNT_PAID').AsFloat),7);
              WriteLn(F,Member + '  ' + Subs +  Paid);
              Total := Total + FieldByName('AMOUNT_PAID').AsFloat;
              next;
           end;
        end;
     end;
     WriteLn(F,BlankLine);
     WriteLn(F,'        Paid Thank You          ' + RightAdjustLeftSpaces(FormatFloat('$###.00',Total),7));
     if dm.qryGeneral.FieldByName('POINTS_USED').AsFloat > 0.00 then begin
        WriteLn(F,'        Points Redeemed         ' + RightAdjustLeftSpaces(FormatFloat('$##0.00',dm.qryGeneral.FieldByName('POINTS_USED').AsFloat),7));
        WriteLn(F,'        Points Available        ' + RightAdjustLeftSpaces(FormatFloat('$##0.00',dm.qryGeneral.FieldByName('POINTS_AVAILABLE').AsFloat),7));
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgPaymentsDblClick(Sender: TObject);
// both grids linked to here - PAYMENT is linked via the datasource
begin
     dm.qryPayTend.Open;
     if TwwDBGrid(Sender).Name = dbgPayments.Name then
        dm.qryPayTend.DataSource := dm.dsrPayments;
     if TwwDBGrid(Sender).Name = dbgPayments_Det.Name then
        dm.qryPayTend.DataSource := dm.dsrPay_Det;
     try
        frmMembersLookup := TfrmMembersLookup.create(nil);
        with frmMembersLookup do begin
           LookupOnly := True;
           btnCancel.Enabled := False;
           //lblSearch.Visible := False;       
           incSearch.Enabled := False;       
           Setup( dm.dsrPayTend,nil,'',0);   
           ShowModal;
        end;  
     finally
        dm.qryPayTend.DataSource := nil;
        frmMembersLookup.free;
     end;
end;
{ **************************************************************************** }
{ **************************************************************************** }
{ *******************   SECTIONS BELOW HERE ********************************** }
{ **************************************************************************** }
// NB cannot prepay or part pay Sections
procedure TfrmMembersPAY.PopulateSectionsGrid;
const SQL1 = 'update M_SECTIONS SET SELECTED = ''N'' where SECTION = %d';
      SQL2 = 'update M_SECTIONS SET SELECTED = ''Y'' where SECTION = %d  %s';
      SQL3 = 'select M.MEMBER,M.FULL_NAME,M.SURNAME,MS.SECTION,MS.AMOUNT_PAID,MS.AMOUNT,' +
             '(MS.AMOUNT - MS.AMOUNT_PAID) AS AMOUNT_DUE,MS.EDIT_DATE,MS.RECEIPT,MS.EXPIRES,MS.PAID ' +
             'from MEMBERS M,M_SECTIONS MS where MS.SECTION = %d and M.MEMBER = MS.MEMBER %s';
// NB ONLY for AMOUNT = AMOUNT_PAID or AMOUNT_PAID = 0.00 - NO part payments 
// because it makes it harder to add on or credit a part payment             
var S : string;
begin
     FLoadingSections := True;
     dm.ibdsPaySections.DisableControls;
     dm.ibdsPaySecMem.Close;
     // make it only = 0.00 or AMOUNT_PAID = AMOUNT
     // part payments make it hard to add on or credit
     s := '';
     case dbcbOptions.ItemIndex of
        0 : begin
               FFilterStr := 'and AMOUNT_PAID = AMOUNT or SECTION = ' + IntToStr(FSection) + ' and AMOUNT_PAID = 0.00';  // all
               S := ' and AMOUNT_PAID = AMOUNT or MS.SECTION = ' + IntToStr(FSection) + 
                    ' and M.MEMBER = MS.MEMBER and AMOUNT_PAID = 0.00';
            end;  
        1 : FFilterStr := 'and AMOUNT_PAID = 0.00';     // unpaid
        2 : FFilterStr := 'and AMOUNT_PAID = AMOUNT ';  // paid
     end;
     // make SELECTED 'N' where  = FSection
     // this is for when they change from Member to surname it keeps
     // the same dataset and doesnt drop paid or unpaid records out
     // depending  on the filter
     dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[FSection])); 
     // set SELECTED depending on the filter
     dm.sqlExecQuery(dm.sqlTransPay,format(SQL2,[FSection,FFilterStr])); 
     // initial selection of data - they can only change sort order
     // until the save or clear
     with dm.ibdsPaySections do begin
        close;
        if dbcbOptions.ItemIndex = 0 then
           SelectSQL.Text := Format(SQL3 + ' ORDER by %s',[FSection,s,FAddOrder])
        else   
           SelectSQL.Text := Format(SQL3 + ' ORDER by %s',[FSection,FFilterStr,FAddOrder]);
        Open;
     end;
     dblcSectionPaidBy.Clear;
     dm.ibdsPaySecMem.Params.ByName('MEMBER').AsInteger := dm.ibdsPAYSections.FieldByName('MEMBER').AsInteger;
     dm.ibdsPaySecMem.Open;
     dblcSectionPaidBy.Text := dm.ibdsPaySecMem.FieldByName('MEMBER').AsString;
     dm.ibdsPaySections.EnableControls;
     btnPrint.Enabled := False;
     btnPreview.Enabled := False;
     CalcSection;
     FLoadingSections := False;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.PopulateSectionsSorted;
var v : variant;
begin
     FLoadingSections := True;
     v := dm.ibdsPaySections.FieldByName('MEMBER').AsInteger;
     //  selection of data - where SELECTED = 'Y' - keeps the same data
     // as the initial select
     dm.ibdsPaySections.DisableControls;
     with dm.ibdsPaySections do begin
        close;
        SelectSQL.Text := Format('select M.MEMBER,M.FULL_NAME,M.SURNAME,' +
          'MS.SECTION,MS.AMOUNT_PAID,MS.AMOUNT,(MS.AMOUNT - MS.AMOUNT_PAID) AS AMOUNT_DUE,' +
          'MS.EDIT_DATE,MS.RECEIPT,MS.EXPIRES,MS.PAID from MEMBERS M,M_SECTIONS MS ' +
          'where MS.SECTION = %d and M.MEMBER = MS.MEMBER and MS.SELECTED = ''Y'' ' +
          'ORDER by %s',[FSection,FAddOrder]);
        Open;
     end;
     dm.ibdsPaySections.Locate('MEMBER',v,[]);  // reposition
     dm.ibdsPaySections.EnableControls;
     FLoadingSections := False;
     CalcSection;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.UpdateSection;
// only does this after the initial lookup and
// increases the prices where EXPIRES < now
// clears PAID where EXPIRES < now
// clears AMOUNT_PAID where EXPIRES < now
// Rolover will handle the EXPIRES date in SECTIONS
const SQL1 = 'update M_SECTIONS MS SET AMOUNT = ( SELECT FEE FROM SECTIONS where MS.SECTION = ' + 
             'SECTION) where MS.EXPIRES < ''%s'' and MS.SECTION = %d';
      SQL2 = 'update M_SECTIONS SET PAID = ''N'',AMOUNT_PAID = 0.00 WHERE SECTION = %d ' +
             'and EXPIRES < ''%s''';
             
begin
      try
         dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[GetShortDate(SectionGoodDate),FSection]));
         dm.sqlExecQuery(dm.sqlTransPay,format(SQL2,[FSection,GetShortDate(SectionGoodDate)]));
      except
        ClearSectionFields; // does a rollback here
        sm( 'There has been an error when updating the Sections' + #13#10 +  RolledBackPaymentMsg );
        exit;
      end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dblcSectionsKeyPress(Sender: TObject; var Key: Char);
var V : Variant;
begin
     if key = #13 then begin
        V := dm.tblSections.FieldByName('SECTION').AsInteger;
        if not dm.tblSections.Locate('SECTION',V,[]) then begin
           dblcSections.Text := '';
           sm('Section not found');
           dblcSections.SetFocus;
        end else begin
           FSection := dm.tblSections.FieldByName('SECTION').AsInteger;
           if not SectionCheckDate then exit;
           UpdateSection;
           PopulateSectionsGrid;
           CalcSection;
           dblcSections.Text := dm.tblSections.FieldByName('DESCRIPTION').AsString;
           SectionName := dblcSections.Text; // pass to dm
           dblcSections.Enabled := False;
           dbcbOptions.Enabled := False;
           dm.ibdsPaySections.First;
           incSearch.SetFocus;
        end;
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dblcSectionsCloseUp(Sender: TObject; LookupTable,
  FillTable: TDataSet; modified: Boolean);
begin
     if modified then begin
        dblcSections.Text := dm.tblSections.FieldByName('DESCRIPTION').AsString;
        SectionName := dblcSections.Text; // pass to dm
        dblcSections.Enabled := False;
        dbcbOptions.Enabled := False;
        FSection := dm.tblSections.FieldByName('SECTION').AsInteger;
        if not SectionCheckDate then exit;
        UpdateSection;
        PopulateSectionsGrid;
        CalcSection;
        dm.ibdsPaySections.First;
        incSearch.SetFocus;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.incSearchExit(Sender: TObject);
begin
     incSearch.Clear;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.btnMembersClick(Sender: TObject);
// changing the order reselects the same data as the original select
// because eg record becomes paid on an unpaid filter
begin
     if dblcSections.Enabled then exit;
     incSearch.Clear;
     if TRadioButton(Sender).Name = btnMembers.Name then begin
        incSearch.SearchField := 'MEMBER';
        FAddOrder := 'MEMBER';
     end else begin
        incSearch.SearchField := 'SURNAME';
        FAddOrder := 'SURNAME';
     end;
     PopulateSectionsSorted;
     CalcSection;
     incSearch.SetFocus;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.ClearSectionFields;
begin
     dm.qryMembersPay.Close;
     dbgSectionsPay.ColumnByName('AMOUNT').FooterValue := '';
     dbgSectionsPay.ColumnByName('AMOUNT_PAID').FooterValue := '';
     dm.qryMembersPAY.Close;
     if not dblcSectionPaidBy.Enabled then
        dblcSectionPaidBy.Enabled := True;
     dblcSectionPaidBy.Text := '';
    // dblcSectionPaidBy.Clear;
     dbeTotalSection.Clear;
     dblcSections.Enabled := True;
     dbcbOptions.Enabled  := True;
     dblcSections.Clear;
     if dblcSections.visible then
       dblcSections.SetFocus;
     if dm.IBTransPay.InTransaction then begin
        dm.IBTransPay.RollBack;
        dm.IBTransPay.StartTransaction;
     end;
     incSearch.Clear;
     FillGrids;
     FillGridsSections;
     cbxUsePointsS.State := cbUnchecked;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.FillGridsSections;
begin
     with dm.ibdsPaySections do begin
        close;
        SelectSQL.Text := 'select M.MEMBER,M.FULL_NAME,M.SURNAME,' +
          'MS.SECTION,MS.AMOUNT_PAID,MS.AMOUNT,(MS.AMOUNT - MS.AMOUNT_PAID) AS AMOUNT_DUE,' +
          'MS.EDIT_DATE,MS.RECEIPT,MS.EXPIRES,MS.PAID from MEMBERS M,M_SECTIONS MS ' +
          'where MS.SECTION = -1 and M.MEMBER = MS.MEMBER';
        Open;
     end;
     with dm.ibdsPaySecMem do begin
        close;
        dblcSectionPaidBy.Clear;
        dm.ibdsPaySecMem.Params.ByName('MEMBER').AsInteger := -1;
        Open;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.CalcSection;
begin
     try
        with dm.sqlTransPAY do begin
           close;
           SQL.Text := Format('select sum(AMOUNT) as AMOUNT,sum(AMOUNT_PAID) as AMOUNT_PAID ' +
             'from M_SECTIONS where SECTION = %d %s',[FSection,FFilterStr]);
           ExecQuery;
          dbgSectionsPay.ColumnByName('AMOUNT').FooterValue :=
             FloatToStrF(FieldByName('AMOUNT').AsFloat,ffcurrency,7,2);
          dbgSectionsPay.ColumnByName('AMOUNT_PAID').FooterValue :=
             FloatToStrF(FieldByName('AMOUNT_PAID').AsFloat,ffcurrency,7,2);
        end;
     finally
        CalcSectionTotal;
        dm.sqlTransPAY.Close;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.cbxUsePointsSClick(Sender: TObject);
begin
    { if Blocked then exit;
     if dm.ibdsPaySecMem.FieldByName('POINTS_AVAILABLE').AsFloat = 0.00 then begin
        cbxUsePointsS.State := cbUnchecked;    // no points to use
        exit;
     end;
     CalcSectionTotal;}
end;
{ **************************************************************************** }
function TfrmMembersPAY.SectionCheckDate : Boolean ;
begin
     Result := False;
     SectionGoodDate := Date;
     if not Global.CheckSystemDate( SectionGoodDate ) then begin
        ClearSectionFields; // does a rollback here
        sm( IncorrectDateMsg + #13#10 + RolledBackPaymentMsg + #13#10 +
            'The Date must be altered! No payments will be accepted');
        exit;
     end;
     Result := True;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgSectionsPayCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;
  ABrush: TBrush);
begin
     if dm.ibdsPaySections.FieldByName('AMOUNT_PAID').AsFloat < 
        dm.ibdsPaySections.FieldByName('AMOUNT').AsFloat then
        AFont.Color := PayColor;
     if Field.FieldName = 'PAID' then ABrush.Color := fcGridEdit;  
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgSectionsPayDblClick(Sender: TObject);
// NB does post in dbgSectionsPayFieldChanged(Sender)
begin
     with dm.ibdsPaySections do begin
        if FieldByName('PAID').AsString = 'N' then begin
           Edit;
           FieldByName('PAID').AsString := 'Y';
        end else begin
           Edit;
           FieldByName('PAID').AsString := 'N';
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgSectionsPayFieldChanged(Sender: TObject;
  Field: TField);
begin
     if blocked then exit;
     dm.ibdsPaySections.Post; // does all work in dm posting
     CalcSection;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgSectionsPayRowChanged(Sender: TObject);
// refresh the Paid By and Points for selected Member
begin
     if FLoadingSections then exit;
     if dm.ibdsPaySections.FieldByName('PAID').AsString = 'Y' then begin
        Blocked := True;
        cbxUsePointsS.State := cbUnchecked;
        Blocked := False;
        dblcSectionPaidBy.Enabled := False;
        cbxUsePointsS.Enabled     := False;
     end else begin
        dblcSectionPaidBy.Enabled := True;
        cbxUsePointsS.Enabled     := True;
     end;
     dm.ibdsPaySecMem.Close;
     dblcSectionPaidBy.Clear;
     dm.ibdsPaySecMem.Params.ByName('MEMBER').AsInteger := dm.ibdsPAYSections.FieldByName('MEMBER').AsInteger;
     dm.ibdsPaySecMem.Open;
     dblcSectionPaidBy.Text := dm.ibdsPaySecMem.FieldByName('MEMBER').AsString;
     CalcSectionTotal;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.CalcSectionTotal;
var SectionPoints : Extended;
    TempTotal : extended;
begin
     TempTotal := Global.RemoveSignSpaceConvert(dbeSectionsDue.Text);
     if TempTotal = -9999999.00 then begin
        ClearSectionFields; // does a rollback here
        sm( 'There has been an error converting the Due amount'  + #13#10 + RolledBackPaymentMsg);
        exit;
     end;
     if dm.ibdsPaySections.FieldByName('AMOUNT_DUE').AsFloat = 0.00 then begin
        dbeSectionsDue.Font.Color  := clBlack;
        dbeSectionsDue.Frame.NonFocusTransparentFontColor  := clBlack;
        dbeTotalSection.Frame.NonFocusTransparentFontColor  := clBlack;
        dbeTotalSection.Font.Color := clBlack;
     end else begin
        dbeSectionsDue.Font.Color  := PayColor;
        dbeSectionsDue.Frame.NonFocusTransparentFontColor  := PayColor;
        dbeTotalSection.Frame.NonFocusTransparentFontColor  := PayColor;
        dbeTotalSection.Font.Color := PayColor;
     end;
     SectionPoints := dm.ibdsPaySecMem.FieldByName('POINTS_AVAILABLE').AsFloat;
     dbePointsS.Hint := Format('Points dollar value = $%f',[SectionPoints]);
     if cbxUsePointsS.Checked then begin
        if SectionPoints >= TempTotal then begin
           PointsUsedSection    := SectionPoints;
           TempTotal := 0.00;
        end else begin
           PointsUsedSection    := SectionPoints;
           TempTotal := TempTotal - SectionPoints;
        end;
        dbeTotalSection.Hint := Format('Points Used = %f',[PointsUsedSection]);
        dbeTotalSection.Text := FloatToStrF( TempTotal,ffCurrency,7,2);
     end else begin
        dbeTotalSection.Text := dbeSectionsDue.Text;
        PointsUsedSection    := 0.00;
        dbeTotalSection.Hint := '';
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuTenderSectionClick(Sender: TObject);
var AmountOK : Boolean;
    TenderPayment : extended;
    Tender,Details : string;
    AmountExt : extended;
    ReceiptNumber : Integer;
    MemberPayments : TMemberPayments;
begin
     TenderPayment := Global.RemoveSignSpaceConvert(dbeTotalSection.Text);
     if TenderPayment = -9999999.00 then begin
        ClearSectionFields; // does a rollback here
        sm( 'There has been an error converting the tender amount'  + #13#10 +  RolledBackPaymentMsg);
        exit;
     end;
     // members must be UNPAID (unticked ) before they can select the tender button
     if dm.ibdsPaySections.FieldByName('PAID').AsString = 'Y' then exit;
     if TenderPayment = 0.00 then exit;
     //SectionReceipt := GetReceiptNumber; // passed to dm
     SectionReceipt := MemberPayments.GetReceiptNumber(dm.sqlTransPAy);
     
     if sfPreCheckLogOn(TfrmTenders) then begin
        try
           frmTenders := TfrmTenders.Create(nil);
           // TenderPayment = unrounded with no points
           frmTenders.CashAmount := TenderPayment;
           AmountOK := (frmTenders.ShowModal = mrOK);
           if AmountOk then begin
              // Post the Payments into PAYMENT_TEND if mrOK
              with dm.ibdsTenders do begin
                 First;
                 while not EOF do begin
                    Tender  := FieldByName('TENDER_TYPE').AsString;
                    Details := FieldByName('DETAILS').AsString;
                    AmountExt  := Global.RemoveSignSpaceConvert(FieldByName('AMOUNT').AsString);
                    if AmountExt = -9999999.00 then begin
                       ClearSectionFields; // does a rollback here
                       sm( 'There has been an error converting the tender amount'  + #13#10 + RolledBackPaymentMsg);
                       exit;
                    end;
                    if AmountExt <> 0.00 then 
                       // PAYMENT_TEND is Auto INC
                       dm.sqlExecQuery(dm.sqlTransPay,format(InsertPaymentsTend,[SectionReceipt,Tender,AmountExt,Details, FieldByName('EX_RATE').AsFloat]));
                    next;
                 end;
              end;
              RoundValue := frmTenders.RndValue; // pass to dm
              // NB does post in dbgSectionsPayFieldChanged(Sender)
              with dm.ibdsPaySections do begin
                 Edit;
                 FieldByName('PAID').AsString := 'Y';
              end;
           end;
           dm.ibdsTenders.Close;
        finally
           frmTenders.Release;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbcbOptionsChange(Sender: TObject);
begin
     dblcSections.SetFocus;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.incSearchKeyPress(Sender: TObject; var Key: Char);
begin
     if key = #13 then
        SelectNext(ActiveControl as TWinControl,True,True);
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dblcSectionPaidByKeyPress(Sender: TObject;
  var Key: Char);
begin
    if key = #13 then begin
       dblcSurnameDropDown(Sender);
       dm.ibdsPaySecMem.Close;
       dblcSectionPaidBy.Clear;
       if dm.qryMembersPAY.EOF then begin
          sm(NotAValidMemMsg);
          dm.ibdsPaySecMem.Params.ByName('MEMBER').AsInteger := dm.ibdsPAYSECTIONS.FieldByName('MEMBER').AsInteger;
          dm.ibdsPaySecMem.Open;
          dblcSectionPaidBy.Text := dm.ibdsPaySecMem.FieldByName('MEMBER').AsString;
       end else begin
          //dm.ibdsPaySecMem.Close;
          //dblcSectionPaidBy.Clear;
          dm.ibdsPaySecMem.Params.ByName('MEMBER').AsInteger := dm.qryMembersPAY.FieldByName('MEMBER').AsInteger;
          dm.ibdsPaySecMem.Open;
          dblcSectionPaidBy.Text := dm.ibdsPaySecMem.FieldByName('MEMBER').AsString;
          SelectNext(ActiveControl as TWinControl,True,True);
          key := #0;
       end;
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dblcSectionPaidByCloseUp(Sender: TObject;
  LookupTable, FillTable: TDataSet; modified: Boolean);
begin
     if modified then begin
        dm.ibdsPaySecMem.Close;
        dblcSectionPaidBy.Clear;
        dm.ibdsPaySecMem.Params.ByName('MEMBER').AsInteger := dm.qryMembersPAY.FieldByName('MEMBER').AsInteger;
        dm.ibdsPaySecMem.Open;
        dblcSectionPaidBy.Text := dm.ibdsPaySecMem.FieldByName('MEMBER').AsString;
        SelectNext(ActiveControl as TWinControl,True,True);
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dtPickerKeyPress(Sender: TObject;
  var Key: Char);
const SQL1 = 'select PAYMENT from PAYMENTS WHERE EDIT_DATE = ''%s'' ORDER BY PAYMENT desc ';
      SQL2 = 'select * from PAYMENTS ORDER BY PAYMENT desc';
begin
     if key = #13 then begin
        with dm.sqlTransPay do begin
           if dtPicker.Date > StrToDateTime('01/01/1900') then begin
              if not dm.sqlExecQueryCheckEOF(dm.sqlTransPay,format(SQL1,[GetShortDateTime(dtPicker.Date)])) then begin
                 sm(Format('No Receipts for %s',[GetShortDateTime(dtPicker.Date)]));
                 exit;
              end;
              PaymentStr := '';
              while not EOF do begin
                 PaymentStr := PaymentStr + FieldByName('PAYMENT').AsString + ',';
                 next;
              end;
              delete(PaymentStr,length(PaymentStr),1); // remove last comma
              GetPayments;
           end else begin // 30/12/1899 all dates selected - press enter with no date
              dm.ibdsPayments.Close;
              dm.ibdsPayments.SelectSQL.Text := SQL2;
              dm.ibdsPayments.Open;
              with dm.ibdsPay_Det do begin // datasource is linked to qryPayments
                 Close;
                 Open;
              end;
           end;
           close;
        end;
        dbeReceipt.Clear;
        dblcSearch.Clear;
        dbgPayments.SetFocus;
        UpdateFooter;
    end;
end;
{ **************************************************************************** }
{procedure TfrmMembersPAY.menuPrintCardClick(Sender: TObject);
begin
     {if Global.CardPrinterIndex = -1 then begin
        sm('No Card Printer Installed!, this can be setup in the Sytem Screen/Other tab');
        exit;
     end;
     // if its already showing;
     if pnlPrintCard.Visible then begin
        btnPrintCard.Down := False;
        pnlPrintCard.Visible := False;
        PageControlChange(Sender);
        exit;
     end;   
     btnPrintCard.Down := True;
     pnlPrintCard.Visible := True;
     DisableButtons;
     // put in the last fully paid member, or one in the grid or blank
     if CardList.Count <> 0 then
        edtPrintCardMember.Text := CardList[0]
     else if MemberList.Count = 0 then 
        edtPrintCardMember.Text := ''
     else
        edtPrintCardMember.Text := MemberList[0];
     edtPrintCardMember.SetFocus;
     edtPrintCardMember.SelectAll;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.UpdateFooter;
var PayAmount, PointsUsed : Extended;
begin
     if dm.ibdsPayments.recordcount = 0 then exit;
     PayAmount := 0.00; PointsUsed := 0.00;
     with dm.ibdsPayments do begin
        DisableControls;
        First;
        while not EOF do begin
           PayAmount := PayAmount + FieldByName('PAY_AMOUNT').AsFloat;
           PointsUsed := PointsUsed + FieldByName('POINTS_USED').AsFloat;
           Next;
        end;
        First;
        dbgPayments.ColumnByName('PAY_AMOUNT' ).FooterValue := FloatToStrF(PayAmount, ffCurrency, 10, 2);
        dbgPayments.ColumnByName('POINTS_USED').FooterValue := FloatToStrF(PointsUsed, ffCurrency, 10, 2);
        EnableControls;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuPrintTodaysTotalsClick(Sender: TObject);
var PayDate : TDateTime;
begin
     {if dtPicker.Date = StrToDate(StrToDate1899) then
        PayDate := Date
     else
        PayDate := dtPicker.Date;
     try
        TooBusy(ExtractRptMsg);
        frmPayTotals := TfrmPayTotals.Create(nil);
        if not frmPayTotals.Extract(PayDate) then exit;
        frmPayTotals.qr.PrinterSettings.PrinterIndex := Printer.PrinterIndex;
        frmPayTotals.qr.Print;
     finally
        frmPayTotals.Free;
        NotBusy;
     end;}
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.AddRemoveLateFeeSub;
var  SaveMemberList : TStringList;
     PaidByMember,Member : LongInt;
const SQL1 = 'INSERT into M_SUBS ( MEMBER,SUBSCRIPTION,AMOUNT_PAID,AMOUNT,PAID,' +
             'EDIT_DATE,RECEIPT,PAY ) Values (%d,%d,0.00,%f,''N'',Current_TimeStamp,0,0.00)';
begin
     if dm.ibdsPayDet.RecordCount = 0 then exit;
     // Spare Char used as Late Fee Subscription
     dm.tblSubs.Open;
     FLateSubAdded := False;
     if dm.tblSubs.Locate('SPARE_CHAR','Y',[loCaseInsensitive]) then begin
        SaveMemberList := TStringList.Create;
        SaveMemberList.Assign(MemberList);
        PaidByMember := dm.ibdsPayMembers.FieldByName('MEMBER').AsInteger;
        Member := dm.ibdsPayDet.FieldByName('MEMBER').AsInteger;
        ClearPayFields;
        MemberList.Assign(SaveMemberList);
        SaveMemberList.Free;
        FLateSub      := dm.tblSubs.FieldByName('SUBSCRIPTION').AsInteger;
        FLateSubAdded := True;
     end else begin
        sm('No Late Fee Subscription has been allocated');
        dm.tblSubs.Close;
        exit;
     end;
     dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[Member,dm.tblSubs.FieldByName('SUBSCRIPTION').AsInteger,dm.tblSubs.FieldByName('FEE').AsFloat]));
     PopulatePayGrid;
     dm.ibdsPayMembers.Close;
     dm.ibdsPayMembers.Params.ByName('MEMBER').AsInteger := PaidByMember;
     dm.ibdsPayMembers.Open;
     dblcPaidBy.Text := dm.ibdsPayMembers.FieldByName('MEMBER').AsString;
     dbgSubPay.Update;
     dbgPaySecMem.Update;
     dm.tblSubs.Close;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuChangeSubsClick(Sender: TObject);
var  SaveMemberList : TStringList;
     Member,PaidByMember : LongInt;
     Subscription,i : Smallint;
     Fee,AmountPaid : Extended;
const SQL1 = 'DELETE FROM M_SUBS WHERE MEMBER = %d and SUBSCRIPTION = %d';
      SQL3 = 'DELETE FROM M_GROUPS WHERE MEMBER = %d';
      SQL4 = 'INSERT INTO M_GROUPS (MEMBER,GROUPS) values ( %d,%d)';
      SQL2 = 'INSERT into M_SUBS (MEMBER,SUBSCRIPTION,AMOUNT_PAID,AMOUNT,PAID,' +
             'EDIT_DATE,RECEIPT,PAY ) Values (%d,%d,%f,%f,''N'',Current_TimeStamp,0,0.00)';
      SQL5 = 'Select Count(Member) as cnt from M_GROUPS where Member = %d';
begin
     if dm.ibdsPayDet.RecordCount = 0 then exit;
     //AmountPaid   := dm.ibdsPayDet.FieldByName('AMOUNT_PAID').AsFloat;
     //if AmountPaid > 0.00 then begin
     //   if not ifMsgDlg(format('The Member has paid %f - you might have to alter'  + #13#10 +
     //                   'the Expires date manually if the fees increase/decrease - continue?',[AmountPaid])) then exit;
     //end;
     try
        frmMembersLookup := TfrmMembersLOOKUP.create(nil);
        frmMembersLookup.Setup( dm.dsrTblSubs,dm.tblSubs,         // subs
                        'SUBSCRIPTION',dm.ibdsPayDet.FieldByName('MEMBER').AsInteger);
        if (frmMembersLookup.ShowModal = mrOK) then begin
           if frmMembersLookup.MultiList.Count > 0 then begin
              // Save MemberList - gets cleared in ClearPayFields
              Member       := dm.ibdsPayDet.FieldByName('MEMBER').AsInteger;
              Subscription := dm.ibdsPayDet.FieldByName('SUBSCRIPTION').AsInteger;
              AmountPaid   := dm.ibdsPayDet.FieldByName('AMOUNT_PAID').AsFloat;
              PaidByMember := dm.ibdsPayMembers.FieldByName('MEMBER').AsInteger;
              SaveMemberList := TStringList.Create;
              SaveMemberList.Assign(MemberList);
              ClearPayFields;
              MemberList.Assign(SaveMemberList);
              SaveMemberList.Free;
              try
                  // Delete the record
                  dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[Member,Subscription]));
                  //PrcssLog it
                  dm.ProcessType := ord(TProcessTypes(ptMembers));
                  dm.LogProcess ( dm.sqlTransPay,format('Subscription %d deleted from Member %d in Payments',[Subscription,Member]));
                  // Get the new Subscription
                  Subscription := dm.tblSubs.FieldByName('SUBSCRIPTION').AsInteger;
                  Fee := GetLatestFee(SubsTableName,FieldNameArray[0],'N',Subscription);
                  //Add Sub Record
                  dm.sqlExecQuery(dm.sqlTransPay,format(SQL2,[Member,Subscription,AmountPaid,Fee]));
                  // PrcssLog it
                  dm.LogProcess ( dm.sqlTransPay,format('Subscription %d added for Member %d in Payments',[Subscription,Member]));
                  try
                  // Export it - change in Subs
                     dm.LogExport(dm.sqlTransPay,Member,0,False);
                  except
                     if dm.IBTransPay.InTransaction then begin
                        dm.IBTransPay.RollBack;
                        dm.IBTransPay.StartTransaction;
                     end;
                     sm(FailedToLogExportMsg);
                  end;
                  // Alter Groups as well
                  dm.sqlExecQuery(dm.sqlTransPay,format(SQL5,[Member]));
                  if dm.sqlTransPay.FieldByName('Cnt').AsInteger > 1 then begin
                     MDlgI(format('Member %d has more than one Group, these must be changed in the Maintenance Screen',[Member]));
                  end else begin
                     frmMembersLookup.Setup( dm.dsrTblGroups,dm.tblGroups,         // subs
                                     'GROUPS',dm.ibdsPayDet.FieldByName('MEMBER').AsInteger);
                     if (frmMembersLookup.ShowModal = mrOK) then begin
                        if frmMembersLookup.MultiList.Count > 0 then begin
                           // delete all from m_groups
                           dm.sqlExecQuery(dm.sqlTransPay,format(SQL3,[Member]));
                           // add into m_groups - can be more than one
                           for i := 0 to frmMembersLookup.MultiList.Count - 1 do begin
                              dm.sqlExecQuery(dm.sqlTransPay,format(SQL4,[Member, strToInt(frmMembersLookup.MultiList[i])]));
                           end;
                        end;
                     end;
                  end;
                  // commit work done
                  CommitAndOpenTables;
                  PopulatePayGrid;
                  // populate paid by member
                  dm.ibdsPayMembers.Close;
                  dm.ibdsPayMembers.Params.ByName('MEMBER').AsInteger := PaidByMember;
                  dm.ibdsPayMembers.Open;
                  dblcPaidBy.Text := dm.ibdsPayMembers.FieldByName('MEMBER').AsString;
                  dbgSubPay.Update;
                  dbgPaySecMem.Update;
              except
                 if dm.IBTransPay.InTransaction then begin
                    dm.IBTransPay.RollBack;
                    dm.IBTransPay.StartTransaction;
                 end;
              end;
           end;
        end;
     finally
        frmMembersLookup.Free;
     end;
end;
{ **************************************************************************** }

procedure TfrmMembersPAY.menuSavetheSectionClick(Sender: TObject);
begin
     if not IfMsgdlg(Format('Save the %s Section',[dblcSections.Text])) then exit;
     try
        CommitAndOpenTables;
        btnPrint.Enabled := True;
        btnPreview.Enabled := True;
     except
        dm.IBTransPay.RollBackRetaining;
        sm(RolledBackPaymentMsg);
     end;
     ClearSectionFields;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuExitClick(Sender: TObject);
begin
     close;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuPrinterSetupClick(Sender: TObject);
begin
     dm.PrintDialog.Execute;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
var tabsheet  : TTabSheet;
begin
     inherited;
     case key of   
         //VK_F3 : menuPaybyCash.OnClick(menuPaybyCash);                 
         //VK_F9 : menuPaybyCash.OnClick(menuPaybyCash);
         VK_ESCAPE : menuExitClick(Sender);
     end;   
     if ( ssCtrl in Shift ) then begin
        case key of 
           70,102 : begin
                       case PageControl.ActivePageIndex of 
                          0 : dblcSurname.SetFocus;           // Find 
                          1 : incSearch.SetFocus;
                          2 : dblcSearch.Setfocus;
                          4 : incAutoPayments.Setfocus;
                       end;   
                    end;
           VK_F10 : menuPrintInvoiceClick(Sender);
        end;
     end;   
     
     if ( ssAlt in shift ) then begin
        tabsheet := nil;
        case key of 
           88,120 : menuExitClick(Sender);  // X Close 
           73,105 : tabsheet := tsSectionsPay;   // I
           77,109 : tabsheet := tsSubsPay;       // M
           78,110 : tabsheet := tsReprint;     // N
           86,118 : menuPreview.OnClick(menuPreview);       // V  Preview
           80,112 : menuPrinterSetupClick(Sender);         // P  Printer Setup
           89,121 : menuPrintTodaysTotals.OnClick(menuPrintTodaysTotals); // Y 
           67,99  : menuPaybyCash.OnClick(menuPaybyCash);   // C 
           82,114 : menuClearSubs.OnClick(menuClearSubs);   // R
           84,116 : menuPaybyCash.OnClick(menuPaybyTender); // T
           85,117 : menuChangeSubs.OnClick(menuChangeSubs); // U
        end;
        if tabsheet <> nil then
           if tabsheet.TabVisible then PageControl.ActivePage := tabsheet;
     end; 
     key := 0;   
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.CommitAndOpenTables;
begin
     if dm.IBTransPay.InTransaction then begin
        dm.IBTransPay.Commit;
        dm.IBTransPay.StartTransaction;
        OpenTAbles;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.FormResize(Sender: TObject);
begin
     ResizeToolBar( Self ,Self.WindowState,OneButtonWidth,FpnlWidth,pnlSpace);
     if Self.WindowState = wsMAximized then begin
        Self.Height := HeightWidth.frmHeight;
        Self.Width  := HeightWidth.frmWidth;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dtpDateFromExit(Sender: TObject);
begin
     (Sender as TwwDbDateTimePicker).DisplayFormat := 'dd mmm yyyy';
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dtpDateFromKeyPress(Sender: TObject; var Key: Char);
begin
    if key = #13 then SelectNext(ActiveControl as TWinControl,True,True);
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuStartCRClick(Sender: TObject);
const SQL1 = 'select pd.AMOUNT_PAID,pd.DESCRIPTION,pd.EXPIRES, pd.FULL_NAME,pd.INVOICE_NUMBER,' +
             'pd.MEMBER,pd.PAY_DATE, pd.PAYMENT,m.PAYMENT_PERIOD, m.AUTO_PAYMENT,' +
             'm.BARCODE_NUMBER,m.PRINT_CARD,m.PRINT_RECEIPT,m.NUMBER_CARDS, ' +
             'm.FIRST_NAME,m.SURNAME,pt.TENDER_TYPE,m.ADDRESS,m.ADDRESS1,m.DISTRICT, ' +
             ' m.CITY, m.COUNTRY ' +
             'from MEMBERS m, PAYMENTS_DET pd , PAYMENTS_TEND pt ' +
             'where m.MEMBER = pd.MEMBER and pd.PAYMENT = pt.PAYMENT and pd.PAY_DATE between ''%s'' and  ''%s'' ' +
             '  order by pd.PAY_DATE ';
      SQL2 = 'select p.PAY_AMOUNT as AMOUNT_PAID,null as DESCRIPTION,m.EXPIRES,p.FULL_NAME,' +
             'p.PAID_BY, cast(p.EDIT_DATE as Date ) as PAY_DATE,m.MEMBER,m.PAYMENT_PERIOD, m.AUTO_PAYMENT,' +
             'm.BARCODE_NUMBER,m.PRINT_CARD,m.PRINT_RECEIPT,m.NUMBER_CARDS,p.INVOICE_NUMBER, ' +
             'm.FIRST_NAME,m.SURNAME,pt.TENDER_TYPE,m.ADDRESS,m.ADDRESS1,m.DISTRICT, ' +
             'm.CITY, m.COUNTRY,p.PAYMENT ' +
             'from MEMBERS m, PAYMENTS_TEND pt, PAYMENTS p ' +
             'where m.MEMBER = p.PAID_BY and p.PAYMENT = pt.PAYMENT ' +
             'and p.EDIT_DATE between ''%s'' and  ''%s'' order by p.EDIT_DATE';
var FromDate,ToDate : TDateTime;
begin
     PageControl.ActivePageIndex := 3;
     with dm.ibdsBulkCardRec do begin
        Close;
        FromDate := dtpDateFrom.Date;
        ToDate   := dtpDateTo.Date;
        case rgpBulk.ItemIndex of
           0 : SelectSQL.TEXT := format(SQL2,[ formatdatetime( 'mm/dd/yyyy' ,FromDate), formatdatetime('mm/dd/yyyy',ToDate)]);
           1 : SelectSQL.TEXT := format(SQL1,[ formatdatetime( 'mm/dd/yyyy' ,FromDate), formatdatetime('mm/dd/yyyy',ToDate)]);
        end;   
        Open;
        dbgBulk.Setfocus;
    end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dtpDateToKeyPress(Sender: TObject; var Key: Char);
begin
    if key = #13 then menuStartCRClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.btnPrintCardsClick(Sender: TObject);
var i : shortint;
begin
     with dm.ibdsBulkCardRec do begin
        if State = dsEdit then Post;
        try
           DisableControls;
           if (Global.CardPrinterIndex = -1) or
              (Global.CardPrinterName = '') then begin
              sm(NoCardPrinterMsg);
           end else begin
              dm.tblCardDetail.Open;
              First;
              while not EOF do begin
                 if FieldByName('PRINT_CARD').AsString = 'Y' then begin
                    // one card if null
                    if FieldByName('NUMBER_CARDS').IsNull then begin
                       Global.CardPrint(FieldByName('MEMBER').AsString,'',1,dm.IBTransPay);
                    end else begin   
                       Global.CardPrint(FieldByName('MEMBER').AsString,'',FieldByName('NUMBER_CARDS').AsInteger,dm.IBTransPay);
                    end;
                 end;   
                 next;
              end;
           end;
        finally
           dm.tblCardDetail.Filtered := False;
           dm.tblCardDetail.Close;
           EnableControls;
        end;   
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuSaveCRClick(Sender: TObject);
begin
     if not ifMsgDlg(SaveChangesMsg) then exit;
     if dm.ibdsBulkCardRec.State = dsEdit then
        dm.ibdsBulkCardRec.Post;
     try
        with dm.ibdsBulkCardRec do begin
           DisableControls;
           first;
           while not EOF do begin
              if TPaymentCardTypes = pcBulk then begin
                 if FieldByName('PRINT_CARD').AsString = varYes then begin
                    edit;
                    FieldByName('PRINT_CARD').AsString := varNo;
                    post;
                 end;   
              end;   
              if TPaymentCardTypes = pcBulk then begin
                 if FieldByName('PRINT_RECEIPT').AsString = varYes then begin
                    edit;
                    FieldByName('PRINT_RECEIPT').AsString := varNo;
                    post;
                 end;   
              end;   
              next;
           end;   
           EnableControls;
        end;   
        CommitAndOpenTables;
        ResetBulkGrid;
     except
        dm.IBTransPay.RollBackRetaining;
        sm(RolledBackPaymentMsg);
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgBulkUpdateFooter(Sender: TObject);
//const SQL1 = 'select sum(pd.AMOUNT_PAID) as AMOUNT_PAID ' +
//             ' from MEMBERS m, PAYMENTS_DET pd where m.MEMBER = pd.MEMBER ' +
//             ' and pd.PAY_DATE between ''%s'' and  ''%s'' ';
             
begin
    // dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[GetShortDate(dtpDateFrom.Date),GetShortDate(dtpDateTo.Date) ]));
    // dbgBulk.ColumnByName('AMOUNT_PAID').FooterValue :=
    //       FloatToStrF(dm.sqlTransPay.FieldByName('AMOUNT_PAID').AsFloat,ffcurrency,7,2);
     //dbgBulk.ColumnByName('AMOUNT').FooterValue :=
      //     FloatToStrF(dm.sqlTransPay.FieldByName('AMOUNT').AsFloat,ffcurrency,7,2);
end; 
{ **************************************************************************** }

{ **************************************************************************** }
procedure TfrmMembersPAY.btnExportReceiptsClick(Sender: TObject);
{type
  TExportRec = Record         
    Receipt     : array [1..20] of Char;
    Comma1      : Char;
    ExpDate     : array [1..20] of Char;
    Comma2      : Char;
    Member      : array [1..20] of Char;
    Comma3      : Char;
    FName       : array [1..25] of Char;
    Comma4      : Char;
    LName       : array [1..25] of Char;
    Comma5      : Char;
    Address1    : array [1..50] of Char;
    Comma6      : Char;
    Address2    : array [1..50] of Char;
    Comma7      : Char;
    Address3    : array [1..50] of Char;
    Comma8      : Char;
    Address4    : array [1..50] of Char;
    Comma9      : Char;
    Address5    : array [1..50] of Char;
    Comma10     : Char;
    Paid        : array [1..10] of Char;
    Comma11     : Char;
    PaidBy      : array [1..10] of Char;
    Comma12     : Char;
    SendReceipt : array [1..20]  of Char;
    Comma13     : Char;
    GSTContent  : array [1..20] of Char;
    LFCR        : array[1..2] of char;

end; }
var //ExportRec : TExportRec;
    ExportReceipt : TextFile;
    //ExportReceipt : File of TExportRec;
    s,tempFieldName : shortstring;
    AddressList : TStringList;
    i : smallint;
begin
       if dm.ibdsBulkCardRec.State = dsEdit then dm.ibdsBulkCardRec.Post;
       if TButton(Sender).Name =  btnExportReceipts.Name then
          tempFieldName := 'PRINT_RECEIPT'
       else   
          tempFieldName := 'PRINT_CARD';
       if dm.SaveDialog.Execute then begin
        try
           AssignFile(ExportReceipt,dm.SaveDialog.FileName);
           Rewrite( ExportReceipt );
           AddressList := TStringList.Create;
           s := 'Receipt,Date,Member#,FName,LName,Address1,Address2,Address3,Address4,Address5,' +
                'Paid,PaidBy,SendReceipt,GSTContent';
           Writeln(ExportReceipt,s);
           with dm.ibdsBulkCardRec do begin
              First;
              while not EOF do begin
                 if FieldByName(tempFieldName).AsString = 'Y' then begin
                    s := '';
                    s := FieldByName('PAYMENT').AsString + ',';
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
                    s := s + FormatFloat('####.00',FieldByName('AMOUNT_PAID').AsFloat) + ',';  
                    s := s + FieldByName('TENDER_TYPE').AsString + ',';  
                    s := s + 'Yes' + ',';
                    //s := s + FormatFloat('####.00',(FieldByName('AMOUNT_PAID').AsFloat - (FieldByName('AMOUNT').AsFloat / GSTRate))) + ','; 
                    s := s + FormatFloat('####.00',FieldByName('AMOUNT_PAID').AsFloat / GSTRate);                    
                    Writeln(ExportReceipt,s);
                 end;
                 next;
              end;
           end;
           MDlgI(CreatedFileMsg + dm.SaveDialog.Filename);
        finally
           CloseFile( ExportReceipt );
           AddressList.Free;
        end;
     end;
     { //with dm.OpWord.OpenDocument('d:\inv.doc') do begin
         dm.qryReport.Close;
         dm.qryReport.SQL.Text := 'select First_Name as First_Name,Surname as Surname,Address1 as Address1 from members ' +
            ' where Auto_payment = ''Y''';
         dm.qryReport.open;
         dm.dsrBuldCardRec
         
        //dm.OpDatasetModel.DataSet := dm.qryReport;
        // MailMerge.OfficeModel := dm.OpDatasetModel;
        // PopulateMailMerge;
        //MailMerge.AsMailMerge.Destination :=   wdSendToNewDocument;
         //ExecuteMailMerge;
       //end;
       //exit;
      try
        TooBusy ('Exporting to Word ...');
        with dm.OpWord do begin
          if not Connected  then begin
            Connected := True;
            Visible := True;
          end;
          Documents[0].PopulateMailMerge;
          Documents[0].MailMerge.AsMailMerge.Destination := wdSendToNewDocument;
          Documents[0].ExecuteMailMerge;

          //MergeDoc := OpenDocument('d:\inv.doc');
           
          //dm.OpDatasetModel.DataSet := dm.ibdsBulkCardRec;
          //MergeDoc.MailMerge.OfficeModel := dm.OpDatasetModel;
          //dm.OpDatasetModel.DataSet := dm.qryReport;
          //MergeDoc.DocFile := 'd:\inv.doc';
          //MergeDoc.PopulateMailMerge;
          //MergeDoc.MailMerge.AsMailMerge.Destination := wdSendToNewDocument;
          //MergeDoc.ExecuteMailMerge;
        end;
     finally
        NotBusy;
       // if Assigned(MergeDoc) then
       //    MergeDoc.Free;
        dm.OpWord.Connected := False;
     end;     }
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuAddMemberCardClick(Sender: TObject);
var CardMembers : TCardMembers;
begin                                 
     if dm.ibdsPayDet.RecordCount = 0 then exit;
     CardMembers.AddMemberInCardMembers(dm.ibdsPayDet.FieldByName('MEMBER').AsInteger,1);
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.btnExportCardsClick(Sender: TObject);
const msg1 = '%d Members added for a Card';
var i : smallint;
begin              
     with dm.ibdsBulkCardRec do begin
        if State = dsEdit then Post;
        try
           DisableControls;
           First;
           i := 0;
           dm.tblCardMember.Open;
           while not EOF do begin
              if FieldByName('PRINT_CARD').AsString = 'Y' then begin
                 dm.tblCardMember.Insert;
                 dm.tblCardMember.FieldByName('MEMBER').AsInteger := FieldByName('MEMBER').AsInteger;
                 if FieldByName('NUMBER_CARDS').IsNull then
                    dm.tblCardMember.FieldByName('NUMBER_CARDS').AsInteger := 1
                 else   
                    dm.tblCardMember.FieldByName('NUMBER_CARDS').AsInteger := FieldByName('NUMBER_CARDS').AsInteger;
                 dm.tblCardMember.Post;
                 inc(i)
              end;
              next;
           end;
           dm.IBCommitRetain;
           MdlgI(format(msg1,[i]));
        finally
           dm.tblCardMember.Close;
           EnableControls;
        end;   
     end;
end;
{ **************************************************************************** }

procedure TfrmMembersPAY.dtpDateFromChange(Sender: TObject);
begin
     if TwwDBDateTimePicker(Sender).Modified then
       TwwDBDateTimePicker(Sender).DisplayFormat := 'dd/mm/yyyy';
end;
 { **************************************************************************** }
procedure TfrmMembersPAY.menuStartAPClick(Sender: TObject);
{ Auto Payments rules
 Only allow members in with Auto Payments and
 then Amount is > 0.00 and Amount is less than the AmountPaid.
 They have to pay in Pay by Member to get the setup expires etc.
 Then months 2 to 12 can be done in here.
}
begin
     PageControl.ActivePageIndex := 4;
     GetAutoPayMembers('Y');
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.GetAutoPayMembers( theChar : char);
var Reselect : Boolean;
    CardList : TStringList;
    i : Integer;
    //YorN : char;
{const SQL1 =
'select M.MEMBER,M.FIRST_NAME,M.SURNAME,M.FULL_NAME,MS.SUBSCRIPTION,MS.AMOUNT_PAID,' +
'MS.AMOUNT,M.SUB_PAID,MS.PAY,M.PAYMENT_PERIOD,M.AUTO_PAYMENT,S.DESCRIPTION,MS.EDIT_DATE,' +
'MS.RECEIPT,MS.PAID,M.PAYMENT_DAY,MS.PRINT_CARD,M.EXPIRES ' +
'from MEMBERS M, SUBSCRIPTIONS S,M_SUBS MS where M.AUTO_PAYMENT = ''%s'' '+
'AND S.SUBSCRIPTION = MS.SUBSCRIPTION and M.MEMBER = MS.MEMBER order by m.%s';}
begin
     with dm.ibdsAutoPay do begin
        // Surname Member
        if Active and ( State = dsEdit) then Post;
        if Active and (RecordCount  > 0) then
           Reselect := True
        else
           Reselect := False;
        close;
        if pos('Order by', SelectSQL.Text) = 0 then
           SelectSQL.Add( ' Order by ' + rpgAutoPay.Items[rpgAutoPay.ItemIndex]);
        ParamByName('AUTOPAYMENT').AsString := theChar;
        //addToClipboard(SelectSQL.Text);
        Open;
        if not Reselect then begin
           try
              DisableControls;
              First;
              CardList := TStringList.Create;
              while not EOF do begin
                 Edit;
                 // watch out for 0 Payments
                 if FieldByName('AMOUNT').AsFloat > 0 then
                    FieldByName('PAID').AsString := varYes
                 else   
                    FieldByName('PAID').AsString := varNo;
                 // DIVIDE BY THE Payment Period
                 if FieldByName('PAYMENT_PERIOD').AsInteger > 0 then
                    FieldByName('PAY').AsFloat := FieldByName('AMOUNT').AsFloat / FieldByName('PAYMENT_PERIOD').AsInteger
                 else 
                    FieldByName('PAY').AsFloat := FieldByName('AMOUNT').AsFloat;
                 // if card is one payment out from being finalised ( 1 month ) 
                 if (FieldByName('PAY').AsFloat + FieldByName('AMOUNT_PAID').AsFloat >= FieldByName('AMOUNT').AsFloat) and (FieldByName('AMOUNT').AsFloat > 0) then begin
                    FieldByName('PRINT_CARD').AsString := varYes;
                    // keep a list of the Members that need a card as some of the zero priced children may require them
                    CardList.Add(FieldByName('MEMBER').AsString);
                 end else begin  
                    FieldByName('PRINT_CARD').AsString := varNo;
                 end;   
                 // use edit_date as PAYMENT_DATE
                 FieldByName('EDIT_DATE').AsDateTime := dbdtSetPayDay.Date;
                 Post;
                 Next;
              end;
              First;
              // Update Children( zero priced ) if the Parents Card is Ticked
              while not EOF do begin
                 for i := 0 to CardList.Count -1 do begin
                    if CardList[i] = FieldByName('MAILING_LIST_CODE').AsString then begin
                       Edit;
                       FieldByName('PRINT_CARD').AsString := varYes;
                       // make a receipt for Zero Priced Children with new expires date
                       FieldByName('PAID').AsString := varYes;
                       Post;
                    end;   
                 end;
                 Next;
              end;
           finally
              if assigned(CardList) then 
                 CardList.Free;
              EnableControls;
              First;
              dbgAutoPayUpdateFooter(dbgAutoPay);
           end;   
        end;   
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuSaveAPClick(Sender: TObject);
var MemberPayments : TMemberPayments;
    ExpDate : TDateTime;
    CardMembers : TCardMembers;
    firstReceiptNumber,i,j,CommaCtr : Integer;
    AutoPayList, ParentCodeList : TStringList;
    Member, Expires ,s : String;
const  SQL2 = 'UPDATE MEMBERS SET EXPIRES = ''%s'', SUB_PAID = 1 where MEMBER = %s';
       //SQL3 = 'UPDATE M_SUBS SET AMOUNT_PAID = 0.00,AMOUNT = %f where MAILING_LIST_CODE = %d';
       SQL1 = 'UPDATE m_subs ms set ms.AMOUNT_PAID = 0.00, ms.AMOUNT = (select s.FEE from subscriptions s ' +
              'where ms.SUBSCRIPTION = s.SUBSCRIPTION ) where ms.MEMBER = %s';
       SQL4 = 'Select m.Member,m.Mailing_List_Code,ms.Amount from Members m, M_Subs ms where m.Member = ms.Member ' +
              'and m.Mailing_List_Code = %d';       
       SQL5 = 'update payments_det pd set pd.expires = ( select m.expires from members m where m.member = pd.member ) ' +
              'where pd.payment between %d and %d ';
begin
     if MessageDlg('Save the changes to the Members Subscriptions?',mtConfirmation,[mbYes,mbNo],0) = mrNo then exit;
     with dm.ibdsAutoPay do begin
        if State = dsEdit then Post;
        First;
        try
           DisableControls;
           AutoPayList := TStringList.Create;
           ParentCodeList := TStringList.Create;
           
           try
              firstReceiptNumber := 0;
              while not EOF do begin
                 if FieldByName('PAID').AsString = 'Y' then begin
                    FReceiptNumber := MemberPayments.GetReceiptNumber(dm.sqlTransPay);
                    If firstReceiptNumber = 0 then
                       firstReceiptNumber := FReceiptNumber;
                    MemberPayments.PaymentUpdate(dm.sqlTransPAy,FReceiptNumber,FieldByName('MEMBER').AsInteger,
                                   FieldByName('PAY').AsFloat,0.00,0.00,0.00,0.00,
                                   FieldByName('EDIT_DATE').AsDateTime,FieldByName('FULL_NAME').AsString,'');
                    MemberPayments.PaymentDetailInsert( dm.sqlTransPay,FReceiptNumber,FieldByName('MEMBER').AsInteger,
                                   FieldByName('MEMBER').AsInteger,FieldByName('SUBSCRIPTION').AsInteger,
                                   ord(TSubPayTypes(spPart)),ord(TPayTypes(ptSubs)),
                                   FieldByName('AMOUNT').AsFloat,FieldByName('PAY').AsFloat,0.00,
                                   FieldByName('EDIT_DATE').AsDateTime,FieldByName('DESCRIPTION').AsString,'',
                                   GetShortDate(FieldByName('EXPIRES').AsDAteTime),
                                   FieldByName('FULL_NAME').AsString,'');
                    MemberPayments.PaymentTenderInsert( dm.sqlTransPay,FReceiptNumber,
                                   'Direct Debit','',FieldByName('PAY').AsFloat,0.00);
                    edit;
                    // credit amounts OK
                    FieldByName('AMOUNT_PAID').AsFloat := FieldByName('AMOUNT_PAID').AsFloat +
                                                          FieldByName('PAY').AsFloat;
                    FieldByName('RECEIPT').AsInteger   := FReceiptNumber;
                    Post;
                    // Save Card may require a Card - but expires is wrong
                    if FieldByName('PRINT_CARD').AsString = varYes then begin
                       // Print with next years Expiry Date on the Card, Expires must be within 6 months of final payment
                       if FieldByName('EXPIRES').AsDateTime > Date + 180 then 
                          ExpDate := FieldByName('EXPIRES').AsDateTime
                       else   
                       
                          ExpDate := Global.AddYears(FieldByName('EXPIRES').AsDateTime,1);
                       AutoPayList.Add(FieldByName('MEMBER').AsString + ',' + GetShortMonthDate(ExpDate));
                    end;   
                    { ****************************************************************************}
                    // Amount Paid now has Pay added in and if >= (rounding ) Fee then add 1 year to expires
                    // Need to get the Children as well - watch out for zero price fees
                    if (FieldByName('AMOUNT_PAID').AsFloat >= FieldByName('AMOUNT').AsFloat) and (FieldByName('AMOUNT').AsFloat > 0 ) then begin
                       // update members expires,Expires must be within 6 months of final payment 
                       if FieldByName('EXPIRES').AsDateTime > Date + 180 then 
                          ExpDate := FieldByName('EXPIRES').AsDateTime
                       else   
                          ExpDate := Global.AddYears(FieldByName('EXPIRES').AsDateTime,1);   
                       // Get Parent and Children
                       dm.sqlExecQuery(dm.sqlTransPay,format(SQL4,[FieldByName('MEMBER').AsInteger]));
                       ParentCodeList.Clear;
                       While not dm.sqlTransPay.EOF do begin
                          // Dont wont a second priced Child
                          // Zero Priced Children
                          if (dm.sqlTransPay.FieldByName('AMOUNT').AsFloat = 0) and (dm.sqlTransPay.FieldByName('MEMBER').AsInteger <> FieldByName('MEMBER').AsInteger) then
                             ParentCodeList.Add(dm.sqlTransPay.FieldByName('MEMBER').AsString);
                          // Parent Code Member
                          if (dm.sqlTransPay.FieldByName('MEMBER').AsInteger = FieldByName('MEMBER').AsInteger) then
                             ParentCodeList.Add(dm.sqlTransPay.FieldByName('MEMBER').AsString);
                          dm.sqlTransPay.Next;                        
                       end;
                       // update each Parent Code and Children
                       for i := 0 to ParentCodeList.Count -1 do 
                          dm.sqlExecQuery(dm.sqlTransPay,format(SQL2,[GetShortDate(ExpDate) ,ParentCodeList[i]]));
                       // update M_Subs and get the latest Fee for Parent Code and Children
                       for i := 0 to ParentCodeList.Count -1 do 
                          dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[ParentCodeList[i] ]));
                       // log export
                       for i := 0 to ParentCodeList.Count -1 do 
                          dm.LogExport(dm.sqlTransPay,StrToInt(ParentCodeList[i]),0,False);
                    end;
                 end;
                 next;
              end;
              // update expires in Payments Detail 
              dm.sqlExecQuery(dm.sqlTransPay,format(SQL5,[firstReceiptNumber,FReceiptNumber ]));
              if dm.IBTransPay.InTransaction then begin
                 dm.IBTransPay.Commit;
                 dm.IBTransPay.StartTransAction;
              end;
              dm.tblCardDetail.Open;
              // print cards from the List - Member,Expires in AutoPayList
              // Do Cards before the Report ( RPNYC )
              if AutoPayList.Count > 0 then begin
                 for i := 0 to AutoPayList.Count -1 do begin
                    s := AutoPayList[i];
                    CommaCtr := pos(',',S);
                    Member  := copy(s,1,CommaCtr -1);
                    Expires := copy(s,CommaCtr + 1,Length(s) - (CommaCtr));
                    Global.CardPrint(Member,Expires,1,dm.IBTransPay);
                    for j := 0 to 500 do
                       // a slight pause maybe between each card
                 end;
              end;
              // print a report
              frmDirectDebit := TfrmDirectDebit.Create(nil);
              try
                if frmDirectDebit.Extract(firstReceiptNumber,FReceiptNumber) then
                   frmDirectDebit.qr.Preview;
              finally
                frmDirectDebit.Free;
                dm.qryReport.Close;
              end;
              
              GetAutoPayMembers( 'Z' );
           except
              sm('The changes to Auto Payments have NOT been accepted');
              dm.IBTransPay.Rollback;
              dm.IBTransPay.StartTransAction;
              open;
           end;
        finally
           dm.tblCardDetail.Filtered := False;
           dm.tblCardDetail.Close;
           AutoPayList.free;
           ParentCodeList.free;
           EnableControls;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgAutoPayCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;  ABrush: TBrush);
var    Yr, Mnth, Day,ExpYr, ExpMnth, ExpDay: Word;
begin
     if dm.ibdsAutoPay.RecordCount = 0 then exit;
     if (Field.FieldName = 'PAY') or
        (Field.FieldName = 'EDIT_DATE') or
        (Field.FieldName = 'PRINT_CARD') or
        //(Field.FieldName = 'EXPIRES') or
        (Field.FieldName = 'PAID') then
        ABrush.Color := Global.fcGridEdit;
        //if NextMonthExpires( dm.ibdsAutoPay.FieldByName('EXPIRES').AsDateTime )  then
     if (dm.ibdsAutoPay.FieldByName('PAID').AsString = varYes) and (dm.ibdsAutoPay.FieldByName('PAY').AsFloat + dm.ibdsAutoPay.FieldByName('AMOUNT_PAID').AsFloat >= dm.ibdsAutoPay.FieldByName('AMOUNT').AsFloat) then
        ABrush.Color := clYellow;
     //if (Field.FieldName = 'PRINT_CARD') then begin
     //   if dm.ibdsAutoPay.FieldByName('PRINT_CARD').AsString = varYes then
     //      ABrush.Color := clGray;
     //      
     //end;
end; 
{ **************************************************************************** }
procedure TfrmMembersPAY.menuCancelAPClick(Sender: TObject);
begin
      if IfMsgDlg(CancelClearMsg) then begin
        dm.IBRollBackStart(dm.IBTransPay);
        GetAutoPayMembers( 'Z' );     
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgAutoPayIButtonClick(Sender: TObject);
begin
     if dm.ibdsAutoPay.RecordCount = 0 then exit;
     try
       frmMembersLook := TfrmMembersLook.Create(self);
       frmMembersLook.LookupLinks := False;
       frmMembersLook.Member      := dm.ibdsAutoPay.FieldByName('MEMBER').AsInteger;
       frmMembersLook.PayExpires  := Global.AddYears(dm.ibdsAutoPay.FieldByName('EXPIRES').AsDateTime,-1);
       frmMembersLook.ShowModal;
     finally
       frmMembersLook.Free;
     end;  
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuCancelCRClick(Sender: TObject);
begin
     if IfMsgDlg(CancelClearMsg) then begin
        dm.IBRollBackStart(dm.IBTransPay);
        ResetBulkGrid;
     end; 
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.ResetBulkGrid;
const SQL1 = 'select pd.AMOUNT_PAID,pd.DESCRIPTION,pd.EXPIRES, pd.FULL_NAME,pd.INVOICE_NUMBER,' +
             'pd.MEMBER,pd.PAY_DATE, pd.PAYMENT,m.PAYMENT_PERIOD, m.AUTO_PAYMENT,' +
             'm.BARCODE_NUMBER,m.PRINT_CARD,m.PRINT_RECEIPT,m.NUMBER_CARDS, ' +
             'm.FIRST_NAME,m.SURNAME,pt.TENDER_TYPE,m.ADDRESS,m.ADDRESS1,m.DISTRICT, ' +
             ' m.CITY, m.COUNTRY ' +
             'from MEMBERS m, PAYMENTS_DET pd , PAYMENTS_TEND pt ' +
             'where m.MEMBER = pd.MEMBER and m.member = -1';
begin
     dm.ibdsBulkCardRec.SelectSQL.TEXT := SQL1;
     dm.ibdsBulkCardRec.Open;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbeDiscountExit(Sender: TObject);
var TempTotal : Extended;
begin
     // if the user chooses a discount
     TempTotal := FJoiningFeeDue + FSubsDue + FSectionDue
                 - RemoveSignSpaceConvert(dbeDiscount.Text) - FPointsUsed;//FDiscount;
     dbeTotal.Text := FloatToStrF( TempTotal,ffCurrency,7,2);
     TempTotal := RemoveSignSpaceConvert(dbeDiscount.Text);  //FDiscount;
     dbeDiscount.Text := FloatToStrF( TempTotal,ffCurrency,7,2);
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuPrintInvoiceClick(Sender: TObject);
var InvoiceStr : shortstring;
    Disc : Extended; 
    PartPay : shortInt;
const  SQL1 = 'update MEMBERS set PAYMENT_DISC = %f,PAYMENT_DAY = %s where MAILING_LIST_CODE = %d';
begin
     with dm.ibdsPayDet do begin
        if State = dsEdit then 
           post;
        if RecordCount = 0 then exit;
        if RemoveSignSpaceConvert(dbeTotal.Text) = 0.00 then begin
           sm(NoTotalPaymentMsg);
           exit;
        end;   
        try
           PartPay := StrToInt(dbePartPay.Text);  
           Disc    := RemoveSignSpaceConvert(dbeDiscount.Text);
           dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[RemoveSignSpaceConvert(dbeDiscount.Text),dbePartPay.Text,FKeepMailingListCode]));
           DisableControls;
           InvoiceStr := '';
           First;
           while not EOF do begin
              if FieldByName('PAY').AsFloat > 0.00 then begin
                 // may get the member twice
                 InvoiceStr := InvoiceStr + FieldByName('MEMBER').AsString + ',';
                 Edit;
                 FieldByName('INVOICED').AsString := varYes;
                 Post;
              end;   
              next;   
           end;
           system.delete(InvoiceStr,length(InvoiceStr),1);
           try
              CommitAndOpenTAbles;             
              //ClearPayFields;
             // refresh the Pay by MEMBER 
             dm.ibdsPayMembers.Close;
             dm.ibdsPayMembers.Params.ByName('MEMBER').AsInteger := FKeepMailingListCode;
             dm.ibdsPayMembers.Open;
              
           except                                    
              ClearPayFields;
              sm(RolledBackPaymentMsg);
              exit;
           end;   
        finally
           EnableControls;
        end;
     end;   
     // now the Invoice
     try
        InSaveMode := True;
        frmInvoiceLandscape := TfrmInvoiceLandscape.create(self);
       if not frmInvoiceLandscape.ExtractMember(Global.ReportPrinterIndex,PartPAy,InvoiceStr,Disc ) then 
          exit;
        //frmInvoiceLandscape.qr.Preview;
        frmInvoiceLandscape.qr.Print;
        InSaveMode := False;
     finally
         frmInvoiceLandscape.free;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.rpgAutoPayChange(Sender: TObject;   ButtonIndex: Integer);
begin
     case ButtonIndex of 
       0 : incAutoPayments.SearchField := 'SURNAME';
       1 : incAutoPayments.SearchField := 'MEMBER';
     end;
end;
{ **************************************************************************** }

procedure TfrmMembersPAY.incAutoPaymentsKeyPress(Sender: TObject; var Key: Char);
begin
     if key = #13 then begin
        incAutoPayments.FindValue;
        incAutoPayments.Clear;
        dbgAutoPay.Setfocus;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbeDayToKeyPress(Sender: TObject; var Key: Char);
begin
     if key = #13 then menuStartAPClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.btnSaveSectionClick(Sender: TObject);
begin
     case PageControl.ActivePageIndex of
        1 : menuSavetheSectionClick(Sender);
        3 : menuSaveCRClick(Sender);
        4 : menuSaveAPClick(Sender);
     end;  
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.btnClearClick(Sender: TObject);
begin
     case PageControl.ActivePageIndex of
         0 : begin
              menuClearSubsClick(Sender);
              Self.Caption := ScreenName;
             end; 
         1 : menuClearSectionClick(Sender);
         3 : menuCancelCRClick(Sender);
         4 : menuCancelAPClick(Sender); 
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.btnStartClick(Sender: TObject);
begin
     case PageControl.ActivePageIndex of 
        3 : menuStartCRClick(Sender);
        4 : menuStartAPClick(Sender);
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuClearSectionClick(Sender: TObject);
begin
    if IfMsgDlg(CancelClearMsg) then begin
        stbStatus.Panels[0].Text := '';
        ClearSectionFields;
        SectionReceiptList.Clear;
        btnPrint.Enabled := True;
        btnPreview.Enabled := True;
     end; 
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbeProRataCloseUp(Sender: TwwDBComboBox;  Select: Boolean);
begin
     // recalc the pro rata payment
     FPartPayUserModified := True;
     CalcSubPaySetup;
     CalcTotal;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuViewSubscriptionsGridClick(Sender: TObject);
begin
    { case TMenuItem(Sender).Checked of
        True  : TMenuItem(Sender).Checked := False;
        False : TMenuItem(Sender).Checked := True;
     end;
     if TMenuItem(Sender).Name = menuViewJoiningFeeGrid.Name then 
        pnlJoinFee.Visible := menuViewJoiningFeeGrid.Checked;
     if TMenuItem(Sender).Name = menuViewSubscriptionsGrid.Name then 
        pnlSubs.Visible := menuViewSubscriptionsGrid.Checked;
     if TMenuItem(Sender).Name = menuViewSectionsGrid.Name then
        pnlSections.Visible := menuViewSectionsGrid.Checked;   }
end;                                                        
{ **************************************************************************** }
procedure TfrmMembersPAY.menuUndoInvoiceClick(Sender: TObject);
begin
     if not IfMsgDlg('Undo Invoice Information?') then exit;
     try
        InSaveMode := True;
        with dm.ibdsPayDet do begin
            first;
            while not EOF do begin
               edit;
               FieldByName('INVOICED').AsString := varNo;
               Post;
               next;
            end;  
            try
              CommitAndOpenTAbles;
              CalcSubPaySetup; 
            except                                     
              ClearPayFields;
              sm(RolledBackPaymentMsg);
              exit;
            end;  
        end;
     finally
        InSaveMode := False;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.btnSaveReceiptsClick(Sender: TObject);
// save changes to receipts
begin
     if not ifMsgDlg(SaveChangesMsg)   then  exit;
     if dm.ibdsPay_Det.State = dsEdit  then  dm.ibdsPay_Det.Post;
     if dm.ibdsPayments.State = dsEdit then  dm.ibdsPayments.Post;
     try
       CommitAndOpenTables;
     except
        sm('The Receipt changes have not been accepted');
        dm.IBTransPay.Rollback;
        dm.IBTransPay.StartTransaction;
        OpenTables;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgAutoPayUpdateFooter(Sender: TObject);
const SQL1 = 'select sum(ms.PAY) as PAY from M_SUBS ms,MEMBERS m where m.MEMBER = ms.MEMBER and ms.PAID = ''Y'' ' +
             'and ms.AMOUNT_PAID > 0.00 and m.EXPIRES > CURRENT_DATE and m.AUTO_PAYMENT = ''Y'' and MS.AMOUNT_PAID < MS.AMOUNT';
begin
     if not active then exit;
     dm.sqlExecquery(dm.sqlTransPay,SQL1);
     dbgAutoPay.ColumnByName('Pay').FooterValue :=
        FloatToStrF(dm.sqlTransPay.FieldByName('PAY').AsFloat, ffCurrency, 10, 2);
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbePointsEnter(Sender: TObject);
begin
     dbePoints.SelectAll;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbePointsDblClick(Sender: TObject);
begin
     if dm.ibdsPayMembers.FieldByName('POINTS_AVAILABLE').AsFloat > RemoveSignSpaceConvert(dbeTotal.Text) then begin
        dbePoints.Text :=  dbeTotal.Text;
     end else begin
        dbePoints.Text := formatFloat('$##0.00',dm.ibdsPayMembers.FieldByName('POINTS_AVAILABLE').AsFloat);
     end;
     SelectNext(ActiveControl as TWinControl,True,True);
     CalcTotal;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.CalcPoints;
var Points : Extended;
//    s : String;
begin
     {s := dbePoints.Text;
     if POS('$',s) > 0 then
       system.delete(s,POS('$',s),1);
     P := StrToCurr(s);
     PA := dm.ibdsPayMembers.FieldByName('POINTS_AVAILABLE').AsFloat;
     // Need to have this comparison for small amounts - some rounding error
     if P > PA then begin
        if not IfMsgDlg ( 'Member has not enough points, Override') then begin
           dbePoints.Text := '0.00';
           dbePoints.SetFocus;
           exit;  
        end;   
     end;         }
     if dm.ibdsPayMembers.FieldByName('POINTS_AVAILABLE').AsFloat = 0.00 then begin
        dbePoints.Text := '0.00';
       // sm ( 'Member has no points to use');
     end;
     if (FSubsDue + FJoiningFeeDue + FSectionDue) < 0.00 then begin
       dbePoints.Text := '0.00';
       sm('Cannot use points on a Credit Payment');
     end;
     if dbePoints.Text = '' then
        dbePoints.Text := '0.00';
     if RemoveSignSpaceConvert(dbePoints.Text) > RemoveSignSpaceConvert(dbeTotal.Text) then begin
        sm( 'Points amount is greater than the total payment!');
        dbePoints.Setfocus;
        exit;  
     end;   
     // Howick Request MenuMate sites have points that are up to date
     if MenuMateInstalled and ( RemoveSignSpaceConvert(dbePoints.Text) > dm.ibdsPayMembers.FieldByName('POINTS_AVAILABLE').AsFloat ) then begin 
        dbePoints.Text := '0.00';
        sm( 'Points amount is greater than the total of the Members Points!');
        exit;  
     end;
     Points := RemoveSignSpaceConvert(dbePoints.Text);
     dbePoints.Text := FloatToStrF( Points,ffcurrency,7,2);
     //SelectNext(ActiveControl as TWinControl,True,True);
     CalcTotal;
end;
{ ****************************************************************************}
procedure TfrmMembersPAY.dbePointsExit(Sender: TObject);
begin
     SelectNext(ActiveControl as TWinControl,True,True);
     CalcPoints;                               
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.menuMemberBriefClick(Sender: TObject);
var SaveMemberList : TStringList;
    PaidByMember : LongInt;
begin
     if PageControl.ActivePageIndex <> 0 then exit;
     if dm.ibdsPayDet.RecordCount = 0 then exit;
     frmMembersEditBrief := TfrmMembersEditBrief.Create(Self);
     try
        frmMembersEditBrief.Setup(CheckedFieldsList);
        frmMembersEditBrief.Showmodal;
        // Refreshes Members Details - Expires cahnge only
        if frmMembersEditBrief.NeedsFormActivate then begin
            try
               PaidByMember := dm.ibdsPayMembers.FieldByName('MEMBER').AsInteger;
               SaveMemberList := TStringList.Create;
               SaveMemberList.Assign(MemberList);
               ClearPayFields;
               MemberList.Assign(SaveMemberList);
            finally
               SaveMemberList.Free;
            end;
            FDoneMembersEditBrief := True;
            PopulatePayGrid;
            // populate paid by member
            dm.ibdsPayMembers.Close;
            dm.ibdsPayMembers.Params.ByName('MEMBER').AsInteger := PaidByMember;
            dm.ibdsPayMembers.Open;
        end;
        if frmMembersEditBrief.ChangeCheckedFields then begin
           BuildCheckedFieldsList;
        end;
     finally
        frmMembersEditBrief.free;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgSubPayDblClick(Sender: TObject);
begin
    menuMemberBriefClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.BuildCheckedFieldsList;
var i : smallint;
begin
     // Build a list of fields that need to be checked
     CheckedFieldsList.Clear;
     for i := 0 to Length(CheckedFields) -1 do begin
        if CheckedFields[i] = varYes then
           CheckedFieldsList.add( dm.ibdsMembersBrief.Fields[i].FieldName);
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersPAY.dbgSubPayRowChanged(Sender: TObject);
var //s : String[14];
    age,YrsJoined : Smallint;
begin
     with dm.ibdsPayDet do begin
        if RecordCount = 0 then exit;
        if DateTimeToStr(FieldByName('JOINED').AsDateTime) <> StrToDate1899 then begin
           YrsJoined := WorkOutAge(FieldByName('JOINED').AsDateTime);
        end else begin
           YrsJoined := 0;
        end;
        if DateTimeToStr(FieldByName('DATE_OF_BIRTH').AsDateTime) <> StrToDate1899 then begin
           age := WorkOutAge(FieldByName('DATE_OF_BIRTH').AsDateTime);
           //s := formatDateTime('dd mmm yyyy',FieldByName('DATE_OF_BIRTH').AsDateTime);                         
        end else begin
           age := 0;
           //s   := 'No DOB';
        end;   
        Self.Caption := ScreenName + GetAddressAsString(FieldByName('ADDRESS').AsString,
                                                 FieldByName('DISTRICT').AsString,
                                                 FieldByName('ADDRESS1').AsString,
                                                 FieldByName('CITY').AsString,
                                                 FieldByName('COUNTRY').AsString) + 
                                                 ' - ' + IntToStr(age) + ' yrs old' +
                                                // ' - ' + s + ' - ' + IntToStr(age) + ' yrs old' +
                                                 ' - ' +  IntToStr(YrsJoined) + ' yrs joined';
     end;     
end;
{ ****************************************************************************}
procedure TfrmMembersPAY.DBText1DblClick(Sender: TObject);
begin
     if dm.ibdsPayMembers.FieldByName('POINTS_AVAILABLE').AsFloat > RemoveSignSpaceConvert(dbeTotal.Text) then begin
        dbePoints.Text :=  dbeTotal.Text;
     end else begin
        dbePoints.Text := formatFloat('$##0.00',dm.ibdsPayMembers.FieldByName('POINTS_AVAILABLE').AsFloat);
     end;
     //SelectNext(ActiveControl as TWinControl,True,True);
     CalcTotal;
end;
{ ****************************************************************************}
procedure TfrmMembersPAY.btnLateFeeClick(Sender: TObject);
begin
     AddRemoveLateFeeSub;
end;
{ ****************************************************************************}
procedure TfrmMembersPAY.btnChangeSectionClick(Sender: TObject);
var  SaveMemberList : TStringList;
     Member,PaidByMember : LongInt;
     Section,i : Smallint;
     Fee : Extended;
     SectionExpires : ShortString;
const SQL1 = 'DELETE FROM M_SECTIONS WHERE MEMBER = %d and SECTION = %d';
      SQL2 = 'INSERT into M_SECTIONS (MEMBER,SECTION,EXPIRES,PAID,RECEIPT,SPONSORSHIP,' +
             'EDIT_DATE,AMOUNT,AMOUNT_PAID,SELECTED,PAY ) Values (%d,%d,''%s'',''N'',0,''Y'',Current_TimeStamp,%f,0,''N'',0)';
begin
     if dm.ibdsPayMemSec.RecordCount = 0 then exit;
     //AmountPaid   := dm.ibdsPayDet.FieldByName('AMOUNT_PAID').AsFloat;
     //if AmountPaid > 0.00 then begin
     //   if not ifMsgDlg(format('The Member has paid %f - you might have to alter'  + #13#10 +
     //                   'the Expires date manually if the fees increase/decrease - continue?',[AmountPaid])) then exit;
     //end;
     try
        frmMembersLookup := TfrmMembersLOOKUP.create(nil);
        frmMembersLookup.Setup( dm.dsrTblSections,dm.tblSections,         // sections
                        'SECTION',dm.ibdsPayMemSec.FieldByName('MEMBER').AsInteger);
        if (frmMembersLookup.ShowModal = mrOK) then begin
           if frmMembersLookup.MultiList.Count > 0 then begin
              // Save MemberList - gets cleared in ClearPayFields
              Member       := dm.ibdsPayMemSec.FieldByName('MEMBER').AsInteger;     
              Section      := dm.ibdsPayMemSec.FieldByName('SECTION').AsInteger;
              //AmountPaid   := dm.ibdsPaySecMem.FieldByName('AMOUNT_PAID').AsFloat;

              PaidByMember := dm.ibdsPayMembers.FieldByName('MEMBER').AsInteger;
              SaveMemberList := TStringList.Create;
              SaveMemberList.Assign(MemberList);
              ClearPayFields;
              MemberList.Assign(SaveMemberList);
              SaveMemberList.Free;
              try
                  // Delete the record
                  dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[Member,Section]));
                  //PrcssLog it
                  dm.ProcessType := ord(TProcessTypes(ptMembers));
                  dm.LogProcess ( dm.sqlTransPay,format('Section %d deleted from Member %d in Payments',[Section,Member]));
                  // Get the new Section
                  Section := dm.tblSections.FieldByName('SECTION').AsInteger;
                  SectionExpires := GetShortDate(dm.tblSections.FieldByName('EXPIRES').AsDateTime - 365);
                  // if there is no date for the Section
                  if dm.tblSections.FieldByName('EXPIRES').IsNull then
                     SectionExpires := GetShortDate(Date)
                  else if dm.tblSections.FieldByName('EXPIRES').AsDateTime <= Date then
                     SectionExpires := GetShortDate(dm.tblSections.FieldByName('EXPIRES').AsDateTime)
                  else
                     SectionExpires := GetShortDate(AddYears(dm.tblSections.FieldByName('EXPIRES').AsDateTime,-1));
                  Fee := GetLatestFee(SectionsTableName,FieldNameArray[1],'N',Section);
                  //Add Section Record
                  dm.sqlExecQuery(dm.sqlTransPay,format(SQL2,[Member,Section,SectionExpires,Fee]));
                  // PrcssLog it
                  dm.LogProcess ( dm.sqlTransPay,format('Section %d added for Member %d in Payments',[Section,Member]));
                  // commit work done
                  CommitAndOpenTables;
                  PopulatePayGrid;
                  // populate paid by member
                  dm.ibdsPayMembers.Close;
                  dm.ibdsPayMembers.Params.ByName('MEMBER').AsInteger := PaidByMember;
                  dm.ibdsPayMembers.Open;
                  dblcPaidBy.Text := dm.ibdsPayMembers.FieldByName('MEMBER').AsString;
                  dbgSubPay.Update;
                  dbgPaySecMem.Update;
              except
                 if dm.IBTransPay.InTransaction then begin
                    dm.IBTransPay.RollBack;
                    dm.IBTransPay.StartTransaction;
                 end;
              end;
           end;
        end;
     finally
        frmMembersLookup.Free;
     end;
//  
end;
{ ****************************************************************************}
procedure TfrmMembersPAY.dbgPaySecMemDblClick(Sender: TObject);
var  SaveMemberList : TStringList;
     Member,PaidByMember : LongInt;
     Section,i : Smallint;
     Fee : Extended;
     SectionExpires : ShortString;
const SQL1 = 'DELETE FROM M_SECTIONS WHERE MEMBER = %d and SECTION = %d';
begin
     if dm.ibdsPayMemSec.RecordCount = 0 then exit;
     // Save MemberList - gets cleared in ClearPayFields
     if not IfMsgDlg(format('Do you wish to delete Section %s',[dm.ibdsPayMemSec.FieldByName('DESCRIPTION').AsString])) then exit;
     Member       := dm.ibdsPayMemSec.FieldByName('MEMBER').AsInteger;     
     Section      := dm.ibdsPayMemSec.FieldByName('SECTION').AsInteger;
     PaidByMember := dm.ibdsPayMembers.FieldByName('MEMBER').AsInteger;
     SaveMemberList := TStringList.Create;
     SaveMemberList.Assign(MemberList);
     ClearPayFields;
     MemberList.Assign(SaveMemberList);
     SaveMemberList.Free;
     try
         // Delete the record
         dm.sqlExecQuery(dm.sqlTransPay,format(SQL1,[Member,Section]));
         //PrcssLog it
         dm.ProcessType := ord(TProcessTypes(ptMembers));
         dm.LogProcess ( dm.sqlTransPay,format('Section %d deleted from Member %d in Payments',[Section,Member]));
         // commit work done
         CommitAndOpenTables;
         PopulatePayGrid;
         // populate paid by member
         dm.ibdsPayMembers.Close;
         dm.ibdsPayMembers.Params.ByName('MEMBER').AsInteger := PaidByMember;
         dm.ibdsPayMembers.Open;
         dblcPaidBy.Text := dm.ibdsPayMembers.FieldByName('MEMBER').AsString;
         dbgSubPay.Update;
         dbgPaySecMem.Update;
     except
        if dm.IBTransPay.InTransaction then begin
           dm.IBTransPay.RollBack;
           dm.IBTransPay.StartTransaction;
        end;
     end;
end;


end.
