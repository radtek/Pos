unit rReceiptReport;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmReceiptReport = class(TForm)
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
    QRLabel3: TQRLabel;
    QRLabel5: TQRLabel;
    QRLabel8: TQRLabel;
    DetailBand: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText1: TQRDBText;
    QRDBText3: TQRDBText;
    QRDBText7: TQRDBText;
    QRBand4: TQRBand;
    lblLogo: TQRLabel;
    QRLabel7: TQRLabel;
    lblPointsUsed: TQRLabel;
    lblGroup2: TQRLabel;
    lblGroup3: TQRLabel;
    lblGroup4: TQRLabel;
    lblGroup5: TQRLabel;
    lblGroup6: TQRLabel;
    lblGroup7: TQRLabel;
    lblGroup8: TQRLabel;
    lblGroup1: TQRLabel;
    lblGroup1Amount: TQRLabel;
    lblGroup4Amount: TQRLabel;
    lblGroup3Amount: TQRLabel;
    lblGroup2Amount: TQRLabel;
    lblGroup5Amount: TQRLabel;
    lblGroup6Amount: TQRLabel;
    lblGroup7Amount: TQRLabel;
    lblGroup8Amount: TQRLabel;
    SummaryBAnd: TQRBand;
    QRLabel20: TQRLabel;
    QRLabel21: TQRLabel;
    QRExpr1: TQRExpr;
    lblGroup5Total: TQRLabel;
    lblGroup4Total: TQRLabel;
    lblGroup3Total: TQRLabel;
    lblGroup2Total: TQRLabel;
    lblGroup1Total: TQRLabel;
    lblGroup6Total: TQRLabel;
    lblGroup7Total: TQRLabel;
    lblGroup8Total: TQRLabel;
    lblAmountTotal: TQRLabel;
    lblAmount: TQRLabel;
    lblDiscountTotal: TQRLabel;
    lblGroup9: TQRLabel;
    lblGroup9Amount: TQRLabel;
    lblGroup9Total: TQRLabel;
    QRLabel6: TQRLabel;
    lblDiscountAmount: TQRLabel;
    lblPointsUsedTotal: TQRLabel;
    QRDBText4: TQRDBText;
    procedure SummaryBAndBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    function Extract(PrtIndex,SortOrder : smallint; DateFrom,DateTo : TDateTime) : Boolean;
    procedure DetailBandBeforePrint(Sender: TQRCustomBand;      var PrintBand: Boolean);
    procedure FormDestroy(Sender: TObject);
    procedure StringBandBeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
  private                         
    { Private declarations }
    FAmount,FGroup1,FGroup2,FGroup3,FGroup4,FGroup5,FGroup6,FGroup7,FGroup8,FGroup9,FAmountDiscount,FPointsUsed : Extended;
    ArrayGroups : array [1..30] of SmallInt;
    FKeepReceipt : LongInt;                               
    FDateFrom,FDateTo :TDateTime;
    
  public
    { Public declarations }
  end;

var
  frmReceiptReport: TfrmReceiptReport;
  ReceiptList : TStringList;
                                     
implementation

{$R *.DFM}
uses
    Global,dmMembers;
{ **************************************************************************** }

procedure TfrmReceiptReport.SummaryBAndBeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
begin
     lblAmountTotal.Caption := FloatToStrF(FAmount,ffCurrency,6,2);
     //if FAmountDiscount > 0.00 then
     //  lblDiscountTotal.Caption := FloatToStrF(FAmountDiscount,ffCurrency,6,2);
     if FPointsUsed > 0.00 then
       lblPointsUsedTotal.Caption := FloatToStrF(FPointsUsed,ffCurrency,6,2);
     if FGroup1 <> 0.00 then
       lblGroup1Total.Caption := FloatToStrF(FGroup1,ffCurrency,6,2);
     if FGroup2 <> 0.00 then
        lblGroup2Total.Caption := FloatToStrF(FGroup2,ffCurrency,6,2);
     if FGroup3 <> 0.00 then
        lblGroup3Total.Caption := FloatToStrF(FGroup3,ffCurrency,6,2);
     if FGroup4 <> 0.00 then
        lblGroup4Total.Caption := FloatToStrF(FGroup4,ffCurrency,6,2);
     if FGroup5 <> 0.00 then
        lblGroup5Total.Caption := FloatToStrF(FGroup5,ffCurrency,6,2);
     if FGroup6 <> 0.00 then
        lblGroup6Total.Caption := FloatToStrF(FGroup6,ffCurrency,6,2);
     if FGroup7 <> 0.00 then
        lblGroup7Total.Caption := FloatToStrF(FGroup7,ffCurrency,6,2);
     if FGroup8 <> 0.00 then  
        lblGroup8Total.Caption := FloatToStrF(FGroup8,ffCurrency,6,2);
     if FGroup9 <> 0.00 then
        lblGroup9Total.Caption := FloatToStrF(FGroup9,ffCurrency,6,2);
        
     

