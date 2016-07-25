unit MainMenu;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons, ExtCtrls, ComCtrls, Stdform ;

type
  TfrmMainMenu = class(TForm)
    Panel1: TPanel;
    SpeedButton1: TSpeedButton;
    Label1: TLabel;
    Panel2: TPanel;
    SpeedButton2: TSpeedButton;
    Label2: TLabel;
    Panel3: TPanel;
    SpeedButton3: TSpeedButton;
    Label3: TLabel;
    Panel4: TPanel;
    SpeedButton4: TSpeedButton;
    Label4: TLabel;
    Panel5: TPanel;
    SpeedButton5: TSpeedButton;
    Label5: TLabel;
    Panel6: TPanel;
    SpeedButton6: TSpeedButton;
    Label6: TLabel;
    Panel7: TPanel;
    SpeedButton7: TSpeedButton;
    Label7: TLabel;
    Panel8: TPanel;
    SpeedButton8: TSpeedButton;
    Label8: TLabel;
    procedure SpeedButton1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    class function ScreenCode : Integer;
    class function ScreenName : String;
    class function ScreenDesc : String;
  end;

var
  frmMainMenu: TfrmMainMenu;

implementation

{$R *.DFM}
uses
   Global,      // StockMaster
   MemberMenu,  // StockMaster
   dmMembers;  // StockMaster
{ **************************************************************************** }
class function TfrmMainMenu.ScreenCode : Integer;
begin Result := scMainMenu; end;
{ **************************************************************************** }
class function TfrmMainMenu.ScreenName : String;
begin Result := 'StockMaster Main Menu'; end;
{ **************************************************************************** }
class function TfrmMainMenu.ScreenDesc : String;
begin Result := '[StockMaster Main Menu]'; end;
{ **************************************************************************** }
procedure TfrmMainMenu.SpeedButton1Click(Sender: TObject);
begin
     if frmMemberMenu = nil then begin
        if not dm.PreCheckLogOn(TfrmMemberMenu.ScreenCode,TfrmMemberMenu.ScreenDesc) then exit;
        frmMemberMenu := TfrmMemberMenu.Create(nil);
     end else begin
        //UserLog(
     end;
     frmMemberMenu.WindowState := wsNormal;
     frmMemberMenu.Show;
     frmMemberMenu.BringToFront;
end;
{ **************************************************************************** }
procedure TfrmMainMenu.FormCreate(Sender: TObject);
begin
     Self.Caption := Global.BusinessLogo;// + '    ' + Global.StockMaster;
end;
{ **************************************************************************** }
end.
