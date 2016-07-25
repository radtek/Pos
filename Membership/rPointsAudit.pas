unit rPointsAudit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmPointsAudit = class(TForm)
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
    QRLabel12: TQRLabel;
    QRLabel13: TQRLabel;
    QRLabel16: TQRLabel;
    qrDetailBand: TQRBand;
    QRDBText1: TQRDBText;
    dbtFullName: TQRDBText;
    QRDBText4: TQRDBText;
    QRDBText8: TQRDBText;
    dbtMember: TQRDBText;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
   function Extract(PrtIndex : shortint ) : Boolean;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmPointsAudit: TfrmPointsAudit;

implementation

{$R *.DFM}
uses
   dmMembers,
   Global;

{ ****************************************************************************}   
procedure TfrmPointsAudit.qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
end;
{ ****************************************************************************}
function TfrmPointsAudit.Extract(PrtIndex : shortint ) : Boolean;
const {SQL1 = 'select m.member,m.FULL_NAME,m.TOTAL_EARNT - m.TOTAL_REDEEMED as mPOINTS_AVAILABLE,' +
             'i.POINTS_AVAILABLE as iPOINTS_AVAILABLE,(m.TOTAL_EARNT - m.TOTAL_REDEEMED) - i.POINTS_AVAILABLE as DIFF ' +
             'from members m ,import_points i where (m.TOTAL_EARNT - m.TOTAL_REDEEMED) <> i.POINTS_AVAILABLE';}
       SQL1 = 'select m.member,m.FULL_NAME,m.KNOWN_AS,m.POINTS_AVAILABLE,' +
             'i.POINTS_AVAILABLE as iPOINTS_AVAILABLE,m.POINTS_AVAILABLE - i.POINTS_AVAILABLE as DIFF ' +
             'from members m ,import_points i where CARD_LAST_USED = CURRENT_DATE and i.MEMBER = m.MEMBER';
begin
     result := False;
     with dm.qryReport do begin
       close;
       SQL.Text := SQL1;
       open;
       if RecordCount = 0 then exit;
     end;
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     result := True;
end;
{ ****************************************************************************}
                   

end.
