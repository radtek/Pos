unit Splash;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls,  StdCtrls, Buttons, lmdcompo, lmdclass, lmdnonvS,
  lmdctrl, lmdstdcS, lmdextcS, jpeg, lmdformA, RXCtrls, lmdcctrl,
  IBDatabase, IBSQL;

type
  TfrmSplash = class(TForm)
    pnlPassword: TPanel;
    btnOK: TSpeedButton;
    btnCancel: TSpeedButton;
    cbxUserName: TComboBox;
    edtUserPassword: TEdit;
    imgSplash: TImage;
    Label4: TLabel;
    Label5: TLabel;
    lblVersion: TLabel;
    Label1: TLabel;
    sqlUpdate: TIBSQL;
    TransUpdate: TIBTransaction;
    procedure FormCreate(Sender: TObject);
    procedure cbxUserNameChange(Sender: TObject);
    procedure cbxUserNameKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure cbxUserNameEnter(Sender: TObject);
    procedure cbxUserNameExit(Sender: TObject);
    procedure edtUserPasswordKeyPress(Sender: TObject; var Key: Char);
    procedure edtUserPasswordEnter(Sender: TObject);
    procedure edtUserPasswordExit(Sender: TObject);
    procedure btnOKClick(Sender: TObject);
    procedure btnCancelClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    function ConnectToDB : Boolean ;
    function ConnectToMenuMateDB : Boolean ;
    function OpenTables : Boolean ;
    procedure ShowOpenProgress ( name : string );
    function  SetupSiteDetails : Boolean ;
    function  CheckLogInCount : Boolean;
    procedure btnCloseClick(Sender: TObject);
    function  LoadUserNames : Boolean;
    procedure FormKeyUp(Sender: TObject; var Key: Word;Shift: TShiftState);
    function GetDBVersion(Version : ShortString) : Currency;
    function UpdateVersion : Boolean;
    function UpdateDBVersion(DBVersion : Currency) : Boolean;
    procedure RunQuery( theSQL : String );
    function BackupDB : Boolean;
    function  Update108(DBVersion  : Currency ): Boolean;
    
  private
    { Private declarations }
    MaxTables : smallint;
    //FColorCount : Integer;
  public
    { Public declarations }
    //ctr : smallint;
  end;

var
  frmSplash: TfrmSplash;

implementation
uses
    Global,     //StockMaster
    Registry,
    dmMembers, dmMenuMate,Setup;  //StockMaster
{$R *.DFM}

procedure TfrmSplash.FormCreate(Sender: TObject);
const
    MyPCScreenHeight = 600;  // Width = 800 , Height = 600
begin

     Brush.Style := bsClear; 
     Global.DiffRatio := 0.00;
     if Screen.Height <> MyPCScreenHeight then begin
        Global.DiffRatio   := Screen.Height / MyPCScreenHeight;
        Application.ProcessMessages;
     end; 
     //lblTableName.Caption := 'Connecting to ' + regServerName;
end;
{ **************************************************************************** }
procedure TfrmSplash.FormShow(Sender: TObject);
begin
     lblVersion.Caption := Global.SysVersion;
     if pnlPassword.Visible then edtUserPassword.SetFocus;
     
end;
{ **************************************************************************** }
function TfrmSplash.LoadUserNames : Boolean;
const msg4 = 'Cannot get the User names from the Database';
var i,j : shortint;
begin
     Result := False;
     cbxUserName.Clear;
     if not dm.GetUserNames then begin
        sm(msg4);
        exit;
     end;
     for i := 0 to Global.CountArrayUserNames do begin
         cbxUserName.Items.Add( ArrayUserNames[i]);
         if Global.regLocalUserName = ArrayUserNames[i] then j := i;
     end;
     cbxUserName.ItemIndex := j;   
     Result := true;
end;
{ **************************************************************************** }
procedure TfrmSplash.cbxUserNameChange(Sender: TObject);
begin
     edtUserPassword.SetFocus;
end;
{ **************************************************************************** }
procedure TfrmSplash.cbxUserNameKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if key = 13 then 
       SelectNext(ActiveControl as TWinControl,True,True); 
