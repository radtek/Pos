unit rPointsStockmaster;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  QuickRpt, Qrctrls, ExtCtrls;

type
  TfrmPointsStockmaster = class(TForm)
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
    qrSummaryBand: TQRBand;
    QRExpr2: TQRExpr;
    QRExpr3: TQRExpr;
    QRExpr7: TQRExpr;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    QRLabel3: TQRLabel;
    QRLabel5: TQRLabel;
    QRDBText2: TQRDBText;
    QRDBText3: TQRDBText;
    QRDBText5: TQRDBText;
    QRLabel6: TQRLabel;
    QRLabel7: TQRLabel;
    QRDBText6: TQRDBText;
    QRExpr6: TQRExpr;
    QRExpr8: TQRExpr;
    QRExpr9: TQRExpr;
    QRExpr10: TQRExpr;
    procedure qrBeforePrint(Sender: TCustomQuickRep;
      var PrintReport: Boolean);
    function Extract(PrtIndex,Option : Smallint) : Boolean;
      
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmPointsStockmaster: TfrmPointsStockmaster;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster

{ **************************************************************************** }
function TfrmPointsStockmaster.Extract(PrtIndex,Option : Smallint ) : Boolean;
const  SQL1 = 'SELECT MEMBER,FULL_NAME,KNOWN_AS,EXPIRES,POINTS_AVAILABLE,POINTS_AVAILABLE * 100 as POINTS,' +
              'TOTAL_REDEEMED * 100 as TOTAL_REDEEMED,TOTAL_EARNT * 100 as TOTAL_EARNT,' +
              'BONUS_CREDIT * 100 as BONUS_CREDIT,EARNT_CREDIT * 100 as EARNT_CREDIT,' +
              'PRE_PAID_CREDIT * 100 as PRE_PAID_CREDIT FROM MEMBERS order by MEMBER';
const  SQL2 = 'SELECT mg.GROUPS as MEMBER,g.DESCRIPTION as FULL_NAME,sum(POINTS_AVAILABLE) as POINTS_AVAILABLE,sum(POINTS_AVAILABLE) * 100 as POINTS,' +
              'sum(TOTAL_REDEEMED) * 100 as TOTAL_REDEEMED,sum(TOTAL_EARNT) * 100 as TOTAL_EARNT,' +
              'sum(BONUS_CREDIT) * 100 as BONUS_CREDIT,sum(EARNT_CREDIT) * 100 as EARNT_CREDIT,' +
              'sum(PRE_PAID_CREDIT) * 100 as PRE_PAID_CREDIT FROM MEMBERS m, M_GROUPS mg , GROUPS g ' +
              'where m.MEMBER = mg.MEMBER and mg.GROUPS = g.GROUPS ' +
              'Group By mg.GROUPS,g.DESCRIPTION order by mg.GROUPS';
                               
begin
     result := False;
     with dm.qryReport do begin
        close;
        case Option of 
           0 : SQL.Text := SQL2;  // by Groups
           1 : SQL.Text := SQL1;  // by Member
        end;   
        open;
     end;
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     result := True;
end;
{ ****************************************************************************}
procedure TfrmPointsStockmaster.qrBeforePrint(Sender: TCustomQuickRep;
  var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
end;

end.

