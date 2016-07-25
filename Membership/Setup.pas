unit Setup;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Mask, ToolEdit, lmdctrl, lmdstdcS, lmdextcS, ExtCtrls, Buttons,Registry,
  lmdcompo, lmdclass, lmddlgS, lmdcctrl, lmdeditb, lmdeditc, LMDEdit,FileCtrl;

type
  TfrmSetup = class(TForm)
    GroupBox1: TGroupBox;
    GroupBox2: TGroupBox;
    btnInstallInterbase: TButton;
    btnAddUserName: TButton;
    pnlMembership: TPanel;
    Label12: TLMDLabelFill;
    Label13: TLMDLabelFill;
    Label11: TLabel;
    Label1: TLabel;
    edtLocalUserName: TEdit;
    cbxUserLogon: TComboBox;
    Label2: TLabel;
    btnSaveRegistry: TButton;
    Label3: TLabel;
    Label4: TLabel;
    BitBtn1: TBitBtn;
    lblRestart: TLabel;
    edtMembershipServer: TLMDEdit;
    BrowseDlg: TLMDBrowseDlg;
    edtMembershipDataBase: TLMDEdit;
    edtMenuMateServer: TLMDEdit;
    edtMenuMateDataBase: TLMDEdit;
    procedure FormCreate(Sender: TObject);
    procedure btnSaveRegistryClick(Sender: TObject);
    procedure btnAddUserNameClick(Sender: TObject);
    procedure btnInstallInterbaseClick(Sender: TObject);
    procedure edtMembershipServerCustomButtons0Click(Sender: TObject;
      index: Integer);
    procedure edtMembershipDataBaseCustomButtons0Click(Sender: TObject;
      index: Integer);
    procedure edtMenuMateServerCustomButtons0Click(Sender: TObject;
      index: Integer);
    procedure edtMenuMateDataBaseCustomButtons0Click(Sender: TObject;
      index: Integer);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmSetup: TfrmSetup;

implementation

{$R *.DFM}
uses
   Global,dmMembers;

{ ****************************************************************************}
procedure TfrmSetup.FormCreate(Sender: TObject);
begin
     edtMembershipServer.Text := Global.ReadComputerName;
     edtMenuMateServer.Text   := edtMembershipServer.Text;
     // fill in some of the settings - if F10 pressed
     if regServerName <> '' then
        edtMembershipServer.Text := regServerName;
     if regDatabaseName <> '' then
        edtMembershipDataBase.Text := regDatabaseName;
     if regMenuMateServerName <> '' then
        edtMenuMateServer.Text     := regMenuMateServerName;
     if regMenuMateDatabaseName <> '' then
        edtMenuMateDataBase.Text   := regMenuMateDatabaseName;
end;
{ ****************************************************************************}
procedure TfrmSetup.btnSaveRegistryClick(Sender: TObject);
var   reg : TRegistry; 
      regLogonStr : shortString;
const regSection    = '\SOFTWARE\IQWORKS\MenuMate\Membership';
begin
      reg := TRegistry.Create;
      reg.RootKey  := HKEY_LOCAL_MACHINE;
      try
         try
            if reg.OpenKey(regSection, True) then begin
               // write to program files
               case cbxUserLogon.ItemIndex of
                  0 : begin 
                         regLogon := false;
                         regLogonStr := '0';
                      end;   
                  1 : begin
                         regLogon := True;
                         regLogonStr := '1';
                      end;   
               end;   
               Reg.WriteString ('ServerName', edtMembershipServer.Text);
               Reg.WriteString ('DatabaseName',edtMembershipDataBase.Text);
               Reg.WriteString ('MenuMateServerName', edtMenuMateServer.Text );
               Reg.WriteString ('MenuMateDatabaseName',edtMenuMateDataBase.Text);
               Reg.WriteString ('UserLogon',  regLogonStr );
               Reg.WriteString ('LocalUserName', regLocalUserName );
               regServerName            := edtMembershipServer.Text;
               regDatabaseName          := edtMembershipDataBase.Text;
               regMenuMateServerName    := edtMenuMateServer.Text;
               regMenuMateDatabaseName  := edtMenuMateDataBase.Text;
               regLocalUserName         := edtLocalUserName.Text;
            end;
         except   
            showmessage('Cannot write to the Registry');
         end;
         showmessage('Save Registry completed OK');
      finally
         reg.CloseKey;
         reg.Free;
      end;
