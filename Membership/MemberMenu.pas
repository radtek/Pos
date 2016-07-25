unit MemberMenu;
{
 Best Viewed on 
    Color Palette = 65536  
    Desktop Area 800 * 600 ( scales down to smallfonts on 640 * 480)
}
interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Menus, ToolWin, ComCtrls, CheckLst, StdForm, Db,
  DBTables, ImgList, Buttons,lmdcctrl, lmdctrl, lmdextcS, lmdstdcS, lmdcompo,
  lmdclass, lmdformA,FileCtrl,IBTable, lmdnonvS, lmdextcA, lmdctrlA,
  ovclabel ;
type
  TfrmMemberMenu = class(TForm)
    AutoBackupTimer: TTimer;
    Panel2: TPanel;
    Panel5: TPanel;
    Panel7: TPanel;
    btnDebtors: TBitBtn;
    SpeedButton5: TBitBtn;
    SpeedButton4: TBitBtn;
    SpeedButton3: TBitBtn;
    SpeedButton2: TBitBtn;
    SpeedButton1: TBitBtn;
    BitBtn1: TBitBtn;
    Panel8: TPanel;
    Panel9: TPanel;
    lblVersion: TLabel;
    Label1: TLabel;
    Image1: TImage;
    Image2: TImage;
    procedure TooBusy( Status : String );
    procedure NotBusy;
    procedure FormActivate(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure btnMaintainClick(Sender: TObject);
    procedure btnExitClick(Sender: TObject);
    procedure btnMembersClick(Sender: TObject);
    procedure btnReportsClick(Sender: TObject);
    procedure pnlPaymentsClick(Sender: TObject);
    procedure pnlSystemClick(Sender: TObject);
    procedure pnlRolloverClick(Sender: TObject);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer);
    procedure FormShow(Sender: TObject);
    procedure AutoBackupTimerTimer(Sender: TObject);
    procedure StartAutoBackup; 
    function  WriteBackupLog : Boolean; 
    function GetMiscData : Boolean;
    procedure WriteBackupLogBlob( Success : char; DirAndFileName : shortString );
    procedure FormDestroy(Sender: TObject);
    procedure UpdateBackupDate;
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure menuExitClick(Sender: TObject);
    procedure menuAboutClick(Sender: TObject);
    procedure menuDatabaseConnectionClick(Sender: TObject);
    procedure menuSystemDetailsClick(Sender: TObject);
    procedure menuCalculatorClick(Sender: TObject);
    procedure CopyTestDB;
    procedure ConnectTestDB;
    procedure btnCloseClick(Sender: TObject);

  private
    { Private declarations }
    Busy : Boolean;
    notified : Boolean;
    PrevHintMethod : TNotifyEvent;  { Save Hint method on Activate - Restore on DeActivate }
    FdirBackup : shortstring;
    FDiffFileNames : Boolean;
    FKeepDays : smallint;
    FStrings  : TStrings;
    FBackupGenID : LongInt;
    procedure DisplayHint( Sender : TObject );
  public
    { Public declarations }
    //procedure wmSysCommand(var Message : TWMSysCommand); message WM_SYSCOMMAND;
    class function ScreenCode : Integer;
    class function ScreenName : String;
    class function ScreenDesc : String;
    class function FormColor   : TColor;
    procedure wmSysCommand(var Message : TWMSysCommand); message WM_SYSCOMMAND;

  end;

var
  frmMemberMenu: TfrmMemberMenu;
  
const
   SC_COPY_DB     = $00FF;  // Windows SysCommands start at $F000 - Ours must be lessCopyFilesMsg = 'Copying files';
   SC_CONNECT_DB  = $00F0;
   SC_TEST_DB  = $00F1;

implementation

{$R *.DFM}
uses
   Global,      // StockMaster
   Splash,
   dmMembers, //StockMaster
   MembersEDIT,  // StockMaster
   MembersMAINT,  // StockMaster
   MembersREPORT,
   MembersPAY,
   MembersSYS,
   MembersCLASSES,
   Multi,
   MembersROLL;  // StockMaster
