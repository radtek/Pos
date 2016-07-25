unit Multi;
{
 Multiple uses
 
 Countdown timer Screen - Automatic Backup
 About Detail Screen - uses memo
 BackupLog Blob Viewer Screen - uses memo

 see FormShow
}
interface

uses Windows, SysUtils, Classes, Graphics, Forms, Controls, StdCtrls, 
  Buttons,Db,Dialogs,  ComCtrls,  Gauges, ExtCtrls, 
     lmdcompo, lmdclass, lmdformA, lmdctrl,
  lmdextcA, lmdstdcS, lmdextcS, lmdcctrl, Lmdled, lmdnonvS, stdForm, Menus,ToolWin, Grids  ;
type
  TfrmPasswordDlg = class(TSMStdForm)
    Memo: TMemo;
    lblTimer: TLabel;
    btnCancelTimer: TBitBtn;
    CountDownTimer: TTimer;
    btnClose: TToolButton;
    menuExit: TMenuItem;
    strGrid: TStringGrid;
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure LoadStats;
    procedure CountDownTimerTimer(Sender: TObject);
    procedure btnCancelTimerClick(Sender: TObject);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure menuExitClick(Sender: TObject);
    
  private
    { Private declarations }
  public
    { Public declarations }
    ShowAsSystemDetails : Boolean; //  as about System Details
    ShowAsMemo  : Boolean; // and as a show Memo form
    ShowAsTimer : Boolean; // and as a Countdown Timer form for auto backup
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;                       

var
  frmPasswordDlg: TfrmPasswordDlg;

implementation

uses
    Global,     //StockMaster
    Registry,
    dmMembers;  //StockMaster

{$R *.DFM}
{ **************************************************************************** }
{ **************************************************************************** }
class function TfrmPasswordDlg.ScreenCode : Integer;
begin Result := Global.scMembersDetails; end;
{ **************************************************************************** }
class function TfrmPasswordDlg.ScreenName : String;
begin Result := 'Members Details'; end;
{ **************************************************************************** }
class function TfrmPasswordDlg.ScreenDesc : String;
begin Result := 'Members Details Screen'; end;
{ **************************************************************************** }
procedure TfrmPasswordDlg.FormCreate(Sender: TObject);
begin
     //Brush.Style := bsClear; 
     inherited;
     ShowAsSystemDetails := False;
     ShowAsMemo  := False;
     ShowAsTimer := False;
end;   
{ **************************************************************************** }
procedure TfrmPasswordDlg.FormShow(Sender: TObject);
const msg1 = 'System Details ...';
      msg2 = 'Backup Log ...';
      msg3 = 'CountDown for Auto Backup ...';
begin
     if ShowAsSystemDetails then begin
        Self.Caption := msg1;
        strGrid.Visible := True;
        LoadStats;
        exit;
     end;   
     if ShowAsMemo then begin
        Self.Caption := msg2;
        memo.Visible         := True;
        exit;
     end;
     if ShowAsTimer then begin
        Self.Caption := msg3;
        lblTimer.Visible       := True;
        btnCancelTimer.Visible := True;
        CountdownTimer.Enabled := True;
        btnClose.Enabled := False;
        exit;
     end;
end;
{ **************************************************************************** }
procedure TfrmPasswordDlg.LoadStats;
var s : string;
    i,j : longint;
const
   m = 0;
   n = 1;
   ml  = 'Member Licenses';
   cn  = 'Computer Name';
   sn  = 'Server Name';
   slo = 'System Log On';
   lun = 'Local User Name';
   loa = 'Logged on as';
   ver = 'Interbase Version  ';
   dbf = 'DB FileName';
   cdb = 'Curr DB Memory Used';
   mdb = 'Max DB Memory Used';
   uc  = 'Update Count';
   ic  = 'Insert Count';
   dc  = 'Delete Count';
   mc  = 'Member Count';
   lu  = 'Logged on User Count = ';
   os  = 'Operating System';
   pt  = 'Processor Type';
   mt  = 'Memory Total';
   mf  = 'Memory Free';
   sh  = 'Screen Height';
   sp  = 'Screen PPI';
   ss  = 'Screen Saver';
   ssd = 'Screen Saver Delay';
   dl  = 'Default Language';
   tz  = 'Time Zone';
   sdf = 'Short Date Format';
   stf = 'Short Time Format';
