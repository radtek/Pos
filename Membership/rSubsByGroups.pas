unit rSubsByGroups;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmSubsByGroups = class(TForm)
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
    QRBand3: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText3: TQRDBText;
    QRBand4: TQRBand;
    lblLogo: TQRLabel;
    QRBand5: TQRBand;
    QRExpr1: TQRExpr;
    QRLabel2: TQRLabel;
    QRGroup1: TQRGroup;
    FooterBAnd: TQRBand;
    QRLabel5: TQRLabel;
    QRDBText1: TQRDBText;
    QRLabel6: TQRLabel;
    QRExpr2: TQRExpr;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    function Extract(PrtIndex : SmallInt ; mGroups : string) : Boolean;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmSubsByGroups: TfrmSubsByGroups;

implementation

{$R *.DFM}
Uses          
    dmMembers, //StockMaster
    Global; //StockMaster
    
{ **************************************************************************** }
procedure TfrmSubsByGroups.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
end;
{ **************************************************************************** }
function TfrmSubsByGroups.Extract(PrtIndex : SmallInt ; mGroups : string) : Boolean;
const SQL1 = 'select s.description,g.description  as GROUPDESC,count(ms.subscription) as CNT ' +
             'from SUBSCRIPTIONS s , M_SUBS ms, GROUPS g where  ' +
             's.subscription = ms.subscription and s.POINTS_SPENT = g.GROUPS %s ' +
             'group by g.description ,s.description order by g.description';
             // POINTS_SPENT houses Rpt Group
       SQL2 = 'and g.GROUPS in (%s)';
var s : shortstring;       
begin
     result := False;
     with dm.qryReport do begin
        close;
        if mGroups <> '' then begin
           s := format(SQL2,[mGroups]);
           SQL.Text := format(SQL1,[s])
        end else   
           SQL.Text := format(SQL1,['']);
        open;
        qr.PrinterSettings.PrinterIndex := PrtIndex;
        result := True;
     end;
end;
{ **************************************************************************** }

end.
