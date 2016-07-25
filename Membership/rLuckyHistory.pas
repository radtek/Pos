unit rLuckyHistory;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmLuckyHistory = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    QRSysData2: TQRSysData;
    lblHeading: TQRLabel;
    lblReportTitle: TQRLabel;
    QRLabel10: TQRLabel;
    QRBand2: TQRBand;
    QRLabel1: TQRLabel;
    QRLabel3: TQRLabel;
    QRLabel5: TQRLabel;
    QRLabel6: TQRLabel;
    QRLabel7: TQRLabel;
    QRBand3: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText3: TQRDBText;
    QRDBText5: TQRDBText;
    QRDBText6: TQRDBText;
    QRDBText7: TQRDBText;
    QRDBText8: TQRDBText;
    QRBand4: TQRBand;
    lblLogo: TQRLabel;
    procedure qrBeforePrint(Sender: TCustomQuickRep;
      var PrintReport: Boolean);
    function Extract (qrPrint : Boolean ; PrtIndex : SmallInt) : Boolean;

  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmLuckyHistory: TfrmLuckyHistory;

implementation

{$R *.DFM}
Uses          
    dmMembers, //StockMaster
    Global; //StockMaster
var
    Printing : Boolean;

{ **************************************************************************** }
procedure TfrmLuckyHistory.qrBeforePrint(Sender: TCustomQuickRep;
  var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
end;
{ **************************************************************************** }
function TfrmLuckyHistory.Extract(qrPrint : Boolean ; PrtIndex : SmallInt) : Boolean;
begin
     result := False;
     with dm.qryReport do begin
        close;
        SQL.Text := 'SELECT MEMBER,FULL_NAME,SURNAME,KNOWN_AS,ADDRESS,ADDRESS1,HOME_PHONE,LUCKY_DATE' +
                    ' from MEMBERS where LUCKY_DATE is not null ORDER by LUCKY_DATE';
        open;
        qr.PrinterSettings.PrinterIndex := PrtIndex;
        Printing := qrPrint;
        result := True;
     end;
end;
{ **************************************************************************** }
end.