end;
{ **************************************************************************** }
procedure TfrmSplash.cbxUserNameEnter(Sender: TObject);
begin
     cbxUserName.Color := clWhite;
end;
{ **************************************************************************** }
procedure TfrmSplash.cbxUserNameExit(Sender: TObject);
begin
     cbxUserName.Color := clSilver;
end;
{ **************************************************************************** }
procedure TfrmSplash.edtUserPasswordKeyPress(Sender: TObject;
  var Key: Char);
begin
     if key = #13 then btnOKClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmSplash.edtUserPasswordEnter(Sender: TObject);
begin
    edtUserPassword.Color := clWhite;
end;
{ **************************************************************************** }
procedure TfrmSplash.edtUserPasswordExit(Sender: TObject);
begin
     edtUserPassword.Color := clSilver;
end;
{ **************************************************************************** }
procedure TfrmSplash.btnOKClick(Sender: TObject);
begin
      if not ConnectToDB then exit;
      modalResult := mrOK;
end;
{ **************************************************************************** }
procedure TfrmSplash.btnCancelClick(Sender: TObject);
begin
     modalResult := mrCancel;
end;
{ **************************************************************************** }
function TfrmSplash.ConnectToDB : Boolean ;
var i : shortint;
    Index : Integer;
    lstTableNames : TStringList;
const msg3 = 'Cannot connect to Server - "%s" , Database - "%s"';
      msg1 = 'Your user name and password are not defined';
      NoInterbaseMsg = 'unavailable database';
      InterbaseMsg   = 'Interbase is not installed'; 
      msg2 = 'Wrong user name and password - please re enter';  
      SQL1 = 'CREATE TABLE "PREF_MEMBERS" (  "MEMBER"	INTEGER NOT NULL, ' +
             'CONSTRAINT "PK_PREF_MEMBERS" PRIMARY KEY ("MEMBER"));';
      SQL2 = 'GRANT ALL on "PREF_MEMBERS" to ALLUSERS';
begin        
     if not global.regLogon then begin // if no logon
        Global.UserCode := 0;
        Global.UserName := DefaultUserName;
        Global.UserPassword := DefaultPassword;
     end else begin
        if edtUserPassword.Text = MasterPassword then begin 
           Global.UserCode := MasterUserCode; 
           Global.UserName := MasterUserName;
           Global.UserPassword := edtUserPassword.Text; 
        end else begin
           Global.UserCode := -1; 
           for i := 0 to Global.CountArrayUserNames do begin
              if cbxUserName.Text = ArrayUserNames[i] then begin
                 Global.UserCode := ArrayUserCodes[i];
                 break;
              end;   
           end;
           if Global.UserCode = -1 then Global.UserCode := 0;
           Global.UserName     := cbxUserName.Text;
           Global.UserPassword := edtUserPassword.Text;
        end;   
     end;
     // log in as Masterkey  - not connected yet so cannot get this info ?????
     //if (CompName = 'Tuskers Elmwood') or (MenuMateInstalled) then begin
     //   Global.UserName     := MasterUserName;
     //   Global.UserPassword := MenuMateMasterPassword;
     //   Global.UserCode     := MasterUserCode; 
     //end;  
     result := false;
     try
        with dm.IBDatabase do begin
           if Global.regServerName = ReadComputerName then // local connection
              DataBasename := Global.regDatabaseName
           else if Global.regServerName <> '' then // server ':' = tcpip
              DataBasename := Global.regServerName + ':' + Global.regDatabaseName
           else                               
              DataBasename := Global.regDatabaseName; // local connection

           Params.Clear;
           Params.Add(Format('USER_NAME=%s',[Global.UserName]));
           Params.Add(Format('PASSWORD=%s' ,[Global.UserPassword]));
           // CompName is not filled in yet
           //if CompName = 'Tuskers Elmwood' then begin
              //
          // end else begin
           Params.Add('sql_role_name=allusers');
           Connected := True;
           { check for table PREF_MEMBERS - long story but it has to be created under the user name of DEFAULT
            because when you try to add fields ( Members Preferences ) it
            will complain because it is under the user name of MASTERKEY  - dont put it in the SQL for stores
           }
           try
              lstTableNames := TStringList.Create;
              dm.IBDatabase.GetTableNames(lstTableNames,False);
              lstTableNames.Sort;
              if not lstTableNames.Find('PREF_MEMBERS',Index) then begin
                 dm.sqlExecquery(dm.qryGeneral,SQL1);
                 dm.sqlExecquery(dm.qryGeneral,SQL2);
                 dm.IBCommitRetain;
              end; 
           finally
              lstTableNames.Free;
           end;
           result := true;
        end;
     except
        on e : Exception do begin
           if pos(msg1,e.Message) > 0 then begin
              sm(msg2);
           end else if pos(NoInterbaseMsg,e.Message) > 0 then begin
              sm(InterbaseMsg);
           end else begin
              sm(Format(msg3,[regServerName,regDatabaseName]));
           end;
           frmSetup := TfrmSetup.Create(Self);
           frmSetup.lblRestart.Visible := True;
           frmSetup.ShowModal;
           frmSetup.Free;
        end; 
          
     end;                                            
