unit rPayTotals;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmPayTotals = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    lblHeading: TQRLabel;
    lblReportTitle: TQRLabel;
    QRBand3: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText9: TQRDBText;
    QRBand6: TQRBand;
    QRExpr1: TQRExpr;
    QRGroup1: TQRGroup;
    QRBand4: TQRBand;
    lblSubTotal: TQRLabel;
    QRLabel12: TQRLabel;
    lblGroupTotal: TQRLabel;
    QRLabel2: TQRLabel;
    QRDBText1: TQRDBText;
    lblGroup: TQRLabel;
    procedure qrBeforePrint(Sender: TCustomQuickRep;      var PrintReport: Boolean);
    procedure QRGroup1BeforePrint(Sender: TQRCustomBand;      var PrintBand: Boolean);
    function Extract(PayDate : TDateTime) : Boolean;
    procedure QRBand3BeforePrint(Sender: TQRCustomBand;      var PrintBand: Boolean);
    procedure QRBand4BeforePrint(Sender: TQRCustomBand;      var PrintBand: Boolean);
    procedure QRBand4AfterPrint(Sender: TQRCustomBand;       BandPrinted: Boolean);
      
  private
    { Private declarations }
    FSubTotal : Extended;
  public
    { Public declarations }
    Previewing : Boolean
  end;                     

var
  frmPayTotals : TfrmPayTotals;

implementation

{$R *.DFM}
uses
    Global,
    dmMembers;
var
  PayTypes : TPayTypes;

{ **************************************************************************** }    
procedure TfrmPayTotals.qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;

     FSubTotal := 0.00;	
end;
{ **************************************************************************** }
function TfrmPayTotals.Extract(PayDate : TDateTime) : Boolean;
const SQL1 = 'Select DISTINCT PAYMENT from PAYMENTS_DET where PAY_DATE = ''%s''';
      SQL2 = 'Select PD.PAY_TYPE,PT.TENDER_TYPE, Sum(PD.AMOUNT_PAID) as AMOUNT from PAYMENTS_TEND PT, PAYMENTS_DET PD ' +
             'where PT.PAYMENT = PD.PAYMENT AND PD.PAYMENT in (%s) group by PD.PAY_TYPE,PT.TENDER_TYPE';
      msg1 = 'No Payments for this date ';
var PaymentStr : String;
    PayDateStr,fPayDateStr : String[12];
begin
     result := False;
     PayDateStr := GetShortDate(PayDate);
     fPayDateStr := formatDateTime('dd mmm yyyy',PayDate);
     lblReportTitle.Caption := 'Payment Totals for ' + fPayDateStr;
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[PayDateStr])) then begin
        while not dm.qryGeneral.EOF do begin
           PaymentStr := PaymentStr + dm.qryGeneral.FieldByName('PAYMENT').AsString + ',';
           dm.qryGeneral.Next;
        end;
        system.delete(PaymentStr,length(PaymentStr),1); // remove last comma
        if dm.qryOpenCheckEOF(dm.qryReport ,format(SQL2,[PaymentStr])) then 
           result := True
        else
           MDlgI( msg1 + fPayDateStr);
     end else begin   
        MDlgI( msg1 + fPayDateStr);
     end;
end;
{ **************************************************************************** }
procedure TfrmPayTotals.QRGroup1BeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
begin
     case dm.qryReport.FieldByName('PAY_TYPE').AsInteger of
        0 : PayTypes := ptSubs;
        1 : PayTypes := ptSections;
        2 : PayTypes := ptJoinFee;
     end;  
     lblGroup.Caption := PayTypesArray[ord(PayTypes)];
end;
{ **************************************************************************** }
procedure TfrmPayTotals.QRBand3BeforePrint(Sender: TQRCustomBand;
  var PrintBand: Boolean);
begin
     FSubTotal := FSubTotal + dm.qryReport.FieldByName('AMOUNT').AsFloat;
end;
{ **************************************************************************** }
procedure TfrmPayTotals.QRBand4BeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
begin
     //lblGroupTotal.Caption := 'Total ' + PayTypesArray[ord(PayTypes)];
     lblSubTotal.Caption := FloatToStrF( FSubTotal,ffcurrency,7,2);
end;
{ **************************************************************************** }
procedure TfrmPayTotals.QRBand4AfterPrint(Sender: TQRCustomBand; BandPrinted: Boolean);
begin
     FSubTotal := 0.00;
end;
{ **************************************************************************** }
end.
