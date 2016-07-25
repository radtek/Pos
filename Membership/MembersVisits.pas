unit MembersVisits;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm, Menus, ExtCtrls, ToolWin, ComCtrls, wwSpeedButton,DB,
  wwDBNavigator, wwclearpanel, wwdbdatetimepicker, StdCtrls, Mask, wwdbedit,Global,dmMembers;

type
  TfrmMemberVisits = class(TSMStdForm)
    btnClose: TToolButton;
    Label1: TLabel;
    dbeMember: TwwDBEdit;
    dbdtExpires: TwwDBDateTimePicker;
    Label2: TLabel;
    wwDBEdit1: TwwDBEdit;
    Label3: TLabel;
    DBNavigator: TwwDBNavigator;
    wwNavButton4: TwwNavButton;
    wwNavButton5: TwwNavButton;
    wwNavButton6: TwwNavButton;
    procedure btnCloseClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;

var
  frmMemberVisits: TfrmMemberVisits;

implementation

{$R *.DFM}
{ **************************************************************************** }
class function TfrmMemberVisits.ScreenCode : Integer;
begin Result := Global.scMembersMaintenance; end;
{ **************************************************************************** }
class function TfrmMemberVisits.ScreenName : String;
begin Result := 'Members Visits'; end;
{ **************************************************************************** }
class function TfrmMemberVisits.ScreenDesc : String;
begin Result := '  Members Visits Screen'; end;
{ **************************************************************************** }
procedure TfrmMemberVisits.btnCloseClick(Sender: TObject);
begin
     if dm.dsrMembers.State = dsEdit then begin
        if IfMsgDlg('Save Changes?') then 
           dm.ibdsMembers.Post
        else   
           dm.ibdsMembers.Cancel;
     end;
     Close;
end;
{ ****************************************************************************}
end.