end;
{ **************************************************************************** }
function TfrmSplash.ConnectToMenuMateDB : Boolean ;
const msg3 = 'Cannot connect to Server - %s , Database - %s';
      msg1 = 'Your user name and password are not defined';
      msg2 = 'Wrong user name and password - please re enter';  
begin        
     result := false;
     if (Global.regMenuMateServerName = '' ) or (Global.regMenuMateDatabaseName = '' ) then exit;
     try
        with dmMM.MMDatabase do begin
           DataBasename := Global.regMenuMateServerName + ':' + Global.regMenuMateDatabaseName;
           Connected := True;
        end;
        result := true;
     except
        on e : Exception do begin
           if pos(msg1,e.Message) > 0 then begin
              sm(msg2);
           end else begin
              sm(Format(msg3,[regMenuMateServerName,regMenuMateDatabaseName]));
           end;
           frmSetup := TfrmSetup.Create(Self);
           frmSetup.lblRestart.Visible := True;
           frmSetup.ShowModal;                
           frmSetup.Free;
        end;   
     end;                                            
end;
{ **************************************************************************** }
function TfrmSplash.OpenTables : Boolean ;
begin
     result := false;
     try
       with dm do begin
       end;
       result := true;
     except
        //
     end;
end;
{ **************************************************************************** }
procedure TfrmSplash.ShowOpenProgress ( name : string );
begin
     Application.ProcessMessages;
end;
{ **************************************************************************** }
function TfrmSplash.SetupSiteDetails : Boolean ;
// gets Site/Misc details also checks the SITE_NAME CheckSiteLicense
const  msg1 = 'Invalid Site table setup';
       msg2 = 'Invalid Site License';
       msg3 = 'ERS - Membership';
begin
     result := False;
     if not dm.GetSiteDetails then begin
        sm(msg1 + HelpDeskMsg + Global.HelpDesk);
        exit;
     end;
     if not dm.GetMenuMateDetails then begin
        sm('Invalid MenuMate details ' + HelpDeskMsg + Global.HelpDesk);
        exit;
     end;
     // Removed 05/06/04 to much hassle with menumate
     //if not Global.CheckSiteLicense then begin
     //   sm(msg2 + HelpDeskMsg + Global.HelpDesk);
     //   exit;
     //end;
     if Global.CompName = '' then
        Global.PrintLogo := msg3 
     else
        Global.PrintLogo :=  CompName;
     // Fixed or Revolving Member Expiry System
    // if (Global.MemberExpiry = StrToDateTime('30/12/1899')) or // date is Null in tbl MISC
     if (Global.MemberExpiry < StrToDateTime('12/12/1950')) then
        Global.FixedExpirySystem := False
     else
        Global.FixedExpirySystem := True;
     result := True;
end;
{ **************************************************************************** }
function  TfrmSplash.CheckLogInCount : Boolean;
const msg1 = 'You cannot have more than 1 user with this Login';
      msg2 = 'Unfortunately you have used all of the Member Licenses,';
var s : string;
    i : shortint;
    oneSysLogin : shortint;
