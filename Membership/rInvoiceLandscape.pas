unit rInvoiceLandscape;
interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmInvoiceLandscape = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    bandDetail: TQRBand;
    QRDBText2: TQRDBText;
    dbtFee: TQRDBText;
    dbtDue: TQRDBText;
    QRDBText7: TQRDBText;
    bandGroupFooter: TQRBand;
    QRBand6: TQRBand;
    QRLabel4: TQRLabel;
    QRLabel10: TQRLabel;
    QRLabel32: TQRLabel;
    QRLabel33: TQRLabel;
    QRLabel35: TQRLabel;
    lblYear: TQRLabel;
    txtAddress: TQRRichText;
    QRLabel21: TQRLabel;
    QRLabel22: TQRLabel;
    QRLabel23: TQRLabel;
    QRLabel24: TQRLabel;
    QRLabel25: TQRLabel;
    QRLabel26: TQRLabel;
    grpMailingListCode: TQRGroup;
    QRLabel1: TQRLabel;
    QRLabel3: TQRLabel;
    QRShape1: TQRShape;
    lblTotalAmountDue: TQRLabel;
    grpMember: TQRGroup;
    QRDBText4: TQRDBText;
    QRDBText5: TQRDBText;
    lblPage: TQRLabel;
    lblDate: TQRLabel;
    txtAddress1: TQRRichText;
    QRShape2: TQRShape;
    lblTotalAmountDue1: TQRLabel;
    QRLabel9: TQRLabel;
    lblPage1: TQRLabel;
    dbtDue1: TQRDBText;
    QRDBText8: TQRDBText;
    QRDBText9: TQRDBText;
    QRDBText10: TQRDBText;
    QRLabel12: TQRLabel;
    QRLabel14: TQRLabel;
    QRLabel15: TQRLabel;
    lblYear1: TQRLabel;
    lblGST: TQRLabel;
    lblAmountDue: TQRLabel;
    lblAmountDue1: TQRLabel;
    lblAmountPAid: TQRLabel;
    lblAmountPaid1: TQRLabel;
    QRLabel6: TQRLabel;
    MemoInvoice: TQRMemo;
    lblCompName: TQRDBText;
    lblAddress1: TQRDBText;
    lblAddress2: TQRDBText;
    lblAddress3: TQRDBText;
    function ExtractMember( PrtIndex,PartPay : Integer; InvoiceMembers : ShortString ; 
                            Discount : Extended ) : Boolean;
    procedure qrStartPage(Sender: TCustomQuickRep);
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    procedure bandGroupFooterAfterPrint(Sender: TQRCustomBand;  BandPrinted: Boolean);
    procedure bandDetailAfterPrint(Sender: TQRCustomBand;BandPrinted: Boolean);
    procedure bandGroupFooterBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    function ExtractSubsRun(  PrtIndex : Integer   ) : Boolean;
    function ExtractSubsExpires(  PrtIndex,PaymentIndex : Integer ;mGroups,mMembers : ShortString ;
                                  DateFrom,DateTo : TdateTime  ) : Boolean;
    procedure FormDestroy(Sender: TObject);
    procedure bandDetailBeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
    procedure FormCreate(Sender: TObject);
    procedure QRBand1BeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
    
  private                                   
    { Private declarations }
     FPageNumber{,FDiscountable} : smallint;
     FAmountPAid, FAmountDue{,FDiscount,FDiscountMember }: Extended;
     FExtractByMember,FFirstTimeMember : Boolean;
     FMember : LongInt;
  public
    { Public declarations }
  end;

var
  frmInvoiceLandscape: TfrmInvoiceLandscape;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
{ **************************************************************************** }
function TfrmInvoiceLandscape.ExtractMember(  PrtIndex,PartPay : Integer ; 
                                              InvoiceMembers : ShortString ;
                                              Discount : Extended  ) : Boolean;
             // use Amount as the Member could have paid ahead
const SQL1 = 'SELECT m.MAILING_LIST_CODE,m.MEMBER,m.FULL_NAME,m.KNOWN_AS,s.DESCRIPTION,ms.PAY,' +
             'ms.AMOUNT,(ms.AMOUNT / %d) as PART_PAY_DUE, ms.AMOUNT_PAID,(s.FEE - ms.AMOUNT_PAID) as AMOUNT_DUE, '+
             '(ms.AMOUNT - ms.AMOUNT_PAID) as AMOUNT_DUE_AMOUNT,m.EXPIRES,s.FEE,s.DISCOUNTABLE ' +
             'from MEMBERS m,M_SUBS ms,SUBSCRIPTIONS s ' + 
             'where m.MEMBER = ms.MEMBER and ms.SUBSCRIPTION = s.SUBSCRIPTION ' +
             'and s.ON_INVOICE = ''Y'' ' +   // may want to get some 0.00 amounts - bridge,life etc
            // 'and (ms.AMOUNT - ms.AMOUNT_PAID) > 0.00 ' +   
             'and m.MEMBER in (%s) order by m.MAILING_LIST_CODE,m.MEMBER';
