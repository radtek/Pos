program RPDemo;

uses
  Forms,
  MAIN in 'MAIN.PAS' {MainForm},
  demo01 in 'demo01.PAS' {Demo01Form},
  DEMO02 in 'DEMO02.PAS' {Demo02Form},
  demo03 in 'demo03.PAS' {Demo03Form},
  demo04 in 'demo04.PAS' {Demo04Form},
  demo05 in 'demo05.PAS' {Demo05Form},
  demo06 in 'demo06.PAS' {Demo06Form},
  demo07 in 'demo07.PAS' {Demo07Form},
  demo08 in 'demo08.PAS' {Demo08Form},
  RTFDemo in 'RTFDemo.PAS' {RTFDemoForm};

{$R *.RES}

begin
  Application.CreateForm(TMainForm, MainForm);
  Application.Run;
end.