end;

{ **************************************************************************** }
procedure TfrmReceiptReport.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
    FAmount := 0.00; FAmountDiscount := 0.00; FPointsUsed := 0.00;
    ReceiptList := TStringList.Create;
     FGroup1 := 0.00;
     FGroup2 := 0.00;
     FGroup3 := 0.00;
     FGroup4 := 0.00;
     FGroup5 := 0.00;
     FGroup6 := 0.00;
     FGroup7 := 0.00;
     FGroup8 := 0.00;      
     FGroup9 := 0.00;

end;
{ **************************************************************************** }
function TfrmReceiptReport.Extract(PrtIndex,SortOrder : smallint; DateFrom,DateTo : TDateTime) : Boolean;
{const SQL1 = 'select pd.PAYMENT,pd.PAY_DATE,pd.MEMBER,pd.FULL_NAME,p.PAY_AMOUNT,p.DISCOUNT, ' +
             'pd.PAY_TYPE,pd.AMOUNT,pd.AMOUNT_PAID from PAYMENTS_DET pd,PAYMENTS p ' +
             'where pd.PAY_DATE between ''%s'' and ''%s'' and p.PAYMENT = pd.PAYMENT ' ;}
const SQL2 = 'select distinct pd.MEMBER,pd.PAYMENT,pd.PAY_DATE,p.DISCOUNT,POINTS_USED,pd.full_name,m.First_Name,' +
             'p.Pay_amount,m.SURNAME,M.KNOWN_AS,m.MAILING_LIST_CODE,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY  ' +
             'from PAYMENTS_DET pd,PAYMENTS p,MEMBERS m  ' +
             'where pd.PAY_DATE between ''%s'' and ''%s'' and p.PAYMENT = pd.PAYMENT ' +
             'and m.MEMBER = pd.MEMBER order by %s,pd.PAY_DATE';
var i : Shortint;  ByOrder : String[30];
    s : string;
    J : integer;
const ByReceipt = 'pd.PAYMENT';
      BySurname = 'm.SPARE_VARCHAR_50,m.SURNAME';
begin
     result := False;
     FDateFrom := DateFrom;
     FDateTo := DateTo;
     with dm.qryReport do begin
         case SortOrder of
            0 : ByOrder := ByReceipt;
            1 : ByOrder := BySurname;
         end;
        sql.text := format(SQL2,[GetShortDate(DateFrom),GetShortdate(DateTo),ByOrder]);
        dm.tblGroups.Open;
        dm.tblGroups.First;
        i := 1;
        while not dm.tblGroups.EOF do begin
           // Pauanui - dont include 9 and 10 year groups in the report
           // Hard Code Fix - Pauanui seem to be the only one using the report
          { if (POS('Pauanui',CompName) > 0) and
              (dm.tblGroups.FieldByName('GROUPS').AsInteger = 104) or
              (dm.tblGroups.FieldByName('GROUPS').AsInteger = 105) then begin
              dm.tblGroups.next;
              continue;
           end;}
           // Pauanui 31/08/2004 - can only have 9 groups on the report
           if (dm.tblGroups.FieldByName('IN_REPORT').AsString = varNo) or
              (dm.tblGroups.FieldByName('IN_REPORT').IsNull) then begin
              dm.tblGroups.next;
              continue;
           end;
           
           //truncate sports player at Pauanui
           s := dm.tblGroups.FieldByName('DESCRIPTION').AsString;
           if pos('SPORTS PLAYER',s) > 0 then begin
              J := pos('SPORTS',s);
              system.delete(s,J,13);
              s := 'SP ' + s;
           end;
           if i <= 9 then begin
              case i of
                 1 : lblGroup1.Caption := s;
                 2 : lblGroup2.Caption := s;
                 3 : lblGroup3.Caption := s;
                 4 : lblGroup4.Caption := s;
                 5 : lblGroup5.Caption := s;
                 6 : lblGroup6.Caption := s;
                 7 : lblGroup7.Caption := s;
                 8 : lblGroup8.Caption := s;
                 9 : lblGroup9.Caption := s;
              end;
              ArrayGroups[i] := dm.tblGroups.FieldByName('GROUPS').AsInteger;
              inc(i);
           end;   
           dm.tblGroups.next;
        end;
        open;
        //FKeepReceipt := -1;
     end;
     dm.tblGroups.Close;
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     result := True;
end;
{ **************************************************************************** }
procedure TfrmReceiptReport.DetailBandBeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
const SQL1 = 'Select s.sub_groups,sum(Amount_paid) as AMOUNT_PAID from  payments_det pd,Subscriptions s ' +
             'where pd.code = s.subscription and pd.payment = %d and pd.Member = %d ' +
             'group by s.sub_groups order by s.Sub_groups';
      SQL2 = 'Select TENDER_TYPE from PAYMENTS_TEND where PAYMENT = %d';       