begin
     result    := False;
     FExtractByMember := True;
     //FDiscount := Discount;
     // use amount in this case as it could be pro rata from the Pay By Member tab
     dbtFee.DataField := 'AMOUNT';
     // and due as Amount - Amount Paid
     dbtDue.DataField  := 'PAY';
     dbtDue1.DataField := 'PAY';
     

     {if PartPay = 1 then begin
        dbtDue.DataField := 'AMOUNT_DUE_AMOUNT';
        dbtDue1.DataField := 'AMOUNT_DUE_AMOUNT';
     end else begin
        dbtDue.DataField := 'PART_PAY_DUE';
        dbtDue1.DataField := 'PART_PAY_DUE';
     end; }  
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     dm.qryOpen(dm.qryReport ,format(SQL1,[PArtPay,InvoiceMembers]));
     Result := True;
end; 
{ **************************************************************************** }
function TfrmInvoiceLandscape.ExtractSubsRun(  PrtIndex : Integer   ) : Boolean;
//const SQL1 =   ' order by m.MAILING_LIST_CODE,m.MEMBER';
begin
  {   result    := False;
     FExtractByMember := False;
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     if Extract( SQLInvoice + SQL1) then
        Result := True;}
end;
{ **************************************************************************** }
function TfrmInvoiceLandscape.ExtractSubsExpires(  PrtIndex,PaymentIndex : Integer; mGroups,mMembers : ShortString; 
                                                   DateFrom,DateTo : TdateTime ) : Boolean;
const SQL1 = 'delete from INVOICE';    
      SQL2 = 'INSERT INTO INVOICE SELECT distinct m.MEMBER,m.MAILING_LIST_CODE,M.SPARE_VARCHAR_50,' +
             'm.FULL_NAME,m.EXPIRES,s.DESCRIPTION,ms.AMOUNT, ms.AMOUNT_PAID,' +
             '(ms.AMOUNT - ms.AMOUNT_PAID) as AMOUNT_DUE,s.FEE,0.00,'''',S.DISCOUNTABLE,'''',' +
             'M.SUB_PAID from MEMBERS m,M_SUBS ms,SUBSCRIPTIONS s %s where m.MEMBER = ms.MEMBER ' +
             'and MS.SUBSCRIPTION = s.SUBSCRIPTION %s AND s.ON_INVOICE = ''Y'' ' +
             'and m.EXPIRES between ''%s'' and ''%s'' and m.deceased <> ''Y'' %s';
      // update amounts where just prior to expires or after expires
      SQL3 = 'UPDATE INVOICE SET AMOUNT = FEE, AMOUNT_PAID = 0.00,AMOUNT_DUE = FEE,' +
             'SUB_PAID = 0';
      SQL5 = 'UPDATE INVOICE SET AMOUNT = FEE, AMOUNT_PAID = 0.00,AMOUNT_DUE = FEE,' +
             'SUB_PAID = 0 WHERE (CURRENT_DATE + 40) >= EXPIRES';
      // keep sort for grouping 
      SQL4 = 'select * from invoice where sub_paid <> 2 order by spare_varchar_50,mailing_list_code,member'; 
      GroupsTable  = ',M_GROUPS mg';
      GroupsFields = 'and m.MEMBER = mg.MEMBER AND mg.GROUPS in (%s)';
