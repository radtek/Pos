unit KioskPas;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,StdCtrls,DB, 
  ExtCtrls,  DBCtrls, ovclabel, lmdcctrl, lmdctrl, lmdextcS,
   jpeg, ComCtrls, Buttons  ;
  

type
  TfrmKiosk = class(TForm)
    pnlTop: TLMDPanelFill;
    pnlBottom: TLMDPanelFill;
    Image2: TImage;
    Image1: TImage;
    edtCardSwipe: TEdit;
    lblTop: TLabel;
    DBRichEdit: TDBRichEdit;
    btnCancel: TButton;
    btnClose: TButton;
    lblBottom: TLabel;
    lblCompanyName: TOvcLabel;
    lblNotify: TOvcLabel;
    LMDFill1: TLMDFill;
    LMDFill2: TLMDFill;
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure btnCloseClick(Sender: TObject);
    procedure btnCancelClick(Sender: TObject);
    
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmKiosk: TfrmKiosk;

implementation

uses dmMembers,GLobal;


{$R *.DFM}

{ ****************************************************************************}
procedure TfrmKiosk.FormCreate(Sender: TObject);
var Diff : Extended;
begin
     Diff := 0.00;
     if Screen.Height <> 768 then  // Designed in 1024 * 768
        Diff   := Screen.Height / 768;
     if (Diff > 0) and (Diff < 1.00) then begin  
        Self.ChangeScale(Trunc(Diff * 100),100); 
        btnClose.Left := Self.Width - 120;
        btnClose.Width := 87;
        btnCancel.Left := Self.Width - 120;
        btnCancel.Width := 87;
        Image2.Left := Self.Width - 180;
        Repaint;
        Application.ProcessMessages;
     end;      
     lblCompanyName.Caption := Global.CompName;
     if length(Global.CompName) > 20 then
        // one line
       lblTop.Height := 5
     else   
        // 2 Lines
        lblTop.Height := 25;
     lblBottom.Height := (pnlBottom.Height div 2) - 20;    
end;
{ ****************************************************************************}
procedure TfrmKiosk.FormShow(Sender: TObject);
begin                                             
     if FileExists('logo.jpg') then begin
        Image1.Picture.LoadFromFile('logo.jpg');
        Image2.Picture.LoadFromFile('logo.jpg');
     end;
     DBRichEdit.Setfocus;
     // leave here ( Close Open ) as it creates errors
     dm.tblMisc.Close;
     dm.tblMisc.Open;
end;
{ ****************************************************************************}
procedure TfrmKiosk.btnCloseClick(Sender: TObject);
begin
     if dm.TblMisc.State = dsEdit then begin
        dm.TblMisc.Post;
        dm.IBTransactionCommit; // Commit changes on deactivate
     end;   
     Close;
end;
{ ****************************************************************************}
procedure TfrmKiosk.btnCancelClick(Sender: TObject);
begin
     if dm.TblMisc.State = dsEdit then begin
        dm.TblMisc.Post;
        dm.IBTransactionCommit; // Commit changes on deactivate
     end;
     Close;
end;
{ ****************************************************************************}
end.
