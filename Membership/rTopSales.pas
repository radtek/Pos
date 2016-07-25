unit rTopSales;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls,IBTable;

type
  TfrmTopSales = class(TForm)
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
    QRLabel6: TQRLabel;
    DetailBand: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText3: TQRDBText;
    QRBand4: TQRBand;
    lblLogo: TQRLabel;
    QRLabel3: TQRLabel;
    lblCount: TQRLabel;
    lblSales: TQRLabel;
    lblVIP: TQRLabel;
    QRLabel4: TQRLabel;
    QRLabel5: TQRLabel;
    lblPointsEarned: TQRLabel;
    lblRedeemed: TQRLabel;
    QRBand3: TQRBand;
    lblSalesTotal: TQRLabel;
    lblPointsEarnedTotal: TQRLabel;
    lblRedeemedTotal: TQRLabel;
    QRLabel7: TQRLabel;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    function Extract(  PrtIndex,Count : Integer ; DateFrom, DateTo : TDateTime ;tblName : String ) : Boolean;
    procedure DetailBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    procedure QRBand3BeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
  private
    { Private declarations }
    //fCount : Integer;
  public
    { Public declarations }
  end;

var
  frmTopSales: TfrmTopSales;
  ctr : smallint;
  Sales,Earned,Redeemed : Currency;

implementation

{$R *.DFM}

uses
dmMembers,dmMenuMate,Global;
{ ****************************************************************************}
procedure TfrmTopSales.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
     ctr := 1;
     lblVIP.Caption := '';
     if VIPCreditName <> '' then
        lblVIP.Caption := '( VIP Credit Sales are excluded )'; 
     Sales := 0.00;
     Earned := 0.00;
     Redeemed := 0.00;
end;                                                    
{ ****************************************************************************}
function TfrmTopSales.Extract(  PrtIndex,Count : Integer ; DateFrom, DateTo : TDateTime ;tblName : String ) : Boolean;

const SQL1 = 
      'Select ' +
         //'Contacts.Name, ' +
         'Contacts.Contacts_3RDParty_Key Member,' +                                                 
   		'cast(Sum(Archive.Price + Archive.Price_Adjust + Archive.Discount) as Numeric(15,2)) Sales, ' +
   		'Sum(Archive.Redeemed) Redeemed,' +
   		'Sum(Archive.Points_Earned) Points_Earned ' +
		'From ' +
			'Archive Inner Join Contacts on Archive.Loyalty_Key = Contacts.Contacts_Key ' +
		'Where ' + 
			'Archive.Order_Type in (0,3) and ' +
			'Archive.Time_Stamp_Billed between :DateFrom and :DateTo and ' +
			'Archive.Item_Name <> :VIPCreditName ' +
      'Group By ' +
         'Contacts.Name, ' +
         'Contacts.Contacts_3RDParty_Key ' + 
      'Order by 2 desc';                                                    
SQL2 = 'CREATE TABLE %s ( FULL_NAME varchar(45),FIRST_NAME varchar(20),SURNAME varchar(25),KNOWN_AS varchar(25),' +
         'ADDRESS varchar(30),ADDRESS1 varchar(30),DISTRICT varchar(20),CITY varchar(25),COUNTRY varchar(25),' +
         'MEMBER Integer,SALES Numeric(15,2),REDEEMED Numeric(15,2),POINTS_EARNED Numeric(15,2))';
SQL3 = 'Select m.MEMBER,m.FULL_NAME,m.FIRST_NAME,m.SURNAME,m.KNOWN_AS,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.COUNTRY ' +
       'from Members M, %s t where m.MEMBER = t.Member';
var i : smallint;
    tblTemp : TIBTable;