//const
   //SC_TEST        = $00FD;  // Windows SysCommands start at $F000 - Ours must be less
   //S/C_ABOUT       = $00FE;  
   //SC_TOOLBAR     = $00FC;  
   //SC_SYSTEM_DETAILS  = $00FB;  
  // SC_RESETEVENT  = $00FC;

{ **************************************************************************** }
class function TfrmMemberMenu.ScreenCode : Integer;
begin Result := scMemberMenu; end;
{ **************************************************************************** }
class function TfrmMemberMenu.ScreenName : String;
begin Result := 'Membership Menu'; end;
{ **************************************************************************** }
class function TfrmMemberMenu.ScreenDesc : String;
begin Result := '[Membership Main Menu]'; end;
{ **************************************************************************** }
class function TfrmMemberMenu.FormColor   : TColor;
begin Result := fcOther; end;
{ **************************************************************************** }
procedure TfrmMemberMenu.FormCreate(Sender: TObject);
var
   hSystemMenu : hMenu;
   i : Smallint;
begin // if this is the main screen
     if Global.MemberSystem then
        Self.Caption := Global.BusinessLogo + '     Membership';
     Caption := ScreenName; 
     lblVersion.Caption := Global.SysVersion;
     notified := False;
     Application.HintHidePause := 3000;
     Application.HintPause     := 800; 
     // Turn on timer for Auto Backup if this is the PC to do it on
     if (Global.AutoBackupPCName = ReadComputerName) and  AutoBackupEnabled then begin
        AutoBackupTimer.Enabled := True;
        FStrings := TStringList.Create;
     end;
     //dm.ImageListLarge.GetBitmap(1, btnMembers.Glyph);
     //dm.ImageListLarge.GetBitmap(2, btnPayments.Glyph);
     //dm.ImageListLarge.GetBitmap(6, btnReports.Glyph);
     //dm.ImageListLarge.GetBitmap(4, btnMaintain.Glyph);
     //dm.ImageListLarge.GetBitmap(3, btnSystem.Glyph);
     //dm.ImageListLarge.GetBitmap(5, btnRollover.Glyph);
     {if Global.DiffRatio > 1.00 then begin   // scale up on 1024 * 768 resolutions
        i := Controlbar.Height;
        Self.ChangeScale(Trunc(DiffRatio * 100),100);
        Controlbar.Height := i;
        ToolBar.Width := ControlBar.Width;
        Application.ProcessMessages;
     end;}

     //get system menu handle
     hSystemMenu := GetSystemMenu(handle, false);
     //change the Restore menu item to About with a dividing line}
     InsertMenu(hSystemMenu, 1, MF_BYPOSITION + MF_SEPARATOR, 0, NIL);
     InsertMenu(hSystemMenu, 2, MF_BYPOSITION + MF_STRING, SC_COPY_DB,'Copy DB for Testing' );
     InsertMenu(hSystemMenu, 3, MF_BYPOSITION + MF_STRING, SC_CONNECT_DB,'Connect to Test DB' );
     InsertMenu(hSystemMenu, 4, MF_BYPOSITION + MF_STRING, SC_TEST_DB,'Test Database Connection' );

end;
{ **************************************************************************** }
procedure TfrmMemberMenu.FormDestroy(Sender: TObject);
begin
     FStrings.Free;
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.TooBusy( Status : String );
begin
     //stbStatus.SimpleText  := Status;
     Screen.Cursor := crHourGlass;
     Busy := True;
     Application.ProcessMessages;
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.NotBusy;
begin
     //stbStatus.SimpleText  := '';
     Screen.Cursor := crDefault;
     Busy := False;
     Application.ProcessMessages;
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.FormActivate(Sender: TObject);
var  NewItem: TMenuItem;
     i  : integer;
begin
     // clear the Windows Menu
    // menuWindows.Clear;
     // now create and add a menu item for each form 
     {for i := 0 to Screen.FormCount -1 do  begin
        NewItem := TMenuItem.Create(Self);
        NewItem.Caption := Screen.Forms[i].Caption;
        NewItem.Name    := Screen.Forms[i].Name + '1' ;
        NewItem.OnClick := menuWindowsOnClick;
        if Screen.Forms[i]  = Screen.ActiveForm then 
           NewItem.Checked := True;
        menuWindows.Add(NewItem);
        
     end; }
     PrevHintMethod := Application.OnHint;
     Application.OnHint := DisplayHint;
     Repaint;
     Application.ProcessMessages;
