unit rKioskHistory;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmKioskHistory = class(TForm)
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
    QRLabel1: TQRLabel;
    qrDetailBand: TQRBand;
    dbtFullName: TQRDBText;
    dbtMember: TQRDBText;
    QRDBText2: TQRDBText;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    QRLabel2: TQRLabel;
    QRLabel3: TQRLabel;
    QRDBText1: TQRDBText;
    lblDrawn: TQRLabel;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    function Extract(PrtIndex : shortint; DateFrom,DateTo : TDAteTime ) : Boolean;
    procedure qrDetailBandBeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmKioskHistory: TfrmKioskHistory;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
{ ****************************************************************************}
procedure TfrmKioskHistory.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
end;
{ ****************************************************************************}
function TfrmKioskHistory.Extract(PrtIndex : shortint; DateFrom,DateTo : TDAteTime ) : Boolean;
const sql1 = 'Select m.MEMBER,m.FULL_NAME ,m.FIRST_NAME,m.SURNAME,m.EXPIRES,m.SUB_PAID, ' +
             'k.DRAWN,k.PRIZE,k.DATE_TIME from MEMBERS m, KIOSK_HISTORY k where m.member = k.member ' +
             'and cast (k.DATE_TIME as DATE) between ''%s'' and ''%s'' order by k.DATE_TIME';
begin                    
     result := False;
     with dm.qryReport do begin
         close;
         SQL.Text := format(SQL1,[GetShortDate(DAteFrom),GetShortDate(DAteTo)]);
         open;
     end; //case
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     result := True;
end;
{ ****************************************************************************}
procedure TfrmKioskHistory.qrDetailBandBeforePrint(Sender: TQRCustomBand;
  var PrintBand: Boolean);
begin
     if dm.qryReport.FieldByName('DRAWN').AsString = varYes then
        lblDrawn.Caption := 'Yes'
     else
        lblDrawn.Caption := '';
end;
{ ****************************************************************************}
end.
