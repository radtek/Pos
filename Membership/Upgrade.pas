unit Upgrade;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls, Buttons;

type
  TfrmUpGrade = class(TForm)
    Memo1: TMemo;
    Panel1: TPanel;
    BitBtn1: TBitBtn;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmUpGrade: TfrmUpGrade;

implementation

{$R *.DFM}
{************************************************** }
{procedure TForm1.btnUpgradeClick(Sender: TObject);
const MainPath = 'c:\Stockmaster';
var   SiteVersion,s : string;
      i : integer;
begin    
     if MessageDlg('Update this PC?',mtConfirmation,[mbOK,mbCancel],0) =
        mrCancel then exit;
     screen.Cursor := crHourGlass;
     if DirectoryExists(MainPath) then begin
        // copy to an old file
        CopyFiles(MainPath + '\Main.Exe',MainPath + '\Main.old',False);
        // copy from CD
        CopyFiles(FCurrentDrive + 'Main.Exe',MainPath + '\Main.Exe',False);
     end;                                   
     // now upgrade the DB
     IBScript.Script.LoadFromFile(FCurrentDrive + 'UpGrade.SQL');
     if IBScript.Script.Text <> '' then begin
        try
           if not ReadReg then begin
              showmessage('Cannot Read the Registry');
              exit;
           end;   
           // Upgrade version is in the top Line
           SiteVersion := '';
{           this must be in UPGRADE.SQL
            /* Latest Version in Quotes '1.17'  must be the top line -  */   
            /* Use no quotes in the top line and no SQL below for no DB upgrade  */   }
 {          S := IBScript.Script.Strings[0];
           i := pos('''',s);
           system.delete(s,1,i);
           i := pos('''',s);
           SiteVersion := copy(s,1,i -1);
           // connect the DB
           IBDataBase.DataBasename := regServerName + ':' + regDatabaseName ;
           try
              IBDataBase.Connected := True;
           except 
           // Menu Mate user
             on e : Exception do begin
               if POS('Your user name and password are not defined',e.Message) > 0 then begin
                  IBDataBase.Params.Clear;
                  IBDataBase.Params.Add('user_name=SYSDBA');
                  IBDataBase.Params.Add('password=masterkey');
                  IBDataBase.Connected := True;
                end; 
             end; 
           end;   
           // Read the Site Version
           IBSQL.Close;
           IBSQL.SQL.Text := 'Select POS_LICENSE from SITE';          
           IBSQL.ExecQuery;
          
           if IBSQL.FieldByName('POS_LICENSE').AsString = SiteVersion then begin
              if IBTransaction.InTransaction then
                 IBTransaction.Commit;
              IBDataBase.Connected := False;
              MessageDlg('Completed Updating this PC',mtInformation,[mbOK],0);
              screen.Cursor := crDefault;
              
              exit;
           end;              
           try
              // update from SQL - may commit in here
              IBScript.ExecuteScript;
               // Update the Site Version
              if not IBTransaction.InTransaction then
                 IBTransaction.StartTransaction;
              IBSQL.Close;
              IBSQL.SQL.Text := format('Update SITE set POS_LICENSE = %s',[SiteVersion]);          
              IBSQL.ExecQuery;
              // Commit
              if IBTransaction.InTransaction then
                 IBTransaction.Commit;
           except
              on E : Exception do begin 
                 MessageDlg('Error when Updating this PC ' + E.Message ,mtError,[mbOK],0);
                 if IBTransaction.InTransaction then
                    IBTransaction.RollBack;
                 IBDataBase.Connected := False;
                 screen.Cursor := crDefault;
                 exit;                  
              end;   
                                       
           end;   
           IBDataBase.Connected := False;
        except            
           on E : Exception do begin
              MessageDlg('Error when Updating this PC ' + E.Message ,mtError,[mbOK],0);
              screen.Cursor := crDefault;
              exit;
           end;   
        end;
     end;
     screen.Cursor := crDefault;
     MessageDlg('Completed Updating this PC',mtInformation,[mbOK],0);}

end.
