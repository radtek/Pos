unit MembersEMAIL;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm,  ComCtrls, ExtCtrls, Buttons, StdCtrls, lmdcombo,
  lmdextcS, OpShared, OpOlk2k, OpOutlk, lmdcompo, lmdclass, lmdnonvS,
  Menus, ToolWin, Db, OpDbOlk, OpOlkXP, wwriched  ;

type 
    TfrmTypes = ( AsWholeReport,AsMemberEmail,AsMemberReport );
    {  asWholeReport - the report is emailed
       asMemnberEmail - selected from option 63 where you can select groups etc
       asmemberReport - a report is run then email members from the report data
    }
type
  TfrmMembersEmail = class(TSMStdForm)
    Bevel1: TBevel;
    OpOutlook: TOpOutlook;
    ToolButton1: TToolButton;
    btnEmail: TToolButton;
    menuOptions: TMenuItem;
    menuSendEmail: TMenuItem;
    menuExit: TMenuItem;
    pnlSpace: TPanel;
    Panel1: TPanel;
    Label5: TLabel;
    lbxAttachments: TListBox;
    Panel2: TPanel;
    pnlToCc: TPanel;
    Label2: TLabel;
    Label3: TLabel;
    cbxTo: TLMDComboBox;
    cbxCC: TLMDComboBox;
    memoBody: TwwDBRichEdit;
    btnDelete: TToolButton;
    btnInsert: TToolButton;
    btnRichText: TToolButton;
    Panel4: TPanel;
    Label1: TLabel;
    edtSubject: TEdit;
    pnlBcc: TPanel;
    Label4: TLabel;
    lbxBcc: TListBox;
    btnClipboard: TToolButton;
    btnFile: TToolButton;
    N1: TMenuItem;
    menuInsertAtt: TMenuItem;
    menuDeleteAtt: TMenuItem;
    N2: TMenuItem;
    menuRichText: TMenuItem;
    N3: TMenuItem;
    menuBccClipboard: TMenuItem;
    menuBccNotepad: TMenuItem;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure lbxAttachmentsKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure FormActivate(Sender: TObject);
    procedure cbxToKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure menuSendEmailClick(Sender: TObject);
    procedure menuExitClick(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure btnInsertClick(Sender: TObject);
    procedure btnDeleteClick(Sender: TObject);
    procedure SendReport;
    procedure SendMembers;
    procedure btnRichTextClick(Sender: TObject);
    function  Extract(Links :integer; ExtFields,mMembers:String) : Boolean;
    procedure btnClipboardClick(Sender: TObject);
    procedure btnFileClick(Sender: TObject);
  private
    { Private declarations } 
    FSubject : string;  
    FBody    : string; 
    //FBccMembers : String;
    FBccCount  : Integer; 
    FpnlWidth : SmallInt;
    FAsMember : Boolean;
    frmTypes : TfrmTypes;
    FBccMember : String;
    FBccStrings : TStringList;
  public
    { Public declarations }
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
    property ESubject : string  write FSubject;
    property EBody    : string  write FBody;
    //property EBccMembers : string  write FBccMembers; 
    property EBccCount : Integer  write FBccCount; 
    property AsMember : boolean  write FAsMember default False;
    property EfrmTypes : TfrmTypes  write frmTypes;
    property EBccStrings : String  write FBccMember;
    
  end;

var
  frmMembersEmail: TfrmMembersEmail;

implementation

{$R *.DFM}
uses
    Global,    // StockMaster
    dmMembers;

{ **************************************************************************** }
class function TfrmMembersEmail.ScreenCode : Integer;
begin Result := Global.scMembersEmail; end;
{ **************************************************************************** }
class function TfrmMembersEmail.ScreenName : String;
begin Result := 'Members Email'; end;
{ **************************************************************************** }
class function TfrmMembersEmail.ScreenDesc : String;
begin Result := '  Members Email Screen'; end;
{ **************************************************************************** }
procedure TfrmMembersEmail.menuExitClick(Sender: TObject);
begin
     Close;
end;
{ **************************************************************************** }
procedure TfrmMembersEmail.FormCreate(Sender: TObject);
begin

     inherited;
     cbxTo.Clear;
     cbxCC.Clear;
     SetupOpenDialog('*.*','All Files|*.*','Add file attachments');
     FpnlWidth := pnlSpace.Width;
end;
{ **************************************************************************** }
procedure TfrmMembersEmail.FormActivate(Sender: TObject);
var
    i : integer;                          
    oAddressList: AddressList;
begin
     try
        case frmTypes of 
        AsMemberEmail : begin
                            btnClipboard.Enabled := True;
                            btnFile.Enabled := True;
                            btnEmail.Hint   := 'Send the Email to Outlook';
                            pnlToCc.Visible := False;
                            pnlBcc.Visible  := True;
                            pnlSpace.Caption := format('Member Count = %d',[FBccCount]);
                            if assigned(FBccStrings) then
                               lbxBcc.Items.text := FBccStrings.Text;
                        end;
        asMemberReport : begin
                            btnClipboard.Enabled := True;
                            btnFile.Enabled := True;
                            btnEmail.Hint   := 'Send the Email to Outlook';
                            pnlToCc.Visible := False;
                            pnlBcc.Visible  := True;
                            pnlSpace.Caption := format('Member Count = %d',[FBccCount]);
                            FBccStrings := TStringList.Create;
                            FBccStrings.Text := FBccMember;
                            lbxBcc.Items.AddStrings(FBccStrings);
                         end;   
        asWholeReport : begin 
                            btnClipboard.Enabled := False;
                            btnFile.Enabled := False;
                            pnlSpace.Caption := '';
                            btnEmail.Hint := 'Send this Report to Outlook';
                            pnlToCc.Visible := True;
                            pnlBcc.Visible  := False;
                            TooBusy( 'Loading Global Address List ... '); 
                            if not OpOutlook.Connected then
                               OpOutlook.Connected := True;
                            // fill with defaults
                            lbxAttachments.Items.Add(Global.LastFileExported);   
                            edtSubject.Text := FSubject;
                            memoBody.Text   := FBody;  
                            oAddressList    := OpOutlook.MapiNamespace.AddressLists.Item(1);  // Global Address List
                            for i := 1 to oAddressList.AddressEntries.Count do 
                                cbxTo.Items.Add(oAddressList.AddressEntries.Item(i).Name);
                            cbxCC.Items := cbxTo.Items;
                            //for i := 0 to cbxTo.Items.Count -1 do
                            //    cbxCC.Items.Add(cbxTo.Items[i]);
                            cbxTo.droppeddown := True;    
                         end;   
        end;                 
     finally
        NotBusy;
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEmail.FormShow(Sender: TObject);
begin
end;
{ **************************************************************************** }
procedure TfrmMembersEmail.FormDestroy(Sender: TObject);
begin
    if assigned(FBccStrings) then
       FBccStrings.Free; 
    if OpOutlook.Connected then OpOutlook.Connected := False;
end;
{ **************************************************************************** }
function TfrmMembersEmail.Extract(Links :integer; ExtFields,mMembers:String) : Boolean;
var WhereUsed : Boolean;
     s : String;
begin
     result := False;
     with dm.qryReport do begin
         close;
         SQL.Text := 'SELECT M.EMAIL,M.MEMBER,M.FULL_NAME,m.RECEIVE_EMAIL,M.HOME_PHONE, ' +
                     'M.ADDRESS,M.DISTRICT,M.ADDRESS1,M.CITY,M.COUNTRY FROM MEMBERS M';
         case Links of  // add table
            0 : if ExtFields <> '' then SQL.Add( ',M_GROUPS S' );  
            1 : if ExtFields <> '' then SQL.Add( ',M_SECTIONS S' );
            2 : if ExtFields <> '' then SQL.Add( ',M_SUBS S' );
         end;
         WhereUsed := False;
         if WhereUsed then 
            SQL.Add( ' and M.DECEASED <> ''Y'' and m.RECEIVE_EMAIL = ''Y''' ) // get live ones
         else
            SQL.Add( ' WHERE M.DECEASED <> ''Y'' and m.RECEIVE_EMAIL = ''Y''' ); // get live ones
            
         case Links of  // add table
         0 : begin
             if ExtFields <> '' then begin
                SQL.Add(' and M.MEMBER = S.MEMBER' );
                SQL.Add( Format( ' and S.GROUPS in (%s)',[ExtFields]));
             end;
             end;                   
         1 : begin
             if ExtFields <> '' then begin
                  SQL.Add(' and M.MEMBER = S.MEMBER' );
                  SQL.Add( Format( ' and S.SECTION in (%s)',[ExtFields]));
              end;
              end; 
         2 : begin
             if ExtFields <> '' then begin
                SQL.Add(' and M.MEMBER = S.MEMBER' );
                SQL.Add( Format( ' and S.SUBSCRIPTION in (%s)',[ExtFields]));
              end;
              end;
         end;
         if mMembers <> '' then SQL.Add( Format( '%s',[mMembers]));
         //if mMailJoin <> '' then SQL.Add( Format( '%s',[mMailJoin]));
        // AddToClipBoard(SQL.Text);
         Open;
         First;
         FBccCount := 0;                       
         FBccStrings := TStringList.Create;
         FBccStrings.Clear;
         While not EOF do begin
            if pos('@',FieldByName('EMAIL').AsString) > 0 then begin
               FBccStrings.Add(FieldByName('EMAIL').AsString + ';');
               inc(FBccCount);
            end;
            next;
         end;
         if FBccCount = 0 then begin
            sm('There are no Members to email');
            exit;
         end;
     end;
     result := True;
end;
{ ****************************************************************************}
procedure TfrmMembersEmail.menuSendEmailClick(Sender: TObject);
begin
     if not OpOutlook.Connected then
        OpOutlook.Connected := True;
     case frmTypes of
        AsWholeReport  : SendReport;
        AsMemberEmail  : SendMembers;
        AsMemberReport : SendMembers;
     end;   
end;
{ ****************************************************************************}
procedure TfrmMembersEmail.SendReport;
var
     MailItem: TOpMailItem;
     i : shortint;
begin
     MailItem := OPOutlook.CreateMailItem;
     try
        TooBusy( 'Sending Email ...');
        if assigned( MailItem ) then begin
           with MailItem do begin
              if cbxTo.Text = '' then begin
                 sm('Must select "To" ');
                 exit;
              end;    
              MsgTo := cbxTo.Text;
              if cbxCC.Text <> '' then          
                MsgCC := cbxCC.Text;
              if edtSubject.Text <> '' then
                Subject := edtSubject.Text;
              if memoBody.Text <> '' then
                Body := memoBody.Text;
              if lbxAttachments.Items.Count <> 0 then begin
                 for i := 0 to lbxAttachments.Items.Count -1 do begin
                    Attachments.Add(lbxAttachments.Items[i]);  
                 end;
              end;
              try
                Send;
                dm.ProcessType := ord(TProcessTypes(ptEmail));
                dm.LogProcess(dm.qryGeneral,format('Email message sent to %s with attachment %s',[cbxTo.Text,lbxAttachments.Items[0]])); 
                dm.IBCommitRetain;
                lbxAttachments.Clear;
                memoBody.Clear;
                edtSubject.Clear;
                cbxTo.Clear;
                cbxCC.Clear;
                stbStatus.Panels[1].Text := 'Last message sent OK ...'; 
              except
                MessageDlg( 'Mail send failure', mtWarning, [mbOK], 0 )
              end;
           end;   
        end;
     finally
        MailItem.Free;
        NotBusy;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersEmail.SendMembers;
var  MailItem: TOpMailItem;
     i : smallint;
begin
     MailItem := OPOutlook.CreateMailItem;
     try
        TooBusy( 'Sending Emails ...');
        if assigned( MailItem ) then begin
           MailItem.MsgBCC := FBccStrings.Text;
           if edtSubject.Text <> '' then
             MailItem.Subject := edtSubject.Text;
           if memoBody.Text <> '' then
             MailItem.Body := memoBody.Text;
           if lbxAttachments.Items.Count <> 0 then begin
              for i := 0 to lbxAttachments.Items.Count -1 do begin
                 MailItem.Attachments.Add(lbxAttachments.Items[i]);  
              end;
           end;
           try
             MailItem.Send;
           except
             MessageDlg( 'Mail send failure', mtWarning, [mbOK], 0 );
             exit;
           end;
        end;
        mDlgI(format('There were %d Members that were sent an Email',[FBccCount]));
     finally
        MailItem.Free;
        NotBusy;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersEmail.lbxAttachmentsKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     if Key = VK_Delete then begin
        if lbxAttachments.ItemIndex >= 0 then 
           lbxAttachments.Items.Delete(lbxAttachments.ItemIndex);
     end;
     if Key = VK_Insert then begin
        if dm.OpenDialog.execute then 
           lbxAttachments.Items.Add(dm.OpenDialog.Filename);
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEmail.cbxToKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     if key = 13 then 
        SelectNext(ActiveControl as TWinControl,True,True); 
end;
{ **************************************************************************** }
procedure TfrmMembersEmail.FormKeyUp(Sender: TObject; var Key: Word;  Shift: TShiftState);
begin
     inherited;
     case key of   
         VK_ESCAPE : menuExitClick(Sender);
     end; 
     if  ( ssAlt in shift ) then begin
        case key of
           88,120  : menuExitClick(Sender);      // x Close
           83,115  : menuSendEmailClick(Sender); // S
        end;  
     end;
end;
{ **************************************************************************** }
procedure TfrmMembersEmail.FormResize(Sender: TObject);
begin
     ResizeToolBar( Self ,Self.WindowState,OneButtonWidth,FpnlWidth,pnlSpace);
end;
{ ****************************************************************************}
procedure TfrmMembersEmail.btnInsertClick(Sender: TObject);
begin
     if dm.OpenDialog.execute then 
        lbxAttachments.Items.Add(dm.OpenDialog.Filename);
end;
{ ****************************************************************************}
procedure TfrmMembersEmail.btnDeleteClick(Sender: TObject);
begin
     if lbxAttachments.ItemIndex >= 0 then 
        lbxAttachments.Items.Delete(lbxAttachments.ItemIndex);
end;
{ ****************************************************************************}
procedure TfrmMembersEmail.btnRichTextClick(Sender: TObject);
begin
    memoBody.Execute;
end;
{ ****************************************************************************}
procedure TfrmMembersEmail.btnClipboardClick(Sender: TObject);
begin
     AddToClipBoard(lbxBcc.Items.Text);
     MDlgI('Successfully copied to the Clipboard, use Paste (Ctrl V) to copy them to an Email Message');
end;
{ ****************************************************************************}
procedure TfrmMembersEmail.btnFileClick(Sender: TObject);
begin
     lbxBcc.Items.SaveToFile(GetCurrentDir + '\Member.bcc');
     DOSExec('Notepad.exe ' + GetCurrentDir + '\Member.bcc'); 
end;
{ ****************************************************************************}
end.
 