end;
{ ****************************************************************************}
procedure TfrmSetup.btnAddUserNameClick(Sender: TObject);
var ID : Word;
begin
     ID := MessageDlg('Is MenuMate Point of Sale installed?',mtConfirmation,[mbYes,mbNo,mbCancel],0);
     if ID = mrCancel then exit;
     try
        with dm.IBSecurityService do begin
           Active := False;
           ServerName := regServerName;
           Params.Clear;
           Params.Add(Format('USER_NAME=%s',['SYSDBA']));
          // MenuMate password is different - must use SYSDBA
           if ID = mrYes then
              Params.Add(Format('PASSWORD=%s' ,['masterkey']))
           else   
              Params.Add(Format('PASSWORD=%s' ,['master']));
           Active := True;
           UserName := 'DEFAULT';
           Password := 'password';
           UserID   := 0;
           AddUser;
           Active := False;
        end;
        showmessage('Added Default User OK');
     except
        On E: Exception  do begin 
           if pos( 'attempt to store duplicate',E.Message) > 0 then
              showmessage('Default User already added')
           else
              raise;
        end;      
     end;   

end;
{ ****************************************************************************}
procedure TfrmSetup.btnInstallInterbaseClick(Sender: TObject);
begin
     SetupOpenDialog('*.Exe','Exe Files|*.Exe','Interbase Install File ');
     dm.OpenDialog.InitialDir := 'c:\program files\menumate';
     if FileExists('c:\program files\menumate\Interbase 6.0.1.6.exe') then begin
        WinExec('c:\program files\menumate\Interbase 6.0.1.6.exe',SW_SHOW);
     end else begin
        if dm.OpenDialog.Execute then begin
           WinExec(PChar(dm.OpenDialog.FileName),SW_SHOW);
        end;   
     end;
end;
{ ****************************************************************************}
procedure TfrmSetup.edtMembershipServerCustomButtons0Click(Sender: TObject;  index: Integer);
begin
     BrowseDlg.StartFolder := bfoNetworkNeighborhood;
     BrowseDlg.Options     := [doBrowseForComputer,doStatusText,doShowPath,doEnableOK];
     if BrowseDlg.Execute then begin
       edtMembershipServer.Text := BrowseDlg.SelectedName;
     end;
end;
{ ****************************************************************************}
procedure TfrmSetup.edtMenuMateServerCustomButtons0Click(Sender: TObject; index: Integer);
begin
     BrowseDlg.StartFolder := bfoNetworkNeighborhood;
     BrowseDlg.Options     := [doBrowseForComputer,doStatusText,doShowPath,doEnableOK];
     if BrowseDlg.Execute then begin
       edtMenuMateServer.Text := BrowseDlg.SelectedName;
     end;
end;
{ ****************************************************************************}
procedure TfrmSetup.edtMembershipDataBaseCustomButtons0Click(Sender: TObject; index: Integer);
begin
     BrowseDlg.StartFolder := bfoMyComputer;                   
     BrowseDlg.Options     := [doStatusText,doShowFiles,doShowPath,doEnableOK];
     if DirectoryExists('C:\Program Files\MenuMate') then begin
        BrowseDlg.SelectedFolder := 'C:\Program Files\MenuMate';                     
     end;
     if DirectoryExists('C:\StockMaster\Sockmaster Sites') then begin
        BrowseDlg.SelectedFolder := 'C:\StockMaster\Sockmaster Sites';                     
     end;
     if BrowseDlg.Execute then begin
       edtMembershipDataBase.Text := BrowseDlg.SelectedFolder;
     end;
end;
{ ****************************************************************************}
procedure TfrmSetup.edtMenuMateDataBaseCustomButtons0Click(Sender: TObject; index: Integer);
begin
     BrowseDlg.StartFolder := bfoMyComputer;                   
     BrowseDlg.Options     := [doStatusText,doShowFiles,doShowPath,doEnableOK];
     if DirectoryExists('C:\Program Files\MenuMate') then begin
        BrowseDlg.SelectedFolder := 'C:\Program Files\MenuMate';                     
     end;
     if DirectoryExists('C:\DB') then begin
        BrowseDlg.SelectedFolder := 'C:\DB';                     
     end;
     if BrowseDlg.Execute then begin
       edtMenuMateDataBase.Text := BrowseDlg.SelectedFolder;
     end;
end;                                                                       
{ ****************************************************************************}
end.
