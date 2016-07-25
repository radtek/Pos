unit rRollover;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  QuickRpt, Qrctrls, ExtCtrls;

type
  TfrmRollover = class(TForm)
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
    QRLabel2: TQRLabel;
    QRLabel4: TQRLabel;
    lblSection: TQRLabel;
    DetailBand: TQRBand;
    dbtMember: TQRDBText;
    dbtFullName: TQRDBText;
    dbtAddress: TQRDBText;
    dbtPhone: TQRDBText;
    QRDBText1: TQRDBText;
    QRGroup1: TQRGroup;
    lblRollover: TQRLabel;
    QRBand4: TQRBand;
    lblLogo: TQRLabel;
    QRGroup2: TQRGroup;
    lblAccepted: TQRLabel;
    GroupFooter1: TQRBand;
    QRLabel21: TQRLabel;
    QRExpr2: TQRExpr;
    QRLabel8: TQRLabel;
    lblFinancial: TQRLabel;
    lblAnalyse: TQRLabel;
    lblNewsletter: TQRLabel;
    procedure QRGroup1AfterPrint(Sender: TQRCustomBand;
      BandPrinted: Boolean);
    procedure QRGroup2AfterPrint(Sender: TQRCustomBand;
      BandPrinted: Boolean);
    procedure qrBeforePrint(Sender: TCustomQuickRep;
      var PrintReport: Boolean);
    procedure DetailBandBeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmRollover: TfrmRollover;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
var
   Printing : Boolean;

{ **************************************************************************** }
procedure TfrmRollover.QRGroup1AfterPrint(Sender: TQRCustomBand;   BandPrinted: Boolean);
begin
     lblRollover.Caption := 'Keep Expires Current Members';
     if dm.qryReport.FieldByName('ACCEPT').AsString = varYes then
        lblAccepted.Caption := 'Accepted'
     else
        lblAccepted.Caption := 'NOT Accepted';
end;
{ **************************************************************************** }
procedure TfrmRollover.QRGroup2AfterPrint(Sender: TQRCustomBand; BandPrinted: Boolean);
begin
     if lblAccepted.Caption = 'Accepted' then
        lblAccepted.Caption := 'NOT Accepted'
     else
        lblAccepted.Caption := 'Accepted';
end;
{ **************************************************************************** }
procedure TfrmRollover.qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
const SQL1 = 'Select * from M_ROLLOVER Order BY KEEP_EXPIRES_CURRENT,ACCEPT,MEMBER';
begin
     with dm.qryReport do begin
         Close;
         SQL.Text := SQL1;
         Open;
     end;
     lblHeading.Caption := Global.PrintLogo;
     qr.PrinterSettings.PrinterIndex := Global.ReportPrinterIndex;
     lblReportTitle.Caption := 'Members Rollover Report ' + GetShortMonthDate(Date);
end;
{ **************************************************************************** }
procedure TfrmRollover.DetailBandBeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
begin
     with dm.qryReport do begin
         lblFinancial.Caption := YesNoTypesArray[FieldByName('SUB_PAID').AsInteger];
         if FieldByName('NEWSLETTER').AsString = varYes then
            lblNewsletter.Caption := varYesWord
         else   
            lblNewsletter.Caption := varNoWord;
         if FieldByName('ANALYSE').AsString = varYes then
            lblAnalyse.Caption := varYesWord
         else   
            lblAnalyse.Caption := varNoWord;
     end;
end;

end.
 