end; 
{ **************************************************************************** }
procedure TfrmMemberMenu.FormShow(Sender: TObject);
var  MenuMatePointsUpdate : TMenuMatePointsUpdate;
begin
     Repaint;
     Application.ProcessMessages;
     //MenuMatePointsUpdate.MembersPointsUpdate('-1');
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.FormDeactivate(Sender: TObject);
begin
     //Application.OnHint := PrevHintMethod;
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.DisplayHint( Sender : TObject );
begin
     //stbStatus.SimpleText := GetLongHint( Application.Hint );
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.btnMaintainClick(Sender: TObject);
begin
     if frmMembersMAINT = nil then
        if sfPreCheckLogOn(TfrmMembersMAINT)
           then frmMembersMAINT := TfrmMembersMAINT.Create(self)
        else exit;
     SMStdForm.ShowStdForm(frmMembersMAINT);
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.btnExitClick(Sender: TObject);
begin
     close
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.btnMembersClick(Sender: TObject);
begin
    if frmMembersEDIT = nil then
        if sfPreCheckLogOn(TfrmMembersEDIT)
           then frmMembersEDIT := TfrmMembersEDIT.Create(self)
        else exit;
     SMStdForm.ShowStdForm(frmMembersEDIT);
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.btnReportsClick(Sender: TObject);
begin
    if frmMembersREPORT = nil then
        if sfPreCheckLogOn(TfrmMembersREPORT)
           then frmMembersREPORT := TfrmMembersREPORT.Create(self)
        else exit;
     SMStdForm.ShowStdForm(frmMembersREPORT);
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.pnlPaymentsClick(Sender: TObject);
begin
    if frmMembersPAY = nil then
        if sfPreCheckLogOn(TfrmMembersPAY)
           then frmMembersPAY := TfrmMembersPAY.Create(self)
        else exit;
     SMStdForm.ShowStdForm(frmMembersPAY);
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.pnlSystemClick(Sender: TObject);
begin
     if frmMembersSYS = nil then
        if sfPreCheckLogOn(TfrmMembersSYS)
           then frmMembersSYS := TfrmMembersSYS.Create(self)
        else exit;
     SMStdForm.ShowStdForm(frmMembersSYS);
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.pnlRolloverClick(Sender: TObject);
begin

     
     if frmMembersROLL = nil then
        if sfPreCheckLogOn(TfrmMembersROLL)
           then frmMembersROLL := TfrmMembersROLL.Create(self)
        else exit;
     SMStdForm.ShowStdForm(frmMembersROLL);
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.wmSysCommand(var Message : TWMSysCommand);
//Process the WM_SYSCOMMAND message for what we want and pass others back
begin
     case message.CmdType of
        SC_COPY_DB    : CopyTestDB;
        SC_CONNECT_DB : ConnectTestDB;
        SC_TEST_DB    : SMStdForm.menuDatabaseConnectionClick(Self);
     else
        //do the default for everything else
        inherited;
     end;
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.FormMouseMove(Sender: TObject; Shift: TShiftState;  X, Y: Integer);
const msg1 = 'The last MANUAL BACKUP was performed on the ';  
      msg2 = 'It is advised to do one now';  
      SQL1 = 'Select BACKUP,DATE_TIME from BACKUP_LOG where NOTIFIED = ''N''';
      SQL2 = 'update BACKUP_LOG set NOTIFIED = ''Y'' where BACKUP = %d';
var   Failedbackup : longInt;

