unit Main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, RpDefine, RpRave, StdCtrls;

type
  TForm1 = class(TForm)
    Button1: TButton;
    RvProject1: TRvProject;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.Button1Click(Sender: TObject);
begin
  RvProject1.ExecuteReport('TableOfContentsReport');
end;

end.
