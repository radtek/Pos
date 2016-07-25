unit rPointsLog;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  QuickRpt, Qrctrls, ExtCtrls;

type
  TfrmPointsLog = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    QRSysData2: TQRSysData;
    lblHeading: TQRLabel;
    lblReportTitle: TQRLabel;
    QRLabel10: TQRLabel;
    QRBand2: TQRBand;
    QRLabel1: TQRLabel;
    QRLabel2: TQRLabel;
    QRLabel5: TQRLabel;
    QRLabel6: TQRLabel;
    lblPaidHeader: TQRLabel;
    DetailBand: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText1: TQRDBText;
    QRDBText3: TQRDBText;
    QRBand4: TQRBand;
    lblLogo: TQRLabel;
    QRLabel3: TQRLabel;
    QRLabel8: TQRLabel;
    QRLabel16: TQRLabel;
    QRLabel17: TQRLabel;
    QRLabel18: TQRLabel;
    QRLabel19: TQRLabel;
    QRDBText4: TQRDBText;
    QRLabel9: TQRLabel;
    QRDBText7: TQRDBText;
    QRDBText8: TQRDBText;
    QRLabel11: TQRLabel;
    QRLabel12: TQRLabel;
    QRDBText20: TQRDBText;
    QRLabel13: TQRLabel;
    QRDBText21: TQRDBText;
    QRLabel14: TQRLabel;
    QRExpr2: TQRExpr;
    QRExpr3: TQRExpr;
    QRExpr1: TQRExpr;
    QRExpr7: TQRExpr;
    QRExpr6: TQRExpr;
    QRExpr5: TQRExpr;
    QRExpr4: TQRExpr;
    QRShape1: TQRShape;
    QRDBText16: TQRDBText;
    QRDBText15: TQRDBText;
    QRDBText14: TQRDBText;
    QRDBText13: TQRDBText;
    QRDBText11: TQRDBText;
    QRDBText12: TQRDBText;
    QRDBText9: TQRDBText;
    QRDBText5: TQRDBText;
    QRDBText10: TQRDBText;
    QRDBText6: TQRDBText;
    QRLabel7: TQRLabel;
    QRLabel15: TQRLabel;
    QRLabel20: TQRLabel;
    QRLabel21: TQRLabel;
    QRLabel22: TQRLabel;
    QRBand3: TQRBand;
    QRExpr8: TQRExpr;
    QRExpr9: TQRExpr;
    QRExpr10: TQRExpr;
    QRExpr11: TQRExpr;
    QRExpr12: TQRExpr;
    QRExpr13: TQRExpr;
    QRExpr14: TQRExpr;
    QRLabel23: TQRLabel;
    QRLabel4: TQRLabel;
    lblOtherCap: TQRLabel;
    QRLabel25: TQRLabel;
    QRLabel26: TQRLabel;
    QRLabel27: TQRLabel;
    lblSub: TQRLabel;
    lblDel: TQRLabel;
    lblAdj: TQRLabel;
    lblOther: TQRLabel;
    QRLabel24: TQRLabel;
    lblRes: TQRLabel;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    function Extract(  PrtIndex : Integer; DateFrom, DateTo : TDateTime  ) : Boolean;
    procedure DetailBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    procedure QRBand3BeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    
  private
    { Private declarations }
  public
    { Public declarations }
    Res,Adj,Del,Subs,Other : Currency;
  end;

var
  frmPointsLog: TfrmPointsLog;

implementation
uses Global,dmMembers;

{$R *.DFM}

procedure TfrmPointsLog.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
     Res := 0;Adj := 0;Del := 0; Subs := 0; Other := 0;
end;
{ ****************************************************************************}
function TfrmPointsLog.Extract(  PrtIndex : Integer ;DateFrom, DateTo : TDateTime  ) : Boolean;
var s : string;
const SQL1 = 'Select p.FULL_NAME,p.BEF_BONUS_CREDIT,p.BEF_EARNT_CREDIT,p.BEF_PRE_PAID_CREDIT,p.BEF_TOTAL_EARNT,' +
             'p.BEF_TOTAL_REDEEMED,p.BONUS_CREDIT,p.EARNT_CREDIT,p.EDIT_DATE,p.MEMBER,p.POINTS_ID,p.POINTS_VALUE,' +
             'p.PRE_PAID_CREDIT,p.PROCESS,p.REASON,p.TOTAL_EARNT,p.TOTAL_REDEEMED,p.USER_CODE,' +
             'p.BEF_BONUS_CREDIT + p.BEF_EARNT_CREDIT + p.BEF_PRE_PAID_CREDIT as BEF_VALUE_OF_CREDIT,' +
             'p.BONUS_CREDIT + p.EARNT_CREDIT + p.PRE_PAID_CREDIT as VALUE_OF_CREDIT,' +
             //'p.BEF_TOTAL_EARNT - p.BEF_TOTAL_REDEEMED as BEF_POINTS_AVAILABLE,' +
             'p.BEF_POINTS_AVAILABLE,p.POINTS_AVAILABLE from POINTS_LOG p  ' +
             'where p.EDIT_DATE  between ''%s'' and ''%s'' order by p.EDIT_DATE ';
begin
     result := False;
     
     with dm.qryReport do begin
         close;
         // date is a timestamp            midnight                              to 11:59:59 
         SQL.Text := format(SQL1,[GetShortDate(DateFrom) ,GetShortDateTime(DateTo + 0.999999) ]);
         open;
         qr.PrinterSettings.PrinterIndex := PrtIndex;
         Result := True;
     end;   
end;   
{ ****************************************************************************}
procedure TfrmPointsLog.DetailBandBeforePrint(Sender: TQRCustomBand;
  var PrintBand: Boolean);
begin
    case qr.DataSet.FieldByName('PROCESS').AsInteger of
      1,2 : Adj  := Adj + (qr.DataSet.FieldByName('POINTS_AVAILABLE').AsFloat -  qr.DataSet.FieldByName('BEF_POINTS_AVAILABLE').AsFloat);
      3   : Subs := Subs + qr.DataSet.FieldByName('POINTS_VALUE').AsFloat;
      9   : Res  := Res + (qr.DataSet.FieldByName('POINTS_AVAILABLE').AsFloat -  qr.DataSet.FieldByName('BEF_POINTS_AVAILABLE').AsFloat);
      10  : Del  := Del + (qr.DataSet.FieldByName('POINTS_AVAILABLE').AsFloat);
    else
      Other  := Other + (qr.DataSet.FieldByName('POINTS_AVAILABLE').AsFloat);
    end;   
end;
{ ****************************************************************************}
procedure TfrmPointsLog.QRBand3BeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
begin
     lblAdj.Caption :=  CurrToStrF(Adj,ffCurrency,2);
     lblDel.Caption :=  CurrToStrF(Del,ffCurrency,2);
     lblSub.Caption :=  CurrToStrF(Subs,ffCurrency,2);
     lblRes.Caption :=  CurrToStrF(Res,ffCurrency,2);
     // hopefully no others
     if Other <> 0 then begin
        lblOther.Enabled    := True;
        lblOtherCap.Enabled := True;
        lblOther.Caption    := CurrToStrF(Other,ffCurrency,2);
     end;
end;

end.




