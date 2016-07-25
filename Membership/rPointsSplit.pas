unit rPointsSplit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  QuickRpt, Qrctrls, ExtCtrls;

type
  TfrmPointsSplit = class(TForm)
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
    QRLabel1: TQRLabel;
    qrDetailBand: TQRBand;
    QRDBText1: TQRDBText;
    dbtFullName: TQRDBText;
    QRDBText4: TQRDBText;
    QRDBText8: TQRDBText;
    dbtMember: TQRDBText;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    QRDBText2: TQRDBText;
    QRLabel2: TQRLabel;
    QRDBText3: TQRDBText;
    procedure qrBeforePrint(Sender: TCustomQuickRep;
      var PrintReport: Boolean);
      
  function Extract(PrtIndex : shortint;DateFrom,DateTo : TDateTime) : Boolean;
  private                          
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmPointsSplit: TfrmPointsSplit;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster

procedure TfrmPointsSplit.qrBeforePrint(Sender: TCustomQuickRep;
  var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
end;
{ ****************************************************************************}
function TfrmPointsSplit.Extract(PrtIndex : shortint;DateFrom,DateTo : TDateTime) : Boolean;
const SQL1 = 'select p.DATE_TIME, p.DONATION_MEMBER, p.DONATION_POINTS, p.LAST_DONATED_POINTS_AVAILABLE, ' +
             'p.LAST_POINTS_AVAILABLE, p.MEMBER, p.POINTS, p.POINTS_SPLIT_ID, p.UPDATED,m.FULL_NAME ' +
             'from POINTS_SPLIT p ,MEMBERS m where m.MEMBER = p.MEMBER ' +
             'and p.DATE_TIME >= ''%s'' and  p.DATE_TIME <= ''%s'' order by p.MEMBER,p.DATE_TIME';
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

end.
