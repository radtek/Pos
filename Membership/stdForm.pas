unit stdForm;

interface
                        
uses       
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls,   ComCtrls, Menus,  CheckLst, Db,
  DBTables, ImgList, Buttons, wwDialog, wwrcdvw, FileCtrl, RXCtrls,
  Wwdbdlg,wwrcdpnl,wwdblook,wwDataInspector,wwDbGrid,ToolWin,wwdbedit,wwdbcomb;

  //WinSpool,  ,Printers,Registry,, ,,,,extctrls;
   
type
  TSMStdForm = class(TForm)
    stbStatus: TStatusBar;
    ControlBar1: TControlBar;
    ToolBar: TToolBar;
    MainMenu: TMainMenu;     
    menuFile: TMenuItem;
    menuHelpTop: TMenuItem;
    menuAbout: TMenuItem;
    menuDatabaseConnection: TMenuItem;
    menuSystemDetails: TMenuItem;
    menuHelp: TMenuItem;
    menuWindows: TMenuItem;
    pnlFill: TPanel;
    menuCalculator: TMenuItem;
    procedure FormActivate(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure menuDatabaseConnectionClick(Sender: TObject);
    procedure menuAboutClick(Sender: TObject);
    procedure menuSystemDetailsClick(Sender: TObject);
    procedure menuHelpClick(Sender: TObject);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure FormResize(Sender: TObject);
    procedure menuCalculatorClick(Sender: TObject);
  private
    { Private declarations }
    StatusType : Boolean;  { Save the Status Bar Type }
    PrevHintMethod : TNotifyEvent;  { Save Hint method on Activate - Restore on DeActivate }
  public
    { Public declarations }
    Busy : Boolean;         { Flag to prevent multiple activations }
    OneButtonWidth : ShortInt;
    TwoButtonWidth : ShortInt;
    procedure DisplayHint( Sender : TObject );
    procedure TooBusy( Status : String );
    procedure NotBusy;
    procedure AppIdle(Sender: TObject; var Done: Boolean);
    procedure AppException(Sender: TObject; E: Exception);
    procedure menuWindowsOnClick( Sender : TObject );
    procedure ShowStdForm ( theForm : TForm );
    procedure FormPrintCards;
   // procedure ResizeToolBar( theForm : TSMStdForm ; theWindowState : TWindowState; 
   //                          theButtonWidth : ShortInt ; pnlWidth,pnlLeft : SmallInt ); 
    
    procedure wmSysCommand(var Message : TWMSysCommand); message WM_SYSCOMMAND;
    class function  ScreenCode  : Integer; virtual; abstract;
    class function  ScreenName  : String; virtual; abstract;
    class function  ScreenDesc  : String; virtual; abstract;
    class function  FormColor   : TColor; virtual;
  end;

type

TSMStdFormClass = class of TSMStdForm;

function sfPreCheckLogOn(  TSMForm : TSMStdFormClass ) : Boolean;

var
  SMStdForm: TSMStdForm;

{const
   SC_TEST        = $00FD;  // Windows SysCommands start at $F000 - Ours must be less
   SC_ABOUT       = $00FE;  
   SC_SYSTEM_DETAILS  = $00FB;  
   SC_TOOLBAR     = $00FC;                                           
 }
implementation

uses
   Global,      // StockMaster
   Splash,
   Multi,
   dmMembers, 
   MemberMenu, 
   MembersCARDS,
   rInvoiceLandscape,
   MembersPAY, ShowHelp, dmMenuMate;



{$R *.DFM}
{ **************************************************************************** }
procedure TSMStdForm.TooBusy( Status : String );
begin
     StatusType := stbStatus.SimplePanel;
     stbStatus.SimplePanel := True;
     stbStatus.SimpleText  := Status;
     Screen.Cursor := crHourGlass;
     Busy := True;
     Application.ProcessMessages;
end;
{ **************************************************************************** }
procedure TSMStdForm.NotBusy;
begin
     stbStatus.SimpleText  := '';
     stbStatus.SimplePanel := StatusType;
     Screen.Cursor := crDefault;
     Busy := False;
     Application.ProcessMessages;
end;
{ **************************************************************************** }
procedure TSMStdForm.DisplayHint( Sender : TObject );
begin
     stbStatus.Panels[1].Text := GetLongHint( Application.Hint );
end;
{ **************************************************************************** }
//function FindaWin: hWnd;
//begin
//  Result := FindWindow(nil, 'Title');
//end;
{ **************************************************************************** }
procedure TSMStdForm.menuWindowsOnClick( Sender : TObject );
var  i  : integer;
     frmName : string;
begin
     for i := 0 to Screen.FormCount -1 do begin
        frmName := Screen.Forms[i].Name;
        system.delete(frmName,length(frmName),1);
        if frmName= TMenuItem(sender).Name then begin
           // dont reshow this form 
           if frmName = 'frmMembersLookup' then break;
           ShowStdForm(Screen.Forms[i]);
           Application.ProcessMessages;
           break;
        end;
     end;
end;
{ **************************************************************************** }
procedure TSMStdForm.FormActivate(Sender: TObject);
var  NewItem: TMenuItem;
     i  : integer;
begin
     // clear the Windows Menu
     menuWindows.Clear;
     // now create and add a menu item for each form 
     for i := 0 to Screen.FormCount -1 do  begin
        try
           NewItem := TMenuItem.Create(Self);
           NewItem.Caption := Screen.Forms[i].Caption;
           NewItem.Name    := Screen.Forms[i].Name + '1';
           NewItem.OnClick :=  menuWindowsOnClick;
           if Screen.Forms[i]  = Screen.ActiveForm then 
              NewItem.Checked := True;
           menuWindows.Add(NewItem);
        except
           // do nothing 
        end;   
     end; 
     PrevHintMethod := Application.OnHint;
     Application.OnHint := DisplayHint;
     if InTestMode then begin
        if POS(TestDBMsg,Caption) = 0 then
           Caption := ScreenName + TestDBMsg;
        stbStatus.Color := fcTestMode;
     end;
     Application.ProcessMessages;
end;
{ **************************************************************************** }
procedure TSMStdForm.FormDeactivate(Sender: TObject);
begin
     Application.OnHint := PrevHintMethod;
end;
{ **************************************************************************** }
procedure TSMStdForm.FormCreate(Sender: TObject);
var i,n : integer;                             
    temp : TComponent;
    k,b,j,l,m : smallint;
    s : string;
    len : string[3];
    hSystemMenu : hMenu;                  
begin
     // Novell does not like print screen
     //Application.OnIdle := AppIdle;   //remove while debugging for printing
     Color := FormColor;
     // leave here because at design time the toolbar is less than controlbar1 width
     Caption := ScreenName;
     Application.HintHidePause := 3000; //
     Application.HintPause     := 800; // 
     Busy := False;
     // set in Password unit
     if Global.DiffRatio  <> 0.00 then begin // different resolution
        // Default MS Sans Serif lowest font size is 8
        // Small Fonts lowest is 2
        // if a smaller resolution then change font
        // ParentFont Property must be set to True on components
        {if DiffRatio < 1.00 then begin  // 600 * 480 resolution
           if (self.Name = 'frmMembersEDIT') or  // only resize these forms
              (self.Name = 'frmMembersSYS')  or 
              (self.Name = 'frmMembersPAY')  then begin
              // changes all components on form
              // save control bar height and form width - then restore
              // it because you must see all the toolbar controls
              i := Controlbar1.Height;
              n := Self.Width;   
              Self.ChangeScale(Trunc(DiffRatio * 100),100); 
              Application.ProcessMessages;
              Controlbar1.Height := i;
              ToolBar.Width  := ControlBar1.Width;
              Self.Width := n;   
              Self.Height := Self.Height + 40;   
              Self.Font.Name := 'Small Fonts';
              Self.Font.Size := 7;
              //ToolBar.Align := alClient;
              // Label Fonts not affected by Parent Font - MembersSys Form
              for I := ComponentCount - 1 downto 0 do begin
                 temp := Components[i];
                 if ( temp is TwwRecordViewPanel )  then begin 
                    TwwRecordViewPanel(temp).LabelFont.Name := 'Small Fonts';
                    TwwRecordViewPanel(temp).LabelFont.Size := 7;
                 end;   
                 if ( temp is TwwDataInspector   ) then begin 
                    TwwDataInspector(temp).CaptionFont.Name := 'Small Fonts';
                    TwwDataInspector(temp).CaptionFont.Size := 7;
                 end; 
                 // reduce the grid display width
                 // 'TENDER_TYPE'#9'25'#9'Tender'   #9 = horizontal tab
                 if ( temp is TwwDBGrid   ) then begin 
                    with TwwDBGrid(temp) do begin
                       for m := 0 to Selected.Count -1 do begin
                           s := Selected[m];
                           len := '';
                           for k := 0 to 9 do begin
                              if POS(IntToStr(k),s) > 0 then begin
                                 b := POS(IntToStr(k),s);
                                 l := POS(IntToStr(k),s);
                                 len := copy(s,b,1);
                                 if s[b + 1] in ['0'..'9'] then begin
                                    inc(b);
                                    len := len + copy(s,b,1);
                                    if s[b + 1] in ['0'..'9'] then begin 
                                       inc(b);
                                       len := len + copy(s,b,1);
                                    end;   
                                 end;   
                                 try
                                    j := StrToInt(len);  
                                 except
                                    break;
                                 end;   
                                 j := trunc( j * Global.DiffRatio);
                                 system.delete(s,l,((b - l) + 1));
                                 insert(IntToStr(j),s,l);
                                 Selected[m] := s;
                                 break;
                              end;
                           end;
                       end; 
                    end;
                 end;
              end;
           end; 
        end else begin  
           // changes all components on form
           // 1024 * 768 resolution or greater
           i := Controlbar1.Height;
           Self.ChangeScale(Trunc(DiffRatio * 100),100); 
           Controlbar1.Height := i;
           ToolBar.Width  := ControlBar1.Width;
           Application.ProcessMessages;
           //Self.Font.Size := Trunc(Self.Font.Size * DiffRatio );
         end;                  }
      end;                     
      OneButtonWidth := ToolBar.ButtonWidth + ToolBar.Indent + ToolbarSpace;
      TwoButtonWidth := (ToolBar.ButtonWidth  * 2) + ToolBar.Indent + ToolbarSpace;
      Application.OnException := AppException;
end;                                                  
{ **************************************************************************** }
procedure TSMStdForm.AppException(Sender: TObject; E: Exception);
begin
     if POS('Win32 error 10054',E.Message) > 0 then begin
        sm('NO connection to the Database Server ,check the Server!!');
        dm.IBDatabase.Connected := False;
        Application.Terminate;
     end;
     if (POS ( ForeignKeyMsg,e.Message) > 0) then begin
        sm('Cannot be deleted as it is in use ( Foreign Key )'); 
        exit;   // deletion of Mail Codes - ref
     end;  
     if pos('Failed to create Outlook instance',e.message) > 0 then begin 
        sm('Outlook is not installed - cannot continue');
        exit;
     end;
     if pos('Class not registered',e.message) > 0 then begin 
        sm('Excel is not installed - cannot continue');
        exit;
     end;
     if Pos( LockConflictMsg ,E.Message) > 0 then begin 
        sm('You are changing the record/s in another screen ( try Payments )' + #13#10 +
           'Clear or Save these changes first' );
        exit;
     end;
     Application.ShowException(E);
end;
{ **************************************************************************** }
class function TSMStdForm.FormColor  : TColor;
begin
     Result := Global.fcStdForm;
end;
{ **************************************************************************** }
procedure TSMStdForm.AppIdle(Sender: TObject; var Done: Boolean);
// print the screen
var
   FormColor : TColor;
begin
    if Busy then Exit;
    try                           
       if GetAsyncKeyState(VK_SNAPSHOT) <> 0 then begin
          {Clear Status Panel before printing}
          Screen.Cursor := crHourGlass;
          StatusType := stbStatus.SimplePanel;
          stbStatus.SimplePanel := True;
          stbStatus.SimpleText  := 'Printing this Screen';
          FormColor := Self.Color;
          Self.Color := clWhite;
          Self.Print;
          Self.Color := FormColor;
          stbStatus.SimpleText  := '';
          stbStatus.SimplePanel := StatusType;
       end;
    finally
       Done := True;
       Screen.Cursor := crDefault;
    end;
end;
{ **************************************************************************** }
function sfPreCheckLogOn ( TSMForm : TSMStdFormClass ) : Boolean;
// Not a form method, Pass a Form Class
begin
    result := False;
    Screen.Cursor := crHourGlass;
    try
       if not dm.PreCheckLogOn(TSMForm.ScreenCode,TSMForm.ScreenDesc) then exit;
       Result := True;
    finally
       Screen.Cursor := crDefault;
    end;
end;
{ **************************************************************************** }
procedure TSMStdForm.wmSysCommand(var Message : TWMSysCommand);
begin
     case message.CmdType of
        // double click on the top bar ,61458
        SC_MAXIMIZE : begin
             if (Screen.Height >= 600) and (Self.WindowState in [wsMinimized,wsNormal]) then begin
               Self.ChangeScale(110,100);
               Self.Font.Size := 10;
               DefaultHandler(message);
             end else begin
                DefaultHandler(message); 
             end;   
        end;                                                  
        SC_RESTORE : begin
           //end else begin  ,61730
             if (Screen.Height >= 600) and (Self.WindowState in [wsMaximized,wsNormal])  then begin
               Self.ChangeScale(90,100); 
               Self.Font.Size := 8;
               DefaultHandler(message);
            end else begin
               DefaultHandler(message);   
            end;  
        end;
     else
        //do the default for everything else
        inherited;
     end;
end;    
{ **************************************************************************** }
procedure TSMStdForm.menuDatabaseConnectionClick(Sender: TObject);
var MembershipConnection,MMConnection,MembershipOK,MMOK : shortstring;
const msg = 'Database Connections to ' + #13#10 + '%s  %s' + #13#10 + '%s  %s';
begin
     MembershipConnection :=  'Membership - ' + regServerName + '  ' + regDatabaseName;
     MMConnection := 'MenuMate - ' + regMenuMateServerName + '  ' + regMenuMateDatabaseName;
     if regMenuMateDatabaseName <> '' then begin
        if dmMM.MMDatabase.TestConnected then 
           MMOK := ' = Connected'
        else   
           MMOK := ' = NOT Connected';
     end else begin
        MMConnection := '';
        MMOK := '';
     end;
     if dm.IBDatabase.TestConnected then begin
        if dm.IBDatabase.TestConnected then 
           MembershipOK := ' = Connected'
        else   
           MembershipOK := ' = NOT Connected';
     end;
     MessageDlg(format(msg,[MembershipConnection,MembershipOK,MMConnection,MMOK]),mtInformation,[mbOK],0);
end;
{ **************************************************************************** }
procedure TSMStdForm.menuAboutClick(Sender: TObject);
begin
     with TfrmSplash.Create( nil ) do begin
        try
           //btnClose.Visible := True;
           Label1.Visible := False;
           ShowModal;
        finally
           Free;
        end;   
     end;
end;
{ **************************************************************************** }
procedure TSMStdForm.menuSystemDetailsClick(Sender: TObject);
begin
     with TfrmPasswordDlg.Create( nil ) do begin
        try
           ShowAsSystemDetails := True;
           ShowModal;
        finally
           Free;
        end;   
     end;      
end;
{ **************************************************************************** }
procedure TSMStdForm.menuHelpClick(Sender: TObject);
begin
     {if frmShowHelp = nil then 
        frmShowHelp := TfrmShowHelp.Create(application);
     with frmShowHelp do begin
        HelpDocument := Self.HelpFile + '.WRI';
        StartText    := IntToStr(Self.ActiveControl.HelpContext) + ' )';
        TextType     := ttRichEdit;
        Show;                           
     end;}   
end;
{ **************************************************************************** }
procedure TSMStdForm.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
var i : integer;
   Wnd : HWnd;
begin
     
     if key = VK_F1 then menuHelpClick(Sender);
     if key = VK_F10 then FormPrintCards;
     if key = VK_F12 then frmMemberMenu.Show;
     //if key = VK_F6 then begin
     //   Wnd := GetNextWindow( Self.Handle, GW_HWNDNEXT );
       // SetForeGroundWindow( Wnd );
     //   ShowWindow(Wnd,SW_SHOWNORMAL);
     //end;  
     {   for i := 0 to Screen.FormCount -1 do begin
           if Screen.Forms[i] = Screen.ActiveForm then begin
              if i = (Screen.FormCount -1) then
                 Screen.Forms[0].Show
              else   
                 Screen.Forms[i + 1].Show;
              Application.ProcessMessages;
              break;
           end;
        end;  
     end; }  
end;
{ **************************************************************************** }
procedure TSMStdForm.FormResize(Sender: TObject);
// pnlFill is used only on Forms with a close button and Nothing Else
// for other Forms - they do not call Inherited
begin             
     ToolBar.Width   := ControlBar1.Width -1;
     pnlFill.Width   := ToolBar.Width  - ToolBar.Indent - OneButtonWidth; 
end;
{ **************************************************************************** }
procedure TSMStdForm.ShowStdForm ( theForm : TForm );
// if the form is minimized and if they click on the menu 
begin
     if theForm.WindowState = wsMinimized then begin
        PostMessage( theForm.Handle, wm_SysCommand, sc_Restore, 0 );
     end else begin    
        theForm.Show;
        theForm.BringToFront;
     end;   
end;
{ **************************************************************************** }
procedure TSMStdForm.menuCalculatorClick(Sender: TObject);
//const ctlpanel = 'rundll32.exe';
    //dtcpl = 'shell32.dll,Control_RunDLL calc.cpl';
      //dtcpl = 'shell32.dll,Control_RunDLL timedate.cpl';
begin
     //WinExec(Self.Handle, nil, ctlpanel, dtcpl, nil, sw_Normal);
     WinExec('calc.exe',SW_SHOW);

end;
{ **************************************************************************** }
procedure TSMStdForm.FormPrintCards;
var SetActive : Boolean;
begin                     
     Global.ReadPrinters;
     SetActive := False;
     // if in Settings and on Card tab - leave the table open
     if dm.tblCardDetail.Active then       
        SetActive := True;
     frmMembersCARDS := TfrmMembersCARDS.Create(Self);
     try
        frmMembersCARDS.Showmodal;
     finally
        if SetActive then
           dm.tblCardDetail.Active := true;
        frmMembersCARDS.Free;
     end;
end;
    
end.
