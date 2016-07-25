unit ShowHelp;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm, StdCtrls, ComCtrls, Buttons, ExtCtrls, 
  Menus, ToolWin, ShellAPI;

type
    TTextType = (ttRichEdit,ttWord97);

type
  TfrmShowHelp = class(TSMStdForm)
//    RichEdit: TRichEdit98;
    menuExit: TMenuItem;
    ToolButton1: TToolButton;
    btnPrint: TToolButton;
    pnlSpace: TPanel;
    menuPrint: TMenuItem;
    menuPrinterSetup: TMenuItem;
    N1: TMenuItem;
    lblInfinityURL: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormResize(Sender: TObject);
    //procedure RichEditURLClick(Sender: TObject; URL: String);
    procedure menuExitClick(Sender: TObject);
    procedure menuPrintClick(Sender: TObject);
    procedure menuPrinterSetupClick(Sender: TObject);
    procedure lblInfinityURLClick(Sender: TObject);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
  private
    { Private declarations }
    FHelpDocument : ShortString;
    FStartText    : ShortString; 
    FTextType     : TTextType;
    FpnlWidth : SmallInt;
  public
    { Public declarations }
    property HelpDocument  : ShortString Write FHelpDocument;
    property StartText     : ShortString Write FStartText;  
    property TextType      : TTextType Write FTextType default ttRichEdit;
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;
  end;

var
  frmShowHelp: TfrmShowHelp;
const
     Word97Import = 'Word 97';  
     Word97Export = 'Word 97 & 6.0/95 RTF';  

implementation

uses 
    dmmembers,
    Global;
{$R *.DFM}
{ **************************************************************************** }
class function TfrmShowHelp.ScreenCode : Integer;
begin Result := Global.scMembersEDIT; end;
{ **************************************************************************** }
class function TfrmShowHelp.ScreenName : String;
begin Result := 'Membership Help'; end;
{ **************************************************************************** }
class function TfrmShowHelp.ScreenDesc : String;
begin Result := '  Membership Help Screen'; end;
{ **************************************************************************** }
procedure TfrmShowHelp.FormCreate(Sender: TObject);
begin
     inherited;
     BorderStyle := bsSizeable;
     FpnlWidth  := pnlSpace.Width;
          //WindowState := wsNormal;
end;
{ **************************************************************************** }
procedure TfrmShowHelp.menuExitClick(Sender: TObject);
begin                   
     { ttRichEdit -  Native format. Save directly, without any converter.
       ttWord97 - handles upto Word 2000 - uses wrd6er32.cnv
                - is loaded onto NT and win98  win95 ???
     }
     //if You save the file from here Word97 then open it with Word 2000
     //( which it converts as it opens) - it will not open it here again
    { if RichEdit.Modified then begin
        if IfMsgDlg('Save the changes') then begin
           case FTextType of 
              ttRichEdit : RichEdit.Lines.SaveToFile(FHelpDocument);       
              //ttWord97   : ExportRTF(FHelpDocument, Word97Export, RichEdit )   
           end;
        end;
     end;                                     
     Close;   }
end;
{ **************************************************************************** }
procedure TfrmShowHelp.FormShow(Sender: TObject);
var   FoundAt,ToEnd : Integer;
begin
     { ttRichEdit -  Native format. Load directly, without any converter.
       ttWord97 - handles upto Word 2000 - uses mswrd832.cnv
                - is loaded onto NT and win98  win95 ???
     }
     {case FTextType of 
        ttRichEdit : RichEdit.Lines.LoadFromFile( FHelpDocument );       
        ttWord97   : ImportAsRTF( FHelpDocument,Word97Import,RichEdit )   
     end;
     // find starting position in the file
     ToEnd := Length(RichEdit.Text);
     FoundAt := RichEdit.FindText(FStartText, 0, ToEnd, [stMatchCase]);
     if FoundAt <> -1 then begin
       RichEdit.SetFocus;
       RichEdit.SelStart := FoundAt;
       RichEdit.SelLength := Length(FStartText);
     end;
     RichEdit.Modified := False;}
end;
{ **************************************************************************** }
procedure TfrmShowHelp.FormResize(Sender: TObject);
begin
     ResizeToolBar( Self ,Self.WindowState,OneButtonWidth,FpnlWidth,pnlSpace);
end;
{ **************************************************************************** }
{procedure TfrmShowHelp.RichEditURLClick(Sender: TObject; URL: String);
var s : String;
begin
       
end;                             }
{ **************************************************************************** }
procedure TfrmShowHelp.menuPrintClick(Sender: TObject);
begin
     //RichEdit.Print(FHelpDocument);
end;
{ **************************************************************************** }
procedure TfrmShowHelp.menuPrinterSetupClick(Sender: TObject);
begin
     dm.PrintDialog.Execute;
end;
{ **************************************************************************** }
procedure TfrmShowHelp.lblInfinityURLClick(Sender: TObject);
//var pc : Pchar;
begin
     //pc := pchar('http://' + lblInfinityURL.Caption);
     //ShellExecute(Self.Handle,nil,pc,nil,nil,SW_SHOWNORMAL);
end;
{ **************************************************************************** }
procedure TfrmShowHelp.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     inherited;
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