var  GroupsFieldsStr : ShortString;
begin
     result    := False;
     FExtractByMember := False;
     dm.sqlExecQuery(dm.qryGeneral,SQL1);
     if mGroups <> '' then begin
        GroupsFieldsStr := format(GroupsFields,[mGroups]);
        dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[GroupsTable,GroupsFieldsStr,GetShortDate(DateFrom),GetShortDate(DateTo),mMembers]))
     end else
        dm.sqlExecQuery(dm.qryGeneral,format(SQL2,['','',GetShortDate(DateFrom),GetShortDate(DateTo),mMembers]));

     case PaymentIndex of
        //0 :  Dont Reset Payments - therefore dont do anything here
        1 : dm.sqlExecQuery(dm.qryGeneral,SQL3); // reset payment and get latest fees 
        2 : dm.sqlExecQuery(dm.qryGeneral,SQL5); // reset payment and get latest fees if within 40 days of expiry date
        
     end;    
     dm.qryOpen(dm.qryReport,SQL4);
     dm.IBCommitRetain;
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     result    := True;
end;
{ **************************************************************************** }
procedure TfrmInvoiceLandscape.qrStartPage(Sender: TCustomQuickRep);
const SQL1 = 'Select FULL_NAME,MEMBER,ADDRESS,ADDRESS1,DISTRICT,CITY,COUNTRY from MEMBERS where MEMBER = %d and MAILING_LIST_CODE = %d';
begin
     lblPage.Caption := IntToStr(FPageNumber);
     lblPage1.Caption := IntToStr(FPageNumber);
     lblGST.Caption := Global.GSTNo;
     inc(FPageNumber);
     txtAddress.Lines.Clear;
     txtAddress1.Lines.Clear;
     //lblMailingListCodeMemberMember.Caption := dm.qryReport.FieldByName('MAILING_LIST_CODE').AsString;
     //lblMailingListCodeMemberMember1.Caption := lblMailingListCodeMemberMember.Caption;
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[dm.qryReport.FieldByName('MAILING_LIST_CODE').AsInteger,dm.qryReport.FieldByName('MAILING_LIST_CODE').AsInteger])) then begin
        with dm.qryGeneral do begin
           GetAddressList(FieldByName('ADDRESS').AsString,FieldByName('ADDRESS1').AsString,
                          FieldByName('DISTRICT').asString,FieldByName('CITY').asString,
                          FieldByName('COUNTRY').asString,txtAddress.Lines);
           GetAddressList(FieldByName('ADDRESS').AsString,FieldByName('ADDRESS1').AsString,
                          FieldByName('DISTRICT').asString,FieldByName('CITY').asString,
                          FieldByName('COUNTRY').asString,txtAddress1.Lines);
           txtAddress.Lines.Insert( 0,FieldByName('FULL_NAME').asString);
           txtAddress1.Lines.Insert( 0,FieldByName('FULL_NAME').asString);
          // txtAddress1.Lines := txtAddress.Lines;              
        end;
     end;  
end;
{ **************************************************************************** }
procedure TfrmInvoiceLandscape.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
const Msg1 = 'Amount Due ( Please Pay Immediately )';
var Year,Month,DAy : Word;
    TempInvoiceMsg,s : ShortString;
    i : shortint;
begin
     FPageNumber := 1;
     //FDiscountable := 0;
     if FixedExpirySystem then begin
        DecodeDAte(Global.MemberExpiry,Year,Month,DAy);   
        // passed expires allow 30 days - printout invoices prior to expires
        if (Global.MemberExpiry - 60) <= date then
           lblYear.Caption := IntToStr(Year) + '/' + IntToStr(Year + 1)
        else
           lblYear.Caption := IntToStr(Year -1) + '/' + IntToStr(Year);
     end else begin
        DecodeDAte(DAte,Year,Month,DAy);   
        lblYear.Caption := IntToStr(Year) + '/' + IntToStr(Year + 1);
     end;
     lblYear1.CAption := lblYear.Caption;
     lblDate.Caption  := GetShortMonthDate(Date);
     FAmountPAid := 0.00;
     FAmountDue  := 0.00;
     FMember := 0;
     FFirstTimeMember := True;
     dm.tblMisc.Open;
     TempInvoiceMsg := dm.tblMisc.FieldByName('MSG').AsString;
     dm.tblMisc.Close;
     MemoInvoice.Lines.Clear;
     if TempInvoiceMsg = '' then begin
        MemoInvoice.Lines.Add(msg1);
     // allow up to 3 lines 
     end else begin   
        for i := 1 to 3 do begin            
           s := copy(TempInvoiceMsg,1,70);
           MemoInvoice.Lines.Add(s);
           system.Delete(TempInvoiceMsg,1,70);
           if TempInvoiceMsg = '' then break; 
        end;   
     end;   
end;
{ **************************************************************************** }
procedure TfrmInvoiceLandscape.bandGroupFooterAfterPrint(Sender: TQRCustomBand;   BandPrinted: Boolean);
begin
     if BAndPrinted then begin
        FPageNumber := 1;
        //FDiscountable := 0;
        FAmountPAid := 0.00;
        FAmountDue  := 0.00;
        //FDiscountMember   := 0.00;
     end;  