begin
     s := Global.XorDecode(copy(Global.CompName,1,3),Global.MemberLicense);
     s := Global.GetLicenseNumber(s);
     with strGrid do begin
        Cells[m,0] := 'Version'; Cells[n,0] := Global.SysVersion; 
        Cells[m,1] := ml;        Cells[n,1] := s; 
        Cells[m,2] := cn;        Cells[n,2] := dm.SysInfo.Computername; 
        Cells[m,3] := sn;        Cells[n,3] := regServerName; 
        Cells[m,4] := slo;       
        if regLogon then         
           Cells[n,4] := 'Yes'
        else                     
           Cells[n,4] := 'No';
        Cells[m,5] := lun;       Cells[n,5] := regLocalUserName; 
        Cells[m,6] := loa;       Cells[n,6] := Global.UserName; 
        with dm.IBDatabaseInfo do begin
           Cells[m,7]  := ver + VERSION;  Cells[n,7]  := 'Base Level = ' + IntToStr(BASELEVEL); 
           Cells[m,8]  := dbf;            Cells[n,8]  := DBFileName; 
           Cells[m,9]  := cdb;            Cells[n,9]  := IntToStr(trunc(CurrentMemory / 1024)); 
           Cells[m,10] := mdb;            Cells[n,10] := IntToStr(trunc(MaxMemory / 1024)); 
           Cells[m,11] := uc;             Cells[n,11] := IntToStr(UpdateCount.Count); 
           Cells[m,12] := ic;             Cells[n,12] := IntToStr(InsertCount.Count); 
           Cells[m,13] := dc;             Cells[n,13] := IntToStr(DeleteCount.Count); 
           Cells[m,14] := mc;             Cells[n,14] := Global.GetMemberCount; 
        end;
        with dm.SysInfo do begin
           Cells[m,15] := os;             Cells[n,15] := Global.GetOpSystem; 
           Cells[m,16] := pt;             Cells[n,16] := ProcessorType; 
           Cells[m,17] := mt;             Cells[n,17] := IntToStr(MemPhysTotal); 
           Cells[m,18] := mf;             Cells[n,18] := IntToStr(MemPhysFree); 
           Cells[m,19] := sh;             Cells[n,19] := IntToStr(ScreenHeight); 
           Cells[m,20] := sp;             Cells[n,20] := IntToStr(ScreenPPI); 
           if ScreenSaveActive then begin
              Cells[m,21] := ss;          Cells[n,21] := 'Active';
              Cells[m,22] := ssd;         Cells[n,22] := IntToStr(ScreenSaverDelay) + ' minutes';
           end else begin   
              Cells[m,21] := ss;          Cells[n,21] := 'Not Active';
              Cells[m,22] := ssd;         Cells[n,22] := '';
           end;   
           j := 23;
           for I := 0 to dm.IBDatabaseInfo.UserNames.Count - 1 do begin
              Cells[n,j] := dm.IBDatabaseInfo.UserNames[I]; 
              inc(j);
           end;   
           Cells[m,23] := lu + IntToStr(i);             
           Cells[m,j] := dl;             Cells[n,j] := DefaultLanguageString;
           inc(j);
           Cells[m,j] := tz;             Cells[n,j] := TimeZone;
           inc(j);
           Cells[m,j] := sdf;            Cells[n,j] := LocaleShortDateFormat;
           inc(j);
           Cells[m,j] := stf;            Cells[n,j] := LocaleShortTimeFormat;
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmPasswordDlg.CountDownTimerTimer(Sender: TObject);
// btncaneltimer  - modal result mrCancel
var tmr : shortint;
begin
     tmr := StrToInt(lblTimer.Caption);
     dec(tmr);
     lblTimer.Caption := IntToStr(tmr);
     if tmr <= 0 then
        ModalResult := mrOK;
end;
{ **************************************************************************** }
procedure TfrmPasswordDlg.btnCancelTimerClick(Sender: TObject);
begin
     CountDownTimer.Enabled := False;
end;
{ **************************************************************************** }

{ **************************************************************************** }
procedure TfrmPasswordDlg.menuExitClick(Sender: TObject);
begin
     Close;
end;
{ **************************************************************************** }           
procedure TfrmPasswordDlg.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     case key of
         VK_ESCAPE : menuExitClick(Sender);
     end; 
     if ( ssAlt in shift ) then begin
        case key of 
           88,120 : menuExitClick(Sender);  // X Close 
        end;
     end;   
end;


end.








           