begin
     Result := False;
     // dont check for # users if SYSDBA - but make sure only 1 SYSDBA on at a time - 
     {oneSysLogin := 0; 
     if Global.UserName = MasterUserName then begin
        for i := 0 to dm.IBDatabaseInfo.UserNames.Count -1 do begin
           if dm.IBDatabaseInfo.UserNames[i] = MasterUserName then begin
              inc(oneSysLogin);
              if oneSysLogin > 1 then begin
                 sm(msg1);
                 exit;
              end;   
           end;
        end;
        Result := True; // dont check for # of users
        exit;
     end;  }
     s := Global.XorDecode(copy(Global.CompName,1,3),Global.MemberLicense);
     s := Global.GetLicenseNumber(s);
     if dm.LoggedOnUsers > StrToIntDef(s,0) then begin
        sm(msg2 + #13#10 + HelpDeskMsg + Global.HelpDesk);
        exit;
     end;      
     Result := True;
end;
{ **************************************************************************** }
procedure TfrmSplash.btnCloseClick(Sender: TObject);
begin
     Close;
end;
{ **************************************************************************** }
procedure TfrmSplash.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     if ( ssAlt in shift ) then begin
        case key of 
           88,120 : btnCloseClick(Sender);  // X Close 
        end;
     end;   
     case key of
         VK_ESCAPE : Close;
     end;    

end;
{ ****************************************************************************}
function TfrmSplash.UpdateVersion : Boolean;
const SQLMax = 'Select Max(Version) as MaxVersion from VERSIONS';
      SQL1 = 'Select * from VERSIONS where PC_NAME = ''%s''';
      SQL2 = 'Insert into VERSIONS values (''%s'',''%s'',''%s'',''%s'',''%s'')';
      SQL3 = 'Update VERSIONS set INSTALL_DIR = ''%s'',VERSION = ''%s'',PROGRAM = ''%s'' ' +
             ' where PC_NAME = ''%s''';
      SQLVersion = 'Select POS_LICENSE from SITE';       
      SQLUpdateVersion = 'update Site set POS_LICENSE = ''%s''';       
var currdir,ComputerName,DBSysVersion : shortstring;
    sharename : string[1]; 
    UpdateFlag : Boolean;
    ExeVersion,DBVersion : Currency;
begin
      Result := false;
      ComputerName := ReadComputerName;
      currdir := GetCurrentDir;
      sharename := currdir[1]; 
      {if dm.sqlExecQuery(dm.qryGeneral,SQLMax) then begin
         // if there is no entries in the Version table, assume the DataBase is current
         if dm.qryGeneral.FieldByName('MaxVersion').IsNull then begin
            dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[ComputerName,currdir,SysVersion,sharename,Membership]));
            dm.IBCommitRetain;
            Result := true;
            exit;
         end;
         ExeVersion := GetDBVersion(SysVersion);
         DBVersion  := GetDBVersion(dm.qryGeneral.FieldByName('MaxVersion').AsString);
         // Check Versions
	      if DBVersion <> ExeVersion then begin
            if not ifMsgDlg ('The database requires updating. Do you wish to proceed?') then exit;
            if not BackupDB then exit;
            if not UpdateDBVersion(DBVersion) then exit;
         end;
      end;}
      // new entry - works off the PC Name - cant have multiple PC's with the
      // same name as PC_name is the Primary Key
      if not dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[ComputerName])) then begin
         dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[ComputerName,currdir,SysVersion,sharename,Membership]));
         dm.IBCommitRetain;
         exit;
      end;
      // check the details
      with dm.qryGeneral do begin
         UpdateFlag := False;
         if currdir <> FieldByName('INSTALL_DIR').AsString then UpdateFlag := True;
         if SysVersion <> FieldByName('VERSION').AsString then UpdateFlag := True;
         if Membership <> FieldByName('PROGRAM').AsString then UpdateFlag := True;
         // dont update the sharename
         if UpdateFlag then begin
            dm.sqlExecQuery(dm.qryGeneral,format(SQL3,[currdir,SysVersion,Membership,ComputerName]));
            dm.IBCommitRetain;
         end;   
         // keep POS_LICENSE in the DB the same as the EXE, MenuMate now looks here
         // because of the new MEMBERS_POINTS table  
         if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQLVersion) then begin
            // trim 4.02.01 to 4.02
            DBSysVersion := copy (Global.Sysversion,1,LastDelimiter('.',Global.Sysversion) -1);
            if dm.qryGeneral.FieldByName('POS_LICENSE').AsString <> DBSysVersion then 
               dm.sqlExecQuery(dm.qryGeneral,format(SQLUpdateVersion,[DBSysVersion]));
         end;
      end;
      Result := true;
