unit MembersLOOKUP;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm, ComCtrls, Grids, Wwdbigrd, Wwdbgrid, StdCtrls,
  Wwkeycb, Buttons, ExtCtrls,DB,IBTable, lmdcompo, lmdclass, lmdnonvS,
  Menus, ToolWin, lmdcctrl, lmdctrl, lmdgroup;

type
  TfrmMembersLookup = class(TSMStdForm)
    dbgLookup: TwwDBGrid;
    btnOK: TToolButton;
    btnCancel: TToolButton;
    pnlFind: TPanel;
    pnlSpace: TPanel;
    menuExit: TMenuItem;
    incSearch: TwwIncrementalSearch;
    rpgSearch: TLMDRadioGroup;
    procedure btnCancelClick(Sender: TObject);
    procedure btnOKClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure Setup( theDataSource : TDataSource; ActualTable : TIBTable ; theField : string ; theMember : LongInt );       
    function CheckBeforeInsert ( theTable,theField : string ; theMember,theInteger : Integer ) : Boolean;
    procedure FormCreate(Sender: TObject);
    procedure incSearchKeyPress(Sender: TObject; var Key: Char);
    procedure FormKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure menuExitClick(Sender: TObject);
    procedure dbgLookupKeyPress(Sender: TObject; var Key: Char);
    procedure FormResize(Sender: TObject);
    procedure rpgSearchChange(Sender: TObject; ButtonIndex: Integer);
    procedure FormActivate(Sender: TObject);
    procedure dbgLookupCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont;
      ABrush: TBrush);
    
  private
    { Private declarations }
    FActualTable : TIBTable;
    FTableName : String;
    FField     : String;
    FMember    : LongInt;
    FpnlWidth  : SmallInt;
  public
    { Public declarations }
    MultiList : tStringList;
    LookupOnly : Boolean;
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;

var
  frmMembersLookup: TfrmMembersLookup;

implementation

{$R *.DFM}
uses
   global,
   dmMembers;
{ **************************************************************************** }
class function TfrmMembersLookup.ScreenCode : Integer;
begin Result := Global.scLookup; end;
{ **************************************************************************** }
class function TfrmMembersLookup.ScreenName : String;
begin Result := 'Lookup'; end;
{ **************************************************************************** }
class function TfrmMembersLookup.ScreenDesc : String;
begin Result := '  Lookup Screen'; end;
{ **************************************************************************** }
procedure TfrmMembersLookup.Setup( theDataSource : TDataSource; ActualTable : TIBTable ;  
          theField : string ; theMember : LongInt );
var i : shortint;                  
    s : string[20];
    Sender : TObject;
