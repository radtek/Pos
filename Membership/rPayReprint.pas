unit rPayReprint;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmPayReprint = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    lblHeading: TQRLabel;
    lblReportTitle: TQRLabel;
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
    QRLabel8: TQRLabel;
    QRLabel16: TQRLabel;
    QRLabel11: TQRLabel;
    QRBand4: TQRBand;
    QRExpr1: TQRExpr;
    QRLabel15: TQRLabel;
    lblTaxInvoice: TQRLabel;
    QRLabel17: TQRLabel;
    QRLabel18: TQRLabel;
    lblPointsUsed: TQRLabel;
    lblTender: TQRLabel;
    QRLabel3: TQRLabel;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    function Extract( Payment : String ) : Boolean;
    procedure QRBand4BeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
    Previewing : Boolean
  end;

var
  frmPayReprint: TfrmPayReprint;

implementation

{$R *.DFM}
uses
   dmMembers, // StockMaster
   Global;
{ **************************************************************************** }
procedure TfrmPayReprint.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
    lblTaxinvoice.Caption := ' Tax Invoice GST Number ' + Global.GSTNo;
    {if Previewing then begin
       QRBand1.Color := clAqua;
       lblHeading.Color  := clAqua;
       lblReportTitle.Color  := clAqua;
       lblTaxInvoice.Color  := clAqua;
       QRSysData1.Color  := clAqua;
       QRLabel10.Color  := clAqua;
       QRSysData2.Color  := clAqua;
    end; }
end;
{ **************************************************************************** }
function TfrmPayReprint.Extract( Payment : String ) : Boolean;
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
     dm.qryOpenCheckEOF(dm.qryReport,format(SQL1,[Payment]));
     result := true;
    // with dm.qryReport1 do begin
    //    dm.qryOpenCheckEOF(dm.qryReport1,format(SQL2,[Payment]));
    // end;
    // result := True;
        // only put in if multi tender
        {if dm.qryReport1.RecordCount > 1 then begin
           first;
           i := 1;
           while not EOF do begin
              case i of // allows for 3 tenders
                 1 : begin
                     lblTender1.Caption  := FieldByName('TENDER_TYPE').AsString;
                     lblAmount1.Caption  := FloatToStrF(FieldByName('AMOUNT').AsFloat,ffcurrency,5,2);
                     lblDetails1.Caption := FieldByName('DETAILS').AsString;
                     end;
                 2 : begin
                     lblTender2.Caption := FieldByName('TENDER_TYPE').AsString;
                     lblAmount2.Caption := FloatToStrF(FieldByName('AMOUNT').AsFloat,ffcurrency,5,2);
                     lblDetails2.Caption := FieldByName('DETAILS').AsString;
                     end;
                 3 : begin
                     lblTender3.Caption := FieldByName('TENDER_TYPE').AsString;
                     lblAmount3.Caption := FloatToStrF(FieldByName('AMOUNT').AsFloat,ffcurrency,5,2);
                     lblDetails3.Caption := FieldByName('DETAILS').AsString;
                     end;
              end;
              inc(i);
              next;
           end;
        end else begin
           QRLabel7.Caption  := '';
           QRLabel13.Caption  := '';
           QRLabel12.Caption  := '';
        end;   }
end;
{ **************************************************************************** }
procedure TfrmPayReprint.QRBand4BeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
begin
     lblPointsUsed.Caption := floattoStrF(dm.qryReport.FieldByName('POINTS_USED').AsFloat,ffCurrency,7,2);
     lblTender.Caption := floattoStrF(dm.qryReport.FieldByName('PAY_AMOUNT').AsFloat,ffCurrency,7,2);
end;
{ **************************************************************************** } 
end.
