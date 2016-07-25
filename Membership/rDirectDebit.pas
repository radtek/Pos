unit rDirectDebit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmDirectDebit = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    QRSysData2: TQRSysData;
    lblHeading: TQRLabel;
    lblReportTitle: TQRLabel;
    QRLabel10: TQRLabel;
    QRBand2: TQRBand;
    QRLabel1: TQRLabel;
    QRLabel5: TQRLabel;
    QRLabel6: TQRLabel;
    QRLabel2: TQRLabel;
    QRLabel4: TQRLabel;
    lblSection: TQRLabel;
    QRLabel8: TQRLabel;
    DetailBand: TQRBand;
    dbtMember: TQRDBText;
    dbtAddress: TQRDBText;
    dbtPhone: TQRDBText;
    QRDBText1: TQRDBText;
    QRBand4: TQRBand;
    lblLogo: TQRLabel;
    SummaryBand: TQRBand;
    QRLabel21: TQRLabel;
    QRExpr2: TQRExpr;
    QRExpr1: TQRExpr;
    QRExpr3: TQRExpr;
    QRExpr4: TQRExpr;
    QRDBText2: TQRDBText;
    QRDBText3: TQRDBText;
    QRDBText4: TQRDBText;
    QRLabel3: TQRLabel;
    QRDBText5: TQRDBText;
    QRLabel9: TQRLabel;
    lblFinancial: TQRLabel;
    QRLabel11: TQRLabel;
    QRDBText6: TQRDBText;
    function Extract( MinReceipt,MaxReceipt: Integer) : Boolean;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    procedure DetailBandBeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
  private
    { Private declarations }
     fMinReceipt,fMaxReceipt : LongInt;
  public
    { Public declarations }
  end;

var
  frmDirectDebit: TfrmDirectDebit;

implementation

{$R *.DFM}
uses
   Global,dmMembers;

{ **************************************************************************** }
function TfrmDirectDebit.Extract( MinReceipt,MaxReceipt: Integer) : Boolean;
// gets the Receipts between a Receipt number range - grabbed after the post
const SQL1 = 'select pd.MEMBER,pd.PAYMENT,pd.PAY_DATE,pd.full_name,' +
             'pd.AMOUNT,pd.AMOUNT_PAID as PAY,pd.EXPIRES,pd.DESCRIPTION,ms.AMOUNT_PAID,m.SUB_PAID ' +
             'from PAYMENTS_DET pd,M_SUBS ms, MEMBERS m ' +
             'where ms.MEMBER = pd.MEMBER and pd.CODE = ms.SUBSCRIPTION and m.MEMBER = pd.MEMBER ' +
             'and pd.PAYMENT between %d and %d order by pd.MEMBER';
begin
     Result := False;
     fMinReceipt := MinReceipt;
     fMaxReceipt := MaxReceipt;
     with dm.qryReport do begin
        Close;
        SQL.Text := format(SQL1,[MinReceipt,MaxReceipt]);
        Open;
     end;
     Result := True;
end;
{ **************************************************************************** }
procedure TfrmDirectDebit.qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
     lblReportTitle.Caption := 'Direct Debit Report ' + GetShortMonthDate(Date) +
                               ' from Receipt # ' + intToStr(fMinReceipt) + ' to ' + IntToStr(fMaxReceipt);
end;
{ **************************************************************************** }

procedure TfrmDirectDebit.DetailBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
begin
     lblFinancial.Caption := YesNoTypesArray[dm.qryReport.FieldByName('SUB_PAID').AsInteger];
end;

end.
