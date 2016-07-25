unit rMenuMatePointsLog;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  QuickRpt, Qrctrls, ExtCtrls;

type
  TfrmMenuMatePointsLog = class(TForm)
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
    qrDetailBand: TQRBand;
    QRDBText1: TQRDBText;
    dbtFullName: TQRDBText;
    QRDBText8: TQRDBText;
    dbtMember: TQRDBText;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    QRLabel1: TQRLabel;
    QRLabel5: TQRLabel;
    QRDBText3: TQRDBText;
    QRLabel2: TQRLabel;
    lblPointsDifference: TQRLabel;
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
  frmMenuMatePointsLog: TfrmMenuMatePointsLog;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
{ ****************************************************************************}
procedure TfrmMenuMatePointsLog.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
end;
{ ****************************************************************************}
function TfrmMenuMatePointsLog.Extract(PrtIndex : shortint; DateFrom,DateTo : TDAteTime ) : Boolean;
const sql1 = 'Select m.MEMBER,m.FULL_NAME ,m.FIRST_NAME,m.SURNAME,p.POINTS_DIFFERENCE,p.POINTS_AVAILABLE as ' +
             'OLD_POINTS_AVAILABLE,p.THE_DATE,m.POINTS_AVAILABLE from MEMBERS m, MENUMATE_POINTS p where m.member = p.member ' +
             'and p.THE_DATE between ''%s'' and ''%s'' order by m.FULL_NAME ';
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
procedure TfrmMenuMatePointsLog.qrDetailBandBeforePrint( Sender: TQRCustomBand; var PrintBand: Boolean);
var TempFloat : Extended;
begin
     // rounds to the same as MenuMate  Loyalty History report
     TempFloat := Round((dm.qryReport.FieldByName('POINTS_DIFFERENCE').AsFloat * 1000) ) / 1000.0;
     lblPointsDifference.Caption :=  formatfloat('#####.00',TempFloat );   
end;

end.
