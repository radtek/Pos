unit rMMDiscounts;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmMMDiscounts = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    QRSysData2: TQRSysData;
    lblHeading: TQRLabel;
    lblReportTitle: TQRLabel;
    QRLabel10: TQRLabel;
    QRBand5: TQRBand;
    QRLabel12: TQRLabel;
    QRLabel13: TQRLabel;
    QRLabel1: TQRLabel;
    QRLabel2: TQRLabel;
    qrDetailBand: TQRBand;
    QRDBText7: TQRDBText;
    qrSummaryBand: TQRBand;
    QRExpr6: TQRExpr;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    QRLabel3: TQRLabel;
    QRLabel4: TQRLabel;
    QRDBText2: TQRDBText;
    QRDBText3: TQRDBText;
    QRLabel5: TQRLabel;
    QRLabel6: TQRLabel;
    QRLabel7: TQRLabel;
    lblName: TQRLabel;
    lblDescription: TQRLabel;
    lblMenu: TQRLabel;
    lblPercent: TQRLabel;
    lblAmount: TQRLabel;
    QRLabel8: TQRLabel;
    QRSysData3: TQRSysData;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    procedure qrDetailBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
  private
    { Private declarations }
    fNoDiscount : Boolean;
  public
    { Public declarations }
    function Extract(PrtIndex : shortint; Discounts : string ; NoDiscount : Boolean  ) : Boolean;
  end;

var
  frmMMDiscounts: TfrmMMDiscounts;

implementation

{$R *.DFM}
uses Global,
     dmMembers,
     dmMenuMate;
{ ****************************************************************************}     
procedure TfrmMMDiscounts.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
    lblName.Caption        := '';
    lblDescription.Caption := '';
    lblMenu.Caption        := '';
    lblAmount.Caption      := '';
    lblPercent.Caption     := '';
end;
{ ****************************************************************************}
function TfrmMMDiscounts.Extract(PrtIndex : shortint; Discounts : string ; NoDiscount : Boolean  ) : Boolean;
const SQL1 = 'select m.member, m.Full_Name,m.First_name,m.Surname,d.discount_Key,d.Member_Discounts_Key ';
      SQL2 = 'from members m left join members_discounts d on m.member = d.member ';
      SQL3 = 'from members m,members_discounts d WHERE m.member = d.member ';
begin
     result := false;
     with dm.qryReport do begin
        close;
        if NoDiscount then begin 
           SQL.Text := SQL1 + SQL2 + 'order by m.Surname,d.discount_key';
        end else begin   
           // all discounts
           if Discounts = '' then
              SQL.Text := SQL1 + SQL3 + ' order by m.Surname,d.discount_key'
           else
              SQL.Text := SQL1 + SQL3 + format(' and d.discount_key in (%s) order by m.Surname,d.discount_key',[Discounts]);
        end;   
        open;
     end;
     fNoDiscount := NoDiscount;
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     result := True;
end;
{ ****************************************************************************}
procedure TfrmMMDiscounts.qrDetailBandBeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
begin
     // show Members with no Discounts
     if fNoDiscount then begin
        if not dm.qryReport.FieldByName('DISCOUNT_KEY').IsNull then 
           PrintBand := False;
     end else begin
        if dm.qryReport.FieldByName('DISCOUNT_KEY').IsNull then 
           PrintBand := False;
        with dmMM.tblDiscounts do begin 
           if Locate('DISCOUNT_KEY',dm.qryReport.FieldByName('DISCOUNT_KEY').AsInteger ,[]) then begin
              lblName.Caption        := FieldByName('NAME').AsString; 
              lblDescription.Caption := FieldByName('DESCRIPTION').AsString; 
              lblMenu.Caption        := FieldByName('MENU').AsString; 
              lblAmount.Caption      := formatFloat('$0.00',FieldByName('AMOUNT').AsFloat); 
              lblPercent.Caption     := formatFloat('0.00%',FieldByName('PERCENT').AsFloat); 
           end else begin
              lblName.Caption        := '';
              lblDescription.Caption := '';
              lblMenu.Caption        := '';
              lblAmount.Caption      := '';
              lblPercent.Caption     := '';
           end;   
       end;   
    end;   
end;
{ ****************************************************************************}
end.
