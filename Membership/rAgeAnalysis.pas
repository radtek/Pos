unit rAgeAnalysis;

interface

uses Windows, SysUtils, Messages, Classes, Graphics, Controls,
  StdCtrls, ExtCtrls, Forms, Quickrpt, QRCtrls,IBTable;

type
  TfrmAgeAnalysis = class(TForm)
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
    QRLabel4: TQRLabel;
    QRLabel5: TQRLabel;
    QRLabel6: TQRLabel;
    QRLabel7: TQRLabel;
    QRLabel8: TQRLabel;
    QRLabel9: TQRLabel;
    QRLabel11: TQRLabel;
    QRBand3: TQRBand;
    QRDBText1: TQRDBText;
    QRDBText2: TQRDBText;
    QRDBText3: TQRDBText;
    QRDBText4: TQRDBText;
    QRDBText5: TQRDBText;
    QRDBText6: TQRDBText;
    QRDBText7: TQRDBText;
    QRBand4: TQRBand;
    QRExpr1: TQRExpr;
    QRExpr2: TQRExpr;
    QRExpr3: TQRExpr;
    QRExpr4: TQRExpr;
    QRExpr5: TQRExpr;
    QRExpr6: TQRExpr;
    QRBand5: TQRBand;
    lblLogo: TQRLabel;
    QRLabel13: TQRLabel;
    QRLabel14: TQRLabel;
    QRLabel15: TQRLabel;
    QRDBText8: TQRDBText;
    QRDBText9: TQRDBText;
    QRExpr7: TQRExpr;
    QRExpr8: TQRExpr;
    lblErrors: TQRLabel;
    QRLabel16: TQRLabel;
    function Extract ( qrPrint : Boolean ; PrtIndex : SmallInt ;tblName : string ) : Boolean;
    procedure qrBeforePrint(Sender: TCustomQuickRep;
      var PrintReport: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmAgeAnalysis: TfrmAgeAnalysis;

implementation

{$R *.DFM}

Uses
    dmMembers, //StockMaster
    Global; //StockMaster
var
    Printing : Boolean;

{ **************************************************************************** }
procedure TfrmAgeAnalysis.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
end;
{ **************************************************************************** }
function TfrmAgeAnalysis.Extract ( qrPrint : Boolean ;PrtIndex : SmallInt; tblName : string ) : Boolean;
var Totctr,MNUM,FNUM,NoNUM : Integer;
    tblTemp : TIBTable;
    i,j,k : smallint;
    s : string[12];
    Value : extended;
const  SQL1 = 'SELECT COUNT(*) FROM MEMBERS where DATE_OF_BIRTH is not null ' +
              'and DATE_OF_BIRTH <> ''12/30/1899'' and DATE_OF_BIRTH <= current_date ' +    
             // 'and DATE_OF_BIRTH <> ''12/30/1899'' and MALE_FEMALE is not Null ' +    
              'and DATE_OF_BIRTH > (current_date - (365 * 110))'; // make sure not > 110  
       SQL4 = 'SELECT COUNT(*) FROM MEMBERS where DATE_OF_BIRTH is null ' +   
              'or DATE_OF_BIRTH = ''12/30/1899''' ;
       SQL5 = 'SELECT COUNT(*) FROM MEMBERS where DATE_OF_BIRTH > Current_date';   
       SQL6 = 'SELECT COUNT(*) FROM MEMBERS where DATE_OF_BIRTH  < (current_date - (365 * 110))';   
                                       
       SQL2 = 'CREATE TABLE %s ( DESCRIPTION CHAR(10),MNUM Float,MPER Float,FNUM Float,FPER Float,NoNUM Float,NoPER Float,TOTNUM Float,TOTPER Float)';
       //SQL3 = 'SELECT COUNT(MALE_FEMALE) FROM MEMBERS WHERE MALE_FEMALE = ''%s'' ' +
       //      'and DATE_OF_BIRTH is not null AND Extract(Year from DATE_OF_BIRTH) BETWEEN %d AND %d';
       SQL3 = 'SELECT count(*) FROM MEMBERS WHERE MALE_FEMALE %s ' +
              'and DATE_OF_BIRTH is not null and DATE_OF_BIRTH <> ''12/30/1899'' ' +
              'and (extract(Year from Current_date) - Extract(Year from DATE_OF_BIRTH)) ' +
              'BETWEEN %d AND %d';       
begin
     result := False;
     TotCtr := 0;FNUM := 0;MNUM := 0; NoNUM := 0;
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL1) then
        TotCtr := dm.qryGeneral.Fields[0].AsInteger;
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL4) then
        lblErrors.Caption := dm.qryGeneral.Fields[0].AsString + ' Members with no DOB';
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL5) then
        lblErrors.Caption := lblErrors.Caption + ' - ' + dm.qryGeneral.Fields[0].AsString + ' Members with DOB greater than today';
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL6) then
        lblErrors.Caption := lblErrors.Caption + ' - ' + dm.qryGeneral.Fields[0].AsString + ' Members with DOB greater than 110 yrs';
     try                          
        dm.qryOpen(dm.qryReport ,format(SQL2,[tblName]));
        dm.qryReport.Close;
        dm.IBCommitRetain;
        tblTemp := TIBTable.create(nil); 
        tblTemp.Database := dm.IBDatabase;
        tblTemp.Transaction := dm.IBTransaction;
        tblTemp.Tablename := tblName;
        tblTemp.Open;
        j := 0; k := 10;
        for i := 0 to 10 do begin
           with dm.qryGeneral do begin
              // male
              dm.sqlExecQuery(dm.qryGeneral,format(SQL3,[' = ''M''',j,k]));
              MNUM := Fields[0].AsInteger;
              // female
              dm.sqlExecQuery(dm.qryGeneral,format(SQL3,['= ''F''',j,k]));
              FNUM := Fields[0].AsInteger;
              // No Gender
              dm.sqlExecQuery(dm.qryGeneral,format(SQL3,['is null',j,k]));
              NoNUM := Fields[0].AsInteger;
              Close;
              tblTemp.Insert;
              tblTemp.FieldByName('DESCRIPTION').AsString := format ('%d to %d',[j,k]);
              tblTemp.FieldByName('FNUM').AsInteger := FNUM;
              tblTemp.FieldByName('MNUM').AsInteger := MNUM;
              tblTemp.FieldByName('NoNUM').AsInteger := NoNUM;
              tblTemp.FieldByName('TOTNUM').AsInteger := MNUM + FNUM + NoNUM;
              value := 0.00;
              if TotCtr <> 0 then
                 value := (MNUM / TotCtr) * 100;
              if value <> 0.00 then
                 //tblTemp.FieldByName('MPER').AsFloat := Round((Value * 1000) + 0.005 ) / 1000.0;
                 tblTemp.FieldByName('MPER').AsFloat := Value;
                 //tblTemp.FieldByName('MPER').AsFloat := Round((Value * 100) + 1 ) / 100.0;
              if TotCtr <> 0 then
                 value := (FNUM / TotCtr) * 100;
              if value <> 0.00 then
                 //tblTemp.FieldByName('FPER').AsFloat := Round((Value * 1000) + 0.005 ) / 1000.0;
                 tblTemp.FieldByName('FPER').AsFloat := Value;
              if TotCtr <> 0 then
                 value := (NoNUM / TotCtr) * 100;
              if value <> 0.00 then
                 tblTemp.FieldByName('NoPER').AsFloat := Value;
                 //tblTemp.FieldByName('NoPER').AsFloat := Round((Value * 1000) + 0.005 ) / 1000.0;
                 //tblTemp.FieldByName('FPER').AsFloat := Round((Value * 100) + 1 ) / 100.0;
              value := ((FNUM + MNUM + NoNUM) / TotCtr) * 100;   
              tblTemp.FieldByName('TOTPER').AsFloat := tblTemp.FieldByName('MPER').AsFloat +
                                                       tblTemp.FieldByName('NoPER').AsFloat +
                                                       tblTemp.FieldByName('FPER').AsFloat;
              //tblTemp.FieldByName('TOTPER').AsFloat :=  Round((Value * 1000) ) / 1000.0;
              tblTemp.Post;
              if i = 0 then inc(j); // start j off at 0 - 1st time thru only
              j := j + 10; k := k + 10;
              //if j = 111 then k := 500; // catch any funny dates
           end;
        end;
        dm.qryOpen(dm.qryReport ,format('SELECT * FROM %s',[tblName]));
        qr.PrinterSettings.PrinterIndex := PrtIndex;
        Printing := qrPrint;
        result := True;
     finally
        tblTemp.Close;
        tblTemp.Free;
     end;
end;
{ **************************************************************************** }
end.
