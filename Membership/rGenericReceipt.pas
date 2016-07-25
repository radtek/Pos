unit rGenericReceipt;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmGenericReceipt = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    lblHeading: TQRLabel;
    lblReportTitle: TQRLabel;
    lblTaxInvoice: TQRLabel;
    QRBand3: TQRBand;
    QRDBText8: TQRDBText;
    QRDBText9: TQRDBText;
    QRDBText1: TQRDBText;
    QRBand2: TQRBand;
    QRDBText3: TQRDBText;
    QRDBText5: TQRDBText;
    QRDBText6: TQRDBText;
    QRDBText10: TQRDBText;
    QRLabel1: TQRLabel;
    QRLabel2: TQRLabel;
    QRLabel4: TQRLabel;
    QRLabel8: TQRLabel;
    QRLabel16: TQRLabel;
    QRLabel11: TQRLabel;
    QRBand4: TQRBand;
    QRExpr1: TQRExpr;
    QRLabel15: TQRLabel;
    QRLabel17: TQRLabel;
    QRLabel18: TQRLabel;
    lblPointsUsed: TQRLabel;
    lblTender: TQRLabel;
    QRLabel3: TQRLabel;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    procedure QRBand4BeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    function Extract( Payment : String ) : Boolean;
    procedure qrAfterPrint(Sender: TObject);
  private        
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmGenericReceipt: TfrmGenericReceipt;

implementation

{$R *.DFM}
uses
   dmMembers, // StockMaster
   Global;


procedure TfrmGenericReceipt.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
    lblHeading.Caption    := Uppercase(Global.PrintLogo);
    lblTaxinvoice.Caption := ' Tax Invoice GST Number ' + Global.GSTNo;

end;
{ ****************************************************************************}
procedure TfrmGenericReceipt.QRBand4BeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
begin
     lblPointsUsed.Caption := floattoStrF(dm.qryReport.FieldByName('POINTS_USED').AsFloat,ffCurrency,7,2);
     lblTender.Caption     := floattoStrF(dm.qryReport.FieldByName('PAY_AMOUNT').AsFloat,ffCurrency,7,2);
end;
{ ****************************************************************************}
function TfrmGenericReceipt.Extract( Payment : String ) : Boolean;
var i : Shortint;
const SQL1 = 'select p.PAYMENT, p.PAY_AMOUNT, p.POINTS_USED, p.PAID_BY, p.DISCOUNT,' +
             'p.EDIT_DATE, p.ROUNDING, p.PAY_CREDIT, p.FULL_NAME AS FNAME,' +
             'pd.DESCRIPTION, pd.EXPIRES, pd.SPONSORSHIP, pd.AMOUNT, pd.AMOUNT_PAID,' +
             'pd.PAY_FULL, pd.PAY_TYPE, pd.MEMBER, pd.PAY_DATE, pd.FULL_NAME ' +
             'from PAYMENTS p, PAYMENTS_DET pd where p.PAYMENT = pd.PAYMENT ' +
             'AND p.PAYMENT =  %s  ORDER BY p.PAYMENT DESC,pd.MEMBER';
      SQL2 = 'Select * from PAYMENTS_TEND where PAYMENT = %s';
begin
     result := False;
     if Payment = '' then begin 
        sm('No Receipt is available to Print or Preview');
        exit;
     end;
     dm.qryOpenCheckEOF(dm.qryReport,format(SQL1,[Payment]));
     result := true;
end;
{ ****************************************************************************}     
procedure TfrmGenericReceipt.qrAfterPrint(Sender: TObject);
begin
    Global.RawDataToPrinter('Receipt Printer',ReceiptPrinterName,format( '%s%s%s',[GS,epsoncut,partialcut]),False);
end;

end.
