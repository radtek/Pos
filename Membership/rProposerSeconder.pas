unit rProposerSeconder;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmProposerSeconder = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    QRSysData2: TQRSysData;
    lblHeading: TQRLabel;
    lblReportTitle: TQRLabel;
    QRLabel10: TQRLabel;
    QRBand5: TQRBand;
    QRLabel8: TQRLabel;
    QRLabel9: TQRLabel;
    QRLabel5: TQRLabel;
    QRLabel7: TQRLabel;
    DetailBand: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText9: TQRDBText;
    QRDBText4: TQRDBText;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    QRDBText1: TQRDBText;
    QRDBText5: TQRDBText;
    QRDBText3: TQRDBText;
    QRDBText6: TQRDBText;
    QRLabel1: TQRLabel;
    QRLabel2: TQRLabel;
    function Extract(PrtIndex : shortint;tblName : ShortString;DateFrom,DateTo : TDateTime) : Boolean;
    procedure qrBeforePrint(Sender: TCustomQuickRep;
      var PrintReport: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmProposerSeconder: TfrmProposerSeconder;

implementation
uses 
   Global,
   dmMembers;

{$R *.DFM}
{ ****************************************************************************}
function TfrmProposerSeconder.Extract(PrtIndex : shortint;tblName : ShortString;DateFrom,DateTo : TDateTime) : Boolean;
//const SQL1 = 'INSERT into %s SELECT MEMBER,FIRST_NAME,SURNAME,PROPOSER,'''','''','  +
//             'SECONDER,'''','''',JOINED FROM MEMBERS where JOINED BETWEEN ''%s'' AND ''%s''';       
const SQL1 = 'SELECT MEMBER,FIRST_NAME,SURNAME,PROPOSER,PROPOSER_NAME,SECONDER_NAME, '+
             'SECONDER,JOINED FROM MEMBERS where JOINED BETWEEN ''%s'' AND ''%s''';       
begin
    try
       //with dm.qryGeneral do begin
           {Close;
           SQL.Text := Format('CREATE TABLE %s ( MEMBER INTEGER,FIRST_NAME VARCHAR(20),SURNAME VARCHAR(25),' +
              ' PROPOSER INTEGER,PROP_FIRST_NAME VARCHAR(20),PROP_SURNAME VARCHAR(25),' +
              ' SECONDER INTEGER,SEC_FIRST_NAME VARCHAR(20),SEC_SURNAME VARCHAR(25),JOINED DATE)',[tblName]);
           ExecQuery;   
           dm.IBCommitRetain;
           Close;
           SQL.Text := Format(SQL1,[tblName,GetShortDate(DateFrom),GetShortDate(DateTo)]);
           ExecQuery;       
           dm.IBCommitRetain;
           dm.qryReport.Close;
           dm.qryReport.SQL.Text := format('Select * from %s',[tblName]);
           dm.qryReport.open;
           dm.qryReport.first;
           while not dm.qryReport.EOF do begin
              if not dm.qryReport.FieldByName('PROPOSER').IsNull then begin
                 close;
                 SQL.Text := format('UPDATE %s set PROP_FIRST_NAME = (Select FIRST_NAME from MEMBERS ' +
                                    'where MEMBER = %d) where MEMBER = %d',[tblName,dm.qryReport.FieldByName('PROPOSER').AsInteger,dm.qryReport.FieldByName('MEMBER').AsInteger]);
                 Execquery;                   
                 close;
                 SQL.Text := format('UPDATE %s set PROP_SURNAME = (Select SURNAME from MEMBERS ' +
                                    'where MEMBER = %d) where MEMBER = %d',[tblName,dm.qryReport.FieldByName('PROPOSER').AsInteger,dm.qryReport.FieldByName('MEMBER').AsInteger]);
                 Execquery;                   
                                  
                     
              end;
              if not dm.qryReport.FieldByName('SECONDER').IsNull then begin
                 close;      
                 SQL.Text := format('UPDATE %s set SEC_FIRST_NAME = (Select FIRST_NAME from MEMBERS ' +
                                    'where MEMBER = %d) where MEMBER = %d',[tblName,dm.qryReport.FieldByName('SECONDER').AsInteger,dm.qryReport.FieldByName('MEMBER').AsInteger]);
                 Execquery;  
                 close;
                 SQL.Text := format('UPDATE %s set SEC_SURNAME = (Select SURNAME from MEMBERS ' +
                                    'where MEMBER = %d) where MEMBER = %d',[tblName,dm.qryReport.FieldByName('SECONDER').AsInteger,dm.qryReport.FieldByName('MEMBER').AsInteger]);
                 Execquery;                   
              end;   
              dm.qryReport.Next;
           end;
           close;
           dm.IBCommitRetain; }
           dm.qryReport.Close;
           dm.qryReport.SQL.Text := Format(SQL1,[GetShortDate(DateFrom),GetShortDate(DateTo)]);
           dm.qryReport.open;
       //end;
    finally
       //
    end;
end;
{ ****************************************************************************}
procedure TfrmProposerSeconder.qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
end;

end.
