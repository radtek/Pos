unit rPHMPointsLog;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmPHMPointsLog = class(TForm)
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
    QRLabel13: TQRLabel;
    QRLabel16: TQRLabel;
    QRLabel1: TQRLabel;
    QRLabel2: TQRLabel;
    qrDetailBand: TQRBand;
    QRDBText1: TQRDBText;
    dbtFullName: TQRDBText;
    QRDBText8: TQRDBText;
    dbtMember: TQRDBText;
    QRDBText2: TQRDBText;
    QRDBText3: TQRDBText;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    function Extract(PrtIndex : shortint;DateFrom,DateTo : TDateTime) : Boolean;

  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmPHMPointsLog: TfrmPHMPointsLog;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster

{ ****************************************************************************}
procedure TfrmPHMPointsLog.qrBeforePrint(Sender: TCustomQuickRep;
  var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
end;
{ ****************************************************************************}
function TfrmPHMPointsLog.Extract(PrtIndex : shortint;DateFrom,DateTo : TDateTime) : Boolean;
const SQL1 = 'select p.DATE_TIME, p.MEMBER, m.KNOWN_AS,p.POINTS, p.COMMENT, ' +
             'p.BOOKING,m.FULL_NAME ' +
             'from PHM_LOG p ,MEMBERS m where m.MEMBER = p.MEMBER ' +
             'and p.DATE_TIME >= ''%s'' and  p.DATE_TIME <= ''%s'' order by p.DATE_TIME';
begin
     result := False;
      with dm.qryReport do begin
         close;
         SQL.Text := Format(SQL1,[GetShortDate(DateFrom),GetShortDate(DateTo)]);
         open;
     end;    
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     result := True;
end;
{ ****************************************************************************}

end.