begin
     if notified then exit;
     // only show once
     notified := True;
     // if Manual backup enabled and setup on this computer
     //if Global.ManBackupEnabled and (ManBackupPCName = ReadComputerName) then begin
     //   if (date - 5) >= Global.ManLastBackup then 
     //      messagedlg(format('%s %s ' + #13#10 + ' %s',[msg1,GetShortMonthDate(ManLastBackup),msg2]),mtWarning,[mbOK],0);
     //end;  
     // if Auto backup enabled and setup on this computer
     if Global.AutoBackupEnabled and (AutoBackupPCName = ReadComputerName) then begin
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL1) then begin
           Failedbackup := dm.qryGeneral.FieldByName('BACKUP').AsInteger;
           messagedlg(format('The AUTOMATIC BACKUP on %s FAILED '  + #13#10 +
                         'Please look at the Backup Log for the reason',[GetShortDateTime(dm.qryGeneral.FieldByName('DATE_TIME').AsDateTime)])
                         ,mtWarning,[mbOK],0);
           // set Notified
           dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[FailedBackup]));
           dm.IBCommitRetain;
        end;
     end;
     Self.OnMouseMove := nil;
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.AutoBackupTimerTimer(Sender: TObject);
const AutoBackupMsg = 'Automatic Database Backup';
      msg1 = 'Cannot read from table MISC for the ';
      msg2 = 'unable to write to the backup log';
      msg3 = 'User Cancelled Backup from Timer Screen';
var
   SavefrmCaption : shortstring;      
