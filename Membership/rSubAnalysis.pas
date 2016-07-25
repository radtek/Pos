unit rSubAnalysis;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmSubAnalysis = class(TForm)
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
    DetailBand: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText6: TQRDBText;
    QRDBText7: TQRDBText;
    QRDBText1: TQRDBText;
    SummaryBand: TQRBand;
    QRExpr1: TQRExpr;
    QRExpr2: TQRExpr;
    QRExpr3: TQRExpr;
    QRLabel2: TQRLabel;
    QRLabel4: TQRLabel;
    lblDesc: TQRLabel;
    lblFee: TQRLabel;
    lblUnPaid: TQRLabel;
    lblUnPaidCount: TQRLabel;
    QRBand5: TQRBand;
    lblLogo: TQRLabel;
    QRLabel9: TQRLabel;
    QRDBText3: TQRDBText;
    QRExpr4: TQRExpr;
    procedure qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
    function Extract(qrPrint,BySection : Boolean; PrtIndex : SmallInt ) : Boolean;
    procedure DetailBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    procedure SummaryBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    procedure FormDestroy(Sender: TObject);

  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmSubAnalysis: TfrmSubAnalysis;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
 var
    Printing : Boolean;
    RptBySection : Boolean;
{ **************************************************************************** }
procedure TfrmSubAnalysis.qrBeforePrint(Sender: TCustomQuickRep;
  var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
     {if not Printing then begin
       QRBand1.Color := clAqua;
       lblHeading.Color  := clAqua;
       lblReportTitle.Color  := clAqua;
       QRSysData1.Color  := clAqua;
       QRLabel10.Color  := clAqua;
       QRSysData2.Color  := clAqua;
    end;}
end;
{ **************************************************************************** }
function TfrmSubAnalysis.Extract(qrPrint,BySection : Boolean; PrtIndex : SmallInt ) : Boolean;
begin
     dm.tblSections.Open;
     result := False;
     RptBySection := BySection;
     with dm.qryReport do begin
         close;
         if RptBySection then
             SQL.Text := 'SELECT SECTION AS CODE,SUM(AMOUNT) AS AMOUNT,SUM(AMOUNT_PAID) AS' +
                      ' AMOUNT_PAID, (SUM(AMOUNT) - SUM(AMOUNT_PAID)) as AMOUNT_DUE,COUNT(MEMBER) AS MEMBERCNT'+
                      ' from M_SECTIONS ' +
                      ' GROUP BY SECTION ORDER BY SECTION'
         else
            SQL.Text := 'SELECT SUBSCRIPTION AS CODE,SUM(AMOUNT) AS AMOUNT,SUM(AMOUNT_PAID) AS' +
                         ' AMOUNT_PAID,(SUM(AMOUNT) - SUM(AMOUNT_PAID)) as AMOUNT_DUE, COUNT(MEMBER) AS MEMBERCNT from M_SUBS ' +
                         ' GROUP BY SUBSCRIPTION ORDER BY SUBSCRIPTION';

         open;
     end;
     with dm.qryReport1 do begin
         close;
         if RptBySection then
            SQL.Text :=  'Select SECTION, count(PAID) as PAID from M_SECTIONS where ' +
               'PAID = ''N'' GROUP BY SECTION order by SECTION '
         else
            SQL.Text := 'select ms.SUBSCRIPTION, COUNT(ms.member) AS PAID FROM Members m,M_SUBS ms ' +
              'where m.SUB_PAID between 0 and 1 and m.member = ms.member GROUP BY ms.SUBSCRIPTION ORDER BY ms.SUBSCRIPTION';
             // 'where AMOUNT_PAID < AMOUNT or AMOUNT = 0.00 GROUP BY SUBSCRIPTION ORDER BY SUBSCRIPTION';
         open;
     end;
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     Printing := qrPrint;
     result := True;
end;
{ **************************************************************************** }
procedure TfrmSubAnalysis.DetailBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
var v : Variant;
begin
     if not RptBySection then begin
        // locate sometimes does not see all the records
        TblActiveOrRefresh(dm.tblSubs);
        v := dm.qryReport.FieldByName('CODE').AsInteger;
        if dm.qryReport1.Locate('SUBSCRIPTION',V,[]) then
           lblUnPaid.Caption := IntToStr(dm.qryReport1.FieldByName('PAID').AsInteger)
        else
           lblUnPaid.Caption := '0';
        if dm.tblSubs.Locate('SUBSCRIPTION',V,[]) then begin
           lblDesc.Caption := dm.tblSubs.FieldByName('DESCRIPTION').AsString;
           lblFee.Caption  := FloatToStrF(dm.tblSubs.FieldByName('FEE').AsFloat,ffCurrency,7,2);
        end else begin
           lblDesc.Caption := '';
           lblFee.Caption  := '';
        end;
     end else begin
        v := dm.qryReport.FieldByName('CODE').AsInteger;
        if dm.qryReport1.Locate('SECTION',V,[]) then
           lblUnPaid.Caption := IntToStr(dm.qryReport1.FieldByName('PAID').AsInteger)
        else
           lblUnPaid.Caption := '';
        if dm.tblSECTIONS.Locate('SECTION',V,[]) then begin
           lblDesc.Caption := dm.tblSECTIONS.FieldByName('DESCRIPTION').AsString;
           lblFee.Caption  := FloatToStrF(dm.tblSECTIONS.FieldByName('FEE').AsFloat,ffCurrency,7,2);
        end else begin
           lblDesc.Caption := '';
           lblFee.Caption  := '';
        end;
     end;
end;
{ **************************************************************************** }
procedure TfrmSubAnalysis.SummaryBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
begin
     with dm.qryGeneral do begin
         close;
         if RptBySection then
            SQL.Text :=  'Select count(PAID) as PAID from M_SECTIONS where PAID = ''N'''
         else
            SQL.Text := 'select COUNT(member) AS PAID FROM Members where SUB_PAID between 0 and 1';
            //SQL.Text := 'select COUNT(member) AS PAID FROM M_SUBS where AMOUNT_PAID < AMOUNT or AMOUNT = 0.00';
         execquery;
         if not EOF then
            lblUnPaidCount.Caption := IntToStr(dm.qryGeneral.FieldByName('PAID').AsInteger);
     end; 
end;
{ **************************************************************************** }
procedure TfrmSubAnalysis.FormDestroy(Sender: TObject);
begin
     dm.tblSections.Close;
end;
{ ****************************************************************************}
end.