begin
     with dbgLookup do begin
        DataSource  := theDataSource; 
	     Selected.Clear;
        if btnCancel.Enabled = False then begin // Payments call
           Selected.Add('PAYMENT' + #9 + '8' + #9 + 'Receipt');
           Selected.Add('TENDER_TYPE' + #9 + '15' + #9 + 'Tender');
           Selected.Add('AMOUNT' + #9 + '10' + #9 + 'Amount');
           Selected.Add('DETAILS' + #9 + '15' + #9 + 'Details');
           Selected.Add('EX_RATE' + #9 + '9' + #9 + 'Exch Rate'); 
           rpgSearch.Visible := False;
           incSearch.Visible := False;
           pnlFind.Caption := '';
        end else if theField = FieldNameArray[0] then begin   // members call subs
      	  Selected.Add('SUBSCRIPTION' + #9 + '10' + #9 + 'Subscription');
      	  Selected.Add('CODE'  + #9 + '5' + #9 + 'Code');
      	  Selected.Add('DESCRIPTION'  + #9 + '45' + #9 + 'Description');
        end else if theField = '' then begin   // members call Inactive Search
      	  Selected.Add('MEMBER' + #9 + '15' + #9 + 'Member');
      	  Selected.Add('FULL_NAME' + #9 + '30' + #9 + 'Full Name');
      	  Selected.Add('EXPIRES' + #9 + '15' + #9 + 'Expires');
           rpgSearch.Visible := False;
           incSearch.Visible := False;
           pnlFind.Caption := '';
        end else begin   // members call Events Section Groups
	        Selected.Add('EVENT' + #9 + '10' + #9 + 'Event');
   	     Selected.Add('GROUPS' + #9 + '10' + #9 + 'Group');
   	     Selected.Add('SECTION' + #9 + '10' + #9 + 'Section');
   	     Selected.Add('DISCOUNT_KEY' + #9 + '10' + #9 + 'Discount');
      	  Selected.Add('DESCRIPTION'  + #9 + '45' + #9 + 'Description');
        end;
   	  dbgLookup.ApplySelected;
     end;                         
     if ActualTable <> nil then begin
        FActualTable := ActualTable;
        FActualTable.Close;  // want to see the latest Records
        FActualTable.Open;  // want to see the latest Records
        incSearch.DataSource  := theDataSource; 
        FActualTable.First; 
     end;
     FField       := theField;
     FMember      := theMember;
     for i := 0 to MaxFieldNames do begin
        if theField = FieldNameArray[i] then begin
           rpgSearch.Items.Clear;
           rpgSearch.Items.Add('Description');
           s := copy(FField,1,1) + copy(lowercase(FField),2,Length(FField));
           rpgSearch.Items.Add(s);
           case i of 
              0 : rpgSearch.Items.Add('Code');
           end;    
           FTableName := FieldTableArray[i];
           Self.Caption := ScreenName + '  ' + FieldNamesArray[i];
        end;
     end;
     // Discounts 
     if FTableName = '' then begin
        FTableName := 'MEMBERS_DISCOUNTS';
        rpgSearch.Items.Clear;
        rpgSearch.Items.Add('Description');
        Self.Caption := ScreenName + '   Discounts';
   	  dbgLookup.ApplySelected;
       // dbgLookup.DataSource  := theDataSource; 
       // incSearch.DataSource  := theDataSource; 
     end;
     rpgSearch.ItemIndex := 1;
     rpgSearchChange(Sender,rpgSearch.ItemIndex);
end;
{ **************************************************************************** }   
procedure TfrmMembersLookup.btnCancelClick(Sender: TObject);
begin
    ModalResult := mrCancel;
end;
{ **************************************************************************** }
procedure TfrmMembersLookup.btnOKClick(Sender: TObject);
var i : integer;
    OK : Boolean;
    myField : String;
begin
     if not LookupOnly then begin // only used to lookup a value
        dbgLookup.OnKeyPress := nil;
        MultiList.Clear;

        myField := FField;
        if myField = 'DISCOUNT_KEY' then myField := 'Discount';
        // one record only - is not selected
        if dbgLookup.SelectedList.Count = 0 then begin
           if not CheckBeforeInsert ( FTableName,FField,FMember,FActualTable.FieldByName(FField).AsInteger) then begin
              showmessage( format('%s %d is already selected for Member %d',[myField,FActualTable.FieldByName(FField).AsInteger,Fmember]));
              FActualTable.Refresh; // may have been added by another user
              exit;
           end;
           MultiList.Add(FActualTable.FieldByName(FField).AsString);
        end else if dbgLookup.SelectedList.Count = 1 then begin // 1 selected
           FActualTable.GotoBookmark(dbgLookup.SelectedList.items[0]);
           if not CheckBeforeInsert ( FTableName,FField,FMember,FActualTable.FieldByName(FField).AsInteger) then begin
              showmessage( format('%s %d is already selected for Member %d',[myField,FActualTable.FieldByName(FField).AsInteger,Fmember]));
              FActualTable.Refresh; // may have been added by another user
              exit;
           end;
           MultiList.Add(FActualTable.FieldByName(FField).AsString);
        end else begin // many selected
           for i := 0 to dbgLookup.SelectedList.Count -1 do begin 
              FActualTable.GotoBookmark(dbgLookup.SelectedList.items[i]);
              if CheckBeforeInsert ( FTableName,FField,FMember,FActualTable.FieldByName(FField).AsInteger) then 
                 MultiList.Add(FActualTable.FieldByName(FField).AsString);
           end;   
        end;
        dbgLookup.OnKeyPress := dbgLookupKeyPress;
        
     end;
     ModalResult := mrOK;
end;
{ **************************************************************************** }
procedure TfrmMembersLookup.FormDestroy(Sender: TObject);
begin
     if assigned (MultiList) then
        MultiList.free;
end;
{ **************************************************************************** }
function TfrmMembersLookup.CheckBeforeInsert ( theTable,theField : string ; theMember,theInteger : Integer ) : Boolean;
// looks to see if the already inserted into M_SUBS etc 
// post error takes care of duplicates
var i : integer;
const SQL1 = 'SELECT COUNT(*) FROM %s WHERE MEMBER = %d AND %s = %d';
begin
     Result := False;
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[theTable,theMember,theField,theInteger])) then 
        if dm.qryGeneral.Fields[0].AsInteger = 0 then Result := True;
end;
{ **************************************************************************** }
procedure TfrmMembersLookup.FormCreate(Sender: TObject);
begin
     inherited;
     MultiList := TStringList.create;
     FpnlWidth := pnlSpace.Width;
end;
{ **************************************************************************** }
procedure TfrmMembersLookup.incSearchKeyPress(Sender: TObject;  var Key: Char);
begin
     if key = #13 then begin
        key := #0;
        incSearch.FindValue;
        incSearch.SelectAll;
        //key := #0;
        dbgLookup.Setfocus;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersLookup.FormKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     inherited;
     if ( ssCtrl in Shift ) then begin
        if btnCancel.Enabled then begin
           case key of
              70,102 : incSearch.SetFocus; // F
           end;
        end;
     end;
    case key of   
         VK_ESCAPE : btnCancelClick(Sender);
         VK_RETURN : btnOKClick(Sender);
     end;
     if ( ssAlt in Shift ) then begin
        case key of 
           88,120  : btnCancelClick(Sender); // X Close - treat this as Cancel
        end;
     end;
     key := 0;
end;
{ **************************************************************************** }
procedure TfrmMembersLookup.menuExitClick(Sender: TObject);
begin
     ModalResult := mrCancel;
end;
{ **************************************************************************** }
procedure TfrmMembersLookup.dbgLookupKeyPress(Sender: TObject; var Key: Char);
begin
     if key = #13 then btnOKClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersLookup.FormResize(Sender: TObject);
begin
     ResizeToolBar( Self ,Self.WindowState,TwoButtonWidth,FpnlWidth,pnlSpace);
end;
{ **************************************************************************** }
procedure TfrmMembersLookup.rpgSearchChange(Sender: TObject; ButtonIndex: Integer);
begin
     case ButtonIndex of 
       0 : incSearch.SearchField := 'DESCRIPTION';
       1 : incSearch.SearchField := FField;
       2 : incSearch.SearchField := 'CODE';
     end;
     if frmMembersLookup.Active then
        incSearch.Setfocus;
end;
{ **************************************************************************** }
procedure TfrmMembersLookup.FormActivate(Sender: TObject);
begin
     if incSearch.Visible then
        incSearch.Setfocus;
end;
{ **************************************************************************** }
procedure TfrmMembersLookup.dbgLookupCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont;
  ABrush: TBrush);
begin
     if State = [gdSelected] then begin
        ABrush.Color := clSilver;
        AFont.Color  := clBlue;
     end;
     if Highlight then AFont.Color := clWhite;
end;


end.