begin
     if Busy then exit;
     if (Date >= Global.AutoBackupDate) and (Time >= Global.AutoBackupTime) then begin
        try
           Busy := True;
           Screen.Cursor := crHourGlass;
           SavefrmCaption := Self.Caption;
           Self.Caption := AutoBackupMsg + ' Started ...';
           FStrings.Clear;
           with TfrmPasswordDlg.Create(nil) do begin
              try
                 ShowAsTimer := True;
                 if not GetMiscData then begin
                    FStrings.Add(msg1 + AutoBackupMsg );
                    if not WriteBackupLog then begin
                       sm( AutoBackupMsg + DidntWorkMsg + #13#10 + msg2);
                       exit;
                    end;
                    WriteBackupLogBlob('N','');
                    notified := False;
                    exit;
                 end;
                 if not WriteBackupLog then begin
                    sm( AutoBackupMsg + DidntWorkMsg + #13#10 + msg2);
                    exit;
                 end;
                 if (ShowModal = mrOK) then begin
                    Application.ProcessMessages; // come back from timer screen
                    StartAutoBackup;
                 end else begin
                    Application.ProcessMessages; // come back from timer screen
                    FStrings.Add(msg3);
                    WriteBackupLogBlob('N','');
                    notified := False;
                    // sets AutoBackupDate to tomorrow and wont refire today
                    UpdateBackupDate;
                 end;
              finally
                 release;
              end;
           end;
        finally
           Screen.Cursor := crDefault;
           Self.Caption := SavefrmCaption;
           Busy := False;
        end;
     end;
end;
{ **************************************************************************** }
function TfrmMemberMENU.GetMiscData : Boolean;
const SQL1 = 'Select A_BACKUP_DIR,A_BACKUP_FILE_NAMES,A_BACKUP_KEEP_DAYS from MISC';
begin
     Result := True;
     try
        with dm.qryGeneral do begin
           if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL1) then begin
              FDiffFileNames := StrToBoolean(FieldByName('A_BACKUP_FILE_NAMES').AsString);
              FdirBackup     := FieldByName('A_BACKUP_DIR').AsString;
              if FdirBackup[length(FdirBackup)] = '\' then  // incase c:\
                system.delete(FdirBackup,length(FdirBackup),1);
              FKeepDays      := FieldByName('A_BACKUP_KEEP_DAYS').AsInteger; 
           end else 
              Result := False;        
        end;      
     except
        Result := False;        
     end;
end;
{ **************************************************************************** }
procedure TfrmMemberMENU.StartAutoBackup;
const
    AutoBackupStart = 'the Automatic Backup is starting' + #13#10 +
                      'Press OK and stop work for several minutes' + #13#10 +
                      'until the next message';
    AutoBackupEnd = 'the Automatic Backup has completed' + #13#10 +
                    'you can continue ...';
    msg1 = 'Backup is already running';
    msg2 = 'Unexpected Automatic Backup error';
var MaxBackup : LongInt;
    strDate,DirAndFileName : shortstring;
begin
     if inTestMode then Exit;
     if not directoryexists(FdirBackup) then begin
        UpdateBackupDate;
        FStrings.Add(format('Backup %s %s',[DirNotExistMsg,FdirBackup]));
        WriteBackupLogBlob('N','');
        notified := False;
        exit;
     end;
     try  // Internal IBBAckup Service
        with dm.IBBackupService do begin
             ServerName := global.regServerName;
             Active := True;
             if IsServiceRunning then begin
                UpdateBackupDate;
                FStrings.Add(msg1 + CantContinueMsg); 
                WriteBackupLogBlob('N','');
                Active := False;
                notified := False;
                exit;
             end;
             try
               Verbose := True;
               //Options := [NonTransportable, IgnoreLimbo]; set in IBBackupService
               DatabaseName := regDataBaseName;
               // add on backup file name
               if FDiffFileNames then
                  DirAndFileName := FdirBackup + '\' + FilePrefix + formatdatetime('ddmmyy',date) + FileSuffix 
               else    // SMddmmyy.gbk  8.3 for Novel
                  DirAndFileName := FdirBackup + StockMasterBackupName;
               BackupFile.Clear;
               BackupFile.Add(DirAndFileName);
               //if Win32Platform = 2 then // NT
                //  dm.SendEvent(AutoBackupStart);// notify logged on users
               Application.ProcessMessages;
               ServiceStart;
               While not Eof do
                 FStrings.Add(GetNextLine);
               // increment date in tblMisc - also inc AutoBackupDate as app may not be restarted
               UpdateBackupDate;
               WriteBackupLogBlob('Y',DirAndFileName);
               dm.IBCommitRetain;
               if FDiffFileNames then
                  DeleteOldFiles(FdirBackup,FilePrefix,FileSuffix,FKeepDays);
               dm.DeleteLog(BackupLogTableName,'DATE_TIME',90);
             except
               UpdateBackupDate; // dont want it to keep trying
               FStrings.Add(msg2);
               WriteBackupLogBlob('N',DirAndFileName);
               notified := False;
            end;
        end;
     finally
         //if Win32Platform = 2 then // NT
         //   dm.SendEvent(AutoBackupEnd);
         dm.IBCommitRetain;
         dm.IBBackupService.Active := False;
         NotBusy;
     end;
end;
{ **************************************************************************** }
procedure TfrmMemberMENU.UpdateBackupDate;
begin
     with dm.tblMisc do begin
        if not Active then Active := True;
        Edit;
        FieldByName('A_BACKUP_DATE').AsDateTime := (Date + 1);
        Global.AutoBackupDate := (Date + 1);
        Post;
        dm.IBCommitRetain;
     end;
end;
{ **************************************************************************** }
function TfrmMemberMENU.WriteBackupLog : Boolean;
const SQL1 = 'select GEN_ID(BACKUP_GEN,0) as NEXT_BACKUP from RDB$Database';
      SQL2 = 'Insert into BACKUP_LOG (BACKUP,NAME,DATE_TIME,SUCCESS,"TYPE",' +
              'MAN_AUTO,PCNAME,NOTIFIED) values (0,''%s'',''%s'',''N'',0,''A'',''%s'',''N'')';
begin
     Result := True;
     try
        // get next value - see  WriteBackupLogBlob
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL1) then
           FBackupGenID := dm.qryGeneral.FieldByName('NEXT_BACKUP').AsInteger;
        inc(FBackupGenID);
        dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[FdirBackup,GetShortDateTime(Now),ReadComputerName]));
        dm.IBCommitRetain;
     except
        Result := False;
     end;
end;
{ **************************************************************************** }
procedure TfrmMemberMENU.WriteBackupLogBlob( Success : char ;DirAndFileName : ShortString );
var tblTemp : TIBTable;
    v : variant;