begin
     // changes for Uppe Hutt so they can Mail Merge them
     result    := False;
     if not dmMM.MMTransaction.InTransaction then
        dmMM.MMTransaction.StartTransaction;
     try
        with dm.qryReport do begin
           // create table
           dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[tblName]));
           dm.IBCommitRetain;
           // Get Sales from MenuMate
           dmMM.qryMMReport.close;
           dmMM.qryMMReport.SQL.Text := SQL1;
           dmMM.qryMMReport.ParamByName('DateFrom').AsDateTime := DateFrom;
           dmMM.qryMMReport.ParamByName('DateTo').AsDateTime   := DateTo;
           // Exclude VIP Credit Name
           dmMM.qryMMReport.ParamByName('VIPCreditName').AsString := VIPCreditName;
           dmMM.qryMMReport.open;

           tblTemp := TIBTable.create(nil); 
           tblTemp.Database    := dm.IBDatabase;
           tblTemp.Transaction := dm.IBTransaction;
           tblTemp.Tablename   := tblName;
           tblTemp.Open;
           i := 1;
           while not dmMM.qryMMReport.EOF do begin
              if i > Count then break;
              tblTemp.Append;
              tblTemp.FieldByName('MEMBER').AsInteger :=  dmMM.qryMMReport.FieldByName('Member').AsInteger;
              tblTemp.FieldByName('SALES').AsFloat    :=  dmMM.qryMMReport.FieldByName('SALES').AsFloat;
              tblTemp.FieldByName('REDEEMED').AsFloat :=  dmMM.qryMMReport.FieldByName('REDEEMED').AsFloat;
              tblTemp.FieldByName('POINTS_EARNED').AsFloat :=  dmMM.qryMMReport.FieldByName('POINTS_EARNED').AsFloat;
              tblTemp.Post;
              inc(i);
              dmMM.qryMMReport.Next;
           end;
           dm.sqlExecQuery(dm.qryGeneral,format(SQL3,[tblName]));
           tblTemp.Close;
           tblTemp.Open;
           //dm.qryOpen(dm.qryReport ,format(SQL3,[tblName]));
           while not dm.qryGeneral.EOF do begin
              if tblTemp.Locate('MEMBER',dm.qryGeneral.FieldByName('MEMBER').AsVariant,[]) then begin 
                 tblTemp.Edit;
                 tblTemp.FieldByName('FULL_NAME').AsString  :=  dm.qryGeneral.FieldByName('FULL_NAME').AsString;
                 tblTemp.FieldByName('FIRST_NAME').AsString :=  dm.qryGeneral.FieldByName('FIRST_NAME').AsString;
                 tblTemp.FieldByName('SURNAME').AsString    :=  dm.qryGeneral.FieldByName('SURNAME').AsString;
                 tblTemp.FieldByName('KNOWN_AS').AsString   :=  dm.qryGeneral.FieldByName('KNOWN_AS').AsString;
                 tblTemp.FieldByName('ADDRESS').AsString   :=  dm.qryGeneral.FieldByName('ADDRESS').AsString;
                 tblTemp.FieldByName('ADDRESS1').AsString   :=  dm.qryGeneral.FieldByName('ADDRESS1').AsString;
                 tblTemp.FieldByName('DISTRICT').AsString   :=  dm.qryGeneral.FieldByName('DISTRICT').AsString;
                 tblTemp.FieldByName('CITY').AsString       :=  dm.qryGeneral.FieldByName('CITY').AsString;
                 tblTemp.FieldByName('COUNTRY').AsString    :=  dm.qryGeneral.FieldByName('COUNTRY').AsString;
                 tblTemp.Post;
              end;
              dm.qryGeneral.Next;
           end;  
           tblTemp.Close;
           Close;
           dm.IBTransaction.Commit;
           //if dmMM.MMTransaction.InTransaction then
           //  dmMM.MMTransaction.Commit;
          // Database    := dm.IBDataBase;
           //Transaction := dm.IBTransaction;
           SQL.Text := 'Select * from ' + tblName;
           Open;                       
        end;
        qr.PrinterSettings.PrinterIndex := PrtIndex;
        Result := True;
     finally
        tblTemp.Close;
        tblTemp.Free;
     end;   
end;
{ ****************************************************************************}
procedure TfrmTopSales.DetailBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
begin
     lblSales.Caption        := formatFloat('$0.00',dm.qryReport.FieldByName('SALES').AsFloat);
     lblPointsEarned.Caption := formatFloat('$0.00',dm.qryReport.FieldByName('POINTS_EARNED').AsFloat);
     lblRedeemed.Caption     := formatFloat('$0.00',dm.qryReport.FieldByName('REDEEMED').AsFloat);
     Sales    := Sales    + dm.qryReport.FieldByName('SALES').AsFloat;
     Earned   := Earned   + dm.qryReport.FieldByName('POINTS_EARNED').AsFloat;
     Redeemed := Redeemed + dm.qryReport.FieldByName('REDEEMED').AsFloat;
     lblCount.Caption := IntToStr(ctr);
     //if ctr > fCount then
     //   PrintBand := false;
     inc(ctr);
end;
{ ****************************************************************************}
procedure TfrmTopSales.QRBand3BeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
begin
     lblSalesTotal.Caption        := formatFloat('$0.00',Sales);
     lblPointsEarnedTotal.Caption := formatFloat('$0.00',Earned);
     lblRedeemedTotal.Caption     := formatFloat('$0.00',Redeemed);
end;
{ ****************************************************************************}
end.


                                                      