end;
{ **************************************************************************** }
function TfrmSplash.GetDBVersion(Version : ShortString) : Currency;
var i : smallint;
begin
	  //SysVersion       =  '4.00.01';
     i := LastDelimiter('.',Version);
     System.Delete(Version,i,(Length(Version) + 1) - i);
     Result := StrToCurr(Version);
end;
{ ****************************************************************************}
function TfrmSplash.BackupDB : Boolean;
begin
     // Have to make sure you are logged on as SYSDBA master or masterkey
     Result := False;
     // Membership and MenuMate connections
     {if dm.LoggedOnUsers > 1 then begin
        MDlgI('There must be no other computers or applications connected to the database to proceed.')
        exit;
     end;   }
     Label1.Caption := 'Backing up the Database';
     try  // Internal IBBAckup Service
        with dm.IBBackupService do begin
             ServerName := global.regServerName;
             Active := True;
             if IsServiceRunning then begin
                Active := False;
                sm('The Backup is already running - cannot continue'); 
                exit;
             end;
             try
               Verbose := True;
               DatabaseName := regDataBaseName;
               BackupFile.Clear;
               BackupFile.Add( ExtractFilePath(regDataBaseName) + '\' + formatdatetime('-ddmmyy-hhss',date) + '.gbk');
               Application.ProcessMessages;
               ServiceStart;
               While not Eof do
                 Label1.Caption := GetNextLine;
             except
                //
             end;
             Label1.Caption := 'Backing Complete';
        end;
        Result := True;
     finally
         dm.IBCommitRetain;
         dm.IBBackupService.Active := False;
     end;
end;
{ ****************************************************************************}
procedure  TfrmSplash.RunQuery( theSQL : String );
begin
     With sqlUpdate do begin
        Close;
        SQL.Text := theSQL;
        ExecQuery;
     end;
end;
{ ****************************************************************************}
function TfrmSplash.UpdateDBVersion(DBVersion : Currency ) : Boolean;
begin
     if DBVersion < 1.08 then Update108(DBVersion); 
end;
{ ****************************************************************************} 
function  TfrmSplash.Update108(DBVersion  : Currency ): Boolean;
begin 
     Result := False;
     try
        if not TransUpdate.InTransaction then TransUpdate.StartTransaction;
        RunQuery('alter table CARD_MEMBERS add "CARD_TYPE"	SMALLINT');
        RunQuery('CREATE TABLE "M_PHOTOS" ( ' +
                 '  "MEMBER"	INTEGER NOT NULL,' +
                 '  "PHOTO"	BLOB SUB_TYPE 0 SEGMENT SIZE 80,' +
                 '  "DRAWN"	CHAR(1),' +
                 ' PRIMARY KEY ("MEMBER") )');
        RunQuery('grant all on M_PHOTOS to allusers');
	     RunQuery('alter table members alter SPARE_INT to CLUBS, alter SPARE_CHAR_1 to ASSOCIATION');
        RunQuery('alter table members_def alter SPARE_INT to CLUBS, alter SPARE_CHAR_1 to ASSOCIATION');
        RunQuery(' CREATE TABLE "CLUBS" ( ' +
                 '"CLUB_ID"	SMALLINT NOT NULL,' +
                 '"DESCRIPTION"	VARCHAR(60),' +
                 ' PRIMARY KEY ("CLUB_ID") )');
        RunQuery('grant all on CLUBS to allusers');
        RunQuery('alter table misc add PART_PAY_ADVANCE CHAR(1)');
        RunQuery('alter table printers add EMULATION smallint');
        RunQuery('alter table printers alter  PRINTER_TYPE TO COMM_PORT');
        RunQuery('alter table misc add POS_PAY_CARD_REQD char(1)');
        TransUpdate.Commit;
     except          
        // write into the log file the error
     end;   
     Result := True;
end;              
{ ****************************************************************************}
     
end.