end;
{ **************************************************************************** }
procedure TfrmInvoiceLandscape.bandDetailAfterPrint(Sender: TQRCustomBand; BandPrinted: Boolean);
begin
     with dm.qryReport do begin
        // if extract by Member then use the discount supplied
        // Discount is still by member but there can be more than one member on a page
       { if not FExtractByMember then begin
            if FieldByName('DISCOUNTABLE').AsString = varYes then begin
               if FMember = FieldByName('MEMBER').AsInteger then begin
                  inc(FDiscountable);
               end else begin // <> Member
                  // watch does bandGroupFooterBeforePrint before a detailBandAfterPrint
                  if FDiscountable = 2 then     
                     FDiscountMember := FDiscountMember + Global.Discount2
                  else if FDiscountable >= 3 then 
                     FDiscountMember := FDiscountMember + Global.Discount3;
                  FMember := FieldByName('MEMBER').AsInteger;
                  FDiscountable := 1;
               end;      
            end else begin  // 'N'  not Discountable
               if FMember <> FieldByName('MEMBER').AsInteger then begin
                  if FDiscountable = 2 then     
                     FDiscountMember := FDiscountMember + Global.Discount2
                  else if FDiscountable >= 3 then 
                     FDiscountMember := FDiscountMember + Global.Discount3;
                  FDiscountable := 0;
                  FMember := FieldByName('MEMBER').AsInteger;
               end;   
            end;    
        end;        }
        FAmountPAid  := FAmountPAid + FieldByName('AMOUNT_PAID').AsFloat;  
        // if extract by Member then use Amount - AmountPaid and not Fee - AmountPaid 
        if FExtractByMember then
           FAmountDue   := FAmountDue  + FieldByName('PAY').AsFloat
          // FAmountDue   := FAmountDue  + FieldByName('AMOUNT_DUE_AMOUNT').AsFloat
        else     
           FAmountDue   := FAmountDue  + FieldByName('AMOUNT_DUE').AsFloat;
     end;   
end;
{ **************************************************************************** }
procedure TfrmInvoiceLandscape.bandGroupFooterBeforePrint( Sender: TQRCustomBand; var PrintBand: Boolean);
//var Discount,AmountDue : Extended;
begin
    // if FExtractByMember then begin
    //    Discount := FDiscount;
    { end else begin   
        // watch does this before a detailBandAfterPrint ie the last member before this
        if FDiscountable = 2 then     
           FDiscountMember := FDiscountMember + Global.Discount2
        else if FDiscountable >= 3 then 
           FDiscountMember := FDiscountMember + Global.Discount3;
        FDiscountable := 0;
        Discount := FDiscountMember;
      end; }    
     //lblDiscount.CAption := FloatToStrF( Discount,ffcurrency,7,2);
     //lblDiscount1.CAption := lblDiscount.CAption;
     
     lblAmountPaid.CAption := FloatToStrF( FAmountPAid,ffcurrency,7,2);
     lblAmountPaid1.CAption := lblAmountPaid.Caption;

     lblAmountDue.CAption := FloatToStrF( FAmountDue,ffcurrency,7,2);
     lblAmountDue1.CAption := lblAmountDue.Caption;
     
     //AmountDue := FAmountDue - Discount;                  
     //AmountDue := FAmountDue - FAmountPAid;
     //AmountDue := FAmountDue - Discount - FAmountPAid;
     lblTotalAmountDue.CAption := FloatToStrF( FAmountDue,ffcurrency,7,2);
     lblTotalAmountDue1.CAption := lblTotalAmountDue.CAption;
end;
{ **************************************************************************** }
procedure TfrmInvoiceLandscape.FormDestroy(Sender: TObject);
begin
     dm.tblSite.Close;
end;
{ **************************************************************************** }
procedure TfrmInvoiceLandscape.bandDetailBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
begin
     if not FFirstTimeMember then exit;
     if FFirstTimeMember then FMember := dm.qryReport.FieldByName('MEMBER').AsInteger;
     FFirstTimeMember := False;
end;
{ **************************************************************************** }
procedure TfrmInvoiceLandscape.FormCreate(Sender: TObject);
begin
     dm.tblSite.Open;
end;
{ ****************************************************************************}
procedure TfrmInvoiceLandscape.QRBand1BeforePrint(Sender: TQRCustomBand;
  var PrintBand: Boolean);
begin
 //Pauanui Sports & Recreation Club
     if pos('Pauanui',Global.CompName) > 0 then begin
        lblCompName.Enabled := False;
        lblAddress1.Enabled := False;
        lblAddress2.Enabled := False;
        lblAddress3.Enabled := False;
     end; 
end;

end.
