unit MembersQUERY;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm, StdCtrls, ComCtrls, Grids, Wwdbigrd, Wwdbgrid, ExtCtrls, Menus,
   ToolWin, lmdcctrl, lmdctrl, lmdgroup,DB,IBQuery, wwSpeedButton,
  wwDBNavigator, wwclearpanel,ClipBrd,OpExcel;

type
  TfrmMembersQUERY = class(TSMStdForm)
    Panel1: TPanel;
    Panel2: TPanel;
    PageControl: TPageControl;
    tsBuilder: TTabSheet;
    tsSQL: TTabSheet;
    btnCancel: TToolButton;
    tsResults: TTabSheet;
    dbgQueryResults: TwwDBGrid;
    tsTableData: TTabSheet;
    wwDBGrid2: TwwDBGrid;
    menuExit: TMenuItem;
    ToolButton2: TToolButton;
    btnPrintSQL: TToolButton;
    menuOptions: TMenuItem;
    menuExecuteQuery: TMenuItem;
    pnlSpace: TPanel;
    btnPrinterSetup: TToolButton;
    menub: TMenuItem;
    menuPrinterSetup: TMenuItem;
    menuPrinttheSQLStatement: TMenuItem;
    menubb: TMenuItem;
    menuLoadanSQLScript: TMenuItem;
    memoQuery: TRichEdit;
    dbnQry: TwwDBNavigator;
    dbnQryFirst: TwwNavButton;
    dbnQryPrior: TwwNavButton;
    dbnQryNext: TwwNavButton;
    dbnLast: TwwNavButton;
    btnOK: TToolButton;
    menuSaveSQLScript: TMenuItem;
    Panel3: TPanel;
    Panel6: TPanel;
    GroupBox3: TGroupBox;
    cbxFieldNames: TComboBox;
    cbxUseSorting: TCheckBox;
    cbxAscending: TComboBox;
    Panel7: TPanel;
    GroupBox4: TGroupBox;
    Panel8: TPanel;
    btnAdd: TButton;
    btnDelete: TButton;
    btnClearAll: TButton;
    Panel9: TPanel;
    lbxQueryLines: TListBox;
    Panel10: TPanel;
    Label1: TLabel;
    lbxTableNames: TListBox;
    Panel11: TPanel;
    Label2: TLabel;
    lbxFieldNames: TListBox;
    btnQueries: TToolButton;
    menuSaveSQLDataBase: TMenuItem;
    PageControl1: TPageControl;
    tsMatching: TTabSheet;
    edtMatching: TEdit;
    rgpOther: TLMDRadioGroup;
    tsRange: TTabSheet;
    Label5: TLabel;
    Label4: TLabel;
    edtFrom: TEdit;
    edtTo: TEdit;
    Label3: TLabel;
    Label6: TLabel;
    lblMatching: TLabel;
    lblRange: TLabel;
    menuPaste: TMenuItem;
    btnExportExcel: TToolButton;
    procedure FormCreate(Sender: TObject);
    procedure lbxTableNamesClick(Sender: TObject);
    procedure cbxUseSortingClick(Sender: TObject);
    //procedure rgpMatchingChange(Sender: TObject; ButtonIndex: Integer);
    function GetLbxTableName : String;
    function GetLbxFieldName ( WithQuotes : Boolean ) : String;
    procedure OpenTable(WithData : Boolean);
    procedure PageControlChange(Sender: TObject);
    procedure lbxFieldNamesClick(Sender: TObject);
    procedure menuExitClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure menuExecuteQueryClick(Sender: TObject);
    procedure menuLoadanSQLScriptClick(Sender: TObject);
    procedure menuPrinterSetupClick(Sender: TObject);
    procedure menuPrinttheSQLStatementClick(Sender: TObject);
    procedure lbxTableNamesStartDrag(Sender: TObject;
      var DragObject: TDragObject);
    procedure memoQueryDragOver(Sender, Source: TObject; X, Y: Integer;
      State: TDragState; var Accept: Boolean);
    procedure memoQueryDragDrop(Sender, Source: TObject; X, Y: Integer);
    procedure memoQueryKeyPress(Sender: TObject; var Key: Char);
    procedure edtFromKeyPress(Sender: TObject; var Key: Char);
    procedure btnAddClick(Sender: TObject);
    procedure rgpOtherChange(Sender: TObject; ButtonIndex: Integer);
    procedure btnDeleteClick(Sender: TObject);
    function GetSelected ( ListBox : TListBox ) : Integer ;
    procedure btnClearAllClick(Sender: TObject);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure btnOKClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure menuSaveSQLScriptClick(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure menuSaveSQLDataBaseClick(Sender: TObject);
    procedure PageControl1Change(Sender: TObject);
    procedure menuPasteClick(Sender: TObject);
    procedure btnExportExcelClick(Sender: TObject);
  private
    { Private declarations }
    MemberList : TStringList;
    MemberDescList : TStringList;
    KeepFieldList : TStringList; 
    FTableOrField,FKeepTableName : ShortString;
    Fieldtype : TFieldType;
    FNumeric,FDate,FNeedQuotes : Boolean;
    FMembersOnly : Boolean;
    FpnlWidth : SmallInt;
  public
    { Public declarations }
    property MembersOnly : Boolean Write FMembersOnly default False;
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;

var
  frmMembersQUERY: TfrmMembersQUERY;

implementation

{$R *.DFM}
uses
   Global,
   dmMembers, MembersLetter;
{ **************************************************************************** }
class function TfrmMembersQUERY.ScreenCode : Integer;
begin Result := Global.scMembersQuery; end;
{ **************************************************************************** }
class function TfrmMembersQUERY.ScreenName : String;
begin Result := 'Members Query'; end;
{ **************************************************************************** }
class function TfrmMembersQUERY.ScreenDesc : String;
begin Result := '  Members Query Screen'; end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.FormCreate(Sender: TObject);
const fltr = 'SQL Script files|*.sql|All Files|*.*';
begin
     inherited;
     FpnlWidth := pnlSpace.Width;
     SetupOpenDialog('*.sql',fltr,'Load an SQL Script file');
     SetupSaveDialog('*.sql',fltr,'Save an SQL Script file','my.sql','');
     MemberList := TStringList.Create;
     MemberDescList := TStringList.Create;
     KeepFieldList  := TStringList.Create; 
     FKeepTableName := '';
     MemberList.Add('ADDRESS');       MemberDescList.Add('Address ( 0 to 9, A to Z )');
     MemberList.Add('ADDRESS1');      MemberDescList.Add('Address ( 0 to 9, A to Z )');
     MemberList.Add('ALLOW_CREDIT');  MemberDescList.Add('Allow Credit ( Y or N )');
     MemberList.Add('AMOUNT_DONATED');MemberDescList.Add('Amount Donated ( > 0.00 )');
     MemberList.Add('ANALYSE');       MemberDescList.Add('Analyse ( Y or N )');
     MemberList.Add('APPROVED');      MemberDescList.Add('Approved ( Y or N ) ');
     MemberList.Add('BONUS_CREDIT');  MemberDescList.Add('Bonus Credit ( > 0.00 )');
     MemberList.Add('CARD_TYPE');     MemberDescList.Add('Card Type ( N,F,E or S )');
     MemberList.Add('CELL_PHONE');    MemberDescList.Add('Cell Phone ( 0 to 9 possibly - )');
     MemberList.Add('CHARGE_CODE');   MemberDescList.Add('Charge Code ( > 0 )');
     MemberList.Add('CREDIT_LIMIT');  MemberDescList.Add('Credit Limit ( > 0.00 )');
     MemberList.Add('DATE_OF_BIRTH'); MemberDescList.Add('Date of Birth ( mm/dd/yyyy ) ');
     MemberList.Add('DEBENTURE');     MemberDescList.Add('Debenture ( > 0 )');
     MemberList.Add('DECEASED');      MemberDescList.Add('Deceased ( Y or N )');
     MemberList.Add('DISCOUNT');      MemberDescList.Add('Discount ( 0.00 to 99.99 )');
     MemberList.Add('DISTRICT');      MemberDescList.Add('District ( A .. Z )');
     MemberList.Add('DONATED');       MemberDescList.Add('Donated ( A .. Z )');
     MemberList.Add('EARN_POINTS');   MemberDescList.Add('Earn Points ( Y or N )');
     MemberList.Add('EARNT_CREDIT');  MemberDescList.Add('Earnt Credit ( > 0.00 )');
     MemberList.Add('EDIT_DATE');     MemberDescList.Add('Edit Date ( mm/dd/yyyy )');
     MemberList.Add('EMAIL');         MemberDescList.Add('Email ( 0 to 9 , A to Z , @ )');
     MemberList.Add('EXPIRES');       MemberDescList.Add('Expires ( mm/dd/yyyy ) ');
     MemberList.Add('FOUNDATION');    MemberDescList.Add('Foundation ( Y or N )');
     MemberList.Add('GL_ACCOUNT');    MemberDescList.Add('GL Account ( > 0 )');
     MemberList.Add('GROUPS');        MemberDescList.Add('Groups ( > 0 )');
     MemberList.Add('HOME_PHONE');    MemberDescList.Add('Home Phone ( 0 to 9 possibly - )');
     MemberList.Add('INCL_EOD');      MemberDescList.Add('Include in EOD  ( Y or N )');
     MemberList.Add('INITIALS');      MemberDescList.Add('Initials ( A to Z )');
     MemberList.Add('IS_ACTIVE');     MemberDescList.Add('IS Active  ( Y or N )');
     MemberList.Add('JOINED');        MemberDescList.Add('Joined ( mm/dd/yyyy ) ');
     MemberList.Add('JOINING_FEE');   MemberDescList.Add('Joining Fee ( > 0.00 )');
     MemberList.Add('KNOWN_AS');      MemberDescList.Add('Known As ( 0 to 9 , A to Z )');
     MemberList.Add('LUCKY_DATE');    MemberDescList.Add('Lucky Date ( mm/dd/yyyy ) ');
     MemberList.Add('MAILING_CODE');  MemberDescList.Add('Mailing Code ( > 0 )');
     MemberList.Add('MAILING_LIST');  MemberDescList.Add('Mailing List ( A to Z )');
     MemberList.Add('MAILING_LIST_CODE'); MemberDescList.Add('Mailing List Code ( > 0 )');
     MemberList.Add('MALE_FEMALE');   MemberDescList.Add('Male or Female ( M or F )');
     MemberList.Add('NEWSLETTER');    MemberDescList.Add('NewsLetter ( Y or N )');
     MemberList.Add('POINTS_AVAILABLE');  MemberDescList.Add('Points Available ( > 0.00 )');
     MemberList.Add('POSITIONS');     MemberDescList.Add('Positions ( A to Z )');
     MemberList.Add('INACTIVE_DATE'); MemberDescList.Add('Inactive Date ( mm/dd/yyyy )');
     //MemberList.Add('SECTION_PAID');  MemberDescList.Add('Section Paid ( 0 to 2 )');
     MemberList.Add('SUB_PAID');      MemberDescList.Add('Sub Paid ( 0 to 2 )');
     MemberList.Add('PRE_PAID_CREDIT');   MemberDescList.Add('Pre Paid Credit  ( > 0.00 )');
     MemberList.Add('PRICE_LEVEL');   MemberDescList.Add('Price Level ( 0 to 99 )');
     MemberList.Add('PRODUCT_PURCHASE');  MemberDescList.Add('Product Purchase ( > 0.00 )');
     MemberList.Add('PROPOSER');      MemberDescList.Add('Proposer ( > 0 )');
     MemberList.Add('RECEIPT');       MemberDescList.Add('Receipt ( > 0 )');
     MemberList.Add('REFUNDED');      MemberDescList.Add('Refunded ( Y or N )');
     MemberList.Add('SAVED');         MemberDescList.Add('Saved ( Y or N )');
     MemberList.Add('SECONDER');      MemberDescList.Add('Seconder ( > 0 )');
     MemberList.Add('SENIOR');        MemberDescList.Add('Senior ( Y or N )');
     MemberList.Add('TITLE');         MemberDescList.Add('Title ( A to Z )');
     MemberList.Add('TOTAL_EARNT');   MemberDescList.Add('Total Earnt ( > 0.00 )');
     MemberList.Add('TOTAL_REDEEMED');  MemberDescList.Add('Total Redeemed ( > 0.00 )');
     MemberList.Add('TRANS_FROM');    MemberDescList.Add('Transferred From ( 0 to 9, A to Z )'); 
     MemberList.Add('VALUE_OF_CREDIT'); MemberDescList.Add('Value of Credit ( > 0.00 )');
     MemberList.Add('VIP_PAY_ONLY');  MemberDescList.Add('VIP Pay Only ( Y or N )');
     MemberList.Add('WORK_PHONE');    MemberDescList.Add('Work Phone ( 0 to 9 possibly - )');
     MemberList.Add('FIRST_NAME');    MemberDescList.Add('First Name ( A to Z )');
     MemberList.Add('FULL_NAME');     MemberDescList.Add('Full Name ( A to Z )');
     MemberList.Add('MEMBER');        MemberDescList.Add('Member ( 1 to 2147483647 )');
     MemberList.Add('SURNAME');       MemberDescList.Add('Surname ( A to Z )');
     PageControl.ActivePAge := tsBuilder;
     with stbStatus do  begin
       try
          Panels.BeginUpdate;
          Panels.Add;
          Panels[1].Width := 310;
          Panels[2].Width := 80;
       finally
         Panels.EndUpdate;
       end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.lbxTableNamesClick(Sender: TObject);
begin
      dm.IBDatabase.GetFieldNames(GetLbxTableName,lbxFieldNames.Items);
      lbxFieldNames.ItemIndex := 0;
      case PageControl.ActivePageIndex of 
         0 : begin  
                OpenTable(False);     
                PageControl1Change(Sender);
                cbxFieldNames.Items   := lbxFieldNames.Items;
             end;  
         3 : OpenTable(True); // if in Table Data Tab
      end;                     
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.cbxUseSortingClick(Sender: TObject);
begin
     if cbxUseSorting.Checked then begin
        cbxFieldNames.Enabled := true;
        cbxAscending.Enabled  := true;
        cbxAscending.ItemIndex := 0;
     end else begin   
        cbxFieldNames.Enabled := False;
        cbxAscending.Enabled  := False;
        cbxFieldNames.Clear;
     end;   
end;     
{ **************************************************************************** }
procedure TfrmMembersQUERY.PageControl1Change(Sender: TObject);
//procedure TfrmMembersQUERY.rgpMatchingChange(Sender: TObject; ButtonIndex: Integer);
var i : smallint;
    s : shortstring;
const msg1 = 'Field values for - ';         
begin                                 
     if PageControl.ActivePageIndex <> 0 then exit;
     Fieldtype := TFieldType(dm.qryReport1.FieldByName(GetLbxFieldName(False)).DataType); {datatype from field}
     FNumeric := False ; FDate := False;
     i := MemberList.IndexOf( GetLbxFieldName(False) );
     Label3.Caption := GetLbxFieldName(True) + ' Value';
     if i <> -1 then begin
        s := MemberDescList[i]
     end else begin                       
         case FieldType of
            ftUnknown:   s := '( 0 to 9, A to Z )';
            ftString:  begin
                          if dm.qryReport1.FieldByName(GetLbxFieldName(False)).Size = 1 then
                             s := '( Y or N )' // probably a Boolean in Interbase
                          else   
                             s := '( 0 to 9, A to Z )';
                       end;  
            ftSmallint:  s := '( -32768..0..32767 )';	
            ftInteger:   s := '( -2147483648..0..2147483647 )';
            ftWord:      s := '( 0..65535 )';
            ftBoolean:   s := '( True or False )';
            ftFloat:     s := '( -21474836.00..0.00.21474836.00 )';
            ftCurrency:  s := '( -21474836.00..0.00.21474836.00 )';
            ftBCD:       s := '( -21474836.00..0.00.21474836.00 )'; // numeric
            ftDate:      s := '( mm/dd/yyyy )';
            ftTime:      s := '( hh:mm:ss )';
            ftDateTime:  s := '( mm/dd/yyyy hh:mm:ss )';
            ftBytes:     s := 'String';
            ftVarBytes:  s := 'String';
            ftBlob:      s := 'Blob';
            ftMemo:      s := 'Blob';
         end; { end case }
         s :=  GetLbxFieldName(False) + ' ' + s;
     end;
     lblRange.Caption    := msg1 + s;    
     lblMatching.Caption := msg1 +  s;    
     case FieldType of
         ftSmallInt,ftFloat,ftInteger,ftBCD : FNumeric := True;  
         ftDate,ftDateTime,ftTime           : FDate    := True;  
     end;                                  
     case FieldType of
        ftUnknown,ftString,ftDateTime,ftTime,ftDate,ftVarBytes,ftBytes,ftBlob,ftMemo : FNeedQuotes := True;
        ftSmallint,ftInteger,ftWord,ftBoolean,ftFloat,ftCurrency,ftBCD : FNeedQuotes := False;
     end;
     // only do if in the Query Builder tab
     {if PageControl.ActivePAgeIndex = 0 then begin
        if FNumeric or FDate then begin
           gbxRange.Enabled := True;
           gbxMatching.Enabled := True;
           edtFrom.Enabled := True;
           edtTo.Enabled   := True;
        end else begin   
           gbxRange.Enabled := False;
           gbxMatching.Enabled := True;
           if rgpMatching.ItemIndex = 1 then begin 
              sm('String Fields cannot be in a Range');
              rgpMatching.ItemIndex := 0;
           end;   
           edtFrom.Enabled := False;
           edtTo.Enabled   := False;
        end;   
     end; }  
end;
{ **************************************************************************** }
function TFrmMembersQUERY.GetLbxTableName : String;
var i : smallint;
begin
     Result := '';
     for i := 0 to (lbxTableNames.Items.Count - 1) do 
        if lbxTableNames.Selected[i] then Result := lbxTableNames.Items.Strings[i];
     if Result = '' then Result := lbxTableNames.Items.Strings[0];    
end;
{ **************************************************************************** }
function TFrmMembersQUERY.GetSelected ( ListBox : TListBox ) : Integer ;
var i : smallint;
begin             
     Result := 0;
     for i := 0 to (ListBox.Items.Count - 1) do 
        if ListBox.Selected[i] then Result := i;
end;
{ **************************************************************************** }
function TFrmMembersQUERY.GetLbxFieldName ( WithQuotes : Boolean ) : String;
var i : smallint;
// need quotes on some fields as they are reserved words
begin
     Result := '';
     for i := 0 to (lbxFieldNames.Items.Count - 1) do 
        if lbxFieldNames.Selected[i] then Result := lbxFieldNames.Items.Strings[i];
     if Result = '' then Result := lbxFieldNames.Items.Strings[0];    
     if WithQuotes then Result  := '"' + Result + '"';
end;
{ **************************************************************************** }
procedure TFrmMembersQUERY.OpenTable(WithData : Boolean);
const SQL1 = 'Select * from %s';
      SQL2 = 'Select * from %s where %s is null';
begin
     if WithData then  
        dm.qryOpen(dm.qryQuery ,format(SQL1,[GetLbxTableName]))
     else
        dm.qryOpen(dm.qryReport1 ,format(SQL2,[GetLbxTableName,GetLbxFieldName(True)]));
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.PageControlChange(Sender: TObject);
begin
     PageControl1.ActivePageIndex  := 0;
     lbxFieldNames.DragMode := dmManual;
     lbxTableNames.DragMode := dmManual;
     lbxTableNames.Enabled  := True;
     dbnQry.DataSource      := nil;
     case PageControl.ActivePageIndex of        
        0 : begin
               // keep the Query Built
               if FKeepTableName <> '' then begin
                  lbxTableNames.ItemIndex := lbxTableNames.Items.IndexOf( FKeepTableName );
                  lbxTableNames.Enabled   := False;
                  lbxFieldNames.Clear;
                  lbxFieldNames.Items     := KeepFieldList;
               end;
               OpenTable(False);  
            end;
        1 : dbnQry.DataSource := dm.dsrQuery;
        2 : begin
              lbxFieldNames.DragMode := dmAutomatic;
              lbxTableNames.DragMode := dmAutomatic;
            end;  
        3 : begin
              dbnQry.DataSource := dm.dsrQuery;
              OpenTable(True);  // View the tables contents
            end;  
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.lbxFieldNamesClick(Sender: TObject);
// reset up the Field Options
begin
     //rgpMatching.ItemIndex := 0;
     //rgpOther.ItemIndex    := 0;
     PageControl1Change(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.menuExitClick(Sender: TObject);
begin
     ModalResult := mrCancel;
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.FormDestroy(Sender: TObject);
begin
     MemberList.Free;
     MemberDescList.Free;
     KeepFieldList.Free;
     dm.qryReport1.Close;
     dm.qryQuery.Close;
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.menuExecuteQueryClick(Sender: TObject);
var SQL1,s : String;
    SQLOrder,Joiner : String[7];
    i : integer;
begin
    case PageControl.ActivePageIndex of
       0 : begin  // Build up Query
              if lbxQueryLines.Items.Count = 0 then 
                 SQL1 := ''
              else begin   
                 SQL1 := format('SELECT * from %s',[GetLbxTableName]);
                 Joiner := ' where ';
                 for i := 0 to lbxQueryLines.Items.Count -1 do begin
                    SQL1 := SQL1 + Joiner + lbxQueryLines.Items[i];
                    Joiner := ' and ';
                 end;
                 if cbxUseSorting.Checked then begin
                    case cbxAscending.ItemIndex of
                       0 : SQLOrder := 'ASC';
                       1 : SQLOrder := 'DESC';
                    end;
                    SQL1 := SQL1 + Format(' ORDER BY %s %s',[cbxFieldNames.Items[cbxFieldNames.ItemIndex], SQLOrder ]);    
                 end;
              end;
           end;
       2 : SQL1 := memoQuery.Text; 
    end;   
    // weed out any inserts etc
    //sm(sQL1);
    S := UpperCase(SQL1);            
    if copy(s,1,6) <> 'SELECT' then begin
       sm('Select statements only - must be in the first 6 letters' );
       exit;
    end; 
    {if (pos('INSERT',S) > 0 ) or
       (pos('UPDATE',S) > 0 ) then begin
       sm('Select only - NO Inserts or Updates');
       exit;
    end;}   
    SQL1 := Trim(SQL1);
    if SQL1 = '' then begin
       sm('There is no Query');
       exit;
    end;   
    dm.qryOpen(dm.qryQuery,SQL1);
    PageControl.ActivePageIndex := 1;
    dbnQry.DataSource := dm.dsrQuery;
    dm.qryQuery.FetchAll;
    stbStatus.Panels[2].Text := Format( '%d Records',[dm.qryQuery.RecordCount] );
    // pass thru BUILDER to SQL
    if memoQuery.Text = '' then memoQuery.Text := SQL1;
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.menuLoadanSQLScriptClick(Sender: TObject);
begin
     if dm.OpenDialog.Execute then begin
        memoQuery.Lines.LoadFromFile(dm.OpenDialog.Filename);
        PageControl.ActivePageIndex := 2;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.menuSaveSQLScriptClick(Sender: TObject);
begin
     if dm.SaveDialog.Execute then memoQuery.Lines.SaveToFile(dm.SaveDialog.Filename);
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.menuPrinterSetupClick(Sender: TObject);
begin
     dm.PrintDialog.Execute;
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.menuPrinttheSQLStatementClick(Sender: TObject);
begin
     memoQuery.Print('SQL Statement'); // actually is Richedit
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.lbxTableNamesStartDrag(Sender: TObject; var DragObject: TDragObject);
begin
     memoQuery.DragMode := dmAutomatic;
     FTableOrField := '';
     if TListBox(sender).NAme = lbxTableNames.Name then begin
        FTableOrField := GetLbxTableName;
        if memoQuery.Text = '' then
           FTableOrField := 'SELECT * from ' + FTableOrField;    
     end else begin
        FTableOrField := GetLbxFieldName(False);
        if POS(',',memoQuery.Text) > 0  then
           FTableOrField := ',' + FTableOrField;   
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.memoQueryDragOver(Sender, Source: TObject; X,
  Y: Integer; State: TDragState; var Accept: Boolean);
begin
     if Source is TListBox then
        Accept := True
     else
        Accept := False;
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.memoQueryDragDrop(Sender, Source: TObject; X, Y: Integer);
begin
      memoQuery.Lines.Append(FTableOrField);   
      memoQuery.DragMode := dmManual;
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.memoQueryKeyPress(Sender: TObject; var Key: Char);
begin
     //if key = '''' then key := '`'; // change ' to `  - for sql
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.edtFromKeyPress(Sender: TObject; var Key: Char);
type  TCharSet = set of Char;
const Digits   : TCharSet = ['0'..'9','-','.','/'];
begin
     if not (Key in Digits) then begin
        edtFrom.Clear;
        edtTo.Clear;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.btnAddClick(Sender: TObject);
var s: shortstring;
    ADAteTime : TDateTime;
const Quotes = '''';    
begin
     {if FDate then begin
        try
           if edtFrom.Text <> ''     then  ADAteTime := StrToDateTime(edtFrom.Text);
           if edtTo.Text <> ''       then  ADAteTime := StrToDateTime(edtTo.Text);
           if edtMatching.Text <> '' then  ADAteTime := StrToDateTime(edtMatching.Text);
        except
           sm('Not a Valid Date or Time');
           exit;
        end;   
     end;                  }
     lbxTableNames.Enabled := False;
     s := '';
     case PageControl1.ActivePageIndex of 
        0 : begin   // Matching
               case rgpOther.ItemIndex of
                  0 : begin 
                         if edtMatching.Text <> '' then begin
                            case FNeedQuotes of  // Exact
                               True  : s := ' = ' + Quotes + edtMatching.Text + Quotes;  
                               False : s := ' = ' + edtMatching.Text;
                            end;
                         end;
                      end;
                  1 : begin
                         if edtMatching.Text <> '' then begin
                            case FNeedQuotes of // Partial
                               True  : s := ' LIKE ' + Quotes + '%' + edtMatching.Text + '%' + Quotes;  
                               False : s := ' LIKE ' + edtMatching.Text;
                            end;
                         end;
                      end;
                  2 : begin
                         if edtMatching.Text <> '' then begin
                            case FNeedQuotes of // Non
                               True  : s := ' <> ' + Quotes + edtMatching.Text + Quotes;  
                               False : s := ' <> ' + edtMatching.Text;
                            end;
                         end;
                      end;
                  3 : begin  // Null
                         s := ' IS NULL ';  
                      end;
                  4 : begin  // Is Not Null
                         s := ' IS NOT NULL ';  
                  
                         {if edtMatching.Text <> '' then begin
                            case FNeedQuotes of // Non
                               True  : s := ' <> ' + Quotes + edtMatching.Text + Quotes;  
                               False : s := ' <> ' + edtMatching.Text;
                            end;
                         end;}
                      end;    
                      
               end;                                          
            end;
        1 : begin 
               if (edtFrom.Text <> '') and (edtTo.Text <> '') then begin 
                  case FNeedQuotes of // Non
                     True  : s := ' BETWEEN ' + Quotes + edtFrom.Text + Quotes + ' and ' + Quotes + edtTo.Text + Quotes;
                     False : s := ' BETWEEN ' + edtFrom.Text + ' and ' + edtTo.Text;
                  end;
               end;  
            end;  
     end;
     if s <> '' then begin
        lbxQueryLines.Items.Add( GetLbxFieldName(False) + ' ' + s);
        // delete the entry from  lbxFieldNames they cannot reuse it
       // lbxFieldNames.Items.Delete( GetSelected (lbxFieldNames));
       // FKeepTableName         :=  GetLbxTableName;
       // KeepFieldList.AddStrings(lbxFieldNames.Items);
     end;   
     edtFrom.Clear;
     edtTo.Clear;
     edtMatching.Clear;
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.rgpOtherChange(Sender: TObject; ButtonIndex: Integer);
begin
     if FNumeric or FDate then begin
        if ButtonIndex  = 1 then begin
           sm('Numerics or Dates can only be an EXACT or NON matching');
           rgpOther.ItemIndex := 0;
        end;   
     end;   
     case ButtonIndex of 
        0,1,2 : begin  
                   edtMatching.Visible := True;
                   Label3.Visible := True;
                end;   
        3,4   : begin
                   edtMatching.Visible := False;
                   Label3.Visible := False;
                end;   
                   
     end;

end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.btnDeleteClick(Sender: TObject);
var i : smallint;
begin
     if lbxQueryLines.Items.Count = 0 then exit;
     i := GetSelected (lbxQueryLines);
     lbxFieldNames.Items.Add( copy (lbxQueryLines.Items[i],1,pos(' ',lbxQueryLines.Items[i]) -1));
     lbxQueryLines.Items.Delete(i);
     // if no more entries then refresh
     //if lbxQueryLines.Items.Count = 0 then btnClearAllClick(Sender);      cww
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.btnClearAllClick(Sender: TObject);
begin
     lbxQueryLines.Clear;
     lbxTableNames.Enabled := True;
     cbxUseSorting.Checked := False;
     // refresh the lbxFieldNames in case some deleted
     lbxTableNamesClick(Sender);
     FKeepTableName := '';
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     inherited;
     //case key of   
     //    VK_ESCAPE : menuExitClick(Sender);
     //end;   
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.btnOKClick(Sender: TObject);
begin
     ModalResult := mrOK;
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.FormShow(Sender: TObject);
begin
     if FMembersOnly then begin
        lbxTableNames.Items.Add('MEMBERS');
        tsSQL.tabvisible := False;
     end else   
        dm.IBDatabase.GetTableNames(lbxTableNames.Items,False);
     lbxTableNames.ItemIndex := 0;
     lbxTableNamesClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.FormResize(Sender: TObject);
begin
     ResizeToolBar( Self ,Self.WindowState,TwoButtonWidth,FpnlWidth,pnlSpace);
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.menuSaveSQLDataBaseClick(Sender: TObject);
begin
    // Queries use the Letter Form
    AddToClipBoard(memoQuery.Text);
    if frmMembersLetter = nil then
        if sfPreCheckLogOn(TfrmMembersLetter)
           then frmMembersLetter := TfrmMembersLetter.Create(nil)
        else exit;
     frmMembersLetter.SetAsLetter := False;
     SMStdForm.ShowStdForm(frmMembersLetter);
end;
{ **************************************************************************** }
procedure TfrmMembersQUERY.menuPasteClick(Sender: TObject);
begin
     MemoQuery.Lines.Add( Clipboard.AsText);
end;
{ ****************************************************************************}
procedure TfrmMembersQUERY.btnExportExcelClick(Sender: TObject);
var
    //Rng : TOpExcelRange;
    oWorkBook: TOpExcelWorkbook;
    oSheet: TOpExcelWorksheet;
    oRange: TOpExcelRange;
begin
     try
        menuExecuteQueryClick(Sender);
        if dm.qryQuery.RecordCount = 0 then begin
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
          dm.OpDataSetModel.DataSet := dm.qryQuery;
          oRange.Populate;                       // fill the range from the model
          Visible := True;                       // show it
          Interactive := True                    // Let the user access it
        end;
     finally
        dm.OpDataSetModel.DataSet := nil;
        NotBusy;           
     end;
end;

end.



