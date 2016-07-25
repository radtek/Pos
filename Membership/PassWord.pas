unit Password;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TfrmPassword = class(TForm)
    btnOK: TButton;
    btnCancel: TButton;
    edtPAssword: TEdit;
    Label1: TLabel;
    procedure edtPAsswordKeyPress(Sender: TObject; var Key: Char);
    procedure btnOKClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmPassword: TfrmPassword;

implementation

{$R *.DFM}

uses
   global;
{ **************************************************************************** }
procedure TfrmPassword.edtPAsswordKeyPress(Sender: TObject; var Key: Char);
begin
     if key = #13 then btnOKClick(Sender);
end;
{ **************************************************************************** }
procedure TfrmPassword.btnOKClick(Sender: TObject);
const msg1 = 'Wrong Password entered';
var Year, Month, Day : Word;
    d,a : Integer;
begin
     if (edtPassword.Text = '') or  
        not Alldigits(edtPassword.Text) then begin
        edtPassword.SetFocus;
        edtPassword.SelectAll;
        sm(msg1);
        ModalResult := mrNone;
        exit;
     end;
     Year  := StrToInt(copy(edtPassword.Text,1,4)); 
     Month := StrToInt(copy(edtPassword.Text,5,2)); 
     Day   := StrToInt(copy(edtPassword.Text,7,2)); 
     d := Trunc(EncodeDate(Year,month,Day));
     a := Trunc(Date);
     if d <> Date then begin
        edtPassword.SetFocus;
        edtPassword.SelectAll;
        sm(msg1);
        ModalResult := mrNone;
        exit;
     end;
     ModalResult := mrOK;
     
end;
{ **************************************************************************** }

end.