var i : Shortint;
    j : Integer;
begin
     lblDiscountAmount.Caption := ' ';
     lblPointsUsed.Caption := ' ';
     lblGroup1Amount.Caption := '';
     lblGroup2Amount.Caption := '';
     lblGroup3Amount.Caption := '';
     lblGroup4Amount.Caption := '';
     lblGroup5Amount.Caption := '';
     lblGroup6Amount.Caption := '';
     lblGroup7Amount.Caption := '';
     lblGroup8Amount.Caption := '';
     lblGroup9Amount.Caption := '';
     ReceiptList.Sort;
     if ReceiptList.Find( dm.qryReport.FieldByName('PAYMENT').AsString,j) then begin
        lblAmount.Caption := ' ';
        lblPointsUsed.Caption := ' ';
        lblDiscountAmount.Caption := ' ';
     end else begin
        ReceiptList.Add(dm.qryReport.FieldByName('PAYMENT').AsString);
        lblAmount.Caption := FloatToStrF(dm.qryReport.FieldByName('PAY_AMOUNT').AsFloat,ffCurrency,6,2);
        if dm.qryReport.FieldByName('POINTS_USED').AsFloat <> 0.00 then
           lblPointsUsed.Caption := FloatToStrF(dm.qryReport.FieldByName('POINTS_USED').AsFloat,ffCurrency,6,2);
        //if dm.qryReport.FieldByName('DISCOUNT').AsFloat <> 0.00 then
        //   lblDiscountAmount.Caption := FloatToStrF(dm.qryReport.FieldByName('DISCOUNT').AsFloat,ffCurrency,6,2);
        // changed 10/03/03 for Patatoetoe Discount to tender_type
        // bug - as Points PAyments dont have an Entry in  PAYMENTS_TEND
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL2,[dm.qryReport.FieldByName('PAYMENT').AsInteger,dm.qryReport.FieldByName('MEMBER').AsInteger])) then 
           lblDiscountAmount.Caption := dm.qryGeneral.FieldByName('TENDER_TYPE').AsString
        else
           lblDiscountAmount.Caption := 'Points';
        FAmount := FAmount + dm.qryReport.FieldByName('PAY_AMOUNT').AsFloat;
        FPointsUsed := FPointsUsed + dm.qryReport.FieldByName('POINTS_USED').AsFloat;
        FAmountDiscount := FAmountDiscount + dm.qryReport.FieldByName('DISCOUNT').AsFloat;
     end;
     //FKeepReceipt := dm.qryReport.FieldByName('PAYMENT').AsInteger;
     // fill in each group and keep totals
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[dm.qryReport.FieldByName('PAYMENT').AsInteger,dm.qryReport.FieldByName('MEMBER').AsInteger])) then begin
        with dm.qryGeneral do begin
           while not EOF do begin
              for i := 1 to 9 do begin
                if ArrayGroups[i] = 0 then continue;
                case i of                   
                   1 : begin
                          if FieldByName('SUB_GROUPS').AsInteger = ArrayGroups[i] then begin
                             lblGroup1Amount.Caption := FloatToStrF(FieldByName('AMOUNT_PAID').AsFloat,ffCurrency,6,2);
                             Fgroup1 := FGroup1 + FieldByName('AMOUNT_PAID').AsFloat;
                          end;
                       end;
                   2 : begin
                          if FieldByName('SUB_GROUPS').AsInteger = ArrayGroups[i] then begin
                             lblGroup2Amount.Caption := FloatToStrF(FieldByName('AMOUNT_PAID').AsFloat,ffCurrency,6,2);
                             Fgroup2 := FGroup2 + FieldByName('AMOUNT_PAID').AsFloat;
                          end;
                       end;
                   3 : begin
                          if FieldByName('SUB_GROUPS').AsInteger = ArrayGroups[i] then begin
                             lblGroup3Amount.Caption := FloatToStrF(FieldByName('AMOUNT_PAID').AsFloat,ffCurrency,6,2);
                             Fgroup3 := FGroup3 + FieldByName('AMOUNT_PAID').AsFloat;
                          end;        
                       end;
                   4 : begin
                          if FieldByName('SUB_GROUPS').AsInteger = ArrayGroups[i] then begin
                             lblGroup4Amount.Caption := FloatToStrF(FieldByName('AMOUNT_PAID').AsFloat,ffCurrency,6,2);
                             Fgroup4 := FGroup4 + FieldByName('AMOUNT_PAID').AsFloat;
                          end;        
                       end;
                   5 : begin
                          if FieldByName('SUB_GROUPS').AsInteger = ArrayGroups[i] then begin
                             lblGroup5Amount.Caption := FloatToStrF(FieldByName('AMOUNT_PAID').AsFloat,ffCurrency,6,2);
                             Fgroup5 := FGroup5 + FieldByName('AMOUNT_PAID').AsFloat;
                          end;        
                       end;
                   6 : begin
                          if FieldByName('SUB_GROUPS').AsInteger = ArrayGroups[i] then begin
                             lblGroup6Amount.Caption := FloatToStrF(FieldByName('AMOUNT_PAID').AsFloat,ffCurrency,6,2);
                             Fgroup6 := FGroup6 + FieldByName('AMOUNT_PAID').AsFloat;
                          end;        
                       end;
                   7 : begin
                          if FieldByName('SUB_GROUPS').AsInteger = ArrayGroups[i] then begin
                             lblGroup7Amount.Caption := FloatToStrF(FieldByName('AMOUNT_PAID').AsFloat,ffCurrency,6,2);
                             Fgroup7 := FGroup7 + FieldByName('AMOUNT_PAID').AsFloat;
                          end;        
                       end;
                   8 : begin
                          if FieldByName('SUB_GROUPS').AsInteger = ArrayGroups[i] then begin
                             lblGroup8Amount.Caption := FloatToStrF(FieldByName('AMOUNT_PAID').AsFloat,ffCurrency,6,2);
                             Fgroup8 := FGroup8 + FieldByName('AMOUNT_PAID').AsFloat;
                          end;        
                       end;
                   9 : begin
                          if FieldByName('SUB_GROUPS').AsInteger = ArrayGroups[i] then begin
                             lblGroup9Amount.Caption := FloatToStrF(FieldByName('AMOUNT_PAID').AsFloat,ffCurrency,6,2);
                             Fgroup9 := FGroup9 + FieldByName('AMOUNT_PAID').AsFloat;
                          end;
                       end;
                       
                end;
             end;
             next;
           end;
        end;
     end;
end; 
{ **************************************************************************** }
procedure TfrmReceiptReport.FormDestroy(Sender: TObject);
begin
    ReceiptList.Free;
end;
{ **************************************************************************** }
procedure TfrmReceiptReport.StringBandBeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
const SQL1 = 'select CODE,DESCRIPTION,sum(AMOUNT_PAID) as AMOUNT_PAID,sum(AMOUNT),count(*) as "COUNT" from PAYMENTS_DET ' +
             'where PAY_DATE between ''%s'' and ''%s'' and PAY_TYPE = 0 group by CODE,DESCRIPTION';
      sp = '  ';
      tab  = chr(09);
begin
     // make up the final part of the query
     // use Rich Text to format the data
     with dm.qryGeneral do begin
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[GetShortDAte(FDateFrom),GetShortDAte(FDateTo)])) then begin
           while not EOF do begin
              //StringBand.Items.Add(FieldByName('CODE').AsString + tab +
              //                   FieldByName('DESCRIPTION').AsString + tab + tab +
              //                   FieldByName('COUNT').AsString + tab + 
             //                    formatFloat('$###0.00',FieldByName('AMOUNT_PAID').AsFloat));
              next;                   
           end;
        end;
     end;
     PrintBand := True;

end;

end.