// cant seem to write a blob with SQL
begin
     with TIBTable.create(nil) do begin
        try
           Database    := dm.IBDatabase;
           Transaction := dm.IBTransaction;
           Tablename   := 'BACKUP_LOG';
           Open;
           v := FBackupGenID;
           if Locate('BACKUP',v,[]) then begin
              Edit;
              if DirAndFileName <> '' then
                 FieldByName('NAME').AsString   := DirAndFileName;
              FieldByName('LOG'    ).AsString   := FStrings.Text;
              FieldByName('SUCCESS').AsString   := Success;
              if Success = 'Y' then  // set as 'N'
                 FieldByName('NOTIFIED').AsString  := 'Y';
              Post;
           end;
        finally
           dm.IBCommitRetain;
           close;
           free;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
var i : Integer;
   Wnd : HWnd;
begin
     if ( ssAlt in Shift ) then begin
        case key of
           77,109 : btnMembersClick(Sender); // M
           82,114 : btnReportsClick(Sender); // R
           80,112 : pnlPaymentsClick(Sender);// P
           69,101 : btnMaintainClick(Sender);// E
           79,111 : pnlRolloverClick(Sender);// O
           83,115 : pnlSystemClick(Sender);  // S
           88,120 : menuExitClick(Sender); // x Close
        end;
     end;
    // if key = VK_F6 then begin
    //    Wnd := GetNextWindow( Self.Handle, GW_HWNDNEXT );
        //SetForeGroundWindow( Wnd );
        //ShowWindow(Wnd,SW_SHOWNORMAL);

    // end;
     {if key = VK_F6 then begin
        for i := 0 to Screen.FormCount -1 do begin
           if Screen.Forms[i] = Screen.ActiveForm then begin
              if i = (Screen.FormCount -1) then       
                 Screen.Forms[0].Show
              else   
                 Screen.Forms[i + 1].Show;
              Application.ProcessMessages;
              break;
           end;
        end;  
     end;}    
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.menuExitClick(Sender: TObject);
begin
     Close;
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.menuAboutClick(Sender: TObject);
begin
     //SMStdForm.menuAboutClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.menuDatabaseConnectionClick(Sender: TObject);
begin
    // SMStdForm.menuDatabaseConnectionClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.menuSystemDetailsClick(Sender: TObject);
begin
     //SMStdForm.menuSystemDetailsClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.menuCalculatorClick(Sender: TObject);
begin
     //SMStdForm.menuCalculatorClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.CopyTestDB;
var DBFrom,DBTo,TestDB : shortstring;
begin
      if not ifMsgDlg ('Make a copy of the Test Database') then exit;
      if ReadComputerName = regServerName then begin
         DBFrom := regDatabaseName;
         DBTo := ChangeFileExt(regDatabaseName, '.TST');
      end else begin
         DBFrom := '\\' + regServerName + '\' + regDatabaseName;
         TestDB := ChangeFileExt(regDatabaseName, '.TST');
         DBTo := '\\' + regServerName + '\' + TestDB;
      end;
      if not CopyFiles(DBFrom,DBTo,False) then begin
         sm('Failed to Setup the Test DB - try it on the Server');
         exit;
      end;
      MDlgI('Successfully made a Test DataBase');
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.ConnectTestDB;
begin
     if not ifMsgDlg ('Connect to the Test Database') then exit;
     regDatabaseName := ChangeFileExt(regDatabaseName, '.TST');
     dm.IBCommitRetain;
     dm.IBDatabase.Close;
     try
        frmSplash := TfrmSplash.Create(nil);
        with frmSplash do begin
           if not ConnectToDB then begin
             sm('Cannot connect to the Test DB - restart Membership');
             exit;
           end;
           // no more Exporting;
           dm.ExportTimer.Enabled := False;
           InTestMode := True;
           GSTNo := 'No GST #';
           CompName := ' *** TESTING ONLY *** ';
           Global.PrintLogo := CompName;
           Panel7.Color := fcTestMode;
           Self.Caption := Self.Caption + TestDBMsg;
        end;
        MDlgI('Connected Successfully to the Test DataBase');
     finally
        frmSplash.Free;
     end;
end;
{ **************************************************************************** }
procedure TfrmMemberMenu.btnCloseClick(Sender: TObject);
begin
     Close;
end;

end.
